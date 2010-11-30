#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <comm_printf_macros.h>
#include "chplcgfns.h"
#include "chplcomm_locales.h"
#include "chpllaunch.h"
#include "chpl_mem.h"
#include "chpltypes.h"
#include "error.h"


static void chpl_launch_sanity_checks(const char* argv0) {
  // Do sanity checks just before launching.
  struct stat statBuf;

  // Make sure the _real binary exists
  // (this should be called after someone has called
  // chpl_compute_real_binary_name() )
  if (stat(chpl_get_real_binary_name(), &statBuf) != 0) {
    char errorMsg[256];
    sprintf(errorMsg, "unable to locate file: %s", chpl_get_real_binary_name());
    chpl_error(errorMsg, -1, "<internal>");
  }
}

//
// This function returns a NULL terminated argv list as required by
// chpl_launch_using_exec(), i.e., execve(3).
//
//     argc, argv: as passed to main()
//     largc, largv: launcher command and args
//
char** chpl_bundle_exec_args(int argc, char *const argv[],
                              int largc, char *const largv[]) {
  int len = argc+largc+1;
  char **newargv = chpl_malloc(len, sizeof(char*),
                               CHPL_RT_MD_COMMAND_BUFFER, -1, "");
  if (!newargv) {
    chpl_error("Could not allocate memory", -1, "<internal>");
  }

  chpl_compute_real_binary_name(argv[0]);

  newargv[len-1] = NULL;

  if (largc > 0) {
    // launcher args
    memcpy(newargv, largv, largc*sizeof(char *));
  }
  // binary
  newargv[largc] = (char *) chpl_get_real_binary_name();
  if (argc > 1) {
    // other args (skip binary name)
    memcpy(newargv+largc+1, argv+1, (argc-1)*sizeof(char *));
  }

  return newargv;
}

//
// This function calls execvp(3)
//
int chpl_launch_using_exec(const char* command, char * const argv1[], const char* argv0) {
  if (verbosity > 1) {
    char * const *arg;
    printf("%s ", command);
    fflush(stdout);
    for (arg = argv1+1; *arg; arg++) {
      printf(" %s", *arg);
      fflush(stdout);
    }
    printf("\n");
  }
  chpl_launch_sanity_checks(argv0);

  execvp(command, argv1);
  {
    char msg[256];
    sprintf(msg, "execvp() failed: %s", strerror(errno));
    chpl_error(msg, -1, "<internal>");
  }
  return -1;
}

int chpl_launch_using_system(char* command, char* argv0) {
  if (verbosity > 1) {
    printf("%s\n", command);
  }
  chpl_launch_sanity_checks(argv0);
  return system(command);
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


void printAdditionalHelp(void) {
  chpl_launch_print_help();
}


static char chpl_real_binary_name[256];

void chpl_compute_real_binary_name(const char* argv0) {

  int retVal;
  const char* default_real_suffix = "_real";
  const char* real_suffix = getenv("CHPL_LAUNCHER_SUFFIX");
  
  if (NULL == real_suffix) {
    real_suffix = default_real_suffix;
  }
  
  retVal = snprintf(chpl_real_binary_name, 256, "%s%s", argv0, real_suffix);

  if (retVal < 0 || retVal >= 256) {
    chpl_internal_error("error generating back-end filename");
  }
}

const char* chpl_get_real_binary_name(void) {
  return &chpl_real_binary_name[0];
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
  // This may not return (e.g., if calling chpl_launch_using_exec())
  //
  return chpl_launch(argc, argv, execNumLocales);
}
