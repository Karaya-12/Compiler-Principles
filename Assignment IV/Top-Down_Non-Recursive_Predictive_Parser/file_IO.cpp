#include "file_IO.h"

vector<string> parsingResult; // Final Parsing Result

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
