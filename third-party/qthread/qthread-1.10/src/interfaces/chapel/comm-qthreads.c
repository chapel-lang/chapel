//
// Qthreads implementation of Chapel communication interface
//
// Copyright 2011 Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000, there is a non-exclusive license for use of this work by
// or on behalf of the U.S. Government. Export of this program may require a
// license from the United States Government
//

// For SVID definitions (setenv)
#define _SVID_SOURCE

// XXX: Workaround for problems with "" escaping
#undef CHPL_TASKS_MODEL_H
#undef CHPL_THREADS_MODEL_H
#define CHPL_TASKS_MODEL_H "tasks-qthreads.h"
#define CHPL_THREADS_MODEL_H "threads-none.h"

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "chplrt.h"
#include "chplsys.h"
#include "chpl-mem.h"
#include "tasks-qthreads.h" // for sync typedefs
#include "arg.h" // for chpl_specify_locales_error()
#include "chplsys.h"
#include "comm-qthreads.h"
#include "chpl-tasks.h"
#include "chplcgfns.h" // for chpl_ftable()
#include "config.h"   // for chpl_config_get_value()
#include "error.h"    // for chpl_warning()
#include <stdio.h>
#include <stdlib.h> // for setenv()
#include <assert.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include "qthread/qthread.h"
#include "qthread/qtimer.h"
#include "qt_atomics.h" /* for SPINLOCK_BODY() */
#include "qt_envariables.h"
#include "qt_debug.h"

#include "qthread/multinode.h"
#include "qthread/spr.h"

#ifdef CHAPEL_PROFILE
# define PROFILE_INCR(counter,count) do { (void)qthread_incr(&counter,count); } while (0)

# define PROFILE_BIN_CNT 18
# define PROFILE_BIN_INCR(counter,size) \
    do { \
        size_t i; \
        for (i = 0; i < PROFILE_BIN_CNT-1; i++) { \
            size_t bin_size = 8 << i; \
            if (size <= bin_size) { \
                (void)qthread_incr(&counter[i],1); \
                break; \
            } \
        } \
        if (i == PROFILE_BIN_CNT) { \
            (void)qthread_incr(&counter[PROFILE_BIN_CNT-1],1); \
        } \
    } \
    while (0)
# define PROFILE_BIN_PRINT(counter) \
    do { \
        for (size_t i = 0; i < PROFILE_BIN_CNT-1; i++) { \
            fprintf(stderr, "comm put size %lu: %lu\n", 8<<i, counter[i]); \
        } \
        fprintf(stderr, "comm put size large: %lu\n", counter[PROFILE_BIN_CNT-1]); \
    } while (0)

static aligned_t profile_comm_broadcast_private = 0;
static aligned_t profile_comm_barrier = 0;
static aligned_t profile_comm_put = 0;
static aligned_t profile_comm_put_msg_size[PROFILE_BIN_CNT] = {0};
static aligned_t profile_comm_get = 0;
static aligned_t profile_comm_get_msg_size[PROFILE_BIN_CNT] = {0};
static aligned_t profile_comm_fork = 0;
static aligned_t profile_comm_fork_size[PROFILE_BIN_CNT] = {0};
static aligned_t profile_comm_fork_nb = 0;
static aligned_t profile_comm_fork_nb_size[PROFILE_BIN_CNT] = {0};
static aligned_t profile_comm_fork_fast = 0;
static aligned_t profile_comm_fork_fast_size[PROFILE_BIN_CNT] = {0};

static void profile_print(void)
{
    fprintf(stderr, "comm broadcast_private: %lu\n", (unsigned long)profile_comm_broadcast_private);
    fprintf(stderr, "comm barrier: %lu\n", (unsigned long)profile_comm_barrier);
    fprintf(stderr, "comm put: %lu\n", (unsigned long)profile_comm_put);
    PROFILE_BIN_PRINT(profile_comm_put_msg_size);
    fprintf(stderr, "comm get: %lu\n", (unsigned long)profile_comm_get);
    PROFILE_BIN_PRINT(profile_comm_get_msg_size);
    fprintf(stderr, "comm fork: %lu\n", (unsigned long)profile_comm_fork);
    PROFILE_BIN_PRINT(profile_comm_fork_size);
    fprintf(stderr, "comm fork_nb: %lu\n", (unsigned long)profile_comm_fork_nb);
    PROFILE_BIN_PRINT(profile_comm_fork_nb_size);
    fprintf(stderr, "comm fork_fast: %lu\n", (unsigned long)profile_comm_fork_fast);
    PROFILE_BIN_PRINT(profile_comm_fork_fast_size);
}
#else /* ! CHAPEL_PROFILE */
# define PROFILE_INCR(counter,count)
# define PROFILE_BIN_INCR(counter,size)
# define PROFILE_BIN_PRINT(counter)
#endif /* ! CHAPEL_PROFILE */

