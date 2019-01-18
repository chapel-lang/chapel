/* 
 * myth_init.c
 */
#include <sys/time.h>
#include <sched.h>

#include "myth/myth.h"

#include "myth_init.h"
#include "myth_log.h"

#include "myth_init_func.h"
#include "myth_log_func.h"


myth_globalattr_t g_attr;

#if 0

/* fill attr with default values;
   this gives the user a convenient way to customize
   MassiveThreads.
   myth_globalattr_t attr[1];
   myth_get_global_attr_default(attr);
   attr->n_workers = whatever;
   myth_set_global_attr(attr);
*/
int myth_get_attr_default(myth_globalattr_t * attr) {
  myth_globalattr_t a;
  {
    /* number of workers */
    int nw = 0;
    char * env = getenv(ENV_MYTH_WORKER_NUM);
    if (env) {
      nw = atoi(env);
    }
    if (nw <= 0) {
      nw = myth_get_cpu_num();
    }
    a.n_workers = nw;
  }
  {
    /* default stack size */
    size_t sz = 0;
    char * env = getenv(ENV_MYTH_DEF_STKSIZE);
    if (env) {
      sz = atoi(env);
    }
    if (sz <= 0) {
      sz = MYTH_DEF_STACK_SIZE;
    }
    a.default_stack_size = sz;
  }
  {
    /* bind workers */
    int bw = MYTH_BIND_WORKERS;
    char * env = getenv(ENV_MYTH_BIND_WORKERS);
    if (env){
      bw = atoi(env);
    }
    a.bind_workers = bw;
  }
  *attr = a;
  return 0;
}

int myth_set_attr(const myth_globalattr_t * attr) {
  if (g_myth_init_state == myth_init_state_initialized) {
    fprintf(stderr,
	    "myth_set_attr called after massivethreads has been initialized\n");
    return 0;			/* NG */
  }
  if (attr) {
    g_attr = *attr;
  } else {
    myth_get_attr_default(&g_attr);
  }
  return 1;			/* OK */
}
#endif

int myth_init_ex_body_really(const myth_globalattr_t * attr) {
  myth_init_read_available_cpu_list();
  myth_globalattr_set_default_body(attr);
  int nw = g_attr.n_workers;
  //Initialize logger
  myth_log_init();
  //Initialize memory allocators
  myth_flmalloc_init(nw);
  //myth_malloc_wrapper_init(nthreads);
#if MYTH_WRAP_SOCKIO
  //Initialize I/O
  myth_io_init();
#endif
  //Initialize TLS
  myth_tls_init(nw);
  //Create barrier
  real_pthread_barrier_init(&g_worker_barrier, NULL, nw);
  //Allocate worker thread descriptors
  g_envs = myth_malloc(sizeof(myth_running_env) * nw);
  g_envs_sz = nw;
  //Initialize TLS for worker thread descriptor
  myth_env_init();
#if MYTH_ECO_MODE
  myth_eco_init();
#endif

  //Create worker threads
  intptr_t i;
  for (i = 1; i < nw; i++){
    real_pthread_create(&g_envs[i].worker, NULL,
			myth_worker_thread_fn, (void*)i);
  }
  g_envs[0].worker = real_pthread_self();
  myth_worker_thread_fn((void*)0);
  return 0;
}

int myth_init_once_ctl_try_set(volatile int * var, int old, int new) {
  return __sync_bool_compare_and_swap(var, old, new);
}

void myth_init_once_ctl_wait(volatile int * var, int val) {
  while (*var != val) {
    real_sched_yield();
  }
}

volatile int g_myth_init_state = myth_init_state_uninit;
cpu_set_t g_proc_cpuset;

//Initialize
int myth_init_ex_body(const myth_globalattr_t * attr) {
  if (g_myth_init_state == myth_init_state_initialized) {
    return 1;			/* OK */
  }
  if (!myth_init_once_ctl_try_set(&g_myth_init_state,
				  myth_init_state_uninit,
				  myth_init_state_initializing)) {
    myth_init_once_ctl_wait(&g_myth_init_state, myth_init_state_initialized);
    return 1;			/* OK */
  }
  assert(g_myth_init_state == myth_init_state_initializing);
  myth_init_ex_body_really(attr);
  g_myth_init_state = myth_init_state_initialized;
  return 1;			/* OK */
}


