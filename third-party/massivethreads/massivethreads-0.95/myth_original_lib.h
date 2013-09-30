#ifndef MYTH_ORIGINAL_LIB_H_
#define MYTH_ORIGINAL_LIB_H_

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "myth_config.h"

//pthread functions
extern int (*real_pthread_key_create) (pthread_key_t *,void (*)(void *));
extern int (*real_pthread_key_delete) (pthread_key_t);
extern void *(*real_pthread_getspecific)(pthread_key_t);
extern int (*real_pthread_setspecific) (pthread_key_t,const void *);
extern int (*real_pthread_create) (pthread_t *,const pthread_attr_t *,void *(*)(void *),void *);
extern int (*real_pthread_join) (pthread_t, void **);
extern int (*real_pthread_kill)(pthread_t, int);
extern int (*real_pthread_sigmask)(int,const sigset_t *,sigset_t *);
extern int (*real_pthread_mutex_init) (pthread_mutex_t *,const pthread_mutexattr_t *);
extern int (*real_pthread_mutex_destroy) (pthread_mutex_t *);
extern int (*real_pthread_mutex_lock) (pthread_mutex_t *);
extern int (*real_pthread_mutex_trylock) (pthread_mutex_t *);
extern int (*real_pthread_mutex_unlock) (pthread_mutex_t *);
extern int (*real_pthread_barrier_init) (pthread_barrier_t *,const pthread_barrierattr_t *, unsigned int);
extern int (*real_pthread_barrier_destroy) (pthread_barrier_t *);
extern int (*real_pthread_barrier_wait) (pthread_barrier_t *);
extern int (*real_pthread_setaffinity_np) (pthread_t, size_t,cpu_set_t *);
extern pthread_t (*real_pthread_self) (void);
extern int (*real_pthread_spin_init) (pthread_spinlock_t *, int);
extern int (*real_pthread_spin_destroy) (pthread_spinlock_t *);
extern int (*real_pthread_spin_lock) (pthread_spinlock_t *);
extern int (*real_pthread_spin_trylock) (pthread_spinlock_t *);
extern int (*real_pthread_spin_unlock) (pthread_spinlock_t *);

extern int (*real_sched_yield)(void);

extern void *(*real_calloc)(size_t,size_t);
extern void *(*real_malloc)(size_t);
extern void (*real_free)(void *);
extern void *(*real_realloc)(void *,size_t);

#ifdef MYTH_WRAP_MALLOC_RUNTIME
extern int (*real_posix_memalign)(void **, size_t, size_t);
extern void *(*real_valloc)(size_t);
extern int g_wrap_malloc_completed;
extern int g_wrap_malloc;
#define ENV_MYTH_DONT_WRAP_MALLOC "MYTH_DONT_WRAP_MALLOC"
#endif


extern int (*real_socket)(int, int, int);
extern int (*real_connect)(int, const struct sockaddr *, socklen_t);
extern int (*real_accept)(int , struct sockaddr *,socklen_t *);
extern int (*real_bind)(int , const struct sockaddr *, socklen_t);
extern int (*real_listen)(int , int);
extern ssize_t (*real_send)(int , const void *, size_t , int );
extern ssize_t (*real_recv)(int , void *, size_t , int );
extern int (*real_close)(int );
extern int (*real_fcntl)(int , int , ...);
extern int (*real_select)(int, fd_set*, fd_set*,fd_set *, struct timeval *);
extern ssize_t (*real_sendto)(int, const void *, size_t, int,const struct sockaddr *, socklen_t);
extern ssize_t (*real_recvfrom)(int , void *, size_t , int ,struct sockaddr *, socklen_t *);

//Initialize/Finalize
void myth_get_original_funcs(void);
void myth_free_original_funcs(void);

#endif /* MYTH_SYNC_H_ */
