#include "chplrt.h"
#include "chplcomm.h"
#include "chplmem.h"
#include "chplthreads.h"
#include "error.h"

#define GASNET_PAR 1
#include "gasnet.h"

/*#define _DIST_DEBUG 1*/

#ifdef _DIST_DEBUG
#define PRINTF(_s)                                                      \
  printf("%d:%s\n", _chpl_comm_locale_id(), _s);                        \
  fflush(stdout)
#else
#define PRINTF(_s)
#endif // _DIST_DEBUG

/* The following macros were taken from the GasNet test.h distribution */
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

// AM functions
#define FORK_NB   128    // (non-blocking) async fork 
#define FORK      129    // synchronous fork
#define SIGNAL    130    // ack of synchronous fork
#define PTR_TABLE 131    // get address of remote pointer table
#define PTR_RET   132    // return pointer value

typedef struct {
  int   *done;     // local pointer to blocking condition
  void  *val;      // return value; if 0 is not legal value, fold with done
  void **val_addr; // where to store ti
} _AM_ptr_ret_t;

typedef struct {
  int     caller;
  int    *ack;
  func_p  fun;
  int     arg_size;
  void   *arg;      // variable-sized data here
} dist_fork_t;

void _AM_fork_nb(gasnet_token_t token, void *buf, size_t nbytes);
void _AM_fork(gasnet_token_t token, void *buf, size_t nbytes);
void _AM_signal(gasnet_token_t token, void *buf, size_t nbytes);
void _AM_ptr_table(gasnet_token_t token, _AM_ptr_ret_t *ret_info, size_t nbytes);
void _AM_ptr_ret(gasnet_token_t token, _AM_ptr_ret_t *ret_info, size_t nbytes);

gasnet_handlerentry_t ftable[] = {
  {FORK, _AM_fork},           // fork remote thread asynchronously
  {FORK_NB, _AM_fork_nb},     // fork remote thread synchronously
  {SIGNAL, _AM_signal},       // set remote (int) condition
  {PTR_TABLE, _AM_ptr_table}, // get address of global global ptr table
  {PTR_RET, _AM_ptr_ret}      // reply, return pointer value
};

void _AM_fork_nb_wrapper(dist_fork_t *i) {
  (*(i->fun))(&(i->arg));
  _chpl_free(i, 0, 0);
}


// AM non-blocking fork handler, medium sized, receiver side
void _AM_fork_nb(gasnet_token_t token, 
                 void   *buf,
                 size_t  nbytes) {
  _chpl_thread_t    t;
  dist_fork_t *fork_info;

  fork_info = (dist_fork_t*) _chpl_malloc(nbytes, sizeof(char), "", 0, 0);
  bcopy(buf, fork_info, nbytes);
  _chpl_thread_create(&t, NULL, (_void_star_fun_p)_AM_fork_nb_wrapper, (void*)fork_info);
}


void _AM_fork_wrapper(dist_fork_t *i) {
  PRINTF("_AM_fork_wrapper");

  if (i->arg_size)
    (*(i->fun))(&(i->arg));
  else
    (*(i->fun))(0);
  GASNET_Safe(gasnet_AMRequestMedium0(i->caller, 
                                      SIGNAL, 
                                      &(i->ack), 
                                      sizeof(i->ack)));
  _chpl_free(i, 0, 0);
}


// AM sfork handler, medium sized, receiver side
void _AM_fork(gasnet_token_t  token, 
       void    *buf,
       size_t   nbytes) {
  _chpl_thread_t     t;
  dist_fork_t *fork_info;

  PRINTF("_AM_fork");

  fork_info = (dist_fork_t*) _chpl_malloc(nbytes, sizeof(char), "", 0, 0);
  bcopy(buf, fork_info, nbytes);
  _chpl_thread_create(&t, NULL, (_void_star_fun_p)_AM_fork_wrapper, (void*)fork_info);
}


// AM signal handler, medium sized, receiver side
void _AM_signal(gasnet_token_t  token, 
                void    *buf,
                size_t   nbytes) {
  int **done = (int**)buf;

  PRINTF("_AM_signal");
  **done = 1;
}


// AM reply to return a pointer value
void _AM_ptr_ret(gasnet_token_t token, _AM_ptr_ret_t *ret_info, size_t nbytes) {
  PRINTF("_AM_ptr_ret");
  *(ret_info->val_addr) = ret_info->val;
  *(ret_info->done) = 1;
}


