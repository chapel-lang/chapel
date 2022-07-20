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

#include "mysystem.h"

#include "chpl/util/subprocess.h"

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

int myshell(const char* command,
             const char* description,
             bool        ignoreStatus,
             bool        quiet) {

  int status = chpl::myshell(std::string(command), std::string(description),
                             ignoreStatus, quiet, printSystemCommands);

  if (status == -1) {
    USR_FATAL("system() fork failed: %s", strerror(errno));
  } else if (status != 0 && ignoreStatus == false) {
    USR_FATAL("%s", description);
  }

  return status;
}

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
  pid_t childPid = 0;
  int status = chpl::mysystem(commandVec, std::string(description), childPid,
                              ignoreStatus, quiet, printSystemCommands);
  if (childPid == 0) {
    // if there was an error and we shouldn't ignore them, then exit
    if (status != 0 && !ignoreStatus) {
      USR_FATAL("%s", description);
    }
  // handle the case when the child couldn't be forked
  } else if (childPid == -1) {
    USR_FATAL("fork() failed: %s", strerror(errno));
  }
  return status;
}
