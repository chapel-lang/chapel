#include "chplrt.h"
#include "chplcomm.h"
#include "chplmem.h"
#include "chplthreads.h"

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
  /*
    pthread_mutex_lock(&sysinfo.local_thread_cnt_lock);
    sysinfo.local_thread_cnt--;
    pthread_cond_signal(&sysinfo.process_can_exit);
    pthread_mutex_unlock(&sysinfo.local_thread_cnt_lock);
  */
}


// AM non-blocking fork handler, medium sized, receiver side
void _AM_fork_nb(gasnet_token_t token, 
                 void   *buf,
                 size_t  nbytes) {
  pthread_t    t;
  dist_fork_t *fork_info;

  fork_info = (dist_fork_t*) _chpl_malloc(nbytes, sizeof(char), "", 0, 0);
  bcopy(buf, fork_info, nbytes);
  /*
    pthread_mutex_lock(&sysinfo.local_thread_cnt_lock);
    sysinfo.local_thread_cnt++;
    pthread_mutex_unlock(&sysinfo.local_thread_cnt_lock);
  */
  pthread_create(&t, NULL, (thread_p)_AM_fork_nb_wrapper, (void*)fork_info);
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
  /*
    pthread_mutex_lock(&sysinfo.local_thread_cnt_lock);
    sysinfo.local_thread_cnt--;
    pthread_cond_signal(&sysinfo.process_can_exit);
    pthread_mutex_unlock(&sysinfo.local_thread_cnt_lock);
  */
}


// AM sfork handler, medium sized, receiver side
void _AM_fork(gasnet_token_t  token, 
       void    *buf,
       size_t   nbytes) {
  pthread_t     t;
  dist_fork_t *fork_info;

  PRINTF("_AM_fork");

  fork_info = (dist_fork_t*) _chpl_malloc(nbytes, sizeof(char), "", 0, 0);
  bcopy(buf, fork_info, nbytes);
  /*
    pthread_mutex_lock(&sysinfo.local_thread_cnt_lock);
    sysinfo.local_thread_cnt++;
    pthread_mutex_unlock(&sysinfo.local_thread_cnt_lock);
  */
  pthread_create(&t, NULL, (thread_p)_AM_fork_wrapper, (void*)fork_info);
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

void _chpl_comm_init(int *argc_p, char ***argv_p) {
  gasnet_init(argc_p, argv_p);

  GASNET_Safe(gasnet_attach(ftable, 
                            sizeof(ftable)/sizeof(gasnet_handlerentry_t),
                            0,   // share everything
                            0));
  printf("starting locale %d of %d locale(s)\n", gasnet_mynode(), gasnet_nodes());
  fflush(stdout);
  _chpl_comm_barrier("_chpl_comm_init");
}

void _chpl_comm_barrier(char *msg) {
  PRINTF(msg);
  gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);
  GASNET_Safe(gasnet_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS));
  PRINTF(msg);
}

_int32 _chpl_comm_locale_id(void) {
  return gasnet_mynode();
}

_int32 _chpl_comm_num_locales(void) {
  return gasnet_nodes();
}

void _chpl_comm_done(void) {
  _chpl_comm_barrier("_chpl_comm_done");
  gasnet_exit(0);
}

void  _chpl_comm_write(_chpl_comm_ptr_t *p, void *addr) {
  PRINTF("_chpl_comm_write");
  if (_chpl_comm_locale_id() == p->locale) {
    bcopy(addr, p->addr, p->size);
  } else {
    gasnet_put(p->locale, p->addr, addr, p->size); // node, dest, src, size
  }
}

void  _chpl_comm_read(void *addr, _chpl_comm_ptr_t *p) {
  PRINTF("_chpl_comm_read");
  if (_chpl_comm_locale_id() == p->locale) {
    bcopy(p->addr, addr, p->size);
  } else {
    gasnet_get(addr, p->locale, p->addr, p->size); // dest, node, src, size
  }
}

void  _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size) {
  dist_fork_t *info;
  int           info_size;

  PRINTF("_chpl_comm_fork_nb");

  info_size = sizeof(dist_fork_t) - sizeof(void*) + arg_size;
  info = (dist_fork_t*) _chpl_malloc(info_size, sizeof(char), "", 0, 0);

  info->caller = _chpl_comm_locale_id();
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

  info->caller = _chpl_comm_locale_id();
  info->ack = &done;
  info->fun = f;
  info->arg_size = arg_size;
  if (arg_size)
    bcopy(arg, &(info->arg), arg_size);

  done = 0;
  GASNET_Safe(gasnet_AMRequestMedium0(locale, FORK, info, info_size));
  GASNET_BLOCKUNTIL(1==done);
}
