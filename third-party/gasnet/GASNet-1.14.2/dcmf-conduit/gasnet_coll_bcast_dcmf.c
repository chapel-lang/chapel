/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_coll_bcast_dcmf.c,v $
 *     $Date: 2010/05/10 23:15:37 $
 * $Revision: 1.8.2.1 $
 * Description: GASNet broadcast implementation on DCMF
 * Copyright 2009, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

#include <string.h>

#include <gasnet_coll_bcast_dcmf.h>

/* #define G_DCMF_COLL_TRACE */

int gasnete_dcmf_bcast_num_colors;
static int gasnete_use_dcmf_bcast;

/* Broadcast protocol registration data */
static DCMF_CollectiveProtocol_t gasnete_dcmf_bcast_proto[G_DCMF_BCAST_PROTO_NUM];
static int gasnete_dcmf_bcast_enabled[G_DCMF_BCAST_PROTO_NUM];

/* Global Broadcast protocol registration data */
static DCMF_Protocol_t gasnete_dcmf_globalbcast_proto; 

void gasnete_coll_bcast_proto_register(void)
{
  DCMF_GlobalBcast_Configuration_t gbcast_conf;
  DCMF_Broadcast_Configuration_t bcast_conf;
  DCMF_AsyncBroadcast_Configuration_t a_bcast_conf;
  char *tmp_str;

  GASNETC_DCMF_LOCK(); /* for DCMF_SAFE */
  
  gasnete_use_dcmf_bcast =
    gasneti_getenv_yesno_withdefault("GASNET_USE_DCMF_BCAST", 1);

  /* tree broadcast registration */
  gasnete_dcmf_bcast_enabled[TREE_BROADCAST] =
    gasneti_getenv_yesno_withdefault("GASNET_DCMF_TREE_BCAST", 1);
  if (gasnete_dcmf_bcast_enabled[TREE_BROADCAST]) 
    {
      gbcast_conf.protocol = DCMF_TREE_GLOBALBCAST_PROTOCOL;
      DCMF_SAFE(DCMF_GlobalBcast_register(&gasnete_dcmf_globalbcast_proto, 
                                          &gbcast_conf));
      bcast_conf.protocol = DCMF_TREE_BROADCAST_PROTOCOL;
      DCMF_SAFE(DCMF_Broadcast_register(&gasnete_dcmf_bcast_proto[TREE_BROADCAST], 
                                        &bcast_conf));
    }
  
  /* torus broadcast registration */
  gasnete_dcmf_bcast_enabled[TORUS_RECTANGLE_BROADCAST] =
    gasneti_getenv_yesno_withdefault("GASNET_DCMF_TORUS_RECTANGLE_BCAST", 1);
  if (gasnete_dcmf_bcast_enabled[TORUS_RECTANGLE_BROADCAST]) 
    {
      bcast_conf.protocol = DCMF_TORUS_RECTANGLE_BROADCAST_PROTOCOL;
      DCMF_SAFE(DCMF_Broadcast_register(&gasnete_dcmf_bcast_proto[TORUS_RECTANGLE_BROADCAST],
                                        &bcast_conf));
    }
  
  gasnete_dcmf_bcast_enabled[TORUS_BINOMIAL_BROADCAST] =
    gasneti_getenv_yesno_withdefault("GASNET_DCMF_TORUS_BINOMIAL_BCAST", 1);
  if (gasnete_dcmf_bcast_enabled[TORUS_BINOMIAL_BROADCAST])
    {
      bcast_conf.protocol = DCMF_TORUS_BINOMIAL_BROADCAST_PROTOCOL;
      DCMF_SAFE(DCMF_Broadcast_register(&gasnete_dcmf_bcast_proto[TORUS_BINOMIAL_BROADCAST],
                                        &bcast_conf));
    }
  
  /* asynchronous broadcast registration */
  gasnete_dcmf_bcast_enabled[TORUS_ASYNCBROADCAST_RECTANGLE] =
    gasneti_getenv_yesno_withdefault("GASNET_DCMF_TORUS_ASYNCBROADCAST_RECTANGLE_BCAST", 1);
  if (gasnete_dcmf_bcast_enabled[TORUS_ASYNCBROADCAST_RECTANGLE])
    {
      a_bcast_conf.protocol = DCMF_TORUS_ASYNCBROADCAST_RECTANGLE_PROTOCOL;
      a_bcast_conf.isBuffered = 1;
      a_bcast_conf.cb_geometry = gasnete_dcmf_get_geometry;
      DCMF_SAFE(DCMF_AsyncBroadcast_register(&gasnete_dcmf_bcast_proto[TORUS_ASYNCBROADCAST_RECTANGLE],
                                             &a_bcast_conf));
    }

  gasnete_dcmf_bcast_enabled[TORUS_ASYNCBROADCAST_BINOMIAL] =
    gasneti_getenv_yesno_withdefault("GASNET_DCMF_TORUS_ASYNCBROADCAST_BINOMIAL_BCAST", 1);
  if (gasnete_dcmf_bcast_enabled[TORUS_ASYNCBROADCAST_BINOMIAL])
    {
      a_bcast_conf.protocol = DCMF_TORUS_ASYNCBROADCAST_BINOMIAL_PROTOCOL;
      a_bcast_conf.isBuffered = 1;
      a_bcast_conf.cb_geometry = gasnete_dcmf_get_geometry;
      DCMF_SAFE(DCMF_AsyncBroadcast_register(&gasnete_dcmf_bcast_proto[TORUS_ASYNCBROADCAST_BINOMIAL],
                                             &a_bcast_conf));
    }

  /* number of bcast streams used in the torus rectangle bcast */
  gasnete_dcmf_bcast_num_colors = gasneti_getenv_int_withdefault("GASNET_DCMF_BCAST_NUM_COLORS", 1, 0); 
  if (gasnete_dcmf_bcast_num_colors < 1 || gasnete_dcmf_bcast_num_colors > 6)
      gasnete_dcmf_bcast_num_colors = 1;
  
  GASNETC_DCMF_UNLOCK(); 
}

