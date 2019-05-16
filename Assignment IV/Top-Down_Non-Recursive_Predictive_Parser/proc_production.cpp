#include "proc_production.h"

int prod_Line = 0; // Processed Production Line Counter

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
