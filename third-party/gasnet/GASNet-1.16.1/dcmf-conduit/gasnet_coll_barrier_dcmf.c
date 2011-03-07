/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_coll_barrier_dcmf.c,v $
 *     $Date: 2009/10/28 03:01:34 $
 * $Revision: 1.6 $
 * Description: GASNet barrier implementation on DCMF
 * Copyright 2009, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

#include <gasnet_extended_coll_dcmf.h>

/* barrier protocol registration data */
static DCMF_CollectiveProtocol_t gasnete_dcmf_barrier_protos[G_DCMF_BARRIER_PROTO_NUM];
static int gasnete_dcmf_barrier_enabled[G_DCMF_BARRIER_PROTO_NUM];

DCMF_CollectiveProtocol_t *gasnete_dcmf_barriers[G_DCMF_BARRIER_PROTO_NUM];
int gasnete_dcmf_num_barriers; 

DCMF_CollectiveProtocol_t *gasnete_dcmf_localbarriers[G_DCMF_BARRIER_PROTO_NUM];
int gasnete_dcmf_num_localbarriers;

/* callback function for DCMF_Barrier() */
static void gasnete_dcmf_coll_cb_done(void *clientdata, DCMF_Error_t *e)
{
  int *p = (int *)clientdata;
  *p = 0;
}

void gasnete_coll_barrier_proto_register(void)
{
  DCMF_Result rv;
  DCMF_Barrier_Configuration_t barrier_conf;
  char *tmp_str;

  GASNETC_DCMF_LOCK(); /* for DCMF_SAFE */

  gasnete_dcmf_num_barriers = 0;
  gasnete_dcmf_num_localbarriers = 0;

  /* global interrupt barrier */
  gasnete_dcmf_barrier_enabled[GI_BARRIER] = 
    gasneti_getenv_yesno_withdefault("GASNET_DCMF_GI_BARRIER", 1);
  if (gasnete_dcmf_barrier_enabled[GI_BARRIER])
    {
      barrier_conf.protocol = DCMF_GI_BARRIER_PROTOCOL;
      barrier_conf.cb_geometry = gasnete_dcmf_get_geometry;
      DCMF_SAFE(DCMF_Barrier_register(&gasnete_dcmf_barrier_protos[GI_BARRIER], 
                                      &barrier_conf));
      gasnete_dcmf_barriers[gasnete_dcmf_num_barriers] = &gasnete_dcmf_barrier_protos[GI_BARRIER];
      gasnete_dcmf_num_barriers++;
    }

  /* torus rectangle barrier */
  gasnete_dcmf_barrier_enabled[TORUS_RECTANGLE_BARRIER] = 
    gasneti_getenv_yesno_withdefault("GASNET_DCMF_TORUS_RECTANGLE_BARRIER", 1);
  if (gasnete_dcmf_barrier_enabled[TORUS_RECTANGLE_BARRIER])
    {
      barrier_conf.protocol = DCMF_TORUS_RECTANGLE_BARRIER_PROTOCOL;
      barrier_conf.cb_geometry = gasnete_dcmf_get_geometry;
      DCMF_SAFE(DCMF_Barrier_register(&gasnete_dcmf_barrier_protos[TORUS_RECTANGLE_BARRIER], 
                                      &barrier_conf));
      gasnete_dcmf_barriers[gasnete_dcmf_num_barriers] = 
        &gasnete_dcmf_barrier_protos[TORUS_RECTANGLE_BARRIER];
      gasnete_dcmf_num_barriers++;
    }

  /* torus rectangle lockbox barrier */
  gasnete_dcmf_barrier_enabled[TORUS_RECTANGLE_LB_BARRIER] = 
    gasneti_getenv_yesno_withdefault("GASNET_DCMF_TORUS_RECTANGLE_LB_BARRIER", 1);
  if (gasnete_dcmf_barrier_enabled[TORUS_RECTANGLE_LB_BARRIER])
    {
      barrier_conf.protocol = DCMF_TORUS_RECTANGLELOCKBOX_BARRIER_PROTOCOL_SINGLETH;
      barrier_conf.cb_geometry = gasnete_dcmf_get_geometry;
      DCMF_SAFE(DCMF_Barrier_register(&gasnete_dcmf_barrier_protos[TORUS_RECTANGLE_LB_BARRIER], 
                                      &barrier_conf));
      gasnete_dcmf_barriers[gasnete_dcmf_num_barriers] = 
        &gasnete_dcmf_barrier_protos[TORUS_RECTANGLE_LB_BARRIER];
      gasnete_dcmf_num_barriers++;
    }

  /* torus binomial barrier, which must be enabled if DCMF collectives are used */
  gasnete_dcmf_barrier_enabled[TORUS_BINOMIAL_BARRIER] = 1;
  barrier_conf.protocol = DCMF_TORUS_BINOMIAL_BARRIER_PROTOCOL;
  barrier_conf.cb_geometry = gasnete_dcmf_get_geometry;
  DCMF_SAFE(DCMF_Barrier_register(&gasnete_dcmf_barrier_protos[TORUS_BINOMIAL_BARRIER], 
                                  &barrier_conf));
  gasnete_dcmf_barriers[gasnete_dcmf_num_barriers] = 
    &gasnete_dcmf_barrier_protos[TORUS_BINOMIAL_BARRIER];
  gasnete_dcmf_num_barriers++;

  
  /* lockbox barrier, which must be enabled if DCMF collectives are used */
  gasnete_dcmf_barrier_enabled[LOCKBOX_BARRIER] = 1;
  barrier_conf.protocol = DCMF_LOCKBOX_BARRIER_PROTOCOL;
  DCMF_SAFE(DCMF_Barrier_register(&gasnete_dcmf_barrier_protos[LOCKBOX_BARRIER], 
                                  &barrier_conf));
  gasnete_dcmf_localbarriers[gasnete_dcmf_num_localbarriers] = 
    &gasnete_dcmf_barrier_protos[LOCKBOX_BARRIER];
  gasnete_dcmf_num_localbarriers++;

  gasneti_assert(gasnete_dcmf_num_barriers > 0);
  gasneti_assert(gasnete_dcmf_num_localbarriers > 0);

  GASNETC_DCMF_UNLOCK();
}

