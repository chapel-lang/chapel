#ifndef _chplcomm_h_
#define _chplcomm_h_

#ifndef LAUNCHER

#include <stdint.h>
#include <comm_heap_macros.h>
#include <chplcomm_plat_md.h>
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
extern void** chpl_globals_registry;
extern void* chpl_globals_registry_static[];

extern void* const chpl_private_broadcast_table[];

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
    (wide).locale = chpl_localeID;                                      \
    (wide).addr = chpl_malloc(chpl_macro_len, sizeof(char),             \
                              CHPL_RT_MD_SET_WIDE_STRING, 0, 0);        \
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
                                   wide2, object, chpl__cid, chpl__class_id, ln, fn);   \
    (wide1).locale = (wide2).locale;                                    \
    (wide1).addr = (cond) ? (type)((wide2).addr) : NULL;                \
  } while (0)

#define CHPL_WIDE_GET_PRIVATIZED_CLASS(wide, id)                        \
  do {                                                                  \
    (wide).locale = chpl_localeID;                                          \
    (wide).addr = chpl_getPrivatizedClass(id);                          \
  } while (0)

// Enable to support hybrid computing
//#define CHPL_COMM_HETEROGENEOUS
//

#ifdef CHPL_COMM_HETEROGENEOUS
#define SPECIFY_SIZE(type) chpl_rt_type_id_##type
#define SPECIFY_STRING_SIZE(size) -CHPL_TYPE_chpl_string
#else
#define SPECIFY_SIZE(type) (/*chpl_rt_type_id_##type,*/sizeof(type))
#define SPECIFY_STRING_SIZE(size) (size)
#endif


#define CHPL_COMM_WIDE_GET(local, wide, type, ln, fn)                   \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      local = *(wide).addr;                                             \
    else                                                                \
      chpl_comm_get(&(local), (wide).locale, (wide).addr,               \
                    SPECIFY_SIZE(type), ln, fn);                        \
  } while (0)

#define CHPL_COMM_WIDE_GET_SVEC(local, wide, type, ln, fn)            \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      CHPL_ASSIGN_SVEC(local, (*(wide).addr));                             \
    else                                                                \
      chpl_comm_get(&(local), (wide).locale, (wide).addr,               \
                    SPECIFY_SIZE(type), ln, fn);                        \
  } while (0)

#define CHPL_COMM_WIDE_GET_LOCALE(local, wide, ln, fn)                  \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      local = (wide).addr->locale;                                      \
    else                                                                \
      chpl_comm_get(&(local), (wide).locale, (wide).addr,               \
                    SPECIFY_SIZE(int32_t), ln, fn);                     \
  } while (0)

#define CHPL_COMM_WIDE_PUT(type, wide, local, ln, fn)                   \
  do {                                                                  \
    type chpl_macro_tmp2 = local;                                       \
    if (chpl_localeID == (wide).locale)                                 \
      *(wide).addr = chpl_macro_tmp2;                                   \
    else                                                                \
      chpl_comm_put(&chpl_macro_tmp2, (wide).locale,                    \
                    (wide).addr, SPECIFY_SIZE(type), ln, fn);           \
  } while (0)

#define CHPL_COMM_WIDE_PUT_SVEC(type, wide, local, ln, fn)            \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      CHPL_ASSIGN_SVEC(*(wide).addr, local);                               \
    else                                                                \
      chpl_comm_put(&local, (wide).locale,                              \
                    (wide).addr, SPECIFY_SIZE(type), ln, fn);           \
  } while (0)

#define CHPL_COMM_WIDE_GET_STRING(local, wide, ln, fn)                  \
  do {                                                                  \
    char* chpl_macro_tmp = chpl_malloc((wide).size,                     \
                                       sizeof(char),                    \
                                       CHPL_RT_MD_GET_WIDE_STRING,      \
                                       -1, "<internal>");               \
    if (chpl_localeID == (wide).locale)                                 \
      memcpy(chpl_macro_tmp, (wide).addr, (wide).size);                 \
    else                                                                \
      chpl_comm_get(chpl_macro_tmp, (wide).locale,                      \
                    (void*)((wide).addr), SPECIFY_STRING_SIZE((wide).size), ln, fn); \
    local = chpl_macro_tmp;                                             \
  } while (0)

#define CHPL_WIDE_GET_FIELD(wide1, wide2, stype, sfield)                \
  do {                                                                  \
    (wide1).locale = (wide2).locale;                                    \
    (wide1).addr = &((stype)((wide2).addr))->sfield;                    \
  } while (0)

