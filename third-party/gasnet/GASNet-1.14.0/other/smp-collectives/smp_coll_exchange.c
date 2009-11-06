/*   $Source: /var/local/cvs/gasnet/other/smp-collectives/smp_coll_exchange.c,v $
 *     $Date: 2009/10/22 20:24:55 $
 * $Revision: 1.4 $
 * Description: Shared Memory Collectives
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <smp_coll_internal.h>

void smp_coll_set_exchange_routine(smp_coll_t handle, smp_coll_exchange_routine_t routine_id, int in_radix) {
  smp_coll_safe_barrier(handle, 0);
  if(!POWEROFTWO(in_radix)) {
    
    if(handle->MYTHREAD==0) fprintf(stderr, "broadcast and reductions do not yet support non-power 2 radices\n");
    exit(1);
  }
  
  handle->exchange_radix = in_radix;
  handle->exchange_log_2_radix = smp_coll_mylogn(in_radix,2);
  handle->exchange_log_radix_THREADS = smp_coll_mylogn(handle->THREADS, in_radix);
  handle->exchange_log_2_THREADS = smp_coll_mylogn(handle->THREADS, 2);
  handle->curr_exchange_routine = routine_id;
  smp_coll_safe_barrier(handle, 0);
}


#if !(INLINE_ALL_COLLECTIVES)
void smp_coll_exchange(smp_coll_t handle, int num_addrs, void *const dstlist[], void * const srclist[],  
                       size_t nbytes, int flags) {
  (*handle->exchange_fns[handle->curr_exchange_routine])(handle, num_addrs, dstlist, srclist, 
                                                         nbytes, flags);
}
#endif

void smp_coll_exchange_flat(smp_coll_t handle,  int num_addrs, 
                            void * const dstlist[], void *const srclist[], 
                            size_t nbytes, int flags){
  int i;
  uint8_t *mysrc=(uint8_t*) srclist[handle->MYTHREAD];
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
  for(i=0; i<num_addrs; i++) {
    int dstproc = (handle->MYTHREAD+i) % handle->THREADS;
    memcpy(((uint8_t*)(dstlist[dstproc]))+handle->MYTHREAD*nbytes, mysrc+dstproc*nbytes, nbytes);
  }
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
}

static inline int pack_all_to_all_msg(void *src, void *dest, size_t nbytes,
                                     int digit, int radix, int j, int total_ranks) {
  int i_idx;
  
  int ret=0;
  
  if(POWEROFTWO(radix) && POWEROFTWO(total_ranks)) {
    int log2radix = smp_coll_mylogn(radix, 2);
    for(i_idx=0; i_idx < total_ranks; i_idx++) {
      if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(i_idx, digit, radix, log2radix) == j) {
        memcpy((int8_t*)dest+ret*nbytes, (int8_t*)src+i_idx*nbytes, nbytes);
        ret++;
      }
    }
  } else {
    /*pack if the digit_th digit of the radix-r representation of block_id is j*/
    for(i_idx=0; i_idx<total_ranks; i_idx++) {
      if( ((i_idx / smp_coll_mypown(radix, digit)) % radix) == j ) {
        memcpy((int8_t*)dest+ret*nbytes, (int8_t*)src+i_idx*nbytes, nbytes);
        ret++;
      }
    }
  }
  return ret;
}

static inline void unpack_all_to_all_msg(void *src, void *dest, size_t nbytes,
                                        int digit, int radix, int j, int total_ranks) {
  int i_idx;
  
  int blk_count=0;
  if(POWEROFTWO(radix) && POWEROFTWO(total_ranks)) {
    int log2radix = smp_coll_mylogn(radix, 2);
    for(i_idx=0; i_idx < total_ranks; i_idx++) {
      if(SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(i_idx, digit, radix, log2radix) == j) {
        memcpy((int8_t*)dest+i_idx*nbytes, (int8_t*)src+blk_count*nbytes, nbytes);
        blk_count++;
      }
    }
  } else {
    for(i_idx=0; i_idx<total_ranks; i_idx++) {
      if( ((i_idx / smp_coll_mypown(radix, digit)) % radix) == j ) {
        memcpy((int8_t*)dest+i_idx*nbytes, (int8_t*)src+blk_count*nbytes, nbytes);
        blk_count++;
      }
    }
  }
}

