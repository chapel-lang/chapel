/*   $Source: /var/local/cvs/gasnet/other/smp-collectives/smp_coll_barrier.c,v $
 *     $Date: 2009/10/22 20:24:55 $
 * $Revision: 1.5 $
 * Description: Shared Memory Collectives
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <smp-collectives/smp_coll_internal.h>

#if !(INLINE_ALL_COLLECTIVES)
void smp_coll_barrier(smp_coll_t handle, int flags) {
  (*handle->barr_fns[handle->curr_barrier_routine])(handle, flags);
}
#endif

static void print_barrier_tree(smp_coll_t handle) {
  int i;
  printf("%d> parent: %d numchildren: %d\n", handle->MYTHREAD, handle->barrier_parent, handle->barrier_num_children);
  for(i=0; i<handle->barrier_num_children; i++) {
    printf("%d> child %d: %d\n", handle->MYTHREAD, i, handle->barrier_children[i]);
  }
}

void smp_coll_set_barrier_routine_with_root(smp_coll_t handle, smp_coll_barrier_routine_t routine_id, int in_radix, int root) {
  
  smp_coll_safe_barrier(handle, 0);
  if(handle->dissem_info) smp_coll_free_dissemination(handle->dissem_info);
  handle->dissem_info = smp_coll_build_dissemination(in_radix, handle->MYTHREAD, handle->THREADS);
  
  handle->barrier_root = root;
  handle->barrier_radix = in_radix;
  handle->barrier_log_2_radix = smp_coll_mylogn(in_radix,2);
  handle->barrier_log_radix_THREADS = smp_coll_mylogn(handle->THREADS, in_radix);
  
  
  if(routine_id < SMP_COLL_NUM_BARR_ROUTINES && routine_id >=0) { 
    handle->curr_barrier_routine = routine_id;
  } else {
    if(handle->MYTHREAD==0) fprintf(stderr, "bad barrier routine id: %d\n", routine_id);
    exit(1);
  }
#define ACT2REL(actrank) ( ((actrank) >= (root)) ? (actrank) - (root) : (actrank) \
- (root) + (handle->THREADS) )
#define REL2ACT(relrank) ( (relrank) < (handle->THREADS-root) ? \
(relrank) + (root) : (relrank) + (root) - (handle->THREADS))
  
  {
    int num_digits = handle->barrier_log_radix_THREADS;
    int radixlog2 = handle->barrier_log_2_radix;
    int radix = handle->barrier_radix;
    int i,j,k;
    int child_count=0;
    int myrelrank = ACT2REL(handle->MYTHREAD);
    
    if(myrelrank!=0) {
      i=0; 
      while(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(myrelrank,i,radix,radixlog2)==0) {
        i++;
      }
      handle->barrier_parent = REL2ACT(SMP_COLL_REPLACE_DIGIT_POWER2RADIX(myrelrank,i,0,radix,radixlog2));
    } else {
      handle->barrier_parent = -1;
    }
    
    /* reduce data from all the children*/
    for(i=num_digits-1,j=0; i>=0; i--,j++) {
      /*if my i^th digit is 0 that means that i am a sender for this round*/
      if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(myrelrank, i, radix, radixlog2)==0 && 
         SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(myrelrank, i, radix, radixlog2)==0) {
        for(k=1;k<radix;k++) {
          int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(myrelrank, i, k, radix, radixlog2);
          if(dest<handle->THREADS) {
            child_count++;
          }
        }
      } 
    }
    
    handle->barrier_children = (int*) gasneti_malloc(sizeof(int)*child_count);
    

    
    /*     if(child_count > 0) { */
    
    /*     } else { */
    /*       handle->barrier_children = NULL; */
    /*     } */
    
    handle->barrier_num_children = child_count;
    child_count = 0;
    
    for(i=num_digits-1,j=0; i>=0; i--,j++) {
      /*if my i^th digit is 0 that means that i am a sender for this round*/
      if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(myrelrank, i, radix, radixlog2)==0 && 
         SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(myrelrank, i, radix, radixlog2)==0) {
        for(k=1;k<radix;k++) {
          int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(myrelrank, i, k, radix, radixlog2);
          if(dest<handle->THREADS) {
            handle->barrier_children[child_count] = REL2ACT(dest);
            child_count++;
          }
        }
      } 
    }
    
    /*    print_barrier_tree(handle);  */
    
  }
  
  smp_coll_safe_barrier(handle, 0);
  
}

