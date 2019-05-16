#include <set>
#include <stack>
#include <queue>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>
#define MAX_ROWS 20
#define MAX_COLS 20
using namespace std;

int raw_Line = 0;                        // Raw Production Line Counter
int prod_Line = 0;                       // Processed Production Line Counter
int input_Line = 0;                      // Input Text Counter
char inputText[MAX_ROWS][MAX_COLS];      // Input Text
char production[MAX_ROWS][MAX_COLS];     // Processed Productions
char raw_production[MAX_ROWS][MAX_COLS]; // Raw Productions
char FIRST_Result[MAX_ROWS][MAX_COLS];   // FIRST Set Result
char FOLLOW_Result[MAX_ROWS][MAX_COLS];  // FOLLOW Set Result
char parsing_table[MAX_ROWS][MAX_COLS];  // LL(1) Grammar Parsing Table

string errorLog;              // Error Log Info
vector<string> parsingResult; // Final Parsing Result

set<char>::iterator iter, iter_curr, iter_index;
set<char> set_current;  // Temp FIRST Set for Each Production
set<char> set_first;    // Temp FIRST Set for Unique Production LHS
set<char> set_follow;   // Temp FOLLOW Set for Unique Production LHS
set<char> set_calc;     // Temp Set for Calculated Produciton LHS
set<char> set_index;    // LL(1) Parsing Table Index Line
stack<char> sta_parser; // LL(1) Parser Stack

void pt_first(int row, int num_prod);

/*                 Assumptions
 * 1. Epsilon Εε is represented by ‘#’
 * 2. Grammar is not left recursive
 * 3. Only upper case letters are Non-Terminals(NT), thus everything else is a Terminal(T)
 */

void calculate_first(char prod_LHS, int pre_Prod, int pre_Column, int root_Row, bool isRoot)
{
    int i = 0, end_condition = prod_Line; // Default 'for' Loop Start & End Condition
    if (isRoot)
    {
        i = root_Row;
        end_condition = root_Row + 1;
    }
    // Find The Match for Current Production's LHS
    for (; i < end_condition; i++)
    {
        if (production[i][0] == prod_LHS)
        {
            // Case --> Production LHS is Terminal (e.g. 4 -> +aB)
            // * Result I --> FIRST Set += Starting Symbol
            if (!(isupper(prod_LHS)))
            {
                set_current.insert(prod_LHS);
                set_first.insert(prod_LHS);
                return;
            }

            // Case --> Production LHS is Non-Terminal
            for (int j = 5; j < strlen(production[i]); j++) // Start With 5th Char
            {
                char previous = production[i][j - 1];
                if (production[i][j] == '|' || production[i][j] == ' ')
                    continue;

                // Case I --> Production Contains ε (#) (e.g. X -> aB | ε)
                // * Result II --> FIRST Set += ε (#)
                else if (production[i][j] == '#')
                {
                    set_current.insert('#');
                    set_first.insert('#');
                    // Case I/1 --> Current Iteration's Production Contains ε & Has Follow Up Non-Terminal
                    // * Result III --> Continue to Iterate, Start from The Next Character from Previous Iteration
                    if (production[pre_Prod][pre_Column] != '\0' && (pre_Prod != 0 || pre_Column != 0))
                        calculate_first(production[pre_Prod][pre_Column], pre_Prod, (pre_Column + 1), -1, false);
                }

                // Case II --> Production Starts With Terminal (e.g. E -> +TE')
                // * Result IV --> FIRST Set += Non-Terminal Character
                else if (!isupper(production[i][j]))
                {
                    if ((previous == ' ') || (previous == '|'))
                    {
                        set_current.insert(production[i][j]);
                        set_first.insert(production[i][j]);
                    }
                }

                // Case III --> Production Starts With Non-Terminal (e.g. X -> Y)
                // * Result V --> FIRST Set += FIRST(Y1Y2..Yk), Stop by The First Set Which doesn't Contain ε
                else if (previous == ' ' || previous == '|')
                    calculate_first(production[i][j], i, (j + 1), -1, false);
            }
        }
    }
}

