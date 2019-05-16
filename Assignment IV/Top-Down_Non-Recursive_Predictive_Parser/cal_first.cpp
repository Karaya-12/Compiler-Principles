#include "cal_first.h"

set<char> set_current; // Temp FIRST Set for Each Production
set<char> set_first;   // Temp FIRST Set for Unique Production LHS

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
