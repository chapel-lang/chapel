/*   $Source$
 *     $Date$
 * $Revision$
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_coll_internal.h>
#include <gasnet_coll_trees.h>
#include <gasnet_coll_scratch.h>
#include <gasnet_coll_autotune.h>
#include <gasnet_vis.h>

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcast_nb() */


/* bcast Eager: root node performs carefully ordered eager puts */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_bcast_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_put_all(op, args->src, args->nbytes, 0, 0, 1);	/* broadcast data */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);
      } else {
	break;	/* Stalled until data arrives */
      }
      data->state = 2;

    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcast_Eager(gasnet_team_handle_t team,
			 void *dst,
			 gasnet_image_t srcimage, void *src,
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)  |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  gasneti_assert(nbytes <= gasnete_coll_p2p_eager_min);

  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_Eager, options,
					   NULL, sequence GASNETE_THREAD_PASS);
}


/* bcast TreeEager */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
/* Naturally IN_MYSYNC, OUT_MYSYNC */
/* Max size is the eager limit */
static int gasnete_coll_pf_bcast_TreeEager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  const gasnete_coll_broadcast_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcast);
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  int barrier_count;
  int result = 0;
  int child;


  switch (data->state) {
    case 0:	/* Thread barrier */
      if (!gasnete_coll_generic_all_threads(data)) {
	break;
      }

      data->state = 1;

    case 1:	/* Optional IN barrier over the SAME tree */
      if(op->flags & GASNET_COLL_IN_ALLSYNC) {
    	if (gasneti_weakatomic_read(&data->p2p->counter, 0) != child_count) {
	  break;
	}
        if (gasneti_mynode != args->srcnode) {
	  gasnete_coll_p2p_advance(op, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom));
	}
      }
      data->state = 2;

    case 2:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
	for (child=0;child<child_count; child++){
#if 0
	  gasnete_coll_p2p_eager_put(op, GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom)[child], args->src, args->nbytes, 0, 1);
#else
	  gasnete_coll_p2p_eager_put_tree(op, children[child], args->src, args->nbytes);
#endif
	}

	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst, args->src, args->nbytes);
	
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);
	for (child=0;child<child_count;child++) {
#if 0
	  gasnete_coll_p2p_eager_put(op, GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom)[child], args->dst, args->nbytes, 0, 1);
#else
	  gasnete_coll_p2p_eager_put_tree(op, children[child], args->dst, args->nbytes);
#endif	
	  
	}
      } else {
	break;	/* Stalled until data arrives */
      }
      data->state = 3;
    
    case 3: /*optional out barrier over the same tree*/
          if (!gasnete_coll_generic_outsync(data)) {
            break;
          }
          

      data->state = 4;
        
    case 4: /*done*/
      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_bcast_TreeEager(gasnet_team_handle_t team,
			     void *dst,
			     gasnet_image_t srcimage, void *src,
			     size_t nbytes, int flags,
			     gasnete_coll_tree_type_t tree_type,
			     uint32_t sequence
			     GASNETE_THREAD_FARG)
{
  int options = /*GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC)  |*/
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P;

  gasneti_assert(nbytes <= gasnete_coll_p2p_eager_min);
 
  return gasnete_coll_generic_broadcast_nb(team, dst, srcimage, src, nbytes, flags,
					   &gasnete_coll_pf_bcast_TreeEager, options,
					   gasnete_coll_tree_init(tree_type,
								  gasnete_coll_image_node(srcimage), team
								  GASNETE_THREAD_PASS),
					   sequence
					   GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_broadcastM_nb() */

/* bcastM Eager: root node performs carefully ordered eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_bcastM_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_put_all(op, args->src, args->nbytes, 0, 0, 1);	/* broadcast data */
	gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				     args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				     data->p2p->data, args->nbytes);
      } else {
        break;  /* Stalled until data arrives */
      }
      data->state = 2;

    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_bcastM_Eager(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_Eager, options,
					    NULL, sequence GASNETE_THREAD_PASS);
}

