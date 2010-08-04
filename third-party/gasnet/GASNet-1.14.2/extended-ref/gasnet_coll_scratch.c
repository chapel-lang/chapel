/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_coll_scratch.c,v $
 *     $Date: 2009/10/22 20:14:56 $
 * $Revision: 1.8 $
 * Description: Reference implemetation of GASNet Collectives team
 * Copyright 2009, Rajesh Nishtala <rajeshn@eecs.berkeley.edu>, Paul H. Hargrove <PHHargrove@lbl.gov>, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */


#include "gasnet_coll_scratch.h"
#define GASNETE_COLL_SCRATCH_DEBUG_PRINTS 0
struct gasnete_coll_op_info_t_;
typedef struct gasnete_coll_op_info_t_ gasnete_coll_op_info_t;

struct gasnete_coll_scratch_config_t_;
typedef struct gasnete_coll_scratch_config_t_ gasnete_coll_scratch_config_t;


typedef enum {GASNETE_COLL_SCRATCH_NO_WAIT=0, GASNETE_COLL_SCRATCH_BAD_CONFIG, 
  GASNETE_COLL_SCRATCH_FULL, GASNETE_COLL_SCRATCH_DRAIN_IN_PROGRESS} gasnete_coll_scratch_reason_t;


struct gasnete_coll_node_scratch_status_t_  {
  /*head and tail of the circular buffer that represents the active scratch space on a particular node*/
  uint64_t head;
  
  /*since the tail is the only one that gets updated by the active message handlers it needs to be the atomic one*/
  gasneti_weakatomic_t reset_signal_sent;
  gasneti_weakatomic_t reset_signal_recv;
};

struct gasnete_coll_op_info_t_ {
  
  gasnete_coll_op_info_t *next;
  gasnete_coll_op_info_t *prev;
  
  gasnete_coll_scratch_req_t *req; /* the associated request with this op*/
  
  gasnete_coll_tree_type_t tree_type;
  gasnet_node_t root;
  
  int tree_op;
  gasnete_coll_tree_dir_t tree_dir;
  
    gasnete_coll_op_t *op;
  /* a pointer to the actual op handle so that we can do a wait sync on it */
  gasnet_coll_handle_t op_handle; 
  
  /*amount of scratch space used locally*/
  uint32_t local_scratch_used;
  
  uint32_t seq_number;
  
  /*is this operation finished*/
  int done;
  
  gasnete_coll_scratch_reason_t wait_reason; /* what event caused the wait*/
};

struct gasnete_coll_scratch_config_t_ {
  gasnete_coll_op_type_t op_type;
  gasnete_coll_tree_type_t tree_type;
  gasnet_node_t root;
  gasnete_coll_tree_dir_t tree_dir;
  int dissem_radix;
  
  gasnete_coll_op_info_t *op_list_head;
  gasnete_coll_op_info_t *op_list_tail;
  int num_ops;
  
  /* these should be ignored when the config is active*/
  gasnete_coll_scratch_config_t *next;
  gasnete_coll_scratch_config_t *prev;
  
  /* this should be ignored when the config is waiting*/
  /*nodes that will send to me*/
  int numpeers;
  gasnet_node_t *peers;
  
};


/*this structure describes an operation info*/
struct gasnete_coll_scratch_status_t_ {
  /*creates an array of node statuses*/
  /* list of currently active peers*/
  
  gasnete_coll_scratch_config_t *active_config_and_ops;
  
  gasnete_coll_scratch_config_t *waiting_config_and_ops_head;
  gasnete_coll_scratch_config_t *waiting_config_and_ops_tail;
  uint32_t num_waiting_ops;
  
  gasnete_coll_node_scratch_status_t *node_status;
  
  /*an indicator telling you whether the upcoming collective op is the first after a barrier*/
  uint8_t scratch_empty;
  uint8_t clear_signal_sent;
  gasnete_coll_team_t team;

};


