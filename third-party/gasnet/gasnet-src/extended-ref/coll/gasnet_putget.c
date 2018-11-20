/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_putget.c $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <coll/gasnet_coll_internal.h>
#include <coll/gasnet_trees.h>
#include <coll/gasnet_scratch.h>
#include <coll/gasnet_autotune_internal.h>
#include <gasnet_vis.h>

#define USE_CONSENSUS_BARRIER 1
typedef struct {int num_handles; gex_Event_t *handles;} gasnete_coll_handle_vec_t;
/*for the segmented algorithms limit the maximum number of subcollectives to 2048*/


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcast_nb() */

/* bcast TreePutScratch */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */

/* Naturally IN_MYSYNC, OUT_MYSYNC and should only be used when IN_MYSYNC is needed*/
/* Goes through the scratch space */
/* max size is MaxLongRequest */
static int gasnete_coll_pf_bcast_TreePutScratch(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_local_tree_geom_t *geom = data->tree_geom;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  gex_Rank_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  int result = 0;
  int child;
  
  switch (data->state) {
    case 0: /*scratch alloc*/
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETI_THREAD_PASS))
          break;
      }
      data->state = 1; GASNETI_FALLTHROUGH

      case 1: /*thread barrier*/
      if (!gasnete_coll_generic_all_threads(data)) {
        break;
      }
      data->state = 2; GASNETI_FALLTHROUGH

      case 2:   /* Optional IN barrier over the SAME tree */
      if ((op->flags & GASNET_COLL_IN_ALLSYNC) &&
          !gasnete_coll_generic_upsync_acq(op, args->srcnode, 0, child_count)) {
        break;
      }
      data->state = 3; GASNETI_FALLTHROUGH
      
      case 3:
      if (op->team->myrank == args->srcnode) {
        for (child = 0; child < child_count; child++) {
          
          gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[child]), 
                                          (int8_t*)op->team->scratch_segs[children[child]].addr+op->scratchpos[child], 
                                          args->src, args->nbytes, 0, 1);
          
        }
        GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
        
        for (child = 0; child < child_count; child++) {
          
          gasnete_coll_p2p_signalling_put/*Async*/(op, GASNETE_COLL_REL2ACT(op->team, children[child]), 
                                                   (int8_t*)op->team->scratch_segs[children[child]].addr+op->scratchpos[child], 
                                                   (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, 
                                                   args->nbytes, 0, 1);
          
        }
        
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, args->nbytes);
        
      } else {
        break;	/* Waiting for parent to push data and signal */
      }
      data->state = 4; GASNETI_FALLTHROUGH
      
      
      case 4:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      /*free up the scratch space used by this op*/
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}
extern gex_Event_t
gasnete_coll_bcast_TreePutScratch(gasnet_team_handle_t team,
                                  void *dst,
                                  gasnet_image_t srcimage, void *src,
                                  size_t nbytes, int flags,
                                  gasnete_coll_implementation_t coll_params,
                                  uint32_t sequence
                                  GASNETI_THREAD_FARG)
{
 /*never allocated an insync barrier since it is folded in with the collective*/
  int options =
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF (flags & GASNET_COLL_OUT_ALLSYNC) |
  GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH;
  
  gasneti_assert(nbytes <= gex_AM_LUBRequestLong());

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_TreePutScratch, options,
                                           gasnete_coll_local_tree_geom_fetch(coll_params->tree_type, 
                                                                              srcimage, team),
                                           sequence, coll_params->num_params, coll_params->param_list
                                           GASNETI_THREAD_PASS);
	
}

static int gasnete_coll_pf_bcast_TreePutSeg(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1; GASNETI_FALLTHROUGH
      
      case 1:	/* Initiate data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec;
      size_t seg_size;
      int num_segs;
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
      
      gasnete_coll_implementation_t impl = gasnete_coll_get_implementation();

      
      gex_Rank_t srcproc = args->srcnode;
      size_t sent_bytes=0;
      int i;
      
      impl->fn_ptr = NULL;
      /*strip the last argument off which contains the pipeline segment size*/
      impl->num_params = op->num_coll_params;
      GASNETI_MEMCPY_SAFE_IDENTICAL(impl->param_list, op->param_list, sizeof(uint32_t)*op->num_coll_params);
      impl->tree_type = data->tree_geom->tree_type;
    
      seg_size = (size_t) op->param_list[0];
      num_segs = ((args->nbytes % seg_size) == 0 ? args->nbytes/seg_size : (args->nbytes/seg_size)+1);
      
      data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t));
      handle_vec = data->private_data;
      handle_vec->num_handles = num_segs;
      handle_vec->handles = gasneti_malloc(sizeof(gex_Event_t)*num_segs);
      
      {
        for(i=0; i<num_segs-1; i++) {
          handle_vec->handles[i] = gasnete_coll_bcast_TreePutScratch(op->team, gasnete_coll_scale_ptr(args->dst, sent_bytes, 1), 
                                                                     srcproc, gasnete_coll_scale_ptr(args->src, sent_bytes, 1), seg_size, flags, 
                                                                     impl, op->sequence+i+1 GASNETI_THREAD_PASS);
          gasnete_coll_save_event(&handle_vec->handles[i]);
          sent_bytes+=seg_size;
        }
        handle_vec->handles[i] = gasnete_coll_bcast_TreePutScratch(op->team, gasnete_coll_scale_ptr(args->dst, sent_bytes, 1), 
                                                                   srcproc, 
                                                                   gasnete_coll_scale_ptr(args->src, sent_bytes, 1), args->nbytes-sent_bytes, flags, 
                                                                   impl, op->sequence+i+1 GASNETI_THREAD_PASS);
        gasnete_coll_save_event(&handle_vec->handles[i]);          
      }
      gasnete_coll_free_implementation(impl);
    }
      data->state = 2; GASNETI_FALLTHROUGH
      
      case 2:	/* Sync data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec = (gasnete_coll_handle_vec_t*) data->private_data;
      if (!gasnete_coll_generic_coll_sync(handle_vec->handles, handle_vec->num_handles GASNETI_THREAD_PASS)) {
        break;
      }
      gasneti_free(handle_vec->handles);
    }   
      data->state = 3; GASNETI_FALLTHROUGH

      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gex_Event_t
gasnete_coll_bcast_TreePutSeg(gasnet_team_handle_t team,
                              void * dst,
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, int flags,
                             gasnete_coll_implementation_t coll_params,
                              uint32_t sequence
                              GASNETI_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE)) | 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE));

  
  size_t seg_size;
  uint32_t num_segs;
  
  gasneti_assert(coll_params->num_params >= 1);
  seg_size = (size_t)coll_params->param_list[0];
  num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  gasneti_assert(num_segs < GASNETE_COLL_MAX_NUM_SEGS);


  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_TreePutSeg, options,
                                           gasnete_coll_local_tree_geom_fetch(coll_params->tree_type, 
                                                                              srcimage, team),
                                           (flags & GASNETE_COLL_SUBORDINATE ? sequence : num_segs), coll_params->num_params, coll_params->param_list
                                           GASNETI_THREAD_PASS);
}

static int gasnete_coll_pf_bcast_ScatterAllgather(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result =0;
  
  switch (data->state) {
  case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1; GASNETI_FALLTHROUGH
      
  case 1:	/* Initiate Scatter */
      {
        gex_Event_t *handle;
        uint8_t *tempspace;
        size_t seg_size = (args->nbytes)/op->team->total_ranks;
        size_t remainder = (args->nbytes) % op->team->total_ranks;
        int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
        gex_Rank_t srcproc = args->srcnode;
       
        data->private_data = gasneti_malloc(sizeof(gex_Event_t)*2+seg_size);
        handle = (gex_Event_t*) data->private_data;
        
        tempspace = ((uint8_t*) data->private_data) + sizeof(gex_Event_t)*2; 

        handle[0] = gasnete_coll_scatter_nb_default(op->team, tempspace, srcproc, args->src, seg_size, flags, op->sequence+1 GASNETI_THREAD_PASS);
        gasnete_coll_save_event(&handle[0]);
        
        if(remainder > 0) {

          handle[1] = gasnete_coll_broadcast_nb_default(op->team, 
                                                        gasnete_coll_scale_ptr(args->dst,op->team->total_ranks,seg_size), 
                                                        srcproc, 
                                                        gasnete_coll_scale_ptr(args->src,op->team->total_ranks,seg_size), 
                                                        remainder, flags, op->sequence+2 GASNETI_THREAD_PASS);
        } else {
          handle[1] = GEX_EVENT_INVALID;
        }
        gasnete_coll_save_event(&handle[1]);
      }
      data->state = 2; GASNETI_FALLTHROUGH
      
  case 2:	/* Initiate all gather*/
      
      /*we only need the scatter to be done before we initiate the allgather*/
      if (!gasnete_coll_generic_coll_sync((gex_Event_t*) data->private_data, 1 GASNETI_THREAD_PASS)) {
        break;
      } else {
        gex_Event_t *handle;
        uint8_t *tempspace;
        size_t seg_size = (args->nbytes)/op->team->total_ranks;
        int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
        
        handle = (gex_Event_t*) data->private_data;
      
        tempspace = ((uint8_t*) data->private_data) + sizeof(gex_Event_t)*2; 
        if(seg_size) {
          handle[0] = gasnete_coll_gather_all_nb_default(op->team, args->dst, tempspace, seg_size, flags, op->sequence+3 GASNETI_THREAD_PASS);
        } else {
          handle[0] = GEX_EVENT_INVALID;
        }
        gasnete_coll_save_event(&handle[0]);
      }
      data->state = 3; GASNETI_FALLTHROUGH
      
  case 3:
      /*sync both the allgather and broadcast*/
      if (!gasnete_coll_generic_coll_sync((gex_Event_t*) data->private_data, 2 GASNETI_THREAD_PASS)) {
        break;
      }
      data->state = 4; GASNETI_FALLTHROUGH
      
  case 4:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gex_Event_t
