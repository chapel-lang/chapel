/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_extended_coll_dcmf.c,v $
 *     $Date: 2010/05/10 23:15:37 $
 * $Revision: 1.6.2.1 $
 * Description: GASNet extended collectives implementation on DCMF
 * Copyright 2009, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

#include <gasnet_extended_coll_dcmf.h>
#include <gasnet_coll_exchange_dcmf.h>
#include <gasnet_coll_bcast_dcmf.h>
#include <gasnet_coll_barrier_dcmf.h>
#include <gasnet_coll_allreduce_dcmf.h>

int gasnete_coll_dcmf_inited = 0;
int gasnete_use_dcmf_coll;
volatile int gasnete_dcmf_busy = 0; /* only 1 DCMF collective op can be in flight, need to be locked properly for multi-threaded collectives */

DCMF_Geometry_t * gasnete_dcmf_get_geometry(int team_id)
{
  gasnet_team_handle_t team = gasnete_coll_team_lookup(team_id);
  gasnete_coll_team_dcmf_t *dcmf_tp;
  gasneti_assert(team != NULL);
  dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
#ifdef G_DCMF_TRACE
  fprintf(stderr, "gasnete_dcmf_get_geometry: team_id %u, geometry %p \n", 
          &dcmf_tp->geometry);
#endif
  return &dcmf_tp->geometry;
}
  
void gasnete_coll_init_dcmf()
{
  static gasneti_mutex_t init_lock = GASNETI_MUTEX_INITIALIZER;
  
#ifdef G_DCMF_TRACE
  fprintf(stderr, "gasnete_coll_init_dcmf is executed!\n");
#endif
  
  gasneti_mutex_lock(&init_lock);
  if(gasnete_coll_dcmf_inited) {
    gasneti_mutex_unlock(&init_lock);
    return;
  }
  
  gasnete_use_dcmf_coll =
    gasneti_getenv_yesno_withdefault("GASNET_USE_DCMF_COLL", 1);

  DCMF_Collective_initialize();
  
  gasnete_coll_barrier_proto_register();
  gasnete_coll_bcast_proto_register();
  gasnete_coll_a2a_proto_register();
  gasnete_coll_allreduce_proto_register();
  
  gasnete_coll_dcmf_inited = 1;
  gasneti_mutex_unlock(&init_lock);
}

void gasnete_coll_fini_dcmf(void)
{
#ifdef G_DCMF_TRACE
  fprintf(stderr, "gasnete_coll_fini_dcmf is executed!\n");
#endif
  gasneti_assert(gasnete_coll_dcmf_inited == 1);
}

static gasnete_coll_team_dcmf_t * gasnete_coll_team_dcmf_new(void)
{
  void *p;

#ifdef G_DCMF_TRACE
  fprintf(stderr, "gasnete_coll_team_dcmf_new is executed!\n");
#endif 
  p = gasneti_malloc_aligned(64, sizeof(gasnete_coll_team_dcmf_t));
  return (gasnete_coll_team_dcmf_t *)p;
}

static void gasnete_coll_team_dcmf_delete(gasnete_coll_team_dcmf_t * dcmf_tp)
{
#ifdef G_DCMF_TRACE
  fprintf(stderr, "gasnete_coll_team_dcmf_delete is executed!\n");
#endif 
  gasneti_free_aligned(dcmf_tp);
}

void gasnete_dcmf_team_init(gasnet_team_handle_t team,
                            DCMF_CollectiveProtocol_t ** bar_protos,
                            int num_barriers,
                            DCMF_CollectiveProtocol_t ** lbar_protos, 
                            int num_localbarriers)
{
  gasnete_coll_team_dcmf_t *dcmf_tp;
  
#ifdef G_DCMF_TRACE
  fprintf(stderr, "gasnete_coll_team_init_dcmf is executed!\n");
#endif

  gasneti_assert(team != NULL);
  
  /* return if it has already been initialized */
  if (team->dcmf_tp != NULL)
    return;
  
  if (gasnete_coll_dcmf_inited == 0)
    gasnete_coll_init_dcmf();
  
  /* initialize the base gasnet team object */
  /* gasnete_coll_team_init_default(team); */
  
  dcmf_tp = gasnete_coll_team_dcmf_new();
  team->dcmf_tp = dcmf_tp;

#ifdef G_DCMF_TRACE
  if (team->myrank == 0)
    {
      fprintf(stderr, "team->team_id %u\n", team->team_id);
      fprintf(stderr, "team->myrank %u\n", team->myrank);
      fprintf(stderr, "team->total_ranks %u\n", team->total_ranks);
      {
        int i;
        fprintf(stderr, "team->rel2act_map: ");
        for (i=0;i<team->total_ranks;i++)
          fprintf(stderr, "%u ", team->rel2act_map[i]);
        fprintf(stderr, "\n");
      }
      fprintf(stderr, "&dcmf_tp->geometry %x\n", &dcmf_tp->geometry);
      fprintf(stderr, "&dcmf_tp->barrier_req %x\n", &dcmf_tp->barrier_req);
    }
#endif
  
  /* Initialize the dcmf-specific data members in the team object */
  GASNETC_DCMF_LOCK();
  DCMF_SAFE(DCMF_Geometry_initialize (&dcmf_tp->geometry,
                                      team->team_id, 
                                      team->rel2act_map, 
                                      team->total_ranks,
                                      bar_protos,
                                      num_barriers,
                                      lbar_protos,
                                      num_localbarriers,
                                      &dcmf_tp->barrier_req, 
                                      gasnete_dcmf_bcast_num_colors,
                                      (team == GASNET_TEAM_ALL))); 
  GASNETC_DCMF_UNLOCK();

  /* set up collective protocols for the team */
  gasnete_coll_bcast_set_default_proto(team);
  gasnete_coll_a2a_set_default_proto(team);
  gasnete_coll_allreduce_set_default_proto(team);
  gasnete_coll_namedbarrier_set_default_proto(team);
}

/* DCMF conduit-specific team initialization, called outside the DCMF
   conduit */
void gasnete_coll_team_init_dcmf(gasnet_team_handle_t team)
{
  gasnete_dcmf_team_init(team, gasnete_dcmf_barriers, gasnete_dcmf_num_barriers,
                         gasnete_dcmf_localbarriers, gasnete_dcmf_num_localbarriers);
}

/* DCMF conduit-specific team finalization, called outside the DCMF
   conduit */
void gasnete_coll_team_fini_dcmf(gasnet_team_handle_t  team)
{
  gasnete_coll_team_dcmf_t *dcmf_tp = (gasnete_coll_team_dcmf_t *)team->dcmf_tp;
  
#ifdef G_DCMF_TRACE
  fprintf(stderr, "gasnete_coll_team_fini_dcmf is executed!\n");
#endif
  
  /* clean up DCMF_Geometry */
  DCMF_Geometry_free(&dcmf_tp->geometry);
  gasnete_coll_team_dcmf_delete(dcmf_tp);
}
