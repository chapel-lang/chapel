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

add_executable(chpldef
    chpldef.cpp
    command-line-flags.cpp
    Logger.cpp
    Message.cpp
    message-compute.cpp
    misc.cpp
    protocol-types.cpp
    Server.cpp
    Transport.cpp)
set_property(TARGET chpldef PROPERTY CXX_STANDARD 17)
target_link_libraries(chpldef ChplFrontend)
target_include_directories(chpldef PRIVATE
                           ${CHPL_MAIN_INCLUDE_DIR}
                           ${CHPL_INCLUDE_DIR}
                           ${CMAKE_CURRENT_SOURCE_DIR})
