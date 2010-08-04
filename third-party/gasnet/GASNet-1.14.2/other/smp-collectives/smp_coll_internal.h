/*   $Source: /var/local/cvs/gasnet/other/smp-collectives/smp_coll_internal.h,v $
 *     $Date: 2009/10/22 20:24:55 $
 * $Revision: 1.4 $
 * Description: Shared Memory Collectives
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef __SMP_COLL_INTERNAL_H_INC__
#define __SMP_COLL_INTERNAL_H_INC__ 1
#include <smp-collectives/smp_coll.h>
#include <smp-collectives/smp_coll_dissem.h>


#if HAVE_PTHREAD_BARRIER
#ifndef __USE_XOPEN2K
#include "pthread_barrier.h"
#endif
#endif


#define POWEROFTWO(P) (((P)&((P)-1)) == 0)
#define ALIGNDOWN(p,P)    (((uintptr_t)(p))&~((uintptr_t)((P)-1)))
#define ALIGNUP(p,P)     (ALIGNDOWN((uintptr_t)(p)+((uintptr_t)((P)-1)),P))

#define SMP_COLL_CACHE_LINE MAX(GASNETT_CACHE_LINE_BYTES,64)
#define SMP_COLL_AUX_SPACE_SIZE 64*1024
static inline int smp_coll_mylogn(int num, int base) {
  int ret=1;
  int mult = base;
  while (num > mult) {
    ret++;
    mult*=base;
  }
  return ret;
  
}

static inline int smp_coll_mypown(int base, int power) {  
  int ret=1;
  
  while(power>0) {
    ret*=base;
    power--;
  }
  return ret;
}


#define SMP_COLL_GET_FLAG(HANDLE,THREAD_ID,IDX)\
((volatile uint32_t*)(HANDLE)->flags)[(THREAD_ID)*SMP_COLL_CACHE_LINE+(IDX)]

#define SMP_COLL_SET_FLAG(HANDLE,THREAD_ID,IDX,NEW_VAL)\
((volatile uint32_t*)(HANDLE)->flags)[(THREAD_ID)*SMP_COLL_CACHE_LINE+(IDX)] = (uint32_t)(NEW_VAL)

#define SMP_COLL_GET_BARRIER_FLAG(HANDLE,THREAD_ID,IDX)\
((volatile uint32_t*)(HANDLE)->barrier_flags)[(THREAD_ID)*SMP_COLL_CACHE_LINE+(IDX)]

#define SMP_COLL_SET_BARRIER_FLAG(HANDLE,THREAD_ID,IDX,NEW_VAL)\
((volatile uint32_t*)(HANDLE)->barrier_flags)[(THREAD_ID)*SMP_COLL_CACHE_LINE+(IDX)] = (uint32_t)(NEW_VAL)

#define SMP_COLL_GET_BCAST_FLAG(HANDLE,THREAD_ID,IDX)\
((volatile uint32_t*)(HANDLE)->bcast_flags)[(THREAD_ID)*SMP_COLL_CACHE_LINE+(IDX)]

#define SMP_COLL_SET_BCAST_FLAG(HANDLE,THREAD_ID,IDX,NEW_VAL)\
((volatile uint32_t*)(HANDLE)->bcast_flags)[(THREAD_ID)*SMP_COLL_CACHE_LINE+(IDX)] = (uint32_t)(NEW_VAL)

#define SMP_COLL_READ_ATOMIC(HANDLE,THREAD,IDX,FLAG_SET)\
(int)gasnett_atomic_read(&((HANDLE)->atomic_vars)[(FLAG_SET)*(HANDLE)->THREADS*SMP_COLL_CACHE_LINE+(THREAD)*SMP_COLL_CACHE_LINE+IDX], GASNETT_ATOMIC_NONE)

#define SMP_COLL_INC_ATOMIC(HANDLE,THREAD,IDX,FLAG_SET)\
gasnett_atomic_increment(&((HANDLE)->atomic_vars)[(FLAG_SET)*(HANDLE)->THREADS*SMP_COLL_CACHE_LINE+(THREAD)*SMP_COLL_CACHE_LINE+IDX], GASNETT_ATOMIC_NONE)

#define SMP_COLL_DEC_ATOMIC(HANDLE,THREAD,IDX,FLAG_SET)\
gasnett_atomic_decrement(&((HANDLE)->atomic_vars)[(FLAG_SET)*(HANDLE)->THREADS*SMP_COLL_CACHE_LINE+(THREAD)*SMP_COLL_CACHE_LINE+IDX], GASNETT_ATOMIC_NONE)

#define SMP_COLL_RESET_ATOMIC(HANDLE,THREAD,IDX,FLAG_SET)\
gasnett_atomic_set(&((HANDLE)->atomic_vars)[(FLAG_SET)*(HANDLE)->THREADS*SMP_COLL_CACHE_LINE+(THREAD)*SMP_COLL_CACHE_LINE+IDX], 0, GASNETT_ATOMIC_NONE)


/*
 Utility operations for constructing power 2 trees
*/
#define SMP_COLL_GET_ITH_DIGIT_POWER2RADIX(NUMBER,DIGIT_ID,RADIX,LOG_2_RADIX) \
(((NUMBER) & ((RADIX)-1)<<((DIGIT_ID)*(LOG_2_RADIX))) >> ((DIGIT_ID)*(LOG_2_RADIX)))

