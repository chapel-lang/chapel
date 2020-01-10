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

#ifndef _CHPL_LAUNCH_H_
#define _CHPL_LAUNCH_H_

#include "chpltypes.h"

//
// Defined in main_launcher.c
//
void chpl_append_to_largv(int* largc, const char*** largv, int* largv_len,
                          const char* arg);
int chpl_run_utility1K(const char *command, char *const argv[],
                       char *outbuf, int outbuflen);
int chpl_run_cmdstr(const char *commandStr, char *outbuf, int outbuflen);
void chpl_launcher_record_env_var(const char*, const char *);
char **chpl_bundle_exec_args(int argc, char *const argv[],
                             int largc, char *const largv[]);
int chpl_launch_using_fork_exec(const char* command, char * const argv1[],
                                const char* argv0);
int chpl_launch_using_exec(const char* command, char * const argv1[],
                           const char* argv0);
int chpl_launch_using_system(char* command, char* argv0);

char* chpl_get_enviro_keys(char sep);
int chpl_get_charset_env_nargs(void);
int chpl_get_charset_env_args(char *argv[]);

void chpl_compute_real_binary_name(const char* argv0);
const char* chpl_get_real_binary_wrapper(void);
const char* chpl_get_real_binary_name(void);
int chpl_launch_prep(int* c_argc, char* argv[], int32_t* c_execNumLocales);
int chpl_launcher_main(int argc, char* argv[]);

//
// Defined in launch_<CHPL_LAUNCHER>.c
//
int chpl_launch(int argc, char* argv[], int32_t numLocales);
int chpl_launch_handle_arg(int argc, char* argv[], int argNum, 
                           int32_t lineno, int32_t filename);
void chpl_launch_print_help(void);

#endif
