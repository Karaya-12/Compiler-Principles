#ifndef BOTTOM_UP_LR_PARSER_CAL_FOLLOW_H
#define BOTTOM_UP_LR_PARSER_CAL_FOLLOW_H

#include "global.h"
#include "cal_first.h"

extern set<char> set_follow; // Temp FOLLOW Set for Unique Production LHS
extern set<char> set_calc;   // Temp Set for Calculated Produciton LHS

void calculate_follow(char prod_LHS);
void fun_follow_set();

#endif //BOTTOM_UP_LR_PARSER_CAL_FOLLOW_H