void gasnete_coll_alloc_new_scratch_status(gasnete_coll_team_t team) {
  gasnete_coll_scratch_status_t *stat;
  int i;
  
  stat = (gasnete_coll_scratch_status_t*) gasneti_calloc(1,sizeof(gasnete_coll_scratch_status_t));
  stat->node_status = (gasnete_coll_node_scratch_status_t*)gasneti_malloc(sizeof(gasnete_coll_node_scratch_status_t)*team->total_ranks);
  
  stat->active_config_and_ops = NULL;
  stat->waiting_config_and_ops_head = stat->waiting_config_and_ops_tail = NULL;
  stat->num_waiting_ops = 0;
  stat->team = team;
  stat->scratch_empty = 1;
  stat->clear_signal_sent = 0;
  for(i=0; i<team->total_ranks; i++) {
    stat->node_status[i].head = 0;
    gasneti_weakatomic_set(&(stat->node_status[i].reset_signal_sent),0,0);
    gasneti_weakatomic_set(&(stat->node_status[i].reset_signal_recv),0,0);
  }
  
  team->scratch_status = stat;

}


void gasnete_coll_free_scratch_status(gasnete_coll_scratch_status_t *in GASNETE_THREAD_FARG) {
 /* do nothing for now*/
}


void gasnete_coll_scratch_send_updates(gasnete_coll_team_t team, int seq) {
  int i;
  gasnete_coll_scratch_status_t *stat = team->scratch_status;
  
  /*Becareful with the teams here and how the peer list is specified*/
  /*for gasnet team all it doesn't matter but in other cases it does
  stat->active_config_and_ops->peers[i] needs to be translated to an absolute rank*/
  for(i=0; i<stat->active_config_and_ops->numpeers; i++) {
    GASNETI_SAFE(SHORT_REQ(2,2,(GASNETE_COLL_REL2ACT(team, stat->active_config_and_ops->peers[i]),
                                gasneti_handleridx(gasnete_coll_scratch_update_reqh),
                                team->team_id, team->myrank)));
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS
    fprintf(stderr, "%d,%d> CLEAR!->%d\n", seq, gasneti_mynode, stat->active_config_and_ops->peers[i]); 
#endif
    
  }
 }


