#include <map>
#include <list>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unistd.h>
using namespace std;

string keyword[6] = {"for", "if", "then", "else", "while", "do"};
static map<string, int> lex_Dict;
/*Initialize The Lexical Analyzer String-Integer Dictionary*/
void LexDict_Init()
{
    lex_Dict.insert(map<string, int>::value_type("for", 1));
    lex_Dict.insert(map<string, int>::value_type("if", 2));
    lex_Dict.insert(map<string, int>::value_type("then", 3));
    lex_Dict.insert(map<string, int>::value_type("else", 4));
    lex_Dict.insert(map<string, int>::value_type("while", 5));
    lex_Dict.insert(map<string, int>::value_type("do", 6));
    lex_Dict.insert(map<string, int>::value_type("var", 10));
    lex_Dict.insert(map<string, int>::value_type("num", 11));
    lex_Dict.insert(map<string, int>::value_type("+", 13));
    lex_Dict.insert(map<string, int>::value_type("-", 14));
    lex_Dict.insert(map<string, int>::value_type("*", 15));
    lex_Dict.insert(map<string, int>::value_type("/", 16));
    lex_Dict.insert(map<string, int>::value_type(":", 17));
    lex_Dict.insert(map<string, int>::value_type(":=", 18));
    lex_Dict.insert(map<string, int>::value_type("<", 19));
    lex_Dict.insert(map<string, int>::value_type("<>", 21));
    lex_Dict.insert(map<string, int>::value_type("<=", 22));
    lex_Dict.insert(map<string, int>::value_type(">", 23));
    lex_Dict.insert(map<string, int>::value_type(">=", 24));
    lex_Dict.insert(map<string, int>::value_type("=", 25));
    lex_Dict.insert(map<string, int>::value_type(";", 26));
    lex_Dict.insert(map<string, int>::value_type("(", 27));
    lex_Dict.insert(map<string, int>::value_type(")", 28));
    lex_Dict.insert(map<string, int>::value_type("#", 0));
}

int GetIDbyLexeme(string key)
{
    map<string, int>::iterator lexDict_Iterator;
    lexDict_Iterator = lex_Dict.find(key);
    if (lexDict_Iterator == lex_Dict.end()) // Not Found
        return -1;
    return lexDict_Iterator->second; // Return Value (Dict ID)
}

bool isLetter(char letter)
{
    if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z'))
        return true;
    return false;
}

bool isDigit(char digit)
{
    if (digit >= '0' && digit <= '9')
        return true;
    return false;
}

bool isKeyword(string keywords)
{
    for (int i = 0; i < 6; i++)
    {
        if (keyword[i] == keywords)
            return true;
    }
    return false;
}

bool isVar(string var) // Var = letter (letter | digit) *
{
    if (isLetter(var[0]))
    {
        for (int i = 0; i < var.length(); i++)
        {
            if (!(isLetter(var[i]) || isDigit(var[i])))
                return false;
        }
        return true;
    }
    return false;
}

bool isNum(string num)
{
    if (isDigit(num[0]))
    {
        int isDecimal = 1, isScientific = 1;
        for (int i = 0; i < num.length(); i++)
        {
            if (isDigit(num[i]))
                ;
            else if ((num[i] == '+' || num[i] == '-') && isDigit(num[i + 1])) // e.g., +10 or -10
                ;
            else if (num[i] == '.' && isDigit(num[i + 1]) && isDecimal) // Decimal Representation
                isDecimal = 0;
            else if (((num[i] == 'E' || num[i] == 'e') && (num[i + 1] == '+' || num[i + 1] == '-' || isDigit(num[i + 1])) && isScientific)) // Scientific Notation
                isScientific = 0;
            else
                return false;
        }
        return true;
    }
    return false;
}

string generateToken(string lexeme, int type)
{
    int id;
    if (type == 0)
        id = GetIDbyLexeme(lexeme);
    else if (type == 1)
        id = GetIDbyLexeme("var");
    else if (type == 2)
        id = GetIDbyLexeme("num");

    string token;
    token += "<" + to_string(id);
    token += "," + lexeme;
    token += ">";
    return token;
}

