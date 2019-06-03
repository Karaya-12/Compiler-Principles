#ifndef LR_SEMANTIC_ANALYSIS_CAL_FOLLOW_H
#define LR_SEMANTIC_ANALYSIS_CAL_FOLLOW_H

#include "global.h"
#include "cal_first.h"

extern set<char> set_follow; // Temp FOLLOW Set for Unique Production LHS
extern set<char> set_calc;   // Temp Set for Calculated Produciton LHS

void calculate_follow(char prod_LHS);
void fun_follow_set();

#endif //LR_SEMANTIC_ANALYSIS_CAL_FOLLOW_H
