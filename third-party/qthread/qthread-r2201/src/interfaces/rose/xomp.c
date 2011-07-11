
//  include libxomp.h from ROSE implementation as the full list
//  of functions needed to support OpenMP 3.0 with Rose.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>		       // for malloc()
#include <stdio.h>


#include <time.h>                      // for omp_get_wtick/wtime
#if defined(CLOCK_REALTIME) || defined(CLOCK_MONOTONIC)
// Otherwise get tick resolution from sysconf
#include <unistd.h>                    // for omp_get_wtick
#else
# include <sys/time.h>                 // for gettimeofday()
#endif
#include <string.h>                    // for strcmp

#include "qthread/qthread.h"
#include "qthread/qtimer.h"
#include "qt_barrier.h"	               // for qt_global_barrier
#include "qt_arrive_first.h"           // for qt_global_arrive_first
#include "qthread/qloop.h"	       // for qt_loop_f
#include "qthread_innards.h"	       // for qthread_debug()
#include "qt_shepherd_innards.h"       // for qthread_shepherd_t
#include "qloop_innards.h"	       // for qqloop_handle_t
#include "qt_qthread_struct.h"	       // for qthread_t
#include "qt_touch.h"		       // for qthread_run_needed_task()
#include <qthread/qthread.h>           // for syncvar_t
#include <qthread/feb_barrier.h>
#include <qthread/omp_defines.h>       // Wrappered OMP functions from omp.h
#include <qt_atomics.h>       // Wrappered OMP functions from omp.h
#ifdef QTHREAD_OMP_AFFINITY
#include <omp_affinity.h>	       // Headers for OMP affinity functions
#endif

#include <rose_xomp.h>

#ifdef QTHREAD_RCRTOOL
#include "rcrtool/qt_rcrtool.h"
#include "maestro_sched.h"
#endif

#if defined(__i386__) || defined(__x86_64__)
#define USE_RDTSC 1
#endif

#define bool unsigned char
#define TRUE 1
#define FALSE 0

void xomp_internal_loop_init(enum qloop_handle_type type,
			     int ordered,
			     void ** loop,
			     int lower,
			     int upper,
			     int stride,
			     int chunk_size);
void xomp_internal_set_ordered_iter(qqloop_step_handle_t *loop, int lower);

int compute_XOMP_block(qqloop_step_handle_t * loop);
void qt_omp_parallel_region_add_loop(qqloop_step_handle_t *loop);
syncvar_t *getSyncTaskVar(void);

typedef enum xomp_nest_level{NO_NEST=0, ALLOW_NEST, AUTO_NEST}xomp_nest_level_t;

static uint64_t *staticStartCount;
static volatile int orderedLoopCount = 0;
QTHREAD_FASTLOCK_TYPE critLock;
syncvar_t XOMP_critical;

#ifdef USE_RDTSC
static uint64_t rdtsc(void);
#endif
int get_nprocs(void); // in sys/sysinfo.h but __FUNCTION__ in that file collides with
                      // the same (un)define in config.h

//
// XOMP_Status
//
// Internal structure used to maintain state of XOMP system
typedef struct XOMP_Status
{
  bool inside_xomp_parallel;
  xomp_nest_level_t allow_xomp_nested_parallel;
  int64_t xomp_nested_parallel_level;
  clock_t start_time;
  qthread_shepherd_id_t num_omp_threads;
  int64_t dynamic;
  enum qloop_handle_type runtime_sched_option;
  aligned_t atomic_lock;
} XOMP_Status;
static XOMP_Status xomp_status;

static void XOMP_Status_init(XOMP_Status *);
static void set_inside_xomp_parallel(XOMP_Status *, bool);
static void set_xomp_dynamic(int, XOMP_Status *);
static int64_t get_xomp_dynamic(XOMP_Status *);
static void xomp_set_nested(XOMP_Status *, bool val);
static xomp_nest_level_t xomp_get_nested(XOMP_Status *);

// Initalize the structure to a known state
static void XOMP_Status_init(
    XOMP_Status *p_status)
{
  p_status->inside_xomp_parallel = FALSE;
  p_status->allow_xomp_nested_parallel = 0;
  p_status->xomp_nested_parallel_level = NO_NEST;
  p_status->start_time = clock();
  p_status->num_omp_threads = qthread_num_shepherds();
  p_status->dynamic = 0;
  p_status->runtime_sched_option = GUIDED_SCHED; // Set default scheduling type
  p_status->atomic_lock = 0;
}

// Get atomic lock address location
static aligned_t *get_atomic_lock(
    XOMP_Status *p_status)
{
	return (&(p_status->atomic_lock));
}

// Set runtime schedule option
static void set_runtime_sched_option(
    XOMP_Status *p_status,
    enum qloop_handle_type sched_option)
{
    p_status->runtime_sched_option = sched_option;
}

// Get runtime schedule option
static enum qloop_handle_type get_runtime_sched_option(
    XOMP_Status *p_status)
{
    return p_status->runtime_sched_option;
}

// Set status of inside parallel section (TRUE || FALSE)
static void set_inside_xomp_parallel(
    XOMP_Status *p_status,
    bool state)
{
  p_status->inside_xomp_parallel = state;
}

// Get status of inside parallel section (return TRUE || FALSE)
static bool get_inside_xomp_parallel(
    XOMP_Status *p_status)
{
  return p_status->inside_xomp_parallel;
}
// increasing level of nesting of OMP parallel regions -- returns new level
static void xomp_set_nested (XOMP_Status *p_status, bool val)
{
  p_status->allow_xomp_nested_parallel =  val;
}

static xomp_nest_level_t xomp_get_nested (XOMP_Status *p_status)
{
  return p_status->allow_xomp_nested_parallel;
}

