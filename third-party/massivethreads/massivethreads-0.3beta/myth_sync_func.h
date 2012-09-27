#ifndef MYTH_SYNC_FUNC_H_
#define MYTH_SYNC_FUNC_H_

#include "myth_sync.h"

static inline myth_barrier_t myth_barrier_create_body(int nthreads)
{
	myth_running_env_t env;
	myth_barrier_t ret;
	env=myth_get_current_env();
	ret=myth_flmalloc(env->rank,sizeof(myth_barrier));
	ret->nthreads=nthreads;
	ret->rest=nthreads;
	myth_internal_lock_init(&ret->lock);
	if (nthreads>1){
		ret->th=myth_flmalloc(env->rank,sizeof(myth_thread_t)*(nthreads-1));
	}
	myth_wbarrier();
	return ret;
}

MYTH_CTX_CALLBACK void myth_barrier_wait_1(void *arg1,void *arg2,void *arg3)
{
	myth_running_env_t env;
	myth_thread_t next;
	myth_internal_lock_t *plock;
	env=(myth_running_env_t)arg1;
	next=(myth_thread_t)arg2;
	plock=(myth_internal_lock_t*)arg3;
	myth_assert(next);
	env->this_thread=next;
	myth_internal_lock_unlock(plock);
}

MYTH_CTX_CALLBACK void myth_barrier_wait_2(void *arg1,void *arg2,void *arg3)
{
	myth_running_env_t env;
	myth_internal_lock_t *plock;
	env=(myth_running_env_t)arg1;
	plock=(myth_internal_lock_t*)arg2;
	env->this_thread=NULL;
	myth_internal_lock_unlock(plock);
}

static inline int myth_barrier_wait_body(myth_barrier_t bar)
{
	if (bar->nthreads<2)return MYTH_BARRIER_SERIAL_THREAD;
	myth_barrier_t b;
	b=bar;
	int newval;
	myth_running_env_t env;
	myth_thread_t this_thread;
	env=myth_get_current_env();
	this_thread=env->this_thread;
	myth_internal_lock_lock(&bar->lock);
	newval=b->rest;
	newval--;
	b->rest=newval;
	if (newval!=0){
		//This thread is not last, will be blocked
		b->th[newval-1]=this_thread;
		//Try to get a runnable thread
		myth_thread_t next;
		next=myth_queue_pop(&env->runnable_q);
		if (next){
			next->env=env;
			env->this_thread=next;
			//Swap the contexts
			myth_swap_context_withcall(&this_thread->context,&next->context,myth_barrier_wait_1,(void*)env,(void*)next,(void*)&bar->lock);
		}
		else{
			//Switch to the scheduler and do work-stealing
			myth_swap_context_withcall(&this_thread->context,&env->sched.context,myth_barrier_wait_2,(void*)env,(void*)&bar->lock,NULL);
		}
		return 0;
	}
	else{
		//This thread is last, continue all other threads
		//Restore barrier status
		b->rest=b->nthreads;
		//Enqueue all the threads in the barrier
		int i;
		for (i=0;i<b->nthreads-1;i++){
			//set env
			b->th[i]->env=env;
			myth_queue_push(&env->runnable_q,b->th[i]);
		}
		myth_internal_lock_unlock(&bar->lock);
		return MYTH_BARRIER_SERIAL_THREAD;
	}
	myth_unreachable();
	return 0;
}

static inline int myth_barrier_destroy_body(myth_barrier_t bar)
{
	myth_running_env_t env;
	env=myth_get_current_env();
	myth_flfree(env->rank,sizeof(myth_thread_t)*((bar)->nthreads-1),(bar)->th);
	myth_flfree(env->rank,sizeof(myth_barrier),bar);
	return 0;
}

//Join counter implementation is incomplete
//Do not use them!
static inline myth_jc_t myth_jc_create_body(int val)
{
	//Create a join counter with initial value val
	//val must be positive
	assert(val>0);
	myth_jc_t ret;
	myth_running_env_t env;
	env=myth_get_current_env();
	ret=myth_flmalloc(env->rank,sizeof(myth_jc));
	ret->initial=val;
	ret->count=val;
	ret->th=NULL;
	myth_internal_lock_init(&ret->lock);
	return ret;
}

