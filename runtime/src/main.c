#include "arg.h"
#include "chapel_code.h"
#include "chplcast.h"
#include "chplcgfns.h"
#include "chplcomm.h"
#include "chplexit.h"
#include "chplio.h"
#include "chplmem.h"
#include "chplrt.h"
#include "chplthreads.h"
#include "config.h"
#include "error.h"
#include "gdb.h"
#include <stdint.h>


int main(int argc, char* argv[]) {
  int32_t execNumLocales;
  int runInGDB;

  _chpl_comm_init(&argc, &argv);
  _chpl_comm_init_shared_heap();

  _chpl_comm_barrier("about to leave comm init code");
  _heapAllocateGlobals();    // allocate global vars on heap for multilocale
  parseArgs(argc, argv);
  runInGDB = _runInGDB();
  if (runInGDB) {
    int status;
    if (_chpl_comm_run_in_gdb(argc, argv, runInGDB, &status)) {
      _chpl_exit_all(status);
    }
  }
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
  _chpl_comm_rollcall();
  initMemTable();            // get ready to start tracking memory
  CreateConfigVarTable();    // get ready to start tracking config vars
  chpl_init_chapel_code();
  initChplThreads();         // initialize the threads layer
  _initModuleGuards();       // initialize _run_mod_firsttime vars

  if (_localeID == 0)        // have locale #0 run the user's main function
    chpl_main();

  _chpl_exit_all(0);         // have everyone exit
  return 0;                  // should never get here
}
