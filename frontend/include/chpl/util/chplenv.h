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

} // namespace chpl

#endif
