#ifndef MYTH_H_
#define MYTH_H_

#ifdef __cplusplus
extern "C" {
#endif

//Exported function declarations

#ifdef MYTH_USE_INLINE
//Inline functions
#include "myth_if_inline.h"

#else
typedef void*(*myth_func_t)(void*);
typedef struct myth_thread* myth_thread_t;
typedef struct myth_barrier* myth_barrier_t;
typedef struct myth_pickle* myth_pickle_t;
typedef struct myth_thread* (*myth_steal_func_t)(int);
typedef struct myth_felock* myth_felock_t;
typedef struct myth_mutex* myth_mutex_t;
typedef struct myth_cond* myth_cond_t;
typedef unsigned int myth_key_t;

//Native functions
#include "myth_if_native.h"

#endif

#ifdef __cplusplus
} //extern "C"
#endif

#endif /* MYTH_H_ */