gasnete_coll_bcast_ScatterAllgather(gasnet_team_handle_t team,
                              void * dst,
                              gasnet_image_t srcimage, void *src,
                              size_t nbytes, int flags,
                              gasnete_coll_implementation_t coll_params,
                              uint32_t sequence
                              GASNETI_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC | GASNETE_COLL_GENERIC_OPT_OUTSYNC;
  
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
                                           &gasnete_coll_pf_bcast_ScatterAllgather, options,
                                           NULL, 
                                           3 + 2*GASNETE_COLL_MAX_NUM_SEGS +team->total_ranks, /*scatter and broadcast can initiate upto GASNETE_COLL_MAX_NUM_SEGS sub scatters, broadcasts and 
   all gather can initiate upto total ranks sub collectives*/
                                           
                                           coll_params->num_params, coll_params->param_list
                                           GASNETI_THREAD_PASS);
}


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatter_nb() */

/* scat Put: root node performs carefully ordered puts */
/* Valid for SINGLE and LOCAL, any size < scratch size ... since we write into the scratch space we need not worry about whether the sender knows the dest address */
static int gasnete_coll_pf_scat_TreePut(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_local_tree_geom_t *geom = data->tree_geom;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  gex_Rank_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);
  const gex_Rank_t child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  int result = 0,p=1,i,j;
  size_t sent_bytes=0;
  
  
  switch (data->state) {
    case 0: /*scratch alloc*/
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETI_THREAD_PASS))
          break;
      }
      data->state = 1; GASNETI_FALLTHROUGH

      case 1:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data)){
        break;
      }
      data->state = 2; GASNETI_FALLTHROUGH
      
      case 2:    /* Optional IN barrier over the SAME tree */
      if ((op->flags & GASNET_COLL_IN_ALLSYNC) &&
          !gasnete_coll_generic_upsync_acq(op, args->srcnode, 0, child_count)) {
        break;
      }
      data->state = 3; GASNETI_FALLTHROUGH
      
      case 3:
      if (op->team->myrank == args->srcnode) {
        if(args->dist!=args->nbytes) {
          int8_t *myscratchpos = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
          int8_t *send_arr;
          gasnete_begin_nbi_accessregion(0,1 GASNETI_THREAD_PASS);
          /*compress the data and send it*/
          for(i=0,p=0; i<child_count; i++) {
             gex_Rank_t child = children[i];
            send_arr = myscratchpos+p*args->nbytes;
            for(j=0; j<geom->subtree_sizes[i]; j++,p++) {
              size_t src_pos = geom->child_offset[i]+j+1;
              src_pos=(src_pos+args->srcnode)%op->team->total_ranks;
              GASNETE_FAST_UNALIGNED_MEMCPY(myscratchpos+p*args->nbytes, gasnete_coll_scale_ptr(args->src, args->dist, src_pos), args->nbytes);
            }
            if(op->flags & GASNET_COLL_OUT_MYSYNC) {
              /* use AMLong*/
              gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                              send_arr,
                                              args->nbytes*geom->subtree_sizes[i], 0, 1);              
            } else {
              {
                /* else if i am sending to internal node AM long into scratch space*/
                gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                send_arr, 
                                                args->nbytes*geom->subtree_sizes[i], 0, 1);              
                
              }
            }
          }
              
          GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, gasnete_coll_scale_ptr(args->src,args->dist,(op->team->myrank)), args->nbytes);
          data->handle = gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS);
          gasnete_coll_save_event(&data->handle);
        } else {
          int8_t* src_arr;
          int8_t* scratch_space= (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
          gasnete_begin_nbi_accessregion(0,1 GASNETI_THREAD_PASS);
          
          sent_bytes+=args->nbytes;
          if(args->srcnode == 0) {
            /*if 0 is the source node then there's no need to shift the array around we can do it direclty from the source*/
            src_arr = args->src;
          } else {
            /*perform the rotations*/
            src_arr = scratch_space;
            gasneti_assert(geom->num_rotations==1); /*for now only works w/ one level trees*/
            gasnete_coll_local_rotate_left(scratch_space, args->src, 
                                      args->nbytes, op->team->total_ranks, geom->rotation_points[0]);
          }
          for(i=0; i<child_count; i++) {
            gex_Rank_t child = children[i];
            int8_t *send_arr = gasnete_coll_scale_ptr(src_arr,(geom->child_offset[i]+1),args->nbytes);

            if(op->flags & GASNET_COLL_OUT_MYSYNC) {
              /* use AMLong*/
              gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                               send_arr,
                                              args->nbytes*geom->subtree_sizes[i], 0, 1);              
            } else {
              {
                /* else if i am sending to internal node AM long into scratch space*/
                  gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                  (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                  send_arr, 
                                                  args->nbytes*geom->subtree_sizes[i], 0, 1);              
                  
              }
            }
          }
          GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, src_arr, args->nbytes);
          data->handle = gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS);
          gasnete_coll_save_event(&data->handle);
        }
      } else if(data->p2p->state[0]){
        int8_t *scratchspace = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
        /*read memory barrier to ensure that the reading of the data doesn't preceed the flag*/
        gasneti_sync_reads();
        gasnete_begin_nbi_accessregion(0,1 GASNETI_THREAD_PASS);
        /*skip the first slot of the input array since it is destined for me*/
        for(i=0; i<child_count; i++) {
          gex_Rank_t child = children[i];
          {
            /*need to stick the data into scratch space*/
              gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                              gasnete_coll_scale_ptr(scratchspace,(geom->child_offset[i]+1),args->nbytes),
                                              args->nbytes*geom->subtree_sizes[i], 0, 1);
          }
          sent_bytes+=geom->subtree_sizes[i]*args->nbytes;
        }
        data->handle = gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS);
        gasnete_coll_save_event(&data->handle);
        
        
        /* In the case of Mysync the data is always sent to the scratch space so copy it out*/
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, scratchspace, args->nbytes);
        
      } else {
        break; /* data not yet arrived*/
      }
      data->state = 4; GASNETI_FALLTHROUGH
      
      case 4: /* wait for all puts to finish*/
      if (data->handle != GEX_EVENT_INVALID) {
        break;
      }
      data->state = 5; GASNETI_FALLTHROUGH
      
      case 5:	/* Final Out Barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
        
      }
      data->state = 6; GASNETI_FALLTHROUGH
      
      case 6: /*done*/    
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      
  }
  
  return result;
}
extern gex_Event_t
gasnete_coll_scat_TreePut(gasnet_team_handle_t team,
                          void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, size_t dist, int flags, 
                          gasnete_coll_implementation_t coll_params,   
                          uint32_t sequence
                          GASNETI_THREAD_FARG)
{
  int options = 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  
  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
                                         &gasnete_coll_pf_scat_TreePut, options,
                                         gasnete_coll_local_tree_geom_fetch(coll_params->tree_type,
                                                                            srcimage, team),
                                         sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}  

