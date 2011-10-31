/*   $Source: /var/local/cvs/gasnet/other/smp-collectives/smp_coll_reductions.cc,v $
 *     $Date: 2009/10/22 20:24:55 $
 * $Revision: 1.4 $
 * Description: Shared Memory Collectives
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if PREFETCH
#include "/opt/SUNWspro/prod/include/cc/sun_prefetch.h"
#define PREFETCH_READ(address) __sparc_prefetch_read_once_intrinsic((address))
#define PREFETCH_WRITE(address) __sparc_prefetch_write_once_intrinsic((address))
#define PREFETCH_DIST 8
#define APPLY_OP(OP, FNPTR, C, A, B, LEN) do {\
  int i;\
  if(OP==SMP_COLL_ADD) {\
    for(i=0; i<(LEN); i++) {\
      PREFETCH_WRITE(&((C)[i+PREFETCH_DIST]));\
      PREFETCH_READ(&((A)[i+PREFETCH_DIST]));\
      PREFETCH_READ(&((B)[i+PREFETCH_DIST]));\
      (C)[i] = (A)[i]+(B)[i];\
    }\
  } else if (OP==SMP_COLL_MIN){\
    for(i=0; i<(LEN); i++) {(C)[i] = MIN((A)[i],(B)[i]);} \
  } else if (OP==SMP_COLL_MAX){\
    for(i=0; i<(LEN); i++) {(C)[i] = MAX((A)[i],(B)[i]);}\
  }\
} while(0)

#else
#define APPLY_OP(OP, FNPTR, C, A, B, LEN) do {\
  int i;\
  switch(OP) {\
    case SMP_COLL_ADD: for(i=0; i<(LEN); i++) {(C)[i] = (A)[i]+(B)[i];} break;\
    case SMP_COLL_MIN: for(i=0; i<(LEN); i++) {(C)[i] = MIN((A)[i],(B)[i]);} break;\
    case SMP_COLL_MAX: for(i=0; i<(LEN); i++) {(C)[i] = MAX((A)[i],(B)[i]);} break;\
  }\
} while(0)

#endif

extern "C" {
#include <smp_coll_internal.h>
}

void smp_coll_set_all_reduce_int_routine(smp_coll_t handle, smp_coll_all_reduce_routine_t routine_id, int in_radix) {
  smp_coll_safe_barrier(handle, 0);
  if(!POWEROFTWO(in_radix)) {
    
    if(handle->MYTHREAD==0) fprintf(stderr, "broadcast and reductions do not yet support non-power 2 radices\n");
    exit(1);
  }
  if(routine_id==SMP_COLL_ALL_REDUCE_DISSEMK_ATOMIC && !POWEROFTWO(handle->THREADS)) {
    if(handle->MYTHREAD==0) fprintf(stderr, "dissem-k reduction requires power of 2 thread count\n");
    exit(1);
  }
  if(routine_id==SMP_COLL_ALL_REDUCE_DISSEM2_FLAG && !POWEROFTWO(handle->THREADS)) {
    if(handle->MYTHREAD==0) fprintf(stderr, "dissem-2 reduction requires power of 2 thread count\n");
    exit(1);
  }
  
  if(routine_id==SMP_COLL_ALL_REDUCE_DISSEM2_FLAG && in_radix!=2) {
    if(handle->MYTHREAD==0) fprintf(stderr, "dissem-2 reduction requires radix to be 2\n");
    exit(1);
  }
  
  handle->allred_int_radix = in_radix;
  handle->allred_int_log_2_radix = smp_coll_mylogn(in_radix,2);
  handle->allred_int_log_radix_THREADS = smp_coll_mylogn(handle->THREADS, in_radix);
  handle->allred_int_log_2_THREADS = smp_coll_mylogn(handle->THREADS, 2);
  handle->curr_allred_int_routine = routine_id;
  smp_coll_safe_barrier(handle, 0);
}

void smp_coll_set_all_reduce_double_routine(smp_coll_t handle, smp_coll_all_reduce_routine_t routine_id, int in_radix) {
  smp_coll_safe_barrier(handle, 0);
  if(!POWEROFTWO(in_radix)) {
    
    if(handle->MYTHREAD==0) fprintf(stderr, "broadcast and reductions do not yet support non-power 2 radices\n");
    exit(1);
  }
  if(routine_id==SMP_COLL_ALL_REDUCE_DISSEMK_ATOMIC && !POWEROFTWO(handle->THREADS)) {
    if(handle->MYTHREAD==0) fprintf(stderr, "dissem-k reduction requires power of 2 thread count\n");
    exit(1);
  }
  if(routine_id==SMP_COLL_ALL_REDUCE_DISSEM2_FLAG && !POWEROFTWO(handle->THREADS)) {
    if(handle->MYTHREAD==0) fprintf(stderr, "dissem-2 reduction requires power of 2 thread count\n");
    exit(1);
  }
  
  if(routine_id==SMP_COLL_ALL_REDUCE_DISSEM2_FLAG && in_radix!=2) {
    if(handle->MYTHREAD==0) fprintf(stderr, "dissem-2 reduction requires radix to be 2\n");
    exit(1);
  }
  
  handle->allred_double_radix = in_radix;
  handle->allred_double_log_2_radix = smp_coll_mylogn(in_radix,2);
  handle->allred_double_log_radix_THREADS = smp_coll_mylogn(handle->THREADS, in_radix);
  handle->allred_double_log_2_THREADS = smp_coll_mylogn(handle->THREADS, 2);
  handle->curr_allred_double_routine = routine_id;
  smp_coll_safe_barrier(handle, 0);
}

void smp_coll_set_reduce_int_routine(smp_coll_t handle, smp_coll_reduce_routine_t routine_id, int in_radix) {
  smp_coll_safe_barrier(handle, 0);
  if(!POWEROFTWO(in_radix)) {
    
    if(handle->MYTHREAD==0) fprintf(stderr, "broadcast and reductions do not yet support non-power 2 radices\n");
    exit(1);
  }
  
  
  handle->red_int_radix = in_radix;
  handle->red_int_log_2_radix = smp_coll_mylogn(in_radix,2);
  handle->red_int_log_radix_THREADS = smp_coll_mylogn(handle->THREADS, in_radix);
  handle->red_int_log_2_THREADS = smp_coll_mylogn(handle->THREADS, 2);
  handle->curr_red_int_routine = routine_id;
  smp_coll_safe_barrier(handle, 0);
}

void smp_coll_set_reduce_double_routine(smp_coll_t handle, smp_coll_reduce_routine_t routine_id, int in_radix) {
  smp_coll_safe_barrier(handle, 0);
  if(!POWEROFTWO(in_radix)) {
    
    if(handle->MYTHREAD==0) fprintf(stderr, "broadcast and reductions do not yet support non-power 2 radices\n");
    exit(1);
  }
  
  handle->red_double_radix = in_radix;
  handle->red_double_log_2_radix = smp_coll_mylogn(in_radix,2);
  handle->red_double_log_radix_THREADS = smp_coll_mylogn(handle->THREADS, in_radix);
  handle->red_double_log_2_THREADS = smp_coll_mylogn(handle->THREADS, 2);
  handle->curr_red_double_routine = routine_id;
  smp_coll_safe_barrier(handle, 0);
}
#if !(INLINE_ALL_COLLECTIVES)
void smp_coll_all_reduce_int(smp_coll_t handle, int num_addrs,  
                             int *const dstlist[], int *const srclist[], 
                             size_t n_ints, smp_coll_red_op_t op, RED_FN_INT fnptr, int flags){
  (*handle->allred_int_fns[handle->curr_allred_int_routine])(handle, num_addrs, dstlist, srclist, 
                                                             n_ints, op, fnptr, flags);
}
#endif

#if !(INLINE_ALL_COLLECTIVES)
void smp_coll_all_reduce_double(smp_coll_t handle, int num_addrs,  
                                double *const dstlist[], double *const srclist[], 
                                size_t nelem, smp_coll_red_op_t op, RED_FN_DBL fnptr, int flags){
  (*handle->allred_double_fns[handle->curr_allred_double_routine])(handle, num_addrs, dstlist, srclist, 
                                                                   nelem, op, fnptr, flags);
}
#endif

#if !(INLINE_ALL_COLLECTIVES)
void smp_coll_reduce_int(smp_coll_t handle, int num_addrs,  
                         int *dst, int root, int *const srclist[], 
                         size_t n_ints, smp_coll_red_op_t op, RED_FN_INT fnptr, int flags){
  (*handle->red_int_fns[handle->curr_red_int_routine])(handle, num_addrs, dst, root, srclist, 
                                                       n_ints, op, fnptr, flags);
}
#endif

#if !(INLINE_ALL_COLLECTIVES)
void smp_coll_reduce_double(smp_coll_t handle, int num_addrs,  
                            double *dst, int root, double *const srclist[], 
                            size_t n_doubles, smp_coll_red_op_t op, RED_FN_DBL fnptr, int flags){
  (*handle->red_double_fns[handle->curr_red_double_routine])(handle, num_addrs, dst, root, srclist, 
                                                             n_doubles, op, fnptr, flags);
}
#endif

template<class T> 
static inline void smp_coll_all_reduce_flat_generic(smp_coll_t handle, int num_addrs,  
						    T *const dstlist[], T *const srclist[], 
						    size_t nelem, smp_coll_red_op_t op, 
						    int flags, int num_digits, int radixlog2, int radix){
  int i;
  
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags);
  memcpy(dstlist[handle->MYTHREAD], srclist[handle->MYTHREAD], nelem*sizeof(T));
  for(i=1; i<num_addrs; i++) {
    int src_proc = (i+handle->MYTHREAD) % handle->THREADS;
    APPLY_OP(op, NULL, dstlist[handle->MYTHREAD], dstlist[handle->MYTHREAD], srclist[src_proc], nelem);
  }
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags);

} 

template<class T>
static inline void smp_coll_all_reduce_tree_flag_generic(smp_coll_t handle, int num_addrs,  
                             T *const dstlist[], T *const srclist[], 
                             size_t nelem, smp_coll_red_op_t op,  int flags, int num_digits, int radixlog2, int radix){
  
  int idx = 0;
  int i,j,k;
  int parent=-1;
  double a=2.0;
  
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags);
  
  
  memcpy(dstlist[handle->MYTHREAD], srclist[handle->MYTHREAD], nelem*sizeof(T));
 
  
  /* reduce data from all the children*/
  for(i=num_digits-1,j=0; i>=0; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
          /*wait for dest to be ready before we send*/
          while(SMP_COLL_GET_FLAG(handle, dest, 0)==0){a=1.0/a;}
          gasnett_local_rmb();
          APPLY_OP(op, NULL, dstlist[handle->MYTHREAD], dstlist[handle->MYTHREAD], dstlist[dest], nelem); 
        }
      }
    } 
  }
  
  /*set the flag indicating that my data (and all the data under my subtree is ready)*/
  SMP_COLL_SET_FLAG(handle, handle->MYTHREAD, 0, 1);
  
  if(handle->MYTHREAD!=0) {
    /*Wait for parent to signal that my data for the entire tree has arrived*/
    while(SMP_COLL_GET_FLAG(handle, handle->MYTHREAD, 0)!=0){a=1.0/a;}
    /*read memory barrier to ensure that the data has been put*/
    gasnett_local_rmb();
  }

  /*broadcast the data back down my subtree*/
  for(i=num_digits-1,j=0; i>=0; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
          
          /*wait for dest to be ready before we send*/
          memcpy(dstlist[dest], dstlist[handle->MYTHREAD], nelem*sizeof(T)); 
          /*write memory barrier to ensure data is transfered before we set the flag*/
          gasnett_local_wmb();
          SMP_COLL_SET_FLAG(handle, dest, 0, 0);
        }
      }
    } 
  } 
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags); 
}

