/*   $Source: /var/local/cvs/gasnet/shmem-conduit/gasnet_core.c,v $
 *     $Date: 2009/09/18 23:33:44 $
 * $Revision: 1.43 $
 * Description: GASNet shmem conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sched.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

gasnet_handlerentry_t const *gasnetc_get_handlertable(void);
static void gasnetc_atexit(void);

static gasnet_seginfo_t gasnetc_SHMallocSegmentSearch(void);
static uintptr_t        gasnetc_aligndown_pow2(uintptr_t addr);
static uintptr_t        gasnetc_alignup_pow2(uintptr_t addr);

gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS]; /* handler table (recommended impl) */

gasnet_seginfo_t	 gasnetc_seginfo_init;
int			 gasnetc_seginfo_allocated = 0;
size_t			 gasnetc_pagesize;
pid_t                   *gasnetc_pid = NULL;

int  gasnetc_amq_idx[2] = { 0, 0 }; /* request, reply */
int  gasnetc_amq_depth = -1;	/* max is GASNETC_AMQUEUE_MAX_DEPTH */
int  gasnetc_amq_depth_log2;
int  gasnetc_amq_mask;

int  gasnetc_verbose_spawn = 0;

gasnetc_am_packet_t  gasnetc_amq_reqs[2*GASNETC_AMQUEUE_MAX_DEPTH];

#ifdef CRAY_SHMEM
  volatile long	gasnetc_amq_reqfields[GASNETC_AMQUEUE_MAX_FIELDS];
  long		gasnetc_amq_numfields;
  extern uintptr_t gasnete_pe_bits_shift;
  extern uintptr_t gasnete_addr_bits_mask;
#elif defined(SGI_SHMEM)
  uintptr_t gasnetc_sgi_segbase;
  /*
   * Altix requires mpirun to start jobs, and also requests that jobs
   * explicitly call MPI_Finalize() or else they abort.
   * Origin 2000 only seems happy w/ both MPI_Init() and _Finalize().
   */
  extern int MPI_Init(int *, char ***);
  extern void MPI_Finalize(void);
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnetc_check_config(void) {
  /* add code to do some sanity checks on the number of nodes, handlers
   * and/or segment sizes */ 
  /* ensure our AM buffer placement and sizes are 8-byte aligned */
  gasneti_assert((void *)GASNETI_ALIGNUP(gasnetc_amq_reqs,8) == (void *)gasnetc_amq_reqs);
  gasneti_assert((size_t)GASNETI_ALIGNUP(sizeof(gasnetc_amq_reqs[0]),8) == sizeof(gasnetc_amq_reqs[0]));
  /* assert various other properties assumed for correct AM alignment */
  gasneti_assert(sizeof(gasnetc_amq_reqs[0].state) == 4);
}

static void gasnetc_bootstrapBarrier(void) {
	shmem_barrier_all();
}

static void gasnetc_bootstrapExchange(void *src, size_t len, void *dest) {
  static long pSync[_SHMEM_COLLECT_SYNC_SIZE];
  static void *tmpbuf = NULL;
  static int tmpbufsz = 0;
  size_t elemlen = (size_t)GASNETI_ALIGNUP(len,8);
  size_t totallen = elemlen*gasneti_nodes;

  GASNETI_TRACE_PRINTF(D,("gasnetc_bootstrapExchange(%i bytes)",(int)len));
  gasnetc_bootstrapBarrier();
  if (tmpbufsz < totallen) {
    if (tmpbuf) shfree(tmpbuf);
    else { /* first-time call */
      int i;
      for (i=0; i < _SHMEM_COLLECT_SYNC_SIZE; i++)
	  pSync[i] = _SHMEM_SYNC_VALUE;
    }
    tmpbuf = shmalloc(totallen);
    gasneti_assert_always(tmpbuf);
    tmpbufsz = totallen;
    gasnetc_bootstrapBarrier(); /* wait for pSync init */ 
  }
  memcpy(((char*)tmpbuf)+(elemlen*gasneti_mynode), src, len); /* copy into place */

  /* perform exchange */
  shmem_fcollect64(tmpbuf,((char*)tmpbuf)+(elemlen*gasneti_mynode),
                   elemlen/8,0,0,gasneti_nodes,pSync);
  gasneti_assert(!memcmp(((char*)tmpbuf)+(elemlen*gasneti_mynode), src, len));

  { int i; /* copy back */
    for (i=0; i < gasneti_nodes; i++) {
      memcpy(((char*)dest)+(len*i), ((char*)tmpbuf)+(elemlen*i), len);
    }
  }
  gasnetc_bootstrapBarrier();
}

