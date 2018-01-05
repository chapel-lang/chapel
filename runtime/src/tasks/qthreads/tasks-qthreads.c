//
// Qthreads implementation of Chapel tasking interface
//
// Copyright 2011 Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000, there is a non-exclusive license for use of this work by
// or on behalf of the U.S. Government. Export of this program may require a
// license from the United States Government
//

/*
 * Copyright 2004-2018 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// For SVID definitions (setenv)
#define _SVID_SOURCE

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "chplrt.h"

#include "arg.h"
#include "error.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "chpl-locale-model.h"
#include "chpl-mem.h"
#include "chplsys.h"
#include "chpl-linefile-support.h"
#include "chpl-tasks.h"
#include "chpl-tasks-callbacks-internal.h"
#include "tasks-qthreads.h"

#include "qthread.h"
#include "qthread/qtimer.h"
#include "qthread-chapel.h"

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>


//#define SUPPORT_BLOCKREPORT
//#define SUPPORT_TASKREPORT

#ifdef CHAPEL_PROFILE
# define PROFILE_INCR(counter,count) do { (void)qthread_incr(&counter,count); } while (0)

/* Tasks */
static aligned_t profile_task_yield = 0;
static aligned_t profile_task_addToTaskList = 0;
static aligned_t profile_task_executeTasksInList = 0;
static aligned_t profile_task_taskCallFTable = 0;
static aligned_t profile_task_startMovedTask = 0;
static aligned_t profile_task_getId = 0;
static aligned_t profile_task_sleep = 0;
static aligned_t profile_task_getSerial = 0;
static aligned_t profile_task_setSerial = 0;
static aligned_t profile_task_getCallStackSize = 0;
/* Sync */
static aligned_t profile_sync_lock= 0;
static aligned_t profile_sync_unlock= 0;
static aligned_t profile_sync_waitFullAndLock= 0;
static aligned_t profile_sync_waitEmptyAndLock= 0;
static aligned_t profile_sync_markAndSignalFull= 0;
static aligned_t profile_sync_markAndSignalEmpty= 0;
static aligned_t profile_sync_isFull= 0;
static aligned_t profile_sync_initAux= 0;
static aligned_t profile_sync_destroyAux= 0;

static void profile_print(void)
{
    /* Tasks */
    fprintf(stderr, "task yield: %lu\n", (unsigned long)profile_task_yield);
    fprintf(stderr, "task addToTaskList: %lu\n", (unsigned long)profile_task_addToTaskList);
    fprintf(stderr, "task executeTasksInList: %lu\n", (unsigned long)profile_task_executeTasksInList);
    fprintf(stderr, "task taskCallFTable: %lu\n", (unsigned long)profile_task_taskCallFTable);
    fprintf(stderr, "task startMovedTask: %lu\n", (unsigned long)profile_task_startMovedTask);
    fprintf(stderr, "task getId: %lu\n", (unsigned long)profile_task_getId);
    fprintf(stderr, "task sleep: %lu\n", (unsigned long)profile_task_sleep);
    fprintf(stderr, "task getSerial: %lu\n", (unsigned long)profile_task_getSerial);
    fprintf(stderr, "task setSerial: %lu\n", (unsigned long)profile_task_setSerial);
    fprintf(stderr, "task getCallStackSize: %lu\n", (unsigned long)profile_task_getCallStackSize);
    /* Sync */
    fprintf(stderr, "sync lock: %lu\n", (unsigned long)profile_sync_lock);
    fprintf(stderr, "sync unlock: %lu\n", (unsigned long)profile_sync_unlock);
    fprintf(stderr, "sync waitFullAndLock: %lu\n", (unsigned long)profile_sync_waitFullAndLock);
    fprintf(stderr, "sync waitEmptyAndLock: %lu\n", (unsigned long)profile_sync_waitEmptyAndLock);
    fprintf(stderr, "sync markAndSignalFull: %lu\n", (unsigned long)profile_sync_markAndSignalFull);
    fprintf(stderr, "sync markAndSignalEmpty: %lu\n", (unsigned long)profile_sync_markAndSignalEmpty);
    fprintf(stderr, "sync isFull: %lu\n", (unsigned long)profile_sync_isFull);
    fprintf(stderr, "sync initAux: %lu\n", (unsigned long)profile_sync_initAux);
    fprintf(stderr, "sync destroyAux: %lu\n", (unsigned long)profile_sync_destroyAux);
}
#else
# define PROFILE_INCR(counter,count)
#endif /* CHAPEL_PROFILE */

//
// Startup and shutdown control.  The mutex is used just for the side
// effect of its (very portable) memory fence.
//
volatile int chpl_qthread_done_initializing;
static syncvar_t canexit = SYNCVAR_STATIC_EMPTY_INITIALIZER;
static volatile int done_finalizing;
static pthread_mutex_t done_init_final_mux = PTHREAD_MUTEX_INITIALIZER;