/*take the number template and replace digits 0-DIGIT_ID-1 with 0 and DIGIT with NEW_DIGIT*/
#define SMP_COLL_MAKE_NUM_POWER2RADIX(TEMPLATE_NUMBER,DIGIT_ID,NEW_DIGIT,RADIX,LOG_2_RADIX) \
(((TEMPLATE_NUMBER) & (-1)<<(((DIGIT_ID+1))*(LOG_2_RADIX))) + ((NEW_DIGIT) << ((DIGIT_ID))*(LOG_2_RADIX)))

/*take the number template and replace i^th digit with NEW_DIGIT*/
#define SMP_COLL_REPLACE_DIGIT_POWER2RADIX(TEMPLATE_NUMBER,DIGIT_ID,NEW_DIGIT,RADIX,LOG_2_RADIX) \
(((TEMPLATE_NUMBER) & ~(((RADIX)-1)<<(((DIGIT_ID)*(LOG_2_RADIX))))) + ((NEW_DIGIT) << ((DIGIT_ID))*(LOG_2_RADIX)))

/*take the number and get digits DIGIT_ID through last digit*/
#define SMP_COLL_GET_UPPER_K_DIGITS_POWER2RADIX(NUMBER,DIGIT_ID,RADIX,LOG_2_RADIX) \
((NUMBER) >> ((DIGIT_ID)*(LOG_2_RADIX)))

#define SMP_COLL_GET_LOWER_K_DIGITS_POWER2RADIX(NUMBER,DIGIT_ID,RADIX,LOG_2_RADIX) \
((NUMBER) & ~((-1)<<((DIGIT_ID)*(LOG_2_RADIX))))


typedef void (*SMP_COLL_BARR_FN)(smp_coll_t handle, int);

void smp_coll_barrier_cond_var(smp_coll_t handle, int flags);
void smp_coll_barrier_dissem_atomic(smp_coll_t handle, int flags);
void smp_coll_barrier_tree_push_push(smp_coll_t handle, int flags);
void smp_coll_barrier_tree_push_pull(smp_coll_t handle, int flags);
void smp_coll_barrier_tree_pull_push(smp_coll_t handle, int flags);
void smp_coll_barrier_tree_pull_pull(smp_coll_t handle, int flags);

void smp_coll_barrier_pthread(smp_coll_t handle, int flags);

#if HAVE_PTHREAD_BARRIER
#define SMP_COLL_CONSTRUCT_BARR_ROUTINES(HANDLE) do{\
(HANDLE)->barr_fns[0] = smp_coll_barrier_cond_var; \
(HANDLE)->barr_fns[1] = smp_coll_barrier_dissem_atomic; \
(HANDLE)->barr_fns[2] = smp_coll_barrier_tree_push_push; \
(HANDLE)->barr_fns[3] = smp_coll_barrier_tree_push_pull; \
(HANDLE)->barr_fns[4] = smp_coll_barrier_tree_pull_push; \
(HANDLE)->barr_fns[5] = smp_coll_barrier_tree_pull_pull; \
(HANDLE)->barr_fns[6] = smp_coll_barrier_pthread; \
(HANDLE)->curr_barrier_routine=3;\
} while(0)
#else
#define SMP_COLL_CONSTRUCT_BARR_ROUTINES(HANDLE) do{\
(HANDLE)->barr_fns[0] = smp_coll_barrier_cond_var; \
(HANDLE)->barr_fns[1] = smp_coll_barrier_dissem_atomic; \
(HANDLE)->barr_fns[2] = smp_coll_barrier_tree_push_push; \
(HANDLE)->barr_fns[3] = smp_coll_barrier_tree_push_pull; \
(HANDLE)->barr_fns[4] = smp_coll_barrier_tree_pull_push; \
(HANDLE)->barr_fns[5] = smp_coll_barrier_tree_pull_pull; \
(HANDLE)->curr_barrier_routine=3;\
} while(0)
#endif

