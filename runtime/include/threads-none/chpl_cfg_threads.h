#ifndef _chpl_cfg_threads_h_
#define _chpl_cfg_threads_h_

// bogus mutex type (no mutexes)
typedef int _chpl_mutex_t;

typedef _chpl_bool _chpl_sync_aux_t;       // only needs to store the full/empty bit
typedef _chpl_sync_aux_t _chpl_single_aux_t;


#define _chpl_read_FE(x,y,lineno,filename) \
  do {if ((y)->sync_aux) { \
        (x) = (y)->value; \
        (y)->sync_aux = false; } \
      else _chpl_error("sync var empty (running in single-threaded mode)", lineno, filename); \
     } while (0)

#define _chpl_read_FF(x,y,lineno,filename) \
  do {if ((y)->sync_aux) (x) = (y)->value; \
      else _chpl_error("sync var empty (running in single-threaded mode)", lineno, filename); \
     } while (0)

#define _chpl_read_XX(x,y) (x) = (y)->value

#define _chpl_write_EF(x,y,lineno,filename) \
  do {if ((x)->sync_aux) \
        _chpl_error("sync var full (running in single-threaded mode)", lineno, filename); \
      else { \
        (x)->value = (y); \
        (x)->sync_aux = true;} \
     } while (0)

#define _chpl_write_FF(x,y,lineno,filename) \
  do {if (!(x)->sync_aux) \
        _chpl_error("sync var empty (running in single-threaded mode)", lineno, filename); \
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
  do {if ((y)->single_aux) (x) = (y)->value; \
      else _chpl_error("single var not yet defined (running in single-threaded mode)", lineno, filename); \
     } while (0)

#define _chpl_single_write_EF(x,y,lineno,filename) \
  do {if ((x)->single_aux) \
        _chpl_error("single var already defined", lineno, filename); \
      else { \
        (x)->value = (y); \
        (x)->single_aux = true;} \
     } while (0)

#define _chpl_single_reset(x) \
  do {(x)->value = 0; \
      (x)->single_aux = false; \
     } while (0)

#endif
