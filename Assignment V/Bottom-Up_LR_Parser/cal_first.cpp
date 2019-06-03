#include "cal_first.h"

set<char> set_first; // Temp FIRST Set for Unique Production LHS

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
                        calculate_first(production[pre_Prod][pre_Column], pre_Prod, (pre_Column + 1), -1, false);
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
                    calculate_first(production[i][j], i, (j + 1), -1, false);
            }
        }
    }
}
