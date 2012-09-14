/*   $Source: /var/local/cvs/gasnet/gemini-conduit/gasnet_core.c,v $
 *     $Date: 2012/05/13 21:08:50 $
 * $Revision: 1.25 $
 * Description: GASNet gemini conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Gemini conduit by Larry Stewart <stewart@serissa.com>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>
#include <gasnet_gemini.h>
/* #include <alps/libalpslli.h> */

#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <sys/mman.h>

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

  /* (###) add code to do some sanity checks on the number of nodes, handlers
   * and/or segment sizes */ 

  /* Request and Reply must be distinguishable by least-significant bit: */
  gasneti_assert(1 == (1 & GC_CMD_AM_SHORT));
  gasneti_assert(1 == (1 & GC_CMD_AM_MEDIUM));
  gasneti_assert(1 == (1 & GC_CMD_AM_LONG));
  gasneti_assert(0 == (1 & GC_CMD_AM_SHORT_REPLY));
  gasneti_assert(0 == (1 & GC_CMD_AM_MEDIUM_REPLY));
  gasneti_assert(0 == (1 & GC_CMD_AM_LONG_REPLY));
}

static void gasnetc_bootstrapBarrier(void) {
  /* LCS */
  PMI_Barrier();
}

static void gasnetc_bootstrapExchange(void *src, size_t len, void *dest) {
  gasnetc_GNIT_Allgather(src, len, dest);
}


/* code from portals_conduit */
/* ---------------------------------------------------------------------------------
 * Helpers for try_pin() and gasnetc_portalsMaxPinMem()
 * --------------------------------------------------------------------------------- */
static void *try_pin_region = NULL;
static uintptr_t try_pin_size = 0;

#if HAVE_MMAP
  static void *try_pin_alloc_inner(const uintptr_t size) {
    void *addr = gasneti_mmap(size);
    if (addr == MAP_FAILED) addr = NULL;
    return addr;
  }
  static void try_pin_free_inner(void *addr, const uintptr_t size) {
    gasneti_munmap(addr, size);
  }
#else
  static void *try_pin_alloc_inner(const uintptr_t size) {
    void *addr = gasneti_malloc_allowfail(size);
    return addr;
  }
  static void try_pin_free_inner(void *addr, const uintptr_t size) {
    gasneti_free(addr);
  }
#endif

static uintptr_t try_pin_alloc(uintptr_t size, const uintptr_t step) {
  void *addr = try_pin_alloc_inner(size);

  if (!addr) {
    /* Binary search */
    uintptr_t high = size;
    uintptr_t low = step;
    int found = 0;

    while ((high - low) > step) {
      uint64_t mid = (low + high)/2;
      addr = try_pin_alloc_inner(mid);
      if (addr) {
        try_pin_free_inner(addr, mid);
        low = mid;
        found = 1;
      } else {
        high = mid;
      }
    }

    if (!found) return 0;

    size = low;
    addr = try_pin_alloc_inner(low);
    gasneti_assert_always(addr);
  }

  try_pin_region = addr;
  try_pin_size = size;
  return size;
}

static void try_pin_free(void) {
  try_pin_free_inner(try_pin_region, try_pin_size);
  try_pin_region = NULL;
  try_pin_size = 0;
}

/* ---------------------------------------------------------------------------------
 * Determine the largest amount of memory that can be pinned on the node.
 * --------------------------------------------------------------------------------- */