MYTH_CTX_CALLBACK void myth_jc_wait_1(void *arg1,void *arg2,void *arg3)
{
	myth_running_env_t env;
	myth_thread_t next;
	myth_internal_lock_t *plock;
	env=(myth_running_env_t)arg1;
	next=(myth_thread_t)arg2;
	plock=(myth_internal_lock_t*)arg3;
	myth_assert(next);
	env->this_thread=next;
	myth_internal_lock_unlock(plock);
}

MYTH_CTX_CALLBACK void myth_jc_wait_2(void *arg1,void *arg2,void *arg3)
{
	myth_running_env_t env;
	myth_internal_lock_t *plock;
	env=(myth_running_env_t)arg1;
	plock=(myth_internal_lock_t*)arg2;
	env->this_thread=NULL;
	myth_internal_lock_unlock(plock);
}

static inline void myth_jc_wait_body(myth_jc_t jc)
{
	if (jc->count<=0){
		//reset is join counter reaches 0
		myth_internal_lock_lock(&jc->lock);
		assert(jc->th==NULL);
		jc->count+=jc->initial;
		myth_internal_lock_unlock(&jc->lock);
		return;
	}
	else{
		assert(jc->th==NULL);
		//Wait
		myth_running_env_t env;
		env=myth_get_current_env();
		myth_thread_t this_thread;
		this_thread=env->this_thread;
		myth_internal_lock_lock(&jc->lock);
		jc->th=this_thread;
		//Deque a thread
		myth_thread_t next;
		next=myth_queue_pop(&env->runnable_q);
		if (next){
			next->env=env;
			env->this_thread=next;
			//Swap context
			myth_swap_context_withcall(&this_thread->context,&next->context,myth_jc_wait_1,(void*)env,(void*)next,(void*)&jc->lock);
		}
		else{
			//Switch to the scheduler
			myth_swap_context_withcall(&this_thread->context,&env->sched.context,myth_jc_wait_2,(void*)env,(void*)&jc->lock,NULL);
		}
		assert(jc->th==NULL);
		myth_internal_lock_lock(&jc->lock);
		jc->count+=jc->initial;
		myth_internal_lock_unlock(&jc->lock);
	}
}

static inline void myth_jc_dec_body(myth_jc_t jc)
{
	int dec_ret;
	myth_internal_lock_lock(&jc->lock);
	dec_ret=jc->count;
	if (1 || dec_ret>0){
		//decrement join counter
		dec_ret=--jc->count;
		//assert(dec_ret>=0);
		if (dec_ret<=0){
			//Now the join counter is 0
			//Enqueue waiters
			if (jc->th){
				myth_thread_t th=jc->th;
				jc->th=NULL;
				myth_running_env_t env=myth_get_current_env();
				th->env=env;
				myth_queue_push(&env->runnable_q,th);
			}
		}
		myth_internal_lock_unlock(&jc->lock);
	}
	else{
		//The join counter is stull full
		//TODO:Switch context
	}
}

//#define ALIGN_MUTEX

//mutex

static inline void myth_mutex_init(myth_mutex_t mtx)
{
#ifndef MYTH_MUTEX_BY_SPIN
	mtx->wsize=DEFAULT_FESIZE;
	mtx->nw=0;
	mtx->wl=mtx->def_wl;
	mtx->locked=0;
#endif
	myth_internal_lock_init(&mtx->lock);
}

static inline void myth_mutex_fini(myth_running_env_t env,myth_mutex_t mtx)
{
#ifndef MYTH_MUTEX_BY_SPIN
	if (mtx->wl!=mtx->def_wl){
		myth_flfree(env->rank,sizeof(myth_thread_t)*mtx->wsize,mtx->wl);
	}
#endif
	myth_internal_lock_destroy(&mtx->lock);
}

static inline myth_mutex_t myth_mutex_create_body()
{
	myth_mutex_t ret;
	myth_running_env_t env;
	env=myth_get_current_env();
	ret=myth_flmalloc(env->rank,sizeof(myth_mutex));
	myth_mutex_init(ret);
	return ret;
}

static inline int myth_mutex_destroy_body(myth_mutex_t mtx)
{
	myth_running_env_t env;
	env=myth_get_current_env();
	myth_mutex_fini(env,mtx);
	myth_flfree(env->rank,sizeof(myth_mutex),mtx);
	return 0;
}

