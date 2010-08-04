/*   $Source: /var/local/cvs/gasnet/portals-conduit/gasnet_core.c,v $
 *     $Date: 2010/05/18 02:54:15 $
 * $Revision: 1.40.2.2 $
 * Description: GASNet portals conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 *                 Michael Welcome <mlwelcome@lbl.gov>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>
#include <gasnet_portals.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

gasnet_handlerentry_t const *gasnetc_get_handlertable(void);
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int, void*);
#else
static void gasnetc_atexit(void);
#endif

gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS]; /* handler table (recommended impl) */

uintptr_t gasnetc_segbase, gasnetc_segend;

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
  gasneti_assert_always(sizeof(gasneti_weakatomic_val_t) == sizeof(uint32_t));
  {
    int mincred = GASNETC_CHUNKSIZE/GASNETC_BYTES_PER_CREDIT +
      (GASNETC_CHUNKSIZE%GASNETC_BYTES_PER_CREDIT?1:0);
    gasneti_assert_always(GASNETC_MIN_CREDITS == mincred);
  }
  /* Medium header data: max of 16 args + optional seq no + 8-byte alignment = 16*4 or 18*4 */
#if GASNET_DEBUG
  gasneti_assert_always(gasnet_AMMaxMedium() == (GASNETC_CHUNKSIZE - 18*4));
#else
  gasneti_assert_always(gasnet_AMMaxMedium() == (GASNETC_CHUNKSIZE - 16*4));
#endif
}

static int gasnetc_init(int *argc, char ***argv) {
#if GASNETC_DEBUG
  printf("Entering gasnetc_init\n");
#endif
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

    /* setup portals network */
  gasnetc_init_portals_network(argc,argv);

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif

  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    {
      /* try to determine the max amount of memory we can alloc and pin on each node */
      uintptr_t max_pin = gasnetc_portalsMaxPinMem();

#if GASNETC_DEBUG
      {
	printf("[%d] Portals Conduit reports Max Pin Mem = %ld\n",gasneti_mynode,(long)max_pin);
	fflush(stdout);
      }
#endif

      /* localSegmentLimit provides a conduit-specific limit on the max segment size.
       * can use (uintptr_t)-1 as unlimited.
       * In case of Portals/Catamount there is no mmap so both MaxLocalSegmentSize
       * and MaxGlobalSegmentSize are basically set to the min of localSegmentLimit
       * and GASNETI_MALLOCSEGMENT_MAX_SIZE, which defaults to 100MB.
       * Can set GASNET_MAX_SEGSIZE=XXXM env var to over-ride this.
       */
      gasneti_segmentInit( max_pin, &gasnetc_bootstrapExchange);
    }
  #elif GASNET_SEGMENT_EVERYTHING
    /* segment is everything - nothing to do */
  #else
    #error Bad segment config
  #endif

#if 0  /* MLW: tested this, and it seems Cray does propogate env on XT3 */
    /* Enable this if you wish to use the default GASNet services for broadcasting 
        the environment from one compute node to all the others (for use in gasnet_getenv(),
        which needs to return environment variable values from the "spawning console").
        You need to provide two functions (gasnetc_bootstrapExchange and gasnetc_bootstrapBroadcast)
        which the system can safely and immediately use to broadcast and exchange information 
        between nodes (gasnetc_bootstrapBroadcast is optional but highly recommended).
        See gasnet/other/mpi-spawner/gasnet_bootstrap_mpi.c for definitions of these two
        functions in terms of MPI collective operations.
       This system assumes that at least one of the compute nodes has a copy of the 
        full environment from the "spawning console" (if this is not true, you'll need to
        implement something yourself to get the values from the spawning console)
       If your job system already always propagates environment variables to all the compute
        nodes, then you probably don't need this.
     */
    gasneti_setupGlobalEnvironment(gasneti_nodes, gasneti_mynode, 
                                   &gasnetc_bootstrapExchange, &gasnetc_bootstrapBroadcast);
  #endif

  gasneti_auxseg_init(); /* adjust max seg values based on auxseg */

#if GASNETC_DEBUG
  printf("[%d] Leaving gasnetc_init\n",gasneti_mynode);
  fflush(stdout);
#endif
  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
extern int gasnet_init(int *argc, char ***argv) {
  int retval;
#if GASNETC_USE_SANDIA_ACCEL
  gasneti_weakatomic_set(&gasnetc_got_signum,0,0);  
#endif
  retval = gasnetc_init(argc, argv);
  if (retval != GASNET_OK) GASNETI_RETURN(retval);
  #if 0
    /* called within gasnet_init to allow init tracing */
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
    gasnetc_handler[(gasnet_handler_t)newindex] = (gasneti_handler_fn_t)table[i].fnptr;

    /* The check below for !table[i].index is redundant and present
     * only to defeat the over-aggressive optimizer in pathcc 2.1
     */
    if (dontcare && !table[i].index) table[i].index = newindex;

    (*numregistered)++;
  }
  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int gasnetc_attach(gasnet_handlerentry_t *table, int numentries,
                          uintptr_t segsize, uintptr_t minheapoffset) {
  void *segbase = NULL;
#if GASNETC_DEBUG
  {
  printf("[%d] Entering gasnetc_attach\n",gasneti_mynode);
  fflush(stdout);
  }
#endif
  
  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(table (%i entries), segsize=%lu, minheapoffset=%lu)",
                          numentries, (unsigned long)segsize, (unsigned long)minheapoffset));

  /* check for system messages */
  gasnetc_sys_poll(GASNETC_EQ_LOCK);

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
    GASNETI_TRACE_PRINTF(C,("Registering %d default AM Handlers",GASNETC_MAX_NUMHANDLERS));
    for (i = 0; i < GASNETC_MAX_NUMHANDLERS; i++) 
      gasnetc_handler[i] = (gasneti_handler_fn_t)&gasneti_defaultAMHandler;
  }
  { /*  core API handlers */
    gasnet_handlerentry_t *ctable = (gasnet_handlerentry_t *)gasnetc_get_handlertable();
    int len = 0;
    int numreg = 0;
    gasneti_assert(ctable);
    while (ctable[len].fnptr) len++; /* calc len */
    GASNETI_TRACE_PRINTF(C,("Registering %d core Handlers",len));
    if (gasnetc_reghandlers(ctable, len, 1, 63, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering core API handlers");
    gasneti_assert(numreg == len);
  }

  { /*  extended API handlers */
    gasnet_handlerentry_t *etable = (gasnet_handlerentry_t *)gasnete_get_handlertable();
    int len = 0;
    int numreg = 0;
    gasneti_assert(etable);
    while (etable[len].fnptr) len++; /* calc len */
    GASNETI_TRACE_PRINTF(C,("Registering %d Extended API Handlers",len));
    if (gasnetc_reghandlers(etable, len, 64, 127, 0, &numreg) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering extended API handlers");
    gasneti_assert(numreg == len);
  }

  if (table) { /*  client handlers */
    int numreg1 = 0;
    int numreg2 = 0;

    /*  first pass - assign all fixed-index handlers */
    if (gasnetc_reghandlers(table, numentries, 128, 255, 0, &numreg1) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering fixed-index client handlers");

    /*  second pass - fill in dontcare-index handlers */
    if (gasnetc_reghandlers(table, numentries, 128, 255, 1, &numreg2) != GASNET_OK)
      GASNETI_RETURN_ERRR(RESOURCE,"Error registering fixed-index client handlers");
    GASNETI_TRACE_PRINTF(C,("Registering %d Client Handlers",numentries));

    gasneti_assert(numreg1 + numreg2 == numentries);
  }

  /* ------------------------------------------------------------------------------------ */
  /*  register fatal signal handlers */

  /* catch fatal signals and convert to SIGQUIT */
#if GASNETC_USE_SANDIA_ACCEL
  gasneti_registerSignalHandlers(gasnetc_portalsSignalHandler);
#else
  gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);
#endif

#if HAVE_ON_EXIT
  on_exit(gasnetc_on_exit, NULL);
#else
  atexit(gasnetc_atexit);
#endif

  /* ------------------------------------------------------------------------------------ */
  /*  register segment  */

  gasneti_seginfo = (gasnet_seginfo_t *)gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));

  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    if (segsize == 0) segbase = NULL; /* no segment */
    else {
      gasneti_segmentAttach(segsize, minheapoffset, gasneti_seginfo, &gasnetc_bootstrapExchange);
      segbase = gasneti_seginfo[gasneti_mynode].addr;
      segsize = gasneti_seginfo[gasneti_mynode].size;
      gasnetc_assert_aligned(segbase, GASNET_PAGESIZE);
      gasnetc_assert_aligned(segsize, GASNET_PAGESIZE);
    }
    gasnetc_segbase = (uintptr_t)segbase;
    gasnetc_segend = gasnetc_segbase + segsize;
  #else
    /* GASNET_SEGMENT_EVERYTHING */
    segbase = (void *)0;
    segsize = (uintptr_t)-1;
    gasnetc_segbase = 0;
    gasnetc_segend = (uintptr_t)-1;
    /* (###) add any code here needed to setup GASNET_SEGMENT_EVERYTHING support */
  #endif

  /* ------------------------------------------------------------------------------------ */
  /*  gather segment information */
  /* This was done by segmentAttach above */

  /* ------------------------------------------------------------------------------------ */
  /*  primary attach complete */
  gasneti_attach_done = 1;
  gasnetc_bootstrapBarrier();

  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(): primary attach complete"));

  gasneti_assert(gasneti_seginfo[gasneti_mynode].addr == segbase &&
         gasneti_seginfo[gasneti_mynode].size == segsize);

  gasneti_auxseg_attach(); /* provide auxseg */

  /* Init all the portals resources to allow for AMs, puts and gets */
  gasnetc_init_portals_resources();

  gasnete_init(); /* init the extended API */

  gasneti_nodemapFini();

  /* ensure extended API is initialized across nodes */
  gasnetc_bootstrapBarrier();
  gasnetc_resource_init_complete = 1;

  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int exitcode, void *arg) {
    gasnetc_exit(exitcode);
}
#else
static void gasnetc_atexit(void) {
    gasnetc_exit(0);
}
#endif

