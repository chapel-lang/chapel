/*   $Source: /var/local/cvs/gasnet/lapi-conduit/gasnet_core.c,v $
 *     $Date: 2009/10/10 03:38:25 $
 * $Revision: 1.134 $
 * Description: GASNet lapi conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

/* =======================================================================
 * LAPI Conduit Implementation for IBM SP.
 * Michael Welcome
 * Lawrence Berkeley National Laboratory
 * mlwelcome@lbl.gov
 * November, 2002
 * =======================================================================
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/processor.h>

#ifndef GASNETC_VERBOSE_EXIT
#define GASNETC_VERBOSE_EXIT 0
#endif

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name, "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

#ifdef GASNETC_LAPI_RDMA
  GASNETI_IDENT(gasnetc_IdentString_LAPIRDMA, "$GASNetLAPIRDMA: 1 $");
#else
  GASNETI_IDENT(gasnetc_IdentString_LAPIRDMA, "$GASNetLAPIRDMA: 0 $");
#endif

gasnet_handlerentry_t const *gasnetc_get_handlertable(void);

/* Firehose configuration */
/* XXX: TODO make these enviroment tunable and/or auto-probe */
#define GASNETC_FH_MAXREGIONS 1024
/*  before was 2 megs and 64 megs */
/* #define GASNETC_FH_MAXREGION_SIZE (16L*1024L*1024L) */
/* #define GASNETC_FH_MAXREGION_SIZE (64L*1024L) */
#define GASNETC_FH_MAXREGION_SIZE (2L*1024L*1024L)
#define GASNETC_FH_MAX_PINNABLE (128L*1024L*1024L)

/* -------------------------------------------------------------------
 * Begin: LAPI specific variables
 * -------------------------------------------------------------------
 */
lapi_handle_t  gasnetc_lapi_context;
int            gasnetc_max_lapi_uhdr_size;
unsigned long  gasnetc_max_lapi_data_size = LAPI_MAX_MSG_SZ;

#if 0 /* For debugging */
static const char *gasnetc_catname[] = {"Short","Medium","Long","AsyncLong"};
#endif

#if GASNETC_LAPI_RDMA

/* PJRH
 * Extra information needed for LAPI User Level RDMA
 */

static int gasnetc_num_pvos = 0;
static lapi_get_pvo_t *gasnetc_node_pvo_list = NULL;
int gasnetc_lapi_use_rdma;
lapi_remote_cxt_t **gasnetc_remote_ctxts = NULL;
lapi_user_pvo_t **gasnetc_pvo_table = NULL;
lapi_long_t *gasnetc_segbase_table = NULL;
lapi_long_t gasnetc_my_segbase = 0;
lapi_long_t gasnetc_my_segtop = 0;
int *gasnetc_lapi_local_target_counters = NULL;
lapi_cntr_t **gasnetc_lapi_completion_ptrs = NULL;
lapi_long_t *gasnetc_lapi_target_counter_directory = NULL;
#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
extern void gasnete_lapi_setup_nb(void);
extern void gasnete_lapi_free_nb(void);
#endif
/* In case people call exit before attach */
int gasnetc_lapi_rdma_initialized = 0;
/* For opening up some concurrency in multiple transfers to the same node */
gasneti_weakatomic_t *gasnetc_lapi_current_rctxt;
gasneti_weakatomic_val_t gasnetc_rctxts_per_node_mask = 0;
firehose_info_t gasnetc_firehose_info;
lapi_cntr_t gasnetc_incoming_puts_cntr;
lapi_cntr_t gasnetc_incoming_gets_cntr;
int gasnetc_initiated_am_mediums = 0;
extern int gasnete_pin_threshold;
extern int gasnete_pin_max;
#endif

/* This is the official core AM handler table.  All registered
 * entries go here
 */
gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS] = { NULL };
void** gasnetc_remote_req_hh = NULL;
void** gasnetc_remote_reply_hh = NULL;

volatile int gasnetc_got_exit_signal = 0;

/* functions and data needed for AM Exit code */
void** gasnetc_remote_amexit_hh = NULL;
void* gasnetc_amexit_hh(lapi_handle_t *context, void *uhdr, uint *uhdr_len,
			ulong *msg_len, compl_hndlr_t **comp_h, void **uinfo);
void gasnetc_amexit_ch(lapi_handle_t *context, void *uinfo);

static volatile int gasnetc_called_exit = 0;
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int, void*);
#else
static void gasnetc_atexit(void);
#endif

gasnetc_lapimode_t gasnetc_lapi_default_mode = gasnetc_Interrupt;

#if GASNETC_USE_INTERRUPTS
volatile int gasnetc_interrupt_held[GASNETC_MAX_THREAD] = { 0 };
#endif

static gasnetc_token_queue_t gasnetc_req_q;
void gasnetc_run_handler(gasnetc_token_t *token);

gasnetc_uhdr_freelist_t gasnetc_uhdr_freelist;

#define GASNETC_CONFIG_MSG(to_stderr,msg) do { \
	if (to_stderr) {                       \
	    fprintf(stderr,"%s\n",msg);        \
	}                                      \
	GASNETI_TRACE_MSG(C,msg);              \
    } while (0)
    
/* -------------------------------------------------------------------
 * End: LAPI specific variables
 * -------------------------------------------------------------------
 */
/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnetc_check_config(void) {
  gasneti_check_config_preinit();

  /* (###) add code to do some sanity checks on the number of nodes, handlers
     * and/or segment sizes */ 
}

static void gasnetc_bootstrapBarrier(void) {
    /* (###) add code here to implement an external barrier 
       this barrier should not rely on AM or the GASNet API because it's used 
       during bootstrapping before such things are fully functional
       It need not be particularly efficient, because we only call it a few times
       and only during bootstrapping - it just has to work correctly
       If your underlying spawning or batch system provides barrier functionality,
       that would probably be a good choice for this
    */
    /* MLW: All calls to bootstrapBarrier occur after LAPI_init and thus
     * we can safely use the LAPI global fence operations
     */
    GASNETC_LCHECK(LAPI_Gfence(gasnetc_lapi_context));
}

/* --------------------------------------------------------------------------
 * NOTE: the POE job control system on the IBM SP guarantees that argc 
 *       and argv, as well as the environment variables, are distributed 
 *       to all tasks in the parallel jobs.  
 * --------------------------------------------------------------------------
 */
static int gasnetc_init(int *argc, char ***argv) {
    int task_id;
    int num_tasks;
    lapi_info_t    gasnetc_lapi_info;

    /*  check system sanity */
    gasnetc_check_config();

    if (gasneti_init_done) 
	GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");
    gasneti_init_done = 1; /* enable early to allow tracing */

    gasneti_freezeForDebugger();

#if GASNET_DEBUG_VERBOSE
    /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
    fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
#endif

    /* Init the uhdr buffer free list used in GASNET AM calls */
    gasnetc_uhdr_init(GASNETC_UHDR_INIT_CNT);

    /* Init the token queue */
    gasnetc_token_queue_init(&gasnetc_req_q);

    /* (###) add code here to bootstrap the nodes for your conduit */
    memset((void *) &gasnetc_lapi_info, 0, sizeof(lapi_info_t));

    gasnetc_lapi_info.err_hndlr = gasnetc_lapi_err_handler;
    {
	int rc = LAPI_Init(&gasnetc_lapi_context, &gasnetc_lapi_info);
        GASNETI_TRACE_PRINTF(C,("LAPI Init done\n"));
	if (rc != LAPI_SUCCESS) {
          gasneti_fatalerror("In the initialization of the LAPI communication layer\n\n"
		"This application must be run using the poe job scheduler with the following options: \n"
		"  poe appname -euilib us -msg_api lapi -rmpool 1 -procs nproc -nodes numnodes args...\n"
		"See the IBM poe documentation for details\n\n[NOTE: Error code %d at line %d in file %s]\n\n",
                rc,__LINE__,__FILE__);
	}
    }

    /* get task number and number of tasks in job */
    GASNETC_LCHECK(LAPI_Qenv(gasnetc_lapi_context, TASK_ID, &task_id));
    GASNETC_LCHECK(LAPI_Qenv(gasnetc_lapi_context, NUM_TASKS, &num_tasks));
    if (num_tasks < 0 || num_tasks > GASNET_MAXNODES) {
	gasneti_fatalerror("Invalid number of LAPI tasks: %d, must be < %d",
			   num_tasks,GASNET_MAXNODES);
    }
    if (task_id < 0 || task_id > GASNET_MAXNODES) {
	gasneti_fatalerror("Invalid LAPI id: %d, must be < %d",
			   task_id,GASNET_MAXNODES);
    }
    gasneti_mynode = (gasnet_node_t)task_id;
    gasneti_nodes = (gasnet_node_t)num_tasks;

    GASNETC_LCHECK(LAPI_Qenv(gasnetc_lapi_context, MAX_UHDR_SZ, &gasnetc_max_lapi_uhdr_size));

    /* Init tracing early */
    gasneti_trace_init(argc, argv);

    {
	char *gas_ver = getenv("GASNET_LAPI_VERSION");
	int to_stderr = (gasneti_mynode == 0) && 
          (((gas_ver != NULL) && (gas_ver[0] != '0')) || gasneti_verboseenv());
	char buf[80];

#if GASNETC_LAPI_FEDERATION
	GASNETC_CONFIG_MSG(to_stderr,"IBM FEDERATION HARDWARE");
#endif
#if GASNETC_LAPI_COLONY
	GASNETC_CONFIG_MSG(to_stderr,"IBM COLONY HARDWARE");
#endif
#ifdef GASNETC_LAPI_VERSION_A
	(void)snprintf(buf,80,"LAPI VERSION NUMBER = %d.%d.%d.%d",
                              GASNETC_LAPI_VERSION_A, GASNETC_LAPI_VERSION_B,
                              GASNETC_LAPI_VERSION_C, GASNETC_LAPI_VERSION_D);
	GASNETC_CONFIG_MSG(to_stderr,buf);
#endif
	(void)snprintf(buf,80,"GASNET TOKEN SIZE   = %d",(int)GASNETC_TOKEN_SIZE);
	GASNETC_CONFIG_MSG(to_stderr,buf);
	(void)snprintf(buf,80,"GASNET TOKEN REC    = %d",(int)sizeof(gasnetc_token_t));
	GASNETC_CONFIG_MSG(to_stderr,buf);
	(void)snprintf(buf,80,"MAX LAPI UHDR SIZE  = %d",(int)gasnetc_max_lapi_uhdr_size);
	GASNETC_CONFIG_MSG(to_stderr,buf);
	(void)snprintf(buf,80,"MAX LAPI DATA SIZE  = %lu",gasnetc_max_lapi_data_size);
	GASNETC_CONFIG_MSG(to_stderr,buf);
    }

    if (sizeof(gasnetc_token_t) > gasnetc_max_lapi_uhdr_size) {
	gasneti_fatalerror("gasnetc_token_t is %d bytes > max lapi uhdr %d",
			   (int)sizeof(gasnetc_token_t),gasnetc_max_lapi_uhdr_size);
    }
    if (gasnetc_max_lapi_uhdr_size > GASNETC_TOKEN_SIZE) {
	/* we pack data into uhdr, which is of fixed size */
	gasnetc_max_lapi_uhdr_size = GASNETC_TOKEN_SIZE;
    }
    if (gasnetc_max_lapi_data_size < GASNETC_AM_MAX_LONG) {
	gasneti_fatalerror("Must recompile with GASNETC_AM_MAX_LONG <= %lu",
			   gasnetc_max_lapi_data_size);
    }

    /* Do we want to use polling or interrupt mode?  How to
     * communicate this?  Env variable?
     */
    {
	char *mode = gasneti_getenv_withdefault("GASNET_LAPI_MODE", _STRINGIFY(GASNET_LAPI_MODE_DEFAULT));
        char tmp[255];
        int i;
        strncpy(tmp, mode, 255);
        tmp[254] = '\0';
        for (i=0; i < strlen(tmp); i++) tmp[i] = toupper(tmp[i]);

	if (!strcmp(mode,"P") || !strcmp(mode,"POLLING")) {
	    gasnetc_lapi_default_mode = gasnetc_Polling;
        } else if (!strcmp(mode,"I") || !strcmp(mode,"INTERRUPT")) {
	    gasnetc_lapi_default_mode = gasnetc_Interrupt;
        } else 
          gasneti_fatalerror("If set, environment variable GASNET_LAPI_MODE must be 'INTERRUPT' or 'POLLING'");
    }
    if (gasnetc_lapi_default_mode == gasnetc_Interrupt) {
	/* turn on interrupt mode */
	GASNETC_LCHECK(LAPI_Senv(gasnetc_lapi_context, INTERRUPT_SET, 1));
    } else {
	/* polling mode, turn off interrupts */
	GASNETC_LCHECK(LAPI_Senv(gasnetc_lapi_context, INTERRUPT_SET, 0));
    }
 
    #if GASNET_NDEBUG
      GASNETC_LCHECK(LAPI_Senv(gasnetc_lapi_context, ERROR_CHK, 0));   /* Turn error checking off */
    #endif

    /* collect remote addresses of header handler function */
    gasnetc_remote_req_hh = (void**)gasneti_malloc(num_tasks*sizeof(void*));
    gasnetc_remote_reply_hh = (void**)gasneti_malloc(num_tasks*sizeof(void*));
    GASNETC_LCHECK(LAPI_Address_init(gasnetc_lapi_context,
				     (void*)&gasnetc_lapi_AMreq_hh,
				     gasnetc_remote_req_hh));
    GASNETC_LCHECK(LAPI_Address_init(gasnetc_lapi_context,
				     (void*)&gasnetc_lapi_AMreply_hh,
				     gasnetc_remote_reply_hh));
    gasnetc_remote_amexit_hh = (void**)gasneti_malloc(num_tasks*sizeof(void*));
    GASNETC_LCHECK(LAPI_Address_init(gasnetc_lapi_context,
				     (void*)&gasnetc_amexit_hh,
				     gasnetc_remote_amexit_hh));
    
#if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
	    gasneti_mynode, gasneti_nodes); fflush(stderr);