int gasnete_coll_bcast_set_proto(gasnet_team_handle_t team,
                                 gasnete_dcmf_bcast_kind_t kind)
{
  gasnete_coll_team_dcmf_t *dcmf_tp;
  
  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  gasneti_assert(dcmf_tp != NULL);

  if (kind < G_DCMF_BCAST_PROTO_NUM 
      && DCMF_Geometry_analyze(&dcmf_tp->geometry, &gasnete_dcmf_bcast_proto[kind])
      && gasnete_dcmf_bcast_enabled[kind])
    {
      dcmf_tp->bcast_proto = &gasnete_dcmf_bcast_proto[kind];
      return 0; /* SUCCESS */
    }
  return 1; /* FAILED */
}

void gasnete_coll_bcast_set_default_proto(gasnet_team_handle_t team)
{
  gasnete_coll_team_dcmf_t *dcmf_tp;
  
  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  gasneti_assert(dcmf_tp != NULL);

  if (gasnete_coll_bcast_set_proto(team, TORUS_RECTANGLE_BROADCAST))
    if (gasnete_coll_bcast_set_proto(team, TORUS_BINOMIAL_BROADCAST))
      dcmf_tp->bcast_proto = NULL;
}

static void gasnete_dcmf_bcast_cb_done(void *clientdata, DCMF_Error_t *e)
{
  unsigned *p = (unsigned *)clientdata;
  *p = 0;
}

static void gasnete_coll_dcmf_bcast_init(gasnete_dcmf_bcast_data_t *bcast,
                                         gasnet_team_handle_t team,
                                         DCMF_Geometry_t *geometry,
                                         unsigned root,
                                         char *src,
                                         char *dst,
                                         unsigned bytes)
{
#ifdef G_DCMF_COLL_TRACE
  fprintf(stderr, "gasnete_coll_dcmf_bcast_init is executed.\n");
#endif
  bcast->team = team;
  bcast->geometry = geometry;
  bcast->cb_done.function = gasnete_dcmf_bcast_cb_done;
  bcast->cb_done.clientdata = (void *)&bcast->active;
  bcast->active = 1;
  bcast->consistency = DCMF_MATCH_CONSISTENCY;
  bcast->root = root;
  bcast->src = src; 
  bcast->dst = dst;
  bcast->bytes = bytes;
}