void smp_coll_set_barrier_routine(smp_coll_t handle, smp_coll_barrier_routine_t routine_id, int in_radix) {
  smp_coll_set_barrier_routine_with_root(handle, routine_id, in_radix, 0);
}

#define VERBOSE_TUNING 0
void smp_coll_tune_barrier(smp_coll_t handle) {
  int iter;
  int i;
  gasnett_tick_t start,stop;
  int radix=2;
  double best_time=1e20;
  double time;
  int barrier_iters=gasneti_getenv_int_withdefault("GASNET_COLL_TUNE_SMP_BARRIER_ITER", 1000, 0);
  int root;
  static int best_barrier_radix = 2;
  static int best_barrier_routine =1;
  static int best_root = 0;

#if VERBOSE_TUNING
  if(handle->MYTHREAD==0) fprintf(stderr, "starting autotuning of local barrier\n");
#endif
  for(root=0; root<1; root++) {

    if(handle->MYTHREAD==0 && VERBOSE_TUNING) fprintf(stderr, "ROOT: %d\n", root);
    for(i=0; i<SMP_COLL_NUM_BARR_ROUTINES; i++) {
      if(i==SMP_COLL_BARRIER_COND_VAR) continue;
      if(handle->MYTHREAD==0 && VERBOSE_TUNING) fprintf(stderr, "\t routine: %d\n",i);
      for(radix=2; radix<=handle->THREADS; radix*=2) {
        
        
        
        if(i==SMP_COLL_BARRIER_COND_VAR && radix>2) continue;
        if(i==SMP_COLL_BARRIER_PTHREAD && radix>2) continue;
        if(handle->MYTHREAD==0 && VERBOSE_TUNING) fprintf(stderr, "\t\t radix: %d\n",radix);
        smp_coll_set_barrier_routine_with_root(handle, i, radix, root);
        
        start = gasnett_ticks_now();
        for(iter = 0; iter<barrier_iters; iter++) {
          smp_coll_barrier(handle, 0);
        }
        stop = gasnett_ticks_now();
        smp_coll_safe_barrier(handle, 0);
        time = ((double) gasnett_ticks_to_ns(stop-start))/barrier_iters;
        
        
        if(time < best_time && handle->MYTHREAD==0) {
          best_barrier_radix=radix;
          best_barrier_routine = i;
          best_time = time;
          best_root = root;
        }
        smp_coll_reset_all_flags(handle);
        /*run a root agnostic barrier*/
        smp_coll_safe_barrier(handle, 0);
      }
    }
  }
#if VERBOSE_TUNING
  if(handle->MYTHREAD==0) fprintf(stderr, "setting best barrier: routine: %d radix: %d root: %d time: %g ns\n", best_barrier_routine, best_barrier_radix, best_root, best_time);
#endif
  smp_coll_set_barrier_routine_with_root(handle, best_barrier_routine, best_barrier_radix, best_root);
  
  
}


void smp_coll_barrier_pthread(smp_coll_t handle, int flags) {
#if HAVE_PTHREAD_BARRIER
  pthread_barrier_wait(handle->pthread_barrier);
#endif
}


