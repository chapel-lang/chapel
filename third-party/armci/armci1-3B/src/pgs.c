/* $Id$ 
 * Note: the general ARMCI copyright does not apply to code included in this file 
 *       Explicit permission is required to copy/modify this code. 
 */


#include <stdlib.h>
#include <stdio.h>

#include <qsnet/config.h>
#include <qsnet/types.h>
#include <qsnet/fence.h>
#include <elan/elan.h>
#include <elan4/library.h>
#include <elan4/lib_spinlock.h>

#include <elan4/types.h>
#include <elan4/events.h>
#include <elan4/dma.h>
#include <elan4/registers.h>
#include "pgs_thread.h"

#if QSNETLIBS_VERSION_CODE < QSNETLIBS_VERSION(1,18,0)
#define BINLOAD 1
#else
#define DIRECTLOAD 1
#endif


extern void armci_die();
extern int armci_me;

#ifndef offsetof
#define offsetof(T,F)	((int)&(((T *)0)->F))
#endif

/* Hardware event defines */
#define EVENT			E4_Event
#define EVENT32			E4_Event32
#define EVENT_ALIGN		E4_EVENT_ALIGN
#define EVENT_WORD		volatile E4_uint64
#define EVENT_BLK		E4_Event_Blk

#define EVENT_WORD_READY(WORD)	(*(WORD))

#define INITEVENT		elan4_initevent
#define INITEVENT_WORD		elan4_initevent_write
#define PRIMEEVENT_WORD		elan4_primeevent_write
#define RESETEVENT_WORD(WORD)	(*(WORD) = 0)		
#define INITEVENT_BLK		elan4_initevent_blk
#define PRIMEEVENT_BLK		elan4_primeevent_blk
#define POLLEVENT_WORD		elan4_pollevent_word
#define WAITEVENT_WORD		elan4_waitevent_word

/* Input Queue defines */
#define INPUT_QUEUE		E4_InputQueue
#define INPUT_QUEUE_SIZE	sizeof(E4_InputQueue)
#define INPUT_QUEUE_ALIGN	E4_INPUTQ_ALIGN
#define INPUT_QUEUE_MIN		sizeof(E4_uint64)
#define INPUT_QUEUE_MAX		2048

/* The datatype of a DMA descriptor */
#define DMA		E4_DMA
#define DMA64		E4_DMA64
#define DMA_ALIGN	E4_DMA_ALIGN

/* Memory allocation */
#define ELAN_ALLOCATOR_SIZE (128*1024)
#define MAIN_ALLOCATOR_SIZE (1024*1024)

#define ALLOC_MAIN(ALLOC, ALIGN, SIZE) elan4_allocMain(ALLOC, ALIGN, SIZE)
#define ALLOC_ELAN(ALLOC, ALIGN, SIZE) elan4_allocElan(ALLOC, ALIGN, SIZE)
#define ALLOC_VADDR(ALLOC, ALIGN, SIZE) elan4_allocVaddr(ALLOC, ALIGN, SIZE)
#define ALLOC_ELAN_VADDR(ALLOC, ALIGN, SIZE) elan4_allocElanVaddr(ALLOC, ALIGN, SIZE)

#define FREE_MAIN(R, ADDR) elan4_freeMain(ALLOC, ADDR)
#define FREE_ELAN(R, ADDR) elan4_freeElan(ALLOC, ADDR)


/* Address translation */
#define MAIN2ELAN(CTX, ADDR) elan4_main2elan(((ELAN4_CTX *)CTX), ADDR)
#define ELAN2MAIN(CTX, ADDR) elan4_elan2main(((ELAN4_CTX *)CTX), ADDR)
#define SDRAM2ELAN(CTX, ADDR) elan4_main2elan(((ELAN4_CTX *)CTX), ADDR)
#define ELAN2SDRAM(CTX, ADDR) elan4_elan2main(((ELAN4_CTX *)CTX), ADDR)

/* Default co-processor stack alignment */
#define THREAD_STACK_ALIGN	E4_STACK_ALIGN

/* Co-processor code loading */
#define LOADSO		ELAN4_LOAD
#define OPEN_SO		elan4_open_so
#define LOAD_SO		elan4_load_so
#define FIND_SYM_SO	elan4_find_sym
#define CLOSE_SO	elan4_close_so
#define UNLOAD_SO	elan4_unload_so


/* DEBUG macros */
#  define PRINTF0(STATE,m,fmt)
#  define PRINTF1(STATE,m,fmt,a)
#  define PRINTF2(STATE,m,fmt,a,b)
#  define PRINTF3(STATE,m,fmt,a,b,c)
#  define PRINTF4(STATE,m,fmt,a,b,c,d)
#  define PRINTF5(STATE,m,fmt,a,b,c,d,e)
#  define PRINTF6(STATE,m,fmt,a,b,c,d,e,f)
#  define PRINTF7(STATE,m,fmt,a,b,c,d,e,f,g)
#  define PRINTF8(STATE,m,fmt,a,b,c,d,e,f,g,h)


/* 
 * Generic libelan EVENT managament functions
 *
 */

typedef enum eventState
{
    EVENT_BAD = 0,    /* Unconfigured */
    EVENT_LIVE_LIVE,  /* In hands of a higher level library-possibly ourselfs */
    EVENT_LIVE_DONE,  /* In hands of a higher level library but known finished */
    EVENT_FREE        /* Idle */
} EVENTSTATE;

/* Generic event structure returned by libelan interfaces.
 *
 * Currently there is 1.5 pointers worth of padding in this struct.
 *
 * Many [poll|wait]Fns simply call elan_pollWord (look in queueTx.c)
 * so it might be an idea to have the argumnets to elan_pollWord
 * embedded in this struct instead of poll/waitFn.  This would
 * need 2.5 pointers (RAIL, event, doneWord) however they could exist
 * in a union with the poll/waitFn.
 * 
 * The "handle" could be recoverd to store a pointer in too if needed.
 *
 * I would need to borrow a bit from the "state" to store a type bit to
 * implement this.
 */
