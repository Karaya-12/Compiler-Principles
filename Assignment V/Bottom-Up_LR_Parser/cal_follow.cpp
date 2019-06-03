#include "cal_follow.h"

set<char> set_follow; // Temp FOLLOW Set for Unique Production LHS
set<char> set_calc;   // Temp Set for Calculated Produciton LHS

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