#endif

    /* Construct nodemap using LAPI_Address_init() rather than gasnetc_lapi_exchange() */
    {   void **tmp = (void**)gasneti_malloc(num_tasks*sizeof(void*));
        void *myid = (void*)(uintptr_t)gethostid();
        GASNETC_LCHECK(LAPI_Address_init(gasnetc_lapi_context, myid, tmp));
        gasneti_nodemapInit(NULL, tmp, sizeof(void*), sizeof(void*));
        gasneti_free(tmp);
    }

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    { 
	/* Add code here to determine optimistic maximum segment size and
	 * the MIN(MaxLocalSegmentSize) over all nodes 
	gasneti_MaxLocalSegmentSize = ###;
	gasneti_MaxGlobalSegmentSize = ###;
	 * - OR -
	 * it may be appropriate to use gasneti_segmentInit() here to set 
	   gasneti_MaxLocalSegmentSize and gasneti_MaxGlobalSegmentSize,
	   if your conduit can use memory anywhere in the address space
	   (you may want to tune GASNETI_MMAP_MAX_SIZE to limit the max size)

           it may also be appropriate to first call gasneti_mmapLimit() to
           account for limitations imposed by having multiple GASNet nodes
           per shared-memory compute node
	*/
	/* On the SP, mmaped regions are allocated in segments distinct from
	 * static, stack and heap data.  gasneti_segmentInit should work
	 * well.
	 */
	gasneti_segmentInit((uintptr_t)-1,gasnetc_lapi_exchange);
    }
#elif GASNET_SEGMENT_EVERYTHING
    /* segment is everything - nothing to do */
#else
#error Bad segment config
#endif

    /* set up atexit handler to call gasnet_exit in the
     * case where a program returns from main without
     * calling gasnet_exit()
     */
#if HAVE_ON_EXIT
    on_exit(gasnetc_on_exit, NULL);
#else
    atexit(gasnetc_atexit);
#endif

#if 0
    /* Done earlier to allow tracing */
    gasneti_init_done = 1;  
#endif

    gasneti_auxseg_init(); /* adjust max seg values based on auxseg */
   
    if(gasneti_mynode == 0) {
      char *mp_task_affinity = gasneti_getenv("MP_TASK_AFFINITY");
      char *memory_affinity = gasneti_getenv("MEMORY_AFFINITY");
      if((mp_task_affinity != NULL) && (strcmp(mp_task_affinity,"-1") != 0)) {
        fprintf(stderr,"WARNING: The environment variable MP_TASK_AFFINITY is set (value = %s).  This has the potential for serious performance degradation.\n", mp_task_affinity);
        fflush(stderr);
      }

      if((memory_affinity != NULL) && (strcmp(memory_affinity,"-1") != 0)) {
        fprintf(stderr,"WARNING: The environment variable MEMORY_AFFINITY is set (value = %s).  This has the potential for serious performance degradation.\n", memory_affinity);
        fflush(stderr);
      }
    }

    return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
extern int gasnet_init(int *argc, char ***argv) {
    int retval = gasnetc_init(argc, argv);
    if (retval != GASNET_OK) GASNETI_RETURN(retval);
#if 0
    /* Already done in gasnetc_init() to allow tracing of init steps */
    gasneti_trace_init(argc, argv);
#endif

    return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
static char checkuniqhandler[256] = { 0 };
static int gasnetc_reghandlers(gasnet_handlerentry_t *table, int numentries,
                               int lowlimit, int highlimit,
                               int dontcare, int *numregistered) {
    int i;
    *numregistered = 0;
    for (i = 0; i < numentries; i++) {
	int newindex;

        if ((table[i].index == 0 && !dontcare) || 
            (table[i].index && dontcare)) continue;
	else if (table[i].index) newindex = table[i].index;
	else { /* deterministic assignment of dontcare indexes */
	    for (newindex = lowlimit; newindex <= highlimit; newindex++) {
		if (!checkuniqhandler[newindex]) break;
	    }
	    if (newindex > highlimit) {
		char s[255];
		sprintf(s,"Too many handlers. (limit=%i)", highlimit - lowlimit + 1);
		GASNETI_RETURN_ERRR(BAD_ARG, s);
	    }
	}

	/*  ensure handlers fall into the proper range of pre-assigned values */
	if (newindex < lowlimit || newindex > highlimit) {
	    char s[255];
	    sprintf(s, "handler index (%i) out of range [%i..%i]", newindex, lowlimit, highlimit);
	    GASNETI_RETURN_ERRR(BAD_ARG, s);
	}

	/* discover duplicates */
	if (checkuniqhandler[newindex] != 0) 
	    GASNETI_RETURN_ERRR(BAD_ARG, "handler index not unique");
	checkuniqhandler[newindex] = 1;

	/* register the handler */
	/* (###) add code here to register table[i].fnptr 
	   on index (gasnet_handler_t)newindex */
	gasnetc_handler[newindex] = table[i].fnptr;
	GASNETI_TRACE_PRINTF(C,("Registered handler "GASNETI_LADDRFMT" at index %d",
				GASNETI_LADDRSTR(table[i].fnptr),newindex));

	/* The check below for !table[i].index is redundant and present
	 * only to defeat the over-aggressive optimizer in pathcc 2.1
	 */
	if (dontcare && !table[i].index) table[i].index = newindex;

	(*numregistered)++;
    }
    return GASNET_OK;
}

#if GASNETC_LAPI_RDMA

int gasnetc_use_firehose = 0;

void gasnetc_lapi_get_remote_contexts(void)
{
  int i,j;
  int rctxts_per_node;

  /* Too verbose? */
  rctxts_per_node = (int) gasneti_getenv_int_withdefault("GASNET_LAPI_RCTXTS_PER_NODE",4,0);
  if (!GASNETI_POWEROFTWO(rctxts_per_node)) {
    gasneti_fatalerror("If set, GASNET_LAPI_RCTXTS_PER_NODE must be a power of two (is %d)", rctxts_per_node);
  }
  gasnetc_rctxts_per_node_mask = (rctxts_per_node - 1);

  /* Get rCtxts, the connections to remote nodes */
  gasnetc_remote_ctxts = gasneti_malloc(gasneti_nodes*sizeof(lapi_remote_cxt_t *));
  gasnetc_lapi_current_rctxt = gasneti_malloc(gasneti_nodes*sizeof(gasneti_weakatomic_t));

  for(i=0;i < gasneti_nodes;i++) {
    gasneti_weakatomic_set(&gasnetc_lapi_current_rctxt[i], 0, 0);
    /* This will give an error if you try to get a remote context for yourself */
    gasnetc_remote_ctxts[i] = gasneti_calloc(rctxts_per_node,sizeof(lapi_remote_cxt_t));
    if(i != gasneti_mynode) {
      for(j=0;j < rctxts_per_node;j++) {
        gasnetc_remote_ctxts[i][j].Util_type = LAPI_REMOTE_RCXT;
        gasnetc_remote_ctxts[i][j].operation = LAPI_RDMA_ACQUIRE;
        gasnetc_remote_ctxts[i][j].dest = i;
        GASNETC_LCHECK(LAPI_Util(gasnetc_lapi_context, (lapi_util_t *) (&(gasnetc_remote_ctxts[i][j]))));
        GASNETI_TRACE_PRINTF(C,("node %d got rCtxt for node %d (number %d) (%d) (%ld)\n",gasneti_mynode,i,j,gasnetc_remote_ctxts[i][j].usr_rcxt,sizeof(lapi_user_cxt_t)));
      }
    }
  }
}

#define GASNETC_LAPI_RDMA_TEST_BUFFER_SIZE 1024
static char gasnetc_rdma_test_buffer[GASNETC_LAPI_RDMA_TEST_BUFFER_SIZE];
void gasnetc_lapi_test_pin(int rdma_declared_on)
{
  lapi_get_pvo_t test_pvo;
  lapi_long_t return_code;
  lapi_long_t *return_codes = NULL;
  int i;
  /* Try to pin */
  test_pvo.Util_type = LAPI_XLATE_ADDRESS;  
  test_pvo.length = GASNETC_LAPI_RDMA_TEST_BUFFER_SIZE;
  test_pvo.usr_pvo = 0;
  test_pvo.address = gasnetc_rdma_test_buffer;
  test_pvo.operation = LAPI_RDMA_ACQUIRE;
  return_code = LAPI_Util(gasnetc_lapi_context, (lapi_util_t *) &test_pvo);

  /* Unpin if successful */
  if(return_code == LAPI_SUCCESS) {
    test_pvo.operation = LAPI_RDMA_RELEASE;
    (void) LAPI_Util(gasnetc_lapi_context, (lapi_util_t *) &test_pvo);
  }

  /* Exchange results with your peers */
  return_codes = (lapi_long_t *) gasneti_malloc(gasneti_nodes*sizeof(lapi_long_t)); 
  GASNETC_LCHECK(LAPI_Address_init64(gasnetc_lapi_context, return_code, return_codes));

  /* P0 does the talking */
  for(i=0;i < gasneti_nodes;i++) {
    int rc = return_codes[i];
    const char *error_str = NULL;
    if(rc != LAPI_SUCCESS) {
      gasnetc_lapi_use_rdma = 0;

      switch(rc) {
         case(LAPI_ERR_UTIL_CMD): /* RDMA not supported on this system */
           error_str =  "RDMA is not supported on this version of the LAPI library.";
           break;
         case(LAPI_ERR_NO_RDMA_RESOURCE): /* No rdma resources.  May need -rdma_count 2 or something*/ 
           error_str = "conduit cannot initialize RDMA resources. Was -rdma_count n/MP_RDMA_COUNT passed to poe or rcxtblocks passed to LoadLeveler?";
           break;
         default:  /* Have no idea what just happened */
           error_str = "unknown error occured during RDMA initialization.";
      }   

      if(rdma_declared_on) {
        if(gasneti_mynode == 0) {
          gasneti_fatalerror("GASNET_LAPI_USE_RDMA set to yes, but %s",error_str);
        } else {
          /* Wait for the error */
          gasneti_fatalerror("exiting"); 
        }
      } else {
        if(gasneti_mynode == 0) {
          fprintf(stderr,"WARNING: Switching to non-RDMA conduit because %s\n",error_str);
          fflush(stderr);
        }
      }
      break;
    }
  }
  gasneti_free(return_codes);
}

#endif

/* ------------------------------------------------------------------------------------ */
extern int gasnetc_attach(gasnet_handlerentry_t *table, int numentries,
                          uintptr_t segsize, uintptr_t minheapoffset) {
    void *segbase = NULL;
    int numreg;
 
    GASNETI_TRACE_PRINTF(C,("gasnetc_attach(table (%i entries), segsize=%lu, minheapoffset=%lu)",
                            numentries, (unsigned long)segsize, (unsigned long)minheapoffset));

    if (!gasneti_init_done) 
	GASNETI_RETURN_ERRR(NOT_INIT, "GASNet attach called before init");
    if (gasneti_attach_done) 
	GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already attached");

    /*  check argument sanity */
#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    if ((segsize % GASNET_PAGESIZE) != 0) 
	GASNETI_RETURN_ERRR(BAD_ARG, "segsize not page-aligned");
    if (segsize > gasneti_MaxLocalSegmentSize) 
	GASNETI_RETURN_ERRR(BAD_ARG, "segsize too large");
    if ((minheapoffset % GASNET_PAGESIZE) != 0) /* round up the minheapoffset to page sz */
	minheapoffset = ((minheapoffset / GASNET_PAGESIZE) + 1) * GASNET_PAGESIZE;
#else
    segsize = 0;
    minheapoffset = 0;
#endif

    segsize = gasneti_auxseg_preattach(segsize); /* adjust segsize for auxseg reqts */

    /* ------------------------------------------------------------------------------------ */
    /*  register handlers */
    { int i;
      for (i = 0; i < GASNETC_MAX_NUMHANDLERS; i++) 
        gasnetc_handler[i] = (gasneti_handler_fn_t)&gasneti_defaultAMHandler;
    }
    { /*  core API handlers */
	gasnet_handlerentry_t *ctable = (gasnet_handlerentry_t *)gasnetc_get_handlertable();
	int len = 0;
	numreg = 0;
	gasneti_assert(ctable);
	while (ctable[len].fnptr) len++; /* calc len */
	if (gasnetc_reghandlers(ctable, len, 1, 63, 0, &numreg) != GASNET_OK)
	    GASNETI_RETURN_ERRR(RESOURCE,"Error registering core API handlers");
	gasneti_assert(numreg == len);
    }

    { /*  extended API handlers */
	gasnet_handlerentry_t *etable = (gasnet_handlerentry_t *)gasnete_get_handlertable();
	int len = 0;
	numreg = 0;
	gasneti_assert(etable);
	while (etable[len].fnptr) len++; /* calc len */
	if (gasnetc_reghandlers(etable, len, 64, 127, 0, &numreg) != GASNET_OK)
	    GASNETI_RETURN_ERRR(RESOURCE,"Error registering extended API handlers");
	gasneti_assert(numreg == len);
    }

#if GASNETC_LAPI_RDMA
    gasnetc_lapi_use_rdma = (int) gasneti_getenv_yesno_withdefault("GASNET_LAPI_USE_RDMA",1);
    if(gasnetc_lapi_use_rdma) {
      gasnetc_lapi_test_pin(gasneti_getenv("GASNET_LAPI_USE_RDMA") != NULL);
    }
#endif

#if GASNETC_LAPI_RDMA && GASNET_SEGMENT_EVERYTHING
      /* Register handlers and such ... 
         Copied from vapi conduit */
   if_pt(gasnetc_lapi_use_rdma) { /* firehose handlers */
      gasnet_handlerentry_t *ftable = (gasnet_handlerentry_t *)firehose_get_handlertable();
      int len = 0;
      int base = 64 + numreg;
      int numreg;
      gasneti_assert(ftable);
      while (ftable[len].fnptr) len++; /* calc len */
      if (gasnetc_reghandlers(ftable, len, base, 127, 1, &numreg) != GASNET_OK)
        GASNETI_RETURN_ERRR(RESOURCE, "Error registering firehose handlers");
      gasneti_assert(numreg == len);
   }
#endif
    if (table) { /*  client handlers */
	int numreg1 = 0;
	int numreg2 = 0;

	/*  first pass - assign all fixed-index handlers */
	if (gasnetc_reghandlers(table, numentries, 128, 255, 0, &numreg1) != GASNET_OK)
	    GASNETI_RETURN_ERRR(RESOURCE,"Error registering fixed-index client handlers");

	/*  second pass - fill in dontcare-index handlers */
	if (gasnetc_reghandlers(table, numentries, 128, 255, 1, &numreg2) != GASNET_OK)
	    GASNETI_RETURN_ERRR(RESOURCE,"Error registering fixed-index client handlers");

	gasneti_assert(numreg1 + numreg2 == numentries);
    }

    /* ------------------------------------------------------------------------------------ */
    /*  register fatal signal handlers */

    /* catch fatal signals and convert to SIGQUIT */
    gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

    /*  (###) register any custom signal handlers required by your conduit 
     *        (e.g. to support interrupt-based messaging)
     */
    
    /* ------------------------------------------------------------------------------------ */
    /*  register segment  */

    gasneti_seginfo = (gasnet_seginfo_t *)gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));

