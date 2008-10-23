#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include "chplrt.h"
#include "chplcomm.h"
#include "chplmem.h"
#include "chplsys.h"
#include "chplthreads.h"
#include "error.h"

#define GASNET_PAR 1
#include "gasnet.h"

#define CHPL_DIST_DEBUG 0
static int chpl_comm_debug = 0; // set via _startCommDiagnosis in .chpl

//
// The following macro is from the GASNet test.h distribution
//
#define GASNET_Safe(fncall) do {                                        \
    int _retval;                                                        \
    if ((_retval = fncall) != GASNET_OK) {                              \
      fprintf(stderr, "ERROR calling: %s\n"                             \
              " at: %s:%i\n"                                            \
              " error: %s (%s)\n",                                      \
              #fncall, __FILE__, __LINE__,                              \
              gasnet_ErrorName(_retval), gasnet_ErrorDesc(_retval));    \
      fflush(stderr);                                                   \
      gasnet_exit(_retval);                                             \
    }                                                                   \
  } while(0)

typedef struct {
  int     caller;
  int    *ack;
  _Bool   serial_state; // true if not allowed to spawn new threads
  func_p  fun;
  int     arg_size;
  char    arg[0];       // variable-sized data here
} fork_t;

//
// AM functions
//
#define FORK_NB   128    // (non-blocking) async fork 
#define FORK      129    // synchronous fork
#define SIGNAL    130    // ack of synchronous fork

static void fork_nb_wrapper(fork_t *f) {
  if (f->arg_size)
    (*(f->fun))(&(f->arg));
  else
    (*(f->fun))(0);
  chpl_free(f, 0, 0);
}

// AM non-blocking fork handler, medium sized, receiver side
static void AM_fork_nb(gasnet_token_t  token,
                        void           *buf,
                        size_t          nbytes) {
  fork_t *f;

  f = (fork_t*)chpl_malloc(nbytes, sizeof(char), "", 0, 0);
  bcopy(buf, f, nbytes);
  chpl_begin((chpl_threadfp_t)fork_nb_wrapper, (chpl_threadarg_t)f,
             true, f->serial_state, NULL);
}

static void fork_wrapper(fork_t *f) {
  if (f->arg_size)
    (*(f->fun))(&(f->arg));
  else
    (*(f->fun))(0);
  GASNET_Safe(gasnet_AMRequestMedium0(f->caller,
                                      SIGNAL,
                                      &(f->ack),
                                      sizeof(f->ack)));
  chpl_free(f, 0, 0);
}

// AM fork handler, medium sized, receiver side
static void AM_fork(gasnet_token_t  token,
                     void           *buf,
                     size_t          nbytes) {
  fork_t *f;

  f = (fork_t*)chpl_malloc(nbytes, sizeof(char), "", 0, 0);
  bcopy(buf, f, nbytes);
  chpl_begin((chpl_threadfp_t)fork_wrapper, (chpl_threadarg_t)f,
             true, f->serial_state, NULL);
}

// AM signal handler, medium sized, receiver side
static void AM_signal(gasnet_token_t  token,
                       void          *buf,
                       size_t         nbytes) {
  int **done = (int**)buf;
  **done = 1;
}

static gasnet_handlerentry_t ftable[] = {
  {FORK,    AM_fork},    // fork remote thread synchronously
  {FORK_NB, AM_fork_nb}, // fork remote thread asynchronously
  {SIGNAL,  AM_signal},  // set remote (int) condition
};

static gasnet_seginfo_t seginfo_table[1024];
static char numLocalesString[64];
static int exitSignal = 0;
static int gasnet_init_called = 0;
static pthread_t polling_thread;

//
// Chapel interface starts here
//

int32_t _chpl_comm_getMaxThreads(void) {
  return 255;
}

int32_t _chpl_comm_maxThreadsLimit(void) {
  return 255;
}

int _chpl_comm_user_invocation(int argc, char* argv[]) {
#if defined(GASNET_CONDUIT_PORTALS)
  return false;
#else
  return ((argc <= 1) ||
          (argc > 1 && strcmp(argv[1], "__AMUDP_SLAVE_PROCESS__") != 0));
#endif
}