extern void gasnetc_exit(int exitcode) {
  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  gasneti_reghandler(SIGQUIT, SIG_IGN);

  {  /* ensure only one thread ever continues past this point */
    static gasneti_mutex_t exit_lock = GASNETI_MUTEX_INITIALIZER;
    gasneti_mutex_lock(&exit_lock);
  }

  /* should prevent us from entering again */
  gasnetc_shutdownInProgress = 1;

  /* NOTE: shutdown messages are sent in out-of-band SYS queue, which means they
   * get processed ahead of regular AMs on target nodes.
   * If program does not have a barrier at the end (before gasnetc_exit is called)
   * this can cause the program to fail.  The first to arrive will send a shutdown
   * message to all other nodes and they may see the message before processing
   * remaining messages in their AM queues, and terminate early.
   * UPC runtime layer has barrier before calling gasnet_exit so this should not
   * be a problem.  However, if all threads call upc_global_exit() without
   * a preceeding barrier, this may cause some program threads to terminate
   * before they reach their call to upc_global_exit(). -MLW
   *
   * NOTE: Spec does say client should include a barrier before gasnet_exit().
   * So, the behavior described above is fine in my opinion. -PHH
   */
#if 0
  /* send a shutdown message to everyone */
  {
    gasnet_node_t node;
    gasnetc_conn_state[gasneti_mynode].flags |= GASNETC_SYS_GOT_SHUTDOWN_MSG;
    GASNETI_TRACE_PRINTF(C,("Sending SHUTDOWN Messages to all nodes"));
    for (node = gasneti_mynode+1; node < gasneti_nodes; node++) {
      gasnetc_sys_poll(GASNETC_EQ_TRYLOCK);
      gasnetc_sys_SendMsg(node,GASNETC_SYS_SHUTDOWN_REQUEST, gasneti_mynode, exitcode, 0);
    }
    for (node = 0; node < gasneti_mynode; node++) {
      gasnetc_sys_poll(GASNETC_EQ_TRYLOCK);
      gasnetc_sys_SendMsg(node,GASNETC_SYS_SHUTDOWN_REQUEST, gasneti_mynode, exitcode, 0);
    }
  }

  /* dump final credit state (if compiled with GASNETC_CREDIT_TESTING flag) */
  GASNETC_DUMP_CREDITS(gasneti_weakatomic_read(&gasnetc_AMRequest_count,0));

  /* Now, poll for a while to see if all nodes either sent us a shutdown request
   * or replied to our shutdown request
   */
  if (gasnetc_shutdown_seconds > 0) {
    gasnet_node_t node;
    int cnt = 0;
    uint64_t starttime = GASNETC_CURRENT_TIME();  /* in nanoseconds */
    uint64_t stoptime= starttime;
    uint64_t shutdowntime = 1e9 * gasnetc_shutdown_seconds;
    while (( cnt < gasneti_nodes) && (stoptime-starttime<shutdowntime)) {
      cnt = 0;
      for (node = 0; node < gasneti_nodes; node++) cnt += (gasnetc_conn_state[node].flags & GASNETC_SYS_GOT_SHUTDOWN_MSG ? 1 : 0);
      if (cnt < gasneti_nodes) gasnetc_sys_poll(GASNETC_EQ_TRYLOCK);
      stoptime = GASNETC_CURRENT_TIME();
    } 

    if (cnt < gasneti_nodes) {
      /* have not heard back from some nodes, terminate job */
      printf("[%d] In shutdown, Only %d/%d nodes responded after %lu milliseconds\n",gasneti_mynode,cnt,gasneti_nodes,(stoptime-starttime)/1000000);
      raise(SIGKILL); /* Fatal signal causes launcher to kill job */
    }
  }
#else
  /* dump final credit state (if compiled with GASNETC_CREDIT_TESTING flag) */
  GASNETC_DUMP_CREDITS(gasneti_weakatomic_read(&gasnetc_AMRequest_count,0));

  #if PLATFORM_OS_CNL /* Never had a chance to test for alarm() on Catamount */
    gasneti_reghandler(SIGALRM, SIG_DFL);
    alarm(2 + gasnetc_shutdown_seconds);
  #endif

  if (gasnetc_sys_exit(&exitcode)) {
    printf("[%d] Failed to coordinate shutdown after %lu milliseconds\n",gasneti_mynode,(unsigned long)(1e3*gasnetc_shutdown_seconds));
    /* Death of any process by a fatal signal will cause launcher to kill entire job.
     * We don't use INT or TERM since one could be blocked if we are in its handler. */
    raise(SIGKILL);
    gasneti_killmyprocess(exitcode); /* last chance */
  }

  #if PLATFORM_OS_CNL
    alarm(0);
  #endif

#endif

  /* if we got here, this is a clean shutdown.  Clean up portals resources */
  gasnetc_portals_exit();

  /* preform cleanup operations */
  gasneti_flush_streams();
  gasneti_trace_finish();
  gasneti_sched_yield();
  
  /* kill myself without generateing core dumps, etc */
  gasneti_killmyprocess(exitcode);
  gasneti_fatalerror("gasnetc_exit: killmyprocess should not have returned");
}

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/
extern int gasnetc_AMGetMsgSource(gasnet_token_t token, gasnet_node_t *srcindex) {
  gasnet_node_t sourceid;
  gasnetc_ptl_token_t *ptok = (gasnetc_ptl_token_t*)token;
  GASNETI_CHECKATTACH();
  GASNETI_CHECK_ERRR((!token),BAD_ARG,"bad token");
  GASNETI_CHECK_ERRR((!srcindex),BAD_ARG,"bad src ptr");

  /* MLW: for now, we sent node ID in data packet.  Could hash loopup on portals address */
  sourceid = ptok->srcnode;

  gasneti_assert(sourceid < gasneti_nodes);
  *srcindex = sourceid;
  return GASNET_OK;
}