/* This function should be called by only one thread at a time. */
static int gasnete_coll_pf_bcast_dcmf(gasnete_coll_op_t *op GASNETE_THREAD_FARG) 
{
  gasnete_coll_generic_data_t *data;
  gasnet_team_handle_t team;
  gasnete_dcmf_bcast_data_t *bcast;
  gasnete_coll_team_dcmf_t *dcmf_tp;  
  
#ifdef G_DCMF_COLL_TRACE
  fprintf(stderr, "gasnete_coll_pf_bcast_dcmf is executed.\n");
#endif
  
  gasneti_assert(op != NULL);
  team = op->team; 
  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  data = op->data;
  gasneti_assert(data != NULL);
  bcast = (gasnete_dcmf_bcast_data_t *)data->private_data;
  gasneti_assert(bcast != NULL);
  
  switch (data->state) {
  case 0:	
    if (gasnete_dcmf_busy)
      break;

    if (!gasnete_coll_generic_insync(op->team, data))
      break;

    data->state = 1;
#ifdef G_DCMF_COLL_TRACE
    fprintf(stderr, "gasnete_coll_pf_bcast_dcmf state 1.\n");
#endif

    
  case 1:	/* Initiate data movement */
    gasnete_dcmf_busy = 1;  
    if (gasnete_dcmf_bcast_enabled[TREE_BROADCAST] && team==GASNET_TEAM_ALL)
      {
        GASNETC_DCMF_LOCK();
        DCMF_SAFE(DCMF_GlobalBcast(&gasnete_dcmf_globalbcast_proto,
                                   &bcast->request.global, 
                                   bcast->cb_done,
                                   DCMF_MATCH_CONSISTENCY,
                                   team->rel2act_map[bcast->root],
                                   (team->myrank==bcast->root) ? bcast->src : bcast->dst, 
                                   bcast->bytes));
        GASNETC_DCMF_UNLOCK();
      }
    else
      {
        GASNETC_DCMF_LOCK();
        DCMF_SAFE(DCMF_Broadcast(dcmf_tp->bcast_proto,
                                 &bcast->request.coll, 
                                 bcast->cb_done,
                                 bcast->consistency,
                                 bcast->geometry, 
                                 team->rel2act_map[bcast->root],
                                 (team->myrank==bcast->root) ? bcast->src : bcast->dst, 
                                 bcast->bytes)); 
        GASNETC_DCMF_UNLOCK();
      }
    data->state = 2;
#ifdef G_DCMF_COLL_TRACE
    fprintf(stderr, "gasnete_coll_pf_bcast_dcmf state 2.\n");
#endif
    
  case 2:	/* Sync data movement */
    DCMF_Messager_advance();
    if (bcast->active)
      break;
    gasnete_dcmf_busy = 0;
    data->state = 3;
#ifdef G_DCMF_COLL_TRACE
    fprintf(stderr, "gasnete_coll_pf_bcast_dcmf state 3.\n");
#endif
    
  case 3: 
    if(team->myrank == bcast->root) 
      GASNETE_FAST_UNALIGNED_MEMCPY(bcast->dst, bcast->src, bcast->bytes);
    
    if (!gasnete_coll_generic_outsync(op->team, data))
      break;
    
    /* clean up storage space */
    gasneti_free_aligned(bcast);
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);

#ifdef G_DCMF_COLL_TRACE
    fprintf(stderr, "gasnete_coll_pf_bcast_dcmf last state.\n");
#endif
    
    return (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);

  defaul:
    gasneti_fatalerror("gasnete_coll_pf_bcast_dcmf: invalid state %d\n", data->state);
  } /* end of switch (data->state) */
  
  return 0;
}

gasnet_coll_handle_t 
gasnete_coll_broadcast_nb_dcmf(gasnet_team_handle_t team,
                               void * dst,
                               gasnet_image_t srcimage, 
                               void *src,
                               size_t nbytes, 
                               int flags,
                               /* gasnete_coll_implementation_t coll_params, */
                               uint32_t sequence
                               GASNETE_THREAD_FARG)
{
  gasnete_dcmf_bcast_data_t *bcast;
  gasnete_coll_team_dcmf_t *dcmf_tp;
  uint32_t root;
  gasnete_coll_generic_data_t *data;

  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  gasneti_assert(dcmf_tp != NULL);
  
  if (!gasnete_use_dcmf_coll 
      || !gasnete_use_dcmf_bcast
      || (dcmf_tp->bcast_proto == NULL 
          && !(team == GASNET_TEAM_ALL && gasnete_dcmf_bcast_enabled[TREE_BROADCAST]))) {
    return gasnete_coll_broadcast_nb_default(team, dst, srcimage, src, nbytes, 
                                             flags, sequence GASNETE_THREAD_PASS); 
  }

#ifdef G_DCMF_COLL_TRACE
  fprintf(stderr, "gasnete_coll_broadcast_nb_dcmf is executed\n");
#endif

  gasneti_assert(gasnete_coll_dcmf_inited == 1);
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  
  root = gasnete_coll_image_node(team, srcimage);
  data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
  gasneti_assert(data != NULL);
  GASNETE_COLL_GENERIC_SET_TAG(data, broadcast);
  data->options = GASNETE_COLL_GENERIC_OPT_INSYNC_IF(flags & GASNET_COLL_IN_ALLSYNC) 
    | GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC);
  
  /* initialize DCMF specific data structures */
  bcast = (gasnete_dcmf_bcast_data_t *)gasneti_malloc_aligned(64, sizeof(gasnete_dcmf_bcast_data_t));
  gasnete_coll_dcmf_bcast_init(bcast, team, &dcmf_tp->geometry, root, src, dst, nbytes);
  data->private_data = bcast;
  
