#include <mpi.h>
#include <armci.h>
#include <gpc.h>

#include "chplrt.h"
#include "chplcomm.h"
#include "chplmem.h"
#include "error.h"

// Undefine this symbol to deactivate extra runtime error checking
#define _ARMCI_DEBUG

#ifdef _ARMCI_DEBUG
#define ARMCI_SAFE(fncall)                            \
{                                                     \
  int ret;                                            \
                                                      \
  ret = fncall;                                       \
  if (ret != 0)                                       \
    ARMCI_Error("ARMCI function call error", ret); \
}
#else
#define ARMCI_SAFE(fncall) fncall
#endif

#ifdef _ARMCI_DEBUG
#define MPI_SAFE(fncall)                              \
{                                                     \
  char errmsg[MPI_MAX_ERROR_STRING];                  \
  int len;                                            \
  int ret;                                            \
                                                      \
  ret = fncall;                                       \
  if (ret != MPI_SUCCESS) {                           \
    MPI_Error_string(ret, errmsg, &len);              \
    _printInternalError("MPI error:");                \
    _printInternalError(errmsg);                      \
    MPI_Abort(MPI_COMM_WORLD, ret);                   \
  }                                                   \
}
#else
#define MPI_SAFE(fncall) fncall
#endif

static int gpc_call_handler(int to, int from, void *hdr, int hlen,
                            void *data,  int dlen,
                            void *rhdr,  int rhlen, int *rhsize,
                            void *rdata, int rdlen, int *rdsize,
                            int rtype);
static int ghndl = -1;

//
// given the program arguments, returns whether the invocation of
// main() is the original invocation by the user (1) or a recursive
// invocation made by the comm layer to get the program started (0).
//
int _chpl_comm_user_invocation(int argc, char* argv[]) {
  // This will probably need to look for a special argument that was
  // inserted during the user invocation call to chpl_comm_init()

  _chpl_msg(2, "Called _chpl_comm_user_invocation() on: %d\n", _localeID);
  // For now return always false ... ? -- danich 10/23/07
  return 0;
}

//
// Returns the default number of locales to use for this comm layer if
// the user does not specify a number.  For most comm layers, this
// should probably print a helpful error and exit rather than
// defaulting to anything.  For comm layer "none" a default of 1
// locale makes sense which is why this routine exists.  If the
// routine returns a value, that value needs to be consistent across
// multiple calls to the routine.
//
int _chpl_comm_default_num_locales(void) {
  // This is probably a good default for ARMCI:
  _printError("Specify number of locales via -nl <#> or --numLocales=<#>", 0, 0);
  return 0;
}


//
// This routine allows a comm layer to screen the number of locales to
// be used.  In particular, if a number exceeding some sort of maximum
// was provided, an error should be reported.
//
void _chpl_comm_verify_num_locales(_int32 proposedNumLocales) {
  // This can probably remain empty for ARMCI unless there is any sort of
  // maximum that you'd need to check against
}


//
// This routine allows a comm layer to process the argc/argv calls
// provided by main(), and optionally to create a modified version of
// argc/argv for use with _chpl_comm_init().  It is called in the case
// of a user invocation of main() after Chapel has parsed the command
// line arguments to determine the number of locales.
//
// This routine takes as its input arguments:
//
// - execNumLocales: indicating the number of locales the program will
//                   be executed on;
//
// - argc/argv as provided to main().  
//
// It specifies the *commArgc value as an output argument and returns
// a corresponding commArgv vector of size commArgc -- these values
// are then passed into _chpl_comm_init() in the case of a user
// invocation of main().  If the comm layer does not need to modify
// argc/argv, it can just pass them through to commArgc/commArgv.
//
char** _chpl_comm_create_argcv(_int32 execNumLocales, int argc, char* argv[], 
                               int* commArgc) {
  // Depending on how _chpl_comm_init() is called, this may need to
  // add extra command line arguments to those provided by the user
  char **commArgv;
  int i;

  _chpl_msg(2, "Called _chpl_comm_create_argcv() on: %d\n", _localeID);

  *commArgc = argc;
  commArgv = _chpl_malloc((*commArgc) + 1, sizeof(char *), "ARMCI argv", __LINE__, __FILE__);
  for (i = 0; i < argc; i++)
    commArgv[i] = argv[i];
  commArgv[argc] = NULL;

  return commArgv;
}