typedef struct event_main
{
    void               *handle;                  /* Generic handle */
    int (*pollFn)(ELAN_EVENT *ready, long how);  /* fn to poll for completion */
    void (*waitFn)(ELAN_EVENT *ready, long how); /* fn to wait for completion */
    void (*freeFn)(ELAN_EVENT *ready);           /* fn to free off descriptor when done */
    struct event_main  *evm_next;
    enum eventState     e_state;                /* State (word) */

}		EVENT_MAIN;

typedef int (*EVENT_POLLFN)(ELAN_EVENT *ready, long how);
typedef void (*EVENT_WAITFN)(ELAN_EVENT *ready, long how);
typedef void (*EVENT_FREEFN)(ELAN_EVENT *ready);

extern int	elan_pollWord (ELAN_STATE *elan_state, ELAN_RAIL *rail,
			       EVENT_WORD *ready, long how);
extern void	elan_waitWord (ELAN_STATE *elan_state, ELAN_RAIL *rail,
			       void *readyEvent, EVENT_WORD *ready, long how);

/* Called every time it's before it's released to the user */
#define ELAN_EVENT_PRIME(X)   (((EVENT_MAIN *)X)->e_state = EVENT_LIVE_LIVE);


#include "pgs_sys.h"

static void _elan_pgsFree (ELAN_EVENT *event);
static void _elan_pgsWait (ELAN_EVENT *event, long how);
static int  _elan_pgsPoll (ELAN_EVENT *event, long how);

int elan_pgsGlobalMemSize (ELAN_STATE *state)
{
    return(INPUT_QUEUE_SIZE);
}

void pgs_getStats(pgsstate_t *state, ELAN_PUTGETSTATS *stats)
{
}

#define idivide(x,y) ((y) ? (x)/(y) : 0)


/*
 * allocate and initialise our request
 */
int pgs_initDesc (ELAN_STATE *state, ELAN_RAIL *rail, void *handle,
		  void *main, ADDR_SDRAM elan)
{
    PGS_REQDESC *r = (PGS_REQDESC *)main;
    PGS_REQELAN *re;
    PGS_RAIL *pgsrail = (PGS_RAIL*)handle;
    PGS_REQ *req;
    DMA64 *qdma;

    r->r_elan = re = elan;
    r->r_dests = 0;
    r->r_rail = pgsrail;

    INITEVENT_WORD(rail->rail_ctx, (E4_Event *)&re->re_doneEvent, &r->r_done);
    PRIMEEVENT_WORD(rail->rail_ctx, (E4_Event *)&re->re_doneEvent, 1);
    re->re_chainEvent.ev_Params[0] = elan4_main2elan (rail->rail_ctx, (void *) &re->buf_dma);
    re->re_chainEvent.ev_Params[1] = elan4_alloccq_space (rail->rail_ctx, 8, CQ_Size8K);
    
    req = &r->r_req;
    req->req_doneEvent = MAIN2ELAN(rail->rail_ctx, &re->re_doneEvent);
    req->req_srcvp = state->vp;
    
    /*
     * allocate and initialise the QDMA 
     */
    qdma = &r->r_qdma;
    qdma->dma_dstAddr  = 0; /* Queue slot offset */

    r->r_event.evm_next = NULL;
    r->r_event.handle = pgsrail;
    r->r_event.pollFn = _elan_pgsPoll;
    r->r_event.waitFn = _elan_pgsWait;
    r->r_event.freeFn = _elan_pgsFree;
 
    return(TRUE);
}

static E4_uint64* _bflags;

