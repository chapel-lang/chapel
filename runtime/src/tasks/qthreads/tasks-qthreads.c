//
// Qthreads implementation of Chapel tasking interface
//
// Copyright 2011 Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000, there is a non-exclusive license for use of this work by
// or on behalf of the U.S. Government. Export of this program may require a
// license from the United States Government
//

/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "chplrt.h"

#include "arg.h"
#include "error.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chpl-env.h"
#include "chplexit.h"
#include "chpl-locale-model.h"
#include "chpl-mem.h"
#include "chplsys.h"
#include "chpl-linefile-support.h"
#include "chpl-tasks.h"
#include "chpl-tasks-callbacks-internal.h"
#include "chpl-tasks-impl.h"
#include "chpl-topo.h"

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

#define ALIGN_DN(i, size)  ((i) & ~((size) - 1))
#define ALIGN_UP(i, size)  ALIGN_DN((i) + (size) - 1, size)

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
static aligned_t canexit = 0;
static pthread_mutex_t init_mutex = PTHREAD_MUTEX_INITIALIZER;

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

static pthread_t initer;

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
                               .bundle = &chpl_qthread_process_bundle };

chpl_qthread_tls_t chpl_qthread_comm_task_tls = {
                               .bundle = &chpl_qthread_comm_task_bundle };

//
// chpl_qthread_get_tasklocal() is in chpl-tasks-impl.h
//

static aligned_t exit_ret = 0;

static chpl_bool guardPagesInUse = true;

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
    PROFILE_INCR(profile_sync_lock, 1);

    qthread_lock(&s->lock);
}

void chpl_sync_unlock(chpl_sync_aux_t *s)
{
    PROFILE_INCR(profile_sync_unlock, 1);

    qthread_unlock(&s->lock);
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                               int32_t          lineno,
                               int32_t         filename)
{
    PROFILE_INCR(profile_sync_waitFullAndLock, 1);

    chpl_sync_lock(s);
    while (s->is_full == 0) {
        chpl_sync_unlock(s);
        qthread_readFE(NULL, &(s->signal_full));
        chpl_sync_lock(s);
    }
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s,
                                int32_t          lineno,
                                int32_t         filename)
{
    PROFILE_INCR(profile_sync_waitEmptyAndLock, 1);

    chpl_sync_lock(s);
    while (s->is_full != 0) {
        chpl_sync_unlock(s);
        qthread_readFE(NULL, &(s->signal_empty));
        chpl_sync_lock(s);
    }
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s)         // and unlock
{
    PROFILE_INCR(profile_sync_markAndSignalFull, 1);

    qthread_fill(&(s->signal_full));
    s->is_full = 1;
    chpl_sync_unlock(s);
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s)         // and unlock
{
    PROFILE_INCR(profile_sync_markAndSignalEmpty, 1);

    qthread_fill(&(s->signal_empty));
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

    s->lock         = 0;
    s->is_full      = 0;
    s->signal_empty = 0;
    s->signal_full  = 0;
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s)
{
    PROFILE_INCR(profile_sync_destroyAux, 1);
}

static void SIGINT_handler(int sig)
{
    signal(sig, SIG_IGN);

    if (blockreport) {
        fprintf(stderr,
                "Blockreport is currently unsupported by the qthreads "
                "tasking layer.\n");
    }

    if (taskreport) {
        fprintf(stderr, "Taskreport is currently unsupported by the qthreads tasking layer.\n");
    }

    chpl_exit_any(1);
}