template<class T>
static inline void smp_coll_all_reduce_flag_tree_up_flat_down_generic(smp_coll_t handle, int num_addrs,  
                                                         T *const dstlist[], T *const srclist[], 
                                                         size_t nelem, smp_coll_red_op_t op,  int flags, int num_digits, int radixlog2, int radix){
  
  int idx = 0;
  int i,j,k;
  int parent=-1;
  double a=2.0;
  
  static volatile uint32_t allred_done[2] = {0,0};
  static volatile uint32_t allred_phase = 0;
  int myphase;
  
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags);
  
  
  memcpy(dstlist[handle->MYTHREAD], srclist[handle->MYTHREAD], nelem*sizeof(T));
  
  myphase = allred_phase;
  /* reduce data from all the children*/
  for(i=num_digits-1,j=0; i>=0; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
          /*wait for dest to be ready before we send*/
          while(SMP_COLL_GET_FLAG(handle, dest, myphase)==0){a=1.0/a;}
          gasnett_local_rmb();
          APPLY_OP(op, NULL, dstlist[handle->MYTHREAD], dstlist[handle->MYTHREAD], dstlist[dest], nelem); 
        }
      }
    } 
  }
  
  /*set the flag indicating that my data (and all the data under my subtree is ready)*/
  SMP_COLL_SET_FLAG(handle, handle->MYTHREAD, myphase, 1);
  
  if(handle->MYTHREAD!=0) {
    /*wait for barrier to raise the flag indicating that this allredyce us done round is done.*/
    while(allred_done[myphase] != 1){a=1.0/a;}
    gasnett_local_rmb();
  } else {
    allred_phase = !allred_phase;
    allred_done[!myphase] = 0; /*reset for next time*/
    gasnett_local_wmb();
    allred_done[myphase] = 1;
  }
  
  SMP_COLL_SET_FLAG(handle, handle->MYTHREAD, myphase, 0);
  /*copy the data from the root*/
  memcpy(dstlist[handle->MYTHREAD], dstlist[0], nelem*sizeof(T));
  
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags); 
}


