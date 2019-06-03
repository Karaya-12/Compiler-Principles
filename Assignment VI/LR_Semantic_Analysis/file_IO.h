#ifndef LR_SEMANTIC_ANALYSIS_FILE_IO_H
#define LR_SEMANTIC_ANALYSIS_FILE_IO_H

#include "global.h"
#include <fstream>

void load2array(const string &textPath, bool isProduction);
void output2file(const string &OutputPath);

#endif //LR_SEMANTIC_ANALYSIS_FILE_IO_H
