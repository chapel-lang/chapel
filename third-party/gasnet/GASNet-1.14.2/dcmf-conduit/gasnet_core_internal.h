/*   $Source: /var/local/cvs/gasnet/dcmf-conduit/gasnet_core_internal.h,v $
 *     $Date: 2010/04/17 03:07:18 $
 * $Revision: 1.6.2.1 $
 * Description: GASNet dcmf conduit header for internal definitions in Core API
 * Copyright 2008, Rajesh Nishtala <rajeshn@cs.berkeley.edu>
 *                 Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_CORE_INTERNAL_H
#define _GASNET_CORE_INTERNAL_H

/*#include <dcmf_impl.h>*/
#include <dcmf.h>
#include <dcmf_collectives.h>
#include <dcmf_globalcollectives.h>
#include <gasnet_internal.h>
#include <gasnet_handler.h>



/*DCMF allows up to 7 but since gasnet 
 * only allows 16 am args we can artificailly limit
 * this number to 1 header + 4 data quads*/

#define GASNETC_MAXQUADS_PER_AM 5
#define GASNETC_MAX_AM_ARGS 16


#define GASNETC_DCMF_INTERRUPTS 0


#if GASNET_DEBUG
extern uint8_t gasnetc_have_dcmf_lock;
#define DCMF_SAFE(FUNCALL) do{int retval; gasneti_assert(gasnetc_have_dcmf_lock); retval=FUNCALL; if(retval!=DCMF_SUCCESS) gasneti_fatalerror("DCMF fatal error(%d) at %s:%d\n", retval, __FILE__, __LINE__);} while(0)

#define DCMF_SAFE_NO_CHECK(FUNCALL) do{int retval; retval=FUNCALL; if(retval!=DCMF_SUCCESS) gasneti_fatalerror("DCMF fatal error(%d) at %s:%d\n", retval, __FILE__, __LINE__);} while(0)

#if GASNET_SEQ && (GASNETC_DCMF_INTERRUPTS==0)
#define GASNETC_DCMF_LOCK() do {gasneti_assert(gasnetc_have_dcmf_lock==0); gasnetc_have_dcmf_lock=1;} while(0)
#define GASNETC_DCMF_UNLOCK()  do {gasneti_assert(gasnetc_have_dcmf_lock==1); gasnetc_have_dcmf_lock=0;} while(0)
#define GASNETC_DCMF_CYCLE()  do {gasneti_assert(gasnetc_have_dcmf_lock==1);} while(0)
#else
#define GASNETC_DCMF_LOCK() do {DCMF_CriticalSection_enter(0); gasnetc_have_dcmf_lock=1;} while(0)
#define GASNETC_DCMF_UNLOCK() do {gasnetc_have_dcmf_lock=0; DCMF_CriticalSection_exit(0);} while(0)
#define GASNETC_DCMF_CYCLE() do {gasneti_assert(gasnetc_have_dcmf_lock==1); gasnetc_have_dcmf_lock=0; DCMF_CriticalSection_cycle(0); gasnetc_have_dcmf_lock=1;} while(0)
#endif /*END GASNET_SEQ/PAR IF*/

#else /*!GASNET_DEBUG*/

#define DCMF_SAFE(FUNCALL) if_pf(FUNCALL!=DCMF_SUCCESS) gasneti_fatalerror("DCMF fatal error at %s:%d\n", __FILE__, __LINE__)
#define DCMF_SAFE_NO_CHECK(FUNCALL) if_pf(FUNCALL!=DCMF_SUCCESS) gasneti_fatalerror("DCMF fatal error at %s:%d\n", __FILE__, __LINE__)

#if GASNET_SEQ && (GASNETC_DCMF_INTERRUPTS==0)
#define GASNETC_DCMF_LOCK() do {} while(0)
#define GASNETC_DCMF_UNLOCK()  do {} while(0)
#define GASNETC_DCMF_CYCLE()  do {} while(0)
#else 
#define GASNETC_DCMF_LOCK() DCMF_CriticalSection_enter(0)
#define GASNETC_DCMF_UNLOCK() DCMF_CriticalSection_exit(0)
#define GASNETC_DCMF_CYCLE()  DCMF_CriticalSection_cycle(0)
#endif
#endif

#if GASNETI_STATS_OR_TRACE
#define DCMF_MESSAGER_POLL() do {\
unsigned count; \
gasneti_assert(gasnetc_have_dcmf_lock);\
 count = DCMF_Messager_advance();\
/*GASNETI_TRACE_EVENT_VAL(C, DCMF_POLL_NUM_PROCESSED, count);*/\
/*GASNETI_TRACE_EVENT(C, DCMF_POLL_CALLS);*/\
} while(0)
#else
#define DCMF_MESSAGER_POLL() do {\
gasneti_assert(gasnetc_have_dcmf_lock); DCMF_Messager_advance();\
} while(0)
#endif


