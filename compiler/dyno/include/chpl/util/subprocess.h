/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CHPL_UTIL_SUBPROCESS_H
#define CHPL_UTIL_SUBPROCESS_H

#include <string>
#include <vector>


namespace chpl {


/**
 Launch a subprocess

 commandVec
   first element is the command to run, the rest are the arguments.

 envVec
   a list of environment variables represented as strings

 description
   a string representation of the command to run

 printSystemCommands if true, print the command to be run

 returns
   -1 if there was an error in fork, waitpid, or the child process was killed
   255 if the exec call failed
   the exit code from the subprocess (0-255) otherwise
 */
int executeAndWait(const std::vector<std::string>& commandVec,
                   const std::vector<std::string>& envVec,
                   const std::string& description,
                   bool printSystemCommands = false);

#endif


} // namespace chpl