#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
      {
        int64_t user_pin_threshold = gasneti_getenv_int_withdefault("GASNET_LAPI_PIN_THRESHOLD", gasnete_pin_threshold, 1);
        if(user_pin_threshold < 0 || user_pin_threshold > gasnete_pin_max) {
          if(gasneti_mynode == 0) {
            fprintf(stderr,"WARNING: GASNET_LAPI_PIN_THRESHOLD must be greater than 0 and less than or equal to %d.  Using default (%d bytes)\n",gasnete_pin_max, gasnete_pin_threshold);
            fflush(stderr);
          }
        } else {
          gasnete_pin_threshold = user_pin_threshold;
        }
        GASNETI_TRACE_PRINTF(C,("GASNET_LAPI_PIN_THRESHOLD is %ld",(long)gasnete_pin_threshold));
      }
#if GASNET_SEGMENT_EVERYTHING
    /* Always use firehose for segment everything, else there's no point */
    gasnetc_use_firehose = 1;
#else    
    gasnetc_use_firehose = (int) gasneti_getenv_yesno_withdefault("GASNET_LAPI_USE_FIREHOSE",1);
#endif /* GASNET_SEGMENT_EVERYTHING */
    if(gasnetc_use_firehose) {
      uint32_t flags = 0;
      size_t max_regions = GASNETC_FH_MAXREGIONS;
      /* uintptr_t max_pinnable_memory = GASNETC_FH_MAXREGION_SIZE*GASNETC_FH_MAXREGIONS; */
#if GASNET_SEGMENT_EVERYTHING
#else
      flags = FIREHOSE_INIT_FLAG_LOCAL_ONLY;
#endif
      firehose_init(GASNETC_FH_MAX_PINNABLE, max_regions, GASNETC_FH_MAXREGION_SIZE, NULL, 0, flags, &gasnetc_firehose_info);
    }
    }
#endif /* GANSETC_LAPI_RDMA */

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    if (segsize == 0) segbase = NULL; /* no segment */
    else {
	/* (###) add code here to choose and register a segment 
	   (ensuring alignment across all nodes if this conduit sets GASNET_ALIGNED_SEGMENTS==1) 
	   you can use gasneti_segmentAttach() here if you used gasneti_segmentInit() above
	*/
	gasneti_assert(segsize % GASNET_PAGESIZE == 0);
	gasneti_segmentAttach(segsize,minheapoffset,gasneti_seginfo,gasnetc_lapi_exchange);
	segbase = gasneti_seginfo[gasneti_mynode].addr;
	segsize = gasneti_seginfo[gasneti_mynode].size;
	gasneti_assert(((uintptr_t)segbase) % GASNET_PAGESIZE == 0);
	gasneti_assert(segsize % GASNET_PAGESIZE == 0);
    }
#if GASNETC_LAPI_RDMA
    gasnetc_my_segbase = (lapi_long_t)segbase;
    gasnetc_my_segtop = (lapi_long_t)segbase + (segsize - 1);
    if_pt(gasnetc_lapi_use_rdma) {
	/* 
	 * For LAPI RDMA attempt to pin, i.e. get PVOs 
	 * (Protocol Virtual Offsets) for
	 * the region. */
	 {
	 int *pvo_count;
	 lapi_user_pvo_t *tmp_pvo_ptr;
	 int my_num_pvos;
	 int total_num_pvos;
         int i=0;
         uintptr_t tmp_offset = 0;
         if (segbase) { /* bug 2176: warn if segment is not large page */
           size_t large_pagesz = sysconf(_SC_LARGE_PAGESIZE);
           size_t segment_pagesz = gasnetc_get_pagesize(segbase);
           if (segment_pagesz != large_pagesz && !gasneti_getenv_yesno_withdefault("GASNET_QUIET",0)) {
             fprintf(stderr,"WARNING: node %i is not using large pages for the GASNet segment "
                            "(segment_pagesz=%ld large_pagesz=%ld). "
                            "This may lead to stability problems for LAPI-RDMA. "
                            "Try setting LDR_CNTRL=LARGE_PAGE_DATA=Y\n",
                            gasneti_mynode, (long)segment_pagesz, (long)large_pagesz);
             fflush(stderr);
           }
         }
	 /* Break up the segment */
	 my_num_pvos = (segsize + (GASNETC_LAPI_PVO_EXTENT-1)) >> GASNETC_LAPI_PVO_EXTENT_BITS;
         GASNETI_TRACE_PRINTF(C,("num_pvos = %d pvo_extent = %ld",my_num_pvos,GASNETC_LAPI_PVO_EXTENT));
	 gasnetc_node_pvo_list = gasneti_calloc(my_num_pvos,sizeof(lapi_get_pvo_t));

	 while(tmp_offset != segsize) {
	 	/* Attempt to get a PVO for this section */
	 	uintptr_t len = MIN(segsize - tmp_offset, GASNETC_LAPI_PVO_EXTENT);
	 	gasnetc_node_pvo_list[i].Util_type = LAPI_XLATE_ADDRESS;
	 	gasnetc_node_pvo_list[i].length = len;
	 	gasnetc_node_pvo_list[i].usr_pvo = 0;
	 	gasnetc_node_pvo_list[i].address = (void *) (((lapi_long_t) segbase) + tmp_offset);
	 	gasnetc_node_pvo_list[i].operation = LAPI_RDMA_ACQUIRE;									
	 	GASNETC_LCHECK(LAPI_Util(gasnetc_lapi_context, (lapi_util_t *) (&(gasnetc_node_pvo_list[i]))));
                GASNETI_TRACE_PRINTF(C,("pvo[i=%d]: usr_pvo=0x%lx length=0x%lx address=%p",i,(ulong) gasnetc_node_pvo_list[i].usr_pvo,len,gasnetc_node_pvo_list[i].address));
	 	tmp_offset += len;
	 	i++;
	 }
	 
    /* Exchange PVOs with everybody else so that given a
     * (node, offset) pair, a remode node can find the
     * corresponding PVO
     */
    gasnetc_num_pvos = my_num_pvos;
    total_num_pvos = 0;
    pvo_count = gasneti_malloc(gasneti_nodes * sizeof(int));
    for (i=0;i < gasneti_nodes;i++) {
	int tmp = ( gasneti_seginfo[i].size + (GASNETC_LAPI_PVO_EXTENT-1)) >> GASNETC_LAPI_PVO_EXTENT_BITS;
	pvo_count[i] = tmp;
	if (tmp > gasnetc_num_pvos) gasnetc_num_pvos = tmp;
	total_num_pvos += tmp;
    }
    gasneti_assert(my_num_pvos == pvo_count[gasneti_mynode]);
	 
    /*
     * Allocate the pvo table, indexed [node][pvo]
     * Actual storage is a single contiguous array with total_num_pvos elements
     */
    
    tmp_pvo_ptr = gasneti_malloc(total_num_pvos * sizeof(lapi_user_pvo_t));
    gasnetc_pvo_table = (lapi_user_pvo_t **) gasneti_malloc(gasneti_nodes * sizeof(lapi_user_pvo_t *));
    for(i=0;i < gasneti_nodes;i++) {
      gasnetc_pvo_table[i] = tmp_pvo_ptr;
      tmp_pvo_ptr += pvo_count[i];
    }
    
    /* Exchange, and "transpose" to construct the pvo table
     */
	  
    gasneti_assert(sizeof(lapi_user_pvo_t) <= sizeof(lapi_long_t));
    tmp_pvo_ptr = gasneti_malloc(gasneti_nodes * sizeof(lapi_user_pvo_t));
    for(i=0;i < gasnetc_num_pvos;i++) {
      int j;
      lapi_long_t tmp_long = (i < my_num_pvos) ? (lapi_long_t) (gasnetc_node_pvo_list[i].usr_pvo) : 0;
      GASNETC_LCHECK(LAPI_Address_init64(gasnetc_lapi_context, tmp_long, tmp_pvo_ptr));
      for (j=0;j < gasneti_nodes;j++) {
        if (i < pvo_count[j]) {
          gasnetc_pvo_table[j][i] = tmp_pvo_ptr[j];
          GASNETI_TRACE_PRINTF(C,("gasnetc_pvo_table[node=%d][idx=%d] = 0x%lx",j,i,(ulong)gasnetc_pvo_table[j][i]));
        } else {
          gasneti_assert(!tmp_pvo_ptr[j]);
        }
      }
    }		
    gasneti_free(tmp_pvo_ptr);
    gasneti_free(pvo_count);

    GASNETC_LCHECK(LAPI_Gfence(gasnetc_lapi_context));

    gasnetc_lapi_get_remote_contexts();

    GASNETC_LCHECK(LAPI_Gfence(gasnetc_lapi_context));
	  
    GASNETI_TRACE_PRINTF(C,("gasnetc_attach: %d exchanging base addresses\n",gasneti_mynode));
    /* Finally, exchange the base addresses */
    gasnetc_segbase_table = gasneti_malloc(gasneti_nodes*sizeof(lapi_long_t));
    GASNETC_LCHECK(LAPI_Address_init64(gasnetc_lapi_context, (lapi_long_t) segbase, gasnetc_segbase_table));
    
    /* Finally, really, set up the bounce buffers */
    GASNETI_TRACE_PRINTF(C,("gasnetc_attach: %d bounce buffer setup\n",gasneti_mynode));
    gasnete_lapi_setup_nb();

    GASNETI_TRACE_PRINTF(C,("gasnetc_attach: %d init done\n",gasneti_mynode));
    /* Make sure we're all done */
    GASNETC_LCHECK(LAPI_Gfence(gasnetc_lapi_context));
    gasnetc_lapi_rdma_initialized = 1;
    }
    }
