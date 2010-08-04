/*   $Source: /var/local/cvs/gasnet/other/smp-collectives/smp_coll_bcast_scatter_gather.c,v $
 *     $Date: 2009/10/22 20:24:55 $
 * $Revision: 1.4 $
 * Description: Shared Memory Collectives
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <smp-collectives/smp_coll_internal.h>
#if 0
void smp_coll_set_broadcast_routine(smp_coll_t handle, smp_coll_broadcast_routine_t routine_id, int in_radix) {
  smp_coll_safe_barrier(handle, 0);
  if(!POWEROFTWO(in_radix)) {
    fprintf(stderr, "broadcast and reductions do not yet support non-power 2 radices\n");
    exit(1);
  }
  handle->broadcast_radix = in_radix;
  handle->broadcast_log_2_radix = smp_coll_mylogn(in_radix,2);
  handle->broadcast_log_radix_THREADS = smp_coll_mylogn(handle->THREADS, in_radix);
  handle->curr_bcast_routine = routine_id;
  smp_coll_safe_barrier(handle, 0);
}

#if !(INLINE_ALL_COLLECTIVES)
void smp_coll_broadcast(smp_coll_t handle, int num_addrs, void * const dstlist[], const void *src, 
                        size_t nbytes, int flags) {
  (*handle->bcast_fns[handle->curr_bcast_routine])(handle, num_addrs, dstlist, src, 
                                                   nbytes, flags);
}
#endif
#if !(INLINE_ALL_COLLECTIVES)
void smp_coll_scatter_offset(smp_coll_t handle, int num_addrs, void * const dstlist[], void *src, 
                             size_t nbytes, int flags, size_t offset) {
  (*handle->scatter_fns[handle->curr_scatter_routine])(handle, num_addrs, dstlist, src, 
                                                       nbytes, flags, offset);
}
#endif

#if !(INLINE_ALL_COLLECTIVES)
void smp_coll_gather(smp_coll_t handle, int num_addrs, const void *dst, void * const srclist[],  
                     size_t nbytes, int flags) {
  (*handle->gather_fns[handle->curr_gather_routine])(handle, num_addrs, dst, srclist, 
                                                     nbytes, flags);
}
#endif

void smp_coll_gather_all(smp_coll_t handle,  int num_addrs, 
                         void * const dstlist[], void *const srclist[], 
                         size_t nbytes, int flags){
  int new_flags = (flags & ~(SMP_COLL_ALL_SYNC|SMP_COLL_MY_SYNC)) | SMP_COLL_NO_SYNC; 
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
  smp_coll_gather(handle, num_addrs, dstlist[0], srclist, nbytes, new_flags);
  smp_coll_broadcast(handle, num_addrs, dstlist, dstlist[0], nbytes*handle->THREADS, new_flags);
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
}
#endif

void smp_coll_broadcast_flat(smp_coll_t handle, int num_addrs, void * const dstlist[], const void *src, 
			size_t nbytes, int flags){
  int idx = 0;
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
  if(handle->MYTHREAD==0) {
    for(idx=0; idx<num_addrs; idx++) {
      GASNETE_FAST_UNALIGNED_MEMCPY(dstlist[idx], src, nbytes); 
    }
  }
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
}

#if 0
void smp_coll_broadcast_tree_atomic(smp_coll_t handle, int num_addrs, void * const dstlist[], const void *src, 
                             size_t nbytes, int flags, int radix){
  int idx = 0;
  
  int num_digits = smp_coll_mylogn(handle->THREADS, radix); 
  int radixlog2 = smp_coll_mylogn(radix,2);
  int i,j,k;
  int parent=-1;
  
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags);
  for(i=0; i<num_digits; i++) {
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)!=0) {
      parent = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, 0,  radix, radixlog2);
      break;
    }
  }
  if(parent!=-1) {
    gasneti_waitwhile(SMP_COLL_READ_ATOMIC(handle, handle->MYTHREAD, 0, handle->curr_atomic_set)!=1);
    SMP_COLL_DEC_ATOMIC(handle, handle->MYTHREAD, 0, handle->curr_atomic_set);
    gasnett_local_rmb();
  } else {
    memcpy(dstlist[0], src, nbytes);
  }
  for(i=num_digits-1,j=0; i>=0; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
          GASNETE_FAST_UNALIGNED_MEMCPY(dstlist[dest], dstlist[handle->MYTHREAD], nbytes); 
          gasnett_local_wmb();
          SMP_COLL_INC_ATOMIC(handle, dest, 0, handle->curr_atomic_set);
        }
      }
    } 
    
  } 
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags); 
  handle->curr_atomic_set = !handle->curr_atomic_set;
}
#endif

void smp_coll_broadcast_tree_flag(smp_coll_t handle, int num_addrs, void * const dstlist[], const void *src, 
                                    size_t nbytes, int flags, int radix){
  int idx = 0;
  
  
  int num_digits = smp_coll_mylogn(handle->THREADS, radix); 
  int radixlog2 = smp_coll_mylogn(radix,2);

  int i,j,k;
  int parent=-1;
  
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags);
  /*first thing all threads do is set their flag to be 1 indicating they have arrived*/
  SMP_COLL_SET_BCAST_FLAG(handle, handle->MYTHREAD, 0, 1);
  
  /*
   Don't care who my parent is for this algorithm
  for(i=0; i<num_digits; i++) {
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)!=0) {
      parent = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, 0,  radix, radixlog2);
      break;
    }
  }
   */
  
  /*they then wait for the parent to come around and reset their flag back to 0 indicating the data has also arrived*/
  if(handle->MYTHREAD!=0) {
    gasneti_waitwhile(SMP_COLL_GET_BCAST_FLAG(handle, handle->MYTHREAD,0)!=0);
  } else {
    memcpy(dstlist[0], src, nbytes);
  }
  for(i=num_digits-1,j=0; i>=0; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
          /*wait for dest to be ready before we send*/
          gasneti_waitwhile(SMP_COLL_GET_BCAST_FLAG(handle, dest, 0)==0);
          memcpy(dstlist[dest], dstlist[handle->MYTHREAD], nbytes); 
          /*write memory barrier to ensure data is transfered before we set the flag*/
          gasnett_local_wmb();
          SMP_COLL_SET_BCAST_FLAG(handle, dest, 0, 0);
        }
      }
    } 
    
  } 
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags); 

}

