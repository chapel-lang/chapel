/*   $Source: /var/local/cvs/gasnet/sci-conduit/gasnet_core.c,v $
 *     $Date: 2009/09/21 04:01:01 $
 * $Revision: 1.29 $
 * Description: GASNet sci conduit Implementation
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 *				   Hung-Hsun Su <su@hcs.ufl.edu>
 *				   Burt Gordon <gordon@hcs.ufl.edu>
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
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int, void*);
#else
static void gasnetc_atexit(void);
#endif

extern void gasnetc_exit(int exitcode);

/*  New variables for SCI Conduit */
#define GASNETC_SCI_FORCE_SCAN_THRESHOLD 1000000
volatile int            gasnetc_sci_MEF_zero_count = 0;
volatile int            gasnetc_exit_began = 0;
gasneti_mutex_t         gasnetc_sci_exit_lock = GASNETI_MUTEX_INITIALIZER;
gasneti_mutex_t	        gasnetc_sci_cb_exit = GASNETI_MUTEX_INITIALIZER;
gasneti_mutex_t		gasnetc_sci_AMPoll_mutex = GASNETI_MUTEX_INITIALIZER;

/* function to be called whenever we exit */
void gasnetc_sci_call_exit(unsigned int sig)
{
        int test;
        gasneti_mutex_lock(&gasnetc_sci_cb_exit);
        test = gasnetc_exit_began;
        gasneti_mutex_unlock(&gasnetc_sci_cb_exit);

        if(test == 0)
        {
                gasneti_local_wmb();
                gasnetc_exit(sig);
        }
}

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
	gasnetc_sci_internal_Barrier(); /* call to our sci barrier function */
}