// Make qt env sizes uniform. Same as qt, but they use the literal everywhere
#define QT_ENV_S 100

// aka chpl_task_list_p
struct chpl_task_list {
    chpl_fn_p        fun;
    void            *arg;
    int32_t          filename;
    int              lineno;
    chpl_task_list_p next;
};

static aligned_t next_task_id = 1;

pthread_t chpl_qthread_process_pthread;
pthread_t chpl_qthread_comm_pthread;

chpl_task_bundle_t chpl_qthread_process_bundle = {
                                   .is_executeOn = false,
                                   .lineno = 0,
                                   .filename = CHPL_FILE_IDX_MAIN_TASK,
                                   .requestedSubloc = c_sublocid_any_val,
                                   .requested_fid = FID_NONE,
                                   .requested_fn = NULL,
                                   .id = chpl_nullTaskID };

chpl_task_bundle_t chpl_qthread_comm_task_bundle = {
                                   .is_executeOn = false,
                                   .lineno = 0,
                                   .filename = CHPL_FILE_IDX_COMM_TASK,
                                   .requestedSubloc = c_sublocid_any_val,
                                   .requested_fid = FID_NONE,
                                   .requested_fn = NULL,
                                   .id = chpl_nullTaskID };

chpl_qthread_tls_t chpl_qthread_process_tls = {
                               .bundle = &chpl_qthread_process_bundle,
                               .lock_filename = 0,
                               .lock_lineno = 0 };

chpl_qthread_tls_t chpl_qthread_comm_task_tls = {
                               .bundle = &chpl_qthread_comm_task_bundle,
                               .lock_filename = 0,
                               .lock_lineno = 0 };

//
// chpl_qthread_get_tasklocal() is in tasks-qthreads.h
//

static syncvar_t exit_ret = SYNCVAR_STATIC_EMPTY_INITIALIZER;

void chpl_task_yield(void)
{
    PROFILE_INCR(profile_task_yield,1);
    if (qthread_shep() == NO_SHEPHERD) {
        sched_yield();
    } else {
        qthread_yield();
    }
}

// Sync variables
void chpl_sync_lock(chpl_sync_aux_t *s)
{
    aligned_t l;
    chpl_bool uncontested_lock = true;

    PROFILE_INCR(profile_sync_lock, 1);

    //
    // To prevent starvation due to never switching away from a task that is
    // spinning while doing readXX() on a sync variable, yield if this sync var
    // has a "lot" of uncontested locks. Note that the uncontested locks do not
    // have to be consecutive. Also note that the number of uncontested locks
    // is a lossy counter. Currently a "lot" is defined as ~100 uncontested
    // locks, with care taken to not yield on the first uncontested lock.
    //
    // If real qthreads sync vars were used, it's possible this wouldn't be
    // needed.
    //

    l = qthread_incr(&s->lockers_in, 1);

    while (l != s->lockers_out) {
        uncontested_lock = false;
        qthread_yield();
    }

    if (uncontested_lock) {
        if ((++s->uncontested_locks & 0x5F) == 0) {
            qthread_yield();
        }
    }
}

void chpl_sync_unlock(chpl_sync_aux_t *s)
{
    PROFILE_INCR(profile_sync_unlock, 1);

    qthread_incr(&s->lockers_out, 1);
}

static inline void about_to_block(int32_t  lineno,
                                  int32_t filename)
{
    chpl_qthread_tls_t * data = chpl_qthread_get_tasklocal();
    assert(data);

    data->lock_lineno   = lineno;
    data->lock_filename = filename;
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                               int32_t          lineno,
                               int32_t         filename)
{
    PROFILE_INCR(profile_sync_waitFullAndLock, 1);

    if (blockreport) { about_to_block(lineno, filename); }
    chpl_sync_lock(s);
    while (s->is_full == 0) {
        chpl_sync_unlock(s);
        qthread_syncvar_readFE(NULL, &(s->signal_full));
        chpl_sync_lock(s);
    }
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s,
                                int32_t          lineno,
                                int32_t         filename)
{
    PROFILE_INCR(profile_sync_waitEmptyAndLock, 1);

    if (blockreport) { about_to_block(lineno, filename); }
    chpl_sync_lock(s);
    while (s->is_full != 0) {
        chpl_sync_unlock(s);
        qthread_syncvar_readFE(NULL, &(s->signal_empty));
        chpl_sync_lock(s);
    }
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s)         // and unlock
{
    PROFILE_INCR(profile_sync_markAndSignalFull, 1);

    qthread_syncvar_fill(&(s->signal_full));
    s->is_full = 1;
    chpl_sync_unlock(s);
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s)         // and unlock
{
    PROFILE_INCR(profile_sync_markAndSignalEmpty, 1);

    qthread_syncvar_fill(&(s->signal_empty));
    s->is_full = 0;
    chpl_sync_unlock(s);
}

