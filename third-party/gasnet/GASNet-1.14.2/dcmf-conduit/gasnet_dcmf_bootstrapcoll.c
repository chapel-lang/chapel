/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_dcmf_bootstrapcoll.c,v $
 *     $Date: 2009/09/16 01:13:22 $
 * $Revision: 1.4 $
 * Description: GASNet dcmf conduit Implementation
 * Copyright 2008, Rajesh Nishtala <rajeshn@cs.berkeley.edu>, 
                   Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_core_internal.h>


#if 0
static gasnetc_dcmf_req_t *gasnetc_dcmf_req_free_list;
static inline gasnetc_dcmf_req_t * gasnetc_get_dcmf_req(void) {
    gasnetc_dcmf_req_t *req;
    if(gasnetc_dcmf_req_free_list) {
  req = gasnetc_dcmf_req_free_list;
  gasnetc_dcmf_req_free_list = req->next;
    } else {
  req = (gasnetc_dcmf_req_t*) gasneti_malloc(sizeof(gasnetc_dcmf_req_t));
    }
    return req;
}

static inline void gasnetc_free_dcmf_req(gasnetc_dcmf_req_t *req){
    req->next = gasnetc_dcmf_req_free_list;
    gasnetc_dcmf_req_free_list = req;
}
#endif

static void gasnetc_inc_uint64_arg_cb(void* arg, DCMF_Error_t *e) {
    uint64_t *in = (uint64_t*) arg;
    (*in)++;
}

static void gasnetc_inc_uint32_arg_cb(void* arg, DCMF_Error_t *e) {
    uint32_t *in = (uint32_t*) arg;
    (*in)++;
}

static void gasnetc_inc_uint8_arg_cb(void* arg, DCMF_Error_t *e) {
    uint8_t *in = (uint8_t*) arg;
    (*in)++;
}

typedef struct gasnetc_recv_done_cb_args_{
    gasnetc_dcmf_req_t *req;
    uint64_t *counter;
} gasnetc_recv_done_cb_args_t __attribute__((__aligned__(8)));

void gasnetc_recv_done_cb(void *arg, DCMF_Error_t *e) {
    gasnetc_recv_done_cb_args_t *in = (gasnetc_recv_done_cb_args_t*) arg;
    if(in->counter) (*(in->counter))++;
    gasnetc_free_dcmf_req(in->req);
    gasneti_free(in);
}



typedef struct gasnetc_bootstrap_coll_t_{
    DCMF_Protocol_t registration; /*512 bytes*/
    DCMF_Callback_t callback; /*two pointers (8 bytes)*/
    DCMF_Request_t request; /*512 bytes*/
    uint64_t done_counter,init_counter; /*16 bytes*/
} gasnetc_bootstrap_coll_t __attribute__ ((__aligned__ (2048)));
/* align up to the next power of 2*/

gasnetc_bootstrap_coll_t *gasnetc_bootstrap_barrier;
gasnetc_bootstrap_coll_t *gasnetc_bootstrap_broadcast;
gasnetc_bootstrap_coll_t *gasnetc_bootstrap_exchange;

