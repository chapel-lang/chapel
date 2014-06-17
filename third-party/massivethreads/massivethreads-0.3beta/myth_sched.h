
#ifndef _MYTH_SCHED_H_
#define _MYTH_SCHED_H_
#include <stdint.h>

#include "myth_context.h"

//Profiling data
typedef struct myth_prof_data
{
#ifdef MYTH_CREATE_PROF
	uint64_t create_cnt,create_cycles;
	uint64_t create_cycles_tmp;
#endif
#ifdef MYTH_CREATE_PROF_DETAIL
	uint64_t create_d_cnt,create_d_tmp;
	uint64_t create_alloc,create_switch,create_push;
#endif
#ifdef MYTH_ENTRY_POINT_PROF
	uint64_t ep_cnt;
	uint64_t ep_cyclesA,ep_cyclesB;
	uint64_t ep_cycles_tmp;
#endif
#ifdef MYTH_EP_PROF_DETAIL
	uint64_t ep_d_cnt,ep_d_tmp;
	uint64_t ep_join,ep_switch,ep_pop;
#endif
#ifdef MYTH_JOIN_PROF
	uint64_t join_cnt,join_cycles;
#endif
#ifdef MYTH_JOIN_PROF_DETAIL
	uint64_t join_d_cnt;
	uint64_t join_join,join_release;
#endif
#ifdef MYTH_WS_PROF_DETAIL
	uint64_t ws_hit_cnt;
	uint64_t ws_hit_cycles;
	uint64_t ws_miss_cnt;
	uint64_t ws_miss_cycles;
	uint64_t *ws_attempt_count;
#endif
#ifdef MYTH_SWITCH_PROF
	uint64_t sw_cnt;
	uint64_t sw_cycles;
	uint64_t sw_tmp;
#endif
#ifdef MYTH_ALLOC_PROF
	uint64_t alloc_cnt;
	uint64_t malloc_cnt;
	uint64_t malloc_cycles;
	uint64_t addlist_cycles;
	uint64_t dalloc_cnt;
	uint64_t dmalloc_cnt;
	uint64_t dmalloc_cycles;
	uint64_t daddlist_cycles;
	uint64_t salloc_cnt;
	uint64_t smalloc_cnt;
	uint64_t smalloc_cycles;
	uint64_t saddlist_cycles;
#endif
#ifdef MYTH_IO_PROF_DETAIL
	uint64_t io_succ_send_cnt,io_succ_recv_cnt;
	uint64_t io_succ_send_cycles,io_succ_recv_cycles;
	uint64_t io_block_send_cnt,io_block_recv_cnt;
	uint64_t io_block_send_cycles,io_block_recv_cycles;
	uint64_t io_epoll_miss_cycles,io_epoll_miss,io_epoll_hit_cycles,io_epoll_hit;
	uint64_t io_chk_miss_cycles,io_chk_miss,io_chk_hit_cycles,io_chk_hit;
	uint64_t io_block_syscall;
	uint64_t io_genreq;
	uint64_t io_fdmap;
	uint64_t io_rqpop;
	uint64_t io_addlist;
#endif
}myth_prof_data,*myth_prof_data_t;

//Thread scheduler
typedef struct myth_sched
{
	void *stack;//Call stack
	myth_context context;//Scheduler context
}myth_sched,*myth_sched_t;

//External Global variables
extern int g_log_worker_stat;
extern int g_sched_prof;

extern size_t g_default_stack_size;

//Cancel constants, set as the same as those of pthreads
#define MYTH_CANCEL_DEFERRED PTHREAD_CANCEL_DEFERRED
#define MYTH_CANCEL_ENABLE PTHREAD_CANCEL_ENABLE
#define MYTH_CANCEL_DISABLE PTHREAD_CANCEL_DISABLE
#define MYTH_CANCELED PTHREAD_CANCELED

#endif //_MYTH_H_