static int gasnetc_init(int *argc, char ***argv) {
  char *qdepth;
  /*  check system sanity */
  gasnetc_check_config();

  if (gasneti_init_done) 
    GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");

  gasneti_freezeForDebugger();

  /*
   * Print information about shmalloc segment search when verbose environment
   * or debug mode
   */
  #ifdef GASNET_DEBUG
    gasnetc_verbose_spawn = 1;
  #else
    gasnetc_verbose_spawn = !!gasnet_getenv("GASNET_SHMEM_DEBUGALLOC");
  #endif

  #if GASNET_DEBUG_VERBOSE
    /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
    fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
  #endif

  #if defined(CRAY_SHMEM) || GASNETI_ARCH_ALTIX
    start_pes(0);
  #elif PLATFORM_OS_IRIX
    MPI_Init(argc, argv);
  #elif defined(QUADRICS_SHMEM)
    shmem_init();
  #endif

  gasneti_mynode = shmem_my_pe();
  gasneti_nodes = shmem_n_pes();

  /* setup queue depth once we have node id, for best env tracing */
  gasnetc_amq_depth = gasneti_getenv_int_withdefault("GASNET_NETWORKDEPTH", GASNETC_AMQUEUE_DEFAULT_DEPTH, 0);
  if (!GASNETC_AMQUEUE_SIZE_VALID(gasnetc_amq_depth))
      GASNETI_RETURN_ERRR(BAD_ARG, "Invalid QDepth parameter");
  gasnetc_amq_mask = (gasnetc_amq_depth-1);
  gasnetc_amq_depth_log2 = 0;
  { int tmp = gasnetc_amq_mask; 
    while (tmp) { gasnetc_amq_depth_log2++; tmp >>= 1; }
    gasneti_assert((1<<gasnetc_amq_depth_log2) == gasnetc_amq_depth);
  }
  #ifdef CRAY_SHMEM
    gasnetc_amq_numfields = GASNETI_ALIGNUP(2*gasnetc_amq_depth,64)/64;
  #endif

  gasnetc_pid = gasneti_malloc(gasneti_nodes*sizeof(pid_t));
  gasnetc_pid[gasneti_mynode] = getpid();
  gasnetc_bootstrapExchange(&(gasnetc_pid[gasneti_mynode]), sizeof(pid_t), gasnetc_pid);

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif

  gasneti_nodemapInit(&gasnetc_bootstrapExchange, NULL, 0, 0);

  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    { 
	#if defined(CRAY_SHMEM) || defined(SGI_SHMEM)

	/* XXX Currently SHMallocSegmentSearch takes about 1 second.  We may
	 * want to take another approach if it is too long.  Since Cray
	 * machines do not necessarily ship with much configuration variance,
	 * perhaps there's a static way of determining the amount of physical
	 * memory.
	 */

	 gasnetc_seginfo_init = gasnetc_SHMallocSegmentSearch();

	 /* Since shmalloc() is collective, local == global */
	 gasneti_MaxLocalSegmentSize = gasneti_MaxGlobalSegmentSize 
			= gasnetc_seginfo_init.size;

	 gasnetc_seginfo_allocated = 1;

	 /* We keep the allocation live until gasnet_attach(), in which case we
	  * can simply use realloc to reduce its size */

	#elif defined(QUADRICS_SHMEM)
	  #error Not implemented yet.
	#endif

    }
  #elif GASNET_SEGMENT_EVERYTHING
    /* segment is everything - nothing to do */
  #else
    #error Bad segment config
  #endif

  /*  register signal handlers early to support signalling exit */
  gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);
  gasnetc_bootstrapBarrier(); /* wait for global register, to handle exit in init/attach interval */ 

  gasneti_init_done = 1;  

  gasneti_trace_init(argc, argv);

  gasneti_auxseg_init(); /* adjust max seg values based on auxseg */

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
    /* add code here to register table[i].fnptr 
             on index (gasnet_handler_t)newindex */
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

  /* wait for all nodes to arrive - increases system stability if there's a gasnet_exit()
     call between init and attach 
   */
  gasnetc_bootstrapBarrier(); 

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

  atexit(gasnetc_atexit);

  /* ------------------------------------------------------------------------------------ */
  /*  register segment  */

  gasneti_seginfo = (gasnet_seginfo_t *)gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));
  memset(gasneti_seginfo, 0, gasneti_nodes*sizeof(gasnet_seginfo_t));


  #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    if (segsize == 0) { 
	    segbase = NULL;
	    shfree(gasnetc_seginfo_init.addr);
    }
    else {
      gasneti_assert(((uintptr_t)segbase) % GASNET_PAGESIZE == 0);
      gasneti_assert(segsize % GASNET_PAGESIZE == 0);

      /* With Cray, we use our preallocated segment and optionally truncate it
       * with realloc if the user requests something smaller
       */
	#if defined(CRAY_SHMEM) || defined(SGI_SHMEM)
      	{
	    int		i;

	    if (segsize < gasnetc_seginfo_init.size) {
		/* Enforce power of 2 per thread on Altix and Origin */
		#if 0 && defined(SGI_SHMEM)
		  char buf[64];
		  uintptr_t segup = gasnetc_alignup_pow2(segsize);
		  segsize = segup > gasnetc_seginfo_init.size
				? gasnetc_aligndown_pow2(gasnetc_seginfo_init.size) 
				: segup;
		#endif

		#if defined(CRAY_SHMEM) || defined(SGI_SHMEM)
		    /* X1: shrealloc on a pointer returned by shmemalign dumps core */
		    shfree(gasnetc_seginfo_init.addr);
		    segbase = shmemalign(GASNETT_PAGESIZE, segsize);
		    if (segbase == NULL) 
			gasneti_fatalerror(
			    "shmalloc couldn't resize GASNet segment from "
			    "%lu bytes down to %lu bytes\n", 
			    gasnetc_seginfo_init.size, segsize);

		#endif
		#if 0
		    segbase = 
			shrealloc(gasnetc_seginfo_init.addr, segsize);

		    if (segbase == NULL) {
			shfree(gasnetc_seginfo_init.addr);
			gasneti_fatalerror(
			    "shrealloc() failed on initial GASNet segment");
		    }
                #endif
		gasnetc_seginfo_init.addr = (void *) segbase;
		gasnetc_seginfo_init.size = segsize;
	    }
	    else {
		segbase = gasnetc_seginfo_init.addr;
		segsize = gasnetc_seginfo_init.size;
	    }

            #if 0
	    printf("segbase=%p, segsize=%lu\n", segbase, segsize);
            #endif

	    #ifdef CRAY_SHMEM
	    {
		void **shm_collect;
		uintptr_t	pemask, shmoff;
		long		lastnode;

                shm_collect = gasneti_malloc(sizeof(void *) * gasneti_nodes);
                gasnetc_bootstrapExchange(&segbase, sizeof(void *), shm_collect);

		for (i=0;i<gasneti_nodes;i++) {
		    gasneti_seginfo[i].addr = shm_collect[i];
		    gasneti_seginfo[i].size = segsize;
		    shmoff = (intptr_t) shm_collect[i] - (intptr_t) segbase;

		    if (0) {
			printf("%d> seg %2d: %p12,%9lu => off = %16lx\n",
			    gasneti_mynode, i, gasneti_seginfo[i].addr, 
			    (unsigned long) gasneti_seginfo[i].size, 
			    (unsigned long) shmoff);
			fflush(stdout);
		    }
		}

		if (gasneti_nodes == 1) {
		    gasnete_pe_bits_shift = 0;
		    gasnete_addr_bits_mask = (uintptr_t) -1;
		}   
		else {
		    gasnete_pe_bits_shift = 63-_leadz((long)shm_collect[1]);
		    gasnete_addr_bits_mask = 
			    (uintptr_t) (1UL<<gasnete_pe_bits_shift)-1;
		}
		gasneti_free(shm_collect);
	    }
	    #elif defined(SGI_SHMEM)
	    {
		gasnetc_sgi_segbase = (uintptr_t) segbase;

		for (i=0; i<gasneti_nodes; i++) {

		    gasneti_seginfo[i].addr = (void *) shmem_ptr(segbase, i);
		    #if 0
			if (i == gasneti_mynode)
			    gasneti_seginfo[i].addr = (void *)shmem_ptr(segbase,i);
			else
			    gasneti_seginfo[i].addr = (void *)segbase;
		    #endif

		    gasneti_seginfo[i].size = segsize;

		    #if 0
		    printf("%d> seg %2d: %p,%9lu\n",
			    gasneti_mynode, i, gasneti_seginfo[i].addr, 
			    (unsigned long) gasneti_seginfo[i].size);
		    fflush(stdout);
		    #endif
		}
	    }
	    #endif

	}

	#else
	    #error Currently only support shmalloc() and shrealloc() Segment allocators
	#endif

      /* add code here to choose and register a segment 
         (ensuring alignment across all nodes if this conduit sets GASNET_ALIGNED_SEGMENTS==1) 
         you can use gasneti_segmentAttach() here if you used gasneti_segmentInit() above
      */
    }
  #else
    /* GASNET_SEGMENT_EVERYTHING.  */
    segbase = (void *)0;
    segsize = (uintptr_t)-1;
    {
	int i;

	for (i=0;i<gasneti_nodes;i++) {
	    gasneti_seginfo[i].addr = (void *)0;
	    gasneti_seginfo[i].size = (uintptr_t)-1;

	}
    }
    #if PLATFORM_ARCH_CRAYX1
    { /* Although there is no GASNet segment, we must still recover the
       * mask/shift bits to translate X1 pointers */
       static int dummy = 0; /* an indicator of where our static data resides on each node */
       void *ptr;
       void **ptrs;

       ptr = &dummy;
       ptrs = gasneti_malloc(gasneti_nodes*sizeof(void*));
       gasnetc_bootstrapExchange(&ptr, sizeof(void *), ptrs);

	if (gasneti_nodes == 1) {
	    gasnete_pe_bits_shift = 0;
	    gasnete_addr_bits_mask = (uintptr_t) -1;
	} else {
	    gasnete_pe_bits_shift = 63-_leadz((long)ptrs[1]);
	    gasnete_addr_bits_mask = (uintptr_t) (1UL<<gasnete_pe_bits_shift)-1;
	}
        { int other = (gasneti_mynode ^ 1) % gasneti_nodes;
	  GASNETI_TRACE_PRINTF(D,("addr=%p mask=%p shift=%d other(%i)=%p ptr_other=%p\n",
			(void *)ptr,(void *)gasnete_addr_bits_mask, gasnete_pe_bits_shift, 
		        other, GASNETE_TRANSLATE_X1(ptr, other), ptrs[other]));
        }
	gasneti_free(ptrs);
    }
    #endif /* CRAY X1 */
  #endif

  /* ------------------------------------------------------------------------------------ */
  /*  gather segment information */

  /* add code here to gather the segment assignment info into 
           gasneti_seginfo on each node (may be possible to use AMShortRequest here)
   */

  /* ------------------------------------------------------------------------------------ */
  /*  primary attach complete */
  gasneti_attach_done = 1;
  gasnetc_bootstrapBarrier();

  GASNETI_TRACE_PRINTF(C,("gasnetc_attach(): primary attach complete"));

