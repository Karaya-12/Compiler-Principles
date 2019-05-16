#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include <unistd.h>
using namespace std;

int n = 0;                       // Input Text Pointer, Starting from 0
bool balancedParentheses = true; // Matching Parentheses

string inputLine;          // Custom Input Line
string output;             // Temp Output Info
string errorLog;           // Error Log Info
vector<string> result;     // Final Result
vector<string> cust_Input; // Custom Input Text

// All Total 5 Productions <--> Production LHS
void production_E(string inputLine);
void production_M(string inputLine);
void production_T(string inputLine);
void production_N(string inputLine);
void production_F(string inputLine);

void parsingLog(string str_log)
{
    cout << str_log << endl;
    result.push_back(str_log);
}

void checkParentheses(string inputLine)
{
    if (inputLine[n] == '(' && inputLine.find(')') > 100)
        balancedParentheses = false;
}

void production_E(string inputLine)
{
    // Production E Accepts 'a' & '(' (FIRST Set)
    if (inputLine[n] == '(' || inputLine[n] == 'a')
    {
        output = "E -> TM";
        parsingLog(output);
        checkParentheses(inputLine);

        production_T(inputLine);
        production_M(inputLine);
    }
    else
    {
        if (inputLine[n] == ')')
        {
            errorLog = "Error: Expected '(' or 'a' But Found '" + string(1, inputLine[n]) + "'";
            result.push_back(errorLog);
            cerr << errorLog << endl;
        }
        else // Discard Current Character & Recall Funtion
        {
            errorLog = "Error: Discard '" + string(1, inputLine[n]) + "'";
            result.push_back(errorLog);
            cerr << errorLog << endl;

            n++;
            production_E(inputLine);
        }
    }
}

void production_M(string inputLine)
{
    // Production M First Part Only Accepts '+' (FIRST Set)
    if (inputLine[n] == '+')
    {
        output = "M -> +TM";
        parsingLog(output);

        n++;
        production_T(inputLine);
        production_M(inputLine);
    }
    // Production M Second Part Accepts '$' (FOLLOW Set)
    else if (inputLine[n] == ')' || inputLine[n] == '$')
    {
        output = "M -> ε";
        parsingLog(output);
    }
    else // Discard Current Character & Recall Funtion
    {
        errorLog = "Error: Discard '" + string(1, inputLine[n]) + "'";
        result.push_back(errorLog);
        cerr << errorLog << endl;

        n++;
        production_M(inputLine);
    }
}

void production_T(string inputLine)
{
    // Production T Accepts 'a' & '(' (FIRST Set)
    if (inputLine[n] == '(' || inputLine[n] == 'a')
    {
        output = "T -> FN";
        parsingLog(output);
        checkParentheses(inputLine);

        production_F(inputLine);
        production_N(inputLine);
    }
    else
    {
        if (inputLine[n] == '+' || inputLine[n] == ')' || inputLine[n] == '#')
        {
            errorLog = "Error: Expected '(' or 'a' But Found '" + string(1, inputLine[n]) + "'";
            result.push_back(errorLog);
            cerr << errorLog << endl;
        }
        else // Discard Current Character & Recall Funtion
        {
            errorLog = "Error: Discard '" + string(1, inputLine[n]) + "'";
            result.push_back(errorLog);
            cerr << errorLog << endl;

            n++;
            production_T(inputLine);
        }
    }
}

void production_N(string inputLine)
{
    // Production N First Part Only Accepts '*' (FIRST Set)
    if (inputLine[n] == '*' && (inputLine[n + 1] == '+' || inputLine[n + 1] == '*'))
    {
        // Got Duplicate Terminating Character
        output = "N -> *FN";
        parsingLog(output);

        errorLog = "Error: Missing Terminating Character 'a'";
        result.push_back(errorLog);
        cerr << errorLog << endl;

        // Discard Current Character & Recall Funtion
        output = "F -> a";
        parsingLog(output);

        n++;
        production_N(inputLine);
    }
    else if (inputLine[n] == '*') // Proper Input
    {
        output = "N -> *FN";
        parsingLog(output);

        n++;
        production_F(inputLine);
        production_N(inputLine);
    }
    // Production N Second Part Accepts '+', )' & '$' (FOLLOW Set)
    else if (inputLine[n] == '+' || inputLine[n] == ')' || inputLine[n] == '$')
    {
        output = "N -> ε";
        parsingLog(output);
    }
    else // Discard Current Character & Recall Funtion
    {
        errorLog = "Error: Discard '" + string(1, inputLine[n]) + "'";
        result.push_back(errorLog);
        cerr << errorLog << endl;

        n++;
        production_N(inputLine);
    }
}