void gasnetc_dcmf_bootstrap_coll_init(void) {
    DCMF_GlobalBarrier_Configuration_t barrier_config;
    DCMF_GlobalBcast_Configuration_t broadcast_config;
 
    GASNETC_DCMF_LOCK();
    /*DCMF_Collective_initialize();*/
    /** Initialize the barrier **/
    gasnetc_bootstrap_barrier = (gasnetc_bootstrap_coll_t*) gasneti_malloc(sizeof(gasnetc_bootstrap_coll_t));
    barrier_config.protocol = DCMF_GI_GLOBALBARRIER_PROTOCOL;
    gasnetc_bootstrap_barrier->done_counter = 0;
    gasnetc_bootstrap_barrier->init_counter = 0;
    gasnetc_bootstrap_barrier->callback.function = gasnetc_inc_uint64_arg_cb;
    gasnetc_bootstrap_barrier->callback.clientdata = &gasnetc_bootstrap_barrier->done_counter;
    
    DCMF_SAFE(DCMF_GlobalBarrier_register(&gasnetc_bootstrap_barrier->registration, &barrier_config));

    /** Initialize the broadcast **/
    gasnetc_bootstrap_broadcast = (gasnetc_bootstrap_coll_t*) gasneti_malloc(sizeof(gasnetc_bootstrap_coll_t));
    broadcast_config.protocol = DCMF_TREE_GLOBALBCAST_PROTOCOL;
    gasnetc_bootstrap_broadcast->done_counter = 0;
    gasnetc_bootstrap_broadcast->init_counter = 0;
    gasnetc_bootstrap_broadcast->callback.function = gasnetc_inc_uint64_arg_cb;
    gasnetc_bootstrap_broadcast->callback.clientdata = &gasnetc_bootstrap_broadcast->done_counter;
    
    DCMF_SAFE(DCMF_GlobalBcast_register(&gasnetc_bootstrap_broadcast->registration, &broadcast_config));
    GASNETC_DCMF_UNLOCK();
}

void gasnetc_dcmf_bootstrapBarrier(void) {
    gasnetc_bootstrap_coll_t *barr = gasnetc_bootstrap_barrier;
    barr->init_counter++;
    GASNETC_DCMF_LOCK();
    DCMF_SAFE(DCMF_GlobalBarrier(&barr->registration, 
                                 &barr->request,
                                 barr->callback));
    while(barr->done_counter!=barr->init_counter) {
      DCMF_MESSAGER_POLL();
    }
    GASNETC_DCMF_UNLOCK();
}

void gasnetc_dcmf_bootstrapBroadcast(void *src, size_t len, void *dest, int rootnode) {
    gasnetc_bootstrap_coll_t *bcast = gasnetc_bootstrap_broadcast;
    int mynode  = DCMF_Messager_rank();
    gasnetc_dcmf_bootstrapBarrier();
    bcast->init_counter++;
    GASNETI_TRACE_PRINTF(C,("bootstrap bcast src: %p size: %d dst: %p root: %d\n", src, 
          len, dest, rootnode));
    GASNETC_DCMF_LOCK();

    if(mynode == rootnode) {
      DCMF_SAFE(DCMF_GlobalBcast(&bcast->registration,
                                 &bcast->request,
                                 bcast->callback,
                                 DCMF_MATCH_CONSISTENCY,
                                 rootnode,
                                 (char*) src,
                                 len));
      memcpy(dest, src, len);
    } else {
      DCMF_SAFE(DCMF_GlobalBcast(&bcast->registration, 
                                 &bcast->request,
                                 bcast->callback,
                                 DCMF_MATCH_CONSISTENCY,
                                 rootnode,
                                 (char*) dest,
                                 len));
    }
    while(bcast->done_counter!=bcast->init_counter) {
      DCMF_MESSAGER_POLL();
    }
    GASNETC_DCMF_UNLOCK();
    gasnetc_dcmf_bootstrapBarrier();
}

typedef struct gasnetc_bootstrapExchange_arg_t_ {
    void *dst;
    size_t nbytes;
    uint64_t *counter;
} gasnetc_bootstrapExchange_arg_t;

void gasnetc_bootstrapExchange_recv_short_cb(void *clientdata,
               const DCQuad *msginfo,
               unsigned count, 
               unsigned peer,
               const char *src,
               unsigned bytes) {
    
    gasnetc_bootstrapExchange_arg_t *arg = (gasnetc_bootstrapExchange_arg_t*) clientdata;
    
    memcpy((void*)((uintptr_t)arg->dst+peer*arg->nbytes), src, arg->nbytes);
    (*arg->counter)++;
    
}

