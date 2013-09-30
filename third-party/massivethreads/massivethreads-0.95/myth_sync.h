#ifndef MYTH_SYNC_H_
#define MYTH_SYNC_H_

#include "myth_desc.h"

//Barrier data structure
typedef struct myth_barrier
{
	int rest;
	int nthreads;
	struct myth_thread **th;
	myth_internal_lock_t lock;
}myth_barrier,*myth_barrier_t;

#define MYTH_BARRIER_SERIAL_THREAD PTHREAD_BARRIER_SERIAL_THREAD

//Join counter data structure
typedef struct myth_jc
{
	struct myth_thread *th;
	intptr_t count;
	myth_internal_lock_t lock;
	int initial;
}myth_jc,*myth_jc_t;

#define DEFAULT_FESIZE 10

//Mutex data structure
typedef struct myth_mutex
{
	myth_internal_lock_t lock;
#ifndef MYTH_MUTEX_BY_SPIN
	struct myth_thread **wl;
	int wsize;
	int nw;
	int locked;
	struct myth_thread *def_wl[DEFAULT_FESIZE];
#endif
}__attribute__((aligned(CACHE_LINE_SIZE))) myth_mutex,*myth_mutex_t;

//Full/empty lock data structure
typedef struct myth_felock
{
	myth_mutex lock;
	struct myth_thread **fl;//full-waiting list
	struct myth_thread **el;//empty-waiting list
	volatile int fe;//full/empty
	int fsize;
	int esize;
	int nf;
	int ne;
	struct myth_thread *def_fl[DEFAULT_FESIZE],*def_el[DEFAULT_FESIZE];
}__attribute__((aligned(CACHE_LINE_SIZE))) myth_felock,*myth_felock_t;

//Conditional variable data structure
typedef struct myth_cond
{
	struct myth_thread **wl;
	int wsize;
	int nw;
	struct myth_thread *def_wl[DEFAULT_FESIZE];
}__attribute__((aligned(CACHE_LINE_SIZE))) myth_cond,*myth_cond_t;

#endif /* MYTH_SYNC_H_ */
