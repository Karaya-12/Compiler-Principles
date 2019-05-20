cmake_minimum_required(VERSION 3.14)
project(Bottom_Up_LR_Parser)

set(CMAKE_CXX_STANDARD 17)

add_executable(Bottom_Up_LR_Parser main.cpp bottom_up_lr_parser.cpp bottom_up_lr_parser.h global.h cal_first.cpp cal_first.h cal_follow.cpp cal_follow.h file_IO.cpp file_IO.h generate_LR_pt.cpp generate_LR_pt.h proc_production.cpp proc_production.h)