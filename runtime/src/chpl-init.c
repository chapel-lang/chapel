/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "chplrt.h"

#include "arg.h"
#include "chpl_rt_utils_static.h"
#include "chplcast.h"
#include "chplcgfns.h"
#include "chpl-cache.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "chplio.h"
#include "chpl-init.h"
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chpl-privatization.h"
#include "chpl-tasks.h"
#include "chpl-topo.h"
#include "chpl-linefile-support.h"
#include "chplsys.h"
#include "config.h"
#include "error.h"

#include <stdint.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <sys.h>

static const int32_t myFilename = CHPL_FILE_IDX_INTERNAL;

chpl_main_argument chpl_gen_main_arg;

char* chpl_executionCommand;

int handleNonstandardArg(int* argc, char* argv[], int argNum, 
                         int32_t lineno, int32_t filename) {

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


//
// Pre-user-code hook
//
// This is called on all locales.  The call on locale 0 is made from the
// compiler-emitted code in chpl_gen_main(), right before we enter user
// code.  The call on non-0 locales is made from chpl_main(), above.
//
void chpl_rt_preUserCodeHook(void) {
  chpl_comm_barrier("pre-user-code hook begin");

  chpl_taskRunningCntReset(0, 0);
  if (chpl_nodeID == 0) {
    chpl_taskRunningCntInc(0, 0);
  }

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


//
// Chapel runtime initialization
//
// Factored out of "int main(int argc, char* argv[])" for the purpose
// re-using it when in "library-mode".
//
// Called from main.c:main(...) and chpl-init.c:chpl_library_init(...)
//
void chpl_rt_init(int argc, char* argv[]) {
  int32_t execNumLocales;
  int runInGDB;
  int runInLLDB;

  // Check that we can get the page size.
  assert( sys_page_size() > 0 );

  // Declare that we are 'locale aware' so that
  // UTF-8 functions (e.g. wcrtomb) work as
  // indicated by the locale environment variables.
  setlocale(LC_CTYPE,"");
  qio_set_glocale();
  // So that use of localtime_r is portable.
  tzset();

  //
  // Handle options that set the environment before doing any other
  // runtime initialization.
  //
  parseArgs(false, parse_dash_E, &argc, argv);

  chpl_error_init();  // This does local-only initialization
  chpl_topo_init();
  chpl_comm_init(&argc, &argv);
  chpl_mem_init();
  chpl_comm_post_mem_init();

  chpl_comm_barrier("about to leave comm init code");

  CreateConfigVarTable();      // get ready to start tracking config vars
  chpl_gen_main_arg.argv = chpl_malloc(argc * sizeof(char*));
  chpl_gen_main_arg.argv[0] = argv[0];
  chpl_gen_main_arg.argc = 1;
  chpl_gen_main_arg.return_value = 0;
  parseArgs(false, parse_normally, &argc, argv);
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

  runInLLDB = _runInLLDB();
  if (runInLLDB) {
    int status;
    if (chpl_comm_run_in_lldb(argc, argv, runInLLDB, &status)) {
      chpl_exit_all(status);
    }
  }

  //
  // Initialize the task management layer.
  //
  chpl_task_init();

  // Initialize privatization, needs to happen before hitting module init
  chpl_privatization_init();

  //
  // Some comm layer initialization has to wait until after the
  // tasking layer is initialized.
  //
  chpl_comm_post_task_init();
#ifdef HAS_CHPL_CACHE_FNS
  chpl_cache_init();
#endif
  chpl_comm_rollcall();

  //
  // Make sure the runtime is fully set up on all locales before we start
  // running Chapel code.
  //
  chpl_comm_barrier("barrier before main");
}

//
// Called by "main.c:main(...)" and "cphl-init.c:chpl_library_finalize()".
//
void chpl_rt_finalize(int return_value) {
  //chpl_rt_postUserCodeHook();
  chpl_exit_all(return_value);
}

//
// Chapel standard module initialization.
//
// Factored out of "main.c:chpl_main(...)" this needs to be called from within the
// "main-task" which is either "chpl_executable_init" or "chpl_library_init".
//
void chpl_std_module_init(void) {
  // chpl__initStringLiterals runs the constructors for all string literals. We
  // need to setup the literals on every locale before any other chapel code is
  // run.
  chpl__initStringLiterals();
  chpl__heapAllocateGlobals(); // allocate global vars on heap for multilocale

  if (chpl_nodeID == 0) {
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
  } else {
    //
    // On non-0 locales, just call the pre- and post-user-code hooks
    // directly.
    //
    chpl_rt_preUserCodeHook();
    chpl_rt_postUserCodeHook();
  }

}

//
// The function previously known as "chpl_main".
//
// Chapel standard module initialization has been factored out
// into chpl-init.c:chapel_std_module_init() for reuse by 
// chpl-init.c:chpl_library_init().
//
void chpl_executable_init(void) {

  chpl_std_module_init();
  if (chpl_nodeID == 0) {
    //
    // Call the compiler-generated main() routine
    //
    chpl_gen_main_arg.return_value = chpl_gen_main(&chpl_gen_main_arg);
  }

}

void chpl_execute_module_deinit(c_fn_ptr deinitFun) {
  void (*deinitFn)(void);
  deinitFn = deinitFun;
  deinitFn();
}

//
// A program using Chapel as a library might look like:
//
// int main(int argc, char* argv) {
//
//   chpl_library_init(...)
//   chpl__init_MODULE_1(LINE, FILENAME)
//   ...
//   chpl__init_MODULE_N(LINE, FILENAME)
//
//   call_chapel_function_from_MODULE_1()
//   ...
//   call_chapel_function_from_MODULE_N()
//
//   chpl_library_finalize()
//
// }
//
void chpl_library_init(int argc, char* argv[]) {
    chpl_rt_init(argc, argv);                   // Initialize the runtime
  chpl_task_callMain(chpl_std_module_init);     // Initialize std modules
}

// Defined in modules/internal/ChapelUtil.chpl.  Used to clean up any modules
// we may have initialized
extern void chpl_deinitModules(void);

//
// A wrapper around chpl-init.c:chpl_rt_finalize(...), sole purpose is 
// to provide a "chpl_library_*" interface for the Chapel "library-user".
void chpl_library_finalize(void) {
  chpl_deinitModules();
  chpl_rt_finalize(0);
}
