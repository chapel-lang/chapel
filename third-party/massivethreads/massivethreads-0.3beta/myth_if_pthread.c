//myth_pthread_if.c : pthread-like interface

#include "myth_init.h"
#include "myth_sched.h"
#include "myth_worker.h"
#include "myth_io.h"
#include "myth_tls.h"

#include "myth_sched_proto.h"
#include "myth_io_proto.h"
#include "myth_tls_proto.h"

#include "myth_worker_func.h"
#include "myth_io_func.h"
#include "myth_sync_func.h"
#include "myth_sched_func.h"
#include "myth_tls_func.h"

#include "myth_if_pthread.h"

int sched_yield(void)
{

	real_sched_yield();
	myth_yield_body(1);
	return 0;
}

pthread_t pthread_self(void)
{
	return (pthread_t)myth_self_body();
}

int pthread_create(pthread_t *pth,const pthread_attr_t * attr,void *(*func)(void*),void *args)
{
	myth_thread_t mt;
	mt=myth_create_body(func,args,0);
	*pth=(pthread_t)mt;
	return 0;
}

int pthread_join(pthread_t th,void**ret)
{
	myth_join_body((myth_thread_t)th,ret);
	return 0;
}

void pthread_exit(void *ret)
{
	myth_exit_body(ret);
	//To avoid warning, this code is unreachable
	while (1);
}

int pthread_detach (pthread_t th)
{
	myth_detach_body((myth_thread_t)th);
	return 0;
}

int pthread_setcancelstate (int state, int *oldstate)
{
	return myth_setcancelstate_body(state,oldstate);
}

int pthread_setcanceltype (int type, int *oldtype)
{
	return myth_setcanceltype_body(type,oldtype);
}

int pthread_cancel (pthread_t th)
{
	return myth_cancel_body((myth_thread_t)th);
}

void pthread_testcancel(void)
{
	myth_testcancel_body();
}

int pthread_key_create (pthread_key_t *key,void (*destructor) (void *))
{
	return myth_key_create_body((myth_key_t*)key,destructor);
}

int pthread_key_delete (pthread_key_t key)
{
	return myth_key_delete_body((myth_key_t)key);
}

void *pthread_getspecific (pthread_key_t key)
{
	return myth_getspecific_body((myth_key_t)key);
}

int pthread_setspecific (pthread_key_t key,const void *ptr)
{
	return myth_setspecific_body((myth_key_t)key,(void*)ptr);
}

//TODO:these functions are too linux-dependent, need to make more portable

int pthread_barrier_init (pthread_barrier_t *barrier,const pthread_barrierattr_t *attr, unsigned int count)
{
	*((myth_barrier_t*)barrier)=myth_barrier_create_body(count);
	return 0;
}

/* Destroy a previously dynamically initialized barrier BARRIER.  */
int pthread_barrier_destroy (pthread_barrier_t *barrier)
{
	return myth_barrier_destroy_body(*((myth_barrier_t*)barrier));
}

/* Wait on barrier BARRIER.  */
int pthread_barrier_wait (pthread_barrier_t *barrier)
{
	return myth_barrier_wait_body(*((myth_barrier_t*)barrier));
}

static inline void handle_mutex_initializer(pthread_mutex_t *mtx)
{
#ifdef MYTH_SUPPORT_MUTEX_INITIALIZER
  //#ifdef MYTH_UNSAFE_MUTEX_INITIALIZER
	myth_mutex_t *m=(myth_mutex_t*)&(mtx->__size[0]);
#ifdef MYTH_UNSAFE_MUTEX_INITIALIZER
	if (*m)return;
#else
	static const pthread_mutex_t s_mtx_init=PTHREAD_MUTEX_INITIALIZER;
	if (mtx->data.lock!=s_mtx_init.data.lock)return;
	if (mtx->data.count!=s_mtx_init.data.count)return;
	if (mtx->data.owner!=s_mtx_init.data.owner)return;
	if (mtx->data.nusers!=s_mtx_init.data.nusers)return;
	//if (mtx->data.kind!=s_mtx_init.data.kind)return;
	if (mtx->data.spins!=s_mtx_init.data.spins)return;
	if (mtx->data.list.next!=s_mtx_init.data.list.next)return;
	if (mtx->data.list.prev!=s_mtx_init.data.list.prev)return;
#endif
	//real_pthread_mutex_init(mtx,NULL);
	*m=myth_mutex_create_body();
#endif
}