static int gasnetc_init(int *argc, char ***argv) {
        sci_error_t gasnetc_sci_error;
  /*  check system sanity */
  gasnetc_check_config();

  if (gasneti_init_done)
    GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");

  gasneti_freezeForDebugger();

  #if GASNET_DEBUG_VERBOSE
    /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
    fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
  #endif

  /*  add code here to bootstrap the nodes for your conduit */

        SCIInitialize(GASNETC_SCI_NO_FLAGS, &gasnetc_sci_error); /* Initialize SISCI library */
    if (gasnetc_sci_error != SCI_ERR_OK)
        {
        gasneti_fatalerror("Could not get SCI initialized 0x%x\n",gasnetc_sci_error);
    }

  /* Now call a function to read node information and set up the node
     to have the proper number of segments, and know the segment size limit
     returns total number of nodes that will be running*/

  gasnetc_SCIInit();

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_init(): spawn successful - node %i/%i starting...\n",
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif

  /* (###) Add code here to determine which GASNet nodes may share memory.
     If the conduit has already communicated endpoint address information or
     a similar identifier that is unique per shared-memory compute node, then
     that info can be passed via arguments 2 through 4.
     Otherwise the conduit should pass a non-null gasnetc_bootstrapExchange
     as argument 1 to use platform-specific IDs, such as gethostid().
     See below for info on gasnetc_bootstrapExchange()

     If the conduit can build gasneti_nodemap[] w/o assistance, it should
     call gasneti_nodemapParse() after constructing it.
  */
  /* Currently support only one GASNet process per SCI ID, and therefore
   * the [0,1,2,...] nodemap is the correct one, and is also the natural
   * result of providing neither exchangefn nor ids to gasneti_nodemapInit().
   */
  gasneti_nodemapInit(NULL, NULL, 0, 0);

  #if GASNET_SEGMENT_FAST
    {
      /* already set in gasnetc_getSCIglobal_seg() and gasnetc_get_free_mem() */
      gasneti_assert(gasneti_MaxLocalSegmentSize > 0 && gasneti_MaxGlobalSegmentSize > 0 &&
             gasneti_MaxGlobalSegmentSize <= gasneti_MaxLocalSegmentSize);

      /* it may be appropriate to use gasneti_segmentInit() here to set
         gasneti_MaxLocalSegmentSize and gasneti_MaxGlobalSegmentSize,
         if your conduit can use memory anywhere in the address space
         (you may want to tune GASNETI_MMAP_MAX_SIZE to limit the max size)

         it may also be appropriate to first call gasneti_mmapLimit() to
         account for limitations imposed by having multiple GASNet nodes
         per shared-memory compute node
      */
    }
  #elif GASNET_SEGMENT_EVERYTHING || GASNET_SEGMENT_LARGE/* currently not implemented */
    #error Bad segment config
  #else
    #error Bad segment config
  #endif

  #if 0/* never */
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
      gasnetc_sci_handler_table[(gasnet_handler_t) newindex] = table[i].fnptr;

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
    for (i = 0; i < GASNETC_SCI_MAX_HANDLER_NUMBER; i++) 
      gasnetc_sci_handler_table[i] = &gasneti_defaultAMHandler;
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
   gasnetc_sci_handler_table[63] = gasnetc_sci_call_exit;

  #if HAVE_ON_EXIT
    on_exit(gasnetc_on_exit, NULL);
  #else
    atexit(gasnetc_atexit);
  #endif

  /* ------------------------------------------------------------------------------------ */
  /*  register segment  */

  gasneti_seginfo = (gasnet_seginfo_t *)gasneti_malloc(gasneti_nodes*sizeof(gasnet_seginfo_t));

  #if GASNET_SEGMENT_FAST
    if (segsize == 0) segbase = NULL; /* no segment */
    else {
      /* (###) add code here to choose and register a segment
         (ensuring alignment across all nodes if this conduit sets GASNET_ALIGNED_SEGMENTS==1)
         you can use gasneti_segmentAttach() here if you used gasneti_segmentInit() above
      */
          segbase = gasnetc_create_gasnetc_sci_seg(&segsize, gasneti_nodes);
      gasneti_assert(((uintptr_t)segbase) % GASNET_PAGESIZE == 0);
      gasneti_assert(segsize % GASNET_PAGESIZE == 0);
    }
  #else /*GASNET_SEGMENT_LARGE*/
    /* GASNET_SEGMENT_EVERYTHING */
    segbase = (void *)0;
    segsize = (uintptr_t)-1;
    /* (###) add any code here needed to setup GASNET_SEGMENT_EVERYTHING support */
        /*  SCI Conduit does not support GASNET_SEGMENT_EVERYTHING nor LARGE at this time due to SISCI API limitations */
  #endif

  /* ------------------------------------------------------------------------------------ */
  /*  gather segment information */

  /* (###) add code here to gather the segment assignment info into
           gasneti_seginfo on each node (may be possible to use AMShortRequest here)
   */
   gasnetc_get_SegInfo (gasneti_seginfo, segsize, segbase); /* place all segment information into the table */

   GASNETI_TRACE_PRINTF(C, ("Final segment: segbase="GASNETI_LADDRFMT"  segsize=%lu",
    GASNETI_LADDRSTR(segbase), (unsigned long)segsize));
  /* ------------------------------------------------------------------------------------ */
	/* create the environment for DMA transfers in SCI */
  gasnetc_create_dma_queues();

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
  gasnetc_sci_internal_barrier_flag = GASNETC_SCI_FALSE;

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

  int i;
  static int done = 0;
  sci_error_t gasnetc_sci_error;

  /* used to ensure gasnet_exit is not called by two functions at once */
  gasneti_mutex_lock(&gasnetc_sci_cb_exit);
  gasnetc_exit_began = 1;
  gasneti_mutex_unlock(&gasnetc_sci_cb_exit);

  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  gasneti_reghandler(SIGQUIT, SIG_IGN);

  /* ensure only one thread ever continues past this point */
  gasneti_mutex_lock(&gasnetc_sci_exit_lock); /* never unlock */

 GASNETI_TRACE_PRINTF(C,("gasnet_exit(%i)\n", exitcode));

  gasneti_flush_streams();
  gasneti_trace_finish();
  gasneti_sched_yield();

  /* (###) add code here to terminate the job across _all_ nodes
           with gasneti_killmyprocess(exitcode) (not regular exit()), preferably
           after raising a SIGQUIT to inform the client of the exit
  */

  /* disconnect from everybody -- triggers a callback function on everybody's node and tells them to shut down
     This disconnects all mailboxes and payload regions
  */

  for (i = 0; i < (gasneti_nodes+2) ; i++)
  {
	  if( (gasneti_attach_done == 1)) /*gasnet_attach() completed successfully so disconnect all segments from the network*/
	  {
		SCISetSegmentUnavailable(gasnetc_sci_localSegment[i], gasnetc_sci_localAdapterNo,SCI_FLAG_FORCE_DISCONNECT,&gasnetc_sci_error);
	  }
	  else
	  {/* attach was not completed so don't set the main payload region unavailable as it
	          was never created. This should be the rare case.*/
		if( i != gasneti_nodes ) /* as long as this isn't the payload region, disconnect from the network */
		  {
			SCISetSegmentUnavailable(gasnetc_sci_localSegment[i], gasnetc_sci_localAdapterNo,SCI_FLAG_FORCE_DISCONNECT,&gasnetc_sci_error);
		  }
	  }

  }

  /* if attach was completed, disconnect from all the remote segments*/
  if(gasneti_attach_done == 1)
	{
	  for (i =0 ; i < gasneti_nodes ; i++)
	  {
		  	  SCIDisconnectSegment(gasnetc_sci_remoteSegment_long[i],GASNETC_SCI_NO_FLAGS,&gasnetc_sci_error);
	  }
	}

  /* unmap all segments */
  for (i = 0; i < gasneti_nodes; i++ )
  {
	  SCIUnmapSegment(gasnetc_sci_localMap[i], GASNETC_SCI_NO_FLAGS, &gasnetc_sci_error);
	  SCIUnmapSegment(gasnetc_sci_remoteMap[i], GASNETC_SCI_NO_FLAGS, &gasnetc_sci_error);
	 /* SCIUnmapSegment(gasnetc_sci_remoteMap_gb[i], GASNETC_SCI_NO_FLAGS, &gasnetc_sci_error); -- forced disconnect takes care of this*/
  }

  /* close all descriptors */
  for (i = 0; i < (gasneti_nodes + 2); i++)
  {
	  if( i < gasneti_nodes)
	  {
		  SCIClose(gasnetc_sci_sd_remote[i], GASNETC_SCI_NO_FLAGS, &gasnetc_sci_error);
		  SCIClose(gasnetc_sci_sd_gb[i], GASNETC_SCI_NO_FLAGS, &gasnetc_sci_error);
	  }
	  SCIClose(gasnetc_sci_sd[i], GASNETC_SCI_NO_FLAGS, &gasnetc_sci_error);
  }

	if(gasneti_attach_done == 1)
	{
	  for (i =0 ; i < gasneti_nodes ; i++)
	  {
		  	SCIClose(gasnetc_sci_sd_long[i], GASNETC_SCI_NO_FLAGS, &gasnetc_sci_error);
	  }
	}

  /* free all SCI related resources */
  SCITerminate();

  /* free all memory allocated by the conduit */
  gasnetc_free_env ();

  done =1;

  gasneti_killmyprocess(exitcode);
}

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/
extern int gasnetc_AMGetMsgSource(gasnet_token_t token, gasnet_node_t *srcindex) {
  gasnet_node_t sourceid;
  gasnetc_sci_token_t * curr_token = (gasnetc_sci_token_t *) token;

  GASNETI_CHECKATTACH();
  GASNETI_CHECK_ERRR((!token),BAD_ARG,"bad token");
  GASNETI_CHECK_ERRR((!srcindex),BAD_ARG,"bad src ptr");

  /* (###) add code here to write the source index into sourceid */
  sourceid = curr_token->source_id;

  gasneti_assert(sourceid < gasneti_nodes);
  *srcindex = sourceid;
  return GASNET_OK;
}

