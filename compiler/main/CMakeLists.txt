# Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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
set(SRCS
    arg.cpp
    checks.cpp
    commonFlags.cpp
    config.cpp
    driver.cpp
    log.cpp
    PhaseTracker.cpp
    runpasses.cpp
    version.cpp
   )
target_sources(chpl
               PRIVATE
               ${SRCS})
get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set_property(SOURCE ${SRCS}
             TARGET_DIRECTORY chpl
             PROPERTY COMPILE_DEFINITIONS COMPILER_SUBDIR=${DIR_NAME}
             APPEND)