void pgs_railInit (pgsstate_t *pgsstate, PGS_RAIL *pgsrail, int nSlots)
{
    ELAN_RAIL *rail = pgsrail->pr_rail;
    ELAN_ESTATE *estate;
    E4_ThreadRegs tregs;
    E4_CmdQ   *tcmdq;
    PGS_ELAN  *pe;
    
    pgsrail->pr_state = pgsstate;

    /* Create an SDRAM allocator */
    if ((pgsrail->pr_sdram = elan4_open_sdram (pgsrail->pr_id, 0, ELAN_ALLOCATOR_SIZE)) == NULL)
    {
	armci_die ("Failed: elan4_open_sdram",-1);
    }
    
    /* Create a Main/Elan memory allocator */
    if ((pgsrail->pr_alloc = elan4_createAllocator (MAIN_ALLOCATOR_SIZE, pgsrail->pr_sdram, 0, ELAN_ALLOCATOR_SIZE)) == NULL) {
	armci_die ("Failed: elan4_createAllocator\n",-1);
    }

#if 0
    /*
     * Setup for descriptor allocation 
     */
    _elan_caRegister(pgsstate->elan_state,
		     &pgsrail->pr_ca,		          /* control      */
		     rail,	                          /* rail         */
		     "PGS_REQDESC",	                  /* name         */
		     offsetof(PGS_REQDESC, r_event),
		     sizeof(PGS_REQDESC), ELAN_ALIGN,     /* main         */
		     sizeof(PGS_REQELAN), ELAN_ALIGN,     /* elan         */
		     pgs_initDesc,	                  /* initFn       */
		     CA_AUTODESTROY,		          /* destroyFn    */
		     pgsrail,			          /* handle       */
		     4, 4, 128,	   	                  /* init, add (0 = double), max */
		     DBG_PGS);                            /* debug flags  */
#endif

    /* Allocate a cookie pool */
    pgsrail->pr_cpool = elan4_allocCookiePool(rail->rail_ctx, pgsstate->elan_state->vp);

    /* 
     * allocate Elan control structure
     */
    if(!(pe=ALLOC_ELAN(pgsrail->pr_alloc,ELAN_ALIGN,ELAN_ALIGNUP(sizeof(PGS_ELAN),ELAN_ALIGN))))
	elan_exception(pgsstate->elan_state, ELAN_ENOMEM, "pgs_init: Elan memory exhausted");
    pgsrail->pr_elan = pe;

    memset(pe, 0, sizeof(PGS_ELAN));
    MEMBAR_STORESTORE();
    

    /*
     * Allocate a command queue for issuing DMAs/STEN packets
     * and running pgs_thread
     */
    if ((pgsrail->pr_cmdq = elan4_alloc_cmdq(rail->rail_ctx, 
#if QSNETLIBS_VERSION_CODE < QSNETLIBS_VERSION(1,7,0)
                                             pgsrail->pr_alloc, 
#endif
                                             CQ_Size8K,
					     CQ_ThreadStartEnableBit |
					     CQ_WriteEnableBit |
					     CQ_DmaStartEnableBit | 
                                             CQ_STENEnableBit,
					     NULL)) == NULL)
	elan_exception (pgsstate->elan_state, ELAN_EINTERNAL,
			"pgs_init: failed to allocate a command queue");
    
    /*
     * Allocate a command port for the thread
     */
#define PGS_CMDQ_WORK_PER_QSLOT 10
    {
	unsigned cqsize = elan4_cqsize_inputq_ctrlflow (0, PGS_CMDQ_WORK_PER_QSLOT, nSlots);
	if ((tcmdq = elan4_alloc_cmdq(rail->rail_ctx,
#if QSNETLIBS_VERSION_CODE < QSNETLIBS_VERSION(1,7,0)
                                      pgsrail->pr_alloc, 
#endif
                                      cqsize, CQ_EnableAllBits, NULL)) == NULL)
	    elan_exception (pgsstate->elan_state, ELAN_EINTERNAL,
			    "pgs_init: failed to allocate thread command queue on rail(rail %p)\n", rail);

	/* Translate CQ address for thread use */
	pe->pe_work_cmdq = MAIN2ELAN (rail->rail_ctx, tcmdq->cmdq_mapping);
	/* allocate a shared cmdq for thread rescheduling */
	if ((pe->pe_sched_cmdq = elan4_alloccq_space(rail->rail_ctx, 8, CQ_Size1K)) == 0)
	    elan_exception (pgsstate->elan_state, ELAN_ENOMEM,
			    "pgs_init: failed to alloc shared command queue resource rail ctx %p",
			    rail->rail_ctx);
    }

    /*
     * Copy down Elan pgs state
     */
    estate = (ELAN_ESTATE *)pgsstate->elan_state->estate;
    pe->pe_state = MAIN2ELAN(rail->rail_ctx, estate);
    pe->pe_shared = MAIN2ELAN(rail->rail_ctx, &pgsrail->pr_shared);

    /* initialise and prime the thread started event */
    INITEVENT_WORD(rail->rail_ctx, (E4_Event *)&pe->pe_started, &pgsrail->pr_started);
    PRIMEEVENT_WORD(rail->rail_ctx, (E4_Event *)&pe->pe_started, 1);

    /* Zero the InputQueue */
    pgsrail->pr_q = ELAN2MAIN(rail->rail_ctx, pgsstate->pgs_qaddr);
    memset(pgsrail->pr_q, 0, sizeof(INPUT_QUEUE)); 
    
    /*
     * Now initialise the Elan Input Queue structure
     * We create this in Elan memory as it will be the Elan thread
     * that has to read the envelope information
     */
    if (!(pgsrail->pr_qbase = ALLOC_ELAN(pgsrail->pr_alloc, ELAN_ALIGN, nSlots * PGS_QSLOTSIZE)))
	elan_exception (pgsstate->elan_state, ELAN_ENOMEM, "pgs_init: Elan memory exhausted");
    
    pgsrail->pr_qtop = (char *)pgsrail->pr_qbase + ((nSlots-1) * PGS_QSLOTSIZE);
    
    /* Write Queue address to the Elan */
    pe->pe_qaddr = pgsstate->pgs_qaddr;
    
    /* Stash the queue FirstItem/LastItem for receiver thread use */
    pe->pe_qbase = MAIN2ELAN(rail->rail_ctx, pgsrail->pr_qbase);
    pe->pe_qtop = MAIN2ELAN(rail->rail_ctx, pgsrail->pr_qtop);
    
    pgsrail->pr_q->q_bptr = MAIN2ELAN(rail->rail_ctx, pgsrail->pr_qbase);
    pgsrail->pr_q->q_fptr = MAIN2ELAN(rail->rail_ctx, pgsrail->pr_qbase);
    
    pgsrail->pr_q->q_control = 
	    E4_InputQueueControl (MAIN2ELAN(rail->rail_ctx, pgsrail->pr_qbase),
				  MAIN2ELAN(rail->rail_ctx, pgsrail->pr_qtop), PGS_QSLOTSIZE);
	     
#ifdef DEBUG
    /* Fill Input queue slots with silly pattern */
    memset(pgsrail->pr_qbase, 0xeb, nSlots * PGS_QSLOTSIZE);
#else
    memset(pgsrail->pr_qbase, 0, nSlots * PGS_QSLOTSIZE);
#endif
    
    /* Allocate queue event */
    if (!(pgsrail->pr_qevent = (EVENT *) ALLOC_ELAN(pgsrail->pr_alloc, 64, sizeof(E4_Event32))))
	elan_exception (pgsstate->elan_state, ELAN_ENOMEM, "pgs_init: Elan memory exhausted:");
    
    /* Initialise the event */
    pgsrail->pr_qevent->ev_CountAndType = 0;
    pgsrail->pr_qevent->ev_CopySource = 0;
    pgsrail->pr_qevent->ev_CopyDest =  0;
    
    pgsrail->pr_q->q_event = MAIN2ELAN(rail->rail_ctx, pgsrail->pr_qevent);


#if 0
    elan_dbg(pgsstate->elan_state, DBG_INIT | DBG_PGS,
	     "pgs_init: queue %lx q_bptr %llx q_control %llx q_event %lx(%llx)\n",
	     pgsrail->pr_q, pgsrail->pr_q->q_bptr,pgsrail->pr_q->q_control,
	     pgsrail->pr_qevent, pgsrail->pr_q->q_event);
#endif
    
    /* Allocate thread stack */
    if (!(pgsrail->pr_stack = ALLOC_ELAN(pgsrail->pr_alloc, THREAD_STACK_ALIGN, PGS_THREADSTACK)))
	elan_exception (pgsstate->elan_state, ELAN_ENOMEM, 
			"pgsInit: Elan memory exhausted: stack allocate: pgs %p", pgsstate);
    

     /* Load the co-processor thread */
    {
	void *cspace;
	LOADSO *code;

#if DIRECTLOAD
        if((code = elan4_open_so_static(rail->rail_ctx, bin_data, BIN_DATA_ELEMS,0)) ==NULL)
                                        armci_die("failed to load code",-1); 

#else

#if BINLOAD
	char file_name[20];
	FILE *fp;
	int sz;
	extern int armci_msg_me();
	char *tmp;

        if (!(tmp = getenv("ARMCI_PSGTMP"))) tmp = "/tmp";
	sprintf(file_name, "%s/PGS%d.%d.so", tmp, getpid(), getuid()); 
	fp = fopen((const char*)file_name, "w");
	if(fp==NULL)armci_die("cannot open so file",-1);
	sz = fwrite(bin_data, 1, BIN_DATA_ELEMS, fp);
	if(sz<BIN_DATA_ELEMS) {armci_die("pgs:file write error",-1);}
	fclose(fp);
#else
        char *file_name = "elan/pgs_thread.so";
#endif
	
	if ((code = pgsrail->pr_pgsCode = OPEN_SO(rail->rail_ctx,
				  (const char*)file_name, NULL, 0)) == NULL)
	    armci_die("failed to load thread code",-1);
#endif
	
	cspace = elan4_allocElan (pgsrail->pr_alloc, 8192, code->loadsize);

	if (LOAD_SO(code, cspace, elan4_main2elan (rail->rail_ctx, cspace)) < 0)
	    armci_die("failed to load code",-1);
	
	pgsrail->pr_pgsSym  = elan4_find_sym (code, "pgs_thread");

#if !defined(BINLOAD) && !defined(DIRECTLOAD)

	unlink((const char*)file_name);
#endif


    }

#if 0
    elan_dbg(pgsstate->elan_state, DBG_INIT | DBG_PGS,
	     "pgs_init: loaded lib @ %p(%Lx) : 'pgs_thread' symbol @ %Lx\n",
	     pgsrail->pr_pgsCode->loadbase, pgsrail->pr_pgsCode->loadaddr, rail->r_pgsSym);
    
    elan_dbg(pgsstate->elan_state, DBG_INIT | DBG_PGS,
	     "pgs_init: starting elan pgs thread (%Lx) - stack %p args %Lx\n",
	     rail->r_pgsSym, pgsrail->pr_stack, MAIN2ELAN(rail->rail_ctx, pe));
#endif
    
    /* Create the initial thread register state */
    elan4_init_thread(rail->rail_ctx, &tregs, pgsrail->pr_pgsSym,
		      pgsrail->pr_stack, PGS_THREADSTACK,
		      /* nargs */ 1, 
		      MAIN2ELAN(rail->rail_ctx, pe));
        
    /* 
     * Start the thread and wait until it sets the started event
     */
    elan4_run_thread_cmd(pgsrail->pr_cmdq, &tregs);
    pgsrail->pr_cmdq->cmdq_flush (pgsrail->pr_cmdq);

    /* Wait for thread to get started */
    elan_waitWord(pgsstate->elan_state, rail, &pe->pe_started, &pgsrail->pr_started, ELAN_POLL_EVENT);

#if 0
    elan_dbg(pgsstate->elan_state, DBG_INIT | DBG_PGS, "pgs_init: thread started\n");
#endif
}



