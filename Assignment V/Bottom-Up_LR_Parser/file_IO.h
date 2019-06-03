#ifndef TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_FILE_IO_H
#define TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_FILE_IO_H

#include "global.h"
#include <fstream>

void load2array(const string &textPath, bool isProduction);
void output2file(const string &OutputPath);

#endif //TOP_DOWN_NON_RECURSIVE_PREDICTIVE_PARSER_FILE_IO_H