#ifdef G_DCMF_COLL_TRACE
  gasnete_coll_dcmf_bcast_print(stderr, bcast);
#endif

  return gasnete_coll_op_generic_init_with_scratch(team, flags, data, 
                                                   gasnete_coll_pf_bcast_dcmf, 
                                                   sequence, NULL, 0, NULL, NULL
                                                   GASNETE_THREAD_PASS);
}


void gasnete_coll_broadcast_dcmf(gasnet_team_handle_t team, void *dst,
                                 gasnet_image_t srcimage, void *src,
                                 size_t nbytes, int flags GASNETE_THREAD_FARG)
{
  gasnete_coll_team_dcmf_t *dcmf_tp;
  const unsigned root = gasnete_coll_image_node(team, srcimage);
  DCMF_Callback_t cb_done;
  volatile unsigned active;

  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  gasneti_assert(dcmf_tp != NULL);
  
  if (!gasnete_use_dcmf_coll
      || !gasnete_use_dcmf_bcast
      || (dcmf_tp->bcast_proto == NULL 
          && !(team == GASNET_TEAM_ALL && gasnete_dcmf_bcast_enabled[TREE_BROADCAST]))) {
      gasnet_coll_handle_t handle;
      handle=gasnete_coll_broadcast_nb_default(team, dst, srcimage, src, nbytes,
                                               flags, 0 GASNETE_THREAD_PASS);
      gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
    return;
  }
    
  gasneti_assert(gasnete_coll_dcmf_inited == 1);
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  
#ifdef G_DCMF_COLL_TRACE
  fprintf(stderr, "rank %d, gasnete_coll_broadcast_dcmf is executed.\n",
          team->myrank);
#endif
  
  active = 1;
  cb_done.function = gasnete_dcmf_bcast_cb_done;
  cb_done.clientdata = (void *)&active;
  
  /* optional barrier for the out_allsync mode, should be a team barrier */
  if (flags & GASNET_COLL_IN_ALLSYNC)
    gasnete_coll_teambarrier_dcmf(team);

  if (gasnete_dcmf_bcast_enabled[TREE_BROADCAST] && team==GASNET_TEAM_ALL)
    {
      DCMF_Request_t  request;
      
      GASNETC_DCMF_LOCK();
      DCMF_SAFE(DCMF_GlobalBcast(&gasnete_dcmf_globalbcast_proto,
                                 &request, 
                                 cb_done,
                                 DCMF_MATCH_CONSISTENCY,
                                 team->rel2act_map[root],
                                 (team->myrank == root) ? src : dst,
                                 nbytes));
      GASNETC_DCMF_UNLOCK();
      /* Fix for bug 2791.  GASNet may deadlock if polling the
         dcmf-network in a tight loop and not making necessary
         progress in other GASNet components such as processing the AM
         queue.  gasnetc_AMPoll() should be used in place of
         Dcmf_Messager_Advance() because DCMF_Messager_Advance() only
         makes progress in DCMF but not in GASNet.  gasnet_AMPoll()
         makes progress in both GASNet and DCMF. */
      gasneti_polluntil(active == 0);
    } 
  else 
    {
      DCMF_CollectiveRequest_t   request;
      
      /* Is is really necessary to lock ? */
      GASNETC_DCMF_LOCK();
      DCMF_Broadcast(dcmf_tp->bcast_proto,
                     &request,
                     cb_done,
                     DCMF_MATCH_CONSISTENCY,
                     &dcmf_tp->geometry,
                     team->rel2act_map[root],
                     (team->myrank == root) ? src : dst,
                     nbytes);
      GASNETC_DCMF_UNLOCK();
      gasneti_polluntil(active == 0);
    }
  
  if (team->myrank == root)
    GASNETE_FAST_UNALIGNED_MEMCPY(dst, src, nbytes);

  /* optional barrier for the out_allsync mode, should be a team barrier */
  if (flags & GASNET_COLL_OUT_ALLSYNC)
    gasnete_coll_teambarrier_dcmf(team);
}

 
void gasnete_coll_dcmf_bcast_print(FILE *fp, gasnete_dcmf_bcast_data_t *bcast)
{
  gasneti_assert(bcast != NULL);
  fprintf(stderr, "%d, bcast->root %u\n", bcast->team->myrank, bcast->root);
  fprintf(stderr, "%d, bcast->src %p\n", bcast->team->myrank, bcast->src);
  fprintf(stderr, "%d, bcast->bytes %d\n", bcast->team->myrank, bcast->bytes);
}
