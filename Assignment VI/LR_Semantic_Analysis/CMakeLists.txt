cmake_minimum_required(VERSION 3.14)
project(LR_Semantic_Analysis)

set(CMAKE_CXX_STANDARD 17)

add_executable(LR_Semantic_Analysis main.cpp global.h bottom_up_lr_parser.cpp bottom_up_lr_parser.h cal_first.cpp cal_first.h cal_follow.cpp cal_follow.h file_IO.cpp file_IO.h proc_production.cpp proc_production.h proc_input.cpp proc_input.h generate_LR_pt.cpp generate_LR_pt.h semantic_rules.cpp semantic_rules.h)