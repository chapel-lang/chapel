#ifndef _chplcomm_h_
#define _chplcomm_h_

#ifndef LAUNCHER

#include <stdint.h>
#include "chplcomm_locales.h"
#include "chpltypes.h"
#include "chpl_cfg_comm.h"


extern int32_t _localeID;   // unique ID for each locale: 0, 1, 2, ...
extern int32_t _numLocales; // number of locales

extern int32_t chpl_numPrivateObjects;
extern void** chpl_privateObjects; // privatized array and domain objects

#define chpl_numPrivateClasses() chpl_numPrivateObjects
extern void chpl_setPrivateClass(void*);
extern void* chpl_getPrivateClass(int32_t);


extern void _heapAllocateGlobals(void);

extern const int numGlobalsOnHeap;
extern char** _global_vars_registry;
extern char* _global_vars_registry_static[];

typedef void (*func_p)(void*);

#define LOCAL_CHECK(wide, lineno, filename) \
  if ((wide).locale != _here.locale) \
    chpl_error("cannot access remote data in local block", lineno, filename);

#define LOCAL_DEREF(wide, local) \
  (local) = (wide).addr

#define _HEAP_REGISTER_GLOBAL_VAR(i, wide) \
  (wide).locale = 0; \
  if (_localeID == 0) { \
    _global_vars_registry[i] = (char*)((wide).addr); \
  } else { \
    _global_vars_registry[i] = (char*)(&((wide).addr)); \
  }

#define _SET_WIDE_CLASS(wide, cls) \
  (wide).locale = _localeID; (wide).addr = cls

#define _SET_WIDE_STRING(wide, str)                                \
  do {                                                             \
    const char* chpl_macro_tmp = str;                              \
    size_t chpl_macro_len = strlen(chpl_macro_tmp) + 1;            \
    (wide).locale = _localeID;                                     \
    if (chpl_macro_tmp >= chpl_meminfo.head && chpl_macro_tmp <= chpl_meminfo.tail) {  \
      (wide).addr = chpl_macro_tmp;                                \
    } else {                                                       \
      (wide).addr = chpl_malloc(chpl_macro_len, sizeof(char), "set wide string", 0, 0); \
      strncpy((char*)(wide).addr, chpl_macro_tmp, chpl_macro_len); \
   }                                                               \
    (wide).size = chpl_macro_len;                                  \
  } while (0)

#define _WIDE_CLASS_EQ(wide1, wide2)                                    \
  ((((wide1).addr == 0) && ((wide2).addr == 0)) ||                      \
   (((wide1).locale == (wide2).locale) && ((wide1).addr == (wide2).addr)))

#define _WIDE_CLASS_NE(wide1, wide2)                                    \
  (((wide1).addr != (wide2).addr) ||                                    \
   (((wide1).addr != 0) && ((wide1).locale != (wide2).locale)))

#define _WIDE_CLASS_CAST(wide1, type, wide2) \
  (wide1).locale = (wide2).locale; (wide1).addr = (type)((wide2).addr)

#define _WIDE_CLASS_DYNAMIC_CAST(wide1, type, cond, wide2)              \
  do {                                                                  \
    chpl__class_id chpl_macro_tmp;                                      \
    _COMM_WIDE_CLASS_GET_OFF(chpl__class_id, chpl_macro_tmp, wide2, object, chpl__cid); \
    (wide1).locale = (wide2).locale;                                    \
    (wide1).addr = (cond) ? (type)((wide2).addr) : NULL;                \
  } while (0)

#define _WIDE_CLASS_GET_PRIVATE_CLASS(wide, id)                         \
  (wide).locale = _localeID; (wide).addr = chpl_getPrivateClass(id)

#define _SET_WIDE_REF(wide, ref) \
  (wide).locale = _localeID; (wide).addr = ref

#define _SET_WIDE_REF_OFF(wide1, wide2, stype, sfield)                  \
  (wide1).locale = (wide2).locale; (wide1).addr = (&(((stype)((wide2).addr))->sfield))

#define _COMM_SET_WIDE_REF_WIDE_ARRAY(type, wide, cls, ind, stype, sfield, etype) \
  (wide).locale = (cls).locale;                                         \
  _COMM_WIDE_CLASS_GET_OFF(type, (wide).addr, cls, stype, sfield);      \
  (wide).addr += ind