#ifndef MYTH_MUTEX_BY_SPIN
MYTH_CTX_CALLBACK void myth_mutex_lock_1(void *arg1,void *arg2,void *arg3)
{
	myth_running_env_t env;
	myth_thread_t next;
	myth_internal_lock_t *lock;
	env=(myth_running_env_t)arg1;
	next=(myth_thread_t)arg2;
	lock=(myth_internal_lock_t*)arg3;
	env->this_thread=next;
	if (next){
		next->env=env;
	}
	myth_internal_lock_unlock(lock);
}

#endif

static inline int myth_mutex_lock_body(myth_mutex_t mtx)
{
#ifdef MYTH_MUTEX_BY_SPIN
	const int loop_count_start=100;
	int loop_count=loop_count_start;
	while (!myth_internal_lock_trylock(&mtx->lock)){
		loop_count--;
		if (!loop_count){
			loop_count=loop_count_start;
			//yield execution
			myth_yield_body(0);
		}
	}
	return 0;
#else
	int prev;
	myth_internal_lock_lock(&mtx->lock);
	prev=mtx->locked;
	mtx->locked=1;
	if (prev){
		//switch context
		myth_running_env_t env=myth_get_current_env();
		myth_thread_t this_thread=env->this_thread;
		//Re-allocate if waiting list is full
		if (mtx->wsize<=mtx->nw){
			if (mtx->wsize==DEFAULT_FESIZE){
				mtx->wl=myth_flmalloc(env->rank,sizeof(myth_thread_t)*mtx->wsize*2);
				memcpy(mtx->wl,mtx->def_wl,sizeof(myth_thread_t)*mtx->wsize);
			}
			else{
				mtx->wl=myth_flrealloc(env->rank,
						sizeof(myth_thread_t)*mtx->wsize,
						mtx->wl,
						sizeof(myth_thread_t)*mtx->wsize*2);
			}
			mtx->wsize*=2;
		}
		//add
		mtx->wl[mtx->nw]=this_thread;
		mtx->nw++;
		myth_thread_t next;
		myth_context_t next_ctx;
		next=myth_queue_pop(&env->runnable_q);
		if (next){
			next->env=env;
			next_ctx=&next->context;
		}
		else{
			next_ctx=&env->sched.context;
		}
		//Switch context
		myth_swap_context_withcall(&this_thread->context,next_ctx,myth_mutex_lock_1,(void*)env,(void*)next,(void*)&mtx->lock);
	}
	else{
		myth_internal_lock_unlock(&mtx->lock);
	}
#endif
	return 0;
}

static inline int myth_mutex_trylock_body(myth_mutex_t mtx)
{
#ifdef MYTH_MUTEX_BY_SPIN
	//Try to obtain a lock
	return myth_internal_lock_trylock(&mtx->lock);
#else
	int prev;
	myth_internal_lock_lock(&mtx->lock);
	prev=mtx->locked;
	mtx->locked=1;
	myth_internal_lock_unlock(&mtx->lock);
	return prev==0;
#endif
}

static inline int myth_mutex_unlock_body(myth_mutex_t mtx)
{
#ifdef MYTH_MUTEX_BY_SPIN
	myth_internal_lock_unlock(&mtx->lock);
	return 0;
#else
	myth_internal_lock_lock(&mtx->lock);
	if (mtx->nw){
		myth_thread_t next;
		myth_running_env_t env=myth_get_current_env();
		//resume one blocked thread
		mtx->nw--;
		next=mtx->wl[mtx->nw];
		next->env=env;
		myth_queue_push(&env->runnable_q,next);
	}
	else{
		mtx->locked=0;
	}
	myth_internal_lock_unlock(&mtx->lock);
	return 0;
#endif
}

static inline myth_felock_t myth_felock_create_body(void)
{
	myth_felock_t ret;
	myth_running_env_t env;
	env=myth_get_current_env();
	ret=myth_flmalloc(env->rank,sizeof(myth_felock));
	myth_mutex_init(&ret->lock);
	ret->fe=0;
	ret->fsize=ret->esize=DEFAULT_FESIZE;
	ret->ne=ret->nf=0;
	ret->el=ret->def_el;//myth_flmalloc(env->rank,sizeof(myth_thread_t)*ret->esize);
	ret->fl=ret->def_fl;//myth_flmalloc(env->rank,sizeof(myth_thread_t)*ret->fsize);
	return ret;
}