extern int gasnetc_AMPoll(void) {
  GASNETI_CHECKATTACH();

  gasnetc_portals_poll(GASNETC_FULL_POLL);

  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/

/* The NoOp AM handler function */
GASNETI_INLINE(gasnetc_noop_reph_inner)
void gasnetc_noop_reph_inner(gasnet_token_t token)
{
  GASNETI_TRACE_PRINTF(C,("Running No-Op Handler"));
}
SHORT_HANDLER(gasnetc_noop_reph,0,0,
              (token),
              (token) );


#define AM_RELEASE_RESOURCES(th) do {			\
    if (th->snd_tickets) {				\
      gasnetc_return_tickets(&gasnetc_send_tickets, th->snd_tickets);\
      th->snd_tickets = 0;				\
    }							\
    if (th->tmpmd_tickets) {				\
      gasneti_assert(th->tmpmd_tickets == 1);		\
      gasnetc_return_ticket(&gasnetc_tmpmd_tickets);	\
      th->tmpmd_tickets = 0;				\
    }							\
    gasneti_assert(th->snd_credits == 0);		\
  } while(0)

#define GASNETC_AM_LOOPBACK_COMMON(_args) \
    GASNETC_ZERO_AMARGS(_args);				\
    do {						\
      va_list argptr; int i;				\
      va_start(argptr, numargs);			\
      for (i = 0; i < numargs; i++) _args[i] = va_arg(argptr,gasnet_handlerarg_t);\
      va_end(argptr);					\
    } while (0)

#define GASNETC_AM_HEADER_COMMON(cred_word) do {			\
    va_list argptr;							\
    int i;								\
									\
    gasnetc_assert_aligned(data32,GASNETI_MEDBUF_ALIGNMENT);		\
									\
    /* pack all the args in to data payload */				\
    va_start(argptr, numargs);						\
    for (i = 0; i < numargs; i++) {					\
      uint32_t garg = va_arg(argptr,gasnet_handlerarg_t);		\
      *(data32++) = garg;						\
      GASNETC_ADD_HARG(garg); /* debug */				\
    }									\
    va_end(argptr);							\
									\
    GASNETC_INJECT_NEXT_SEQNO(data32); /* debug */			\
									\
    /* construct the match bits */					\
    GASNETC_PACK_AM_MBITS(mbits,local_offset,numargs,handler,amtype,targ_mbits); \
									\
    /* pack srcnode and cred_word into hdr_data */			\
    GASNETC_PACK_2INT(hdr_data,gasneti_mynode,cred_word);		\
  } while (0)

extern int gasnetc_AMRequestShortM( 
                            gasnet_node_t dest,       /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  ptl_size_t         local_offset;
  ptl_size_t         remote_offset = 0;
  ptl_handle_md_t    md_h = gasnetc_ReqSB.md_h;
  ptl_process_id_t   target_id = gasnetc_procid_map[dest].ptl_id;
  ptl_ac_index_t     ac_index = GASNETC_PTL_AC_ID;
  uint64_t           amtype = GASNETC_PTL_AM_SHORT | GASNETC_PTL_AM_REQUEST;
  uint64_t           targ_mbits = GASNETC_PTL_REQRB_BITS  | GASNETC_PTL_MSG_AM;
  ptl_match_bits_t   mbits;
  ptl_hdr_data_t     hdr_data;
  ptl_size_t         msg_bytes;
  gasnetc_conn_t    *state = &gasnetc_conn_state[dest];
  uint32_t          *data32;
  gasnetc_threaddata_t *th = gasnetc_mythread();
  int                nsend,ncredit;
  uint8_t            cred_byte = 0;

  GASNETC_DEF_HARGS();  /* debugging, must be first statement */

  GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs);

  /* handle loopback case */
  if (dest == gasneti_mynode) {
    gasnetc_ptl_token_t tok;
    gasnet_token_t      token = (gasnet_token_t)&tok;
    GASNETC_AM_LOOPBACK_COMMON(tok.args);
    tok.srcnode = gasneti_mynode;
    tok.initiator_offset = 0;
    GASNETI_TRACE_PRINTF(C,("AM_LOOPBACK: S_Req handler=%d, narg=%d",handler,numargs));
    GASNETC_DBGMSG(1,1,"S",gasneti_mynode,dest,handler,numargs,tok.args,0,cred_byte,0,NULL,th);
    GASNETI_RUN_HANDLER_SHORT(1, handler, gasnetc_handler[handler], token, tok.args, numargs);
    gasneti_AMPoll();
    GASNETI_RETURN(GASNET_OK);
  }

  gasneti_assert(th->snd_credits == 0);

  /* Short Request Data Format:
   *     HD=[srcnode,cred] MB=[off,XX] Data=[args][seqno][pad]
   * NOTE: seqno included only in debug mode
   */

  /* Here we pre-compute msg length based on number of arguments and message format */
  msg_bytes = numargs * sizeof(gasnet_handlerarg_t);
  GASNETC_SEQNO_MSGLEN(msg_bytes);           /* debug */
  msg_bytes = GASNETI_ALIGNUP(msg_bytes,GASNETI_MEDBUF_ALIGNMENT);
  ncredit = gasnetc_compute_credits(msg_bytes);
  if (!gasnetc_use_flow_control) gasneti_assert(ncredit == 0);
  nsend = 1;

  /* poll until ok to send message, allocate ReqSB chunk */
  GASNETC_COMMON_AMREQ_START(state,local_offset,th,nsend,ncredit,cred_byte);
  data32 = (uint32_t*)((uintptr_t)gasnetc_ReqSB.start + local_offset);

  GASNETC_AM_HEADER_COMMON(cred_byte);

  gasneti_assert(th->snd_tickets);
  th->snd_tickets--;

  gasneti_assert(th->snd_credits >= ncredit);
  th->snd_credits -= ncredit;

  GASNETC_DBGMSG(1,1,"S",gasneti_mynode,dest,handler,numargs,hargs,msg_bytes,cred_byte,0,NULL,th);

  /* send message */
  GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, msg_bytes, PTL_NOACK_REQ, target_id, GASNETC_PTL_AM_PTE, ac_index, mbits, remote_offset, hdr_data));

  /* Always poll at end of AM Request */
  gasneti_AMPoll();

  /* release any allocated but unused resources here */
  AM_RELEASE_RESOURCES(th);

  GASNETI_RETURN(GASNET_OK);
}

