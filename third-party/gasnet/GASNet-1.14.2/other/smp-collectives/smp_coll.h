/*   $Source: /var/local/cvs/gasnet/other/smp-collectives/smp_coll.h,v $
 *     $Date: 2009/10/22 20:24:55 $
 * $Revision: 1.3 $
 * Description: Shared Memory Collectives
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */
#ifndef __SMP_COLL_H_INC__
#define __SMP_COLL_H_INC__ 1
#include <pthread.h>
#include <gasnet_tools.h>

#define HAVE_PTHREAD_BARRIER 0

/*declarations that we'll need inside smp_coll_internal that are defined inside this header*/
struct smp_coll_t_;
typedef struct smp_coll_t_* smp_coll_t;
typedef enum{SMP_COLL_ADD=0, SMP_COLL_MIN, SMP_COLL_MAX, SMP_COLL_CUSTOM_FN} smp_coll_red_op_t;

typedef int (*RED_FN_INT)(int, int);
typedef double (*RED_FN_DBL)(double, double);

#define SMP_COLL_SKIP_TUNE_BARRIERS 0x1

#define SMP_COLL_SET_AFFINITY 1<<2

#define INLINE_ALL_COLLECTIVES 0
void smp_coll_set_affinity(int location);
smp_coll_t smp_coll_init(size_t aux_space_per_thread, int flags, int THREADS, int MYTHREAD);
void smp_coll_reset_all_flags(smp_coll_t handle);
void smp_coll_tune_barrier(smp_coll_t handle);
/*tuning knobs*/

#if HAVE_PTHREAD_BARRIER
#define SMP_COLL_NUM_BARR_ROUTINES 7
#else
#define SMP_COLL_NUM_BARR_ROUTINES 6
#endif

typedef enum{SMP_COLL_BARRIER_COND_VAR=0, SMP_COLL_BARRIER_DISSEM_ATOMIC, 
SMP_COLL_BARRIER_TREE_PUSH_PUSH, SMP_COLL_BARRIER_TREE_PUSH_PULL, SMP_COLL_BARRIER_TREE_PULL_PUSH, SMP_COLL_BARRIER_TREE_PULL_PULL, SMP_COLL_BARRIER_PTHREAD} smp_coll_barrier_routine_t;
void smp_coll_set_barrier_routine(smp_coll_t handle, smp_coll_barrier_routine_t routine_id, int in_radix);

#define SMP_COLL_NUM_BROADCAST_ROUTINES 4
typedef enum{SMP_COLL_BROADCAST_FLAT=0, SMP_COLL_BROADCAST_TREE_ATOMIC, SMP_COLL_BROADCAST_TREE_FLAG, SMP_COLL_BROADCAST_TREE_LEAF_GET_FLAG} smp_coll_broadcast_routine_t;
void smp_coll_set_broadcast_routine(smp_coll_t handle, smp_coll_broadcast_routine_t routine_id, int in_radix);

#define SMP_COLL_NUM_ALL_REDUCE_ROUTINES 6
typedef enum{SMP_COLL_ALL_REDUCE_FLAT=0, SMP_COLL_ALL_REDUCE_DISSEM2_FLAG,
SMP_COLL_ALL_REDUCE_TREE_FLAG, SMP_COLL_ALL_REDUCE_DISSEMK_ATOMIC, SMP_COLL_ALL_REDUCE_TREE_UP_FLAT_DOWN, SMP_COLL_ALL_REDUCE_TREE_UP_TREE_DOWN} smp_coll_all_reduce_routine_t;
void smp_coll_set_all_reduce_int_routine(smp_coll_t handle, smp_coll_all_reduce_routine_t routine_id, int in_radix);
void smp_coll_set_all_reduce_double_routine(smp_coll_t handle, smp_coll_all_reduce_routine_t routine_id, int in_radix);

#define SMP_COLL_NUM_REDUCE_ROUTINES 3
typedef enum{SMP_COLL_REDUCE_FLAT=0, 
	     SMP_COLL_REDUCE_TREE_FLAG, SMP_COLL_REDUCE_CHAIN_FLAG} smp_coll_reduce_routine_t; 

void smp_coll_set_reduce_int_routine(smp_coll_t handle, smp_coll_reduce_routine_t routine_id, int in_radix);
void smp_coll_set_reduce_double_routine(smp_coll_t handle, smp_coll_reduce_routine_t routine_id, int in_radix);

#define SMP_COLL_NUM_EXCHANGE_ROUTINES 4
typedef enum{SMP_COLL_EXCHANGE_FLAT=0, SMP_COLL_EXCHANGE_DISSEM_FLAG, SMP_COLL_EXCHANGE_HIEARCHICAL, SMP_COLL_EXCHANGE_HIEARCHICAL_OVERLAP} smp_coll_exchange_routine_t;
void smp_coll_set_exchange_routine(smp_coll_t handle, smp_coll_exchange_routine_t routine_id, int in_radix);

#include <smp-collectives/smp_coll_internal.h>

void smp_coll_safe_barrier(smp_coll_t handle, int flags);

#if INLINE_ALL_COLLECTIVES
#define smp_coll_barrier(HANDLE, FLAGS) do {\
(*(HANDLE)->barr_fns[(HANDLE)->curr_barrier_routine])(HANDLE, (FLAGS));\
}while(0)
#else
void smp_coll_barrier(smp_coll_t handle, int flags);
#endif


#if INLINE_ALL_COLLECTIVES
#define smp_coll_broadcast(HANDLE, NUM_ADDRS, DSTLIST, SRC, NBYTES, FLAGS) do {\
(*(HANDLE)->bcast_fns[(HANDLE)->curr_bcast_routine])((HANDLE), (NUM_ADDRS), (DSTLIST), (SRC), (NBYTES), (FLAGS));\
}while(0)
#else
void smp_coll_broadcast(smp_coll_t handle, int num_addrs, void * const dstlist[], const void *src, 
			size_t nbytes, int flags);
