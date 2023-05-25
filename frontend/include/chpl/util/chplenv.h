/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UTIL_CHPLENV_H
#define CHPL_UTIL_CHPLENV_H

#include <map>
#include <unordered_map>

#include "llvm/Support/ErrorOr.h"
#include "subprocess.h"

namespace chpl {

using ChplEnvMap = std::unordered_map<std::string, std::string>;

/**
  Run printchplenv and collect its output into a map.

  varMap
    additional environment variables to feed to printchplenv

  chplHome
    the CHPL_HOME environment variable, to locate printchplenv

  returns
    an error code if something went wrong while running printchplenv,
    or a map containing key-value pairs for each variable in
    printchplenv's output.
 */
llvm::ErrorOr<ChplEnvMap>
getChplEnv(const std::map<std::string, const char*>& varMap,
           const char* chplHome);
llvm::ErrorOr<ChplEnvMap>
getChplEnv(const std::unordered_map<std::string, std::string>& varMap,
           const char* chplHome);

/*
  Check if a given path might be CHPL_HOME based on it containing
  util/chplenv
*/
bool isMaybeChplHome(std::string path);

/*
  Try to locate a proper CHPL_HOME value given the `main` executable's name
  and memory address. Output variables chplHomeOut, installed, fromEnv, and
  warningMessage are used to capture probable CHPL_HOME, whether chpl appears
  to be installed, whether we got the value of CHPL_HOME from the environment var,
  and a possible warning message if CHPL_HOME and the chpl executable's location
  do not match.
*/
std::error_code findChplHome(char* argv0, void* mainAddr,
                             std::string& chplHomeOut,
                             bool& installed, bool& fromEnv,
                             std::string& warningMessage);

} // namespace chpl

#endif