/*like the scatter treePut but data is not resuffled, instead it is sent from
  directly where it is*/
static int gasnete_coll_pf_scat_TreePutNoCopy(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_local_tree_geom_t *geom = data->tree_geom;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  gex_Rank_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);
  const gex_Rank_t child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  int result = 0,i;
  size_t sent_bytes=0;
  
  switch (data->state) {
  case 0: /*scratch alloc*/
  
      if(op->scratch_req) {
        if(!gasnete_coll_scratch_alloc_nb(op GASNETI_THREAD_PASS))
          break;
      }
      data->state = 1; GASNETI_FALLTHROUGH

      case 1:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data)){
        break;
      }
      data->state = 2; GASNETI_FALLTHROUGH
      
      case 2:    /* Optional IN barrier over the SAME tree */
      if ((op->flags & GASNET_COLL_IN_ALLSYNC) &&
          !gasnete_coll_generic_upsync_acq(op, args->srcnode, 0, child_count)) {
        break;
      }
      data->state = 3; GASNETI_FALLTHROUGH
      
      case 3:
     
      if (op->team->myrank == args->srcnode) {
        if(args->dist!=args->nbytes) {
          gasneti_fatalerror("not yet supported!");
        } else {
          gasneti_assert(geom->num_rotations == 1);

          gasnete_begin_nbi_accessregion(0,1 GASNETI_THREAD_PASS);
          for(i=0; i<child_count; i++) {
            gex_Rank_t child = children[i];

            if(children[i]+geom->subtree_sizes[i] <= op->team->total_ranks) {
              /*can do one put since data is already contiguous*/
              int8_t *send_arr = gasnete_coll_scale_ptr(args->src,(geom->child_offset[i]+1+op->team->myrank)%op->team->total_ranks,args->nbytes);

              {
                gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                                (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                                send_arr,
                                                args->nbytes*geom->subtree_sizes[i], 0, 1);
                
              }
            } else {
              int8_t *send_arr = gasnete_coll_scale_ptr(args->src,(geom->child_offset[i]+1+op->team->myrank),args->nbytes);
              int8_t *send_arr2 = args->src;
              size_t first_part = op->team->total_ranks - children[i];
              size_t second_part = geom->subtree_sizes[i]-first_part;
              /*we dont' need the case for a direct put here because if there is exactly one node in the subtree then the top condition will always be true*/
              /*need to do two puts w/ the wrap*/

              gasnete_coll_p2p_counting_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]),
                                            (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                            send_arr,
                                            args->nbytes*first_part,0);
              gasnete_coll_p2p_counting_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]),
                                            (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i]+first_part*args->nbytes, 
                                            send_arr2,
                                            args->nbytes*second_part,0);
              
            }
          }
          data->handle = gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS);
          gasnete_coll_save_event(&data->handle);
        }
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, gasnete_coll_scale_ptr(args->src,args->dist,(op->team->myrank)), args->nbytes);
      } else if (data->p2p->state[0] 
                 || (op->team->myrank+geom->mysubtree_size > op->team->total_ranks && 
                     gasneti_weakatomic_read(&(data->p2p->counter[0]),0)==(op->flags& GASNET_COLL_IN_ALLSYNC ? child_count:0)+2)){
        int8_t *scratchspace = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
        /*read memory barrier to ensure that the reading of the data doesn't preceed the flag*/
        gasneti_sync_reads();
        gasnete_begin_nbi_accessregion(0,1 GASNETI_THREAD_PASS);
        /*skip the first slot of the input array since it is destined for me*/
        for(i=0; i<child_count; i++) {
          gex_Rank_t child = children[i];
          {
            /*need to stick the data into scratch space*/
              gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team, children[i]), 
                                              (int8_t*)op->team->scratch_segs[child].addr+op->scratchpos[i], 
                                              gasnete_coll_scale_ptr(scratchspace,(geom->child_offset[i]+1),args->nbytes),
                                              args->nbytes*geom->subtree_sizes[i], 0, 1);
          }
          sent_bytes+=geom->subtree_sizes[i]*args->nbytes;
        }
        
        data->handle = gasnete_end_nbi_accessregion(0 GASNETI_THREAD_PASS);
        gasnete_coll_save_event(&data->handle);
        
        /* In the case of Mysync or being an intermediate node  the data is always sent to the scratch space so copy it out*/
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, scratchspace, args->nbytes);
        
      } else {
        break; /* data not yet arrived*/
      }
      data->state = 4; GASNETI_FALLTHROUGH
      
      case 4: /* wait for all puts to finish*/
      if (data->handle != GEX_EVENT_INVALID) {
        break;
      }
      data->state = 5; GASNETI_FALLTHROUGH
      
      
      case 5:	/* Final Out Barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
        
      }
      data->state = 6; GASNETI_FALLTHROUGH
      
      case 6: /*done*/    
      gasnete_coll_free_scratch(op);
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      
  }
  
  return result;
}
extern gex_Event_t
gasnete_coll_scat_TreePutNoCopy(gasnet_team_handle_t team,
                          void *dst,
                          gasnet_image_t srcimage, void *src,
                          size_t nbytes, size_t dist, int flags, 
                          gasnete_coll_implementation_t coll_params,   
                          uint32_t sequence
                          GASNETI_THREAD_FARG)
{
  
  if(srcimage!=0) {
    return gasnete_coll_scat_TreePut(team, dst, srcimage, src, nbytes, dist, flags, coll_params, sequence GASNETI_THREAD_PASS);
  } else {
    int options = 
    GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
    GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
    
    return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
                                           &gasnete_coll_pf_scat_TreePutNoCopy, options,
                                           gasnete_coll_local_tree_geom_fetch(coll_params->tree_type,
                                                                              srcimage, team),
                                           sequence,coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
  }
}  