void fun_first_set()
{
    cout << "FIRST Set of Given Grammar Rules\n"
         << endl;
    int row = 0;
    for (int i = 0; i < prod_Line; i++)
    {
        // Call Function to Calculate FIRST Set of Current Production
        char prod_LHS = production[i][0];
        bool isRoot = true; // Current Calculation is The Root
        calculate_first(prod_LHS, 0, 0, i, isRoot);
        pt_first(row, i);    // Generate The Parsing Table Based On Every Production
        set_current.clear(); // Clear Current Production Temp Set

        // Display The Final FIRST Set Result of Current Production
        int column = 0;
        if (production[i + 1][0] != prod_LHS)
        {
            cout << "FIRST(" << prod_LHS << ") = { ";
            FIRST_Result[row][0] = prod_LHS; // First Character is LHS
            for (iter = set_first.begin(); iter != set_first.end(); iter++)
            {
                cout << *iter << ", ";
                FIRST_Result[row][++column] = *iter;
            }
            set_first.clear(); // Clear The Whole FIRST Set of Current Production
            cout << "}\n";
            row++;
        }
    }
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}

/*----------------------------------------------------------------------------------------------------*/

void calculate_follow(char prod_LHS)
{
    // Case --> Current Production LHS is Start Symbol
    // * Result I --> FOLLOW Set += $
    if (prod_LHS == production[0][0])
        set_follow.insert('$');

    // Find The Match for Passed In LHS On Current Production's RHS
    for (int i = 0; i < prod_Line; i++)
    {
        for (int j = 5; j < sizeof(production[i]); j++) // Start With 5th Char
        {
            if (production[i][j] == prod_LHS) // When Target Char Appears on Production RHS
            {
                set_calc.insert(production[i][j]);
                char next = production[i][j + 1];
                // Case I --> Production Type: A -> aBc
                if (next != '\0' && next != ' ' && next != '\r')
                {
                    if (isupper(next))
                    {
                        calculate_first(next, i, (j + 2), -1, false);
                        // Case I/1 --> FIRST(c) Contains ε
                        // * Result II --> FOLLOW(B) += FOLLOW(A)
                        if (set_first.count('#'))
                            // If Current Production's LHS Has Been Calculated --> Skip
                            // * e.g. Given Production: B -> aBc, Calculate FOLLOW(B)
                            if (!set_calc.count(production[i][0]))
                                calculate_follow(production[i][0]);

                        // * Result III --> FOLLOW(B) += (FIRST(c) - ε)
                        set_first.erase('#');
                        for (iter = set_first.begin(); iter != set_first.end(); iter++)
                            set_follow.insert(set_first.begin(), set_first.end());

                        set_first.clear();
                    }
                    // Case I/2 --> c is Non-Terminal
                    // * Result IV --> FOLLOW(B) += c
                    else
                        set_follow.insert(next);
                }

                // Case II --> Production Type: A -> aB
                // * Result V --> FOLLOW(B) += FOLLOW(A)
                else if (prod_LHS != production[i][0])
                    calculate_follow(production[i][0]);
            }
        }
    }
}

void fun_follow_set()
{
    cout << "FOLLOW Set of Given Grammar Rules\n"
         << endl;
    int row = 0;
    for (int i = 0; i < prod_Line; i++)
    {
        // Call Function to Calculate FOLLOW Set of Current Production
        char prod_LHS = production[i][0];
        calculate_follow(prod_LHS);

        // Display The Final FOLLOW Set Result of Current Production
        int column = 0;
        if (production[i + 1][0] != prod_LHS)
        {
            set_calc.clear(); // Clear Set After Current LHS Iteration
            cout << "FOLLOW(" << prod_LHS << ") = { ";
            FOLLOW_Result[row][0] = prod_LHS; // First Character is Production LHS
            for (iter = set_follow.begin(); iter != set_follow.end(); iter++)
            {
                cout << *iter << ", ";
                FOLLOW_Result[row][++column] = *iter;
            }
            set_follow.clear(); // Clear The Whole FOLLOW Set of Current Production
            cout << "}\n";
            row++;
        }
    }
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}

/*----------------------------------------------------------------------------------------------------*/

void production_preprocess()
{
    // Split Raw Productions, e.g. D -> b | c | # --> D -> b, D -> c, D -> #
    for (int i = 0; i < raw_Line; i++, prod_Line++)
    {
        int prod_Column = 0;
        for (int j = 0; j < sizeof(raw_production[i]); j++)
        {
            if (raw_production[i][j] != '\0' && raw_production[i][j] != '\r')
            {
                if (raw_production[i][j] == '|')
                {
                    prod_Line++; // Start A New Line
                    j += 2;      // Skip The '|' Mark & The Follow Up Whitespace
                    int k = 0;
                    for (; k < 5; k++) // Generate The Header of Current Production
                        production[prod_Line][k] = raw_production[i][k];
                    // Add The RHS Characters After Current '|' Mark Till The End or Next '|' Mark
                    while (raw_production[i][j + 1] != '\0')
                    {
                        if (raw_production[i][j + 1] == '|')
                            break;
                        production[prod_Line][k++] = raw_production[i][j++];
                    }
                }
                else if (raw_production[i][j + 1] != '|')
                    production[prod_Line][prod_Column++] = raw_production[i][j];
            }
        }
    }
}

