/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_scratch.c $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */


#include <coll/gasnet_scratch.h>
#define GASNETE_COLL_SCRATCH_DEBUG_PRINTS 0

struct gasnete_coll_scratch_config_t_;
typedef struct gasnete_coll_scratch_config_t_ gasnete_coll_scratch_config_t;


struct gasnete_coll_node_scratch_status_t_  {
  /*head and tail of the circular buffer that represents the active scratch space on a particular node*/
  uintptr_t head;
  
  /*since the tail is the only one that gets updated by the active message handlers it needs to be the atomic one*/
  gasneti_weakatomic_t reset_signal_sent;
  gasneti_weakatomic_val_t reset_signal_recv;
};

struct gasnete_coll_scratch_config_t_ {
  gasnete_coll_op_type_t op_type;
  gasnete_coll_tree_type_t tree_type;
  gex_Rank_t root;
  gasnete_coll_tree_dir_t tree_dir;
  
  gasnete_coll_scratch_req_t *op_list_head;
  gasnete_coll_scratch_req_t **op_list_tail_p;
  
  /* these should be ignored when the config is active*/
  gasnete_coll_scratch_config_t *next;
  gasnete_coll_scratch_config_t **prev_p;
  
  /* this should be ignored when the config is waiting*/
  /*nodes that will send to me*/
  int numpeers, maxpeers;
  gex_Rank_t *peers;
  
  // A short peers array
  // TODO: this could be a flexible array member sized according to team size?
  gex_Rank_t inline_peers[GASNETE_COLL_NUM_INLINE_IN_PEERS];
};


struct gasnete_coll_scratch_status_t_ {
  gasnete_coll_scratch_config_t active_config_and_ops;
  
  gasnete_coll_scratch_config_t *waiting_config_and_ops_head;
  gasnete_coll_scratch_config_t **waiting_config_and_ops_tail_p;
  
  gasnete_coll_node_scratch_status_t *node_status;
  
  uint8_t scratch_empty;
  uint8_t clear_signal_sent;
  gasnete_coll_team_t team;
};


void gasnete_coll_alloc_new_scratch_status(gasnete_coll_team_t team) {
  gasnete_coll_scratch_status_t *stat;
  int i;
  
  stat = (gasnete_coll_scratch_status_t*) gasneti_calloc(1,sizeof(gasnete_coll_scratch_status_t));
  stat->node_status = (gasnete_coll_node_scratch_status_t*)gasneti_malloc(sizeof(gasnete_coll_node_scratch_status_t)*team->total_ranks);
  
  stat->waiting_config_and_ops_tail_p = & stat->waiting_config_and_ops_head;
  stat->team = team;
  stat->scratch_empty = 1;
  stat->clear_signal_sent = 0;
  for(i=0; i<team->total_ranks; i++) {
    stat->node_status[i].head = 0;
    gasneti_weakatomic_set(&(stat->node_status[i].reset_signal_sent),0,0);
    stat->node_status[i].reset_signal_recv = 0;
  }

  // Initialize active_config_and_ops
  stat->active_config_and_ops.root = GEX_RANK_INVALID;
  stat->active_config_and_ops.op_list_tail_p = &stat->active_config_and_ops.op_list_head;
  stat->active_config_and_ops.peers = stat->active_config_and_ops.inline_peers;
  stat->active_config_and_ops.maxpeers = GASNETE_COLL_NUM_INLINE_IN_PEERS;
  
  team->scratch_status = stat;
}


void gasnete_coll_free_scratch_status(gasnete_coll_team_t team) {
  gasnete_coll_scratch_status_t *stat = team->scratch_status;
  gasneti_free(stat->node_status);
  gasneti_free(stat);
}


void gasnete_coll_scratch_send_updates(gasnete_coll_team_t team, int seq) {
  gasnete_coll_scratch_config_t *config = &team->scratch_status->active_config_and_ops;
  
  for (int i = 0; i < config->numpeers; i++) {
    gex_AM_RequestShort(team->e_tm, config->peers[i],
                                gasneti_handleridx(gasnete_coll_scratch_update_reqh), 0,
                                team->team_id, team->myrank);
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS
    fprintf(stderr, "%d,%d> CLEAR!->%d\n", seq, gasneti_mynode, config->peers[i]);
#endif
  }
 }