template<class T>
static inline void smp_coll_all_reduce_flag_tree_up_tree_down_generic(smp_coll_t handle, int num_addrs,  
                                                                      T *const dstlist[], T *const srclist[], 
                                                                      size_t nelem, smp_coll_red_op_t op,  int flags, int num_digits, int radixlog2, int radix){
  
  int idx = 0;
  int i,j,k;
  double a=2.0;
  int myphase = 0;
  
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags);
  
  SMP_COLL_SET_BCAST_FLAG(handle, handle->MYTHREAD, 0, 1);
  
  memcpy(dstlist[handle->MYTHREAD], srclist[handle->MYTHREAD], nelem*sizeof(T));
  
  /* reduce data from all the children*/
  for(i=num_digits-1,j=0; i>=0; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
          /*wait for dest to be ready before we send*/
          while(SMP_COLL_GET_FLAG(handle, dest, myphase)==0){a=1.0/a;}
          gasnett_local_rmb();
          APPLY_OP(op, NULL, dstlist[handle->MYTHREAD], dstlist[handle->MYTHREAD], dstlist[dest], nelem); 
        }
      }
    } 
  }
  
  /*set the flag indicating that my data (and all the data under my subtree is ready)*/
  SMP_COLL_SET_FLAG(handle, handle->MYTHREAD, myphase, 1);
  
  /*take part ina  broadcast*/
  /*they then wait for the parent to come around and reset their flag back to 0 indicating the data has also arrived*/
  if(handle->MYTHREAD!=0) {
    while(SMP_COLL_GET_BCAST_FLAG(handle, handle->MYTHREAD,0)!=0){a=1.0/a;}
    /*read memory barrier to ensure data is transfered before we use it*/
    gasnett_local_rmb();
  } 
  
  /* broadcast down the tree except to the last level*/
  for(i=num_digits-1,j=0; i>=1; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
          memcpy(dstlist[dest], dstlist[handle->MYTHREAD], nelem*sizeof(T)); 
          /*write memory barrier to ensure data is transfered before we set the flag*/
          gasnett_local_wmb();
          SMP_COLL_SET_BCAST_FLAG(handle, dest, 0, 0);
        }
      }
    } 
    
  } 
  /* all leaf nodes perform a final get of the data*/
  /* i am parent of a leaf node*/
  if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, 0, radix, radixlog2)==0) {
    for(k=1;k<radix;k++) {
      int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, 0, k, radix, radixlog2);
      if(dest<handle->THREADS) {
        /*write memory barrier to ensure data is transfered before we set the flag*/
        gasnett_local_wmb();
        SMP_COLL_SET_BCAST_FLAG(handle, dest, 0, 0);
      }
    }
    /* wait for all children to finish reads*/
    for(k=1; k<radix; k++) {
      int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, 0, k, radix, radixlog2);
      if(dest<handle->THREADS) {
        while(SMP_COLL_GET_BCAST_FLAG(handle, dest, 1)==0){a=1.0/a;}
        SMP_COLL_SET_BCAST_FLAG(handle, dest, 1, 0);
      }
    }
  } else { /*i am a leaf*/
    int parent = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, 0, 0, radix, radixlog2);
    /*wait for the parent to set the flag to 0 indicating the data is ready to go*/
    gasnett_local_rmb();
    memcpy(dstlist[handle->MYTHREAD], dstlist[parent], nelem*sizeof(T));
    /*raise a second flag indicating that my data read is done*/
    gasnett_local_wmb();
    SMP_COLL_SET_BCAST_FLAG(handle, handle->MYTHREAD, 1, 1);
  }
  
  SMP_COLL_SET_FLAG(handle, handle->MYTHREAD, myphase, 0);
  
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags); 
}



