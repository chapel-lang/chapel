#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

// bogus mutex type (no mutexes)
typedef int _chpl_mutex_t;
typedef _chpl_mutex_t *_chpl_mutex_p;

typedef _bool _chpl_sync_aux_t;            // only needs to store the full/empty bit
typedef _chpl_sync_aux_t _chpl_single_aux_t;

#define _chpl_write_EF(x,y,lineno,filename) \
  do {if ((x)->sync_aux) \
        _printError("sync var full (running in single-threaded mode)", lineno, filename); \
      else { \
        (x)->value = (y); \
        (x)->sync_aux = true;} \
     } while (0)

#define _chpl_write_FF(x,y,lineno,filename) \
  do {if (!(x)->sync_aux) \
        _printError("sync var empty (running in single-threaded mode)", lineno, filename); \
      else (x)->value = (y); \
     } while (0)

#define _chpl_write_XF(x,y) \
  do {(x)->value = (y); \
      (x)->sync_aux = true; \
     } while (0)

#define _chpl_write_XE0(x)  \
  do {(x)->value = 0; \
      (x)->sync_aux = false; \
     } while (0)

#define _chpl_read_FE(x,lineno,filename) \
  ((x)->sync_aux ? (((x)->sync_aux = false), (x)->value) : \
   _printError("sync var empty (running in single-threaded mode)", lineno, filename))

#define _chpl_read_FF(x,lineno,filename) \
  ((x)->sync_aux ? (x)->value : \
   _printError("sync var empty (running in single-threaded mode)", lineno, filename))

#define _chpl_read_XX(x) (x)->value

#define _chpl_single_write_EF(x,y,lineno,filename) \
  do {if ((x)->single_aux) \
        _printError("single var already defined", lineno, filename); \
      else { \
        (x)->value = (y); \
        (x)->single_aux = true;} \
     } while (0)

#define _chpl_single_read_FF(x) \
  ((x)->single_aux ? (x)->value : \
   _printError("single var not yet defined (running in single-threaded mode)", 0, 0))

// thread-related
typedef void* (*_chpl_threadfp_t)(void*);  // function pointer
typedef void* _chpl_threadarg_t;           // function argument
typedef int _chpl_thread_attr_t;           // bogus thread attributes
typedef int _chpl_thread_t;                // bogus thread handle
typedef struct {                           // temporary work space
  _chpl_thread_t thread;                   // thread handle for join/wait
  int            error;                    // to store fork error code
} _chpl_cobegin_wkspace_t;

#define _chpl_thread_create(thread, attr, fun, arg) \
  (*fun)(arg)

#endif
