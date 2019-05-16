#ifndef TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_CAL_FIRST_H
#define TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_CAL_FIRST_H

#include "global.h"
#include "generate_pt.h"

extern set<char> set_current; // Temp FIRST Set for Each Production
extern set<char> set_first;   // Temp FIRST Set for Unique Production LHS

void calculate_first(char prod_LHS, int pre_Prod, int pre_Column, int root_Row, bool isRoot);
void fun_first_set();

#endif //TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_CAL_FIRST_H
