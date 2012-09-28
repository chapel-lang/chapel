#include <sys/time.h>

#include "myth_worker.h"
#include "myth_worker_proto.h"
#include "myth_tls.h"
#include "myth_malloc_wrapper.h"

#include "myth_io.h"
#include "myth_io_proto.h"
#include "myth_io_func.h"
#include "myth_tls_func.h"

//Initialize specifying the number of worker threads
//If specified less than 1, MYTH_WORKER_NUM or the number of CPU cores used instead.
int myth_init_ex_body(int worker_num, size_t def_stack_size)
{
	intptr_t nthreads;
	myth_init_process_affinity_info();
	//Load original functions
	//myth_get_original_funcs();
	//Decide the number of worker threads.
	//If environment variable is set, use it. Otherwise use the number of CPU cores.
	char *env;
	env=getenv(ENV_MYTH_WORKER_NUM);
	nthreads=(worker_num>0)?worker_num:-1;
	if (nthreads<=0 && env){nthreads=atoi(env);}
	if (nthreads<=0){nthreads=myth_get_cpu_num();}
	g_worker_thread_num=nthreads;
	//set default stack size
	size_t s;
	env=getenv(ENV_MYTH_DEF_STKSIZE);
	s=def_stack_size;
	if (s==0 && env){
		s=atoi(env);
	}
	if (s>0){myth_set_def_stack_size_body(s);}
	//Initialize logger
	myth_log_init();
	//Initialize memory allocators
	myth_flmalloc_init(nthreads);
	myth_malloc_wrapper_init(nthreads);
#ifdef MYTH_WRAP_SOCKIO
	//Initialize I/O
	myth_io_init();
#endif
	//Initialize TLS
	myth_tls_init(nthreads);
	//Create barrier
	real_pthread_barrier_init(&g_worker_barrier,NULL,nthreads);
	//Allocate worker thread descriptors
	g_envs=myth_malloc(sizeof(myth_running_env)*nthreads);
	//Initialize TLS for worker thread descriptor
	myth_env_init();
	return nthreads;
}

//Initialize
void myth_init_body(int worker_num,size_t def_stack_size)
{
	myth_init_ex_body(worker_num,def_stack_size);
	//Create worker threads
	intptr_t i;
	for (i=1;i<g_worker_thread_num;i++){
		real_pthread_create(&g_envs[i].worker,NULL,myth_worker_thread_fn,(void*)i);
	}
	g_envs[0].worker=real_pthread_self();
	//Initialize each worker threads
	myth_worker_thread_fn((void*)(intptr_t)0);
}

void myth_emit_log(FILE *fp_prof_out)
{
	//Write profiling log
	uint64_t t1,t0,tx;
	{
		t0=0;t1=0;tx=0;
		int i;
		for (i=0;i<100;i++){
			t0=myth_get_rdtsc();
			t1=myth_get_rdtsc();
			tx+=t1-t0;
		}
		tx/=100;
	}
#ifdef MYTH_PROF_SHOW_WORKER
	int i;
	for (i=0;i<g_worker_thread_num;i++){
#if defined MYTH_CREATE_PROF && !defined MYTH_PROF_COUNT_CSV
		fprintf(fp_prof_out,"Create threads %lu : %lf cycles/creation\n",(unsigned long)env[i].prof_data.create_cnt,env[i].prof_data.create_cycles/(double)env[i].prof_data.create_cnt-tx);
#endif
#if defined MYTH_CREATE_PROF_DETAIL && !defined MYTH_PROF_COUNT_CSV
		fprintf(fp_prof_out,"A:%lf B:%lf C:%lf D:%lf\n"
				,env[i].prof_data.create_cyclesA/(double)env[i].prof_data.create_cnt-tx
				,env[i].prof_data.create_cyclesB/(double)env[i].prof_data.create_cnt-tx
				,env[i].prof_data.create_cyclesC/(double)env[i].prof_data.create_cnt-tx
				,env[i].prof_data.create_cyclesD/(double)env[i].prof_data.create_cnt-tx);
#endif
#if defined MYTH_ENTRY_POINT_PROF && !defined MYTH_PROF_COUNT_CSV
#ifdef SWITCH_AFTER_CREATE
		fprintf(fp_prof_out,"Ran threads %lu : %lf cycle overhead/run\n",(unsigned long)env[i].prof_data.ep_cnt,(env[i].prof_data.ep_cyclesB)/(double)env[i].prof_data.ep_cnt-tx);
#else
		fprintf(fp_prof_out,"Ran threads %lu : %lf cycle overhead/run\n",(unsigned long)env[i].prof_data.ep_cnt,(env[i].prof_data.ep_cyclesA+env[i].prof_data.ep_cyclesB)/(double)env[i].prof_data.ep_cnt-tx*2);
		fprintf(fp_prof_out,"A:%lf B:%lf\n"
				,env[i].prof_data.ep_cyclesA/(double)env[i].prof_data.ep_cnt-tx
				,env[i].prof_data.ep_cyclesB/(double)env[i].prof_data.ep_cnt-tx);
#endif
#endif
#if defined MYTH_JOIN_PROF  && !defined MYTH_PROF_COUNT_CSV
		fprintf(fp_prof_out,"Joins %lu : %lf cycles/join\n",(unsigned long)env[i].prof_data.join_cnt,env[i].prof_data.join_cycles/(double)env[i].prof_data.join_cnt-tx);
#endif
#ifdef MYTH_ALLOC_PROF
		fprintf(fp_prof_out,"Malloc %lu\n",(unsigned long)env[i].prof_data.malloc_cnt);
#endif
#ifdef MYTH_PROF_COUNT_CSV
		fprintf(fp_prof_out,"%lu,%lu,%lu\n",(unsigned long)env[i].prof_data.create_cnt,(unsigned long)env[i].prof_data.ep_cnt,(unsigned long)env[i].prof_data.join_cnt);
#endif
	}
#else
	MAY_BE_UNUSED uint64_t sum1,sum2,sum3,sum4,sum5;
	MAY_BE_UNUSED int i;
	i=0;
	sum1=0;sum2=0;sum3=0;sum4=0;sum5=0;
#if defined MYTH_CREATE_PROF && !defined MYTH_PROF_COUNT_CSV
	sum1=0;sum2=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.create_cnt;
		sum2+=g_envs[i].prof_data.create_cycles;
	}
	fprintf(fp_prof_out,"Create threads %lu : %lf cycles/creation\n",
			(unsigned long)sum1,sum2/(double)sum1-tx);
