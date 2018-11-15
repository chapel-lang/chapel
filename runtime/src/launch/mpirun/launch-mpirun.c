/*
 * Copyright 2004-2018 Cray Inc.
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
#include "chplcgfns.h"
#include "error.h"

#define CHPL_SPMD "--spmd"

static char* mpi_num_ranks = NULL;

static char _nlbuf[16];
static char** chpl_launch_create_argv(const char *launch_cmd,
                                      int argc, char* argv[],
                                      int32_t numLocales) {
  const int maxlargc = 5 + 4;
  char *largv[maxlargc];

  int numranks;

  // TODO When MPI Comm layer is supported, support -nl>1 && CHPL_COMM!=none
  if (numLocales != 1) {
    chpl_error("For mpirun, specify number of SPMD images via --spmd <#> instead of -nl", 0, 0);
  }
  if (strcmp(CHPL_COMM, "none") != 0) {
    chpl_error("mpirun only supports CHPL_COMM=none", 0, 0);
  }
  // Get the number of ranks
  if (mpi_num_ranks == NULL) {
    numranks = 1;
  } else {
    numranks = atoi(mpi_num_ranks);
  }


  int largc = 5;
  largv[0] = (char *) launch_cmd;
  largv[1] = (char *) "-np";
  sprintf(_nlbuf, "%d", numranks);
  largv[2] = _nlbuf;
  largv[3] = (char*) "-E";
  largv[4] = chpl_get_enviro_keys(',');

  // If any of the relevant character set environment variables
  // are set, replicate the state of all of them.  This needs to
  // be done separately from the -E mechanism because the
  // launcher is written in Perl, which mangles the character
  // set environment.
  //
  // Note that if we are setting these variables, and one or more
  // of them is empty, we must set it with explicitly empty
  // contents (e.g. LC_ALL= instead of -u LC_ALL) so that the
  // Chapel launch mechanism will not overwrite it.
  char *lang = getenv("LANG");
  char *lc_all = getenv("LC_ALL");
  char *lc_collate = getenv("LC_COLLATE");
  if (lang || lc_all || lc_collate) {
    largc += 4;
    largv[5] = "env";
    if (lang == NULL)
      lang = "";
    char *lang_buf = chpl_mem_allocMany(sizeof("LANG=") + strlen(lang),
                        sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
    strcpy(lang_buf, "LANG=");
    strcat(lang_buf, lang);
    largv[6] = lang_buf;
    if (lc_all == NULL)
      lc_all = "";
    char *lc_all_buf = chpl_mem_allocMany(sizeof("LC_ALL=") + strlen(lc_all),
                        sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
    strcpy(lc_all_buf, "LC_ALL=");
    strcat(lc_all_buf, lc_all);
    largv[7] = lc_all_buf;
    if (lc_collate == NULL)
      lc_collate = "";
    char *lc_collate_buf = chpl_mem_allocMany(
                        sizeof("LC_COLLATE=") + strlen(lc_collate),
                        sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, 0);
    strcpy(lc_collate_buf, "LC_COLLATE=");
    strcat(lc_collate_buf, lc_collate);
    largv[8] = lc_collate_buf;
  }

  return chpl_bundle_exec_args(argc, argv, largc, largv);
}

int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  const char *cmd = "mpirun";

  return chpl_launch_using_exec(cmd,
                                chpl_launch_create_argv(cmd, argc, argv,
                                                        numLocales),
                                argv[0]);
}



int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, int32_t filename) {
  // handle --spmd <nrank> or --spmd=<nrank>
  if (!strcmp(argv[argNum], CHPL_SPMD)) {
    mpi_num_ranks = argv[argNum+1];
    return 2;
  } else if (!strncmp(argv[argNum], CHPL_SPMD"=", strlen(CHPL_SPMD))) {
    mpi_num_ranks = &(argv[argNum][strlen(CHPL_SPMD)+1]);
    return 1;
  }

  return 0;
}


void chpl_launch_print_help(void) {
  fprintf(stdout, "LAUNCHER FLAGS:\n");
  fprintf(stdout, "===============\n");
  fprintf(stdout, "  %s : specify number of SPMD images \n", CHPL_SPMD);
}
