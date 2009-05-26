#include <stdio.h>
#include <sys/stat.h>
#include <chplio_md.h>
#include "chplcgfns.h"
#include "chplcomm_locales.h"
#include "chpllaunch.h"
#include "chpl_mem.h"
#include "chpltypes.h"
#include "error.h"


static void chpl_launch_sanity_checks(char* argv0) {
  // Do sanity checks just before launching.
  struct stat statBuf;
  char realName[256];
  int retVal;
  
  retVal = snprintf(realName, 256, "%s_real", argv0);
  if (retVal < 0 || retVal >= 256) {
    chpl_internal_error("error generating back-end filename");
  }

  // Make sure the _real binary exists
  if (stat(realName, &statBuf) != 0) {
    char errorMsg[256];
    sprintf(errorMsg, "unable to locate file: %s", realName);
    chpl_error(errorMsg, -1, "<internal>");
  }
}


void chpl_launch_using_system(char* command, char* argv0) {
  if (verbosity > 1) {
    printf("%s\n", command);
  }
  chpl_launch_sanity_checks(argv0);
  system(command);
}


int handleNonstandardArg(int* argc, char* argv[], int argNum, 
                         int32_t lineno, chpl_string filename) {
  int numHandled = chpl_launch_handle_arg(*argc, argv, argNum, 
                                          lineno, filename);
  if (numHandled == 0) {
    char* message = chpl_glom_strings(3, "Unexpected flag:  \"", argv[argNum], 
                                      "\"");
    chpl_error(message, lineno, filename);
    return 0;
  } else {
    int i;
    for (i=argNum+numHandled; i<*argc; i++) {
      argv[i-numHandled] = argv[i];
    }
    *argc -= numHandled;
    return -1;  // back the cursor up in order to re-parse this arg
  }
}


int main(int argc, char* argv[]) {
  //
  // This is a user invocation, so parse the arguments to determine
  // the number of locales.
  //
  int32_t execNumLocales;
  CreateConfigVarTable();
  parseArgs(&argc, argv);

  execNumLocales = getArgNumLocales();

  //
  // If the user did not specify a number of locales let the
  // comm layer decide how many to use (or flag an error)
  //
  if (execNumLocales == 0) {
    execNumLocales = chpl_comm_default_num_locales();
  }
  //
  // Before proceeding, allow the comm layer to verify that the
  // number of locales is reasonable
  //
  chpl_comm_verify_num_locales(execNumLocales);
  //
  // Launch the program
  //
  chpl_launch(argc, argv, execNumLocales);

  return 0;
}
