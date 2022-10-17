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


# generate a git-version.cpp file if one doesn't exist during the cmake
# configure step. If one exists, don't overwrite it, since that will force
# re-linking. Instead, we will wait for libdyno to pre-link, and then
# re-generate and compile the file at that time.
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/util/git-version.cpp OR NOT EXISTS ${CHPL_HOME}/compiler/main/BUILD_VERSION)
  execute_process(COMMAND ${CMAKE_SOURCE_DIR}/util/config/write-git-sha ${CMAKE_CURRENT_SOURCE_DIR}/util --build-version --chpl-home=${CHPL_HOME})
  message(STATUS "wrote git-version.cpp")
endif()
# target_sources(libdyno-obj PUBLIC git-sha-obj)
add_library(git-sha-obj OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/util/git-version.cpp)

# TODO: I tried adding the git-sha-obj here but it didn't end up working for libdyno
# that way - why not?
add_library(libdyno-obj OBJECT)
# turn on position-independent code so we can use the same
# dyno-obj for both a static and dynamic library
set_property(TARGET libdyno-obj PROPERTY POSITION_INDEPENDENT_CODE 1)

# Include the public library .h files as well as any generated .h files
target_include_directories(libdyno-obj PUBLIC
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR})

# Library code can also use headers from the lib/ directory
# but these are not to be public
target_include_directories(libdyno-obj PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# SHELL: is needed here to remove unwanted quotes from the list of arguments
# COMPILE_LANGUAGE:CXX is needed here to make sure we only set the flags
# for C++ files, not C files.
target_compile_options(libdyno-obj PUBLIC SHELL:$<$<COMPILE_LANGUAGE:CXX>:${CHPL_LLVM_COMP_ARGS}>)

add_subdirectory(immediates)
add_subdirectory(parsing)
add_subdirectory(framework)
add_subdirectory(resolution)
add_subdirectory(types)
add_subdirectory(uast)
add_subdirectory(util)

add_library(libdyno SHARED $<TARGET_OBJECTS:libdyno-obj> $<TARGET_OBJECTS:git-sha-obj>)

target_include_directories(libdyno PUBLIC
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR})

target_link_libraries(libdyno PUBLIC ${CHPL_LLVM_LINK_ARGS})
# TODO: Get printchplenv output proper so that we don't need to SHELL here
target_compile_options(libdyno PUBLIC SHELL:$<$<COMPILE_LANGUAGE:CXX>:${CHPL_LLVM_COMP_ARGS}>)

# TODO: the ideal way to cleanup would be to add ${CMAKE_CURRENT_SOURCE_DIR}/util/git-version.cpp
# to the list of BYPRODUCTS, but that creates a strong cycle between libdyno and git-sha-obj
# which is not allowed when the libraries are not static. The workaround so far is to set
# the ADDITIONAL_CLEAN_FILES property of libdyno to add git-version.cpp, but that property
# wasn't added until 3.15 and our reqs list 3.13.4
# generate the git-version.cpp and compile it to produce the .o before linking libdyno
# If .git doesn't exist, we are probably in a release tarball or Dockerfile
# and expect that the git-version.cpp file was copied into our source tree
# from their respective build process
if (EXISTS ${CMAKE_SOURCE_DIR}/.git)
  add_custom_command(TARGET libdyno
                    PRE_LINK
                    COMMAND ${CMAKE_SOURCE_DIR}/util/config/write-git-sha ${CMAKE_CURRENT_SOURCE_DIR}/util --build-version --chpl-home=${CHPL_HOME} # generate the file with git sha
                    COMMAND ${CMAKE_CXX_COMPILER} -c ${CMAKE_CURRENT_SOURCE_DIR}/util/git-version.cpp -o ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/git-sha-obj.dir/util/git-version.cpp.o # compile the generated file to a .o
                    COMMENT "Generating git-version.cpp"
                    BYPRODUCTS ${CHPL_HOME}/compiler/main/BUILD_VERSION
                    VERBATIM
                    ) # results in a .o file
endif()
# TODO: ADDITIONAL_CLEAN_FILES requires cmake v3.15 ... Can we find another way to mark these files for cleanup?
set_target_properties(libdyno PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/util/git-version.cpp)


# install libdyno
# TODO: also install headers with PUBLIC_HEADER DESTINATION <dir>
install(TARGETS libdyno
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
