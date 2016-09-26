/* 
 * myth_real.h
 */
#pragma once
#ifndef MYTH_REAL_H_
#define MYTH_REAL_H_

#include <fcntl.h>
#include <malloc.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include "myth_config.h"

/* TODO: add condition HAVE_RESTRICT or something */
#define restrict



int real_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
			void *(*start_routine) (void *), void *arg);
void real_pthread_exit(void *retval) __attribute__((noreturn));
int real_pthread_join(pthread_t thread, void **retval);
#if _GNU_SOURCE
int real_pthread_tryjoin_np(pthread_t thread, void **retval);
int real_pthread_timedjoin_np(pthread_t thread, void **retval,
			      const struct timespec *abstime);
#endif
int real_pthread_detach(pthread_t thread);
pthread_t real_pthread_self(void);
int real_pthread_equal(pthread_t t1, pthread_t t2);
int real_pthread_attr_init(pthread_attr_t *attr);
int real_pthread_attr_destroy(pthread_attr_t *attr);
int real_pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);
int real_pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int real_pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize);
int real_pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
int real_pthread_attr_getschedparam(const pthread_attr_t *attr,
				    struct sched_param *param);
int real_pthread_attr_setschedparam(pthread_attr_t *attr,
				    const struct sched_param *param);
int real_pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
int real_pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int real_pthread_attr_getinheritsched(const pthread_attr_t *attr,
				      int *inheritsched);
int real_pthread_attr_setinheritsched(pthread_attr_t *attr,
				      int inheritsched);
int real_pthread_attr_getscope(const pthread_attr_t *attr, int *scope);
int real_pthread_attr_setscope(pthread_attr_t *attr, int scope);
#if 0
int real_pthread_attr_getstackaddr(const pthread_attr_t *attr, void **stackaddr);
int real_pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);
#endif
int real_pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);
int real_pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int real_pthread_attr_getstack(const pthread_attr_t *attr,
			       void **stackaddr, size_t *stacksize);
int real_pthread_attr_setstack(pthread_attr_t *attr,
			       void *stackaddr, size_t stacksize);
#if _GNU_SOURCE
int real_pthread_attr_setaffinity_np(pthread_attr_t *attr,
				     size_t cpusetsize, const cpu_set_t *cpuset);
int real_pthread_attr_getaffinity_np(const pthread_attr_t *attr,
				     size_t cpusetsize, cpu_set_t *cpuset);
int real_pthread_getattr_default_np(pthread_attr_t *attr);
int real_pthread_setattr_default_np(const pthread_attr_t *attr);
int real_pthread_getattr_np(pthread_t thread, pthread_attr_t *attr);
#endif

int real_pthread_setschedparam(pthread_t thread, int policy,
			       const struct sched_param *param);
int real_pthread_getschedparam(pthread_t thread, int *policy,
			       struct sched_param *param);
int real_pthread_setschedprio(pthread_t thread, int prio);
#if _GNU_SOURCE
int real_pthread_getname_np(pthread_t thread, char *name, size_t len);
int real_pthread_setname_np(pthread_t thread, const char *name);
#endif

int real_pthread_getconcurrency(void);
int real_pthread_setconcurrency(int new_level);
#if _GNU_SOURCE
int real_pthread_yield(void);
int real_pthread_setaffinity_np(pthread_t thread, size_t cpusetsize,
				const cpu_set_t *cpuset);
int real_pthread_getaffinity_np(pthread_t thread, size_t cpusetsize,
				cpu_set_t *cpuset);
#endif

int real_pthread_once(pthread_once_t *once_control,
			void init_routine(void));
int real_pthread_setcancelstate(int state, int *oldstate);
int real_pthread_setcanceltype(int type, int *oldtype);
int real_pthread_cancel(pthread_t thread);
void real_pthread_testcancel(void);
int real_pthread_mutex_init(pthread_mutex_t *restrict mutex,
			    const pthread_mutexattr_t *restrict attr);
int real_pthread_mutex_destroy(pthread_mutex_t *mutex);
int real_pthread_mutex_trylock(pthread_mutex_t *mutex);
int real_pthread_mutex_lock(pthread_mutex_t *mutex);
int real_pthread_mutex_timedlock(pthread_mutex_t *restrict mutex,
				 const struct timespec *restrict abstime);
int real_pthread_mutex_unlock(pthread_mutex_t *mutex);
int real_pthread_mutex_getprioceiling(const pthread_mutex_t *restrict mutex,
				      int *restrict prioceiling);
int real_pthread_mutex_setprioceiling(pthread_mutex_t *restrict mutex,
				      int prioceiling, int *restrict old_ceiling);
int real_pthread_mutex_consistent(pthread_mutex_t *mutex);
int real_pthread_mutexattr_init(pthread_mutexattr_t *attr);
int real_pthread_mutexattr_destroy(pthread_mutexattr_t *attr);
int real_pthread_mutexattr_getpshared(const pthread_mutexattr_t *restrict attr,
				      int *restrict pshared);
int real_pthread_mutexattr_setpshared(pthread_mutexattr_t *attr,
				      int pshared);
int real_pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr,
				   int *restrict type);
int real_pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type);
int real_pthread_mutexattr_getprotocol(const pthread_mutexattr_t *restrict attr,
				       int *restrict protocol);
int real_pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol);
int real_pthread_mutexattr_getprioceiling(const pthread_mutexattr_t *restrict attr,
					  int *restrict prioceiling);
int real_pthread_mutexattr_setprioceiling(pthread_mutexattr_t *attr,
					  int prioceiling);
int real_pthread_mutexattr_getrobust(const pthread_mutexattr_t *restrict attr,
				     int *restrict robust);
