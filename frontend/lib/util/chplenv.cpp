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
#include "chpl/util/chplenv.h"

#include "chpl/util/subprocess.h"

namespace chpl {

static void parseChplEnv(std::string& output, ChplEnvMap& into) {
  // Lines
  std::string line= "";
  std::string lineDelimiter = "\n";
  size_t linePos = 0;        // Line break position

  // Tokens
  std::string tokenDelimiter = "=";
  size_t delimiterPos = 0;    // Position of delimiter
  size_t valuePos = 0;        // Position of value

  std::string key = "";
  std::string value = "";

  while ((linePos = output.find(lineDelimiter)) != std::string::npos)
  {
    line = output.substr(0, linePos);

    // Key is substring up until "=" on a given line
    delimiterPos = line.find(tokenDelimiter);
    key = line.substr(0, delimiterPos);

    // Value is substring after "=" on a given line
    valuePos = delimiterPos + tokenDelimiter.length();
    value = line.substr(valuePos);

    into[key] = value;
    output.erase(0, linePos + lineDelimiter.length());
  }
}

template <typename InputMap>
llvm::ErrorOr<ChplEnvMap>
getChplEnvImpl(const InputMap& varMap,
           const char* chplHome) {
  // Run printchplenv script, passing currently known CHPL_vars as well
  std::string command;

  // Pass known variables in varMap into printchplenv by prepending to command
  for (auto& ii : varMap)
    command += ii.first + "=" + ii.second + " ";

  command += "CHPLENV_SKIP_HOST=true ";
  command += "CHPLENV_SUPPRESS_WARNINGS=true ";
  command += std::string(chplHome) + "/util/printchplenv --all --internal --no-tidy --simple";

  auto commandOutput = getCommandOutput(command);
  if (auto err = commandOutput.getError()) {
    // forward error code
    return err;
  }
  ChplEnvMap result;
  parseChplEnv(commandOutput.get(), result);
  return result;
}

llvm::ErrorOr<ChplEnvMap>
getChplEnv(const std::map<std::string, const char*>& varMap,
           const char* chplHome) {
  return getChplEnvImpl(varMap, chplHome);
}

llvm::ErrorOr<ChplEnvMap>
getChplEnv(const std::unordered_map<std::string, std::string>& varMap,
           const char* chplHome) {
  return getChplEnvImpl(varMap, chplHome);
}

} // namespace chpl