/* "Segment" information table */
typedef struct seginfo_s {
    void * addr;
    size_t size;
} seginfo_t;

static seginfo_t * seginfo_table = NULL;

/******************************************************************************
 * Remote Actions                                                             *
 ******************************************************************************/

static aligned_t spawn_wrapper(void *arg);
static aligned_t bcast_seginfo(void *arg_);
static aligned_t bcast_private(void *arg_);

static qthread_f
chapel_remote_functions[4] = {
    spawn_wrapper,
    bcast_seginfo,
    bcast_private,
    NULL};

aligned_t bcast_seginfo(void *arg_)
{
    seginfo_t * buf = (seginfo_t *)arg_;

    assert(buf);
    assert(seginfo_table);

    qthread_debug(CHAPEL_DETAILS, "[%d] buf=%p buf[0].addr=%p, buf[0].size=%d\n", chpl_localeID, buf, buf[0].addr, buf[0].size);

    memcpy(seginfo_table, buf, chpl_numLocales * sizeof(seginfo_t));

    qthread_debug(CHAPEL_DETAILS, "[%d] seginfo_table=%p seginfo_table[0].addr=%p, seginfo_table[0].size=%d\n", chpl_localeID, seginfo_table, seginfo_table[0].addr, seginfo_table[0].size);

    return 0;
}

typedef struct bcast_private_args_s {
    int id;
    int32_t size;
    uint8_t data[];
} bcast_private_args_t;

aligned_t bcast_private(void *arg_)
{
    bcast_private_args_t * arg = (bcast_private_args_t *)arg_;

    assert(arg);

    qthread_debug(CHAPEL_DETAILS, "[%d] arg={.id=%d; .size=%d; .data=?}\n", chpl_localeID, arg->id, arg->size);

    memcpy(chpl_private_broadcast_table[arg->id], arg->data, arg->size);

    return 0;
}

/******************************************************************************
 * Launcher Methods                                                           *
 ******************************************************************************/

int64_t chpl_comm_default_num_locales(void) {
    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);
    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);

    return chpl_specify_locales_error();
}

void chpl_comm_verify_num_locales(int64_t proposedNumLocales) {
    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);
    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);

    return;
}

/******************************************************************************
 * Initialization                                                             *
 ******************************************************************************/

//
// returns the maximum number of threads that can be handled
// by this communication layer (used to ensure numThreadsPerLocale is
// legal); should return the sentinel value of 0 if the communication
// layer imposes no particular limit on the number of threads.
//
int32_t chpl_comm_getMaxThreads(void)
{
    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);
    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);

    return 0;
}

//
// initializes the communications package
//   set chpl_localeID and chpl_numLocales
// notes:
//   * Called with the argc/argv pair passed to main()
//
void chpl_comm_init(int *argc_p, char ***argv_p)
{
    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);

    // Set stack size >= 8 pages (lower bound derived from experience)
    unsigned long const default_stack_size = 32768;
    unsigned long const stack_size = 
        qt_internal_get_env_num("STACK_SIZE",
                                default_stack_size,
                                default_stack_size);
    char stack_size_str[100] = {0};
    if (default_stack_size > stack_size) {
        snprintf(stack_size_str, 99, "%lu", default_stack_size);
    } else {
        snprintf(stack_size_str, 99, "%lu", stack_size);
    }
    setenv("QT_STACK_SIZE", stack_size_str, 1);

    /* Initialize SPR:                              *
     * - All locales participate in initialization. */
    int const rc = spr_init(SPR_SPMD, chapel_remote_functions);
    assert(SPR_OK == rc);

    /* Record locale info */
    chpl_localeID = spr_locale_id();
    chpl_numLocales = spr_num_locales();

    qthread_debug(CHAPEL_BEHAVIOR, "[%d] initialized SPR with %d locales\n", chpl_localeID, chpl_numLocales);

    /* Set up segment information table */
