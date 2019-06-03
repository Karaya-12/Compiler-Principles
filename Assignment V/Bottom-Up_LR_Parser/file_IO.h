#ifndef BOTTOM_UP_LR_PARSER_FILE_IO_H
#define BOTTOM_UP_LR_PARSER_FILE_IO_H

#include "global.h"
#include <fstream>

void load2array(const string &textPath, bool isProduction);
void output2file(const string &OutputPath);

#endif //BOTTOM_UP_LR_PARSER_FILE_IO_H
