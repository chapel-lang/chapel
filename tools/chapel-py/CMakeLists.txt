cmake_minimum_required(VERSION 3.26)

project(
  ${SKBUILD_PROJECT_NAME}
  VERSION ${SKBUILD_PROJECT_VERSION}
  LANGUAGES CXX
  DESCRIPTION "Python bindings for Chapel's frontend library, Dyno"
)

find_package(Python REQUIRED COMPONENTS Interpreter Development.Module Development.SABIModule)

# Detect Chapel environment
message(STATUS "Detecting Chapel environment...")

# Get Chapel home directory (3 levels up from this CMakeLists.txt)
get_filename_component(CHPL_HOME "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
message(STATUS "CHPL_HOME: ${CHPL_HOME}")

# Set Chapel utility paths
set(CHPL_PRINTCHPLENV "${CHPL_HOME}/util/printchplenv")
set(CHPL_LLVM_PY "${CHPL_HOME}/util/chplenv/chpl_llvm.py")
set(CHPL_HOME_UTILS "${CHPL_HOME}/util/chplenv/chpl_home_utils.py")

# Function to execute Chapel environment commands
function(get_chapel_env_var VAR_NAME OUTPUT_VAR)
  execute_process(
    COMMAND ${CHPL_PRINTCHPLENV} --internal --all --anonymize --simple
    OUTPUT_VARIABLE CHPL_ENV_OUTPUT
    RESULT_VARIABLE CHPL_ENV_RESULT
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  if(NOT CHPL_ENV_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to get Chapel environment variables")
  endif()

  # Parse the output to find our variable
  string(REPLACE "\n" ";" CHPL_ENV_LINES "${CHPL_ENV_OUTPUT}")
  foreach(LINE ${CHPL_ENV_LINES})
    if(LINE MATCHES "^${VAR_NAME}=(.*)$")
      set(${OUTPUT_VAR} "${CMAKE_MATCH_1}" PARENT_SCOPE)
      return()
    endif()
  endforeach()

  # Variable not found
  set(${OUTPUT_VAR} "" PARENT_SCOPE)
endfunction()

# Get Chapel environment variables
get_chapel_env_var("CHPL_LLVM" CHPL_LLVM)
get_chapel_env_var("CHPL_HOST_CC" CHPL_HOST_CC)
get_chapel_env_var("CHPL_HOST_CXX" CHPL_HOST_CXX)
get_chapel_env_var("CHPL_HOST_BIN_SUBDIR" CHPL_HOST_BIN_SUBDIR)
get_chapel_env_var("CHPL_SANITIZE" CHPL_SANITIZE)
get_chapel_env_var("CHPL_HOST_PLATFORM" CHPL_HOST_PLATFORM)

message(STATUS "CHPL_LLVM: ${CHPL_LLVM}")
message(STATUS "CHPL_HOST_CC: ${CHPL_HOST_CC}")
message(STATUS "CHPL_HOST_CXX: ${CHPL_HOST_CXX}")
message(STATUS "CHPL_HOST_BIN_SUBDIR: ${CHPL_HOST_BIN_SUBDIR}")

# Set compilers if specified by Chapel
if(CHPL_HOST_CC)
    set(CMAKE_C_COMPILER "${CHPL_HOST_CC}")
endif()
if(CHPL_HOST_CXX)
    set(CMAKE_CXX_COMPILER "${CHPL_HOST_CXX}")
endif()

# Chapel library path
set(CHPL_LIB_PATH "${CHPL_HOME}/lib/compiler/${CHPL_HOST_BIN_SUBDIR}")
message(STATUS "Chapel lib path: ${CHPL_LIB_PATH}")

# Get Chapel install lib path
execute_process(
  COMMAND ${Python_EXECUTABLE} ${CHPL_HOME_UTILS} --configured-install-lib-prefix
  OUTPUT_VARIABLE CHPL_INSTALL_LIB_PATH
  RESULT_VARIABLE CHPL_INSTALL_RESULT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(CHPL_INSTALL_RESULT EQUAL 0 AND NOT CHPL_INSTALL_LIB_PATH STREQUAL "None")
  message(STATUS "Chapel install lib path: ${CHPL_INSTALL_LIB_PATH}")
else()
  set(CHPL_INSTALL_LIB_PATH "")
endif()

# Get LLVM flags
set(LLVM_CXX_FLAGS "")
if(CHPL_LLVM AND NOT CHPL_LLVM STREQUAL "none")
  execute_process(
    COMMAND ${Python_EXECUTABLE} ${CHPL_LLVM_PY} --host-cxxflags
    OUTPUT_VARIABLE LLVM_CXX_FLAGS
    RESULT_VARIABLE LLVM_FLAGS_RESULT
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  if(NOT LLVM_FLAGS_RESULT EQUAL 0)
    message(WARNING "Failed to get LLVM CXX flags")
    set(LLVM_CXX_FLAGS "")
  endif()
endif()

# Create the Python module
file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")
python_add_library(chapel_core MODULE ${CPP_SOURCES} USE_SABI 3 WITH_SOABI )

# Set the module name to match the expected Python import path
set_target_properties(chapel_core PROPERTIES
  OUTPUT_NAME "core"
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)

# Add compile definitions
if(CHPL_LLVM AND NOT CHPL_LLVM STREQUAL "none")
  target_compile_definitions(chapel_core PRIVATE HAVE_LLVM)
endif()

# Add compile options
target_compile_options(chapel_core PRIVATE
  -Wno-c99-designator
  $<$<CONFIG:Release>:-O3>
  $<$<CONFIG:Debug>:-g>
)

# Parse and add LLVM flags
if(LLVM_CXX_FLAGS)
  separate_arguments(LLVM_FLAGS_LIST UNIX_COMMAND "${LLVM_CXX_FLAGS}")
  target_compile_options(chapel_core PRIVATE ${LLVM_FLAGS_LIST})
endif()

# Add include directories
target_include_directories(chapel_core PRIVATE
  "${CHPL_HOME}/frontend/include"
  "src"
)

# Add library directories and libraries
target_link_directories(chapel_core PRIVATE "${CHPL_LIB_PATH}")
target_link_libraries(chapel_core PRIVATE ChplFrontendShared)

# Add install lib path if it exists
if(CHPL_INSTALL_LIB_PATH)
  target_link_directories(chapel_core PRIVATE "${CHPL_INSTALL_LIB_PATH}")
endif()

# Handle address sanitization
if(CHPL_SANITIZE STREQUAL "address")
  if(CHPL_HOST_PLATFORM STREQUAL "darwin")
    message(
      FATAL_ERROR
      "Cannot use chapel-py on Mac OS when address sanitization is enabled; "
      "please unset 'CHPL_SANITIZE' then rebuild Chapel"
    )
  endif()

  target_compile_options(chapel_core PRIVATE -fsanitize=address)
  target_link_options(chapel_core PRIVATE -fsanitize=address)

  if(CHPL_HOST_CC MATCHES "clang")
    target_compile_options(chapel_core PRIVATE -shared-libasan)
    target_link_options(chapel_core PRIVATE -shared-libasan)
  endif()
endif()

# Set RPATH for runtime library discovery
if(APPLE)
  set_target_properties(chapel_core PROPERTIES
    INSTALL_RPATH "@loader_path;${CHPL_LIB_PATH}"
  )
  if(CHPL_INSTALL_LIB_PATH)
    set_target_properties(chapel_core PROPERTIES
      INSTALL_RPATH "@loader_path;${CHPL_LIB_PATH};${CHPL_INSTALL_LIB_PATH}"
    )
  endif()
elseif(UNIX)
  set_target_properties(chapel_core PROPERTIES
    INSTALL_RPATH "$ORIGIN:${CHPL_LIB_PATH}"
  )
  if(CHPL_INSTALL_LIB_PATH)
    set_target_properties(chapel_core PROPERTIES
      INSTALL_RPATH "$ORIGIN:${CHPL_LIB_PATH}:${CHPL_INSTALL_LIB_PATH}"
    )
  endif()
endif()

# Enable py_limited_api for stable ABI
target_compile_definitions(chapel_core PRIVATE Py_LIMITED_API=0x03080000)

# Install the module in the correct location
install(TARGETS chapel_core DESTINATION chapel COMPONENT python)
