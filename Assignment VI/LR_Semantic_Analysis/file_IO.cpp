#include "file_IO.h"

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
                strcpy(raw_inputText[input_Line++], line.c_str());
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

void output2file(const string &OutputPath) // FOLLOW Set Result
{
    ofstream(tsFile);
    tsFile.open(OutputPath);
    if (tsFile.is_open())
    {
        int i = 0;
        do
        {
            tsFile << FOLLOW_Result[i][0] << " -> { ";
            for (int j = 1; j < sizeof(FOLLOW_Result[i]); j++)
            {
                if (FOLLOW_Result[i][j] == '#')
                    tsFile << "\u03B5, "; // Unicode Escape Characters of ε
                else if (FOLLOW_Result[i][j] != '\0')
                {
                    if (FOLLOW_Result[i][j + 1] == '\0')
                        tsFile << FOLLOW_Result[i][j] << " }";
                    else
                        tsFile << FOLLOW_Result[i][j] << ", ";
                }
            }
            tsFile << endl;
            i++;
        } while (FOLLOW_Result[i][0] != '\0');
    }
    else // File Not Exists or Failed to Write File
    {
        cerr << "Unable to Write to The File...\n";
        exit(-1);
    }
    tsFile.close();
    cout << "FOLLOW Set Result File Closed Successfully...\n";
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}