static int gasnete_coll_pf_scat_TreePutSeg(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1; GASNETI_FALLTHROUGH
      
      case 1:	/* Initiate data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec;
      size_t seg_size = gasnete_coll_get_pipe_seg_size(op->team->autotune_info, GASNET_COLL_SCATTER_OP, op->flags);
      int num_segs = ((args->nbytes % seg_size) == 0 ? args->nbytes/seg_size : (args->nbytes/seg_size)+1);
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
      gasnete_coll_implementation_t impl;
      gex_Rank_t srcproc = args->srcnode;
      size_t sent_bytes=0;
      int i;
      
      impl = gasnete_coll_get_implementation();
      data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t));
      handle_vec = data->private_data;
      handle_vec->num_handles = num_segs;
      handle_vec->handles = gasneti_malloc(sizeof(gex_Event_t)*num_segs);
      impl->fn_ptr = NULL;
      /*strip the last argument off which contains the pipeline segment size*/
      impl->num_params = op->num_coll_params;
      GASNETI_MEMCPY_SAFE_IDENTICAL(impl->param_list, op->param_list, sizeof(uint32_t)*op->num_coll_params);
      impl->tree_type = data->tree_geom->tree_type;
      
      for(i=0; i<num_segs - 1; i++) {
        /*ignore the handle returned*/
        handle_vec->handles[i] = gasnete_coll_scat_TreePut(op->team, gasnete_coll_scale_ptr(args->dst,1,sent_bytes) , srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                           seg_size, args->nbytes, flags, impl, op->sequence+i+1 GASNETI_THREAD_PASS);   
        gasnete_coll_save_event(&handle_vec->handles[i]);
        sent_bytes += seg_size;
      }
      
      handle_vec->handles[i] = gasnete_coll_scat_TreePut(op->team, gasnete_coll_scale_ptr(args->dst,1,sent_bytes) , srcproc, gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                         args->nbytes-sent_bytes, args->nbytes, flags, impl, op->sequence+i+1 GASNETI_THREAD_PASS);   
      gasnete_coll_save_event(&handle_vec->handles[i]);
      gasnete_coll_free_implementation(impl);

    }
      data->state = 2; GASNETI_FALLTHROUGH
      
      case 2:	/* Sync data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec = (gasnete_coll_handle_vec_t*) data->private_data;
      if (!gasnete_coll_generic_coll_sync(handle_vec->handles, handle_vec->num_handles GASNETI_THREAD_PASS)) {
        break;
      }
      gasneti_free(handle_vec->handles);
    }   
      data->state = 3; GASNETI_FALLTHROUGH

      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}


extern gex_Event_t
gasnete_coll_scat_TreePutSeg(gasnet_team_handle_t team,
                             void *dst,
                             gasnet_image_t srcimage, void *src,
                             size_t nbytes, size_t dist, int flags, 
                             gasnete_coll_implementation_t coll_params,    
                             uint32_t sequence
                             GASNETI_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE)) | 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE));
  
  int num_segs;
  size_t seg_size;
  gasneti_assert(coll_params->num_params >= 1);
  seg_size = coll_params->param_list[0];
  num_segs = ((nbytes % seg_size) == 0 ? nbytes/seg_size : (nbytes/seg_size)+1);
  gasneti_assert(num_segs < GASNETE_COLL_MAX_NUM_SEGS);


    return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, dist, flags,
                                           &gasnete_coll_pf_scat_TreePutSeg, options,
                                           gasnete_coll_local_tree_geom_fetch(coll_params->tree_type,
                                                                              srcimage, team),
                                           (flags & GASNETE_COLL_SUBORDINATE ? sequence : num_segs),
                                           coll_params->num_params, coll_params->param_list
                                           GASNETI_THREAD_PASS);
}  

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_nb() */

/* gath Put: all nodes perform uncoordinated puts */
/* Valid for SINGLE and LOCAL, any size < scratch size ... remote threads will not touch user buffers*/
/* XXX Note that an optimization can be made here where the data can be put into the user buffer at the root*/
/* XXX However this will make this implementation only valid for SINGLE and not LOCAL*/