static inline int myth_felock_destroy_body(myth_felock_t fe)
{
	myth_running_env_t env;
	env=myth_get_current_env();
	myth_mutex_fini(env,&fe->lock);
	if (fe->el!=fe->def_el){
		assert(fe->esize>DEFAULT_FESIZE);
		myth_flfree(env->rank,sizeof(myth_thread_t)*fe->esize,fe->el);
	}
	if (fe->fl!=fe->def_fl){
		assert(fe->fsize>DEFAULT_FESIZE);
		myth_flfree(env->rank,sizeof(myth_thread_t)*fe->fsize,fe->fl);
	}
	myth_flfree(env->rank,sizeof(myth_felock),fe);
	return 0;
}

static inline int myth_felock_lock_body(myth_felock_t fe)
{
	return myth_mutex_lock_body(&fe->lock);
}

MYTH_CTX_CALLBACK void myth_felock_wait_lock_1(void *arg1,void *arg2,void *arg3)
{
	myth_running_env_t env;
	myth_thread_t next;
	myth_felock_t fe;
	env=(myth_running_env_t)arg1;
	next=(myth_thread_t)arg2;
	fe=(myth_felock_t)arg3;
	env->this_thread=next;
	if (next){
		next->env=env;
	}
	myth_mutex_unlock_body(&fe->lock);
}

static inline int myth_felock_wait_lock_body(myth_felock_t fe,int val)
{
	myth_running_env_t e=myth_get_current_env();
	myth_thread_t this_thread=e->this_thread;
	while (1){
		myth_mutex_lock_body(&fe->lock);
		volatile myth_running_env_t ev;
		ev=myth_get_current_env();
		myth_running_env_t env;
		env=ev;
		assert(this_thread==env->this_thread);
		if ((!!val)==(!!fe->fe)){
			return 0;
		}
		//add to queue
#if 1
		if (val){
			//wait for fe to be !0
			//check size and reallocate
			if (fe->fsize<=fe->nf){
				if (fe->fsize==DEFAULT_FESIZE){
					fe->fl=myth_flmalloc(env->rank,sizeof(myth_thread_t)*fe->fsize*2);
					memcpy(fe->fl,fe->def_fl,sizeof(myth_thread_t)*fe->fsize);
				}
				else{
					fe->fl=myth_flrealloc(env->rank,
							sizeof(myth_thread_t)*fe->fsize,
							fe->fl,
							sizeof(myth_thread_t)*fe->fsize*2);
				}
				fe->fsize*=2;
			}
			//add
			fe->fl[fe->nf]=this_thread;
			fe->nf++;
		}
		else{
			//wait for fe to be 0
			//check size and reallocate
			if (fe->esize<=fe->ne){
				if (fe->esize==DEFAULT_FESIZE){
					fe->el=myth_flmalloc(env->rank,sizeof(myth_thread_t)*fe->esize*2);
					memcpy(fe->el,fe->def_el,sizeof(myth_thread_t)*fe->esize);
				}
				else{
					fe->el=myth_flrealloc(env->rank,
							sizeof(myth_thread_t)*fe->esize,
							fe->el,
							sizeof(myth_thread_t)*fe->esize*2);
				}
				fe->esize*=2;
			}
			//add
			fe->el[fe->ne]=this_thread;
			fe->ne++;
		}
		myth_thread_t next;
		myth_context_t next_ctx;
		next=myth_queue_pop(&env->runnable_q);
		if (next){
			next->env=env;
			next_ctx=&next->context;
		}
		else{
			next_ctx=&env->sched.context;
		}
		myth_swap_context_withcall(&this_thread->context,next_ctx,myth_felock_wait_lock_1,(void*)env,(void*)next,(void*)fe);
#else
		myth_mutex_unlock_body(&fe->lock);
		myth_yield_body();
#endif
	}
}