static int gasnete_coll_pf_bcastM_TreeEager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_broadcastM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, broadcastM);
  int result = 0;
  gasnete_coll_tree_data_t *tree = data->tree_info;
  gasnet_node_t * const children = GASNETE_COLL_TREE_GEOM_CHILDREN(tree->geom);
  const int child_count = GASNETE_COLL_TREE_GEOM_CHILD_COUNT(tree->geom);
  int child;

  
  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS)) {
	break;
      }
      data->state = 1;
      
    case 1:
      if((op->flags & GASNET_COLL_IN_ALLSYNC)) {
        
        if (gasneti_weakatomic_read(&(data->p2p->counter), 0) != child_count) {
          break;
        }
        if (gasneti_mynode != args->srcnode) {
          gasnete_coll_p2p_advance(op, GASNETE_COLL_TREE_GEOM_PARENT(tree->geom));
        }
      }
      data->state = 2;
      
      

    case 2:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
        gasneti_sync_reads();
        for(child=0; child<child_count; child++) {
            gasnete_coll_p2p_eager_put_tree(op, children[child], args->src, args->nbytes);
        }
	gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				     args->src, args->nbytes);
      } else if (data->p2p->state[0]) {
        gasneti_sync_reads();
	for (child=0;child<child_count;child++) {
	  gasnete_coll_p2p_eager_put_tree(op, children[child], 
                                          data->p2p->data, 
                                          args->nbytes);
	}
        
        gasnete_coll_local_broadcast(gasnete_coll_my_images,
				     &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				     data->p2p->data, args->nbytes);
      } else {
        break;  /* Stalled until data arrives */
      }
      data->state = 3;
        
      case 3:	/* Optional OUT barrier */
        if (!gasnete_coll_generic_outsync(data)) {
          break;
        }
        
        gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
        result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return result;
}

extern gasnet_coll_handle_t
gasnete_coll_bcastM_TreeEager(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, 
                          gasnete_coll_tree_type_t tree_type,
                          uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P;
  
  return gasnete_coll_generic_broadcastM_nb(team, dstlist, srcimage, src, nbytes, flags,
					    &gasnete_coll_pf_bcastM_TreeEager, options,
					    gasnete_coll_tree_init(tree_type,
                                                                   gasnete_coll_image_node(srcimage), team
                                                                   GASNETE_THREAD_PASS),
                                            sequence GASNETE_THREAD_PASS);
}


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatter_nb() */