void display_productions()
{
    cout << "Local CFG Grammar Productions As Shown Below\n"
         << endl;
    for (int i = 0; i < raw_Line; i++)
        cout << raw_production[i] << "\n";
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;

    cout << "Processed Program Embedded Productions As Shown Below\n"
         << endl;
    for (int i = 0; i < prod_Line; i++)
        cout << production[i] << "\n";
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}

/*----------------------------------------------------------------------------------------------------*/

void pt_index()
{
    // Parsing Table Index Row Consists of All Terminals In Given CFG Grammar + '$'
    for (int i = 0; i < prod_Line; i++)
    {
        for (int j = 5; j < sizeof(production[i]); j++)
        {
            char temp = production[i][j];
            if (!isupper(temp) && temp != ' ' && temp != '#' && temp != '\0')
                set_index.insert(temp);
        }
    }
    set_index.insert('$');
}

void pt_initialize()
{
    // Initialize The 2-Dimensional Array --> Line = Production Number, Column = Index + 1
    for (int i = 0; i < raw_Line; i++)
    {
        parsing_table[i][0] = raw_production[i][0]; // First Column = Production LHS
        for (int j = 1; j < (set_index.size() + 1); j++)
            parsing_table[i][j] = '/';
    }
}

void pt_first(int row, int num_prod)
{
    set_current.erase('#');
    for (iter_curr = set_current.begin(); iter_curr != set_current.end(); iter_curr++)
    {
        int pos_first = 1;
        // Calculate The Mapped Index of Current FIRST Set Element In Parsing Table
        for (iter_index = set_index.begin(); iter_index != set_index.end(); iter_index++)
        {
            if (*iter_curr == *iter_index)
                break;
            pos_first++;
        }
        parsing_table[row][pos_first] = ('0' + num_prod);
    }
}

void pt_follow()
{
    // If FIRST Set Contains ε --> Add ε to Current Production FOLLOW Set Elements Column
    // * e.g. "M -> +TM | #" FIRST Set Contains ε --> Add "M -> #" to All Its FOLLOW Set Element { $, ) }
    for (int i = 0; i < raw_Line; i++)
    {
        for (int j = 0; j < sizeof(FIRST_Result[i]); j++)
        {
            if (FIRST_Result[i][j] == '#')
            {
                // Get The Mapped Index of All FOLLOW Set Elements
                for (int k = 1; k < sizeof(FOLLOW_Result[i]); k++)
                {
                    int pos_follow = 1;
                    for (iter = set_index.begin(); iter != set_index.end(); iter++)
                    {
                        if (FOLLOW_Result[i][k] == *iter)
                            break;
                        pos_follow++;
                    }
                    parsing_table[i][pos_follow] = '#';
                }
            }
        }
    }
}

void displayParsingTable()
{
    cout << "LL(1) Grammar Parsing Table\n\n ";
    // Display The Index Row
    for (iter_index = set_index.begin(); iter_index != set_index.end(); iter_index++)
        cout << " " << *iter_index;
    cout << endl;
    // Display LL(1) Grammar Parsing Table Main Body
    for (int i = 0; i < raw_Line; i++)
    {
        for (int j = 0; j < (set_index.size() + 1); j++)
            cout << parsing_table[i][j] << " ";
        cout << "\n";
    }
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}

/*----------------------------------------------------------------------------------------------------*/

void printStack(const stack<char> &target)
{
    cout << "Current Stack - ";
    for (stack<char> dump = target; !dump.empty(); dump.pop())
        cout << dump.top() << " ";
    cout << "\nStack Size - " << sta_parser.size() << endl;
}

void printQueue(const queue<char> &target)
{
    cout << "Current Input Line Queue - ";
    for (queue<char> dump = target; !dump.empty(); dump.pop())
        cout << dump.front() << " ";
    cout << endl
         << endl;
}

