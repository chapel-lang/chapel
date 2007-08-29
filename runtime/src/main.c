#include "arg.h"
#include "chplcast.h"
#include "chplcomm.h"
#include "chplexit.h"
#include "chplio.h"
#include "chplmem.h"
#include "chplrt.h"
#include "chplthreads.h"
#include "config.h"
#include "error.h"
#include "gdb.h"


extern void _chpl_main(void);

int main(int argc, char* argv[]) {
  // Was this version of main invoked by the user, or by the comm layer?
  if (_chpl_comm_user_invocation(argc, argv)) {
    //
    // If it's the user, parse the arguments to determine the number
    // of locales.
    //
    _int32 execNumLocales;
    parseArgs(argc, argv);
    if (_runInGDB()) {
      runInGDB(argc, argv);
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

    //
    // Give the comm layer a chance to munge the argc/argv vector as
    // appropriate and invoke chpl_comm_init
    //
    {
      int commArgc;
      char** commArgv;

      commArgv = _chpl_comm_create_argcv(execNumLocales, argc, argv, &commArgc);
      _chpl_comm_init(&commArgc, &commArgv);
    }
  } else {
    //
    // If it was not the user that invoked this program, let the comm
    // layer parse the arguments first, then the Chapel runtime
    //
    _chpl_comm_init(&argc, &argv);
    parseArgs(argc, argv);
  }
  _chpl_comm_rollcall();
  _chpl_comm_barrier("about to leave comm init code");
  initMemTable();            // get ready to start tracking memory
  CreateConfigVarTable();    // get ready to start tracking config vars
  initChplThreads();         // initialize the threads layer

  if (_localeID == 0)        // have locale #0 run the user's main function
    _chpl_main();

  _chpl_exit_all(0);         // have everyone exit
  return 0;                  // should never get here
}