extern int gasnetc_AMRequestMediumM( 
                            gasnet_node_t dest,      /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  ptl_size_t         local_offset;
  ptl_size_t         remote_offset = 0;
  ptl_handle_md_t    md_h = gasnetc_ReqSB.md_h;
  ptl_process_id_t   target_id = gasnetc_procid_map[dest].ptl_id;
  ptl_ac_index_t     ac_index = GASNETC_PTL_AC_ID;
  uint64_t           amtype = GASNETC_PTL_AM_MEDIUM | GASNETC_PTL_AM_REQUEST;
  uint64_t           targ_mbits = GASNETC_PTL_REQRB_BITS  | GASNETC_PTL_MSG_AM;
  uint32_t           len_cred;
  ptl_match_bits_t   mbits;
  ptl_hdr_data_t     hdr_data;
  gasnetc_conn_t    *state = gasnetc_conn_state + dest;
  int                i;
  int                msg_bytes;
  uint32_t           *data32;
  gasnetc_threaddata_t *th = gasnetc_mythread();
  int                nsend, ncredit;
  uint8_t            cred_byte = 0;

  GASNETC_DEF_HARGS();  /* debugging, must be first statement */

  GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);

  gasneti_assert(nbytes <= gasnet_AMMaxMedium());

  /* handle loopback case */
  if (dest == gasneti_mynode) {
    gasnetc_ptl_token_t tok;
    gasnet_token_t      token = (gasnet_token_t)&tok;
    void *tmpdata = gasnetc_alloc_tmp(nbytes);
    GASNETC_AM_LOOPBACK_COMMON(tok.args);
    gasneti_assert(!((uintptr_t)tmpdata & 7)); /* x86_64 ABI ensures this, alloca() does not */
    tok.srcnode = gasneti_mynode;
    tok.initiator_offset = 0;
    /* dont allow handler to modify source memory */
    memcpy(tmpdata,source_addr,nbytes);
    GASNETI_TRACE_PRINTF(C,("AM_LOOPBACK: M_Req handler=%d, narg=%d nbytes=%d",handler,numargs,(int)nbytes));
    GASNETC_DBGMSG(1,1,"M",gasneti_mynode,dest,handler,numargs,tok.args,0,cred_byte,nbytes,source_addr,th);
    GASNETI_RUN_HANDLER_MEDIUM(1, handler, gasnetc_handler[handler], token, tok.args, numargs, tmpdata, nbytes);
    gasnetc_free_tmp(tmpdata);
    gasneti_AMPoll();
    GASNETI_RETURN(GASNET_OK);
  }

  /* Medium Request Data Format:
   *     HD=[srcnode,len:cred] MB=[off,XX] Data=[args][seqno][pad][data][pad]
   * NOTE: seqno included only in debug mode
   */

  gasneti_assert(th->snd_credits == 0);

  msg_bytes = numargs * sizeof(gasnet_handlerarg_t);
  /* have accounted for args.  Now add [seqno] [pad1] [data] [pad2]*/
  GASNETC_SEQNO_MSGLEN(msg_bytes);           /* debug */
  msg_bytes = GASNETI_ALIGNUP(msg_bytes,GASNETI_MEDBUF_ALIGNMENT);
  msg_bytes += nbytes;
  msg_bytes = GASNETI_ALIGNUP(msg_bytes,GASNETI_MEDBUF_ALIGNMENT);
  gasneti_assert(msg_bytes <= GASNETC_CHUNKSIZE);

  /* resources we need */
  ncredit = gasnetc_compute_credits(msg_bytes);
  if (!gasnetc_use_flow_control) gasneti_assert(ncredit == 0);
  nsend = 1;

  /* poll until ok to send message, allocate ReqSB chunk */
  GASNETC_COMMON_AMREQ_START(state,local_offset,th,nsend,ncredit,cred_byte);
  data32 = (uint32_t*)((uintptr_t)gasnetc_ReqSB.start + local_offset);

  /* pack cred_byte together with message length, we need only 10 bits for nbytes */
  len_cred = (nbytes << 8) | cred_byte;

  GASNETC_AM_HEADER_COMMON(len_cred);

  /* copy data payload (aligned) */
  memcpy((void*)GASNETI_ALIGNUP(data32,GASNETI_MEDBUF_ALIGNMENT),source_addr,nbytes);

  gasneti_assert(th->snd_tickets);
  th->snd_tickets--;

  gasneti_assert(th->snd_credits >= ncredit);
  th->snd_credits -= ncredit;

  GASNETC_DBGMSG(1,1,"M",gasneti_mynode,dest,handler,numargs,hargs,msg_bytes,cred_byte,nbytes,source_addr,th);

  /* send message */
  GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, msg_bytes, PTL_NOACK_REQ, target_id, GASNETC_PTL_AM_PTE, ac_index, mbits, remote_offset, hdr_data));

  /* Always poll at end of AM Request */
  gasneti_AMPoll();

  /* release any allocated but unused resources here */
  AM_RELEASE_RESOURCES(th);

  GASNETI_RETURN(GASNET_OK);
}

#if GASNET_DEBUG
    /* Reserve an extra sizeof(uint32_t) for the debug sequence number */
    #define MAX_PACKED_LONG (GASNETC_CHUNKSIZE - sizeof(uint32_t))
#else
    #define MAX_PACKED_LONG GASNETC_CHUNKSIZE
#endif

/* Compute the resources needed to send this Long AM.  If isPacked is true, message
 * will fit into one ReqSB chunk.  However, this may require more send credits
 * than we have available.  In this case, revert to two-message send if uses
 * fewer credits.
 *   dest:      IN
 *   isPacked:  OUT
 *   msg_bytes: OUT
 *   nsend:     OUT
 *   nscredit:  OUT
 *   ntmpmd:    OUT
 *
 * Regular Format: HD=[srcnode,cred]     MB=[off,XX] data=[args][seqno][pad]
 * Packed  Format: HD=[srcnode,len:cred] MB=[off,XX] data=[args][seqno][destaddr][data][pad]
 * seqno only in debug mode 
 */
#define AM_LONG_COMPUTE_RESOURCES(dest,isPacked,msg_bytes,nsend,ncredit,ntmpmd) do { \
    int packed_credits = 0;						\
    int packed_bytes = 0;						\
    int arg_bytes = numargs * sizeof(gasnet_handlerarg_t);		\
									\
    /* Begin assuming Regular Format */					\
    msg_bytes = arg_bytes;						\
    GASNETC_SEQNO_MSGLEN(msg_bytes);					\
    msg_bytes = GASNETI_ALIGNUP(msg_bytes,GASNETI_MEDBUF_ALIGNMENT);	\
    gasneti_assert((gasnetc_use_flow_control == 0) ||			\
		   (gasnetc_use_flow_control == 1));			\
    ncredit = gasnetc_compute_credits(msg_bytes) +			\
	      gasnetc_use_flow_control; /* for PUT_END event of RARAM */ \
    nsend = 2;								\
    ntmpmd = !gasnetc_use_firehose &&					\
	     !gasnetc_in_local_rar(source_addr,nbytes);			\
    isPacked = 0;							\
									\
    /* Can/should we use Packed Format? */				\
    if (gasnetc_allow_packed_long) {					\
      packed_bytes = arg_bytes + sizeof(void*) + nbytes;		\
      GASNETC_SEQNO_MSGLEN(packed_bytes);				\
      packed_bytes = GASNETI_ALIGNUP(packed_bytes,GASNETI_MEDBUF_ALIGNMENT);\
      if (packed_bytes <= MAX_PACKED_LONG) {				\
	packed_credits = gasnetc_compute_credits(packed_bytes);		\
	/* Use packed unless doing so creates an avoidable stall */	\
	if (!(gasnetc_use_flow_control &&				\
	      (packed_credits > ncredit) &&				\
	      (packed_credits > gasnetc_avail_credits(dest)))) {	\
          msg_bytes = packed_bytes;					\
          ncredit = packed_credits;					\
          nsend = 1;							\
          ntmpmd = 0;							\
          isPacked = 1;							\
        }								\
      }									\
    }									\
    gasnetc_assert_aligned(msg_bytes,GASNETI_MEDBUF_ALIGNMENT);		\
    gasneti_assert(msg_bytes <= GASNETC_CHUNKSIZE);			\
  } while(0)