#undef malloc
    seginfo_table = malloc(chpl_numLocales * sizeof(seginfo_t));
#define malloc dont_use_malloc_use_chpl_mem_allocMany_instead

    if (0 == chpl_localeID) {
        int i;

        int global_table_size = chpl_numGlobalsOnHeap * sizeof(void *) + getpagesize();
#undef malloc
        void * global_table = malloc(global_table_size);
#define malloc dont_use_malloc_use_chpl_mem_allocMany_instead
       
        // Make sure segment is page-aligned.
        seginfo_table[0].addr = ((void *)(((uint8_t *)global_table) +
                                 (((((uintptr_t)global_table) % getpagesize()) == 0) ? 0 :
                                  (getpagesize() - (((uintptr_t)global_table) % getpagesize())))));
        seginfo_table[0].size = global_table_size;

        for (i = 1; i < chpl_numLocales; i++) {
            seginfo_table[i].addr = NULL;
            seginfo_table[i].size = 0;
        }
    }

    chpl_comm_barrier("waiting for seginfo table setup at root");

    // Broadcast segment info
    if (0 == chpl_localeID) {
        int i;
        aligned_t rets[chpl_numLocales];
        for (i = 1; i < chpl_numLocales; i++) {
            qthread_fork_remote(bcast_seginfo, seginfo_table, &rets[i], i,
                                chpl_numLocales * sizeof(seginfo_t));
        }
        for (i = 1; i < chpl_numLocales; i++) {
            qthread_readFF(&rets[i], &rets[i]);
        }
    }

    chpl_comm_barrier("waiting for seginfo table bcast");

    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);
}

//
// Allow the communication layer to do any secondary initialization it needs
// to, after the memory layer is initialized.
//
void chpl_comm_post_mem_init(void)
{ 
    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);
    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);
}

//
// if possible, run in gdb (because the user threw the --gdb flag)
// using argc and argv.  gdbArgnum gives the index of the argv[]
// element containing the --gdb flag.  Return the status of that
// process in "status" and return 1 if it was possible to run in gdb,
// 0 otherwise
//
int32_t chpl_comm_run_in_gdb(int argc, char * argv[],
                             int gdbArgnum, int * status)
{
    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);
    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);

    return 0;
}

//
// Allow the communication layer to do any further initialization it
// needs to, after the tasking layer is initialized.
//
void chpl_comm_post_task_init(void)
{
    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);
    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);
}

//
// a final comm layer stub before barrier synching and calling into
// the user code.  It is recommended that a debugging message be
// printed here indicating that each locale has started using
// chpl_msg() and a verbosity level of 2 (which will cause it to be
// displayed using the -v flag).
//
// Cannot call spr_unify() here because there is a barrier afterwards.
void chpl_comm_rollcall(void)
{
    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);

    chpl_msg(2, "executing on locale %d of %d locale(s): %s\n", chpl_localeID,
             chpl_numLocales, chpl_localeName());

    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);
}

//
// Inform callers as to the communication layer's desired starting address
// and length for the shared heap, if any.
//
void chpl_comm_desired_shared_heap(void** start_p, size_t* size_p)
{
    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);

    *start_p = NULL;
    *size_p  = 0;

    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);
}

//
// allocate chpl_globals_registry or make it point to
// chpl_globals_registry_static depending on the communication layer
//
void chpl_comm_alloc_registry(int numGlobals)
{
    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);

    // Use the statically allocate array defined in generated C code.
    chpl_globals_registry = chpl_globals_registry_static;

    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);
}

//
// chpl_globals_registry is an array of pointers to addresses; on
// locale 0, these addresses point locally to a class; they are
// uninitialized elsewhere.  This function makes it so that the
// addresses on all other locales are the same as on locale 0.
//
// This function is called collectively.
//
void chpl_comm_broadcast_global_vars(int numGlobals)
{
    int i;

    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);

    if (0 != chpl_localeID) {
        for (i = 0; i < numGlobals; i++) {
            chpl_comm_get(chpl_globals_registry[i], 0,
                          &((void **)seginfo_table[0].addr)[i],
                          sizeof(void *), -1, 1, 0, "");
        }
    }

    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);
}

