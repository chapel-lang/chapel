#include "arg.h"
#include "chapel_code.h"
#include "chplcast.h"
#include "chplcgfns.h"
#include "chplcomm.h"
#include "chplexit.h"
#include "chplio.h"
#include "chpl_mem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "chplthreads.h"
#include "config.h"
#include "error.h"
#include <stdint.h>
#include <string.h>


char* chpl_executionCommand;

int chpl_threads_initialized = 0;

int handleNonstandardArg(int* argc, char* argv[], int argNum, 
                         int32_t lineno, chpl_string filename) {
  char* message = chpl_glom_strings(3, "Unexpected flag:  \"", argv[argNum], 
                                    "\"");
  chpl_error(message, lineno, filename);
  return 0;
}


static void recordExecutionCommand(int argc, char *argv[]) {
  int i, length = 0;
  for (i = 0; i < argc; i++) {
    length += strlen(argv[i]) + 1;
  }
  chpl_executionCommand = (char*)chpl_malloc(length+1, sizeof(char), CHPL_RT_EXECUTION_COMMAND, 0, 0);
  sprintf(chpl_executionCommand, "%s", argv[0]);
  for (i = 1; i < argc; i++) {
    strcat(chpl_executionCommand, " ");
    strcat(chpl_executionCommand, argv[i]);
  }
}


int main(int argc, char* argv[]) {
  int32_t execNumLocales;
  int runInGDB;

  chpl_comm_init(&argc, &argv);
  chpl_comm_init_shared_heap();

  chpl_comm_barrier("about to leave comm init code");
  chpl__heapAllocateGlobals(); // allocate global vars on heap for multilocale
  CreateConfigVarTable();      // get ready to start tracking config vars
  parseArgs(&argc, argv);
  runInGDB = _runInGDB();
  if (runInGDB) {
    int status;
    if (chpl_comm_run_in_gdb(argc, argv, runInGDB, &status)) {
      chpl_exit_all(status);
    }
  }
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
  chpl_comm_rollcall();
  chpl_initMemTable();            // get ready to start tracking memory
  chpl_init_chapel_code();

  initChplThreads();         // initialize the threads layer
  chpl_threads_initialized = 1;

  chpl__initModuleGuards();  // initialize per-locale run once guard vars
  recordExecutionCommand(argc, argv);

  if (chpl_localeID == 0) {      // have locale #0 run the user's main function
    chpl_main();
  }

  chpl_exit_all(0);         // have everyone exit
  return 0;                  // should never get here
}