#if 0
void smp_coll_broadcast_tree_leaf_get_flag(smp_coll_t handle, int num_addrs, void * const dstlist[], const void *src, 
                                  size_t nbytes, int flags){
  int idx = 0;
  int num_digits = handle->broadcast_log_radix_THREADS;
  int radixlog2 = handle->broadcast_log_2_radix;
  int radix = handle->broadcast_radix;
  int i,j,k;
  int parent=-1;
  double a=2.0;

  
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags);
  /*first thing all threads do is set their flag to be 1 indicating they have arrived*/
  SMP_COLL_SET_BCAST_FLAG(handle, handle->MYTHREAD, 0, 1);
  /*they then wait for the parent to come around and reset their flag back to 0 indicating the data has also arrived*/
  if(handle->MYTHREAD!=0) {
    while(SMP_COLL_GET_BCAST_FLAG(handle, handle->MYTHREAD,0)!=0){a=1.0/a;}
    /*read memory barrier to ensure data is transfered before we use it*/
    gasnett_local_rmb();
  } else {
    memcpy(dstlist[0], src, nbytes);
  }
  /* broadcast down the tree except to the last level*/
  for(i=num_digits-1,j=0; i>=1; i--,j++) {
    /*if my i^th digit is 0 that means that i am a sender for this round*/
    if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0 && 
       SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, i, radix, radixlog2)==0) {
      for(k=1;k<radix;k++) {
        int dest = SMP_COLL_MAKE_NUM_POWER2RADIX(handle->MYTHREAD, i, k, radix, radixlog2);
        if(dest<handle->THREADS) {
          /*wait for dest to be ready before we send*/
          while(SMP_COLL_GET_BCAST_FLAG(handle, dest, 0)==0){a=1.0/a;}
          memcpy(dstlist[dest], dstlist[handle->MYTHREAD], nbytes); 
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
        double a=2.0;
        /*wait for dest to be ready before we send*/
        while(SMP_COLL_GET_BCAST_FLAG(handle, dest, 0)==0){a=1.0/a;}
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
    /*grab the data*/

    gasnett_local_rmb();
    memcpy(dstlist[handle->MYTHREAD], dstlist[parent], nbytes);
    /*raise a second flag indicating that my data read is done*/
    gasnett_local_wmb();
    SMP_COLL_SET_BCAST_FLAG(handle, handle->MYTHREAD, 1, 1);
  }
  if((flags & SMP_COLL_ALL_SYNC)) smp_coll_barrier(handle, flags); 
  
}


void smp_coll_scatter_offset_flat(smp_coll_t handle, int num_addrs, void * const dstlist[], void *src,
			     size_t nbytes, int flags, size_t offset) {
  int idx = 0;
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
  if(handle->MYTHREAD==0) {
    for(idx=0; idx<num_addrs; idx++) {
      memcpy(((uint8_t*) dstlist[idx])+offset, (uint8_t*) src+idx*nbytes, nbytes); 
    }
  }
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
}


void smp_coll_gather_flat(smp_coll_t handle, int num_addrs, const void *dst, void *const srclist[], 
		     size_t nbytes, int flags){
  int idx = 0;
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
  if(handle->MYTHREAD==0) {
    for(idx=0; idx<num_addrs; idx++) {
      memcpy((uint8_t*) dst+idx*nbytes, srclist[idx], nbytes); 
    }
  }
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
}
#endif
