#ifndef _CHPL_LAUNCH_H_
#define _CHPL_LAUNCH_H_

#include "chpltypes.h"

//
// Defined in main_launcher.c
//
int chpl_run_utility1K(const char *command, char *const argv[],
                       char *outbuf, int outbuflen);
char **chpl_bundle_exec_args(int argc, char *const argv[],
                             int largc, char *const largv[]);
int chpl_launch_using_fork_exec(const char* command, char * const argv1[],
                                const char* argv0);
int chpl_launch_using_exec(const char* command, char * const argv1[],
                           const char* argv0);
int chpl_launch_using_system(char* command, char* argv0);

char* chpl_get_enviro_keys(char sep);

void chpl_compute_real_binary_name(const char* argv0);
const char* chpl_get_real_binary_name(void);

//
// Defined in launch_<CHPL_LAUNCHER>.c
//
int chpl_launch(int argc, char* argv[], int32_t numLocales);
int chpl_launch_handle_arg(int argc, char* argv[], int argNum, 
                           int32_t lineno, chpl_string filename);
void chpl_launch_print_help(void);

#endif