#if 0
typedef void (*SMP_COLL_BCAST_FN)(smp_coll_t handle, int num_addrs, void * const dstlist[], const void *src, 
                                  size_t nbytes, int flags);

void smp_coll_broadcast_flat(smp_coll_t handle, int num_addrs, void * const dstlist[], const void *src, 
                             size_t nbytes, int flags);
void smp_coll_broadcast_tree_atomic(smp_coll_t handle, int num_addrs, void * const dstlist[], const void *src, 
                             size_t nbytes, int flags);
void smp_coll_broadcast_tree_flag(smp_coll_t handle, int num_addrs, void * const dstlist[], const void *src, 
                                    size_t nbytes, int flags);
void smp_coll_broadcast_tree_leaf_get_flag(smp_coll_t handle, int num_addrs, void * const dstlist[], const void *src, 
                                           size_t nbytes, int flags);
#define SMP_COLL_CONSTRUCT_BCAST_ROUTINES(HANDLE) do{\
(HANDLE)->bcast_fns[0] = smp_coll_broadcast_flat; \
(HANDLE)->bcast_fns[1] = smp_coll_broadcast_tree_atomic; \
(HANDLE)->bcast_fns[2] = smp_coll_broadcast_tree_flag; \
(HANDLE)->bcast_fns[3] = smp_coll_broadcast_tree_leaf_get_flag; \
(HANDLE)->curr_bcast_routine=2;\
} while(0)

typedef void (*SMP_COLL_SCATTER_OFFSET_FN)(smp_coll_t handle, int num_addrs, void * const dstlist[], void *src, 
                                    size_t nbytes, int flags, size_t offset);
#define SMP_COLL_NUM_SCATTER_ROUTINES 1
void smp_coll_scatter_offset_flat(smp_coll_t handle, int num_addrs, void * const dstlist[], void *src, 
                             size_t nbytes, int flags, size_t offset);

#define SMP_COLL_CONSTRUCT_SCATTER_ROUTINES(HANDLE) do{\
(HANDLE)->scatter_fns[0] = smp_coll_scatter_offset_flat; \
(HANDLE)->curr_scatter_routine=0;\
} while(0)

typedef void (*SMP_COLL_GATHER_FN)(smp_coll_t handle, int num_addrs,  const void *dst, void * const srclist[], 
                                    size_t nbytes, int flags);
#define SMP_COLL_NUM_GATHER_ROUTINES 1
void smp_coll_gather_flat(smp_coll_t handle, int num_addrs, const void *dst, void * const srclist[],  
                           size_t nbytes, int flags);

#define SMP_COLL_CONSTRUCT_GATHER_ROUTINES(HANDLE) do{\
(HANDLE)->gather_fns[0] = smp_coll_gather_flat; \
(HANDLE)->curr_gather_routine=0;\
} while(0)



typedef void (*SMP_COLL_ALLRED_INT_FN)(smp_coll_t handle, int num_addrs,  
                                       int *const dstlist[], int *const srclist[], 
                                       size_t nelem, smp_coll_red_op_t op, RED_FN_INT fnptr, int flags);

typedef void (*SMP_COLL_ALLRED_DOUBLE_FN)(smp_coll_t handle, int num_addrs,  
                                       double *const dstlist[], double *const srclist[], 
                                       size_t nelem, smp_coll_red_op_t op, RED_FN_DBL fnptr, int flags);



#define DECLARE_REDFUNS(TYPE)\
void smp_coll_all_reduce_flat_ ## TYPE (smp_coll_t handle, int num_addrs,\
                                  TYPE *const dstlist[], TYPE *const srclist[],\
                                  size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags);\
void smp_coll_all_reduce_dissem2_flag_ ## TYPE (smp_coll_t handle, int num_addrs,\
                                          TYPE *const dstlist[], TYPE *const srclist[],\
                                          size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags);\
void smp_coll_all_reduce_tree_flag_ ## TYPE (smp_coll_t handle, int num_addrs,\
                                  TYPE *const dstlist[], TYPE *const srclist[],\
                                  size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags);\
void smp_coll_all_reduce_dissemk_atomic_ ## TYPE (smp_coll_t handle, int num_addrs,\
                                         TYPE *const dstlist[], TYPE *const srclist[],\
                                         size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags);\
void smp_coll_all_reduce_flag_tree_up_flat_down_ ## TYPE (smp_coll_t handle, int num_addrs,\
                                                         TYPE *const dstlist[], TYPE *const srclist[],\
                                                        size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags);\
