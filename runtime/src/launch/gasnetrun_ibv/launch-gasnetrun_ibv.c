#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "error.h"

#define LAUNCH_PATH_HELP WRAP_TO_STR(LAUNCH_PATH)
#define WRAP_TO_STR(x) TO_STR(x)
#define TO_STR(x) #x

// TODO: Un-hard-code this stuff:

static char _nlbuf[16];
static char** chpl_launch_create_argv(const char *launch_cmd,
                                      int argc, char* argv[],
                                      int32_t numLocales) {
  const int largc = 5;
  char *largv[largc];

  largv[0] = (char *) launch_cmd;
  largv[1] = (char *) "-n";
  sprintf(_nlbuf, "%d", numLocales);
  largv[2] = _nlbuf;
  largv[3] = (char*) "-E";
  largv[4] = chpl_get_enviro_keys(',');

  return chpl_bundle_exec_args(argc, argv, largc, largv);
}


int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  int len = strlen(WRAP_TO_STR(LAUNCH_PATH)) + strlen("gasnetrun_ibv") + 1;
  char *cmd = chpl_mem_allocMany(len, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  sprintf(cmd, "%sgasnetrun_ibv", WRAP_TO_STR(LAUNCH_PATH));

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
