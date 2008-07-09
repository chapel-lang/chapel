#include <stdio.h>
#include "chplcgfns.h"
#include "chplcomm_locales.h"
#include "chpllaunch.h"
#include "chplmem.h"
#include "chpltypes.h"
#include "error.h"
/*
#include "arg.h"
*/


int main(int argc, char* argv[]) {
  //
  // This is a user invocation, so parse the arguments to determine
  // the number of locales.
  //
  int32_t execNumLocales;
  whichMalloc = 1;
  parseArgs(argc, argv);
  //  initMemTable();
  CreateConfigVarTable();

  execNumLocales = getArgNumLocales();

  //
  // If the user did not specify a number of locales let the
  // comm layer decide how many to use (or flag an error)
  //
  if (execNumLocales == 0) {
    execNumLocales = _chpl_comm_default_num_locales();
  }
  //
  // Before proceeding, allow the comm layer to verify that the
  // number of locales is reasonable
  //
  _chpl_comm_verify_num_locales(execNumLocales);

  {
    const char* command = chpl_launch_create_command(argc, argv, execNumLocales);

    if (verbosity > 1) {
      printf("%s\n", command);
    }
    system(command);
  }

  return 0;
}
