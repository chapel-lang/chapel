/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

bool printSystemCommands = false;

int mysystem(const char* command, 
             const char* description,
             bool        ignoreStatus,
             bool        quiet) {
  int status = 0;

  if (printSystemCommands && !quiet) {
    printf("\n# %s\n", description);
    printf("%s\n", command);
    fflush(stdout);
    fflush(stderr);
  }

  // Treat a '#' at the start of a line as a comment
  if (command[0] == '#')
    return 0;

  status = system(command);

  if (status == -1) {
    USR_FATAL("system() fork failed: %s", strerror(errno));

  } else if (status != 0 && ignoreStatus == false) {
    USR_FATAL(description);
  }

  return status;
}