void * pgs_init (ELAN_STATE *state, void *qMem)
{
    pgsstate_t *pgsstate;
    int r;
    
    /*
     * setup the pgs state for this putget control
     */
    if ((pgsstate = (pgsstate_t *)calloc(1, sizeof(pgsstate_t))) == NULL)
	elan_exception(state, ELAN_ENOMEM, "pgs_init: failed to allocate memory");
    
    pgsstate->elan_state = state;
    pgsstate->pgs_vp = state->vp;
    
    pgsstate->pgs_waitType = (elan_base ? elan_base->waitType : ELAN_POLL_EVENT);
    
    /* Stash the main and Elan Queue addresses */
    pgsstate->pgs_qaddr = (ADDR_ELAN) ((uintptr_t) qMem);
    
    /*
     * initialisation each rail
     */
    pgsstate->pgs_nrails = elan_nRails(pgsstate->elan_state);
    if((pgsstate->pgs_rails=(PGS_RAIL*)calloc(pgsstate->pgs_nrails, sizeof(PGS_RAIL))) == NULL)
	elan_exception(pgsstate->elan_state, ELAN_ENOMEM,"pgs_init:failed to allocate memory");
    for (r=0; r < pgsstate->pgs_nrails; r++)
    {
	PGS_RAIL *pr = pgsstate->pgs_rails + r;
	pr->pr_id = r;
	pr->pr_rail = pgsstate->elan_state->rail[r];
	
	pgs_railInit(pgsstate, pr, 16);
    }

    pgsstate->pgs_profile = 0;

#if 0
    printf("pgs initialized\n"); fflush(stdout);
#endif
    return pgsstate;
}