extern uintptr_t gasnetc_portalsMaxPinMem(uintptr_t msgspace)
{
#define MBYTE 1048576ULL
  uintptr_t granularity = 16ULL * MBYTE;
  uintptr_t low;
  uintptr_t high;
  uintptr_t limit = 16ULL * 1024ULL * MBYTE;
#undef MBYTE

  /* On CNL, if we try to pin beyond what the OS will allow, the job is killed.
   * So, there is really no way (that we know of) to determine the EXACT maximum
   * pinnable memory under CNL without dire consequences.
   * For this platform, we will simply try a large fraction of the physical
   * memory.  If that is too big, then the job will be killed at startup.
   * The gasneti_mmapLimit() ensures limit is per compute node, not per process.
   */
  uintptr_t pm_limit = gasneti_getPhysMemSz(1) *
                      gasneti_getenv_dbl_withdefault(
                        "GASNET_PHYSMEM_PINNABLE_RATIO", 
                        GASNETC_DEFAULT_PHYSMEM_PINNABLE_RATIO);

  pm_limit = gasneti_getenv_int_withdefault("GASNET_PHYSMEM_MAX", pm_limit, 1);

  msgspace *= gasneti_nodemap_local_count;
  if (pm_limit < msgspace || (pm_limit - msgspace) < (granularity * gasneti_nodemap_local_count)) {
    gasneti_fatalerror("Insufficient physical memory left for a GASNet segment");
  }
  pm_limit -= msgspace;

  limit = gasneti_mmapLimit(limit, pm_limit,
                            &gasnetc_bootstrapExchange,
                            &gasnetc_bootstrapBarrier);


  if_pf (gasneti_getenv_yesno_withdefault("GASNET_PHYSMEM_NOPROBE", 0)) {
    /* User says to trust them... */
    return (uintptr_t)limit;
  }

  /* Allocate a block of memory on which to try pinning */
  high = try_pin_alloc(limit, granularity);
  low = high;
  /* Free the block we've been pinning */
  try_pin_free();

  if (low < granularity) {
    gasnetc_GNIT_Abort("Unable to alloc and pin minimal memory of size %d bytes",(int)granularity);
  }
  GASNETI_TRACE_PRINTF(C,("MaxPinMem = %lu",(unsigned long)low));
  return (uintptr_t)low;
}



static int gasnetc_init(int *argc, char ***argv) {
  uintptr_t msgspace;
  char *errstring;
  int ret;
  int localranks;
  uint32_t  minlocalrank;
  uint32_t i;

#if GASNETC_DEBUG
  fprintf(stderr, "Entering gasnetc_init\n"); fflush(stderr);
#endif
  /*  check system sanity */
  gasnetc_check_config();
  
  if (gasneti_init_done) 
    GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");

  gasneti_init_done = 1; /* enable early to allow tracing */

  gasneti_freezeForDebugger();

#if GASNET_DEBUG_VERBOSE
    /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
    fprintf(stderr,"gasnetc_init(): about to call gasnetc_init...\n"); fflush(stderr);
#endif

    /* LCS Gemini init */

    if ((ret = gasnetc_gem_init(&errstring)) != GASNET_OK) {
      GASNETI_RETURN_ERRR(NOT_INIT, errstring);
    }

    if (!gasneti_mynode) {
      fflush(NULL);
      fprintf(stdout,
              "-----------------------------------------------------------------------\n"
              " WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING\n"
              "\n"
              " GASNet's gemini-conduit is currently in BETA status.\n"
              " You should NOT trust any performance numbers from this run as\n"
              " predictive of the performance of the conduit when completed.\n"
              "\n"
              " WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING\n"
              "-----------------------------------------------------------------------\n");
      fflush(NULL);
    }

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): gasnetc_init done - node %i/%i starting...\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif

  /* determine which GASNet nodes may share memory.
   * build gasneti_nodemap[]
   *  call gasneti_nodemapParse() after constructing it.
   */
  gasneti_nodemap = gasneti_malloc(gasneti_nodes * sizeof(uint32_t));
  gasneti_assert(gasneti_nodemap);
  /* PMI uses int, gni and gasnet use uint32_t */
  gasneti_assert(sizeof(int32_t) == sizeof(gasnett_atomic_t));
  gasneti_assert(sizeof(int) == sizeof(uint32_t));
  ret = PMI_Get_numpes_on_smp(&localranks);
  gasneti_assert(ret == PMI_SUCCESS);
  gasneti_assert(localranks <= gasneti_nodes);
  /* OK to use the base of gasneti_nodemap as a temp because it isn't filled in */
  ret = PMI_Get_pes_on_smp((int *) gasneti_nodemap, localranks);
  gasneti_assert(ret == PMI_SUCCESS);
  /* find minimum rank on local supernode */
  minlocalrank = gasneti_nodes;  /* one larger than largest possible */
  for (i = 0; i < localranks; i += 1) {
    if (gasneti_nodemap[i] < minlocalrank) minlocalrank = gasneti_nodemap[i];
  }