/*BG/P hardware requires DCMF data structures to be 16 byte aligned to take advantage of the double hummer so explicitly check for htis in the debug build*/
#if GASNET_DEBUG
#define GASNETC_DCMF_CHECK_PTR(DCMF_PTR) gasneti_assert((DCMF_PTR)!=NULL && (((intptr_t)(DCMF_PTR))%16 == 0))
#else
#define GASNETC_DCMF_CHECK_PTR(DCMF_PTR) do {} while(0)
#endif

#if 0
#define ALIGN_STRUCT(BYTES) __attribute__((__aligned__(BYTES)))
#else
#define ALIGN_STRUCT(BYTES) 
#endif


typedef struct gasnetc_dcmf_req_t_{
  struct gasnetc_dcmf_req_t_ *next;
  uint8_t _pad1[GASNETI_CACHE_LINE_BYTES - sizeof(void*)];
  DCMF_Request_t req;
} gasnetc_dcmf_req_t;

gasnetc_dcmf_req_t * gasnetc_get_dcmf_req(void);
void gasnetc_free_dcmf_req(gasnetc_dcmf_req_t *req);

typedef struct gasnetc_dcmf_coll_req_t_{
  struct gasnetc_dcmf_coll_req_t_ *next;
  uint8_t _pad1[GASNETI_CACHE_LINE_BYTES - sizeof(void*)];
  DCMF_CollectiveRequest_t req;
} gasnetc_dcmf_coll_req_t;

gasnetc_dcmf_coll_req_t * gasnetc_get_dcmf_coll_req(void);
void gasnetc_free_dcmf_coll_req(gasnetc_dcmf_coll_req_t *req);

typedef struct gasnetc_dcmf_nack_req_t_ {
  struct gasnetc_dcmf_nack_req_t_ *next;
  unsigned peer;
  unsigned remote_replay_buffer;
  uint8_t _pad1[MAX(GASNETI_CACHE_LINE_BYTES - sizeof(void*) - sizeof(unsigned)*2,16)];
  DCMF_Request_t req;
} gasnetc_dcmf_nack_req_t ALIGN_STRUCT(1024);


typedef enum{
  GASNETC_MSG_ACK=0,
  GASNETC_MSG_NACK,
  GASNETC_NUM_ACKTYPES
} gasnetc_dcmf_ack_t;

typedef enum{
  GASNETC_AMREQ=0, 
  GASNETC_AMREP,
  GASNETC_NUM_AMTYPES
} gasnetc_dcmf_amtype_t;

/* AM category (recommended impl if supporting PSHM) */
typedef enum {
  gasnetc_Short=0,
  gasnetc_Medium=1,
  gasnetc_Long=2,
  /* conduit-specific additions: */
  gasnetc_LongAsync=3,
  GASNETC_NUM_AMCATS
} gasnetc_category_t;


typedef enum{
  GASNETC_DCMF_SEND_DEFAULT=0,
  GASNETC_DCMF_SEND_EAGER, 
  GASNETC_DCMF_SEND_RVOUS,
  GASNETC_DCMF_NUM_SENDCATS
} gasnetc_dcmf_send_category_t;

typedef struct gasnetc_ambuf_t_ {
  struct gasnetc_ambuf_t_ *next;
  char _pad[16 - sizeof(void*)]; /*padding to ensure that the buffer lies on an 16 byte boundry*/
  uint8_t data[GASNETC_AMMAXMED];
} gasnetc_ambuf_t;

typedef struct gasnetc_replay_buffer_t_ {
  struct gasnetc_replay_buffer_t_ *next;
  char _pad[16 - sizeof(void*)]; /*ensure the quads are aligned on 16 bytes*/
  DCQuad quads[GASNETC_MAXQUADS_PER_AM]; 
  gasnetc_dcmf_amtype_t amtype;
  gasnetc_category_t amcat;
  gasnet_node_t dest_node;
  unsigned numquads;
  gasnetc_ambuf_t *buffer;
  size_t buffer_size;
  int retry_count;

} gasnetc_replay_buffer_t;


typedef struct gasnetc_token_t_ {
  struct gasnetc_token_t_ *next;
  gasnet_node_t srcnode;
  uint8_t sent_reply;
  gasnetc_dcmf_amtype_t amtype;
  gasnetc_category_t amcat;
  gasnetc_dcmf_req_t *dcmf_req;
  unsigned remote_replay_buffer;
} gasnetc_token_t;

typedef struct gasnetc_amhandler_t_{ 
  struct gasnetc_amhandler_t_ *next;
  gasnetc_token_t *token;
  gasnet_handler_t handleridx;
  /*when the buffer is known this is  a pointer to data
    otherwise its just a pointer to a ambuf_t*/
  void *buffer;
  uint8_t buffer_needs_free;
  size_t nbytes;
  int numargs;
  unsigned seq_number;

#if GASNETI_STATS_OR_TRACE
  gasneti_tick_t req_handler_queued;
#endif
  gasnet_handlerarg_t amargs[GASNETC_MAX_AM_ARGS];
} gasnetc_amhandler_t  ALIGN_STRUCT(1024);