void * pgs_ds_init (ELAN_STATE *state, void *qMem, void *dsqMem, int maxbufs)
{
    pgsstate_t *pgsstate;
    int r;
   
    /*
     * setup the pgs state for this putget control
     */
    if ((pgsstate = (pgsstate_t *)calloc(1, sizeof(pgsstate_t))) == NULL)
        elan_exception(state, ELAN_ENOMEM, "pgs_init: failed to allocate memory");
   
    pgsstate->elan_state = state;
    pgsstate->pgs_vp = state->vp;
   
    pgsstate->pgs_waitType = (elan_base ? elan_base->waitType : ELAN_POLL_EVENT);
   
    /* Stash the main and Elan Queue addresses */
    pgsstate->pgs_qaddr = (ADDR_ELAN) ((uintptr_t) qMem);
    pgsstate->ds_qaddr = (ADDR_ELAN) ((uintptr_t) dsqMem);
   
    _bflags = elan_gallocElan(elan_base, elan_base->allGroup, 64, maxbufs*sizeof(E4_uint64));
    if(!_bflags)elan_exception(pgsstate->elan_state, ELAN_ENOMEM, "bflags: no Elan memory");
#if QSNETLIBS_VERSION_CODE > QSNETLIBS_VERSION(1,7,0)
    _bflags =elan4_elan2main(elan_base->state->ctx,(ADDR_ELAN)_bflags);
#endif
    bzero(_bflags,maxbufs*sizeof(E4_uint64));
    MEMBAR_STORESTORE();

    /*
     * initialisation each rail
     */
    pgsstate->pgs_nrails = elan_nRails(pgsstate->elan_state);
    if((pgsstate->pgs_rails=(PGS_RAIL*)calloc(pgsstate->pgs_nrails, sizeof(PGS_RAIL))) == NULL)
        elan_exception(pgsstate->elan_state, ELAN_ENOMEM, "pgs_init:failed to allocate memory");
    for (r=0; r < pgsstate->pgs_nrails; r++)
    {
        PGS_RAIL *pr = pgsstate->pgs_rails + r;
        pr->pr_id = r;
        pr->pr_rail = pgsstate->elan_state->rail[r];

        pgs_railInit(pgsstate, pr, 16);
    }

    pgsstate->pgs_profile = 0;

#if 0
    printf("pgs initialized\n"); fflush(stdout);
#endif
    return pgsstate;
}


static ELAN_EVENT *issueStridedRequest(PGS_RAIL *pgsr, PGS_REQDESC *r)
{
    PGS_REQ *req;
    DMA64 *qdma;

    MUTEX_LOCK(&pgsr->pr_mutex);
    qdma = &r->r_qdma;
    req = &r->r_req;
    
    /* 
     * Update the request 
     */
    qdma->dma_typeSize = E4_DMA_TYPE_SIZE(r->r_qdmasize, DMA_DataTypeByte, DMA_QueueWrite, 16);
    qdma->dma_dstEvent = pgsr->pr_state->pgs_qaddr;
    qdma->dma_srcEvent = 0;
    qdma->dma_srcAddr  = MAIN2ELAN(pgsr->pr_rail->rail_ctx, req);
    
    /* 
     * Update the DMA descriptor 
     */
    qdma->dma_cookie = elan4_local_cookie(pgsr->pr_cpool, E4_COOKIE_TYPE_LOCAL_DMA, r->r_vp);
    qdma->dma_vproc = r->r_vp;
    
    /* 
     * Send the QDMA 
     */
    elan4_run_dma_cmd(pgsr->pr_cmdq, (DMA *)qdma);
    pgsr->pr_cmdq->cmdq_flush(pgsr->pr_cmdq);

    MUTEX_UNLOCK(&pgsr->pr_mutex);
    
    ELAN_EVENT_PRIME(&r->r_event);
    return(&r->r_event);
}



static ELAN_EVENT *issueDSRequest_(PGS_RAIL *pgsr, PGS_REQDESC *r, void *buf)
{
    DMA64 *qdma;
    PGS_REQ *req=&r->r_req;

    MUTEX_LOCK(&pgsr->pr_mutex);

    /*  Update the DMA descriptor */
    qdma = &r->r_qdma;
    qdma->dma_typeSize = E4_DMA_TYPE_SIZE(r->r_qdmasize, DMA_DataTypeByte, DMA_QueueWrite, 16);
    qdma->dma_dstEvent = pgsr->pr_state->ds_qaddr;
    qdma->dma_cookie   = elan4_local_cookie(pgsr->pr_cpool, E4_COOKIE_TYPE_LOCAL_DMA, r->r_vp);
    qdma->dma_vproc    = r->r_vp;
    qdma->dma_srcEvent = req->req_doneEvent;
    qdma->dma_srcAddr  = MAIN2ELAN(pgsr->pr_rail->rail_ctx, buf);

    /* Send the QDMA */
    elan4_run_dma_cmd(pgsr->pr_cmdq, (DMA *)qdma);
    pgsr->pr_cmdq->cmdq_flush(pgsr->pr_cmdq);

    MUTEX_UNLOCK(&pgsr->pr_mutex);

    ELAN_EVENT_PRIME(&r->r_event);
    return(&r->r_event);
}



