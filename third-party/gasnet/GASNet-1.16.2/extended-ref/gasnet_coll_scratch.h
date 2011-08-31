/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_scratch.h,v $
 *     $Date: 2009/10/22 20:14:56 $
 * $Revision: 1.3 $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

/* in all the functions below i assume that the scratch space is no bigger than 4GB*/

#ifndef __GASNET_COLL_SCRATCH_H__
#define __GASNET_COLL_SCRATCH_H__ 1

#define GASNETE_COLL_SCRATCH_TREE_OP 0
#define GASNETE_COLL_SCRATCH_DISSEM_OP 1


struct gasnete_coll_node_scratch_status_t_;
typedef struct gasnete_coll_node_scratch_status_t_ gasnete_coll_node_scratch_status_t;

/* down tree means we send to relative ranks that are higher than us*/
/* up tree means that we send to relative ranks that are lower than us*/
typedef enum {GASNETE_COLL_UP_TREE=0, GASNETE_COLL_DOWN_TREE} gasnete_coll_tree_dir_t;

typedef enum {GASNETE_COLL_DISSEM_OP=0, GASNETE_COLL_TREE_OP} gasnete_coll_op_type_t;

struct gasnete_coll_scratch_req_t_ {
  
  gasnete_coll_tree_type_t tree_type;
  gasnet_node_t root;
  gasnete_coll_team_t team;
  /*notice that we don't need to keep track of the dissemination radix since we don't do anything withit */
  
  /* whether this is a tree op where peers are fixed from phase to phase*/
  gasnete_coll_op_type_t op_type;
  gasnete_coll_tree_dir_t tree_dir;
		
  /*this is the sum incoming space of all the peers sending to me*/
  /*for now, for non treeops this is the amount of data that everyone is requesting*/
  uint64_t incoming_size; 
  
  /*information for all the data for which i am the target*/
  /*for non tree ops these values not used*/
  int num_in_peers;
  gasnet_node_t *in_peers;
  
  
  /*information for all the data for which i am an initiator*/
  /*for non tree ops this information is not used*/
  int num_out_peers; 
  gasnet_node_t *out_peers;
  uint64_t *out_sizes;
  
  
};

/* try to allocate scratch space*/
/* returns 1 on success or zero on failure*/
int8_t gasnete_coll_scratch_alloc_nb(gasnete_coll_op_t* op GASNETE_THREAD_FARG);


/* release the associated scratch space with this op*/
void gasnete_coll_free_scratch(gasnete_coll_op_t *op);

/* function calls for coll init*/
void gasnete_coll_alloc_new_scratch_status(gasnete_coll_team_t team);
void gasnete_coll_free_scratch_status(gasnete_coll_scratch_status_t *in GASNETE_THREAD_FARG);


#endif