static inline void print_arr(int MYTHREAD, uint8_t* arr, int nbytes, char *id_str) {
  int i;
  int* myarr = (int*) arr;
  int nelem = nbytes/sizeof(int);
  
  for(i=0; i<nelem; i++) {
    fprintf(stdout, "%d> %s %d %d\n", MYTHREAD, id_str, i, myarr[i]);
  }
}

void smp_coll_exchange_dissemk_flag(smp_coll_t handle,  int num_addrs, 
                            void * const dstlist[], void *const srclist[], 
                            size_t nbytes, int flags){
  int i,j;

  uint8_t *mysrc=(uint8_t*) srclist[handle->MYTHREAD];
  uint8_t *mydst=(uint8_t*) dstlist[handle->MYTHREAD];
  uint8_t *myscratch=handle->aux_space;
  uint8_t *myscratch2=handle->aux_space+handle->THREADS*nbytes;
  int r = handle->exchange_radix;
  size_t offset = (handle->THREADS/r)*nbytes;
  int w = handle->exchange_log_radix_THREADS;
  int h;
  int dist;
  
  /*first make sure that we have enough scratch space to handle the brucks alltoall*/
  /*if not throw a loud warning and revert to the flat tree*/
  if((handle->THREADS*nbytes)*(r+2) > SMP_COLL_AUX_SPACE_SIZE) {
    smp_coll_exchange_flat(handle, num_addrs, dstlist, srclist, nbytes, flags);
    return;
  }
  
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
  memcpy(handle->aux_space+(handle->THREADS-handle->MYTHREAD)*nbytes, mysrc, handle->MYTHREAD*nbytes);
  memcpy(handle->aux_space, mysrc+(handle->MYTHREAD)*nbytes, (handle->THREADS-handle->MYTHREAD)*nbytes);

  dist = 1; 
  for(i=0; i<w; i++) {
    if(i==(w-1)) {
      /*h = ceil(THREADS/DIST);*/
      h = handle->THREADS/dist;
      if(handle->THREADS % dist != 0) 
	h++;
    } else {
      h = r;
    }
    
    /*ensures remote temp buffers are safe to read*/
    smp_coll_barrier(handle, flags);
    
    for(j=1; j<h; j++) {
      int dst = (handle->MYTHREAD +j*dist) % handle->THREADS;
      int nblocks = pack_all_to_all_msg(handle->aux_space, handle->aux_space+(handle->THREADS*nbytes), nbytes, i, r,j, handle->THREADS);
      memcpy(((uint8_t*)(handle->aux_space_all[dst]))+(handle->THREADS*nbytes)*2+((j-1)*(handle->THREADS*nbytes)), handle->aux_space+(handle->THREADS*nbytes), nblocks*nbytes);
    }
    /*ensures all data movement has finished*/
    smp_coll_barrier(handle, flags);
    
    for(j=1; j<h; j++) {
      unpack_all_to_all_msg(handle->aux_space+(handle->THREADS*nbytes)*2+((j-1)*(handle->THREADS*nbytes)), handle->aux_space, nbytes, i, r, j, handle->THREADS);
    }
    dist*=r;
  }
  
  for(i=0; i<handle->THREADS; i++) {
    int srcproc  = (handle->MYTHREAD - i) % handle->THREADS;
    if(srcproc < 0) {
      srcproc = handle->THREADS+srcproc;
    }
    memcpy((uint8_t*)mydst+i*nbytes, 
           handle->aux_space+srcproc*nbytes,
           nbytes);
  }
  
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
}
#define MEMCPY_CHECK(DST, SRC, SIZE) do{if((DST)!=(SRC)) memcpy((DST), (SRC), (SIZE));} while(0)

