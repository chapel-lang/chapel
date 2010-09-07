/* $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_coll_allreduce_dcmf.c,v $
 * $Date: 2010/05/10 23:15:37 $
 * $Revision: 1.2.2.2 $
 * Description: GASNet broadcast implementation on DCMF
 * Copyright 2010, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_allreduce_dcmf.h>

/* #define G_DCMF_COLL_TRACE */

static int gasnete_use_dcmf_allreduce;

/* Allreduce protocol registration data */
static DCMF_CollectiveProtocol_t gasnete_dcmf_allreduce_proto[DCMF_NUM_ALLREDUCE_PROTOCOLS];
static int gasnete_dcmf_allreduce_enabled[DCMF_NUM_ALLREDUCE_PROTOCOLS];

void gasnete_coll_allreduce_proto_register(void)
{
  DCMF_Allreduce_Configuration_t allreduce_conf;
  allreduce_conf.cb_geometry = gasnete_dcmf_get_geometry;

  GASNETC_DCMF_LOCK(); /* for DCMF_SAFE */
  
  gasnete_use_dcmf_allreduce =
    gasneti_getenv_yesno_withdefault("GASNET_USE_DCMF_ALLREDUCE", 1);
  
  /* tree allreduce registration */
  gasnete_dcmf_allreduce_enabled[DCMF_TREE_ALLREDUCE_PROTOCOL] =
    gasneti_getenv_yesno_withdefault("DCMF_TREE_ALLREDUCE_PROTOCOL", 1);
  if (gasnete_dcmf_allreduce_enabled[DCMF_TREE_ALLREDUCE_PROTOCOL]) 
    {
      allreduce_conf.protocol = DCMF_TREE_ALLREDUCE_PROTOCOL;
      DCMF_SAFE(DCMF_Allreduce_register(&gasnete_dcmf_allreduce_proto[DCMF_TREE_ALLREDUCE_PROTOCOL], 
                                        &allreduce_conf));
    }
  
  /* torus allreduce registration */
  gasnete_dcmf_allreduce_enabled[DCMF_TORUS_BINOMIAL_ALLREDUCE_PROTOCOL] =
    gasneti_getenv_yesno_withdefault("DCMF_TORUS_BINOMIAL_ALLREDUCE_PROTOCOL", 1);
  if (gasnete_dcmf_allreduce_enabled[DCMF_TORUS_BINOMIAL_ALLREDUCE_PROTOCOL]) 
    {
      allreduce_conf.protocol = DCMF_TORUS_BINOMIAL_ALLREDUCE_PROTOCOL;
      DCMF_SAFE(DCMF_Allreduce_register(&gasnete_dcmf_allreduce_proto[DCMF_TORUS_BINOMIAL_ALLREDUCE_PROTOCOL],
                                        &allreduce_conf));
    }
  
  gasnete_dcmf_allreduce_enabled[DCMF_TORUS_RECTANGLE_ALLREDUCE_PROTOCOL] =
    gasneti_getenv_yesno_withdefault("DCMF_TORUS_RECTANGLE_ALLREDUCE_PROTOCOL", 1);
  if (gasnete_dcmf_allreduce_enabled[DCMF_TORUS_RECTANGLE_ALLREDUCE_PROTOCOL])
    {
      allreduce_conf.protocol = DCMF_TORUS_RECTANGLE_ALLREDUCE_PROTOCOL;
      DCMF_SAFE(DCMF_Allreduce_register(&gasnete_dcmf_allreduce_proto[DCMF_TORUS_RECTANGLE_ALLREDUCE_PROTOCOL],
                                        &allreduce_conf));
    }
  
  gasnete_dcmf_allreduce_enabled[DCMF_TORUS_RECTANGLE_RING_ALLREDUCE_PROTOCOL] =
    gasneti_getenv_yesno_withdefault("DCMF_TORUS_RECTANGLE_RING_ALLREDUCE_PROTOCOL", 1);
  if (gasnete_dcmf_allreduce_enabled[DCMF_TORUS_RECTANGLE_RING_ALLREDUCE_PROTOCOL]) 
    {
      allreduce_conf.protocol = DCMF_TORUS_RECTANGLE_RING_ALLREDUCE_PROTOCOL;
      DCMF_SAFE(DCMF_Allreduce_register(&gasnete_dcmf_allreduce_proto[DCMF_TORUS_RECTANGLE_RING_ALLREDUCE_PROTOCOL],
                                        &allreduce_conf));
    }
  
  gasnete_dcmf_allreduce_enabled[DCMF_TREE_PIPELINED_ALLREDUCE_PROTOCOL] =
    gasneti_getenv_yesno_withdefault("DCMF_TREE_PIPELINED_ALLREDUCE_PROTOCOL", 0);
  if (gasnete_dcmf_allreduce_enabled[DCMF_TREE_PIPELINED_ALLREDUCE_PROTOCOL]) 
    {
      allreduce_conf.protocol = DCMF_TREE_PIPELINED_ALLREDUCE_PROTOCOL;
      DCMF_SAFE(DCMF_Allreduce_register(&gasnete_dcmf_allreduce_proto[DCMF_TREE_PIPELINED_ALLREDUCE_PROTOCOL],
                                        &allreduce_conf));
    }

  gasnete_dcmf_allreduce_enabled[DCMF_TORUS_SHORT_BINOMIAL_ALLREDUCE_PROTOCOL] =
    gasneti_getenv_yesno_withdefault("DCMF_TORUS_SHORT_BINOMIAL_ALLREDUCE_PROTOCOL", 1);
  if (gasnete_dcmf_allreduce_enabled[DCMF_TORUS_SHORT_BINOMIAL_ALLREDUCE_PROTOCOL]) 
    {
      allreduce_conf.protocol = DCMF_TORUS_SHORT_BINOMIAL_ALLREDUCE_PROTOCOL;
      DCMF_SAFE(DCMF_Allreduce_register(&gasnete_dcmf_allreduce_proto[DCMF_TORUS_SHORT_BINOMIAL_ALLREDUCE_PROTOCOL],
                                        &allreduce_conf));
    }
  
  gasnete_dcmf_allreduce_enabled[DCMF_TREE_DPUT_PIPELINED_ALLREDUCE_PROTOCOL] =
    gasneti_getenv_yesno_withdefault("DCMF_TREE_DPUT_PIPELINED_ALLREDUCE_PROTOCOL", 0);
  if (gasnete_dcmf_allreduce_enabled[DCMF_TREE_DPUT_PIPELINED_ALLREDUCE_PROTOCOL]) 
    {
      allreduce_conf.protocol = DCMF_TREE_DPUT_PIPELINED_ALLREDUCE_PROTOCOL;
      DCMF_SAFE(DCMF_Allreduce_register(&gasnete_dcmf_allreduce_proto[DCMF_TREE_DPUT_PIPELINED_ALLREDUCE_PROTOCOL],
                                        &allreduce_conf));
    }

  /* disable the allreduce protocols not supported */
  gasnete_dcmf_allreduce_enabled[DCMF_TORUS_ASYNC_BINOMIAL_ALLREDUCE_PROTOCOL] = 0;
  gasnete_dcmf_allreduce_enabled[DCMF_TORUS_ASYNC_RECTANGLE_ALLREDUCE_PROTOCOL] = 0;
  gasnete_dcmf_allreduce_enabled[DCMF_TORUS_ASYNC_RECTANGLE_RING_ALLREDUCE_PROTOCOL] = 0;
  gasnete_dcmf_allreduce_enabled[DCMF_TORUS_ASYNC_SHORT_BINOMIAL_ALLREDUCE_PROTOCOL] = 0;
  gasnete_dcmf_allreduce_enabled[DCMF_TORUS_ASYNC_SHORT_RECTANGLE_ALLREDUCE_PROTOCOL] = 0;
  gasnete_dcmf_allreduce_enabled[DCMF_TORUS_RRING_DPUT_ALLREDUCE_PROTOCOL_SINGLETH] = 0;

  GASNETC_DCMF_UNLOCK();
}