#define CHPL_WIDE_GET_TUPLE_COMPONENT(wide1, wide2,  index)             \
  do {                                                                  \
    (wide1).locale = (wide2).locale;                                    \
    (wide1).addr = &(*(wide2).addr)[index];                             \
  } while (0)

#define CHPL_COMM_WIDE_GET_FIELD_VALUE(local, wide, stype, sfield, type, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      local = ((stype)((wide).addr))->sfield;                           \
    else                                                                \
      chpl_comm_get(&(local),                                           \
                    (wide).locale,                                      \
                    &((stype)((wide).addr))->sfield,                    \
                    SPECIFY_SIZE(type), ln, fn);                        \
  } while (0)

#define CHPL_COMM_WIDE_GET_FIELD_VALUE_SVEC(local, wide, stype, sfield, type, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      CHPL_ASSIGN_SVEC(local, ((stype)((wide).addr))->sfield);             \
    else                                                                \
      chpl_comm_get(&(local),                                           \
                    (wide).locale,                                      \
                    &((stype)((wide).addr))->sfield,                    \
                    SPECIFY_SIZE(type), ln, fn);                        \
  } while (0)

#define CHPL_COMM_WIDE_GET_TUPLE_COMPONENT_VALUE(local, wide, index, type, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      local = (*(wide).addr)[index];                                    \
    else                                                                \
      chpl_comm_get(&(local),                                           \
                    (wide).locale,                                      \
                    &(*(wide).addr)[index],                             \
                    SPECIFY_SIZE(type), ln, fn);                        \
  } while (0)

#define CHPL_COMM_WIDE_GET_TUPLE_COMPONENT_VALUE_SVEC(local, wide, index, type, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      CHPL_ASSIGN_SVEC(local, (*(wide).addr)[index]);                      \
    else                                                                \
      chpl_comm_get(&(local),                                           \
                    (wide).locale,                                      \
                    &(*(wide).addr)[index],                             \
                    SPECIFY_SIZE(type), ln, fn);                        \
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
                    SPECIFY_SIZE(type), ln, fn);                        \
  } while (0)

#define CHPL_COMM_WIDE_SET_FIELD_VALUE_SVEC(type, wide, local, stype, sfield, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      CHPL_ASSIGN_SVEC(((stype)((wide).addr))->sfield, local);             \
    else                                                                \
      chpl_comm_put(&local,                                             \
                    (wide).locale,                                      \
                    &((stype)((wide).addr))->sfield,                    \
                    SPECIFY_SIZE(type), ln, fn);                        \
  } while (0)

#define CHPL_COMM_WIDE_SET_TUPLE_COMPONENT_VALUE(type, wide, local, stype, index, ln, fn) \
  do {                                                                  \
    type chpl_macro_tmp = local;                                        \
    if (chpl_localeID == (wide).locale)                                 \
      (*(wide).addr)[index] = chpl_macro_tmp;                           \
    else                                                                \
      chpl_comm_put(&chpl_macro_tmp,                                    \
                    (wide).locale,                                      \
                    &(*(wide).addr)[index],                             \
                    SPECIFY_SIZE(type), ln, fn);                        \
  } while (0)

