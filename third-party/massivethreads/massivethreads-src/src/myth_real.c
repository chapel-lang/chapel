/* 
 * myth_real_fun.c
 */

#include <assert.h>
#include <stdarg.h>

#include "myth_config.h"

/* this is how function-wrapping works.

   let's say we want to wrap a function (e.g., pthread_create).
   it means when user program calls the function, it calls the version
   defined in massivethreads library. this is simply done by
   having a .c file defining these functions. for example,
   myth_pthread.c defines pthread_create that calls an 
   myth_create_body.
   a complication arises when massivethreads internally uses
   the original version of pthread_create, defined in libpthread.
   to ensure massivethreads calls the original version, 
   massivethreads never calls these function directly. instead,
   it calls real_pthread_create function (defined in this file)
   whenever it wants to call the original pthread_create.

   depending on compile time flags real_pthread_create behaves
   differently. when MYTH_WRAP_PTHREAD is zero, then it simply
   calls pthread_create.  this version does not expose the wrapped
   version of pthread_create, so thigs are straightforward.
   otherwise it first dynamically obtains the address of pthread_create
   defined in libpthread. 

   a difficulty here is how to know from which file massivethreads
   can find the right symbol.  in the previous implementation it
   used to use dlsym(RTLD_NEXT, "pthread_create"), which means
   the symbol is searched for from the "next" shared library in the
   link command line.  this works as long as -lpthread does not come
   before -lmyth in the link command line.  that is, it works if the link
   command line looks like gcc ... -lmyth -lpthread; if you, for 
   whatever reasons, do something like gcc ... -lpthread -lmyth, however, 
   dlsym fails to load pthread_create.
   
 */

#include "myth_config.h"
#include "myth_real.h"

#if MYTH_WRAP == MYTH_WRAP_DL
/* used for ensure_real_functions */
#include <stdio.h>
#include <stdlib.h>
#include <link.h>
#include <assert.h>
#include <string.h>
#include <link.h>