// AM get global table ptr handler, small sized, receiver side
void _AM_ptr_table(gasnet_token_t token, _AM_ptr_ret_t *ret_info, size_t nbytes) {
  PRINTF("_AM_ptr_table");
  // ret_info = (global_ret_int_t*) buf;
  // ret_info->val = (void*) globals_table;

  GASNET_Safe(gasnet_AMReplyMedium0(token, PTR_RET, ret_info, sizeof(_AM_ptr_ret_t)));
}


// Chapel interface starts here

int _chpl_comm_user_invocation(int argc, char* argv[]) {
  return ((argc <= 1) ||
          (argc > 1 && strcmp(argv[1], "__AMUDP_SLAVE_PROCESS__") != 0));
}


int _chpl_comm_default_num_locales(void) {
  _printError("Specify number of locales via -nl <#> or --numLocales=<#>", 0, 0);
  return 0;
}


void _chpl_comm_verify_num_locales(_int32 proposedNumLocales) {
}

static char numLocalesString[64];

char** _chpl_comm_create_argcv(_int32 numLocales, int argc, char* argv[],
                               int* commArgc) {
  char** commArgv;
  int i;
  sprintf(numLocalesString, "%d", numLocales);
  *commArgc = argc+1;
  commArgv = _chpl_malloc((*commArgc)+1, sizeof(char*), "GASNet argv", 
                          __LINE__, __FILE__);
  commArgv[0] = argv[0];
  commArgv[1] = numLocalesString;
  for (i=1; i< argc; i++) {
    commArgv[i+1] = argv[i];
  }
  commArgv[argc+1] = NULL;

  return commArgv;
}

static int gasnet_init_called = 0;

void _chpl_comm_init(int *argc_p, char ***argv_p) {
  gasnet_init(argc_p, argv_p);
  gasnet_init_called = 1;

  _localeID = gasnet_mynode();
  _numLocales = gasnet_nodes();

  GASNET_Safe(gasnet_attach(ftable, 
                            sizeof(ftable)/sizeof(gasnet_handlerentry_t),
                            0,   // share everything
                            0));
}

void _chpl_comm_rollcall(void) {
  _chpl_msg(2, "executing on locale %d of %d locale(s)\n", _localeID, 
            _numLocales);
}

void _chpl_comm_barrier(const char *msg) {
  PRINTF(msg);
  gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);
  GASNET_Safe(gasnet_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS));
  PRINTF(msg);
}

static void _chpl_comm_exit_common(int status) {
  if (gasnet_init_called) {
    gasnet_exit(status);
  }
}

void _chpl_comm_exit_all(int status) {
  _chpl_comm_exit_common(status);
}

void _chpl_comm_exit_any(int status) {
  _chpl_comm_exit_common(status);
}

void  _chpl_comm_put(void* addr, _int32 locale, void* raddr, _int32 size) {
  PRINTF("_chpl_comm_write");
  if (_localeID == locale) {
    bcopy(addr, raddr, size);
  } else {
    gasnet_put(locale, raddr, addr, size); // node, dest, src, size
  }
}

void  _chpl_comm_get(void *addr, _int32 locale, void* raddr, _int32 size) {
  PRINTF("_chpl_comm_read");
  if (_localeID == locale) {
    bcopy(raddr, addr, size);
  } else {
    gasnet_get(addr, locale, raddr, size); // dest, node, src, size
  }
}

void  _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size) {
  dist_fork_t *info;
  int           info_size;

  PRINTF("_chpl_comm_fork_nb");

  info_size = sizeof(dist_fork_t) - sizeof(void*) + arg_size;
  info = (dist_fork_t*) _chpl_malloc(info_size, sizeof(char), "", 0, 0);

  info->caller = _localeID;
  info->fun = f;
  info->arg_size = arg_size;
  bcopy(arg, &(info->arg), arg_size);

  GASNET_Safe(gasnet_AMRequestMedium0(locale, FORK_NB, info, info_size));
}

void  _chpl_comm_fork(int locale, func_p f, void *arg, int arg_size) {
  dist_fork_t *info;
  int          info_size;
  int          done;

  PRINTF("_chpl_comm_fork");

  info_size = sizeof(dist_fork_t) - sizeof(void*) + arg_size;
  info = (dist_fork_t*) _chpl_malloc(info_size, sizeof(char), "", 0, 0);

  info->caller = _localeID;
  info->ack = &done;
  info->fun = f;
  info->arg_size = arg_size;
  if (arg_size)
    bcopy(arg, &(info->arg), arg_size);

  done = 0;
  GASNET_Safe(gasnet_AMRequestMedium0(locale, FORK, info, info_size));
  GASNET_BLOCKUNTIL(1==done);
}