int gasnete_coll_allreduce_set_proto(gasnet_team_handle_t team,
                                     DCMF_Allreduce_Protocol kind)
{
  gasnete_coll_team_dcmf_t *dcmf_tp;
  
  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  gasneti_assert(dcmf_tp != NULL);

  if (kind < DCMF_NUM_ALLREDUCE_PROTOCOLS
      && gasnete_dcmf_allreduce_enabled[kind]
      && DCMF_Geometry_analyze(&dcmf_tp->geometry, &gasnete_dcmf_allreduce_proto[kind])) 
    {
      dcmf_tp->allreduce_proto = &gasnete_dcmf_allreduce_proto[kind];
      return 0; /* SUCCESS */
    }
  return 1; /* FAILED */
}

void gasnete_coll_allreduce_set_default_proto(gasnet_team_handle_t team)
{
  gasnete_coll_team_dcmf_t *dcmf_tp;
  
  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  gasneti_assert(dcmf_tp != NULL);
  
  if (gasnete_coll_allreduce_set_proto(team, DCMF_TREE_ALLREDUCE_PROTOCOL))
    if (gasnete_coll_allreduce_set_proto(team, DCMF_TORUS_BINOMIAL_ALLREDUCE_PROTOCOL))
      dcmf_tp->allreduce_proto = NULL;
  
  /* set the named barrier, which use allreduce, to use the same
     protocol as allreduce */
  gasneti_assert(dcmf_tp->allreduce_proto != NULL);
}

