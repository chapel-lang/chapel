/*
 * Copyright 2004-2016 Cray Inc.
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

#include <stdint.h>

//
// Please see runtime/src/main_launcher.c for top-level documentation
//

//
// Utilities that are defined in main_launcher.c
// and that might be invoked during cmd-line parsing
//

int         handleNonstandardArg(int*    argc,
                                 char*   argv[],
                                 int     argNum,
                                 int32_t lineno,
                                 int32_t filename);

void        printAdditionalHelp(void);


//
// Utilities that are defined in main_launcher.c
// These are likely to be invoked by the specialized launchers
//

char**      chpl_bundle_exec_args(int         argc,
                                  char* const argv[],
                                  int         largc,
                                  char* const largv[]);

const char* chpl_get_real_binary_wrapper(void);

const char* chpl_compute_real_binary_name(const char* argv0);

const char* chpl_get_real_binary_name(void);

char*       chpl_get_enviro_keys(char sep);

int         chpl_run_utility1K(const char* command,
                               char* const argv[],
                               char        outbuf[],
                               int         outbuflen);

//
// Utilities that are defined in main_launcher.c
// One of these will be invoked to launch the node application
//

int         chpl_launch_using_fork_exec(const char* command,
                                        char* const argv1[],
                                        const char* argv0);

int         chpl_launch_using_exec(const char* command,
                                   char* const argv1[],
                                   const char* argv0);

int         chpl_launch_using_system(char* command,
                                     char* argv0);

//
// Functions that are used by main_launcher.c but that are
// specialized by each launcher
//
int         chpl_launch_handle_arg(int     argc,
                                   char*   argv[],
                                   int     argNum,
                                   int32_t lineno,
                                   int32_t filename);

void        chpl_launch_print_help(void);

int         chpl_launch(int     argc,
                        char*   argv[],
                        int32_t numLocales);

#endif