/*condition variable based pthread implementation*/
/*all threads except last to arrive at barrier grab a lock and then fall asleep on a condition variable*/
/*last thread grabs the lock and broadcats to all other threads to wake up*/
#if PLATFORM_ARCH_CRAYX1 || PLATFORM_OS_CYGWIN
/* pthread_cond is unreliable on some versions of these OS's - use semaphores */
#include <semaphore.h>
void smp_coll_barrier_cond_var(smp_coll_t handle, int flags){
  static gasneti_mutex_t barrier_mutex = GASNETT_MUTEX_INITIALIZER;
  static volatile int phase = 0;
  static volatile unsigned int barrier_count = 0;
  static sem_t sem[2];
  gasneti_mutex_lock(&barrier_mutex);
  { int myphase = phase;
    static volatile int firsttime = 1;
    if (firsttime) {
      gasneti_assert_zeroret(sem_init(&sem[0], 0, 0));
      gasneti_assert_zeroret(sem_init(&sem[1], 0, 0));
      firsttime = 0;
    }
    barrier_count++;
    if (barrier_count < handle->THREADS) {
      gasneti_mutex_unlock(&barrier_mutex); 
      gasneti_assert_zeroret(sem_wait(&sem[myphase]));
    } else {
      int i;
      barrier_count = 0;
      phase = !phase;
      gasneti_mutex_unlock(&barrier_mutex);
      for (i=0; i < (handle->THREADS-1); i++) {
        gasneti_assert_zeroret(sem_post(&sem[myphase]));
      }
    }
  }
}
#else
void smp_coll_barrier_cond_var(smp_coll_t handle, int flags){
  /* cond variables must be phased on some OS's (HPUX) */
  static struct {
    gasnett_cond_t cond;
    gasnett_mutex_t mutex;
  } barrier[2] = { { GASNETT_COND_INITIALIZER, GASNETT_MUTEX_INITIALIZER },
                   { GASNETT_COND_INITIALIZER, GASNETT_MUTEX_INITIALIZER }};
  static volatile unsigned int barrier_count = 0;
  static volatile int phase = 0;
  const int myphase = phase;
  gasneti_mutex_lock(&(barrier[myphase].mutex));
  barrier_count++;
  if (barrier_count != handle->THREADS) {
    /* CAUTION: changing the "do-while" to a "while" triggers a bug in the SunStudio 2006-08
     * compiler for x86_64.  See http://upc-bugs.lbl.gov/bugzilla/show_bug.cgi?id=1858
     * which includes a link to Sun's own database entry for this issue.
     */
    do {
      gasneti_cond_wait(&(barrier[myphase].cond), &(barrier[myphase].mutex));
    } while (myphase == phase);
  } else {  
    barrier_count = 0;
    phase = !phase;
    gasneti_cond_broadcast(&(barrier[myphase].cond));
  }       
  gasneti_mutex_unlock(&(barrier[myphase].mutex));
}
#endif

/*bruck's dissemination style barrier*/
/*log_r(n) rounds with O(n) messages per round*/
void smp_coll_barrier_dissem_atomic(smp_coll_t handle, int flags) {
  const dissem_info_t *dissem = handle->dissem_info;
  const dissem_vector_t *barrier_order = dissem->barrier_order;
  int i,j;
  double a=2.0;
  gasnett_local_wmb();
  for(i=0; i<dissem->dissemination_phases; i++) {
    if(barrier_order[i].n > 0) {
      const int* elem_list = barrier_order[i].elem_list;
      /*post on the sems*/
      for(j=0; j<barrier_order[i].n; j++) {
        int dest = elem_list[j];
        /*increment counter i on dest by 1*/
        SMP_COLL_INC_ATOMIC(handle, dest, i, handle->curr_atomic_set);
      }
      /*wait for counter i to be barrier_order[i].n*/
      gasneti_waitwhile(SMP_COLL_READ_ATOMIC(handle, handle->MYTHREAD, i, handle->curr_atomic_set)!=barrier_order[i].n);
      SMP_COLL_RESET_ATOMIC(handle, handle->MYTHREAD, i, handle->curr_atomic_set);
    }
  }
  handle->curr_atomic_set = !handle->curr_atomic_set;
  gasnett_local_rmb();

}


/*implements a tree going up and a flat broadcast back down*/
/*this is a push based implementation since each thread signals on the remote thread when it is ready*/
void smp_coll_barrier_tree_push_push(smp_coll_t handle, int flags) {
  int i;
  double a=2.0;
  int flagset = handle->barrier_flag_set;
  int atomicset = handle->curr_atomic_set;
  gasnett_local_wmb();
  
  /*push based tree wait for all children*/
  gasneti_waitwhile(SMP_COLL_READ_ATOMIC(handle, handle->MYTHREAD, 0, atomicset)!=handle->barrier_num_children);
  SMP_COLL_RESET_ATOMIC(handle, handle->MYTHREAD, 0, atomicset);

  /*if i'm not root*/
  if(handle->MYTHREAD!=handle->barrier_root) {
    /*singal parent and wait for parent to signal us*/
    SMP_COLL_INC_ATOMIC(handle, handle->barrier_parent, 0, atomicset);
    gasneti_waitwhile(SMP_COLL_GET_BARRIER_FLAG(handle, handle->MYTHREAD, flagset)==0);
    SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, flagset, 0);
  }
  
  /*signal all my children*/
  for(i=0; i<handle->barrier_num_children; i++) {
   SMP_COLL_SET_BARRIER_FLAG(handle,  handle->barrier_children[i], flagset, 1);
  }

  handle->barrier_flag_set = !handle->barrier_flag_set;
  handle->curr_atomic_set = !handle->curr_atomic_set;
  gasnett_local_rmb();

}