void gasnete_coll_scratch_update_reqh(gex_Token_t token,
				      gex_AM_Arg_t teamid,
				      gex_AM_Arg_t node) {
  gasnete_coll_team_t team;
  gasnete_coll_scratch_status_t *stat;
  
  team = gasnete_coll_team_lookup(teamid);
  stat = team->scratch_status;
  gasneti_assert(stat);
  gasneti_assert(stat->node_status);
  gasneti_weakatomic_increment(&(stat->node_status[node].reset_signal_sent),0);
}
/***************************/


GASNETI_INLINE(gasnete_coll_scratch_compare_config)
uint8_t gasnete_coll_scratch_compare_config(gasnete_coll_scratch_config_t *A,
                                            gasnete_coll_scratch_req_t *scratch_req) {
  gasneti_assert(A);
  if((A->root != scratch_req->root) || 
     !gasnete_coll_compare_tree_types(A->tree_type, scratch_req->tree_type) || 
     A->op_type != scratch_req->op_type ||
     A->tree_dir != scratch_req->tree_dir) return 0;
  else return 1;
}

// Init data fields, not linkage
GASNETI_INLINE(gasnete_coll_scratch_init_config)
void gasnete_coll_scratch_init_config(gasnete_coll_scratch_config_t *config, gasnete_coll_scratch_req_t *scratch_req)
{
  config->root = scratch_req->root;
  config->tree_type = scratch_req->tree_type;
  config->op_type = scratch_req->op_type;
  config->tree_dir = scratch_req->tree_dir;
}

// Protected by gasnete_coll_poll_lock
static gasnete_coll_scratch_config_t *gasnete_coll_scratch_config_free_list = NULL;

GASNETI_INLINE(gasnete_coll_scratch_allocate_new_config)
gasnete_coll_scratch_config_t * gasnete_coll_scratch_allocate_new_config(void)
{
  gasneti_mutex_assertlocked(&gasnete_coll_poll_lock);
  gasnete_coll_scratch_config_t *ret;
  ret = gasnete_coll_scratch_config_free_list;
  if (ret) {
    gasnete_coll_scratch_config_free_list = ret->next;
  } else {
    ret = gasneti_malloc(sizeof(gasnete_coll_scratch_config_t));
  }
  ret->numpeers = 0;
  ret->op_list_head = NULL;
  ret->op_list_tail_p = &ret->op_list_head;
  return ret;
}

GASNETI_INLINE(gasnete_coll_scratch_free_config)
void gasnete_coll_scratch_free_config(gasnete_coll_scratch_config_t *config)
{
  gasneti_mutex_assertlocked(&gasnete_coll_poll_lock);
  config->next = gasnete_coll_scratch_config_free_list;
  gasnete_coll_scratch_config_free_list = config;
}

/* add the op to the tail of the configuration*/
GASNETI_INLINE(gasnete_coll_scratch_add_op_to_config)
void gasnete_coll_scratch_add_op_to_config(gasnete_coll_scratch_config_t *config,
                                           gasnete_coll_scratch_req_t* req) {
  gasneti_assert(config->tree_dir == req->tree_dir);
  req->next = NULL;
  *config->op_list_tail_p = req;
  req->prev_p = config->op_list_tail_p;
  config->op_list_tail_p = &req->next;
}

/* remove the first op from the configuration and return it*/
GASNETI_INLINE(gasnete_coll_scratch_remove_first_op_from_config)
gasnete_coll_scratch_req_t* gasnete_coll_scratch_remove_first_op_from_config(gasnete_coll_scratch_config_t *config) {
  gasnete_coll_scratch_req_t *ret;

  gasneti_assert(config->op_list_head !=NULL);

  ret = config->op_list_head;
  *ret->prev_p = ret->next;
  if (ret->next) {
    ret->next->prev_p = ret->prev_p;
  } else {
    config->op_list_tail_p = ret->prev_p;
  }
  ret->prev_p = NULL;
  ret->next = NULL;
  return ret;
}

