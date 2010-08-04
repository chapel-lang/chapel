/*   $Source: /var/local/cvs/gasnet/other/smp-collectives/smp_coll.c,v $
 *     $Date: 2009/10/22 20:24:55 $
 * $Revision: 1.3 $
 * Description: Shared Memory Collectives
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <smp-collectives/smp_coll_internal.h>
#include <smp-collectives/smp_coll_dissem.c>
#if defined(ENABLE_AFFINITY_VIA_SOLARIS)
  #include <sys/types.h>
  #include <sys/processor.h>
  #include <sys/procset.h>
#endif




#define BOOTSTRAP_BARRIER(HANDLE, FLAGS) smp_coll_barrier_cond_var(HANDLE, FLAGS)
static volatile uint32_t *smp_coll_all_flags;
static volatile uint32_t *smp_coll_all_barrier_flags;
static volatile uint32_t *smp_coll_all_bcast_flags;
static gasnett_atomic_t *atomic_vars;


void smp_coll_set_affinity(int location) {
#if defined(ENABLE_AFFINITY_VIA_SOLARIS)
  if(processor_bind(P_LWPID, P_MYID, location, NULL)<0){
    fprintf(stderr, "WARNING: Couldn't bind thread %d\n",location);
  }
#else
  gasnett_set_affinity(location);
#endif
}

void smp_coll_reset_all_flags(smp_coll_t handle) {
  int i;
  BOOTSTRAP_BARRIER(handle, 0);
  for(i=0; i<SMP_COLL_CACHE_LINE; i++) {
    SMP_COLL_SET_FLAG(handle, handle->MYTHREAD, i, 0);    
    SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, i, 0);    
    SMP_COLL_SET_BCAST_FLAG(handle, handle->MYTHREAD, i, 0);    
    gasnett_atomic_set(&handle->atomic_vars[handle->MYTHREAD*SMP_COLL_CACHE_LINE+i], 0, GASNETT_ATOMIC_MB_POST);
    gasnett_atomic_set(&handle->atomic_vars[handle->THREADS*SMP_COLL_CACHE_LINE+handle->MYTHREAD*SMP_COLL_CACHE_LINE+i], 0, GASNETT_ATOMIC_MB_POST);
  }
  BOOTSTRAP_BARRIER(handle, 0);
}

smp_coll_t smp_coll_init(size_t aux_space_per_thread, int flags, int THREADS, int MYTHREAD){

  static uint8_t **allscratch;
  smp_coll_t ret;
  int i;
  ret = (struct smp_coll_t_*) gasneti_malloc(sizeof(struct smp_coll_t_));
  ret->MYTHREAD = MYTHREAD;
  ret->THREADS = THREADS;
  ret->flag_set = 0;
  ret->tempaddrs = (void**) gasneti_malloc(sizeof(void*)*THREADS);
  if(flags & SMP_COLL_SET_AFFINITY) {
    smp_coll_set_affinity(MYTHREAD);
  }
  BOOTSTRAP_BARRIER(ret,0);
  SMP_COLL_CONSTRUCT_BARR_ROUTINES(ret);
#if HAVE_PTHREAD_BARRIER
  {
    int temp;
    static pthread_barrier_t *ptr;
    if(MYTHREAD==0) {
      ptr = (pthread_barrier_t *) gasneti_malloc(sizeof(pthread_barrier_t));
      pthread_barrier_init(ptr, NULL, THREADS);
      BOOTSTRAP_BARRIER(ret,0);
      ret->pthread_barrier = ptr;
    } else {
      BOOTSTRAP_BARRIER(ret,0);
      ret->pthread_barrier = ptr;
    }
    temp = pthread_barrier_wait(ret->pthread_barrier);
    if(temp!=0 && temp!=PTHREAD_BARRIER_SERIAL_THREAD) {
      fprintf(stderr, "barrier error (%d) %d %d\n", temp, EINVAL, PTHREAD_BARRIER_SERIAL_THREAD);
      exit(1);
    }
  }
#endif
#if 0
  SMP_COLL_CONSTRUCT_BCAST_ROUTINES(ret);
  SMP_COLL_CONSTRUCT_SCATTER_ROUTINES(ret);
  SMP_COLL_CONSTRUCT_GATHER_ROUTINES(ret);
  SMP_COLL_CONSTRUCT_ALLRED_INT_ROUTINES(ret);
  SMP_COLL_CONSTRUCT_ALLRED_DOUBLE_ROUTINES(ret);
  SMP_COLL_CONSTRUCT_RED_INT_ROUTINES(ret);
  SMP_COLL_CONSTRUCT_RED_DOUBLE_ROUTINES(ret);
  SMP_COLL_CONSTRUCT_EXCHANGE_ROUTINES(ret);
#endif
  
  BOOTSTRAP_BARRIER(ret,0);
  if(MYTHREAD==0) {
    int t;
    smp_coll_all_flags = (volatile uint32_t*) gasneti_malloc(SMP_COLL_CACHE_LINE*THREADS*sizeof(uint32_t)+SMP_COLL_CACHE_LINE);
    smp_coll_all_barrier_flags = (volatile uint32_t*) gasneti_malloc(SMP_COLL_CACHE_LINE*THREADS*sizeof(uint32_t)+SMP_COLL_CACHE_LINE);
    smp_coll_all_bcast_flags = (volatile uint32_t*) gasneti_malloc(SMP_COLL_CACHE_LINE*THREADS*sizeof(uint32_t)+SMP_COLL_CACHE_LINE);
    atomic_vars = (gasnett_atomic_t*) gasneti_malloc(2*SMP_COLL_CACHE_LINE*THREADS*sizeof(gasnett_atomic_t)+SMP_COLL_CACHE_LINE);
    allscratch = (uint8_t**) gasneti_malloc(sizeof(uint8_t*)*THREADS);
  }
  BOOTSTRAP_BARRIER(ret,0);
  /*gasnett_mmap*/
  ret->aux_space = (uint8_t*) gasneti_malloc(sizeof(uint8_t)*SMP_COLL_AUX_SPACE_SIZE);
  
  allscratch[MYTHREAD] = ret->aux_space;

  BOOTSTRAP_BARRIER(ret,0);
  ret->aux_space_all = gasneti_malloc(sizeof(uint8_t*)*THREADS);
  memcpy(ret->aux_space_all, allscratch, sizeof(uint8_t*)*THREADS);
  ret->flag_set = 0;
  ret->barrier_flag_set = 0;
  ret->flags = (volatile uint32_t*) ALIGNUP(smp_coll_all_flags,SMP_COLL_CACHE_LINE);
  ret->barrier_flags = (volatile uint32_t*) ALIGNUP(smp_coll_all_barrier_flags,SMP_COLL_CACHE_LINE);
  ret->bcast_flags = (volatile uint32_t*) ALIGNUP(smp_coll_all_bcast_flags,SMP_COLL_CACHE_LINE);
  ret->atomic_vars = (gasnett_atomic_t*) ALIGNUP(atomic_vars,SMP_COLL_CACHE_LINE);  
  
  ret->curr_atomic_set = 0;

  smp_coll_reset_all_flags(ret);

  ret->dissem_info=NULL;
  if(!(flags & SMP_COLL_SKIP_TUNE_BARRIERS)) {
    smp_coll_tune_barrier(ret);
  } else {
    smp_coll_set_barrier_routine(ret, 3, 4);
  }
#if 0
  smp_coll_set_broadcast_routine(ret, SMP_COLL_BROADCAST_TREE_FLAG, 2);
  smp_coll_set_all_reduce_int_routine(ret, SMP_COLL_ALL_REDUCE_FLAT, THREADS);
  smp_coll_set_all_reduce_double_routine(ret, SMP_COLL_ALL_REDUCE_FLAT, THREADS);
  smp_coll_set_exchange_routine(ret, SMP_COLL_EXCHANGE_FLAT, THREADS);
#endif
  
  BOOTSTRAP_BARRIER(ret,0);
  smp_coll_reset_all_flags(ret);
  return ret;
}

void smp_coll_safe_barrier(smp_coll_t handle, int flags) {
  int i,j;
  BOOTSTRAP_BARRIER(handle, flags);
  if(handle->MYTHREAD==0){
    for(i=0; i<handle->THREADS; i++) {
      for(j=0; j<SMP_COLL_CACHE_LINE; j++) {
        SMP_COLL_SET_FLAG(handle, i, j, 0);
      }
    }
  }
  BOOTSTRAP_BARRIER(handle, flags);
}