/* Factored code common to both AMRequestLong and AMRequestAsync */
#define AM_LONG_COMMON(do_sync,isPacked,th,cred_byte) do {		\
    ptl_size_t           remote_offset = 0;				\
    ptl_handle_md_t      md_h = gasnetc_ReqSB.md_h;			\
    ptl_process_id_t     target_id = gasnetc_procid_map[dest].ptl_id;	\
    ptl_ac_index_t       ac_index = GASNETC_PTL_AC_ID;			\
    uint64_t             amtype = GASNETC_PTL_AM_LONG | GASNETC_PTL_AM_REQUEST;	\
    uint64_t             targ_mbits = GASNETC_PTL_REQRB_BITS  | GASNETC_PTL_MSG_AM; \
    uint32_t             len_cred;					\
    ptl_match_bits_t     mbits;						\
    ptl_hdr_data_t       hdr_data;					\
    uint32_t             *data32;					\
									\
    if_pf (!gasnetc_use_firehose) {					\
      int pollcnt = 0;							\
      GASNETC_GET_TMPMD_TICKETS(th,ntmpmd,pollcnt);			\
      gasneti_assert(th->tmpmd_tickets >= ntmpmd);			\
    }									\
									\
    len_cred = cred_byte;						\
    if (isPacked) {							\
      /* pack nbytes w/ the cred_byte, len fits in 10 bits */		\
      len_cred |= nbytes << 8;						\
      amtype |= GASNETC_PTL_AM_PACKED;					\
    }									\
									\
    /* get the addr of the start of the chunk */			\
    data32 = (uint32_t*)((uintptr_t)gasnetc_ReqSB.start + local_offset);\
									\
    GASNETC_AM_HEADER_COMMON(len_cred);					\
									\
    if (isPacked) {							\
									\
      /* pack the target node RAR destination address for this message */ \
      *(data32++) = GASNETI_LOWORD(dest_addr);				\
      *(data32++) = GASNETI_HIWORD(dest_addr);				\
									\
      /* copy the data payload */					\
      memcpy(data32,source_addr,nbytes);				\
									\
      gasneti_assert(th->snd_tickets > 0);				\
      th->snd_tickets--;						\
									\
      gasneti_assert(th->snd_credits >= ncredit);			\
      th->snd_credits -= ncredit;					\
									\
      GASNETC_DBGMSG(1,1,"L",gasneti_mynode,dest,handler,numargs,hargs,msg_bytes,cred_byte,nbytes,source_addr,th); \
      /* send message */						\
      GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, msg_bytes, PTL_NOACK_REQ, target_id, GASNETC_PTL_AM_PTE, ac_index, mbits, remote_offset, hdr_data)); \
									\
      GASNETI_TRACE_EVENT(C, LONG_PACKED);				\
    } else {								\
      ptl_handle_md_t data_md_h;					\
      ptl_match_bits_t data_mbits = GASNETC_PTL_MSG_AMDATA | GASNETC_PTL_RARAM_BITS; \
      ptl_size_t data_offset = 0;					\
      ptl_size_t data_rmt_offset = GASNETC_PTL_OFFSET(dest,dest_addr);	\
      ptl_hdr_data_t data_hdr_data = GASNETI_MAKEWORD(gasneti_mynode, local_offset);\
									\
      gasneti_assert(th->snd_credits >= ncredit);			\
      gasneti_assert(th->snd_tickets > 1);				\
									\
      /* first issue data put message and request sync */		\
      data_mbits |= ((ptl_match_bits_t)GASNETC_PTL_AM_REQUEST << 8);	\
      data_mbits |= ((ptl_match_bits_t)(th->threadidx) << 24);		\
      data_mbits |= ((ptl_match_bits_t)(handler) << 48);		\
      data_mbits |= ((ptl_match_bits_t)(numargs) << 56);		\
      if (gasnetc_in_local_rar(source_addr,nbytes)) {			\
	data_md_h = gasnetc_RARSRC.md_h;				\
	data_offset = GASNETC_PTL_OFFSET(gasneti_mynode,source_addr);	\
	GASNETI_TRACE_EVENT(C, LONG_RAR);				\
      } else GASNETC_IF_USE_FIREHOSE (					\
	gasnetc_fh_op_t *op =						\
	    	gasnetc_fh_aligned_local_pin(source_addr, nbytes);	\
	const firehose_request_t *fh_loc = &op->fh[0];			\
	data_md_h = fh_loc->client;					\
	data_offset = (uintptr_t)source_addr - fh_loc->addr;		\
	data_mbits |= ((ptl_match_bits_t)(op->addr.fulladdr) << 32);	\
	gasneti_assert(nbytes <= (fh_loc->len - data_offset));		\
	GASNETI_TRACE_EVENT(C, LONG_FH);				\
      ) else {								\
	gasneti_assert(th->tmpmd_tickets > 0);				\
	data_md_h = gasnetc_alloc_tmpmd(source_addr, nbytes);		\
	th->tmpmd_tickets--;						\
	GASNETI_TRACE_EVENT(C, LONG_TMPMD);				\
      }									\
      if (do_sync) {							\
	data_mbits |= ( (ptl_match_bits_t)GASNETC_PTL_AM_SYNC  << 8);	\
	GASNETC_INC_INFLIGHT(&th->amlongReq_data_inflight);		\
      }									\
      GASNETC_PTLSAFE(PtlPutRegion(data_md_h, data_offset, nbytes, PTL_NOACK_REQ, target_id, GASNETC_PTL_RAR_PTE, ac_index, data_mbits, data_rmt_offset, data_hdr_data)); \
									\
      GASNETC_DBGMSG(1,1,"L",gasneti_mynode,dest,handler,numargs,hargs,msg_bytes,cred_byte,nbytes,source_addr,th); \
      GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, msg_bytes, PTL_NOACK_REQ, target_id, GASNETC_PTL_AM_PTE, ac_index, mbits, remote_offset, hdr_data)); \
									\
      th->snd_tickets -= 2;						\
      th->snd_credits -= ncredit;					\
									\
      /* now wait for data put to complete locally */			\
      if (do_sync) {							\
	gasneti_pollwhile(GASNETC_TEST_INFLIGHT(&th->amlongReq_data_inflight)); \
      }									\
    }									\
  } while(0)

