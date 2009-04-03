#ifndef _chplcomm_h_
#define _chplcomm_h_

#ifndef LAUNCHER

#include <stdint.h>
#include <chplcomm_md.h>
#include "chplcomm_locales.h"
#include "chpltypes.h"

extern int32_t chpl_localeID; // unique ID for each locale: 0, 1, 2, ...
extern int32_t chpl_numLocales; // number of locales

extern int32_t chpl_numPrivateObjects;
extern void** chpl_privateObjects; // privatized array and domain objects

#define chpl_numPrivatizedClasses() chpl_numPrivateObjects
extern void chpl_newPrivatizedClass(void*);
extern void* chpl_getPrivatizedClass(int32_t);


extern void chpl__heapAllocateGlobals(void);

extern const int chpl_numGlobalsOnHeap;
extern char** chpl_globals_registry;
extern char* chpl_globals_registry_static[];

//
// Multi-locale macros
//
// Note: Macros starting with CHPL_COMM involve some kind of communication
//

#define CHPL_WIDEN(wide, ref)                   \
  do {                                          \
    (wide).locale = chpl_localeID;                  \
    (wide).addr = (ref);                        \
  } while (0)

#define CHPL_WIDEN_NULL(wide)                   \
  do {                                          \
    (wide).locale = 0;                          \
    (wide).addr = NULL;                         \
  } while (0)

#define CHPL_WIDEN_STRING(wide, str)                                    \
  do {                                                                  \
    const char* chpl_macro_tmp = str;                                   \
    size_t chpl_macro_len = strlen(chpl_macro_tmp) + 1;                 \
    (wide).locale = chpl_localeID;                                          \
    (wide).addr = chpl_malloc(chpl_macro_len, sizeof(char),             \
                              "set wide string", 0, 0);                 \
    strncpy((char*)(wide).addr, chpl_macro_tmp, chpl_macro_len);        \
    (wide).size = chpl_macro_len;                                       \
  } while (0)

#define CHPL_NARROW(ref, wide)                  \
  (ref) = (wide).addr

#define CHPL_WIDE_TEST_NEQ(wide1, wide2)                                \
  (((wide1).addr != (wide2).addr) ||                                    \
   (((wide1).addr != 0) && ((wide1).locale != (wide2).locale)))

#define CHPL_WIDE_TEST_EQ(wide1, wide2)         \
  (!CHPL_WIDE_TEST_NEQ(wide1, wide2))

#define CHPL_WIDE_CAST(wide1, type, wide2)                              \
  do {                                                                  \
    (wide1).locale = (wide2).locale;                                    \
    (wide1).addr = (type)((wide2).addr);                                \
  } while (0)

#define CHPL_COMM_WIDE_DYNAMIC_CAST(wide1, type, cond, wide2, ln, fn)   \
  do {                                                                  \
    chpl__class_id chpl_macro_tmp;                                      \
    CHPL_COMM_WIDE_GET_FIELD_VALUE(chpl_macro_tmp,                      \
                                   wide2, object, chpl__cid, ln, fn);   \
    (wide1).locale = (wide2).locale;                                    \
    (wide1).addr = (cond) ? (type)((wide2).addr) : NULL;                \
  } while (0)

#define CHPL_WIDE_GET_PRIVATIZED_CLASS(wide, id)                        \
  do {                                                                  \
    (wide).locale = chpl_localeID;                                          \
    (wide).addr = chpl_getPrivatizedClass(id);                          \
  } while (0)

#define CHPL_COMM_WIDE_GET(local, wide, ln, fn)                         \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      local = *(wide).addr;                                             \
    else                                                                \
      chpl_comm_get(&(local), (wide).locale, (wide).addr,               \
                    sizeof(local), ln, fn);                             \
  } while (0)

#define CHPL_COMM_WIDE_GET_LOCALE(local, wide, ln, fn)                  \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      local = (wide).addr->locale;                                      \
    else                                                                \
      chpl_comm_get(&(local), (wide).locale, (wide).addr,               \
                    sizeof(local), ln, fn);                             \
  } while (0)

#define CHPL_COMM_WIDE_PUT(type, wide, local, ln, fn)                   \
  do {                                                                  \
    type chpl_macro_tmp2 = local;                                       \
    if (chpl_localeID == (wide).locale)                                 \
      *(wide).addr = chpl_macro_tmp2;                                   \
    else                                                                \
      chpl_comm_put(&chpl_macro_tmp2, (wide).locale,                    \
                    (wide).addr, sizeof(chpl_macro_tmp2), ln, fn);      \
  } while (0)

