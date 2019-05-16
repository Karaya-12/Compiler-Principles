#ifndef TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_NON_RECURSIVE_PARSER_H
#define TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_NON_RECURSIVE_PARSER_H

#include "global.h"
#include <vector>

extern string errorLog;              // Error Log Info
extern set<char> set_index;          // LL(1) Parsing Table Index Line
extern stack<char> sta_parser;       // LL(1) Parser Stack
extern vector<string> parsingResult; // Final Parsing Result

void printStack(const stack<char> &target);
void printQueue(const queue<char> &target);
void parsingLog(const string &str_log);
void non_recursive_parse();

#endif //TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_NON_RECURSIVE_PARSER_H