chpl_bool chpl_sync_isFull(void            *val_ptr,
                           chpl_sync_aux_t *s)
{
    PROFILE_INCR(profile_sync_isFull, 1);

    return s->is_full;
}

void chpl_sync_initAux(chpl_sync_aux_t *s)
{
    PROFILE_INCR(profile_sync_initAux, 1);

    s->lockers_in   = 0;
    s->lockers_out  = 0;
    s->is_full      = 0;
    s->signal_empty = SYNCVAR_EMPTY_INITIALIZER;
    s->signal_full  = SYNCVAR_EMPTY_INITIALIZER;
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s)
{
    PROFILE_INCR(profile_sync_destroyAux, 1);
}

#ifdef SUPPORT_BLOCKREPORT
static void chapel_display_thread(qt_key_t     addr,
                                  qthread_f    f,
                                  void        *arg,
                                  void        *retloc,
                                  unsigned int thread_id,
                                  void        *tls,
                                  void        *callarg)
{
    chpl_qthread_tls_t *rep = (chpl_qthread_tls_t *)tls;

    if (rep) {
        if ((rep->lock_lineno > 0) && rep->lock_filename) {
          fprintf(stderr, "Waiting at: %s:%zu (task %s:%zu)\n",
                  chpl_lookupFilename(rep->lock_filename), rep->lock_lineno,
                  chpl_lookupFilename(rep->chpl_data.task_filename),
                  rep->chpl_data.task_lineno);
        } else if (rep->lock_lineno == 0 && rep->lock_filename) {
          fprintf(stderr,
                  "Waiting for more work (line 0? file:%s) (task %s:%zu)\n",
                  chpl_lookupFilename(rep->lock_filename),
                  chpl_lookupFilename(rep->chpl_data.task_filename),
                  rep->chpl_data.task_lineno);
        } else if (rep->lock_lineno == 0) {
          fprintf(stderr,
                  "Waiting for dependencies (uninitialized task %s:%zu)\n",
                  chpl_lookupFilename(rep->chpl_data.task_filename),
                  rep->chpl_data.task_lineno);
        }
        fflush(stderr);
    }
}

static void report_locked_threads(void)
{
    qthread_feb_callback(chapel_display_thread, NULL);
    qthread_syncvar_callback(chapel_display_thread, NULL);
}
#endif  // SUPPORT_BLOCKREPORT

static void SIGINT_handler(int sig)
{
    signal(sig, SIG_IGN);

    if (blockreport) {
#ifdef SUPPORT_BLOCKREPORT
        report_locked_threads();
#else
        fprintf(stderr,
                "Blockreport is currently unsupported by the qthreads "
                "tasking layer.\n");
#endif
    }

    if (taskreport) {
#ifdef SUPPORT_TASKREPORT
        report_all_tasks();
#else
        fprintf(stderr, "Taskreport is currently unsupported by the qthreads tasking layer.\n");
#endif
    }

    chpl_exit_any(1);
}

// Tasks

static void *initializer(void *junk)
{
    qthread_initialize();
    (void) pthread_mutex_lock(&done_init_final_mux);  // implicit memory fence
    chpl_qthread_done_initializing = 1;
    (void) pthread_mutex_unlock(&done_init_final_mux);

    qthread_syncvar_readFF(NULL, &canexit);

    qthread_finalize();
    (void) pthread_mutex_lock(&done_init_final_mux);  // implicit memory fence
    done_finalizing = 1;
    (void) pthread_mutex_unlock(&done_init_final_mux);

    return NULL;
}

//
// Qthreads environment helper functions.
//

static char* chpl_qt_getenv_str(const char* var) {
    char name[100];
    char* ev;

    snprintf(name, sizeof(name), "QT_%s", var);
    if ((ev = getenv(name)) == NULL) {
        snprintf(name, sizeof(name), "QTHREAD_%s", var);
        ev = getenv(name);
    }

    return ev;
}

static unsigned long int chpl_qt_getenv_num(const char* var,
                                            unsigned long int default_val) {
    char* ev;
    unsigned long int ret_val = default_val;

    if ((ev = chpl_qt_getenv_str(var)) != NULL) {
        unsigned long int val;
        if (sscanf(ev, "%lu", &val) == 1)
            ret_val = val;
    }

    return ret_val;
}