// used to multiply by sizeof(etype) but since (wide).addr is etype, not necessary
//   (wide).addr += ind * sizeof(etype)

#define _COMM_WIDE_ARRAY_GET(type, wide_type, local, cls, ind, stype, sfield, etype) \
  do {                                                                  \
    wide_type chpl_macro_tmp;                                           \
    _COMM_SET_WIDE_REF_WIDE_ARRAY(type, chpl_macro_tmp, cls, ind, stype, sfield, etype); \
    _COMM_WIDE_GET(etype, local, chpl_macro_tmp);                       \
  } while (0)

#define _COMM_WIDE_ARRAY_SET(type, wide_type, cls, ind, stype, sfield, etype, val) \
  do {                                                                  \
    wide_type chpl_macro_tmp;                                           \
    _COMM_SET_WIDE_REF_WIDE_ARRAY(type, chpl_macro_tmp, cls, ind, stype, sfield, etype); \
    _COMM_WIDE_PUT(etype, chpl_macro_tmp, val);                         \
  } while (0)

#define _COMM_WIDE_GET(type, local, wide)                               \
  _chpl_comm_get(&(local), (wide).locale, (wide).addr, sizeof(type))

#define _COMM_WIDE_GET_STRING(local, wide)                              \
  do {                                                                  \
    char* chpl_macro_tmp = chpl_malloc((wide).size,                    \
                                        sizeof(char),                   \
                                        "wide_get_string", -1, "<internal>"); \
    _chpl_comm_get(chpl_macro_tmp, (wide).locale, (wide).addr, (wide).size); \
    local = chpl_macro_tmp;                                             \
  } while (0)

#define _COMM_WIDE_GET_OFF(type, local, wide, stype, sfield)            \
  _chpl_comm_get(&(local),                                              \
                 (wide).locale,                                         \
                 (char*)((wide).addr) +                                 \
                 ((char*)(&(((stype*)(&(local)))->sfield)) -            \
                  (char*)(&(local))),                                   \
                 sizeof(((stype*)(&(local)))->sfield))

#define _WIDE_CLASS_GET_SUPER(type, local, wide)                        \
  (local).locale = (wide).locale;                                       \
  (local).addr = (type)((wide).addr)

#define _COMM_WIDE_GET_LOCALE(local, wide)                              \
  _chpl_comm_get(&(local), (wide).locale, (wide).addr, sizeof(int32_t)) \

#define _COMM_WIDE_CLASS_GET_OFF(type, local, wide, stype, sfield)      \
  _chpl_comm_get(&(local),                                              \
                 (wide).locale,                                         \
                 (char*)((wide).addr) +                                 \
                 ((char*)(&(((stype)(&(local)))->sfield)) -             \
                  (char*)(&(local))),                                   \
                 sizeof(((stype)(&(local)))->sfield))

#define _COMM_WIDE_CLASS_GET_CID(local, wide, cid, stype)               \
  do {                                                                  \
    chpl__class_id chpl_macro_tmp;                                      \
    _COMM_WIDE_CLASS_GET_OFF(chpl__class_id, chpl_macro_tmp, wide, stype, chpl__cid); \
    local = chpl_macro_tmp == cid;                                      \
  } while (0)

#define _COMM_WIDE_PUT(type, wide, local)                               \
  do {                                                                  \
    type chpl_macro_tmp2 = local;                                       \
    _chpl_comm_put(&chpl_macro_tmp2, (wide).locale, (wide).addr, sizeof(type)); \
  } while (0)

#define _COMM_WIDE_PUT_OFF(type, wide, local, stype, sfield)            \
  do {                                                                  \
    type chpl_macro_tmp = local;                                        \
    _chpl_comm_put(&chpl_macro_tmp,                                     \
                   (wide).locale,                                       \
                   (char*)((wide).addr) +                               \
                   ((char*)(&(((stype*)(&chpl_macro_tmp))->sfield)) -   \
                    (char*)(&chpl_macro_tmp)),                          \
                   sizeof(((stype*)(&chpl_macro_tmp))->sfield));        \
  } while (0)