#define CHPL_COMM_WIDE_SET_TUPLE_COMPONENT_VALUE_SVEC(type, wide, local, stype, index, ln, fn) \
  do {                                                                  \
    if (chpl_localeID == (wide).locale)                                 \
      CHPL_ASSIGN_SVEC((*(wide).addr)[index], local);                      \
    else                                                                \
      chpl_comm_put(&local,                                             \
                    (wide).locale,                                      \
                    &(*(wide).addr)[index],                             \
                    SPECIFY_SIZE(type), ln, fn);                        \
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_GET(wide, cls, ind, stype, sfield, etype, ln, fn) \
  do {                                                                  \
    (wide).locale = (cls).locale;                                       \
    CHPL_COMM_WIDE_GET_FIELD_VALUE((wide).addr, cls, stype, sfield, etype, ln, fn); \
    (wide).addr += ind;                                                 \
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_GET_SVEC(wide, cls, ind, stype, sfield, etype, ln, fn) \
  do {                                                                  \
    (wide).locale = (cls).locale;                                       \
    CHPL_COMM_WIDE_GET_FIELD_VALUE_SVEC((wide).addr, cls, stype, sfield, etype, ln, fn); \
    (wide).addr += ind;                                                 \
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_GET_VALUE(wide_type, local, cls, ind, stype, sfield, etype, etype2, ln, fn) \
  do {                                                                  \
    wide_type chpl_macro_tmp;                                           \
    CHPL_COMM_WIDE_ARRAY_GET(chpl_macro_tmp, cls, ind, stype, sfield, etype, ln, fn); \
    CHPL_COMM_WIDE_GET(local, chpl_macro_tmp, etype2, ln, fn);           \
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_SET_VALUE(wide_type, cls, ind, stype, sfield, etype, val, ln, fn) \
  do {                                                                  \
    wide_type chpl_macro_tmp;                                           \
    CHPL_COMM_WIDE_ARRAY_GET(chpl_macro_tmp, cls, ind, stype, sfield, etype, ln, fn); \
    CHPL_COMM_WIDE_PUT(etype, chpl_macro_tmp, val, ln, fn);             \
  } while (0)

#define CHPL_COMM_WIDE_ARRAY_SET_VALUE_SVEC(wide_type, cls, ind, stype, sfield, etype, val, ln, fn) \
  do {                                                                  \
    wide_type chpl_macro_tmp;                                           \
    CHPL_COMM_WIDE_ARRAY_GET_SVEC(chpl_macro_tmp, cls, ind, stype, sfield, etype, ln, fn); \
    CHPL_COMM_WIDE_PUT_SVEC(etype, chpl_macro_tmp, val, ln, fn);      \
  } while (0)

#define CHPL_COMM_WIDE_CLASS_GET_CID(local, wide, stype, ln, fn)        \
  CHPL_COMM_WIDE_GET_FIELD_VALUE(local, wide, stype, chpl__cid, chpl__class_id, ln, fn)

#define CHPL_WIDE_CLASS_GET_SUPER(type, local, wide)                    \
  do {                                                                  \
    (local).locale = (wide).locale;                                     \
    (local).addr = (type)((wide).addr);                                 \
  } while (0)

#define CHPL_COMM_WIDE_CLASS_TEST_CID(local, wide, cid, stype, ln, fn)   \
  do {                                                                  \
    chpl__class_id chpl_macro_tmp;                                      \
    CHPL_COMM_WIDE_GET_FIELD_VALUE(chpl_macro_tmp, wide,                \
                                   stype, chpl__cid, chpl__class_id, ln, fn); \
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
    chpl_globals_registry[i] = (&((wide).addr));   \
    CHPL_HEAP_REGISTER_GLOBAL_VAR_EXTRA(i, wide)          \
  } while (0)

#ifdef DEBUG_COMM_INIT
#define CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals) \
  for (int i = 0; i < numGlobals; i++) \
    printf("[%d] chpl_globals_registry[%d] = %p\n", chpl_localeID, i, *((void **)chpl_globals_registry[i]));
#else
#define CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals) ;
#endif

#define CHPL_COMM_BROADCAST_GLOBAL_VARS(numGlobals)             \
  do {                                                          \
    chpl_comm_barrier("barrier before broadcasting globals");  \
    chpl_comm_broadcast_global_vars(numGlobals);               \
    CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals);         \
    chpl_comm_barrier("barrier after broadcasting globals");   \
  } while (0)

//
// returns the default maximum number of threads that can be handled by this
// communication layer (initial value of maxThreadsPerLocale); use the sentinel value 0
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
// Broadcast the value of 'id'th entry in chpl_private_broadcast_table
// on the calling locale onto every other locale.  This is done to set
// up global constants of simple scalar types (primarily).
//
void chpl_comm_broadcast_private(int id, int size);

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
void chpl_comm_exit_any_dirty(int status);
void chpl_comm_exit_any_clean(int status);

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

void  chpl_comm_get_offset(long *addr,long offset, int32_t locale, long* raddr, int32_t size);


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

//
// fast (non-forking) fork (i.e., run in handler)
//
void chpl_comm_fork_fast(int locale, chpl_fn_int_t fid, void *arg, int arg_size);

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
int32_t chpl_numCommFastForks(void);
int32_t chpl_numCommNBForks(void);

#else // LAUNCHER

#define chpl_comm_barrier(x)
#define chpl_comm_exit_all(x) exit(x)
#define chpl_comm_exit_any(x) exit(x)

#endif // LAUNCHER

#endif