#endif

#define smp_coll_scatter(HANDLE, NUMADDRS, DSTLIST, SRC, NBYTES, FLAGS) smp_coll_scatter_offset(HANDLE, NUMADDRS, DSTLIST, SRC, NBYTES, FLAGS, 0)

#if INLINE_ALL_COLLECTIVES
#define smp_coll_scatter_offset(HANDLE, NUM_ADDRS, DSTLIST, SRC, NBYTES, FLAGS, OFFSET) do {\
(*(HANDLE)->scatter_fns[(HANDLE)->curr_scatter_routine])((HANDLE), (NUM_ADDRS), (DSTLIST), (SRC), (NBYTES), (FLAGS), (OFFSET));\
}while(0)
#else
void smp_coll_scatter_offset(smp_coll_t handle, int num_addrs, void * const dstlist[], void *src, 
			     size_t nbytes, int flags, size_t offset);
#endif

#if INLINE_ALL_COLLECTIVES
#define smp_coll_gather(HANDLE, NUM_ADDRS, DST, SRCLIST, NBYTES, FLAGS) do {\
(*(HANDLE)->gather_fns[(HANDLE)->curr_gather_routine])((HANDLE), (NUM_ADDRS), (DST), (SRCLIST), (NBYTES), (FLAGS));\
}while(0)
#else
void smp_coll_gather(smp_coll_t handle, int num_addrs, const void *dst, void *const srclist[], 
		     size_t nbytes, int flags);
#endif


  

void smp_coll_gather_all(smp_coll_t handle, int num_addrs,  
			 void * const dstlist[], void *const srclist[], 
			 size_t nbytes, int flags);

#if INLINE_ALL_COLLECTIVES
#define smp_coll_exchange(HANDLE, NUM_ADDRS, DSTLIST, SRCLIST, NBYTES, FLAGS) do{\
(*(HANDLE)->exchange_fns[(HANDLE)->curr_exchange_routine])((HANDLE), (NUM_ADDRS), (DSTLIST), (SRCLIST), (NBYTES), (FLAGS));\
} while(0)
#else
void smp_coll_exchange(smp_coll_t handle, int num_addrs,  
		       void * const dstlist[], void *const srclist[], 
		       size_t nbytes, int flags);
#endif

#if 1

#if INLINE_ALL_COLLECTIVES
#define smp_coll_reduce_int(HANDLE, NUM_ADDRS, DST, ROOT, SRCLIST, NINTS, OP, FNPTR, FLAGS) do {	\
    (*(HANDLE)->red_int_fns[(HANDLE)->curr_red_int_routine])((HANDLE), (NUM_ADDRS), (DST), (ROOT), (SRCLIST), (NINTS), (OP), (FNPTR), (FLAGS)); \
}while(0)
#else
void smp_coll_reduce_int(smp_coll_t handle, int num_addrs,  
			 int *dst, int root, int *const srclist[], 
			 size_t n_ints, smp_coll_red_op_t op, RED_FN_INT fnptr, int flags);
#endif

#if INLINE_ALL_COLLECTIVES
#define smp_coll_reduce_double(HANDLE, NUM_ADDRS, DST, ROOT, SRCLIST, NDOUBLES, OP, FNPTR, FLAGS) do {	\
    (*(HANDLE)->red_double_fns[(HANDLE)->curr_red_int_routine])((HANDLE), (NUM_ADDRS), (DST), (ROOT), (SRCLIST), (NDOUBLES), (OP), (FNPTR), (FLAGS)); \
}while(0)
#else
void smp_coll_reduce_double(smp_coll_t handle, int num_addrs,  
			    double *dst, int root, double *const srclist[], 
			    size_t n_doubles, smp_coll_red_op_t op, RED_FN_DBL fnptr, int flags);
#endif


#if INLINE_ALL_COLLECTIVES
#define smp_coll_all_reduce_int(HANDLE, NUM_ADDRS, DSTLIST, SRCLIST, NINTS, OP, FNPTR, FLAGS) do {\
(*(HANDLE)->allred_int_fns[(HANDLE)->curr_allred_int_routine])((HANDLE), (NUM_ADDRS), (DSTLIST), (SRCLIST), (NINTS), (OP), (FNPTR), (FLAGS));\
}while(0)
#else
void smp_coll_all_reduce_int(smp_coll_t handle, int num_addrs,  
                             int *const dstlist[], int *const srclist[], 
                             size_t n_ints, smp_coll_red_op_t op, RED_FN_INT fnptr, int flags);
#endif


#if INLINE_ALL_COLLECTIVES
#define smp_coll_all_reduce_double(HANDLE, NUM_ADDRS, DSTLIST, SRCLIST, NDBLS, OP, FNPTR, FLAGS) do {\
(*(HANDLE)->allred_double_fns[(HANDLE)->curr_allred_double_routine])((HANDLE), (NUM_ADDRS), (DSTLIST), (SRCLIST), (NDBLS), (OP), (FNPTR), (FLAGS));\
}while(0)
#else
void smp_coll_all_reduce_double(smp_coll_t handle, int num_addrs,  
                             double *const dstlist[], double *const srclist[], 
                             size_t nelem, smp_coll_red_op_t op, RED_FN_DBL fnptr, int flags);
#endif

#endif

#define SMP_COLL_NO_SYNC 1<<0
#define SMP_COLL_MY_SYNC 1<<1
#define SMP_COLL_ALL_SYNC 1<<2
#endif