#endif /* GASNETC_LAPI_RDMA */
#else
    /* GASNET_SEGMENT_EVERYTHING */
#if GASNETC_LAPI_RDMA
    if_pt(gasnetc_lapi_use_rdma) {
    /* Segment everything setup */
    gasnetc_lapi_get_remote_contexts();
    GASNETC_LCHECK(LAPI_Gfence(gasnetc_lapi_context));
    }
#endif /* GASNETC_LAPI_RDMA */
    {
	int i;
	for (i=0;i<gasneti_nodes;i++) {
	    gasneti_seginfo[i].addr = (void *)0;
	    gasneti_seginfo[i].size = (uintptr_t)-1;
	}
	segbase = gasneti_seginfo[gasneti_mynode].addr;
	segsize = gasneti_seginfo[gasneti_mynode].size;
    }
#endif

    /* ------------------------------------------------------------------------------------ */
    /*  primary attach complete */
    gasneti_attach_done = 1;
    gasnetc_bootstrapBarrier();

    GASNETI_TRACE_PRINTF(C,("gasnetc_attach(): primary attach complete. GASNET_LAPI_MODE=%s",
          (gasnetc_lapi_default_mode == gasnetc_Polling ? "POLLING" : "INTERRUPT")));

    {
	int i;
	for (i = 0; i < gasneti_nodes; i++) {
	    GASNETI_TRACE_PRINTF(C,("For node %d seginfo.addr = "GASNETI_LADDRFMT" seginfo.size = %lu",
				    i,GASNETI_LADDRSTR(gasneti_seginfo[i].addr),
				    (unsigned long)gasneti_seginfo[i].size));
	}
    }

    gasneti_auxseg_attach(); /* provide auxseg */

    gasnete_init(); /* init the extended API */

    gasneti_nodemapFini();

    /* ensure extended API is initialized across nodes */
    gasnetc_bootstrapBarrier();

    return GASNET_OK;
}
    
#if GASNETC_LAPI_RDMA
void gasnetc_lapi_free(void)
{
#if 0
  int i;

  for(i=0;i < gasnetc_num_pvos;i++) {
   lapi_get_pvo_t new_pvo;
   new_pvo.Util_type = LAPI_XLATE_ADDRESS;
   new_pvo.length = 0;
   new_pvo.usr_pvo = gasnetc_node_pvo_list[i].usr_pvo;
   new_pvo.address = 0;
   new_pvo.operation = LAPI_RDMA_RELEASE;
   GASNETC_LCHECK(LAPI_Util(gasnetc_lapi_context, (lapi_util_t *) &new_pvo));
  }
#endif
  gasneti_free(gasnetc_node_pvo_list);
  gasneti_free(gasnetc_pvo_table[0]); /* The dense array of pvos */
  gasneti_free(gasnetc_pvo_table);
  gasneti_free(gasnetc_segbase_table);
  
#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
  /* Free and unpin the network buffers */
  gasnete_lapi_free_nb();
#endif
}
#endif
/* ------------------------------------------------------------------------------------ */
/* All the common cleanup tasks between the various exit routines */
static void gasnetc_exit_cleanup(void) {
    GASNETI_TRACE_PRINTF(C,("GASNETC_EXIT: UHDR_BUF HWM %d, numfree %d, numalloc %d",
			    gasnetc_uhdr_freelist.high_water_mark,
			    gasnetc_uhdr_freelist.numfree,
			    gasnetc_uhdr_freelist.numalloc));
#if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"GASNETC_EXIT: NODE %d UHDR_BUF HWM %d, numfree %d, numalloc %d\n",
	    gasneti_mynode,
	    gasnetc_uhdr_freelist.high_water_mark,
	    gasnetc_uhdr_freelist.numfree,
	    gasnetc_uhdr_freelist.numalloc);
#endif

#if GASNETC_LAPI_RDMA
    if(gasnetc_lapi_rdma_initialized) {
      gasnetc_lapi_free();
    }
#endif
    gasneti_flush_streams();
    gasneti_trace_finish();
}
/* ------------------------------------------------------------------------------------ */
static int amexit_exitcode = 0;
void gasnetc_sigalarm_handler(int sig)
{
#if GASNETC_VERBOSE_EXIT
    fprintf(stderr,">> GASNET_SIGALARM_HNDLR[%d]: called\n",gasneti_mynode);
    fflush(stderr);
#endif
    gasneti_reghandler(SIGQUIT, SIG_DFL);
    kill(getpid(),SIGQUIT);
}

void *gasnetc_run_shutdown(void *arg)
{
    /* detach so no-one has to wait for us */
    pthread_detach(pthread_self());
    /* we were created to die */
    gasnet_exit(amexit_exitcode);
    /* NOT REACHED */
    return NULL;
}

void* gasnetc_amexit_hh(lapi_handle_t *context, void *uhdr, uint *uhdr_len,
			ulong *msg_len, compl_hndlr_t **comp_h, void **uinfo)
{
    *comp_h = gasnetc_amexit_ch;
    *uinfo = NULL;
    amexit_exitcode = *(int*)uhdr;
    return NULL;
}

void gasnetc_amexit_ch(lapi_handle_t *context, void *uinfo)
{
    if (gasnetc_got_exit_signal) {
	/* Ignore... in process of exiting */
	return;
    }
    gasnetc_got_exit_signal = 1;

    /* spawn another pthread for shutdown.  Cant run LAPI_Term in
     * the completion thread.  Dont have to wait for this thread
     * to terminate, it will detach itself.
     */
    {
	pthread_t shutdown_thread;
	pthread_create(&shutdown_thread, NULL, gasnetc_run_shutdown, NULL);
    }
}

#if HAVE_ON_EXIT
static
void gasnetc_on_exit(int exitcode, void *arg)
{
    if (gasnetc_called_exit)
	return;
    gasnetc_exit(exitcode);
}
#else
static
void gasnetc_atexit(void)
{
    if (gasnetc_called_exit)
	return;
    gasnetc_exit(0);
}
#endif
    
extern void gasnetc_exit(int exitcode) {
    double sleep_time;

    gasnetc_called_exit = 1;
    
    /* ARBITRARY args:  MIN(120s, 5s + nodes*0.05s), error if user ask for less than 2s */
    sleep_time = gasneti_get_exittimeout(120., 5., 0.05, 2.);

    /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
    gasneti_reghandler(SIGQUIT, SIG_IGN);
    gasneti_reghandler(SIGUSR1, SIG_IGN);
    gasneti_reghandler(SIGTERM, SIG_IGN);

    {  /* ensure only one thread ever continues past this point */
      static gasneti_mutex_t exit_lock = GASNETI_MUTEX_INITIALIZER;
      gasneti_mutex_lock(&exit_lock);
    }

#if GASNETC_VERBOSE_EXIT
    fprintf(stderr,">> GASNET_EXIT[%d]: reset sigquit,sigusr1,sigterm\n",gasneti_mynode);
    fprintf(stderr,">> GASNET_EXIT[%d]: setting %g second alarm\n",gasneti_mynode, sleep_time);
    fflush(stderr);
#endif

    gasnetc_exit_cleanup();
    /* sleep a variable amount of time, attempting to avoid everyone
     * spraying AM messages to each other at once.
     */
    usleep((gasneti_mynode % 10) * 100000 );

    /* Experience has shown that using AMs in error cases may hang
     * in many situations.  Register an alarm handler here which
     * will forcefully kill the task if called.
     */
    gasneti_reghandler(SIGALRM, gasnetc_sigalarm_handler );
    /* set the alarm for avoid hangs... 
     */
    alarm((int)sleep_time);

    if (gasnetc_got_exit_signal) {
	/* async exit, got shutdown AM from remote node */
#if GASNETC_VERBOSE_EXIT
	fprintf(stderr,">> GASNET_EXIT[%d]: async exit\n",gasneti_mynode);
	fflush(stderr);
#endif
	
    } else {
	/* synchronous exit */
	/* We are the first task to enter gasnet_exit... Send AM to all others */

	gasnet_node_t node;
	lapi_cntr_t cntr;

	/* Set gasnetc_got_exit_signal locally */
	gasnetc_got_exit_signal = 1;

#if GASNETC_VERBOSE_EXIT
	fprintf(stderr,">> GASNET_EXIT[%d]: Sending exit AM to all others\n",gasneti_mynode);
	fflush(stderr);
#endif
	LAPI_Setcntr(gasnetc_lapi_context,&cntr,0);
	for (node=0; node < gasneti_nodes; node++) {
	    if (node == gasneti_mynode) continue;
	    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, node,
			       gasnetc_remote_amexit_hh[node],
			       (void*)&exitcode, sizeof(exitcode), NULL, 0,
			       NULL, &cntr, NULL));
	}

	/* wait for local completion so arg to Amsend does not go out of scope */
	GASNETC_WAITCNTR(&cntr,gasneti_nodes-1,NULL);
    	    
#if GASNETC_VERBOSE_EXIT
	fprintf(stderr,">> GASNET_EXIT[%d]: Finished sending exit AMs\n",gasneti_mynode);
	fflush(stderr);
#endif
    }

#if GASNETC_VERBOSE_EXIT
    fprintf(stderr,">> GASNET_EXIT[%d]: Start LAPI_Gfence\n",gasneti_mynode);
    fflush(stderr);
#endif
    LAPI_Gfence(gasnetc_lapi_context);
#if GASNETC_VERBOSE_EXIT
    fprintf(stderr,">> GASNET_EXIT[%d]: Ignore SIGILL and start LAPI_Term\n",gasneti_mynode);
    fflush(stderr);
#endif
    /* Ignore SIGILL caused by inter-thread interacions in LAPI_Term() (bug 2362) */
    gasneti_reghandler(SIGILL, SIG_IGN);
    LAPI_Term(gasnetc_lapi_context);
#if GASNETC_VERBOSE_EXIT
    fprintf(stderr,">> GASNET_EXIT[%d]: cancel alarm and exit\n",gasneti_mynode);
    fflush(stderr);
#endif
    /* cancel previous alarm and exit normally */
    alarm(0);

    gasneti_killmyprocess(exitcode);

    /* should never get here */
    gasneti_fatalerror("gasneti_killmyprocess failed to kill the process!");
}

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/
extern int gasnetc_AMGetMsgSource(gasnet_token_t token, gasnet_node_t *srcindex) {
    gasnet_node_t sourceid;
    GASNETI_CHECKATTACH();
    GASNETI_CHECK_ERRR((!token),BAD_ARG,"bad token");
    GASNETI_CHECK_ERRR((!srcindex),BAD_ARG,"bad src ptr");

    /* (###) add code here to write the source index into sourceid */
    sourceid = ((gasnetc_token_t*)token)->msg.sourceId;

    gasneti_assert(sourceid < gasneti_nodes);
    *srcindex = sourceid;
    return GASNET_OK;
}

/* Run up to max_count request handlers (essentially unbounded if max_count==0) */
GASNETI_INLINE(gasnetc_service_req_q)
void gasnetc_service_req_q(unsigned int max_count, int update_schedule) {
    gasnetc_token_t *q_token = NULL;
    unsigned int count = 0;

    while ( (q_token = gasnetc_token_dequeue(&gasnetc_req_q, update_schedule)) != NULL ) {
	gasnetc_run_handler(q_token);
	/* deallocate the token, it was allocated in the header handler */
	gasnetc_uhdr_free(q_token);
	++count;
	if (count == max_count) break; /* If max_count==0, will match only when count wraps */
    }
}