typedef struct {
  int (*pthread_create)(pthread_t *thread, const pthread_attr_t *attr,
			void *(*start_routine) (void *), void *arg);
  void (*pthread_exit)(void *retval) __attribute__((noreturn)) ;
  int (*pthread_join)(pthread_t thread, void **retval);
#if _GNU_SOURCE
  int (*pthread_tryjoin_np)(pthread_t thread, void **retval);
  int (*pthread_timedjoin_np)(pthread_t thread, void **retval,
			      const struct timespec *abstime);
#endif
  int (*pthread_detach)(pthread_t thread);
  pthread_t (*pthread_self)(void);
  int (*pthread_equal)(pthread_t t1, pthread_t t2);
  int (*pthread_attr_init)(pthread_attr_t *attr);
  int (*pthread_attr_destroy)(pthread_attr_t *attr);
  int (*pthread_attr_getdetachstate)(const pthread_attr_t *attr, int *detachstate);
  int (*pthread_attr_setdetachstate)(pthread_attr_t *attr, int detachstate);
  int (*pthread_attr_getguardsize)(const pthread_attr_t *attr, size_t *guardsize);
  int (*pthread_attr_setguardsize)(pthread_attr_t *attr, size_t guardsize);
  int (*pthread_attr_getschedparam)(const pthread_attr_t *attr,
				    struct sched_param *param);
  int (*pthread_attr_setschedparam)(pthread_attr_t *attr,
				    const struct sched_param *param);
  int (*pthread_attr_getschedpolicy)(const pthread_attr_t *attr, int *policy);
  int (*pthread_attr_setschedpolicy)(pthread_attr_t *attr, int policy);
  int (*pthread_attr_getinheritsched)(const pthread_attr_t *attr,
				      int *inheritsched);
  int (*pthread_attr_setinheritsched)(pthread_attr_t *attr,
				      int inheritsched);
  int (*pthread_attr_getscope)(const pthread_attr_t *attr, int *scope);
  int (*pthread_attr_setscope)(pthread_attr_t *attr, int scope);
#if 0				/* deprecated */
  int (*pthread_attr_getstackaddr)(const pthread_attr_t *attr, void **stackaddr);
  int (*pthread_attr_setstackaddr)(pthread_attr_t *attr, void *stackaddr);
#endif
  int (*pthread_attr_getstacksize)(const pthread_attr_t *attr, size_t *stacksize);
  int (*pthread_attr_setstacksize)(pthread_attr_t *attr, size_t stacksize);
  int (*pthread_attr_getstack)(const pthread_attr_t *attr,
			       void **stackaddr, size_t *stacksize);
  int (*pthread_attr_setstack)(pthread_attr_t *attr,
			       void *stackaddr, size_t stacksize);
#if _GNU_SOURCE
  int (*pthread_attr_setaffinity_np)(pthread_attr_t *attr,
				     size_t cpusetsize, const cpu_set_t *cpuset);
  int (*pthread_attr_getaffinity_np)(const pthread_attr_t *attr,
				     size_t cpusetsize, cpu_set_t *cpuset);
  int (*pthread_getattr_default_np)(pthread_attr_t *attr);
  int (*pthread_setattr_default_np)(const pthread_attr_t *attr);
  int (*pthread_getattr_np)(pthread_t thread, pthread_attr_t *attr);
#endif
  int (*pthread_setschedparam)(pthread_t thread, int policy,
			       const struct sched_param *param);
  int (*pthread_getschedparam)(pthread_t thread, int *policy,
			       struct sched_param *param);
  int (*pthread_setschedprio)(pthread_t thread, int prio);
#if _GNU_SOURCE
  int (*pthread_getname_np)(pthread_t thread, char *name, size_t len);
  int (*pthread_setname_np)(pthread_t thread, const char *name);
#endif
  int (*pthread_getconcurrency)(void);
  int (*pthread_setconcurrency)(int new_level);
#if _GNU_SOURCE
  int (*pthread_yield)(void);
  int (*pthread_setaffinity_np)(pthread_t thread, size_t cpusetsize,
				const cpu_set_t *cpuset);
  int (*pthread_getaffinity_np)(pthread_t thread, size_t cpusetsize,
				cpu_set_t *cpuset);
#endif
  int (*pthread_once)(pthread_once_t *once_control,
		      void (*init_routine)(void));
  int (*pthread_setcancelstate)(int state, int *oldstate);
  int (*pthread_setcanceltype)(int type, int *oldtype);
  int (*pthread_cancel)(pthread_t thread);
  void (*pthread_testcancel)(void);
  int (*pthread_mutex_init)(pthread_mutex_t *restrict mutex,
			    const pthread_mutexattr_t *restrict attr);
  int (*pthread_mutex_destroy)(pthread_mutex_t *mutex);
  int (*pthread_mutex_trylock)(pthread_mutex_t *mutex);
  int (*pthread_mutex_lock)(pthread_mutex_t *mutex);
  int (*pthread_mutex_timedlock)(pthread_mutex_t *restrict mutex,
				 const struct timespec *restrict abstime);
  int (*pthread_mutex_unlock)(pthread_mutex_t *mutex);
  int (*pthread_mutex_getprioceiling)(const pthread_mutex_t *restrict mutex,
				      int *restrict prioceiling);
  int (*pthread_mutex_setprioceiling)(pthread_mutex_t *restrict mutex,
				      int prioceiling, int *restrict old_ceiling);
  int (*pthread_mutex_consistent)(pthread_mutex_t *mutex);
  int (*pthread_mutexattr_init)(pthread_mutexattr_t *attr);
  int (*pthread_mutexattr_destroy)(pthread_mutexattr_t *attr);
  int (*pthread_mutexattr_getpshared)(const pthread_mutexattr_t *restrict attr,
				      int *restrict pshared);
  int (*pthread_mutexattr_setpshared)(pthread_mutexattr_t *attr,
				      int pshared);
  int (*pthread_mutexattr_gettype)(const pthread_mutexattr_t *restrict attr,
				   int *restrict type);
  int (*pthread_mutexattr_settype)(pthread_mutexattr_t *attr, int type);
  int (*pthread_mutexattr_getprotocol)(const pthread_mutexattr_t *restrict attr,
				       int *restrict protocol);
  int (*pthread_mutexattr_setprotocol)(pthread_mutexattr_t *attr, int protocol);
  int (*pthread_mutexattr_getprioceiling)(const pthread_mutexattr_t *restrict attr,
					  int *restrict prioceiling);
  int (*pthread_mutexattr_setprioceiling)(pthread_mutexattr_t *attr,
					  int prioceiling);
  int (*pthread_mutexattr_getrobust)(const pthread_mutexattr_t *restrict attr,
				     int *restrict robust);
  int (*pthread_mutexattr_setrobust)(pthread_mutexattr_t *attr, int robust);
  int (*pthread_rwlock_init)(pthread_rwlock_t *restrict rwlock,
			     const pthread_rwlockattr_t *restrict attr);
  int (*pthread_rwlock_destroy)(pthread_rwlock_t *rwlock);
  int (*pthread_rwlock_rdlock)(pthread_rwlock_t *rwlock);
  int (*pthread_rwlock_tryrdlock)(pthread_rwlock_t *rwlock);
  int (*pthread_rwlock_timedrdlock)(pthread_rwlock_t *restrict rwlock,
				    const struct timespec *restrict abstime);
  int (*pthread_rwlock_wrlock)(pthread_rwlock_t *rwlock);
  int (*pthread_rwlock_trywrlock)(pthread_rwlock_t *rwlock);
  int (*pthread_rwlock_timedwrlock)(pthread_rwlock_t *restrict rwlock,
				    const struct timespec *restrict abstime);
  int (*pthread_rwlock_unlock)(pthread_rwlock_t *rwlock);
  int (*pthread_rwlockattr_init)(pthread_rwlockattr_t *attr);
  int (*pthread_rwlockattr_destroy)(pthread_rwlockattr_t *attr);
  int (*pthread_rwlockattr_getpshared)(const pthread_rwlockattr_t *restrict attr,
				       int *restrict pshared);
  int (*pthread_rwlockattr_setpshared)(pthread_rwlockattr_t *attr,
				       int pshared);
  int (*pthread_rwlockattr_getkind_np)(const pthread_rwlockattr_t *attr,
				       int *pref);
  int (*pthread_rwlockattr_setkind_np)(pthread_rwlockattr_t *attr,
				       int pref);
  int (*pthread_cond_init)(pthread_cond_t *restrict cond,
			   const pthread_condattr_t *restrict attr);
  int (*pthread_cond_destroy)(pthread_cond_t *cond);
  int (*pthread_cond_signal)(pthread_cond_t *cond);
  int (*pthread_cond_broadcast)(pthread_cond_t *cond);
  int (*pthread_cond_wait)(pthread_cond_t *restrict cond,
			   pthread_mutex_t *restrict mutex);
  int (*pthread_cond_timedwait)(pthread_cond_t *restrict cond,
			   pthread_mutex_t *restrict mutex,
				const struct timespec *restrict abstime);
  int (*pthread_condattr_init)(pthread_condattr_t *attr);
  int (*pthread_condattr_destroy)(pthread_condattr_t *attr);
  int (*pthread_condattr_getpshared)(const pthread_condattr_t *restrict attr,
				     int *restrict pshared);
  int (*pthread_condattr_setpshared)(pthread_condattr_t *attr,
				     int pshared);
  int (*pthread_condattr_getclock)(const pthread_condattr_t *restrict attr,
				   clockid_t *restrict clock_id);
  int (*pthread_condattr_setclock)(pthread_condattr_t *attr,
				   clockid_t clock_id);
  int (*pthread_spin_init)(pthread_spinlock_t *lock, int pshared);
  int (*pthread_spin_destroy)(pthread_spinlock_t *lock);
  int (*pthread_spin_lock)(pthread_spinlock_t *lock);
  int (*pthread_spin_trylock)(pthread_spinlock_t *lock);
  int (*pthread_spin_unlock)(pthread_spinlock_t *lock);
  int (*pthread_barrier_init)(pthread_barrier_t *restrict barrier,
			      const pthread_barrierattr_t *restrict attr,
			      unsigned count);
  int (*pthread_barrier_destroy)(pthread_barrier_t *barrier);
  int (*pthread_barrier_wait)(pthread_barrier_t *barrier);
  int (*pthread_barrierattr_init)(pthread_barrierattr_t *attr);
  int (*pthread_barrierattr_destroy)(pthread_barrierattr_t *attr);
  int (*pthread_barrierattr_getpshared)(const pthread_barrierattr_t *restrict attr,
					int *restrict pshared);
  int (*pthread_barrierattr_setpshared)(pthread_barrierattr_t *attr,
					int pshared);
  int (*pthread_key_create)(pthread_key_t *key, void (*destructor)(void*));
  int (*pthread_key_delete)(pthread_key_t key);
  void * (*pthread_getspecific)(pthread_key_t key);
  int (*pthread_setspecific)(pthread_key_t key, const void *value);
  int (*pthread_getcpuclockid)(pthread_t thread, clockid_t *clock_id);
#if 0				/* could not find where it is */
  int (*pthread_atfork)(void (*prepare)(void), void (*parent)(void),
			void (*child)(void));
#endif
  int (*pthread_kill)(pthread_t thread, int sig);
#if 0				/* could not find where it is */
  void (*pthread_kill_other_threads_np)(void);
#endif
  int (*pthread_sigqueue)(pthread_t thread, int sig,
			  const union sigval value);
  int (*pthread_sigmask)(int how, const sigset_t *set, sigset_t *oldset);

  int (*sched_yield)(void);
  unsigned int (*sleep)(useconds_t seconds);
  int (*usleep)(useconds_t usec);
  int (*nanosleep)(const struct timespec *req, struct timespec *rem);

  /* alloc */
  void * (*malloc)(size_t size);
  void (*free)(void * ptr);
  void * (*calloc)(size_t nmemb, size_t size);
  void * (*realloc)(void *ptr, size_t size);
  int (*posix_memalign)(void **memptr, size_t alignment, size_t size);
  void * (*aligned_alloc)(size_t alignment, size_t size);
  void * (*valloc)(size_t size);
  void * (*memalign)(size_t alignment, size_t size);
  void * (*pvalloc)(size_t size);

  /* socket */
  int (*socket)(int domain, int type, int protocol);
  int (*socketpair)(int domain, int type, int protocol, int sv[2]);
  int (*accept)(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
#if _GNU_SOURCE
  int (*accept4)(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);
#endif
  int (*bind)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
  int (*close)(int fd);
  int (*connect)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
  int (*fcntl)(int fd, int cmd, ... /* arg */ );
  int (*listen)(int sockfd, int backlog);
  ssize_t (*recv)(int sockfd, void *buf, size_t len, int flags);
  ssize_t (*recvfrom)(int sockfd, void *buf, size_t len, int flags,
		      struct sockaddr *src_addr, socklen_t *addrlen);
  ssize_t (*recvmsg)(int sockfd, struct msghdr *msg, int flags);
  ssize_t (*read)(int fd, void *buf, size_t count);
  int (*select)(int nfds, fd_set *readfds, fd_set *writefds,
		fd_set *exceptfds, struct timeval *timeout);
  ssize_t (*send)(int sockfd, const void *buf, size_t len, int flags);
  ssize_t (*sendto)(int sockfd, const void *buf, size_t len, int flags,
		    const struct sockaddr *dest_addr, socklen_t addrlen);
  ssize_t (*sendmsg)(int sockfd, const struct msghdr *msg, int flags);
  ssize_t (*write)(int fd, const void *buf, size_t count);

} real_function_table_t;

enum {
  n_real_functions = sizeof(real_function_table_t) / sizeof(void *)
};

enum {
  real_function_table_init_state_uninit = 0,
  real_function_table_init_state_initializing = 1,
  real_function_table_init_state_initialized = 2
};

/* a data structure representing a symbol in shared objects */
enum { max_addr_and_files = 5 };
typedef struct {
  const char * name;	    /* symbol name */
  const char * file_pat; /* file pattern asked to search */
  void ** dest;	/* address to which the found addr
		   is asked to be written */
  int n;    /* the number of symbols found */
  struct {
    void * addr;      /* the address found */
    const char * filename; /* file the the symbol was found in */
  } addr_and_files[max_addr_and_files];
} shared_object_symbol;

static real_function_table_t real_function_table;

#define so_symbol_entry(fun, file)					\
  { .name = #fun, .file_pat = #file, .dest = (void **)&real_function_table.fun }

static shared_object_symbol s_so_syms[n_real_functions + 1] = {
  so_symbol_entry(pthread_create, libpthread),
  so_symbol_entry(pthread_exit, libpthread),
  so_symbol_entry(pthread_join, libpthread),
#if _GNU_SOURCE
  so_symbol_entry(pthread_tryjoin_np, libpthread),
  so_symbol_entry(pthread_timedjoin_np, libpthread),
#endif
  so_symbol_entry(pthread_detach, libpthread),
  so_symbol_entry(pthread_self, libpthread),
  so_symbol_entry(pthread_equal, libpthread),
  so_symbol_entry(pthread_attr_init, libpthread),
  so_symbol_entry(pthread_attr_destroy, libpthread),
  so_symbol_entry(pthread_attr_getdetachstate, libpthread),
  so_symbol_entry(pthread_attr_setdetachstate, libpthread),
  so_symbol_entry(pthread_attr_getguardsize, libpthread),
  so_symbol_entry(pthread_attr_setguardsize, libpthread),
  so_symbol_entry(pthread_attr_getschedparam, libpthread),
  so_symbol_entry(pthread_attr_setschedparam, libpthread),
  so_symbol_entry(pthread_attr_getschedpolicy, libpthread),
  so_symbol_entry(pthread_attr_setschedpolicy, libpthread),
  so_symbol_entry(pthread_attr_getinheritsched, libpthread),
  so_symbol_entry(pthread_attr_setinheritsched, libpthread),
  so_symbol_entry(pthread_attr_getscope, libpthread),
  so_symbol_entry(pthread_attr_setscope, libpthread),
#if 0				/* deprecated */
  so_symbol_entry(pthread_attr_getstackaddr, libpthread),
  so_symbol_entry(pthread_attr_setstackaddr, libpthread),
#endif
  so_symbol_entry(pthread_attr_getstacksize, libpthread),
  so_symbol_entry(pthread_attr_setstacksize, libpthread),
  so_symbol_entry(pthread_attr_getstack, libpthread),
  so_symbol_entry(pthread_attr_setstack, libpthread),
#if _GNU_SOURCE
  so_symbol_entry(pthread_attr_setaffinity_np, libpthread),
  so_symbol_entry(pthread_attr_getaffinity_np, libpthread),
  so_symbol_entry(pthread_getattr_default_np, libpthread),
  so_symbol_entry(pthread_setattr_default_np, libpthread),
  so_symbol_entry(pthread_getattr_np, libpthread),
#endif
  so_symbol_entry(pthread_setschedparam, libpthread),
  so_symbol_entry(pthread_getschedparam, libpthread),
  so_symbol_entry(pthread_setschedprio, libpthread),
#if _GNU_SOURCE
  so_symbol_entry(pthread_getname_np, libpthread),
  so_symbol_entry(pthread_setname_np, libpthread),
#endif
  so_symbol_entry(pthread_getconcurrency, libpthread),
  so_symbol_entry(pthread_setconcurrency, libpthread),
#if _GNU_SOURCE
  so_symbol_entry(pthread_yield, libpthread),
  so_symbol_entry(pthread_setaffinity_np, libpthread),
  so_symbol_entry(pthread_getaffinity_np, libpthread),
#endif
  so_symbol_entry(pthread_once, libpthread),
  so_symbol_entry(pthread_setcancelstate, libpthread),
  so_symbol_entry(pthread_setcanceltype, libpthread),
  so_symbol_entry(pthread_cancel, libpthread),
  so_symbol_entry(pthread_testcancel, libpthread),
  so_symbol_entry(pthread_mutex_init, libpthread),
  so_symbol_entry(pthread_mutex_destroy, libpthread),
  so_symbol_entry(pthread_mutex_trylock, libpthread),
  so_symbol_entry(pthread_mutex_lock, libpthread),
  so_symbol_entry(pthread_mutex_timedlock, libpthread),
  so_symbol_entry(pthread_mutex_unlock, libpthread),
  so_symbol_entry(pthread_mutex_getprioceiling, libpthread),
  so_symbol_entry(pthread_mutex_setprioceiling, libpthread),
  so_symbol_entry(pthread_mutex_consistent, libpthread),
  so_symbol_entry(pthread_mutexattr_init, libpthread),
  so_symbol_entry(pthread_mutexattr_destroy, libpthread),
  so_symbol_entry(pthread_mutexattr_getpshared, libpthread),
  so_symbol_entry(pthread_mutexattr_setpshared, libpthread),
  so_symbol_entry(pthread_mutexattr_gettype, libpthread),
  so_symbol_entry(pthread_mutexattr_settype, libpthread),
  so_symbol_entry(pthread_mutexattr_getprotocol, libpthread),
  so_symbol_entry(pthread_mutexattr_setprotocol, libpthread),
  so_symbol_entry(pthread_mutexattr_getprioceiling, libpthread),
  so_symbol_entry(pthread_mutexattr_setprioceiling, libpthread),
  so_symbol_entry(pthread_mutexattr_getrobust, libpthread),
  so_symbol_entry(pthread_mutexattr_setrobust, libpthread),
  so_symbol_entry(pthread_rwlock_init, libpthread),
  so_symbol_entry(pthread_rwlock_destroy, libpthread),
  so_symbol_entry(pthread_rwlock_rdlock, libpthread),
  so_symbol_entry(pthread_rwlock_tryrdlock, libpthread),
  so_symbol_entry(pthread_rwlock_timedrdlock, libpthread),
  so_symbol_entry(pthread_rwlock_wrlock, libpthread),
  so_symbol_entry(pthread_rwlock_trywrlock, libpthread),
  so_symbol_entry(pthread_rwlock_timedwrlock, libpthread),
  so_symbol_entry(pthread_rwlock_unlock, libpthread),
  so_symbol_entry(pthread_rwlockattr_init, libpthread),
  so_symbol_entry(pthread_rwlockattr_destroy, libpthread),
  so_symbol_entry(pthread_rwlockattr_getpshared, libpthread),
  so_symbol_entry(pthread_rwlockattr_setpshared, libpthread),
  so_symbol_entry(pthread_rwlockattr_getkind_np, libpthread),
  so_symbol_entry(pthread_rwlockattr_setkind_np, libpthread),
  so_symbol_entry(pthread_cond_init, libpthread),
  so_symbol_entry(pthread_cond_destroy, libpthread),
  so_symbol_entry(pthread_cond_signal, libpthread),
  so_symbol_entry(pthread_cond_broadcast, libpthread),
  so_symbol_entry(pthread_cond_wait, libpthread),
  so_symbol_entry(pthread_cond_timedwait, libpthread),
  so_symbol_entry(pthread_condattr_init, libpthread),
  so_symbol_entry(pthread_condattr_destroy, libpthread),
  so_symbol_entry(pthread_condattr_getpshared, libpthread),
  so_symbol_entry(pthread_condattr_setpshared, libpthread),
  so_symbol_entry(pthread_condattr_getclock, libpthread),
  so_symbol_entry(pthread_condattr_setclock, libpthread),
  so_symbol_entry(pthread_spin_init, libpthread),
  so_symbol_entry(pthread_spin_destroy, libpthread),
  so_symbol_entry(pthread_spin_lock, libpthread),
  so_symbol_entry(pthread_spin_trylock, libpthread),
  so_symbol_entry(pthread_spin_unlock, libpthread),
  so_symbol_entry(pthread_barrier_init, libpthread),
  so_symbol_entry(pthread_barrier_destroy, libpthread),
  so_symbol_entry(pthread_barrier_wait, libpthread),
  so_symbol_entry(pthread_barrierattr_init, libpthread),
  so_symbol_entry(pthread_barrierattr_destroy, libpthread),
  so_symbol_entry(pthread_barrierattr_getpshared, libpthread),
  so_symbol_entry(pthread_barrierattr_setpshared, libpthread),
  so_symbol_entry(pthread_key_create, libpthread),
  so_symbol_entry(pthread_key_delete, libpthread),
  so_symbol_entry(pthread_getspecific, libpthread),
  so_symbol_entry(pthread_setspecific, libpthread),
  so_symbol_entry(pthread_getcpuclockid, libpthread),
#if 0
  so_symbol_entry(pthread_atfork, libc),
#endif
  so_symbol_entry(pthread_kill, libpthread),
#if 0
  so_symbol_entry(pthread_kill_other_threads_np, libc),
#endif
  so_symbol_entry(pthread_sigqueue, libpthread),
  so_symbol_entry(pthread_sigmask, libpthread),
  so_symbol_entry(sched_yield, libpthread),
  so_symbol_entry(sleep, libpthread),
  so_symbol_entry(usleep, libpthread),
  so_symbol_entry(nanosleep, libpthread),

  /* alloc */
  so_symbol_entry(malloc, libc),
  so_symbol_entry(free, libc),
  so_symbol_entry(calloc, libc),
  so_symbol_entry(realloc, libc),
  so_symbol_entry(posix_memalign, libc),
  so_symbol_entry(aligned_alloc, libc),
  so_symbol_entry(valloc, libc),
  so_symbol_entry(memalign, libc),
  so_symbol_entry(pvalloc, libc),

  so_symbol_entry(socket, libc),
  so_symbol_entry(socketpair, libc),
  so_symbol_entry(accept, libc),
#if _GNU_SOURCE
  so_symbol_entry(accept4, libc),
#endif
  so_symbol_entry(bind, libc),
  so_symbol_entry(close, libc),
  so_symbol_entry(connect, libc),
  so_symbol_entry(fcntl, libc),
  so_symbol_entry(listen, libc),
  so_symbol_entry(recv, libc),
  so_symbol_entry(recvfrom, libc),
  so_symbol_entry(recvmsg, libc),
  so_symbol_entry(read, libc),
  so_symbol_entry(select, libc),
  so_symbol_entry(send, libc),
  so_symbol_entry(sendto, libc),
  so_symbol_entry(sendmsg, libc),
  so_symbol_entry(write, libc),
  
  { .name = 0 }
};

/* find SYMBOL from a file represented by HANDLE */
static void * find_symbol_in_handle(void * handle, const char * symbol) {
  char * e = dlerror();
  void * a = dlsym(handle, symbol);
  (void)e;
  return a;
}

/* info represents a shared object loaded in the current address
   space.  check if the shared object contains each symbol 
   in so_syms.  definitions found in the shared object 
   are accumulated in so_syms. */
static int find_symbols_in_phdr(struct dl_phdr_info *info,
				size_t size, void * data) {
  int i;
  const char * filename = info->dlpi_name;
  void * handle = 0;
  (void)size;
  (void)data;
  if (!filename || !filename[0]) return 0;
  /* check each symbol */
  for (i = 0; i < n_real_functions; i++) {
    shared_object_symbol * s = &s_so_syms[i];
    if (*s->dest) continue;
    /* when this assertion error happens, it means s_so_syms table
       does not have as many elements as the number of fields in 
       real_function_table. it is likely because s_so_syms miss
       entries for some fields in real_function_table. check if 
       s_so_syms table has lines for all fields of real_function_table */
    assert(s->name);
    
    /* check if FILENAME matches the pattern specified 
       (e.g., "libpthread")  */
    if (!strstr(filename, s->file_pat)) continue;
    if (!handle) {
      dlerror();
      handle = dlopen(filename, RTLD_LAZY | RTLD_NOLOAD);
      /* could not open the file; quit */
      if (!handle) return 0;
    }
    /* check if s->name is defined in the shared object */
    void * a = find_symbol_in_handle(handle, s->name);
    if (a) {
      /* yes, it is. put the result into the array (until
	 it overflows) */
      int n = s->n;
      if (n < max_addr_and_files) {
	s->addr_and_files[n].filename = filename;
	s->addr_and_files[n].addr = a;
	if (*s->dest == 0) {
	  /* put it in the real_function_table if it is not
	     already defined. it is defined either because
	     the origianl function was found in another
	     shared object or it is not wrapped (and set
	     statically in the above definition of 
	     real_function_table) */
	  *s->dest = a;
	}
      }
      s->n = n + 1;
    }
  }
  if (handle) dlclose(handle);
  return 0;
}

/* check if S was found in one and only one shared object. 
   if it is found in two or more shared objects, it prints
   a warning. if it is not found in any shared object,
   it prints a fatal error (and we will eventually abort) */
static int check_symbol_unique(shared_object_symbol * s) {
  if (s->n == 0) {
    fprintf(stderr,
	    "fatal: %s not found in any shared objects\n",
	    s->name);
  } else if (s->n > 1) {
    int i;
    int n = s->n < max_addr_and_files ? s->n : max_addr_and_files;
    fprintf(stderr,
	    "warning: %s found in multiple (%d) shared objects:",
	    s->name, s->n);
    for (i = 0; i < n; i++) {
      fprintf(stderr, " %s", s->addr_and_files[i].filename);
    }
    fprintf(stderr, " (the first one is returned)\n");
  }
  return s->n;
}

/* 
   find original definition of all wrapped functions (pthread_create,
   etc.) in loaded shared objects, and put them in real_function_table.

   it uses dl_iterate_phdr defined in glibc, which allows you
   to iterate over all loaded shared objects.
*/

static int real_function_table_init_state = real_function_table_init_state_uninit;

static int ensure_real_functions_(const char * caller_fun) {
  int i;
  int n_not_found = 0;
  int n_multiply_found = 0;
  if (real_function_table_init_state != real_function_table_init_state_uninit) {
    fprintf(stderr,
	    "error: function %s called while initializing real_function_table is in progress\n",
	    caller_fun);
    abort();
  }

  real_function_table_init_state = real_function_table_init_state_initializing;
  dl_iterate_phdr(find_symbols_in_phdr, (void *)s_so_syms);
  /* check uniqueness of each symbol */
  for (i = 0; i < n_real_functions; i++) {
    shared_object_symbol * s = &s_so_syms[i];
    int o = check_symbol_unique(s);
    if (o == 0) {
      n_not_found++;
    } else if (o > 1) {
      n_multiply_found++;
    }
  }
  /* if any function is not found, abort */
  if (n_not_found > 0) {
    fprintf(stderr,
	    "could not find original definition of some"
	    " wrapped functions, bail out\n");
    abort();
  }
  real_function_table_init_state = real_function_table_init_state_initialized;
  return 1;			/* OK */
}

#define ensure_real_functions() ensure_real_functions_(__func__)

#endif	/* MYTH_WRAP == MYTH_WRAP_DL */


/* original versions of wrapped functions */

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
			  void *(*start_routine) (void *), void *arg);
#endif
int real_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
			 void *(*start_routine) (void *), void *arg) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_create(thread, attr, start_routine, arg);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_create(thread, attr, start_routine, arg);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_create) ensure_real_functions();
  assert(real_function_table.pthread_create);
  return real_function_table.pthread_create(thread, attr, start_routine, arg);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