void gasnete_coll_scratch_update_reqh(gasnet_token_t token,
				      gasnet_handlerarg_t teamid,
				      gasnet_handlerarg_t node) {
  gasnete_coll_team_t team;
  gasnete_coll_scratch_status_t *stat;
  
  team = gasnete_coll_team_lookup(teamid);
  stat = team->scratch_status;
  gasneti_assert(stat);
  gasneti_assert(stat->node_status);
  /* for now signal the new val as 1*/
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

GASNETI_INLINE(gasnete_coll_scratch_alloate_new_config)
gasnete_coll_scratch_config_t * gasnete_coll_scratch_alloate_new_config(gasnete_coll_scratch_req_t * scratch_req){
  gasnete_coll_scratch_config_t *ret;
  ret = gasneti_calloc(1, sizeof(gasnete_coll_scratch_config_t));
  ret->root = scratch_req->root;
  ret->tree_type = scratch_req->tree_type;
  ret->op_type = scratch_req->op_type;
  ret->tree_dir = scratch_req->tree_dir;
  return ret;
}
/* add the op to the tail of the configuration*/
GASNETI_INLINE(gasnete_coll_scratch_add_op_to_config)
void gasnete_coll_scratch_add_op_to_config(gasnete_coll_scratch_config_t *config,
                                           gasnete_coll_op_info_t* info) {
    gasneti_assert(config->tree_dir == info->tree_dir);
  if(config->op_list_head == NULL) {
    config->op_list_head = config->op_list_tail = info;
  } else {
    config->op_list_tail->next = info;
    info->prev = config->op_list_tail;
    info->next = NULL;
    config->op_list_tail = info;
  }
  config->num_ops++;
}

/* remove the first op from the configuration and return it*/
GASNETI_INLINE(gasnete_coll_scratch_remove_first_op_from_config)
gasnete_coll_op_info_t* gasnete_coll_scratch_remove_first_op_from_config(gasnete_coll_scratch_config_t *config) {
  gasnete_coll_op_info_t *ret;

  gasneti_assert(config->op_list_head !=NULL);
  gasneti_assert(config->num_ops > 0);
  if(config->num_ops == 1) {
    ret = config->op_list_head;
    config->op_list_head = config->op_list_tail = NULL;
  } else {
    ret = config->op_list_head;
    config->op_list_head = ret->next;
    config->op_list_head->prev = NULL;
  }
  ret->prev = NULL;
  ret->next = NULL;
  config->num_ops--;
  return ret;
}

GASNETI_INLINE(gasnete_coll_scratch_add_to_wait)
void gasnete_coll_scratch_add_to_wait(gasnete_coll_scratch_req_t *scratch_req, gasnete_coll_op_t* op) {
  gasnete_coll_scratch_status_t *stat = scratch_req->team->scratch_status;
  gasnete_coll_op_info_t *new_op;
  gasnete_coll_scratch_config_t *temp;
  int op_attached=0;
    
  gasneti_assert(op->waiting_scratch_op == 0);
  op->waiting_scratch_op = 1;
  op->active_scratch_op = 0;
  

  new_op = (gasnete_coll_op_info_t*) gasneti_calloc(1,sizeof(gasnete_coll_op_info_t));
  new_op->next = NULL;
  new_op->prev = NULL;
  new_op->req = scratch_req;
  new_op->op = op;
  new_op->local_scratch_used = scratch_req->incoming_size;
  new_op->seq_number = op->sequence;
  new_op->tree_dir = scratch_req->tree_dir;
  new_op->done = 0;
  
  temp = stat->waiting_config_and_ops_tail;
  if(temp!=NULL && gasnete_coll_scratch_compare_config(temp, scratch_req)) {
      gasnete_coll_scratch_add_op_to_config(temp, new_op);
  } else {
    /* attach to the end of the list*/
    if(stat->waiting_config_and_ops_head == NULL) {
      temp =  gasnete_coll_scratch_alloate_new_config(scratch_req);
      stat->waiting_config_and_ops_tail = stat->waiting_config_and_ops_head = temp;
    } else {
      temp = gasnete_coll_scratch_alloate_new_config(scratch_req);
      temp->prev = stat->waiting_config_and_ops_tail;
      temp->next = NULL;
      stat->waiting_config_and_ops_tail->next = temp;
      stat->waiting_config_and_ops_tail = temp;
    }
    gasnete_coll_scratch_add_op_to_config(temp, new_op);
  }
  stat->num_waiting_ops++;
}

GASNETI_INLINE(gasnete_coll_scratch_remove_first_waiting_op)
gasnete_coll_op_info_t* gasnete_coll_scratch_remove_first_waiting_op(gasnete_coll_scratch_status_t *stat) {
  gasnete_coll_op_info_t* ret;
  ret = gasnete_coll_scratch_remove_first_op_from_config(stat->waiting_config_and_ops_head);
  stat->num_waiting_ops--;
  if(stat->waiting_config_and_ops_head->num_ops == 0) {
    gasnete_coll_scratch_config_t *temp;
    temp = stat->waiting_config_and_ops_head;
    stat->waiting_config_and_ops_head = stat->waiting_config_and_ops_head->next;
    if(stat->waiting_config_and_ops_head==NULL) {
      stat->waiting_config_and_ops_tail = NULL;
    } else {
      stat->waiting_config_and_ops_head->prev = NULL;
    }
    temp->prev = NULL;
    temp->next = NULL;
    gasneti_free(temp);
  }
  /*fprintf(stderr, "%d,%d> remove from wait\n",  ret->seq_number, gasneti_mynode);*/

  return ret;
}

GASNETI_INLINE(gasnete_coll_scratch_reconfigure)
void gasnete_coll_scratch_reconfigure(gasnete_coll_scratch_status_t *stat, 
                                      gasnete_coll_scratch_req_t *req,
                                      gasnete_coll_scratch_config_t *new_config) {
  /* free the old configuration*/
  /* the old ops should be empty*/
  gasnete_coll_scratch_config_t *config;
  int flag = 0;
  /*fprintf(stderr, "%d> reconfigure!\n", gasneti_mynode);*/
  if(!stat->active_config_and_ops) {
    stat->active_config_and_ops = 
    (gasnete_coll_scratch_config_t*) gasneti_calloc(1,sizeof(gasnete_coll_scratch_config_t));
    flag = 1;
  }
  if(flag || !gasnete_coll_scratch_compare_config(stat->active_config_and_ops, req)) {
    config = stat->active_config_and_ops;
    config->op_type = new_config->op_type;
    config->tree_type = new_config->tree_type;
    config->root = new_config->root;
    config->tree_dir = new_config->tree_dir;
    config->dissem_radix = new_config->dissem_radix;
  
    if(config->numpeers > 0) {
      gasneti_free(config->peers);    
    }
  
    /* set the new config and the information about who weill send to me*/
    config->numpeers = req->num_in_peers;
    config->peers = gasneti_malloc(sizeof(gasnet_node_t)*config->numpeers);
    GASNETE_FAST_UNALIGNED_MEMCPY_CHECK(config->peers, req->in_peers, sizeof(gasnet_node_t)*config->numpeers);
  }
}

GASNETI_INLINE(gasnete_coll_scratch_check_local_alloc)
uint8_t gasnete_coll_scratch_check_local_alloc(gasnete_coll_scratch_req_t *req,
                                               gasnete_coll_scratch_status_t *stat) {
  return (req->incoming_size + stat->node_status[req->team->myrank].head <= 
          req->team->scratch_segs[req->team->myrank].size);
}

GASNETI_INLINE(gasnete_coll_scratch_make_local_alloc)
uint64_t gasnete_coll_scratch_make_local_alloc(gasnete_coll_scratch_req_t *req,
                                               gasnete_coll_scratch_status_t *stat) {
  uint64_t ret;
  ret = stat->node_status[req->team->myrank].head;
  stat->node_status[req->team->myrank].head += req->incoming_size;
  return ret;
}

GASNETI_INLINE(gasnete_coll_scratch_check_remote_clear)
uint8_t gasnete_coll_scratch_check_remote_clear(gasnete_coll_scratch_req_t *req,
                                                gasnete_coll_scratch_status_t *stat) {
  gasnet_node_t i;
  
  for(i=0; i<req->num_out_peers; i++) {
    /*fprintf(stderr, "%d> waiting for clear from %d\n", gasneti_mynode, req->out_peers[i]);*/
    if(gasneti_weakatomic_read(&(stat->node_status[req->out_peers[i]].reset_signal_sent),0)
       == gasneti_weakatomic_read(&(stat->node_status[req->out_peers[i]].reset_signal_recv),0)
       ) {
      return 0;
    }
  }
  /* reset all the signals once we get all of them*/
  /*fprintf(stderr, "%d> got all clear\n", gasneti_mynode);*/
  for(i=0; i<req->num_out_peers; i++) {
    gasneti_weakatomic_increment(&(stat->node_status[req->out_peers[i]].reset_signal_recv),0);
    stat->node_status[req->out_peers[i]].head = 0;
  }
  return 1;
}

GASNETI_INLINE(gasnete_coll_scratch_check_remote_alloc)
uint8_t gasnete_coll_scratch_check_remote_alloc(gasnete_coll_scratch_req_t *req,
                                                gasnete_coll_scratch_status_t *stat) {
  gasnet_node_t i;
  int8_t check_for_clear=0;
  
  for(i=0; i<req->num_out_peers; i++) {
    if(stat->node_status[req->out_peers[i]].head + req->out_sizes[(req->op_type == GASNETE_COLL_DISSEM_OP ? 0 : i)] >  
       req->team->scratch_segs[req->out_peers[i]].size) {
      /*fprintf(stderr, "%d> waiting for clear from %d\n", gasneti_mynode, req->out_peers[i]);*/
      /* remote space is full */
      if(gasneti_weakatomic_read(&(stat->node_status[req->out_peers[i]].reset_signal_sent),0)==
         gasneti_weakatomic_read(&(stat->node_status[req->out_peers[i]].reset_signal_recv),0)
         ) {
        return 0;
      } else {
        /*fprintf(stderr, "%d> got clear from %d\n", gasneti_mynode, req->out_peers[i]);*/
        stat->node_status[req->out_peers[i]].head = 0;
        gasneti_weakatomic_increment(&(stat->node_status[req->out_peers[i]].reset_signal_recv),0);
      }
    }
  }
  return 1;
}

GASNETI_INLINE(gasnete_coll_scratch_make_remote_alloc)
void gasnete_coll_scratch_make_remote_alloc(gasnete_coll_scratch_req_t *req,
                                            gasnete_coll_scratch_status_t *stat,
                                            uint64_t *rem_pos) {
  gasnet_node_t i;
  for(i=0; i<req->num_out_peers; i++) {
    rem_pos[i] = stat->node_status[req->out_peers[i]].head;
    stat->node_status[req->out_peers[i]].head += req->out_sizes[(req->op_type == GASNETE_COLL_DISSEM_OP ? 0 : i)]; 
  }  
}

int8_t gasnete_coll_scratch_alloc_nb(gasnete_coll_op_t* op GASNETE_THREAD_FARG) {
  gasnete_coll_scratch_req_t *scratch_req = op->scratch_req;
  gasnete_coll_scratch_status_t *stat = scratch_req->team->scratch_status;
  gasnete_coll_op_info_t *new_op;
  gasnete_coll_node_scratch_status_t node_stat;
  uint32_t my_head_pos;
  uint32_t my_tail_pos;
  uint64_t retpos;


  gasneti_assert(scratch_req);
  gasneti_assert(stat);
  /*if the incoming size is greater than the total allocated scratch space signal an error*/
  if(scratch_req->incoming_size > scratch_req->team->scratch_segs[scratch_req->team->myrank].size) {
    gasneti_fatalerror("%d> collective requires temporary storage (%ld bytes) which is greater than total scratch space (%ld bytes)\nIncrease size of collective scratch space through GASNET_COLL_SCRATCH_SIZE environment variable to at least %ld bytes\n", 
                       (int)scratch_req->team->myrank, 
                       (long int)scratch_req->incoming_size, (long int)scratch_req->team->scratch_segs[0].size, (long int)scratch_req->incoming_size); 
  }
  gasneti_assert(op->waiting_scratch_op == 0 || op->waiting_scratch_op == 1);
  if(op->waiting_scratch_op) {
    /* this operation is already on the wait queue */
    /* check if this op is at the head of the wait queue 
    (do not modify the wait queue until we know the op can be activated)*/
    if(!stat->waiting_config_and_ops_head) 
      
    gasneti_assert(stat->waiting_config_and_ops_head);
    gasneti_assert(stat->waiting_config_and_ops_head->op_list_head);
    gasneti_assert(stat->waiting_config_and_ops_head->op_list_head->seq_number <= op->sequence); /* make sure that the head is an op w/ a lower sequence # */
    if(stat->waiting_config_and_ops_head->op_list_head->seq_number != op->sequence) {
      /* operation is not at the head of the wait queue so return an allocate fail*/
      return 0;
    }   
  } else if(stat->num_waiting_ops > 0){
    /* i go at the end of the wait queue*/
  gasneti_assert(stat->waiting_config_and_ops_head->op_list_head->seq_number <= op->sequence); /* make sure that the head is an op w/ a lower sequence # */
    gasnete_coll_scratch_add_to_wait(scratch_req, op);
    
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS
    fprintf(stderr, "%d,%d> op added to wait (wait queue non empty)\n",  op->sequence, gasneti_mynode); 
#endif
    return 0;
  } 
  
  /* if we get here then op is a new op w/ no ops waiting or it is the first on the wait queue */
 /* fprintf(stderr, "%d,%d> polling scratch\n", op->sequence, gasneti_mynode);*/
  if(op->waiting_for_reconfig_clear || 
     stat->active_config_and_ops == NULL || 
     (!gasnete_coll_scratch_compare_config(stat->active_config_and_ops, scratch_req) && stat->active_config_and_ops->num_ops == 0)) {
    /* empty scratch space is misconfigured*/
 
   

    if(!op->waiting_for_reconfig_clear) {
      if(stat->num_waiting_ops>0) {
        gasnete_coll_scratch_reconfigure(stat, scratch_req, stat->waiting_config_and_ops_head);
      } else {
        gasnete_coll_scratch_reconfigure(stat, scratch_req, gasnete_coll_scratch_alloate_new_config(scratch_req));
      } 
    }
    
    
    if(stat->clear_signal_sent==0) {
      stat->node_status[scratch_req->team->myrank].head = 0; 
      /* send a clear signal to the new peers*/
      gasnete_coll_scratch_send_updates(scratch_req->team, op->sequence);
      stat->clear_signal_sent=1; /* make sure clear signal is sent only once per drain cycle*/
    }

    op->waiting_for_reconfig_clear=1;

    if(gasnete_coll_scratch_check_remote_clear(scratch_req, stat)) {
      gasnete_coll_op_info_t *op_info;
      uint64_t *positions;
      stat->scratch_empty = 0;
      stat->clear_signal_sent = 0;
      
      op->active_scratch_op = 1;
      /* move it from the waiting to the active list*/
      
      if(op->waiting_scratch_op) {
        op_info = gasnete_coll_scratch_remove_first_waiting_op(stat);
        op->waiting_scratch_op = 0;
      } else {
        op_info = (gasnete_coll_op_info_t*) gasneti_calloc(1,sizeof(gasnete_coll_op_info_t));
        op_info->next = NULL;
        op_info->prev = NULL;
        op_info->req = scratch_req;
        op_info->tree_dir = scratch_req->tree_dir;
        op_info->local_scratch_used = scratch_req->incoming_size;
        op_info->seq_number = op->sequence;
        op_info->done = 0;
        op_info->op = op;
      }
     gasnete_coll_scratch_add_op_to_config(stat->active_config_and_ops, op_info);
      /* return the appropriate amount of local/remote scratch space*/
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS
       fprintf(stderr, "%d> allocating for op %d\n", gasneti_mynode, op->sequence); 
#endif
      op->myscratchpos = gasnete_coll_scratch_make_local_alloc(scratch_req, stat);
      
      op->scratchpos = gasneti_malloc(sizeof(uint64_t)*(scratch_req->num_out_peers));
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
  } else if(gasnete_coll_scratch_compare_config(stat->active_config_and_ops, scratch_req)) {
    /*fprintf(stderr, "%d,%d> polling scratch --> correct config check for space\n", op->sequence, gasneti_mynode);*/
    /* configuration matches the current configuration ... check if we can allocate*/
    if(!gasnete_coll_scratch_check_local_alloc(scratch_req, stat)) { 
      if(stat->active_config_and_ops->num_ops==0) {
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
      gasnete_coll_op_info_t *op_info;
      op->active_scratch_op = 1;
      stat->scratch_empty = 0;
      stat->clear_signal_sent=0;

      /* remove the operation from the wait queue and add it to the active queue if it was waiting*/
      /* allocate op and return*/
 
      if(op->waiting_scratch_op) {
        op_info = gasnete_coll_scratch_remove_first_waiting_op(stat);
        op->waiting_scratch_op = 0;
      } else {
        op_info = (gasnete_coll_op_info_t*) gasneti_calloc(1,sizeof(gasnete_coll_op_info_t));
        op_info->next = NULL;
        op_info->prev = NULL;
        op_info->req = scratch_req;
        op_info->tree_dir = scratch_req->tree_dir;
        op_info->local_scratch_used = scratch_req->incoming_size;
        op_info->seq_number = op->sequence;
        op_info->done = 0;
        op_info->op = op;
      }
      gasnete_coll_scratch_add_op_to_config(stat->active_config_and_ops, op_info);
      /* return the appropriate amount of local/remote scratch space*/
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS      
      fprintf(stderr, "%d> allocating for op %d\n", gasneti_mynode, op->sequence); 
#endif
      op->myscratchpos = gasnete_coll_scratch_make_local_alloc(scratch_req, stat);
      op->scratchpos = gasneti_malloc(sizeof(uint64_t)*(scratch_req->num_out_peers));
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
  /* find the op in the active scratch op list and remove it*/
  gasnete_coll_op_info_t *temp= op->scratch_req->team->scratch_status->active_config_and_ops->op_list_head;
  int op_found = 0;
  int first = 1;

  gasneti_assert(op->scratch_op_freed==0);

  gasneti_assert(temp);
#if GASNETE_COLL_SCRATCH_DEBUG_PRINTS 
  fprintf(stderr, "%d,%d> finishing op\n", op->sequence, gasneti_mynode);
#endif
  while(temp!=NULL) {
    if(temp->seq_number == op->sequence) {
      if(temp->next) temp->next->prev = temp->prev;
      if(temp->prev) temp->prev->next = temp->next;
      if(temp == op->scratch_req->team->scratch_status->active_config_and_ops->op_list_head) {
        op->scratch_req->team->scratch_status->active_config_and_ops->op_list_head = temp->next;
      } 
      if(temp == op->scratch_req->team->scratch_status->active_config_and_ops->op_list_tail) {
        op->scratch_req->team->scratch_status->active_config_and_ops->op_list_tail = temp->prev;
      }
      op_found = 1;
      gasneti_free(temp);
      if(op->scratch_req->out_sizes) {
        gasneti_free(op->scratch_req->out_sizes);
      }
      break;
      
    } else {
      temp = temp->next;
      first = 0;
    }
  }
  gasneti_assert(op_found);
#if GASNET_DEBUG
  op->scratch_op_freed = 1;
#endif
  op->scratch_req->team->scratch_status->active_config_and_ops->num_ops--;
  
  if(op->scratch_req->team->scratch_status->active_config_and_ops->num_ops==0) {
    op->scratch_req->team->scratch_status->active_config_and_ops->op_list_head = 
    op->scratch_req->team->scratch_status->active_config_and_ops->op_list_tail = NULL;
  } else {
    gasneti_assert(op->scratch_req->team->scratch_status->active_config_and_ops->op_list_head);
    gasneti_assert(op->scratch_req->team->scratch_status->active_config_and_ops->op_list_tail);
  }

  gasneti_free(op->scratch_req);

}