static inline void scale_ptrM(void * out_ptr[], void * const in_ptr[], size_t elem_count, size_t elem_size, int num_addrs) {
  int i;
  for(i=0; i<num_addrs; i++) {
    out_ptr[i] = (void *)((uintptr_t)in_ptr[i] + (elem_count * elem_size));
  }
}


static inline void local_gather(size_t count, void * dst, void * const srclist[], size_t nbytes) {
  uint8_t *dst_addr = (uint8_t *)dst;
  
  while (count--) {
    MEMCPY_CHECK(dst_addr, *srclist, nbytes);
    dst_addr += nbytes;
    srclist++;
  }
  
}

static inline void local_scatter(size_t count, void * const dstlist[], const void *src, size_t nbytes) {
  const uint8_t *src_addr = (const uint8_t *)src;
  
  while (count--) {
    MEMCPY_CHECK(*dstlist, src_addr, nbytes);
    dstlist++;
    src_addr += nbytes;
  }
  
}

static inline void local_broadcast(size_t count, void * const dstlist[], const void *src, size_t nbytes) {
  const uint8_t *src_addr = (const uint8_t *)src;
  
  while (count--) {
    MEMCPY_CHECK(*dstlist, src_addr, nbytes);
    dstlist++;
  }
  
}

void smp_coll_exchange_hierarchical(smp_coll_t handle,  int num_addrs, 
                                    void * const dstlist[], void *const srclist[], 
                                    size_t nbytes, int flags) {
  double a = 2.0;
  int r = handle->exchange_radix;
  int mynode = SMP_COLL_GET_UPPER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, 1, handle->exchange_radix, handle->exchange_log_2_radix);
  int my_local_thread = SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, 1, handle->exchange_radix, handle->exchange_log_2_radix);
  int nodes =  SMP_COLL_GET_UPPER_K_DIGITS_POWER2RADIX(handle->THREADS, 1, handle->exchange_radix, handle->exchange_log_2_radix);
  int threads_per_node =  handle->exchange_radix;
  int i,j;
  uint8_t *src, *dst;
  size_t dst_offset =  threads_per_node*handle->THREADS*nbytes;
  size_t nbytes_per_node = threads_per_node*threads_per_node*nbytes;
  
  
  src = handle->aux_space;
  dst = handle->aux_space + dst_offset;
  if(threads_per_node*handle->THREADS*nbytes*2 > SMP_COLL_AUX_SPACE_SIZE) {
    smp_coll_exchange_flat(handle, num_addrs, dstlist, srclist, nbytes, flags);
    return;
  }

  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
  if(my_local_thread == 0) {

    for(i=0; i<nodes; i++) {
      for(j=0; j<threads_per_node; j++) {
        scale_ptrM((void**) handle->tempaddrs, 
                   srclist+mynode*threads_per_node,
                   (i*threads_per_node+j)*nbytes, 1, threads_per_node);
        
        local_gather(threads_per_node,
                     (uint8_t*) src+
                     i*(threads_per_node*threads_per_node*nbytes)+
                     j*threads_per_node*nbytes,
                     (void* const*) handle->tempaddrs, nbytes);
      }
    }
    


    /*"distributed alltoall"*/
    for(i=1; i<=nodes; i++) {
      int dst= ((mynode+i)%nodes);
      memcpy(((uint8_t*)(handle->aux_space_all[dst*r])+dst_offset)+mynode*nbytes_per_node, src+dst*nbytes_per_node, nbytes_per_node);
    }

    gasnett_local_wmb();
    /*barrier to indicate all data transfers have finished*/
    if(handle->MYTHREAD==0) {
      for(i=1; i<nodes; i++) {
        while(SMP_COLL_GET_FLAG(handle,i*r, 0) == 0) {a = 1.0/a;}
      }      
      for(i=1; i<nodes; i++) {
        SMP_COLL_SET_FLAG(handle, i*r, 0, 0);
      }
    } else {
      SMP_COLL_SET_FLAG(handle, handle->MYTHREAD, 0, 1);
      while(SMP_COLL_GET_FLAG(handle, handle->MYTHREAD, 0) == 1) {a = 1.0/a;}
    }
    gasnett_local_rmb();

    /* local data movement*/
    for(i=0; i<nodes; i++) {
       scale_ptrM((void**) handle->tempaddrs, dstlist+mynode*threads_per_node, 
                 i*threads_per_node,
                 nbytes, threads_per_node);
      local_scatter(threads_per_node,
                    (void* const*) handle->tempaddrs,
                    (uint8_t*) dst+
                    i*threads_per_node*threads_per_node*nbytes,
                    nbytes*threads_per_node);
    }
    
  }
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags);
}