void smp_coll_barrier_tree_push_pull(smp_coll_t handle, int flags) {
  int i;
  double a=2.0;
  int flagset = handle->barrier_flag_set;
  gasnett_local_wmb();
  
  /*push based tree wait for all children*/
  gasneti_waitwhile(SMP_COLL_READ_ATOMIC(handle, handle->MYTHREAD, 0, handle->curr_atomic_set)!=handle->barrier_num_children);
  SMP_COLL_RESET_ATOMIC(handle, handle->MYTHREAD, 0, handle->curr_atomic_set);
  
  /*signal parent and wiat for parent*/
  if(handle->MYTHREAD!=handle->barrier_root) { 
    SMP_COLL_INC_ATOMIC(handle, handle->barrier_parent, 0, handle->curr_atomic_set);
    gasneti_waitwhile(SMP_COLL_GET_BARRIER_FLAG(handle, handle->barrier_parent, flagset)==0);
  }
  
  /*parent has now acked my signal so we can clear the up signal*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, flagset, 0);
  
  /*clear my down flags from previous round*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, (!flagset), 0);
  
  /*set my down flag for this round*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, flagset, 1);
  
  handle->barrier_flag_set = !handle->barrier_flag_set;
  handle->curr_atomic_set = !handle->curr_atomic_set;
  gasnett_local_rmb();
}
#define SPIN() gasnett_spinloop_hint()
void smp_coll_barrier_tree_pull_push(smp_coll_t handle, int flags) {
  int i;
  double a=2.0;
  int flagset = handle->barrier_flag_set;
  gasnett_local_wmb();
  for(i=0; i<handle->barrier_num_children; i++) {
    gasneti_waitwhile(SMP_COLL_GET_BARRIER_FLAG(handle, handle->barrier_children[i], flagset)==0);
  }
  
  /*reset old one*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, !flagset, 0);

  /*set my flag indicating barrier is done*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, flagset, 1);  
  if(handle->MYTHREAD!=handle->barrier_root) {
    /*singal parent and wait for parent to signal us*/
    gasneti_waitwhile(SMP_COLL_GET_BARRIER_FLAG(handle, handle->MYTHREAD, 2+flagset)==0);
    SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, 2+flagset, 0);
  }
  
  /*signal all my children*/
  for(i=0; i<handle->barrier_num_children; i++) {
    SMP_COLL_SET_BARRIER_FLAG(handle,  handle->barrier_children[i], 2+flagset, 1);
  }
  
  handle->barrier_flag_set = !handle->barrier_flag_set;
  gasnett_local_rmb();
}