void __real_pthread_exit(void *retval) __attribute__((noreturn));
#endif
void real_pthread_exit(void *retval) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  pthread_exit(retval);
#elif MYTH_WRAP == MYTH_WRAP_LD
  __real_pthread_exit(retval);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_exit) ensure_real_functions();
  assert(real_function_table.pthread_exit);
  real_function_table.pthread_exit(retval);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_join(pthread_t thread, void **retval);
#endif
int real_pthread_join(pthread_t thread, void **retval) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_join(thread, retval);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_join(thread, retval);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_join) ensure_real_functions();
  assert(real_function_table.pthread_join);
  return real_function_table.pthread_join(thread, retval);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if _GNU_SOURCE
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_tryjoin_np(pthread_t thread, void **retval);
#endif
int real_pthread_tryjoin_np(pthread_t thread, void **retval) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_tryjoin_np(thread, retval);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_tryjoin_np(thread, retval);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_tryjoin_np) ensure_real_functions();
  assert(real_function_table.pthread_tryjoin_np);
  return real_function_table.pthread_tryjoin_np(thread, retval);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_timedjoin_np(pthread_t thread, void **retval,
			      const struct timespec *abstime);
#endif
int real_pthread_timedjoin_np(pthread_t thread, void **retval, const struct timespec *abstime) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_timedjoin_np(thread, retval, abstime);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_timedjoin_np(thread, retval, abstime);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_timedjoin_np) ensure_real_functions();
  assert(real_function_table.pthread_timedjoin_np);
  return real_function_table.pthread_timedjoin_np(thread, retval, abstime);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif	/* _GNU_SOURCE */

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_detach(pthread_t thread);
#endif
int real_pthread_detach(pthread_t thread) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_detach(thread);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_detach(thread);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_detach) ensure_real_functions();
  assert(real_function_table.pthread_detach);
  return real_function_table.pthread_detach(thread);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
pthread_t __real_pthread_self(void);
#endif
pthread_t real_pthread_self(void) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_self();
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_self();
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_self) {
    if (real_function_table_init_state
	== real_function_table_init_state_initializing) {
      return 0;
    }
    ensure_real_functions();
  }
  assert(real_function_table.pthread_self);
  return real_function_table.pthread_self();
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_equal(pthread_t t1, pthread_t t2);
#endif
int real_pthread_equal(pthread_t t1, pthread_t t2) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_equal(t1, t2);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_equal(t1, t2);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_equal) ensure_real_functions();
  assert(real_function_table.pthread_equal);
  return real_function_table.pthread_equal(t1, t2);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_init(pthread_attr_t *attr);
#endif
int real_pthread_attr_init(pthread_attr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_init(attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_init(attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_init) ensure_real_functions();
  assert(real_function_table.pthread_attr_init);
  return real_function_table.pthread_attr_init(attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_destroy(pthread_attr_t *attr);
#endif
int real_pthread_attr_destroy(pthread_attr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_destroy(attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_destroy(attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_destroy) ensure_real_functions();
  assert(real_function_table.pthread_attr_destroy);
  return real_function_table.pthread_attr_destroy(attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
#endif
int real_pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_getdetachstate(attr, detachstate);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_getdetachstate(attr, detachstate);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_getdetachstate) ensure_real_functions();
  assert(real_function_table.pthread_attr_getdetachstate);
  return real_function_table.pthread_attr_getdetachstate(attr, detachstate);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
#endif
int real_pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_setdetachstate(attr, detachstate);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_setdetachstate(attr, detachstate);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_setdetachstate) ensure_real_functions();
  assert(real_function_table.pthread_attr_setdetachstate);
  return real_function_table.pthread_attr_setdetachstate(attr, detachstate);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize);
#endif
int real_pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_getguardsize(attr, guardsize);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_getguardsize(attr, guardsize);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_getguardsize) ensure_real_functions();
  assert(real_function_table.pthread_attr_getguardsize);
  return real_function_table.pthread_attr_getguardsize(attr, guardsize);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
#endif
int real_pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_setguardsize(attr, guardsize);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_setguardsize(attr, guardsize);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_setguardsize) ensure_real_functions();
  assert(real_function_table.pthread_attr_setguardsize);
  return real_function_table.pthread_attr_setguardsize(attr, guardsize);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_getschedparam(const pthread_attr_t *attr,
				    struct sched_param *param);
#endif
int real_pthread_attr_getschedparam(const pthread_attr_t *attr,
				    struct sched_param *param) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_getschedparam(attr, param);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_getschedparam(attr, param);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_getschedparam) ensure_real_functions();
  assert(real_function_table.pthread_attr_getschedparam);
  return real_function_table.pthread_attr_getschedparam(attr, param);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_setschedparam(pthread_attr_t *attr,
				    const struct sched_param *param);