#if 0
  gasneti_assert(gasneti_seginfo[gasneti_mynode].addr == segbase &&
         gasneti_seginfo[gasneti_mynode].size == segsize);
#endif

  gasneti_auxseg_attach(); /* provide auxseg */

  gasnete_init(); /* init the extended API */

  gasneti_nodemapFini();

  /* ensure extended API is initialized across nodes */
  gasnetc_bootstrapBarrier();

  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
static void gasnetc_atexit(void) {
    gasnetc_exit(0);
}

extern void gasnetc_exit(int exitcode) {
  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  gasneti_reghandler(SIGQUIT, SIG_IGN);

  {  /* ensure only one thread ever continues past this point */
    static gasneti_mutex_t exit_lock = GASNETI_MUTEX_INITIALIZER;
    gasneti_mutex_lock(&exit_lock);
  }

  GASNETI_TRACE_PRINTF(C,("gasnet_exit(%i)\n", exitcode));

  if (fflush(stdout)) 
    gasneti_fatalerror("failed to flush stdout in gasnetc_exit: %s", strerror(errno));
  if (fflush(stderr)) 
    gasneti_fatalerror("failed to flush stderr in gasnetc_exit: %s", strerror(errno));
  gasneti_trace_finish();
  gasneti_sched_yield();

  /* add code here to terminate the job across _all_ nodes 
           with gasneti_killmyprocess(exitcode) (not regular exit()), preferably
           after raising a SIGQUIT to inform the client of the exit
  */
  #if defined(CRAY_SHMEM) || defined(SGI_SHMEM)
    { /* send a sigquit to every process */
      int i=0;
      for (i=0; i < gasneti_nodes; i++) {
        if (i != gasneti_mynode) kill(gasnetc_pid[i], SIGQUIT); 
      }
    }
  #endif 

  #if defined(CRAY_SHMEM) || defined(SGI_SHMEM)
    if (gasnetc_seginfo_allocated)
	shfree(gasnetc_seginfo_init.addr);
  #endif
  #if defined(SGI_SHMEM)
    MPI_Finalize();
  #endif

  gasneti_killmyprocess(exitcode);
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

  /* add code here to write the source index into sourceid */
  sourceid = (gasnet_node_t) GASNETC_AMHEADER_NODEID(token);

  gasneti_assert(sourceid < gasneti_nodes);
  *srcindex = sourceid;
  return GASNET_OK;
}

