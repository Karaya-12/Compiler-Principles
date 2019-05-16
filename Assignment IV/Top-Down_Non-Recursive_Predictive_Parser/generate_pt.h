#ifndef TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_GENERATE_PT_H
#define TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_GENERATE_PT_H

#include "global.h"
#include "cal_first.h"

extern set<char> set_index; // LL(1) Parsing Table Index Line

void pt_index();
void pt_initialize();
void pt_first(int row, int num_prod);
void pt_follow();
void displayParsingTable();

#endif //TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_GENERATE_PT_H
