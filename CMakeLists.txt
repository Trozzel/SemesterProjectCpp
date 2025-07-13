cmake_minimum_required(VERSION 3.31)
project(CS517SemesterProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk)

add_executable(CS517SemesterProject main.cpp NDVector.cpp)
