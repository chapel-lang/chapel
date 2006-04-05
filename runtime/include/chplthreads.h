#ifndef _chplthreads_h_
#define _chplthreads_h_

#include <pthread.h>

typedef void* (*_chpl_threadfp_t)(void*);  // function pointer
typedef void* _chpl_threadarg_t;           // function argument
typedef pthread_t _chpl_thread_t;          // thread handle
typedef struct {                           // temporary work space
  _chpl_thread_t thread;                   // thread handle for wait
  int            error;                    // to store fork error code
} _chpl_cobegin_wkspace_t;


int
_chpl_cobegin (int                      nthreads, // number of threads
               _chpl_threadfp_t        *fp,       // ptrs to functions to fork
               _chpl_threadarg_t       *a,        // function args
               _chpl_cobegin_wkspace_t *twrk);    // temporary work space

#endif  // _chplthreads_h_