static int gasnete_coll_pf_gath_TreePut(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_local_tree_geom_t *geom = data->tree_geom;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  gex_Rank_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);
  gex_Rank_t parent = GASNETE_COLL_TREE_GEOM_PARENT(geom);
  const gex_Rank_t child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  gex_Rank_t expected_count;
  gex_Rank_t child;
  int result = 0;
  int i=0;
  
  switch (data->state) {
    case 0:
      if(!gasnete_coll_scratch_alloc_nb(op GASNETI_THREAD_PASS))    break;
      data->state = 1; GASNETI_FALLTHROUGH
      
    case 1:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data)||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }

      data->state = 2; GASNETI_FALLTHROUGH
      
    case 2:	/* Local Data Movement */
      /* go up the tree with the data */
      /* copy my data into the start of the scratch space */
      
      if(child_count > 0) {
        {
          GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, 
                                        (int8_t*)args->src, args->nbytes);
          
        }
      } else if_pf(op->team->total_ranks == 1) {
        GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, 
                                      args->src, args->nbytes);
        data->state = 4;
        break;
      }
      data->state = 3; GASNETI_FALLTHROUGH
      
    case 3:
      if(op->team->myrank == args->dstnode) {        
        {
          int8_t* scratchspace =  (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
          if(gasneti_weakatomic_read(&(data->p2p->counter[0]),0) < child_count) {
            /* all children have not yet reported*/ 
            break;
          }
          gasneti_sync_reads();
          /*all children have reported so now need to reshuffle the data into the correct place*/
          if(args->nbytes == args->dist) {
            gasneti_assert(geom->num_rotations==1);
            gasnete_coll_local_rotate_right(args->dst,scratchspace, 
                                            args->nbytes, op->team->total_ranks, geom->rotation_points[0]);
          } else { 
            for(i=0; i<op->team->total_ranks; i++) {
              GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(args->dst, (i+geom->rotation_points[0])%op->team->total_ranks, args->dist),
                                            gasnete_coll_scale_ptr(scratchspace, i,args->nbytes),
                                            args->nbytes);
            } 
          } 
        }
      } else {
     
        int8_t* src_addr;
        if(child_count > 0) {
          if(gasneti_weakatomic_read(&(data->p2p->counter[0]),0) < child_count) {
            /* all children have not yet reported*/ 
            break;
          }
          src_addr  = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
        } else {
          src_addr = (int8_t*) args->src;
        }
        /*all children have reported*/
        gasneti_sync_reads();
        {
          /*data is going into the scratch space*/
          if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
            /*since the parent will signal us anyway with a barrier when the data is complete we can safely send w/o local completion*/
            gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team, parent),
                                                   (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(geom->sibling_offset+1)*args->nbytes,
                                                   src_addr,
                                                   args->nbytes*geom->mysubtree_size,0);
          } else  {
            /*in the case of an out NOSYNC the parent will not be waiting to signal so the first level of children will be the last in the tree to leave*/
            /*in the case of an out mysync there will be no signal when the scratch space nor the src data is safe to reuse so we must wait*/
            /*in either case we need local completion semantics*/
            gasnete_coll_p2p_counting_put/*Async*/(op, GASNETE_COLL_REL2ACT(op->team, parent),
                                                   (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(geom->sibling_offset+1)*args->nbytes,
                                                   src_addr,
                                                   args->nbytes*geom->mysubtree_size,0);
          }
        }
      }
      data->state = 4; GASNETI_FALLTHROUGH

  case 4: /*sync data movement*/
    if(data->handle !=GEX_EVENT_INVALID) {
      break;
    }
    data->state = 5; GASNETI_FALLTHROUGH

  case 5: /*node level barrier*/
    
    /* go down the tree with the barrier again*/
    if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
      if(op->team->myrank!=args->dstnode) {
        /*wait for clear signal from parent*/
        expected_count = child_count + 1;
        if (gasneti_weakatomic_read(&(data->p2p->counter[0]), 0) < expected_count) {
          break;
        }
      }
      /*send clear signal to all the other nodes*/
      for(child=0; child<child_count; child++) {
        gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[child]),0);
      }
    }
    data->state = 6; GASNETI_FALLTHROUGH
    
  case 6: /* done*/
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
      gasnete_coll_free_scratch(op);
  }
  return result;
}

GASNETE_COLL_DECLARE_GATHER_ALG(TreePut) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
  
  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, dist, flags,
                                        &gasnete_coll_pf_gath_TreePut, options,
                                        gasnete_coll_local_tree_geom_fetch(coll_params->tree_type, 
                                                                           dstimage, team),
                                        sequence, 
                                        coll_params->num_params, coll_params->param_list  GASNETI_THREAD_PASS);
}

static int gasnete_coll_pf_gath_TreePutNoCopy(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_local_tree_geom_t *geom = data->tree_geom;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  gex_Rank_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(geom);
  gex_Rank_t parent = GASNETE_COLL_TREE_GEOM_PARENT(geom);
  const gex_Rank_t child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(geom);
  gex_Rank_t child;
  int result = 0;
  
  switch (data->state) {
  case 0:
    if(!gasnete_coll_scratch_alloc_nb(op GASNETI_THREAD_PASS))    break;
    data->state = 1; GASNETI_FALLTHROUGH
    
  case 1:	/* Optional IN barrier */
    if (!gasnete_coll_generic_all_threads(data)||
        !gasnete_coll_generic_insync(op->team, data)) {
      break;
    }
    
    data->state = 2; GASNETI_FALLTHROUGH
    
  case 2: /* no need for local datamovement since all data movement is done through the network*/
    /*since i am here, my data is ready to be shipped up the tree*/
    if(op->team->myrank!=args->dstnode) {
      {
        /*data is going into the scratch space*/
        if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
          /*since the parent will signal us anyway with a barrier when the data is complete we can safely send w/o local completion*/
          gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team, parent),
                                             (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(geom->sibling_offset)*args->nbytes,
                                             args->src,
                                             args->nbytes,0);
        } else  {
          /*in the case of an out NOSYNC the parent will not be waiting to signal so the first level of children will be the last in the tree to leave*/
          /*in the case of an out mysync there will be no signal when the scratch space nor the src data is safe to reuse so we must wait*/
          /*in either case we need local completion semantics*/
          gasnete_coll_p2p_counting_put/*Async*/(op, GASNETE_COLL_REL2ACT(op->team, parent),
                                                 (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(geom->sibling_offset)*args->nbytes,
                                                 args->src,
                                                 args->nbytes,0);
        }
      }
    } else {
      GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(args->dst,op->team->myrank,args->nbytes), 
                                    (int8_t*)args->src, args->nbytes);
    }
    data->state = 3; GASNETI_FALLTHROUGH
    
  case 3:
    if(op->team->myrank == args->dstnode) {        
      gasneti_assert(geom->num_rotations==1);
      {
        int8_t* scratchspace =  (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
        /*leaf children will singal once while non-leaf children will signal twice times*/
        if(gasneti_weakatomic_read(&(data->p2p->counter[0]),0) < (geom->num_leaf_children + geom->num_non_leaf_children*2)) {
          /* all children have not yet reported*/ 
          break;
        }
        gasneti_sync_reads();
        /*all children have reported so now need to reshuffle the data into the correct place*/

#if 0
        if(args->nbytes == args->dist) {
          gasneti_assert(geom->num_rotations==1);
          gasnete_coll_local_rotate_right(args->dst,scratchspace, 
                                          args->nbytes, op->team->total_ranks, geom->rotation_points[0]);
        } else { 
          int i;
          for(i=0; i<op->team->total_ranks; i++) {
            GASNETE_FAST_UNALIGNED_MEMCPY(gasnete_coll_scale_ptr(args->dst, (i+geom->rotation_points[0])%op->team->total_ranks, args->dist),
                                          gasnete_coll_scale_ptr(scratchspace, i,args->nbytes),
                                          args->nbytes);
          } 
        } 
#else
        GASNETI_MEMCPY_SAFE_EMPTY(    gasnete_coll_scale_ptr(args->dst,1,args->nbytes),
                                      scratchspace,
                                      args->nbytes*(op->team->total_ranks-1));

#endif
      }
    } else if(child_count>0){
      
      int8_t* src_addr;
      
      if(gasneti_weakatomic_read(&(data->p2p->counter[0]),0) < (geom->num_leaf_children+geom->num_non_leaf_children*2)) {
        /* all children have not yet reported*/ 
        break;
      }
      gasneti_sync_reads();
      src_addr  = (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos;
      /*all children have reported*/
      
      {
        /*data is going into the scratch space*/
        if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
          /*since the parent will signal us anyway with a barrier when the data is complete we can safely send w/o local completion*/
          gasnete_coll_p2p_counting_putAsync(op, GASNETE_COLL_REL2ACT(op->team,parent),
                                             (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(1+geom->sibling_offset)*args->nbytes,
                                             src_addr,
                                             args->nbytes*(geom->mysubtree_size-1),0);
        } else  {
          /*in the case of an out NOSYNC the parent will not be waiting to signal so the first level of children will be the last in the tree to leave*/
          /*in the case of an out mysync there will be no signal when the scratch space nor the src data is safe to reuse so we must wait*/
          /*in either case we need local completion semantics*/
          gasnete_coll_p2p_counting_put/*Async*/(op, GASNETE_COLL_REL2ACT(op->team, parent),
                                                 (int8_t*)op->team->scratch_segs[parent].addr+op->scratchpos[0]+(1+geom->sibling_offset)*args->nbytes,
                                                 src_addr,
                                                 args->nbytes*(geom->mysubtree_size-1),0);
        }
      }
    } else {
      /*no children data movement already hadled in state 2*/
    }
    data->state = 4; GASNETI_FALLTHROUGH
    
  case 4: /*sync data movement*/
    /*since we have initiated possibly more than one put we need to sync all of them*/
    if(data->handle !=GEX_EVENT_INVALID || data->handle2 !=GEX_EVENT_INVALID) {
      break;
    }
    data->state = 5; GASNETI_FALLTHROUGH
    
  case 5: /*node level barrier*/
    
    /* go down the tree with the barrier again*/
    if(op->flags & GASNET_COLL_OUT_ALLSYNC) {
      if(op->team->myrank!=args->dstnode) {
        /*wait for clear signal from parent*/
         if (gasneti_weakatomic_read(&(data->p2p->counter[1]), 0) == 0) {
          break;
        }
      }
      /*send clear signal to all the other nodes*/
      for(child=0; child<child_count; child++) {
        gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team, children[child]),1);
      }
    }
    data->state = 6; GASNETI_FALLTHROUGH
    
  case 6: /* done*/
    gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    gasnete_coll_free_scratch(op);
  }
  return result;
}