#if GASNETC_DEBUG
  {
    int i;
    fprintf(stderr, "rank %d, localranks (%d) are ", 
	    gasneti_mynode, localranks);
    for (i = 0; i < localranks; i += 1) {
      fprintf(stderr," %d", gasneti_nodemap[i]);
    }
    fprintf(stderr, "\n"); fflush(stderr);
  }
#endif
  gasnetc_GNIT_Allgather(&minlocalrank, sizeof(uint32_t), gasneti_nodemap);
  for (i = 0; i < gasneti_nodes; i += 1) {
    /* gasneti_assert(gasneti_nodemap[i] >= 0);  type is unsigned, so this is moot */
    gasneti_assert(gasneti_nodemap[i] < gasneti_nodes);
  }

  gasneti_nodemapParse();

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): node %i/%i calling gasnetc_init_messaging.\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif
  msgspace = gasnetc_init_messaging();
  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): node %i/%i finished gasnetc_init_messaging.\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif

  #if GASNET_PSHM
    /* If your conduit will support PSHM, you should initialize it here.
     * The 1st argument is normally "&gasnetc_bootstrapExchange" (described below).
     * The 2nd argument is the amount of shared memory space needed for any
     * conduit-specific uses.  The return value is a pointer to the space
     * requested by the 2nd argument.
     */
    /* LCS  no shared mem needed for the conduit, we get it from the gni
     * shared message queue machinery 
     */
    gasneti_pshm_init(&gasnetc_bootstrapExchange, 0);
  #endif
    /* LCS  Use segment size strategy from portals-conduit (CNL only) */
  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    { 
      uintptr_t max_pin = gasnetc_portalsMaxPinMem(msgspace);


#if GASNET_DEBUG_VERBOSE
      {
	fprintf(stderr, "node %i Gemini Conduit reports Max Pin Mem = %ld\n",
	       gasneti_mynode,(long) max_pin);
	fflush(stderr);
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

  #if 0
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
                                   gasnetc_bootstrapExchange, gasnetc_bootstrapBroadcast);
  #endif

  gasneti_auxseg_init(); /* adjust max seg values based on auxseg */
#if GASNET_DEBUG_VERBOSE
  fprintf(stderr, "node %i Leaving gasnetc_init\n",gasneti_mynode);
  fflush(stderr);
#endif

  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
extern int gasnet_init(int *argc, char ***argv) {
  /* after this, ams should work, but the segments aren't registered yet */
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

  /*  (###) register any custom signal handlers required by your conduit 
   *        (e.g. to support interrupt-based messaging)
   */
  /* LCS None needed */

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
      gasnetc_assert_aligned(segbase, GASNET_PAGESIZE);
      gasnetc_assert_aligned(segsize, GASNET_PAGESIZE);
      /* (###) add code here to choose and register a segment 
         (ensuring alignment across all nodes if this conduit sets GASNET_ALIGNED_SEGMENTS==1) 
         you can use gasneti_segmentAttach() here if you used gasneti_segmentInit() above
      */
      gasneti_assert(((uintptr_t)segbase) % GASNET_PAGESIZE == 0);
      gasneti_assert(segsize % GASNET_PAGESIZE == 0);
    }
  #else
    /* GASNET_SEGMENT_EVERYTHING */
    segbase = (void *)0;
    segsize = (uintptr_t)-1;
    /* (###) add any code here needed to setup GASNET_SEGMENT_EVERYTHING support */
  #endif

  /* ------------------------------------------------------------------------------------ */
  /*  gather segment information */

  /* (LCS) This was done by segmentAttach above
   */

  /* ------------------------------------------------------------------------------------ */
  /*  primary attach complete */
  gasneti_attach_done = 1;
  gasnetc_bootstrapBarrier();

  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(): primary attach complete"));

  gasneti_assert(gasneti_seginfo[gasneti_mynode].addr == segbase &&
         gasneti_seginfo[gasneti_mynode].size == segsize);

  gasneti_auxseg_attach(); /* provide auxseg */
  /* LCS After this, puts, and gets should work */
  gasnetc_init_segment(segbase, segsize);

  gasnete_init(); /* init the extended API */

  gasneti_nodemapFini();

  gasnetc_init_bounce_buffer_pool();  /* auxseg should be set by now */

  gasnetc_init_post_descriptor_pool();
  /* ensure extended API is initialized across nodes */
  gasnetc_bootstrapBarrier();

  GASNETI_TRACE_PRINTF(C,("gasnetc_attach: done\n"));
  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int exitcode, void *arg) {
  if (!gasnetc_shutdownInProgress) gasnetc_exit(exitcode);
}
#else
static void gasnetc_atexit(void) {
  if (!gasnetc_shutdownInProgress) gasnetc_exit(0);
}
#endif