void parsingLog(const string &str_log)
{
    cout << "Corresponding Production - " << str_log << endl;
    parsingResult.push_back(str_log);
}

void non_recursive_parse()
{
    for (int i = 0; i < input_Line; i++) // Iterate for Every Custom Input Line
    {
        // Store Current Processing Input Line Into Queue
        queue<char> inputLine;
        for (int j = 0; j < sizeof(inputText[i]); j++)
            if (inputText[i][j] != '\0' && inputText[i][j] != '\r')
                inputLine.push(inputText[i][j]);
        cout << "Current Processing User Input Line - ";
        printQueue(inputLine);

        // Push '$' & The Starting Non-Terminal Symbol Into Parser Stack
        sta_parser.push('$');
        sta_parser.push(production[0][0]);
        printStack(sta_parser);
        printQueue(inputLine);
        while (sta_parser.top() != '$')
        {
            // Get The Index for Current Input Queue Front & Stack Top
            int index_Stack = 0, index_Input = 0;
            for (int j = 0; j < raw_Line; j++, index_Stack++)
                if (parsing_table[j][0] == sta_parser.top())
                    break;
            for (iter_index = set_index.begin(); iter_index != set_index.end(); iter_index++, index_Input++)
                if (*iter_index == inputLine.front())
                    break;
            char pt_target = parsing_table[index_Stack][index_Input + 1]; // Target Location for Queue Front in The Parsing Table

            // Case I --> Stack Top Matches Input Queue Front
            // * Action I --> Pop Stack, Pop Input Line Queue
            if (sta_parser.top() == inputLine.front())
            {
                cout << "Current Matched Input Character - " << inputLine.front() << endl;
                sta_parser.pop();
                inputLine.pop();
            }
            // Case II --> Stack Top is Terminal But Doesn't Match Up to The Input Queue Front
            // * Action II --> Report Error & Discard Current Character
            else if (!isupper(sta_parser.top()))
            {
                errorLog = "Error: Expected Non-Terminal In Stack But Found Terminal - '" + string(1, sta_parser.top()) + "'";
                parsingResult.push_back(errorLog);
                cerr << errorLog << endl;
                sta_parser.pop();
            }
            // Case III --> Stack Top is Non-Terminal
            // * Action III --> Pop Stack Top Element & Add Corresponding Production RHS to The Stack In Reverse Order
            else if (pt_target != '/') // Find A Match In Parsing Table
            {
                // Generate The Full Correponding Production
                char *corr_production = new char[sizeof(production[pt_target - '0']) + 1];
                if (pt_target != '#') // FIRST Set
                {
                    corr_production = production[pt_target - '0'];
                    parsingLog(corr_production);
                }
                else // FOLLOW Set
                {
                    string temp(1, parsing_table[index_Stack][0]);
                    temp += " -> #";
                    strcpy(corr_production, temp.c_str());
                    parsingLog(corr_production);
                }

                sta_parser.pop();
                // Add Corresponding Production RHS to The Stack In Reverse Order
                for (int j = (sizeof(corr_production) - 1); j >= 5; j--)
                    if (corr_production[j] != '|' && corr_production[j] != '#' && corr_production[j] != '\0' && corr_production[j] != '\r')
                        sta_parser.push(corr_production[j]);
                printStack(sta_parser);
                printQueue(inputLine);
            }
            else // Can't Find A Match In Parsing Table --> Discard Current Input Character
            {
                errorLog = "Error: Discard Current Input Character - '" + string(1, inputLine.front()) + "' Due to: Can't Find A Match In Parsing Table.";
                parsingResult.push_back(errorLog);
                cerr << errorLog << endl;
                inputLine.pop();
            }
        }
        if (inputLine.front() == '$') // Both Stack & Input Queue Have Reached Their Ends
        {
            cout << "LL(1) Grammar Non-Recursive Predictive Parsing Procedure Finished" << endl;
            cout << "\n/*------------------------------------------------------------*/\n"
                 << endl;
        }
        else
        {
            errorLog = "Error: Custom Input Line Hasn't Reached Its End !";
            parsingResult.push_back(errorLog);
            cerr << errorLog << endl;
        }
    }
}

/*----------------------------------------------------------------------------------------------------*/