#define AM_LONG_REQUEST_LOOPBACK_CHECK() do {				\
    if (dest == gasneti_mynode) {					\
      gasnetc_ptl_token_t tok;						\
      gasnet_token_t      token = (gasnet_token_t)&tok;			\
      GASNETC_AM_LOOPBACK_COMMON(tok.args);				\
      tok.srcnode = gasneti_mynode;					\
      tok.initiator_offset = 0;						\
      memcpy(dest_addr,source_addr,nbytes);				\
      GASNETI_TRACE_PRINTF(C,("AM_LOOPBACK: L_Req handler=%d, narg=%d nbytes=%d",handler,numargs,(int)nbytes));	\
      GASNETC_DBGMSG(1,1,"L",gasneti_mynode,dest,handler,numargs,tok.args,0,cred_byte,nbytes,source_addr,th); \
      GASNETI_RUN_HANDLER_LONG(1, handler, gasnetc_handler[handler], token, tok.args, numargs, dest_addr, nbytes); \
      gasneti_AMPoll();							\
      GASNETI_RETURN(GASNET_OK);					\
    }									\
  } while(0)

extern int gasnetc_AMRequestLongM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  ptl_size_t           local_offset;
  gasnetc_conn_t      *state = gasnetc_conn_state + dest;
  int                  isPacked, msg_bytes, nsend, ncredit, ntmpmd;
  uint8_t              cred_byte = 0;
  gasnetc_threaddata_t *th = gasnetc_mythread();

  GASNETC_DEF_HARGS();  /* debugging, must be first statement */

  GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);

  /* if loopback, run handler and return */
  AM_LONG_REQUEST_LOOPBACK_CHECK();

  /* compute message len and allocate resources needed to send message */
  gasneti_assert(th->snd_credits == 0);
  AM_LONG_COMPUTE_RESOURCES(dest,isPacked,msg_bytes,nsend,ncredit,ntmpmd);
  if (!gasnetc_use_flow_control) gasneti_assert(ncredit == 0);
  if (! isPacked) gasneti_assert(nsend == 2);

  /* poll until ok to send message, allocate ReqSB chunk */
  GASNETC_COMMON_AMREQ_START(state,local_offset,th,nsend,ncredit,cred_byte);

  /* do all the work in sending the messages(s) */
  AM_LONG_COMMON(1 /*do_sync*/,isPacked,th,cred_byte);
  
  /* Always poll at end of AM Request */
  gasneti_AMPoll();

  /* release any allocated but unused resources here */
  AM_RELEASE_RESOURCES(th);

  GASNETI_RETURN(GASNET_OK);
}
  

extern int gasnetc_AMRequestLongAsyncM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  ptl_size_t           local_offset;
  gasnetc_conn_t      *state = gasnetc_conn_state + dest;
  int                  isPacked, msg_bytes, nsend, ncredit, ntmpmd;
  uint8_t              cred_byte = 0;
  gasnetc_threaddata_t *th = gasnetc_mythread();

  GASNETC_DEF_HARGS();  /* debugging, must be first statement */

  GASNETI_COMMON_AMREQUESTLONGASYNC(dest,handler,source_addr,nbytes,dest_addr,numargs);

  /* if loopback, run handler and return */
  AM_LONG_REQUEST_LOOPBACK_CHECK();

  /* compute message len and number of resources needed to send message */
  gasneti_assert(th->snd_credits == 0);
  AM_LONG_COMPUTE_RESOURCES(dest,isPacked,msg_bytes,nsend,ncredit,ntmpmd);
  if (!gasnetc_use_flow_control) gasneti_assert(ncredit == 0);

  /* poll until all required resources are allocated */
  GASNETC_COMMON_AMREQ_START(state,local_offset,th,nsend,ncredit,cred_byte);

  /* do all the work in sending the messages(s) */
  AM_LONG_COMMON(0 /*do_sync*/,isPacked,th,cred_byte);
  
  /* Always poll at end of AM Request */
  gasneti_AMPoll();

  /* release any allocated but unused resources here */
  AM_RELEASE_RESOURCES(th);

  GASNETI_RETURN(GASNET_OK);
}

#undef AM_LONG_COMPUTE_RESOURCES
#undef AM_LONG_COMMON

extern int gasnetc_AMReplyShortM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  gasnetc_ptl_token_t *ptok = (gasnetc_ptl_token_t*)token;
  ptl_size_t           local_offset;
  ptl_size_t           remote_offset = ptok->initiator_offset;
  ptl_handle_md_t      md_h = gasnetc_RplSB.md_h;
  ptl_process_id_t     target_id = ptok->initiator;
  ptl_ac_index_t       ac_index = GASNETC_PTL_AC_ID;
  uint64_t             amtype = GASNETC_PTL_AM_SHORT;
  uint64_t             targ_mbits = GASNETC_PTL_REQSB_BITS | GASNETC_PTL_MSG_AM;
  ptl_match_bits_t     mbits;
  ptl_hdr_data_t       hdr_data = 0;
  ptl_size_t           msg_bytes;
  uint32_t            *data32;
  gasnetc_threaddata_t *th = gasnetc_mythread();

  GASNETC_DEF_HARGS();       /* debugging, must be first statement */
  GASNETC_GET_SEQNO(ptok);   /* debug */

  GASNETI_COMMON_AMREPLYSHORT(token,handler,numargs);

  /* handle loopback case */
  if (ptok->srcnode == gasneti_mynode) {
    gasnet_handlerarg_t args[gasnet_AMMaxArgs()];
    GASNETC_AM_LOOPBACK_COMMON(args);
    GASNETI_TRACE_PRINTF(C,("AM_LOOPBACK: S_Rpl handler=%d, narg=%d",handler,numargs));
    GASNETC_DBGMSG(1,0,"S",gasneti_mynode,ptok->srcnode,handler,numargs,args,0,ptok->credits,0,NULL,th);
    GASNETI_RUN_HANDLER_SHORT(0, handler, gasnetc_handler[handler], token, args, numargs);
    GASNETI_RETURN(GASNET_OK);
  }

  /* AM Short Reply Data Format:
   *     HD=[srcnode,cred] MB=[off,XX] Data=[args][seqno]
   * NOTE: seqno included only in debug mode
   */

  data32 = claim_rplsb(th);
  local_offset = (ptl_size_t)((uintptr_t)data32 - (uintptr_t)gasnetc_RplSB.start);

  GASNETC_AM_HEADER_COMMON(ptok->credits);

  /* already allocated a send ticket */
  gasneti_assert(th->snd_tickets > 0);
  th->snd_tickets--;

  msg_bytes = numargs * sizeof(gasnet_handlerarg_t);
  GASNETC_SEQNO_MSGLEN(msg_bytes);
  gasneti_assert(msg_bytes <= GASNETC_CHUNKSIZE);

  GASNETC_DBGMSG(1,0,"S",gasneti_mynode,ptok->srcnode,handler,numargs,hargs,msg_bytes,ptok->credits,0,NULL,th);

  /* send message */
  GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, msg_bytes, PTL_NOACK_REQ, target_id, GASNETC_PTL_AM_PTE, ac_index, mbits, remote_offset, hdr_data));

  /* Indicate to reply code that AM did in fact send a reply message */
  ptok->need_reply = 0;

  GASNETI_RETURN(GASNET_OK);
}

