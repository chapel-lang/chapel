/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_coll_exchange_dcmf.c,v $
 *     $Date: 2010/05/10 23:15:37 $
 * $Revision: 1.7.2.2 $
 * Description: GASNet exchange (alltoall) implementation on DCMF
 * Copyright 2009, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_exchange_dcmf.h>

/* #define G_DCMF_COLL_TRACE */

static int gasnete_use_dcmf_a2a;

static DCMF_CollectiveProtocol_t gasnete_dcmf_a2a_protos[G_DCMF_A2A_PROTO_NUM];
static int gasnete_dcmf_a2a_enabled[G_DCMF_A2A_PROTO_NUM];

/* callback function for DCMF_Alltoallv() */
static void gasnete_dcmf_coll_cb_done(void *clientdata, DCMF_Error_t *e)
{
  unsigned *p = (unsigned *)clientdata;
  *p = 0;
}

/* alltoall protocol registration */
void gasnete_coll_a2a_proto_register(void)
{
  DCMF_Alltoallv_Configuration_t alltoallv_conf;
  
  GASNETC_DCMF_LOCK(); /* for DCMF_SAFE */

  gasnete_use_dcmf_a2a =
    gasneti_getenv_yesno_withdefault("GASNET_USE_DCMF_EXCHANGE", 1);

  gasnete_dcmf_a2a_enabled[TORUS_ALLTOALLV] =
    gasneti_getenv_yesno_withdefault("GASNET_DCMF_TORUS_ALLTOALLV", 1);
  if (gasnete_dcmf_a2a_enabled[TORUS_ALLTOALLV])
    {
      alltoallv_conf.protocol = DCMF_TORUS_ALLTOALLV_PROTOCOL;
      DCMF_SAFE(DCMF_Alltoallv_register(&gasnete_dcmf_a2a_protos[TORUS_ALLTOALLV], 
                                        &alltoallv_conf));
    }
  
  GASNETC_DCMF_UNLOCK();
}

int gasnete_coll_a2a_set_proto(gasnet_team_handle_t team,
                               gasnete_dcmf_a2a_proto_t kind)
{
  gasnete_coll_team_dcmf_t *dcmf_tp;
  
  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  gasneti_assert(dcmf_tp != NULL);
  gasneti_assert(kind < G_DCMF_A2A_PROTO_NUM);

  if (kind < G_DCMF_A2A_PROTO_NUM 
      && DCMF_Geometry_analyze(&dcmf_tp->geometry, &gasnete_dcmf_a2a_protos[kind])
      && gasnete_dcmf_a2a_enabled[kind]
      && team->total_ranks >= 2)
    {
      dcmf_tp->a2a_proto = &gasnete_dcmf_a2a_protos[kind];
      return 0; /* SUCCESS */
    }
  return 1; /* FAILED */
}

void gasnete_coll_a2a_set_default_proto(gasnet_team_handle_t team)
{
  int rv;
  gasnete_coll_team_dcmf_t *dcmf_tp;
  
  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  gasneti_assert(dcmf_tp != NULL);
  
  rv = gasnete_coll_a2a_set_proto(team, TORUS_ALLTOALLV);
  if (rv)
    dcmf_tp->a2a_proto = NULL;
}

void gasnete_coll_dcmf_a2a_init(gasnete_dcmf_a2a_data_t *a2a,
                                DCMF_CollectiveProtocol_t *registration,
                                DCMF_Geometry_t *geometry,
                                int nprocs, 
                                void *src, 
                                void *dst,
                                size_t nbytes)

{
  int i;

#ifdef G_DCMF_COLL_TRACE
  fprintf(stderr, "gasnete_coll_dcmf_a2a_init is executed.\n");
#endif 

  gasneti_assert(a2a != NULL);
  a2a->registration = registration;
  a2a->geometry = geometry;
  a2a->cb_done.function = gasnete_dcmf_coll_cb_done;
  a2a->cb_done.clientdata = &a2a->active;
  a2a->active = 1;
  a2a->consistency = DCMF_MATCH_CONSISTENCY;
  a2a->sndbuf = src;
  a2a->rcvbuf = dst;

  a2a->sndlens = (unsigned *)gasneti_malloc(nprocs*sizeof(unsigned));
  a2a->rcvlens = (unsigned *)gasneti_malloc(nprocs*sizeof(unsigned));
  a2a->sdispls = (unsigned *)gasneti_malloc(nprocs*sizeof(unsigned));
  a2a->rdispls = (unsigned *)gasneti_malloc(nprocs*sizeof(unsigned));
  a2a->sndcounters = (unsigned *)gasneti_malloc(nprocs*sizeof(unsigned));
  a2a->rcvcounters = (unsigned *)gasneti_malloc(nprocs*sizeof(unsigned));
  
  for (i = 0; i < nprocs; i++) {
    a2a->sndlens[i] = nbytes;
    a2a->sdispls[i] = i * nbytes;
    a2a->rcvlens[i] = nbytes;
    a2a->rdispls[i] = i * nbytes;
  }
}
       