/*generic mutex based implementation for now*/
typedef struct gasnetc_fifo_t_ {
  gasneti_mutex_t   lock;
  void **head;
  void **tail;
  char _pad[GASNETT_CACHE_LINE_BYTES - sizeof(void**)*2];
} gasnetc_fifo_t;
#define GASNETC_FIFO_INITIALIZER {GASNETI_MUTEX_INITIALIZER, NULL,NULL}

/*GASNETC_FIFO Operations*/
GASNETI_ALWAYS_INLINE(_gasnetc_fifo_add)
void _gasnetc_fifo_add(gasnetc_fifo_t* fifo, void **elem) {
  gasneti_assert(elem);
  gasneti_assert(fifo);
  
  gasneti_mutex_lock(&(fifo->lock));
  if(fifo->head == NULL) {
    gasneti_assert(fifo->tail == NULL);
    /*assumes the first sizeof(void*) bytes are used for linkage*/
    fifo->head = fifo->tail = elem;
  } else {
    gasneti_assert(fifo->head && fifo->tail);
    *elem = NULL;
    *fifo->tail = elem;
    fifo->tail = elem;
  }
  gasneti_mutex_unlock(&(fifo->lock));
}

GASNETI_ALWAYS_INLINE(_gasnetc_fifo_remove)
void *_gasnetc_fifo_remove(gasnetc_fifo_t *fifo) {
  void **ret;
  

  gasneti_mutex_lock(&(fifo->lock));
  if(fifo->head == NULL){ 
    ret = NULL;
  } else if(fifo->head == fifo->tail) {
    /*this was the last elem on the list*/
    ret = fifo->head;
    fifo->head = fifo->tail = NULL; 
  } else {
    ret = fifo->head;
    fifo->head = *ret;
  }
  gasneti_mutex_unlock(&(fifo->lock));
  
  return (void*) ret;
}

/*add the element to the fifo queue element must be nonNull*/
/*like gasneti_lifo opps we assume that the first sizeof(void*) bytes
 * of the struct are used for linkage*/
GASNETI_INLINE(gasnetc_fifo_add)
void gasnetc_fifo_add(gasnetc_fifo_t* fifo, void *element) {
  _gasnetc_fifo_add(fifo, element);
}

GASNETI_INLINE(gasnetc_fifo_remove)
void *gasnetc_fifo_remove(gasnetc_fifo_t *fifo) {
  return _gasnetc_fifo_remove(fifo);
}

typedef void (*GASNETC_DCMF_RECV_SHORT_CB)(void *client_data, const DCQuad *msginfo, unsigned numquads,
             unsigned peer, const char *src, unsigned nbytes);

typedef void (*GASNETC_DCMF_RECV_HEADER_CB)(void *client_data, const DCQuad *msginfo, unsigned numquads,
              unsigned peer, unsigned sendlen, unsigned *rcvlen, char **rcvbuff, DCMF_Callback_t *cb_done);


typedef struct gasnetc_dcmf_amregistration_t_ {
  DCMF_Protocol_t registration;
  DCMF_Send_Protocol send_category;
} gasnetc_dcmf_amregistration_t;

extern gasnetc_dcmf_amregistration_t *gasnetc_dcmf_amregistration[GASNETC_NUM_AMTYPES][GASNETC_NUM_AMCATS][GASNETC_DCMF_NUM_SENDCATS];

#define GASNETC_DCMF_AM_REGISTARTION(AMTYPE, AMCATEGORY, AMSENDCAT) \
  gasnetc_dcmf_amregistration[AMTYPE][AMCATEGORY][AMSENDCAT]->registration

#define GASNETC_DCMF_AM_SEND_CAT(AMTYPE, AMCATECORY) \
  gasnetc_dcmf_amregistration[AMTYPE][AMCATEGORY][AMSENDCAT]->send_category






void gasnetc_dcmf_finalize(void);

void gasnetc_dcmf_bootstrap_coll_init(void);
void gasnetc_dcmf_p2p_init(void);


void gasnetc_dcmf_bootstrapBarrier(void);
void gasnetc_dcmf_bootstrapBroadcast(void *src, size_t len, void *dest, int rootnode);
void gasnetc_dcmf_bootstrapExchange(void *src, size_t nbytes, void *dst);

/*  whether or not to use spin-locking for HSL's */
#define GASNETC_HSL_SPINLOCK 1

/* ------------------------------------------------------------------------------------ */
#define GASNETC_HANDLER_BASE  1 /* reserve 1-63 for the core API */
#define _hidx_gasnetc_auxseg_reqh             (GASNETC_HANDLER_BASE+0)
/* add new core API handlers here and to the bottom of gasnet_core.c */

/* ------------------------------------------------------------------------------------ */
/* handler table (recommended impl) */
#define GASNETC_MAX_NUMHANDLERS   256
extern gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS];

/* ------------------------------------------------------------------------------------ */

#endif
