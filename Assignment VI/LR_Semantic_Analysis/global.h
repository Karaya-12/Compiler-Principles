#ifndef LR_SEMANTIC_ANALYSIS_GLOBAL_H
#define LR_SEMANTIC_ANALYSIS_GLOBAL_H

#include <set>
#include <stack>
#include <queue>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#define MAX_ROWS 20
#define MAX_COLS 20
#define MAX_INPUT 40
using namespace std;

extern int raw_Line;                            // Raw Production Line Counter
extern int prod_Line;                           // Processed Production Line Counter
extern int input_Line;                          // Input Text Counter
extern char inputText[MAX_ROWS][MAX_INPUT];      // Input Text With Symbols for LR Parsing
extern char raw_inputText[MAX_ROWS][MAX_INPUT];  // Input Text With Values for Calculating
extern char production[MAX_ROWS][MAX_COLS];     // Processed Productions
extern char raw_production[MAX_ROWS][MAX_COLS]; // Raw Productions
extern char FOLLOW_Result[MAX_ROWS][MAX_COLS];  // FOLLOW Set Result

extern set<char>::iterator iter;

#endif //LR_SEMANTIC_ANALYSIS_GLOBAL_H