extern void gasnetc_exit(int exitcode) {
  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  gasneti_reghandler(SIGQUIT, SIG_IGN);

  {  /* ensure only one thread ever continues past this point */
    static gasneti_mutex_t exit_lock = GASNETI_MUTEX_INITIALIZER;
    gasneti_mutex_lock(&exit_lock);
  }

  GASNETI_TRACE_PRINTF(C,("gasnetc_exit(%i)\n", exitcode));

  /* LCS Code modelled after portals-conduit */
  /* should prevent us from entering again */
  gasnetc_shutdownInProgress = 1;

    gasneti_reghandler(SIGALRM, SIG_DFL);
    alarm(2 + gasnetc_shutdown_seconds);

  if (gasnetc_sys_exit(&exitcode)) {
    fprintf(stderr, "node %i Failed to coordinate shutdown after %lu milliseconds\n",gasneti_mynode,(unsigned long)(1e3*gasnetc_shutdown_seconds));
    fflush(stderr);
    /* Death of any process by a fatal signal will cause launcher to kill entire job.
     * We don't use INT or TERM since one could be blocked if we are in its handler. */
    
    raise(SIGKILL);
    gasneti_killmyprocess(exitcode); /* last chance */
  }
  alarm(0);

  gasneti_flush_streams();
  gasneti_trace_finish();
  gasneti_sched_yield();
  gasnetc_shutdown();

  gasnetc_GNIT_Finalize();  /* normal exit via PMI */
  gasneti_killmyprocess(exitcode); /* last chance */
  gasnetc_GNIT_Abort("gasnetc_exit failed!");
}

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/
#if GASNET_PSHM
/* (###) GASNETC_GET_HANDLER
 *   If your conduit will support PSHM, then there needs to be a way
 *   for PSHM to see your handler table.  If you use the recommended
 *   implementation (gasnetc_handler[]) then you don't need to do
 *   anything special.  Othwerwise, #define GASNETC_GET_HANDLER in
 *   gasnet_core_fwd.h and implement gasnetc_get_handler() here, or
 *   as a macro or inline in gasnet_core_internal.h
 *
 * (###) GASNETC_TOKEN_CREATE
 *   If your conduit will support PSHM, then there needs to be a way
 *   for the conduit-specific and PSHM token spaces to co-exist.
 *   The default PSHM implementation produces tokens with the least-
 *   significant bit set and assumes the conduit never will.  If that
 *   is true, you don't need to do anything special here.
 *   If your conduit cannot use the default PSHM token code, then
 *   #define GASNETC_TOKEN_CREATE in gasnet_core_fwd.h and implement
 *   the associated routines described in gasnet_pshm.h.  That code
 *   could be functions located here, or could be macros or inlines
 *   in gasnet_core_internal.h.
 */