// Get wtime used by omp
static double XOMP_get_wtime(
    XOMP_Status *p_status)
{
// Copied from libgomp/config/posix/time.c included in gcc-4.4.4
#if defined(CLOCK_REALTIME) || defined(CLOCK_MONOTONIC)
  struct timespec ts;
# ifdef CLOCK_MONOTONIC
  if (clock_gettime (CLOCK_MONOTONIC, &ts) < 0)
# endif
    clock_gettime (CLOCK_REALTIME, &ts);
  return ts.tv_sec + ts.tv_nsec / 1e9;
#else
  struct timeval tv;
  gettimeofday (&tv, NULL);
  return tv.tv_sec + tv.tv_usec / 1e6;
#endif
}

// get wtick used by omp
static double XOMP_get_wtick(
    XOMP_Status *p_status)
{
// Copied from libgomp/config/posix/time.c included in gcc-4.4.4
#if defined(CLOCK_REALTIME) || defined(CLOCK_MONOTONIC)
  struct timespec ts;
# ifdef CLOCK_MONOTONIC
  if (clock_getres (CLOCK_MONOTONIC, &ts) < 0)
# endif
    clock_getres (CLOCK_REALTIME, &ts);
  return ts.tv_sec + ts.tv_nsec / 1e9;
#else
  return 1.0 / sysconf(_SC_CLK_TCK);
#endif
}

// Get dynamic scheduler value
static int64_t get_xomp_dynamic(
    XOMP_Status *p_status)
{
  return p_status->dynamic;
}

// Set dynamic scheduler value
static void set_xomp_dynamic(
    int val,
    XOMP_Status *p_status)
{
  p_status->dynamic = val;
}
//
// END XOMP_Status
//

//
// Setup local variables
//

//
// END Setup local variables 
//

//Runtime library initialization routine
void XOMP_init(
    int argc,
    char **argv)
{
    char *env;  // Used to get Envionment variables
    qthread_initialize();

#ifdef QTHREAD_RCRTOOL
    //Log the XOMP initialization and the application name into the RCRTool RAT Table.
    rcrtool_log(RCR_APP_STATE_DUMP, XOMP_INIT, 0, 0, argv[0]);
#endif

    XOMP_Status_init(&xomp_status);  // Initialize XOMP_Status

    // Process special environment variables
    if ((env=getenv("OMP_SCHEDULE")) != NULL) {
      if (!strcmp(env, "GUIDED_SCHED")) set_runtime_sched_option(&xomp_status, GUIDED_SCHED);
       else if (!strcmp(env, "STATIC_SCHED")) set_runtime_sched_option(&xomp_status, STATIC_SCHED);
       else if (!strcmp(env, "DYNAMIC_SCHED")) set_runtime_sched_option(&xomp_status, DYNAMIC_SCHED);
       else { // Environemnt variable set to something else, we're going to abort
         fprintf(stderr, "OMP_SCHEDULE set to '%s' which is not a valid value, aborting.\n", env);
	 abort();
       }
    }
    if ((env=getenv("OMP_NESTED")) != NULL) {
      if (!strcmp(env, "TRUE")) omp_set_nested(ALLOW_NEST);
      else if (!strcmp(env, "FALSE")) omp_set_nested(NO_NEST);
      else {
	fprintf(stderr, "OMP_NESTED should be TRUE or FALSE set to %s which is not a valid value, aborting.\n", env);
	abort();
      }
    }
    else {
      omp_set_nested(NO_NEST);
    }

    int lim_s = qthread_num_shepherds();
    staticStartCount = calloc(lim_s, sizeof(uint64_t));
    
    qthread_syncvar_fill(&XOMP_critical);

    QTHREAD_FASTLOCK_INIT(critLock);

    if (! staticStartCount){
      fprintf(stderr,"XOMP_init build shepherd aux structure malloc failed\n");
    }
    return;
}

// Runtime library termination routine
void XOMP_terminate(
    int exitcode)
{
  if (qthread_worker(NULL) !=0) {
    qt_move_to_orig();  // for termination need to be on the original thread - 4/1/11 AKP
  }
#ifdef STEAL_PROFILE
  qthread_steal_stat();  // qthread_finalize called by at_exit handler
#endif
  return;
}

// start a parallel task
#ifdef QTHREAD_RCRTOOL
void XOMP_parallel_start(
    void (*func) (void *),
    void *data,
    unsigned ifClause,
    unsigned numThread,
    const char* funcName)
#else
void XOMP_parallel_start(
    void (*func) (void *),
    void *data,
    unsigned ifClause,
    unsigned numThread)
#endif
{
  // allocate block to hold parallel for loop pointer for any loop directly created within this region
  //    --- parallel for loops directly created within other for loops will be handled by passing
  //   this value in as part of the XOMP_loop_*_init function
  if (qt_omp_parallel_region_create()) {
    set_inside_xomp_parallel(&xomp_status, TRUE);
  }

#ifdef QTHREAD_RCRTOOL
    //Here we log entering an open MP section into the RCRTool RAT Table.
    int numberOfThreads = numThread;
    if (numberOfThreads == 0) numberOfThreads = qthread_num_workers();
    rcrtool_log(RCR_APP_STATE_DUMP, XOMP_PARALLEL_START, numberOfThreads, (uint64_t) func, funcName);
#endif

  // allocate and set new feb barrier
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  qthread_shepherd_id_t parallelWidth = qthread_num_workers();
#else
  qthread_shepherd_id_t parallelWidth = qthread_num_shepherds();
#endif
  int save_thread_cnt = 0;  // double duty - non-zero we changed thread count - value is the old thread count
  if ( numThread & (parallelWidth != numThread)) {
    save_thread_cnt = parallelWidth;
    omp_set_num_threads(numThread);
    parallelWidth = numThread;
  }
  qt_loop_step_f f = (qt_loop_step_f) func;
  qt_parallel_step(f, parallelWidth, data);

  if (save_thread_cnt) {
    omp_set_num_threads(save_thread_cnt);
  }

  return;
}