GASNETI_INLINE(gasnetc_AMProcess)
void
gasnetc_AMProcess(gasnetc_am_header_t *hdr, uint32_t *args /* header */)
{
	gasneti_handler_fn_t	handler;
	gasnet_token_t		token;
	size_t			numargs = (size_t) hdr->numargs;


	handler = gasnetc_handler[hdr->handler];
	token = (uintptr_t) args[0];

	switch (hdr->type) {
	    case GASNETC_AMSHORT_T:
		{   gasnet_handlerarg_t *pargs =
			(gasnet_handlerarg_t *) &args[1];
		    GASNETI_RUN_HANDLER_SHORT(GASNETC_AMHEADER_ISREQUEST(hdr->reqrep),hdr->handler,
                                              handler,token,pargs,numargs);
		}
		break;
	    case GASNETC_AMMED_T:
		{   gasnet_handlerarg_t *pargs =
			(gasnet_handlerarg_t *) &args[2];
		    int nbytes = args[1];
		    void *pdata = (pargs + numargs + 
				    GASNETC_MEDHEADER_PADARG(numargs));
		    GASNETI_RUN_HANDLER_MEDIUM(GASNETC_AMHEADER_ISREQUEST(hdr->reqrep),hdr->handler,
                                               handler,token,pargs,numargs,
					       pdata, nbytes);
		}
		break;
	    case GASNETC_AMLONG_T:
		{   int nbytes = args[1];
#if PLATFORM_ARCH_64
		    gasnet_handlerarg_t *pargs =
			(gasnet_handlerarg_t *) &args[4];
		    void *pdata = (void *) GASNETI_MAKEWORD(args[2],args[3]);
#else
		    gasnet_handlerarg_t *pargs =
			(gasnet_handlerarg_t *) &args[3];
		    void *pdata = (void *) args[2];
#endif
		    GASNETI_RUN_HANDLER_LONG(GASNETC_AMHEADER_ISREQUEST(hdr->reqrep),hdr->handler,
                                             handler,token,pargs,numargs,
					     pdata,nbytes);
		}
		break;
	    default: gasneti_fatalerror("bad am category");
	}

	return;
}

#ifdef CRAY_SHMEM
extern int _gasnetc_AMPoll(int replyonly) {
    int	    i;
    long    idx, bits, index, off, mask;

    gasnetc_am_header_t	amhdr;

    for (i = 0, off = 0; i < gasnetc_amq_numfields; i++, off += 64) {
	
        /* we use _amo_afax here although all we need is a simple load -
           the rules for atomic ops require all access be through
           the atomic interface to ensure proper cache coherence */
	bits = _amo_afax((volatile unsigned long *) &gasnetc_amq_reqfields[i], 
			 0xffffffffffffffff, 0);

        while (bits) {
	    /* fetch the number of leading zeros in the field */
	    index = _leadz64(bits);

	    /* map the (field no,idx) --> index */
	    idx = index + off;

	    /* get the mask of the current field index */
	    mask = 0x8000000000000000ul >> index;

	    GASNETC_AMHEADER_UNPACK(
		gasnetc_amq_reqs[idx].header,
		amhdr.reqrep, amhdr.type, amhdr.numargs, 
		amhdr.handler, amhdr.pe);

            if (!replyonly || amhdr.reqrep == GASNETC_REPLY_T) {
	      gasnetc_AMProcess(&amhdr, &gasnetc_amq_reqs[idx].header);

	      /* Mask off the index in the global bitfield */
	      _amo_aax((volatile unsigned long *) &gasnetc_amq_reqfields[i], 
		      ~mask, 0);

	      /* Mark the slot as free */
	      gasnetc_amq_reqs[idx].state = GASNETC_AMQUEUE_FREE_S;
            }

	    /* Mask off the index in the current bitfield */
	    bits &= ~mask;
        }
    }
    return GASNET_OK;
}
#elif defined(SGI_SHMEM)
extern int _gasnetc_AMPoll(int replyonly) {
  static int serviced_gidx[2] = { 0, 0 };

  GASNETI_CHECKATTACH();

  /* take advantage of the fact that AM queue arbitration uses
     an atomic increment with a modulus >>> max queue depth + max node count : 
       each remote enqueue increments the gasnetc_amq_idx counter, 
       and each local dequeue increments the serviced_gidx counter
     if the two counters are equal, then we've serviced the same number of 
       AM's that have ever been enqueued, and nothing can be pending
     otherwise, some AM has arrived or is in the process of arriving and
       poll operations need to scan for it until it is serviced
     note this works correctly even with wraparound (no < or > comparisons)
     also note we may service AM's in a different order than they were 
       queued, because deliveries complete out of order with respect to enqueue,
       and we sometimes dequeue AM's out of order - so the actual idx value
       does not reliably tell us *which* slot needs serviced, only how many
   */
  /* service requests (optionally), then replies - we could safely use a loop with 
     two iterations here, but manually unrolling the loop provides a measurable
     improvement in unsuccessful poll time with Intel C
   */

  #define AMPOLL_SERVICE(ISREPLY) do {                                                               \
      int const gidx = gasnetc_amq_idx[ISREPLY]; /* unmod-ed index we need to reach */               \
      int idx = gidx;                                                                                \
      int iters = gasnetc_amq_depth;                                                                 \
      while (serviced_gidx[ISREPLY] != gidx && iters-- > 0) {                                        \
        /* repeat until we've scanned all slots or nothing further is pending */                     \
        idx = ((idx - 1) & gasnetc_amq_mask) + (ISREPLY ? gasnetc_amq_depth : 0);                    \
        if (gasnetc_amq_reqs[idx].state == GASNETC_AMQUEUE_DONE_S) {                                 \
            gasnetc_am_header_t amhdr;                                                               \
            gasneti_local_rmb(); /* ensure we see updated contents after successful flag read */     \
            GASNETC_AMHEADER_UNPACK(                                                                 \
                gasnetc_amq_reqs[idx].header,                                                        \
                amhdr.reqrep, amhdr.type, amhdr.numargs,                                             \
                amhdr.handler, amhdr.pe);                                                            \
            gasneti_assert((amhdr.reqrep==GASNETC_REPLY_T) == ISREPLY);                              \
            gasneti_assert(!replyonly || (amhdr.reqrep==GASNETC_REPLY_T));                           \
            gasnetc_AMProcess(&amhdr, &gasnetc_amq_reqs[idx].header);                                \
                                                                                                     \
            /* ensure any handler modifications to the med buf are committed before releasing buf */ \
            if (amhdr.type == GASNETC_AMMED_T) gasneti_local_wmb();                                  \
                                                                                                     \
            gasnetc_amq_reqs[idx].state = GASNETC_AMQUEUE_FREE_S;                                    \
            serviced_gidx[ISREPLY]++; /* serviced an AM */                                           \
        }                                                                                            \
      }                                                                                              \
    } while (0)
  if (!replyonly) AMPOLL_SERVICE(0); /* REQUESTS */
  AMPOLL_SERVICE(1); /* REPLIES */
  #undef AMPOLL_SERVICE

  return GASNET_OK;
}
#else
/* 
 * Unlike the Cray/SGI AMPoll, this (generic) poll has not been tuned yet.  It
 * currently cycles through every AM slot every time AMPoll is called.
 *
 */