template <class T>
static inline void smp_coll_all_reduce_dissemk_atomic_generic(smp_coll_t handle, int num_addrs,  
							      T *const dstlist[], T *const srclist[], 
							      size_t nelem, smp_coll_red_op_t op,  int flags, int num_digits, int radixlog2, int radix) {
  

  int i,j;
  int distance=1;
  double a=2.0;
  
  if(nelem*sizeof(T) > SMP_COLL_AUX_SPACE_SIZE) {
    fprintf(stderr, "%d> REDUCTION TOO LARGE\n", handle->MYTHREAD);
    exit(1);
  }
  
  memcpy(dstlist[handle->MYTHREAD], srclist[handle->MYTHREAD], nelem*sizeof(T));
  for(i=0; i<num_digits; i++) {
    int expected =0;
    gasnett_local_wmb();
    /*signal all peers in this round that my data is ready for reading*/
    for(j=1; j<radix; j++) {
      int my_id = SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2);
      int src_proc = SMP_COLL_REPLACE_DIGIT_POWER2RADIX(handle->MYTHREAD, i, (my_id+j) % radix, radix, radixlog2);
      if(src_proc < handle->THREADS) {
        SMP_COLL_INC_ATOMIC(handle, src_proc, 2*i, handle->curr_atomic_set);
        expected++;
      }
    }
    
    /*wait for all peers in this round to say that it is ok to read*/
    while(SMP_COLL_READ_ATOMIC(handle, handle->MYTHREAD, 2*i, handle->curr_atomic_set)!=expected) {a=1.0/a;}
    SMP_COLL_RESET_ATOMIC(handle, handle->MYTHREAD, 2*i, handle->curr_atomic_set);

    /*read the data and apply the reduction to all peers in this round*/
    for(j=1; j<radix; j++) {
      int my_id = SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2);
      int src_proc = SMP_COLL_REPLACE_DIGIT_POWER2RADIX(handle->MYTHREAD, i, (my_id+j) % radix, radix, radixlog2);
      if(src_proc < handle->THREADS) {
      gasnett_local_rmb();
      
      if(j==1) 
        APPLY_OP(op, NULL, (T*)(handle->aux_space), dstlist[handle->MYTHREAD], dstlist[src_proc], nelem);
      else 
        APPLY_OP(op, NULL, (T*)(handle->aux_space), (T*)(handle->aux_space), dstlist[src_proc], nelem);
      
      /*tell the peer that we have finished our read of the remote data*/
      SMP_COLL_INC_ATOMIC(handle, src_proc, 2*i+1, handle->curr_atomic_set);
      }
    }
    
    /*wait for all peers to say that they have finished reading*/
    while(SMP_COLL_READ_ATOMIC(handle, handle->MYTHREAD, 2*i+1, handle->curr_atomic_set)!=expected) {a=1.0/a;}
    SMP_COLL_RESET_ATOMIC(handle, handle->MYTHREAD, 2*i+1, handle->curr_atomic_set);
    memcpy(dstlist[handle->MYTHREAD], handle->aux_space, sizeof(T)*nelem);
    
  }  
  handle->curr_atomic_set !=  handle->curr_atomic_set;
}