// end a parallel task
void XOMP_parallel_end(
    void)
{
    XOMP_taskwait();

#ifdef QTHREAD_RCRTOOL
    //Here we log leaving an open MP section into the RCRTool RAT Table.
    rcrtool_log(RCR_APP_STATE_DUMP, XOMP_PARALLEL_END, -1, 0, 0);
#endif

    qt_omp_parallel_region_destroy();  //  need to free parallel region and all it contains

    return;
}

static qqloop_step_handle_t *testLoop; // akp - temp needs rose change to pass
                                       // loop value to ordered_start function

qqloop_step_handle_t *qt_loop_rose_queue_create(
    int64_t start,
    int64_t stop,
    int64_t incr)
{
    qqloop_step_handle_t *ret;
    int array_size = 0;
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    array_size = qthread_num_workers(); 
#else
    array_size = qthread_num_shepherds(); 
#endif
    int malloc_size = sizeof(qqloop_step_handle_t) + // base size
      array_size * sizeof(aligned_t) + // static iteration array size
      array_size * sizeof(aligned_t) + // current iteration array size 
      array_size * sizeof(aligned_t);  // table for current_worker array
    ret = (qqloop_step_handle_t *) malloc(malloc_size);
    
    ret->workers = 0;
    ret->next = NULL;
    ret->departed_workers = 0;
    ret->assignNext = start;
    ret->assignStart = start;
    ret->assignStop = stop;
    ret->assignStep = incr;
    ret->assignDone = stop;
    ret->work_array_size = array_size;
    ret->current_workers = ((aligned_t*)&(ret->work_array)) + (2 * array_size);
    // zero work array
    int i; 
    aligned_t * tmp = &ret->work_array;
    for (i = 0; i < 2*array_size; i++) {
      *tmp++ = 0;
    }

    testLoop = ret;
    // add to linked list so that I can delete all the loops when the parallel region
    qt_omp_parallel_region_add_loop(ret);
    return ret;
}

void qt_loop_rose_queue_free(qqloop_step_handle_t * qqloop)
{  
  return;
}

// used to compute time to dynamically determine minimum effective block size 
#ifdef USE_RDTSC
#include <stdint.h>
static uint64_t rdtsc(void);
static QINLINE uint64_t rdtsc() {
uint32_t lo, hi;
__asm__ __volatile__ (      // serialize
"xorl %%eax,%%eax \n        cpuid"
::: "%rax", "%rbx", "%rcx", "%rdx");
/* We cannot use "=A", since this would use %rax on x86_64 */
__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
return (uint64_t)hi << 32 | lo;
}

// NOTE: need to replace these magic numbers with #defs 
#endif

// handle Qthread default openmp loop initialization
void xomp_internal_loop_init(
		    enum qloop_handle_type type,
		    int ordered,
		    void ** loop,
		    int lower,
		    int upper,
		    int stride,
		    int chunk_size)
{
#ifdef QTHREAD_RCRTOOL
    //Here we log entering a loop section.
    rcrtool_log(RCR_APP_STATE_DUMP, XOMP_FOR_LOOP_START, 0, (uint64_t)0, "");
#endif
  qthread_parallel_region_t *pr = qt_parallel_region();

  qqloop_step_handle_t *t = NULL;

  if (pr) { // have active parallel region -- one parallel for loop structure per region
    t = qthread_cas(&pr->forLoop, NULL, -1);
  }
  else { // already within parallel loop -- use loop argument to store loop structure
    t = qthread_cas(loop, NULL, -1);
  }
  
  if (t == NULL) { // am I first?
    t  =  qt_loop_rose_queue_create(lower, upper, stride);
    t->chunkSize = chunk_size;
    t->type = type;
    t->iterations = 0;
    int numSheps = qthread_num_shepherds();
    int i;
    for (i =0; i < numSheps; i++) {
      t->current_workers[i] = 0; // not me
    }
#ifdef QTHREAD_RCRTOOL
    t->allowed_workers = maestro_allowed_workers();
#endif
    if (pr) pr->forLoop = t;
  }
  // akp - this feels more than should be needed but I was having problems with simpler
  //       waiting mechanisms (at both compilation and execution) 
  if ((int64_t)pr->forLoop == -1) { // wait for region to be created 
    t = qthread_cas(&pr->forLoop, -1, -1);
    while( (int64_t)t == -1){
      t = qthread_cas(&pr->forLoop, -1, -1);
    }
  }
  else t = pr->forLoop; // t got no value but pr->forLoop was full by the time we got here
  // just use the value

  if (t && ((t->departed_workers == 0)|(type == STATIC_SCHED))) {
    qthread_incr(&t->workers,1);
    *loop = (void*)t;
  }
  else {
    *loop = NULL;
  }
  return;
}
  
void xomp_internal_set_ordered_iter(qqloop_step_handle_t *loop, int lower) {
  if (loop == NULL) return; // loop is completed (and destroyed) we just need to
                            // complete -- no more work
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  qthread_worker_id_t myid = qthread_barrier_id();
  aligned_t *iter = ((aligned_t*)&loop->work_array) + qthread_num_workers() + myid;
#else
  qthread_shepherd_id_t myid = qthread_shep();
  aligned_t *iter = ((aligned_t*)&loop->work_array) + qthread_num_shepherds() + myid;
#endif
  *iter = lower; // insert lower bound
}