extern int _gasnetc_AMPoll(int replyonly) {
  int repoff = !!replyonly;
  gasnetc_am_header_t amhdr;

  GASNETI_CHECKATTACH();

  for ( ; repoff < 2; repoff++) { /* service requests (optionally), then replies */
    int idx = gasnetc_amq_idx[repoff];
    int iters = gasnetc_amq_depth;

    while (iters-- > 0) {
      idx = ((idx - 1) & gasnetc_amq_mask) | (repoff << gasnetc_amq_depth_log2);

      if (gasnetc_amq_reqs[idx].state == GASNETC_AMQUEUE_DONE_S) {
          gasneti_local_rmb(); /* ensure we see updated contents after successful flag read */
          GASNETC_AMHEADER_UNPACK(
              gasnetc_amq_reqs[idx].header,
              amhdr.reqrep, amhdr.type, amhdr.numargs, 
              amhdr.handler, amhdr.pe);
          gasneti_assert((amhdr.reqrep==GASNETC_REPLY_T) == repoff);
          gasneti_assert(!replyonly || (amhdr.reqrep==GASNETC_REPLY_T));
          gasnetc_AMProcess(&amhdr, &gasnetc_amq_reqs[idx].header);

          /* ensure any handler modifications to the med buf are committed before releasing buf */
          if (amhdr.type == GASNETC_AMMED_T) gasneti_local_wmb();

          gasnetc_amq_reqs[idx].state = GASNETC_AMQUEUE_FREE_S;
      }
    }
  }

  return GASNET_OK;
}
#endif

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/

extern int gasnetc_AMRequestShortM( 
                            gasnet_node_t dest,       /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  int retval, i;
  va_list argptr;
  gasnetc_am_stub_t   _amstub;
  uint32_t  *args;

  GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs);
  va_start(argptr, numargs); /*  pass in last argument */

  gasneti_AMPoll();

#if 0 && PLATFORM_ARCH_CRAYX1
  myidx = gasnetc_AMQueueAcquire(dest, GASNETC_REQUEST_T);

  args = (uint32_t *) shmem_ptr(&gasnetc_amq_reqs[myidx].header, dest);
  args[0] = GASNETC_AMHEADER_PACK(
			GASNETC_REQUEST_T, GASNETC_AMSHORT_T, 
			numargs, handler, gasneti_mynode);

  GASNETC_VECTORIZE
  for (i = 1; i <= numargs; i++)
	  args[i] = (gasnet_handlerarg_t)va_arg(argptr, uint32_t);
#else

  /* Write header and pack args */
  _amstub.args[0] = GASNETC_AMHEADER_PACK(
			GASNETC_REQUEST_T, GASNETC_AMSHORT_T, 
			numargs, handler, gasneti_mynode);

  GASNETC_VECTORIZE
  for (i = 1; i <= numargs; i++)
	  _amstub.args[i] = (gasnet_handlerarg_t)va_arg(argptr, uint32_t);

  if (dest == gasneti_mynode) { /* loopback */
    static gasnetc_am_header_t amhdr = { GASNETC_REQUEST_T, GASNETC_AMSHORT_T };
    amhdr.numargs = numargs; amhdr.handler = handler; amhdr.pe = gasneti_mynode;
    gasnetc_AMProcess(&amhdr, _amstub.args);
  } else {
    size_t const len = GASNETC_SHORT_HEADERSZ + 4 * numargs;

    /* Get a slot in shared AMQueue */
    int const myidx = gasnetc_AMQueueAcquire(dest, GASNETC_REQUEST_T);;

    /* Put the header and arguments */
    shmem_putmem(&gasnetc_amq_reqs[myidx].header, &_amstub, len, dest);
    shmem_fence();

    /* Release a slot in shared AMQueue */
    gasnetc_AMQueueRelease(dest, myidx);
  }
