/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_team.h,v $ 
 *     $Date: 2009/10/28 04:17:13 $ 
 * $Revision: 1.3 $ 
 * Description: GASNet team data structure
 * Copyright 2009, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt 
 */

#ifndef GASNET_COLL_TEAM_H_
#define GASNET_COLL_TEAM_H_

#include <gasnet_coll_internal.h>
#include <gasnet_coll_hashtable.h>

#define TEAM_DIR_SIZE 1021
/* extern gasnete_hashtable_t *team_dir; */

#define TEAM_ROOT(team_id) (((team_id)>>12) & 0xfffff) /* high 20 bits for root node id */
#define TEAM_SEQ(team_id) ((team_id) & 0xfff) /* low 12 bits for team sequence */

void gasnete_coll_team_init(gasnet_team_handle_t team, 
                            uint32_t team_id, 
                            uint32_t total_ranks,
                            gasnet_node_t myrank,
                            gasnet_node_t *rel2act_map,
                            gasnet_seginfo_t *scratch_segs,
                            const gasnet_image_t images[] GASNETE_THREAD_FARG);


void gasnete_coll_team_fini(gasnet_team_handle_t team);

gasnet_team_handle_t gasnete_coll_team_create(uint32_t total_ranks,
                                              gasnet_node_t myrank,
                                              gasnet_node_t *rel2act_map, gasnet_seginfo_t* scratch_segs GASNETE_THREAD_FARG);


void gasnete_coll_team_free(gasnet_team_handle_t team);

gasnet_team_handle_t gasnete_coll_team_lookup(uint32_t team_id);

gasnet_team_handle_t gasnete_coll_team_split(gasnet_team_handle_t team,
                                             gasnet_node_t mycolor,
                                             gasnet_node_t myrelrank,
                                             void *clientdata
                                             GASNETE_THREAD_FARG);

void gasnete_print_team(gasnet_team_handle_t team, FILE *fp);

/* redefine gasnete_coll_team_lookup in gasnet_coll_team.c */
#define GASNETE_COLL_TEAMS_OVERRIDE 

gasnet_team_handle_t gasnete_coll_team_lookup(uint32_t team_id);

gasnet_node_t gasnete_coll_team_rank2node(gasnete_coll_team_t team, int rank);

gasnet_node_t gasnete_coll_team_node2rank(gasnete_coll_team_t team, gasnet_node_t node);

uint32_t gasnete_coll_team_id(gasnete_coll_team_t team) ;

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

#endif /* GASNET_COLL_TEAM_H_ */
