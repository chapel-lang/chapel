/*   $Source: bitbucket.org:berkeleylab/gasnet.git/portals4-conduit/gasnet_core.c $
 * Description: GASNet portals4 conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include "gasnet_portals4.h"

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

gasnet_handlerentry_t const *gasnetc_get_handlertable(void);
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int, void*);
#else
static void gasnetc_atexit(void);
#endif

gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS]; /* handler table (recommended impl) */


/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/

/* called at startup to check configuration sanity */
static void gasnetc_check_config(void) {
  gasneti_check_config_preinit();
}


static int gasnetc_init(int *argc, char ***argv) {
    int ret;
    
    /*  check system sanity */
    gasnetc_check_config();

    if (gasneti_init_done) 
        GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");

    gasneti_freezeForDebugger();

#if GASNET_DEBUG_VERBOSE
    /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
    fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
#endif

#if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
            gasneti_mynode, gasneti_nodes); fflush(stderr);
#endif

    /* gasnetc_p4_init has barrier semantics such that
       gasnetc_bootstrapBarrier and gasnetc_bootstrapExchange will
       work upon return of init. */
    ret = gasnetc_p4_init(&gasneti_mynode, &gasneti_nodes);
    if (GASNET_OK != ret) gasneti_fatalerror("gasnetc_p4_init failed: %d\n", ret);

    /* node map set in gasnetc_p4_init */

#if GASNET_PSHM
    gasneti_pshm_init(&gasnetc_bootstrapExchange, 0);
#endif

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
  { uintptr_t limit;
    limit = gasneti_mmapLimit((uintptr_t)-1, (uint64_t)-1,
                              &gasnetc_bootstrapExchange,
                              &gasnetc_bootstrapBarrier);
    gasneti_segmentInit(limit, &gasnetc_bootstrapExchange);
  }
#elif GASNET_SEGMENT_EVERYTHING
    /* segment is everything - nothing to do */
#else
#error Bad segment config
#endif

    gasneti_init_done = 1;  

    gasneti_auxseg_init(); /* adjust max seg values based on auxseg */

    return GASNET_OK;
}


/* ------------------------------------------------------------------------------------ */
extern int gasnet_init(int *argc, char ***argv) {
  int retval = gasnetc_init(argc, argv);
  if (retval != GASNET_OK) GASNETI_RETURN(retval);
  gasneti_trace_init(argc, argv);
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
        snprintf(s, sizeof(s), "Too many handlers. (limit=%i)", highlimit - lowlimit + 1);
        GASNETI_RETURN_ERRR(BAD_ARG, s);
      }
    }

    /*  ensure handlers fall into the proper range of pre-assigned values */
    if (newindex < lowlimit || newindex > highlimit) {
      char s[255];
      snprintf(s, sizeof(s), "handler index (%i) out of range [%i..%i]", newindex, lowlimit, highlimit);
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
  int ret;
  
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
    int numreg = 0;
    gasneti_assert(ctable);
    while (ctable[len].fnptr) len++; /* calc len */
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

    gasneti_assert(numreg1 + numreg2 == numentries);
  }

  /* ------------------------------------------------------------------------------------ */
  /*  register fatal signal handlers */

  /* catch fatal signals and convert to SIGQUIT */
  gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

  #if HAVE_ON_EXIT
    on_exit(gasnetc_on_exit, NULL);
  #else
    atexit(gasnetc_atexit);
  #endif

  /* ------------------------------------------------------------------------------------ */
  /*  register segment  */

  gasneti_seginfo = (gasnet_seginfo_t *)gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));
  gasneti_leak(gasneti_seginfo);

  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    if (segsize == 0) segbase = NULL; /* no segment */
    else {
      gasneti_segmentAttach(segsize, minheapoffset, gasneti_seginfo, &gasnetc_bootstrapExchange);
      segbase = gasneti_seginfo[gasneti_mynode].addr;
      segsize = gasneti_seginfo[gasneti_mynode].size;
      gasneti_assert(((uintptr_t)segbase) % GASNET_PAGESIZE == 0);
      gasneti_assert(segsize % GASNET_PAGESIZE == 0);
    }
  #else
  { /* GASNET_SEGMENT_EVERYTHING */
    gasnet_node_t i;
    for (i=0; i<gasneti_nodes; i++) {
      gasneti_seginfo[i].addr = (void *)0;
      gasneti_seginfo[i].size = (uintptr_t)-1;
    }
    segbase = (void *)0;
    segsize = (uintptr_t)-1;
  }
  #endif
  ret = gasnetc_p4_attach(segbase, segsize);
  if (GASNET_OK != ret) {
      GASNETI_RETURN_ERRR(RESOURCE,"Error attaching Portals4 resources");
  }

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

  gasnete_init(); /* init the extended API */

  gasneti_nodemapFini();

  /* ensure extended API is initialized across nodes */
  gasnetc_bootstrapBarrier();

  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */

static int gasnetc_exit_in_progress = 0;

#if HAVE_ON_EXIT
static void gasnetc_on_exit(int exitcode, void *arg) {
  if (!gasnetc_exit_in_progress)
    gasnetc_exit(exitcode);
}
#else
static void gasnetc_atexit(void) {
  if (!gasnetc_exit_in_progress)
    gasnetc_exit(0);
}
#endif