GASNETI_INLINE(gasnete_coll_scratch_add_to_wait)
void gasnete_coll_scratch_add_to_wait(gasnete_coll_scratch_req_t *scratch_req, gasnete_coll_op_t* op) {
  gasnete_coll_scratch_status_t *stat = scratch_req->team->scratch_status;
  gasnete_coll_scratch_config_t *temp;
    
  gasneti_assert(op->waiting_scratch_op == 0);
  op->waiting_scratch_op = 1;
  op->active_scratch_op = 0;

  temp = stat->waiting_config_and_ops_head
       ? gasneti_container_of(stat->waiting_config_and_ops_tail_p, gasnete_coll_scratch_config_t, next)
       : NULL;

  if (!temp || !gasnete_coll_scratch_compare_config(temp, scratch_req)) {
    /* attach to the end of the list*/
    temp =  gasnete_coll_scratch_allocate_new_config();
    gasnete_coll_scratch_init_config(temp, scratch_req);
    *stat->waiting_config_and_ops_tail_p = temp;
    temp->prev_p = stat->waiting_config_and_ops_tail_p;
    stat->waiting_config_and_ops_tail_p = &temp->next;
    temp->next = NULL;
  }
  gasnete_coll_scratch_add_op_to_config(temp, scratch_req);
}

GASNETI_INLINE(gasnete_coll_scratch_remove_first_waiting_op)
void gasnete_coll_scratch_remove_first_waiting_op(gasnete_coll_scratch_status_t *stat) {
  (void) gasnete_coll_scratch_remove_first_op_from_config(stat->waiting_config_and_ops_head);

  if (! stat->waiting_config_and_ops_head->op_list_head) {
    gasnete_coll_scratch_config_t *temp = stat->waiting_config_and_ops_head;
    stat->waiting_config_and_ops_head = temp->next;
    if (temp->next) {
      temp->next->prev_p = temp->prev_p;
    } else {
      stat->waiting_config_and_ops_tail_p = temp->prev_p;
    }
    gasnete_coll_scratch_free_config(temp);
  }
}

GASNETI_INLINE(gasnete_coll_scratch_reconfigure)
void gasnete_coll_scratch_reconfigure(gasnete_coll_scratch_status_t *stat, 
                                      gasnete_coll_scratch_req_t *req)
{
  gasnete_coll_scratch_config_t *config = &stat->active_config_and_ops;
  gasneti_assert(!config->op_list_head); // must be empty of ops
  if (! gasnete_coll_scratch_compare_config(config, req)) {
    /* set the new config and the information about who will send to me */
    config->op_type   = req->op_type;
    config->tree_type = req->tree_type;
    config->root      = req->root;
    config->tree_dir  = req->tree_dir;
  
    size_t n = req->num_in_peers;
    config->numpeers = n;
    if (n) {
      size_t space = n * sizeof(gex_Rank_t);
      if_pf (n > config->maxpeers) {
        if (config->peers != config->inline_peers) {
          gasneti_free(config->peers);
        }
        config->peers = gasneti_malloc(space);
        config->maxpeers = n;
      }
      GASNETI_MEMCPY(config->peers, req->in_peers, space);
    }
  }
}

GASNETI_INLINE(gasnete_coll_scratch_check_local_alloc)
uint8_t gasnete_coll_scratch_check_local_alloc(gasnete_coll_scratch_req_t *req,
                                               gasnete_coll_scratch_status_t *stat) {
  return (req->incoming_size + stat->node_status[req->team->myrank].head <= req->team->scratch_size);
}