GASNETE_COLL_DECLARE_GATHER_ALG(TreePutNoCopy)
{

  if(dstimage != 0) {
    /*tree put copy as written doesn't work when the root is not 0*/
    return gasnete_coll_gath_TreePut(team,
                                     dstimage, dst,
                                     src,
                                     nbytes, dist, flags, 
                                     coll_params,
                                     sequence
                                     GASNETI_THREAD_PASS);
  } else {
    int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
    GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P_IF(1);
    return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, dist, flags,
                                          &gasnete_coll_pf_gath_TreePutNoCopy, options,
                                          gasnete_coll_local_tree_geom_fetch(coll_params->tree_type, 
                                                                             dstimage, team),
                                          sequence, coll_params->num_params, coll_params->param_list
                                          GASNETI_THREAD_PASS);
  }
}

static int gasnete_coll_pf_gath_TreePutSeg(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result =0;
  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
          !gasnete_coll_generic_insync(op->team, data)) {
        break;
      }
      data->state = 1; GASNETI_FALLTHROUGH
      
      case 1:	/* Initiate data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec;
      size_t seg_size = gasnete_coll_get_pipe_seg_size(op->team->autotune_info, GASNET_COLL_GATHER_OP, op->flags);
      int num_segs = ((args->nbytes % seg_size) == 0 ? args->nbytes/seg_size : (args->nbytes/seg_size)+1);
      int flags = GASNETE_COLL_FORWARD_FLAGS(op->flags);
      gex_Rank_t dstproc = args->dstnode;
      size_t sent_bytes=0;
      
      int i;
      gasnete_coll_implementation_t impl = gasnete_coll_get_implementation();
      impl->fn_ptr = NULL;
      /*strip the last argument off which contains the pipeline segment size*/
      impl->num_params = op->num_coll_params;
      GASNETI_MEMCPY_SAFE_IDENTICAL(impl->param_list, op->param_list, sizeof(uint32_t)*op->num_coll_params);
      impl->tree_type = data->tree_geom->tree_type;
      
      data->private_data = gasneti_malloc(sizeof(gasnete_coll_handle_vec_t));
      handle_vec = data->private_data;
      handle_vec->num_handles = num_segs;
      handle_vec->handles = gasneti_malloc(sizeof(gex_Event_t)*num_segs);
      
      for(i=0; i<num_segs - 1; i++) {
        /*ignore the handle returned*/
        handle_vec->handles[i] = gasnete_coll_gath_TreePut(op->team, GASNETE_COLL_REL2ACT(op->team, dstproc), gasnete_coll_scale_ptr(args->dst,1,sent_bytes) , 
                                                           gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                           seg_size, args->nbytes, flags, impl, op->sequence+i+1 GASNETI_THREAD_PASS);   
        gasnete_coll_save_event(&handle_vec->handles[i]);
        sent_bytes += seg_size;
      }
      
      handle_vec->handles[i] = gasnete_coll_gath_TreePut(op->team, GASNETE_COLL_REL2ACT(op->team, dstproc), gasnete_coll_scale_ptr(args->dst,1,sent_bytes) , 
                                                         gasnete_coll_scale_ptr(args->src,1,sent_bytes), 
                                                         args->nbytes-sent_bytes, args->nbytes, flags, impl, op->sequence+i+1 GASNETI_THREAD_PASS);   
      gasnete_coll_save_event(&handle_vec->handles[i]);
      gasnete_coll_free_implementation(impl);
    }
      data->state = 2; GASNETI_FALLTHROUGH
      
      case 2:	/* Sync data movement */
    {
      gasnete_coll_handle_vec_t *handle_vec = (gasnete_coll_handle_vec_t*) data->private_data;
      if (!gasnete_coll_generic_coll_sync(handle_vec->handles, handle_vec->num_handles GASNETI_THREAD_PASS)) {
        break;
      }
      gasneti_free(handle_vec->handles);
    }   
      data->state = 3; GASNETI_FALLTHROUGH

      case 3:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(op->team, data)) {
        break;
      }
      
      gasneti_free(data->private_data);
      gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

GASNETE_COLL_DECLARE_GATHER_ALG(TreePutSeg) {
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE)) | 
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNETE_COLL_SUBORDINATE));

  size_t seg_size; 
  int num_segs;
  gasneti_assert(coll_params->num_params >= 1);
  seg_size = (size_t)coll_params->param_list[0];
  num_segs = (nbytes + seg_size - 1)/seg_size;
  
  gasneti_assert(num_segs < GASNETE_COLL_MAX_NUM_SEGS);
  


  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, nbytes, flags,
                                        &gasnete_coll_pf_gath_TreePutSeg, options,
                                        gasnete_coll_local_tree_geom_fetch(coll_params->tree_type, 
                                                                           dstimage, team),
                                        (flags & GASNETE_COLL_SUBORDINATE ? sequence : num_segs), 
                                        coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}  

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_all_nb() */

