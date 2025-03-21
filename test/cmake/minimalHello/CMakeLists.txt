cmake_minimum_required(VERSION 3.21.1)
find_package(chpl REQUIRED)
project(hello LANGUAGES CHPL)
add_executable(hello hello.chpl)
