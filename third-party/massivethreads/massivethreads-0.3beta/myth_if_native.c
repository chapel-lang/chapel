#include "myth_init.h"
#include "myth_sched.h"
#include "myth_worker.h"
#include "myth_io.h"
#include "myth_tls.h"
#include "myth_sync.h"

#include "myth_if_native.h"

#include "myth_worker_func.h"
#include "myth_io_func.h"
#include "myth_sync_func.h"
#include "myth_sched_func.h"
#include "myth_tls_func.h"

void myth_init(void)
{
	myth_init_body(0,0);
}
void myth_init_withparam(int worker_num,size_t def_stack_size)
{
	myth_init_body(worker_num,def_stack_size);
}
void myth_fini(void)
{
	myth_fini_body();
}

//alternative init/fini
int myth_init_ex(int worker_num,size_t def_stack_size)
{
	return myth_init_ex_body(worker_num,def_stack_size);
}

void myth_fini_ex(void)
{
	myth_fini_ex_body();
}

void myth_exit_workers_ex(void)
{
	myth_notify_workers_exit();
}

void myth_ext_exit_workers_ex(void)
{
	myth_exit_workers_ex();
}

void myth_worker_start_ex(int rank)
{
	myth_worker_start_ex_body(rank);
}

void myth_startpoint_init_ex(int rank)
{
	myth_startpoint_init_ex_body(rank);
}

void myth_startpoint_exit_ex(int rank)
{
	myth_startpoint_exit_ex_body(rank);
}

int myth_get_worker_num(void)
{
	return myth_get_worker_num_body();
}

int myth_get_num_workers(void)
{
	return myth_get_num_workers_body();
}

myth_thread_t myth_self(void)
{
	return myth_self_body();
}

myth_thread_t myth_create(myth_func_t func,void *arg)
{
	return myth_create_body(func,arg,0);
}

myth_thread_t myth_create_ex(myth_func_t func,void *arg,myth_thread_option_t opt)
{
	if (opt->switch_immediately){
		return myth_create_body(func,arg,opt->stack_size);
	}
	else{
		return myth_create_ns_body(func,arg,opt->stack_size);
	}
}

void myth_exit(void *ret)
{
	myth_exit_body(ret);
}

void myth_detach(myth_thread_t th)
{
	myth_detach_body(th);
}

void myth_yield(int force_worksteal)
{
	myth_yield_body(force_worksteal);
}
void myth_yield2(void)
{
	myth_yield2_body();
}

void myth_join(myth_thread_t th,void **result)
{
	myth_join_body(th,result);
}

int myth_setcancelstate(int state, int *oldstate)
{
	return myth_setcancelstate_body(state,oldstate);
}

int myth_setcanceltype(int type, int *oldtype)
{
	return myth_setcanceltype_body(type,oldtype);
}

int myth_cancel(myth_thread_t th)
{
	return myth_cancel_body(th);
}

void myth_testcancel(void)
{
	myth_testcancel_body();
}

int myth_key_create(myth_key_t *__key,void (*__destr_function) (void *))
{
	return myth_key_create_body(__key,__destr_function);
}

int myth_key_delete(myth_key_t __key)
{
	return myth_key_delete_body(__key);
}

void *myth_getspecific(myth_key_t __key)
{
	return myth_getspecific_body(__key);
}

int myth_setspecific(myth_key_t __key,void *__pointer)
{
	return myth_setspecific_body(__key,__pointer);
}

void myth_set_def_stack_size(size_t newsize)
{
	myth_set_def_stack_size_body(newsize);
}

void myth_log_start(void)
{
	myth_log_start_body();
}

void myth_log_pause(void)
{
	myth_log_pause_body();
}

void myth_log_flush(void)
{
	myth_log_flush_body();
}

void myth_log_reset(void)
{
	myth_log_reset_body();
}

void myth_log_annotate_thread(myth_thread_t th,char *name)
{
	myth_log_annotate_thread_body(th,name);
}
/*
void myth_log_get_thread_annotation(myth_thread_t th,char *name)
{
	myth_log_get_thread_annotation_body(th,name);
}
*/
void myth_sched_prof_start(void)
{
	myth_sched_prof_start_body();
}

