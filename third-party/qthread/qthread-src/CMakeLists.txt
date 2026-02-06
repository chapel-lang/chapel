cmake_minimum_required(VERSION 3.12...4.1)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_C_STANDARD 11)
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build using shared libraries.")

if(CMAKE_VERSION VERSION_LESS "3.9.0")
  project(qthreads
    VERSION 1.23
    DESCRIPTION "A user-level threading library"
    LANGUAGES C ASM CXX)
else()
  project(qthreads
    VERSION 1.23
    LANGUAGES C ASM CXX)
endif()

# This must be done after `project` or it gets overridden by cmake
if(APPLE)
  set(CMAKE_OSX_DEPLOYMENT_TARGET "" CACHE STRING "" FORCE)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  add_subdirectory(test)
endif()