int gasnete_coll_namedbarrier_set_proto(gasnet_team_handle_t team,
                                        DCMF_Allreduce_Protocol kind)
{
  gasnete_coll_team_dcmf_t *dcmf_tp;
  
  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  gasneti_assert(dcmf_tp != NULL);

  if (kind < DCMF_NUM_ALLREDUCE_PROTOCOLS
      && gasnete_dcmf_allreduce_enabled[kind]
      && DCMF_Geometry_analyze(&dcmf_tp->geometry, &gasnete_dcmf_allreduce_proto[kind])) 
    {
      dcmf_tp->named_barrier_proto = &gasnete_dcmf_allreduce_proto[kind];
      return 0; /* SUCCESS */
    }
  return 1; /* FAILED */
}

void gasnete_coll_namedbarrier_set_default_proto(gasnet_team_handle_t team)
{
  const char *barrier_protocol;
  int rv = 0; 

  if (team == GASNET_TEAM_ALL)
    barrier_protocol = gasneti_getenv_withdefault("GASNET_DCMF_NAMEDBARRIER_PROTOCOL", 
                                                  "DCMF_TREE_ALLREDUCE");
  else
    barrier_protocol = gasneti_getenv_withdefault("GASNET_DCMF_NAMEDBARRIER_PROTOCOL", 
                                                  "DCMF_TORUS_SHORT_BINOMIAL_ALLREDUCE");

  if (!strcmp(barrier_protocol, "DCMF_TREE_ALLREDUCE")) 
    rv = gasnete_coll_namedbarrier_set_proto(team, DCMF_TREE_ALLREDUCE_PROTOCOL);
  else if (!strcmp(barrier_protocol, "DCMF_TORUS_BINOMIAL_ALLREDUCE")) 
    rv = gasnete_coll_namedbarrier_set_proto(team, DCMF_TORUS_BINOMIAL_ALLREDUCE_PROTOCOL);
  else if (!strcmp(barrier_protocol, "DCMF_TORUS_RECTANGLE_ALLREDUCE_PROTOCOL")) 
    rv = gasnete_coll_namedbarrier_set_proto(team, DCMF_TORUS_RECTANGLE_ALLREDUCE_PROTOCOL);
  else if (!strcmp(barrier_protocol, "DCMF_TORUS_RECTANGLE_RING_ALLREDUCE")) 
    rv = gasnete_coll_namedbarrier_set_proto(team, DCMF_TORUS_RECTANGLE_RING_ALLREDUCE_PROTOCOL);
  else if (!strcmp(barrier_protocol, "DCMF_TREE_PIPELINED_ALLREDUCE")) 
    rv = gasnete_coll_namedbarrier_set_proto(team, DCMF_TREE_PIPELINED_ALLREDUCE_PROTOCOL);
  else if (!strcmp(barrier_protocol, "DCMF_TORUS_SHORT_BINOMIAL_ALLREDUCE")) 
    rv = gasnete_coll_namedbarrier_set_proto(team, DCMF_TORUS_SHORT_BINOMIAL_ALLREDUCE_PROTOCOL);
  else if (!strcmp(barrier_protocol, "DCMF_TREE_DPUT_PIPELINED_ALLREDUCE")) 
    rv = gasnete_coll_namedbarrier_set_proto(team, DCMF_TREE_DPUT_PIPELINED_ALLREDUCE_PROTOCOL);
  else 
    gasneti_fatalerror("unknown dcmf named barrier protocol: %s", barrier_protocol);

  if (rv)
    gasneti_fatalerror("Failed to set the default named barrier protocol for team %p.\n",
                       team);
}

