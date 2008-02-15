/* $Id: pgs_sys.h,v 1.3.4.2 2006/09/13 17:58:03 andriy Exp $ */
/* $Source: /msrc/proj/hpctools/cvs/armci/src/pgs_sys.h,v $ */

#ifndef _ELAN_PGSSYS_H
#define _ELAN_PGSSYS_H

#ident	"@(#)$Id: pgs_sys.h,v 1.3.4.2 2006/09/13 17:58:03 andriy Exp $"

#define PGS_QSLOTSIZE    1024
#define PGS_THREADSTACK 16384

#define PGS_PUTV   0x1000
#define PGS_GETV   0x2000
#define PGS_PUTS   0x3000
#define PGS_GETS   0x4000
#define PGS_ACC    0x5000
#define PGS_INIT   0x6000
#define PGS_BFLAG  0x7000

#define PGS_PACKED 0x0001
#define PGS_DMA    0x0002

#define ADDR_SDRAM void *
#define ADDR_ELAN  E4_Addr

typedef E4_uint64 bflag_t;

/*
 * PGS components that are shared by Elan and Main
 */
typedef struct pgs_shared
{
    volatile E4_uint64   ps_val1;               /* main/thread: debug word              */
    volatile E4_uint64   ps_val2;               /* main/thread: debug word              */
    volatile E4_uint64   ps_reqServiced;        /* total number of requests serviced    */
    volatile E4_uint64   ps_reqMaxServiced;     /* max requests serviced before waiting */
} PGS_SHARED;

typedef struct pgs_elan
{
    E4_Event32           pe_started;

    ADDR_ELAN            pe_qaddr;		/* Elan receiver queue (global elan addr) */
    ADDR_ELAN            pe_qbase;		/* Elan: first InputQueue packet slot */
    ADDR_ELAN            pe_qtop;		/* Elan: last InputQueue packet slot */
    
#ifdef __ELAN__
    struct pgs_shared    *pe_shared;		/* Shared Main proc control structure */
    ELAN_ESTATE          *pe_state;		/* Elan estate info */
    E4_CommandPort       *pe_work_cmdq;	        /* thread's command queue mapping for inputq work */
    E4_CommandPort       *pe_sched_cmdq;	/* thread's shared command queue for rescheduling */
#else
    ADDR_ELAN            pe_shared;	     	/* Shared Main proc control structure */
    ADDR_ELAN            pe_state;		/* Elan estate info */
    ADDR_ELAN            pe_work_cmdq;		/* thread's command queue mapping for inputq work */
    ADDR_ELAN            pe_sched_cmdq;		/* thread's shared command queue for rescheduling */
#endif /* __ELAN__ */

    /* Must come last (variable amount of space allocated) */
    E4_uint64            pe_local[1];           /* local variables */
} PGS_ELAN;

#define PGS_HEADERSIZE 64
#define PAYLOAD_WORDS ((PGS_QSLOTSIZE - PGS_HEADERSIZE) / sizeof(E4_uint64))
#define PAYLOAD_PAIRS ((PGS_QSLOTSIZE - PGS_HEADERSIZE) / sizeof(pair_t))
#define PGS_MAXRAILS 4

#define PGS_THREAD_BUFSIZE 4096

#define MAXSTRIDE 7
#define MAXCOUNTSIZE 8
#define STRIDEDHSIZE 128
#define BFLAGHSIZE   64
#define RangeLo count[0] 
#define RangeHi count[1] 
#define CanWait count[2]

typedef struct pgsreq {

    /* 128 byte header */
#ifdef __ELAN__
    E4_Event   *req_doneEvent;
    void       *src;
    void       *dst;
#else
    E4_Addr    req_doneEvent;
    E4_Addr    src;
    E4_Addr    dst;
#endif
    E4_uint32  req_type;
    E4_uint32  req_srcvp;	/* calling process */
    E4_uint32  req_rvp;		/* remote process in DMA operations */
    E4_uint32  count[MAXCOUNTSIZE];
    E4_uint32  src_stride_arr[MAXSTRIDE];
    E4_uint32  dst_stride_arr[MAXSTRIDE];
    E4_uint32  strides;
    
} PGS_REQ; 


#define req_buf       addr.remote.buf
#define req_resultBuf addr.remote.resultBuf
#define req_source    addr.local.source
#define req_dest      addr.local.dest

