#ifndef BOTTOM_UP_LR_PARSER_CAL_FIRST_H
#define BOTTOM_UP_LR_PARSER_CAL_FIRST_H

#include "global.h"

extern set<char> set_first; // Temp FIRST Set for Unique Production LHS

void calculate_first(char prod_LHS, int pre_Prod, int pre_Column, int root_Row, bool isRoot);

#endif //BOTTOM_UP_LR_PARSER_CAL_FIRST_H