void gasnete_coll_dcmf_a2a_fini(gasnete_dcmf_a2a_data_t *a2a)
{
  gasneti_assert(a2a != NULL);
#ifdef G_DCMF_COLL_TRACE
  fprintf(stderr, "gasnete_coll_dcmf_a2a_fini is executed.\n");
#endif 
  gasneti_free(a2a->sndlens);
  gasneti_free(a2a->rcvlens);
  gasneti_free(a2a->sdispls);
  gasneti_free(a2a->rdispls);
  gasneti_free(a2a->sndcounters);
  gasneti_free(a2a->rcvcounters);
}

static int gasnete_coll_pf_exchg_dcmf(gasnete_coll_op_t *op GASNETE_THREAD_FARG) 
{
  gasnete_coll_generic_data_t *data;
  gasnete_dcmf_a2a_data_t *a2a;

  gasneti_assert(op != NULL);
  data = op->data;
  gasneti_assert(data != NULL);
  a2a = (gasnete_dcmf_a2a_data_t *)data->private_data;
  gasneti_assert(a2a != NULL);

  switch (data->state) {
  case 0:	
#ifdef G_DCMF_COLL_TRACE
    fprintf(stderr, "gasnete_coll_pf_exchg_dcmf is executed.\n");
#endif
    if (gasnete_dcmf_busy)
      break;
    data->state = 1;
    
  case 1:	/* Initiate data movement */
    gasnete_dcmf_busy = 1;
    /**
     * DCMF_Alltoallv has an explicit barrier at the beginning and an
     * implicit barrier at the end. see class A2AProtocol and class
     * AlltoallFactory, and function AlltoallFactory::generate().
     */
    GASNETC_DCMF_LOCK();
    DCMF_SAFE(DCMF_Alltoallv(a2a->registration,
                             &a2a->request, 
                             a2a->cb_done,
                             a2a->consistency,
                             a2a->geometry, 
                             a2a->sndbuf,
                             a2a->sndlens, 
                             a2a->sdispls, 
                             a2a->rcvbuf,
                             a2a->rcvlens, 
                             a2a->rdispls,
                             a2a->sndcounters, 
                             a2a->rcvcounters));
    GASNETC_DCMF_UNLOCK();
    data->state = 2;
    
  case 2:	/* Sync data movement */
    DCMF_Messager_advance();
    if (a2a->active)
      break;

    /* Fix for Bug 2786. Because DCMF_Alltoallv() doesn't copy the
       data from the send buffer to the receive buffer when the send
       node is the same as the receive node.  Therefore, the local
       data copy is performed here in GASNet. */ 

    {
        gasnet_team_handle_t team = op->team;
        char *dst, *src;
        size_t nbytes;

        dst = (char *)a2a->rcvbuf+a2a->rdispls[team->myrank];
        src = (char *)a2a->sndbuf+a2a->sdispls[team->myrank];
        nbytes = a2a->rcvlens[team->myrank];
        GASNETE_FAST_UNALIGNED_MEMCPY(dst, src, nbytes);
    }
    gasnete_dcmf_busy = 0;
    data->state = 3;
    
  case 3: 
    if (!gasnete_coll_generic_outsync(op->team, data))
      break;

    /* clean up storage space */
    gasnete_coll_dcmf_a2a_fini(a2a);
    gasneti_free_aligned(a2a);
    gasnete_coll_generic_free(op->team, data GASNETE_THREAD_PASS);
    return (GASNETE_COLL_OP_COMPLETE | GASNETE_COLL_OP_INACTIVE);
  }
  
  return 0;
}

/** Non-blocking version of gasnete_coll_exchange */
gasnet_coll_handle_t gasnete_coll_exchange_nb_dcmf(gasnet_team_handle_t team, 
                                                   void *dst, void *src, 
                                                   size_t nbytes, int flags, 
                                                   uint32_t sequence
                                                   GASNETE_THREAD_FARG)
{
  gasnete_coll_generic_data_t *data;
  int i;
  int nprocs =  team->total_ranks;
  gasnete_dcmf_a2a_data_t *a2a;
  gasnete_coll_team_dcmf_t *dcmf_tp;
  
  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  gasneti_assert(dcmf_tp != NULL);
  
  if (!gasnete_use_dcmf_coll 
      || !gasnete_use_dcmf_a2a  
      || dcmf_tp->a2a_proto == NULL) {
    return gasnete_coll_exchange_nb_default(team, dst, src, nbytes, flags, 
                                            sequence GASNETE_THREAD_PASS);
  }
  
  gasneti_assert(gasnete_coll_dcmf_inited == 1);
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));

#ifdef G_DCMF_COLL_TRACE
  fprintf(stderr, "gasnete_coll_exchange_nb_dcmf is executed!\n");