/* scat Eager: root node performs carefully ordered eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_scat_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatter_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatter);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
	gasnete_coll_p2p_eager_put_all(op, args->src, args->nbytes, 1, 0, 1);	/* scatter data */
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(args->dst,
				      gasnete_coll_scale_ptr(args->src, gasneti_mynode, args->nbytes),
				      args->nbytes);
      } else if (data->p2p->state[0]) {
	gasneti_sync_reads();
	GASNETE_FAST_UNALIGNED_MEMCPY(args->dst, data->p2p->data, args->nbytes);
      } else {
	break;	/* Stalled until data arrives */
      }
      data->state = 2;

    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scat_Eager(gasnet_team_handle_t team,
			void *dst,
			gasnet_image_t srcimage, void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  return gasnete_coll_generic_scatter_nb(team, dst, srcimage, src, nbytes, nbytes, flags,
					 &gasnete_coll_pf_scat_Eager, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_scatterM_nb() */

/* scatM Eager: root node performs carefully ordered eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on non-root nodes */
static int gasnete_coll_pf_scatM_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_scatterM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, scatterM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->dstlist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

    case 1:	/* Data movement */
      if (gasneti_mynode == args->srcnode) {
	const void * const src   = args->src;
	size_t nbytes = args->nbytes;
	uintptr_t src_addr;
	int i;

	/* Send to nodes to the "right" of ourself */
	if (gasneti_mynode < gasneti_nodes - 1) {
	  src_addr = (uintptr_t)gasnete_coll_scale_ptr(src, gasnete_coll_all_offset[gasneti_mynode + 1], nbytes);
	  for (i = gasneti_mynode + 1; i < gasneti_nodes; ++i) {
	    const size_t count = gasnete_coll_all_images[i];

	    gasnete_coll_p2p_eager_putM(op, i, (void *)src_addr, count, nbytes, 0, 1);
	    src_addr += count * nbytes;
	  }
	}
	/* Send to nodes to the "left" of ourself */
	if (gasneti_mynode > 0) {
	  src_addr = (uintptr_t)gasnete_coll_scale_ptr(src, 0, nbytes);
	  for (i = 0; i < gasneti_mynode; ++i) {
	    const size_t count = gasnete_coll_all_images[i];

	    gasnete_coll_p2p_eager_putM(op, i, (void *)src_addr, count, nbytes, 0, 1);
	    src_addr += count * nbytes;
	  }
	}

	/* Local data movement */
	gasnete_coll_local_scatter(gasnete_coll_my_images,
				   &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags),
				   gasnete_coll_scale_ptr(src, gasnete_coll_my_offset, nbytes), nbytes);
      } else {
	gasnete_coll_p2p_t *p2p = data->p2p;
	volatile uint32_t *state;
	size_t nbytes = args->nbytes;
	void * const *p;
	uintptr_t src_addr;
	int i, done;

	gasneti_assert(p2p != NULL);
	gasneti_assert(p2p->state != NULL);
	state = data->p2p->state;
	gasneti_assert(p2p->data != NULL);

	done = 1;
	p = &GASNETE_COLL_MY_1ST_IMAGE(args->dstlist, op->flags);
	src_addr = (uintptr_t)(p2p->data);
	for (i = 0; i < gasnete_coll_my_images; ++i, ++p, src_addr += nbytes) {
	  uint32_t s = state[i];

	  if (s == 0) {
	    /* Nothing received yet */
	    done = 0;
	  } else {
	    /* Received but not yet copied into place */
	    gasneti_sync_reads();
	    GASNETE_FAST_UNALIGNED_MEMCPY(*p, (void *)src_addr, nbytes);
	    state[i] = 2;
	  }
	}

	if (!done) { break; }
      }
      data->state = 2;

    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_scatM_Eager(gasnet_team_handle_t team,
			  void * const dstlist[],
			  gasnet_image_t srcimage, void *src,
			  size_t nbytes, int flags, uint32_t sequence
                          GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
		GASNETE_COLL_GENERIC_OPT_P2P_IF(!gasnete_coll_image_is_local(srcimage));

  return gasnete_coll_generic_scatterM_nb(team, dstlist, srcimage, src, nbytes, nbytes, flags,
					  &gasnete_coll_pf_scatM_Eager, options,
					  NULL, sequence GASNETE_THREAD_PASS);
}


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_nb() */

/* gath Eager: all nodes perform uncoordinated eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on root node */
static int gasnete_coll_pf_gath_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gather_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gather);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_generic_all_threads(data) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

      /* Initiate data movement */
      if (gasneti_mynode != args->dstnode) {
	gasnete_coll_p2p_eager_put(op, args->dstnode, args->src, args->nbytes, gasneti_mynode, 1);
      } else {
	GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(gasnete_coll_scale_ptr(args->dst, gasneti_mynode, args->nbytes),
				      args->src, args->nbytes);
	data->p2p->state[gasneti_mynode] = 2;
      }

    case 1:	/* Complete data movement */
      if (gasneti_mynode == args->dstnode) {
	gasnete_coll_p2p_t *p2p = data->p2p;
	volatile uint32_t *state;
	uintptr_t dst_addr, src_addr;
	size_t nbytes = args->nbytes;
	int i, done;

	gasneti_assert(p2p != NULL);
	gasneti_assert(p2p->state != NULL);
	state = data->p2p->state;
	gasneti_assert(p2p->data != NULL);

	done = 1;
	dst_addr = (uintptr_t)(args->dst);
	src_addr = (uintptr_t)(p2p->data);
	for (i = 0; i < gasneti_nodes; ++i, dst_addr += nbytes, src_addr += nbytes) {
	  uint32_t s = state[i];

	  if (s == 0) {
	    /* Nothing received yet */
	    done = 0;
	  } else if (s == 1) {
	    /* Received but not yet copied into place */
	    gasneti_sync_reads();
	    GASNETE_FAST_UNALIGNED_MEMCPY((void *)dst_addr, (void *)src_addr, nbytes);
	    state[i] = 2;
	  }
	}

	if (!done) { break; }
      }
      data->state = 2;

    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gath_Eager(gasnet_team_handle_t team,
			gasnet_image_t dstimage, void *dst,
			void *src,
			size_t nbytes, int flags, uint32_t sequence
                        GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC)|
		GASNETE_COLL_GENERIC_OPT_P2P_IF(gasnete_coll_image_is_local(dstimage));

  return gasnete_coll_generic_gather_nb(team, dstimage, dst, src, nbytes, nbytes, flags,
					&gasnete_coll_pf_gath_Eager, options,
					NULL, sequence GASNETE_THREAD_PASS);
}


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gatherM_nb() */