#endif

extern int gasnetc_AMGetMsgSource(gasnet_token_t token, gasnet_node_t *srcindex) {
  gasnet_node_t sourceid;
  GASNETI_CHECKATTACH();
  GASNETI_CHECK_ERRR((!token),BAD_ARG,"bad token");
  GASNETI_CHECK_ERRR((!srcindex),BAD_ARG,"bad src ptr");

#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let the PSHM code
   * have a chance to recognize the token first, as shown here. */
  if (gasneti_AMPSHMGetMsgSource(token, &sourceid) != GASNET_OK)
#endif
  {
    /* (###) add code here to write the source index into sourceid. */
    sourceid = ((gasnetc_token_t *)token)->source;

  }
  gasneti_assert(sourceid < gasneti_nodes);
  *srcindex = sourceid;
  return GASNET_OK;
}

extern int gasnetc_AMPoll(void) {
  GASNETI_CHECKATTACH();

#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it make progress here. */
  gasneti_AMPSHMPoll(0);
#endif

  /* (###) add code here to run your AM progress engine */
  /* LCS */
  gasnetc_poll();
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
  int i;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs);
  gasneti_AMPoll(); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Short, dest, handler,
                                           0, 0, 0,
                                           numargs, argptr);
  } else
#endif
  {
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */
    /* LCS send a short gni message (all header) */

    gasnetc_am_short_packet_t m;
    gasnetc_get_am_credit(dest);
    m.header.command = GC_CMD_AM_SHORT;
    m.header.misc    = 0;
    m.header.numargs = numargs;
    m.header.handler = handler;
    for (i = 0; i < numargs; i += 1) {
      m.args[i] = va_arg(argptr, uint32_t);
    }
    retval = gasnetc_send(dest, &m, GASNETC_HEADLEN(short, numargs), NULL, 0);
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
  int i;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);
  gasneti_AMPoll(); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Medium, dest, handler,
                                           source_addr, nbytes, 0,
                                           numargs, argptr);
  } else
#endif
  {
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */
    /* LCS send short message, using header for args and body for nbytes data */

    gasnetc_am_medium_packet_t m;
    gasnetc_get_am_credit(dest);
    m.header.command = GC_CMD_AM_MEDIUM;
    m.header.misc    = nbytes;
    m.header.numargs = numargs;
    m.header.handler = handler;
    for (i = 0; i < numargs; i += 1) {
      m.args[i] = va_arg(argptr, uint32_t);
    }
    /* TODO: round up nbytes to multiple of 8 to avoid rmw at dest nic */
    retval = gasnetc_send(dest, &m, GASNETC_HEADLEN(medium, numargs), source_addr, nbytes);
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
  int i;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);
  gasneti_AMPoll(); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                                           source_addr, nbytes, dest_addr,
                                           numargs, argptr);
  } else
