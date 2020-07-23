cmake_minimum_required(VERSION 3.16)
project(dinamico)

set(CMAKE_CXX_STANDARD 14)
set(DCMAKE_SH="CMAKE_SH-NOTFOUND")
add_executable(dinamico main.cpp Image.h Matches.h Greedy.h Dynamic.h DynamicAverage.h)