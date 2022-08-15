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
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace chpl {


// TODO: body of this function should call llvm::sys::ExecuteAndWait instead
// see: https://llvm.org/doxygen/namespacellvm_1_1sys.html#a67688ad4697f1d516a7c71e41d78c5ba
std::pair<int, std::string>
executeAndWait(const std::vector<std::string>& commandVec,
               const std::vector<std::string>& envVec,
               const std::string& description,
               bool printSystemCommands) {
  // if an empty command or comment is passed, do nothing
  if (commandVec.empty() || commandVec[0].empty() || commandVec[0][0] == '#') {
    return std::make_pair(0, "");
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

  int pipefd[2];
  if (pipe(pipefd) != 0) {
    if (printSystemCommands) {
      printf("failed to open pipe: %s\n", strerror(errno));
    }
    return std::make_pair(-1, "");
  }
  pid_t childPid = fork();

  if (childPid == 0) {
    // in child process
    // don't need output from pipe
    close(pipefd[0]);
    // redirect stderr to stdout, and stdout to pipe
    while ((dup2(STDOUT_FILENO, STDERR_FILENO) == -1) && errno == EINTR) {};
    while ((dup2(pipefd[1], STDOUT_FILENO) == -1) && errno == EINTR) {};
    // configure environment
    for (const auto& envStr : envVec) {
      auto equalSign = envStr.find('=');
      auto varName = envStr.substr(0, equalSign);
      auto varValue = envStr.substr(equalSign+1, envStr.length()-equalSign-1);
      setenv(varName.c_str(), varValue.c_str(), 1);
    }
    execvp(execArgs[0], const_cast<char* const *> (execArgs.data()));
    // if execvp returned, there was an error
    if (printSystemCommands) {
      printf("error in exec: %s\n", strerror(errno));
    }
    // indicate problem with exec by exiting the child process
    exit(-1);
  } else if (childPid > 0 ) {
    // in parent process
    // don't need input to pipe
    close(pipefd[1]);
    int w = waitpid(childPid, &status, 0);
    if (w == -1) {
      if (printSystemCommands) {
        printf("error in waitpid: %s\n", strerror(errno));
      }
      // indicate problem with waiting
      return std::make_pair(-1, "");
    }
    // read from pipe into string
    std::ostringstream oss;
    ssize_t readBytes;
    char buffer[1024];
    while ((readBytes = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
      oss.write(buffer, readBytes);
    }
    if (readBytes < 0) {
      if (printSystemCommands) {
        printf("failed to read child output: %s\n", strerror(errno));
      }
      return std::make_pair(-1, "");
    }
    auto output = oss.str();
    if (!WIFEXITED(status)) {
      // indicate program did not exit normally
      if (printSystemCommands) {
        printf("child process did not exit normally\n");
      }
      // indicate program did not exit normally
      return std::make_pair(-1, output);
    }
    // filter status down to key bits
    int code = WEXITSTATUS(status);
    if (printSystemCommands && code != 0) {
      printf("child process exited with code %i\n", code);
    }
    return std::make_pair(code, output);
  }

  // this can be reached if the 'fork' failed
  return std::make_pair(-1, "");
}


} // namespace chpl
