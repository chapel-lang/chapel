#pragma once
#ifndef MYTH_CONFIG_H_
#define MYTH_CONFIG_H_

#include "config.h"

//Enable debug
//#define MYTH_DEBUG 1

/* debug join with fcc */
//#define MYTH_DEBUG_JOIN_FCC 1

/* TODO: get rid of them, move them in configure */
#define MYTH_ECO_TEST 0
#define MYTH_ECO_CIRCLE_STEAL 0

#define MYTH_PAUSE 0

//Enable debug for eco-mode

//Cache line size
#define CACHE_LINE_SIZE 64

//Page size
#define PAGE_SIZE 4096

//TLS key size
#define MYTH_TLS_KEY_SIZE 256

#define USE_STACK_GUARDPAGE 0

//How many stack blocks allocated at stack allocation
//#define STACK_ALLOC_UNIT 128
#define STACK_ALLOC_UNIT 1
#define INITIAL_STACK_ALLOC_UNIT 0

//Use malloc+freelist instead of mmap+freelist
#define ALLOCATE_STACK_BY_MALLOC 0

//Split desc and stack allocation
#define MYTH_SPLIT_STACK_DESC 1

//Runqueue length
#define INITIAL_QUEUE_SIZE (65536*2)

//Wrap and multipelx I/O functions
#define MYTH_WRAP_SOCKIO 0

//Quick emptiness check on io wait list
#define QUICK_CHECK_IO_WAIT_LIST 1

//Quick emptiness check on io fd list
#define QUICK_CHECK_IO_FD_LIST 1

//Wrapped select() return immediately regardless of I/O readiness
#define SELECT_ALWAYS_RETURN_IMMEDIATELY 1

//Replace myth_assert() with assert(): used for debugging
#define MYTH_SANITY_CHECK 1

//Insert special instruction into unreachable code: used for debugging
#define USE_MYTH_UNREACHABLE 1

//epoll file decriptor size
#define MYTH_EPOLL_SIZE 256

//Decide a fd's worker thread at random
#define MYTH_RANDOM_IO_WORKER 1
//Make all fds belong to worker thread 0
//#define MYTH_ONE_IO_WORKER 1

// set 1 to use interval timer for I/O checking and preemption
#define MYTH_USE_ITIMER 0
// set 1 to use SIGIO for I/O checking
#define MYTH_USE_SIGIO 0

//Call myth_fini automatically on exit
//#define MYTH_CLEANUP_ON_EXIT 1

// set 1 to use signal handler
#define MYTH_USE_SIGHANDLER 0
// set 1 to use another thread
#define MYTH_USE_IO_THREAD 0
#define MYTH_IO_THREAD_PERIOD 1

//Debug log setting
#define FREE_MYTH_THREAD_STRUCT_DEBUG 0
#define MYTH_INIT_DEBUG 0
#define MYTH_WORKER_INIT_DEBUG 0
#define MYTH_FINI_DEBUG 0
#define MYTH_YIELD_DEBUG 0
#define MYTH_JOIN_DEBUG 0
#define MYTH_SCHED_ADD_DEBUG 0
#define MYTH_SCHED_LOOP_DEBUG 0
#define MYTH_ENTRY_POINT_DEBUG 0

//Profiling option
#define MYTH_CREATE_PROF 0
#define MYTH_CREATE_PROF_DETAIL 0
#define MYTH_ENTRY_POINT_PROF 0
#define MYTH_EP_PROF_DETAIL 0
#define MYTH_JOIN_PROF 0
#define MYTH_JOIN_PROF_DETAIL 0
#define MYTH_WS_PROF_DETAIL 0
#define MYTH_ALLOC_PROF 0
#define MYTH_SWITCH_PROF 0
#define MYTH_IO_PROF_DETAIL 0
#define MYTH_FL_PROF 0
//Display result as CSV
#define MYTH_PROF_COUNT_CSV 0
//Display result of each worker thread
#define MYTH_PROF_SHOW_WORKER 0

//Profile myth_flmalloc
#define MYTH_FLMALLOC_PROF 0

//Collect and emit sslog
#define MYTH_COLLECT_LOG 0
//Log buffer size
#define MYTH_LOG_INITIAL_BUFFER_SIZE 1024
//Collect context switching events
#define MYTH_COLLECT_CONTEXT_SWITCH 1

//Enable setting the name for each thread
//By default thread-specific name (<memory address of thread descriptor>@<recycle count>) is assigned,
//so huge number of categories are generated.
//Thus it is strongly recommended to annotate by hand for each thread when enabling this option
#define MYTH_ENABLE_THREAD_ANNOTATION 0

#define MYTH_LOG_MERGE_SAME_NAME_THREADS 1

//Maximum length for thread annotation
#define MYTH_THREAD_ANNOTATION_MAXLEN 100

/* --------------------
   Barrier implementation choices
   -------------------- */