static ELAN_EVENT *
issueDSRequest(PGS_RAIL *pgsr, PGS_REQDESC *r, void *head, void* data, void *buf, int dlen)
{
    DMA64 *qdma;
    DMA64 *dma;
    PGS_REQ *req=&r->r_req;

    MUTEX_LOCK(&pgsr->pr_mutex);

    /*  Update the DMA descriptor */
    qdma = &r->r_qdma;
    qdma->dma_vproc    = r->r_vp;
    qdma->dma_cookie   = elan4_local_cookie(pgsr->pr_cpool, E4_COOKIE_TYPE_LOCAL_DMA, r->r_vp);
    qdma->dma_typeSize = E4_DMA_TYPE_SIZE(r->r_qdmasize, DMA_DataTypeByte, DMA_QueueWrite, 16);
    qdma->dma_dstEvent = pgsr->pr_state->ds_qaddr;
    qdma->dma_srcEvent = req->req_doneEvent;
    qdma->dma_srcAddr  = MAIN2ELAN(pgsr->pr_rail->rail_ctx, head);
    qdma->dma_typeSize |= RUN_DMA_CMD;
    qdma->dma_pad      = NOP_CMD;

    dma = &r->r_dma;
    dma->dma_vproc    = r->r_vp;
    dma->dma_cookie   = elan4_local_cookie(pgsr->pr_cpool, E4_COOKIE_TYPE_LOCAL_DMA, r->r_vp);
    dma->dma_typeSize = E4_DMA_TYPE_SIZE(dlen, DMA_DataTypeByte, 0, 16);
    dma->dma_srcEvent = MAIN2ELAN(pgsr->pr_rail->rail_ctx, &r->r_elan->re_chainEvent); 
    dma->dma_dstEvent = 0;
    dma->dma_srcAddr  = MAIN2ELAN(pgsr->pr_rail->rail_ctx, data);
    dma->dma_dstAddr  = MAIN2ELAN(pgsr->pr_rail->rail_ctx, buf);

    /* Copy down the chain dma to the chain buffer in elan sdram  */
    memcpy ((void *)&r->r_elan->buf_dma, (void *)qdma, sizeof (E4_DMA64));

    /* initialize the chain event  */
    r->r_elan->re_chainEvent.ev_CountAndType = E4_EVENT_INIT_VALUE(-32,E4_EVENT_COPY,E4_EVENT_DTYPE_LONG,8);

    /* Perform a memory barrier to flush all outstanding stores */
#if defined(__ia64__)
    _armci_ia64_mb();
#endif
    
#if 0
    printf("%d ChainDMA to=%d h=%p d=%p b=%p hlen=%d dlen=%d\n",
            armci_me,r->r_vp,head,data,buf,r->r_qdmasize,dlen);
#endif

    /* run primary DMA */
    elan4_run_dma_cmd(pgsr->pr_cmdq, (DMA *)dma);
    pgsr->pr_cmdq->cmdq_flush(pgsr->pr_cmdq);

#if 0
    sleep(2);
    elan4_run_dma_cmd(pgsr->pr_cmdq, (DMA *)qdma);
    pgsr->pr_cmdq->cmdq_flush(pgsr->pr_cmdq);
#endif

    MUTEX_UNLOCK(&pgsr->pr_mutex);

    ELAN_EVENT_PRIME(&r->r_event);
    return(&r->r_event);
}



static void pack_sdscr(void *src, void *dst, int *src_stride_arr, int *dst_stride_arr, u_int *count, u_int strides, u_int destvp, PGS_REQ *r, ELAN_RAIL *rail)
{
int i;

    r->src = MAIN2ELAN(rail->rail_ctx, src);
    r->dst = MAIN2ELAN(rail->rail_ctx, dst);
    r->strides = (E4_uint32)strides;
    for(i=0; i<strides; i++)r->src_stride_arr[i] = (E4_uint32)src_stride_arr[i];
    for(i=0; i<strides; i++)r->dst_stride_arr[i] = (E4_uint32)dst_stride_arr[i];
    for(i=0; i<strides+1; i++)r->count[i] = (E4_uint32)count[i];
}


PGS_REQDESC *
ACQUIRE_LH (PGS_RAIL *pgsrail)
{
    PGS_REQDESC *r, *rbase;
    PGS_REQELAN *re;
    
    int i, growth = 16;

    if ((r = pgsrail->pr_freeDescs))
    {
	pgsrail->pr_freeDescs = r->r_suc;
	r->r_suc = NULL;
	return r;
    }
    
    /* New to grow pool of free descs */
    r = rbase = ALLOC_MAIN(pgsrail->pr_alloc, ELAN_ALIGN, sizeof(PGS_REQDESC) * growth);
    re = ALLOC_ELAN(pgsrail->pr_alloc, ELAN_ALIGN, sizeof(PGS_REQELAN) * growth);

    for (i = 0; i < growth; i++, r++, re++)
    {
	/* Initialise each newly allocated desc */
	pgs_initDesc(pgsrail->pr_state->elan_state, pgsrail->pr_rail, pgsrail, r, re);
	
	/* Save all but first on freeList */
	if (i)
	{
	    r->r_suc = pgsrail->pr_freeDescs;
	    pgsrail->pr_freeDescs = r;
	}
    }
    
    return rbase;
}

void RELEASE_LH (PGS_REQDESC *r)
{
    PGS_RAIL *pgsrail = r->r_rail;

    r->r_suc = pgsrail->pr_freeDescs;
    pgsrail->pr_freeDescs = r;
}


ELAN_EVENT *armci_sendq(void *pgs,u_int destvp, void *head, int hlen, void *data, void *buf, int dlen)
{
    extern int armci_me;
    int rail=0 ;
    pgsstate_t *pgsstate = (pgsstate_t *) pgs;
    ELAN_EVENT *event = 0;
    PGS_REQDESC *rdesc;
    PGS_RAIL *pgsr = pgsstate->pgs_rails;

    if (!(rdesc = (PGS_REQDESC *)ACQUIRE_LH(pgsr)))
          elan_exception(pgsr->pr_state->elan_state, ELAN_ENOMEM,
                       "elan_getbuflag: failed to allocate descriptor");
    rdesc->r_qdmasize = hlen;
    rdesc->r_vp = destvp;

    event = issueDSRequest(pgsr, rdesc, head, data, buf, dlen);
    return(event);
    
}



#define PUTSIZE 512 
#define GETSIZE 512 

