/*
 * myth_if_native.h
 *
 *  Created on: 2011/10/20
 *      Author: jnakashima
 */

#ifndef MYTH_IF_NATIVE_H_
#define MYTH_IF_NATIVE_H_

#ifndef MYTH_THREAD_OPTION_DEFINED
#define MYTH_THREAD_OPTION_DEFINED
typedef struct myth_thread_option{
	size_t stack_size;
	int switch_immediately;
	size_t custom_data_size;
	void *custom_data;
}myth_thread_option,*myth_thread_option_t;
#endif

void myth_init(void);
void myth_init_withparam(int worker_num,size_t def_stack_size);
void myth_fini(void);

int myth_init_ex(int worker_num,size_t def_stack_size);
void myth_fini_ex(void);
void myth_exit_workers_ex(void);
void myth_ext_exit_workers_ex(void);
void myth_worker_start_ex(int rank);
void myth_startpoint_init_ex(int rank);
void myth_startpoint_exit_ex(int rank);

myth_thread_t myth_create(myth_func_t func,void *arg);
myth_thread_t myth_create_ex(myth_func_t func,void *arg,myth_thread_option_t opt);
myth_thread_t myth_create_nosched(myth_func_t func,void *arg,myth_thread_option_t opt);
void myth_exit(void *ret);
void myth_yield(int force_worksteal);
void myth_yield2(void);
void myth_join(myth_thread_t th,void **result);
myth_thread_t myth_self(void);
void myth_detach(myth_thread_t th);

int myth_setcancelstate(int state, int *oldstate);
int myth_setcanceltype(int type, int *oldtype);
int myth_cancel(myth_thread_t th);
void myth_testcancel(void);

void myth_set_def_stack_size(size_t newsize);

myth_barrier_t myth_barrier_create(int nthreads);
int myth_barrier_wait(myth_barrier_t bar);
int myth_barrier_destroy(myth_barrier_t bar);

myth_mutex_t myth_mutex_create(void);
void myth_mutex_destroy(myth_mutex_t mtx);
int myth_mutex_trylock(myth_mutex_t mtx);
void myth_mutex_lock(myth_mutex_t mtx);
void myth_mutex_unlock(myth_mutex_t mtx);

myth_cond_t myth_cond_create(void);
void myth_cond_destroy(myth_cond_t c);
void myth_cond_signal(myth_cond_t c);
void myth_cond_broadcast(myth_cond_t c);
void myth_cond_wait (myth_cond_t c,myth_mutex_t mtx);

myth_felock_t myth_felock_create(void);
int myth_felock_destroy(myth_felock_t fe);
int myth_felock_lock(myth_felock_t fe);
int myth_felock_wait_lock(myth_felock_t fe,int val);
int myth_felock_unlock(myth_felock_t fe);
int myth_felock_status(myth_felock_t fe);
int myth_felock_set_unlock(myth_felock_t fe,int val);

int myth_key_create(myth_key_t *__key,void (*__destr_function) (void *));
int myth_key_delete(myth_key_t __key);
void *myth_getspecific(myth_key_t __key);
int myth_setspecific(myth_key_t __key,void *__pointer);

void myth_serialize(myth_thread_t th,myth_pickle_t p);
#define myth_ext_serialize(th,p) myth_serialize(th,p)
myth_thread_t myth_deserialize(myth_pickle_t p);
myth_thread_t myth_ext_deserialize(myth_pickle_t p);

myth_thread_t myth_steal(void);
#define myth_ext_steal() myth_steal()
void myth_import(myth_thread_t th);
void myth_ext_import(myth_thread_t th);

void myth_release_stack(myth_thread_t th);
void myth_release_desc(myth_thread_t th);

void myth_log_start(void);
void myth_log_pause(void);
void myth_log_flush(void);
void myth_log_reset(void);
void myth_log_annotate_thread(myth_thread_t th,char *name);
//void myth_log_get_thread_annotation(myth_thread_t th,char *name);
void myth_sched_prof_start(void);
void myth_sched_prof_pause(void);

int myth_get_worker_num(void);
int myth_get_num_workers(void);

typedef int (*myth_wsapi_decidefn_t)(myth_thread_t th,void *udata);

myth_thread_t myth_wsapi_runqueue_peek(int victim,void *ptr,size_t *psize);
size_t myth_wsapi_get_hint_size(myth_thread_t th);
void *myth_wsapi_get_hint_ptr(myth_thread_t th);
void myth_wsapi_set_hint(myth_thread_t th,void **data,size_t *size);
int myth_wsapi_rand(void);
void myth_wsapi_randarr(int *ret,int n);
myth_thread_t myth_wsapi_runqueue_take(int victim,myth_wsapi_decidefn_t decidefn,void *udata);
int myth_wsapi_runqueue_pass(int target,myth_thread_t th);
void myth_wsapi_runqueue_push(myth_thread_t th);
myth_thread_t myth_wsapi_runqueue_pop(void);
int myth_wsapi_rand(void);
int myth_wsapi_rand2(int min,int max);
myth_steal_func_t myth_wsapi_set_stealfunc(myth_steal_func_t fn);

#endif /* MYTH_IF_NATIVE_H_ */