void XOMP_loop_guided_init(
    void ** loop,
    int lower,
    int upper,
    int stride,
    int chunk_size)
{
  xomp_internal_loop_init(GUIDED_SCHED, FALSE, loop, lower, upper, stride, chunk_size);
  return;
}

int compute_XOMP_block(
    qqloop_step_handle_t * loop)
{
  int ret = 1;
  switch (loop->type)
    {
    case GUIDED_SCHED: // default schedule -- varient of guided self-schedule to prevent overly small blocks
      {
      ret = qloop_internal_computeNextBlock(loop);
      break;
      }
    case STATIC_SCHED: // get my next chunk -- used to access same data with same threads over time
      {
      ret = loop->chunkSize;
      break;
      }
    case DYNAMIC_SCHED: // get next available chunk
      {
      ret = loop->chunkSize;
      break;
      }
    case RUNTIME_SCHED: // look at runtime variable to determine which type to use
      // this really should never be hit -- runtime init should pick one of the others
      // default to dynamic because its the shortest to type
      {
      ret = loop->chunkSize;
      break;
      }
    default:
      fprintf(stderr, "compute_XOMP_block invalid loop type %d\n", loop->type);
    }


  return ret;
}


// start Qthreads default openmp loop execution
// get next iteration(if any) for Qthreads default openmp loop execution
static bool xomp_internal_guided_next(
    qqloop_step_handle_t * loop,
    long *returnLower,
    long *returnUpper)
{
    // spin waiting for either 
    qthread_shepherd_id_t myShepId = qthread_shep();
#ifdef QTHREAD_RCRTOOL
    if (rcrSchedulingOff) { // has cache control been turned off by an environment variable?
        if (loop->current_workers[myShepId] > maestro_current_workers(myShepId)) {
	    qthread_incr(&loop->current_workers[myShepId],-1); // not working spinning
	    while ((loop->current_workers[myShepId] + 1) > maestro_current_workers(myShepId)) { // A) the number of workers to be increased
	      if(loop->departed_workers) break; // B) some worker to notice the loop is done  -- could repeat code and return instead of breaking
	    }
	    qthread_incr(&loop->current_workers[myShepId],1); // back at work  -- skipped in departed workers case OK since everyone leaving

	}
    }
#endif

    int dynamicBlock = compute_XOMP_block(loop);

    dynamicBlock = (dynamicBlock <= 1) ? 1:(dynamicBlock - 1); // min size 1
    aligned_t iterationNumber = qthread_incr(&loop->assignNext, dynamicBlock);
    *returnLower = iterationNumber;
    *returnUpper = iterationNumber + (dynamicBlock-1); // top is inclusive

    if (iterationNumber > loop->assignStop) {  // already assigned everything
        *returnLower = 0;
	*returnUpper = 0;
	qthread_incr(&loop->departed_workers,1);
	qthread_incr(&loop->current_workers[myShepId],-1);
	return FALSE;
    }
    if (*returnUpper > loop->assignStop) {// this iteration goes past end of loop
        *returnUpper = loop->assignStop;
    }

    qthread_debug(ALL_DETAILS,
		  "limit %10d lower %10d upper %10d block %10d id %d\n",
		  loop->assignDone, *returnLower, *returnUpper, dynamicBlock,
		  qthread_shep()
		  );
    return TRUE;
}


bool XOMP_loop_guided_start(
    void * lp,
    long startLower,
    long startUpper,
    long stride,
    long chunk_size,
    long *returnLower,
    long *returnUpper)
{
    if (lp == NULL){
      return FALSE;
    }

    qqloop_step_handle_t *loop = (qqloop_step_handle_t *)lp;
    qthread_shepherd_id_t myShepId = qthread_shep();
    
    qthread_incr(&loop->current_workers[myShepId],1);
    return xomp_internal_guided_next(loop, returnLower, returnUpper);

}

// get next iteration(if any) for Qthreads default openmp loop execution
bool XOMP_loop_guided_next(
    void * loop,
    long *returnLower,
    long *returnUpper)
{
    return xomp_internal_guided_next(loop, returnLower, returnUpper);
}

volatile aligned_t spinLock = 0;
volatile aligned_t spinLock_rel = 0;

void XOMP_spin_lock(
    void * lp)
{
  int val = qthread_incr(&spinLock,1);
  int limit = qthread_num_workers()-1;
  if (val == limit) spinLock_rel = 1;
  while (!spinLock_rel){}; // spin until my turn

  if(XOMP_master()) {
    qthread_parallel_region_t *pr = qt_parallel_region();
    pr->forLoop = NULL;
    lp = NULL;
  } 

  val = qthread_incr(&spinLock,-1);
  if (val == 1) spinLock_rel = 0;
  while (spinLock_rel){}; // spin until my turn
}

// Openmp parallel for loop is completed (waits for all to complete)
void XOMP_loop_end(
    void * lp)
{
    qqloop_step_handle_t *loop = (qqloop_step_handle_t *)lp;
    XOMP_loop_end_nowait(loop);

    if (!get_inside_xomp_parallel(&xomp_status)) {
        XOMP_spin_lock(loop); // need barrier to make sure loop is freed 
    }
    else {
      XOMP_barrier(); // need barrier to make sure loop is freed after everyone has used it
      if (XOMP_master()) {
	qthread_parallel_region_t *pr = qt_parallel_region();
	pr->forLoop = NULL;
	loop = NULL;
      }
      XOMP_barrier(); // need barrier to make sure loop is freed after everyone has used it
    }
#ifdef QTHREAD_RCRTOOL
    //Here we log exiting a loop section.
    rcrtool_log(RCR_APP_STATE_DUMP, XOMP_FOR_LOOP_END, 0, (uint64_t)0, "");
#endif
}

// Openmp parallel for loop is completed
void XOMP_loop_end_nowait(
    void * loop)
{
}

