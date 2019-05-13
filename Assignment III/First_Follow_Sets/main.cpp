#include <set>
#include <cstdio>
#include <cctype>
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
char production[MAX_ROWS][MAX_COLS];     // Processed Productions
char raw_production[MAX_ROWS][MAX_COLS]; // Raw Productions
char FIRST_Result[MAX_ROWS][MAX_COLS];   // FIRST Set Result
char FOLLOW_Result[MAX_ROWS][MAX_COLS];  // FOLLOW Set Result

set<char>::iterator iter;
set<char> set_first;  // Temp FIRST Set for Current Production
set<char> set_follow; // Temp FOLLOW Set for Current Production
set<char> set_calc;   // Temp Set for Calculated Produciton LHS

/*                 Assumptions
 * 1. Epsilon Εε is represented by ‘#’
 * 2. Grammar is not left recursive
 * 3. Each production of a non terminal is entered on a different line
 * 4. Only Upper Case letters are Non-Terminals, thus everything else is a terminal
 */

void calculate_first(char prod_LHS, int pre_Prod, int pre_Column)
{
    // Find The Match for Current Production's LHS
    for (int i = 0; i < prod_Line; i++)
    {
        if (production[i][0] == prod_LHS)
        {
            // Case --> Production LHS is Terminal (e.g. 4 -> +aB)
            // * Result I --> FIRST Set += Starting Symbol
            if (!(isupper(prod_LHS)))
            {
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
                    set_first.insert('#');
                    // Case I/1 --> Current Iteration's Production Contains ε & Has Follow Up Non-Terminal
                    // * Result III --> Continue to Iterate, Start from The Next Character from Previous Iteration
                    if (production[pre_Prod][pre_Column] != '\0' && (pre_Prod != 0 || pre_Column != 0))
                        calculate_first(production[pre_Prod][pre_Column], pre_Prod, (pre_Column + 1));
                }

                    // Case II --> Production Starts With Terminal (e.g. E -> +TE')
                    // * Result IV --> FIRST Set += Non-Terminal Character
                else if (!isupper(production[i][j]))
                {
                    if ((previous == ' ') || (previous == '|'))
                        set_first.insert(production[i][j]);
                }

                    // Case III --> Production Starts With Non-Terminal (e.g. X -> Y)
                    // * Result V --> FIRST Set += FIRST(Y1Y2..Yk), Stop by The First Set Which doesn't Contain ε
                else if (previous == ' ' || previous == '|')
                    calculate_first(production[i][j], i, (j + 1));
            }
        }
    }
}

void fun_first_set()
{
    cout << "FIRST Set of Given Grammar Rules\n"
         << endl;
    for (int i = 0; i < prod_Line; i++)
    {
        // Call Function to Calculate FIRST Set of Current Production
        char prod_LHS = production[i][0];
        calculate_first(prod_LHS, 0, 0);

        // Display The Final FIRST Set Result of Current Production
        if (production[i + 1][0] != prod_LHS)
        {
            int line = 0, column = 0;
            cout << "FIRST(" << prod_LHS << ") = { ";
            FIRST_Result[i][0] = prod_LHS; // First Character is LHS
            for (iter = set_first.begin(); iter != set_first.end(); iter++)
            {
                cout << *iter << ", ";
                FIRST_Result[i][++column] = *iter;
            }
            set_first.clear(); // Clear The Whole FIRST Set of Current Production
            cout << "}\n";
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
                        calculate_first(next, i, (j + 2));
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
    for (int i = 0; i < prod_Line; i++)
    {
        // Call Function to Calculate FOLLOW Set of Current Production
        char prod_LHS = production[i][0];
        calculate_follow(prod_LHS);

        // Display The Final FOLLOW Set Result of Current Production
        if (production[i + 1][0] != prod_LHS)
        {
            set_calc.clear(); // Clear Set After Current LHS Iteration
            int column = 0;
            cout << "FOLLOW(" << prod_LHS << ") = { ";
            FOLLOW_Result[i][0] = prod_LHS; // First Character is Production LHS
            for (iter = set_follow.begin(); iter != set_follow.end(); iter++)
            {
                cout << *iter << ", ";
                FOLLOW_Result[i][++column] = *iter;
            }
            set_follow.clear(); // Clear The Whole FOLLOW Set of Current Production
            cout << "}\n";
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

void load2array(string textPath)
{
    /*Load Local Test Grammar Into Production Rules*/
    ifstream(tsFile);
    tsFile.open(textPath);
    if (tsFile.is_open())
    {
        while (!tsFile.eof() && raw_Line < MAX_ROWS)
        {
            string line;
            getline(tsFile, line);
            // Copy The Input Grammar to 2-Dimensional Char Array
            strcpy(raw_production[raw_Line++], line.c_str());
        }
        raw_Line--;
    }
    else // File Not Exists or Failed to Load File
    {
        cerr << "Unable to Open The File...\n";
        exit(-1);
    }
    cout << "All Target CFG Grammar Rules Have Been Copied to Local Production Array\n"
         << endl;
    tsFile.close();
    cout << "Local Production Text File Closed Successfully...\n";
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}

void output2file(string OutputPath, char resultSet[][MAX_COLS])
{
    ofstream(tsFile);
    tsFile.open(OutputPath);
    if (tsFile.is_open())
    {
        for (int i = 0; i < prod_Line; i++)
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

int main(int argc, char **argv)
{
    /*Get Current Working Directory*/
    char cwd[256];
    getcwd(cwd, sizeof(cwd)); // Store In String "cwd"
    string result_dir = cwd, tsPath = cwd, ppPath = cwd, firPath = cwd, flwPath = cwd;
    result_dir += "/IO_Text";
    cout << "Current Working Directory: " << cwd << "\n";
    cout << "FIRST & FOLLOW Set Result Directory: " << result_dir << endl
         << endl;

    /*Generate Project Local Files Directory*/
    string test_set = "/IO_Text/Test_Set.txt";              // Test Set File Path
    string prod_set = "/IO_Text/Processed_Productions.txt"; // Processed Productions File Path
    string firSuffix = "/IO_Text/FIRST_Set.txt";            // FIRST Set File Path
    string flwSuffix = "/IO_Text/FOLLOW_Set.txt";           // FOLLOW Set File Path
    tsPath += test_set;
    ppPath += prod_set;
    firPath += firSuffix;
    flwPath += flwSuffix;

    /*Load Local CFG Grammar Into Production Arrays & Preprocess Given Productions*/
    load2array(tsPath);
    production_preprocess(); // Split Given Productions

    /*Display Loaded CFG Grammar Productions*/
    display_productions();

    /*Calculate The FIRST & FOLLOW Sets of Given Grammar Rules*/
    /*Part I --> Calculate FIRST Set for Each Production*/
    fun_first_set();
    output2file(firPath, FIRST_Result);

    /*Part II --> Calculate FOLLOW Set for Each Production*/
    fun_follow_set();
    output2file(flwPath, FOLLOW_Result);

    return 0;
}