template <class T>
void smp_coll_all_reduce_dissem2_flag_generic(smp_coll_t handle, int num_addrs,  
					      T *const dstlist[], T *const srclist[], 
					      size_t nelem, smp_coll_red_op_t op, 
					      int flags, int num_digits, int radixlog2, int radix) {  
  
  int i,j;
  int distance=1;
  double a=2.0;

  if(nelem*sizeof(T) > SMP_COLL_AUX_SPACE_SIZE) {
    fprintf(stderr, "%d> REDUCTION TOO LARGE\n", handle->MYTHREAD);
    exit(1);
  }
  if(radix!=2) {
    fprintf(stderr, "%d> dissem2 reduction can only be called with radix of 2\n", handle->MYTHREAD);
    exit(1);
  }
  
  
  memcpy(dstlist[handle->MYTHREAD], srclist[handle->MYTHREAD], nelem*sizeof(T));
  for(i=0; i<num_digits; i++) {
    
    gasnett_local_wmb();
    int my_id = SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, 2, 1);
    int peer_proc = SMP_COLL_REPLACE_DIGIT_POWER2RADIX(handle->MYTHREAD, i, !my_id, 2, 1);
    /*signal all peers in this round that my data is ready for reading*/
    SMP_COLL_SET_FLAG(handle, handle->MYTHREAD, i, 1);
    while(SMP_COLL_GET_FLAG(handle, peer_proc, i)==0) {a=1.0/a;}
    /*read the data and apply the reduction to all peers in this round*/
    gasnett_local_rmb();
    APPLY_OP(op, NULL, (T*)(handle->aux_space), dstlist[handle->MYTHREAD], dstlist[peer_proc], nelem);
      
    /*tell the peer that we have finished our read of the remote data*/
    SMP_COLL_SET_FLAG(handle, peer_proc, i, 0);
  
    /*wait for hte signal that peer has finished reading the data*/
    while(SMP_COLL_GET_FLAG(handle, handle->MYTHREAD, i)==1) {a=1.0/a;}
    
    memcpy(dstlist[handle->MYTHREAD], handle->aux_space, sizeof(T)*nelem);
  }  
}