extern int gasnetc_AMPoll(void) {
    GASNETI_CHECKATTACH();

    /* Check if any request handlers are queued for processing
     */
    gasnetc_service_req_q(GASNETC_POLL_LIMIT, 0);

    /* NOTE: a call to probe is not needed when LAPI is executing
     * in interrupt mode.  In that mode, polling can sometimes
     * decrease performance due to lock contention between the
     * notification thread and this thread.
     * If this funciton is called from a spinloop, LAPI should
     * be switched to POLLING mode before the AMPoll calls,
     * and switched back to the default mode afterwards.
     * We do this in the BLOCKUNTIL macro.
     */
    GASNETC_LAPI_POLL(gasnetc_lapi_context);

    /* Check again if any request handlers are queued for processing
     */
    gasnetc_service_req_q(GASNETC_POLL_LIMIT, 0);
    
#if GASNETC_LAPI_RDMA
    if(gasnetc_use_firehose) {
      firehose_poll();
    }
#endif
    return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/
extern int gasnetc_AMRequestShortM( 
    gasnet_node_t dest,       /* destination node */
    gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
    int numargs, ...) {
    int retval;
    lapi_cntr_t o_cntr;
    int cur_cntr, token_len, i;
    char raw_token[GASNETC_TOKEN_SIZE + GASNETC_DOUBLEWORD];
    gasnetc_token_t *token;
    gasnetc_msg_t  *msg;
    va_list argptr;

    GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs);

    token = (gasnetc_token_t*)GASNETC_ALIGN_PTR(&raw_token[0]);
    msg = &token->msg;

    msg->handlerId = handler;
    msg->sourceId = gasneti_mynode;
    GASNETC_MSG_SETFLAGS(msg,1,gasnetc_Short,0,numargs);
    msg->destLoc = (uintptr_t)NULL;
    msg->dataLen = (size_t)0;
    msg->uhdrLoc = (uintptr_t)NULL;

    /* copy the arguments */
    va_start(argptr, numargs);
    for (i = 0; i < numargs; i++) {
	msg->args[i] = va_arg(argptr, gasnet_handlerarg_t);
    }
    va_end(argptr);

    GASNETC_GEN_CSUM(msg, numargs, NULL, 0);

    /* Do Loopback check here */
#if GASNETC_ENABLE_LOOPBACK
    if (dest == gasneti_mynode) {
	gasneti_handler_fn_t pfn = gasnetc_handler[handler];
	GASNETI_RUN_HANDLER_SHORT(1,handler,pfn,token,&msg->args[0],numargs);
	GASNETI_RETURN(GASNET_OK);
    }
#endif
    
    /* only send as many much of token structure as necessary */
    token_len = TOKEN_LEN(numargs);
    
    /* issue the request for remote execution of the user handler */ 
    gasneti_assert( token_len <= gasnetc_max_lapi_uhdr_size);
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&o_cntr,0));
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, dest,
			       gasnetc_remote_req_hh[dest],
			       (void*)token, token_len, NULL, 0,
			       NULL, &o_cntr, NULL));
    gasneti_resume_spinpollers();
    
    /* wait for the Amsend call to complete locally */
    GASNETC_WAITCNTR(&o_cntr,1,&cur_cntr);

    retval = GASNET_OK;
    GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestMediumM( 
    gasnet_node_t dest,      /* destination node */
    gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
    void *source_addr, size_t nbytes,   /* data payload */
    int numargs, ...) {
    int retval;
    lapi_cntr_t o_cntr;
    int cur_cntr, token_len, i;
    char raw_token[GASNETC_TOKEN_SIZE + GASNETC_DOUBLEWORD];
    gasnetc_token_t *token;
    gasnetc_msg_t  *msg;
    void *udata_start = NULL;
    int udata_avail;
    int udata_packed = 0;
    va_list argptr;

    GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);

    token = (gasnetc_token_t*)GASNETC_ALIGN_PTR(&raw_token[0]);
    msg = &token->msg;

    msg->handlerId = handler;
    msg->sourceId = gasneti_mynode;
    GASNETC_MSG_SETFLAGS(msg,1,gasnetc_Medium,0,numargs);
    msg->destLoc = (uintptr_t)NULL;
    msg->dataLen = (size_t)nbytes;
    msg->uhdrLoc = (uintptr_t)NULL;

    /* copy the arguments */
    va_start(argptr, numargs);
    for (i = 0; i < numargs; i++) {
	msg->args[i] = va_arg(argptr, gasnet_handlerarg_t);
    }
    va_end(argptr);
    udata_start = (void*)&msg->args[numargs];
    token_len = TOKEN_LEN(numargs);
    udata_avail = gasnetc_max_lapi_uhdr_size - token_len - GASNETC_DOUBLEWORD;

    /* can we pack the data into the uhdr? */
    if (nbytes <= udata_avail) {
	memcpy(udata_start,source_addr,nbytes);
	token_len += nbytes;
	udata_packed = 1;
	GASNETC_MSG_SET_PACKED(msg);
    }

    GASNETC_GEN_CSUM(msg, numargs, source_addr, nbytes);

    /* Do Loopback check here */
#if GASNETC_ENABLE_LOOPBACK
    if (dest == gasneti_mynode) {
	gasneti_handler_fn_t pfn = gasnetc_handler[handler];
	void *destloc;
	if (udata_packed) {
	    destloc = udata_start;
	} else {
	    destloc = gasneti_malloc(nbytes > 0 ? nbytes : 1);
	    memcpy(destloc,source_addr,nbytes);
	}
	GASNETI_RUN_HANDLER_MEDIUM(1,handler,pfn,token,&msg->args[0],numargs,destloc,nbytes);
	if (! udata_packed) {
	    gasneti_free(destloc);
	}
	GASNETI_RETURN(GASNET_OK);
    }
#endif

    /* insure token_len is a multiple of 8 */
    token_len = GASNETC_ROUND_DOUBLEWORD(token_len);
    /* issue the request for remote execution of the user handler */
    gasneti_assert( token_len <= gasnetc_max_lapi_uhdr_size);
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&o_cntr,0));
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, dest,
			       gasnetc_remote_req_hh[dest],
			       (void*)token, token_len,
			       (udata_packed ? NULL : source_addr),
			       (udata_packed ? 0    : nbytes),
			       NULL, &o_cntr, NULL));
    gasneti_resume_spinpollers();
    
    /* wait for the Amsend call to complete locally */
    GASNETC_WAITCNTR(&o_cntr,1,&cur_cntr);

    retval = GASNET_OK;
    GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestLongM( gasnet_node_t dest,        /* destination node */
				   gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
				   void *source_addr, size_t nbytes,   /* data payload */
				   void *dest_addr,                    /* data destination on destination node */
				   int numargs, ...) {
    int retval;
    lapi_cntr_t o_cntr;
    int cur_cntr, token_len, i;
    char raw_token[GASNETC_TOKEN_SIZE + GASNETC_DOUBLEWORD];
    gasnetc_token_t *token;
    gasnetc_msg_t  *msg;
    void *udata_start = NULL;
    int udata_avail;
    int udata_packed = 0;
    va_list argptr;

    GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);

    token = (gasnetc_token_t*)GASNETC_ALIGN_PTR(&raw_token[0]);
    msg = &token->msg;

    msg->handlerId = handler;
    msg->sourceId = gasneti_mynode;
    GASNETC_MSG_SETFLAGS(msg,1,gasnetc_Long,0,numargs);
    msg->destLoc = (uintptr_t)dest_addr;
    msg->dataLen = (size_t)nbytes;
    msg->uhdrLoc = (uintptr_t)NULL;

    /* copy the arguments */
    va_start(argptr, numargs);
    for (i = 0; i < numargs; i++) {
	msg->args[i] = va_arg(argptr, gasnet_handlerarg_t);
    }
    va_end(argptr);
    udata_start = (void*)&msg->args[numargs];
    token_len = TOKEN_LEN(numargs);
    udata_avail = gasnetc_max_lapi_uhdr_size - token_len - GASNETC_DOUBLEWORD;

    GASNETC_GEN_CSUM(msg, numargs, source_addr, nbytes);

    /* Do Loopback check here */
#if GASNETC_ENABLE_LOOPBACK
    if (dest == gasneti_mynode) {
	gasneti_handler_fn_t pfn = gasnetc_handler[handler];
	/* must do local copy of data from source to dest */
	if_pt(dest_addr != source_addr) memcpy((char*)dest_addr,source_addr,nbytes);
	GASNETI_RUN_HANDLER_LONG(1,handler,pfn,token,&msg->args[0],numargs,dest_addr,nbytes);
	GASNETI_RETURN(GASNET_OK);
    }
#endif

    /* can we pack the data into the uhdr? */
    if (nbytes <= udata_avail) {
	memcpy(udata_start,source_addr,nbytes);
	token_len += nbytes;
	udata_packed = 1;
	GASNETC_MSG_SET_PACKED(msg);
    }

    /* issue the request for remote execution of the user handler */
    token_len = GASNETC_ROUND_DOUBLEWORD(token_len);
    gasneti_assert( token_len <= gasnetc_max_lapi_uhdr_size);
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&o_cntr,0));
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, dest,
			       gasnetc_remote_req_hh[dest],
			       (void*)token, token_len,
			       (udata_packed ? NULL : source_addr),
			       (udata_packed ? 0    : nbytes),
			       NULL, &o_cntr, NULL));
    gasneti_resume_spinpollers();
    
    /* wait for the Amsend call to complete locally */
    GASNETC_WAITCNTR(&o_cntr,1,&cur_cntr);

    retval = GASNET_OK;
    GASNETI_RETURN(retval);
}

#ifndef HAVE_BUG2582
#define HAVE_BUG2582 1
#endif 

extern int gasnetc_AMRequestLongAsyncM( gasnet_node_t dest,        /* destination node */
					gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
					void *source_addr, size_t nbytes,   /* data payload */
					void *dest_addr,                    /* data destination on destination node */
					int numargs, ...) {

    int token_len, i;
    gasnetc_token_t *token;
    gasnetc_msg_t  *msg;
    void *udata_start = NULL;
    int udata_avail;
    int udata_packed = 0;
    int retval;
#if HAVE_BUG2582
    char raw_token[GASNETC_TOKEN_SIZE + GASNETC_DOUBLEWORD];
#endif
    va_list argptr;

    GASNETI_COMMON_AMREQUESTLONGASYNC(dest,handler,source_addr,nbytes,dest_addr,numargs);

#if HAVE_BUG2582
    token = (gasnetc_token_t*)GASNETC_ALIGN_PTR(&raw_token[0]);
#else
    token = gasnetc_uhdr_alloc();
#endif
    msg = &token->msg;
    msg->handlerId = handler;
    msg->sourceId = gasneti_mynode;
    GASNETC_MSG_SETFLAGS(msg,1,gasnetc_AsyncLong,0,numargs);
    msg->destLoc = (uintptr_t)dest_addr;
    msg->dataLen = (size_t)nbytes;
    /* stash the location of token, so that it can be deallocated
     * in the completion handler of the corresponding GASNET reply
     * when it executes on this node.
     */
    msg->uhdrLoc = (uintptr_t)token;

    /* copy the arguments */
    va_start(argptr, numargs);
    for (i = 0; i < numargs; i++) {
	msg->args[i] = va_arg(argptr, gasnet_handlerarg_t);
    }
    va_end(argptr);
    udata_start = (void*)&msg->args[numargs];
    token_len = TOKEN_LEN(numargs);
    udata_avail = gasnetc_max_lapi_uhdr_size - token_len - GASNETC_DOUBLEWORD;

    GASNETC_GEN_CSUM(msg, numargs, source_addr, nbytes);

    /* Do Loopback check here */
#if GASNETC_ENABLE_LOOPBACK
    if (dest == gasneti_mynode) {
	gasneti_handler_fn_t pfn = gasnetc_handler[handler];
	/* must do local copy of data from source to dest */
	if_pt(dest_addr != source_addr) memcpy((char*)dest_addr,source_addr,nbytes);
	/* Note: we will deallocate the token below, just to be safe
	 * remove the address from the uhdrLoc field so that no-one
	 * else messes with it.
	 */
	msg->uhdrLoc = (uintptr_t)NULL;
	GASNETI_RUN_HANDLER_LONG(1,handler,pfn,token,&msg->args[0],numargs,dest_addr,nbytes);
	gasnetc_uhdr_free(token);
	GASNETI_RETURN(GASNET_OK);
    }
#endif

    /* can we pack the data into the uhdr? */
    if (nbytes <= udata_avail) {
	memcpy(udata_start,source_addr,nbytes);
	token_len += nbytes;
	udata_packed = 1;
	GASNETC_MSG_SET_PACKED(msg);
    }
    
    token_len = GASNETC_ROUND_DOUBLEWORD(token_len);
    gasneti_assert( token_len <= gasnetc_max_lapi_uhdr_size);

#if HAVE_BUG2582 /* Work around BUG 2582 - memory leak w/ LongAsync. */
  {
    lapi_cntr_t o_cntr;
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&o_cntr,0));
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, dest,
			       gasnetc_remote_req_hh[dest],
			       (void*)token, token_len,
			       (udata_packed ? NULL : source_addr),
			       (udata_packed ? 0    : nbytes),
			       NULL, &o_cntr, NULL));
    gasneti_resume_spinpollers();
    
    /* wait for the Amsend call to complete locally */
    GASNETC_WAITCNTR(&o_cntr,1,NULL);
  }