DCMF_Request_t* gasnetc_bootstrapExchange_recv_cb(void *clientdata,
              const DCQuad *msginfo,
              unsigned count, 
              unsigned peer,
              unsigned sendlen,
              unsigned *rcvlen,
              char **rcvbuf,
              DCMF_Callback_t *cb_done) {
    gasnetc_dcmf_req_t *ret;
    gasnetc_bootstrapExchange_arg_t *arg = (gasnetc_bootstrapExchange_arg_t*) clientdata;
    gasnetc_recv_done_cb_args_t *cb_args;
    
    cb_args = gasneti_malloc(sizeof(gasnetc_recv_done_cb_args_t));
    
    *rcvbuf = (char*)arg->dst+peer*arg->nbytes;
    *rcvlen = arg->nbytes;
    
   
    ret = gasnetc_get_dcmf_req();
    cb_args->req = ret;
    cb_args->counter = arg->counter;
    cb_done->function = gasnetc_recv_done_cb;
    cb_done->clientdata = cb_args;
    
    return &ret->req;
}
void gasnetc_dcmf_bootstrapExchange(void *src, size_t nbytes, void *dst) {
    DCMF_Send_Configuration_t config;
    DCMF_Protocol_t registration;
    DCMF_Callback_t callback;
    DCMF_Request_t *send_reqs; /*a pointer (4 bytes) to MAX_OUTSTANDING_SENDS*sizeof(DCMF_Request_t)*/
    uint64_t *num_send_recv_done;
    int i;
    gasnetc_bootstrapExchange_arg_t *cb_args;

    GASNETI_TRACE_PRINTF(C,("bootstrap exchange src: %p size: %d dst: %p\n", src, nbytes, dst));
    if(gasneti_nodes ==1 ) {
  
      memcpy(dst, src, nbytes);
      return;
    }
    num_send_recv_done = gasneti_malloc(sizeof(uint64_t)*2);
    num_send_recv_done[0] = 0;
    num_send_recv_done[1] = 0;
    cb_args = gasneti_malloc(sizeof(gasnetc_bootstrapExchange_arg_t));
    send_reqs = gasneti_malloc(sizeof(DCMF_Request_t)*gasneti_nodes);

    cb_args->dst = dst;
    cb_args->counter = &num_send_recv_done[1];
    cb_args->nbytes = nbytes;
    
    config.network = 0;
    config.protocol = DCMF_DEFAULT_SEND_PROTOCOL;
    config.cb_recv_short = gasnetc_bootstrapExchange_recv_short_cb;
    config.cb_recv_short_clientdata = cb_args;
    config.cb_recv = gasnetc_bootstrapExchange_recv_cb;
    config.cb_recv_clientdata = cb_args;
    callback.function = gasnetc_inc_uint64_arg_cb;
    callback.clientdata = &num_send_recv_done[0];
 
    GASNETC_DCMF_LOCK();
    DCMF_SAFE(DCMF_Send_register(&registration, &config));
    GASNETC_DCMF_UNLOCK();
  
        
    gasnetc_dcmf_bootstrapBarrier();
    GASNETC_DCMF_LOCK();
    for(i=1; i<gasneti_nodes; i++) {
      
      DCQuad msginfo;
      int dest = (gasneti_mynode+i) % gasneti_nodes;
      
      DCMF_SAFE(DCMF_Send(&registration,
                          send_reqs+i,
                          callback,
                          DCMF_MATCH_CONSISTENCY,
                          dest,
                          nbytes,
                          src,
                          &msginfo,
                          1));
    }
    
    GASNETE_FAST_UNALIGNED_MEMCPY((void*)((uintptr_t)dst+gasneti_mynode*nbytes), src, nbytes);
    while(num_send_recv_done[0]!=gasneti_nodes-1) DCMF_MESSAGER_POLL();
    while(num_send_recv_done[1]!=gasneti_nodes-1) DCMF_MESSAGER_POLL();
    
    GASNETC_DCMF_UNLOCK();
    gasnetc_dcmf_bootstrapBarrier();
    
    gasneti_free(cb_args);
    gasneti_free(num_send_recv_done);
    gasneti_free(send_reqs);
}
