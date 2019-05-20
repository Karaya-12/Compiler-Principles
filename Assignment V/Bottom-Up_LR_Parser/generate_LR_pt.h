#ifndef TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_GENERATE_LR_PT_H
#define TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_GENERATE_LR_PT_H

#include "global.h"
#define pt_Col 12
#define action_Col 6
#define goto_Col 3

extern stack<char> index_action;
extern stack<char> index_goto;
extern char terminal[6];
extern char non_terminal[3];
extern string pt_action[pt_Col][action_Col];
extern string pt_goto[pt_Col][goto_Col];

void generate_LR_pt();
void display_LR_pt();

#endif //TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_GENERATE_LR_PT_H