GASNETI_INLINE(gasnete_coll_scratch_make_local_alloc)
uintptr_t gasnete_coll_scratch_make_local_alloc(gasnete_coll_scratch_req_t *req,
                                               gasnete_coll_scratch_status_t *stat) {
  uintptr_t ret;
  ret = stat->node_status[req->team->myrank].head;
  stat->node_status[req->team->myrank].head += req->incoming_size;
  return ret;
}

GASNETI_INLINE(gasnete_coll_scratch_check_remote_clear)
uint8_t gasnete_coll_scratch_check_remote_clear(gasnete_coll_scratch_req_t *req,
                                                gasnete_coll_scratch_status_t *stat)
{
  const gex_Rank_t n = req->num_out_peers;
  for (gex_Rank_t i = 0; i < n; i++) {
    const gex_Rank_t peer = req->out_peers[i];
    if (gasneti_weakatomic_read(&(stat->node_status[peer].reset_signal_sent),0) ==
        stat->node_status[peer].reset_signal_recv) {
      return 0;
    }
  }
  /* reset all the signals once we get all of them */
  for (gex_Rank_t i = 0; i < n; i++) {
    const gex_Rank_t peer = req->out_peers[i];
    stat->node_status[peer].reset_signal_recv += 1;
    stat->node_status[peer].reset_signal_recv &= GASNETI_ATOMIC_MAX;
    stat->node_status[peer].head = 0;
  }
  return 1;
}

GASNETI_INLINE(gasnete_coll_scratch_check_remote_alloc)
uint8_t gasnete_coll_scratch_check_remote_alloc(gasnete_coll_scratch_req_t *req,
                                                gasnete_coll_scratch_status_t *stat) {
  const gex_Rank_t n = req->num_out_peers;
  const int is_dissem = (req->op_type == GASNETE_COLL_DISSEM_OP);
  for (gex_Rank_t i = 0; i < n; i++) {
    const gex_Rank_t peer = req->out_peers[i];
    if (stat->node_status[peer].head + req->out_sizes[is_dissem ? 0 : i] > req->team->scratch_size) {
      /* remote space is full */
      if (gasneti_weakatomic_read(&(stat->node_status[peer].reset_signal_sent),0) ==
          stat->node_status[peer].reset_signal_recv) {
        return 0;
      } else {
        stat->node_status[peer].head = 0;
        stat->node_status[peer].reset_signal_recv += 1;
        stat->node_status[peer].reset_signal_recv &= GASNETI_ATOMIC_MAX;
      }
    }
  }
  return 1;
}

GASNETI_INLINE(gasnete_coll_scratch_make_remote_alloc)
void gasnete_coll_scratch_make_remote_alloc(gasnete_coll_scratch_req_t *req,
                                            gasnete_coll_scratch_status_t *stat,
                                            uintptr_t *rem_pos) {
  const int is_dissem = (req->op_type == GASNETE_COLL_DISSEM_OP);
  for (gex_Rank_t i=0; i<req->num_out_peers; i++) {
    const gex_Rank_t peer = req->out_peers[i];
    rem_pos[i] = stat->node_status[peer].head;
    stat->node_status[peer].head += req->out_sizes[is_dissem ? 0 : i];
  }  
}

