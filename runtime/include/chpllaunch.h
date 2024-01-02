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

#ifndef _CHPL_LAUNCH_H_
#define _CHPL_LAUNCH_H_

#include "chpltypes.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// A (command-line-arg, description) tuple.  Arrays of these are
// terminated by an entry with the .arg member set to NULL.  The
// other special case is that if .arg=="" then .desc is a second
// or succeeding line of the description.  These are used in
// support of "--help" output.
//
typedef struct {
  const char* arg;
  const char* desc;
} argDescTuple_t;

//
// Defined in chpl-launcher-common.c
//
int chpl_doDryRun(void);
void chpl_append_to_largv(int* largc, const char*** largv, int* largv_len,
                          const char* arg);
int chpl_run_utility1K(const char *command, char *const argv[],
                       char *outbuf, int outbuflen);
int chpl_run_cmdstr(const char *commandStr, char *outbuf, int outbuflen);
char *chpl_find_executable(const char *prog_name);
void chpl_launcher_record_env_var(const char*, const char *);
char **chpl_bundle_exec_args(int argc, char *const argv[],
                             int largc, char *const largv[]);
int chpl_launch_using_fork_exec(const char* command, char * const argv1[],
                                const char* argv0);
int chpl_launch_using_exec(const char* command, char * const argv1[],
                           const char* argv0);
int chpl_launch_using_system(char* command, char* argv0);

char* chpl_get_enviro_keys(char sep);

void chpl_compute_real_binary_name(const char* argv0);
const char* chpl_get_real_binary_wrapper(void);
const char* chpl_get_real_binary_name(void);
int chpl_launch_prep(int* c_argc, char* argv[], int32_t* c_execNumLocales,
                     int32_t *c_execNumLocalesPerNode);
int chpl_launcher_main(int argc, char* argv[]);

void chpl_launcher_get_job_name(char* baseName, char* jobName, int jobLen);

void chpl_launcher_no_colocales_error(const char *name);
//
// Defined in launch_<CHPL_LAUNCHER>.c
//
int chpl_launch(int argc, char* argv[], int32_t numLocales,
                int32_t numLocalesPerNode);
int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, int32_t filename);
const argDescTuple_t* chpl_launch_get_help(void);

#ifdef __cplusplus
}
#endif

#endif
