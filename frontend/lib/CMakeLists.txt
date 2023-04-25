# Copyright 2021-2023 Hewlett Packard Enterprise Development LP
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


# generate a git-version.cpp file during the cmake configure step.
# If one exists, the write-git-sha script will only update it if the
# values are different.
# If CHPL_DONT_BUILD_SHA is set in the ENV, the git-sha will always be xxxxxxxxxx
find_package(Git)
if (EXISTS ${CMAKE_SOURCE_DIR}/.git AND Git_FOUND)
  execute_process(COMMAND ${CHPL_CMAKE_PYTHON}
                          ${CMAKE_SOURCE_DIR}/util/config/write-git-sha
                          ${CMAKE_CURRENT_SOURCE_DIR}/util
                          --build-version
                          --chpl-home=${CHPL_HOME}
                          )
  message(VERBOSE "wrote git-version.cpp")
elseif(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/util/git-version.cpp)
# if we are not in a git repo, or the git binary doesn't exist on this machine,
# write a git-version.cpp file (if one doesn't exist) as if CHPL_DONT_BUILD_SHA
# was set so we dont try to execute the git command
  execute_process(COMMAND ${CMAKE_COMMAND} -E env CHPL_DONT_BUILD_SHA=1
                          ${CHPL_CMAKE_PYTHON}
                          ${CMAKE_SOURCE_DIR}/util/config/write-git-sha
                          ${CMAKE_CURRENT_SOURCE_DIR}/util
                          --build-version
                          --chpl-home=${CHPL_HOME})
  message(VERBOSE "wrote git-version.cpp with dummy sha")
endif()

add_library(git-sha-obj OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/util/git-version.cpp)
# turn on position-independent code so we can use the same
# git-sha-obj for both a static and dynamic library
set_property(TARGET git-sha-obj PROPERTY POSITION_INDEPENDENT_CODE 1)

add_library(ChplFrontend-obj OBJECT)
# turn on position-independent code so we can use the same
# dyno-obj for both a static and dynamic library
set_property(TARGET ChplFrontend-obj PROPERTY POSITION_INDEPENDENT_CODE 1)

# Include the public library .h files as well as any generated .h files
target_include_directories(ChplFrontend-obj PUBLIC
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR})

# Library code can also use headers from the lib/ directory
# but these are not to be public
target_include_directories(ChplFrontend-obj PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# SHELL: is needed here to remove unwanted quotes from the list of arguments
# COMPILE_LANGUAGE:CXX is needed here to make sure we only set the flags
# for C++ files, not C files.
target_compile_options(ChplFrontend-obj PUBLIC
                       SHELL:$<$<COMPILE_LANGUAGE:CXX>:${CHPL_LLVM_COMP_ARGS}>)

add_subdirectory(immediates)
add_subdirectory(parsing)
add_subdirectory(framework)
add_subdirectory(resolution)
add_subdirectory(types)
add_subdirectory(uast)
add_subdirectory(util)

# Toggle static or dynamic build of libChplFrontend based on LLVM
# being static or dynamic to avoid issues where libChplFrontend.so
# relies on symbols only statically linked with it in 'chpl' (rather
# than things it depends upon).
if (CHPL_LLVM_STATIC_DYNAMIC STREQUAL "static")
  add_library(ChplFrontend STATIC
              $<TARGET_OBJECTS:ChplFrontend-obj>
              $<TARGET_OBJECTS:git-sha-obj>)
else()
  add_library(ChplFrontend SHARED
              $<TARGET_OBJECTS:ChplFrontend-obj>
              $<TARGET_OBJECTS:git-sha-obj>)
endif()

target_include_directories(ChplFrontend PUBLIC
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR})

if (CHPL_LLVM_STATIC_DYNAMIC STREQUAL "static")
  if(APPLE)
    if (CHPL_LLVM_VERSION VERSION_LESS 14.0)
      # we see problems with statically linking and using PUBLIC
      # here because we are building a shared object & that would
      # include some of the LLVM libraries. But this pattern seems
      # to work OK with LLVM 14.
      message(FATAL_ERROR "LLVM >= 14.0 required for MacOS builds")
    endif()
    # link the LLVM dependencies with the generated .dylib
    # (we get linker errors on Mac OS X here if it uses INTERFACE)
    # an alternative would be to add link option -undefined dynamic_lookup
    target_link_libraries(ChplFrontend PUBLIC ${CHPL_LLVM_LINK_ARGS})
  elseif(CYGWIN)
    # (we also get linker errors on Cygwin if it uses INTERFACE here)
    target_link_libraries(ChplFrontend PUBLIC ${CHPL_LLVM_LINK_ARGS})
  else()
    # Using PUBLIC here causes problems in some configurations with LLVM<14
    # when the LLVM libraries are included in the .so generated here
    # and also linked in statically with the resulting executable.
    # With INTERFACE, the LLVM libraries are only linked with at
    # the time that this library is used.
    target_link_libraries(ChplFrontend INTERFACE ${CHPL_LLVM_LINK_ARGS})
  endif()
else()
    # Make the ChplFrontend library depend on LLVM
    # PUBLIC is simpler, more portable, and makes more sense when
    # dynamically linking with LLVM.
    target_link_libraries(ChplFrontend PUBLIC ${CHPL_LLVM_LINK_ARGS})
endif()

# TODO: Get printchplenv output proper so that we don't need to SHELL here
target_compile_options(ChplFrontend PUBLIC
                       SHELL:$<$<COMPILE_LANGUAGE:CXX>:${CHPL_LLVM_COMP_ARGS}>)


# TODO: ADDITIONAL_CLEAN_FILES requires cmake v3.15 ...
#       Can we find another way to mark these files for cleanup?
set_target_properties(ChplFrontend PROPERTIES
                      ADDITIONAL_CLEAN_FILES
                      ${CMAKE_CURRENT_SOURCE_DIR}/util/git-version.cpp)


# install ChplFrontend
# TODO: also install headers with PUBLIC_HEADER DESTINATION <dir>
if (INSTALLATION_MODE STREQUAL "prefix")
  install(TARGETS ChplFrontend
          LIBRARY DESTINATION
          "lib/chapel/${CHPL_MAJOR_VERSION}.${CHPL_MINOR_VERSION}/compiler")
else()
  install(TARGETS ChplFrontend
          LIBRARY DESTINATION
          "lib/compiler/${CHPL_HOST_BIN_SUBDIR}")
endif()