/* gathM Eager: all nodes perform uncoordinated eager puts */
/* Valid for SINGLE and LOCAL, size <= available eager buffer space */
/* Requires GASNETE_COLL_GENERIC_OPT_P2P on root node */
static int gasnete_coll_pf_gathM_Eager(gasnete_coll_op_t *op GASNETE_THREAD_FARG) {
  gasnete_coll_generic_data_t *data = op->data;
  const gasnete_coll_gatherM_args_t *args = GASNETE_COLL_GENERIC_ARGS(data, gatherM);
  int result = 0;

  switch (data->state) {
    case 0:	/* Optional IN barrier */
      if (!gasnete_coll_threads_ready1(op, args->srclist GASNETE_THREAD_PASS) ||
	  !gasnete_coll_generic_insync(data)) {
	break;
      }
      data->state = 1;

      /* Initiate data movement */
      if (gasneti_mynode != args->dstnode) {
	size_t nbytes = args->nbytes;
	void * tmp = gasneti_malloc(gasnete_coll_my_images * nbytes);
	gasnete_coll_local_gather(gasnete_coll_my_images, tmp,
				  &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags), nbytes);
        gasnete_coll_p2p_eager_putM(op, args->dstnode, tmp, gasnete_coll_my_images,
				    nbytes, gasnete_coll_my_offset, 1);
	gasneti_free(tmp);
      } else {
	volatile uint32_t *s;
	int i;

	gasnete_coll_local_gather(gasnete_coll_my_images,
				  gasnete_coll_scale_ptr(args->dst, gasnete_coll_my_offset, args->nbytes),
				  &GASNETE_COLL_MY_1ST_IMAGE(args->srclist, op->flags), args->nbytes);
	s = &(data->p2p->state[gasnete_coll_my_offset]);
	for (i = 0; i < gasnete_coll_my_images; ++i) {
	  *(s++) = 2;
        }
      }

    case 1:	/* Complete data movement */
      if (gasneti_mynode == args->dstnode) {
	gasnete_coll_p2p_t *p2p = data->p2p;
	volatile uint32_t *state;
	uintptr_t dst_addr, src_addr;
	size_t nbytes = args->nbytes;
	int i, done;

	gasneti_assert(p2p != NULL);
	gasneti_assert(p2p->state != NULL);
	state = data->p2p->state;
	gasneti_assert(p2p->data != NULL);

	done = 1;
	dst_addr = (uintptr_t)(args->dst);
	src_addr = (uintptr_t)(p2p->data);
	for (i = 0; i < gasnete_coll_total_images; ++i, dst_addr += nbytes, src_addr += nbytes) {
	  uint32_t s = state[i];

	  if (s == 0) {
	    /* Nothing received yet */
	    done = 0;
	  } else if (s == 1) {
	    /* Received but not yet copied into place */
	    gasneti_sync_reads();
	    GASNETE_FAST_UNALIGNED_MEMCPY((void *)dst_addr, (void *)src_addr, nbytes);
	    state[i] = 2;
	  }
	}

	if (!done) { break; }
      }
      data->state = 2;

    case 2:	/* Optional OUT barrier */
      if (!gasnete_coll_generic_outsync(data)) {
	break;
      }

      gasnete_coll_generic_free(data GASNETE_THREAD_PASS);
      result = (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }

  return result;
}
extern gasnet_coll_handle_t
gasnete_coll_gathM_Eager(gasnet_team_handle_t team,
			 gasnet_image_t dstimage, void *dst,
			 void * const srclist[],
			 size_t nbytes, int flags, uint32_t sequence
                         GASNETE_THREAD_FARG)
{
  int options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF (flags & GASNET_COLL_IN_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC) |
		GASNETE_COLL_GENERIC_OPT_P2P_IF(gasnete_coll_image_is_local(dstimage));

  return gasnete_coll_generic_gatherM_nb(team, dstimage, dst, srclist, nbytes, nbytes, flags,
					 &gasnete_coll_pf_gathM_Eager, options,
					 NULL, sequence GASNETE_THREAD_PASS);
}


/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_all_nb() */

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_gather_allM_nb() */

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchange_nb() */

/*---------------------------------------------------------------------------------*/
/* gasnete_coll_exchangeM_nb() */