// Helper function to set a qthreads env var. This is meant to mirror setenv
// functionality, but qthreads has two environment variables for every setting:
// a QT_ and a QTHREAD_ version. We often forget to think about both so this
// wraps the overriding logic. In verbose mode it prints out if we overrode
// values, or if we were prevented from setting values because they existed
// (and override was 0.)
static void chpl_qt_setenv(const char* var, const char* val,
                           int32_t override) {
    char      qt_env[QT_ENV_S]  = { 0 };
    char      qthread_env[QT_ENV_S] = { 0 };
    char      *qt_val;
    char      *qthread_val;
    chpl_bool eitherSet = false;

    strncpy(qt_env, "QT_", sizeof(qt_env));
    strncat(qt_env, var, sizeof(qt_env) - 1);

    strncpy(qthread_env, "QTHREAD_", sizeof(qthread_env));
    strncat(qthread_env, var, sizeof(qthread_env) - 1);

    qt_val = getenv(qt_env);
    qthread_val = getenv(qthread_env);
    eitherSet = (qt_val != NULL || qthread_val != NULL);

    if (override || !eitherSet) {
        if (verbosity >= 2
            && override
            && eitherSet
            && strcmp(val, (qt_val != NULL) ? qt_val : qthread_val) != 0) {
            printf("QTHREADS: Overriding the value of %s and %s "
                   "with %s\n", qt_env, qthread_env, val);
        }
        (void) setenv(qt_env, val, 1);
        (void) setenv(qthread_env, val, 1);
    } else if (verbosity >= 2) {
        char* set_env = NULL;
        char* set_val = NULL;
        if (qt_val != NULL) {
            set_env = qt_env;
            set_val = qt_val;
        } else {
            set_env = qthread_env;
            set_val = qthread_val;
        }
        if (strcmp(val, set_val) != 0)
          printf("QTHREADS: Not setting %s to %s because %s is set to %s and "
                 "overriding was not requested\n", qt_env, val, set_env, set_val);
    }
}

static void chpl_qt_unsetenv(const char* var) {
  char name[100];

  snprintf(name, sizeof(name), "QT_%s", var);
  (void) unsetenv(name);

  snprintf(name, sizeof(name), "QTHREAD_%s", var);
  (void) unsetenv(name);
}

// Determine the number of workers based on environment settings. If a user set
// HWPAR, they are saying they want to use HWPAR many workers, but let the
// runtime figure out the details. If they explicitly set NUM_SHEPHERDS and/or
// NUM_WORKERS_PER_SHEPHERD then they must have specific reasons for doing so.
// Returns 0 if no Qthreads env vars related to the number of threads were set,
// what HWPAR was set to if it was set, or -1 if NUM_SHEP and/or NUM_WPS were
// set since we can't figure out before Qthreads init what this will actually
// turn into without duplicating Qthreads logic (-1 is a sentinel for don't
// adjust the values, and give them as is to Qthreads.)
static int32_t chpl_qt_getenv_num_workers(void) {
    int32_t  hwpar;
    int32_t  num_wps;
    int32_t  num_sheps;

    hwpar = (int32_t) chpl_qt_getenv_num("HWPAR", 0);
    num_wps = (int32_t) chpl_qt_getenv_num("NUM_WORKERS_PER_SHEPHERD", 0);
    num_sheps = (int32_t) chpl_qt_getenv_num("NUM_SHEPHERDS", 0);

    if (hwpar) {
        return hwpar;
    } else if (num_wps || num_sheps) {
        return -1;
    }

    return 0;
}


// Sets up and returns the amount of hardware parallelism to use, limited to
// maxThreads. Returns -1 if we did not setup parallelism because a user
// explicitly requested a specific layout from qthreads.
static int32_t setupAvailableParallelism(int32_t maxThreads) {
    int32_t   numThreadsPerLocale;
    int32_t   qtEnvThreads;
    int32_t   hwpar;
    char      newenv_workers[QT_ENV_S] = { 0 };

    // Experience has shown that Qthreads generally performs best with
    // num_workers = numCores (and thus worker_unit = core) but if the user has
    // explicitly requested more threads through the chapel or Qthread env
    // vars, we override the default.
    numThreadsPerLocale = chpl_task_getenvNumThreadsPerLocale();
    qtEnvThreads = chpl_qt_getenv_num_workers();
    hwpar = 0;

    // User set chapel level env var (CHPL_RT_NUM_THREADS_PER_LOCALE)
    // This is limited to the number of logical CPUs on the node.
    if (numThreadsPerLocale != 0) {
        int32_t numPUsPerLocale;

        hwpar = numThreadsPerLocale;

        numPUsPerLocale = chpl_getNumLogicalCpus(true);
        if (0 < numPUsPerLocale && numPUsPerLocale < hwpar) {
            if (verbosity > 0) {
                printf("QTHREADS: Reduced numThreadsPerLocale=%d to %d "
                       "to prevent oversubscription of the system.\n",
                       hwpar, numPUsPerLocale);
            }

            // Do not oversubscribe the system, use all available resources.
            hwpar = numPUsPerLocale;
        }
    }
    // User set qthreads level env var
    // (HWPAR or (NUM_SHEPHERDS and NUM_WORKERS_PER_SHEPHERD))
    else if (qtEnvThreads != 0) {
        hwpar = qtEnvThreads;
    }
    // User did not set chapel or qthreads vars -- our default
    else {
        hwpar = chpl_getNumPhysicalCpus(true);
    }

    // hwpar will only be <= 0 if the user set QT_NUM_SHEPHERDS and/or
    // QT_NUM_WORKERS_PER_SHEPHERD in which case we assume as "expert" user and
    // don't impose any thread limits or set worker_unit.
    if (hwpar > 0) {
        // Limit the parallelism to the maximum imposed by the comm layer.
        if (0 < maxThreads && maxThreads < hwpar) {
            hwpar = maxThreads;
        }

        // If there is more parallelism requested than the number of cores, set the
        // worker unit to pu, otherwise core.
        if (hwpar > chpl_getNumPhysicalCpus(true)) {
          chpl_qt_setenv("WORKER_UNIT", "pu", 0);
        } else {
          chpl_qt_setenv("WORKER_UNIT", "core", 0);
        }

        // Unset relevant Qthreads environment variables.
        chpl_qt_unsetenv("HWPAR");
        chpl_qt_unsetenv("NUM_SHEPHERDS");
        chpl_qt_unsetenv("NUM_WORKERS_PER_SHEPHERD");

        snprintf(newenv_workers, sizeof(newenv_workers), "%i", (int)hwpar);
        if (CHPL_QTHREAD_SCHEDULER_ONE_WORKER_PER_SHEPHERD) {
            chpl_qt_setenv("NUM_SHEPHERDS", newenv_workers, 1);
            chpl_qt_setenv("NUM_WORKERS_PER_SHEPHERD", "1", 1);
        } else {
            chpl_qt_setenv("HWPAR", newenv_workers, 1);
        }
    }
    return hwpar;
}