void smp_coll_barrier_tree_pull_pull(smp_coll_t handle, int flags) {
  int i;
  double a=2.0;
  int flagset = handle->barrier_flag_set;
  gasnett_local_wmb();
  for(i=0; i<handle->barrier_num_children; i++) {
    gasneti_waitwhile(SMP_COLL_GET_BARRIER_FLAG(handle, handle->barrier_children[i], flagset)==0);
  }
  
  /*set my flag indicating barrier is done*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, !flagset, 0);
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, flagset, 1);
    
  /*wait for parent to raise flag*/
  if(handle->MYTHREAD!=handle->barrier_root) {

    gasneti_waitwhile(SMP_COLL_GET_BARRIER_FLAG(handle, handle->barrier_parent, 2+flagset)==0);
  }  

  /*parent has now acked my signal so we can clear the up signal*/

  /*clear my down flags from previous round*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, 2+(!flagset), 0);
  
  /*set my down flag for this round*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, 2+flagset, 1);
  
  handle->barrier_flag_set = !handle->barrier_flag_set;
  gasnett_local_rmb();
}

#if 0
/*implements a tree going up and a flat broadcast back down*/
/*this is pull based since each thread sets its own bit and the root then comes around and polls it*/

void smp_coll_barrier_tree_flag(smp_coll_t handle, int flags) {
  int idx = 0;
  int num_digits = handle->barrier_log_radix_THREADS;
  int radixlog2 = handle->barrier_log_2_radix;
  int radix = handle->barrier_radix;
  int i,j,k;
  int parent=-1;
  double a=2.0;
  
  gasnett_local_wmb();

  /* reduce data from all the children*/
  for(i=num_digits-1,j=0; i>=0; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
          /*wait for dest to be ready before we send*/
          gasneti_waitwhile(SMP_COLL_GET_BARRIER_FLAG(handle, dest, 0)==0);
        }
      }
    } 
  }
  
  /*set the flag indicating that my data (and all the data under my subtree is ready)*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, 0, 1);
  
  if(handle->MYTHREAD!=barrier_root) {
    /*Wait for parent to signal that my data for the entire tree has arrived*/
    gasneti_waitwhile(SMP_COLL_GET_BARRIER_FLAG(handle, handle->MYTHREAD, 0)!=0);
  }

  /*broadcast the data back down my subtree*/
  for(i=num_digits-1,j=0; i>=0; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
	  /*write memory barrier to ensure data is transfered before we set the flag*/

          SMP_COLL_SET_BARRIER_FLAG(handle, dest, 0, 0);
        }
      }
    } 
  }
  gasnett_local_rmb();

}

void smp_coll_barrier_flag_tree_up_flat_down(smp_coll_t handle, int flags) {
  int idx = 0;
  int num_digits = handle->barrier_log_radix_THREADS;
  int radixlog2 = handle->barrier_log_2_radix;
  int radix = handle->barrier_radix;
  int i,j,k;
  int parent=-1;
  double a=2.0;
  static volatile uint32_t barrier_done[2] = {0,0};
  static volatile uint32_t barrier_phase = 0;
  int myphase;

  
  myphase = barrier_phase;
  /* reduce data from all the children*/
  for(i=num_digits-1,j=0; i>=0; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
          /*wait for dest to be ready before we send*/
          gasneti_while(SMP_COLL_GET_BARRIER_FLAG(handle, dest,  myphase)==0);
        }
      }
    } 
  }


  /*set the flag indicating that my data (and all the data under my subtree is ready)*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD,  myphase, 1);
  
  if(handle->MYTHREAD!=barrier_root) {
    /*wait for barrier to raise the flag indicating that this barrier round is done.*/
    gasneti_while(barrier_done[myphase] != 1);
  } else {
    barrier_phase = !barrier_phase;
    barrier_done[!myphase] = 0; /*reset for next time*/

    barrier_done[myphase] = 1;
  }
  
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, myphase, 0);


}


void smp_coll_barrier_flag_tree_up_tree_down(smp_coll_t handle, int flags) {
  int idx = 0;
  int num_digits = handle->barrier_log_radix_THREADS;
  int radixlog2 = handle->barrier_log_2_radix;
  int radix = handle->barrier_radix;
  int i,j,k;
  double a=2.0;

  int flagset = handle->barrier_flag_set;
  int parent = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, 0, 0, radix, radixlog2);
  
  /* reduce data from all the children*/
  for(i=num_digits-1,j=0; i>=0; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
          /*wait for dest to be ready before we send*/
          gasneti_waitwhile(SMP_COLL_GET_BARRIER_FLAG(handle, dest, flagset)==0);
        }
      }
    } 
  }
  
  
  /*set the flag indicating that my data (and all the data under my subtree is ready)*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, flagset, 1);

  /*wait for parent to raise flag*/
  if(handle->MYTHREAD!=barrier_root) {
    gasneti_waitwhile(SMP_COLL_GET_BARRIER_FLAG(handle, parent, 2+flagset)==0);

  }  
  /*parent has now acked my signal so we can clear the up signal*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, flagset, 0);
  
  /*clear my down flags from previous round*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, 2+(!flagset), 0);

  /*set my down flag for this round*/
  SMP_COLL_SET_BARRIER_FLAG(handle, handle->MYTHREAD, 2+flagset, 1);

  handle->barrier_flag_set = !handle->barrier_flag_set;


  
}
#endif