#endif
  {
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */
    /* LCS if size fits in packet, just send message, and copy will be done
       at destination
     * if size is large, use RDMA plus SMSG
     * for blocking, wait for completion of the rdma, then smsg
     */
    gasnetc_am_long_packet_t m;
    gasnetc_get_am_credit(dest);
    /* fma credit needed here? LCS XXX */
    if (nbytes <= GASNETC_MAX_PACKED_LONG(numargs)) {
      /* send data in packet payload */
      m.header.command = GC_CMD_AM_LONG;
      m.header.misc    = 1; /* indicates packed format */
      m.header.numargs = numargs;
      m.header.handler = handler;
      m.data_length = nbytes;
      m.data = dest_addr;
      for (i = 0; i < numargs; i += 1) {
	m.args[i] = va_arg(argptr, uint32_t);
      }
      /* In this call to gasnetc_send, the argument list length is rounded up
       * to an 8 byte boundary so that the immediate data can be aligned.
       */
      /* TODO: round up nbytes to multiple of 8 to avoid rmw at dest nic */
      retval = gasnetc_send(dest, &m, GASNETC_HEADLEN(long, numargs), source_addr, nbytes);
      
    } else {
      gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor();
      gasneti_atomic_t done = gasneti_atomic_init(0);
      gasneti_assert(gpd);
      gpd->completion = (gasnete_op_t *) &done;
      gpd->flags = GC_POST_COMPLETION_FLAG;

      /* Rdma data, then send header */
      gasnetc_rdma_put(dest, dest_addr, source_addr, nbytes, gpd);

      m.header.command = GC_CMD_AM_LONG;
      m.header.misc    = 0;
      m.header.numargs = numargs;
      m.header.handler = handler;
      m.data_length = nbytes;
      m.data = dest_addr;
      for (i = 0; i < numargs; i += 1) {
	m.args[i] = va_arg(argptr, uint32_t);
      }
      while(!gasneti_atomic_read(&done, 0)) gasnetc_poll_local_queue();
      retval = gasnetc_send(dest, &m, GASNETC_HEADLEN(long, numargs), NULL, 0);
    }

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
  int i;
  va_list argptr;
  GASNETI_COMMON_AMREQUESTLONGASYNC(dest,handler,source_addr,nbytes,dest_addr,numargs);
  gasneti_AMPoll(); /* poll at least once, to assure forward progress */
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the dest first. */
  if_pt (gasneti_pshm_in_supernode(dest)) {
    retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                                           source_addr, nbytes, dest_addr,
                                           numargs, argptr);
  } else
#endif
  {
    gasnetc_get_am_credit(dest);
    if (nbytes <= GASNETC_MAX_PACKED_LONG(numargs)) {
      gasnetc_am_long_packet_t m;
      /* send data in packet payload */
      m.header.command = GC_CMD_AM_LONG;
      m.header.misc    = 1; /* indicates packed format */
      m.header.numargs = numargs;
      m.header.handler = handler;
      m.data_length = nbytes;
      m.data = dest_addr;
      for (i = 0; i < numargs; i += 1) {
	m.args[i] = va_arg(argptr, uint32_t);
      }
      /* TODO: round up nbytes to multiple of 8 to avoid rmw at dest nic */
      retval = gasnetc_send(dest, &m, GASNETC_HEADLEN(long, numargs), source_addr, nbytes);
    } else {
      gasnetc_post_descriptor_t *gpd;
      gpd = gasnetc_alloc_post_descriptor();
      gasneti_assert(gpd);
      gpd->flags = GC_POST_SEND;
      gpd->dest = dest;
      gpd->u.galp.header.command = GC_CMD_AM_LONG;
      gpd->u.galp.header.misc    = 0;
      gpd->u.galp.header.numargs = numargs;
      gpd->u.galp.header.handler = handler;
      gpd->u.galp.data_length = nbytes;
      gpd->u.galp.data = dest_addr;
      for (i = 0; i < numargs; i += 1) {
	gpd->u.galp.args[i] = va_arg(argptr, uint32_t);
      }
      /* Rdma data, then send header as part of completion*/
      gasnetc_rdma_put(dest, dest_addr, source_addr, nbytes, gpd);
      retval = GASNET_OK;
    }
  }
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyShortM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  int retval;
  int i;
  va_list argptr;
  gasnet_node_t dest;
  GASNETI_COMMON_AMREPLYSHORT(token,handler,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the token first. */
  if_pt (gasnetc_token_is_pshm(token)) {
    retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Short, token, handler,
                                         0, 0, 0,
                                         numargs, argptr);
  } else