extern int gasnetc_AMReplyMediumM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  gasnetc_ptl_token_t  *ptok = (gasnetc_ptl_token_t*)token;
  ptl_size_t            local_offset;
  ptl_size_t            remote_offset = ptok->initiator_offset;
  ptl_handle_md_t       md_h = gasnetc_RplSB.md_h;
  ptl_process_id_t      target_id = ptok->initiator;
  ptl_ac_index_t        ac_index = GASNETC_PTL_AC_ID;
  uint64_t              amtype = GASNETC_PTL_AM_MEDIUM;
  uint64_t              targ_mbits = GASNETC_PTL_REQSB_BITS | GASNETC_PTL_MSG_AM;
  uint32_t              len_cred;
  ptl_match_bits_t      mbits;
  ptl_hdr_data_t        hdr_data = 0;
  ptl_size_t            msg_bytes;
  uint32_t             *data32;
  gasnetc_threaddata_t *th = gasnetc_mythread();

  GASNETC_DEF_HARGS();       /* debugging, must be first statement */
  GASNETC_GET_SEQNO(ptok);   /* debug */

  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,numargs);

  /* handle loopback case */
  if (ptok->srcnode == gasneti_mynode) {
    gasnet_handlerarg_t args[gasnet_AMMaxArgs()];
    void *tmpdata = gasnetc_alloc_tmp(nbytes);
    GASNETC_AM_LOOPBACK_COMMON(args);
    gasneti_assert(!((uintptr_t)tmpdata & 7)); /* x86_64 ABI ensures this, alloca() does not */
    /* dont allow handler to modify source memory */
    memcpy(tmpdata,source_addr,nbytes);
    GASNETI_TRACE_PRINTF(C,("AM_LOOPBACK: M_Rpl handler=%d, narg=%d nbytes=%d",handler,numargs,(int)nbytes));
    GASNETC_DBGMSG(1,0,"M",gasneti_mynode,ptok->srcnode,handler,numargs,args,0,ptok->credits,nbytes,source_addr,th);
    GASNETI_RUN_HANDLER_MEDIUM(0, handler, gasnetc_handler[handler], token, args, numargs, tmpdata, nbytes);
    gasnetc_free_tmp(tmpdata);
    GASNETI_RETURN(GASNET_OK);
  }

  /* Medium Reply Data Format:
   *     HD=[srcnode,len:cred] MB=[off,XX] Data=[args][seqno][pad][data]
   * NOTE: seqno included only in debug mode
   */

  data32 = claim_rplsb(th);
  local_offset = (ptl_size_t)((uintptr_t)data32 - (uintptr_t)gasnetc_RplSB.start);
  len_cred = ptok->credits | (nbytes << 8);

  GASNETC_AM_HEADER_COMMON(len_cred);

  msg_bytes = numargs  * sizeof(gasnet_handlerarg_t);
  GASNETC_SEQNO_MSGLEN(msg_bytes);

  /* pad to that data payload is correctly aligned */
  msg_bytes = GASNETI_ALIGNUP(msg_bytes,GASNETI_MEDBUF_ALIGNMENT);

  /* copy handler data payload here (aligned) */
  memcpy((void*)GASNETI_ALIGNUP(data32,GASNETI_MEDBUF_ALIGNMENT),source_addr,nbytes);
  msg_bytes += nbytes;

  /* already allocated a send ticket */
  gasneti_assert(th->snd_tickets > 0);
  th->snd_tickets--;

  gasneti_assert(msg_bytes <= GASNETC_CHUNKSIZE);

  GASNETC_DBGMSG(1,0,"M",gasneti_mynode,ptok->srcnode,handler,numargs,hargs,msg_bytes,ptok->credits,nbytes,source_addr,th);

  /* send message */
  GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, msg_bytes, PTL_NOACK_REQ, target_id, GASNETC_PTL_AM_PTE, ac_index, mbits, remote_offset, hdr_data));

  /* Indicate to reply code that AM did in fact send a reply message */
  ptok->need_reply = 0;

  GASNETI_RETURN(GASNET_OK);
}