void myth_sched_prof_pause(void)
{
	myth_sched_prof_pause_body();
}

//Debug function
/*void myth_dprintf_1(char *func,char *fmt,...)
{
#ifdef MYTH_DEBUG
	static pthread_mutex_t mtx=PTHREAD_MUTEX_INITIALIZER;
	myth_running_env_t env;
	char tmp[1000];
	va_list ap;
	env=myth_get_current_env();
	va_start(ap,fmt);
	vsprintf(tmp,fmt,ap);
	va_end(ap);
	pthread_mutex_lock(&mtx);
	fprintf(stderr,"%s(%d):%s",func,env->rank,tmp);
	fflush(stderr);
	pthread_mutex_unlock(&mtx);
#endif
}*/

myth_barrier_t myth_barrier_create(int nthreads)
{
	return myth_barrier_create_body(nthreads);
}

int myth_barrier_wait(myth_barrier_t bar)
{
	return myth_barrier_wait_body(bar);
}

int myth_barrier_destroy(myth_barrier_t bar)
{
	return myth_barrier_destroy_body(bar);
}

//join counter:Implementation is not complete, not recommended for use
myth_jc_t myth_jc_create(int val)
{
	return myth_jc_create_body(val);
}

void myth_jc_wait(myth_jc_t jc)
{
	myth_jc_wait_body(jc);
}

void myth_jc_dec(myth_jc_t jc)
{
	myth_jc_dec_body(jc);
}

myth_felock_t myth_felock_create(void)
{
	return myth_felock_create_body();
}

int myth_felock_destroy(myth_felock_t fe)
{
	return myth_felock_destroy_body(fe);
}

int myth_felock_lock(myth_felock_t fe)
{
	return myth_felock_lock_body(fe);
}

int myth_felock_wait_lock(myth_felock_t fe,int val)
{
	return myth_felock_wait_lock_body(fe,val);
}

int myth_felock_unlock(myth_felock_t fe)
{
	return myth_felock_unlock_body(fe);
}

int myth_felock_status(myth_felock_t fe)
{
	return myth_felock_status_body(fe);
}

int myth_felock_set_unlock(myth_felock_t fe,int val)
{
	return myth_felock_set_unlock_body(fe,val);
}

myth_mutex_t myth_mutex_create(void)
{
	return myth_mutex_create_body();
}

void myth_mutex_destroy(myth_mutex_t mtx)
{
	myth_mutex_destroy_body(mtx);
}

int myth_mutex_trylock(myth_mutex_t mtx)
{
	return myth_mutex_trylock_body(mtx);
}

void myth_mutex_lock(myth_mutex_t mtx)
{
	myth_mutex_lock_body(mtx);
}

void myth_mutex_unlock(myth_mutex_t mtx)
{
	myth_mutex_unlock_body(mtx);
}

myth_cond_t myth_cond_create(void)
{
	return myth_cond_create_body();
}

void myth_cond_destroy(myth_cond_t c)
{
	myth_cond_destroy_body(c);
}

void myth_cond_signal(myth_cond_t c)
{
	myth_cond_signal_body(c);
}

void myth_cond_broadcast(myth_cond_t c)
{
	myth_cond_broadcast_body(c);
}

void myth_cond_wait (myth_cond_t c,myth_mutex_t mtx)
{
	myth_cond_wait_body(c,mtx);
}

//TODO: temporalily disable
#if 0

//serialize
void myth_serialize(myth_thread_t th,myth_pickle_t p)
{
	myth_serialize_body(th,p);
}

//Deserialize
myth_thread_t myth_deserialize(myth_pickle_t p)
{
	return myth_deserialize_body(p);
}
myth_thread_t myth_ext_deserialize(myth_pickle_t p)
{
	return myth_ext_deserialize_body(p);
}

myth_thread_t myth_steal(void)
{
	return myth_steal_body();
}

void myth_import(myth_thread_t th)
{
	myth_import_body(th);
}

void myth_ext_import(myth_thread_t th)
{
	myth_ext_import_body(th);
}

void myth_release_stack(myth_thread_t th)
{
	myth_release_stack_body(th);
}

void myth_release_desc(myth_thread_t th)
{
	myth_release_desc_body(th);
}
#endif
