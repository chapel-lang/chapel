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

#include "chpl/util/subprocess.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace chpl {


// TODO: body of this function should call llvm::sys::ExecuteAndWait instead
// see: https://llvm.org/doxygen/namespacellvm_1_1sys.html#a67688ad4697f1d516a7c71e41d78c5ba
int executeAndWait(const std::vector<std::string>& commandVec,
                   const std::string& description,
                   bool printSystemCommands) {

  // if an empty command or comment is passed, do nothing
  if (commandVec.empty() || commandVec[0].empty() || commandVec[0][0] == '#') {
    return 0;
  }

  int status = -1;
  std::vector<const char*> execArgs;
  for (const auto& elt : commandVec) {
    execArgs.push_back(elt.c_str());
  }
  execArgs.push_back(NULL);
  execArgs.shrink_to_fit();

  if (printSystemCommands) {
    // Join the elements of commandVec into a single space separated string
    std::string commandStr = commandVec.front();
    for (unsigned int i = 1; i < commandVec.size(); i++) {
      commandStr += " " + commandVec.at(i);
    }

    printf("\n# %s\n", description.c_str());
    printf("%s\n", commandStr.c_str());
    fflush(stdout);
    fflush(stderr);
  }

  pid_t childPid = fork();

  if (childPid == 0) {
    // in child process
    execvp(execArgs[0], const_cast<char* const *> (execArgs.data()));
    // if execvp returned, there was an error
    if (printSystemCommands) {
      printf("error in exec: %s\n", strerror(errno));
    }
    // indicate problem with exec by exiting the child process
    exit(-1);
  } else if (childPid > 0 ) {
    // in parent process
    int w = waitpid(childPid, &status, 0);
    if (w == -1) {
      if (printSystemCommands) {
        printf("error in waitpid: %s\n", strerror(errno));
      }
      // indicate problem with waiting
      return -1;
    }
    if (!WIFEXITED(status)) {
      // indicate program did not exit normally
      if (printSystemCommands) {
        printf("child process did not exit normally\n");
      }
      // indicate program did not exit normally
      return -1;
    }
    // filter status down to key bits
    int code = WEXITSTATUS(status);
    if (printSystemCommands && code != 0) {
      printf("child process exited with code %i\n", code);
    }
    return code;
  }

  // this can be reached if the 'fork' failed
  return -1;
}

llvm::ErrorOr<std::string> getCommandOutput(const std::string& command) {
  // Run arbitrary command and return result
  char buffer[256];
  std::string result = "";

  // Call command
  FILE* pipe = popen(command.c_str(), "r");
  if (!pipe) {
    return std::error_code(errno, std::system_category());
  }

  // Read output of command into result via buffer
  while (!feof(pipe)) {
    if (fgets(buffer, 256, pipe) != NULL) {
      result += buffer;
    }
  }

  if (pclose(pipe)) {
    return std::error_code(errno, std::system_category());
  }

  return result;
}

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

llvm::ErrorOr<ChplEnvMap>
getChplEnv(const std::map<std::string, const char*>& varMap,
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

} // namespace chpl
