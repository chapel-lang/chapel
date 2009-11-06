/*   $Source: /var/local/cvs/gasnet/smp-conduit/gasnet_extended_coll.c,v $
 *     $Date: 2009/10/28 02:49:59 $
 * $Revision: 1.3 $
 * Description: Shared Memory Collectives
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_extended_internal.h>
#include <gasnet_coll_internal.h>
#include <gasnet_coll_autotune_internal.h>
#include <smp-collectives/smp_coll.h>
#include <smp-collectives/smp_coll_bcast_scatter_gather.c>

#define GASNETE_COLL_EVERY_IN_SYNC_FLAG GASNET_COLL_IN_NOSYNC | GASNET_COLL_IN_MYSYNC | GASNET_COLL_IN_ALLSYNC 
#define GASNETE_COLL_EVERY_OUT_SYNC_FLAG GASNET_COLL_OUT_NOSYNC | GASNET_COLL_OUT_MYSYNC | GASNET_COLL_OUT_ALLSYNC 
#define GASNETE_COLL_EVERY_SYNC_FLAG GASNETE_COLL_EVERY_IN_SYNC_FLAG | GASNETE_COLL_EVERY_OUT_SYNC_FLAG

#if GASNETE_COLL_CONDUIT_COLLECTIVES
gasnet_coll_handle_t gasnete_coll_smp_bcast_flat_get(gasnet_team_handle_t team,
                                                 void * const dstlist[],
                                                 gasnet_image_t srcimage, void *src,
                                                 size_t nbytes, int flags, 
                                                 gasnete_coll_implementation_t coll_params, 
                                                 uint32_t sequence
                                                 GASNETE_THREAD_FARG) {
  
  
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
#if GASNET_PAR
  gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
#endif
  if(!(flags & GASNET_COLL_IN_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(dstlist[td->my_local_image], src, nbytes);
  if(!(flags & GASNET_COLL_OUT_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  return GASNET_COLL_INVALID_HANDLE;
}

gasnet_coll_handle_t gasnete_coll_smp_bcast_flat_put(gasnet_team_handle_t team,
                                            void * const dstlist[],
                                            gasnet_image_t srcimage, void *src,
                                            size_t nbytes, int flags, 
                                            gasnete_coll_implementation_t coll_params, 
                                            uint32_t sequence
                                            GASNETE_THREAD_FARG) {
  
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
#if GASNET_PAR
  gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
#endif
  if(!(flags & GASNET_COLL_IN_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  /*regardless of SINGLE or LOCAL dstlist contains as many addresses as the number of images*/
  if(td->my_local_image == srcimage) {
    gasnete_coll_local_broadcast(team->my_images, dstlist, src, nbytes); 
  }
  if(!(flags & GASNET_COLL_OUT_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  return GASNET_COLL_INVALID_HANDLE;
}

gasnet_coll_handle_t gasnete_coll_smp_bcast_tree_intflags(gasnet_team_handle_t team,
                                            void * const dstlist[],
                                            gasnet_image_t srcimage, void *src,
                                            size_t nbytes, int flags, 
                                            gasnete_coll_implementation_t coll_params, 
                                            uint32_t sequence
                                            GASNETE_THREAD_FARG) {
  
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  gasneti_assert(coll_params->num_params >= 1);
#if GASNET_PAR
  gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
#endif
  
  if(!(flags & GASNET_COLL_IN_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  smp_coll_broadcast_tree_flag(td->smp_coll_handle, team->my_images, dstlist, src, 
                                 nbytes, flags, coll_params->param_list[0]);
  if(!(flags & GASNET_COLL_OUT_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  return GASNET_COLL_INVALID_HANDLE;
}

extern gasnet_coll_handle_t
gasnete_coll_smp_reduceM_flat(gasnet_team_handle_t team,
                                gasnet_image_t dstimage, void *dst,
                                void * const srclist[], size_t src_blksz, size_t src_offset,
                                size_t elem_size, size_t elem_count,
                                gasnet_coll_fn_handle_t func, int func_arg,
                                int flags, 
                                gasnete_coll_implementation_t coll_params,
                                uint32_t sequence
                              GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
#if GASNET_PAR
  gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
#endif
  if(!(flags & GASNET_COLL_IN_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  if(td->my_local_image == dstimage) {
    gasnete_coll_local_reduce(team->my_images, (void*) dst, srclist,
                              elem_size, elem_count, func, func_arg);
  }
  if(!(flags & GASNET_COLL_OUT_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  return GASNET_COLL_INVALID_HANDLE;
  
}
extern gasnet_coll_handle_t
gasnete_coll_smp_scatM_flat_put(gasnet_team_handle_t team,
                            void * const dstlist[],
                            gasnet_image_t srcimage, void *src,
                            size_t nbytes, size_t dist, int flags,
                            gasnete_coll_implementation_t coll_params,
                            uint32_t sequence
                            GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
#if GASNET_PAR
  gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
#endif
  if(!(flags & GASNET_COLL_IN_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  
  if(td->my_local_image == srcimage) {
    int i;
    for(i=0; i<team->my_images; i++) {
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(dstlist[i], ((uint8_t*)src)+i*dist, nbytes);
    }
  }
  if(!(flags & GASNET_COLL_OUT_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  return GASNET_COLL_INVALID_HANDLE;
}

gasnet_coll_handle_t gasnete_coll_smp_scatM_flat_get(gasnet_team_handle_t team,
                                                     void * const dstlist[],
                                                     gasnet_image_t srcimage, void *src,
                                                     size_t nbytes, size_t dist, int flags,
                                                     gasnete_coll_implementation_t coll_params,
                                                     uint32_t sequence
                                                     GASNETE_THREAD_FARG) {
  
  
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
#if GASNET_PAR
  gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
#endif
  if(!(flags & GASNET_COLL_IN_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(dstlist[td->my_local_image], ((uint8_t*)src)+td->my_image*dist, nbytes);
  if(!(flags & GASNET_COLL_OUT_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  return GASNET_COLL_INVALID_HANDLE;
}
gasnet_coll_handle_t
gasnete_coll_smp_gathM_flat_put(gasnet_team_handle_t team,
                                gasnet_image_t dstimage, void *dst,
                                void * const srclist[],
                                size_t nbytes, size_t dist, int flags,
                                gasnete_coll_implementation_t coll_params,
                                uint32_t sequence
                                GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
#if GASNET_PAR
  gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
#endif
  if(!(flags & GASNET_COLL_IN_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(((uint8_t*)dst)+td->my_image*dist, srclist[td->my_local_image], nbytes);
  if(!(flags & GASNET_COLL_OUT_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  return GASNET_COLL_INVALID_HANDLE;
}

gasnet_coll_handle_t
gasnete_coll_smp_gathM_flat_get(gasnet_team_handle_t team,
                                gasnet_image_t dstimage, void *dst,
                                void * const srclist[],
                                size_t nbytes, size_t dist, int flags,
                                gasnete_coll_implementation_t coll_params,
                                uint32_t sequence
                                GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
#if GASNET_PAR
  gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
#endif
  
  if(!(flags & GASNET_COLL_IN_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  if(td->my_image == dstimage) {
    int i;
    for(i=0; i<team->my_images; i++) {
      GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(((uint8_t*)dst)+i*dist,srclist[i],nbytes);
    }
  }
  if(!(flags & GASNET_COLL_OUT_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  return GASNET_COLL_INVALID_HANDLE;
}

gasnet_coll_handle_t
gasnete_coll_smp_gath_allM_flat_put(gasnet_team_handle_t team,
                                    void * const dstlist [],
                                    void * const srclist[],
                                    size_t nbytes, int flags,
                                    gasnete_coll_implementation_t coll_params,
                                    uint32_t sequence
                                    GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  int dst;
#if GASNET_PAR
  gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
#endif
  if(!(flags & GASNET_COLL_IN_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  for (dst = td->my_image+1; dst<team->my_images; dst++) {
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(dstlist[dst], nbytes, td->my_image),
                                        srclist[td->my_image], nbytes);
  }
  for (dst = 0; dst<=td->my_image; dst++) {
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(dstlist[dst], nbytes, td->my_image),
                                        srclist[td->my_image], nbytes);
  }
  if(!(flags & GASNET_COLL_OUT_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  return GASNET_COLL_INVALID_HANDLE;
}

gasnet_coll_handle_t
gasnete_coll_smp_gath_allM_flat_get(gasnet_team_handle_t team,
                                    void * const dstlist [],
                                    void * const srclist[],
                                    size_t nbytes, int flags,
                                    gasnete_coll_implementation_t coll_params,
                                    uint32_t sequence
                                    GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  int src;
#if GASNET_PAR
  gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
#endif
  if(!(flags & GASNET_COLL_IN_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  for (src = td->my_image+1; src<team->my_images; src++) {
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(dstlist[td->my_image], nbytes, src),
                                        srclist[src], nbytes);
  }
  for (src = 0; src<=td->my_image; src++) {
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(dstlist[td->my_image], nbytes, src),
                                        srclist[src], nbytes);
  }
  if(!(flags & GASNET_COLL_OUT_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  return GASNET_COLL_INVALID_HANDLE;
}

gasnet_coll_handle_t
gasnete_coll_smp_exchgM_flat_put(gasnet_team_handle_t team,
                                    void * const dstlist [],
                                    void * const srclist[],
                                    size_t nbytes, int flags,
                                    gasnete_coll_implementation_t coll_params,
                                    uint32_t sequence
                                    GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  int dst;
#if GASNET_PAR
  gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
#endif
  if(!(flags & GASNET_COLL_IN_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  for (dst = td->my_image+1; dst<team->my_images; dst++) {
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(dstlist[dst], nbytes, td->my_image),
                                        gasnete_coll_scale_ptr(srclist[td->my_image], nbytes, dst), nbytes);
  }
  for (dst = 0; dst<=td->my_image; dst++) {
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(dstlist[dst], nbytes, td->my_image),
                                        gasnete_coll_scale_ptr(srclist[td->my_image], nbytes, dst), nbytes);
  }
  if(!(flags & GASNET_COLL_OUT_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  return GASNET_COLL_INVALID_HANDLE;
}

gasnet_coll_handle_t
gasnete_coll_smp_exchgM_flat_get(gasnet_team_handle_t team,
                                    void * const dstlist [],
                                    void * const srclist[],
                                    size_t nbytes, int flags,
                                    gasnete_coll_implementation_t coll_params,
                                    uint32_t sequence
                                    GASNETE_THREAD_FARG) {
  gasnete_coll_threaddata_t *td = GASNETE_COLL_MYTHREAD;
  int src;
#if GASNET_PAR
  gasneti_assert(!(flags & GASNETE_COLL_THREAD_LOCAL));
#endif
  if(!(flags & GASNET_COLL_IN_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  for (src = td->my_image+1; src<team->my_images; src++) {
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(dstlist[td->my_image], nbytes, src),
                                        gasnete_coll_scale_ptr(srclist[src], nbytes, td->my_image), nbytes);
  }
  for (src = 0; src<=td->my_image; src++) {
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(dstlist[td->my_image], nbytes, src),
                                        gasnete_coll_scale_ptr(srclist[src], nbytes, td->my_image), nbytes);

  }
  if(!(flags & GASNET_COLL_OUT_NOSYNC)) smp_coll_barrier(td->smp_coll_handle,0);
  return GASNET_COLL_INVALID_HANDLE;
}

void gasnete_coll_register_conduit_collectives(gasnete_coll_autotune_info_t* info) {
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCAST_SMP_FLAT_GET] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0 /*works for all addresses since it's just a memcpy on a the local node*/, 
#if GASNET_PAR
                                           GASNETE_COLL_THREAD_LOCAL /*the algorithms will not work for thread local since there is no method of posting local addresses*/,
#else
                                           0,
#endif                                             
                                           0, 0, 0, 0, NULL, 
                                           (void*) gasnete_coll_smp_bcast_flat_get, "SMP_BCAST_FLAT_GET");
  
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCAST_SMP_FLAT_PUT] =
    gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0 /*works for all addresses since it's just a memcpy on a the local node*/, 
#if GASNET_PAR
                                             GASNETE_COLL_THREAD_LOCAL /*the algorithms will not work for thread local since there is no method of posting local addresses*/,
#else
                                             0,
#endif                                             
                                             0, 0, 0, 0, NULL, 
                                             (void*) gasnete_coll_smp_bcast_flat_put, "SMP_BCAST_FLAT_PUT");

  {
    struct gasnet_coll_tuning_parameter_t tuning_params[1]=
    { 
      {GASNETE_COLL_SMP_COLL_TREE_RADIX, 2, MAX(2,info->team->my_images), 2, GASNET_COLL_TUNING_STRIDE_MULTIPLY}
    }; 
    
  info->collective_algorithms[GASNET_COLL_BROADCASTM_OP][GASNETE_COLL_BROADCAST_SMP_TREE_INTFLAGS] =
	  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_BROADCASTM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                             0 /*works for all addresses since it's just a memcpy on a the local node*/,
#if GASNET_PAR
                                             GASNETE_COLL_THREAD_LOCAL /*the algorithms will not work for thread local since there is no method of posting local addresses*/,
#else
                                             0,
#endif                                             
                                             0, 0, 0, 1, tuning_params,  
                                             (void*) gasnete_coll_smp_bcast_tree_intflags, "SMP_BCAST_TREE_INTFLAGS");
  }
  
  info->collective_algorithms[GASNET_COLL_REDUCEM_OP][GASNETE_COLL_REDUCE_SMP_FLAT] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_REDUCEM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0 /*works for all addresses since it's just a memcpy on a the local node*/, 
#if GASNET_PAR
                                           GASNETE_COLL_THREAD_LOCAL /*the algorithms will not work for thread local since there is no method of posting local addresses*/,
#else
                                           0,
#endif                                             
                                           0, 0, 0, 0, NULL, 
                                           (void*) gasnete_coll_smp_reduceM_flat, "SMP_REDUCE_FLAT_PUT");
  
  info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTER_SMP_FLAT_PUT] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0 /*works for all addresses since it's just a memcpy on a the local node*/, 
#if GASNET_PAR
                                           GASNETE_COLL_THREAD_LOCAL /*the algorithms will not work for thread local since there is no method of posting local addresses*/,
#else
                                           0,
#endif                                             
                                           0, 0, 0, 0, NULL, 
                                           (void*) gasnete_coll_smp_scatM_flat_put, "SMP_SCATTER_FLAT_PUT");
  
  info->collective_algorithms[GASNET_COLL_SCATTERM_OP][GASNETE_COLL_SCATTER_SMP_FLAT_GET] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_SCATTERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0 /*works for all addresses since it's just a memcpy on a the local node*/, 
#if GASNET_PAR
                                           GASNETE_COLL_THREAD_LOCAL /*the algorithms will not work for thread local since there is no method of posting local addresses*/,
#else
                                           0,
#endif                                             
                                           0, 0, 0, 0, NULL, 
                                           (void*) gasnete_coll_smp_scatM_flat_get, "SMP_SCATTER_FLAT_GET");
  
  info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHER_SMP_FLAT_PUT] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0 /*works for all addresses since it's just a memcpy on a the local node*/, 
#if GASNET_PAR
                                           GASNETE_COLL_THREAD_LOCAL /*the algorithms will not work for thread local since there is no method of posting local addresses*/,
#else
                                           0,
#endif                                             
                                           0, 0, 0, 0, NULL, 
                                           (void*) gasnete_coll_smp_gathM_flat_put, "SMP_GATHER_FLAT_PUT");
  
  info->collective_algorithms[GASNET_COLL_GATHERM_OP][GASNETE_COLL_GATHER_SMP_FLAT_GET] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHERM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0 /*works for all addresses since it's just a memcpy on a the local node*/, 
#if GASNET_PAR
                                           GASNETE_COLL_THREAD_LOCAL /*the algorithms will not work for thread local since there is no method of posting local addresses*/,
#else
                                           0,
#endif                                             
                                           0, 0, 0, 0, NULL, 
                                           (void*) gasnete_coll_smp_gathM_flat_get, "SMP_GATHER_FLAT_GET");
  
  info->collective_algorithms[GASNET_COLL_GATHER_ALLM_OP][GASNETE_COLL_GATHER_ALL_SMP_FLAT_PUT] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALLM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0 /*works for all addresses since it's just a memcpy on a the local node*/, 
#if GASNET_PAR
                                           GASNETE_COLL_THREAD_LOCAL /*the algorithms will not work for thread local since there is no method of posting local addresses*/,
#else
                                           0,
#endif                                             
                                           0, 0, 0, 0, NULL, 
                                           (void*) gasnete_coll_smp_gath_allM_flat_put, "SMP_GATHER_ALL_FLAT_PUT");
  
  info->collective_algorithms[GASNET_COLL_GATHER_ALLM_OP][GASNETE_COLL_GATHER_ALL_SMP_FLAT_GET] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_GATHER_ALLM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0 /*works for all addresses since it's just a memcpy on a the local node*/, 
#if GASNET_PAR
                                           GASNETE_COLL_THREAD_LOCAL /*the algorithms will not work for thread local since there is no method of posting local addresses*/,
#else
                                           0,
#endif                                             
                                           0, 0, 0, 0, NULL, 
                                           (void*) gasnete_coll_smp_gath_allM_flat_get, "SMP_GATHER_ALL_FLAT_GET");
  
  info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGE_SMP_FLAT_PUT] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGEM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0 /*works for all addresses since it's just a memcpy on a the local node*/, 
#if GASNET_PAR
                                           GASNETE_COLL_THREAD_LOCAL /*the algorithms will not work for thread local since there is no method of posting local addresses*/,
#else
                                           0,
#endif                                             
                                           0, 0, 0, 0, NULL, 
                                           (void*) gasnete_coll_smp_exchgM_flat_put, "SMP_EXCHANGE_FLAT_PUT");
  
  info->collective_algorithms[GASNET_COLL_EXCHANGEM_OP][GASNETE_COLL_EXCHANGE_SMP_FLAT_GET] =
  gasnete_coll_autotune_register_algorithm(info->team, GASNET_COLL_EXCHANGEM_OP, GASNETE_COLL_EVERY_SYNC_FLAG,
                                           0 /*works for all addresses since it's just a memcpy on a the local node*/, 
#if GASNET_PAR
                                           GASNETE_COLL_THREAD_LOCAL /*the algorithms will not work for thread local since there is no method of posting local addresses*/,
#else
                                           0,
#endif                                             
                                           0, 0, 0, 0, NULL, 
                                           (void*) gasnete_coll_smp_exchgM_flat_get, "SMP_EXCHANGE_FLAT_GET");
  

}
#endif