#define CHPL_COMM_WIDE_GET_STRING(local, wide, ln, fn)                  \
  do {                                                                  \
    char* chpl_macro_tmp = chpl_malloc((wide).size,                     \
                                       sizeof(char),                    \
                                       "wide_get_string",               \
                                       -1, "<internal>");               \
    if (chpl_localeID == (wide).locale)                                 \
      memcpy(chpl_macro_tmp, (wide).addr, (wide).size);                 \
    else                                                                \
      chpl_comm_get(chpl_macro_tmp, (wide).locale,                      \
                    (void*)((wide).addr), (wide).size, ln, fn);         \
    local = chpl_macro_tmp;                                             \
  } while (0)

#define CHPL_WIDE_GET_FIELD(wide1, wide2, stype, sfield)                \
  do {                                                                  \
    (wide1).locale = (wide2).locale;                                    \
    (wide1).addr = &((stype)((wide2).addr))->sfield;                    \
  } while (0)

#define CHPL_COMM_WIDE_GET_FIELD_VALUE(local, wide, stype, sfield, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      local = ((stype)((wide).addr))->sfield;                           \
    else                                                                \
      chpl_comm_get(&(local),                                           \
                    (wide).locale,                                      \
                    &((stype)((wide).addr))->sfield,                    \
                    sizeof(local), ln, fn);                             \
  } while (0)