static void gasnete_dcmf_allreduce_cb_done(void *clientdata, DCMF_Error_t *e)
{
  unsigned *p = (unsigned *)clientdata;
  *p = 0;
}

static void gasnete_coll_dcmf_allreduce_init(gasnete_dcmf_allreduce_data_t *allreduce,
                                             gasnet_team_handle_t team,
                                             DCMF_Geometry_t *geometry,
                                             char *src,
                                             char *dst,
                                             unsigned count,
                                             DCMF_Dt datatype,
                                             DCMF_Op reduceop)
{
#ifdef G_DCMF_COLL_TRACE
  fprintf(stderr, "gasnete_coll_dcmf_allreduce_init is executed.\n");
#endif
  allreduce->team = team;
  allreduce->geometry = geometry;
  allreduce->cb_done.function = gasnete_dcmf_allreduce_cb_done;
  allreduce->cb_done.clientdata = (void *)&allreduce->active;
  allreduce->active = 1;
  allreduce->consistency = DCMF_MATCH_CONSISTENCY;
  allreduce->src = src; 
  allreduce->dst = dst;
  allreduce->count = count;
  allreduce->datatype = datatype;
  allreduce->reduceop = reduceop;
}

/* This function should be called by only one thread at a time. */
static int gasnete_coll_pf_allreduce_dcmf(gasnete_coll_op_t *op GASNETE_THREAD_FARG) 
{
  gasnete_coll_generic_data_t *data;
  gasnet_team_handle_t team;
  gasnete_dcmf_allreduce_data_t *allreduce;
  gasnete_coll_team_dcmf_t *dcmf_tp;  
  
#ifdef G_DCMF_COLL_TRACE
  fprintf(stderr, "gasnete_coll_pf_allreduce_dcmf is executed.\n");
#endif
  
  gasneti_assert(op != NULL);
  team = op->team; 
  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  data = op->data;
  gasneti_assert(data != NULL);
  allreduce = (gasnete_dcmf_allreduce_data_t *)data->private_data;
  gasneti_assert(allreduce != NULL);
  
  switch (data->state) {
  case 0:	
    /* DCMF_Allreduce has an explicit barrier for all processes at the
     * beginning.
     */
    if (gasnete_dcmf_busy)
      break;
    data->state = 1;
#ifdef G_DCMF_COLL_TRACE
    fprintf(stderr, "gasnete_coll_pf_allreduce_dcmf state 1.\n");
#endif

    
  case 1:	/* Initiate data movement */
    gasnete_dcmf_busy = 1;  
    GASNETC_DCMF_LOCK();
    DCMF_SAFE(DCMF_Allreduce(dcmf_tp->allreduce_proto,
                             &allreduce->request, 
                             allreduce->cb_done,
                             allreduce->consistency,
                             allreduce->geometry, 
                             allreduce->src,
                             allreduce->dst, 
                             allreduce->count,
                             allreduce->datatype,
                             allreduce->reduceop)); 
    GASNETC_DCMF_UNLOCK();
    data->state = 2;
#ifdef G_DCMF_COLL_TRACE
    fprintf(stderr, "gasnete_coll_pf_allreduce_dcmf state 2.\n");
#endif
    
  case 2:	/* Sync data movement */
    DCMF_Messager_advance();
    if (allreduce->active)
      break;
    gasnete_dcmf_busy = 0;
    data->state = 3;
#ifdef G_DCMF_COLL_TRACE
    fprintf(stderr, "gasnete_coll_pf_allreduce_dcmf state 3.\n");
#endif
    
  case 3: 
    if (!gasnete_coll_generic_outsync(op->team, data))
      break;
    
    /* clean up storage space */
    gasneti_free_aligned(allreduce);
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);

#ifdef G_DCMF_COLL_TRACE
    fprintf(stderr, "gasnete_coll_pf_allreduce_dcmf last state.\n");
#endif
    
    return (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);

  defaul:
    gasneti_fatalerror("gasnete_coll_pf_allreduce_dcmf: invalid state %d\n", data->state);
  } /* end of switch (data->state) */
  
  return 0;
}