// Qthread implementation of a OpenMP global barrier
void walkSyncTaskList(void);
extern int activeParallelLoop;

void XOMP_barrier(void)
{
    walkSyncTaskList(); // wait for outstanding tasks to complete

#ifdef QTHREAD_LOG_BARRIER
    size_t myid = qthread_barrier_id();
    qt_barrier_enter(qt_thread_barrier(),myid);
#else
    qt_feb_barrier_enter(qt_thread_barrier());
#endif

}
void XOMP_atomic_start(
    void)
{
    qthread_lock(get_atomic_lock(&xomp_status));
}

void XOMP_atomic_end(
    void)
{
    qthread_unlock(get_atomic_lock(&xomp_status));
}

// needed for full OpenMP 3.0 support
void XOMP_loop_ordered_guided_init(
    void ** loop,
    int lower,
    int upper,
    int stride,
    int chunk_size)
{
  // until ordered is handled defualt to standard loop
  xomp_internal_loop_init(GUIDED_SCHED, TRUE, loop, lower, upper, stride, chunk_size);
}

bool XOMP_loop_ordered_guided_start(
    void * loop,
    long a,
    long b,
    long c,
    long d,
    long *e,
    long *f)
{
  // until ordered is handled defualt to standard loop
  bool ret = XOMP_loop_guided_start(loop, a, b, c, d, e, f);

  xomp_internal_set_ordered_iter(loop,a);
  return ret;
}

bool XOMP_loop_ordered_guided_next(
    void * lp,
    long *a,
    long *b)
{
  // until ordered is handled defualt to standard loop
  bool ret =  XOMP_loop_guided_next(lp, a, b);
  
  xomp_internal_set_ordered_iter(lp,*a);

  return ret;
}

syncvar_t *getSyncTaskVar(void)
{
  taskSyncvar_t * syncVar = (taskSyncvar_t *)calloc(1,sizeof(taskSyncvar_t));
  qthread_getTaskListLock();
  //  qthread_syncvar_empty(me,&(syncVar->retValue));
  syncVar->next_task = qthread_getTaskRetVar();
  qthread_setTaskRetVar(syncVar);
  qthread_releaseTaskListLock();
  
  return &(syncVar->retValue);
}


void qthread_run_needed_task(syncvar_t *value);
void walkSyncTaskList(void)
{
  qthread_getTaskListLock();
  taskSyncvar_t * syncVar;
  while ((syncVar = qthread_getTaskRetVar())) {

    // manually check for empty -- check if present in current shepherds ready queue
    //   if present take and start executing
    syncvar_t *lc_p = &syncVar->retValue;

#if AKP_TOUCH_EXECUTION
#if ((QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) || \
     (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64) || \
     (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64) || \
     (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64))
    {
      // taken from qthread_syncvar_readFF in qthread.c - I want empty not full
      /* I'm being  optimistic here; this only works if a basic 64-bit load is
       * atomic (on most platforms it is). Thus, if I've done an atomic read
       * and the syncvar is both unlocked and full, then I figure I can trust
       * that state and do not need to do a locked atomic operation of any
       * kind (e.g. cas) */
      syncvar_t lc_syncVar = *lc_p;
      if (lc_syncVar.u.s.lock == 0 && (lc_syncVar.u.s.state & 2) ) { /* empty and unlocked */
	qthread_releaseTaskListLock();
	qthread_run_needed_task(lc_p);
	qthread_getTaskListLock();
	syncVar = qthread_getTaskRetVar();
      }
    }
#endif
#endif
    qthread_syncvar_readFF(NULL, lc_p);
    qthread_setTaskRetVar(syncVar->next_task);
    free(syncVar);
  }
  qthread_releaseTaskListLock();
  return;
}

aligned_t taskId = 1; // start at first non-master shepherd

void XOMP_task(
    void (*func) (void *),
    void *arg,
    void (*cpyfunc) (void *,
	       void *),
    long arg_size,
    long arg_align,
    bool if_clause,
    unsigned untied)
{
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
#else
  qthread_incr(&taskId,1);
  qthread_debug(LOCK_DETAILS, "me(%p) creating task for shepherd %d\n", me, id%qthread_num_shepherds());
#endif
  syncvar_t *ret = getSyncTaskVar(); // get new syncvar_t -- setup openmpThreadId (if needed)
#ifdef QTHREAD_OMP_AFFINITY
  qthread_t *t = qthread_self();
  if (t->rdata->child_affinity != OMP_NO_CHILD_TASK_AFFINITY)
    qthread_fork_syncvar_copyargs_to((qthread_f)func, arg, arg_size, ret, t->rdata->child_affinity);
  else
    qthread_fork_syncvar_copyargs((qthread_f)func, arg, arg_size, ret);
#else
  qthread_fork_syncvar_copyargs((qthread_f)func, arg, arg_size, ret);
#endif
}

void XOMP_taskwait(
    void)
{
  walkSyncTaskList();
}

void XOMP_loop_static_init(
    void ** loop,
    int lower,
    int upper,
    int stride,
    int chunk_size)
{
  // until ordered is handled defualt to standard loop
  xomp_internal_loop_init(STATIC_SCHED, FALSE, loop, lower, upper, stride, chunk_size);
 return;
}

void XOMP_loop_dynamic_init(
    void ** loop,
    int lower,
    int upper,
    int stride,
    int chunk_size)
{
  xomp_internal_loop_init(DYNAMIC_SCHED, FALSE, loop, lower, upper, stride, chunk_size);
  return;
}


int runtime_sched_chunk = 1;