template<class T> 
static inline void smp_coll_reduce_flat_generic(smp_coll_t handle, int num_addrs,  
						T *dst, int root, T *const srclist[], 
						size_t nelem, smp_coll_red_op_t op, 
						int flags, int num_digits, int radixlog2, int radix){
  int i;


  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags);
  if(handle->MYTHREAD == 0) {
    memcpy(dst, srclist[0], nelem*sizeof(T));
    for(i=1; i<num_addrs; i++) {
      int src_proc = i;
      APPLY_OP(op, NULL, dst, dst, srclist[src_proc], nelem);
    }
  }
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags);

} 

#define SPIN() gasnett_spinloop_hint()
template<class T>
static inline void smp_coll_reduce_chain_flag_generic(smp_coll_t handle, int num_addrs,  
						     T *dst, int root, T *const srclist[], 
						     size_t nelem, smp_coll_red_op_t op,  int flags, int num_digits, int radixlog2, int radix){
 
  int parent= (handle->MYTHREAD == 0 ? -1 : handle->MYTHREAD-1);
  
  
  if(handle->MYTHREAD < handle->THREADS-2) {
    /*child's data is in aux space*/
    
    while(SMP_COLL_GET_FLAG(handle, handle->MYTHREAD+1, 0)==0){SPIN();}
    if(handle->MYTHREAD == 0) {
      APPLY_OP(op, NULL, dst, srclist[handle->MYTHREAD], (T*) handle->aux_space_all[handle->MYTHREAD+1], nelem); 
    } else {
      APPLY_OP(op, NULL, (T*) handle->aux_space, srclist[handle->MYTHREAD], (T*) handle->aux_space_all[handle->MYTHREAD+1], nelem); 
    }
    gasnett_local_mb();
    /*once we have read the data set the flag*/
    SMP_COLL_SET_FLAG(handle, handle->MYTHREAD+1, 0, 0);
  } else if(handle->MYTHREAD==handle->THREADS-2) {
    
    while(SMP_COLL_GET_FLAG(handle, handle->MYTHREAD+1, 0)==0){SPIN();}
    APPLY_OP(op, NULL, (T*) handle->aux_space, srclist[handle->MYTHREAD], (T*) srclist[handle->MYTHREAD+1], nelem); 
    gasnett_local_mb();
    /*once we have read the data set the flag*/
    SMP_COLL_SET_FLAG(handle, handle->MYTHREAD+1, 0, 0);
  } else {
    /*do nothing*/
  }
  
 
  /*raise my flag saying the data is ready to read*/
  SMP_COLL_SET_FLAG(handle, handle->MYTHREAD, 0, 1);
  
  if(handle->MYTHREAD!=0) {
    
    /*wait for the parent to signal that my data has been read and i can exit*/
    while(SMP_COLL_GET_FLAG(handle, handle->MYTHREAD, 0)!=0){SPIN();}
    
  }
  
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags);
}

