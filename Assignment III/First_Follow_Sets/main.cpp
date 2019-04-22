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

int num_Rules = -1;                           // Production Rule Counter
char production[MAX_ROWS][MAX_COLS] = {0};    // Production Rules
char FIRST_Result[MAX_ROWS][MAX_COLS] = {0};  // FIRST Set Result
char FOLLOW_Result[MAX_ROWS][MAX_COLS] = {0}; // FOLLOW Set Result

set<char>::iterator iter;
set<char> set_first;  // Temp FIRST Set for Current Production
set<char> set_follow; // Temp FOLLOW Set for Current Production

/*                 Assumptions
 * 1. Epsilon Εε is represented by ‘#’
 * 2. Grammar is not left recursive
 * 3. Each production of a non terminal is entered on a different line
 * 4. Only Upper Case letters are Non-Terminals, thus everything else is a terminal
 */

void calculate_first(char start_Symbol, int pre_Rule, int pre_Column)
{
    // Check Every Iteration's Start Symbol & Find The Matching Production
    for (int i = 0; i < num_Rules; i++)
    {
        if (production[i][0] == start_Symbol)
        {
            // Case --> Production Start Symbol is Terminal (e.g. 4 -> +aB)
            // * Result I --> FIRST Set += Starting Symbol
            if (!(isupper(start_Symbol)))
            {
                set_first.insert(start_Symbol);
                return;
            }

            // Case --> Production Start Symbol is NOT Terminal
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
                    // Case I/1 --> Current Iteration's Production Contains ε & Has Following Non-Terminal
                    // * Result III --> Continue to Iterate, Start from The Next Character from Previous Iteration
                    if (production[pre_Rule][pre_Column] != '\0' && (pre_Rule != 0 || pre_Column != 0))
                        calculate_first(production[pre_Rule][pre_Column], pre_Rule, (pre_Column + 1));
                }

                // Case II --> Production Starts With a Terminal (e.g. E -> +TE')
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
    for (int num_Prod = 0; num_Prod < num_Rules; num_Prod++)
    {
        int flag = 0, column = 0;

        // Call Function to Calculate FIRST Set of Current Production
        char start_Symbol = production[num_Prod][0];
        calculate_first(start_Symbol, 0, 0);

        // Display The Final FIRST Set Result of Current Grammar Production
        cout << "FIRST(" << start_Symbol << ") = { ";
        FIRST_Result[num_Prod][0] = start_Symbol; // First Character is Start Symbol
        for (iter = set_first.begin(); iter != set_first.end(); iter++)
        {
            cout << *iter << ", ";
            FIRST_Result[num_Prod][++column] = *iter;
        }
        set_first.clear(); // Clear The Whole FIRST Set of Current Production
        cout << "}\n";
    }
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}

/*----------------------------------------------------------------------------------------------------*/

void calculate_follow(char start_Symbol)
{
    // Case --> Start Symbol
    // * Result I --> FOLLOW Set += $
    if (start_Symbol == production[0][0])
        set_follow.insert('$');

    // Check Whole Productions to Find The Match for Current Start Symbol
    for (int i = 0; i < num_Rules; i++)
    {
        for (int j = 5; j < sizeof(production[i]); j++) // Start With 5th Char
        {
            if (production[i][j] == start_Symbol)
            {
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
                else if (start_Symbol != production[i][0])
                    calculate_follow(production[i][0]);
            }
        }
    }
}

void fun_follow_set()
{
    cout << "FOLLOW Set of Given Grammar Rules\n"
         << endl;
    for (int num_Prod = 0; num_Prod < num_Rules; num_Prod++)
    {
        int flag = 0, column = 0;

        // Call Function to Calculate FOLLOW Set of Current Production
        char start_Symbol = production[num_Prod][0];
        calculate_follow(start_Symbol);

        // Display The Final FOLLOW Set Result of Current Grammar Production
        cout << "FOLLOW(" << start_Symbol << ") = { ";
        FOLLOW_Result[num_Prod][0] = start_Symbol; // First Character is Start Symbol
        for (iter = set_follow.begin(); iter != set_follow.end(); iter++)
        {
            cout << *iter << ", ";
            FOLLOW_Result[num_Prod][++column] = *iter;
        }
        set_follow.clear(); // Clear The Whole FOLLOW Set of Current Production
        cout << "}\n";
    }
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
        while (!tsFile.eof() && num_Rules < MAX_ROWS)
        {
            string line;
            getline(tsFile, line);
            // Copy The Input Grammar to 2-Dimensional Char Array
            strcpy(production[++num_Rules], line.c_str());
        }
    }
    else // File Not Exists or Failed to Load File
    {
        cerr << "Unable to Open The File...\n";
        exit(-1);
    }
    tsFile.close();
    cout << "Grammar Text File Closed Successfully...\n";
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}

void output2file(string OutputPath, char resultSet[][MAX_COLS])
{
    ofstream(tsFile);
    tsFile.open(OutputPath);
    if (tsFile.is_open())
    {
        for (int i = 0; i < num_Rules; i++)
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
    string result_dir = cwd, tsPath = cwd, firPath = cwd, flwPath = cwd;
    result_dir += "/IO_Text";
    cout << "Current Working Directory: " << cwd << "\n";
    cout << "FIRST & FOLLOW Set Result Directory: " << result_dir << endl
         << endl;

    /*Generate Project Output Results Directory*/
    string test_set = "/IO_Text/Test_Set_Default.txt"; // Test Set File Path
    string firSuffix = "/IO_Text/FIRST_Set.txt";       // FIRST Set File Path
    string flwSuffix = "/IO_Text/FOLLOW_Set.txt";      // FOLLOW Set File Path
    tsPath += test_set;
    firPath += firSuffix;
    flwPath += flwSuffix;

    /*Load Local CFG Grammar Rules Into Production Arrays*/
    load2array(tsPath);

    /*Display Loaded CFG Grammar Productions*/
    cout << "All Target CFG Grammar Rules Have Been Copied to Local Production Array\n"
         << "Local CFG Grammar Productions As Shown Below\n"
         << endl;
    for (int i = 0; i < num_Rules; i++)
        cout << production[i] << endl;
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;

    /*Calculate The FIRST & FOLLOW Sets of Given Grammar Rules*/
    /*Part I --> Calculate FIRST Set for Each Production*/
    fun_first_set();
    output2file(firPath, FIRST_Result);

    /*Part II --> Calculate FOLLOW Set for Each Production*/
    fun_follow_set();
    output2file(flwPath, FOLLOW_Result);

    return 0;
}
