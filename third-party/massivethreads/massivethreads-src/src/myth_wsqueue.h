/* 
 * myth_wsqueue.h
 */
#pragma once
#ifndef MYTH_WSQUEUE_H_
#define MYTH_WSQUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "myth/myth.h"

#include "myth_config.h"
#include "myth_spinlock_func.h"

#include <pthread.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/types.h>

//Lock the queue on all the operation
#define ALWAYS_LOCK_QUEUE 0

#define USE_LOCK ALWAYS_LOCK_QUEUE
//Enable locks for each function
#define USE_LOCK_CLEAR   0
#define USE_LOCK_PUSH    0
#define USE_LOCK_POP     0
#define USE_LOCK_TAKE    0
#define USE_LOCK_TRYPASS 0

#define USE_LOCK_ANY (USE_LOCK_CLEAR || USE_LOCK_PUSH || USE_LOCK_POP || USE_LOCK_TAKE || USE_LOCK_TRYPASS)

#define WS_CACHE_SIZE 2048

//cache
typedef struct{
  char data[WS_CACHE_SIZE];
  size_t size;
  volatile void* ptr;
  volatile int seq;
}__attribute__((aligned(CACHE_LINE_SIZE))) myth_wscache,*myth_wscache_t;

#if MYTH_USE_ITIMER || MYTH_USE_SIGIO
//If signal-driven I/O is enabled, this option must be defined
#define USE_SIGNAL_CS 1
#else
#define USE_SIGNAL_CS 0
#endif

#if MYTH_USE_IO_THREAD
#define USE_THREAD_CS 1
#else
#define USE_THREAD_CS 0
#endif

//Runqueue data structure
typedef struct myth_thread_queue
{
  /* FIXME: align top/base and allocate only CACHE_LINE_SIZE for them */
#if PAD_MYTH_THREAD_QUEUE_TOP_BASE
  char pad0[CACHE_LINE_SIZE];
#endif
  volatile int top;
  volatile int base;
#if PAD_MYTH_THREAD_QUEUE_TOP_BASE
  char pad1[CACHE_LINE_SIZE];
#endif
  struct myth_thread** ptr;
  int size;
  myth_spinlock_t lock;
#if USE_LOCK || USE_LOCK_ANY
  myth_spinlock_t m_lock;
#endif
#if USE_SIGNAL_CS
  volatile int8_t op_flag;
#endif
#if USE_THREAD_CS
  pthread_mutex_t mtx;
#endif
  myth_wscache wc;
}myth_thread_queue,*myth_thread_queue_t;


static inline void myth_queue_init(myth_thread_queue_t q);
static inline void myth_queue_fini(myth_thread_queue_t q);
static inline void myth_queue_clear(myth_thread_queue_t q);
static inline void myth_queue_push(myth_thread_queue_t q,struct myth_thread *th);
static inline struct myth_thread* myth_queue_pop(myth_thread_queue_t q);
static inline void myth_queue_put(myth_thread_queue_t q,myth_thread_t th);
static inline struct myth_thread* myth_queue_take(myth_thread_queue_t q);
static inline int myth_queue_trypass(myth_thread_queue_t q,struct myth_thread* th);
static inline void myth_queue_pass(myth_thread_queue_t q,struct myth_thread* th);

static inline int myth_queue_is_operating(myth_thread_queue_t q);

#endif /* MYTH_QUEUE_H_ */
