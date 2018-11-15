cmake_minimum_required (VERSION 3.8)
set (CMAKE_CXX_STANDARD 14)
include_directories(include)
add_executable (SmallParsec "main.cpp" "lib/UnitParser.cpp" "lib/OrParser.cpp" "lib/AndParser.cpp")