//
// initializes the communications package
//   set _localeID and _numLocales
// notes:
//   * For a user invocation of main(), this routine is called with
//     the argc/argv pair generated by _chpl_comm_create_argcv
//   * For a comm-layer invocation of main(), it is called with the
//     argc/argv pair passed to main()

static int armci_init_called = 0;

//
void _chpl_comm_init(int *argc_p, char ***argv_p, int runInGDB) {
  // This will probably be one of the trickiest routines to implement;
  // the details will depend on what sorts of mechanisms ARMCI
  // supports to create the multiple cooperating executables.  If, for
  // example, one needed to invoke an mpirun-like command to start the
  // program up, you'd want this routine to:
  //
  //   - for the user's invocation of the program, invoke that command
  //     on this same executable, adding an additional command line
  //     argument so that you could determine that those invocations
  //     were not the user's invocation
  //
  //   - for the non-user inovcations, you'd want to strip off those
  //     sentinel argv arguments so that Chapel's argument parsing
  //     routines wouldn't choke on them
  //
  // Don't forget to also set _localeID and _numLocales before
  // returning (this should only matter for the non-user invocations,
  // I believe)

  int nprocs, me;

  if (runInGDB) {
    _printError("--gdb not yet implemented for ARMCI", runInGDB, "<command-line>");
  }

  MPI_SAFE(MPI_Init(argc_p, argv_p));
  ARMCI_SAFE(ARMCI_Init());
  armci_init_called = 1;

  MPI_SAFE(MPI_Comm_size(MPI_COMM_WORLD, &nprocs));
  MPI_SAFE(MPI_Comm_rank(MPI_COMM_WORLD, &me));

  _localeID = me;
  _numLocales = nprocs;

  ghndl = ARMCI_Gpc_register(gpc_call_handler);
}


//
// a final comm layer stub before barrier synching and calling into
// the user code.  It is recommended that a debugging message be
// printed here indicating that each locale has started using
// _chpl_msg() and a verbosity level of 2 (which will cause it to be
// displayed using the -v flag).
//
void _chpl_comm_rollcall(void) {
  // Something like the following should work:
  _chpl_msg(2, "executing on locale %d of %d locale(s)\n", _localeID, 
            _numLocales);
}


//
// barrier for synchronization between all processes; currently only
// used for startup and teardown.  msg is a string that can be used
// for debugging to determine where the barrier is being called.
//
void _chpl_comm_barrier(const char *msg) {
  _chpl_msg(2, "Called _chpl_comm_barrier(\"%s\") on: %d\n", msg, _localeID);

  // Insert ARMCI barrier between all processes here
  ARMCI_Barrier();
}


static void _chpl_comm_exit_common(int status)
{
  _chpl_msg(2, "Called _chpl_comm_exit_common() on: %d\n", _localeID);

  if (armci_init_called) {
    _chpl_msg(2, "Before ARMCI_Gpc_release() on: %d %d\n", _localeID, ghndl);

    if (ghndl != -1)
      ARMCI_Gpc_release(ghndl);

    _chpl_msg(2, "After ARMCI_Gpc_release() on: %d\n", _localeID);

    ARMCI_Finalize();

    _chpl_msg(2, "After ARMCI_Finalize() on: %d\n", _localeID);

    MPI_SAFE(MPI_Finalize());

    _chpl_msg(2, "After MPI_Finalize() on: %d\n", _localeID);
  }
} /* _chpl_comm_exit_common */

//
// terminates communication package at the end of a normal run of the
// chapel program -- assumes all processes are calling into the
// routine.  If the communication layer likes to call exit, the exit
// code is provided using the "status" argument; if it doesn't, it
// can simply return and the Chapel program will call exit().
//
// notes:
//   this function is called last
//   a barrier is invoked before calling into this function
//   Chapel's program termination is not yet implemented correctly
//
void _chpl_comm_exit_all(int status) {
  // Insert cooperative ARMCI exit call here
  _chpl_comm_exit_common(status);
}


//
// this routine should terminate the communication package when called
// by any thread, and should clean up the communication package's
// resources as best possible.  This routine is called whenever a user
// thread calls halt or exit and we have no guarantees that all threads
// are calling into the halt or exit.  Otherwise, it is much like the
// _chpl_comm_exit_all() routine.
//
void _chpl_comm_exit_any(int status) {
  // Insert "any one thread should be able to call" ARMCI exit call here
  _chpl_comm_exit_common(status);
}


