#ifndef TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_GLOBAL_H
#define TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_GLOBAL_H

#include <set>
#include <stack>
#include <queue>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#define MAX_ROWS 20
#define MAX_COLS 20
using namespace std;

extern int raw_Line;                            // Raw Production Line Counter
extern int prod_Line;                           // Processed Production Line Counter
extern int input_Line;                          // Input Text Counter
extern char inputText[MAX_ROWS][MAX_COLS];      // Input Text
extern char production[MAX_ROWS][MAX_COLS];     // Processed Productions
extern char raw_production[MAX_ROWS][MAX_COLS]; // Raw Productions
extern char FIRST_Result[MAX_ROWS][MAX_COLS];   // FIRST Set Result
extern char FOLLOW_Result[MAX_ROWS][MAX_COLS];  // FOLLOW Set Result
extern char parsing_table[MAX_ROWS][MAX_COLS];  // LL(1) Grammar Parsing Table

extern set<char>::iterator iter, iter_curr, iter_index;

#endif //TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_GLOBAL_H