#else
    /* issue the request for remote execution of the user handler */
    /* NOTE: no LAPI counters are used here, the token will be deallocated
     * later (by the completion handler when the reply handler is executed).
     * It is up to the client not to modify the source_addr data until his 
     * reply handler runs.
     */
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, dest,
			       gasnetc_remote_req_hh[dest],
			       (void*)token, token_len,
			       (udata_packed ? NULL : source_addr),
			       (udata_packed ? 0    : nbytes),
			       NULL, NULL, NULL));
    
    gasneti_resume_spinpollers();
#endif

    retval = GASNET_OK;
    GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyShortM( 
    gasnet_token_t token,       /* token provided on handler entry */
    gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
    int numargs, ...) {
    int retval;

    gasnetc_msg_t *msg = &((gasnetc_token_t *)token)->msg;
    uint requester = (uint)msg->sourceId;
    lapi_cntr_t o_cntr;
    int token_len, i, cur_cntr;

    va_list argptr;
    GASNETI_COMMON_AMREPLYSHORT(token,handler,numargs);
    va_start(argptr, numargs); /*  pass in last argument */

    /* we can re-use the token passed into us.  It was allocated in the
     * LAPI header handler to be large enough to contain the maximum
     * number of arguments.  Upon return from this funciton it will
     * no longer be used
     */
    GASNETC_MSG_SETFLAGS(msg,0,gasnetc_Short,0,numargs);
    msg->handlerId = handler;
    msg->sourceId = gasneti_mynode;
    msg->destLoc = (uintptr_t)NULL;
    msg->dataLen = 0;
    /* do NOT modify the contents of uhdrLoc... needed at origin */
    va_start(argptr, numargs); /*  pass in last argument */
    for (i = 0; i < numargs; i++) {
	msg->args[i] = va_arg(argptr, gasnet_handlerarg_t);
    }
    va_end(argptr);

    GASNETC_GEN_CSUM(msg, numargs, NULL, 0);

#if GASNETC_ENABLE_LOOPBACK
    if (requester == gasneti_mynode) {
	gasneti_handler_fn_t pfn = gasnetc_handler[handler];
	GASNETI_RUN_HANDLER_SHORT(0,handler,pfn,token,&msg->args[0],numargs);
	GASNETI_RETURN(GASNET_OK);
    }
#endif

    /* only send as many much of token structure as necessary */
    token_len = TOKEN_LEN(numargs);
    
    /* issue the request for remote execution of the user handler */
    token_len = GASNETC_ROUND_DOUBLEWORD(token_len);
    gasneti_assert( token_len <= gasnetc_max_lapi_uhdr_size);
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&o_cntr,0));
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, requester,
			       gasnetc_remote_reply_hh[requester],
			       (void*)token, token_len, NULL, 0,
			       NULL, &o_cntr, NULL));
    gasneti_resume_spinpollers();
    
    /* wait for the Amsend call to complete locally */
    GASNETC_WAITCNTR(&o_cntr,1,&cur_cntr);

    retval = GASNET_OK;
    GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyMediumM( 
    gasnet_token_t token,       /* token provided on handler entry */
    gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
    void *source_addr, size_t nbytes,   /* data payload */
    int numargs, ...) {

    int retval;
    gasnetc_msg_t *msg = &((gasnetc_token_t *)token)->msg;
    uint requester = (uint)msg->sourceId;
    lapi_cntr_t o_cntr;
    int token_len, i, cur_cntr;
    void *udata_start = NULL;
    int udata_avail;
    int udata_packed = 0;

    va_list argptr;
    GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,numargs);

    /* we can re-use the token passed into us.  It was allocated in the
     * LAPI header handler to be large enough to contain the maximum
     * number of arguments.  Upon return from this funciton it will
     * no longer be used
     */
    GASNETC_MSG_SETFLAGS(msg,0,gasnetc_Medium,0,numargs);
    msg->handlerId = handler;
    msg->sourceId = gasneti_mynode;
    msg->destLoc = (uintptr_t)NULL;
    msg->dataLen = nbytes;
    /* do NOT modify the contents of uhdrLoc... needed at origin */
    va_start(argptr, numargs); /*  pass in last argument */
    for (i = 0; i < numargs; i++) {
	msg->args[i] = va_arg(argptr, gasnet_handlerarg_t);
    }
    va_end(argptr);
    udata_start = (void*)&msg->args[numargs];
    token_len = TOKEN_LEN(numargs);
    udata_avail = gasnetc_max_lapi_uhdr_size - token_len - GASNETC_DOUBLEWORD;

    /* can we pack the data into the uhdr? */
    if (nbytes <= udata_avail) {
	memcpy(udata_start,source_addr,nbytes);
	token_len += nbytes;
	udata_packed = 1;
	GASNETC_MSG_SET_PACKED(msg);
    }

    GASNETC_GEN_CSUM(msg, numargs, source_addr, nbytes);

#if GASNETC_ENABLE_LOOPBACK
    if (requester == gasneti_mynode) {
	gasneti_handler_fn_t pfn = gasnetc_handler[handler];
	void *destloc;
	if (nbytes > udata_avail) {
	    destloc = gasneti_malloc(nbytes > 0 ? nbytes : 1);
	    memcpy(destloc,source_addr,nbytes);
	} else {
	    destloc = udata_start;
	}
	GASNETI_RUN_HANDLER_MEDIUM(0,handler,pfn,token,&msg->args[0],numargs,destloc,nbytes);
	if (nbytes > udata_avail) {
	    gasneti_free(destloc);
	}
	GASNETI_RETURN(GASNET_OK);
    }
#endif

    /* issue the request for remote execution of the user handler */
    token_len = GASNETC_ROUND_DOUBLEWORD(token_len);
    gasneti_assert( token_len <= gasnetc_max_lapi_uhdr_size);
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&o_cntr,0));
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, requester,
			       gasnetc_remote_reply_hh[requester],
			       (void*)token, token_len,
			       (udata_packed ? NULL : source_addr),
			       (udata_packed ? 0    : nbytes),
			       NULL, &o_cntr, NULL));
     gasneti_resume_spinpollers();
   
    /* wait for the Amsend call to complete locally */
    GASNETC_WAITCNTR(&o_cntr,1,&cur_cntr);

    retval = GASNET_OK;
    GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyLongM( 
    gasnet_token_t token,       /* token provided on handler entry */
    gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
    void *source_addr, size_t nbytes,   /* data payload */
    void *dest_addr,                    /* data destination on destination node */
    int numargs, ...) {
    int retval;

    gasnetc_msg_t *msg = &((gasnetc_token_t *)token)->msg;
    gasnet_node_t dest;
    lapi_cntr_t o_cntr;
    int token_len, i, cur_cntr;
    void *udata_start = NULL;
    int udata_avail;
    int udata_packed = 0;
    va_list argptr;
  
    GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,numargs); 
    GASNETI_SAFE_PROPAGATE(gasnet_AMGetMsgSource(token, &dest));

    /* re-use the token passed to us */
    GASNETC_MSG_SETFLAGS(msg,0,gasnetc_Long,0,numargs);
    msg->handlerId = handler;
    msg->sourceId = gasneti_mynode;
    msg->destLoc = (uintptr_t)dest_addr;
    msg->dataLen = nbytes;
    /* do NOT modify the contents of uhdrLoc... needed at origin */
    va_start(argptr, numargs); /*  pass in last argument */
    for (i = 0; i < numargs; i++) {
	msg->args[i] = va_arg(argptr, gasnet_handlerarg_t);
    }
    va_end(argptr);
    udata_start = (void*)&msg->args[numargs];
    token_len = TOKEN_LEN(numargs);
    udata_avail = gasnetc_max_lapi_uhdr_size - token_len - GASNETC_DOUBLEWORD;

    GASNETC_GEN_CSUM(msg, numargs, source_addr, nbytes);

#if GASNETC_ENABLE_LOOPBACK
    if (dest == gasneti_mynode) {
	gasneti_handler_fn_t pfn = gasnetc_handler[handler];
	/* copy from source to dest, then execute handler */
	if_pt(dest_addr != source_addr) memcpy((char*)dest_addr,source_addr,nbytes);
	GASNETI_RUN_HANDLER_LONG(0,handler,pfn,token,&msg->args[0],numargs,dest_addr,nbytes);
	GASNETI_RETURN(GASNET_OK);
    }
#endif

    /* can we pack the data into the uhdr? */
    if (nbytes <= udata_avail) {
	memcpy(udata_start,source_addr,nbytes);
	token_len += nbytes;
	udata_packed = 1;
	GASNETC_MSG_SET_PACKED(msg);
    }

    /* issue the request for remote execution of the user handler */
    token_len = GASNETC_ROUND_DOUBLEWORD(token_len);
    gasneti_assert( token_len <= gasnetc_max_lapi_uhdr_size);
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&o_cntr,0));
    gasneti_suspend_spinpollers();
    GASNETC_LCHECK(LAPI_Amsend(gasnetc_lapi_context, dest,
			       gasnetc_remote_reply_hh[dest],
			       (void*)token, token_len,
			       (udata_packed ? NULL : source_addr),
			       (udata_packed ? 0    : nbytes),
			       NULL, &o_cntr, NULL));
    gasneti_resume_spinpollers();
    
    /* wait for the Amsend call to complete locally */
    GASNETC_WAITCNTR(&o_cntr,1,&cur_cntr);

    retval = GASNET_OK;

    GASNETI_RETURN(retval);
}

/* ------------------------------------------------------------------------------------ */
/*
  No-interrupt sections
  =====================
  This section is only required for conduits that may use interrupt-based handler dispatch
  See the GASNet spec and http://www.cs.berkeley.edu/~bonachea/upc/gasnet.html for
  philosophy and hints on efficiently implementing no-interrupt sections
  Note: the extended-ref implementation provides a thread-specific void* within the 
  gasnete_threaddata_t data structure which is reserved for use by the core 
  (and this is one place you'll probably want to use it)
*/

/* ============================================================================
 * LAPI Clarification:
 * These calls should not have to be used or defined.  Even when LAPI is executing
 * in "interrupt" mode, the resulting handler is not executed in a traditional
 * signal handler.  A seperate thread, called the notification thread created at
 * LAPI init time, executes the LAPI dispatcher to make progress on a new
 * incoming packet and other outstanding communications.  No client thread
 * will be interrupted to execute the handler.
 * In LAPI POLLING mode, all progress is made when the client explicitly
 * calls LAPI functions via GASNET calls.  Again, no interrupts will occur.
 * ============================================================================
 */
#if GASNETC_USE_INTERRUPTS
extern void gasnetc_hold_interrupts(void) {
    GASNETI_CHECKATTACH();

    /* Check to see of interrupts are already being held */
    {
	int id = pthread_self();
	gasneti_assert(id >= 0 && id < GASNETC_MAX_THREAD);
	if (gasnetc_interrupt_held[id]) {
	    gasneti_fatalerror("gasnetc_hold_interrupts: already held in thread %d",id);
	}
	gasnetc_interrupt_held[id] = 1;
    }

    #error interrupts not implemented
    /* add code here to disable handler interrupts for _this_ thread */
}
extern void gasnetc_resume_interrupts(void) {
    GASNETI_CHECKATTACH();

    /* Check to insure that interrupts are being held */
    {
	int id = pthread_self();
	gasneti_assert(id >= 0 && id < GASNETC_MAX_THREAD);
	if (gasnetc_interrupt_held[id] != 0) {
	    gasneti_fatalerror("gasnetc_resume_interrupts: Not held in thread %d",id);
	}
	gasnetc_interrupt_held[id] = 0;
    }

    #error interrupts not implemented
    /* add code here to re-enable handler interrupts for _this_ thread */
}
#endif

