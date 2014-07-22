#include "chplrt.h"

#include "arg.h"
#include "chpl_rt_utils_static.h"
#include "chplcast.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "chplio.h"
#include "chpl-main.h"
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

static const char myFilename[] = 
#ifdef CHPL_DEVELOPER
  __FILE__;
#else
  "<internal>";
#endif

chpl_main_argument chpl_gen_main_arg;

char* chpl_executionCommand;

int handleNonstandardArg(int* argc, char* argv[], int argNum, 
                         int32_t lineno, c_string filename) {

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
  chpl__heapAllocateGlobals(); // allocate global vars on heap for multilocale

  if (chpl_nodeID == 0) {
    // OK, we can create tasks now.
    chpl_task_setSerial(false);

    //
    // This just sets all of the initialization predicates to false.
    // Must occur before any other call to a chpl__init_<foo> function.
    //
    chpl__init_preInit(0, myFilename);

    // Initialize the internal modules.
    chpl__init_PrintModuleInitOrder(0, myFilename);
    chpl__init_ChapelStandard(0, myFilename);
    // Note that in general, module code can contain "on" clauses
    // and should therefore not be called before the call to
    // chpl_comm_startPollingTask().

    //
    // Permit the tasking layer to do anything it would like to now that
    // the standard modules are initialized.
    //
    CHPL_TASK_STD_MODULES_INITIALIZED();

    //
    // Call the compiler-generated main() routine
    //
    chpl_gen_main_arg.return_value = chpl_gen_main(&chpl_gen_main_arg);
  }
  else {
    //
    // On non-0 locales, just call the pre- and post-user-code hooks
    // directly.
    //
    chpl_rt_preUserCodeHook();
    chpl_rt_postUserCodeHook();
  }
}


//
// Pre-user-code hook
//
// This is called on all locales.  The call on locale 0 is made from the
// compiler-emitted code in chpl_gen_main(), right before we enter user
// code.  The call on non-0 locales is made from chpl_main(), above.
//
void chpl_rt_preUserCodeHook(void) {
  chpl_comm_barrier("pre-user-code hook begin");

  //
  // Set up any memory tracking requested.
  //
  chpl_setMemFlags();

  chpl_comm_barrier("pre-user-code hook end");
}


//
// Post-user-code hook
//
// This is called on all locales.  The call on locale 0 is made from the
// compiler-emitted code in chpl_gen_main(), right after we finish user
// code.  The call on non-0 locales is made from chpl_main(), above.
//
void chpl_rt_postUserCodeHook(void) {
  //
  // empty
  //
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

  CreateConfigVarTable();      // get ready to start tracking config vars
  chpl_gen_main_arg.argv = chpl_malloc(argc * sizeof(char*));
  chpl_gen_main_arg.argv[0] = argv[0];
  chpl_gen_main_arg.argc = 1;
  chpl_gen_main_arg.return_value = 0;
  parseArgs(&argc, argv);
  recordExecutionCommand(argc, argv);

  //
  // If the user specified a number of locales, have the comm layer
  // verify that it is reasonable.
  //
  execNumLocales = getArgNumLocales();
  if (execNumLocales != 0) {
    chpl_comm_verify_num_locales(execNumLocales);
  }

  runInGDB = _runInGDB();
  if (runInGDB) {
    int status;
    if (chpl_comm_run_in_gdb(argc, argv, runInGDB, &status)) {
      chpl_exit_all(status);
    }
  }

  //
  // Initialize the task management layer.
  //
  chpl_task_init();

  //
  // Some comm layer initialization has to wait until after the
  // tasking layer is initialized.
  //
  chpl_comm_post_task_init();
  chpl_comm_rollcall();

  //
  // Make sure the runtime is fully set up on all locales before we start
  // running Chapel code.
  //
  chpl_comm_barrier("barrier before main");

  //
  // Run the main function for this node.
  //
  chpl_task_callMain(chpl_main);

  // have everyone exit, returning the value returned by the user written main
  // or 0 if it didn't return anything
  chpl_exit_all(chpl_gen_main_arg.return_value);         
  return 0;                  // should never get here
}
