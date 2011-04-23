#include "arg.h"
#include "chpl_rt_utils_static.h"
#include "chplcast.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "chplio.h"
#include "chpl_mem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "chpl-tasks.h"
#include "chplstm.h"
#include "config.h"
#include "error.h"
#include <stdint.h>
#include <string.h>


char* chpl_executionCommand;

int handleNonstandardArg(int* argc, char* argv[], int argNum, 
                         int32_t lineno, chpl_string filename) {
  char* message = chpl_glom_strings(3, "Unexpected flag:  \"", argv[argNum], 
                                    "\"");
  chpl_error(message, lineno, filename);
  return 0;
}


void printAdditionalHelp(void) {
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
  chpl_stm_init();                  // Initialize STM layer

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

 
  //
  // initialize the task management layer
  //
  //
  // This is an early call to initialize the ChapelThreads module so
  // that its config consts (maxThreadsPerLocale and callStackSize)
  // can be used to initialize the tasking layer.  It assumes that the
  // ChapelThreads module can be initialized multiple times without
  // harm (currently true).
  //
  chpl__init_ChapelThreads(1, "<internal>");
  //
  chpl_task_init(maxThreadsPerLocale, callStackSize); 
  chpl_init_chpl_rt_utils();

  recordExecutionCommand(argc, argv);

  chpl_comm_barrier("barrier before main");

  if (chpl_localeID == 0) {      // have locale #0 run the user's main function
    chpl_task_callMain(chpl_main);
  }

  chpl_exit_all(0);         // have everyone exit
  return 0;                  // should never get here
}
