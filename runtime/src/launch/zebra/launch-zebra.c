#include <stdio.h>
#include <string.h>
#include "chpllaunch.h"
#include "chpl-mem.h"
#include "error.h"


static char** chpl_launch_create_argv(int argc, char* argv[]) {
  const int largc = 3;
  char *largv[largc];

  largv[0] = (char *) "zebra";
  largv[1] = (char *) "-fast";
  largv[2] = (char *) "-r";

  return chpl_bundle_exec_args(argc, argv, largc, largv);
}


int chpl_launch(int argc, char* argv[], int32_t numLocales) {
  if (numLocales != 1) {
    // This error should be taken care of before we get to this point
    chpl_internal_error("The XMT launcher only supports numLocales==1");
  }

  return chpl_launch_using_exec("zebra",
                                chpl_launch_create_argv(argc, argv),
                                argv[0]);
}


int chpl_launch_handle_arg(int argc, char* argv[], int argNum,
                           int32_t lineno, chpl_string filename) {
  return 0;
}


void chpl_launch_print_help(void) {
}