void XOMP_loop_runtime_init(
    void ** loop,
    int lower,
    int upper,
    int stride)
{
  int chunk_size = 1; // Something has to be done to correctly compute chunk_size
  qqloop_step_handle_t *qqhandle = NULL;
  
  switch(get_runtime_sched_option(&xomp_status))
    {
    case GUIDED_SCHED: // Initalize guided scheduling at runtime
    case STATIC_SCHED: // Initalize static scheding at runtime
    case DYNAMIC_SCHED: // Initialize dynamic scheduling at runtime
      xomp_internal_loop_init(get_runtime_sched_option(&xomp_status), FALSE,
			      loop, lower, upper, stride, chunk_size);
      break;
    default:
      fprintf(stderr, "Weird XOMP_loop_runtime_init case happened that should never happen: %d", qqhandle->type);
      abort();
    }

  return;
}

//ordered case
void XOMP_loop_ordered_static_init(
    void ** loop,
    int lower,
    int upper,
    int stride,
    int chunk_size)
{
  xomp_internal_loop_init(STATIC_SCHED, TRUE,
			  loop, lower, upper, stride, chunk_size);
}

void XOMP_loop_ordered_dynamic_init(
    void ** loop,
    int lower,
    int upper,
    int stride,
    int chunk_size)
{
  xomp_internal_loop_init(DYNAMIC_SCHED, TRUE,
			  loop, lower, upper, stride, chunk_size);
}

void XOMP_loop_ordered_runtime_init(
    void ** loop,
    int lower,
    int upper,
    int stride)
{
  int chunk_size = 1; // Something has to be done to correctly compute chunk_size
  qqloop_step_handle_t *qqhandle = NULL;
  
  switch(get_runtime_sched_option(&xomp_status))
    {
    case GUIDED_SCHED: // Initalize guided scheduling at runtime
    case STATIC_SCHED: // Initalize static scheding at runtime
    case DYNAMIC_SCHED: // Initialize dynamic scheduling at runtime
      xomp_internal_loop_init(get_runtime_sched_option(&xomp_status), TRUE,
			      loop, lower, upper, stride, chunk_size);
      break;
    default:
      fprintf(stderr, "Weird XOMP_loop_runtime_init case happened that should never happen: %d", qqhandle->type);
      abort();
    }

  return;
}


// rest of the functions


// omp ordered directive
void XOMP_ordered_start(
    void)
{
  qqloop_step_handle_t * loop = testLoop;
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  qthread_worker_id_t myid = qthread_barrier_id();
  aligned_t *iter = ((aligned_t*)&loop->work_array) + qthread_num_workers() + myid;
#else
  qthread_shepherd_id_t myid = qthread_shep();
  aligned_t *iter = ((aligned_t*)&loop->work_array) + qthread_num_shepherds() + myid;
#endif

  while (orderedLoopCount != *iter){}; // spin until my turn
  *iter += loop->assignStep;
}

void XOMP_ordered_end(
    void)
{
  orderedLoopCount++;
}

bool XOMP_loop_static_start(
    void * lp,
    long startLower,
    long startUpper,
    long stride,
    long chunkSize,
    long *returnLower,
    long *returnUpper)
{
  //bool ret = 0;
  qqloop_step_handle_t *loop = (qqloop_step_handle_t *)lp;
  aligned_t myid = qthread_barrier_id();
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  aligned_t parallelWidth = qthread_num_workers();
#else
  aligned_t parallelWidth = qthread_num_shepherds();
#endif

  if (!loop){
    return 0;
  }
  aligned_t start = loop->assignStart;
  aligned_t stop = loop->assignStop;
  //aligned_t step = loop->assignStep;

  aligned_t *myIteration = &loop->work_array + myid;
  int iterationNum = qthread_incr(myIteration,1);
  *returnLower = (start * chunkSize) + (iterationNum * chunkSize * parallelWidth) // start
    + (myid * chunkSize);                                                         // + offset

  if ((*returnLower + (chunkSize-1)) >= stop) { // hit limit
    *returnUpper = stop;
    if (*returnLower  > stop) {              // nothing to do quit
      qthread_incr(&loop->departed_workers,1);
      return 0;
    }
    return 1;
  }
  else {                                     // compute upper bound
    *returnUpper = *returnLower + chunkSize; 
    return 1;
  }
}

bool XOMP_loop_dynamic_start(
    void * loop,
    long a,
    long b,
    long c,
    long d,
    long *returnLower,
    long *returnUpper)
{
  // set up so we can call guided and the correct actions happen
  return XOMP_loop_guided_start(loop, -1, -1, -1, -1, returnLower, returnUpper);
}

bool XOMP_loop_runtime_start(
    void * loop,
    long a,
    long b,
    long c,
    long *returnLower,
    long *returnUpper)
{
  // set up so we can call guided and the correct actions happen
  return XOMP_loop_guided_start(loop, -1, -1, -1, -1, returnLower, returnUpper);
}

bool XOMP_loop_ordered_static_start(
    void * loop,
    long a,
    long b,
    long c,
    long d,
    long *e,
    long *f)
{
  bool ret =  XOMP_loop_static_start(loop, a, b, c, d, e, f);
  
  xomp_internal_set_ordered_iter(loop, *e);

  return ret;
}

bool XOMP_loop_ordered_dynamic_start(
    void * loop,
    long a,
    long b,
    long c,
    long d,
    long *e,
    long *f)
{
  bool ret =  XOMP_loop_dynamic_next(loop, e, f);
  
  xomp_internal_set_ordered_iter(loop, *e);

  return ret;
}

bool XOMP_loop_ordered_runtime_start(
    void * loop,
    long a,
    long b,
    long c,
    long *d,
    long *e)
{
  bool ret =  XOMP_loop_runtime_next(loop, d, e);
  xomp_internal_set_ordered_iter(loop, *d);

  return ret;
}