#endif
#if defined MYTH_CREATE_PROF_DETAIL && !defined MYTH_PROF_COUNT_CSV
	sum1=0;sum2=0;sum3=0;sum4=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.create_d_cnt;
		sum2+=g_envs[i].prof_data.create_alloc;
		sum3+=g_envs[i].prof_data.create_switch;
		sum4+=g_envs[i].prof_data.create_push;
	}
	fprintf(fp_prof_out,"At thread creation (count : %ld ):\n",(long)sum1);
	fprintf(fp_prof_out,"Frame allocation : %lf\n",sum2/(double)sum1-tx);
	fprintf(fp_prof_out,"Context switch : %lf\n",sum3/(double)sum1-tx);
	fprintf(fp_prof_out,"Runqueue operation(push) : %lf\n",sum4/(double)sum1-tx);
#endif
#if defined MYTH_ENTRY_POINT_PROF && !defined MYTH_PROF_COUNT_CSV
	sum1=0;sum2=0;sum3=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.ep_cnt;
		sum2+=g_envs[i].prof_data.ep_cyclesA;
		sum3+=g_envs[i].prof_data.ep_cyclesB;
	}
#ifdef SWITCH_AFTER_CREATE
	fprintf(fp_prof_out,"Ran threads %lu : %lf cycle overhead/run\n",(unsigned long)sum1,sum3/(double)sum1-tx);
#else
	fprintf(fp_prof_out,"Ran threads %lu : %lf cycle overhead/run\n",(unsigned long)sum1,(sum2+sum3)/(double)sum1-tx*2);
	fprintf(fp_prof_out,"A:%lf B:%lf\n"
			,sum2/(double)sum1-tx
			,sum3/(double)sum1-tx);
#endif
#endif
#if defined MYTH_EP_PROF_DETAIL
	sum1=0;sum2=0;sum3=0;sum4=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.ep_d_cnt;
		sum2+=g_envs[i].prof_data.ep_join;
		sum3+=g_envs[i].prof_data.ep_switch;
		sum4+=g_envs[i].prof_data.ep_pop;
	}
	fprintf(fp_prof_out,"At thread cleanup (count : %ld ):\n",sum1);
	fprintf(fp_prof_out,"Join operation : %lf\n",sum2/(double)sum1-tx*2);
	fprintf(fp_prof_out,"Context switch : %lf\n",sum3/(double)sum1-tx);
	fprintf(fp_prof_out,"Runqueue operation(pop) : %lf\n",sum4/(double)sum1-tx);
#endif
#if defined MYTH_JOIN_PROF  && !defined MYTH_PROF_COUNT_CSV
	sum1=0;sum2=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.join_cnt;
		sum2+=g_envs[i].prof_data.join_cycles;
	}
	fprintf(fp_prof_out,"Joins %lu : %lf cycles/join\n",(unsigned long)sum1,sum2/(double)sum1-tx);
