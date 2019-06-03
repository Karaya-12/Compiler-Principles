#ifndef LR_SEMANTIC_ANALYSIS_BOTTOM_UP_LR_PARSER_H
#define LR_SEMANTIC_ANALYSIS_BOTTOM_UP_LR_PARSER_H

#include "global.h"
#include "generate_LR_pt.h"
#include "semantic_rules.h"

extern stack<string> sta_parser; // LR Parser Stack
extern vector<string> vec_val;   // LR Parser State Value Vector

void printStack(const stack<string> &target);
void printQueue(const queue<char> &target);
void bottom_up_lr_parse();

#endif //LR_SEMANTIC_ANALYSIS_BOTTOM_UP_LR_PARSER_H