#define CHPL_COMM_WIDE_SET_FIELD_VALUE(type, wide, local, stype, sfield, ln, fn) \
  do {                                                                  \
    type chpl_macro_tmp = local;                                        \
    if (chpl_localeID == (wide).locale)                                 \
      ((stype)((wide).addr))->sfield = chpl_macro_tmp;                  \
    else                                                                \
      chpl_comm_put(&chpl_macro_tmp,                                    \
                    (wide).locale,                                      \
                    &((stype)((wide).addr))->sfield,                    \
                    sizeof(chpl_macro_tmp), ln, fn);                    \
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_GET(wide, cls, ind, stype, sfield, ln, fn) \
  do {                                                                  \
    (wide).locale = (cls).locale;                                       \
    CHPL_COMM_WIDE_GET_FIELD_VALUE((wide).addr, cls, stype, sfield, ln, fn); \
    (wide).addr += ind;                                                 \
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_GET_VALUE(wide_type, local, cls, ind, stype, sfield, etype, ln, fn) \
  do {                                                                  \
    wide_type chpl_macro_tmp;                                           \
    CHPL_COMM_WIDE_ARRAY_GET(chpl_macro_tmp, cls, ind, stype, sfield, ln, fn); \
    CHPL_COMM_WIDE_GET(local, chpl_macro_tmp, ln, fn);                  \
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_SET_VALUE(wide_type, cls, ind, stype, sfield, etype, val, ln, fn) \
  do {                                                                  \
    wide_type chpl_macro_tmp;                                           \
    CHPL_COMM_WIDE_ARRAY_GET(chpl_macro_tmp, cls, ind, stype, sfield, ln, fn); \
    CHPL_COMM_WIDE_PUT(etype, chpl_macro_tmp, val, ln, fn);             \
  } while (0)

#define CHPL_WIDE_CLASS_GET_SUPER(type, local, wide)                    \
  do {                                                                  \
    (local).locale = (wide).locale;                                     \
    (local).addr = (type)((wide).addr);                                 \
  } while (0)

#define CHPL_COMM_WIDE_CLASS_GET_CID(local, wide, cid, stype, ln, fn)   \
  do {                                                                  \
    chpl__class_id chpl_macro_tmp;                                      \
    CHPL_COMM_WIDE_GET_FIELD_VALUE(chpl_macro_tmp, wide,                \
                                   stype, chpl__cid, ln, fn);           \
    local = chpl_macro_tmp == cid;                                      \
  } while (0)

#define CHPL_TEST_LOCAL(wide, ln, fn)                                   \
  do {                                                                  \
    if ((wide).locale != chpl_localeID)                                 \
      chpl_error("cannot access remote data in local block", ln, fn);   \
  } while (0)

#define CHPL_HEAP_REGISTER_GLOBAL_VAR(i, wide)            \
  do {                                                    \
    (wide).locale = 0;                                    \
    if (chpl_localeID == 0) {                                 \
      chpl_globals_registry[i] = (char*)((wide).addr);    \
    } else {                                              \
      chpl_globals_registry[i] = (char*)(&((wide).addr)); \
    }                                                     \
  } while (0)

#define CHPL_COMM_BROADCAST_GLOBAL_VARS(numGlobals)             \
  do {                                                          \
    chpl_comm_barrier("barrier before broadcasting globals");  \
    chpl_comm_broadcast_global_vars(numGlobals);               \
    chpl_comm_barrier("barrier after broadcasting globals");   \
  } while (0)

//
// returns the default maximum number of threads that can be handled by this
// communication layer (initial value of maxThreads); use the sentinel value 0
// if the maximum number of threads is limited only by the system's available
// resources.
//
int32_t chpl_comm_getMaxThreads(void);

//
// returns the upper limit on the maximum number of threads that can be handled
// by this communication layer; use the sentinel value 0 if the maximum number
// of threads is limited only by the system's available resources.
//
int32_t chpl_comm_maxThreadsLimit(void);

//
// initializes the communications package
//   set chpl_localeID and chpl_numLocales
// notes:
//   * Called with the argc/argv pair passed to main()
//
void chpl_comm_init(int *argc_p, char ***argv_p);


//
// if possible, run in gdb (because the user threw the --gdb flag)
// using argc and argv.  gdbArgnum gives the index of the argv[]
// element containing the --gdb flag.  Return the status of that
// process in "status" and return 1 if it was possible to run in gdb,
// 0 otherwise
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status);

//
// a final comm layer stub before barrier synching and calling into
// the user code.  It is recommended that a debugging message be
// printed here indicating that each locale has started using
// chpl_msg() and a verbosity level of 2 (which will cause it to be
// displayed using the -v flag).
//
void chpl_comm_rollcall(void);

void chpl_comm_init_shared_heap(void);

//
// allocate chpl_globals_registry or make it point to
// chpl_globals_registry_static depending on the communication layer
//
void chpl_comm_alloc_registry(int numGlobals);

//
// chpl_globals_registry is an array of pointers to addresses; on
// locale 0, these addresses point locally to a class; they are
// uninitialized elsewhere.  This function makes it so that the
// addresses on all other locales are the same as on locale 0.
//
// This function is called collectively.
//
void chpl_comm_broadcast_global_vars(int numGlobals);

//
// Broadcast the value at addr on the calling locale to the same
// address on every other locale.  This is done to set up global
// constants of simple scalar types
//
// Assumes global variables have the same addresses across locales.
//
void chpl_comm_broadcast_private(void* addr, int size);

//
// barrier for synchronization between all processes; currently only
// used for startup and teardown.  msg is a string that can be used
// for debugging to determine where the barrier is being called.
//
void chpl_comm_barrier(const char *msg);

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
void chpl_comm_exit_all(int status);


//
// this routine should terminate the communication package when called
// by any thread, and should clean up the communication package's
// resources as best possible.  This routine is called whenever a user
// thread calls halt or exit and we have no guarantees that all threads
// are calling into the halt or exit.  Otherwise, it is much like the
// chpl_comm_exit_all() routine.
//
void chpl_comm_exit_any(int status);

//
// put 'size' bytes of local data at 'addr' to remote data at
// 'raddr' on locale 'locale'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  chpl_comm_put(void* addr, int32_t locale, void* raddr, int32_t size, int ln, chpl_string fn);

//
// get 'size' bytes of remote data at 'raddr' on locale 'locale' to
// local data at 'addr'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  chpl_comm_get(void *addr, int32_t locale, void* raddr, int32_t size, int ln, chpl_string fn);

//
// remote fork should launch a thread on locale that runs function f
// passing it arg where the size of arg is stored in arg_size
// notes:
//   multiple forks to the same locale should be handled concurrently
//
void chpl_comm_fork(int locale, chpl_fn_int_t fid, void *arg, int arg_size);

//
// non-blocking fork
//
void chpl_comm_fork_nb(int locale, chpl_fn_int_t fid, void *arg, int arg_size);

void chpl_startVerboseComm(void);
void chpl_stopVerboseComm(void);
void chpl_startVerboseCommHere(void);
void chpl_stopVerboseCommHere(void);

void chpl_startCommDiagnostics(void);
void chpl_stopCommDiagnostics(void);
void chpl_startCommDiagnosticsHere(void);
void chpl_stopCommDiagnosticsHere(void);

int32_t chpl_numCommGets(void);
int32_t chpl_numCommPuts(void);
int32_t chpl_numCommForks(void);
int32_t chpl_numCommNBForks(void);

#else  // LAUNCHER

#define chpl_comm_barrier(x)
#define chpl_comm_exit_all(x) exit(x)
#define chpl_comm_exit_any(x) exit(x)

#endif // LAUNCHER
#endif
