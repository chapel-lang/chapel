#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

#include <pthread.h>

// mutexes
typedef pthread_mutex_t _chpl_mutex_t;

// condition variables
typedef pthread_cond_t    _chpl_condvar_t;
typedef _chpl_condvar_t  *_chpl_condvar_p;

typedef struct {
  volatile _chpl_bool is_full;
  _chpl_mutex_t *lock;          // need to acquire before accessing corresponding _syncvar
  _chpl_condvar_p signal_full;  // wait for full; signal this when full
  _chpl_condvar_p signal_empty; // wait for empty; signal this when empty
} _chpl_sync_aux_t;

typedef struct {
  volatile _chpl_bool is_full;
  _chpl_mutex_t *lock;          // need to acquire before writing to corresponding _singlevar
  _chpl_condvar_p signal_full;  // wait for full; signal this when full
} _chpl_single_aux_t;


#define _chpl_read_FE(x,y,lineno,filename) \
  do {if (_chpl_sync_wait_full_and_lock(&((y)->sync_aux),lineno,filename) == 0) { \
        (x) = (y)->value; \
        _chpl_sync_mark_and_signal_empty(&((y)->sync_aux));} \
     } while (0)

#define _chpl_read_FF(x,y,lineno,filename) \
  do {if (_chpl_sync_wait_full_and_lock(&((y)->sync_aux),lineno,filename) == 0) { \
        (x) = (y)->value; \
        _chpl_sync_mark_and_signal_full(&((y)->sync_aux));} \
     } while (0)

#define _chpl_read_XX(x,y) \
  do {if (_chpl_sync_lock(&((y)->sync_aux)) == 0) { \
        (x) = (y)->value; \
        _chpl_sync_unlock(&((y)->sync_aux));} \
     } while (0)

#define _chpl_write_EF(x,y,lineno,filename) \
  do {if (_chpl_sync_wait_empty_and_lock(&((x)->sync_aux),lineno,filename) == 0) { \
        (x)->value = (y); \
        _chpl_sync_mark_and_signal_full(&((x)->sync_aux));} \
      else chpl_internal_error("invalid mutex in _chpl_write_EF"); \
     } while (0)

#define _chpl_write_FF(x,y,lineno,filename) \
  do {if (_chpl_sync_wait_full_and_lock(&((x)->sync_aux),lineno,filename) == 0) { \
        (x)->value = (y); \
        _chpl_sync_mark_and_signal_full(&((x)->sync_aux));} \
      else chpl_internal_error("invalid mutex in _chpl_write_FF"); \
     } while (0)

#define _chpl_write_XF(x,y) \
  do {if (_chpl_sync_lock(&((x)->sync_aux)) == 0) { \
        (x)->value = (y); \
        _chpl_sync_mark_and_signal_full(&((x)->sync_aux));} \
      else chpl_internal_error("invalid mutex in _chpl_write_XF"); \
     } while (0)

#define _chpl_sync_reset(x)  \
  do {if (_chpl_sync_lock(&((x)->sync_aux)) == 0) { \
        (x)->value = 0; \
        _chpl_sync_mark_and_signal_empty(&((x)->sync_aux));} \
      else chpl_internal_error("invalid mutex in _chpl_sync_reset"); \
     } while (0)


#define _chpl_single_read_FF(x,y,lineno,filename) \
  do {if (_chpl_single_is_full(&((y)->value),&((y)->single_aux),true)) \
        (x) = (y)->value; \
      else if (_chpl_single_wait_full(&((y)->single_aux),lineno,filename) == 0) { \
        (x) = (y)->value; \
        _chpl_single_mark_and_signal_full(&((y)->single_aux));} \
     } while (0)

#define _chpl_single_write_EF(x,y,lineno,filename) \
  do {if (_chpl_single_lock(&((x)->single_aux)) == 0) { \
        if (_chpl_single_is_full(&((x)->value),&((x)->single_aux),true)) \
          chpl_error("single var already defined", lineno, filename); \
        else { \
          (x)->value = (y); \
          _chpl_single_mark_and_signal_full(&((x)->single_aux));}} \
      else chpl_internal_error("invalid mutex in _chpl_single_write_EF"); \
     } while (0)

#define _chpl_single_reset(x) \
  do {(x)->value = 0; \
      (x)->single_aux.is_full = false; \
     } while (0)

#endif