int pthread_mutex_init (pthread_mutex_t *mutex,
			       const pthread_mutexattr_t *attr)
{
	myth_mutex_t *mtx=(myth_mutex_t*)&(mutex->__size[0]);
	*mtx=myth_mutex_create_body();
	return 0;
}

int pthread_mutex_destroy (pthread_mutex_t *mutex)
{
	handle_mutex_initializer(mutex);
	myth_mutex_t *mtx=(myth_mutex_t*)&(mutex->__size[0]);
	myth_mutex_destroy_body(*mtx);
	return 0;
}

int pthread_mutex_trylock (pthread_mutex_t *mutex)
{
	handle_mutex_initializer(mutex);
	myth_mutex_t *mtx=(myth_mutex_t*)&(mutex->__size[0]);
	return myth_mutex_trylock_body(*mtx)?0:EBUSY;
}

int pthread_mutex_lock (pthread_mutex_t *mutex)
{
	handle_mutex_initializer(mutex);
	myth_mutex_t *mtx=(myth_mutex_t*)&(mutex->__size[0]);
	myth_mutex_lock_body(*mtx);
	return 0;
}

int pthread_mutex_unlock (pthread_mutex_t *mutex)
{
	handle_mutex_initializer(mutex);
	myth_mutex_t *mtx=(myth_mutex_t*)&(mutex->__size[0]);
	myth_mutex_unlock_body(*mtx);
	return 0;
}

//Previously needed for Chapel Tasking Layer. No longer needed
#if 0
int pthread_felock_init (pthread_mutex_t *mutex,
			       const pthread_mutexattr_t *mutexattr)
{
	myth_felock_t *mtx=(myth_felock_t*)&(mutex->__size[0]);
	*mtx=myth_felock_create_body();
	return 0;
}

int pthread_felock_destroy (pthread_mutex_t *mutex)
{
	myth_felock_t *mtx=(myth_felock_t*)&(mutex->__size[0]);
	myth_felock_destroy_body(*mtx);
	return 0;
}

int pthread_felock_lock (pthread_mutex_t *mutex)
{
	myth_felock_t *mtx=(myth_felock_t*)&(mutex->__size[0]);
	myth_felock_lock_body(*mtx);
	return 0;
}

int pthread_felock_wait_lock (pthread_mutex_t *mutex,int val)
{
	myth_felock_t *mtx=(myth_felock_t*)&(mutex->__size[0]);
	myth_felock_wait_lock_body(*mtx,val);
	return 0;
}

int pthread_felock_unlock (pthread_mutex_t *mutex)
{
	myth_felock_t *mtx=(myth_felock_t*)&(mutex->__size[0]);
	myth_felock_unlock_body(*mtx);
	return 0;
}

int pthread_felock_set_unlock (pthread_mutex_t *mutex,int val)
{
	myth_felock_t *mtx=(myth_felock_t*)&(mutex->__size[0]);
	myth_felock_set_unlock_body(*mtx,val);
	return 0;
}

int pthread_felock_status (pthread_mutex_t *mutex)
{
	myth_felock_t *mtx=(myth_felock_t*)&(mutex->__size[0]);
	return myth_felock_status_body(*mtx);
}
#endif

int pthread_cond_init (pthread_cond_t * c,
			      const pthread_condattr_t *cond_attr)
{
	myth_cond_t *cond=(myth_cond_t*)&(c->__size[0]);
	*cond=myth_cond_create_body();
	return 0;
}


int pthread_cond_destroy (pthread_cond_t *c)
{
	myth_cond_t *cond=(myth_cond_t*)&(c->__size[0]);
	myth_cond_destroy_body(*cond);
	return 0;
}

int pthread_cond_signal (pthread_cond_t *c)
{
	myth_cond_t *cond=(myth_cond_t*)&(c->__size[0]);
	myth_cond_signal_body(*cond);
	return 0;
}

int pthread_cond_broadcast (pthread_cond_t *c)
{
	myth_cond_t *cond=(myth_cond_t*)&(c->__size[0]);
	myth_cond_broadcast_body(*cond);
	return 0;
}

int pthread_cond_wait (pthread_cond_t * c,
			      pthread_mutex_t *mutex)
{
	myth_cond_t *cond=(myth_cond_t*)&(c->__size[0]);
	myth_mutex_t *mtx=(myth_mutex_t*)&(mutex->__size[0]);
	myth_cond_wait_body(*cond,*mtx);
	return 0;
}