//
// Broadcast the value of 'id'th entry in chpl_private_broadcast_table
// on the calling locale onto every other locale.  This is done to set
// up global constants of simple scalar types (primarily).
//
void chpl_comm_broadcast_private(int id, int32_t size, int32_t tid)
{
    int i;
    bcast_private_args_t *payload;

    PROFILE_INCR(profile_comm_broadcast_private,1);

    qthread_debug(CHAPEL_CALLS, "[%d] begin id=%d, size=%d, tid=%d\n", chpl_localeID, id, size, tid);

    payload = chpl_mem_allocMany(1, sizeof(bcast_private_args_t) + size, 
                                 CHPL_RT_MD_COMM_PRIVATE_BROADCAST_DATA, 0, 0);
    payload->id = id;
    payload->size = size;
    memcpy(payload->data, chpl_private_broadcast_table[id], size);

    qthread_debug(CHAPEL_DETAILS, "[%d] payload={.id=%d; .size=%d; .data=?}\n", chpl_localeID, payload->id, payload->size);

    aligned_t rets[chpl_numLocales];
    for (i = 0; i < chpl_numLocales; i++) {
        if (i != chpl_localeID) {
            qthread_fork_remote(bcast_private, payload, &rets[i], i,
                                sizeof(bcast_private_args_t) + size);
        }
    }
    for (i = 0; i < chpl_numLocales; i++) {
        if (i != chpl_localeID) {
            qthread_readFF(&rets[i], &rets[i]);
        }
    }

    chpl_mem_free(payload,0,0);

    qthread_debug(CHAPEL_CALLS, "[%d] end id=%d, size=%d, tid=%d\n", chpl_localeID, id, size, tid);
}

void chpl_comm_barrier(const char *msg)
{
    qthread_debug(CHAPEL_CALLS, "[%d] begin: %s\n", chpl_localeID, msg);
    PROFILE_INCR(profile_comm_barrier,1);

    spr_locale_barrier();

    qthread_debug(CHAPEL_CALLS, "[%d] end: %s\n", chpl_localeID, msg);
}

//
// Do exit processing that has to occur before the tasking layer is
// shut down.  "The "all" parameter is true for normal, collective
// program termination, and false for that done by a single locale
// due to error or a user program halt or exit.
//
// Note: Chapel's program termination is not yet fully thought out.
//
void chpl_comm_pre_task_exit(int all)
{
    qthread_debug(CHAPEL_CALLS, "[%d] begin all=%d\n", chpl_localeID, all);

    if (all) {
        if (0 != chpl_localeID) {
            qthread_debug(CHAPEL_BEHAVIOR, "[%d] calling spr_unify()\n", chpl_localeID);

            int const rc = spr_unify();
            assert(SPR_OK == rc);
        } else {
            qthread_debug(CHAPEL_BEHAVIOR, "[%d] initiating locale shutdown\n", chpl_localeID);
        }
    }

#ifdef CHAPEL_PROFILE
    profile_print();
#endif /* CHAPEL_PROFILE */

    qthread_debug(CHAPEL_BEHAVIOR, "[%d] locale shutting down\n", chpl_localeID);

    qthread_debug(CHAPEL_CALLS, "[%d] end all=%d\n", chpl_localeID, all);
}

//
// Terminate the communication layer.  "The "all" parameter is true
// for normal, collective program termination, and false for that
// done by a single locale due to error or a user program halt or
// exit.  If the communication layer likes to call exit, the exit
// code is provided using the "status" argument; if it doesn't, it
// can simply return and the Chapel program will call exit().
//
// Notes:
//   this function is called last
//   a barrier is invoked before calling into this function
//   Chapel's program termination is not yet fully thought out.
//
void chpl_comm_exit(int all, int status)
{ 
    qthread_debug(CHAPEL_CALLS, "[%d] begin all=%d, status=%d\n", chpl_localeID, all, status);
    qthread_debug(CHAPEL_CALLS, "[%d] end all=%d, status=%d\n", chpl_localeID, all, status);
}

/******************************************************************************
 * Data Movement: put                                                         *
 ******************************************************************************/