#endif
int real_pthread_attr_setschedparam(pthread_attr_t *attr,
				    const struct sched_param *param) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_setschedparam(attr, param);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_setschedparam(attr, param);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_setschedparam) ensure_real_functions();
  assert(real_function_table.pthread_attr_setschedparam);
  return real_function_table.pthread_attr_setschedparam(attr, param);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
#endif
int real_pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_getschedpolicy(attr, policy);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_getschedpolicy(attr, policy);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_getschedpolicy) ensure_real_functions();
  assert(real_function_table.pthread_attr_getschedpolicy);
  return real_function_table.pthread_attr_getschedpolicy(attr, policy);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
#endif
int real_pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_setschedpolicy(attr, policy);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_setschedpolicy(attr, policy);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_setschedpolicy) ensure_real_functions();
  assert(real_function_table.pthread_attr_setschedpolicy);
  return real_function_table.pthread_attr_setschedpolicy(attr, policy);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_getinheritsched(const pthread_attr_t *attr,
				      int *inheritsched);
#endif
int real_pthread_attr_getinheritsched(const pthread_attr_t *attr,
				      int *inheritsched) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_getinheritsched(attr, inheritsched);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_getinheritsched(attr, inheritsched);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_getinheritsched) ensure_real_functions();
  assert(real_function_table.pthread_attr_getinheritsched);
  return real_function_table.pthread_attr_getinheritsched(attr, inheritsched);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_setinheritsched(pthread_attr_t *attr,
				      int inheritsched);
#endif
int real_pthread_attr_setinheritsched(pthread_attr_t *attr,
				      int inheritsched) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_setinheritsched(attr, inheritsched);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_setinheritsched(attr, inheritsched);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_setinheritsched) ensure_real_functions();
  assert(real_function_table.pthread_attr_setinheritsched);
  return real_function_table.pthread_attr_setinheritsched(attr, inheritsched);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_getscope(const pthread_attr_t *attr, int *scope);
#endif
int real_pthread_attr_getscope(const pthread_attr_t *attr, int *scope) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_getscope(attr, scope);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_getscope(attr, scope);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_getscope) ensure_real_functions();
  assert(real_function_table.pthread_attr_getscope);
  return real_function_table.pthread_attr_getscope(attr, scope);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_setscope(pthread_attr_t *attr, int scope);
#endif
int real_pthread_attr_setscope(pthread_attr_t *attr, int scope) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_setscope(attr, scope);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_setscope(attr, scope);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_setscope) ensure_real_functions();
  assert(real_function_table.pthread_attr_setscope);
  return real_function_table.pthread_attr_setscope(attr, scope);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if 0				/* deprecated */
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr);
#endif
int real_pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_getstackaddr(attr, stackaddr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_getstackaddr(attr, stackaddr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_getstackaddr) ensure_real_functions();
  assert(real_function_table.pthread_attr_getstackaddr);
  return real_function_table.pthread_attr_getstackaddr(attr, stackaddr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);
#endif
int real_pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_setstackaddr(attr, stackaddr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_setstackaddr(attr, stackaddr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_setstackaddr) ensure_real_functions();
  assert(real_function_table.pthread_attr_setstackaddr);
  return real_function_table.pthread_attr_setstackaddr(attr, stackaddr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
#endif
int real_pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_getstacksize(attr, stacksize);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_getstacksize(attr, stacksize);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_getstacksize) ensure_real_functions();
  assert(real_function_table.pthread_attr_getstacksize);
  return real_function_table.pthread_attr_getstacksize(attr, stacksize);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
#endif
int real_pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_setstacksize(attr, stacksize);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_setstacksize(attr, stacksize);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_setstacksize) ensure_real_functions();
  assert(real_function_table.pthread_attr_setstacksize);
  return real_function_table.pthread_attr_setstacksize(attr, stacksize);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_getstack(const pthread_attr_t *attr,
			       void **stackaddr, size_t *stacksize);
#endif
int real_pthread_attr_getstack(const pthread_attr_t *attr,
			       void **stackaddr, size_t *stacksize) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_getstack(attr, stackaddr, stacksize);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_getstack(attr, stackaddr, stacksize);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_getstack) ensure_real_functions();
  assert(real_function_table.pthread_attr_getstack);
  return real_function_table.pthread_attr_getstack(attr, stackaddr, stacksize);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_setstack(pthread_attr_t *attr,
			       void *stackaddr, size_t stacksize);
#endif
int real_pthread_attr_setstack(pthread_attr_t *attr,
			       void *stackaddr, size_t stacksize) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_setstack(attr, stackaddr, stacksize);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_setstack(attr, stackaddr, stacksize);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_setstack) ensure_real_functions();
  assert(real_function_table.pthread_attr_setstack);
  return real_function_table.pthread_attr_setstack(attr, stackaddr, stacksize);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if _GNU_SOURCE
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_setaffinity_np(pthread_attr_t *attr,
				     size_t cpusetsize, const cpu_set_t *cpuset);
#endif
int real_pthread_attr_setaffinity_np(pthread_attr_t *attr,
				     size_t cpusetsize, const cpu_set_t *cpuset) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_setaffinity_np(attr, cpusetsize, cpuset);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_setaffinity_np(attr, cpusetsize, cpuset);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_setaffinity_np) ensure_real_functions();
  assert(real_function_table.pthread_attr_setaffinity_np);
  return real_function_table.pthread_attr_setaffinity_np(attr, cpusetsize, cpuset);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_attr_getaffinity_np(const pthread_attr_t *attr,
				     size_t cpusetsize, cpu_set_t *cpuset);
#endif
int real_pthread_attr_getaffinity_np(const pthread_attr_t *attr,
				     size_t cpusetsize, cpu_set_t *cpuset) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_attr_getaffinity_np(attr, cpusetsize, cpuset);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_attr_getaffinity_np(attr, cpusetsize, cpuset);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_attr_getaffinity_np) ensure_real_functions();
  assert(real_function_table.pthread_attr_getaffinity_np);
  return real_function_table.pthread_attr_getaffinity_np(attr, cpusetsize, cpuset);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if defined(HAVE_PTHREAD_GETATTR_DEFAULT_NP)
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_getattr_default_np(pthread_attr_t *attr);
#endif
int real_pthread_getattr_default_np(pthread_attr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_getattr_default_np(attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_getattr_default_np(attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_getattr_default_np) ensure_real_functions();
  assert(real_function_table.pthread_getattr_default_np);
  return real_function_table.pthread_getattr_default_np(attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif	/* HAVE_PTHREAD_GETATTR_DEFAULT_NP */

#if defined(HAVE_PTHREAD_SETATTR_DEFAULT_NP)
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_setattr_default_np(const pthread_attr_t *attr);
#endif
int real_pthread_setattr_default_np(const pthread_attr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_setattr_default_np(attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_setattr_default_np(attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_setattr_default_np) ensure_real_functions();
  assert(real_function_table.pthread_setattr_default_np);
  return real_function_table.pthread_setattr_default_np(attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif	/* HAVE_PTHREAD_SETATTR_DEFAULT_NP */

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_getattr_np(pthread_t thread, pthread_attr_t *attr);
#endif
int real_pthread_getattr_np(pthread_t thread, pthread_attr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_getattr_np(thread, attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_getattr_np(thread, attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_getattr_np) ensure_real_functions();
  assert(real_function_table.pthread_getattr_np);
  return real_function_table.pthread_getattr_np(thread, attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif	/* _GNU_SOURCE */


#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_setschedparam(pthread_t thread, int policy,
			       const struct sched_param *param);
#endif
int real_pthread_setschedparam(pthread_t thread, int policy,
			       const struct sched_param *param) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_setschedparam(thread, policy, param);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_setschedparam(thread, policy, param);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_setschedparam) ensure_real_functions();
  assert(real_function_table.pthread_setschedparam);
  return real_function_table.pthread_setschedparam(thread, policy, param);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_getschedparam(pthread_t thread, int *policy,
			       struct sched_param *param);
#endif
int real_pthread_getschedparam(pthread_t thread, int *policy,
			       struct sched_param *param) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_getschedparam(thread, policy, param);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_getschedparam(thread, policy, param);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_getschedparam) ensure_real_functions();
  assert(real_function_table.pthread_getschedparam);
  return real_function_table.pthread_getschedparam(thread, policy, param);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_setschedprio(pthread_t thread, int prio);
#endif
int real_pthread_setschedprio(pthread_t thread, int prio) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_setschedprio(thread, prio);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_setschedprio(thread, prio);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_setschedprio) ensure_real_functions();
  assert(real_function_table.pthread_setschedprio);
  return real_function_table.pthread_setschedprio(thread, prio);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if defined(HAVE_PTHREAD_GETNAME_NP)
#if _GNU_SOURCE
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_getname_np(pthread_t thread, char *name, size_t len);
#endif
int real_pthread_getname_np(pthread_t thread, char *name, size_t len) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_getname_np(thread, name, len);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_getname_np(thread, name, len);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_getname_np) ensure_real_functions();
  assert(real_function_table.pthread_getname_np);
  return real_function_table.pthread_getname_np(thread, name, len);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif  /* _GNU_SOURCE */
#endif	/* HAVE_PTHREAD_GETNAME_NP */

#if defined(HAVE_PTHREAD_SETNAME_NP)
#if _GNU_SOURCE
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_setname_np(pthread_t thread, const char *name);
#endif
int real_pthread_setname_np(pthread_t thread, const char *name) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_setname_np(thread, name);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_setname_np(thread, name);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_setname_np) ensure_real_functions();
  assert(real_function_table.pthread_setname_np);
  return real_function_table.pthread_setname_np(thread, name);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif	/* _GNU_SOURCE */
#endif /* HAVE_PTHREAD_SETNAME_NP */

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_getconcurrency(void);
#endif
int real_pthread_getconcurrency(void) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_getconcurrency();
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_getconcurrency();
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_getconcurrency) ensure_real_functions();
  assert(real_function_table.pthread_getconcurrency);
  return real_function_table.pthread_getconcurrency();
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_setconcurrency(int new_level);
#endif
int real_pthread_setconcurrency(int new_level) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_setconcurrency(new_level);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_setconcurrency(new_level);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_setconcurrency) ensure_real_functions();
  assert(real_function_table.pthread_setconcurrency);
  return real_function_table.pthread_setconcurrency(new_level);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if _GNU_SOURCE
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_yield(void);
#endif
int real_pthread_yield(void) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_yield();
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_yield();
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_yield) ensure_real_functions();
  assert(real_function_table.pthread_yield);
  return real_function_table.pthread_yield();
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_setaffinity_np(pthread_t thread, size_t cpusetsize,
				const cpu_set_t *cpuset);