char** _chpl_comm_create_argcv(int32_t numLocales, int argc, char* argv[],
                               int* commArgc) {
  char** commArgv;
  int i;
  sprintf(numLocalesString, "%d", numLocales);
  *commArgc = argc+1;
  commArgv = chpl_malloc((*commArgc)+1, sizeof(char*), "GASNet argv", 
                          __LINE__, __FILE__);
  commArgv[0] = argv[0];
  commArgv[1] = numLocalesString;
  for (i=1; i< argc; i++) {
    commArgv[i+1] = argv[i];
  }
  commArgv[argc+1] = NULL;

  return commArgv;
}

static void polling(void* x) {
  GASNET_BLOCKUNTIL(exitSignal);
}

void _chpl_comm_init(int *argc_p, char ***argv_p, int runInGDB) {
  int needPollingThread = 1;
  //#if defined(GASNET_CONDUIT_PORTALS)
  //  needPollingThread = 0;
  //#endif

  if (runInGDB) {
    setenv("CHPL_COMM_USE_GDB", "true", 1);
  }
  gasnet_init(argc_p, argv_p);
  gasnet_init_called = 1;
  _localeID = gasnet_mynode();
  _numLocales = gasnet_nodes();
  GASNET_Safe(gasnet_attach(ftable, 
                            sizeof(ftable)/sizeof(gasnet_handlerentry_t),
                            gasnet_getMaxLocalSegmentSize(),
                            0));
#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
  GASNET_Safe(gasnet_getSegmentInfo(seginfo_table, 1024));
#endif
  //
  // start polling thread
  // this should call a special function in the threading interface
  // but remember that we have not yet initialized chapel threads!
  //
  if (needPollingThread) {
    int status;

    status = pthread_create(&polling_thread, NULL, (chpl_threadfp_t)polling, 0);
    if (status)
      chpl_internal_error("unable to start polling thread for gasnet");
    pthread_detach(polling_thread);
  }
}

void _chpl_comm_rollcall(void) {
  chpl_msg(2, "executing on locale %d of %d locale(s): %s\n", _localeID, 
           _numLocales, chpl_localeName());
}

void _chpl_comm_init_shared_heap(void) {
#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
  void* heapStart = numGlobalsOnHeap*sizeof(void*) + (char*)seginfo_table[_localeID].addr;
  size_t heapSize = seginfo_table[_localeID].size - numGlobalsOnHeap*sizeof(void*);
  initHeap(heapStart, heapSize);
#else
  initHeap(NULL, 0);
#endif
}

void _chpl_comm_alloc_registry(int numGlobals) {
#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
  _global_vars_registry = numGlobals == 0 ? NULL : seginfo_table[_localeID].addr;
#else
  _global_vars_registry = _global_vars_registry_static;
#endif
}

void _chpl_comm_broadcast_global_vars(int numGlobals) {
  int i;
  if (_localeID != 0) {
    for (i = 0; i < numGlobals; i++) {
#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
      _chpl_comm_get(_global_vars_registry[i], 0,
                     &((void**)seginfo_table[0].addr)[i], sizeof(void*));
#else
      _chpl_comm_get(_global_vars_registry[i], 0,
                     &_global_vars_registry[i], sizeof(void*));
#endif
    }
  }
}

#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
typedef struct __broadcast_private_helper {
  void* addr;
  void* raddr;
  int locale;
  size_t size;
} _broadcast_private_helper;

void _broadcastPrivateHelperFn(struct __broadcast_private_helper* arg);

void _broadcastPrivateHelperFn(struct __broadcast_private_helper* arg) {
  _chpl_comm_get(arg->addr, arg->locale, arg->raddr, arg->size);
}

#endif

void _chpl_comm_broadcast_private(void* addr, int size) {
  int i;
#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
  /* For each private constant, copy it to the heap, fork to each locale,
     and read it off the remote heap into its final non-heap location. Ick.
   */
  void* heapAddr = chpl_malloc(1, size, "broadcast private", 0, 0);
  _broadcast_private_helper bph;

  bcopy(addr, heapAddr, size);
  bph.addr = addr;
  bph.raddr = heapAddr;
  bph.locale = _localeID;
  bph.size = size;
#endif
  for (i = 0; i < _numLocales; i++) {
    if (i != _localeID) {
#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
      _chpl_comm_fork(i, (func_p)_broadcastPrivateHelperFn, &bph, sizeof(_broadcast_private_helper));
#else
      _chpl_comm_put(addr, i, addr, size);
#endif
    }
  }
}

