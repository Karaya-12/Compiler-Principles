#include "generate_pt.h"

set<char> set_index; // LL(1) Parsing Table Index Line

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
