#ifndef MYTH_WSQUEUE_H_
#define MYTH_WSQUEUE_H_

#include "myth_config.h"
#include "myth_internal_lock.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "myth_mem_barrier.h"
#include "myth_original_lib.h"
#include "myth_config.h"
#include "myth_misc.h"

#include <pthread.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/types.h>

//Use locks for all function
//#define USE_LOCK

//If lock is used in any function, this option must be defined
//#define USE_LOCK_ANY

//Enable locks for each function
//#define USE_LOCK_CLEAR
//#define USE_LOCK_PUSH
//#define USE_LOCK_POP
//#define USE_LOCK_TAKE
//#define USE_LOCK_TRYPASS

#ifdef ALWAYS_LOCK_QUEUE
#define USE_LOCK
#define myth_wsqueue_lock_init(lock)
#define myth_wsqueue_lock_lock(lock)
#define myth_wsqueue_lock_trylock(lock) (1)
#define myth_wsqueue_lock_unlock(lock)
#define myth_wsqueue_lock_destroy(lock)
#define myth_wsqueue_rbarrier()
#define myth_wsqueue_wbarrier()
#define myth_wsqueue_rwbarrier()
#else
#define myth_wsqueue_lock_init(lock) myth_internal_lock_init(lock)
#define myth_wsqueue_lock_lock(lock) myth_internal_lock_lock(lock)
#define myth_wsqueue_lock_trylock(lock) myth_internal_lock_trylock(lock)
#define myth_wsqueue_lock_unlock(lock) myth_internal_lock_unlock(lock)
#define myth_wsqueue_lock_destroy(lock) myth_internal_lock_destroy(lock)
#define myth_wsqueue_rbarrier() myth_rbarrier()
#define myth_wsqueue_wbarrier() myth_wbarrier()
#define myth_wsqueue_rwbarrier() myth_rwbarrier()
#endif

#if defined MYTH_USE_ITIMER || defined MYTH_USE_SIGIO
//If signal-driven I/O is enabled, this option must be defined
#define USE_SIGNAL_CS
#endif

#if defined MYTH_USE_IO_THREAD
#define USE_THREAD_CS
#endif

//Runqueue data structure
typedef struct myth_thread_queue
{
       /* FIXME: align top/base and allocate only CACHE_LINE_SIZE for them */
#ifdef PAD_MYTH_THREAD_QUEUE_TOP_BASE
       char pad0[CACHE_LINE_SIZE];
#endif
       volatile int top;
       volatile int base;
#ifdef PAD_MYTH_THREAD_QUEUE_TOP_BASE
       char pad1[CACHE_LINE_SIZE];
#endif
	struct myth_thread** ptr;
	int size;
	myth_internal_lock_t lock;
#if defined USE_LOCK || defined USE_LOCK_ANY
	myth_internal_lock_t m_lock;
#endif
#ifdef USE_SIGNAL_CS
	volatile int8_t op_flag;
#endif
#ifdef USE_THREAD_CS
	pthread_mutex_t mtx;
#endif
}myth_thread_queue,*myth_thread_queue_t;

//User-thread local structure for queue operation: Currently not used
typedef struct myth_queue_data
{
}myth_queue_data,*myth_queue_data_t;

#endif /* MYTH_QUEUE_H_ */