void smp_coll_all_reduce_flag_tree_up_tree_down_ ## TYPE (smp_coll_t handle, int num_addrs,\
                                                          TYPE *const dstlist[], TYPE *const srclist[],\
                                                          size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags)
DECLARE_REDFUNS(int);
DECLARE_REDFUNS(double);
#undef DECLARE_REDFUNS

#define SMP_COLL_CONSTRUCT_ALLRED_INT_ROUTINES(HANDLE) do{	\
(HANDLE)->allred_int_fns[0] =  smp_coll_all_reduce_flat_int;\
(HANDLE)->allred_int_fns[1] =  smp_coll_all_reduce_dissem2_flag_int;\
(HANDLE)->allred_int_fns[2] =  smp_coll_all_reduce_tree_flag_int;\
(HANDLE)->allred_int_fns[3] =  smp_coll_all_reduce_dissemk_atomic_int;\
(HANDLE)->allred_int_fns[4] = smp_coll_all_reduce_flag_tree_up_flat_down_int;\
(HANDLE)->allred_int_fns[5] = smp_coll_all_reduce_flag_tree_up_tree_down_int;\
(HANDLE)->curr_allred_int_routine=1;\
} while(0)

#define SMP_COLL_CONSTRUCT_ALLRED_DOUBLE_ROUTINES(HANDLE) do{\
(HANDLE)->allred_double_fns[0] =  smp_coll_all_reduce_flat_double;\
(HANDLE)->allred_double_fns[1] =  smp_coll_all_reduce_dissem2_flag_double;\
(HANDLE)->allred_double_fns[2] =  smp_coll_all_reduce_tree_flag_double;\
(HANDLE)->allred_double_fns[3] =  smp_coll_all_reduce_dissemk_atomic_double;\
(HANDLE)->allred_double_fns[4] = smp_coll_all_reduce_flag_tree_up_flat_down_double;\
(HANDLE)->allred_double_fns[5] = smp_coll_all_reduce_flag_tree_up_tree_down_double;\
(HANDLE)->curr_allred_double_routine=1;\
} while(0)

typedef void (*SMP_COLL_RED_INT_FN)(smp_coll_t handle, int num_addrs,  
				    int *dst, int root, int *const srclist[], 
                                       size_t nelem, smp_coll_red_op_t op, RED_FN_INT fnptr, int flags);

typedef void (*SMP_COLL_RED_DOUBLE_FN)(smp_coll_t handle, int num_addrs,  
                                       double *dst, int root, double *const srclist[], 
                                       size_t nelem, smp_coll_red_op_t op, RED_FN_DBL fnptr, int flags);


#define DECLARE_REDFUNS(TYPE)\
void smp_coll_reduce_flat_ ## TYPE (smp_coll_t handle, int num_addrs,\
					TYPE * dst, int root, TYPE *const srclist[], \
                                  size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags);\
void smp_coll_reduce_tree_flag_ ## TYPE (smp_coll_t handle, int num_addrs,\
					     TYPE *dst, int root, TYPE *const srclist[], \
                                  size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags);\
void smp_coll_reduce_chain_flag_ ## TYPE (smp_coll_t handle, int num_addrs,\
					     TYPE *dst, int root, TYPE *const srclist[], \
                                  size_t nelem, smp_coll_red_op_t op, TYPE (*fnptr)(TYPE,TYPE), int flags)


DECLARE_REDFUNS(int);
DECLARE_REDFUNS(double);

#undef DECLARE_REDFUNS

#define SMP_COLL_CONSTRUCT_RED_INT_ROUTINES(HANDLE) do{	\
(HANDLE)->red_int_fns[0] =  smp_coll_reduce_flat_int;\
(HANDLE)->red_int_fns[1] =  smp_coll_reduce_tree_flag_int;\
(HANDLE)->red_int_fns[2] =  smp_coll_reduce_chain_flag_int;\
(HANDLE)->curr_red_int_routine=1;\
} while(0)

#define SMP_COLL_CONSTRUCT_RED_DOUBLE_ROUTINES(HANDLE) do{\
(HANDLE)->red_double_fns[0] =  smp_coll_reduce_flat_double;\
(HANDLE)->red_double_fns[1] =  smp_coll_reduce_tree_flag_double;\
(HANDLE)->red_double_fns[2] =  smp_coll_reduce_chain_flag_double;\
(HANDLE)->curr_red_double_routine=1;\
} while(0)


