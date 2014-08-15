#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "error.h"

#define LAUNCH_PATH_HELP WRAP_TO_STR(LAUNCH_PATH)
#define WRAP_TO_STR(x) TO_STR(x)
#define TO_STR(x) #x

static void add_env_options(int* argc, char** argv[]) {
  extern char** environ;
  int envc;
  int new_argc;
  char** new_argv;
  int i;

  if (environ == NULL)
    return;

  //
  // Count the number of environment entries.
  //
  for (i = 0; environ[i] != NULL; i++) ;
  envc = i;

  //
  // Create a new argv with space for -E options for the env vars.
  //
  new_argc = *argc + 2 * envc;
  new_argv = (char**) chpl_mem_allocMany(new_argc, sizeof((*argv)[0]),
                                         CHPL_RT_MD_COMMAND_BUFFER, -1, "");  

  //
  // Duplicate the old argv into the start of the new one.
  //
  memcpy(new_argv, (*argv), *argc * sizeof((*argv)[0]));

  //
  // Add a -E option for each environment variable.
  //
  for (i = 0; i < envc; i++) {
    new_argv[*argc + 2 * i + 0] = "-E";
    new_argv[*argc + 2 * i + 1] = environ[i];
  }

  //
  // Return the new argv.
  //
  *argc = new_argc;
  *argv = new_argv;
}

static char** chpl_launch_create_argv(const char *launch_cmd,
                                      int argc, char* argv[],
                                      int32_t numLocales) {
  static char nlbuf[16];
  const int largc = 3;
  char *largv[largc];

  largv[0] = (char *) launch_cmd;
  largv[1] = (char *) "-np";
  snprintf(nlbuf, sizeof(nlbuf), "%d", numLocales);
  largv[2] = nlbuf;

  {
    const char* s = getenv("GASNET_SPAWNFN");
    if (s == NULL || strcmp(s, "S") == 0)
      add_env_options(&argc, &argv);
  }

  return chpl_bundle_exec_args(argc, argv, largc, largv);
}

int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  int len = strlen(WRAP_TO_STR(LAUNCH_PATH)) + strlen("amudprun") + 1;
  char *cmd = chpl_mem_allocMany(len, sizeof(char), CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  snprintf(cmd, len, "%samudprun", WRAP_TO_STR(LAUNCH_PATH));

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