#define _COMM_WIDE_CLASS_PUT_OFF(type, wide, local, stype, sfield)      \
  do {                                                                  \
    type chpl_macro_tmp = local;                                        \
    _chpl_comm_put(&chpl_macro_tmp,                                     \
                   (wide).locale,                                       \
                   (char*)((wide).addr) +                               \
                   ((char*)(&(((stype)(&chpl_macro_tmp))->sfield)) -    \
                    (char*)(&chpl_macro_tmp)),                          \
                   sizeof(((stype)(&chpl_macro_tmp))->sfield));         \
  } while (0)

#define _COMM_BROADCAST_GLOBAL_VARS(numGlobals)                 \
  _chpl_comm_barrier("barrier before broadcasting globals");    \
  _chpl_comm_broadcast_global_vars(numGlobals);                 \
  _chpl_comm_barrier("barrier after broadcasting globals");

//
// returns the default maximum number of threads that can be handled by this
// communication layer (initial value of maxThreads); use the sentinel value 0
// if the maximum number of threads is limited only by the system's available
// resources.
//
int32_t _chpl_comm_getMaxThreads(void);

//
// returns the upper limit on the maximum number of threads that can be handled
// by this communication layer; use the sentinel value 0 if the maximum number
// of threads is limited only by the system's available resources.
//
int32_t _chpl_comm_maxThreadsLimit(void);

//
// initializes the communications package
//   set _localeID and _numLocales
// notes:
//   * Called with the argc/argv pair passed to main()
//
void _chpl_comm_init(int *argc_p, char ***argv_p);


//
// if possible, run in gdb (because the user threw the --gdb flag)
// using argc and argv.  gdbArgnum gives the index of the argv[]
// element containing the --gdb flag.  Return the status of that
// process in "status" and return 1 if it was possible to run in gdb,
// 0 otherwise
//
int _chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status);

//
// a final comm layer stub before barrier synching and calling into
// the user code.  It is recommended that a debugging message be
// printed here indicating that each locale has started using
// chpl_msg() and a verbosity level of 2 (which will cause it to be
// displayed using the -v flag).
//
void _chpl_comm_rollcall(void);

void _chpl_comm_init_shared_heap(void);

//
// allocate _global_vars_registry or make it point to
// _global_vars_registry_static depending on the communication layer
//
void _chpl_comm_alloc_registry(int numGlobals);

//
// _global_vars_registry is an array of pointers to addresses; on
// locale 0, these addresses point locally to a class; they are
// uninitialized elsewhere.  This function makes it so that the
// addresses on all other locales are the same as on locale 0.
//
// This function is called collectively.
//
void _chpl_comm_broadcast_global_vars(int numGlobals);

//
// Broadcast the value at addr on the calling locale to the same
// address on every other locale.  This is done to set up global
// constants of simple scalar types
//
// Assumes global variables have the same addresses across locales.
//
void _chpl_comm_broadcast_private(void* addr, int size);

//
// barrier for synchronization between all processes; currently only
// used for startup and teardown.  msg is a string that can be used
// for debugging to determine where the barrier is being called.
//
void _chpl_comm_barrier(const char *msg);

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
void _chpl_comm_exit_all(int status);


//
// this routine should terminate the communication package when called
// by any thread, and should clean up the communication package's
// resources as best possible.  This routine is called whenever a user
// thread calls halt or exit and we have no guarantees that all threads
// are calling into the halt or exit.  Otherwise, it is much like the
// _chpl_comm_exit_all() routine.
//
void _chpl_comm_exit_any(int status);

//
// put 'size' bytes of local data at 'addr' to remote data at
// 'raddr' on locale 'locale'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  _chpl_comm_put(void* addr, int32_t locale, void* raddr, int32_t size);

//
// get 'size' bytes of remote data at 'raddr' on locale 'locale' to
// local data at 'addr'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  _chpl_comm_get(void *addr, int32_t locale, const void* raddr, int32_t size);

//
// remote fork should launch a thread on locale that runs function f
// passing it arg where the size of arg is stored in arg_size
// notes:
//   multiple forks to the same locale should be handled concurrently
//
void  _chpl_comm_fork(int locale, func_p f, void *arg, int arg_size);

//
// non-blocking fork
//
void  _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size);

void chpl_startCommDiagnosis(void);
void chpl_stopCommDiagnosis(void);

#else  // LAUNCHER

#define _chpl_comm_barrier(x)
#define _chpl_comm_exit_all(x) exit(x)
#define _chpl_comm_exit_any(x) exit(x)

#endif // LAUNCHER
#endif