gasnet_coll_handle_t gasnete_coll_allreduce_nb_dcmf(gasnet_team_handle_t team,
                                                    void * dst,
                                                    void *src,
                                                    size_t count, 
                                                    DCMF_Dt datatype,
                                                    DCMF_Op reduceop,
                                                    int flags,
                                                    /* gasnete_coll_implementation_t coll_params, */
                                                    uint32_t sequence
                                                    GASNETE_THREAD_FARG)
{
  gasnete_dcmf_allreduce_data_t *allreduce;
  gasnete_coll_team_dcmf_t *dcmf_tp;
  gasnete_coll_generic_data_t *data;

  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  gasneti_assert(dcmf_tp != NULL);
  
  if (!gasnete_use_dcmf_coll 
      || !gasnete_use_dcmf_allreduce
      || (dcmf_tp->allreduce_proto == NULL)) 
    {
      gasneti_fatalerror("gasnete_coll_allreduce_nb_dcmf is disabled or unavailable.\n");
    } 

#ifdef G_DCMF_COLL_TRACE
  fprintf(stderr, "gasnete_coll_allreduce_nb_dcmf is executed\n");
#endif

  gasneti_assert(gasnete_coll_dcmf_inited == 1);
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  
  data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
  gasneti_assert(data != NULL);
  /* GASNETE_COLL_GENERIC_SET_TAG(data, allreduce); */ 
  data->options=GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF(flags & GASNET_COLL_OUT_ALLSYNC);
  
  /* initialize DCMF specific data structures */
  allreduce = (gasnete_dcmf_allreduce_data_t *)gasneti_malloc_aligned(64, sizeof(gasnete_dcmf_allreduce_data_t));
  gasnete_coll_dcmf_allreduce_init(allreduce, team, &dcmf_tp->geometry, src, dst, count, datatype, reduceop);
  data->private_data = allreduce;
  
  return gasnete_coll_op_generic_init_with_scratch(team, flags, data, 
                                                   gasnete_coll_pf_allreduce_dcmf, 
                                                   sequence, NULL, 0, NULL, NULL
                                                   GASNETE_THREAD_PASS);
}