#endif
int real_pthread_setaffinity_np(pthread_t thread, size_t cpusetsize,
				const cpu_set_t *cpuset) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_setaffinity_np(thread, cpusetsize, cpuset);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_setaffinity_np(thread, cpusetsize, cpuset);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_setaffinity_np) ensure_real_functions();
  assert(real_function_table.pthread_setaffinity_np);
  return real_function_table.pthread_setaffinity_np(thread, cpusetsize, cpuset);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_getaffinity_np(pthread_t thread, size_t cpusetsize,
				cpu_set_t *cpuset);
#endif
int real_pthread_getaffinity_np(pthread_t thread, size_t cpusetsize,
				cpu_set_t *cpuset) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_getaffinity_np(thread, cpusetsize, cpuset);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_getaffinity_np(thread, cpusetsize, cpuset);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_getaffinity_np) ensure_real_functions();
  assert(real_function_table.pthread_getaffinity_np);
  return real_function_table.pthread_getaffinity_np(thread, cpusetsize, cpuset);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif	/* _GNU_SOURCE */

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_once(pthread_once_t *once_control,
			void init_routine(void));
#endif
int real_pthread_once(pthread_once_t *once_control,
		      void (*init_routine)(void)) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_once(once_control, init_routine);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_once(once_control, init_routine);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_once) ensure_real_functions();
  assert(real_function_table.pthread_once);
  return real_function_table.pthread_once(once_control, init_routine);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_setcancelstate(int state, int *oldstate);
#endif
int real_pthread_setcancelstate(int state, int *oldstate) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_setcancelstate(state, oldstate);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_setcancelstate(state, oldstate);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_setcancelstate) ensure_real_functions();
  assert(real_function_table.pthread_setcancelstate);
  return real_function_table.pthread_setcancelstate(state, oldstate);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_setcanceltype(int type, int *oldtype);
#endif
int real_pthread_setcanceltype(int type, int *oldtype) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_setcanceltype(type, oldtype);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_setcanceltype(type, oldtype);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_setcanceltype) ensure_real_functions();
  assert(real_function_table.pthread_setcanceltype);
  return real_function_table.pthread_setcanceltype(type, oldtype);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_cancel(pthread_t thread);
#endif
int real_pthread_cancel(pthread_t thread) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_cancel(thread);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_cancel(thread);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_cancel) ensure_real_functions();
  assert(real_function_table.pthread_cancel);
  return real_function_table.pthread_cancel(thread);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
void __real_pthread_testcancel(void);
#endif
void real_pthread_testcancel(void) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_testcancel();
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_testcancel();
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_testcancel) ensure_real_functions();
  assert(real_function_table.pthread_testcancel);
  return real_function_table.pthread_testcancel();
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutex_init(pthread_mutex_t *restrict mutex,
			    const pthread_mutexattr_t *restrict attr);
#endif
int real_pthread_mutex_init(pthread_mutex_t *restrict mutex,
			    const pthread_mutexattr_t *restrict attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutex_init(mutex, attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutex_init(mutex, attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutex_init) {
    ensure_real_functions();
  }
  assert(real_function_table.pthread_mutex_init);
  return real_function_table.pthread_mutex_init(mutex, attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutex_destroy(pthread_mutex_t *mutex);
#endif
int real_pthread_mutex_destroy(pthread_mutex_t *mutex) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutex_destroy(mutex);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutex_destroy(mutex);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutex_destroy) ensure_real_functions();
  assert(real_function_table.pthread_mutex_destroy);
  return real_function_table.pthread_mutex_destroy(mutex);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutex_trylock(pthread_mutex_t *mutex);
#endif
int real_pthread_mutex_trylock(pthread_mutex_t *mutex) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutex_trylock(mutex);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutex_trylock(mutex);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutex_trylock) ensure_real_functions();
  assert(real_function_table.pthread_mutex_trylock);
  return real_function_table.pthread_mutex_trylock(mutex);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutex_lock(pthread_mutex_t *mutex);
#endif
int real_pthread_mutex_lock(pthread_mutex_t *mutex) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutex_lock(mutex);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutex_lock(mutex);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutex_lock) {
    if (real_function_table_init_state
	== real_function_table_init_state_initializing) {
      return 0;
    }
    ensure_real_functions();
  }
  assert(real_function_table.pthread_mutex_lock);
  return real_function_table.pthread_mutex_lock(mutex);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutex_timedlock(pthread_mutex_t *restrict mutex,
				 const struct timespec *restrict abstime);
#endif
int real_pthread_mutex_timedlock(pthread_mutex_t *restrict mutex,
			    const struct timespec *restrict abstime) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutex_timedlock(mutex, abstime);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutex_timedlock(mutex, abstime);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutex_timedlock) ensure_real_functions();
  assert(real_function_table.pthread_mutex_timedlock);
  return real_function_table.pthread_mutex_timedlock(mutex, abstime);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutex_unlock(pthread_mutex_t *mutex);
#endif
int real_pthread_mutex_unlock(pthread_mutex_t *mutex) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutex_unlock(mutex);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutex_unlock(mutex);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutex_unlock) {
    ensure_real_functions();
  }
  assert(real_function_table.pthread_mutex_unlock);
  return real_function_table.pthread_mutex_unlock(mutex);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutex_getprioceiling(const pthread_mutex_t *restrict mutex,
				      int *restrict prioceiling);
#endif
int real_pthread_mutex_getprioceiling(const pthread_mutex_t *restrict mutex,
				      int *restrict prioceiling) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutex_getprioceiling(mutex, prioceiling);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutex_getprioceiling(mutex, prioceiling);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutex_getprioceiling) ensure_real_functions();
  assert(real_function_table.pthread_mutex_getprioceiling);
  return real_function_table.pthread_mutex_getprioceiling(mutex, prioceiling);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutex_setprioceiling(pthread_mutex_t *restrict mutex,
				      int prioceiling, int *restrict old_ceiling);
#endif
int real_pthread_mutex_setprioceiling(pthread_mutex_t *restrict mutex,
				      int prioceiling, int *restrict old_ceiling) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutex_setprioceiling(mutex, prioceiling, old_ceiling);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutex_setprioceiling(mutex, prioceiling, old_ceiling);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutex_setprioceiling) ensure_real_functions();
  assert(real_function_table.pthread_mutex_setprioceiling);
  return real_function_table.pthread_mutex_setprioceiling(mutex, prioceiling, old_ceiling);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if defined(HAVE_PTHREAD_MUTEX_CONSISTENT)
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutex_consistent(pthread_mutex_t *mutex);
#endif
int real_pthread_mutex_consistent(pthread_mutex_t *mutex) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutex_consistent(mutex);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutex_consistent(mutex);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutex_consistent) ensure_real_functions();
  assert(real_function_table.pthread_mutex_consistent);
  return real_function_table.pthread_mutex_consistent(mutex);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif	/* HAVE_PTHREAD_MUTEX_CONSISTENT */

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutexattr_init(pthread_mutexattr_t *attr);
#endif
int real_pthread_mutexattr_init(pthread_mutexattr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutexattr_init(attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutexattr_init(attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutexattr_init) ensure_real_functions();
  assert(real_function_table.pthread_mutexattr_init);
  return real_function_table.pthread_mutexattr_init(attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
#endif
int real_pthread_mutexattr_destroy(pthread_mutexattr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutexattr_destroy(attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutexattr_destroy(attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutexattr_destroy) ensure_real_functions();
  assert(real_function_table.pthread_mutexattr_destroy);
  return real_function_table.pthread_mutexattr_destroy(attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict attr,
				      int *restrict pshared);
#endif
int real_pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict attr,
				      int *restrict pshared) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutexattr_getpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutexattr_getpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutexattr_getpshared) ensure_real_functions();
  assert(real_function_table.pthread_mutexattr_getpshared);
  return real_function_table.pthread_mutexattr_getpshared(attr, pshared);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutexattr_setpshared(pthread_mutexattr_t *attr,
				      int pshared);
#endif
int real_pthread_mutexattr_setpshared(pthread_mutexattr_t *attr,
				      int pshared) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutexattr_setpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutexattr_setpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutexattr_setpshared) ensure_real_functions();
  assert(real_function_table.pthread_mutexattr_setpshared);
  return real_function_table.pthread_mutexattr_setpshared(attr, pshared);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr,
				   int *restrict type);
#endif
int real_pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr,
				   int *restrict type) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutexattr_gettype(attr, type);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutexattr_gettype(attr, type);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutexattr_gettype) ensure_real_functions();
  assert(real_function_table.pthread_mutexattr_gettype);
  return real_function_table.pthread_mutexattr_gettype(attr, type);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
#endif
int real_pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutexattr_settype(attr, type);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutexattr_settype(attr, type);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutexattr_settype) ensure_real_functions();
  assert(real_function_table.pthread_mutexattr_settype);
  return real_function_table.pthread_mutexattr_settype(attr, type);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutexattr_getprotocol(const pthread_mutexattr_t *restrict attr,
				       int *restrict protocol);
#endif
int real_pthread_mutexattr_getprotocol(const pthread_mutexattr_t *restrict attr,
				       int *restrict protocol) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutexattr_getprotocol(attr, protocol);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutexattr_getprotocol(attr, protocol);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutexattr_getprotocol) ensure_real_functions();
  assert(real_function_table.pthread_mutexattr_getprotocol);
  return real_function_table.pthread_mutexattr_getprotocol(attr, protocol);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol);
#endif
int real_pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutexattr_setprotocol(attr, protocol);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutexattr_setprotocol(attr, protocol);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutexattr_setprotocol) ensure_real_functions();
  assert(real_function_table.pthread_mutexattr_setprotocol);
  return real_function_table.pthread_mutexattr_setprotocol(attr, protocol);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *restrict attr,
					  int *restrict prioceiling);
#endif
int real_pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *restrict attr,
					  int *restrict prioceiling) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutexattr_getprioceiling(attr, prioceiling);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutexattr_getprioceiling(attr, prioceiling);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutexattr_getprioceiling) ensure_real_functions();
  assert(real_function_table.pthread_mutexattr_getprioceiling);
  return real_function_table.pthread_mutexattr_getprioceiling(attr, prioceiling);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr,
					  int prioceiling);
#endif
int real_pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr,
					  int prioceiling) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutexattr_setprioceiling(attr, prioceiling);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutexattr_setprioceiling(attr, prioceiling);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutexattr_setprioceiling) ensure_real_functions();
  assert(real_function_table.pthread_mutexattr_setprioceiling);
  return real_function_table.pthread_mutexattr_setprioceiling(attr, prioceiling);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if defined(HAVE_PTHREAD_MUTEXATTR_GETROBUST)
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutexattr_getrobust(const pthread_mutexattr_t *restrict attr,
				     int *restrict robust);
#endif
int real_pthread_mutexattr_getrobust(const pthread_mutexattr_t *restrict attr,
				     int *restrict robust) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutexattr_getrobust(attr, robust);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutexattr_getrobust(attr, robust);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutexattr_getrobust) ensure_real_functions();
  assert(real_function_table.pthread_mutexattr_getrobust);
  return real_function_table.pthread_mutexattr_getrobust(attr, robust);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif /* HAVE_PTHREAD_MUTEXATTR_GETROBUST */

#if defined(HAVE_PTHREAD_MUTEXATTR_SETROBUST)
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust);
#endif
int real_pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_mutexattr_setrobust(attr, robust);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_mutexattr_setrobust(attr, robust);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_mutexattr_setrobust) ensure_real_functions();
  assert(real_function_table.pthread_mutexattr_setrobust);
  return real_function_table.pthread_mutexattr_setrobust(attr, robust);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif	/* HAVE_PTHREAD_MUTEXATTR_SETROBUST */

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
			     const pthread_rwlockattr_t *restrict attr);