template<class T>
static inline void smp_coll_reduce_tree_flag_generic(smp_coll_t handle, int num_addrs,  
						     T *dst, int root, T *const srclist[], 
						     size_t nelem, smp_coll_red_op_t op,  int flags, int num_digits, int radixlog2, int radix){
  
  int idx = 0;
  int i,j,k;
  int parent=-1;
  double a=2.0;
  int first=1;
  
  if(0 && (flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags);
   
  memcpy(handle->aux_space, srclist[handle->MYTHREAD], nelem*sizeof(T));
  
  /* reduce data from all the children*/
  for(i=num_digits-1,j=0; i>=0; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
          /*wait for dest to be ready before we send*/
          while(SMP_COLL_GET_FLAG(handle, dest, 0)==0){a=1.0/a;}
          gasnett_local_rmb();
	  if(handle->MYTHREAD==0) {
	    if(first==1) {
	      APPLY_OP(op, NULL, dst, (T*) handle->aux_space, (T*) handle->aux_space_all[dest], nelem); 
	      first=0;
	    } else {
	      APPLY_OP(op, NULL, dst, dst, (T*) handle->aux_space_all[dest], nelem); 
	    }
	  } else {
	    APPLY_OP(op, NULL, (T*) handle->aux_space, (T*) handle->aux_space,
		     (T*) handle->aux_space_all[dest], nelem); 
	  }
	  
	  if(!(flags & SMP_COLL_ALL_SYNC)) {
	    /*if it is not an all sync then tell teh child the value has been accumulated and
	      it can be done*/
	    SMP_COLL_SET_FLAG(handle, dest, 0, 0);
	  }
        }
      }
    } 
  }
  
  /*set the flag indicating that my data (and all the data under my subtree is ready)*/
  SMP_COLL_SET_FLAG(handle, handle->MYTHREAD, 0, 1);
  
  if(handle->MYTHREAD!=0) {
    /*wait for the parent to signal that my data has been read and i can exit*/
    while(SMP_COLL_GET_FLAG(handle, handle->MYTHREAD, 0)!=0){a=1.0/a;}
  }
  
  
  if((flags & SMP_COLL_ALL_SYNC)) {
    /*broadcast clear signal along same tree back to all children*/
    for(i=num_digits-1,j=0; i>=0; i--,j++) {
      /*if my i^th digit is 0 that means that i am a sender for this round*/
      if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
	 SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
	for(k=1;k<radix;k++) {
	  int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
	  if(dest<handle->THREADS) {
	    SMP_COLL_SET_FLAG(handle, dest, 0, 0);
	  }
	}
      } 
    }
  }

  //  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags); 
}


/**** IDEA FOR NEW ALGORITHM*******/
/*** create a hybrid/tree dissemination approach ***/
/** leaves of tree report to one parent***/
/** ALL NON PARENTS participate in dissem2 reduction**/
/***********************************/

