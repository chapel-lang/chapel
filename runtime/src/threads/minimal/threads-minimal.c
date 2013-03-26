//
// Minimal implementation of Chapel threading interface.
//

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chplrt.h"
#include "chpl-mem.h"
#include "chplcast.h"
#include "chpl-threads.h"
#include "config.h"
#include "error.h"
#include <inttypes.h>
#include <sched.h>
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <pthread.h>

static size_t threadCallStackSize = 0;

static void *saved_private_thread_data = NULL;


// Mutexes

void chpl_thread_mutexInit(chpl_thread_mutex_p mutex) {
  chpl_thread_mutexUnlock(mutex);
}

chpl_thread_mutex_p chpl_thread_mutexNew(void) {
  chpl_thread_mutex_p m;
  m = (chpl_thread_mutex_p) chpl_mem_alloc(sizeof(chpl_thread_mutex_t),
                                           CHPL_RT_MD_MUTEX, 0, 0);
  chpl_thread_mutexInit(m);
  return m;
}

void chpl_thread_mutexLock(chpl_thread_mutex_p mutex)
{
  *mutex = true;
}

void chpl_thread_mutexUnlock(chpl_thread_mutex_p mutex)
{
  *mutex = false;
}


// Thread management

chpl_thread_id_t chpl_thread_getId(void)
{
  return -1;
}


void chpl_thread_yield(void)
{
  sched_yield();
}


void chpl_thread_init(int32_t numThreadsPerLocale,
                      int32_t maxThreadsPerLocale,
                      uint64_t callStackSize,
                      void(*threadBeginFn)(void*),
                      void(*threadEndFn)(void)) {
  //
  // If a value was specified for the call stack size config const, use
  // that (rounded up to a whole number of pages) to set the system
  // stack limit.
  //
  if (callStackSize != 0) {
    uint64_t      pagesize = (uint64_t) sysconf(_SC_PAGESIZE);
    struct rlimit rlim;

    callStackSize = (callStackSize + pagesize - 1) & ~(pagesize - 1);

    if (getrlimit(RLIMIT_STACK, &rlim) != 0)
      chpl_internal_error("getrlimit() failed");

    if (rlim.rlim_max != RLIM_INFINITY && callStackSize > rlim.rlim_max) {
      char warning[128];
      sprintf(warning, "call stack size capped at %lu\n", 
              (unsigned long)rlim.rlim_max);
      chpl_warning(warning, 0, NULL);

      callStackSize = rlim.rlim_max;
    }

    rlim.rlim_cur = threadCallStackSize = callStackSize;

    if (setrlimit(RLIMIT_STACK, &rlim) != 0)
      chpl_internal_error("setrlimit() failed");
  }
}

int chpl_thread_createCommThread(chpl_fn_p fn, void* arg) {
  pthread_t polling_thread;
  return pthread_create(&polling_thread, NULL, (void*(*)(void*))fn, arg);
}

void chpl_thread_exit(void) { }

chpl_bool chpl_thread_canCreate(void)
{
  return false;
}

int chpl_thread_create(void* arg)
{
  return -1;
}

void chpl_thread_destroy(void)
{
  //
  // We only have one thread, which we never destroy.  Instead we just
  // yield the processor.  Eventually, the thread will return to its
  // caller, which is allowed behavior under the threadlayer API.
  //
  sched_yield();
}

void* chpl_thread_getPrivateData(void)
{
  return saved_private_thread_data;
}

void chpl_thread_setPrivateData(void* p)
{
  saved_private_thread_data = p;
}

uint32_t chpl_thread_getMaxThreads(void)
{
  return 1;
}

uint32_t chpl_thread_getNumThreads(void)
{
  return 1;
}

uint64_t chpl_thread_getCallStackSize(void)
{
  return threadCallStackSize;
}