int8_t gasnete_coll_scratch_alloc_nb(gasnete_coll_op_t* op GASNETI_THREAD_FARG) {
  gasnete_coll_scratch_req_t *scratch_req = op->scratch_req;
  gasnete_coll_scratch_status_t *stat = scratch_req->team->scratch_status;

  gasneti_assert(scratch_req);
  gasneti_assert(stat);
  gasneti_assert(op->waiting_scratch_op == 0 || op->waiting_scratch_op == 1);
  
  /*if the incoming size is greater than the total allocated scratch space signal an error*/
  if_pf (!op->waiting_scratch_op &&
         scratch_req->incoming_size > scratch_req->team->scratch_size) {
    gasneti_fatalerror("%d> collective requires temporary storage (%"PRIuPTR" bytes) which is greater than total scratch space (%"PRIuPTR" bytes)\nIncrease size of collective scratch space through GASNET_COLL_SCRATCH_SIZE environment variable to at least %"PRIuPTR" bytes\n", 
                       (int)scratch_req->team->myrank, 
                       (uintptr_t)scratch_req->incoming_size,
                       (uintptr_t)scratch_req->team->scratch_size,
                       (uintptr_t)scratch_req->incoming_size);
  }

  if(op->waiting_scratch_op) {
    /* this operation is already on the wait queue */
    /* check if this op is at the head of the wait queue 
    (do not modify the wait queue until we know the op can be activated)*/
    gasneti_assert(stat->waiting_config_and_ops_head);
    gasneti_assert(stat->waiting_config_and_ops_head->op_list_head);

    if(stat->waiting_config_and_ops_head->op_list_head->op != op) {
      /* operation is not at the head of the wait queue so return an allocate fail*/
      return 0;
    }   
  } else if (stat->waiting_config_and_ops_head) {
    /* i go at the end of the wait queue*/
    gasnete_coll_scratch_add_to_wait(scratch_req, op);
    
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS
    fprintf(stderr, "%d,%d> op added to wait (wait queue non empty)\n",  op->sequence, gasneti_mynode); 
#endif
    return 0;
  } 
  
  /* if we get here then op is a new op w/ no ops waiting or it is the first on the wait queue */
 /* fprintf(stderr, "%d,%d> polling scratch\n", op->sequence, gasneti_mynode);*/
  if(op->waiting_for_reconfig_clear || 
     (!gasnete_coll_scratch_compare_config(&stat->active_config_and_ops, scratch_req) &&
      !stat->active_config_and_ops.op_list_head)) {
    /* empty scratch space is misconfigured*/

    if(!op->waiting_for_reconfig_clear) {
      gasnete_coll_scratch_reconfigure(stat, scratch_req);
    }
    
    if(stat->clear_signal_sent==0) {
      stat->node_status[scratch_req->team->myrank].head = 0; 
      /* send a clear signal to the new peers*/
      gasnete_coll_scratch_send_updates(scratch_req->team, op->sequence);
      stat->clear_signal_sent=1; /* make sure clear signal is sent only once per drain cycle*/
    }

    op->waiting_for_reconfig_clear=1;

    if(gasnete_coll_scratch_check_remote_clear(scratch_req, stat)) {
      stat->scratch_empty = 0;
      stat->clear_signal_sent = 0;
      
      op->active_scratch_op = 1;
      /* move it from the waiting to the active list*/
      
      if(op->waiting_scratch_op) {
        gasneti_assert(stat->waiting_config_and_ops_head->op_list_head == scratch_req);
        gasnete_coll_scratch_remove_first_waiting_op(stat);
        op->waiting_scratch_op = 0;
      }
      gasnete_coll_scratch_add_op_to_config(&stat->active_config_and_ops, scratch_req);
      /* return the appropriate amount of local/remote scratch space*/
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS
       fprintf(stderr, "%d> allocating for op %d\n", gasneti_mynode, op->sequence); 
#endif
      op->myscratchpos = gasnete_coll_scratch_make_local_alloc(scratch_req, stat);
      gasnete_coll_scratch_alloc_pos(scratch_req);
      gasnete_coll_scratch_make_remote_alloc(scratch_req, stat, op->scratchpos);
      return 1;
    } else { 
      /* local or remote allocation failed*/
      if(!op->waiting_scratch_op) {
        gasnete_coll_scratch_add_to_wait(scratch_req, op);
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS        
	fprintf(stderr, "%d,%d> op added to wait (remote alloc fail on reconfig)\n", op->sequence, gasneti_mynode); 
#endif
      }
      return 0;
    }
  } else if(gasnete_coll_scratch_compare_config(&stat->active_config_and_ops, scratch_req)) {
    /*fprintf(stderr, "%d,%d> polling scratch --> correct config check for space\n", op->sequence, gasneti_mynode);*/
    /* configuration matches the current configuration ... check if we can allocate*/
    if(!gasnete_coll_scratch_check_local_alloc(scratch_req, stat)) { 
      if (!stat->active_config_and_ops.op_list_head) {
        /*can't make local allocation but old ops have cleared out*/
        /* send clear signal and reset scratch*/
        if(stat->clear_signal_sent==0) {
          stat->node_status[scratch_req->team->myrank].head = 0; 
          /* send a clear signal to the new peers*/
          gasnete_coll_scratch_send_updates(scratch_req->team, op->sequence);
          stat->clear_signal_sent=1; /* make sure clear signal is sent only once per drain cycle*/
        }
      } else {
        /* can't make local allocation and old ops have not cleared out*/
        if(!op->waiting_scratch_op) {
          gasnete_coll_scratch_add_to_wait(scratch_req, op);
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS          
	  fprintf(stderr, "%d,%d> op added to wait (local full)\n", op->sequence, gasneti_mynode); 
#endif
        }
        return 0;
      }
    }
       
    /* local allocation succeeded*/
    if(gasnete_coll_scratch_check_remote_alloc(scratch_req, stat)) {
      op->active_scratch_op = 1;
      stat->scratch_empty = 0;
      stat->clear_signal_sent=0;

      /* remove the operation from the wait queue and add it to the active queue if it was waiting*/
      /* allocate op and return*/
 
      if(op->waiting_scratch_op) {
        gasneti_assert(stat->waiting_config_and_ops_head->op_list_head == scratch_req);
        gasnete_coll_scratch_remove_first_waiting_op(stat);
        op->waiting_scratch_op = 0;
      }
      gasnete_coll_scratch_add_op_to_config(&stat->active_config_and_ops, scratch_req);
      /* return the appropriate amount of local/remote scratch space*/
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS      
      fprintf(stderr, "%d> allocating for op %d\n", gasneti_mynode, op->sequence); 
#endif
      op->myscratchpos = gasnete_coll_scratch_make_local_alloc(scratch_req, stat);
      gasnete_coll_scratch_alloc_pos(scratch_req);
      gasnete_coll_scratch_make_remote_alloc(scratch_req, stat, op->scratchpos);
      return 1;
    } else {
      /*remote scratch space is full add it to the wait queue and try again later*/
      if(!op->waiting_scratch_op) {
        gasnete_coll_scratch_add_to_wait(scratch_req, op);
        /* fprintf(stderr, "%d,%d> op added to wait (remote alloc fail no reconfig)\n",  op->sequence, gasneti_mynode); */
      }
      return 0;
    }
  } else {
    /* configuration mismatch with outstanding active ops... put this op in the wait queues*/
    if(!op->waiting_scratch_op) {
      gasnete_coll_scratch_add_to_wait(scratch_req, op);
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS
       fprintf(stderr, "%d,%d> op added to wait (config mismatch w/ active ops)\n", op->sequence,  gasneti_mynode); 
#endif
    }
    return 0;
  }
}

  
void gasnete_coll_free_scratch(gasnete_coll_op_t *op) {
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS 
  fprintf(stderr, "%d,%d> finishing op\n", op->sequence, gasneti_mynode);
#endif

  /* remove op from the active scratch op list */
  gasnete_coll_scratch_req_t *scratch_req = op->scratch_req;
  *scratch_req->prev_p = scratch_req->next;
  if (scratch_req->next) {
    scratch_req->next->prev_p = scratch_req->prev_p;
  } else {
    scratch_req->team->scratch_status->active_config_and_ops.op_list_tail_p = scratch_req->prev_p;
  }

#if GASNET_DEBUG
  gasneti_assert(op->scratch_op_freed==0);
  op->scratch_op_freed = 1;
  op->scratch_req = NULL;
#endif

  gasnete_coll_scratch_free_inlines(scratch_req);
  gasnete_coll_scratch_free_req(scratch_req);
}

void gasnete_coll_scratch_req_purge(gasnete_coll_team_t team) {
  gasnete_coll_scratch_req_t *req = team->scratch_free_list;
  while (req) {
    gasnete_coll_scratch_req_t *next = req->next;
    gasneti_free(req);
    req = next;
  }
}