static void setupCallStacks(int32_t hwpar) {
    size_t callStackSize;

    // If the user compiled with no stack checks (either explicitly or
    // implicitly) turn off qthread guard pages. TODO there should also be a
    // chpl level env var backing this at runtime (can be the same var.)
    // Also turn off guard pages if the heap page size isn't the same as
    // the system page size, because when that's the case we can reliably
    // make the guard pages un-referenceable.  (This typically arises when
    // the heap is on hugepages, as is often the case on Cray systems.)
    //
    // Note that we won't override an explicit setting of QT_GUARD_PAGES
    // in the former case, but we do in the latter case.
    if (CHPL_STACK_CHECKS == 0) {
        chpl_qt_setenv("GUARD_PAGES", "false", 0);
    }
    else if (chpl_getHeapPageSize() != chpl_getSysPageSize()) {
        chpl_qt_setenv("GUARD_PAGES", "false", 1);
    }

    // Precedence (high-to-low):
    // 1) Chapel environment (CHPL_RT_CALL_STACK_SIZE)
    // 2) QTHREAD_STACK_SIZE
    // 3) Chapel default
    if ((callStackSize = chpl_task_getEnvCallStackSize()) > 0 ||
        (chpl_qt_getenv_num("STACK_SIZE", 0) == 0 &&
         (callStackSize = chpl_task_getDefaultCallStackSize()) > 0)) {
        char newenv_stack[QT_ENV_S];
        snprintf(newenv_stack, sizeof(newenv_stack), "%zu", callStackSize);
        chpl_qt_setenv("STACK_SIZE", newenv_stack, 1);

        // Qthreads sets up memory pools expecting the item_size to be small.
        // Stacks are allocated in this manner too, but our default stack size
        // is quite large, so we limit the max memory allocated for a pool. We
        // default to a multiple of callStackSize and hwpar, with the thought
        // that available memory is generally proportional to the amount of
        // parallelism. For some architectures, this isn't true so we set a max
        // upper bound. And if the callStackSize is small, we don't want to
        // limit all qthreads pool allocations to a small value, so we have a
        // lower bound as well. Note that qthread stacks are slightly larger
        // than specified to store a book keeping structure and possibly guard
        // pages, so we thrown an extra MB.
        if (hwpar > 0) {
            const size_t oneMB = 1024 * 1024;
            const size_t allocSizeLowerBound = 33 * oneMB;
            const size_t allocSizeUpperBound = 65 * oneMB;
            size_t maxPoolAllocSize;
            char newenv_alloc[QT_ENV_S];

            maxPoolAllocSize = 2 * hwpar * callStackSize + oneMB;
            if (maxPoolAllocSize < allocSizeLowerBound) {
                maxPoolAllocSize = allocSizeLowerBound;
            } else if (maxPoolAllocSize > allocSizeUpperBound) {
                maxPoolAllocSize = allocSizeUpperBound;
            }
            snprintf(newenv_alloc, sizeof(newenv_alloc), "%zu", maxPoolAllocSize);
            chpl_qt_setenv("MAX_POOL_ALLOC_SIZE", newenv_alloc, 0);
        }
    }
}