/* ------------------------------------------------------------------------------------ */
/*
 * Handler-safe locks
 * ==================
 *
 * In the LAPI GASNET implementation, handlers are executed in exactly
 * two contexts:
 *
 * (1) In the LAPI completion handler thread.  Here, only one
 *     completion handler is executed at a time.
 * (2) By the client thread issuing an AM request call
 *     in which the target node is the same as the origin node
 *     and only if the 'GASNETC_ENABLE_LOOPBACK' macro is
 *     set to 1.
 *
 * In both of these cases, posix mutexes are sufficient for HSLs.
 * NOTE: we need the mutex code even in the GASNET_SEQ case.  A handler
 *       can be executing in the LAPI completion handler thread
 *       and modifying data being used by the client thread.
 *
 * Note that we SHOULD add error checking....
*/

extern void gasnetc_hsl_init   (gasnet_hsl_t *hsl) {
    GASNETI_CHECKATTACH();

    gasnetc_spinlock_init(&(hsl->lock));

  #if GASNETC_USE_INTERRUPTS
    /* add code here to init conduit-specific HSL state */
    #error interrupts not implemented
  #endif
}

extern void gasnetc_hsl_destroy(gasnet_hsl_t *hsl) {
    GASNETI_CHECKATTACH();

    gasnetc_spinlock_destroy(&(hsl->lock));

  #if GASNETC_USE_INTERRUPTS
    /* add code here to cleanup conduit-specific HSL state */
    #error interrupts not implemented
  #endif
}

extern void gasnetc_hsl_lock   (gasnet_hsl_t *hsl) {
    GASNETI_CHECKATTACH();

    {
#if GASNETI_STATS_OR_TRACE
    gasneti_tick_t startlock = GASNETI_TICKS_NOW_IFENABLED(L);
#endif
    
    gasnetc_spinlock_lock(&(hsl->lock));

#if GASNETI_STATS_OR_TRACE
    hsl->acquiretime = GASNETI_TICKS_NOW_IFENABLED(L);
    GASNETI_TRACE_EVENT_TIME(L, HSL_LOCK, hsl->acquiretime-startlock);
#endif
    }

  #if GASNETC_USE_INTERRUPTS
    /* conduits with interrupt-based handler dispatch need to add code here to 
       disable handler interrupts on _this_ thread, (if this is the outermost
       HSL lock acquire and we're not inside an enclosing no-interrupt section)
     */
    #error interrupts not implemented
  #endif
}

extern void gasnetc_hsl_unlock (gasnet_hsl_t *hsl) {
    GASNETI_CHECKATTACH();

  #if GASNETC_USE_INTERRUPTS
    /* conduits with interrupt-based handler dispatch need to add code here to 
       re-enable handler interrupts on _this_ thread, (if this is the outermost
       HSL lock release and we're not inside an enclosing no-interrupt section)
     */
    #error interrupts not implemented
  #endif

    GASNETI_TRACE_EVENT_TIME(L, HSL_UNLOCK, GASNETI_TICKS_NOW_IFENABLED(L)-hsl->acquiretime);

    gasnetc_spinlock_unlock(&(hsl->lock));
}

extern int  gasnetc_hsl_trylock(gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();

  {
    int locked = (gasnetc_spinlock_trylock(&(hsl->lock)) == 0);

    GASNETI_TRACE_EVENT_VAL(L, HSL_TRYLOCK, locked);
    if (locked) {
      #if GASNETI_STATS_OR_TRACE
        hsl->acquiretime = GASNETI_TICKS_NOW_IFENABLED(L);
      #endif
      #if GASNETC_USE_INTERRUPTS
        /* conduits with interrupt-based handler dispatch need to add code here to
           disable handler interrupts on _this_ thread, (if this is the outermost
           HSL lock acquire and we're not inside an enclosing no-interrupt section)
         */
        #error interrupts not implemented
      #endif
    }

    return locked ? GASNET_OK : GASNET_ERR_NOT_READY;
  }
}

/* ------------------------------------------------------------------------------------ */
/*
  Private Handlers:
  ================
  see mpi-conduit and extended-ref for examples on how to declare AM handlers here
  (for internal conduit use in bootstrapping, job management, etc.)
*/
static gasnet_handlerentry_t const gasnetc_handlers[] = {
  #ifdef GASNETC_AUXSEG_HANDLERS
    GASNETC_AUXSEG_HANDLERS(),
  #endif
  /* ptr-width independent handlers */

  /* ptr-width dependent handlers */

  { 0, NULL }
};

gasnet_handlerentry_t const *gasnetc_get_handlertable(void) {
    return gasnetc_handlers;
}

/* ==========================================================================
 * MLW: Functions that could be in gasnet_core_internal.c
 * ==========================================================================
 */

/* --------------------------------------------------------------------------
 * This function will be involked in the event of an async error
 * from LAPI.
 * --------------------------------------------------------------------------
 */
static const char* err_type_str[] = {
    "GET_ERR",
    "PUT_ERR",
    "RMW_ERR",
    "AM_ERR",
    "INTERRUPT_ERR"
};
void gasnetc_lapi_err_handler(lapi_handle_t *context, int *error_code,
			      lapi_err_t  *error_type, int *taskid, int *src)
{
    char msg[LAPI_MAX_ERR_STRING];

    LAPI_Msg_string(*error_code,msg);
    gasneti_fatalerror("Async LAPI Error on node %d from task %d of type %s code %d [%s]\n",
		       *src,*taskid,err_type_str[*error_type],*error_code,msg);
}

/* --------------------------------------------------------------------------
 * This function used to as a form of AllGather operation.  It is
 * collective across all the gasnet nodes.
 * Src is a pointer to a data structure of size len.
 * Dest is an array of the same kind of data structures, one element
 * for each gasnet node.
 * 
 * Each node puts a copy of its src into the dest[gasneti_mynode]
 * on every other node.
 * --------------------------------------------------------------------------
 */
void gasnetc_lapi_exchange(void *src, size_t len, void *dest)
{
    void **dest_addr_tab;
    lapi_cntr_t c_cntr;
    int  node;
    int  num_nodes = (int)gasneti_nodes;
    int  cur_val;

    /* First, need to determine address of dest on each node,
     * Note that this is a collective operation.
     */
    dest_addr_tab = gasneti_malloc(num_nodes*sizeof(void*));
    GASNETC_LCHECK(LAPI_Address_init(gasnetc_lapi_context,dest,dest_addr_tab));
    
    /* Now, put my src value into all remote dest arrays */
    GASNETC_LCHECK(LAPI_Setcntr(gasnetc_lapi_context,&c_cntr,0));
    for (node = 0; node < num_nodes; node++) {
	void *ra = (void*)((char*)(dest_addr_tab[node]) + gasneti_mynode*len);
	GASNETC_LCHECK(LAPI_Put(gasnetc_lapi_context,node,len,ra,
				src,NULL,NULL,&c_cntr));
    }

    /* Wait for all puts to complete locally and at targets */
    GASNETC_LCHECK(LAPI_Waitcntr(gasnetc_lapi_context, &c_cntr,num_nodes,&cur_val));
    gasneti_assert(cur_val == 0);

    /* Must barrier to insure all nodes have completed
     * puts to my dest array */
    GASNETC_LCHECK(LAPI_Gfence(gasnetc_lapi_context));

    /* free up array used for remote address locations */
    gasneti_free(dest_addr_tab);
}

void gasnetc_token_queue_init(gasnetc_token_queue_t *q)
{
    gasnetc_spinlock_init(&(q->lock));
    gasnetc_spinlock_lock(&(q->lock));
    q->head = q->tail = NULL;
    q->schedule = 1;
    gasnetc_spinlock_unlock(&(q->lock));
}

gasnetc_token_t* gasnetc_token_dequeue(gasnetc_token_queue_t *q, int update_schedule)
{
    gasnetc_token_t *p;

    /* start by 'peeking' to see if nothing is waiting, to avoid locking overhead 
       cannot do this within the final poll of a completion handler, which must 
       lock to update the schedule flag
     */
    if (!update_schedule && q->head == NULL) return NULL;

    /* spin until queue is available */
    gasnetc_spinlock_lock(&(q->lock));

    /* remove from head */
    p = q->head;

    if (p == NULL) {
	/* the queue was empty, check empty queue condition */
	gasneti_assert(q->tail == NULL);
	/* called from completion handler which just empied the queue,
	 * tell the header handler to schedule completion handler again
	 * on next enqueue
	 */
	if (update_schedule) {
	    q->schedule = 1;
	}
    } else {
	gasneti_assert(q->tail != NULL);
	q->head = p->next;
	p->next = NULL;
	if (p == q->tail) {
	    /* was the last element */
	    q->tail = NULL;
	    gasneti_assert(q->head == NULL);
	}
    }
    /* unlock queue */
    gasnetc_spinlock_unlock(&(q->lock));

    return p;
}

void gasnetc_token_enqueue(gasnetc_token_queue_t *q, gasnetc_token_t *p, int *schedule)
{
    /* gain the queue lock */
    gasnetc_spinlock_lock(&(q->lock));

    /* add to tail of queue */
    p->next = NULL;
    if (q->tail == NULL) {
	/* was empty */
	gasneti_assert(q->head == NULL);
	q->head = q->tail = p;
    } else {
	/* attach to current tail */
	gasneti_assert(q->head != NULL);
	q->tail->next = p;
	q->tail = p;
    }
    
    *schedule = q->schedule;
    q->schedule = 0;
    
    /* unlock queue */
    gasnetc_spinlock_unlock(&(q->lock));
}

void* gasnetc_lapi_AMreply_hh(lapi_handle_t *context, void *uhdr, uint *uhdr_len,
			      ulong *msg_len, compl_hndlr_t **comp_h, void **uinfo)
{
    gasnetc_token_t *token = (gasnetc_token_t*)uhdr;
    gasnetc_msg_t   *msg = &token->msg;
    gasnetc_category_t cat;
    unsigned int is_packed;
    void* destloc = NULL;
    gasnetc_token_t *new_token;
    unsigned int numargs;
    gasnet_handler_t func_ix = msg->handlerId;
    gasneti_handler_fn_t am_func = gasnetc_handler[func_ix];
    gasnet_handlerarg_t *am_args = &msg->args[0];
    int done = 0;

    /* All AM request handlers MUST run in the LAPI completion handler
     * since they will issue an AM reply
     */

    is_packed = GASNETC_MSG_ISPACKED(msg);
    numargs = GASNETC_MSG_NUMARGS(msg);
    cat = GASNETC_MSG_CATEGORY(msg);

#if 0
    { /* For dubugging */
	unsigned int is_req;
	is_req = GASNETC_MSG_ISREQUEST(msg);
	gasneti_assert(!is_req);
    
	GASNETI_TRACE_PRINTF(C,("Reply_HH received %s from %d is_req %d is_packed %d numargs %d uhdr_len %d msg_len %d destLoc %x dataLen %d",
				gasnetc_catname[cat],msg->sourceId,is_req,
				is_packed,numargs,*uhdr_len,*msg_len,
				(void*)msg->destLoc,msg->dataLen));
    }
#endif
    
#if HAVE_BUG2582
    msg->uhdrLoc = (uintptr_t)NULL;
#else
    /* This is a reply. If the uhdrLoc field of the token is set
     * that means the origional GASNET call was an AsyncLong and we
     * should deallocate the origional uhdr memory.
     */
    if (msg->uhdrLoc != (uintptr_t)NULL) {
	gasnetc_token_t* loc = (gasnetc_token_t*)(msg->uhdrLoc);
	gasnetc_uhdr_free(loc);
	msg->uhdrLoc = (uintptr_t)NULL;
    }
#endif

    switch (cat) {
    case gasnetc_Short:
	/* can run the AM handler in-line */
	GASNETI_RUN_HANDLER_SHORT(0,func_ix,am_func,token,am_args,numargs);
	done = 1;
	break;
    case gasnetc_Medium:
	if (is_packed) {
	    /* can run the AM handler in-line, data payload is packed in uhdr */
	    /* Note that bug 2583 is not a problem, since there is no Reply from a Reply */
	    void *srcloc = (void*)&msg->args[numargs];
            GASNETC_CHECK_CSUM(msg, numargs, srcloc, msg->dataLen);
	    GASNETI_RUN_HANDLER_MEDIUM(0,func_ix,am_func,token,am_args,numargs,srcloc,msg->dataLen);
	    done = 1;
	} else {
	    /* data payload not in uhdr, alloc space of it */
	    gasneti_assert( msg->dataLen == *msg_len);
	    destloc = gasneti_malloc( *msg_len > 0 ? *msg_len : 1 );
	    msg->destLoc = (uintptr_t)destloc;
	}
	break;
    case gasnetc_AsyncLong:
    case gasnetc_Long:
	destloc = (void*)msg->destLoc;
	if (is_packed) {
	    /* copy packed data to destination and report to LAPI
	     * dispatcher there is no more data coming
	     */
	    void* udata_start = (void*)&msg->args[numargs];
	    memcpy(destloc,udata_start,msg->dataLen);
	    GASNETI_RUN_HANDLER_LONG(0,func_ix,am_func,token,am_args,numargs,destloc,msg->dataLen);
	    done = 1;
	}
	break;
    default:
	gasneti_fatalerror("Reply_HH: invalid message Category %d",(int)cat);
    }

    if (done) {
	*comp_h = NULL;
	*uinfo = NULL;
	destloc = NULL;
    } else {
	/* Must copy uhdr to allocated uhdr buffer */
	new_token = gasnetc_uhdr_alloc();
	memcpy((void*)new_token,uhdr,*uhdr_len);
	*uinfo = (void*)new_token;
	*comp_h = gasnetc_lapi_AMch;
    }

    return destloc;
}

