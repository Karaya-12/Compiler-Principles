#include "bottom_up_lr_parser.h"

stack<string> sta_parser; // LR Parser Stack
vector<string> vec_val;   // LR Parser State Value Stack

void printStack(const stack<string> &target)
{
    cout << "Current Stack - ";
    for (stack<string> dump = target; !dump.empty(); dump.pop())
        cout << dump.top() << " ";
    cout << endl;
}

void printQueue(const queue<char> &target)
{
    cout << "Current Input Line Queue - ";
    for (queue<char> dump = target; !dump.empty(); dump.pop())
        cout << dump.front() << " ";
    cout << "\n";
    cout << endl;
}

void bottom_up_lr_parse()
{
    for (int i = 0; i < input_Line; i++) // Iterate for Every Custom Input Line
    {
        // Store Current Processing Input Line Into Queue
        queue<char> inputLine;
        for (int j = 0; j < sizeof(inputText[i]); j++)
            if (inputText[i][j] != '\0' && inputText[i][j] != '\r')
                inputLine.push(inputText[i][j]);
        printQueue(inputLine);
        // Store Raw Input Line Into String Queue
        queue<string> valueLine;
        for (int j = 0; j < sizeof(raw_inputText[i]); j++)
            if (raw_inputText[i][j] != '\0' && raw_inputText[i][j] != '\r')
                valueLine.push(string(1, raw_inputText[i][j]));
        // Push Starting State '0' Into Parser Stack
        sta_parser.push("0");
        printStack(sta_parser);
        cout << endl;

        while (true)
        {
            // Locate The First Input Character
            int col_action = 0;
            for (; col_action < sizeof(terminal); col_action++)
                if (terminal[col_action] == inputLine.front())
                    break;
            string action_result = pt_action[stoi(sta_parser.top())][col_action];

            // Case I --> Shift Action
            // * Action I --> 2 * Push Stack (Input & State) + 1 * Pop Queue
            // * Action Value Vector --> Push Back (Input)
            if (action_result[0] == 's')
            {
                cout << "Shift Action - " << action_result << endl;
                sta_parser.push(string(1, inputLine.front())); // Push Input
                sta_parser.push(string(1, action_result[1]));  // Push State
                vec_val.push_back(valueLine.front());          // Push Raw Input
                inputLine.pop();
                valueLine.pop();
                printValue();
            }
                // Case II --> Reduce Action
                // * Action II --> 1 * Pop + 2 * Push Stack (LHS + goto State)
                // * Action Value Vector --> Call Corresponding Semantic Actions
            else if (action_result[0] == 'r') // Reduce Action
            {
                cout << "Reduce Action - " << action_result << endl;
                // 1. Pop 2 * |RHS|
                string corr_production = production[(action_result[1] - '0') - 1];
                int len_RHS = corr_production.size() - 5; // |RHS|
                for (int j = 0; j < 2 * len_RHS; j++)
                    sta_parser.pop();

                // 2. Push Corresponding Production LHS
                string temp_top = sta_parser.top();
                cout << "Reduce to Production - " << corr_production << endl;
                sta_parser.push(string(1, corr_production[0]));
                // 3. Push goto(Reduce State, LHS) Result State
                int col_goto = -1;
                switch (corr_production[0])
                {
                    case 'E':
                        col_goto = 0;
                        break;
                    case 'T':
                        col_goto = 1;
                        break;
                    case 'F':
                        col_goto = 2;
                        break;
                    default:
                        break;
                }
                sta_parser.push(pt_goto[stoi(temp_top)][col_goto]);

                // Pass Reduced Production to Semantic Actions
                semantic_actions((action_result[1] - '0'));
            }
                // Case III --> Accept
                // * Action III --> Finish + Break Out + Print Final Value
            else if (action_result == "acc")
            {
                cout << "LL(1) Grammar Non-Recursive Predictive Parsing Procedure Finished" << endl;
                cout << "\nExpression - " << raw_inputText[i] << endl;
                cout << "Final Result = " << vec_val.back() << endl;
                cout << "\n/*------------------------------------------------------------*/\n"
                     << endl;
                break;
            }
                // Case IV --> No Match in Parsing Table
            else
            {
                cerr << "Error: Can not Find Match for Target State - '" << sta_parser.top()
                     << "', Target Non-Terminal - '" << inputLine.front() << endl;
                cout << "An Error Has Occurred ! Discard Current Input Character - '" << inputLine.front() << "'" << endl;
                inputLine.pop();
                valueLine.pop();
            }
            printStack(sta_parser);
            printQueue(inputLine);
        }
    }
}