static int gasnete_coll_pf_gall_Dissem(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_dissem_info_t *dissem = data->dissem_info;
  const gasnete_coll_gather_all_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather_all);
  int result = 0;
  
  /* State 0: In barrier (if needed)*/
  if(data->state == 0) {
    if(op->team->total_ranks>1) 
      if(!gasnete_coll_scratch_alloc_nb(op GASNETI_THREAD_PASS)) 
        return 0;
    
    gasneti_assert(dissem->dissemination_radix==2); /* this function only works for radix 2*/
    data->state++;
  } 
  if(data->state == 1) {
    if (!gasnete_coll_generic_all_threads(data) || 
        !gasnete_coll_generic_insync(op->team, data)) {
      return 0;
    }
    
    if(op->team->total_ranks == 1) {
      GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, args->src, args->nbytes);
      data->state = dissem->dissemination_phases*2+2;
    } else {
      /*memcpy the data into the start of the scratch space*/
      GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                    args->src, args->nbytes);
      data->state++;
    }
  }
  if(data->state >= 2 && data->state <= (dissem->dissemination_phases-1)*2+1) {
    uint32_t phase = (data->state-2)/2;
    size_t curr_len = args->nbytes*(1<<phase); /* length = nbytes * 2^phase*/
    gex_Rank_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    
    if(data->state % 2 == 0) {
      /* send in this phase */
      gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                      (int8_t*)op->team->scratch_segs[dstnode].addr+op->scratchpos[0]+curr_len,
                                      (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                      curr_len, phase, 1);
      data->state++;
    } 
    if(data->state % 2 == 1){
      if(data->p2p->state[phase] == 1) {
        data->state++;
      } else {
        return 0;
      }
    }
  }
  
  if(data->state == (dissem->dissemination_phases)*2) {
    uint32_t phase = (data->state-2)/2;
    size_t nblk = op->team->total_ranks - (1<<phase); 
    size_t curr_len = args->nbytes*(nblk);
    gex_Rank_t dstnode = (GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase))[0];
    gasnete_coll_p2p_signalling_put(op, GASNETE_COLL_REL2ACT(op->team,dstnode), 
                                    (int8_t*)op->team->scratch_segs[dstnode].addr+op->scratchpos[0]+(1<<phase)*args->nbytes,
                                    (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos,
                                    curr_len, phase, 1);
    data->state++;
  }
  
  if(data->state == (dissem->dissemination_phases)*2+1) {
    uint32_t phase = (data->state-2)/2;
    if(data->p2p->state[phase] !=1) return 0; /*wait for the last transfer to finish*/
    
    /*rotate the data around*/
    GASNETI_MEMCPY_SAFE_IDENTICAL((int8_t*)args->dst+args->nbytes*op->team->myrank, 
                                        (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos, 
                                        args->nbytes*(op->team->total_ranks-op->team->myrank));
    GASNETI_MEMCPY_SAFE_IDENTICAL(args->dst, 
                                        (int8_t*)op->team->scratch_segs[op->team->myrank].addr+op->myscratchpos+args->nbytes*(op->team->total_ranks-op->team->myrank), 
                                        args->nbytes*op->team->myrank);
    data->state++;
  }
  if(data->state == (dissem->dissemination_phases)*2+2) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    /*free up the scratch space used by this op*/
    if(op->team->total_ranks > 1) gasnete_coll_free_scratch(op);    
    gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}


extern gex_Event_t
gasnete_coll_gall_Dissem(gasnet_team_handle_t team,
                         void *dst, void *src,
                         size_t nbytes, int flags, 
                         gasnete_coll_implementation_t coll_params,
                         uint32_t sequence
                         GASNETI_THREAD_FARG)
{
  /*Since the algorithm is naturally in_no / out_no use in-barrier if anything besides IN NOSYNC. 
   Use out barrier only if out_ALLSYNC since algorithm does not need a full barrier for OUT_MYSYNC*/
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF ((flags & GASNET_COLL_IN_ALLSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC)) | 
  GASNETE_COLL_GENERIC_OPT_P2P | GASNETE_COLL_USE_SCRATCH;
  
  

  
  return gasnete_coll_generic_gather_all_nb(team, dst, src, nbytes, flags,
                                            &gasnete_coll_pf_gall_Dissem, options,
                                            NULL, 
                                            sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchange_nb() */

/* exchg Brucks: Implemented using Dissemination based exchange algorithm */
/*
 Based on Bruck et al. algorithm 
 (from IEEE Transactions on Parallel and Distributed Computiing Vol. 8 No. 11 Nov. 1997)
 */
GASNETI_INLINE(gasnete_coll_mypow)
int gasnete_coll_mypow(int base, int pow) {
  int ret = 1;
  while(pow!=0) {
    ret *=base;
    pow--;
  }
  return ret;
}

GASNETI_INLINE(gasnete_coll_pack_all_to_all_msg)
int gasnete_coll_pack_all_to_all_msg(void *src, void *dest, size_t nbytes,
                                     int digit, int radix, int j, int total_ranks) {
  int i_idx;
  
  int ret=0;
  
  
  /*pack if the digit_th digit of the radix-r representation of block_id is j*/
  for(i_idx=0; i_idx<total_ranks; i_idx++) {
    
    if( ((i_idx / gasnete_coll_mypow(radix, digit)) % radix) == j ) {
      GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)dest+ret*nbytes, (int8_t*)src+i_idx*nbytes, nbytes);
      ret++;
    }
  }
  return ret;
}

GASNETI_INLINE(gasnete_coll_unpack_all_to_all_msg)
void gasnete_coll_unpack_all_to_all_msg(void *src, void *dest, size_t nbytes,
                                        int digit, int radix, int j, int total_ranks) {
  int i_idx;
  
  int blk_count=0;
  
  for(i_idx=0; i_idx<total_ranks; i_idx++) {
    if( ((i_idx / gasnete_coll_mypow(radix, digit)) % radix) == j ) {
      GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)dest+i_idx*nbytes, (int8_t*)src+blk_count*nbytes, nbytes);
      blk_count++;
    }
  }
}

/* This algorithm does not directly send into remote data... it uses the remote scratch space instead
 */