#endif
  retval = GASNET_OK;
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestMediumM( 
                            gasnet_node_t dest,      /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  int retval, i;
  size_t    len;
  va_list argptr;
  uint32_t *args, *pptr;
  gasnetc_am_stub_t   _amstub;

  GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);
  va_start(argptr, numargs); /*  pass in last argument */

    /* add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */
  gasneti_AMPoll();

  /* Write args and pack a header */
  _amstub.args[0] = GASNETC_AMHEADER_PACK(
			GASNETC_REQUEST_T, GASNETC_AMMED_T, numargs, 
			handler, gasneti_mynode);
  _amstub.args[1] = (uint32_t) nbytes;
  args = (uint32_t *) &_amstub.args[2];
  for (i = 0; i < numargs; i++)
	  args[i] = (gasnet_handlerarg_t)va_arg(argptr, uint32_t);
  len = GASNETC_MED_HEADERSZ + 4 * numargs;

  if (dest == gasneti_mynode) { /* loopback */
    static char	_payload[GASNETC_MAX_MEDIUM_TOTAL+8]; /* XXX: needs to be tweaked if we ever add pthread support */
    static uint8_t *payload = NULL;
    static gasnetc_am_header_t amhdr = { GASNETC_REQUEST_T, GASNETC_AMMED_T };
    if_pf (!payload) { /* match the alignment of _gasnetc_am_packet.header */
      payload = ((uint8_t *)GASNETI_ALIGNUP(&_payload[0],8))+4; 
      amhdr.pe = gasneti_mynode; 
    }
    amhdr.numargs = numargs; amhdr.handler = handler; 
    memcpy(payload, &_amstub, len);
    memcpy(payload + len + (GASNETC_MEDHEADER_PADARG(numargs)<<2), source_addr, nbytes);
    gasnetc_AMProcess(&amhdr, (uint32_t*)payload);
  } else {
    /* Get a slot in shared AMQueue */
    int const myidx = gasnetc_AMQueueAcquire(dest, GASNETC_REQUEST_T);

    /* Adjust payload pointer according to numargs */
    pptr = (uint32_t *) &gasnetc_amq_reqs[myidx].payload + numargs + 1 +
	      GASNETC_MEDHEADER_PADARG(numargs);

    /* Put the header and arguments, followed by payload and a fence */
    shmem_putmem(&gasnetc_amq_reqs[myidx].header, &_amstub, len, dest);
    shmem_putmem(pptr, source_addr, nbytes, dest);
    shmem_fence();

    /* Release a slot in shared AMQueue */
    gasnetc_AMQueueRelease(dest, myidx);
  }
  retval = GASNET_OK;
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestLongM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval, i;
  va_list argptr;
  uint32_t *args, *pptr;
  gasnetc_am_stub_t   _amstub;

  GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);
  va_start(argptr, numargs); /*  pass in last argument */

    /* add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */

  gasneti_AMPoll();

  /* Write args and pack a header */
  _amstub.args[0] = GASNETC_AMHEADER_PACK(
			GASNETC_REQUEST_T, GASNETC_AMLONG_T, numargs, 
			handler, gasneti_mynode);
  _amstub.args[1] = nbytes;
#if PLATFORM_ARCH_64
  _amstub.args[2] = (gasnet_handlerarg_t) GASNETI_HIWORD(dest_addr);
  _amstub.args[3] = (gasnet_handlerarg_t) GASNETI_LOWORD(dest_addr);
  args = &_amstub.args[4];
#else
  _amstub.args[2] = (gasnet_handlerarg_t) dest_addr;
  args = &_amstub.args[3];
