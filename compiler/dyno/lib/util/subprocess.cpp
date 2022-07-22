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


int myshell(std::string  command,
            std::string description,
            bool        ignoreStatus,
            bool        quiet,
            bool        printSystemCommands) {

  int status = 0;

  if (printSystemCommands && !quiet) {
    printf("\n# %s\n", description.c_str());
    printf("%s\n", command.c_str());
    fflush(stdout);
    fflush(stderr);
  }

  // Treat a '#' at the start of a line as a comment
  if (command[0] == '#')
    return 0;

  status = system(command.c_str());

  return status;
}

// TODO: body of this function should call llvm::sys::ExecuteAndWait instead
// see: https://llvm.org/doxygen/namespacellvm_1_1sys.html#a67688ad4697f1d516a7c71e41d78c5ba
int mysystem(const std::vector<std::string> commandVec,
             std::string description,
             pid_t&      childPidOut,
             bool        ignoreStatus,
             bool        quiet,
             bool        printSystemCommands) {

  // if an empty command passed, do nothing
  if (commandVec.empty()) {
    return 0;
  }

  // Join the elements of commandVec into a single space separated string
  std::string commandStr = commandVec.front();
  for (unsigned int i = 1; i < commandVec.size(); i++) {
    commandStr += " " + commandVec.at(i);
  }

  int status = -1;
  std::vector<const char*> execArgs;
  for (unsigned int i = 0; i < commandVec.size(); ++i) {
    execArgs.push_back(commandVec[i].c_str());
  }
  execArgs.push_back(NULL);
  execArgs.shrink_to_fit();

  if (printSystemCommands && !quiet) {
    printf("\n# %s\n", description.c_str());
    printf("%s\n", commandStr.c_str());
    fflush(stdout);
    fflush(stderr);
  }

  // Treat a '#' at the start of a line as a comment
  if (commandStr.c_str()[0] == '#')
    return 0;

  childPidOut = fork();

  if (childPidOut == 0) {
    // in child process
    execvp(execArgs[0], const_cast<char* const *> (execArgs.data()));
  } else if (childPidOut > 0 ) {
    // in parent process
    waitpid(childPidOut, &status, 0);
    // filter status down to key bits
    status = WEXITSTATUS(status);
  }
  return status;
}


} // namespace chpl
