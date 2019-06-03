#include "proc_input.h"

void input_preprocess()
{
    int i, j, k = -1;
    for (i = 0; i < input_Line; i++)
    {
        for (j = 0; j < sizeof(raw_inputText[i]); j++)
        {
            if (raw_inputText[i][j] != '\0' && raw_inputText[i][j] != '\r')
            {
                // Convert All Numbers to Character 'a'
                if (isdigit(raw_inputText[i][j]))
                {
                    do
                    {
                        j++;
                        if (raw_inputText[i][j] == '.')
                            j++;
                    } while (isdigit(raw_inputText[i][j]));
                    j--;
                    inputText[i][++k] = 'a';
                }
                else
                    inputText[i][++k] = raw_inputText[i][j];
            }
        }
    }
}

void display_input()
{
    cout << "Local Custom Input As Shown Below\n"
         << endl;
    for (int i = 0; i < input_Line; i++)
        cout << raw_inputText[i] << "\n";
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;

    cout << "Processed Input for LR Parsing As Shown Below\n"
         << endl;
    for (int i = 0; i < input_Line; i++)
        cout << inputText[i] << "\n";
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}
