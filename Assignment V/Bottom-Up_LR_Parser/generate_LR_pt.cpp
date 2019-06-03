#include "generate_LR_pt.h"

stack<char> index_action;
stack<char> index_goto;
char terminal[6] = {'a', '+', '*', '(', ')', '$'};
char non_terminal[3] = {'E', 'T', 'F'};

string pt_action[pt_Col][action_Col] = {
    // Action Section of Parsing Table
    {"s5", "/", "/", "s4", "/", "/"},
    {"/", "s6", "/", "/", "/", "acc"},
    {"/", "r2", "s7", "/", "r2", "r2"},
    {"/", "r4", "r4", "/", "r4", "r4"},
    {"s5", "/", "/", "s4", "/", "/"},
    {"/", "r6", "r6", "/", "r6", "r6"},
    {"s5", "/", "/", "s4", "/", "/"},
    {"s5", "/", "/", "s4", "/", "/"},
    {"/", "s6", "/", "/", "s11", "/"},
    {"/", "r1", "s7", "/", "r1", "r1"},
    {"/", "r3", "r3", "/", "r3", "r3"},
    {"/", "r5", "r5", "/", "r5", "r5"},
};

string pt_goto[pt_Col][goto_Col] = {
    // Goto Section of Parsing Table
    {"1", "2", "3"},
    {"/", "/", "/"},
    {"/", "/", "/"},
    {"/", "/", "/"},
    {"8", "2", "3"},
    {"/", "/", "/"},
    {"/", "9", "3"},
    {"/", "/", "10"},
    {"/", "/", "/"},
    {"/", "/", "/"},
    {"/", "/", "/"},
    {"/", "/", "/"},
};

void generate_LR_pt()
{
    for (int i = sizeof(terminal) - 1; i >= 0; i--)
        index_action.push(terminal[i]);
    for (int i = sizeof(non_terminal) - 1; i >= 0; i--)
        index_goto.push(non_terminal[i]);
}

void display_LR_pt()
{
    cout << "                Action                  Goto    \n";
    cout << "Status     ";
    for (stack<char> dump = index_action; !dump.empty(); dump.pop())
        cout << dump.top() << "  ";
    cout << "         ";
    for (stack<char> dump = index_goto; !dump.empty(); dump.pop())
        cout << dump.top() << "  ";
    cout << "\n";

    for (int i = 0; i < pt_Col; i++)
    {
        cout << "   " << i << "       ";
        for (int j = 0; j < action_Col; j++)
            cout << pt_action[i][j] << "  ";
        cout << "       ";
        for (int j = 0; j < goto_Col; j++)
            cout << pt_goto[i][j] << "  ";
        cout << "\n";
    }
    cout << "\n/*------------------------------------------------------------*/\n"
         << endl;
}
