cmake_minimum_required(VERSION 3.4)


# Detect LLVM
# The user is supposed to set this to a valid llvm 3.7 install root
set(LLVM_ROOT "" CACHE PATH "Root of LLVM install.")
# and source tree
# (we use lit.py from the LLVM source tree)
set(LLVM_SRC "" CACHE PATH "Root of LLVM source tree.")

set(LLVM_LIT "${LLVM_SRC}/utils/lit/lit.py")

# sanity check LLVM install path
if(NOT EXISTS "${LLVM_ROOT}/include/llvm" )
    message(FATAL_ERROR
            "LLVM_ROOT (${LLVM_ROOT}) is invalid")
endif()


# sanity check LLVM src path
if(NOT EXISTS "${LLVM_SRC}/lib/Transforms" )
    message(FATAL_ERROR
            "LLVM_SRC (${LLVM_SRC}) is invalid")
endif()

# find Python
find_package(PythonInterp)

#
#set( LLVM_DIR "${LLVM_ROOT}/share/llvm/cmake" )
#set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${LLVM_DIR} )
#find_package(LLVM)
#include(AddLLVM)
#add_definitions(${LLVM_DEFINITIONS})
#include_directories(${LLVM_INCLUDE_DIRS})
#link_directories(${LLVM_LIBRARY_DIRS})
# Load various LLVM config stuff,
# see http://llvm.org/docs/CMake.html#developing-llvm-passes-out-of-source

# Load LLVM CMake config
#list(APPEND CMAKE_PREFIX_PATH "${LLVM_ROOT}/share/llvm/cmake")
#set( LLVM_DIR "${LLVM_ROOT}/share/llvm/cmake" )
#set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${LLVM_DIR} )
find_package(LLVM REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(HandleLLVMOptions) # load additional config
include(AddLLVM) # used to add our own modules

# propagate LLVM-specific variables to this project
add_definitions(${LLVM_DEFINITIONS} -DHAVE_LLVM)
include_directories(${LLVM_INCLUDE_DIRS})
# See commit r197394, needed by add_llvm_module in llvm/CMakeLists.txt
set(LLVM_RUNTIME_OUTPUT_INTDIR "${CMAKE_BINARY_DIR}/bin/${CMAKE_CFG_INT_DIR}")
set(LLVM_LIBRARY_OUTPUT_INTDIR "${CMAKE_BINARY_DIR}/lib/${CMAKE_CFG_INT_DIR}")

set(SOURCES
      llvmAggregateGlobalOps.cpp
      llvmGlobalToWide.cpp
      llvmUtil.cpp
   )

add_llvm_loadable_module( llvm-pgas ${SOURCES} )
#set_target_properties( llvm-pgas PROPERTIES COMPILE_FLAGS "-fno-rtti" )

# set various configuration settings in the test suite
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/test/lit.cfg.in" "${CMAKE_CURRENT_BINARY_DIR}/test/lit.cfg")

file( GLOB_RECURSE test_files RELATIVE
      "${CMAKE_CURRENT_SOURCE_DIR}/" "test/*.ll" )
foreach( test_file ${test_files} ) 
  # test_file is e.g. test/a.ll
  # copy the test to the build directory
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/${test_file}"
    "${CMAKE_CURRENT_BINARY_DIR}/${test_file}" COPYONLY)
endforeach( test_file )

if(NOT EXISTS ${LLVM_ROOT}/bin/FileCheck)
    message(FATAL_ERROR "need FileCheck installed to run tests; configure LLVM with -DLLVM_INSTALL_UTILS")
endif()

# support make check with the LLVM tester lit in the tests directory
add_custom_target(check
    COMMAND ${PYTHON_EXECUTABLE} ${LLVM_LIT}
            "${CMAKE_CURRENT_BINARY_DIR}/test/" -v
            DEPENDS llvm-pgas 
)

