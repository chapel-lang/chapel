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


extern void _chpl_main(void);
extern void CreateConfigVarTable(void);

int main(int argc, char* argv[]) {
  int commArgc;
  char** commArgv;

  // Was this version of main invoked by the user, or by the comm layer?
  if (_chpl_comm_user_invocation(argc, argv)) {
    //
    // If it's the user, parse the arguments to determine the number
    // of locales.
    //
    _int32 execNumLocales;
    parseArgs(argc, argv);
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
    commArgv = _chpl_comm_create_argcv(execNumLocales, argc, argv, &commArgc);
    _chpl_comm_init(&commArgc, &commArgv);
  } else {
    //
    // If it was not the user that invoked this program, let the comm
    // layer parse the arguments first, then the Chapel runtime
    //
    _chpl_comm_init(&argc, &argv);
    parseArgs(argc, argv);
  }
  initMemTable();            // get ready to start tracking memory
  CreateConfigVarTable();    // get ready to start tracking config vars
  initChplThreads();         // initialize the threads layer
  if (_localeID == 0)        // have locale #0 run the user's main function
    _chpl_main();
  exitChplThreads();         // tear down the threads
  printFinalMemStat(0, 0);   // print the final memory statistics
  _chpl_comm_done();         // exit the communication layer

  return 0;
}
