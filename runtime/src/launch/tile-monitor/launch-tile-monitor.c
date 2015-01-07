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

#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "error.h"

#define LAUNCH_PATH_HELP WRAP_TO_STR(LAUNCH_PATH)
#define WRAP_TO_STR(x) TO_STR(x)
#define TO_STR(x) #x

static char** chpl_launch_create_argv(const char *launch_cmd,
                                      int argc, char* argv[],
                                      int32_t numLocales) {
  static char _nlbuf[16];
  const int largc = 9;
  int len = argc + largc + 1;
  char **newargv = chpl_mem_allocMany(len, sizeof(char*),
                                      CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  if (!newargv) {
    chpl_internal_error("Could not allocate memory");
  }

  // sripping off "const" ?
  newargv[0] = (char *) launch_cmd;
  newargv[1] = (char *) "--resume";
  newargv[2] = (char *) "--root";
  newargv[3] = (char *) "--here";
  newargv[4] = (char *) "--env";
  newargv[5] = (char *) "GASNET_SPAWNFN=L";
  newargv[6] = (char *) "--";
  if (argc > 0) {
    chpl_compute_real_binary_name(argv[0]);
    newargv[7] = (char *) chpl_get_real_binary_name();
    // Our executable expects the locale count to follow the executable name.
    sprintf(_nlbuf, "%d", numLocales);
    newargv[8] = _nlbuf;
    if (argc > 1) {
      // other args (skip binary name)
      memcpy(newargv+largc, argv+1, (argc-1)*sizeof(char *));
    }
  }

  newargv[len-1] = NULL;

  return newargv;
}

int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  const char* launcher_name = "tile-monitor";
  int len = 1 + strlen(LAUNCH_PATH_HELP) + strlen(launcher_name);
  char *cmd = chpl_mem_allocMany(len, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  sprintf(cmd, "%s%s", LAUNCH_PATH_HELP, launcher_name);

  return chpl_launch_using_exec(cmd,
                                chpl_launch_create_argv(cmd, argc, argv,
                                                        numLocales),
                                argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, c_string filename) {
  return 0;
}


void chpl_launch_print_help(void) {
}