#endif

  for (i = 0; i < numargs; i++)
	  args[i] = (gasnet_handlerarg_t)va_arg(argptr, uint32_t);

  if (dest == gasneti_mynode) { /* loopback */
    static gasnetc_am_header_t amhdr = { GASNETC_REQUEST_T, GASNETC_AMLONG_T };
    amhdr.numargs = numargs; amhdr.handler = handler; amhdr.pe = gasneti_mynode;
    memcpy(dest_addr, source_addr, nbytes);
    gasnetc_AMProcess(&amhdr, _amstub.args);
  } else {
    size_t const len = GASNETC_LONG_HEADERSZ + 4 * numargs;

    /* Get a slot in shared AMQueue */
    int const myidx = gasnetc_AMQueueAcquire(dest, GASNETC_REQUEST_T);

  #if defined(GASNETC_GLOBAL_ADDRESS) && !defined(GASNET_SEGMENT_EVERYTHING)
    memcpy(dest_addr, source_addr, nbytes);
  #else
    shmem_putmem(dest_addr, source_addr, nbytes, dest);
  #endif
    shmem_quiet();
    /* Put the header and arguments, followed by payload and a fence */
    shmem_putmem(&gasnetc_amq_reqs[myidx].header, &_amstub, len, dest);
    shmem_fence();

    /* Release a slot in shared AMQueue */
    gasnetc_AMQueueRelease(dest, myidx);
  }
  retval = GASNET_OK;
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyShortM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            int numargs, ...) {
  int retval, i;
  va_list argptr;
  gasnet_node_t	dest;
  gasnetc_am_stub_t   _amstub;
  GASNETI_COMMON_AMREPLYSHORT(token,handler,numargs);
  va_start(argptr, numargs); /*  pass in last argument */

    /* add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */

  /* Write header and pack args */
  dest = GASNETC_AMHEADER_NODEID(token);
  _amstub.args[0] = GASNETC_AMHEADER_PACK(
			GASNETC_REPLY_T, GASNETC_AMSHORT_T, 
			numargs, handler, gasneti_mynode);
  for (i = 1; i <= numargs; i++)
	  _amstub.args[i] = (gasnet_handlerarg_t)va_arg(argptr, uint32_t);
  if (dest == gasneti_mynode) { /* loopback */
    static gasnetc_am_header_t amhdr = { GASNETC_REPLY_T, GASNETC_AMSHORT_T };
    amhdr.numargs = numargs; amhdr.handler = handler; amhdr.pe = gasneti_mynode;
    gasnetc_AMProcess(&amhdr, _amstub.args);
  } else {
    size_t const len = GASNETC_SHORT_HEADERSZ + 4 * numargs;

    /* Get a slot in shared AMQueue */
    int const myidx = gasnetc_AMQueueAcquire(dest, GASNETC_REPLY_T);

    /* Put the header and arguments */
    shmem_putmem(&gasnetc_amq_reqs[myidx].header, &_amstub, len, dest);
    shmem_fence();

    /* Release a slot in shared AMQueue */
    gasnetc_AMQueueRelease(dest, myidx);
  }
  retval = GASNET_OK;
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyMediumM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  int retval, i;
  va_list argptr;
  uint32_t *args, *pptr;
  size_t    len;
  gasnet_node_t	dest;
  gasnetc_am_stub_t   _amstub;
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,numargs);
  va_start(argptr, numargs); /*  pass in last argument */

    /* add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */

  /* Write args and pack a header */
  dest = GASNETC_AMHEADER_NODEID(token);
  _amstub.args[0] = GASNETC_AMHEADER_PACK(
			GASNETC_REPLY_T, GASNETC_AMMED_T, numargs, 
			handler, gasneti_mynode);
  _amstub.args[1] = nbytes;
  args = (uint32_t *) &_amstub.args[2];
  for (i = 0; i < numargs; i++)
	  args[i] = (uint32_t) (gasnet_handlerarg_t)va_arg(argptr, int);
  len = GASNETC_MED_HEADERSZ + 4 * numargs;

  if (dest == gasneti_mynode) { /* loopback */
    static char	_payload[GASNETC_MAX_MEDIUM_TOTAL+12]; /* XXX: needs to be tweaked if we ever add pthread support */
    static uint8_t *payload = NULL;
    static gasnetc_am_header_t amhdr = { GASNETC_REPLY_T, GASNETC_AMMED_T };
    if_pf (!payload) { /* match the alignment of _gasnetc_am_packet.header */
      payload = ((uint8_t *)GASNETI_ALIGNUP(&_payload[0],8))+4; 
      amhdr.pe = gasneti_mynode; 
    }
    amhdr.numargs = numargs; amhdr.handler = handler; 
    memcpy(payload, &_amstub, len);
    memcpy(payload + len + (GASNETC_MEDHEADER_PADARG(numargs)<<2), source_addr, nbytes);
    gasnetc_AMProcess(&amhdr, (uint32_t*)payload);
  } else {
    /* Get a slot in shared AMQueue */
    int const myidx = gasnetc_AMQueueAcquire(dest, GASNETC_REPLY_T);

    /* Adjust payload pointer according to numargs */
    pptr = (uint32_t *) &gasnetc_amq_reqs[myidx].payload + numargs + 1 +
	      GASNETC_MEDHEADER_PADARG(numargs);

    /* Put the header and arguments, followed by payload and a fence */
    shmem_putmem(&gasnetc_amq_reqs[myidx].header, &_amstub, len, dest);
    shmem_putmem(pptr, source_addr, nbytes, dest);
    shmem_fence();

    /* Release a slot in shared AMQueue */
    gasnetc_AMQueueRelease(dest, myidx);
  }
  retval = GASNET_OK;
  va_end(argptr);
  GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyLongM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
  int retval, i, myidx;
  gasnet_node_t dest;
  uint32_t *args;
  size_t len;
  va_list argptr;
  gasnetc_am_stub_t   _amstub;
  
  GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,numargs); 
  va_start(argptr, numargs); /*  pass in last argument */

    /* add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) 
             and send the active message 
     */

  /* Write args and pack a header */
  dest = GASNETC_AMHEADER_NODEID(token);
  _amstub.args[0] = GASNETC_AMHEADER_PACK(
			GASNETC_REPLY_T, GASNETC_AMLONG_T, numargs, 
			handler, gasneti_mynode);
  _amstub.args[1] = nbytes;
#if PLATFORM_ARCH_64
  _amstub.args[2] = (gasnet_handlerarg_t) GASNETI_HIWORD(dest_addr);
  _amstub.args[3] = (gasnet_handlerarg_t) GASNETI_LOWORD(dest_addr);
  args = &_amstub.args[4];
#else
  _amstub.args[2] = (gasnet_handlerarg_t) dest_addr;
  args = &_amstub.args[3];
