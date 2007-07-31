#ifndef _chplcomm_h_
#define _chplcomm_h_

#include "chpltypes.h"
#include "chpl_cfg_comm.h"

extern _int32 _localeID;   // unique ID for each locale: 0, 1, 2, ...
extern _int32 _numLocales; // number of locales

typedef void (*func_p)(void*);
typedef void* (*thread_p)(void*);

typedef struct {
  _int32  locale; // locale that owns the data
  _int32  size;   // size in bytes
  void   *addr;   // local address of data
} _chpl_comm_ptr_t;

#define _chpl_comm_set_fat(fat, ilocale, isize, iaddr) \
  (fat).locale = ilocale;                              \
  (fat).size = isize;                                  \
  (fat).addr = iaddr

//
// initializes communications package
//   set _localeID and _numLocales
// notes:
//   this function is called exactly once by each process
//   it is the first comm function that is called
//
void _chpl_comm_init(int *argc_p, char ***argv_p);

//
// barrier for synchronization between all processes
//
void _chpl_comm_barrier(char *msg);

//
// terminates communication package at the end of the chapel program
// notes:
//   this function is called last
//   it must barrier but allow other threads to be started on this process
//   program termination is not yet implemented correctly
//
void _chpl_comm_done(void);

//
// write 'size' bytes of local data at 'addr' to remote data at
// 'raddr' on locale 'locale'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  _chpl_comm_write(void* addr, _int32 locale, void* raddr, _int32 size);

//
// read 'size' bytes of remote data at 'raddr' on locale 'locale' to
// local data at 'addr'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  _chpl_comm_read(void *addr, _int32 locale, void* raddr, _int32 size);

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
