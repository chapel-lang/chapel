/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#include <stdio.h>
#include <string.h>
#include "chplcgfns.h"
#include "chpllaunch.h"
#include "chpl-env.h"
#include "chpl-mem.h"
#include "error.h"

#ifndef GASNETRUN_LAUNCHER
#error GASNETRUN_LAUNCHER must be defined
#endif

#define LAUNCH_PATH_HELP WRAP_TO_STR(LAUNCH_PATH)
#define WRAP_TO_STR(x) TO_STR(x)
#define TO_STR(x) #x

#define CHPL_LPN_VAR "LOCALES_PER_NODE"

static char _nlbuf[16];
static char _nnbuf[16];

static char** chpl_launch_create_argv(const char *launch_cmd,
                                      int argc, char* argv[],
                                      int32_t numLocales,
                                      int32_t numLocalesPerNode) {
  const int maxlargc = 9;
  char *largv[maxlargc];

  int32_t numNodes = (numLocales + numLocalesPerNode - 1) / numLocalesPerNode;

  snprintf(_nlbuf, sizeof(_nlbuf), "%d", numLocales);
  snprintf(_nnbuf, sizeof(_nnbuf), "%d", numNodes);

  int largc = 9;
  largv[0] = (char *) launch_cmd;
  largv[1] = (char *) "-n";
  largv[2] = _nlbuf;
  largv[3] = (char *) "-N";
  largv[4] = _nnbuf;
  largv[5] = (char *) "-c";
  largv[6] = (char *) "0";
  largv[7] = (char*) "-E";
  largv[8] = chpl_get_enviro_keys(',');

  return chpl_bundle_exec_args(argc, argv, largc, largv);
}

int chpl_launch(int argc, char* argv[], int32_t numLocales,
                int32_t numLocalesPerNode) {

  int len = strlen(CHPL_THIRD_PARTY) + strlen(WRAP_TO_STR(LAUNCH_PATH)) + strlen(GASNETRUN_LAUNCHER) + 2;
  char *cmd = chpl_mem_allocMany(len, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
  snprintf(cmd, len, "%s/%s%s", CHPL_THIRD_PARTY, WRAP_TO_STR(LAUNCH_PATH), GASNETRUN_LAUNCHER);

  return chpl_launch_using_exec(cmd,
                                chpl_launch_create_argv(cmd, argc, argv,
                                                        numLocales, numLocalesPerNode),
                                argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, int32_t filename) {
  return 0;
}


const argDescTuple_t* chpl_launch_get_help(void) {
  return NULL;
}