typedef void (*SMP_COLL_EXCHANGE_FN)(smp_coll_t handle,  int num_addrs, 
                                     void * const dstlist[], void *const srclist[], 
                                     size_t nbytes, int flags);


void smp_coll_exchange_flat(smp_coll_t handle,  int num_addrs, 
                            void * const dstlist[], void *const srclist[], 
                            size_t nbytes, int flags);

void smp_coll_exchange_dissemk_flag(smp_coll_t handle,  int num_addrs, 
                                    void * const dstlist[], void *const srclist[], 
                                    size_t nbytes, int flags);

void smp_coll_exchange_hierarchical(smp_coll_t handle,  int num_addrs, 
                                    void * const dstlist[], void *const srclist[], 
                                    size_t nbytes, int flags);

void smp_coll_exchange_hierarchical_overlap(smp_coll_t handle,  int num_addrs, 
                                    void * const dstlist[], void *const srclist[], 
                                    size_t nbytes, int flags);

#define SMP_COLL_CONSTRUCT_EXCHANGE_ROUTINES(HANDLE) do{\
(HANDLE)->exchange_fns[0] = smp_coll_exchange_flat;\
(HANDLE)->exchange_fns[1] = smp_coll_exchange_dissemk_flag;\
(HANDLE)->exchange_fns[2] = smp_coll_exchange_hierarchical;\
(HANDLE)->exchange_fns[3] = smp_coll_exchange_hierarchical_overlap;\
(HANDLE)->curr_exchange_routine = 3;\
} while(0)
#endif

struct smp_coll_t_{
  int THREADS;
  int MYTHREAD;
  volatile uint32_t *flags;
  volatile uint32_t *barrier_flags;
  volatile uint32_t *bcast_flags;
  int barrier_root;
  int flag_set;
  int barrier_flag_set;
  gasnett_atomic_t *atomic_vars;
  int curr_atomic_set;

  
  SMP_COLL_BARR_FN barr_fns[SMP_COLL_NUM_BARR_ROUTINES];
  dissem_info_t *dissem_info;
  int barrier_radix, barrier_log_2_radix, barrier_log_radix_THREADS;
  int curr_barrier_routine;
  uint32_t *barrier_done;
  int barrier_phase;
#if HAVE_PTHREAD_BARRIER
  pthread_barrier_t *pthread_barrier;
#endif

  int barrier_parent;
  int barrier_num_children;
  int *barrier_children;
  
#if 0
  SMP_COLL_BCAST_FN bcast_fns[SMP_COLL_NUM_BROADCAST_ROUTINES];
  int broadcast_radix, broadcast_log_2_radix, broadcast_log_radix_THREADS;
  int curr_bcast_routine;

  SMP_COLL_SCATTER_OFFSET_FN scatter_fns[SMP_COLL_NUM_SCATTER_ROUTINES];
  int curr_scatter_routine;

  SMP_COLL_GATHER_FN gather_fns[SMP_COLL_NUM_GATHER_ROUTINES];
  int curr_gather_routine;


  SMP_COLL_RED_DOUBLE_FN red_double_fns[SMP_COLL_NUM_REDUCE_ROUTINES];
  int red_double_radix, red_double_log_2_radix, red_double_log_radix_THREADS, red_double_log_2_THREADS;
  int curr_red_double_routine;

  SMP_COLL_RED_INT_FN red_int_fns[SMP_COLL_NUM_REDUCE_ROUTINES];
  int red_int_radix, red_int_log_2_radix, red_int_log_radix_THREADS, red_int_log_2_THREADS;
  int curr_red_int_routine;
  
  SMP_COLL_ALLRED_DOUBLE_FN allred_double_fns[SMP_COLL_NUM_ALL_REDUCE_ROUTINES];
  int allred_double_radix, allred_double_log_2_radix, allred_double_log_radix_THREADS, allred_double_log_2_THREADS;
  int curr_allred_double_routine;
  
    SMP_COLL_ALLRED_INT_FN allred_int_fns[SMP_COLL_NUM_ALL_REDUCE_ROUTINES];
  int allred_int_radix, allred_int_log_2_radix, allred_int_log_radix_THREADS, allred_int_log_2_THREADS;
  int curr_allred_int_routine;


  SMP_COLL_EXCHANGE_FN exchange_fns[SMP_COLL_NUM_EXCHANGE_ROUTINES];
  int exchange_radix, exchange_log_2_radix, exchange_log_radix_THREADS, exchange_log_2_THREADS;
  int curr_exchange_routine;
#endif
  
  uint8_t *aux_space;
  uint8_t **aux_space_all;
  void **tempaddrs;
  

  
  
};
#endif