string lexemeAnalyzer(string lexeme, int num_Line, int num_Column)
{
    if (isKeyword(lexeme))
        return generateToken(lexeme, 0);
    else if (isVar(lexeme))
        return generateToken(lexeme, 1);
    else if (isNum(lexeme))
        return generateToken(lexeme, 2);
    else if (GetIDbyLexeme(lexeme) == -1) // Not Found Error
    {
        string line = to_string(num_Line);
        string column = to_string(num_Column);
        string error_text = "Found Some Errors in - Line ";
        error_text += line;
        error_text += " Column ";
        error_text += column;
        error_text += ";";
        return error_text;
    }
    return generateToken(lexeme, 0);
}

string lexical_analyzer(string InputPath)
{
    int num_Line = 0;   // Line Count
    int num_Column = 0; // Column Count
    string token_stream;

    /*Read Local Source Code Text File*/
    ifstream(scFile);
    scFile.open(InputPath);
    if (scFile.is_open())
    {
        string line;
        cout << "Passed In Source Code As Shown Below\n";
        while (!scFile.eof())
        {
            getline(scFile, line);
            num_Line++;
            cout << line << endl;

            string characters, token;
            /*Split Characters & Signs*/
            for (int i = 0; i < line.length(); i++)
            {
                num_Column++;
                if (line[i] != ' ') // Skip All Whitespaces
                {
                    characters += line[i];
                    // End of Current Word or Line
                    if (line[i + 1] == ' ' || line[i + 1] == '\0' || line[i + 1] == ';')
                    {
                        // Generate Lexeme Based on Current Characters
                        string lexeme = characters;
                        characters = ""; //Discard All Used Characters

                        token = lexemeAnalyzer(lexeme, num_Line, num_Column);
                        token_stream += token;
                        token_stream += " ";
                    }
                    // Check Out The Following Characters
                    else
                    {
                        string next_temp = characters;
                        next_temp += line[i + 1]; // Append The Next Character
                        bool isKVN = isKeyword(next_temp) || isVar(next_temp) || isNum(next_temp);
                        // New Charaters Is Not KVN & Can't Be Found In Lexical Dictionary
                        if (!isKVN && GetIDbyLexeme(next_temp) == -1)
                        {
                            // Generate Lexeme Based on Current Characters
                            string lexeme = characters;
                            characters = ""; //Discard All Used Characters

                            token = lexemeAnalyzer(lexeme, num_Line, num_Column);
                            token_stream += token;
                            token_stream += " ";
                        }
                    }
                }
            }
            token_stream += "\n";
        }
    }
    else // File Not Exists or Failed to Load File
    {
        cerr << "Unable to Open The File...\n";
        exit(-1);
    }
    scFile.close();
    cout << "Source Code File Closed Successfully...\n"
         << endl;
    return token_stream;
}

void output2file(string OutputPath, string token_stream)
{
    ofstream(tsFile);
    tsFile.open(OutputPath);
    if (tsFile.is_open())
        tsFile << token_stream;
    else // File Not Exists or Failed to Write File
    {
        cerr << "Unable to Write to The File...\n";
        exit(-1);
    }
    tsFile.close();
    cout << "\nToken Stream File Closed Successfully...\n";
}

int main(int argc, char const *argv[])
{
    LexDict_Init(); // Initialize The Lexical Analyzer lex_Dict

    /*Get Current Working Directory*/
    char cwd[256];
    getcwd(cwd, sizeof(cwd)); // Store In String "cwd"
    cout << "Current Working Directory: " << cwd << "\n";

    /*Generate Project IO Files Directory*/
    string scPath = "/IO_Text/Source_Code.txt";  // Source Code File Path
    string tsPath = "/IO_Text/Token_Stream.txt"; // Token Stream File Path
    string InputPath = cwd, OutputPath = cwd;
    InputPath += scPath;
    OutputPath += tsPath;
    cout << "Lexical Analyzer IO Files Directory: " << InputPath << endl
         << endl;

    /*Pass Source Code to Lexical Analyzer*/
    string token_stream = lexical_analyzer(InputPath);
    if (token_stream != "")
    {
        cout << "Lexical Analyzer Token Stream\n"
             << token_stream;
        cout << "Output The Token Stream to Local File..." << endl;
        cout << "Check Out Directory: " << OutputPath << endl;
        output2file(OutputPath, token_stream);
    }
    else
    {
        cerr << "Fail to Generate Token Stream..." << endl;
        exit(-1);
    }
    return 0;
}