extern int gasnetc_AMReplyLongM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  gasnetc_ptl_token_t  *ptok = (gasnetc_ptl_token_t*)token;
  ptl_size_t            local_offset;
  ptl_size_t            remote_offset = ptok->initiator_offset;
  ptl_handle_md_t       md_h = gasnetc_RplSB.md_h;
  ptl_process_id_t      target_id = ptok->initiator;
  ptl_ac_index_t        ac_index = GASNETC_PTL_AC_ID;
  uint64_t              amtype = GASNETC_PTL_AM_LONG;
  uint64_t              targ_mbits = GASNETC_PTL_REQSB_BITS | GASNETC_PTL_MSG_AM;
  uint32_t              lid, cred_word;
  int                   isPacked = 0;
  ptl_match_bits_t      mbits;
  ptl_hdr_data_t        hdr_data = 0;
  ptl_size_t            msg_bytes;
  uint32_t             *data32;
  gasnet_node_t         dest = ptok->srcnode;
  gasnetc_threaddata_t *th = gasnetc_mythread();

  GASNETC_DEF_HARGS();       /* debugging, must be first statement */
  GASNETC_GET_SEQNO(ptok);   /* debug */

  GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,numargs); 

  /* handle loopback case */
  if (dest == gasneti_mynode) {
    gasnet_handlerarg_t args[gasnet_AMMaxArgs()];
    GASNETC_AM_LOOPBACK_COMMON(args);
    memcpy(dest_addr,source_addr,nbytes);
    GASNETI_TRACE_PRINTF(C,("AM_LOOPBACK: L_Rpl handler=%d, narg=%d nbytes=%d",handler,numargs,(int)nbytes));
    GASNETC_DBGMSG(1,0,"L",gasneti_mynode,ptok->srcnode,handler,numargs,args,0,ptok->credits,nbytes,source_addr,th);
    GASNETI_RUN_HANDLER_LONG(0, handler, gasnetc_handler[handler], token, args, numargs, dest_addr, nbytes);
    GASNETI_RETURN(GASNET_OK);
  }

  /* Regular Format: HD=[srcnode,lid:cred] MB=[off,XXX] data=[args][seqno]
   * Packed  Format: HD=[srcnode,len:cred] MB=[off,XXX] data=[args][seqno][destaddr][data]
   * seqno only in debug mode
   */

  data32 = claim_rplsb(th);
  local_offset = (ptl_size_t)((uintptr_t)data32 - (uintptr_t)gasnetc_RplSB.start);

  msg_bytes = numargs * sizeof(gasnet_handlerarg_t);
  GASNETC_SEQNO_MSGLEN(msg_bytes);

  if (gasnetc_allow_packed_long) {
    const int max_pack = GASNETC_CHUNKSIZE - sizeof(void*);
    isPacked = (msg_bytes + nbytes <= max_pack);
  }

  cred_word = ptok->credits;
  if (isPacked) {
    amtype |= GASNETC_PTL_AM_PACKED;
    cred_word |= nbytes << 8;
  } else {
    lid = gasnetc_new_lid(dest);
    cred_word |= lid << 8;
  }

  GASNETC_AM_HEADER_COMMON(cred_word);

  if (isPacked) {

    /* pack the target node RAR destination address for this message */
    *(data32++) = GASNETI_LOWORD(dest_addr);
    *(data32++) = GASNETI_HIWORD(dest_addr);
    msg_bytes += 2*sizeof(uint32_t);

    /* copy the data payload */
    memcpy(data32,source_addr,nbytes);
    msg_bytes += nbytes;

    gasneti_assert(msg_bytes <= GASNETC_CHUNKSIZE);

    /* already allocated a send ticket */
    gasneti_assert(th->snd_tickets > 0);
    th->snd_tickets--;

    GASNETC_DBGMSG(1,0,"L",gasneti_mynode,ptok->srcnode,handler,numargs,hargs,msg_bytes,ptok->credits,nbytes,source_addr,th);

    /* send message */
    GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, msg_bytes, PTL_NOACK_REQ, target_id, GASNETC_PTL_AM_PTE, ac_index, mbits, remote_offset, hdr_data));
    
  } else {

    /* AM Reply is always executed while polling, need to wait until data payload is
     * off-node before returning, but dont want to poll recursively.
     * We are able to follow the same pattern as the RequestLong, but only because
     * both the RARSRC MD and the possible TMP_MD both use the SAFE_EQ.
     * NOTE: Data Reply sent to RARSRC (not RARAM) MD !!!
     */
    ptl_handle_md_t dp_md_h;
    ptl_match_bits_t dp_mbits = 0;
    ptl_size_t remote_dataoffset = GASNETC_PTL_OFFSET(dest,dest_addr);
    ptl_hdr_data_t dp_hdr_data = GASNETI_MAKEWORD(gasneti_mynode, lid);
    ptl_size_t dp_offset = 0;

    /* already allocated a send ticket, spend them now */
    gasneti_assert(th->snd_tickets > 1);
    th->snd_tickets -= 2;

    if (gasnetc_in_local_rar(source_addr,nbytes)) {
      dp_md_h = gasnetc_RARSRC.md_h;
      dp_offset = GASNETC_PTL_OFFSET(gasneti_mynode,source_addr);
    } else GASNETC_IF_USE_FIREHOSE (
      gasnetc_fh_op_t *op = gasnetc_fh_aligned_local_pin(source_addr, nbytes);
      const firehose_request_t *fh_loc = &op->fh[0];
      dp_md_h = fh_loc->client;
      dp_offset = (uintptr_t)source_addr - fh_loc->addr;
      dp_mbits |= ((ptl_match_bits_t)(op->addr.fulladdr) << 32);
      gasneti_assert(nbytes <= (fh_loc->len - dp_offset));
    ) else {
      gasneti_assert(th->tmpmd_tickets > 0);
      dp_md_h = gasnetc_alloc_tmpmd(source_addr, nbytes);
      th->tmpmd_tickets--;
    }

    /* issue data put message */
    /* NOTE: dp_mbits explicitly does not include the REQUEST flag, since this is a reply */
    dp_mbits |= GASNETC_PTL_MSG_AMDATA | GASNETC_PTL_RARSRC_BITS | (GASNETC_PTL_AM_SYNC << 8);
    dp_mbits |= ((ptl_match_bits_t)(th->threadidx) << 24);
    dp_mbits |= ((ptl_match_bits_t)(handler) << 48);
    dp_mbits |= ((ptl_match_bits_t)(numargs) << 56);
    GASNETC_INC_INFLIGHT(&th->amlongRep_data_inflight);
    GASNETC_PTLSAFE(PtlPutRegion(dp_md_h, dp_offset, nbytes, PTL_NOACK_REQ, target_id, GASNETC_PTL_RAR_PTE, ac_index, dp_mbits, remote_dataoffset, dp_hdr_data));

    gasneti_assert(msg_bytes <= GASNETC_CHUNKSIZE);

    GASNETC_DBGMSG(1,0,"L",gasneti_mynode,ptok->srcnode,handler,numargs,hargs,msg_bytes,ptok->credits,nbytes,source_addr,th);

    /* send message */
    GASNETC_PTLSAFE(PtlPutRegion(md_h, local_offset, msg_bytes, PTL_NOACK_REQ, target_id, GASNETC_PTL_AM_PTE, ac_index, mbits, remote_offset, hdr_data));

    /* poll ONLY the safe eq until local completion of data put */
    while(GASNETC_TEST_INFLIGHT(&th->amlongRep_data_inflight)) {
      gasnetc_portals_poll(GASNETC_SAFE_POLL);
    }
  }

  /* Indicate to reply code that AM did in fact send a reply message */
  ptok->need_reply = 0;

  GASNETI_RETURN(GASNET_OK);
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
#if GASNETC_USE_INTERRUPTS
  #error interrupts not implemented
  extern void gasnetc_hold_interrupts(void) {
    GASNETI_CHECKATTACH();
    /* add code here to disable handler interrupts for _this_ thread */
  }
  extern void gasnetc_resume_interrupts(void) {
    GASNETI_CHECKATTACH();
    /* add code here to re-enable handler interrupts for _this_ thread */
  }
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Handler-safe locks
  ==================
*/
#if !GASNETC_NULL_HSL
extern void gasnetc_hsl_init   (gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();
  gasneti_mutex_init(&(hsl->lock));

  #if GASNETC_USE_INTERRUPTS
    /* add code here to init conduit-specific HSL state */
    #error interrupts not implemented
  #endif
}

extern void gasnetc_hsl_destroy(gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();
  gasneti_mutex_destroy(&(hsl->lock));

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
    #if GASNETC_HSL_SPINLOCK
      if_pf (gasneti_mutex_trylock(&(hsl->lock)) == EBUSY) {
        if (gasneti_wait_mode == GASNET_WAIT_SPIN) {
          while (gasneti_mutex_trylock(&(hsl->lock)) == EBUSY) {
            gasneti_compiler_fence();
            gasneti_spinloop_hint();
          }
        } else {
          gasneti_mutex_lock(&(hsl->lock));
        }
      }
    #else
      gasneti_mutex_lock(&(hsl->lock));
    #endif
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

  gasneti_mutex_unlock(&(hsl->lock));
}

extern int  gasnetc_hsl_trylock(gasnet_hsl_t *hsl) {
  GASNETI_CHECKATTACH();

  {
    int locked = (gasneti_mutex_trylock(&(hsl->lock)) == 0);

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
#endif
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
    gasneti_handler_tableentry_with_bits(gasnetc_noop_reph),

  { 0, NULL }
};

gasnet_handlerentry_t const *gasnetc_get_handlertable(void) {
  return gasnetc_handlers;
}

/* ------------------------------------------------------------------------------------ */
#if GASNETC_CREDIT_TESTING
/* Routines that allow external client access to some credit values for debugging/profiling */
int gasnet_send_credits(gasnet_node_t node)
{
  gasnetc_conn_t *state = &gasnetc_conn_state[node];
  int val;
  GASNETC_LOCK_STATE(state);
  val = state->SendCredits;
  GASNETC_UNLOCK_STATE(state);
  return val;
}
int gasnet_credit_stalls(gasnet_node_t node)
{
  gasnetc_conn_t *state = &gasnetc_conn_state[node];
  int val;
  GASNETC_LOCK_STATE(state);
  val = state->SendStalls_tot;
  GASNETC_UNLOCK_STATE(state);
  return val;
}
int gasnet_banked_credits(void)
{
  return gasneti_semaphore_read(&gasnetc_banked_credits);
}
int gasnet_revoked_credits(gasnet_node_t node)
{
  gasnetc_conn_t *state = &gasnetc_conn_state[node];
  int val;
  GASNETC_LOCK_STATE(state);
  val = state->LoanRevoked_tot;
  GASNETC_UNLOCK_STATE(state);
  return val;
}
#endif
