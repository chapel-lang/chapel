/*
 * Copyright 2004-2020 Cray Inc.
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

//
// These are utility functions for the PALS-based launchers.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "chpltypes.h"
#include "error.h"

#include "pals-utils.h"


static
int getCoresPerLocale(void) {
  int numCores = -1;
  char* numCoresString = getenv("CHPL_LAUNCHER_CORES_PER_LOCALE");

  if (numCoresString != NULL) {
    numCores = atoi(numCoresString);
    if (numCores <= 0)
      chpl_warning("CHPL_LAUNCHER_CORES_PER_LOCALE set to invalid value.",
                   0, 0);
  }

  return (numCores > 0) ? numCores : 0;
}


static
char *getNodeList(void) {
  const char *nodeList = getenv("CHPL_LAUNCHER_NODELIST");
  char *buf = NULL;
  if (nodeList) {
    buf = chpl_mem_alloc(strlen(nodeList) + 1,
                         CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
    strcpy(buf, nodeList);
  }
  return buf;
}


static
char* int_str(int val) {
    size_t buf_len = 20 + 1;
    char* buf = chpl_mem_alloc(buf_len, CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
    if (snprintf(buf, buf_len, "%d", val) >= buf_len) {
        chpl_error("int_str buffer overflow", 0, 0);
    }
    return buf;
}


char** chpl_create_pals_cmd(int argc, char* argv[], int32_t numLocales,
                            const char* ccArg) {
  int largc = 0;
  const char **largv = NULL;
  int largv_len = 0;
#define APPEND_LARGV(arg) chpl_append_to_largv(&largc, &largv, &largv_len, arg)

  APPEND_LARGV("cray");
  APPEND_LARGV("mpiexec");

  if (verbosity < 2) {
    APPEND_LARGV("--quiet");            // quiet
  }

  APPEND_LARGV("-n");                   // num processes
  APPEND_LARGV(int_str(numLocales));

  APPEND_LARGV("--ppn");                // num processes per node
  APPEND_LARGV(int_str(1));

  int numCoresPerLocale;
  if ((numCoresPerLocale = getCoresPerLocale()) > 0) {
    APPEND_LARGV("--depth");            // num CPUs per process
    APPEND_LARGV(int_str(numCoresPerLocale));
  }

  if (ccArg != NULL) {
    APPEND_LARGV("--cpu-bind");         // CPU binding
    APPEND_LARGV(ccArg);
  }

  char *nodeList;
  if ((nodeList = getNodeList()) != NULL) {
    APPEND_LARGV("--hosts");            // list of compute nodes
    APPEND_LARGV(nodeList);
  }

  // craycli arg parser needs a marker at end of system launcher opts
  APPEND_LARGV("--");

  return chpl_bundle_exec_args(argc, argv, largc, (char* const *) largv);
}
