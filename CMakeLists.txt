cmake_minimum_required(VERSION 4.0)
project(BootCamp2025_cpp_tasks)

set(CMAKE_CXX_STANDARD 20)

add_executable(BootCamp2025_cpp_tasks
        1_HelloWorld/HelloWorld.cpp
        2_csv_Input/CsvInput.cpp
        3_csv_Output/CsvOutput.cpp
        4_LPF/ApplyLowPassFilter.cpp
        4_LPF/LowPassFilter.cpp
        4_LPF/LowPassFilter.h
)
