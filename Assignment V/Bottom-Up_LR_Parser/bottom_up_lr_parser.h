#ifndef BOTTOM_UP_LR_PARSER_BOTTOM_UP_LR_PARSER_H
#define BOTTOM_UP_LR_PARSER_BOTTOM_UP_LR_PARSER_H

#include "global.h"
#include "generate_LR_pt.h"

extern stack<string> sta_parser; // LR Parser Stack

void printStack(const stack<string> &target);
void printQueue(const queue<char> &target);
void bottom_up_lr_parse();

#endif //BOTTOM_UP_LR_PARSER_BOTTOM_UP_LR_PARSER_H