static int gasnete_coll_pf_exchg_Dissem(gasnete_coll_op_t *op GASNETI_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_dissem_info_t *dissem = data->dissem_info;
  const gasnete_coll_exchange_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, exchange);
  int result = 0;
  
  size_t offset;
  
  void *scratch2;
  void *scratch1;
  
  /*this will be a slightly different poll function than the other ones*/
  /*the state will be used to describe the dissemination phase*/
  /*reserving state 0 and dissem_phase*2+2+1 for the in/out barrier stages*/
  /*state 1 will be used for local memory copies*/
  /*state dissem_phases*2+2 will represent memory copies on the output side*/
  /*states 2 through dissem_phases*2+1 represent intermediary steps*/
  /*each dissem phase will get two steps, one for sending and one for recieiving*/
  if(data->state == 0) {
    if_pt(op->team->total_ranks != 1) 
    if(!gasnete_coll_scratch_alloc_nb(op GASNETI_THREAD_PASS)) 
      return 0;
    data->state = 1;
  } 
  
  scratch1 = (int8_t*)op->team->scratch_segs[op->team->myrank].addr + op->myscratchpos;
  scratch2 = (int8_t*)scratch1 + ((args->nbytes)*dissem->max_dissem_blocks)*((2)*(dissem->dissemination_radix-1));
  
  
  if(data->state == 1) {
    
    if (!gasnete_coll_generic_all_threads(data) ||
        !gasnete_coll_generic_insync(op->team, data)) {
      return result;
    }
    
    if(op->team->total_ranks == 1) {
      GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, args->src, args->nbytes);
      data->state = dissem->dissemination_phases*3+3;
      return 0; 
    }    
    /* perform local rotation right by total_ranks - myrank elements*/
    gasnete_coll_local_rotate_right((int8_t*) scratch2, args->src, args->nbytes, op->team->total_ranks, op->team->total_ranks - op->team->myrank);
    data->state = 2;
  }
  
  if(data->state>=2 && data->state<=dissem->dissemination_phases*3+1) {
    /*data transfer stages*/
    /*global phase id */
    int destnode,nblocks;
    gex_Rank_t* out_nodes, *in_nodes;
    int phase = (data->state - 2)/3;
    int h,j;
    offset = GASNETE_COLL_DISSEM_MAX_BLOCKS(dissem)*args->nbytes;
    h = GASNETE_COLL_DISSEM_GET_PEER_COUNT_PHASE(dissem, phase);
    out_nodes = GASNETE_COLL_DISSEM_GET_FRONT_PEERS_PHASE(dissem, phase);
    in_nodes =GASNETE_COLL_DISSEM_GET_BEHIND_PEERS_PHASE(dissem, phase);
#define IDX_EXPR (((phase%2)*(dissem->dissemination_radix-1) + (j))*offset)
#define IDXP1_EXPR ((((phase+1)%2)*(dissem->dissemination_radix-1) + (j))*offset)
    
    if((data->state-2) % 3 == 0) {
      /*send the ok to send signal*/
      gasneti_sync_writes();
      for(j=0; j<h; j++) {
        
        gasnete_coll_p2p_advance(op, GASNETE_COLL_REL2ACT(op->team,in_nodes[j]), phase*2);
        /*XXX: switch to counting put for higher radices*/ 
        /*gasnete_coll_p2p_change_states(op,  GASNETE_COLL_REL2ACT(op->team, in_nodes[j]), 1, phase*2, 1);*/
      }
      data->state++;
    }
    if((data->state-2) % 3 == 1) {
      /*XXX: Need atomic counter read for higher radices*/
      /* if(data->p2p->state[phase*2] != h) return 0;*/
      if_pf(gasneti_weakatomic_read(&(data->p2p->counter[phase*2]),0)!=h) return 0;
      gasneti_sync_reads();
      for(j=0; j<h; j++) {
        destnode = out_nodes[j];
        nblocks = 
        gasnete_coll_pack_all_to_all_msg(scratch2, (int8_t*)scratch1+IDX_EXPR,args->nbytes,
                                         phase, dissem->dissemination_radix, j+1, op->team->total_ranks);
        gasnete_coll_p2p_counting_put(op, GASNETE_COLL_REL2ACT(op->team,destnode), 
                                      (int8_t*)op->team->scratch_segs[destnode].addr+op->scratchpos[0]+IDXP1_EXPR, (int8_t*)scratch1+IDX_EXPR,
                                      nblocks*args->nbytes, phase*2+1);
      }
      /*once all the change the state and return 0*/
      /*let the poll function bring us back here*/
      data->state++;
      return 0;
    } 
    if((data->state-2) % 3 == 2) { /*receive in odd sub phases*/
      /*wait for all the states to trip*/
      /*need to change this to an atomic state increment to do this properly for radix>2*/
      if(gasneti_weakatomic_read(&(data->p2p->counter[phase*2+1]),0) == h) {
        gasneti_sync_reads();
        for(j=0; j<h; j++) {
          gasnete_coll_unpack_all_to_all_msg((int8_t*)scratch1+IDXP1_EXPR, (int8_t*)scratch2, args->nbytes, phase,
                                             dissem->dissemination_radix, j+1, op->team->total_ranks);
        }			
        data->state++;
        return 0;
      } else {
        return 0;
      }
    }
    
#undef IDX_EXPR
#undef IDXP1_EXPR
    
  }
  
  if(data->state == dissem->dissemination_phases*3+2) {
    int i;
    int srcnode;
    for(i=0; i<op->team->total_ranks; i++) {
      srcnode  = (op->team->myrank - i);
      if(srcnode < 0) {
        srcnode = op->team->total_ranks+srcnode;
      }
      
      GASNETE_FAST_UNALIGNED_MEMCPY((int8_t*)args->dst+i*args->nbytes,
                                    (int8_t*)scratch2+srcnode*args->nbytes,
                                    args->nbytes);
    }
    data->state ++;
    
  }
  if(data->state == dissem->dissemination_phases*3+3) {
    if (!gasnete_coll_generic_outsync(op->team, data)) {
      return 0;
    }
    
    /*free up the scratch space used by this op*/
    if(op->team->total_ranks != 1) gasnete_coll_free_scratch(op);    
    gasnete_coll_generic_free(op->team, data GASNETI_THREAD_PASS);
    result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
    
  }
  
  return result;
}

extern gex_Event_t
gasnete_coll_exchg_Dissem2(gasnet_team_handle_t team,
                          void *dst, void *src,
                          size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                          GASNETI_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 2;

  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_Dissem, options,
                                          NULL, gasnete_coll_fetch_dissemination(radix ,team), sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}
extern gex_Event_t
gasnete_coll_exchg_Dissem3(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                           GASNETI_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 3;
  
  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_Dissem, options,
                                          NULL, gasnete_coll_fetch_dissemination(radix ,team), sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}
extern gex_Event_t
gasnete_coll_exchg_Dissem4(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                           GASNETI_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 4;
  
  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_Dissem, options,
                                          NULL, gasnete_coll_fetch_dissemination(radix ,team), sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

extern gex_Event_t
gasnete_coll_exchg_Dissem8(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                           GASNETI_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = 8;
  
  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_Dissem, options,
                                          NULL, gasnete_coll_fetch_dissemination(radix ,team), sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

extern gex_Event_t
gasnete_coll_exchg_FlatScratch(gasnet_team_handle_t team,
                           void *dst, void *src,
                           size_t nbytes, int flags, gasnete_coll_implementation_t coll_params, uint32_t sequence
                           GASNETI_THREAD_FARG)
{
  int options =  GASNETE_COLL_USE_SCRATCH | GASNETE_COLL_GENERIC_OPT_P2P | 
  GASNETE_COLL_GENERIC_OPT_INSYNC_IF (!(flags & GASNET_COLL_IN_NOSYNC)) |
  GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(!(flags & GASNET_COLL_OUT_NOSYNC));
  int radix;
  
  radix = team->total_ranks;
  
  return gasnete_coll_generic_exchange_nb(team, dst, src, nbytes, flags,
                                          &gasnete_coll_pf_exchg_Dissem, options,
                                          NULL, gasnete_coll_fetch_dissemination(radix ,team), sequence, coll_params->num_params, coll_params->param_list GASNETI_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
