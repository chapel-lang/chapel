/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#include "mysystem.h"

#include "misc.h"

#include "stringutil.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

bool printSystemCommands = false;

int mysystem(const char* command,
             const char* description,
             bool        ignoreStatus,
             bool        quiet) {
  std::vector<std::string> commandVec;
  std::string commandStr = command;
  splitStringWhitespace(commandStr, commandVec);
  return mysystem(commandVec, description, ignoreStatus, quiet);
}


int mysystem(const std::vector<std::string> commandVec,
             const char* description,
             bool        ignoreStatus,
             bool        quiet) {

  // if an empty command passed, do nothing
  if (commandVec.empty()) {
    return 0;
  }

  // Join the elements of commandVec into a single space separated string
  std::string commandStr = commandVec.front();
  for (unsigned int i = 1; i < commandVec.size(); i++) {
    commandStr += " " + commandVec.at(i);
  }

  int status = 0;
  std::vector<const char*> execArgs;
  for (unsigned int i = 0; i < commandVec.size(); ++i)
      execArgs.push_back(commandVec[i].c_str());
  execArgs.push_back(NULL);
  execArgs.shrink_to_fit();

  if (printSystemCommands && !quiet) {
    printf("\n# %s\n", description);
    printf("%s\n", commandStr.c_str());
    printf("%s", execArgs[0]);
    fflush(stdout);
    fflush(stderr);
  }

  // Treat a '#' at the start of a line as a comment
  if (commandStr.c_str()[0] == '#')
    return 0;



  pid_t childPid;

  childPid = fork();

  if (childPid == 0) {
    // in child process
    execvp(execArgs[0], const_cast<char* const *> (execArgs.data()));
  } else if (childPid > 0 ) {
    // in parent process
    waitpid(childPid, &status, 0);
  // uh-oh cases below
  } else if (childPid == -1) {
    USR_FATAL("fork() failed: %s", strerror(errno));
  } else if (childPid != 0 && !ignoreStatus) {
    USR_FATAL("%s", description);
  }
  return WEXITSTATUS(status);
}
  //status = exec(execArgs.data());

  //status = system(commandStr.c_str());

  // if (status == -1) {
  //   USR_FATAL("system() fork failed: %s", strerror(errno));

  // } else if (status != 0 && ignoreStatus == false) {
  //   USR_FATAL("%s", description);
  // }

  // return status;
// }