#endif
#if defined MYTH_JOIN_PROF_DETAIL
	sum1=0;sum2=0;sum3=0;sum4=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.join_d_cnt;
		sum2+=g_envs[i].prof_data.join_join;
		sum3+=g_envs[i].prof_data.join_release;
	}
	fprintf(fp_prof_out,"At join (count : %ld ):\n",sum1);
	fprintf(fp_prof_out,"Join operation : %lf\n",sum2/(double)sum1-tx);
	fprintf(fp_prof_out,"Frame release : %lf\n",sum3/(double)sum1-tx);
#endif
#ifdef MYTH_WS_PROF_DETAIL
	sum1=0;sum2=0;sum3=0;sum4=0;
	fprintf(fp_prof_out,"WS attempts:\n");
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.ws_hit_cnt;
		sum2+=g_envs[i].prof_data.ws_hit_cycles;
		sum3+=g_envs[i].prof_data.ws_miss_cnt;
		sum4+=g_envs[i].prof_data.ws_miss_cycles;
		int j;
		for (j=0;j<g_worker_thread_num;j++){
			fprintf(fp_prof_out,"%d ",(int)g_envs[i].prof_data.ws_attempt_count[j]);
		}
		fprintf(fp_prof_out,"\n");
	}
	fprintf(fp_prof_out,"At work-stealing :\n");
	fprintf(fp_prof_out,"Hit  : %ld ( %lf )\n",(unsigned long)sum1,sum2/(double)sum1-tx);
	fprintf(fp_prof_out,"Miss : %ld ( %lf )\n",(unsigned long)sum3,sum4/(double)sum3-tx);
#endif
#ifdef MYTH_SWITCH_PROF
	sum1=0;sum2=0;sum3=0;sum4=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.sw_cnt;
		sum2+=g_envs[i].prof_data.sw_cycles;
	}
	fprintf(fp_prof_out,"Context switch (count : %ld ):\n",(unsigned long)sum1);
	fprintf(fp_prof_out,"Overhead : %lf cycles\n",sum2/(double)sum1-tx);
#endif
#ifdef MYTH_ALLOC_PROF
	sum1=0;sum2=0;
	sum3=0;sum4=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.malloc_cnt;
		sum2+=g_envs[i].prof_data.alloc_cnt;
		sum3+=g_envs[i].prof_data.malloc_cycles;
		sum4+=g_envs[i].prof_data.addlist_cycles;
	}
	fprintf(fp_prof_out,"s+d:\n");
	fprintf(fp_prof_out,"Malloc %lu alloc %lu (ratio:%lf)\n",(unsigned long)sum1,(unsigned long)sum2,sum1/(double)sum2);
	fprintf(fp_prof_out,"mmap/malloc : %lf cycles/alloc, addlist : %lf cycles/alloc\n",sum3/(double)sum2,sum4/(double)sum2);
	sum1=0;sum2=0;
	sum3=0;sum4=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.dmalloc_cnt;
		sum2+=g_envs[i].prof_data.dalloc_cnt;
		sum3+=g_envs[i].prof_data.dmalloc_cycles;
		sum4+=g_envs[i].prof_data.daddlist_cycles;
	}
	fprintf(fp_prof_out,"Desc:\n");
	fprintf(fp_prof_out,"Malloc %lu alloc %lu (ratio:%lf)\n",(unsigned long)sum1,(unsigned long)sum2,sum1/(double)sum2);
	fprintf(fp_prof_out,"mmap/malloc : %lf cycles/alloc, addlist : %lf cycles/alloc\n",sum3/(double)sum2,sum4/(double)sum2);
	sum1=0;sum2=0;
	sum3=0;sum4=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.smalloc_cnt;
		sum2+=g_envs[i].prof_data.salloc_cnt;
		sum3+=g_envs[i].prof_data.smalloc_cycles;
		sum4+=g_envs[i].prof_data.saddlist_cycles;
	}
	fprintf(fp_prof_out,"Stack:\n");
	fprintf(fp_prof_out,"Malloc %lu alloc %lu (ratio:%lf)\n",(unsigned long)sum1,(unsigned long)sum2,sum1/(double)sum2);
	fprintf(fp_prof_out,"mmap/malloc : %lf cycles/alloc, addlist : %lf cycles/alloc\n",sum3/(double)sum2,sum4/(double)sum2);

