# chpl CMake Support

This directory contains the necessary files to support building Chapel programs using CMake.

Take the following steps to use Chapel with CMake:

1. Copy the files in this directory to your project's source directory. You may want to copy the files to a subdirectory like `cmake`, or you can just copy them to the root of your project.
2. Add `find_package(chpl REQUIRED HINTS .)` to your `CMakeLists.txt` file.

Here is an example `CMakeLists.txt` file for a minimal Hello World program:

```cmake
find_package(chpl REQUIRED HINTS .)
project(hello LANGUAGES CHPL)

add_executable(hello hello.chpl)
```