static void setupTasklocalStorage(void) {
    unsigned long int tasklocal_size;
    char newenv[QT_ENV_S];

    // Make sure Qthreads knows how much space we need for per-task
    // local storage.
    tasklocal_size = chpl_qt_getenv_num("TASKLOCAL_SIZE", 0);
    if (tasklocal_size < sizeof(chpl_qthread_tls_t)) {
        snprintf(newenv, sizeof(newenv), "%zu", sizeof(chpl_qthread_tls_t));
        chpl_qt_setenv("TASKLOCAL_SIZE", newenv, 1);
    }
}

static void setupWorkStealing(void) {
    // In our experience the current work stealing implementation hurts
    // performance, so disable it. Note that we don't override, so a user could
    // try working stealing out by setting {QT,QTHREAD}_STEAL_RATIO. Also note
    // that not all schedulers support work stealing, but it doesn't hurt to
    // set this env var for those configs anyways.
    chpl_qt_setenv("STEAL_RATIO", "0", 0);
}

static void setupSpinWaiting(void) {
  const char *crayPlatform = "cray-x";
  if (strncmp(crayPlatform, CHPL_TARGET_PLATFORM, strlen(crayPlatform)) == 0) {
    chpl_qt_setenv("SPINCOUNT", "3000000", 0);
  }
}

void chpl_task_init(void)
{
    int32_t   commMaxThreads;
    int32_t   hwpar;
    pthread_t initer;
    pthread_attr_t pAttr;

    chpl_qthread_process_pthread = pthread_self();
    chpl_qthread_process_bundle.id = qthread_incr(&next_task_id, 1);

    commMaxThreads = chpl_comm_getMaxThreads();

    // Set up hardware parallelism, the stack size and stack guards,
    // tasklocal storage, and work stealing
    hwpar = setupAvailableParallelism(commMaxThreads);
    setupCallStacks(hwpar);
    setupTasklocalStorage();
    setupWorkStealing();
    setupSpinWaiting();

    if (verbosity >= 2) { chpl_qt_setenv("INFO", "1", 0); }

    // Initialize qthreads
    pthread_attr_init(&pAttr);
    pthread_attr_setdetachstate(&pAttr, PTHREAD_CREATE_DETACHED);
    pthread_create(&initer, &pAttr, initializer, NULL);
    while (chpl_qthread_done_initializing == 0)
        sched_yield();

    // Now that Qthreads is up and running, do a sanity check and make sure
    // that the number of workers is less than any comm layer limit. This is
    // mainly need for the case where a user set QT_NUM_SHEPHERDS and/or
    // QT_NUM_WORKERS_PER_SHEPHERD in which case we don't impose any limits on
    // the number of threads qthreads creates beforehand
    assert(0 == commMaxThreads || qthread_num_workers() < commMaxThreads);

    if (blockreport || taskreport) {
        if (signal(SIGINT, SIGINT_handler) == SIG_ERR) {
            perror("Could not register SIGINT handler");
        }
    }
}

void chpl_task_exit(void)
{
#ifdef CHAPEL_PROFILE
    profile_print();
#endif /* CHAPEL_PROFILE */

    if (qthread_shep() == NO_SHEPHERD) {
        /* sometimes, tasking is told to shutdown even though it hasn't been
         * told to start yet */
        if (chpl_qthread_done_initializing == 1) {
            qthread_syncvar_fill(&canexit);
            while (done_finalizing == 0)
                sched_yield();
        }
    } else {
        qthread_syncvar_fill(&exit_ret);
    }
}

static inline void wrap_callbacks(chpl_task_cb_event_kind_t event_kind,
                                  chpl_task_bundle_t* bundle) {
    if (chpl_task_have_callbacks(event_kind)) {
        if (bundle->id == chpl_nullTaskID)
            bundle->id = qthread_incr(&next_task_id, 1);
        chpl_task_do_callbacks(event_kind,
                               bundle->requested_fid,
                               bundle->filename,
                               bundle->lineno,
                               bundle->id,
                               bundle->is_executeOn);
    }
}


// If we stored chpl_taskID_t in chpl_task_bundleData_t,
// this struct and the following function may not be necessary.
typedef void (*main_ptr_t)(void);
typedef struct {
  chpl_task_bundle_t arg;
  main_ptr_t chpl_main;
} main_wrapper_bundle_t;

static aligned_t main_wrapper(void *arg)
{
    chpl_qthread_tls_t         *tls = chpl_qthread_get_tasklocal();
    main_wrapper_bundle_t *m_bundle = (main_wrapper_bundle_t*) arg;
    chpl_task_bundle_t      *bundle = &m_bundle->arg;
    chpl_qthread_tls_t         pv = {.bundle = bundle};

    *tls = pv;

    wrap_callbacks(chpl_task_cb_event_kind_begin, bundle);

    (m_bundle->chpl_main)();

    wrap_callbacks(chpl_task_cb_event_kind_end, bundle);

    return 0;
}