ELAN_EVENT *elan_putss (void *pgs, void *src, void *dst, int *src_stride_arr, 
                 int *dst_stride_arr, u_int *count, u_int strides, u_int destvp)
{
    int rail=0 ;
    pgsstate_t *pgsstate = (pgsstate_t *) pgs;
    ELAN_EVENT *event = 0;
    PGS_REQDESC *rdesc;

    if(strides<0 || strides> MAXSTRIDE){
       fprintf(stderr,"error in strided put: bad stride %d\n",strides);
       return(event);
    }

#if 0
    pgsstate->pgs_putscalls++;
    bytes = count[0];
    for(i=0; 1<strides; i++)
      bytes *= count[i];
    pgsstate->pgs_putsbytes += bytes;

    if (bytes == 0)
    {
	event = elan_nullEvent(pgsstate->elan_state);
	return(event);
    }
#endif

    if(count[0] <= PUTSIZE && strides==1 ){ /* use linked elan_puts */
      int i;
      char *ps =    (char*)src;
      char *pd =    (char*)dst;
      for(i=0; i< count[1]; i++){
         event = elan_link(event,elan_put(elan_base->state,ps, pd, count[0], destvp));
         ps += src_stride_arr[0];
         pd += dst_stride_arr[0];
      }

    } else for(rail = 0; rail <1; rail++){ /* for now only 1 rail */
         PGS_RAIL *pgsr = pgsstate->pgs_rails + rail;
         if (!(rdesc = (PGS_REQDESC *)ACQUIRE_LH(pgsr)))
     	   elan_exception(pgsr->pr_state->elan_state, ELAN_ENOMEM,
		       "elan_putss: failed to allocate descriptor");

         pack_sdscr(src, dst, src_stride_arr, dst_stride_arr, count, strides, 
                  destvp, &rdesc->r_req, pgsr->pr_rail);
   
       
         rdesc->r_qdmasize = STRIDEDHSIZE;
         rdesc->r_vp = pgsstate->elan_state->vp; /* where to send the PUTS desc */
         rdesc->r_req.req_type = PGS_PUTS;
         rdesc->r_req.req_rvp = destvp; /* where to send the data to */
         event = elan_link(event, issueStridedRequest(pgsr, rdesc)); 
    }

    return(event);
}

/*\ extra can be scale factor, esize is sizeof(extra)
\*/
ELAN_EVENT *elan_accs (void *pgs, void *src, void *dst, int *src_stride_arr, 
                 int *dst_stride_arr, u_int *count, u_int strides, 
                 void *extra, int esize, u_int destvp)
{
    int rail=0 ;
    pgsstate_t *pgsstate = (pgsstate_t *) pgs;
    ELAN_EVENT *event = 0;
    PGS_REQDESC *rdesc;

    if(strides<0 || strides> MAXSTRIDE){
       fprintf(stderr,"error in strided put: bad stride %d\n",strides);
       return(event);
    }

#if 0
    pgsstate->pgs_putscalls++;
    bytes = count[0];
    for(i=0; 1<strides; i++) bytes *= count[i];
    pgsstate->pgs_putsbytes += bytes;

    if (bytes == 0) {
        event = elan_nullEvent(pgsstate->elan_state);
        return(event);
    }
#endif

    for(rail = 0; rail <1; rail++){ /* for now only 1 rail */
       PGS_RAIL *pgsr = pgsstate->pgs_rails + rail;
       if (!(rdesc = (PGS_REQDESC *)ACQUIRE_LH(pgsr)))
           elan_exception(pgsr->pr_state->elan_state, ELAN_ENOMEM,
                       "elan_putss: failed to allocate descriptor");

       pack_sdscr(src, dst, src_stride_arr, dst_stride_arr, count, strides,
                  destvp, &rdesc->r_req, pgsr->pr_rail);
  
       rdesc->r_qdmasize = STRIDEDHSIZE;
       rdesc->r_vp = destvp; /* where to send the ACC desc (rem node) */
       rdesc->r_req.req_type = PGS_ACC;
       rdesc->r_req.req_rvp = pgsstate->elan_state->vp; /* source data */
       event = elan_link(event, issueStridedRequest(pgsr, rdesc));
    }

    return(event);
}



ELAN_EVENT *elan_getss (void *pgs, void *src, void *dst, int *src_stride_arr, int *dst_stride_arr, u_int *count, u_int strides, u_int destvp)
{
    int rail=0 ;
    pgsstate_t *pgsstate = (pgsstate_t *) pgs;
    ELAN_EVENT *event = 0;
    PGS_REQDESC *rdesc;

    if(strides<0 || strides> MAXSTRIDE){
       fprintf(stderr,"error in strided put: bad stride %d\n",strides);
       return(event);
    }

#if 0
    pgsstate->pgs_putscalls++;
    bytes = count[0];
    for(i=0; 1<strides; i++) bytes *= count[i];
    pgsstate->pgs_putsbytes += bytes;

    if (bytes == 0) {
	event = elan_nullEvent(pgsstate->elan_state);
	return(event);
    }
#endif

    if(count[0] <= GETSIZE && strides==1 ){ /* use linked elan_puts */
      int i;
      char *ps =    (char*)src;
      char *pd =    (char*)dst;
      for(i=0; i< count[1]; i++){
         event = elan_link(event,elan_get(elan_base->state,ps, pd, count[0], destvp));
         ps += src_stride_arr[0];
         pd += dst_stride_arr[0];
      }
    } else for(rail = 0; rail <1; rail++){ /* for now only 1 rail */
       PGS_RAIL *pgsr = pgsstate->pgs_rails + rail;
       if (!(rdesc = (PGS_REQDESC *)ACQUIRE_LH(pgsr)))
   	   elan_exception(pgsr->pr_state->elan_state, ELAN_ENOMEM,
		       "elan_putss: failed to allocate descriptor");

       pack_sdscr(src, dst, src_stride_arr, dst_stride_arr, count, strides, 
                  destvp, &rdesc->r_req, pgsr->pr_rail);
       
       rdesc->r_qdmasize = STRIDEDHSIZE;
       rdesc->r_vp = destvp; /* where to send the PUTS desc to */
       rdesc->r_req.req_type = PGS_PUTS;
       rdesc->r_req.req_rvp = pgsstate->elan_state->vp; /* where to send the data to */
       event = elan_link(event, issueStridedRequest(pgsr, rdesc)); 
    }

    return(event);
}