// next
bool XOMP_loop_static_next(
    void * lp,
    long *a,
    long *b)
{
  qqloop_step_handle_t *loop = (qqloop_step_handle_t *)lp;
  return XOMP_loop_static_start(loop, 0, 0, 0, loop->chunkSize,  a, b);
}

bool XOMP_loop_dynamic_next(
    void * loop,
    long *returnLower,
    long *returnUpper)
{
  // set up so we can call guided and the correct actions happen
  return XOMP_loop_guided_start(loop, -1, -1, -1, -1, returnLower, returnUpper);
}

bool XOMP_loop_runtime_next(
    void * loop,
    long *returnLower,
    long *returnUpper)
{
  // set up so we can call guided and the correct actions happen
  return XOMP_loop_guided_start(loop, -1, -1, -1, -1, returnLower, returnUpper);
}

bool XOMP_loop_ordered_static_next(
    void * loop,
    long *a,
    long *b)
{
  bool ret =  XOMP_loop_static_next(loop, a, b);
  
  xomp_internal_set_ordered_iter(loop, *a);

  return ret;
}

bool XOMP_loop_ordered_dynamic_next(
    void * loop,
    long *a,
    long *b)
{
  bool ret =  XOMP_loop_dynamic_next(loop, a, b);
  
  xomp_internal_set_ordered_iter(loop, *a);

  return ret;
}

bool XOMP_loop_ordered_runtime_next(
    void * loop,
    long *a,
    long *b)
{
  bool ret =  XOMP_loop_runtime_next(loop, a, b);
  
  xomp_internal_set_ordered_iter(loop, *a);

  return ret;
}

//--------------end of  loop functions 

void XOMP_critical_start(
    void **data)
{
  QTHREAD_FASTLOCK_LOCK(&critLock);
}

void XOMP_critical_end(
    void **data)
{
  QTHREAD_FASTLOCK_UNLOCK(&critLock);
}

// really should have a include that defines true and false
// assuming that shepherd 0 is the master -- problem if we get
// to nested parallelism where this may not be true
bool XOMP_master(
    void)
{
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  int myid = qthread_barrier_id();
#else
  int myid = qthread_shep();
#endif
  if (myid == 0) return 1;
  else return 0;
}

// let the first on to the section do it
bool XOMP_single(
    void)
{
  return XOMP_master();  // forces thread 0 no barrier
  /*                     // any thread barrier to make sure its cleaned before next use
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  int myid = qthread_barrier_id();
#else
  int myid = qthread_shep();
#endif
  if (qt_global_arrive_first(myid)){ 
    return 1;
  }
  else{
    return 0;
  }
  */  
}


// flush without variable list
void XOMP_flush_all(
    void)
{
  perror("XOMP_flush_all not yet implmented"); 
  exit(1);
}

void qtar_resize(aligned_t);

// omp flush with variable list, flush one by one, given each's start address and size
void XOMP_flush_one(
    char *startAddress,
    int nbyte)
{
  perror("XOMP_flush_one not yet implmented"); 
  exit(1);
}


void omp_set_num_threads (
    int omp_num_threads_requested)
{

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  qthread_worker_id_t workerid = qthread_worker(NULL);
  qthread_shepherd_id_t num_active = qthread_num_workers();
#else
  qthread_shepherd_id_t num_active = qthread_num_shepherds();
#endif
  qthread_shepherd_id_t i, qt_num_threads_requested;

  qt_num_threads_requested = (qthread_shepherd_id_t) omp_num_threads_requested;

  if ( qt_num_threads_requested > num_active)
    {
      for(i=num_active; i < qt_num_threads_requested; i++)
        {
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
          qthread_enable_worker(i);
#else
          qthread_enable_shepherd(i);
#endif
        }
    }
  else if (qt_num_threads_requested < num_active)
    {
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
      if ((workerid <= num_active) && (workerid>=qt_num_threads_requested))
      	{
	  qt_num_threads_requested--;
	}
#endif
      for(i=num_active-1; i >= qt_num_threads_requested; i--)
        {
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
          if (workerid != i){
	    qthread_disable_worker(i);
	    qthread_pack_workerid(i,-1);
	    //	    if(workerid == i) yield_thread();
	  }
#else
          qthread_disable_shepherd(i);
#endif
        }
    }
  
  if (qt_num_threads_requested != num_active){ 
    // need to reset the barrier size and the first arrival size (if larger or smaller)
    qtar_resize(qt_num_threads_requested);
    qt_barrier_resize(qt_num_threads_requested);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    qthread_worker_id_t newId = 0;
    for(i=0; i < qt_num_threads_requested; i++) {  // repack id's
      qthread_pack_workerid(i,newId++);
    }
    if (workerid >= qt_num_threads_requested) { // carefull about edge conditions
      qthread_pack_workerid(workerid,newId++);
    }
#endif
  }
}

// extern int omp_get_num_threads (void);
int omp_get_num_threads (
    void)
{
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  qthread_worker_id_t num = qthread_num_workers();
#else
  qthread_shepherd_id_t num = qthread_num_shepherds();
#endif
  return (int) num;
}

// extern int omp_get_max_threads (void);
int omp_get_max_threads (
    void)
{
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
  qthread_worker_id_t num = qthread_num_workers();
#else
  qthread_shepherd_id_t num = qthread_num_shepherds();
#endif
  return (int) num;
}

// extern int omp_get_thread_num (void);
int omp_get_thread_num (
    void)
{
  return (int) qthread_barrier_id();
}

// extern int omp_get_num_procs (void);
int omp_get_num_procs (
    void)
{
    return qthread_readstate(ACTIVE_SHEPHERDS);
}

// extern int omp_in_parallel (void);
int omp_in_parallel (
    void)
{
  return (int) get_inside_xomp_parallel(&xomp_status);
}

