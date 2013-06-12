#include "chplrt.h"

#include "arg.h"
#include "chpl_rt_utils_static.h"
#include "chplcast.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "chplio.h"
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chpl-tasks.h"
#include "chplsys.h"
#include "config.h"
#include "error.h"

#include <stdint.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <sys.h>

extern int chpl_no_stdmodules;

static const char myFilename[] = 
#ifdef CHPL_DEVELOPER
  __FILE__;
#else
  "<internal>";
#endif

chpl_main_argument chpl_gen_main_arg;

char* chpl_executionCommand;

int handleNonstandardArg(int* argc, char* argv[], int argNum, 
                         int32_t lineno, chpl_string filename) {

  if (mainHasArgs) {
    chpl_gen_main_arg.argv[chpl_gen_main_arg.argc] = argv[argNum];
    chpl_gen_main_arg.argc++;
  } else {
    char* message = chpl_glom_strings(3, "Unexpected flag:  \"", argv[argNum], "\"");
    chpl_error(message, lineno, filename);
  }

  return 0;
}


void printAdditionalHelp(void) {
}


static void recordExecutionCommand(int argc, char *argv[]) {
  int i, length = 0;
  for (i = 0; i < argc; i++) {
    length += strlen(argv[i]) + 1;
  }
  chpl_executionCommand =
    (char*)chpl_mem_allocMany(length+1, sizeof(char),
                              CHPL_RT_MD_EXECUTION_COMMAND, 0, 0);
  sprintf(chpl_executionCommand, "%s", argv[0]);
  for (i = 1; i < argc; i++) {
    strcat(chpl_executionCommand, " ");
    strcat(chpl_executionCommand, argv[i]);
  }
}


static void chpl_main(void) {
  // OK, we can create tasks now.
  chpl_task_setSerial(false);

  // Initialize the internal modules.
  if( ! chpl_no_stdmodules ) {
    chpl__init_ChapelStandard(0, myFilename);
    // Note that in general, module code can contain "on" clauses
    // and should therefore not be called before the call to
    // chpl_comm_startPollingTask().

    //
    // Permit the tasking layer to do anything it would like to now that
    // the standard modules are initialized.
    //
    CHPL_TASK_STD_MODULES_INITIALIZED();
  }

  //
  // Call the compiler-generated main() routine
  //
  chpl_gen_main_arg.return_value = chpl_gen_main(&chpl_gen_main_arg);
}


int main(int argc, char* argv[]) {
  int32_t execNumLocales;
  int runInGDB;

  // Check that we can get the page size.
  assert( sys_page_size() > 0 );

  // Declare that we are 'locale aware' so that
  // UTF-8 functions (e.g. wcrtomb) work as
  // indicated by the locale environment variables.
  setlocale(LC_CTYPE,"");
  // So that use of localtime_r is portable.
  tzset();

  chpl_error_init();  // This does local-only initialization
  chpl_comm_init(&argc, &argv);
  chpl_mem_init();
  chpl_comm_post_mem_init();

  chpl_comm_barrier("about to leave comm init code");
  chpl__heapAllocateGlobals(); // allocate global vars on heap for multilocale
  CreateConfigVarTable();      // get ready to start tracking config vars
  chpl_gen_main_arg.argv = chpl_malloc(argc * sizeof(char*));
  chpl_gen_main_arg.argv[0] = argv[0];
  chpl_gen_main_arg.argc = 1;
  chpl_gen_main_arg.return_value = 0;
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

  //
  // This just sets all of the initialization predicates to false.
  // Must occur before any other call to a chpl__init_<foo> function.
  //
  chpl__init_preInit(0, myFilename);
 
  //
  // Initialize the task management layer.
  //
  chpl_task_init();

  //
  // Some comm layer initialization may have to be done after the
  // tasking layer is initialized.
  //
  chpl_comm_post_task_init();
  chpl_comm_rollcall();

  recordExecutionCommand(argc, argv);

  chpl_comm_barrier("barrier before main");
  // The call to chpl_comm_barrier makes sure that all locales are listening
  // before an attempt is made to run tasks "on" them.

  if (chpl_nodeID == 0) {      // have locale #0 run the user's main function

    chpl_task_callMain(chpl_main);
  }

  // have everyone exit, returning the value returned by the user written main
  // or 0 if it didn't return anything
  chpl_exit_all(chpl_gen_main_arg.return_value);         
  return 0;                  // should never get here
}