void smp_coll_exchange_hierarchical_overlap(smp_coll_t handle,  int num_addrs, 
                                            void * const dstlist[], void *const srclist[], 
                                            size_t nbytes, int flags) {
  double a = 2.0;
  int r = handle->exchange_radix;
  int mynode = SMP_COLL_GET_UPPER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, 1, handle->exchange_radix, handle->exchange_log_2_radix);
  int my_local_thread = SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(handle->MYTHREAD, 1, handle->exchange_radix, handle->exchange_log_2_radix);
  int nodes =  SMP_COLL_GET_UPPER_K_DIGITS_POWER2RADIX(handle->THREADS, 1, handle->exchange_radix, handle->exchange_log_2_radix);
  int threads_per_node =  handle->exchange_radix;
  int i,j;
  uint8_t *src, *dst;
  size_t dst_offset =  threads_per_node*handle->THREADS*nbytes;
  size_t nbytes_per_node = threads_per_node*threads_per_node*nbytes;
  
  
  src = handle->aux_space;
  dst = handle->aux_space + dst_offset;
  if(threads_per_node*handle->THREADS*nbytes*2 > SMP_COLL_AUX_SPACE_SIZE) {
    smp_coll_exchange_flat(handle, num_addrs, dstlist, srclist, nbytes, flags);
    return;
  }
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags); 
  if(my_local_thread == 0) {
    
    for(i=1; i<=nodes; i++) {
      int dstnode= ((mynode+i)%nodes);
      for(j=0; j<threads_per_node; j++) {
        scale_ptrM((void**) handle->tempaddrs, 
                   srclist+mynode*threads_per_node,
                   (dstnode*threads_per_node+j)*nbytes, 1, threads_per_node);
        
        local_gather(threads_per_node,
                     (uint8_t*) src+
                     dstnode*(threads_per_node*threads_per_node*nbytes)+
                     j*threads_per_node*nbytes,
                     (void* const*) handle->tempaddrs, nbytes);
      }
      memcpy(((uint8_t*)(handle->aux_space_all[dstnode*r])+dst_offset)+mynode*nbytes_per_node, src+dstnode*nbytes_per_node, nbytes_per_node);
    }
    
    
    gasnett_local_wmb();
    /*barrier to indicate all data transfers have finished*/
    if(handle->MYTHREAD==0) {
      for(i=1; i<nodes; i++) {
        while(SMP_COLL_GET_FLAG(handle,i*r, 0) == 0) {a = 1.0/a;}
      }      
      for(i=1; i<nodes; i++) {
        SMP_COLL_SET_FLAG(handle, i*r, 0, 0);
      }
    } else {
      SMP_COLL_SET_FLAG(handle, handle->MYTHREAD, 0, 1);
      while(SMP_COLL_GET_FLAG(handle, handle->MYTHREAD, 0) == 1) {a = 1.0/a;}
    }
    gasnett_local_rmb();
    
    /* local data movement*/
    for(i=0; i<nodes; i++) {
      scale_ptrM((void**) handle->tempaddrs, dstlist+mynode*threads_per_node, 
                 i*threads_per_node,
                 nbytes, threads_per_node);
      local_scatter(threads_per_node,
                    (void* const*) handle->tempaddrs,
                    (uint8_t*) dst+
                    i*threads_per_node*threads_per_node*nbytes,
                    nbytes*threads_per_node);
    }
    
  }
  if(!(flags & SMP_COLL_NO_SYNC)) smp_coll_barrier(handle, flags);
}