int real_pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust);
int real_pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,
			     const pthread_rwlockattr_t *restrict attr);
int real_pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
int real_pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int real_pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int real_pthread_rwlock_timedrdlock(pthread_rwlock_t *restrict rwlock,
				    const struct timespec *restrict abstime);
int real_pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
int real_pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
int real_pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict rwlock,
				    const struct timespec *restrict abstime);
int real_pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
int real_pthread_rwlockattr_init(pthread_rwlockattr_t *attr);
int real_pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);
int real_pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *restrict attr,
				       int *restrict pshared);
int real_pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr,
				       int pshared);
int real_pthread_rwlockattr_getkind_np(const pthread_rwlockattr_t *attr,
				       int *pref);
int real_pthread_rwlockattr_setkind_np(pthread_rwlockattr_t *attr,
				       int pref);
int real_pthread_cond_init(pthread_cond_t *restrict cond,
			   const pthread_condattr_t *restrict attr);
int real_pthread_cond_destroy(pthread_cond_t *cond);
int real_pthread_cond_signal(pthread_cond_t *cond);
int real_pthread_cond_broadcast(pthread_cond_t *cond);
int real_pthread_cond_wait(pthread_cond_t *restrict cond,
			   pthread_mutex_t *restrict mutex);
int real_pthread_cond_timedwait(pthread_cond_t *restrict cond,
			   pthread_mutex_t *restrict mutex,
				const struct timespec *restrict abstime);
int real_pthread_condattr_init(pthread_condattr_t *attr);
int real_pthread_condattr_destroy(pthread_condattr_t *attr);
int real_pthread_condattr_getpshared(const pthread_condattr_t *restrict attr,
				     int *restrict pshared);
int real_pthread_condattr_setpshared(pthread_condattr_t *attr,
				     int pshared);
int real_pthread_condattr_getclock(const pthread_condattr_t *restrict attr,
				   clockid_t *restrict clock_id);
int real_pthread_condattr_setclock(pthread_condattr_t *attr,
				   clockid_t clock_id);
int real_pthread_spin_init(pthread_spinlock_t *lock, int pshared);
int real_pthread_spin_destroy(pthread_spinlock_t *lock);
int real_pthread_spin_lock(pthread_spinlock_t *lock);
int real_pthread_spin_trylock(pthread_spinlock_t *lock);
int real_pthread_spin_unlock(pthread_spinlock_t *lock);
int real_pthread_barrier_init(pthread_barrier_t *restrict barrier,
			      const pthread_barrierattr_t *restrict attr,
			      unsigned count);
int real_pthread_barrier_destroy(pthread_barrier_t *barrier);
int real_pthread_barrier_wait(pthread_barrier_t *barrier);
int real_pthread_barrierattr_init(pthread_barrierattr_t *attr);
int real_pthread_barrierattr_destroy(pthread_barrierattr_t *attr);
int real_pthread_barrierattr_getpshared(const pthread_barrierattr_t *restrict attr,
					int *restrict pshared);
int real_pthread_barrierattr_setpshared(pthread_barrierattr_t *attr,
					int pshared);
int real_pthread_key_create(pthread_key_t *key, void destructor(void*));
int real_pthread_key_delete(pthread_key_t key);
void * real_pthread_getspecific(pthread_key_t key);
int real_pthread_setspecific(pthread_key_t key, const void *value);
int real_pthread_getcpuclockid(pthread_t thread, clockid_t *clock_id);
int real_pthread_atfork(void prepare(void), void parent(void), void child(void));
int real_pthread_kill(pthread_t thread, int sig);
void real_pthread_kill_other_threads_np(void);
int real_pthread_sigqueue(pthread_t thread, int sig,
			  const union sigval value);
int real_pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset);

int real_sched_yield(void);
unsigned int real_sleep(useconds_t seconds);
int real_usleep(useconds_t usec);
int real_nanosleep(const struct timespec *req, struct timespec *rem);

  /* alloc */
void * real_malloc(size_t size);
void real_free(void * ptr);
void * real_calloc(size_t nmemb, size_t size);
void * real_realloc(void *ptr, size_t size);
int real_posix_memalign(void **memptr, size_t alignment, size_t size);
void * real_aligned_alloc(size_t alignment, size_t size);
void * real_valloc(size_t size);
void * real_memalign(size_t alignment, size_t size);
void * real_pvalloc(size_t size);

  /* socket */
int real_socket(int domain, int type, int protocol);
int real_socketpair(int domain, int type, int protocol, int sv[2]);
int real_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
#if _GNU_SOURCE
int real_accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);
#endif
int real_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int real_close(int fd);
int real_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int real_fcntl(int fd, int cmd, ... /* arg */ );
int real_listen(int sockfd, int backlog);
ssize_t real_recv(int sockfd, void *buf, size_t len, int flags);
ssize_t real_recvfrom(int sockfd, void *buf, size_t len, int flags,
		      struct sockaddr *src_addr, socklen_t *addrlen);
ssize_t real_recvmsg(int sockfd, struct msghdr *msg, int flags);
ssize_t real_read(int fd, void *buf, size_t count);
int real_select(int nfds, fd_set *readfds, fd_set *writefds,
		fd_set *exceptfds, struct timeval *timeout);
ssize_t real_send(int sockfd, const void *buf, size_t len, int flags);
ssize_t real_sendto(int sockfd, const void *buf, size_t len, int flags,
		    const struct sockaddr *dest_addr, socklen_t addrlen);
ssize_t real_sendmsg(int sockfd, const struct msghdr *msg, int flags);
ssize_t real_write(int fd, const void *buf, size_t count);

#endif /* MYTH_REAL_H_ */