//
// put 'size' bytes of local data at 'addr' to remote data at
// 'raddr' on locale 'locale'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  chpl_comm_put(void* addr, int32_t locale, void* raddr,
                    int32_t elemSize, int32_t typeIndex, int32_t len,
                    int ln, chpl_string fn)
{
    uint32_t const size = elemSize * len;

    PROFILE_INCR(profile_comm_put,1);
    PROFILE_BIN_INCR(profile_comm_put_msg_size,elemSize);

    qthread_debug(CHAPEL_CALLS, "[%d] begin addr=%p, locale=%d, raddr=%p, elemSize=%d, len=%d\n", chpl_localeID, addr, locale, raddr, elemSize, len);
    qthread_debug(CHAPEL_BEHAVIOR, "[%d] putting from %p to (%d,%p) of size %d\n", chpl_localeID, addr, locale, raddr, size);

    if (chpl_localeID == locale) {
        memmove(raddr, addr, size);
    } else {
        int const rc = spr_put(locale, raddr, addr, size);
        assert(SPR_OK == rc);
    }

    qthread_debug(CHAPEL_CALLS, "[%d] end addr=%p, locale=%d, raddr=%p, elemSize=%d, len=%d\n", chpl_localeID, addr, locale, raddr, elemSize, len);
}

//
// get 'size' bytes of remote data at 'raddr' on locale 'locale' to
// local data at 'addr'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  chpl_comm_get(void *addr, int32_t locale, void* raddr,
                    int32_t elemSize, int32_t typeIndex, int32_t len,
                    int ln, chpl_string fn)
{
    uint32_t const size = elemSize *len;
  
    PROFILE_INCR(profile_comm_get,1);
    PROFILE_BIN_INCR(profile_comm_get_msg_size,elemSize);

    qthread_debug(CHAPEL_CALLS, "[%d] begin addr=%p, locale=%d, raddr=%p, elemSize=%d, len=%d\n", chpl_localeID, addr, locale, raddr, elemSize, len);
    qthread_debug(CHAPEL_BEHAVIOR, "[%d] getting from (%d,%p) to %p of size %d\n", chpl_localeID, locale, raddr, addr, size);

    if (chpl_localeID == locale) {
        memcpy(addr, raddr, size);
    } else {
        int const rc = spr_get(addr, locale, raddr, size);
        assert(SPR_OK == rc);
    }

    qthread_debug(CHAPEL_CALLS, "[%d] end addr=%p, locale=%d, raddr=%p, elemSize=%d, len=%d\n", chpl_localeID, addr, locale, raddr, elemSize, len);
}

/******************************************************************************
 * Remote Spawning                                                            *
 ******************************************************************************/

typedef struct {
    chpl_bool     serial_state;
    chpl_fn_int_t fid;
    int           arg_size;
    uint8_t       arg[0];
} spawn_wrapper_args_t;

static aligned_t spawn_wrapper(void *arg)
{
    spawn_wrapper_args_t * const rarg = arg;

    qthread_debug(CHAPEL_BEHAVIOR, "[%d] executing fn %d with arg-size %d\n", chpl_localeID, rarg->fid, rarg->arg_size);

    chpl_task_setSerial(rarg->serial_state);
    (*chpl_ftable[rarg->fid])(rarg->arg);

    return 0;
}

static inline void spawn(int locale, chpl_fn_int_t fid, void *arg, 
                                int32_t arg_size, int32_t arg_tid, aligned_t *ret)
{
    qthread_debug(CHAPEL_CALLS, "[%d] begin: locale=%d, fid=%d, arg_size=%d\n", chpl_localeID, locale, fid, arg_size);

        spawn_wrapper_args_t *wargs;
    size_t const wargs_size = sizeof(spawn_wrapper_args_t) + arg_size;

    wargs = (spawn_wrapper_args_t *)chpl_mem_allocMany(1, wargs_size, CHPL_RT_MD_COMM_FORK_SEND_INFO, 0, 0);
        wargs->serial_state = chpl_task_getSerial();
    wargs->fid = fid;
    wargs->arg_size = arg_size;
    memcpy(&(wargs->arg), arg, arg_size);

    if (chpl_localeID == locale) {
        int const rc = qthread_fork_copyargs(spawn_wrapper, wargs, wargs_size, ret);
        assert(QTHREAD_SUCCESS == rc);
    } else {
        int const rc = qthread_fork_remote(spawn_wrapper, wargs, ret, locale, wargs_size);
        assert(SPR_OK == rc);
    }

    chpl_mem_free(wargs, 0, NULL);

    qthread_debug(CHAPEL_CALLS, "[%d] end: locale=%d, fid=%d, arg_size=%d\n", chpl_localeID, locale, fid, arg_size);
}