#endif
int real_pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
			     const pthread_rwlockattr_t *restrict attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlock_init(rwlock, attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlock_init(rwlock, attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlock_init) ensure_real_functions();
  assert(real_function_table.pthread_rwlock_init);
  return real_function_table.pthread_rwlock_init(rwlock, attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
#endif
int real_pthread_rwlock_destroy(pthread_rwlock_t *rwlock) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlock_destroy(rwlock);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlock_destroy(rwlock);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlock_destroy) ensure_real_functions();
  assert(real_function_table.pthread_rwlock_destroy);
  return real_function_table.pthread_rwlock_destroy(rwlock);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
#endif
int real_pthread_rwlock_rdlock(pthread_rwlock_t *rwlock) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlock_rdlock(rwlock);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlock_rdlock(rwlock);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlock_rdlock) ensure_real_functions();
  assert(real_function_table.pthread_rwlock_rdlock);
  return real_function_table.pthread_rwlock_rdlock(rwlock);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
#endif
int real_pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlock_tryrdlock(rwlock);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlock_tryrdlock(rwlock);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlock_tryrdlock) ensure_real_functions();
  assert(real_function_table.pthread_rwlock_tryrdlock);
  return real_function_table.pthread_rwlock_tryrdlock(rwlock);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlock_timedrdlock(pthread_rwlock_t *restrict rwlock,
				    const struct timespec *restrict abstime);
#endif
int real_pthread_rwlock_timedrdlock(pthread_rwlock_t *restrict rwlock,
				    const struct timespec *restrict abstime) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlock_timedrdlock(rwlock, abstime);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlock_timedrdlock(rwlock, abstime);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlock_timedrdlock) ensure_real_functions();
  assert(real_function_table.pthread_rwlock_timedrdlock);
  return real_function_table.pthread_rwlock_timedrdlock(rwlock, abstime);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
#endif
int real_pthread_rwlock_wrlock(pthread_rwlock_t *rwlock) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlock_wrlock(rwlock);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlock_wrlock(rwlock);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlock_wrlock) ensure_real_functions();
  assert(real_function_table.pthread_rwlock_wrlock);
  return real_function_table.pthread_rwlock_wrlock(rwlock);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
#endif
int real_pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlock_trywrlock(rwlock);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlock_trywrlock(rwlock);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlock_trywrlock) ensure_real_functions();
  assert(real_function_table.pthread_rwlock_trywrlock);
  return real_function_table.pthread_rwlock_trywrlock(rwlock);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict rwlock,
				    const struct timespec *restrict abstime);
#endif
int real_pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict rwlock,
				    const struct timespec *restrict abstime) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlock_timedwrlock(rwlock, abstime);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlock_timedwrlock(rwlock, abstime);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlock_timedwrlock) ensure_real_functions();
  assert(real_function_table.pthread_rwlock_timedwrlock);
  return real_function_table.pthread_rwlock_timedwrlock(rwlock, abstime);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
#endif
int real_pthread_rwlock_unlock(pthread_rwlock_t *rwlock) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlock_unlock(rwlock);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlock_unlock(rwlock);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlock_unlock) ensure_real_functions();
  assert(real_function_table.pthread_rwlock_unlock);
  return real_function_table.pthread_rwlock_unlock(rwlock);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
#endif
int real_pthread_rwlockattr_init(pthread_rwlockattr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlockattr_init(attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlockattr_init(attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlockattr_init) ensure_real_functions();
  assert(real_function_table.pthread_rwlockattr_init);
  return real_function_table.pthread_rwlockattr_init(attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);
#endif
int real_pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlockattr_destroy(attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlockattr_destroy(attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlockattr_destroy) ensure_real_functions();
  assert(real_function_table.pthread_rwlockattr_destroy);
  return real_function_table.pthread_rwlockattr_destroy(attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *restrict attr,
				       int *restrict pshared);
#endif
int real_pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *restrict attr,
				       int *restrict pshared) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlockattr_getpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlockattr_getpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlockattr_getpshared) ensure_real_functions();
  assert(real_function_table.pthread_rwlockattr_getpshared);
  return real_function_table.pthread_rwlockattr_getpshared(attr, pshared);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr,
				       int pshared);
#endif
int real_pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr,
				       int pshared) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlockattr_setpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlockattr_setpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlockattr_setpshared) ensure_real_functions();
  assert(real_function_table.pthread_rwlockattr_setpshared);
  return real_function_table.pthread_rwlockattr_setpshared(attr, pshared);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlockattr_getkind_np(const pthread_rwlockattr_t *attr,
				       int *pref);
#endif
int real_pthread_rwlockattr_getkind_np(const pthread_rwlockattr_t *attr,
				       int *pref) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlockattr_getkind_np(attr, pref);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlockattr_getkind_np(attr, pref);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlockattr_getkind_np) ensure_real_functions();
  assert(real_function_table.pthread_rwlockattr_getkind_np);
  return real_function_table.pthread_rwlockattr_getkind_np(attr, pref);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_rwlockattr_setkind_np(pthread_rwlockattr_t *attr,
				       int pref);
#endif
int real_pthread_rwlockattr_setkind_np(pthread_rwlockattr_t *attr,
				       int pref) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_rwlockattr_setkind_np(attr, pref);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_rwlockattr_setkind_np(attr, pref);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_rwlockattr_setkind_np) ensure_real_functions();
  assert(real_function_table.pthread_rwlockattr_setkind_np);
  return real_function_table.pthread_rwlockattr_setkind_np(attr, pref);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_cond_init(pthread_cond_t *restrict cond,
			   const pthread_condattr_t *restrict attr);
#endif
int real_pthread_cond_init(pthread_cond_t *restrict cond,
			   const pthread_condattr_t *restrict attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_cond_init(cond, attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_cond_init(cond, attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_cond_init) ensure_real_functions();
  assert(real_function_table.pthread_cond_init);
  return real_function_table.pthread_cond_init(cond, attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_cond_destroy(pthread_cond_t *cond);
#endif
int real_pthread_cond_destroy(pthread_cond_t *cond) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_cond_destroy(cond);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_cond_destroy(cond);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_cond_destroy) ensure_real_functions();
  assert(real_function_table.pthread_cond_destroy);
  return real_function_table.pthread_cond_destroy(cond);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_cond_signal(pthread_cond_t *cond);
#endif
int real_pthread_cond_signal(pthread_cond_t *cond) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_cond_signal(cond);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_cond_signal(cond);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_cond_signal) ensure_real_functions();
  assert(real_function_table.pthread_cond_signal);
  return real_function_table.pthread_cond_signal(cond);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_cond_broadcast(pthread_cond_t *cond);
#endif
int real_pthread_cond_broadcast(pthread_cond_t *cond) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_cond_broadcast(cond);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_cond_broadcast(cond);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_cond_broadcast) ensure_real_functions();
  assert(real_function_table.pthread_cond_broadcast);
  return real_function_table.pthread_cond_broadcast(cond);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_cond_wait(pthread_cond_t *restrict cond,
			   pthread_mutex_t *restrict mutex);
#endif
int real_pthread_cond_wait(pthread_cond_t *restrict cond,
			   pthread_mutex_t *restrict mutex) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_cond_wait(cond, mutex);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_cond_wait(cond, mutex);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_cond_wait) ensure_real_functions();
  assert(real_function_table.pthread_cond_wait);
  return real_function_table.pthread_cond_wait(cond, mutex);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_cond_timedwait(pthread_cond_t *restrict cond,
			   pthread_mutex_t *restrict mutex,
				const struct timespec *restrict abstime);
#endif
int real_pthread_cond_timedwait(pthread_cond_t *restrict cond,
				pthread_mutex_t *restrict mutex,
				const struct timespec *restrict abstime) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_cond_timedwait(cond, mutex, abstime);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_cond_timedwait(cond, mutex, abstime);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_cond_timedwait) ensure_real_functions();
  assert(real_function_table.pthread_cond_timedwait);
  return real_function_table.pthread_cond_timedwait(cond, mutex, abstime);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_condattr_init(pthread_condattr_t *attr);
#endif
int real_pthread_condattr_init(pthread_condattr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_condattr_init(attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_condattr_init(attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_condattr_init) ensure_real_functions();
  assert(real_function_table.pthread_condattr_init);
  return real_function_table.pthread_condattr_init(attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_condattr_destroy(pthread_condattr_t *attr);
#endif
int real_pthread_condattr_destroy(pthread_condattr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_condattr_destroy(attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_condattr_destroy(attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_condattr_destroy) ensure_real_functions();
  assert(real_function_table.pthread_condattr_destroy);
  return real_function_table.pthread_condattr_destroy(attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_condattr_getpshared(const pthread_condattr_t *restrict attr,
				     int *restrict pshared);
#endif
int real_pthread_condattr_getpshared(const pthread_condattr_t *restrict attr,
				     int *restrict pshared) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_condattr_getpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_condattr_getpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_condattr_getpshared) ensure_real_functions();
  assert(real_function_table.pthread_condattr_getpshared);
  return real_function_table.pthread_condattr_getpshared(attr, pshared);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_condattr_setpshared(pthread_condattr_t *attr,
				     int pshared);
#endif
int real_pthread_condattr_setpshared(pthread_condattr_t *attr,
				     int pshared) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_condattr_setpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_condattr_setpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_condattr_setpshared) ensure_real_functions();
  assert(real_function_table.pthread_condattr_setpshared);
  return real_function_table.pthread_condattr_setpshared(attr, pshared);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_condattr_getclock(const pthread_condattr_t *restrict attr,
				   clockid_t *restrict clock_id);
#endif
int real_pthread_condattr_getclock(const pthread_condattr_t *restrict attr,
				   clockid_t *restrict clock_id) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_condattr_getclock(attr, clock_id);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_condattr_getclock(attr, clock_id);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_condattr_getclock) ensure_real_functions();
  assert(real_function_table.pthread_condattr_getclock);
  return real_function_table.pthread_condattr_getclock(attr, clock_id);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_condattr_setclock(pthread_condattr_t *attr,
				   clockid_t clock_id);
#endif
int real_pthread_condattr_setclock(pthread_condattr_t *attr,
				   clockid_t clock_id) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_condattr_setclock(attr, clock_id);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_condattr_setclock(attr, clock_id);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_condattr_setclock) ensure_real_functions();
  assert(real_function_table.pthread_condattr_setclock);
  return real_function_table.pthread_condattr_setclock(attr, clock_id);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_spin_init(pthread_spinlock_t *lock, int pshared);
#endif
int real_pthread_spin_init(pthread_spinlock_t *lock, int pshared) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_spin_init(lock, pshared);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_spin_init(lock, pshared);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_spin_init) ensure_real_functions();
  assert(real_function_table.pthread_spin_init);
  return real_function_table.pthread_spin_init(lock, pshared);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_spin_destroy(pthread_spinlock_t *lock);
#endif
int real_pthread_spin_destroy(pthread_spinlock_t *lock) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_spin_destroy(lock);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_spin_destroy(lock);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_spin_destroy) ensure_real_functions();
  assert(real_function_table.pthread_spin_destroy);
  return real_function_table.pthread_spin_destroy(lock);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_spin_lock(pthread_spinlock_t *lock);
#endif
int real_pthread_spin_lock(pthread_spinlock_t *lock) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_spin_lock(lock);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_spin_lock(lock);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_spin_lock) ensure_real_functions();
  assert(real_function_table.pthread_spin_lock);
  return real_function_table.pthread_spin_lock(lock);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_spin_trylock(pthread_spinlock_t *lock);