extern int gasnetc_AMPoll(void) {
	int retval;
	GASNETI_CHECKATTACH();

	/* (###) add code here to run your AM progress engine */
	if ((gasnetc_msg_exist_flag_status () == GASNETC_SCI_TRUE) ||
            (gasnetc_sci_MEF_zero_count > GASNETC_SCI_FORCE_SCAN_THRESHOLD))
        {
            gasneti_mutex_lock(&gasnetc_sci_AMPoll_mutex);
            if ((gasnetc_msg_exist_flag_status () == GASNETC_SCI_TRUE) ||
                (gasnetc_sci_MEF_zero_count > GASNETC_SCI_FORCE_SCAN_THRESHOLD))
                {
                    gasnetc_msg_exist_flag_release ();
                    gasnetc_sci_MEF_zero_count = 0;
                    gasnetc_MRF_scan ();
                }
            gasneti_mutex_unlock(&gasnetc_sci_AMPoll_mutex);
        }
        else
        {
            gasnetc_sci_MEF_zero_count++;
	}

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
        int i;
        gasnet_handlerarg_t args[gasnet_AMMaxArgs()];

        GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs);

	va_start(argptr, numargs); /*  pass in last argument */

	/*  (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) and send the active message */
	for (i = 0; i < numargs; i++)
	{
		args[i] = va_arg (argptr, gasnet_handlerarg_t);
	}

	retval = gasnetc_SM_request (dest, GASNETC_SCI_SHORT, handler, numargs, args, NULL, 0, NULL);

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
        int i;
        gasnet_handlerarg_t args[gasnet_AMMaxArgs()];

        GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);

	va_start(argptr, numargs); /*  pass in last argument */

        /*  (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) and send the active message */
	for (i = 0; i < numargs; i++)
	{
		args[i] = va_arg (argptr, gasnet_handlerarg_t);
	}

	retval = gasnetc_SM_request (dest, GASNETC_SCI_MEDIUM, handler, numargs, args, source_addr, nbytes, NULL);

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
        int i;
        gasnet_handlerarg_t args[gasnet_AMMaxArgs()];

        GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);

	va_start(argptr, numargs); /*  pass in last argument */

        /*  (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) and send the active message */
	for (i = 0; i < numargs; i++)
	{
		args[i] = va_arg (argptr, gasnet_handlerarg_t);
	}

	retval = gasnetc_SM_request (dest, GASNETC_SCI_LONG, handler, numargs, args, source_addr, nbytes, dest_addr);

        va_end(argptr);

	GASNETI_RETURN(retval);
}