void production_F(string inputLine)
{
    // Production F First Part Accepts '(' & 'a' (FIRST Set)
    if (inputLine[n] == '(')
    {
        output = "F -> (E)";
        parsingLog(output);
        checkParentheses(inputLine);

        n++;
        production_E(inputLine);
    }
    else if (inputLine[n] == 'a')
    {
        output = "F -> a";
        parsingLog(output);
        n++;
    }
    else if (inputLine[n] == '+' || inputLine[n] == '*' || inputLine[n] == ')' || inputLine[n] == '#')
    {
        errorLog = "Error: Expected '(' or 'a' But Found '" + string(1, inputLine[n]) + "'";
        result.push_back(errorLog);
        cerr << errorLog << endl;
    }
    else // Discard Current Character & Recall Funtion
    {
        errorLog = "Error: Discard '" + string(1, inputLine[n]) + "'";
        result.push_back(errorLog);
        cerr << errorLog << endl;

        n++;
        production_F(inputLine);
    }
}

/*----------------------------------------------------------------------------------------------------*/

void load2array(string textPath)
{
    /*Load Local Test Grammar Into Production Rules*/
    ifstream(tsFile);
    tsFile.open(textPath);
    if (tsFile.is_open())
    {
        while (!tsFile.eof())
        {
            string line;
            getline(tsFile, line);
            cust_Input.push_back(line);
        }
    }
    else // File Not Exists or Failed to Load File
    {
        cerr << "Unable to Open Local Custom Input Text File...\n";
        exit(-1);
    }
    cout << "All Custom Inputs Have Been Copied to Local Production Array\n"
         << endl;
    tsFile.close();
    cout << "Local Input Text File Closed Successfully...\n";
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}

void output2file(string OutputPath)
{
    ofstream(tsFile);
    tsFile.open(OutputPath);
    if (tsFile.is_open())
    {
        for (int i = 0; i < result.size(); i++)
            tsFile << result[i] << "\n";
    }
    else // File Not Exists or Failed to Write File
    {
        cerr << "Unable to Write to The File...\n";
        exit(-1);
    }
    tsFile.close();
    cout << "Top-Down Recursive Descent Parsing Result File Closed Successfully...\n";
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}

/*----------------------------------------------------------------------------------------------------*/

void recursive_descent_parsing(string inputLine)
{
    balancedParentheses = true;
    production_E(inputLine);
    if (!balancedParentheses)
    {
        errorLog = "Error: Unbalanced Parentheses, Missing ')' !";
        result.push_back(errorLog);
        cerr << errorLog << endl;
    }
}

int main(int argc, char const *argv[])
{
    /*Get Current Working Directory*/
    char cwd[256];
    getcwd(cwd, sizeof(cwd)); // Store In String "cwd"
    string result_dir = cwd, inputPath = cwd, outputPath = cwd;
    result_dir += "/IO_Text";
    cout << "Current Working Directory: " << cwd << "\n";
    cout << "Results Output Directory: " << result_dir << endl
         << endl;

    /*Generate Project Local Files Directory*/
    string inputSuffix = "/IO_Text/Input_Text.txt";     // Input Text File Path
    string outputSuffix = "/IO_Text/Output_Result.txt"; // Output Text File Path
    inputPath += inputSuffix;
    outputPath += outputSuffix;

    /*Load Local Input Text Into String Array*/
    load2array(inputPath);

    cout << "Custom Input Text As Shown Down Below\n";
    for (int i = 0; i < cust_Input.size(); i++)
        cout << cust_Input[i] << endl;

    int selection = -1;
    cout << "Input the Target Text Line Number (No.1 ~ No.4): ";
    cin >> selection;
    cout << "\nProcessing Input Line - " << cust_Input[selection - 1] << endl;

    /*Parsing Custom Input Text*/
    recursive_descent_parsing(cust_Input[selection - 1]);

    cout << endl
         << "The Result of Complete Parsing Procedure In Proper Order\n";
    for (int i = 0; i < result.size(); i++)
        cout << result[i] << endl;
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;

    /*Output Parsing Result to Local Text File*/
    output2file(outputPath);

    return 0;
}