// We call this routine in a separate pthread for 2 main reasons:
// 1) qthread_initialize() converts the current thread into a shepherd (the
//    "real mccoy" shepherd) and creates a qthread on top of that. If we called
//    this from the main process we'd have to be very careful about using
//    pthread mutexes, sched_yield, and other similar constructs that don't
//    play well with qthreads in the rest of the runtime. That's a lot of
//    effort and easy to forget about, so we want to avoid that.
//
// 2) qthread_finalize() only does anything when called from the original
//    context that called qthread_initialize, so this is an easy way to ensure
//    that.
static void *initializer(void *junk)
{
    qthread_initialize();
    qthread_purge(&canexit);
    (void) pthread_mutex_lock(&init_mutex);
    chpl_qthread_done_initializing = 1;
    (void) pthread_mutex_unlock(&init_mutex);

    qthread_readFF(NULL, &canexit);

    qthread_finalize();

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

static chpl_bool chpl_qt_getenv_bool(const char* var,
                                     chpl_bool default_val) {
    char* ev;
    chpl_bool ret_val = default_val;

    if ((ev = chpl_qt_getenv_str(var)) != NULL) {
        ret_val = chpl_env_str_to_bool(var, ev, default_val);
    }

    return ret_val;
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
        chpl_env_set(qt_env, val, 1);
        chpl_env_set(qthread_env, val, 1);
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


// Setup the amount of hardware parallelism, limited to maxThreads.
static void setupAvailableParallelism(int32_t maxThreads) {
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

        numPUsPerLocale = chpl_topo_getNumCPUsLogical(true);
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
        hwpar = chpl_topo_getNumCPUsPhysical(true);
    }

    // hwpar will only be <= 0 if the user set QT_NUM_SHEPHERDS and/or
    // QT_NUM_WORKERS_PER_SHEPHERD in which case we assume as "expert" user and
    // don't impose any thread limits or set worker_unit.
    if (hwpar > 0) {
        // Limit the parallelism to the maximum imposed by the comm layer.
        if (0 < maxThreads && maxThreads < hwpar) {
            hwpar = maxThreads;
        }

        //
        // If we have NUMA sublocales we have to have at least that many
        // shepherds, or we'll get internal errors when the module code
        // tries to fire tasks on those sublocales.
        //
        {
            int numNumaDomains = chpl_topo_getNumNumaDomains();
            if (hwpar < numNumaDomains
                && strcmp(CHPL_LOCALE_MODEL, "flat") != 0) {
                char msg[100];
                snprintf(msg, sizeof(msg),
                         "%d NUMA domains but only %d Qthreads shepherds; "
                         "may get internal errors",
                         numNumaDomains, (int) hwpar);
                chpl_warning(msg, 0, 0);
            }
        }

        // If there is more parallelism requested than the number of cores, set the
        // worker unit to pu, otherwise core.
        if (hwpar > chpl_topo_getNumCPUsPhysical(true)) {
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
}

static chpl_bool setupGuardPages(void) {
    const char *armArch = "arm-thunderx";
    chpl_bool guardPagesEnabled = true;
    // default value set by compiler (--[no-]stack-checks)
    chpl_bool defaultVal = (CHPL_STACK_CHECKS == 1);

    // Setup guard pages. Default to enabling guard pages, only disabling them
    // under the following conditions (Precedence high-to-low):
    //  - Guard pages disabled at configure time
    //  - Guard pages not supported because of huge pages
    //  - Guard pages not supported by processor
    //  - QT_GUARD_PAGES set to a 'false' value
    //  - CHPL_STACK_CHECKS set (--no-stack-checks thrown at compilation time)
    if (!CHPL_QTHREAD_HAVE_GUARD_PAGES) {
        guardPagesEnabled = false;
    } else if (chpl_getHeapPageSize() != chpl_getSysPageSize()) {
        guardPagesEnabled = false;
    } else if (strncmp(armArch, CHPL_TARGET_CPU, strlen(armArch)) == 0) {
        guardPagesEnabled = false;
    } else {
        guardPagesEnabled = chpl_qt_getenv_bool("GUARD_PAGES", defaultVal);
    }

    chpl_qt_setenv("GUARD_PAGES", guardPagesEnabled ? "true" : "false", 1);
    return guardPagesEnabled;
}

static void setupCallStacks(void) {
    size_t stackSize;
    size_t actualStackSize;
    size_t envStackSize;
    char newenv_stack[QT_ENV_S];

    int guardPagesEnabled;
    size_t pagesize;
    size_t reservedPages;
    size_t qt_rtds_size;

    size_t maxPoolAllocSize;

    char newenv_alloc[QT_ENV_S];

    guardPagesInUse = setupGuardPages();
    guardPagesEnabled = (int)guardPagesInUse;

    // Setup the base call stack size (Precedence high-to-low):
    // 1) Chapel environment (CHPL_RT_CALL_STACK_SIZE)
    // 2) QT_STACK_SIZE
    // 3) Chapel default
    if ((envStackSize = chpl_task_getEnvCallStackSize()) > 0) {
        stackSize = envStackSize;
    } else if ((envStackSize = (size_t)chpl_qt_getenv_num("STACK_SIZE", 0)) > 0) {
        stackSize = envStackSize;
    } else {
        stackSize = chpl_task_getDefaultCallStackSize();
    }

    // We want the entire "stack" including some qthreads runtime data
    // structures and guard pages (if they're enabled) to fit within the stack
    // size envelope. The main motivation for this is for systems with
    // hugepages, we don't want to waste an entire hugepage just for the
    // runtime structure.
    pagesize = chpl_getSysPageSize();
    qt_rtds_size = qthread_readstate(RUNTIME_DATA_SIZE);
    qt_rtds_size = ALIGN_UP(qt_rtds_size, pagesize);
    reservedPages = qt_rtds_size / pagesize;
    reservedPages += 2 * guardPagesEnabled;

    if (reservedPages * pagesize >= stackSize) {
        char msg[1024];
        size_t guardSize = 2 * guardPagesEnabled * pagesize;
        size_t rtdsSize = (reservedPages * pagesize) - guardSize;

        stackSize = (reservedPages + 1) * pagesize;

        sprintf(msg, "Stack size was too small, increasing to %zu bytes (which"
                     " may still not be enough).\n  Note: guard pages use %zu"
                     " bytes and qthread task data structures use %zu bytes.",
                     stackSize, guardSize, rtdsSize);
        chpl_warning(msg, 0, 0);
    } else {
        stackSize = ALIGN_UP(stackSize, pagesize);
    }

    actualStackSize = stackSize - (reservedPages * pagesize);

    snprintf(newenv_stack, sizeof(newenv_stack), "%zu", actualStackSize);
    chpl_qt_setenv("STACK_SIZE", newenv_stack, 1);

    // Setup memory pooling. Qthreads expects the item_size of memory pools to
    // be small so they try to pool many objects. Stacks are allocated with
    // pools too, but our default stack size is huge, so we limit the max size
    // of a pool so pools don't use an absurd amount of memory.  We choose
    // enough space for 2 "stacks", with a minimum of 1MB so we don't make the
    // pool size too small if the user has lowered the stack size.
    maxPoolAllocSize = 2 * stackSize;
    if (maxPoolAllocSize < (1<<20)) {
        maxPoolAllocSize = (1<<20);
    }
    snprintf(newenv_alloc, sizeof(newenv_alloc), "%zu", maxPoolAllocSize);
    chpl_qt_setenv("MAX_POOL_ALLOC_SIZE", newenv_alloc, 0);
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
  if (chpl_env_rt_get_bool("OVERSUBSCRIBED", false)) {
    chpl_qt_setenv("SPINCOUNT", "300", 0);
  } else if (strncmp(crayPlatform, CHPL_TARGET_PLATFORM, strlen(crayPlatform)) == 0) {
    chpl_qt_setenv("SPINCOUNT", "3000000", 0);
  }
}

static void setupAffinity(void) {
  if (chpl_env_rt_get_bool("OVERSUBSCRIBED", false)) {
    chpl_qt_setenv("AFFINITY", "no", 0);
  }
}

void chpl_task_init(void)
{
    int32_t   commMaxThreads;

    chpl_qthread_process_pthread = pthread_self();
    chpl_qthread_process_bundle.id = qthread_incr(&next_task_id, 1);

    commMaxThreads = chpl_comm_getMaxThreads();

    // Set up hardware parallelism, the stack size and stack guards,
    // tasklocal storage, and work stealing
    setupAvailableParallelism(commMaxThreads);
    setupCallStacks();
    setupTasklocalStorage();
    setupWorkStealing();
    setupSpinWaiting();
    setupAffinity();

    if (verbosity >= 2) { chpl_qt_setenv("INFO", "1", 0); }

    // Initialize qthreads
    pthread_create(&initer, NULL, initializer, NULL);
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
            qthread_fill(&canexit);
            pthread_join(initer, NULL);
        }
    } else {
        qthread_fill(&exit_ret);
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

    qthread_fork_copyargs(main_wrapper, &arg, sizeof(arg), &exit_ret);
    qthread_readFF(NULL, &exit_ret);
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
    static comm_task_wrapper_info_t wrapper_info;
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
// chpl_task_getSubloc() is in chpl-tasks-impl.h
//

//
// chpl_task_setSubloc() is in chpl-tasks-impl.h
//

//
// chpl_task_getRequestedSubloc() is in chpl-tasks-impl.h
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

size_t chpl_task_getCallStackSize(void)
{
    PROFILE_INCR(profile_task_getCallStackSize,1);

    return qthread_readstate(STACK_SIZE);
}

chpl_bool chpl_task_guardPagesInUse(void)
{
  return guardPagesInUse;
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

uint32_t chpl_task_impl_getFixedNumThreads(void) {
    assert(chpl_qthread_done_initializing);
    return (uint32_t)qthread_num_workers();
}

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