#endif
int real_pthread_spin_trylock(pthread_spinlock_t *lock) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_spin_trylock(lock);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_spin_trylock(lock);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_spin_trylock) ensure_real_functions();
  assert(real_function_table.pthread_spin_trylock);
  return real_function_table.pthread_spin_trylock(lock);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_spin_unlock(pthread_spinlock_t *lock);
#endif
int real_pthread_spin_unlock(pthread_spinlock_t *lock) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_spin_unlock(lock);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_spin_unlock(lock);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_spin_unlock) ensure_real_functions();
  assert(real_function_table.pthread_spin_unlock);
  return real_function_table.pthread_spin_unlock(lock);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_barrier_init(pthread_barrier_t *restrict barrier,
			      const pthread_barrierattr_t *restrict attr,
			      unsigned count);
#endif
int real_pthread_barrier_init(pthread_barrier_t *restrict barrier,
			      const pthread_barrierattr_t *restrict attr,
			      unsigned count) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_barrier_init(barrier, attr, count);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_barrier_init(barrier, attr, count);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_barrier_init) ensure_real_functions();
  assert(real_function_table.pthread_barrier_init);
  return real_function_table.pthread_barrier_init(barrier, attr, count);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_barrier_destroy(pthread_barrier_t *barrier);
#endif
int real_pthread_barrier_destroy(pthread_barrier_t *barrier) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_barrier_destroy(barrier);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_barrier_destroy(barrier);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_barrier_destroy) ensure_real_functions();
  assert(real_function_table.pthread_barrier_destroy);
  return real_function_table.pthread_barrier_destroy(barrier);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_barrier_wait(pthread_barrier_t *barrier);
#endif
int real_pthread_barrier_wait(pthread_barrier_t *barrier) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_barrier_wait(barrier);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_barrier_wait(barrier);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_barrier_wait) ensure_real_functions();
  assert(real_function_table.pthread_barrier_wait);
  return real_function_table.pthread_barrier_wait(barrier);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_barrierattr_init(pthread_barrierattr_t *attr);
#endif
int real_pthread_barrierattr_init(pthread_barrierattr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_barrierattr_init(attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_barrierattr_init(attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_barrierattr_init) ensure_real_functions();
  assert(real_function_table.pthread_barrierattr_init);
  return real_function_table.pthread_barrierattr_init(attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_barrierattr_destroy(pthread_barrierattr_t *attr);
#endif
int real_pthread_barrierattr_destroy(pthread_barrierattr_t *attr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_barrierattr_destroy(attr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_barrierattr_destroy(attr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_barrierattr_destroy) ensure_real_functions();
  assert(real_function_table.pthread_barrierattr_destroy);
  return real_function_table.pthread_barrierattr_destroy(attr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_barrierattr_getpshared(const pthread_barrierattr_t *restrict attr,
					int *restrict pshared);
#endif
int real_pthread_barrierattr_getpshared(const pthread_barrierattr_t *restrict attr,
					int *restrict pshared) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_barrierattr_getpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_barrierattr_getpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_barrierattr_getpshared) ensure_real_functions();
  assert(real_function_table.pthread_barrierattr_getpshared);
  return real_function_table.pthread_barrierattr_getpshared(attr, pshared);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_barrierattr_setpshared(pthread_barrierattr_t *attr,
					int pshared);
#endif
int real_pthread_barrierattr_setpshared(pthread_barrierattr_t *attr,
					int pshared) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_barrierattr_setpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_barrierattr_setpshared(attr, pshared);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_barrierattr_setpshared) ensure_real_functions();
  assert(real_function_table.pthread_barrierattr_setpshared);
  return real_function_table.pthread_barrierattr_setpshared(attr, pshared);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_key_create(pthread_key_t *key, void destructor(void*));
#endif
int real_pthread_key_create(pthread_key_t *key, void (*destructor)(void*)) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_key_create(key, destructor);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_key_create(key, destructor);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_key_create) ensure_real_functions();
  assert(real_function_table.pthread_key_create);
  return real_function_table.pthread_key_create(key, destructor);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_key_delete(pthread_key_t key);
#endif
int real_pthread_key_delete(pthread_key_t key) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_key_delete(key);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_key_delete(key);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_key_delete) ensure_real_functions();
  assert(real_function_table.pthread_key_delete);
  return real_function_table.pthread_key_delete(key);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
void * __real_pthread_getspecific(pthread_key_t key);
#endif
void * real_pthread_getspecific(pthread_key_t key) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_getspecific(key);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_getspecific(key);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_getspecific) ensure_real_functions();
  assert(real_function_table.pthread_getspecific);
  return real_function_table.pthread_getspecific(key);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_setspecific(pthread_key_t key, const void *value);
#endif
int real_pthread_setspecific(pthread_key_t key, const void *value) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_setspecific(key, value);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_setspecific(key, value);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_setspecific) ensure_real_functions();
  assert(real_function_table.pthread_setspecific);
  return real_function_table.pthread_setspecific(key, value);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_getcpuclockid(pthread_t thread, clockid_t *clock_id);
#endif
int real_pthread_getcpuclockid(pthread_t thread, clockid_t *clock_id) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_getcpuclockid(thread, clock_id);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_getcpuclockid(thread, clock_id);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_getcpuclockid) ensure_real_functions();
  assert(real_function_table.pthread_getcpuclockid);
  return real_function_table.pthread_getcpuclockid(thread, clock_id);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if 0
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_atfork(void prepare(void), void parent(void), void child(void));
#endif
int real_pthread_atfork(void (*prepare)(void), void (*parent)(void),
		   void (*child)(void)) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_atfork(prepare, parent, child);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_atfork(prepare, parent, child);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_atfork) ensure_real_functions();
  assert(real_function_table.pthread_atfork);
  return real_function_table.pthread_atfork(prepare, parent, child);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_kill(pthread_t thread, int sig);
#endif
int real_pthread_kill(pthread_t thread, int sig) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_kill(thread, sig);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_kill(thread, sig);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_kill) ensure_real_functions();
  assert(real_function_table.pthread_kill);
  return real_function_table.pthread_kill(thread, sig);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if 0
#if MYTH_WRAP == MYTH_WRAP_LD
void __real_pthread_kill_other_threads_np(void);
#endif
void real_pthread_kill_other_threads_np(void) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_kill_other_threads_np();
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_kill_other_threads_np();
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_kill_other_threads_np) ensure_real_functions();
  assert(real_function_table.pthread_kill_other_threads_np);
  return real_function_table.pthread_kill_other_threads_np();
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif

#if defined(HAVE_PTHREAD_SIGQUEUE)
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_sigqueue(pthread_t thread, int sig,
			  const union sigval value);
#endif
int real_pthread_sigqueue(pthread_t thread, int sig,
			  const union sigval value) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_sigqueue(thread, sig, value);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_sigqueue(thread, sig, value);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_sigqueue) ensure_real_functions();
  assert(real_function_table.pthread_sigqueue);
  return real_function_table.pthread_sigqueue(thread, sig, value);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif /* HAVE_PTHREAD_SIGQUEUE */

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);
#endif
int real_pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pthread_sigmask(how, set, oldset);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pthread_sigmask(how, set, oldset);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pthread_sigmask) ensure_real_functions();
  assert(real_function_table.pthread_sigmask);
  return real_function_table.pthread_sigmask(how, set, oldset);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_sched_yield(void);
#endif
int real_sched_yield(void) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return sched_yield();
#elif MYTH_WRAP == MYTH_WRAP_LD
  return sched_yield();
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.sched_yield) ensure_real_functions();
  assert(real_function_table.sched_yield);
  return real_function_table.sched_yield();
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
unsigned int __real_sleep(useconds_t seconds);
#endif
unsigned int real_sleep(useconds_t seconds) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return sleep(seconds);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return sleep(seconds);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.sleep) ensure_real_functions();
  assert(real_function_table.sleep);
  return real_function_table.sleep(seconds);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_usleep(useconds_t usec);
#endif
int real_usleep(useconds_t usec) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return usleep(usec);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return usleep(usec);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.usleep) ensure_real_functions();
  assert(real_function_table.usleep);
  return real_function_table.usleep(usec);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_nanosleep(const struct timespec *req, struct timespec *rem);
#endif
int real_nanosleep(const struct timespec *req, struct timespec *rem) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return nanosleep(req, rem);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return nanosleep(req, rem);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.nanosleep) ensure_real_functions();
  assert(real_function_table.nanosleep);
  return real_function_table.nanosleep(req, rem);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_DL
#define uncollectable_memory_sz 16384
enum { uncollectable_memory_min_log_alignment = 3 };
static char s_uncollectable_memory[uncollectable_memory_sz];
static size_t s_uncollectable_memory_sz[uncollectable_memory_sz >> uncollectable_memory_min_log_alignment];
static char * s_uncollectable_ptr = s_uncollectable_memory;

static inline size_t imax(size_t a, size_t b) {
  return (a < b ? a : b);
}

static inline size_t ilog(size_t x) {
  size_t l = 0;
  while (x > 1) {
    assert((x & 1) == 0);
    l = l + 1;
    x = x >> 1;
  }
  return l;
}

static void * aligned_alloc_uncollectable(size_t alignment, size_t size) {
  size_t log_alignment = imax(ilog(alignment), uncollectable_memory_min_log_alignment);
  intptr_t pi = (intptr_t)s_uncollectable_ptr;
  /* advance p to the next aligned address */
  intptr_t qi = (pi + (1 << log_alignment) - 1) >> log_alignment;
  char * q = (char *)qi;
  char * b = s_uncollectable_memory;
  assert(q + size <= b + uncollectable_memory_sz);
  intptr_t idx = (q  - b) >> uncollectable_memory_min_log_alignment;
  s_uncollectable_ptr = q + size;
  s_uncollectable_memory_sz[idx] = size;
  return q;
}

static size_t uncollectable_alloc_size(void * ptr) {
  char * b = s_uncollectable_memory;
  intptr_t idx = ((char *)ptr - b) >> uncollectable_memory_min_log_alignment;
  size_t size;
  assert(b <= (char *)ptr);
  assert((char *)ptr < b + uncollectable_memory_sz);
  size = s_uncollectable_memory_sz[idx];
  assert(size);
  return size;
}
#endif	/* MYTH_WRAP == MYTH_WRAP_DL */

/* alloc */
#if MYTH_WRAP == MYTH_WRAP_LD
void * __real_malloc(size_t size);
#endif
void * real_malloc(size_t size) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return malloc(size);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_malloc(size);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.malloc) {
    if (real_function_table_init_state == real_function_table_init_state_initializing) {
      void * p = aligned_alloc_uncollectable(1, size);
      return p;
    }
    ensure_real_functions();
  }
  {
    assert(real_function_table.malloc);
    void * a = real_function_table.malloc(size);
    return a;
  }
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
void __real_free(void * ptr);
#endif
void real_free(void * ptr) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  free(ptr);
#elif MYTH_WRAP == MYTH_WRAP_LD
  __real_free(ptr);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (s_uncollectable_memory <= (char *)ptr
      && (char *)ptr < s_uncollectable_memory + uncollectable_memory_sz) {
    return;
  }
  if (!real_function_table.free) ensure_real_functions();
  assert(real_function_table.free);
  real_function_table.free(ptr);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
void * __real_calloc(size_t nmemb, size_t size);
#endif
void * real_calloc(size_t nmemb, size_t size) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return calloc(nmemb, size);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_calloc(nmemb, size);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.calloc) {
    if (real_function_table_init_state
	== real_function_table_init_state_initializing) {
      void * p = aligned_alloc_uncollectable(1, nmemb * size);
      /* NOTE: no need to zero this area, as the returned address
	 is in a global array and the same address is never reused */
      return p;
    }
    ensure_real_functions();
  }
  assert(real_function_table.calloc);
  return real_function_table.calloc(nmemb, size);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
