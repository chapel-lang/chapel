#include "arg.h"
#include "chpl_rt_utils_static.h"
#include "chplcast.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "chplio.h"
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chplrt.h"
#include "chpl-tasks.h"
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
  chpl_executionCommand = (char*)chpl_mem_allocMany(length+1, sizeof(char), CHPL_RT_EXECUTION_COMMAND, 0, 0);
  sprintf(chpl_executionCommand, "%s", argv[0]);
  for (i = 1; i < argc; i++) {
    strcat(chpl_executionCommand, " ");
    strcat(chpl_executionCommand, argv[i]);
  }
}


int main(int argc, char* argv[]) {
  int32_t execNumLocales;
  int runInGDB;
  int numPollingTasks;

  chpl_comm_init(&argc, &argv);
  chpl_mem_init();
  chpl_comm_post_mem_init();

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
  // This just sets all of the initialization predicates to false.
  // Must occur before any other call to a chpl__init_<foo> function.
  //
  chpl__init_preInit(1, "<internal>");
 
  //
  // initialize the task management layer
  //
  //
  // This is an early call to initialize the ChapelThreads module so
  // that its config consts (numThreadsPerLocale and callStackSize)
  // can be used to initialize the tasking layer.  
  //
  chpl__init_ChapelThreads(1, "<internal>");
  //
  numPollingTasks = chpl_comm_numPollingTasks();
  if (numPollingTasks != 0 && numPollingTasks != 1) {
    chpl_internal_error("chpl_comm_numPollingTasks() returned illegal value");
  }
  chpl_task_init(numThreadsPerLocale, chpl__maxThreadsPerLocale, 
                 numPollingTasks, callStackSize); 
  // We need to initialize the runtime support 
  // before calling chpl_task_callMain().
  chpl__init_DefaultRectangular(1, "<internal>");   // for defaultDist.
  chpl__init_ChapelRT(1, "<internal>");

  //
  // start communication tasks as necessary
  //
  chpl_comm_startPollingTask();

  recordExecutionCommand(argc, argv);

  chpl_comm_barrier("barrier before main");

  if (chpl_localeID == 0) {      // have locale #0 run the user's main function
    chpl_task_callMain(chpl_main);
  }

  chpl_exit_all(0);         // have everyone exit
  return 0;                  // should never get here
}
