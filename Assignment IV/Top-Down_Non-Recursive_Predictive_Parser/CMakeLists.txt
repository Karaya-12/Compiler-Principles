cmake_minimum_required(VERSION 3.14)
project(Top_Down_Non_Recursive_Predictive_Parser)

set(CMAKE_CXX_STANDARD 17)

add_executable(Top_Down_Non_Recursive_Predictive_Parser main.cpp global.h cal_first.cpp cal_first.h cal_follow.cpp cal_follow.h file_IO.cpp file_IO.h generate_pt.cpp generate_pt.h non_recursive_parser.cpp non_recursive_parser.h proc_production.cpp proc_production.h)