void * __real_realloc(void *ptr, size_t size);
#endif
void * real_realloc(void *ptr, size_t size) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return realloc(ptr, size);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_realloc(ptr, size);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.realloc) {
    if (real_function_table_init_state == real_function_table_init_state_initializing) {
      void * p = aligned_alloc_uncollectable(1, size);
      size_t old_size = uncollectable_alloc_size(ptr);
      memcpy(p, ptr, old_size);
      return p;
    }
    ensure_real_functions();
  }
  assert(real_function_table.realloc);
  return real_function_table.realloc(ptr, size);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_posix_memalign(void **memptr, size_t alignment, size_t size);
#endif
int real_posix_memalign(void **memptr, size_t alignment, size_t size) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return posix_memalign(memptr, alignment, size);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_posix_memalign(memptr, alignment, size);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.posix_memalign) ensure_real_functions();
  assert(real_function_table.posix_memalign);
  return real_function_table.posix_memalign(memptr, alignment, size);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if HAVE_ALIGNED_ALLOC
#if MYTH_WRAP == MYTH_WRAP_LD
void * __real_aligned_alloc(size_t alignment, size_t size);
#endif
void * real_aligned_alloc(size_t alignment, size_t size) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return aligned_alloc(alignment, size);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_aligned_alloc(alignment, size);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.aligned_alloc) ensure_real_functions();
  assert(real_function_table.aligned_alloc);
  return real_function_table.aligned_alloc(alignment, size);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif	/* HAVE_ALIGNED_ALLOC */

#if MYTH_WRAP == MYTH_WRAP_LD
void * __real_valloc(size_t size);
#endif
void * real_valloc(size_t size) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return valloc(size);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_valloc(size);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.valloc) ensure_real_functions();
  assert(real_function_table.valloc);
  return real_function_table.valloc(size);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
void * __real_memalign(size_t alignment, size_t size);
#endif
void * real_memalign(size_t alignment, size_t size) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return memalign(alignment, size);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_memalign(alignment, size);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.memalign) ensure_real_functions();
  assert(real_function_table.memalign);
  return real_function_table.memalign(alignment, size);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
void * __real_pvalloc(size_t size);
#endif
void * real_pvalloc(size_t size) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return pvalloc(size);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_pvalloc(size);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.pvalloc) ensure_real_functions();
  assert(real_function_table.pvalloc);
  return real_function_table.pvalloc(size);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

/* socket */
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_socket(int domain, int type, int protocol);
#endif
int real_socket(int domain, int type, int protocol) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return socket(domain, type, protocol);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_socket(domain, type, protocol);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.socket) ensure_real_functions();
  assert(real_function_table.socket);
  return real_function_table.socket(domain, type, protocol);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_socketpair(int domain, int type, int protocol, int sv[2]);
#endif
int real_socketpair(int domain, int type, int protocol, int sv[2]) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return socketpair(domain, type, protocol, sv);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_socketpair(domain, type, protocol, sv);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.socketpair) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.socketpair(domain, type, protocol, sv);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
#endif
int real_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return accept(sockfd, addr, addrlen);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_accept(sockfd, addr, addrlen);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.accept) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.accept(sockfd, addr, addrlen);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if defined(HAVE_ACCEPT4)
#if _GNU_SOURCE
#if MYTH_WRAP == MYTH_WRAP_LD
int __real_accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);
#endif
int real_accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return accept4(sockfd, addr, addrlen, flags);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_accept4(sockfd, addr, addrlen, flags);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.accept4) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.accept4(sockfd, addr, addrlen, flags);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
#endif	/* _GNU_SOURCE */
#endif	/* HAVE_ACCEPT4 */

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
#endif
int real_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return bind(sockfd, addr, addrlen);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_bind(sockfd, addr, addrlen);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.bind) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.bind(sockfd, addr, addrlen);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_close(int fd);
#endif
int real_close(int fd) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return close(fd);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_close(fd);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.close) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.close(fd);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
#endif
int real_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return connect(sockfd, addr, addrlen);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_connect(sockfd, addr, addrlen);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.connect) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.connect(sockfd, addr, addrlen);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_fcntl(int fd, int cmd, ... /* arg */ );
#endif
int real_fcntl(int fd, int cmd, ...) {
#if MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.fcntl) ensure_real_functions();
  assert(real_function_table.fcntl);
#endif
  switch (cmd) {
    /* cmd not taking any arg */
  case F_GETFD:
  case F_GETFL:
  case F_GETOWN:
  case F_GETSIG:
  case F_GETLEASE:
#if defined(F_GETPIPE_SZ)
  case F_GETPIPE_SZ:		/*  (void; since Linux 2.6.35) */
#endif
#if defined(F_ADD_SEALS)
  case F_GET_SEALS:		/*  (void; since Linux 3.17) */
#endif
#if MYTH_WRAP == MYTH_WRAP_VANILLA
    return fcntl(fd, cmd);
#elif MYTH_WRAP == MYTH_WRAP_LD
    return __real_fcntl(fd, cmd);
#elif MYTH_WRAP == MYTH_WRAP_DL
    return real_function_table.fcntl(fd, cmd);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
    /* cmd taking int */
  case F_DUPFD:
#if defined(F_DUPFD_CLOEXEC)
  case F_DUPFD_CLOEXEC:		/* (int; since Linux 2.6.24) */
#endif
  case F_SETFD:
  case F_SETFL:
  case F_SETOWN:
  case F_SETSIG:
  case F_SETLEASE:
  case F_NOTIFY:
#if defined(F_SETPIPE_SZ)
  case F_SETPIPE_SZ:		/*  (int; since Linux 2.6.35) */
#endif
#if defined(F_ADD_SEALS)
  case F_ADD_SEALS:		/*  (int; since Linux 3.17) */
#endif
    /* cmd taking an int */
    {
      va_list ap;
      va_start(ap, cmd);
      int arg = va_arg(ap, int);
      va_end(ap);
#if MYTH_WRAP == MYTH_WRAP_VANILLA
      return fcntl(fd, cmd, arg);
#elif MYTH_WRAP == MYTH_WRAP_LD
      return __real_fcntl(fd, cmd, arg);
#elif MYTH_WRAP == MYTH_WRAP_DL
      return real_function_table.fcntl(fd, cmd, arg);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
    }
  case F_SETLK:
  case F_SETLKW:
  case F_GETLK:
#if defined(F_OFD_SETLK)
  case F_OFD_SETLK:
#endif
#if defined(F_OFD_SETLKW)
  case F_OFD_SETLKW:
#endif
#if defined(F_OFD_GETLK)
  case F_OFD_GETLK:
#endif
#if defined(F_GETOWN_EX)	 /* (since Linux 2.6.32) */
  case F_GETOWN_EX:	 /* (since Linux 2.6.32) */
#endif
#if defined(F_SETOWN_EX)
  case F_SETOWN_EX: /* (since Linux 2.6.32)  */
#endif
    /* cmd taking a pointer */
    {
      va_list ap;
      va_start(ap, cmd);
      void * arg = va_arg(ap, void*);
      va_end(ap);
#if MYTH_WRAP == MYTH_WRAP_VANILLA
      return fcntl(fd, cmd, arg);
#elif MYTH_WRAP == MYTH_WRAP_LD
      return __real_fcntl(fd, cmd, arg);
#elif MYTH_WRAP == MYTH_WRAP_DL
      return real_function_table.fcntl(fd, cmd, arg);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
    }
  default:
    /* punt; assume int-taking */
    /* cmd taking a pointer */
    {
      va_list ap;
      va_start(ap, cmd);
      void * arg = va_arg(ap, void*);
      va_end(ap);
#if MYTH_WRAP == MYTH_WRAP_VANILLA
      return fcntl(fd, cmd, arg);
#elif MYTH_WRAP == MYTH_WRAP_LD
      return __real_fcntl(fd, cmd, arg);
#elif MYTH_WRAP == MYTH_WRAP_DL
      return real_function_table.fcntl(fd, cmd, arg);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
    }
  }
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_listen(int sockfd, int backlog);
#endif
int real_listen(int sockfd, int backlog) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return listen(sockfd, backlog);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_listen(sockfd, backlog);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.listen) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.listen(sockfd, backlog);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
ssize_t __real_recv(int sockfd, void *buf, size_t len, int flags);
#endif
ssize_t real_recv(int sockfd, void *buf, size_t len, int flags) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return recv(sockfd, buf, len, flags);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_recv(sockfd, buf, len, flags);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.recv) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.recv(sockfd, buf, len, flags);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
ssize_t __real_recvfrom(int sockfd, void *buf, size_t len, int flags,
		      struct sockaddr *src_addr, socklen_t *addrlen);
#endif
ssize_t real_recvfrom(int sockfd, void *buf, size_t len, int flags,
		      struct sockaddr *src_addr, socklen_t *addrlen) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.recvfrom) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}
  
#if MYTH_WRAP == MYTH_WRAP_LD
ssize_t __real_recvmsg(int sockfd, struct msghdr *msg, int flags);
#endif
ssize_t real_recvmsg(int sockfd, struct msghdr *msg, int flags) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return recvmsg(sockfd, msg, flags);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_recvmsg(sockfd, msg, flags);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.recvmsg) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.recvmsg(sockfd, msg, flags);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
ssize_t __real_read(int fd, void *buf, size_t count);
#endif
ssize_t real_read(int fd, void *buf, size_t count) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return read(fd, buf, count);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_read(fd, buf, count);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.read) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.read(fd, buf, count);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
int __real_select(int nfds, fd_set *readfds, fd_set *writefds,
		fd_set *exceptfds, struct timeval *timeout);
#endif
int real_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return select(nfds, readfds, writefds, exceptfds, timeout);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_select(nfds, readfds, writefds, exceptfds, timeout);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.select) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.select(nfds, readfds, writefds, exceptfds, timeout);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
ssize_t __real_send(int sockfd, const void *buf, size_t len, int flags);
#endif
ssize_t real_send(int sockfd, const void *buf, size_t len, int flags) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return send(sockfd, buf, len, flags);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_send(sockfd, buf, len, flags);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.send) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.send(sockfd, buf, len, flags);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
ssize_t __real_sendto(int sockfd, const void *buf, size_t len, int flags,
		    const struct sockaddr *dest_addr, socklen_t addrlen);
#endif
ssize_t real_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return sendto(sockfd, buf, len, flags, dest_addr, addrlen);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.sendto) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.sendto(sockfd, buf, len, flags, dest_addr, addrlen);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
ssize_t __real_sendmsg(int sockfd, const struct msghdr *msg, int flags);
#endif
ssize_t real_sendmsg(int sockfd, const struct msghdr *msg, int flags) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return sendmsg(sockfd, msg, flags);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_sendmsg(sockfd, msg, flags);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.sendmsg) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.sendmsg(sockfd, msg, flags);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

#if MYTH_WRAP == MYTH_WRAP_LD
ssize_t __real_write(int fd, const void *buf, size_t count);
#endif
ssize_t real_write(int fd, const void *buf, size_t count) {
#if MYTH_WRAP == MYTH_WRAP_VANILLA
  return write(fd, buf, count);
#elif MYTH_WRAP == MYTH_WRAP_LD
  return __real_write(fd, buf, count);
#elif MYTH_WRAP == MYTH_WRAP_DL
  if (!real_function_table.write) ensure_real_functions();
  assert(real_function_table.socketpair);
  return real_function_table.write(fd, buf, count);
#else
#error "MYTH_WRAP must be MYTH_WRAP_VANILLA, MYTH_WRAP_LD, or MYTH_WRAP_DL"
#endif
}

