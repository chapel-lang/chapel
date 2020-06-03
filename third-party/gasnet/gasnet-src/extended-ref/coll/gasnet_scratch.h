/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_scratch.h $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

/* in all the functions below i assume that the scratch space is no bigger than 4GB*/

#ifndef __GASNET_SCRATCH_H__
#define __GASNET_SCRATCH_H__ 1

#define GASNETE_COLL_SCRATCH_TREE_OP 0
#define GASNETE_COLL_SCRATCH_DISSEM_OP 1

// How many elements (of appropriate types) to inline in order to avoid dynamic allocations
#define GASNETE_COLL_NUM_INLINE_OUT_PEERS 8
#define GASNETE_COLL_NUM_INLINE_IN_PEERS  8

struct gasnete_coll_node_scratch_status_t_;
typedef struct gasnete_coll_node_scratch_status_t_ gasnete_coll_node_scratch_status_t;

/* down tree means we send to relative ranks that are higher than us*/
/* up tree means that we send to relative ranks that are lower than us*/
typedef enum {GASNETE_COLL_UP_TREE=0, GASNETE_COLL_DOWN_TREE} gasnete_coll_tree_dir_t;

typedef enum {GASNETE_COLL_DISSEM_OP=0, GASNETE_COLL_TREE_OP} gasnete_coll_op_type_t;

struct gasnete_coll_scratch_req_t_ {
  gasnete_coll_scratch_req_t *next;
  gasnete_coll_scratch_req_t **prev_p;

  gasnete_coll_op_t *op;

  gasnete_coll_tree_type_t tree_type;
  gex_Rank_t root;
  gasnete_coll_team_t team;
  /*notice that we don't need to keep track of the dissemination radix since we don't do anything withit */
  
  /* whether this is a tree op where peers are fixed from phase to phase*/
  gasnete_coll_op_type_t op_type;
  gasnete_coll_tree_dir_t tree_dir;
		
  /*this is the sum incoming space of all the peers sending to me*/
  /*for now, for non treeops this is the amount of data that everyone is requesting*/
  uintptr_t incoming_size;
  
  /*information for all the data for which i am the target*/
  /*for non tree ops these values not used*/
  int num_in_peers;
  gex_Rank_t *in_peers;
  
  
  /*information for all the data for which i am an initiator*/
  /*for non tree ops this information is not used*/
  int num_out_peers; 
  gex_Rank_t *out_peers;
  uintptr_t *out_sizes;
  
  // A short array of uintptr_t for out_sizes and scratchpos (thus *2)
  // TODO: this could be a flexible array member sized according to team size?
  uintptr_t inline_uintptr[GASNETE_COLL_NUM_INLINE_OUT_PEERS*2];
};

// Allocate and free scratch_requests
// TODO-EX:
// Storage is currently manged with a simple per-team freelist with no
// serialization for concurrency.  When multiple endpoints are added then either
// the calls need to be serialized by team->threads_mutex, or a lifo used here.
GASNETI_INLINE(gasnete_coll_scratch_alloc_req) GASNETI_MALLOC
gasnete_coll_scratch_req_t *gasnete_coll_scratch_alloc_req(gasnete_coll_team_t team)
{
  gasnete_coll_scratch_req_t *scratch_req = team->scratch_free_list;
  if_pf (! scratch_req) {
    scratch_req = gasneti_calloc(1,sizeof(gasnete_coll_scratch_req_t));
    scratch_req->team = team;
  } else {
    team->scratch_free_list = scratch_req->next;
    gasneti_assert(scratch_req->team == team);
  }
  return scratch_req;
}
GASNETI_INLINE(gasnete_coll_scratch_free_req)
void gasnete_coll_scratch_free_req(gasnete_coll_scratch_req_t *scratch_req)
{
  gasnete_coll_team_t team = scratch_req->team;
  scratch_req->next = team->scratch_free_list;
  team->scratch_free_list = scratch_req;
}

// Allocate and free (consecutive) space used for out_sizes and scratchpos
// For small sizes we use space within the scratch request itself
// The allocation interfaces are such that one could split the two apart
GASNETI_INLINE(gasnete_coll_scratch_alloc_out_sizes)
void gasnete_coll_scratch_alloc_out_sizes(gasnete_coll_scratch_req_t *req, size_t n)
{
  gasneti_assert(n == (req->op_type == GASNETE_COLL_DISSEM_OP ? 1 : req->num_out_peers));
  size_t count = n + req->num_out_peers;
  if (count > 2 * GASNETE_COLL_NUM_INLINE_OUT_PEERS) {
    req->out_sizes = gasneti_malloc(count * sizeof(uintptr_t));
  } else {
    req->out_sizes = req->inline_uintptr;
  }
}
GASNETI_INLINE(gasnete_coll_scratch_alloc_pos)
void gasnete_coll_scratch_alloc_pos(gasnete_coll_scratch_req_t *req)
{
  size_t num_out_sizes = (req->op_type == GASNETE_COLL_DISSEM_OP ? 1 : req->num_out_peers);
  gasnete_coll_op_t *op = req->op;
  op->scratchpos = req->out_sizes + num_out_sizes;
}
GASNETI_INLINE(gasnete_coll_scratch_free_inlines)
void gasnete_coll_scratch_free_inlines(gasnete_coll_scratch_req_t *req)
{
  size_t num_out_sizes = (req->op_type == GASNETE_COLL_DISSEM_OP ? 1 : req->num_out_peers);
  gasneti_assert(req->op->scratchpos == req->out_sizes + num_out_sizes);
  size_t n = req->num_out_peers + num_out_sizes;
  if (n > 2 * GASNETE_COLL_NUM_INLINE_OUT_PEERS) {
    gasneti_assert(req->out_sizes != req->inline_uintptr);
    gasneti_free(req->out_sizes);
  }
}

/* try to allocate scratch space*/
/* returns 1 on success or zero on failure*/
int8_t gasnete_coll_scratch_alloc_nb(gasnete_coll_op_t* op GASNETI_THREAD_FARG);


/* release the associated scratch space with this op*/
void gasnete_coll_free_scratch(gasnete_coll_op_t *op);

/* function calls for coll init*/
void gasnete_coll_alloc_new_scratch_status(gasnete_coll_team_t team);
void gasnete_coll_free_scratch_status(gasnete_coll_scratch_status_t *in GASNETI_THREAD_FARG);


#endif