void* gasnetc_lapi_AMreq_hh(lapi_handle_t *context, void *uhdr, uint *uhdr_len,
			    ulong *msg_len, compl_hndlr_t **comp_h, void **uinfo)
{
    gasnetc_token_t *token = (gasnetc_token_t*)uhdr;
    gasnetc_msg_t *msg = &token->msg;
    gasnetc_category_t cat;
    unsigned int is_packed;
    void* destloc = NULL;
    void *udata_start = NULL;
    unsigned int numargs;
    int token_len = *uhdr_len;
    int is_ready = 0;
    unsigned int is_req;
    gasnetc_token_t *new_token;

    is_req = GASNETC_MSG_ISREQUEST(msg);
    gasneti_assert(is_req);

    is_packed = GASNETC_MSG_ISPACKED(msg);
    numargs = GASNETC_MSG_NUMARGS(msg);
    cat = GASNETC_MSG_CATEGORY(msg);

#if 0
    { /* For dubugging */
    
	GASNETI_TRACE_PRINTF(C,("Req_HH received %s from %d is_req %d is_packed %d numargs %d uhdr_len %d msg_len %d destLoc %x dataLen %d",
				gasnetc_catname[cat],token->sourceId,is_req,
				is_packed,numargs,*uhdr_len,*msg_len,
				(void*)msg->destLoc,msg->dataLen));
    }
#endif
    
    switch (cat) {
    case gasnetc_Short:
	is_ready = 1;
	break;
    case gasnetc_Medium:
	if (is_packed) {
	    /* will use token space as local buffer */
	    gasneti_assert( *msg_len == 0 );
	    msg->destLoc = (uintptr_t)NULL;
	    is_ready = 1;
	} else {
	    gasneti_assert( msg->dataLen == *msg_len);
	    destloc = gasneti_malloc( *msg_len > 0 ? *msg_len : 1 );
	    msg->destLoc = (uintptr_t)destloc;
	}
	break;
    case gasnetc_AsyncLong:
    case gasnetc_Long:
	destloc = (void*)msg->destLoc;
	if (is_packed) {
	    /* copy packed data to destination and report to LAPI
	     * dispatcher there is no more data coming
	     */
	    udata_start = (void*)&msg->args[numargs];
	    memcpy(destloc,udata_start,msg->dataLen);
	    token_len -= msg->dataLen;
	    is_ready = 1;
	}
	break;
    default:
	gasneti_fatalerror("Req_HH: invalid message Category %d",(int)cat);
    }

    /* alloc and copy only the necessary portion of the incoming token */
    new_token = gasnetc_uhdr_alloc();
    if(token_len > GASNETC_TOKEN_SIZE) {
      gasneti_fatalerror("Header too big in HH");
    }
    memcpy((void*)new_token,uhdr,token_len);

    /* By default, schedule the completion handler.  May null this later */
    *comp_h = gasnetc_lapi_AMch;

    if (is_ready) {
      int schedule;
      /* no more data arriving */
      destloc = NULL;
      /* signal to completion handler only to check queue */
      *uinfo = NULL;
      gasnetc_token_enqueue(&gasnetc_req_q,new_token,&schedule);
      /* only schedule completion handler if it has run since
       * last enqueue */
      if (! schedule) {
        *comp_h = NULL;
      }
    } else {
	/* waiting for more data to arrive */
	*uinfo = (void*)new_token;
    }
    return destloc;
}

void gasnetc_lapi_AMch(lapi_handle_t *context, void *uinfo)
{
    gasnetc_token_t *token = (gasnetc_token_t*)uinfo;
    gasnetc_token_t *q_token = NULL;
    int do_schedule = (token == NULL ? 1 : 0);


    /* first, process all items on the request queue to keep
     * latencies to a minimum
     */
    gasnetc_service_req_q(0, do_schedule);

    if (token != NULL) {
	/* Our request was not put on the queue either because
	 * not all data was available or the queue was full.
	 * process it now.
	 */
	gasnetc_run_handler(token);
	/* deallocate the token, it was allocated in the header handler */
	gasnetc_uhdr_free(token);

	/* Check the request queue again */
        gasnetc_service_req_q(0, 1);
    }
}

void gasnetc_run_handler(gasnetc_token_t *token)
{
    gasnetc_msg_t *msg = &token->msg;
    /* extract the token and paramater from the uinfo structure */
    gasnetc_category_t msg_type = GASNETC_MSG_CATEGORY(msg);
    unsigned int numargs = GASNETC_MSG_NUMARGS(msg);
    unsigned int is_request = GASNETC_MSG_ISREQUEST(msg);
    unsigned int is_packed = GASNETC_MSG_ISPACKED(msg);
    void *dataptr = (void*)(msg->destLoc);
    size_t datalen = msg->dataLen;
    gasnet_handler_t func_ix = msg->handlerId;
    gasneti_handler_fn_t am_func = gasnetc_handler[func_ix];
    gasnet_handlerarg_t *am_args = &msg->args[0];

    gasneti_assert(numargs <= GASNETC_AM_MAX_ARGS);

#if 0
    GASNETI_TRACE_PRINTF(C,("CH received %s from %d is_req %d is_packed %d numargs %d handlerix %d dataptr %x datalen %d",
			    gasnetc_catname[msg_type],msg->sourceId,is_request,
			    is_packed,numargs,func_ix,dataptr,datalen));
#endif

    if (am_func == NULL) {
	gasneti_fatalerror("lapi_AMch: node %d, invalid handler index %d",
			   gasneti_mynode,func_ix);
    }

    /* run the GASNET handler */
    switch (msg_type) {
    case gasnetc_Short:
	GASNETI_RUN_HANDLER_SHORT(is_request,func_ix,am_func,token,am_args,numargs);
	break;
	
    case gasnetc_Medium:
	if (is_packed) {
	    /* data is cached in this uhdr, but we must make a copy (bug 2583) */
	    dataptr = gasneti_malloc(datalen);
	    memcpy(dataptr,(void*)&msg->args[numargs],datalen);
	}
        GASNETC_CHECK_CSUM(msg, numargs, dataptr, datalen);
	GASNETI_RUN_HANDLER_MEDIUM(is_request,func_ix,am_func,token,am_args,numargs,dataptr,datalen);
	/* need to free this data memory (either allocated in header handler, or just now) */
	gasneti_free(dataptr);
	break;

    case gasnetc_Long:
    case gasnetc_AsyncLong:
	GASNETI_RUN_HANDLER_LONG(is_request,func_ix,am_func,token,am_args,numargs,dataptr,datalen);
	/* Note that the memory specified by dataptr and datalen must
	 * be in the segment registered on this node.
	 */
	break;
	
    }

}

/* --------------------------------------------------------------------------
 * gasnetc_uhdr_init:
 *
 * Init the LAPI uhdr free list.  Alloc and free called from within
 * LAPI header handler and completion handler.  Must not block
 * indefinately.
 * --------------------------------------------------------------------------
 */
void gasnetc_uhdr_init(int want)
{
    int got;
    gasnetc_spinlock_init(&(gasnetc_uhdr_freelist.lock));
    gasnetc_spinlock_lock(&(gasnetc_uhdr_freelist.lock));
    
    /* init the structure values */
    gasnetc_uhdr_freelist.high_water_mark = 0;
    gasnetc_uhdr_freelist.numfree = 0;
    gasnetc_uhdr_freelist.numalloc = 0;
    gasnetc_uhdr_freelist.head = NULL;
    
    got = gasnetc_uhdr_more(want);

    gasnetc_spinlock_unlock(&(gasnetc_uhdr_freelist.lock));

    if (got == 0) {
	gasneti_fatalerror("Unable to alloc ANY uhdr buffers");
    }
}

/* --------------------------------------------------------------------------
 * gasnetc_uhdr_alloc:
 *
 * return an element from the free list.  If freelist is empty,
 * attempt to alloc more.  If that fails, abort.
 * --------------------------------------------------------------------------
 */
gasnetc_token_t* gasnetc_uhdr_alloc(void)
{
    gasnetc_token_t *p = NULL;

    gasnetc_spinlock_lock(&(gasnetc_uhdr_freelist.lock));

    if (gasnetc_uhdr_freelist.numfree == 0) {
	gasnetc_uhdr_more(GASNETC_UHDR_ADDITIONAL);
	if (gasnetc_uhdr_freelist.numfree == 0) {
	    gasnetc_spinlock_unlock(&(gasnetc_uhdr_freelist.lock));
	    gasneti_fatalerror("Unable to alloc additional uhdr buffers");
	}
    }

    gasneti_assert(gasnetc_uhdr_freelist.head != NULL);
    p = gasnetc_uhdr_freelist.head;
    gasnetc_uhdr_freelist.head = p->next;
    gasnetc_uhdr_freelist.numfree--;
    gasnetc_uhdr_freelist.numalloc++;
    p->next = NULL;
    if (gasnetc_uhdr_freelist.numalloc > gasnetc_uhdr_freelist.high_water_mark)
	gasnetc_uhdr_freelist.high_water_mark = gasnetc_uhdr_freelist.numalloc;

    gasnetc_spinlock_unlock(&(gasnetc_uhdr_freelist.lock));

    return p;
}

/* --------------------------------------------------------------------------
 * gasnetc_uhdr_free:
 *
 * put this uhdr back onto the free list.
 * --------------------------------------------------------------------------
 */
void gasnetc_uhdr_free(gasnetc_token_t *p)
{
    gasnetc_spinlock_lock(&(gasnetc_uhdr_freelist.lock));
    p->next = gasnetc_uhdr_freelist.head;
    gasnetc_uhdr_freelist.head = p;
    gasnetc_uhdr_freelist.numfree++;
    gasnetc_uhdr_freelist.numalloc--;
    gasnetc_spinlock_unlock(&(gasnetc_uhdr_freelist.lock));
}

/* --------------------------------------------------------------------------
 * gasnetc_uhdr_more
 *
 * This is the real allocation function.  It attempts to allocate
 * the requested number of buffers, but decreases the size by a
 * factor of two each time it fails.
 * The allocated buffers are added to the freelist.
 * The return value is the actual number of buffers allocated.
 * --------------------------------------------------------------------------
 */
int gasnetc_uhdr_more(int want)
{
    /* NOTE: assumes lock already held */

    int i;
    char *raw = (char*)gasneti_malloc(want * GASNETC_TOKEN_SIZE * sizeof(char)
				      + GASNETC_DOUBLEWORD);
    gasneti_assert(gasnetc_uhdr_freelist.head == NULL);
    while (raw == NULL) {
	if (want <= 1) {
	    return 0;
	} else {
	    want /= 2;
	}
	raw = (char*)gasneti_malloc(want * GASNETC_TOKEN_SIZE * sizeof(char)
				    + GASNETC_DOUBLEWORD);
    }

    /* align on doubleword boundary.
     * NOTE: we never deallocate these buffers so its ok to lose the
     * allocation address
     */
    raw = (char*)GASNETC_ALIGN_PTR(raw);
    /* link them onto freelist */
    for (i = 0; i < want; i++) {
	gasnetc_token_t *freetok = (gasnetc_token_t*)raw;
	freetok->next = gasnetc_uhdr_freelist.head;
	gasnetc_uhdr_freelist.head = freetok;
	raw += GASNETC_TOKEN_SIZE;
    }
    gasnetc_uhdr_freelist.numfree += want;

    GASNETI_TRACE_PRINTF(C,("Allocated %d more UHDR BUFFERS: numalloc %d numfree %d",
			    want,gasnetc_uhdr_freelist.numalloc,gasnetc_uhdr_freelist.numfree));
    return want;
}

#include <sys/vminfo.h>
/* return the AIX page size for a given memory address */
size_t gasnetc_get_pagesize(void *addr) {
    struct vm_page_info vi;
    vi.addr = (uintptr_t) addr;
    vmgetinfo(&vi,VM_PAGE_INFO,sizeof(struct vm_page_info));
    return (size_t)vi.pagesize;
}