#endif
  { 
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */


    gasnetc_am_short_packet_t m;
    GASNETI_SAFE(gasnetc_AMGetMsgSource(token, &dest));
    gasneti_assert(((gasnetc_token_t *)token)->need_reply);
    ((gasnetc_token_t *)token)->need_reply = 0;
    m.header.command = GC_CMD_AM_SHORT_REPLY;
    m.header.misc    = 0;
    m.header.numargs = numargs;
    m.header.handler = handler;
    for (i = 0; i < numargs; i += 1) {
      m.args[i] = va_arg(argptr, uint32_t);
    }
    retval = gasnetc_send(dest, &m, GASNETC_HEADLEN(short, numargs), NULL, 0);
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
  int i;
  va_list argptr;
  gasnet_node_t dest;
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,numargs);
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the token first. */
  if_pt (gasnetc_token_is_pshm(token)) {
    retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Medium, token, handler,
                                         source_addr, nbytes, 0,
                                         numargs, argptr);
  } else
#endif
  {
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */
    gasnetc_am_medium_packet_t m;
    GASNETI_SAFE(gasnetc_AMGetMsgSource(token, &dest));
    gasneti_assert(((gasnetc_token_t *)token)->need_reply);
    ((gasnetc_token_t *)token)->need_reply = 0;
    m.header.command = GC_CMD_AM_MEDIUM_REPLY;
    m.header.misc    = nbytes;
    m.header.numargs = numargs;
    m.header.handler = handler;
    for (i = 0; i < numargs; i += 1) {
      m.args[i] = va_arg(argptr, uint32_t);
    }
    /* TODO: round up nbytes to multiple of 8 to avoid rmw at dest nic */
    retval = gasnetc_send(dest, &m, GASNETC_HEADLEN(medium, numargs), source_addr, nbytes);
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
  int i;
  va_list argptr;
  gasnet_node_t dest;
  GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,numargs); 
  va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
  /* (###) If your conduit will support PSHM, let it check the token first. */
  if_pt (gasnetc_token_is_pshm(token)) {
    retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Long, token, handler,
                                         source_addr, nbytes, dest_addr,
                                         numargs, argptr);
  } else
#endif
  {
    /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */
    gasnetc_am_long_packet_t m;
    GASNETI_SAFE(gasnetc_AMGetMsgSource(token, &dest));
    gasneti_assert(((gasnetc_token_t *)token)->need_reply);
    ((gasnetc_token_t *)token)->need_reply = 0;
    if (nbytes <= GASNETC_MAX_PACKED_LONG(numargs)) {
      m.header.command = GC_CMD_AM_LONG_REPLY;
      m.header.misc    = 1; /* indicates packed format */
      m.header.numargs = numargs;
      m.header.handler = handler;
      m.data_length = nbytes;
      m.data = dest_addr;
      for (i = 0; i < numargs; i += 1) {
	m.args[i] = va_arg(argptr, uint32_t);
      }
      /* send data in packet payload */
      /* TODO: round up payload to 8-byte boundary to avoid rmw at dest */
      retval = gasnetc_send(dest, &m, GASNETC_HEADLEN(long, numargs), source_addr, nbytes);
    } else {
      gasnetc_post_descriptor_t *gpd = gasnetc_alloc_post_descriptor();
      gasneti_atomic_t done = gasneti_atomic_init(0);
      gasneti_assert(gpd);
      gpd->completion = (gasnete_op_t *) &done;
      gpd->flags = GC_POST_COMPLETION_FLAG;
      /* Rdma data, then send header */
      gasnetc_rdma_put(dest, dest_addr, source_addr, nbytes, gpd);

      m.header.command = GC_CMD_AM_LONG_REPLY;
      m.header.misc    = 0;
      m.header.numargs = numargs;
      m.header.handler = handler;
      m.data_length = nbytes;
      m.data = dest_addr;
      for (i = 0; i < numargs; i += 1) {
	m.args[i] = va_arg(argptr, uint32_t);
      }
      /* cannot process more ams here! */
      while(!gasneti_atomic_read(&done, 0)) gasnetc_poll_local_queue();

      retval = gasnetc_send(dest, &m, GASNETC_HEADLEN(long, numargs), NULL, 0);
    }
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

/* ------------------------------------------------------------------------------------ */
int gasnetc_debug = 0;
void gasnetc_enable_debug(int enable)
{
  gasnetc_debug = enable;
}
