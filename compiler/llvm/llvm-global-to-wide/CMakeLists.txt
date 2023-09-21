cmake_minimum_required(VERSION 3.20.0)
project(llvm-pgas)

# assumes LLVM 15 is installed system-wide
find_package(LLVM 15 REQUIRED)

if(LLVM_VERSION VERSION_LESS 15)
  message(FATAL_ERROR "Did not find LLVM 15 -- found ${LLVM_VERSION}")
endif()

# assumes ~/llvm-project has an LLVM 15 source checkout
set(LLVM_LIT "~/llvm-project/llvm/utils/lit/lit.py")

# find Python
find_package(PythonInterp)

list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

include(LLVM-Config)
include(HandleLLVMOptions)
include(AddLLVM)

include_directories(${LLVM_INCLUDE_DIRS})

# no interesting code unless we define this
add_definitions(-DHAVE_LLVM)

add_llvm_pass_plugin(llvm-pgas MODULE
    llvmAggregateGlobalOps.cpp
    llvmGlobalToWide.cpp
    llvmPgasPlugin.cpp
    llvmUtil.cpp
)

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

#if(NOT EXISTS ${LLVM_ROOT}/bin/FileCheck)
#    message(FATAL_ERROR "need FileCheck installed to run tests; configure LLVM with -DLLVM_INSTALL_UTILS")
#endif()


# support make check with the LLVM tester lit in the tests directory
add_custom_target(check
    COMMAND ${PYTHON_EXECUTABLE} ${LLVM_LIT}
            "${CMAKE_CURRENT_BINARY_DIR}/test/" -v
            DEPENDS llvm-pgas )