static aligned_t chapel_wrapper(void *arg)
{
    chpl_qthread_tls_t    *tls = chpl_qthread_get_tasklocal();
    chpl_task_bundle_t *bundle = (chpl_task_bundle_t*) arg;
    chpl_qthread_tls_t      pv = {.bundle = bundle};

    *tls = pv;

    wrap_callbacks(chpl_task_cb_event_kind_begin, bundle);

    (bundle->requested_fn)(arg);

    wrap_callbacks(chpl_task_cb_event_kind_end, bundle);

    return 0;
}

typedef struct {
    chpl_fn_p fn;
    void *arg;
} comm_task_wrapper_info_t;

static void *comm_task_wrapper(void *arg)
{
    comm_task_wrapper_info_t *rarg = arg;
    chpl_moveToLastCPU();
    (*(chpl_fn_p)(rarg->fn))(rarg->arg);
    return 0;
}

// Start the main task.
//
// Warning: this method is not called within a Qthread task context. Do
// not use methods that require task context (e.g., task-local storage).
void chpl_task_callMain(void (*chpl_main)(void))
{
    // Be sure to initialize Chapel managed task-local state with zeros
    main_wrapper_bundle_t arg = { .chpl_main = NULL };

    arg.arg.is_executeOn      = false;
    arg.arg.requestedSubloc   = c_sublocid_any_val;
    arg.arg.requested_fid     = FID_NONE;
    arg.arg.requested_fn      = NULL;
    arg.arg.lineno            = 0;
    arg.arg.filename           = CHPL_FILE_IDX_MAIN_TASK;
    arg.arg.id                = chpl_qthread_process_bundle.id;
    arg.chpl_main             = chpl_main;

    wrap_callbacks(chpl_task_cb_event_kind_create, &arg.arg);

    qthread_fork_syncvar_copyargs(main_wrapper, &arg, sizeof(arg), &exit_ret);
    qthread_syncvar_readFF(NULL, &exit_ret);
}

void chpl_task_stdModulesInitialized(void)
{
}

int chpl_task_createCommTask(chpl_fn_p fn,
                             void     *arg)
{
    //
    // The wrapper info must be static because it won't be referred to
    // until the new pthread calls comm_task_wrapper().  And, it is
    // safe for it to be static because we will be called at most once
    // on each node.
    //
    static
        comm_task_wrapper_info_t wrapper_info;
    wrapper_info.fn = fn;
    wrapper_info.arg = arg;
    return pthread_create(&chpl_qthread_comm_pthread,
                          NULL, comm_task_wrapper, &wrapper_info);
}

void chpl_task_addToTaskList(chpl_fn_int_t       fid,
                             chpl_task_bundle_t *arg,
                             size_t              arg_size,
                             c_sublocid_t        full_subloc,
                             void              **task_list,
                             int32_t             task_list_locale,
                             chpl_bool           is_begin_stmt,
                             int                 lineno,
                             int32_t             filename)
{
    chpl_fn_p requested_fn = chpl_ftable[fid];

    assert(isActualSublocID(full_subloc) || full_subloc == c_sublocid_any);

    PROFILE_INCR(profile_task_addToTaskList,1);

    c_sublocid_t execution_subloc =
      chpl_localeModel_sublocToExecutionSubloc(full_subloc);

    arg->is_executeOn      = false;
    arg->requestedSubloc   = full_subloc;
    arg->requested_fid     = fid;
    arg->requested_fn      = requested_fn;
    arg->lineno            = lineno;
    arg->filename          = filename;
    arg->id                = chpl_nullTaskID;

    wrap_callbacks(chpl_task_cb_event_kind_create, arg);

    if (execution_subloc == c_sublocid_any) {
        qthread_fork_copyargs(chapel_wrapper, arg, arg_size, NULL);
    } else {
        qthread_fork_copyargs_to(chapel_wrapper, arg, arg_size, NULL,
                                 (qthread_shepherd_id_t) execution_subloc);
    }
}

void chpl_task_executeTasksInList(void **task_list)
{
    PROFILE_INCR(profile_task_executeTasksInList,1);
}

static inline void taskCallBody(chpl_fn_int_t fid, chpl_fn_p fp,
                                void *arg, size_t arg_size,
                                c_sublocid_t full_subloc,
                                int lineno, int32_t filename)
{
    chpl_task_bundle_t *bundle = (chpl_task_bundle_t*) arg;
    c_sublocid_t execution_subloc =
      chpl_localeModel_sublocToExecutionSubloc(full_subloc);

    bundle->is_executeOn       = true;
    bundle->requestedSubloc    = full_subloc;
    bundle->requested_fid      = fid;
    bundle->requested_fn       = fp;
    bundle->lineno             = lineno;
    bundle->filename           = filename;
    bundle->id                 = chpl_nullTaskID;

    wrap_callbacks(chpl_task_cb_event_kind_create, bundle);

    if (execution_subloc < 0) {
        qthread_fork_copyargs(chapel_wrapper, arg, arg_size, NULL);
    } else {
        qthread_fork_copyargs_to(chapel_wrapper, arg, arg_size, NULL,
                                 (qthread_shepherd_id_t) execution_subloc);
    }
}