#ifndef __ELAN__

typedef struct pgsrail {
    
    struct pgsstate *pr_state;
    int          pr_id;       /* rail number 0,1,... */
    ELAN_RAIL   *pr_rail;     
    LOADSO      *pr_pgsCode;  /* main: loaded pgs thread code handle      */
    E4_Addr      pr_pgsSym;   /* main: Base address of the pgs_thread     */
    E4_uint64    pr_started;  /* main: done word for thread started event */
    ADDR_SDRAM   pr_stack;    /* main: stack for atomic thread            */
    PGS_SHARED   pr_shared;   /* Shared Elan/Main component               */
    PGS_ELAN    *pr_elan;     /* Elan component                           */

    INPUT_QUEUE *pr_q;
    void        *pr_qbase;    /* main: first InputQueue packet slot */
    void        *pr_qtop;     /* main: last InputQueue packet slot  */
    E4_Event    *pr_qevent;   /* main: InputQueue hw event */
    E4_CmdQ          *pr_cmdq;  /* command queue for main STEN ops */
    ELAN4_COOKIEPOOL *pr_cpool; /* A cookie pool allocator */
    
    MUTEX        pr_mutex;
    ELAN4_SDRAM *pr_sdram;
    ELAN4_ALLOC *pr_alloc;
    struct pgsdesc *pr_freeDescs;

} PGS_RAIL;

typedef struct pgsreqelan {

    E4_Event32 re_doneEvent;  /* Completion event in the caller */
    E4_Event32 re_chainEvent;  
    E4_DMA64   buf_dma;

} PGS_REQELAN;


typedef struct bdesc {
    struct bdesc *next;
    int           id;
    int           size;
    char          buf[4];
} buf_t;

typedef struct pgsdesc {

    EVENT_MAIN   r_event;
    DMA64        r_qdma;
    DMA64        r_dma;
    PGS_REQ      r_req;
    E4_uint64    r_done;	 
    PGS_REQELAN *r_elan;
    PGS_RAIL    *r_rail;
    E4_Addr      r_pbflags;
    int          r_qdmasize;
    int          r_useDMA;
    void       **r_dests;		/* destination addresses for getv */
    int          r_vp;

    struct pgsdesc *r_suc;

} PGS_REQDESC;


typedef struct pgsstate {

    ELAN_STATE             *elan_state;
    int                     pgs_vp;
    int                     pgs_nrails;
    int                     pgs_minstripe;
    int                     pgs_profile;
    int                     pgs_pageSize;
    ELAN_ADDR               pgs_threadCode;
    int                     pgs_waitType;
    MUTEX                   pgs_mutex;      

    PGS_RAIL               *pgs_rails;    /* per rail state */
    ADDR_ELAN               pgs_qaddr;	  /* InputQueue (Elan memory addr) */
    ADDR_ELAN               ds_qaddr;	  /* InputQueue (Elan memory addr) data server*/
    
    int                     putsPending;
    int                     getsPending;

    struct {

	/* stats reported to caller */
	uint64_t putscalls;
	uint64_t putsbytes;
	uint64_t getscalls;
	uint64_t getsbytes;

	/* profiling info */
	uint64_t putssetup;
	uint64_t getssetup;
	uint64_t waitcalls;
	uint64_t waiting;
	uint64_t posting;
	uint64_t freeing;
	uint64_t pollcalls;
	uint64_t maxpoll;
	uint64_t l64calls;
    } stats;
} pgsstate_t;

#define pgs_putscalls stats.putscalls
#define pgs_putsbytes stats.putsbytes
#define pgs_putssetup stats.putssetup
#define pgs_getscalls stats.getscalls
#define pgs_getsbytes stats.getsbytes
#define pgs_getssetup stats.getssetup
#define pgs_waitcalls stats.waitcalls
#define pgs_waiting   stats.waiting
#define pgs_posting   stats.posting
#define pgs_freeing   stats.freeing
#define pgs_pollcalls stats.pollcalls
#define pgs_maxpoll   stats.maxpoll
#define pgs_l64calls  stats.l64calls

extern void *pgs_init (ELAN_STATE *state, void *qMem);

#endif
#endif

/*
 * Local variables:
 * c-file-style: "stroustrup"
 * End:
 */