static inline int myth_felock_unlock_body(myth_felock_t fe)
{
	myth_running_env_t env=myth_get_current_env();
#if 0
	int i;
	if (fe->fe){
		for (i=0;i<fe->nf;i++){
			fe->fl[i]->env=env;
			myth_queue_push(&env->runnable_q,fe->fl[i]);
		}
		fe->nf=0;
	}
	else{
		for (i=0;i<fe->ne;i++){
			fe->el[i]->env=env;
			myth_queue_push(&env->runnable_q,fe->el[i]);
		}
		fe->ne=0;
	}
#else
	myth_thread_t next;
	if (fe->fe){
		if (fe->nf){
			fe->nf--;
			next=fe->fl[fe->nf];
			next->env=env;
			myth_queue_push(&env->runnable_q,next);
		}
	}
	else{
		if (fe->ne){
			fe->ne--;
			next=fe->el[fe->ne];
			next->env=env;
			myth_queue_push(&env->runnable_q,next);
		}
	}
#endif
	return myth_mutex_unlock_body(&fe->lock);
}

static inline int myth_felock_set_unlock_body(myth_felock_t fe,int val)
{
	fe->fe=val;
	return myth_felock_unlock_body(fe);
}

static inline int myth_felock_status_body(myth_felock_t fe)
{
	return fe->fe;
}

//条件変数
static inline myth_cond_t myth_cond_create_body()
{
	myth_cond_t ret;
	myth_running_env_t env;
	env=myth_get_current_env();
	ret=myth_flmalloc(env->rank,sizeof(myth_cond));
	ret->wsize=DEFAULT_FESIZE;
	ret->nw=0;
	ret->wl=ret->def_wl;
	return ret;
}

static inline int myth_cond_destroy_body(myth_cond_t cond)
{
	myth_running_env_t env;
	env=myth_get_current_env();
	if (cond->wl!=cond->def_wl){
		myth_flfree(env->rank,sizeof(myth_thread_t)*cond->wsize,cond->wl);
	}
	myth_flfree(env->rank,sizeof(myth_cond),cond);
	return 0;
}

static inline int myth_cond_signal_body(myth_cond_t cond)
{
	myth_thread_t next;
	myth_running_env_t env;
	env=myth_get_current_env();
	if (cond->nw){
		cond->nw--;
		next=cond->wl[cond->nw];
		next->env=env;
		myth_queue_push(&env->runnable_q,next);
	}
	return 0;
}

static inline int myth_cond_broadcast_body(myth_cond_t cond)
{
	myth_running_env_t env=myth_get_current_env();
	int i;
	for (i=0;i<cond->nw;i++){
		cond->wl[i]->env=env;
		myth_queue_push(&env->runnable_q,cond->wl[i]);
	}
	cond->nw=0;
	return 0;
}

MYTH_CTX_CALLBACK void myth_cond_wait_1(void *arg1,void *arg2,void *arg3)
{
	myth_running_env_t env;
	myth_thread_t next;
	myth_mutex_t mtx;
	env=(myth_running_env_t)arg1;
	next=(myth_thread_t)arg2;
	mtx=(myth_mutex_t)arg3;
	env->this_thread=next;
	if (next){
		next->env=env;
	}
	myth_mutex_unlock_body(mtx);
}

static inline int myth_cond_wait_body(myth_cond_t cond,myth_mutex_t mtx)
{
	myth_running_env_t env=myth_get_current_env();
	myth_thread_t this_thread=env->this_thread;
	//Re-allocate if waiting list is full
	if (cond->wsize<=cond->nw){
		if (cond->wsize==DEFAULT_FESIZE){
			cond->wl=myth_flmalloc(env->rank,sizeof(myth_thread_t)*cond->wsize*2);
			memcpy(cond->wl,cond->def_wl,sizeof(myth_thread_t)*cond->wsize);
		}
		else{
			cond->wl=myth_flrealloc(env->rank,
					sizeof(myth_thread_t)*cond->wsize,
					cond->wl,
					sizeof(myth_thread_t)*cond->wsize*2);
		}
		cond->wsize*=2;
	}
	//add
	cond->wl[cond->nw]=this_thread;
	cond->nw++;
	myth_thread_t next;
	myth_context_t next_ctx;
	next=myth_queue_pop(&env->runnable_q);
	if (next){
		next->env=env;
		next_ctx=&next->context;
	}
	else{
		next_ctx=&env->sched.context;
	}
	//Switch context
	myth_swap_context_withcall(&this_thread->context,next_ctx,myth_cond_wait_1,(void*)env,(void*)next,(void*)mtx);
	//Lock again
	myth_mutex_lock_body(mtx);
	return 0;
}

#endif /* MYTH_SYNC_FUNC_H_ */