#define MYTH_BARRIER_FENCES    2
#define MYTH_BARRIER_CILK      3
#define MYTH_BARRIER_CILK_WEAK 4
#define MYTH_BARRIER_INTRINSIC 5
#define MYTH_BARRIER_NONE      6

#define MYTH_BARRIER MYTH_BARRIER_CILK

/* --------------------
   WENV implementation choices 
   -------------------- */
#define WENV_IMPL_PTHREAD 2
#define WENV_IMPL_ELF     3
#define WENV_IMPL_NONE    4

#define WENV_IMPL WENV_IMPL_ELF

//Choose work stealing target at random
#define WS_TARGET_RANDOM 1

//Switch to new thread immediately after it is created
#define SWITCH_AFTER_CREATE 1
//When a thread is finished, switch to the thread that waits for it.
#define SWITCH_AFTER_EXIT 1

//Scheduling policy of runqueue
#define MYTH_QUEUE_LIFO 1

//Confirm if a thread descriptor is used in two threads at the same time.
#define MYTH_DESC_REUSE_CHECK 0

#define PAD_MYTH_THREAD_QUEUE_TOP_BASE 1

//Check the number of threads in the local runqueue before poping
//turned out very important; never turn off
#define QUICK_CHECK_ON_POP 1

//Check the number of threads in the victim's runqueue before 
//seriously trying to steal work 
//turned out very important; never turn off
#define QUICK_CHECK_ON_STEAL 1

//Check whether the thread is sure to be ready for release before acquiring the lock
#define QUICK_CHECK_ON_JOIN 0

//Do trylock before work stealing
//#define TRY_LOCK_BEFORE_STEAL 1

//Use malloc with recycle by freelist
#define USE_MYTH_FLMALLOC 1
//Use TLS on malloc with freelist
#define MYTH_FLMALLOC_TLS 0

//Inline context switching codes by inline assembler
#define MYTH_INLINE_CONTEXT 0
//At inlined codes, save callee-saved registers explicitly
#define MYTH_INLINE_PUSH_CALLEE_SAVED 1

//To switch context, use jmp instruction instead of ret
#define USE_JUMP_INSN_A 1
#define USE_JUMP_INSN_B 1

// force to use ucontext
//#define MYTH_FORCE_UCONTEXT 1

/* ------------------
   MYTH_ARCH choices
   ------------------ */

#define MYTH_ARCH_UNIVERSAL 2
#define MYTH_ARCH_i386      3
#define MYTH_ARCH_amd64     4 
#define MYTH_ARCH_amd64_knc 5
#define MYTH_ARCH_sparc_v9  6
#define MYTH_ARCH_sparc_v8  7

// set to 1 to force the universal version
#define MYTH_FORCE_ARCH_UNIVERSAL 0

#if MYTH_FORCE_ARCH_UNIVERSAL
#define MYTH_ARCH MYTH_ARCH_UNIVERSAL

#elif defined(__i386__)
#define MYTH_ARCH MYTH_ARCH_i386

#elif defined(__x86_64__)
#if defined(__MIC__) || defined(__KNC__)
#define MYTH_ARCH MYTH_ARCH_amd64_knc
#else
#define MYTH_ARCH MYTH_ARCH_amd64
#endif

#elif defined(__sparc__)
#if defined(__arch64__)
#define MYTH_ARCH MYTH_ARCH_sparc_v9
#else
#define MYTH_ARCH MYTH_ARCH_sparc_v8
#endif
/* as it's not implemented on this platform;
   TODO: may be we should abandon MYTH_INLINE_CONTEXT altogether */
#define MYTH_INLINE_CONTEXT 0

#else
#define MYTH_ARCH MYTH_ARCH_UNIVERSAL
/* as it's not implemented on this platform;
   TODO: may be we should abandon MYTH_INLINE_CONTEXT altogether */
#define MYTH_INLINE_CONTEXT 0

#endif

/* I don't know what it is for.
   on amd64 platforms, when this is set, myth_contxt_func.h 
   defines two macros PUSH_FPCSR() and POP_FPCSR() so that 
   they save/restore mxcsr register and fpu control register 
   upon context switches.
   it's been a while since they are not used and it seems fine.
   I don't know if there is any point in maintaining this
   (and the two macros). but until we are absolutely sure, 
   leave these two macros (and do not use them) */
#define MYTH_SAVE_FPCSR 0

/* ------------------
   MYTH_CONTEXT choices
   ------------------ */

#define MYTH_CONTEXT_UCONTEXT  2 
#define MYTH_CONTEXT_i386      3
#define MYTH_CONTEXT_amd64     4
#define MYTH_CONTEXT_amd64_knc 5
#define MYTH_CONTEXT_sparc_v9  6
#define MYTH_CONTEXT_sparc_v8  7

/* ------------------
   MYTH_WRAP choices
   ------------------ */
#define MYTH_WRAP_VANILLA 2
#define MYTH_WRAP_DL      3
#define MYTH_WRAP_LD      4

#endif /* MYTH_CONFIG_H_ */
