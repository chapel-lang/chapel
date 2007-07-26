#ifndef _chplcomm_h_
#define _chplcomm_h_

#include "chpltypes.h"
#include "chpl_cfg_comm.h"

typedef void (*func_p)(void*);
typedef void* (*thread_p)(void*);

typedef struct {
  int   locale; // locale that owns the data
  int   size;   // size in bytes
  void *addr;
} _chpl_comm_ptr_t;

#define _chpl_comm_set_fat(fat, ilocale, isize, iaddr) \
  (fat).locale = ilocale;                              \
  (fat).size = isize;                                  \
  (fat).addr = iaddr

void _chpl_comm_init(int *argc_p, char ***argv_p);
void _chpl_comm_barrier(char *msg);
_int32 _chpl_comm_num_locales(void);
_int32 _chpl_comm_locale_id(void);
void _chpl_comm_done(void);

void  _chpl_comm_write(_chpl_comm_ptr_t *p, void *addr);
void  _chpl_comm_read(void *addr, _chpl_comm_ptr_t *p);
void  _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size);
void  _chpl_comm_fork(int locale, func_p f, void *arg, int arg_size);

#endif
