cmake_minimum_required(VERSION 4.0)
project(BootCamp2025_cpp_tasks)

set(CMAKE_CXX_STANDARD 20)

add_executable(1_HelloWorld
        1_HelloWorld/HelloWorld.cpp
)

add_executable(2_CsvInput
        2_csv_Input/CsvInput.cpp
)

add_executable(3_CsvOutput
        3_csv_Output/CsvOutput.cpp
        3_csv_Output/crlserial2025.hpp
)

add_executable(4_LPF
        4_LPF/ApplyLowPassFilter.cpp
        4_LPF/LowPassFilter.cpp
        4_LPF/LowPassFilter.h
)