extern int gasnetc_AMRequestLongAsyncM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...) {
	/*  This is implemented exactly like gasnetc_AMRequestLongM () */
	int retval;
	va_list argptr;
        int i;
        gasnet_handlerarg_t args[gasnet_AMMaxArgs()];

        GASNETI_COMMON_AMREQUESTLONGASYNC(dest,handler,source_addr,nbytes,dest_addr,numargs);

	va_start(argptr, numargs); /*  pass in last argument */

        /*  (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) and send the active message */
	for (i = 0; i < numargs; i++)
	{
		args[i] = va_arg (argptr, gasnet_handlerarg_t);
	}

	retval = gasnetc_SM_request (dest, GASNETC_SCI_LONG, handler, numargs, args, source_addr, nbytes, dest_addr);

	va_end(argptr);

	GASNETI_RETURN(retval);
}

extern int gasnetc_AMReplyShortM(
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */
                            int numargs, ...) {
	int retval;
	va_list argptr;
        int i;
        gasnetc_sci_token_t * curr_token = (gasnetc_sci_token_t *) token;
        gasnet_handlerarg_t args[gasnet_AMMaxArgs()];

        GASNETI_COMMON_AMREPLYSHORT(token,handler,numargs);

	va_start(argptr, numargs); /*  pass in last argument */

        /*  (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) and send the active message */
	for (i = 0; i < numargs; i++)
	{
		args[i] = va_arg (argptr, gasnet_handlerarg_t);
	}

	gasnetc_mls_set (curr_token->source_id, curr_token->msg_number + GASNETC_SCI_MAX_REQUEST_MSG);
	retval = gasnetc_SM_transfer (curr_token->source_id, GASNETC_SCI_MAX_REQUEST_MSG + curr_token->msg_number, GASNETC_SCI_REPLY, GASNETC_SCI_SHORT, handler, numargs, args, NULL, 0, NULL);

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
        int i;
        gasnetc_sci_token_t * curr_token = (gasnetc_sci_token_t *) token;
        gasnet_handlerarg_t args[gasnet_AMMaxArgs()];

        GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,numargs);

	va_start(argptr, numargs); /*  pass in last argument */

        /*  (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) and send the active message */
	for (i = 0; i < numargs; i++)
	{
		args[i] = va_arg (argptr, gasnet_handlerarg_t);
	}

	gasnetc_mls_set (curr_token->source_id, curr_token->msg_number + GASNETC_SCI_MAX_REQUEST_MSG);
	retval = gasnetc_SM_transfer (curr_token->source_id, GASNETC_SCI_MAX_REQUEST_MSG + curr_token->msg_number, GASNETC_SCI_REPLY, GASNETC_SCI_MEDIUM, handler, numargs, args, source_addr, nbytes, NULL);

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
        int i;
        gasnetc_sci_token_t * curr_token = (gasnetc_sci_token_t *) token;
        gasnet_handlerarg_t args[gasnet_AMMaxArgs()];

        GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,numargs); 

	va_start(argptr, numargs); /*  pass in last argument */

        /*  (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t) and send the active message */
	for (i = 0; i < numargs; i++)
	{
		args[i] = va_arg (argptr, gasnet_handlerarg_t);
	}

	/*  command transfer */
	gasnetc_mls_set (curr_token->source_id, curr_token->msg_number + GASNETC_SCI_MAX_REQUEST_MSG);
	retval = gasnetc_SM_transfer (curr_token->source_id, GASNETC_SCI_MAX_REQUEST_MSG + curr_token->msg_number, GASNETC_SCI_REPLY, GASNETC_SCI_LONG, handler, numargs, args, source_addr, nbytes, dest_addr);

	va_end(argptr);

	GASNETI_RETURN(retval);
}

/* ------------------------------------------------------------------------------------ */
/*
  No-interrupt sections
  =====================
  This section is only required for conduits that may use interrupt-based handler dispatch
  See the GASNet spec and http:// www.cs.berkeley.edu/~bonachea/upc/gasnet.html for
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
