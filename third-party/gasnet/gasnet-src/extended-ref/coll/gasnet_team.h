/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_team.h $
 * Description: GASNet team data structure
 * Copyright 2009, The Regents of the University of California
 * Terms of use are as specified in license.txt 
 */

#ifndef GASNET_TEAM_H_
#define GASNET_TEAM_H_

#include <coll/gasnet_coll_internal.h>
#include <coll/gasnet_hashtable.h>

#define TEAM_DIR_SIZE 1021
/* extern gasnete_hashtable_t *team_dir; */

#define TEAM_ROOT(team_id) (((team_id)>>12) & 0xfffff) /* high 20 bits for root node id */
#define TEAM_SEQ(team_id) ((team_id) & 0xfff) /* low 12 bits for team sequence */

void gasnete_coll_team_init(gasnet_team_handle_t team, 
                            uint32_t team_id, 
                            uint32_t total_ranks,
                            gex_Rank_t myrank,
                            gex_Rank_t *rel2act_map,
                            gasnet_seginfo_t *scratch_segs,
                            const gasnet_image_t images[] GASNETI_THREAD_FARG);


void gasnete_coll_team_fini(gasnet_team_handle_t team);

gasnet_team_handle_t gasnete_coll_team_create(uint32_t total_ranks,
                                              gex_Rank_t myrank,
                                              gex_Rank_t *rel2act_map, gasnet_seginfo_t* scratch_segs GASNETI_THREAD_FARG);


void gasnete_coll_team_free(gasnet_team_handle_t team);

gasnet_team_handle_t gasnete_coll_team_lookup(uint32_t team_id);

gasnet_team_handle_t gasnete_coll_team_split(gasnet_team_handle_t team,
                                             int mycolor,
                                             int myrelrank,
                                             void *clientdata
                                             GASNETI_THREAD_FARG);

void gasnete_print_team(gasnet_team_handle_t team, FILE *fp);

/* redefine gasnete_coll_team_lookup in gasnet_coll_team.c */
#define GASNETE_COLL_TEAMS_OVERRIDE 

gasnet_team_handle_t gasnete_coll_team_lookup(uint32_t team_id);

#define PRINT_ARRAY(fp, A, size, format)        \
  do {                                          \
    int i;                                      \
    for(i=0; i<(size); i++)                     \
      {                                         \
        fprintf((fp), "%s[%d]=", #A, i);        \
        fprintf((fp), format, (A)[i]);          \
        fprintf((fp), " ");                     \
      }                                         \
    fprintf((fp), "\n");                        \
  } while(0);

#endif /* GASNET_TEAM_H_ */