//
// write 'size' bytes of local data at 'addr' to remote data at
// 'raddr' on locale 'locale'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  _chpl_comm_put(void* addr, _int32 locale, void* raddr, _int32 size) {
  // this should be an ARMCI put call

  _chpl_msg(2, "Called _chpl_comm_put() on: %d\n", _localeID);

  if (_localeID == locale)
    memmove(raddr, addr, size);
  else {
    ARMCI_SAFE(ARMCI_Put(addr, raddr, size, locale));
  }
}


//
// read 'size' bytes of remote data at 'raddr' on locale 'locale' to
// local data at 'addr'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  _chpl_comm_get(void *addr, _int32 locale, void* raddr, _int32 size) {
  // this should be an ARMCI get call

  _chpl_msg(2, "Called _chpl_comm_get() on: %d\n", _localeID);

  if (_localeID == locale)
    memmove(addr, raddr, size);
  else {
    ARMCI_SAFE(ARMCI_Get(raddr, addr, size, locale));
  }
}

//
// remote fork should launch a thread on locale that runs function f
// passing it arg where the size of arg is stored in arg_size
// notes:
//   multiple forks to the same locale should be handled concurrently
//
void  _chpl_comm_fork(int locale, func_p f, void *arg, int arg_size) {
  // Not sure how to implement this for ARMCI
  int ret;
  gpc_hdl_t nbh;
  void *header, *rheader;
  int rhlen;
  void *rdata;
  int rdlen;
  int i;

  _chpl_msg(2, "Called _chpl_comm_fork() on: %d\n", _localeID);

  if (ghndl == -1) {
    _printInternalError("ARMCI GPC handler function not registered");
    return;
  }

  header = f;
  rhlen = sizeof(f);
  rheader = _chpl_malloc(sizeof(f), sizeof(char), "GPC exec remote header", __LINE__, __FILE__);
  rdata = _chpl_malloc(arg_size, sizeof(char), "GPC exec remote data", __LINE__, __FILE__);
  rdlen = arg_size;

  /* ARMCI_Gpc_init_handle(&nbh); */

  _chpl_msg(2, "Trying to execute on locale: %d from locale: %d\n", locale, _localeID);
  _chpl_msg(2, "f: %p, arg: %p, arg_size: %d\n", f, arg, arg_size);

  ret = ARMCI_Gpc_exec(ghndl, locale, &header, sizeof(f), arg, arg_size,
                       rheader, rhlen, rdata, rdlen, NULL /* &nbh */);
  if (ret != 0) {
    _printInternalError("ARMCI_Gpc_exec() failed");
    _chpl_free(rheader, __LINE__, __FILE__);
    _chpl_free(rdata, __LINE__, __FILE__);
    return;
  }

  _chpl_msg(2, "After ARMCI_Gpc_exec()\n");

  /* ARMCI_Gpc_wait(&nbh); */

  _chpl_free(rheader, __LINE__, __FILE__);
  _chpl_free(rdata, __LINE__, __FILE__);
}

//
// non-blocking fork (not yet used)
//
void  _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size) {
  // Not sure how to implement this for ARMCI

  _chpl_msg(2, "Called _chpl_comm_fork_nb() on: %d\n", _localeID);
}

int gpc_call_handler(int to, int from, void *hdr, int hlen,
                     void *data,  int dlen,
                     void *rhdr,  int rhlen, int *rhsize,
                     void *rdata, int rdlen, int *rdsize,
                     int rtype)
{
  func_p f;

  f = *(func_p *)hdr;
  _chpl_msg(2, "Received callback on locale: %d %p\n", _localeID, f);

  _chpl_msg(2, "hlen: %d, data: %p, dlen: %d, rhdr: %p, rhlen: %d, rdata: %p, rdlen: %d\n",
            hlen, data, dlen, rhdr, rhlen, rdata, rdlen);

  f(data);

  /* No return header */
  *rhsize = 0;
  /* Assumes non-overlapping areas of memory */
  memcpy(rdata, data, dlen);
  *rdsize = dlen;

  return GPC_DONE;
} /* gpc_call_handler */
