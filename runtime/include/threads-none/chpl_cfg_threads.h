#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

// bogus mutex type (no mutexes)
typedef int _chpl_mutex_t;
typedef _chpl_mutex_t *_chpl_mutex_p;

typedef _bool _chpl_sync_aux_t;            // only needs to store the full/empty bit
typedef _chpl_sync_aux_t _chpl_single_aux_t;


#define _chpl_read_FE(x,y,lineno,filename) \
  (x) = (y)->sync_aux ? (((y)->sync_aux = false), (y)->value) \
        : _printError("sync var empty (running in single-threaded mode)", lineno, filename), \
          (y)->value

#define _chpl_read_FF(x,y,lineno,filename) \
  (x) = (y)->sync_aux ? (y)->value \
        : _printError("sync var empty (running in single-threaded mode)", lineno, filename), \
          (y)->value

#define _chpl_read_XX(x,y) (x) = (y)->value

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

#define _chpl_sync_reset(x)  \
  do {(x)->value = 0; \
      (x)->sync_aux = false; \
     } while (0)


#define _chpl_single_read_FF(x,y,lineno,filename) \
  (x) = (y)->single_aux ? (y)->value \
        : _printError("single var not yet defined (running in single-threaded mode)", lineno, filename), \
          (y)->value

#define _chpl_single_write_EF(x,y,lineno,filename) \
  do {if ((x)->single_aux) \
        _printError("single var already defined", lineno, filename); \
      else { \
        (x)->value = (y); \
        (x)->single_aux = true;} \
     } while (0)

#define _chpl_single_reset(x) \
  do {(x)->value = 0; \
      (x)->single_aux = false; \
     } while (0)


// thread-related - used in comm-gasnet
typedef int _chpl_thread_t;                // bogus thread handle

#define _chpl_thread_create(thread, attr, fun, arg) \
  (*fun)(arg)

#endif