ELAN_EVENT *elan_getbflag(void *pgs,u_int destvp, int lo, int hi, int wait, long *retval)
{
    extern int armci_me;
    int rail=0 ;
    pgsstate_t *pgsstate = (pgsstate_t *) pgs;
    ELAN_EVENT *event = 0;
    PGS_REQDESC *rdesc;
    PGS_RAIL *pgsr = pgsstate->pgs_rails;

    if (!(rdesc = (PGS_REQDESC *)ACQUIRE_LH(pgsr)))
          elan_exception(pgsr->pr_state->elan_state, ELAN_ENOMEM,
                       "elan_getbuflag: failed to allocate descriptor");
    //rdesc->r_qdmasize = BFLAGHSIZE;
    rdesc->r_qdmasize = 128; 
    rdesc->r_req.RangeLo = lo;
    rdesc->r_req.RangeHi = hi;
    rdesc->r_req.CanWait = wait;
    rdesc->r_vp = destvp;
    rdesc->r_req.req_type = PGS_BFLAG;
    rdesc->r_req.req_rvp = pgsstate->elan_state->vp;
    //rdesc->r_req.src = rdesc->r_pbflags;
    rdesc->r_req.src = MAIN2ELAN(pgsr->pr_rail->rail_ctx, _bflags);
    rdesc->r_req.dst = MAIN2ELAN(pgsr->pr_rail->rail_ctx, retval); 
    event = issueStridedRequest(pgsr, rdesc);
    return event;
}



void elan_clearbflag(void *pgs, int which)
{
    pgsstate_t *pgsstate = (pgsstate_t *) pgs;
    PGS_RAIL *pgsr = pgsstate->pgs_rails;
    elan4_store64(0,_bflags+which); 
//    bzero(pgsr->bflags+which,sizeof(bflag_t));
//    MEMBAR_STORESTORE();
}


static int _elan_pgsPoll (ELAN_EVENT *event, long how)
{
    PGS_REQDESC *r = (PGS_REQDESC*)event;
    PGS_RAIL    *pgsrail = r->r_event.handle;
    ELAN_RAIL   *rail = pgsrail->pr_rail;
    ELAN_STATE  *state = pgsrail->pr_state->elan_state;
    
    PRINTF2(state, DBG_PGS, "_elan_pgsPoll(%p) polling how=%x magic=%x\n", event, how);
    
    if (!elan_pollWord(state, rail, &r->r_done, how))
    {
	PRINTF1(state, DBG_PGS, "_elan_pgsPoll(%p) returning FALSE\n", event);
	return(FALSE);
    }
    return(TRUE);
}


static void _elan_pgsWait (ELAN_EVENT *event, long how)
{
    PGS_REQDESC *r = (PGS_REQDESC*)event;
    PGS_RAIL    *pgsrail = r->r_event.handle;
    ELAN_RAIL   *rail = pgsrail->pr_rail;
    ELAN_STATE  *state = pgsrail->pr_state->elan_state;
    
    PRINTF2(state, DBG_PGS, "_elan_pgsWait(%p) waiting on event %p\n", event, &r->r_elan->re_doneEvent);
    
    elan_waitWord(state, rail, &r->r_elan->re_doneEvent, &r->r_done, how);
    
    return;
}


static void _elan_pgsFree (ELAN_EVENT *event)
{
    PGS_REQDESC *r = (PGS_REQDESC*)event;
    PGS_RAIL    *pgsrail = r->r_event.handle;
    ELAN_RAIL   *rail = pgsrail->pr_rail;

    // clear the descriptor for reuse
    r->r_dests = 0;

    MUTEX_LOCK(&pgsrail->pr_mutex);
    
    // reprime the software event
    r->r_done = 0;
    PRIMEEVENT_WORD(rail->rail_ctx, (E4_Event *)&r->r_elan->re_doneEvent, 1);
    
    RELEASE_LH(r);
    
    MUTEX_UNLOCK(&pgsrail->pr_mutex);
}



typedef struct devent {
    EVENT_MAIN  de_event;
    E4_uint64   de_doneWord;    /* main memory done word     */
    E4_Event32 *de_doneEvent;   /* Elan event                */
    ADDR_ELAN   de_elan;        /* preconverted address of done event */
    int         de_count;       /* wait count */
    int         de_current;
    ELAN_STATE *de_state;
} DEVENT;

int elan_devent_completed(int setval, ELAN_EVENT *e)
{
    DEVENT *de = (DEVENT*)(e);
    int val = EVENT_COUNT(de->de_doneEvent);
    return(setval + (val>>5));
}

void armci_elan_wait_event(ELAN_EVENT *e, int val)
{
    DEVENT *de = (DEVENT*)(e);
    /*printf("\n%d:calling eventcount %d val=%d\n",armci_me,de->de_count,val);fflush(stdout);*/
    elan_issueWaitevent(e,val);
    elan_wait(e,elan_base->waitType);
    /*printf("\n%d:done with wait\n",armci_me);fflush(stdout);*/
}

void print_event_info(ELAN_EVENT *e)
{
    DEVENT *de = (DEVENT*)(e);
    printf("\n%d:event info %d %d\n",armci_me,de->de_current,de->de_count);
    fflush(stdout);

}

/*
 * Local variables:
 * c-file-style: "stroustrup"
 * End:
 */