extern "C" {
#define CONSTRUCT_ALLRED_ROUTINES(TYPE, NUM_DIGITS, RADIXLOG2, RADIX)\
  void smp_coll_all_reduce_flat_ ## TYPE (smp_coll_t handle, int num_addrs,\
                                    TYPE *const dstlist[], TYPE *const srclist[],\
                                    size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags) {\
    smp_coll_all_reduce_flat_generic<TYPE>(handle, num_addrs, dstlist, srclist, nelem, op, flags, (NUM_DIGITS), (RADIXLOG2), (RADIX));\
    return;\
  }\
  void smp_coll_all_reduce_tree_flag_ ## TYPE (smp_coll_t handle, int num_addrs,\
                                    TYPE *const dstlist[], TYPE *const srclist[],\
                                    size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags) {\
    smp_coll_all_reduce_tree_flag_generic<TYPE>(handle, num_addrs, dstlist, srclist, nelem, op, flags,  (NUM_DIGITS), (RADIXLOG2), (RADIX));\
    return;\
  }\
  void smp_coll_all_reduce_flag_tree_up_flat_down_ ## TYPE (smp_coll_t handle, int num_addrs,\
                                               TYPE  *const dstlist[], TYPE *const srclist[],\
                                               size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags) {\
    smp_coll_all_reduce_flag_tree_up_flat_down_generic<TYPE>(handle, num_addrs, dstlist, srclist, nelem, op, flags,  (NUM_DIGITS), (RADIXLOG2), (RADIX));\
    return;\
  }\
  void smp_coll_all_reduce_flag_tree_up_tree_down_ ## TYPE (smp_coll_t handle, int num_addrs,\
                                                            TYPE  *const dstlist[], TYPE *const srclist[],\
                                                            size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags) {\
    smp_coll_all_reduce_flag_tree_up_flat_down_generic<TYPE>(handle, num_addrs, dstlist, srclist, nelem, op, flags,  (NUM_DIGITS), (RADIXLOG2), (RADIX));\
    return;\
  }\
  void smp_coll_all_reduce_dissemk_atomic_ ## TYPE(smp_coll_t handle, int num_addrs,\
                                              TYPE *const dstlist[], TYPE *const srclist[],\
                                              size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags) {\
    smp_coll_all_reduce_dissemk_atomic_generic<TYPE>(handle, num_addrs, dstlist, srclist, nelem, op, flags,  (NUM_DIGITS), (RADIXLOG2), (RADIX));\
    return;\
  }\
  void smp_coll_all_reduce_dissem2_flag_ ## TYPE(smp_coll_t handle, int num_addrs,\
                                              TYPE *const dstlist[], TYPE *const srclist[],\
                                              size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags) {\
    smp_coll_all_reduce_dissem2_flag_generic<TYPE>(handle, num_addrs, dstlist, srclist, nelem, op, flags,  (NUM_DIGITS), (RADIXLOG2), (RADIX));\
    return;\
  }\

  
  CONSTRUCT_ALLRED_ROUTINES(int,  handle->allred_int_log_radix_THREADS, handle->allred_int_log_2_radix, handle->allred_int_radix);
  CONSTRUCT_ALLRED_ROUTINES(double,  handle->allred_double_log_radix_THREADS, handle->allred_double_log_2_radix, handle->allred_double_radix);
  
#define CONSTRUCT_RED_ROUTINES(TYPE, NUM_DIGITS, RADIXLOG2, RADIX)\
  void smp_coll_reduce_flat_ ## TYPE (smp_coll_t handle, int num_addrs, \
					  TYPE *dst, int root, TYPE *const srclist[], \
                                    size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags) {\
    smp_coll_reduce_flat_generic<TYPE>(handle, num_addrs, dst, root, srclist, nelem, op, flags, (NUM_DIGITS), (RADIXLOG2), (RADIX));	\
    return;\
  }\
  void smp_coll_reduce_tree_flag_ ## TYPE (smp_coll_t handle, int num_addrs,\
					   TYPE *dst, int root, TYPE *const srclist[], \
					   size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags) { \
    smp_coll_reduce_tree_flag_generic<TYPE>(handle, num_addrs, dst, root, srclist, nelem, op, flags,  (NUM_DIGITS), (RADIXLOG2), (RADIX)); \
    return;\
  }\
  void smp_coll_reduce_chain_flag_ ## TYPE (smp_coll_t handle, int num_addrs,\
					   TYPE *dst, int root, TYPE *const srclist[], \
					   size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags) { \
    smp_coll_reduce_chain_flag_generic<TYPE>(handle, num_addrs, dst, root, srclist, nelem, op, flags,  (NUM_DIGITS), (RADIXLOG2), (RADIX)); \
    return;\
  }
  
  CONSTRUCT_RED_ROUTINES(int,  handle->red_int_log_radix_THREADS, handle->red_int_log_2_radix, handle->red_int_radix);
  CONSTRUCT_RED_ROUTINES(double,  handle->red_double_log_radix_THREADS, handle->red_double_log_2_radix, handle->red_double_radix);
  
  
}
