/*
 * Copyright 2004-2015 Cray Inc.
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

#include <cstdio>
#include <cstdlib>

bool printSystemCommands = false;

int mysystem(const char* command, 
             const char* description,
             bool        ignoreStatus) {
  int status = 0;

  // Treat a '#' at the start of a line as a comment
  if (command[0] != '#') {
    status = system(command);
  }

  if (status == -1) {
    USR_FATAL("system() fork failed");

  } else if (status != 0 && ignoreStatus == false) {
    USR_FATAL(description);
  }

  return status;
}