void _chpl_comm_barrier(const char *msg) {
#if CHPL_DIST_DEBUG
  if (chpl_comm_debug)
    printf("%d: barrier for '%s'\n", _localeID, msg);
#endif
  gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);
  GASNET_Safe(gasnet_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS));
}

static void _chpl_comm_exit_common(int status) {
  if (gasnet_init_called) {
    int localExitSignal = 1;
    // DITEN: Is it required by GASNet to do a gasnet_put instead of a normal
    // assignment? exitSignal is always on the current locale.
    //gasnet_put(_localeID, &exitSignal, &localExitSignal, sizeof(int));
    exitSignal = localExitSignal;
    gasnet_exit(status);
  }
}

void _chpl_comm_exit_all(int status) {
  _chpl_comm_exit_common(status);
}

void _chpl_comm_exit_any(int status) {
  _chpl_comm_exit_common(status);
}

void  _chpl_comm_put(void* addr, int32_t locale, void* raddr, int32_t size) {
  if (_localeID == locale) {
    bcopy(addr, raddr, size);
  } else {
#if CHPL_DIST_DEBUG
  if (chpl_comm_debug)
    printf("%d: remote put to %d\n", _localeID, locale);
#endif
    gasnet_put(locale, raddr, addr, size); // node, dest, src, size
  }
}

void  _chpl_comm_get(void* addr, int32_t locale, const void* raddr, int32_t size) {
  if (_localeID == locale) {
    bcopy(raddr, addr, size);
  } else {
#if CHPL_DIST_DEBUG
  if (chpl_comm_debug)
    printf("%d: remote get from %d\n", _localeID, locale);
#endif
    gasnet_get(addr, locale, (void*)raddr, size); // dest, node, src, size
  }
}

void  _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size) {
  fork_t *info;
  int           info_size;

  info_size = sizeof(fork_t) + arg_size;
  info = (fork_t*) chpl_malloc(info_size, sizeof(char), "_chpl_comm_fork_nb", 0, 0);
  info->caller = _localeID;
  info->serial_state = chpl_get_serial();
  info->fun = f;
  info->arg_size = arg_size;
  if (arg_size)
    bcopy(arg, &(info->arg), arg_size);
  if (_localeID == locale) {
    chpl_begin((chpl_threadfp_t)fork_nb_wrapper, (chpl_threadarg_t)info,
               false, info->serial_state, NULL);
  } else {
#if CHPL_DIST_DEBUG
  if (chpl_comm_debug)
    printf("%d: remote non-blocking thread created on %d\n", _localeID, locale);
#endif
    GASNET_Safe(gasnet_AMRequestMedium0(locale, FORK_NB, info, info_size));
  }
}

void  _chpl_comm_fork(int locale, func_p f, void *arg, int arg_size) {
  fork_t *info;
  int          info_size;
  int          done;

  if (_localeID == locale) {
    (*f)(arg);
  } else {
#if CHPL_DIST_DEBUG
  if (chpl_comm_debug)
    printf("%d: remote thread created on %d\n", _localeID, locale);
#endif
    info_size = sizeof(fork_t) + arg_size;
    info = (fork_t*) chpl_malloc(info_size, sizeof(char), "_chpl_comm_fork", 0, 0);

    info->caller = _localeID;
    info->ack = &done;
    info->serial_state = chpl_get_serial();
    info->fun = f;
    info->arg_size = arg_size;
    if (arg_size)
      bcopy(arg, &(info->arg), arg_size);

    done = 0;
    GASNET_Safe(gasnet_AMRequestMedium0(locale, FORK, info, info_size));
    GASNET_BLOCKUNTIL(1==done);
  }
}

void chpl_startCommDiagnosis() {
  chpl_comm_debug = 1;
  _chpl_comm_broadcast_private(&chpl_comm_debug, sizeof(int));
}

void chpl_stopCommDiagnosis() {
  chpl_comm_debug = 0;
  _chpl_comm_broadcast_private(&chpl_comm_debug, sizeof(int));
}


