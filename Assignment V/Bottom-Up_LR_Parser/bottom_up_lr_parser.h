#ifndef TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_BOTTOM_UP_LR_PARSER_H
#define TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_BOTTOM_UP_LR_PARSER_H

#include "global.h"
#include "generate_LR_pt.h"

extern stack<string> sta_parser; // LL(1) Parser Stack

void printStack(const stack<string> &target);
void printQueue(const queue<char> &target);
//void parsingLog(const string &str_log);
void bottom_up_lr_parse();

#endif //TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_BOTTOM_UP_LR_PARSER_H