//
// remote fork should launch a thread on locale that runs function f
// passing it arg where the size of arg is stored in arg_size
// notes:
//   multiple forks to the same locale should be handled concurrently
//
void chpl_comm_fork(int locale, chpl_fn_int_t fid,
                    void *arg, int32_t arg_size, int32_t arg_tid)
{
    aligned_t ret;

    PROFILE_INCR(profile_comm_fork,1);
    PROFILE_BIN_INCR(profile_comm_fork_size,arg_size);

    qthread_debug(CHAPEL_CALLS, "[%d] begin locale=%d, fid=%d, arg_size=%d\n", chpl_localeID, locale, fid, arg_size);

    qthread_debug(CHAPEL_BEHAVIOR, "[%d] (blocking) forking fn %d with arg-size %d\n", chpl_localeID, fid, arg_size);

    qthread_empty(&ret);
    spawn(locale, fid, arg, arg_size, arg_tid, &ret);
    qthread_readFF(NULL, &ret);

    qthread_debug(CHAPEL_CALLS, "[%d] end locale=%d, fid=%d, arg_size=%d\n", chpl_localeID, locale, fid, arg_size);
}

//
// non-blocking fork
//
void chpl_comm_fork_nb(int locale, chpl_fn_int_t fid,
                       void *arg, int32_t arg_size, int32_t arg_tid)
{
    PROFILE_INCR(profile_comm_fork_nb,1);
    PROFILE_BIN_INCR(profile_comm_fork_nb_size,arg_size);

    qthread_debug(CHAPEL_CALLS, "[%d] begin locale=%d, fid=%d, arg_size=%d\n", chpl_localeID, locale, fid, arg_size);
    qthread_debug(CHAPEL_BEHAVIOR, "[%d] (non-blocking) forking fn %d with arg-size %d\n", chpl_localeID, fid, arg_size);

    spawn(locale, fid, arg, arg_size, arg_tid, NULL);

    qthread_debug(CHAPEL_CALLS, "[%d] end locale=%d, fid=%d, arg_size=%d\n", chpl_localeID, locale, fid, arg_size);
}

//
// fast (non-forking) fork (i.e., run in handler)
//
// TODO: implement fast fork in progress thread
void chpl_comm_fork_fast(int locale, chpl_fn_int_t fid, void *arg,
                         int32_t arg_size, int32_t arg_tid)
{
    PROFILE_INCR(profile_comm_fork_fast,1);
    PROFILE_BIN_INCR(profile_comm_fork_fast_size,arg_size);

    qthread_debug(CHAPEL_CALLS, "[%d] begin locale=%d, fid=%d, arg_size=%d\n", chpl_localeID, locale, fid, arg_size);
    qthread_debug(CHAPEL_BEHAVIOR, "[%d] (fast) forking fn %d with arg-size %d\n", chpl_localeID, fid, arg_size);

    chpl_comm_fork(locale, fid, arg, arg_size, arg_tid);
    
    qthread_debug(CHAPEL_CALLS, "[%d] end locale=%d, fid=%d, arg_size=%d\n", chpl_localeID, locale, fid, arg_size);
}

//
// This call specifies the number of polling tasks that the
// communication layer will need (see just below for a definition).
// The value it returns is passed to chpl_task_init(), in order to
// forewarn the tasking layer whether the comm layer will need a
// polling task.  In the current implementation, it should only
// return 0 or 1.
//
int chpl_comm_numPollingTasks(void)
{
    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);
    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);

    return 1;
}

//
// Segment info table setup helper - based on GASnet implementation
//
void chpl_comm_spr_help_register_global_var(int i, void * addr) {
    qthread_debug(CHAPEL_CALLS, "[%d] begin\n", chpl_localeID);

    if (chpl_localeID == 0) {
        ((void **)seginfo_table[0].addr)[i] = addr;
    }

    qthread_debug(CHAPEL_CALLS, "[%d] end\n", chpl_localeID);
}