void chpl_task_taskCallFTable(chpl_fn_int_t fid,
                              chpl_task_bundle_t *arg, size_t arg_size,
                              c_sublocid_t subloc,
                              int lineno, int32_t filename)
{
    PROFILE_INCR(profile_task_taskCallFTable,1);

    taskCallBody(fid, chpl_ftable[fid], arg, arg_size, subloc, lineno, filename);
}

void chpl_task_startMovedTask(chpl_fn_int_t       fid,
                              chpl_fn_p           fp,
                              chpl_task_bundle_t *arg,
                              size_t              arg_size,
                              c_sublocid_t        subloc,
                              chpl_taskID_t       id)
{
    //
    // For now the incoming task ID is simply dropped, though we check
    // to make sure the caller wasn't expecting us to do otherwise.  If
    // we someday make task IDs global we will need to be able to set
    // the ID of this moved task.
    //
    assert(id == chpl_nullTaskID);

    PROFILE_INCR(profile_task_startMovedTask,1);

    taskCallBody(fid, fp, arg, arg_size, subloc, 0, CHPL_FILE_IDX_UNKNOWN);
}

//
// chpl_task_getSubloc() is in tasks-qthreads.h
//

//
// chpl_task_setSubloc() is in tasks-qthreads.h
//

//
// chpl_task_getRequestedSubloc() is in tasks-qthreads.h
//


// Returns '(unsigned int)-1' if called outside of the tasking layer.
chpl_taskID_t chpl_task_getId(void)
{
    chpl_qthread_tls_t *tls = chpl_qthread_get_tasklocal();
    chpl_taskID_t *id_ptr = NULL;

    PROFILE_INCR(profile_task_getId,1);

    if (tls == NULL)
        return (chpl_taskID_t) -1;

    id_ptr = &tls->bundle->id;

    if (*id_ptr == chpl_nullTaskID)
        *id_ptr = qthread_incr(&next_task_id, 1);

    return *id_ptr;
}

chpl_bool chpl_task_idEquals(chpl_taskID_t id1, chpl_taskID_t id2) {
  return id1 == id2;
}

char* chpl_task_idToString(char* buff, size_t size, chpl_taskID_t id) {
  int ret = snprintf(buff, size, "%u", id);
  if(ret>0 && ret<size)
    return buff;
  else
    return NULL;
}

void chpl_task_sleep(double secs)
{
    if (qthread_shep() == NO_SHEPHERD) {
        struct timeval deadline;
        struct timeval now;

        //
        // Figure out when this task can proceed again, and until then, keep
        // yielding.
        //
        gettimeofday(&deadline, NULL);
        deadline.tv_usec += (suseconds_t) lround((secs - trunc(secs)) * 1.0e6);
        if (deadline.tv_usec > 1000000) {
            deadline.tv_sec++;
            deadline.tv_usec -= 1000000;
        }
        deadline.tv_sec += (time_t) trunc(secs);

        do {
            chpl_task_yield();
            gettimeofday(&now, NULL);
        } while (now.tv_sec < deadline.tv_sec
                 || (now.tv_sec == deadline.tv_sec
                     && now.tv_usec < deadline.tv_usec));
    } else {
        qtimer_t t = qtimer_create();
        qtimer_start(t);
        do {
            qthread_yield();
            qtimer_stop(t);
        } while (qtimer_secs(t) < secs);
        qtimer_destroy(t);
    }
}

uint32_t chpl_task_getMaxPar(void) {
    //
    // We assume here that the caller (in the LocaleModel module code)
    // is interested in the number of workers on the whole node, and
    // will decide itself how much parallelism to create across and
    // within sublocales, if there are any.
    //
    return (uint32_t) qthread_num_workers();
}

c_sublocid_t chpl_task_getNumSublocales(void)
{
    // FIXME: What we really want here is the number of NUMA
    // sublocales we are supporting.  For now we use the number of
    // shepherds as a proxy for that.
    return (c_sublocid_t) qthread_num_shepherds();
}

size_t chpl_task_getCallStackSize(void)
{
    PROFILE_INCR(profile_task_getCallStackSize,1);

    return qthread_readstate(STACK_SIZE);
}

// XXX: Should probably reflect all shepherds
uint32_t chpl_task_getNumQueuedTasks(void)
{
    return qthread_readstate(NODE_BUSYNESS);
}

int32_t chpl_task_getNumBlockedTasks(void)
{
    // This isn't accurate, but in the absence of better information
    // it's the best we can do.
    return 0;
}

// Threads

uint32_t chpl_task_getNumThreads(void)
{
    return (uint32_t)qthread_num_workers();
}

// Ew. Talk about excessive bookkeeping.
uint32_t chpl_task_getNumIdleThreads(void)
{
    return 0;
}

/* vim:set expandtab: */