#endif
#if defined MYTH_IO_PROF_DETAIL
	sum1=0;sum2=0;sum3=0;sum4=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.io_succ_send_cnt;
		sum2+=g_envs[i].prof_data.io_block_send_cnt;
		sum3+=g_envs[i].prof_data.io_succ_send_cycles;
		sum4+=g_envs[i].prof_data.io_block_send_cycles;
	}
	fprintf(fp_prof_out,"Send:\n");
	fprintf(fp_prof_out,"Success:Block = %lu : %lu\n",(unsigned long)sum1,(unsigned long)sum2);
	fprintf(fp_prof_out,"Overhead %lf : %lf\n",sum3/(double)sum1-tx,sum4/(double)sum2-tx);
	sum1=0;sum2=0;sum3=0;sum4=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.io_succ_recv_cnt;
		sum2+=g_envs[i].prof_data.io_block_recv_cnt;
		sum3+=g_envs[i].prof_data.io_succ_recv_cycles;
		sum4+=g_envs[i].prof_data.io_block_recv_cycles;
	}
	fprintf(fp_prof_out,"Recv:\n");
	fprintf(fp_prof_out,"Success:Block = %lu : %lu\n",(unsigned long)sum1,(unsigned long)sum2);
	fprintf(fp_prof_out,"Overhead %lf : %lf\n",sum3/(double)sum1-tx,sum4/(double)sum2-tx);
	sum1=0;sum2=0;sum3=0;sum4=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.io_epoll_hit;
		sum2+=g_envs[i].prof_data.io_epoll_hit_cycles;
		sum3+=g_envs[i].prof_data.io_epoll_miss;
		sum4+=g_envs[i].prof_data.io_epoll_miss_cycles;
	}
	fprintf(fp_prof_out,"epoll:\n");
	fprintf(fp_prof_out,"hit: %lu ( %lf cycles)\n",(unsigned long)sum1,sum2/(double)sum1-tx);
	fprintf(fp_prof_out,"miss: %lu ( %lf cycles)\n",(unsigned long)sum3,sum4/(double)sum3-tx);
	fprintf(fp_prof_out,"overall: %lu ( %lf cycles)\n",(unsigned long)(sum1+sum3),(sum2+sum4)/(double)(sum1+sum3)-tx);
	sum1=0;sum2=0;sum3=0;sum4=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.io_chk_hit;
		sum2+=g_envs[i].prof_data.io_chk_hit_cycles;
		sum3+=g_envs[i].prof_data.io_chk_miss;
		sum4+=g_envs[i].prof_data.io_chk_miss_cycles;
	}
	fprintf(fp_prof_out,"I/O check:\n");
	fprintf(fp_prof_out,"hit: %lu ( %lf cycles)\n",(unsigned long)sum1,sum2/(double)sum1-tx);
	fprintf(fp_prof_out,"miss: %lu ( %lf cycles)\n",(unsigned long)sum3,sum4/(double)sum3-tx);
	fprintf(fp_prof_out,"overall: %lu ( %lf cycles)\n",(unsigned long)(sum1+sum3),(sum2+sum4)/(double)(sum1+sum3)-tx);
#endif
#ifdef MYTH_PROF_COUNT_CSV
	sum1=0;sum2=0;sum3=0;
	for (i=0;i<g_worker_thread_num;i++){
		sum1+=g_envs[i].prof_data.create_cnt;
		sum2+=g_envs[i].prof_data.ep_cnt;
		sum3+=g_envs[i].prof_data.join_cnt;
	}
	fprintf(fp_prof_out,"%lu,%lu,%lu\n",(unsigned long)sum1,(unsigned long)sum2,(unsigned long)sum3);
#endif
#endif
}

void myth_fini_ex_body(void)
{
	//Output Log
	myth_emit_log(stderr);
	//Destroy barrier
	real_pthread_barrier_destroy(&g_worker_barrier);
	//Unload DLL and functions
	//myth_free_original_funcs();
	myth_tls_fini();
#ifdef MYTH_WRAP_SOCKIO
	myth_io_fini();
#endif
	//Finalize logger
	myth_log_fini();
	//Release worker thread descriptors
	myth_free(g_envs,sizeof(myth_running_env)*g_worker_thread_num);
	//Release allocator
	myth_flmalloc_fini();
	myth_malloc_wrapper_fini();
	//Release TLS
	myth_env_fini();
}

//Termination
void myth_fini_body(void)
{
	//add context switch as a sentinel for emitting logs
	int i;
	for (i=0;i<g_worker_thread_num;i++){
		myth_log_add_context_switch(&g_envs[i],THREAD_PTR_SCHED_TERM);
	}
	myth_startpoint_exit_ex_body(0);
	myth_running_env_t env;
	int rank;
	env=myth_get_current_env();
	rank=env->rank;
	assert(rank==0);
	//Wait for other worker threads
	for (i=1;i<g_worker_thread_num;i++){
		real_pthread_join(g_envs[i].worker,NULL);
	}
	myth_fini_ex_body();
}

//Tell all the worker threads to terminate
//Worker threads initialized by "myth_startpoint_init_ex" is NOT terminated by this function
//To terminate them, call "myth_startpoint_exit_ex" from the context that "myth_startpoint_init_ex" called
void myth_notify_workers_exit(void)
{
	int i;
	for (i=0;i<g_worker_thread_num;i++){
		if (g_envs[i].exit_flag==0)
			g_envs[i].exit_flag=1;
	}
}