#endif

  a2a = (gasnete_dcmf_a2a_data_t *)
    gasneti_malloc_aligned(64, sizeof(gasnete_dcmf_a2a_data_t));
  gasnete_coll_dcmf_a2a_init(a2a, dcmf_tp->a2a_proto,
                             &dcmf_tp->geometry, nprocs, src, dst, nbytes);
  
  data = gasnete_coll_generic_alloc(GASNETE_THREAD_PASS_ALONE);
  data->private_data = a2a;
  GASNETE_COLL_GENERIC_SET_TAG(data, exchange);
  data->options = 
      GASNETE_COLL_GENERIC_OPT_OUTSYNC_IF((flags & GASNET_COLL_OUT_ALLSYNC));
  
  return gasnete_coll_op_generic_init_with_scratch(team, flags, data, 
                                                   gasnete_coll_pf_exchg_dcmf, 
                                                   sequence, NULL, 0, NULL, 
                                                   NULL GASNETE_THREAD_PASS);
}

/** Blocking version of gasnete_coll_exchange */
void gasnete_coll_exchange_dcmf(gasnet_team_handle_t team, 
                                void *dst, void *src, 
                                size_t nbytes, int flags
                                GASNETE_THREAD_FARG)
{
  int i;
  int  nprocs = team->total_ranks;
  gasnete_dcmf_a2a_data_t * a2a;
  gasnete_coll_team_dcmf_t *dcmf_tp; 

#ifdef G_DCMF_COLL_TRACE
  fprintf(stderr, "gasnete_coll_exchange_dcmf is executed!\n");
#endif

  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  gasneti_assert(dcmf_tp != NULL);

  if (!gasnete_use_dcmf_coll 
      || !gasnete_use_dcmf_a2a  
      || dcmf_tp->a2a_proto == NULL) 
  {
      gasnet_coll_handle_t handle;
      handle = gasnete_coll_exchange_nb_default(team,dst,src,nbytes,flags,0 GASNETE_THREAD_PASS);
      gasnete_coll_wait_sync(handle GASNETE_THREAD_PASS);
      return;
  }

  gasneti_assert(gasnete_coll_dcmf_inited == 1);
  gasneti_assert(!(flags & GASNETE_COLL_SUBORDINATE));
  
  a2a = (gasnete_dcmf_a2a_data_t *)
    gasneti_malloc_aligned(64, sizeof(gasnete_dcmf_a2a_data_t));
  gasnete_coll_dcmf_a2a_init(a2a, dcmf_tp->a2a_proto,
                             &dcmf_tp->geometry, nprocs, src, dst, nbytes);
  
  GASNETC_DCMF_LOCK();
  DCMF_SAFE(DCMF_Alltoallv(a2a->registration,
                           &a2a->request, 
                           a2a->cb_done,
                           a2a->consistency,
                           a2a->geometry, 
                           a2a->sndbuf,
                           a2a->sndlens, 
                           a2a->sdispls, 
                           a2a->rcvbuf,
                           a2a->rcvlens, 
                           a2a->rdispls,
                           a2a->sndcounters, 
                           a2a->rcvcounters));
  GASNETC_DCMF_UNLOCK();
  gasneti_polluntil(a2a->active == 0);
  /* Fix for Bug 2786. Because DCMF_Alltoallv() doesn't copy the data
     from the send buffer to the receive buffer when the send node is
     the same as the receive node.  Therefore, the local data copy is
     performed here in GASNet. */ 
  {
      char *dst, *src;
      size_t nbytes;
      
      dst = (char *)a2a->rcvbuf+a2a->rdispls[team->myrank];
      src = (char *)a2a->sndbuf+a2a->sdispls[team->myrank];
      nbytes = a2a->rcvlens[team->myrank];
      GASNETE_FAST_UNALIGNED_MEMCPY(dst, src, nbytes);
  }
  
  /* optional barrier for the out_allsync mode, should be a team
     barrier */
  if (flags & GASNET_COLL_OUT_ALLSYNC)
    gasnete_coll_teambarrier_dcmf(team);
  
  /* clean up storage space */
  gasnete_coll_dcmf_a2a_fini(a2a);
  gasneti_free_aligned(a2a);
}

void  gasnete_coll_dcmf_a2a_print(FILE *fp, gasnete_dcmf_a2a_data_t * a2a, 
                                  int nprocs)
{
  fprintf(fp, "active flag %d\n", a2a->active);
  fprintf(fp, "registration %p\n", a2a->registration);
  fprintf(fp, "consistency %d\n", a2a->consistency);
  fprintf(fp, "geometry %p\n", a2a->geometry);
  fprintf(fp, "sndbuf %p\n", a2a->sndbuf);
  PRINT_ARRAY(fp, a2a->sndlens, nprocs, "%u");
  PRINT_ARRAY(fp, a2a->sdispls, nprocs, "%u");
  fprintf(fp, "rcvbuf %p\n",a2a->rcvbuf);
  PRINT_ARRAY(fp, a2a->rcvlens, nprocs, "%u");
  PRINT_ARRAY(fp, a2a->rdispls, nprocs, "%u");
  PRINT_ARRAY(fp, a2a->sndcounters, nprocs, "%u");
  PRINT_ARRAY(fp, a2a->rcvcounters, nprocs, "%u");
}