#endif

  for (i = 0; i < numargs; i++)
	  args[i] = (gasnet_handlerarg_t)va_arg(argptr, uint32_t);

  if (dest == gasneti_mynode) { /* loopback */
    static gasnetc_am_header_t amhdr = { GASNETC_REPLY_T, GASNETC_AMLONG_T };
    amhdr.numargs = numargs; amhdr.handler = handler; amhdr.pe = gasneti_mynode;
    memcpy(dest_addr, source_addr, nbytes);
    gasnetc_AMProcess(&amhdr, _amstub.args);
  } else {
    size_t const len = GASNETC_LONG_HEADERSZ + 4 * numargs;

    /* Get a slot in shared AMQueue */
    int const myidx = gasnetc_AMQueueAcquire(dest, GASNETC_REPLY_T);

  #if defined(GASNETC_GLOBAL_ADDRESS) && !defined(GASNET_SEGMENT_EVERYTHING)
    memcpy(dest_addr, source_addr, nbytes);
  #else
    shmem_putmem(dest_addr, source_addr, nbytes, dest);
  #endif
    shmem_quiet();
    /* Put the header and arguments, followed by payload and a fence */
    shmem_putmem(&gasnetc_amq_reqs[myidx].header, &_amstub, len, dest);
    shmem_fence();

    /* Release a slot in shared AMQueue */
    gasnetc_AMQueueRelease(dest, myidx);
  }
  retval = GASNET_OK;
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

  { int retval; 
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

  GASNETI_TRACE_EVENT_TIME(L, HSL_UNLOCK, gasneti_ticks_now()-hsl->acquiretime);

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
#ifdef CRAY_SHMEM
  #define GASNETC_SHMALLOC_GRANULARITY	(256<<20)
#else
  #define GASNETC_SHMALLOC_GRANULARITY	(16<<20)
#endif

static
gasnet_seginfo_t
gasnetc_SHMallocBinarySearch(size_t low, size_t high)
{
	gasnet_seginfo_t	si;

	if (high - low <= GASNETC_SHMALLOC_GRANULARITY) {
		si.addr = NULL;
		si.size = 0;
		return si;
	}

	si.size = GASNETI_PAGE_ALIGNDOWN(low + (high-low)/2);

	si.addr = shmemalign(GASNETT_PAGESIZE, si.size);

	if (si.addr == NULL)
		return gasnetc_SHMallocBinarySearch(low, si.size);
	else {
		gasnet_seginfo_t	si_temp;

		shfree(si.addr);
		si_temp = gasnetc_SHMallocBinarySearch(si.size, high);
		if (si_temp.size)
			return si_temp;
		else
			return si;
	}
}

uintptr_t gasnetc_getMaxMem(void) {
  #ifdef CRAY_SHMEM
    return (uintptr_t)(64UL<<30);
  #else
    return gasneti_getPhysMemSz(1);
  #endif
}


static
uintptr_t
gasnetc_aligndown_pow2(uintptr_t addr)
{
    int	      i, first;
    int	      len = sizeof(uintptr_t)*8-1;
    uintptr_t mask;

    /* 
     * find first bit set and return if found
     */
    for (i = 0; i <= len; i++) {
	#if PLATFORM_ARCH_64
	  mask = 1ULL << (63-i);
	#else
	  mask = 1ULL << (31-i);
	#endif
	if (mask == addr)
	    return addr;
	else if (mask & addr)
	    return mask;
    }

    return 0;
}

static
uintptr_t
gasnetc_alignup_pow2(uintptr_t addr)
{
    int	      i, first;
    int	      len = sizeof(uintptr_t)*8-1;
    uintptr_t mask;

    /* 
     * find first bit set and return if found
     */
    for (i = 0; i <= len; i++) {
	#if PLATFORM_ARCH_64
	  mask = 1ULL << (63-i);
	#else
	  mask = 1ULL << (31-i);
	#endif
	if (mask == addr)
	    return addr;
	else if (mask & addr) 
	    return (mask << 1);
    }

    return 0;
}
    

static
gasnet_seginfo_t
gasnetc_SHMallocSegmentSearch()
{
	gasnet_seginfo_t    si;
	gasneti_tick_t  starttime, endtime;
	int64_t		    start, end;
	uintptr_t	    maxsz;

	maxsz = gasnetc_getMaxMem();
        #if GASNETI_ARCH_ALTIX
          maxsz = gasnetc_aligndown_pow2(maxsz/gasneti_nodes);
        #endif

        if (gasnet_max_segsize) { /* client-provided per-node size */
          gasnet_max_segsize = MIN(gasnet_max_segsize,maxsz);
        } else { /* default to full phys mem size */
          gasnet_max_segsize = maxsz; 
        }
        maxsz = GASNETI_MMAP_LIMIT;
        #if GASNETI_ARCH_ALTIX
            /* alignup here in case user requested a non-power of two size */
	    maxsz = gasnetc_alignup_pow2(maxsz);
        #endif

        if (gasnetc_verbose_spawn && gasneti_mynode == 0)
		printf("maxsiz = %lu (%.2f GB), pagesize=%d\n\n", 
			maxsz, (float) maxsz / (1024*1024*1024),
			(int) GASNET_PAGESIZE);

#if 0
		{
		    char      buf[64];
		    snprintf(buf, 64, "SMA_SYMMETRIC_SIZE=%lu", 
			    (unsigned long) alloc_perthread);
		    putenv(buf);
		}
#endif

	#if GASNETI_ARCH_ALTIX
	{
	    uintptr_t alloc_perthread;
	    double  frac;
 
	    alloc_perthread = maxsz;

	    starttime = gasneti_ticks_now();
	    si.addr = NULL;

	    while (alloc_perthread > 0) {
		si.addr = shmemalign(GASNETT_PAGESIZE, alloc_perthread);
                #if 0
		printf("Difference is %lx\n", (long)shmem_ptr(si.addr,1)  - (long)shmem_ptr(si.addr,0));
                #endif
		if (si.addr != NULL)
			break;
		alloc_perthread /= 2;
	    }
	    endtime = gasneti_ticks_now();

	    if (si.addr != NULL)
		si.size = alloc_perthread;
	}
	#else
	    starttime = gasneti_ticks_now();
	    si = gasnetc_SHMallocBinarySearch(0UL, maxsz);
	    endtime = gasneti_ticks_now();
	#endif

	if (gasnetc_verbose_spawn)
		printf("shmalloc search for %lu bytes (max=%lu) took %lu us (%p,%lu)\n", 
		    si.size, maxsz, 
		    (long)gasneti_ticks_to_ns(endtime-starttime)/1000,
		    (void*)si.addr,(uintptr_t)si.size);

	return si;
}