void load2array(const string &textPath, bool isProduction = true)
{
    /*Load Local Test Grammar Into Production Rules*/
    ifstream(tsFile);
    tsFile.open(textPath);
    if (tsFile.is_open())
    {
        while (!tsFile.eof() && raw_Line < MAX_ROWS && input_Line < MAX_ROWS)
        {
            string line;
            getline(tsFile, line);
            if (isProduction)
                strcpy(raw_production[raw_Line++], line.c_str());
            else
                strcpy(inputText[input_Line++], line.c_str());
        }
    }
    else // File Not Exists or Failed to Load File
    {
        cerr << "Unable to Open Target File...\n";
        exit(-1);
    }
    tsFile.close();
    cout << "Local Text File Closed Successfully...\n";
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}

void output2file(const string &OutputPath, char resultSet[][MAX_COLS]) // FIRST & FOLLOW Set Result
{
    ofstream(tsFile);
    tsFile.open(OutputPath);
    if (tsFile.is_open())
    {
        for (int i = 0; i < (sizeof(char(*)[20]) / sizeof(resultSet[0])); i++)
        {
            tsFile << resultSet[i][0] << " -> { ";
            for (int j = 1; j < sizeof(resultSet[i]); j++)
            {
                if (resultSet[i][j] == '#')
                    tsFile << "\u03B5, "; // Unicode Escape Characters of ε
                else if (resultSet[i][j] != '\0')
                {
                    if (resultSet[i][j + 1] == '\0')
                        tsFile << resultSet[i][j] << " }";
                    else
                        tsFile << resultSet[i][j] << ", ";
                }
            }
            tsFile << endl;
        }
    }
    else // File Not Exists or Failed to Write File
    {
        cerr << "Unable to Write to The File...\n";
        exit(-1);
    }
    tsFile.close();
    cout << "Set Result File Closed Successfully...\n";
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}

void output2file(const string &OutputPath) // Parsing Result
{
    ofstream(tsFile);
    tsFile.open(OutputPath);
    if (tsFile.is_open())
    {
        for (const string &str : parsingResult)
            tsFile << str << "\n";
    }
    else // File Not Exists or Failed to Write File
    {
        cerr << "Unable to Write to The File...\n";
        exit(-1);
    }
    tsFile.close();
    cout << "Top-Down Non-Recursive Predictive Parsing Result File Closed Successfully...\n";
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}

int main(int argc, char **argv)
{
    /*Get Current Working Directory*/
    char cwd[256];
    getcwd(cwd, sizeof(cwd)); // Store In String "cwd"
    string result_dir = cwd, tsPath = cwd, firPath = cwd, flwPath = cwd, inputPath = cwd, outputPath = cwd;
    result_dir += "/IO_Text";
    cout << "Current Working Directory: " << cwd << "\n";
    cout << "Results Output Directory: " << result_dir << endl
         << endl;

    /*Generate Project Local Files Directory*/
    string test_set = "/IO_Text/Test_Set.txt";          // Test Set File Path
    string firSuffix = "/IO_Text/FIRST_Set.txt";        // FIRST Set File Path
    string flwSuffix = "/IO_Text/FOLLOW_Set.txt";       // FOLLOW Set File Path
    string inputSuffix = "/IO_Text/Input_Text.txt";     // Input Text File Path
    string outputSuffix = "/IO_Text/Output_Result.txt"; // Output Text File Path
    tsPath += test_set;
    firPath += firSuffix;
    flwPath += flwSuffix;
    inputPath += inputSuffix;
    outputPath += outputSuffix;

    /*Load Local CFG Grammar Into Production Arrays & Preprocess Given Productions*/
    load2array(tsPath, true); // CFG Grammar
    raw_Line--;
    production_preprocess(); // Split Given Productions

    /*Display Loaded CFG Grammar Productions*/
    display_productions();

    load2array(inputPath, false); // Custom Input Text
    input_Line--;

    /*Generate LL(1) Parsing Table*/
    pt_index();
    pt_initialize();

    /*Calculate The FIRST & FOLLOW Sets of Given Grammar Rules*/
    /*Part I --> Calculate FIRST Set & Generate Parsing Table Based On Each Production*/
    fun_first_set();
    output2file(firPath, FIRST_Result);

    /*Part II --> Calculate FOLLOW Set Based On Each Production*/
    fun_follow_set();
    output2file(flwPath, FOLLOW_Result);

    pt_follow(); // Generate Parsing Table Based On FOLLOW Set Result
    displayParsingTable();

    /*Parse User Input Text*/
    non_recursive_parse();
    output2file(outputPath);

    return 0;
}