void gasnete_coll_teambarrier_dcmf(gasnet_team_handle_t team)
{
  gasnete_coll_teambarrier_notify_dcmf(team);
  gasnete_coll_teambarrier_wait_dcmf(team);
}

void gasnete_coll_teambarrier_notify_dcmf(gasnet_team_handle_t team)
{
  gasnete_coll_team_dcmf_t *dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  DCMF_Callback_t cb_done;
  
  gasneti_sync_reads();
  
  cb_done.function = gasnete_dcmf_coll_cb_done;
  cb_done.clientdata = (void *)&(dcmf_tp->in_barrier);

  GASNETC_DCMF_LOCK();
  gasneti_assert(gasnete_dcmf_busy == 0);
  gasnete_dcmf_busy = 1;  
  dcmf_tp->in_barrier = 1;
  DCMF_SAFE(DCMF_Barrier(&dcmf_tp->geometry, cb_done, DCMF_MATCH_CONSISTENCY));
  GASNETC_DCMF_UNLOCK();
  
  gasneti_sync_writes();
}

void gasnete_coll_teambarrier_wait_dcmf(gasnet_team_handle_t team)
{
  gasnete_coll_team_dcmf_t *dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  
  gasneti_sync_reads();
  
  GASNETC_DCMF_LOCK();
  while (dcmf_tp->in_barrier)
    DCMF_Messager_advance();
  gasnete_dcmf_busy = 0;  
  GASNETC_DCMF_UNLOCK();
  
  gasneti_sync_writes();
}

int gasnete_coll_teambarrier_try_dcmf(gasnet_team_handle_t team)
{
  gasnete_coll_team_dcmf_t *dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
 
  GASNETC_DCMF_LOCK();
  DCMF_Messager_advance();
  GASNETC_DCMF_UNLOCK();
  
  if (!dcmf_tp->in_barrier)
      gasnete_dcmf_busy = 0;

  return (dcmf_tp->in_barrier);
}
