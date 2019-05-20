#include "non_recursive_parser.h"

string errorLog;        // Error Log Info
stack<char> sta_parser; // LL(1) Parser Stack

void printStack(const stack<char> &target)
{
    cout << "Current Stack - ";
    for (stack<char> dump = target; !dump.empty(); dump.pop())
        cout << dump.top() << " ";
    cout << "\nStack Size - " << sta_parser.size() << endl;
}

void printQueue(const queue<char> &target)
{
    cout << "Current Input Line Queue - ";
    for (queue<char> dump = target; !dump.empty(); dump.pop())
        cout << dump.front() << " ";
    cout << endl
         << endl;
}

void parsingLog(const string &str_log)
{
    cout << "Corresponding Production - " << str_log << endl;
    parsingResult.push_back(str_log);
}

void non_recursive_parse()
{
    for (int i = 0; i < input_Line; i++) // Iterate for Every Custom Input Line
    {
        // Store Current Processing Input Line Into Queue
        queue<char> inputLine;
        for (int j = 0; j < sizeof(inputText[i]); j++)
            if (inputText[i][j] != '\0' && inputText[i][j] != '\r')
                inputLine.push(inputText[i][j]);
        printQueue(inputLine);

        // Push '$' & The Starting Non-Terminal Symbol Into Parser Stack
        sta_parser.push('$');
        sta_parser.push(production[0][0]);
        printStack(sta_parser);
        printQueue(inputLine);
        while (sta_parser.top() != '$')
        {
            // Get The Index for Current Input Queue Front & Stack Top
            int index_Stack = 0, index_Input = 0;
            for (int j = 0; j < raw_Line; j++, index_Stack++)
                if (parsing_table[j][0] == sta_parser.top())
                    break;
            for (iter_index = set_index.begin(); iter_index != set_index.end(); iter_index++, index_Input++)
                if (*iter_index == inputLine.front())
                    break;
            char pt_target = parsing_table[index_Stack][index_Input + 1]; // Target Location for Queue Front in The Parsing Table

            // Case I --> Stack Top Matches Input Queue Front
            // * Action I --> Pop Stack, Pop Input Line Queue
            if (sta_parser.top() == inputLine.front())
            {
                cout << "Current Matched Input Character - " << inputLine.front() << endl;
                sta_parser.pop();
                inputLine.pop();
            }
            // Case II --> Stack Top is Terminal But Doesn't Match Up to The Input Queue Front
            // * Action II --> Report Error & Discard Current Character
            else if (!isupper(sta_parser.top()))
            {
                errorLog = "Error: Expected Non-Terminal In Stack But Found Terminal - '" + string(1, sta_parser.top()) + "'";
                parsingResult.push_back(errorLog);
                cerr << errorLog << endl;
                sta_parser.pop();
            }
            // Case III --> Stack Top is Non-Terminal
            // * Action III --> Pop Stack Top Element & Add Corresponding Production RHS to The Stack In Reverse Order
            else if (pt_target != '/') // Find A Match In Parsing Table
            {
                // Generate The Full Correponding Production
                char *corr_production = new char[sizeof(production[pt_target - '0']) + 1];
                if (pt_target != '#') // FIRST Set
                {
                    corr_production = production[pt_target - '0'];
                    parsingLog(corr_production);
                }
                else // FOLLOW Set
                {
                    string temp(1, parsing_table[index_Stack][0]);
                    temp += " -> #";
                    strcpy(corr_production, temp.c_str());
                    parsingLog(corr_production);
                }

                sta_parser.pop();
                // Add Corresponding Production RHS to The Stack In Reverse Order
                for (int j = (sizeof(corr_production) - 1); j >= 5; j--)
                    if (corr_production[j] != '|' && corr_production[j] != '#' && corr_production[j] != '\0' && corr_production[j] != '\r')
                        sta_parser.push(corr_production[j]);
                printStack(sta_parser);
                printQueue(inputLine);
            }
            else // Can't Find A Match In Parsing Table --> Discard Current Input Character
            {
                errorLog = "Error: Discard Current Input Character - '" + string(1, inputLine.front()) + "' Due to: Can't Find A Match In Parsing Table.";
                parsingResult.push_back(errorLog);
                cerr << errorLog << endl;
                inputLine.pop();
            }
        }
        if (inputLine.front() == '$') // Both Stack & Input Queue Have Reached Their Ends
        {
            cout << "LL(1) Grammar Non-Recursive Predictive Parsing Procedure Finished" << endl;
            cout << "\n/*------------------------------------------------------------*/\n"
                 << endl;
        }
        else
        {
            errorLog = "Error: Custom Input Line Hasn't Reached Its End !";
            parsingResult.push_back(errorLog);
            cerr << errorLog << endl;
        }
    }
}