// extern int omp_in_final (void);
char omp_in_final(void)
{
  return 0; // needs to be fixed when omp final clause is supported (coming 3.1)
}

// extern void omp_set_dynamic (int);
void omp_set_dynamic (
    int val)
{
  int b = (val)?1:0;
  set_xomp_dynamic(b, &xomp_status);
}

// extern int omp_get_dynamic (void);
int omp_get_dynamic (
    void)
{
  return get_xomp_dynamic(&xomp_status);
}

// extern void omp_init_lock (omp_lock_t *);
void omp_init_lock (
    void *pval)
{
  qthread_syncvar_empty(pval);
}

// extern void omp_destroy_lock (omp_lock_t *);
void omp_destroy_lock (
    void *pval)
{
}

// extern void omp_set_lock (omp_lock_t *);
void omp_set_lock (
    void *pval)
{
  qthread_syncvar_writeEF_const(pval, 1);
}

// extern void omp_unset_lock (omp_lock_t *);
void omp_unset_lock (
    void *pval)
{
  qthread_syncvar_readFE(NULL, pval);
}

// extern int omp_test_lock (omp_lock_t *);
int omp_test_lock (
    void *pval)
{
  perror("qthread wrapper for omp_test_lock not yet implmented");
  exit(1);
  return 0;
}

// extern void omp_init_nest_lock (omp_nest_lock_t *);
void omp_init_nest_lock (
    void *pval)
{
  perror("qthread wrapper for omp_init_nest_lock not yet implmented");
  exit(1);
}

// extern void omp_destroy_nest_lock (omp_nest_lock_t *);
void omp_destroy_nest_lock (
    void *pval)
{
  perror("qthread wrapper for omp_destroy_nest_lock not yet implmented");
  exit(1);
}
void omp_set_nested (int val)
{
  xomp_nest_level_t b = (val)? ALLOW_NEST:NO_NEST;
  xomp_set_nested(&xomp_status, b);
}

int64_t omp_get_nested (void)
{
  bool b = xomp_get_nested(&xomp_status);
  return (b)? 1:0;
}


// extern void omp_set_nest_lock (omp_nest_lock_t *);
void omp_set_nest_lock (
    void *pval)
{
  perror("qthread wrapper for omp_set_nest_lock not yet implmented");
  exit(1);
}

// extern void omp_unset_nest_lock (omp_nest_lock_t *);
void omp_unset_nest_lock (
    void *pval)
{
  perror("qthread wrapper for omp_unset_nest_lock not yet implmented");
  exit(1);
}

// extern int omp_test_nest_lock (omp_nest_lock_t *);
int omp_test_nest_lock (
    void *pval)
{
  perror("qthread wrapper for omp_test_nest_lock not yet implmented");
  exit(1);
  return 0;
}

// extern double omp_get_wtime (void);
double omp_get_wtime (
    void)
{
  return XOMP_get_wtime(&xomp_status);
}

// extern double omp_get_wtick (void);
double omp_get_wtick (
    void)
{
  return XOMP_get_wtick(&xomp_status);
}

#ifdef QTHREAD_OMP_AFFINITY
// Additional Functions for Scheduling and Locality Control

unsigned int qthread_shepherds_available (
    void)
{
   return qthread_num_shepherds();
}

void qthread_disable_stealing (
    void)
{
  qthread_shepherd_t *shep = qthread_internal_getshep();
  shep->stealing = 1;
}

void qthread_disable_stealing_all (
    void)
{
  int i;

  for (i = 0; i < qthread_num_shepherds(); i++)
     qlib->shepherds[i].stealing = 1;
}

void qthread_disable_stealing_onshep (
    unsigned int shep)
{
  qlib->shepherds[shep].stealing = 1;
}

void qthread_enable_stealing (
    unsigned int stealing_mode)
{
  qthread_shepherd_t *shep = qthread_internal_getshep();
  shep->stealing = 0;
  shep->stealing_mode = stealing_mode;
}

void qthread_enable_stealing_all (
    unsigned int stealing_mode)
{
  int i;

  for (i = 0; i < qthread_num_shepherds(); i++)
  {
     qlib->shepherds[i].stealing = 0;
     qlib->shepherds[i].stealing_mode = stealing_mode;
  }
}

void qthread_enable_stealing_on_shep (
    unsigned int shep,
    unsigned int stealing_mode)
{
  qlib->shepherds[shep].stealing = 0;
  qlib->shepherds[shep].stealing_mode = stealing_mode;
}

void omp_child_task_affinity (
    unsigned int shep)
{
  qthread_t *t = qthread_self();   
  t->rdata->child_affinity = (qthread_shepherd_id_t) shep;
}  

// In the following functions, we just return null if hwloc is not avaiable

void * omp_affinity_alloc(size_t bytes)
{
#ifdef QTHREAD_HAVE_MEM_AFFINITY
  return qt_affinity_alloc(bytes);
#else
  return NULL;
#endif
}

void * omp_affinity_alloc_onshep(size_t bytes, unsigned int shep)
{
#ifdef QTHREAD_HAVE_MEM_AFFINITY
  return qt_affinity_alloc_onnode(bytes, qlib->shepherds[shep].node);
#else
  return NULL;
#endif
}

void omp_affinity_mem_toshep(void * addr, size_t bytes, unsigned int shep)
{
#ifdef QTHREAD_HAVE_MEM_AFFINITY
  return qt_affinity_mem_tonode(addr, bytes, qlib->shepherds[shep].node);
#endif
}

void omp_affinity_free(void * ptr, size_t bytes)
{
#ifdef QTHREAD_HAVE_MEM_AFFINITY
  qt_affinity_alloc_free(ptr, bytes);
#endif
}

#endif
