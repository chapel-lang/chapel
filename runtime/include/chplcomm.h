#ifndef _chplcomm_h_
#define _chplcomm_h_

#include "chpltypes.h"
#include "chpl_cfg_comm.h"

extern _int32 _localeID;   // unique ID for each locale: 0, 1, 2, ...
extern _int32 _numLocales; // number of locales

extern void _heapAllocateGlobals(void);
extern char* _global_vars_registry[];

typedef void (*func_p)(void*);

#define _HEAP_REGISTER_GLOBAL_VAR(i, wide) \
  (wide).locale = 0; _global_vars_registry[i] = (char*)(&((wide).addr))

#define _SET_WIDE_CLASS(wide, cls) \
  (wide).locale = _localeID; (wide).addr = cls

#define _WIDE_CLASS_EQ(wide1, wide2) \
  (((wide1).locale == (wide2).locale) && ((wide1).addr == (wide2).addr))

#define _WIDE_CLASS_NE(wide1, wide2) \
  (((wide1).locale != (wide2).locale) && ((wide1).addr != (wide2).addr))

#define _WIDE_CLASS_CAST(wide1, type, wide2) \
  (wide1).locale = (wide2).locale; (wide1).addr = (type)((wide2).addr)

#define _WIDE_CLASS_DYNAMIC_CAST(wide1, type, cond, wide2)              \
  do {                                                                  \
    _class_id _tmp;                                                     \
    _COMM_WIDE_CLASS_GET_OFF(_class_id, _tmp, wide2, object, _cid);     \
    (wide1).locale = (wide2).locale;                                    \
    (wide1).addr = (cond) ? (type)((wide2).addr) : NULL;                \
  } while (0)

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
    wide_type wide;                                                     \
    _COMM_SET_WIDE_REF_WIDE_ARRAY(type, wide, cls, ind, stype, sfield, etype); \
    _COMM_WIDE_GET(etype, local, wide);                                 \
  } while (0)

#define _COMM_WIDE_ARRAY_SET(type, wide_type, cls, ind, stype, sfield, etype, val) \
  do {                                                                  \
    wide_type wide;                                                     \
    _COMM_SET_WIDE_REF_WIDE_ARRAY(type, wide, cls, ind, stype, sfield, etype); \
    _COMM_WIDE_PUT(etype, wide, val);                                    \
  } while (0)

#define _COMM_WIDE_GET(type, local, wide)                               \
  _chpl_comm_get(&(local), (wide).locale, (wide).addr, sizeof(type))

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

#define _COMM_WIDE_CLASS_GET_OFF(type, local, wide, stype, sfield)      \
  _chpl_comm_get(&(local),                                              \
                 (wide).locale,                                         \
                 (char*)((wide).addr) +                                 \
                 ((char*)(&(((stype)(&(local)))->sfield)) -             \
                  (char*)(&(local))),                                   \
                 sizeof(((stype)(&(local)))->sfield))

#define _COMM_WIDE_CLASS_GET_CID(local, wide, cid, stype)               \
  do {                                                                  \
    _class_id _cid_tmp;                                                 \
    _COMM_WIDE_CLASS_GET_OFF(_class_id, _cid_tmp, wide, stype, _cid);   \
    local = _cid_tmp == cid;                                            \
  } while (0)

#define _COMM_WIDE_PUT(type, wide, local)                               \
  do {                                                                  \
    type _imm = local;                                                  \
    _chpl_comm_put(&_imm, (wide).locale, (wide).addr, sizeof(type));    \
  } while (0)

#define _COMM_WIDE_PUT_OFF(type, wide, local, stype, sfield)            \
  do {                                                                  \
    type imm = local;                                                   \
    _chpl_comm_put(&imm,                                                \
                   (wide).locale,                                       \
                   (char*)((wide).addr) +                               \
                   ((char*)(&(((stype*)(&imm))->sfield)) -              \
                    (char*)(&imm)),                                     \
                   sizeof(((stype*)(&imm))->sfield));                   \
  } while (0)

#define _COMM_WIDE_CLASS_PUT_OFF(type, wide, local, stype, sfield)      \
  do {                                                                  \
    type imm = local;                                                   \
    _chpl_comm_put(&imm,                                                \
                   (wide).locale,                                       \
                   (char*)((wide).addr) +                               \
                   ((char*)(&(((stype)(&imm))->sfield)) -               \
                    (char*)(&imm)),                                     \
                   sizeof(((stype)(&imm))->sfield));                    \
  } while (0)

//
// given the program arguments, returns whether the invocation of
// main() is the original invocation by the user (1) or a recursive
// invocation made by the comm layer to get the program started (0).
//
int _chpl_comm_user_invocation(int argc, char* argv[]);

//
// Returns the default number of locales to use for this comm layer if
// the user does not specify a number.  For most comm layers, this
// should probably print a helpful error and exit rather than
// defaulting to anything.  For comm layer "none" a default of 1
// locale makes sense which is why this routine exists.  If the
// routine returns a value, that value needs to be consistent across
// multiple calls to the routine.
//
int _chpl_comm_default_num_locales(void);

//
// This routine allows a comm layer to screen the number of locales to
// be used.  In particular, if a number exceeding some sort of maximum
// was provided, an error should be reported.
//
void _chpl_comm_verify_num_locales(_int32 proposedNumLocales);

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
                               int* commArgc);

//
// initializes the communications package
//   set _localeID and _numLocales
// notes:
//   * For a user invocation of main(), this routine is called with
//     the argc/argv pair generated by _chpl_comm_create_argcv and
//     if --gdb was used, runInGDB will be set to the argv # that
//     contained the --gdb flag, 0 otherwise.
//   * For a comm-layer invocation of main(), it is called with the
//     argc/argv pair passed to main() and runInGDB will be 0.
//
void _chpl_comm_init(int *argc_p, char ***argv_p, int runInGDB);

//
// a final comm layer stub before barrier synching and calling into
// the user code.  It is recommended that a debugging message be
// printed here indicating that each locale has started using
// _chpl_msg() and a verbosity level of 2 (which will cause it to be
// displayed using the -v flag).
//
void _chpl_comm_rollcall(void);

//
// _global_vars_registry is an array of pointers to addresses; on
// locale 0, these addresses point locally to a class; they are
// uninitialized elsewhere.  This function makes it so that the
// addresses on all other locales are the same as on locale 0.
//
// This function is called collectively.
//
// It is necessary to barrier across all locales when this functions
// is called.
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
void  _chpl_comm_put(void* addr, _int32 locale, void* raddr, _int32 size);

//
// get 'size' bytes of remote data at 'raddr' on locale 'locale' to
// local data at 'addr'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  _chpl_comm_get(void *addr, _int32 locale, void* raddr, _int32 size);

//
// remote fork should launch a thread on locale that runs function f
// passing it arg where the size of arg is stored in arg_size
// notes:
//   multiple forks to the same locale should be handled concurrently
//
void  _chpl_comm_fork(int locale, func_p f, void *arg, int arg_size);

//
// non-blocking fork (not yet used)
//
void  _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size);


#endif