extern void gasnetc_exit(int exitcode) {
  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  gasneti_reghandler(SIGQUIT, SIG_IGN);
  gasnetc_exit_in_progress = 1;

  {  /* ensure only one thread ever continues past this point */
    static gasneti_mutex_t exit_lock = GASNETI_MUTEX_INITIALIZER;
    gasneti_mutex_lock(&exit_lock);
  }

  GASNETI_TRACE_PRINTF(C,("gasnet_exit(%i)\n", exitcode));

  gasnetc_p4_exit();

  gasneti_flush_streams();
  gasneti_trace_finish();
  gasneti_sched_yield();

  gasneti_killmyprocess(exitcode);
  gasneti_fatalerror("gasnetc_exit failed!");
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

#if GASNET_PSHM
  /* If your conduit will support PSHM, let the PSHM code
   * have a chance to recognize the token first, as shown here. */
  if (gasneti_AMPSHMGetMsgSource(token, &sourceid) != GASNET_OK)
#endif
  {
      gasnetc_p4_token_t *p4_token = (gasnetc_p4_token_t*) token;
      sourceid = p4_token->sourceid;
  }

  gasneti_assert(sourceid < gasneti_nodes);
  *srcindex = sourceid;
  return GASNET_OK;
}

extern int gasnetc_AMPoll(void) {
  int retval;
  GASNETI_CHECKATTACH();

#if GASNET_PSHM
  /* If your conduit will support PSHM, let it make progress here. */
  gasneti_AMPSHMPoll(0);
#endif

  gasnetc_p4_poll();

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
  va_list argptr;
  GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs);
  gasneti_AMPoll(); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Short, dest, handler,
                                           0, 0, 0,
                                           numargs, argptr);
  } else
#endif
  {
      retval = gasnetc_p4_TransferGeneric(gasnetc_Short, AM_REQUEST, dest, handler,
                                          NULL, 0, NULL,
                                          numargs, argptr);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestMediumM( 
                            gasnet_node_t dest,      /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);
  gasneti_AMPoll(); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Medium, dest, handler,
                                           source_addr, nbytes, 0,
                                           numargs, argptr);
  } else
#endif
  {
      retval = gasnetc_p4_TransferGeneric(gasnetc_Medium, AM_REQUEST, dest, handler,
                                          source_addr, nbytes, 0,
                                          numargs, argptr);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestLongM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);
  gasneti_AMPoll(); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                                           source_addr, nbytes, dest_addr,
                                           numargs, argptr);
  } else
#endif
  {
      retval = gasnetc_p4_TransferGeneric(gasnetc_Long, AM_REQUEST, dest, handler,
                                          source_addr, nbytes, dest_addr,
                                          numargs, argptr);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestLongAsyncM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTLONGASYNC(dest,handler,source_addr,nbytes,dest_addr,numargs);
  gasneti_AMPoll(); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                                           source_addr, nbytes, dest_addr,
                                           numargs, argptr);
  } else
#endif
  {
      retval = gasnetc_p4_TransferGeneric(gasnetc_LongAsync, AM_REQUEST, dest, handler,
                                          source_addr, nbytes, dest_addr,
                                          numargs, argptr);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyShortM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREPLYSHORT(token,handler,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* If your conduit will support PSHM, let it check the token first. */
  if_pt (gasnetc_token_is_pshm(token)) {
    retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Short, token, handler,
                                         0, 0, 0,
                                         numargs, argptr);
  } else
#endif
  { 
      gasnetc_p4_token_t *p4_token = (gasnetc_p4_token_t*) token;
      if (p4_token->reply_sent != 0) return GASNET_ERR_BAD_ARG;
      p4_token->reply_sent = 1;
      retval = gasnetc_p4_TransferGeneric(gasnetc_Short, AM_REPLY, p4_token->sourceid, handler,
                                          0, 0, 0,
                                          numargs, argptr);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyMediumM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* If your conduit will support PSHM, let it check the token first. */
  if_pt (gasnetc_token_is_pshm(token)) {
    retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Medium, token, handler,
                                         source_addr, nbytes, 0,
                                         numargs, argptr);
  } else
#endif
  {
      gasnetc_p4_token_t *p4_token = (gasnetc_p4_token_t*) token;
      if (p4_token->reply_sent != 0) return GASNET_ERR_BAD_ARG;
      p4_token->reply_sent = 1;
      retval = gasnetc_p4_TransferGeneric(gasnetc_Medium, AM_REPLY, p4_token->sourceid, handler,
                                          source_addr, nbytes, 0,
                                          numargs, argptr);
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyLongM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval;
  va_list argptr;
  GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,numargs); 
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* If your conduit will support PSHM, let it check the token first. */
  if_pt (gasnetc_token_is_pshm(token)) {
    retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Long, token, handler,
                                         source_addr, nbytes, dest_addr,
                                         numargs, argptr);
  } else
#endif
  {
      gasnetc_p4_token_t *p4_token = (gasnetc_p4_token_t*) token;
      if (p4_token->reply_sent != 0) return GASNET_ERR_BAD_ARG;
      p4_token->reply_sent = 1;
      retval = gasnetc_p4_TransferGeneric(gasnetc_Long, AM_REPLY, p4_token->sourceid, handler,
                                          source_addr, nbytes, dest_addr,
                                          numargs, argptr);
  }
  va_end(argptr);
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

  { 0, NULL }
};

gasnet_handlerentry_t const *gasnetc_get_handlertable(void) {
  return gasnetc_handlers;
}
