/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "chplcgfns.h"
#include "chpl-env.h"
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "error.h"

#define WRAP_TO_STR(x) TO_STR(x)
#define TO_STR(x) #x

//
// we will build this command:
//   bsub ... LAUNCH_PATH/gasnetrun_ibv ... REAL_BINNAME ...
//
// bsub options used:
//  -R span[ptile=1]   seems to request just 1 process per node
//  -n NN              request NN processors (i.e. nodes?)
//  -Ip                requests an interactive session with a pseudo-terminal
//

static char _nlbuf[16];
static char** chpl_launch_create_argv(int argc, char* argv[],
                                      int32_t numLocales) {
  int len = strlen(CHPL_THIRD_PARTY) + strlen(WRAP_TO_STR(LAUNCH_PATH)) + strlen("gasnetrun_ibv") + 2;
  char *cmd = chpl_mem_allocMany(len, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  sprintf(cmd, "%s/%sgasnetrun_ibv", CHPL_THIRD_PARTY, WRAP_TO_STR(LAUNCH_PATH));

  const int maxlargc = 15 + chpl_get_charset_env_nargs();
  char *largv[maxlargc];
  
  sprintf(_nlbuf, "%d", numLocales);

  int largc = 15;
  largv[0] = (char *)"bsub";
  largv[1] = (char *)"-Ip";
  largv[2] = (char *)"-R";
  largv[3] = (char *)"span[ptile=1]";
  largv[4] = (char *)"-n";
  largv[5] = _nlbuf;
  largv[6] = cmd;
  largv[7] = (char *)"-n";
  largv[8] = _nlbuf;
  largv[9] = (char *)"-N";
  largv[10] = _nlbuf;
  largv[11] = (char *)"-c";
  largv[12] = (char *)"0";
  largv[13] = (char*)"-E";
  largv[14] = chpl_get_enviro_keys(',');
  largc += chpl_get_charset_env_args(&largv[largc]);

  return chpl_bundle_exec_args(argc, argv, largc, largv);
}


int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  chpl_env_set("BSUB_QUIET", "1", 0);
  return chpl_launch_using_exec("bsub",
                                chpl_launch_create_argv(argc, argv, numLocales),
                                argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, int32_t filename) {
  return 0;
}


void chpl_launch_print_help(void) {
}