void myth_emit_log(FILE *fp_prof_out) {
  //Write profiling log
  (void)fp_prof_out;
  uint64_t t1, t0, tx;
  {
    t0=0; t1=0; tx=0;
    int i;
    for (i=0;i<100;i++){
      t0 = myth_get_rdtsc();
      t1 = myth_get_rdtsc();
      tx += t1-t0;
    }
    tx /= 100;
  }
#if MYTH_PROF_SHOW_WORKER
  int i;
  for (i=0; i < g_attr.n_workers; i++) {
    double nc = env[i].prof_data.create_cnt;
#if MYTH_CREATE_PROF && !MYTH_PROF_COUNT_CSV
    fprintf(fp_prof_out,
	    "Create threads %lu : %lf cycles/creation\n",
	    (unsigned long)env[i].prof_data.create_cnt,
	    env[i].prof_data.create_cycles/nc -tx);
#endif
#if MYTH_CREATE_PROF_DETAIL && !MYTH_PROF_COUNT_CSV
    fprintf(fp_prof_out,"A:%lf B:%lf C:%lf D:%lf\n"
	    ,env[i].prof_data.create_cyclesA / nc - tx
	    ,env[i].prof_data.create_cyclesB / nc - tx
	    ,env[i].prof_data.create_cyclesC / nc - tx
	    ,env[i].prof_data.create_cyclesD / nc - tx);
#endif
#if MYTH_ENTRY_POINT_PROF && !MYTH_PROF_COUNT_CSV
#if SWITCH_AFTER_CREATE
    fprintf(fp_prof_out,
	    "Ran threads %lu : %lf cycle overhead/run\n",
	    (unsigned long)env[i].prof_data.ep_cnt,
	    (env[i].prof_data.ep_cyclesB)/(double)env[i].prof_data.ep_cnt-tx);
#else
    fprintf(fp_prof_out,
	    "Ran threads %lu : %lf cycle overhead/run\n",
	    (unsigned long)env[i].prof_data.ep_cnt,
	    (env[i].prof_data.ep_cyclesA+env[i].prof_data.ep_cyclesB)/(double)env[i].prof_data.ep_cnt-tx*2);
    fprintf(fp_prof_out,
	    "A:%lf B:%lf\n"
	    ,env[i].prof_data.ep_cyclesA/(double)env[i].prof_data.ep_cnt-tx
	    ,env[i].prof_data.ep_cyclesB/(double)env[i].prof_data.ep_cnt-tx);
#endif
#endif
#if MYTH_JOIN_PROF  && !MYTH_PROF_COUNT_CSV
    fprintf(fp_prof_out,
	    "Joins %lu : %lf cycles/join\n",
	    (unsigned long)env[i].prof_data.join_cnt,
	    env[i].prof_data.join_cycles/(double)env[i].prof_data.join_cnt-tx);
#endif
#if MYTH_ALLOC_PROF
    fprintf(fp_prof_out,
	    "Malloc %lu\n",
	    (unsigned long)env[i].prof_data.malloc_cnt);
#endif
#if MYTH_PROF_COUNT_CSV
    fprintf(fp_prof_out,
	    "%lu,%lu,%lu\n",
	    (unsigned long)env[i].prof_data.create_cnt,
	    (unsigned long)env[i].prof_data.ep_cnt,
	    (unsigned long)env[i].prof_data.join_cnt);
#endif
  }
#else
  MAY_BE_UNUSED uint64_t sum1,sum2,sum3,sum4,sum5;
  MAY_BE_UNUSED int i;
  i=0;
  sum1=0;sum2=0;sum3=0;sum4=0;sum5=0;
#if MYTH_CREATE_PROF && !MYTH_PROF_COUNT_CSV
  sum1=0;sum2=0;
  for (i=0;i<g_attr.n_workers;i++){
    sum1+=g_envs[i].prof_data.create_cnt;
    sum2+=g_envs[i].prof_data.create_cycles;
  }
  fprintf(fp_prof_out,"Create threads %lu : %lf cycles/creation\n",
	  (unsigned long)sum1,sum2/(double)sum1-tx);
#endif
#if MYTH_CREATE_PROF_DETAIL && !MYTH_PROF_COUNT_CSV
  sum1=0;sum2=0;sum3=0;sum4=0;
  for (i=0;i<g_attr.n_workers;i++){
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
#if MYTH_ENTRY_POINT_PROF && !MYTH_PROF_COUNT_CSV
  sum1=0;sum2=0;sum3=0;
  for (i=0;i<g_attr.n_workers;i++){
    sum1+=g_envs[i].prof_data.ep_cnt;
    sum2+=g_envs[i].prof_data.ep_cyclesA;
    sum3+=g_envs[i].prof_data.ep_cyclesB;
  }
#if SWITCH_AFTER_CREATE
  fprintf(fp_prof_out,"Ran threads %lu : %lf cycle overhead/run\n",(unsigned long)sum1,sum3/(double)sum1-tx);
#else
  fprintf(fp_prof_out,"Ran threads %lu : %lf cycle overhead/run\n",(unsigned long)sum1,(sum2+sum3)/(double)sum1-tx*2);
  fprintf(fp_prof_out,"A:%lf B:%lf\n"
	  ,sum2/(double)sum1-tx
	  ,sum3/(double)sum1-tx);
#endif
#endif
#if MYTH_EP_PROF_DETAIL
  sum1=0;sum2=0;sum3=0;sum4=0;
  for (i=0;i<g_attr.n_workers;i++){
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
#if MYTH_JOIN_PROF  && !MYTH_PROF_COUNT_CSV
  sum1=0;sum2=0;
  for (i=0;i<g_attr.n_workers;i++){
    sum1+=g_envs[i].prof_data.join_cnt;
    sum2+=g_envs[i].prof_data.join_cycles;
  }
  fprintf(fp_prof_out,"Joins %lu : %lf cycles/join\n",(unsigned long)sum1,sum2/(double)sum1-tx);
#endif
#if MYTH_JOIN_PROF_DETAIL
  sum1=0;sum2=0;sum3=0;sum4=0;
  for (i=0;i<g_attr.n_workers;i++){
    sum1+=g_envs[i].prof_data.join_d_cnt;
    sum2+=g_envs[i].prof_data.join_join;
    sum3+=g_envs[i].prof_data.join_release;
  }
  fprintf(fp_prof_out,"At join (count : %ld ):\n",sum1);
  fprintf(fp_prof_out,"Join operation : %lf\n",sum2/(double)sum1-tx);
  fprintf(fp_prof_out,"Frame release : %lf\n",sum3/(double)sum1-tx);
#endif
#if MYTH_WS_PROF_DETAIL
  sum1=0;sum2=0;sum3=0;sum4=0;
  fprintf(fp_prof_out,"WS attempts:\n");
  for (i=0;i<g_attr.n_workers;i++){
    sum1+=g_envs[i].prof_data.ws_hit_cnt;
    sum2+=g_envs[i].prof_data.ws_hit_cycles;
    sum3+=g_envs[i].prof_data.ws_miss_cnt;
    sum4+=g_envs[i].prof_data.ws_miss_cycles;
    int j;
    for (j=0;j<g_attr.n_workers;j++){
      fprintf(fp_prof_out,"%d ",(int)g_envs[i].prof_data.ws_attempt_count[j]);
    }
    fprintf(fp_prof_out,"\n");
  }
  fprintf(fp_prof_out,"At work-stealing :\n");
  fprintf(fp_prof_out,"Hit  : %ld ( %lf )\n",(unsigned long)sum1,sum2/(double)sum1-tx);
  fprintf(fp_prof_out,"Miss : %ld ( %lf )\n",(unsigned long)sum3,sum4/(double)sum3-tx);
#endif
#if MYTH_SWITCH_PROF
  sum1=0;sum2=0;sum3=0;sum4=0;
  for (i=0;i<g_attr.n_workers;i++){
    sum1+=g_envs[i].prof_data.sw_cnt;
    sum2+=g_envs[i].prof_data.sw_cycles;
  }
  fprintf(fp_prof_out,"Context switch (count : %ld ):\n",(unsigned long)sum1);
  fprintf(fp_prof_out,"Overhead : %lf cycles\n",sum2/(double)sum1-tx);
#endif
#if MYTH_ALLOC_PROF
  sum1=0;sum2=0;
  sum3=0;sum4=0;
  for (i=0;i<g_attr.n_workers;i++){
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
  for (i=0;i<g_attr.n_workers;i++){
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
  for (i=0;i<g_attr.n_workers;i++){
    sum1+=g_envs[i].prof_data.smalloc_cnt;
    sum2+=g_envs[i].prof_data.salloc_cnt;
    sum3+=g_envs[i].prof_data.smalloc_cycles;
    sum4+=g_envs[i].prof_data.saddlist_cycles;
  }
  fprintf(fp_prof_out,"Stack:\n");
  fprintf(fp_prof_out,"Malloc %lu alloc %lu (ratio:%lf)\n",(unsigned long)sum1,(unsigned long)sum2,sum1/(double)sum2);
  fprintf(fp_prof_out,"mmap/malloc : %lf cycles/alloc, addlist : %lf cycles/alloc\n",sum3/(double)sum2,sum4/(double)sum2);

#endif
#if MYTH_IO_PROF_DETAIL
  sum1=0;sum2=0;sum3=0;sum4=0;
  for (i=0;i<g_attr.n_workers;i++){
    sum1+=g_envs[i].prof_data.io_succ_send_cnt;
    sum2+=g_envs[i].prof_data.io_block_send_cnt;
    sum3+=g_envs[i].prof_data.io_succ_send_cycles;
    sum4+=g_envs[i].prof_data.io_block_send_cycles;
  }
  fprintf(fp_prof_out,"Send:\n");
  fprintf(fp_prof_out,"Success:Block = %lu : %lu\n",(unsigned long)sum1,(unsigned long)sum2);
  fprintf(fp_prof_out,"Overhead %lf : %lf\n",sum3/(double)sum1-tx,sum4/(double)sum2-tx);
  sum1=0;sum2=0;sum3=0;sum4=0;
  for (i=0;i<g_attr.n_workers;i++){
    sum1+=g_envs[i].prof_data.io_succ_recv_cnt;
    sum2+=g_envs[i].prof_data.io_block_recv_cnt;
    sum3+=g_envs[i].prof_data.io_succ_recv_cycles;
    sum4+=g_envs[i].prof_data.io_block_recv_cycles;
  }
  fprintf(fp_prof_out,"Recv:\n");
  fprintf(fp_prof_out,"Success:Block = %lu : %lu\n",(unsigned long)sum1,(unsigned long)sum2);
  fprintf(fp_prof_out,"Overhead %lf : %lf\n",sum3/(double)sum1-tx,sum4/(double)sum2-tx);
  sum1=0;sum2=0;sum3=0;sum4=0;
  for (i=0;i<g_attr.n_workers;i++){
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
  for (i=0;i<g_attr.n_workers;i++){
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
#if MYTH_PROF_COUNT_CSV
  sum1=0;sum2=0;sum3=0;
  for (i=0;i<g_attr.n_workers;i++){
    sum1+=g_envs[i].prof_data.create_cnt;
    sum2+=g_envs[i].prof_data.ep_cnt;
    sum3+=g_envs[i].prof_data.join_cnt;
  }
  fprintf(fp_prof_out,"%lu,%lu,%lu\n",(unsigned long)sum1,(unsigned long)sum2,(unsigned long)sum3);
#endif
#endif
}

static void myth_fini_body_really(void) {
  //Output Log
  myth_emit_log(stderr);
  //Destroy barrier
  real_pthread_barrier_destroy(&g_worker_barrier);
  //Unload DLL and functions
  //myth_free_original_funcs();
  myth_tls_fini();
#if MYTH_WRAP_SOCKIO
  myth_io_fini();
#endif
  //Finalize logger
  myth_log_fini();
  //Release worker thread descriptors
  myth_free_with_size(g_envs, sizeof(myth_running_env)*g_attr.n_workers);
  //Release allocator
  myth_flmalloc_fini();
  //myth_malloc_wrapper_fini();
  //Release TLS
  myth_env_fini();
}

int myth_fini_body() {
  if (g_myth_init_state == myth_init_state_uninit) {
    return 1;			/* OK */
  }
  myth_init_once_ctl_wait(&g_myth_init_state, myth_init_state_initialized);
  //add context switch as a sentinel for emitting logs
  int i;
  for (i = 0; i < g_attr.n_workers; i++){
    myth_log_add_context_switch(&g_envs[i], THREAD_PTR_SCHED_TERM);
  }
  myth_startpoint_exit_ex_body(0);
  myth_running_env_t env = myth_get_current_env();
  int rank = env->rank;
  assert(rank == 0);
  //Wait for other worker threads
  for (i = 1; i < g_attr.n_workers; i++) {
    real_pthread_join(g_envs[i].worker, NULL);
  }
  myth_fini_body_really();
  // Restore affinity
  sched_setaffinity(getpid(), sizeof(cpu_set_t), &g_proc_cpuset);
  g_myth_init_state = myth_init_state_uninit;
  return 0;
}


