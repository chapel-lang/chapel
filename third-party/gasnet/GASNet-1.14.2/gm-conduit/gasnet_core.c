/* $Source: /var/local/cvs/gasnet/gm-conduit/gasnet_core.c,v $
 * $Date: 2009/09/21 02:22:32 $
 * $Revision: 1.128 $
 * Description: GASNet GM conduit Implementation
 * Copyright 2002, Christian Bell <csbell@cs.berkeley.edu>
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>
#include <firehose.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

uintptr_t	gasnetc_MaxPinnableMemory = 0;
static size_t	gasnetc_packed_long_limit = 0;
#define		GASNETC_PACKEDLONG_LIMIT_DEFAULT	(2*1024)

firehose_info_t	  gasnetc_firehose_info;

gasneti_mutex_t gasnetc_lock_gm      = GASNETI_MUTEX_INITIALIZER;
gasneti_mutex_t gasnetc_lock_reqpool = GASNETI_MUTEX_INITIALIZER;
gasneti_mutex_t gasnetc_lock_amreq   = GASNETI_MUTEX_INITIALIZER;
gasneti_atomic_t gasnetc_exit_running = gasneti_atomic_init(0);	/* boolean used to coordinate w/ AMPoll() */

gasnetc_state_t _gmc;

gasnet_handlerentry_t const		*gasnetc_get_handlertable(void);
extern gasnet_handlerentry_t const	*gasnete_get_handlertable(void);
extern gasnet_handlerentry_t const	*gasnete_get_extref_handlertable(void);

#if HAVE_ON_EXIT
static void gasnetc_on_exit(int, void*);
#else
static void gasnetc_atexit(void);
#endif

/* Exit coordination timeouts */
#define GASNETC_DEFAULT_EXITTIMEOUT_MAX		360.0	/* 6 minutes! */
#define GASNETC_DEFAULT_EXITTIMEOUT_MIN		2	/* 2 seconds */
#define GASNETC_DEFAULT_EXITTIMEOUT_FACTOR	0.25	/* 1/4 second */
static double gasnetc_exittimeout = GASNETC_DEFAULT_EXITTIMEOUT_MAX;

/*
  Initialization
  ==============
*/
/* called at startup to check configuration sanity */
static void gasnetc_check_config(void) {
  gasneti_check_config_preinit();

  gasneti_assert_always(GASNETC_AM_SIZE >= 12);
  gasneti_assert_always(
    gm_min_size_for_length(GASNETC_AM_MEDIUM_MAX) <= GASNETC_AM_SIZE);
  gasneti_assert_always(
    gm_min_size_for_length(GASNETC_AM_LONG_REPLY_MAX) <= GASNETC_AM_SIZE);
  gasneti_assert_always(
    gm_max_length_for_size(GASNETC_AM_SIZE) <= GASNETC_AM_PACKET);
  gasneti_assert_always(GASNETC_AM_MEDIUM_MAX <= (uint16_t)(-1));
  gasneti_assert_always(GASNETC_AM_MAX_HANDLERS >= 256);
  return;
}

static int 
gasnetc_init(int *argc, char ***argv)
{
	uintptr_t   max_segmentsize;

	/* check system sanity */
	gasnetc_check_config();

	if (gasneti_init_done) 
		GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");

        gasneti_freezeForDebugger();

	#if GASNET_DEBUG_VERBOSE
	/* note - can't call trace macros during gasnet_init because trace
	 * system not yet initialized */
	fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
	#endif

	gasnetc_registerSysHandlers();
	gasnetc_getconf(argc, argv);
	gasnetc_AllocPinnedBufs();
	gasnetc_AllocGatherBufs();

        /* Construct nodemap from GM ids */
        gasneti_nodemapInit(NULL, &_gmc.gm_nodes[0].id,
                            sizeof(_gmc.gm_nodes[0].id),
                            sizeof(_gmc.gm_nodes[0]));

        #if GASNET_PSHM
          gasneti_pshm_init(&gasnetc_bootstrapExchange, 0);
        #endif

	gasnetc_bootstrapBarrier();
	gasneti_init_done = 1; /* Not really done, but need getenv internally */
	/* non-zero gasneti_init_done also enables AMPSH for System AMs */

	gasnetc_exittimeout = gasneti_get_exittimeout(	GASNETC_DEFAULT_EXITTIMEOUT_MAX,
							GASNETC_DEFAULT_EXITTIMEOUT_MIN,
							GASNETC_DEFAULT_EXITTIMEOUT_FACTOR,
							GASNETC_DEFAULT_EXITTIMEOUT_MIN);

	/* Upper bound on "packed long" */
	gasnetc_packed_long_limit = gasneti_getenv_int_withdefault("GASNET_PACKEDLONG_LIMIT", GASNETC_PACKEDLONG_LIMIT_DEFAULT, 1);
	if (gasnetc_packed_long_limit > GASNETC_AM_LEN-GASNETC_LONG_OFFSET) {
	    fprintf(stderr, "WARNING: GASNET_PACKEDLONG_LIMIT reduced from requested value %d to maximum supported value %d.\n", (int)gasnetc_packed_long_limit, (int)(GASNETC_AM_LEN-GASNETC_LONG_OFFSET));
	    gasnetc_packed_long_limit = GASNETC_AM_LEN-GASNETC_LONG_OFFSET;
	}

	/* 
	 * Find the upper bound on pinnable memory for firehose algorithm.
	 *
	 * After getting a global minimum on the amount of pinnable physical
	 * memory, we ask firehose what the M/MaxVictim parameters are for the
	 * current job.
	 */
	{
		int	i;
		uintptr_t M, MaxVictim;
		float pm_ratio;
		uintptr_t *global_exch = (uintptr_t *)
		    gasneti_malloc(gasneti_nodes*sizeof(uintptr_t));

                pm_ratio = gasneti_getenv_dbl_withdefault(
                        "GASNET_PHYSMEM_PINNABLE_RATIO", 
                        GASNETC_DEFAULT_PHYSMEM_PINNABLE_RATIO);

		if_pf (pm_ratio <= 0.0 || pm_ratio >= 1.0)
		    gasneti_fatalerror("GASNET_PHYSMEM_PINNABLE_RATIO "
				       "must be between 0 and 1");

                /* Take only a fair share of the memory */
                { uint64_t my_physmem = gasneti_getPhysMemSz(1) * pm_ratio / gasneti_nodemap_local_count;
#if SIZEOF_VOID_P != 8 /* Watch for overflow! */
                  if (my_physmem > (uint64_t)(uintptr_t)-1) my_physmem = (uintptr_t)-1;
#endif
                  gasnetc_MaxPinnableMemory = (uintptr_t)my_physmem;
                }

		gasnetc_bootstrapExchange(
		    &gasnetc_MaxPinnableMemory, sizeof(uintptr_t), global_exch);

		for (i = 0; i < gasneti_nodes; i++) 
		    gasnetc_MaxPinnableMemory = 
			MIN(gasnetc_MaxPinnableMemory, global_exch[i]);
		gasneti_free(global_exch);

		firehose_get_params(gasnetc_MaxPinnableMemory, &M, &MaxVictim);
		#ifdef GASNET_SEGMENT_FAST
		    max_segmentsize = M;
		#else
		    max_segmentsize = (uintptr_t) -1;
		#endif
	}

        #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
	  max_segmentsize = gasneti_mmapLimit(max_segmentsize, -1,
                                              &gasnetc_bootstrapExchange,
                                              &gasnetc_bootstrapBarrier);
	  gasneti_segmentInit(max_segmentsize, &gasnetc_bootstrapExchange);
	#elif GASNET_SEGMENT_EVERYTHING
          /* segment is everything - nothing to do */
	#else
	  #error Bad segment config
	#endif

	/* Handler for non-collective returns from main() */
        #if HAVE_ON_EXIT
	  on_exit(gasnetc_on_exit, NULL);
        #else
	  atexit(gasnetc_atexit);
        #endif

	gasneti_init_done = 1;
	gasneti_trace_init(argc, argv);

        gasneti_auxseg_init(); /* adjust max seg values based on auxseg */

	#if GASNET_DEBUG_VERBOSE
	printf("%d> done init\n", gasneti_mynode);
	fflush(stdout);
	#endif
	return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
static char checkuniqhandler[256] = { 0 };
void
gasnetc_AM_InitHandler(void)
{
	int	i;

	for (i = 0; i < GASNETC_AM_MAX_HANDLERS; i++) 
		_gmc.handlers[i] = (gasneti_handler_fn_t) gasneti_defaultAMHandler;  

	return;
}

int
gasnetc_AM_SetHandler(gasnet_handler_t handler, gasneti_handler_fn_t func)
{
	if (!handler || func == NULL)
		GASNETI_RETURN_ERRR(BAD_ARG, "Invalid handler paramaters set");
		
	_gmc.handlers[handler] = func;
	return GASNET_OK;
}

int
gasnetc_AM_SetHandlerAny(gasnet_handler_t *handler, gasneti_handler_fn_t func)
{
	int	i;

	if (handler == NULL || func == NULL)
		GASNETI_RETURN_ERRR(BAD_ARG, "Invalid handler paramaters set");

	for (i = 1; i < GASNETC_AM_MAX_HANDLERS; i++) {
		if (_gmc.handlers[i] == gasneti_defaultAMHandler) {
			_gmc.handlers[i] = func;
			*handler = i;
			return GASNET_OK;
		}
	}
	return GASNET_OK;
}

static int 
gasnetc_reghandlers(gasnet_handlerentry_t *table, int numentries,
                               int lowlimit, int highlimit,
                               int dontcare, int *numregistered) {
  int i, retval;
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
    retval = gasnetc_AM_SetHandler((gasnet_handler_t) newindex, table[i].fnptr);
    if (retval != GASNET_OK)
        GASNETI_RETURN_ERRR(RESOURCE, "AM_SetHandler() failed while registering core handlers");

    /* The check below for !table[i].index is redundant and present
     * only to defeat the over-aggressive optimizer in pathcc 2.1
     */
    if (dontcare && !table[i].index) table[i].index = newindex;

    (*numregistered)++;
  }
  return GASNET_OK;
}
/* ------------------------------------------------------------------------------------ */
extern int 
gasnetc_attach(gasnet_handlerentry_t *table, int numentries, uintptr_t segsize,
	       uintptr_t minheapoffset)
{
	int i = 0, fidx = 0;

	#if GASNET_DEBUG_VERBOSE
	printf("%d> starting attach\n", gasneti_mynode);
	fflush(stdout);
	#endif

	GASNETI_TRACE_PRINTF(C,
	    ("gasnetc_attach(table (%i entries), segsize=%lu, minheapoffset=%lu)",
	    numentries, (unsigned long)segsize, (unsigned long)minheapoffset));

	if (!gasneti_init_done) 
		GASNETI_RETURN_ERRR(NOT_INIT,
		    "GASNet attach called before init");
	if (gasneti_attach_done) 
		GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already attached");

        #if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
	if ((segsize % GASNET_PAGESIZE) != 0) 
		GASNETI_RETURN_ERRR(BAD_ARG, "segsize not page-aligned");
	if (segsize > gasneti_MaxLocalSegmentSize) 
		GASNETI_RETURN_ERRR(BAD_ARG, "segsize too large");
	minheapoffset = 
	    GASNETI_ALIGNUP(minheapoffset, GASNETC_SEGMENT_ALIGN);
	#else
	segsize = 0;
	minheapoffset = 0;
	#endif

        segsize = gasneti_auxseg_preattach(segsize); /* adjust segsize for auxseg reqts */

	/*  register handlers */
	gasnetc_AM_InitHandler();

	{ /*  core API handlers */
		gasnet_handlerentry_t *ctable = 
		    (gasnet_handlerentry_t *) gasnetc_get_handlertable();

		int c_len = 0;
		int c_numreg = 0;

		gasneti_assert(ctable);
		while (ctable[c_len].fnptr) c_len++; /* calc len */
		if (gasnetc_reghandlers(ctable, c_len, 1, 63, 0, &c_numreg)
		    != GASNET_OK)
			GASNETI_RETURN_ERRR(RESOURCE,
			    "Error registering core API handlers");
		gasneti_assert(c_numreg == c_len);
	}
	{ /*  extended API handlers */
		gasnet_handlerentry_t *ertable = 
		    (gasnet_handlerentry_t *)gasnete_get_extref_handlertable();
		gasnet_handlerentry_t *etable = 
		    (gasnet_handlerentry_t *)gasnete_get_handlertable();
		int er_len = 0, e_len = 0;
		int er_numreg = 0, e_numreg = 0;
		gasneti_assert(etable && ertable);
	
		while (ertable[er_len].fnptr) er_len++; /* calc len */
		while (etable[e_len].fnptr) e_len++; /* calc len */
		if (gasnetc_reghandlers(ertable, er_len, 64, 127, 0, 
		    &er_numreg) != GASNET_OK)
			GASNETI_RETURN_ERRR(RESOURCE,
			    "Error registering extended reference API handlers");
	    	gasneti_assert(er_numreg == er_len);
	
#if 0
		if (gasnetc_reghandlers(etable, e_len, 64+er_len, 127, 0, 
		    &e_numreg) != GASNET_OK)
			GASNETI_RETURN_ERRR(RESOURCE,
			    "Error registering extended API handlers");
	    	gasneti_assert(e_numreg == e_len);
		fidx = 64+er_len_e_len;
#else
		/* This deals with the non-contiguous allocation of handlers by the ref collectives.
		 * XXX: a better solution is needed */
		if (gasnetc_reghandlers(etable, e_len, 1+ertable[er_len-1].index, 127, 0, 
		    &e_numreg) != GASNET_OK)
			GASNETI_RETURN_ERRR(RESOURCE,
			    "Error registering extended API handlers");
	    	gasneti_assert(e_numreg == e_len);
		if (e_len > 0)
		    fidx = 1+etable[e_len-1].index;
		else
		    fidx = 1+ertable[er_len-1].index;
#endif
	}
	{ /* firehose handlers */
		gasnet_handlerentry_t *ftable = firehose_get_handlertable();
		int f_len = 0;
		int f_numreg = 0;

		gasneti_assert(ftable);

		while (ftable[f_len].fnptr)
			f_len++;

		gasneti_assert(fidx + f_len <= 128);
		if (gasnetc_reghandlers(ftable, f_len, fidx, 127, 1, &f_numreg)
		    != GASNET_OK)
			GASNETI_RETURN_ERRR(RESOURCE,
			    "Error registering firehose handlers");
		gasneti_assert(f_numreg == f_len);
	}

	if (table) { /*  client handlers */
		int numreg1 = 0;
		int numreg2 = 0;

		/*  first pass - assign all fixed-index handlers */
		if (gasnetc_reghandlers(table, numentries, 128, 255, 0, &numreg1) 
		    != GASNET_OK)
			GASNETI_RETURN_ERRR(RESOURCE,
			    "Error registering fixed-index client handlers");

		/*  second pass - fill in dontcare-index handlers */
		if (gasnetc_reghandlers(table, numentries, 128, 255, 1, &numreg2) 
		    != GASNET_OK)
			GASNETI_RETURN_ERRR(RESOURCE,
			    "Error registering fixed-index client handlers");

		gasneti_assert(numreg1 + numreg2 == numentries);
	}

	/* -------------------------------------------------------------------- */
	/*  register fatal signal handlers */

	/*  catch fatal signals and convert to SIGQUIT */
	gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

	/* -------------------------------------------------------------------- */
	/*  register segment  */

	gasneti_seginfo = (gasnet_seginfo_t *)
	    gasneti_calloc(gasneti_nodes, sizeof(gasnet_seginfo_t));

	#if GASNET_DEBUG_VERBOSE
	printf("%d> before firehose init\n", gasneti_mynode);
	fflush(stdout);
	#endif
	#if defined(GASNET_SEGMENT_FAST) 
	{
		firehose_region_t	prereg, *preg = NULL;
		int			pnum = 0;

		if (segsize > 0) {
			gasneti_segmentAttach(segsize, minheapoffset,
					gasneti_seginfo,
					&gasnetc_bootstrapExchange);

			prereg.addr = (uintptr_t) 
				gasneti_seginfo[gasneti_mynode].addr;
			prereg.len = gasneti_seginfo[gasneti_mynode].size;

			if (gm_register_memory(_gmc.port, 
			    (void *) prereg.addr, prereg.len) != GM_SUCCESS)
				gasneti_fatalerror(
				    "Can't pin FAST Segment of %.2f MB", 
				    (float) prereg.len / (1024.0*1024.0));
			pnum++;
			preg = &prereg;
		}

		firehose_init(gasnetc_MaxPinnableMemory, 0, 0, preg, pnum,
					0, &gasnetc_firehose_info);
	}
	#else /* GASNET_SEGMENT_EVERYTHING | GASNET_SEGMENT_LARGE */
	{
		#if defined(GASNET_SEGMENT_LARGE)
		if (segsize > 0) {
			gasneti_segmentAttach(segsize, minheapoffset, 
			    gasneti_seginfo, &gasnetc_bootstrapExchange);
		}
		#else
		    for (i=0;i<gasneti_nodes;i++) {
			gasneti_seginfo[i].addr = (void *)0;
			gasneti_seginfo[i].size = (uintptr_t)-1;
		    }
		#endif

		firehose_init(gasnetc_MaxPinnableMemory, 0, 0, NULL, 0,
			0, &gasnetc_firehose_info);
	}
	#endif

        #if GASNET_TRACE
	for (i = 0; i < gasneti_nodes; i++)
		GASNETI_TRACE_PRINTF(C, ("SEGINFO at %4d ("GASNETI_LADDRFMT", %d)", i,
		    GASNETI_LADDRSTR(gasneti_seginfo[i].addr), 
		    (unsigned int) gasneti_seginfo[i].size) );
	#endif

	/* -------------------------------------------------------------------- */
	/* 
	 * Set up other conduit options 
	 */
        /* GM gets: default is determined by compile-time setting */
	gasnete_getrdma_enabled = 
          gasneti_getenv_yesno_withdefault("GASNET_GM_RDMA_GETS", GASNET_GM_RDMA_GETS);

        #if defined(GASNETC_GM_2)
	  if (gasnete_getrdma_enabled == 0 && gasnet_mynode() == 0) {
	    char *nowarn = gasneti_getenv("GASNET_GM_NO_RDMAGET_WARNING");
            /* 
               GM_gets make a major performance difference, so make it intentionally
               painful and conspicuous if they're disabled for any reason on a GM2 system

               We *always* issue a runtime warning for using GM2 without GM_gets
               unless the user explicitly squelches this specific warning with 
               GASNET_GM_NO_RDMAGET_WARNING. 
               (*never* set this up to happen automatically for end-users)
             */
	    if (nowarn == NULL || *nowarn == '\0') {
	        fprintf(stderr, 
                    "PERFORMANCE WARNING:\n"
		    "  GASNet/GM support for RDMA gets is disabled"
	          #if GASNETC_BROKEN_GM2_VERSION
                    " because of a broken 2.x GM build -- your GM drivers should be updated\n"
                  #else
                    " because of a GASNET_GM_RDMA_GETS environment variable\n"
	          #endif
                );
                fflush(stderr);
	    }
          }
        #else /* GM 1.x */
          if (gasnete_getrdma_enabled && gasnet_mynode() == 0) {
            fprintf(stderr, "WARNING: GM gets are not supported in GM 1.x - GASNET_GM_RDMA_GETS ignored\n");
            fflush(stderr);
            gasnete_getrdma_enabled = 0;
          }
        #endif

	/* -------------------------------------------------------------------- */
	/*  primary attach complete */
	gasneti_attach_done = 1;

	GASNETI_TRACE_PRINTF(C,("gasnetc_attach(): primary attach complete"));

	gasnetc_bootstrapBarrier();
        gasneti_auxseg_attach(); /* provide auxseg */

	gasnete_init();
	gasnetc_bootstrapBarrier();

        gasneti_nodemapFini();

	/*
	 * Free up the bootstrap gather buffers.  If they are reused, an AM
	 * will collectively allocate new memory.
	 */
	gasnetc_DestroyGatherBufs();
	gasnetc_dump_tokens();

	return GASNET_OK;
}

extern int 
gasnet_init(int *argc, char ***argv)
{
	GASNETI_RETURN(gasnetc_init(argc, argv));
}

/* ------------------------------------------------------------------------------------ */
extern void 
gasnetc_exit_old(int exitcode)
{
	/* once we start a shutdown, ignore all future SIGQUIT signals or we
	 * risk reentrancy */
	gasneti_reghandler(SIGQUIT, SIG_IGN);

        {  /* ensure only one thread ever continues past this point */
          static gasneti_mutex_t exit_lock = GASNETI_MUTEX_INITIALIZER;
          gasneti_mutex_lock(&exit_lock);
        }

	#if defined(GASNET_SEGMENT_FAST)
	if (gasneti_attach_done && gasneti_seginfo[gasneti_mynode].size > 0 &&
            gm_deregister_memory(_gmc.port, 
	    (void *) gasneti_seginfo[gasneti_mynode].addr,
	    gasneti_seginfo[gasneti_mynode].size) != GM_SUCCESS)
		fprintf(stderr, 
		    "%d> Couldn't deregister prepinned segment",
		    gasneti_mynode);
	#endif	

	gasnetc_DestroyPinnedBufs();

        gasneti_flush_streams();
        gasneti_trace_finish();
        gasneti_sched_yield();

	if (gasneti_init_done) {
  		gm_close(_gmc.port);
		if (gasneti_attach_done)
			firehose_fini();
	}
	gm_finalize();
	_exit(exitcode);
}


/* -------------------------------------------------------------------------- */
/*
  Exit handling code (originates from Paul's vapi-conduit)
*/

#ifndef GASNETI_HAVE_ATOMIC_CAS
  #error "required atomic compare-and-swap is not yet implemented for your CPU/OS/compiler"
#endif


static gasneti_atomic_t gasnetc_exit_code = gasneti_atomic_init(0);	/* value to _exit() with */
static gasneti_atomic_t gasnetc_exit_reqs = gasneti_atomic_init(0);	/* count of remote exit requests */
static gasneti_atomic_t gasnetc_exit_reps = gasneti_atomic_init(0);	/* count of remote exit replies */
static gasneti_atomic_t gasnetc_exit_done = gasneti_atomic_init(0);	/* flag for exit coordination done */

#define GASNETC_ROOT_NODE 0

enum {
  GASNETC_EXIT_ROLE_UNKNOWN,
  GASNETC_EXIT_ROLE_MASTER,
  GASNETC_EXIT_ROLE_SLAVE
};

static gasneti_atomic_t gasnetc_exit_role = gasneti_atomic_init(GASNETC_EXIT_ROLE_UNKNOWN);

/*
 * Code to disable user's AM handlers when exiting.  We need this because we must call
 * AMPoll to run system-level handlers, including ACKs for flow control.
 *
 * We do it this way because it adds absolutely nothing the normal execution path.
 * Thanks to Dan for the suggestion.
 */
static void gasnetc_noop(void) { return; }

static void 
gasnetc_disable_AMs(void) 
{
  int i;

  for (i = 0; i < GASNETC_AM_MAX_HANDLERS; ++i) {
    _gmc.handlers[i] = (gasneti_handler_fn_t)&gasnetc_noop;
  }
}

/*
 * gasnetc_SysExitRole_reqh()
 *
 * This request handler (invoked only on the "root" node) handles the election
 * of a single exit "master", who will coordinate an orderly shutdown.
 */
static void 
gasnetc_SysExitRole_reqh(gasnet_token_t token, void *nop, size_t nsz)
{
  gasnet_node_t src;
  int local_role, result;
  int rc;

  /* May only send this request to the root node */
  gasneti_assert(gasneti_mynode == GASNETC_ROOT_NODE);	
  
  /* What role would the local node get if the requester is made the master? */
  rc = gasnet_AMGetMsgSource(token, &src);
  gasneti_assert(rc == GASNET_OK);
  local_role = (src == GASNETC_ROOT_NODE) 
	       ? GASNETC_EXIT_ROLE_MASTER : GASNETC_EXIT_ROLE_SLAVE;

  /* Try atomically to assume the proper role.  Result determines role of requester */
  result = gasneti_atomic_compare_and_swap(&gasnetc_exit_role,
		  			   GASNETC_EXIT_ROLE_UNKNOWN, local_role, 0)
           ? GASNETC_EXIT_ROLE_MASTER : GASNETC_EXIT_ROLE_SLAVE;

  /* Inform the requester of the outcome. */
  rc = gasnetc_ReplySystem(token, gasneti_handleridx(gasnetc_SysExitRole_reph),
			    NULL, 0, 1, (gasnet_handlerarg_t)result);
  gasneti_assert(rc == GASNET_OK);
}

/*
 * gasnetc_SysExitRole_reph()
 *
 * This reply handler receives the result of the election of an exit "master".
 * The reply contains the exit "role" this node should assume.
 */
static void 
gasnetc_SysExitRole_reph(gasnet_token_t token, void *nop, size_t nsz, 
			 gasnet_handlerarg_t role) 
{
  #if GASNET_DEBUG
  {
    gasnet_node_t src;
    int rc;

    rc = gasnet_AMGetMsgSource(token, &src);
    gasneti_assert(rc == GASNET_OK);
    /* May only receive this reply from the root node */
    gasneti_assert(src == GASNETC_ROOT_NODE);	
  }
  #endif

  gasneti_assert((role == GASNETC_EXIT_ROLE_MASTER) 
		 || (role == GASNETC_EXIT_ROLE_SLAVE));

  /* Set the role if not yet set.  Then assert that the assigned role has been
   * assumed.  This way the assertion is checking that if the role was obtained
   * by other means (namely by receiving an exit request) it must match the
   * election result. */
  gasneti_atomic_compare_and_swap(&gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, role, 0);
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_role, 0) == role);
}

/*
 * gasnetc_get_exit_role()
 *
 * This function returns the exit role immediately if known.  Otherwise it
 * sends an AMRequest to determine its role and then polls the network until
 * the exit role is determined, either by the reply to that request, or by a
 * remote exit request.
 *
 * Should be called with an alarm timer in-force in case we get hung sending or
 * the root node is not responsive.
 *
 * Note that if we get here as a result of a remote exit request then our role
 * has already been set to "slave" and we won't touch the network from inside
 * the request handler.
 *
 * XXX gasnetc_get_exit_role is called from a single thread.
 */
static int gasnetc_get_exit_role(void)
{
  int role;

  role = gasneti_atomic_read(&gasnetc_exit_role, 0);
  if (role == GASNETC_EXIT_ROLE_UNKNOWN) {
    int rc;

    /* Don't know our role yet.  So, send a system-category AM Request to determine our role */
    rc = gasnetc_RequestSystem(GASNETC_ROOT_NODE, 
		    	       gasneti_handleridx(gasnetc_SysExitRole_reqh), 
			       NULL, NULL, 0, 0);
    gasneti_assert(rc == GASNET_OK);

    /* Now spin until somebody tells us what our role is */
    do {
      gasneti_AMPoll();
      role = gasneti_atomic_read(&gasnetc_exit_role, 0);
    } while (role == GASNETC_EXIT_ROLE_UNKNOWN);
  }

  return role;
}

/* gasnetc_exit_head
 *
 * All exit paths pass through here as the first step.
 * This function ensures that gasnetc_exit_code is written only once
 * by the first call.
 * It also lets the handler for remote exit requests know if a local
 * request has already begun.
 *
 * returns non-zero on the first call only
 * returns zero on all subsequent calls
 */
static int gasnetc_exit_head(int exitcode) {
  static gasneti_atomic_t once = gasneti_atomic_init(1);
  int retval;

  retval = gasneti_atomic_decrement_and_test(&once, 0);

  if (retval) {
    /* Store the exit code for later use */
    gasneti_atomic_set(&gasnetc_exit_code, exitcode, GASNETI_ATOMIC_WMB_POST);
  }

  return retval;
}

/* gasnetc_exit_now
 *
 * First we set the atomic variable gasnetc_exit_done to allow the exit
 * of any threads which are spinning on it in gasnetc_exit().
 * Then this function tries hard to actually terminate the calling thread.
 * If for some unlikely reason the _exit() call returns, we abort().
 *
 * DOES NOT RETURN
 */
static void gasnetc_exit_now(int) GASNETI_NORETURN;
static void gasnetc_exit_now(int exitcode) {
  /* If anybody is still waiting, let them go */
  gasneti_atomic_set(&gasnetc_exit_done, 1, GASNETI_ATOMIC_WMB_POST);

  #if GASNET_DEBUG_VERBOSE
    fprintf(stderr,"gasnetc_exit(): node %i/%i calling killmyprocess...\n", 
      gasneti_mynode, gasneti_nodes); fflush(stderr);
  #endif
  gasneti_killmyprocess(exitcode);
  /* NOT REACHED */

  gasneti_reghandler(SIGABRT, SIG_DFL);
  gasneti_fatalerror("gasnetc_exit_now aborting...");
  /* NOT REACHED */
}

/* gasnetc_exit_tail
 *
 * This the final exit code for the cases of local or remote requested exits.
 * It is not used for the return-from-main case.  Nor is this code used if a fatal
 * signal (including SIGALRM on timeout) is encountered while trying to shutdown.
 *
 * Just a wrapper around gasnetc_exit_now() to actually terminate.
 *
 * DOES NOT RETURN
 */
static void gasnetc_exit_tail(void) GASNETI_NORETURN;
static void gasnetc_exit_tail(void) {
  gasnetc_exit_now((int)gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE));
  /* NOT REACHED */
}

/* gasnetc_exit_sighandler
 *
 * This signal handler is for a last-ditch exit when a signal arrives while
 * attempting the graceful exit.  That includes SIGALRM if we get wedged.
 *
 * Just a signal-handler wrapper for gasnetc_exit_now().
 *
 * DOES NOT RETURN
 */
static void gasnetc_exit_sighandler(int sig) {
  #if GASNET_DEBUG
  /* note - can't call trace macros here, or even sprintf */
  if (sig == SIGALRM) {
    static const char msg1[] = "gasnet_exit(): WARNING: timeout during exit... goodbye\n";
    write(STDERR_FILENO, msg1, sizeof(msg1) - 1);
  } else {
    static const char msg1[] = "gasnet_exit(): ERROR: signal ";
    static const char msg2[] = " received during exit... goodbye\n";
    char digit;

    write(STDERR_FILENO, msg1, sizeof(msg1) - 1);

    /* assume sig < 100 */
    if (sig > 9) {
      digit = '0' + ((sig / 10) % 10);
      write(STDERR_FILENO, &digit, 1);
    }
    digit = '0' + (sig % 10);
    write(STDERR_FILENO, &digit, 1);
    
    write(STDERR_FILENO, msg2, sizeof(msg2) - 1);
  }
  #endif

  #if defined(GASNETC_GM_MPI_COMPAT) && 0	/* DISABLED: could call Abort w/o Init */
  { /* We ask the bootstrap support to kill us, but only once */
    static gasneti_atomic_t once = gasneti_atomic_init(1);
    if (gasneti_atomic_decrement_and_test(&once, 0)) {
      gasneti_reghandler(SIGALRM, gasnetc_exit_sighandler);
      alarm(5);
      gasneti_bootstrapAbort_mpi((int)gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE));
    }
  }
  #endif

  gasnetc_exit_now((int)gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE));
  /* NOT REACHED */
}

/* gasnetc_exit_master
 *
 * We say a polite goodbye to our peers and then listen for their replies.
 * This forms the root nodes portion of a barrier for graceful shutdown.
 *
 * The "goodbyes" are just a system-category AM containing the desired exit code.
 * The AM helps ensure that on non-collective exits the "other" nodes know to exit.
 * If we see a "goodbye" from all of our peers we know we've managed to coordinate
 * an orderly shutdown.  If not, then in gasnetc_exit_body() we can ask the bootstrap
 * support to kill the job in a less graceful way.
 *
 * Takes the exitcode and a timeout in us as arguments
 *
 * Returns 0 on success, non-zero on any sort of failure including timeout.
 */
static int gasnetc_exit_master(int exitcode, int64_t timeout_us) {
  int i, rc;
  gasneti_tick_t start_time;

  gasneti_assert(timeout_us > 0); 

  start_time = gasneti_ticks_now();

  /* Notify phase */
  for (i = 0; i < gasneti_nodes; ++i) {
    if (i == gasneti_mynode) continue;

    if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;

    /* XXX */
    rc = gasnetc_RequestSystem(i, gasneti_handleridx(gasnetc_SysExit_reqh),
			       NULL, NULL, 0, 1, (gasnet_handlerarg_t)exitcode);
    if (rc != GASNET_OK) return -1;
  }

  /* Wait phase - wait for replies from our N-1 peers */
  while (gasneti_atomic_read(&gasnetc_exit_reps, 0) < (gasneti_nodes - 1)) {
    if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;

    gasneti_AMPoll();
  }

  return 0;
}

/* gasnetc_exit_slave
 *
 * We wait for a polite goodbye from the exit master.
 *
 * Takes a timeout in us as arguments
 *
 * Returns 0 on success, non-zero on timeout.
 */
static int gasnetc_exit_slave(int64_t timeout_us) {
  gasneti_tick_t start_time;

  gasneti_assert(timeout_us > 0); 

  start_time = gasneti_ticks_now();

  /* wait until the exit request is received from the master */
  while (gasneti_atomic_read(&gasnetc_exit_reqs, 0) == 0) {
    if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) return -1;

    gasneti_AMPoll(); /* works even before _attach */
  }

  /* XXX: Best if we could wait until our reply has been placed on the wire */

  return 0;
}

/* gasnetc_exit_body
 *
 * This code is common to all the exit paths and is used to perform a hopefully graceful exit in
 * all cases.  To coordinate a graceful shutdown gasnetc_get_exit_role() will select one node as
 * the "master".  That master node will then send a remote exit request to each of its peers to
 * ensure they know that it is time to exit.  If we fail to coordinate the shutdown, we ask the
 * bootstrap to shut us down agressively.  Otherwise we return to our caller.  Unless our caller
 * is the at-exit handler, we are typically followed by a call to gasnetc_exit_tail() to perform
 * the actual termination.  Note also that this function will block all calling threads other than
 * the first until the shutdown code has been completed.
 *
 * XXX: timouts contained here are entirely arbitrary
 */
static void gasnetc_exit_body(void) {
  int i, role, exitcode;
  int graceful = 0;
  int tok_drain = 0;
  int64_t timeout_us;

  /* once we start a shutdown, ignore all future SIGQUIT signals or we risk reentrancy */
  (void)gasneti_reghandler(SIGQUIT, SIG_IGN);

  /* Ensure only one thread ever continues past this point.
   * Others will spin here until time to die.
   * We can't/shouldn't use mutex code here since it is not signal-safe.
   */
  #ifdef GASNETI_USE_GENERIC_ATOMICOPS
    #error "We need real atomic ops with signal-safety for gasnet_exit..."
  #endif
  {
    static gasneti_atomic_t exit_lock = gasneti_atomic_init(1);
    if (!gasneti_atomic_decrement_and_test(&exit_lock, 0)) {
      /* poll until it is time to exit */
      while (!gasneti_atomic_read(&gasnetc_exit_done, GASNETI_ATOMIC_RMB_PRE)) {
        gasneti_sched_yield(); /* NOT safe to use sleep() here - conflicts with alarm() */
      }
      gasnetc_exit_tail();
      /* NOT REACHED */
    }
  }

  /* read exit code, stored by first caller to gasnetc_exit_head() */
  exitcode = gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);

  /* Establish a last-ditch signal handler in case of failure. */
  alarm(0);
  gasneti_reghandler(SIGALRM, gasnetc_exit_sighandler);
  #if GASNET_DEBUG
    gasneti_reghandler(SIGABRT, SIG_DFL);
  #else
    gasneti_reghandler(SIGABRT, gasnetc_exit_sighandler);
  #endif
  gasneti_reghandler(SIGILL,  gasnetc_exit_sighandler);
  gasneti_reghandler(SIGSEGV, gasnetc_exit_sighandler);
  gasneti_reghandler(SIGFPE,  gasnetc_exit_sighandler);
  gasneti_reghandler(SIGBUS,  gasnetc_exit_sighandler);

  /* Disable processing of AMs, except system-level ones */
  gasnetc_disable_AMs();

  GASNETI_TRACE_PRINTF(C,("gasnet_exit(%i)\n", exitcode));

  /* Try to flush out all the output, allowing upto 30s */
  alarm(30);
  {
    gasneti_flush_streams();
    gasneti_trace_finish();
    alarm(0);
    gasneti_sched_yield();
  }

  /* Deterimine our role (master or slave) in the coordination of this shutdown */
  alarm(10);
  role = gasnetc_get_exit_role();

  /* Attempt a coordinated shutdown */
  timeout_us = gasnetc_exittimeout * 1.0e6;
  alarm(1 + (int)gasnetc_exittimeout);
  switch (role) {
  case GASNETC_EXIT_ROLE_MASTER:
    /* send all the remote exit requests and wait for the replies */
    tok_drain = _gmc.stoks.total;
    graceful = (gasnetc_exit_master(exitcode, timeout_us) == 0);
    break;

  case GASNETC_EXIT_ROLE_SLAVE:
    /* wait for the exit request and reply before proceeding */
    tok_drain = _gmc.stoks.total;
    graceful = (gasnetc_exit_slave(timeout_us) == 0);
    /*
     * A sleep is insufficient to rely on our reply being out on the wire.  We
     * must actually verify that we have drained enough tokens before
     * deregistering pinned buffers.
     */
    alarm(0);
    break;

  default:
      gasneti_fatalerror("invalid exit role");
  }

  /* Clean up transport resources, allowing upto 30s */
  alarm(30);
  {
	#if defined(GASNET_SEGMENT_FAST)
	if (gasneti_attach_done && gasneti_seginfo[gasneti_mynode].size > 0 &&
            gm_deregister_memory(_gmc.port, 
	    (void *) gasneti_seginfo[gasneti_mynode].addr,
	    gasneti_seginfo[gasneti_mynode].size) != GM_SUCCESS)
		fprintf(stderr, 
		    "%d> Couldn't deregister prepinned segment",
		    gasneti_mynode);
	#endif	

	/*
	 * Make sure we drain the outgoing queue if we sent any messages before
	 * deregistering any of the prepinned AM buffers
	 */
	while (_gmc.stoks.total > tok_drain)
	    gasnet_AMPoll();

	/* Exclude other threads from calling gm. */
	#if !GASNET_SEQ
	{	const int64_t limit1 = 500000000; /* 0.5s - max time to spin for a lock */
		const int64_t limit2 = 50000; /* 50us - min time to pause for pollers to clear */
		gasneti_tick_t start_time = gasneti_ticks_now();

		/* Exclude AMPoll() from calling gm_receive_pending() */
		gasneti_atomic_set(&gasnetc_exit_running, 1, GASNETI_ATOMIC_WMB_POST);

		/* Try for a bounded time to obtain the gm lock */
		gasneti_waitwhile((gasneti_mutex_trylock(&gasnetc_lock_gm) != 0) &&
				  (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) < limit1));

		/* Ensure we've paused long enough for pollers to notice gasnetc_exit_running */
		gasneti_waitwhile(gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) < limit2);
	}
	#endif

	#if GASNET_SEQ /* NOT thread-safe */
	gasnetc_DestroyPinnedBufs();
	#endif

        gasneti_flush_streams();

	if (gasneti_init_done) {
  		gm_close(_gmc.port);
		if (gasneti_attach_done)
			firehose_fini();
	}
	gm_finalize();
  }

  /* Try again to flush out any recent output, allowing upto 5s */
  alarm(5);
  {
    gasneti_flush_streams();
    #if !GASNET_DEBUG_VERBOSE
      gasneti_close_streams();
    #endif
  }

  /* XXX potential problems here if exiting from the "Wrong" thread, or from a signal handler */
  alarm(10);
  {
    if (graceful) {
	#ifdef GASNETC_GM_MPI_COMPAT
	if (!(getenv("GASNET_GASNETRUN_GM")))
	  gasneti_bootstrapFini_mpi();
	#endif
    } else {
	#ifdef GASNETC_GM_MPI_COMPAT
	  gasneti_bootstrapAbort_mpi(exitcode);
	#else
	  /* We couldn't reach our peers, so hope the bootstrap code can kill the entire job */
	  gasneti_reghandler(SIGABRT, SIG_DFL);
          gasneti_fatalerror("gasnetc_exit_body aborting...");
	  /* NOT REACHED */
	#endif
    }
  }

  alarm(0);
}

/* gasnetc_exit_reqh
 *
 * This is a system-category AM handler and is therefore available as soon as gasnet_init()
 * returns, even before gasnet_attach().  This handler is responsible for receiving the
 * remote exit requests from the master node and replying.  We call gasnetc_exit_head()
 * with the exitcode seen in the remote exit request.  If this remote request is seen before
 * any local exit requests (normal or signal), then we are also responsible for starting the
 * exit procedure, via gasnetc_exit_{body,tail}().  Additionally, we are responsible for
 * firing off a SIGQUIT to let the user's handler, if any, run before we begin to exit.
 */
static void 
gasnetc_SysExit_reqh(gasnet_token_t token, void *nop, size_t nsz, 
		     gasnet_handlerarg_t exitcode) 
{
  int rc;

  /* The master will send this AM, but should _never_ receive it */
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_role, 0) != GASNETC_EXIT_ROLE_MASTER);

  /* We should never receive this AM multiple times */
  gasneti_assert(gasneti_atomic_read(&gasnetc_exit_reqs, 0) == 0);

  /* Count the exit requests, so gasnetc_exit_wait() knows when to return */
  gasneti_atomic_increment(&gasnetc_exit_reqs, 0);

  /* If we didn't already know, we are now certain our role is "slave" */
  (void)gasneti_atomic_compare_and_swap(&gasnetc_exit_role, 
					GASNETC_EXIT_ROLE_UNKNOWN, GASNETC_EXIT_ROLE_SLAVE, 0);

  /* Send a reply so the master knows we are reachable */
  rc = gasnetc_ReplySystem(token, gasneti_handleridx(gasnetc_SysExit_reph), 
			   NULL, 0, 0);
  gasneti_assert(rc == GASNET_OK);

  /* XXX: save the identity of the master here so we can later drain the send queue of the reply? */

  /* Initiate an exit IFF this is the first we've heard of it */
  if (gasnetc_exit_head(exitcode)) {
    gasneti_sighandlerfn_t handler;
    /* IMPORTANT NOTE
     * When we reach this point we are in a request handler which will never return.
     * Care should be taken to ensure this doesn't wedge the AM recv logic.
     *
     * This is currently safe because:
     * 1) request handlers are run w/ no locks held
     * 2) we always have an extra thread to recv AM requests
     */

    /* To try and be reasonably robust, want to avoid performing the shutdown
     * and exit from signal context if we can avoid it.  However, we must raise
     * SIGQUIT if the user has registered a handler.  Therefore we inspect what
     * is registered before calling raise().
     *
     * XXX we don't do this atomically w.r.t the signal
     * XXX we don't do the right thing w/ SIG_ERR and SIG_HOLD
     */
    handler = gasneti_reghandler(SIGQUIT, SIG_IGN);
    if ((handler != gasneti_defaultSignalHandler) &&
	#ifdef SIG_HOLD
	(handler != (gasneti_sighandlerfn_t)SIG_HOLD) &&
	#endif
	(handler != (gasneti_sighandlerfn_t)SIG_ERR) &&
	(handler != (gasneti_sighandlerfn_t)SIG_IGN) &&
	(handler != (gasneti_sighandlerfn_t)SIG_DFL)) {
      (void)gasneti_reghandler(SIGQUIT, handler);
      #if 1
        raise(SIGQUIT);
	/* Note: Both ISO C and POSIX assure us that raise() won't return until
	 * after the signal handler (if any) has executed.  However, if that
	 * handler calls gasnetc_exit(), we'll never return here. */
      #elif 0
	kill(getpid(),SIGQUIT);
      #else
	handler(SIGQUIT);
      #endif
    } else {
      /* No need to restore the handler, since _exit_body will set it to
       * SIG_IGN anyway. */
    }

    gasnetc_exit_body();
    gasnetc_exit_tail();
    /* NOT REACHED */
  }

  return;
}

/* gasnetc_SysExit_reph
 *
 * Simply count replies
 */
static void 
gasnetc_SysExit_reph(gasnet_token_t token, void *nop, size_t nsz) 
{
  gasneti_atomic_increment(&gasnetc_exit_reps, 0);
}
  
/* gasnetc_atexit OR gasnetc_on_exit
 *
 * This is a simple (at,on_}exit() handler to achieve a hopefully graceful exit.
 * We use the functions gasnetc_exit_{head,body}() to coordinate the shutdown.
 * Note that we don't call gasnetc_exit_tail() since we anticipate the normal
 * exit() procedures to shutdown the multi-threaded process nicely and also
 * because with atexit() we don't have access to the exit code!
 *
 * With atexit(), we don't have access to the exit code to send to the other
 * nodes in the event this is a non-collective exit.  However, experience with at
 * lease one MPI suggests that when using MPI for bootstrap a non-zero return from
 * at least one executable is sufficient to produce that non-zero exit code from
 * the parallel job.  Therefore, we can "safely" pass 0 to our peers and still
 * expect to preserve a non-zero exit code for the GASNet job as a whole.  Of course
 * there is no _guarantee_ this will work with all bootstraps.
 */
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int exitcode, void *arg) {
  /* Check return from _head to avoid reentrance */
  if (gasnetc_exit_head(exitcode)) {
    gasnetc_exit_body();
  }
  return;
}
#else
static void gasnetc_atexit(void) {
  /* Check return from _head to avoid reentrance */
  if (gasnetc_exit_head(0)) { /* real exit code is outside our control */
    gasnetc_exit_body();
  }
  return;
}
#endif

/* gasnetc_exit
 *
 * This is the start of a locally requested exit from GASNet.
 * The caller might be the user, some part of the conduit which has detected an error,
 * or possibly gasneti_defaultSignalHandler() responding to a termination signal.
 */
extern void gasnetc_exit(int exitcode) {
  gasnetc_exit_head(exitcode);
  gasnetc_exit_body();
  gasnetc_exit_tail();
  /* NOT REACHED */
}

/* ------------------------------------------------------------------------------------ */
/*
  Misc. Core handlers
*/
void
gasnetc_am_medcopy_inner(gasnet_token_t token, void *addr, size_t nbytes, 
			 void *dest)
{
	memcpy(dest, addr, nbytes);
}
MEDIUM_HANDLER(gasnetc_am_medcopy,1,2,
              (token,addr,nbytes, UNPACK(a0)    ),
              (token,addr,nbytes, UNPACK2(a0, a1)));
/* ------------------------------------------------------------------------------------ */
/*
  Misc. Active Message Functions
  ==============================
*/
extern int gasnetc_AMGetMsgSource(gasnet_token_t token, gasnet_node_t *srcindex) {
  gasnet_node_t sourceid;

  GASNETI_CHECK_ERRR((!token),BAD_ARG,"bad token");
  GASNETI_CHECK_ERRR((!srcindex),BAD_ARG,"bad src ptr");

  if ((void *)token == (void *)-1) {
    /* Need this even in presence of GASNET_PSHM, due to System category AMs */
    sourceid = gasneti_mynode;
  }
#if GASNET_PSHM
  else if (gasneti_AMPSHMGetMsgSource(token, &sourceid) == GASNET_OK) {
    /* Empty */
  }
#endif
  else {
    gasnetc_bufdesc_t *bufd = (gasnetc_bufdesc_t *) token;
    if_pf (!bufd->gm_id) GASNETI_RETURN_ERRR(BAD_ARG, "No GM receive event");
    sourceid = bufd->node;
  }

  gasneti_assert(sourceid < gasneti_nodes);
  *srcindex = sourceid;
  return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */
/*
  Active Message Request Functions
  ================================
*/

extern int 
gasnetc_AMRequestShortM(gasnet_node_t dest, gasnet_handler_t handler, 
                            int numargs, ...) 
{
	va_list argptr;
	gasnetc_bufdesc_t *bufd;
	int len;
        GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs);
	va_start(argptr, numargs);

#if GASNET_PSHM
	if_pt (gasneti_pshm_in_supernode(dest)) { /* Includes loopback */
		int retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Short, dest, handler,
							   0, 0, 0,
							   numargs, argptr);
		va_end(argptr);
		return retval;
	}
#else
	if (dest == gasneti_mynode) { /* local handler */
		int argbuf[GASNETC_AM_MAX_ARGS];
                gasnet_token_t token = (void *) -1;
		GASNETC_ARGS_WRITE(argbuf, argptr, numargs);
		GASNETI_RUN_HANDLER_SHORT(1, handler, _gmc.handlers[handler], token, 
					  argbuf, numargs);
	}
#endif
	else {
		bufd = gasnetc_AMRequestPool_block();
		len = gasnetc_write_AMBufferShort(bufd->buf, handler, numargs, 
		    		argptr, GASNETC_AM_REQUEST);
		gasnetc_GMSend_AMRequest(bufd->buf, len, _gmc.gm_nodes[dest].id,
					 _gmc.gm_nodes[dest].port, 
					 gasnetc_callback_lo, (void *)bufd, 0);
	}

	va_end(argptr);
	return GASNET_OK;
}

extern int gasnetc_AMRequestMediumM( 
                            gasnet_node_t dest,      /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  va_list argptr;
  gasnetc_bufdesc_t *bufd;
  int len;
  GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);
  va_start(argptr, numargs); /*  pass in last argument */

  gasneti_assert(nbytes <= GASNETC_AM_MEDIUM_MAX);
#if GASNET_PSHM
  if_pt (gasneti_pshm_in_supernode(dest)) { /* Includes loopback */
    int retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Medium, dest, handler,
                                               source_addr, nbytes, 0,
                                               numargs, argptr);
    va_end(argptr);
    return retval;
  }
#else
  if (dest == gasneti_mynode) { /* local handler */
    void *loopbuf;
    int argbuf[GASNETC_AM_MAX_ARGS];
    gasnet_token_t token = (void *) -1;
    loopbuf = gasnetc_alloca(nbytes+GASNETI_MEDBUF_ALIGNMENT);
    loopbuf = (void *)GASNETI_ALIGNUP(loopbuf, GASNETI_MEDBUF_ALIGNMENT);
    memcpy(loopbuf, source_addr, nbytes);
    GASNETC_ARGS_WRITE(argbuf, argptr, numargs);
    GASNETI_RUN_HANDLER_MEDIUM(1, handler, _gmc.handlers[handler], token,
				argbuf, numargs, loopbuf, nbytes);
  }
#endif
  else {
    bufd = gasnetc_AMRequestPool_block();
    len = gasnetc_write_AMBufferMedium(bufd->buf, handler, numargs, argptr, 
		 nbytes, source_addr, GASNETC_AM_REQUEST);
    gasnetc_GMSend_AMRequest(bufd->buf, len, 
		  gasnetc_nodeid(dest), gasnetc_portid(dest),
		  gasnetc_callback_lo, (void *)bufd, 0);
  }

  va_end(argptr);
  return GASNET_OK;
}

/* 
 * DMA_inner allows to DMA an AMLong when the local buffer isn't pinned and the
 * remote buffer is (or when single-copy is needed for non-async semantic).
 */
GASNETI_INLINE(gasnetc_AMRequestLongM_DMA_inner)
void
gasnetc_AMRequestLongM_DMA_inner(gasnet_node_t node, gasnet_handler_t handler,
		void *source_addr, size_t nbytes, const firehose_request_t *req,
		uintptr_t dest_addr, int numargs, va_list argptr)
{
	int	bytes_left = nbytes;
	int	port, id, len;
	uint8_t	*psrc, *pdest;
	gasnetc_bufdesc_t	*bufd;

	gasneti_assert(nbytes > 0);
#if defined(GASNET_SEGMENT_FAST)
	gasneti_assert(req == NULL);
#else
	gasneti_assert(req != NULL);
#endif

	psrc  = (uint8_t *) source_addr;
	pdest = (uint8_t *) dest_addr;
	port  = gasnetc_portid(node);
	id    = gasnetc_nodeid(node);

	/* Until the remaining buffer size fits in a Long Buffer, get AM
	 * buffers and DMA out of them.  This assumes the remote destination is
	 * pinned and the local is not */
	while (bytes_left >GASNETC_AM_LEN-GASNETC_LONG_OFFSET) {
		int tmp = MIN(GASNETC_AM_LEN, bytes_left);
		bufd = gasnetc_AMRequestPool_block();
		gasnetc_write_AMBufferBulk(bufd->buf, 
			psrc, tmp);
		gasnetc_GMSend_AMRequest(bufd->buf, 
		   tmp, id, port, gasnetc_callback_lo,
		   (void *) bufd, (uintptr_t) pdest);
		psrc += tmp;
		pdest += tmp;
		bytes_left -= tmp;
	}
	/* Write the header for the AM long buffer */
	bufd = gasnetc_AMRequestPool_block();
	bufd->node = node;
	len =
	    gasnetc_write_AMBufferLong(bufd->buf, 
	        handler, numargs, argptr, nbytes, source_addr, 
		(uintptr_t) dest_addr, GASNETC_AM_REQUEST);

	/* If bytes are left, write them in the remainder of the AM buffer */
	if (bytes_left > gasnetc_packed_long_limit) {
		/* single-copy, using separate gm-level messages for payload and header */
		gasnetc_write_AMBufferBulk(
			(uint8_t *)bufd->buf+GASNETC_LONG_OFFSET, 
			psrc, (size_t) bytes_left);
		gasnetc_GMSend_AMRequest(
		    (uint8_t *)bufd->buf+GASNETC_LONG_OFFSET,
		    bytes_left, id, port, gasnetc_callback_lo, NULL,
		    (uintptr_t) pdest);
	}
	else if (bytes_left) {
		/* two-copy, but with only a single gm-level message for both payload and header */
		gasnetc_write_AMBufferBulk(
			(uint8_t *)bufd->buf+len,
			psrc, (size_t) bytes_left);
		len += bytes_left;
	}

	/* Set the firehose request type in the last bufd, so it may be
	 * released (if non-NULL) once the last AMRequest receives its reply */
	bufd->remote_req = req;
#if defined(GASNET_SEGMENT_FAST)
	gasnetc_GMSend_AMRequest(bufd->buf, len, id, 
	    port, gasnetc_callback_lo, (void *)bufd, 0);
#else
	gasnetc_GMSend_AMRequest(bufd->buf, len, id, 
	    port, gasnetc_callback_lo_rdma, (void *)bufd, 0);
#endif
}

/* When the local and remote regions are not pinned, AM buffers are used and
 * Mediums are sent for the entire payload.  Once the payloads are sent, an
 * AMLong header is sent (with no payload)
 */
GASNETI_INLINE(gasnetc_AMRequestLongM_inner)
void
gasnetc_AMRequestLongM_inner(gasnet_node_t node, gasnet_handler_t handler,
		void *source_addr, size_t nbytes, void *dest_addr, int numargs, 
		va_list argptr)
{

	int	bytes_left = nbytes;
	int	port, id, len, long_len;
	uint8_t	*psrc, *pdest;
	gasnetc_bufdesc_t	*bufd;

	psrc  = (uint8_t *) source_addr;
	pdest = (uint8_t *) dest_addr;
	port  = gasnetc_portid(node);
	id    = gasnetc_nodeid(node);

	/* If the length is greater than what we can fit in an AMLong buffer,
	 * send AM Mediums until that threshold is reached */
	while (bytes_left >GASNETC_AM_LEN-GASNETC_LONG_OFFSET) {
		int tmp = MIN(GASNETC_AM_MEDIUM_MAX, bytes_left);
		bufd = gasnetc_AMRequestPool_block();
		len = gasnetc_write_AMBufferMediumMedcopy(bufd->buf,
			(void *) psrc, tmp, (void *) pdest, 
			GASNETC_AM_REQUEST);
		gasnetc_GMSend_AMRequest(bufd->buf, len, id, 
		    port, gasnetc_callback_lo, (void *) bufd, 0);
		psrc += tmp;
		pdest += tmp;
		bytes_left -= tmp;
	}
	bufd = gasnetc_AMRequestPool_block();
	long_len =
	    gasnetc_write_AMBufferLong(bufd->buf, 
	        handler, numargs, argptr, nbytes, source_addr, 
		(uintptr_t) dest_addr, GASNETC_AM_REQUEST);

	/* Since we must 2-copy anyway, always send remainder as a "packed" long */
	if (bytes_left) {
		gasnetc_write_AMBufferBulk(
			(uint8_t *)bufd->buf+long_len, 
			psrc, (size_t) bytes_left);
		long_len += bytes_left;
	}
	gasnetc_GMSend_AMRequest(bufd->buf, long_len, id, port, 
	    gasnetc_callback_lo, (void *)bufd, 0);
	return;
}

extern int gasnetc_AMRequestLongM( gasnet_node_t dest,        /* destination node */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            void *dest_addr,                    /* data destination on destination node */
                            int numargs, ...)
{
	va_list	argptr;
        GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);
	va_start(argptr, numargs); /*  pass in last argument */

	gasneti_assert(nbytes <= GASNETC_AM_LONG_REQUEST_MAX);

#if GASNET_PSHM
	if_pt (gasneti_pshm_in_supernode(dest)) { /* Includes loopback */
		int retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
							   source_addr, nbytes, dest_addr,
							   numargs, argptr);
		va_end(argptr);
		return retval;
	}
#else
	if (dest == gasneti_mynode) {
		int	argbuf[GASNETC_AM_MAX_ARGS];
                gasnet_token_t token = (void *) -1;

		GASNETC_ARGS_WRITE(argbuf, argptr, numargs);
		if_pt(dest_addr != source_addr) GASNETC_AMPAYLOAD_WRITE(dest_addr, source_addr, nbytes);
		GASNETI_RUN_HANDLER_LONG(1, handler, _gmc.handlers[handler], token, 
		    argbuf, numargs, dest_addr, nbytes);
	}
#endif
	else {
		/* XXX gasneti_assert(GASNET_LONG_OFFSET >= LONG_HEADER) */
		if_pt (nbytes > 0) { /* Handle zero-length messages */
			const firehose_request_t	*req;
			
		    #if defined(GASNET_SEGMENT_FAST)
			/* Remote is always pinned */
	                GASNETI_TRACE_EVENT_VAL(C, AMREQUESTLONG_ONECOPY, nbytes);
			gasnetc_AMRequestLongM_DMA_inner(dest, handler, 
			    source_addr, nbytes, NULL, 
			    (uintptr_t) dest_addr, numargs, argptr);
		    #else
			req = firehose_try_remote_pin(dest, 
				(uintptr_t) dest_addr, nbytes, 0, NULL);

                        if (req != NULL) {
	                        GASNETI_TRACE_EVENT_VAL(C, AMREQUESTLONG_ONECOPY, nbytes);
				gasnetc_AMRequestLongM_DMA_inner(dest, handler, 
				    source_addr, nbytes, req, 
				    (uintptr_t) dest_addr, numargs, argptr);
                        } else {
	                        GASNETI_TRACE_EVENT_VAL(C, AMREQUESTLONG_TWOCOPY, nbytes);
				gasnetc_AMRequestLongM_inner(dest, handler, 
				    source_addr, nbytes, dest_addr, numargs, 
				    argptr);
                        }
		    #endif
		}
		else {
			gasnetc_AMRequestLongM_inner(dest, handler, source_addr, 
			    nbytes, dest_addr, numargs, argptr);
		}
	}

	va_end(argptr);
	return GASNET_OK;
}

extern int 
gasnetc_AMRequestLongAsyncM( 
	gasnet_node_t dest,        /* destination node */
	gasnet_handler_t handler,  /* index to handler */
	void *source_addr, size_t nbytes,   /* data payload */
	void *dest_addr,           /* data destination on destination node */
	int numargs, ...)
{
	va_list	argptr;

	const firehose_request_t	*reql, *reqr = NULL;

	gasnetc_bufdesc_t	*bufd;
        GASNETI_COMMON_AMREQUESTLONGASYNC(dest,handler,source_addr,nbytes,dest_addr,numargs);
	va_start(argptr, numargs); /*  pass in last argument */

#if GASNET_PSHM
	if_pt (gasneti_pshm_in_supernode(dest)) { /* Includes loopback */
		int retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
							   source_addr, nbytes, dest_addr,
							   numargs, argptr);
		va_end(argptr);
		return retval;
	}
#else
	if (dest == gasneti_mynode) {
		int	argbuf[GASNETC_AM_MAX_ARGS];
                gasnet_token_t token = (void *) -1;
		GASNETC_ARGS_WRITE(argbuf, argptr, numargs);
		if_pt(dest_addr != source_addr) GASNETC_AMPAYLOAD_WRITE(dest_addr, source_addr, nbytes);
		GASNETI_RUN_HANDLER_LONG(1, handler, _gmc.handlers[handler], token, 
		    argbuf, numargs, dest_addr, nbytes);

		va_end(argptr);
		return GASNET_OK;
	}
#endif

	/* If length is below the packed long limit, or the remote local is not pinned,
	 * then we send using packed longs, or AMMedium payloads, respectively */
	if (nbytes <= gasnetc_packed_long_limit 
#if !defined(GASNET_SEGMENT_FAST)
	    || !(reqr = firehose_try_remote_pin(dest, (uintptr_t) dest_addr, nbytes, 0, NULL))
#endif
	    ) {
	        GASNETI_TRACE_EVENT_VAL(C, AMREQUESTLONGASYNC_TWOCOPY, nbytes);
		gasnetc_AMRequestLongM_inner(dest, handler, source_addr, 
		    nbytes, dest_addr, numargs, argptr);
        }
	/* If we couldn't pin locally for free, use DMA method where we use AM
	 * buffers and copy+RDMA payloads out of them */
	else if (!(reql = 
	     firehose_try_local_pin((uintptr_t) source_addr, nbytes, NULL))) {
	        GASNETI_TRACE_EVENT_VAL(C, AMREQUESTLONGASYNC_ONECOPY, nbytes);
		gasnetc_AMRequestLongM_DMA_inner(dest, handler, source_addr, 
		    nbytes, reqr, (uintptr_t) dest_addr, numargs, argptr);
        }

	/* If both local and remote locations are pinned, use RDMA and send a
	 * header-only AMLong */
	else {
		uint16_t port, id;
		int	 len;

		gasneti_assert(reql != NULL);
#if defined(GASNET_SEGMENT_FAST)
		gasneti_assert(reqr == NULL);
#else
		gasneti_assert(reqr != NULL);
#endif

	        GASNETI_TRACE_EVENT_VAL(C, AMREQUESTLONGASYNC_ZEROCOPY, nbytes);
		port = gasnetc_portid(dest);
		id   = gasnetc_nodeid(dest);
		bufd = gasnetc_AMRequestPool_block();
		len =
		    gasnetc_write_AMBufferLong(bufd->buf, 
		        handler, numargs, argptr, nbytes, source_addr, 
			(uintptr_t) dest_addr, GASNETC_AM_REQUEST);

		bufd->node = dest;
		bufd->local_req = reql;
		bufd->remote_req = reqr;

		/* send the DMA first */
		gasnetc_GMSend_AMRequest(source_addr, nbytes, id, 
		    port, gasnetc_callback_lo, NULL, 
		    (uintptr_t) dest_addr);

		/* followed by the Long Header which releases firehose */
		gasnetc_GMSend_AMRequest(bufd->buf, len, id, 
		    port, gasnetc_callback_lo_rdma, (void *)bufd, 0);
	}

	va_end(argptr);
	return GASNET_OK;
}

/*
 * System-Level AMs, not user-visible
 */
extern int gasnetc_RequestSystem( 
			    gasnet_node_t dest, /* destination ndoe */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
			    int *done, /* integer to increment when done */
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) 
{
  va_list argptr;
  gasnetc_bufdesc_t *bufd;
  int len;
  gasneti_assert(numargs >= 0 && numargs <= gasnet_AMMaxArgs());

  GASNETI_CHECK_ERRR((nbytes > gasnet_AMMaxMedium()),BAD_ARG,"nbytes too large");
  va_start(argptr, numargs); /*  pass in last argument */
  gasneti_assert(nbytes <= GASNETC_AM_MEDIUM_MAX);

  if (dest == gasneti_mynode) { /* local handler */
    void *loopbuf;
    int argbuf[GASNETC_AM_MAX_ARGS];
    gasnet_token_t token = (void *) -1;
    loopbuf = gasnetc_alloca(nbytes+GASNETI_MEDBUF_ALIGNMENT);
    loopbuf = (void *)GASNETI_ALIGNUP(loopbuf, GASNETI_MEDBUF_ALIGNMENT);
    memcpy(loopbuf, source_addr, nbytes);
    GASNETC_ARGS_WRITE(argbuf, argptr, numargs);
    GASNETI_RUN_HANDLER_MEDIUM(1, handler, _gmc.syshandlers[handler], token,
				argbuf, numargs, loopbuf, nbytes);
    if (done != NULL) {
	int cur = *done;
	*done = cur + 1;
    }
  }
#if GASNET_PSHM
  /* Still need the loopback code above for the !gasneti_init_done case */
  else if_pt (gasneti_init_done && gasneti_pshm_in_supernode(dest)) { /* Includes loopback */
    int retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Medium, dest,
                                               handler | GASNETC_SYS_HANDLER_FLAG,
                                               source_addr, nbytes, 0,
                                               numargs, argptr);
    if (done != NULL) {
	int cur = *done;
	*done = cur + 1;
    }
    va_end(argptr);
    return retval;
  }
#endif
  else {
    bufd = gasnetc_AMRequestPool_block();
    bufd->done = done;
    len = gasnetc_write_AMBufferSystem(bufd->buf, handler, numargs, argptr, 
		 nbytes, source_addr, GASNETC_AM_REQUEST);

    gasneti_mutex_lock(&gasnetc_lock_gm);
    gasnetc_GMSend_AMSystem(bufd->buf, len, gasnetc_nodeid(dest), 
			    gasnetc_portid(dest), bufd);
    gasneti_mutex_unlock(&gasnetc_lock_gm);
  }

  va_end(argptr);
  return GASNET_OK;
}

/* -------------------------------------------------------------------------- */
/* Replies */
/* -------------------------------------------------------------------------- */
void
gasnetc_GMSend_bufd(gasnetc_bufdesc_t *bufd)
{
	uintptr_t			send_ptr;
	uint32_t			len;

	gasneti_mutex_assertlocked(&gasnetc_lock_gm);
	gasneti_assert(bufd != NULL);
	gasneti_assert(bufd->buf != NULL);
	gasneti_assert(bufd->gm_id > 0);

	gasneti_assert(BUFD_ISSET(bufd, BUFD_REPLY));

	if (BUFD_ISSET(bufd, BUFD_PAYLOAD)) {
		gasneti_assert(bufd->dest_addr > 0);
		gasneti_assert(bufd->payload_len > 0);

		if (bufd->source_addr > 0)
			send_ptr = bufd->source_addr;
		else
			send_ptr = (uintptr_t) 
				   bufd->buf + GASNETC_LONG_OFFSET;

		GASNETI_TRACE_PRINTF(C, ("gm_put (%d,%p <- %p,%d bytes)",
		    bufd->node, (void *) bufd->dest_addr, (void *) send_ptr,
		    bufd->payload_len));

		gm_directed_send_with_callback(_gmc.port, 
		    (void *) send_ptr,
		    (gm_remote_ptr_t) bufd->dest_addr,
		    bufd->payload_len,
		    GM_HIGH_PRIORITY,
		    (uint32_t) bufd->gm_id,
		    (uint32_t) bufd->gm_port,
		    gasnetc_callback_hi_rdma,
		    (void *) bufd);
	}
	else {
		len = bufd->len;
		send_ptr = (uintptr_t) bufd->buf;

		GASNETI_TRACE_PRINTF(C, ("gm_send (gm id %d <- %p,%d bytes) %s",
		    (unsigned) bufd->gm_id, (void *) send_ptr, len,
		    GASNETC_AM_IS_REPLY(*((uint8_t *) send_ptr)) ? "AMReply "
		    : "AMSystemReply"));

		gasneti_assert(len > 0 && len <= GASNETC_AM_PACKET);

		if (_gmc.my_port == bufd->gm_port)
		gm_send_to_peer_with_callback(_gmc.port, 
			(void *) send_ptr,
			GASNETC_AM_SIZE,
			len,
			GM_HIGH_PRIORITY,
			(uint32_t) bufd->gm_id,
			gasnetc_callback_hi,
			(void *) bufd);
		else
		gm_send_with_callback(_gmc.port, 
			(void *) send_ptr,
			GASNETC_AM_SIZE,
			len,
			GM_HIGH_PRIORITY,
			(uint32_t) bufd->gm_id,
			(uint32_t) bufd->gm_port,
			gasnetc_callback_hi,
			(void *) bufd);
	}
	return;
}

int
gasnetc_AMReplyLongTrySend(gasnetc_bufdesc_t *bufd)
{
	int	sends = 0;

	gasneti_mutex_lock(&gasnetc_lock_gm);

	gasneti_assert(BUFD_ISSET(bufd, BUFD_REPLY));

	if (gasnetc_token_hi_acquire()) {
		/* First send : the payload if present, header otherwise */
		gasnetc_GMSend_bufd(bufd);
		sends++;

		if_pt (BUFD_ISSET(bufd, BUFD_PAYLOAD)) { 
			/* payload was sent, now worry about the header */
			BUFD_UNSET(bufd, BUFD_PAYLOAD);

			if (gasnetc_token_hi_acquire()) {
				gasnetc_GMSend_bufd(bufd);
				sends++;
			}
			/* If we can't get the second token, unset only
			 * the payload bit and enqueue the header send
			 */
			else {
				gasnetc_fifo_insert(bufd);
			}
		}
	}

	/* We couldn't get a send token, enqueue the whole bufd and
	 * leave the flag bits as is */
	else {
		gasnetc_fifo_insert(bufd);
	}

	gasneti_mutex_unlock(&gasnetc_lock_gm);

	return sends;
}

extern int 
gasnetc_AMReplyShortM(gasnet_token_t token, gasnet_handler_t handler,
                            int numargs, ...) 
{
	va_list argptr;
	gasnetc_bufdesc_t *bufd;
        GASNETI_COMMON_AMREPLYSHORT(token,handler,numargs);
	va_start(argptr, numargs); /*  pass in last argument */

#if GASNET_PSHM
	if_pt (gasnetc_token_is_pshm(token)) { /* Includes loopback */
		int retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Short, token, handler,
							 0, 0, 0,
							 numargs, argptr);
		va_end(argptr);
		return retval;
	}
#else
	if ((void *)token == (void*)-1) { /* local handler */
		int argbuf[GASNETC_AM_MAX_ARGS];
		GASNETC_ARGS_WRITE(argbuf, argptr, numargs);
		GASNETI_RUN_HANDLER_SHORT(0, handler, _gmc.handlers[handler], 
				          token, argbuf, numargs);
	}
#endif
	else {
		bufd = gasnetc_bufdesc_from_token(token);
		bufd->len = 
		    gasnetc_write_AMBufferShort(bufd->buf, handler, 
		    		numargs, argptr, GASNETC_AM_REPLY);
  
		gasneti_mutex_lock(&gasnetc_lock_gm);

		if (gasnetc_token_hi_acquire())
			gasnetc_GMSend_bufd(bufd);
		else
			gasnetc_fifo_insert(bufd);

		gasneti_mutex_unlock(&gasnetc_lock_gm);
	}

	va_end(argptr);
	return GASNET_OK;
}

extern int gasnetc_AMReplyMediumM( 
                            gasnet_token_t token,       /* token provided on handler entry */
                            gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
                            void *source_addr, size_t nbytes,   /* data payload */
                            int numargs, ...) {
  va_list argptr;
  gasnetc_bufdesc_t *bufd;
  GASNETI_COMMON_AMREPLYMEDIUM(token,handler,source_addr,nbytes,numargs);
  va_start(argptr, numargs); /*  pass in last argument */

  gasneti_assert(nbytes <= GASNETC_AM_MEDIUM_MAX);
#if GASNET_PSHM
  if_pt (gasnetc_token_is_pshm(token)) { /* Includes loopback */
    int retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Medium, token, handler,
                                             source_addr, nbytes, 0,
                                             numargs, argptr);
    va_end(argptr);
    return retval;
  }
#else
  if ((void *)token == (void *)-1) { /* local handler */
    int argbuf[GASNETC_AM_MAX_ARGS];
    void *loopbuf;
    loopbuf = gasnetc_alloca(nbytes+GASNETI_MEDBUF_ALIGNMENT);
    loopbuf = (void *)GASNETI_ALIGNUP(loopbuf, GASNETI_MEDBUF_ALIGNMENT);
    memcpy(loopbuf, source_addr, nbytes);
    GASNETC_ARGS_WRITE(argbuf, argptr, numargs);
    GASNETI_RUN_HANDLER_MEDIUM(0, handler, _gmc.handlers[handler], token,
				argbuf, numargs, loopbuf, nbytes);
  }
#endif
  else {
    bufd = gasnetc_bufdesc_from_token(token);
    bufd->len = 
	    gasnetc_write_AMBufferMedium(bufd->buf, handler, numargs, 
                    argptr, nbytes, source_addr, GASNETC_AM_REPLY);
    gasneti_mutex_lock(&gasnetc_lock_gm);
    if (gasnetc_token_hi_acquire()) {
       gasnetc_GMSend_bufd(bufd); 
    } else {
	gasneti_assert(bufd->gm_id > 0);
       gasnetc_fifo_insert(bufd);
    }
    gasneti_mutex_unlock(&gasnetc_lock_gm);
  }
  
  va_end(argptr);
  return GASNET_OK;
}

extern int gasnetc_AMReplyLongM( 
		gasnet_token_t token,       /* token provided on handler entry */
		gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
		void *source_addr, size_t nbytes,   /* data payload */
		void *dest_addr,                    /* data destination on destination node */
		int numargs, ...)
{
	va_list	argptr;
	gasnet_node_t		dest;
	gasnetc_bufdesc_t 	*bufd;

        GASNETI_COMMON_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,numargs); 
        GASNETI_SAFE_PROPAGATE(gasnet_AMGetMsgSource(token, &dest));
	va_start(argptr, numargs); /*  pass in last argument */
	gasneti_assert(nbytes <= GASNETC_AM_LONG_REPLY_MAX);
#if GASNET_PSHM
	if_pt (gasnetc_token_is_pshm(token)) { /* Includes loopback */
		int retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Long, token, handler,
							 source_addr, nbytes, dest_addr,
							 numargs, argptr);
		va_end(argptr);
		return retval;
	}
#else
	if ((void *)token == (void *)-1) {
		int	argbuf[GASNETC_AM_MAX_ARGS];
		GASNETC_ARGS_WRITE(argbuf, argptr, numargs);
		if_pt(dest_addr != source_addr) GASNETC_AMPAYLOAD_WRITE(dest_addr, source_addr, nbytes);
		GASNETI_RUN_HANDLER_LONG(0, handler, _gmc.handlers[handler], token, 
		    argbuf, numargs, dest_addr, nbytes);
	}
#endif
	else {
		uintptr_t	pbuf;
		unsigned int	len;

		const firehose_request_t	*req = NULL;
	
    		bufd            = gasnetc_bufdesc_from_token(token);
		bufd->dest_addr = (uintptr_t) dest_addr;
		bufd->node      = dest;

		if (nbytes > gasnetc_packed_long_limit
#if !defined(GASNET_SEGMENT_FAST)
		   && (req = firehose_try_remote_pin(dest, (uintptr_t) dest_addr, 
	    	            nbytes, 0,  NULL)) != NULL
#endif
		   ) {

			pbuf = (uintptr_t) bufd->buf + 
			    (uintptr_t) GASNETC_LONG_OFFSET;
			len =
			    gasnetc_write_AMBufferLong(bufd->buf, handler,
			        numargs, argptr, nbytes, source_addr, 
				(uintptr_t) dest_addr, GASNETC_AM_REPLY);
			gasnetc_write_AMBufferBulk((void *)pbuf, source_addr, 
			    nbytes);

			bufd->len = len;
			bufd->remote_req = req;
			bufd->local_req = NULL;
			bufd->source_addr = 0;

			bufd->payload_len = nbytes;

    			BUFD_SET(bufd, BUFD_PAYLOAD);
	                GASNETI_TRACE_EVENT_VAL(C, AMREPLYLONG_ONECOPY, nbytes);
		}
		else {
			size_t	header_len;
	
			/* The AMLong Reply doesn't use DMA */
			bufd->remote_req = NULL;
			bufd->local_req = NULL;

			header_len = 
			    gasnetc_write_AMBufferLong(bufd->buf, 
			        handler, numargs, argptr, nbytes, source_addr, 
				(uintptr_t) dest_addr, GASNETC_AM_REPLY);

			if_pt (nbytes > 0) { /* Handle zero-length messages */
				pbuf = (uintptr_t)bufd->buf + (uintptr_t) header_len;
				gasnetc_write_AMBufferBulk((void *)pbuf,
				    (void *)source_addr, nbytes);
	                        GASNETI_TRACE_EVENT_VAL(C, AMREPLYLONG_TWOCOPY, nbytes);
			}

			bufd->len = header_len + nbytes;
		}

		#if !GASNET_TRACE
		(void) gasnetc_AMReplyLongTrySend(bufd);
		#else
		{
			int payload = BUFD_ISSET(bufd, BUFD_PAYLOAD);
			int sends = gasnetc_AMReplyLongTrySend(bufd);
	
			if (sends == 2)
				GASNETC_AMTRACE_ReplyLong(Send);
			else if (sends == 1) {
				if (payload)
					GASNETC_AMTRACE_ReplyLong(Queued);
				else
					GASNETC_AMTRACE_ReplyLong(Send);
			}
			else
				GASNETC_AMTRACE_ReplyLong(Queued);
		}
		#endif
	}
	va_end(argptr);
	return GASNET_OK;
}

/*
 * This is not officially part of the gasnet spec therefore not exported for the
 * user.  The extended API uses it in order to do directed sends followed by an
 * AMReply.  Therefore, there is no boundscheck of any sort.  The extended API
 * knows how/when to call this function and makes sure the source and
 * destination regions are pinned.
 */
int 
gasnetc_AMReplyLongAsyncM( 
		gasnet_token_t token,       /* token provided on handler entry */
		gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
		void *source_addr, size_t nbytes,   /* data payload */
		void *dest_addr,                    /* data destination on destination node */
		int numargs, ...)
{
	va_list	argptr;
	unsigned int		len;
	gasnet_node_t		dest;
	gasnetc_bufdesc_t 	*bufd;

        GASNETI_SAFE_PROPAGATE(gasnet_AMGetMsgSource(token, &dest));
	GASNETI_CHECK_ERRR((dest >= gasneti_nodes),BAD_ARG,"node index too high");
        gasneti_assert(numargs >= 0 && numargs <= gasnet_AMMaxArgs());
	va_start(argptr, numargs); /*  pass in last argument */
	GASNETI_TRACE_AMREPLYLONG(token,handler,source_addr,nbytes,dest_addr,
	    numargs);

#if GASNET_PSHM
	if_pt (gasnetc_token_is_pshm(token)) { /* Includes loopback */
	    int retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Long, token, handler,
						     source_addr, nbytes, dest_addr,
						     numargs, argptr);
	    va_end(argptr);
	    return GASNET_OK;
	}
#endif

	bufd = gasnetc_bufdesc_from_token(token);
	len =
	    gasnetc_write_AMBufferLong(bufd->buf, handler, numargs, argptr, 
	        nbytes, source_addr, (uintptr_t) dest_addr, GASNETC_AM_REPLY);

	bufd->len = len;
	bufd->node = dest;
	bufd->payload_len = nbytes;
	bufd->dest_addr = (uintptr_t) dest_addr;
	bufd->source_addr = (uintptr_t) source_addr;
	bufd->local_req = NULL;
	bufd->remote_req = NULL;

	gasneti_assert(nbytes > 0);

	if_pt (nbytes > 0) {
		/* Also manage loopback by simply copying to the local
		 * destination */
		if_pf (dest == gasneti_mynode) {
			GASNETE_FAST_ALIGNED_MEMCPY(
			    dest_addr, source_addr, nbytes);
		}
		else {
			BUFD_SET(bufd, BUFD_PAYLOAD);
	                GASNETI_TRACE_EVENT_VAL(C, AMREPLYLONGASYNC_ZEROCOPY, nbytes);
		}
	}

	#if !GASNET_TRACE
	(void) gasnetc_AMReplyLongTrySend(bufd);
	#else
	{
		int payload = BUFD_ISSET(bufd, BUFD_PAYLOAD);
		int sends = gasnetc_AMReplyLongTrySend(bufd);

		if (sends == 2)
			GASNETC_AMTRACE_ReplyLong(Send);
		else if (sends == 1) {
			if (payload)
				GASNETC_AMTRACE_ReplyLong(Queued);
			else
				GASNETC_AMTRACE_ReplyLong(Send);
		}
		else
			GASNETC_AMTRACE_ReplyLong(Queued);
	}
	#endif

	va_end(argptr);
	return GASNET_OK;
}

int gasnetc_ReplySystem( 
	gasnet_token_t token,       /* token provided on handler entry */
	gasnet_handler_t handler, /* index into destination endpoint's handler table */ 
	void *source_addr, size_t nbytes,   /* data payload */
	int numargs, ...) 
{
  va_list argptr;
  gasnetc_bufdesc_t *bufd;
  va_start(argptr, numargs); /*  pass in last argument */

  gasneti_assert(numargs >= 0 && numargs <= gasnet_AMMaxArgs());
  gasneti_assert(nbytes <= GASNETC_AM_MEDIUM_MAX);

  if ((void *)token == (void *)-1) { /* local handler */
    void *loopbuf;
    int argbuf[GASNETC_AM_MAX_ARGS];
    loopbuf = gasnetc_alloca(nbytes+GASNETI_MEDBUF_ALIGNMENT);
    loopbuf = (void *)GASNETI_ALIGNUP(loopbuf, GASNETI_MEDBUF_ALIGNMENT);
    memcpy(loopbuf, source_addr, nbytes);
    GASNETC_ARGS_WRITE(argbuf, argptr, numargs);
    GASNETI_RUN_HANDLER_MEDIUM(0, handler, _gmc.syshandlers[handler], token,
				argbuf, numargs, loopbuf, nbytes);
  }
#if GASNET_PSHM
  /* Still need the loopback code above for the !gasneti_init_done case */
  else if_pt (gasneti_init_done && gasnetc_token_is_pshm(token)) {
    int retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Medium, token,
                                             handler | GASNETC_SYS_HANDLER_FLAG,
                                             source_addr, nbytes, 0,
                                             numargs, argptr);
    va_end(argptr);
    return retval;
  }
#endif
  else {
    bufd = gasnetc_bufdesc_from_token(token);
    bufd->len = 
	    gasnetc_write_AMBufferSystem(bufd->buf, handler, numargs, 
                    argptr, nbytes, source_addr, GASNETC_AM_REPLY);

    gasneti_mutex_lock(&gasnetc_lock_gm);
    if (gasnetc_token_hi_acquire()) {
       gasnetc_GMSend_bufd(bufd); 
    } else {
	gasneti_assert(bufd->gm_id > 0);
       gasnetc_fifo_insert(bufd);
    }
    gasneti_mutex_unlock(&gasnetc_lock_gm);
  }
  
  va_end(argptr);
  return GASNET_OK;
}

/* -------------------------------------------------------------------------- */
/* Core misc. functions                                                       */
void
gasnetc_AllocPinnedBufs(void)
{
	int	i, nbufs;
	size_t	buflen;
	uint8_t	*ptr;
	void	*bufptr;

	int	idx_ammed, idx_amreq, idx_rlo, idx_rhi;

	gasnetc_bufdesc_t	*bufd;

	gasneti_assert(_gmc.port != NULL);
	gasneti_mutex_lock(&gasnetc_lock_gm);

	_gmc.stoks.max = gm_num_send_tokens(_gmc.port);
	_gmc.stoks.hi = _gmc.stoks.lo = _gmc.stoks.total = 0;

	_gmc.rtoks.max = gm_num_receive_tokens(_gmc.port);
	_gmc.rtoks.lo = _gmc.rtoks.hi = _gmc.rtoks.max/2;

	/* We need to allocate the following types of DMA'd buffers:
	 * A) 1 AMMedBuf (handling replies after an AMMediumRequest) 
	 * B) (stoks-1) AMRequest bufs
	 * C) rtoks.lo AMRequest receive buffers
	 * D) rtoks.hi AMReply receive buffers
	 *
	 * Note that each of these have a bufdesc_t attached to them
	 */
	nbufs = 1 + _gmc.stoks.max-1 + _gmc.rtoks.lo + _gmc.rtoks.hi;

	buflen = (size_t) (nbufs * GASNETC_AM_LEN);

	/* Allocate and register DMA buffers */ 
	_gmc.dma_bufs = gm_alloc_pages(buflen);
	if_pf (_gmc.dma_bufs == NULL)
		gasneti_fatalerror("gm_alloc_pages(%d) %s", (int)buflen,
		   gasneti_current_loc);

	if (gm_register_memory(_gmc.port, _gmc.dma_bufs, buflen) != GM_SUCCESS)
		gasneti_fatalerror("Can't pin GASNet buffers");

	/* Allocate the AMRequest send buffer pool stack */
	_gmc.reqs_pool = (int *) 
	    gasneti_malloc(sizeof(int) * (_gmc.stoks.max-1));

	/* Allocate a buffer descriptor (bufdesc_t) for each DMA'd buffer
	 * and fill in id/sendbuf for cheap reverse lookups */
	_gmc.bd_ptr = (gasnetc_bufdesc_t *)
	    gasneti_malloc(nbufs * sizeof(gasnetc_bufdesc_t));
	ptr = (uint8_t *) _gmc.dma_bufs;

	/* Initialize each of the buffer descriptors with their buffers */
	for (i = 0; i < nbufs; i++) {
		_gmc.bd_ptr[i].id = i;
		_gmc.bd_ptr[i].buf = (void *)(ptr + i*GASNETC_AM_LEN);
	}

	/* Token ids have the following assignments */
	idx_ammed = 0;
	idx_amreq = 1;
	idx_rlo   = 1 + (_gmc.stoks.max-1);
	idx_rhi   = idx_rlo + _gmc.rtoks.lo;
	gasneti_assert(idx_rhi + _gmc.rtoks.hi == nbufs);

	/* A) AMMedBuf for AMMediums */
	_gmc.AMMedBuf = &_gmc.bd_ptr[idx_ammed]; 
	BUFD_SETSTATE(&(_gmc.bd_ptr[idx_ammed]), BUFD_S_TEMPMED);

	/* B) stoks-1 AMRequest bufs */
	_gmc.reqs_pool_max = _gmc.stoks.max-1;
	_gmc.reqs_pool_cur = -1;
	for (i = idx_amreq; i < idx_rlo; i++) {
		bufptr = (void *)(ptr + (i<<GASNETC_AM_SIZE));
		bufd = GASNETC_BUFDESC_PTR(bufptr);
		GASNETC_ASSERT_BUFDESC_PTR(bufd, bufptr);

		BUFD_SETSTATE(bufd, BUFD_S_USED);
		gasnetc_provide_AMRequestPool(bufd);
	}
	gasneti_assert(_gmc.reqs_pool_cur == _gmc.reqs_pool_max-1);
	
	/* C) rtoks.lo AMRequest receive buffers given to GM */
	for (i = idx_rlo; i < idx_rhi; i++) {
		bufptr = (void *)(ptr + (i<<GASNETC_AM_SIZE));
		bufd = GASNETC_BUFDESC_PTR(bufptr);
		GASNETC_ASSERT_BUFDESC_PTR(bufd, bufptr);

		BUFD_SETSTATE(bufd, BUFD_S_USED);
		gasnetc_provide_AMReply(bufd);
	}

	/* D) rtoks.hi AMReply receive buffers given to GM */
	for (i = idx_rhi; i < nbufs; i++) { 
		bufptr = (void *)(ptr + (i<<GASNETC_AM_SIZE));
		bufd = GASNETC_BUFDESC_PTR(bufptr);
		GASNETC_ASSERT_BUFDESC_PTR(bufd, bufptr);

		BUFD_SETSTATE(bufd, BUFD_S_USED);
		gasnetc_provide_AMRequest(bufd);
	}

	if (gm_set_acceptable_sizes(_gmc.port, GM_HIGH_PRIORITY, 
			1<<GASNETC_AM_SIZE) != GM_SUCCESS)
		gasneti_fatalerror("can't set acceptable sizes for HIGH "
			"priority");
	if (gm_set_acceptable_sizes(_gmc.port, GM_LOW_PRIORITY, 
			1<<GASNETC_AM_SIZE) != GM_SUCCESS)
		gasneti_fatalerror("can't set acceptable sizes for LOW "
			"priority");
	gm_allow_remote_memory_access(_gmc.port);

	_gmc.bd_list_num = nbufs;

	gasneti_mutex_unlock(&gasnetc_lock_gm);

	return;
}

void
gasnetc_DestroyPinnedBufs(void)
{
	if (_gmc.dma_bufs != NULL)
		gm_free_pages(_gmc.dma_bufs, 
		    _gmc.bd_list_num << GASNETC_AM_SIZE);
	if (_gmc.bd_ptr != NULL)
		gasneti_free(_gmc.bd_ptr);
	if (_gmc.reqs_pool != NULL)
		gasneti_free(_gmc.reqs_pool);
}

int	
gasnetc_gm_nodes_compare(const void *k1, const void *k2)
{
	gasnetc_gm_nodes_rev_t	*a = (gasnetc_gm_nodes_rev_t *) k1;
	gasnetc_gm_nodes_rev_t	*b = (gasnetc_gm_nodes_rev_t *) k2;

	if (a->id > b->id)
		return 1;
	else if (a->id < b->id)
		return -1;
	else {
		if (a->port > b->port) return 1;
		if (a->port < b->port) return -1;
		else
			return 0;
	}
}

/* -------------------------------------------------------------------------- */

void
gasnetc_GMSend_AMRequest(void *buf, uint32_t len, 
		uint32_t id, uint32_t port,
		gm_send_completion_callback_t callback, 
		void *callback_ptr, uintptr_t dest_addr)
{
	int sent = 0;

	while (!sent) {
		/* don't force locking when polling */
		while (!GASNETC_TOKEN_LO_AVAILABLE())
			gasnetc_AMPoll();

		gasneti_mutex_lock(&gasnetc_lock_gm);
		/* assure last poll was successful */
		if (GASNETC_TOKEN_LO_AVAILABLE()) {
			if (dest_addr > 0)
				GASNETC_GM_PUT(_gmc.port, buf, dest_addr, 
					(unsigned int) len, GM_LOW_PRIORITY, 
					id, port, callback, callback_ptr);
			else {
				gasneti_assert(GASNETC_AM_IS_REQUEST(
				       *((uint8_t *) buf)));
				gasneti_assert(len <= GASNETC_AM_PACKET);

				if (_gmc.my_port == port)
				gm_send_to_peer_with_callback(_gmc.port, buf, 
					GASNETC_AM_SIZE, (unsigned int) len,
					GM_LOW_PRIORITY, id, callback,
					callback_ptr);
				else
				gm_send_with_callback(_gmc.port, buf, 
					GASNETC_AM_SIZE, (unsigned int) len,
					GM_LOW_PRIORITY, id, port, callback,
					callback_ptr);
			}
			_gmc.stoks.lo += 1;
			_gmc.stoks.total += 1;
			sent = 1;
		}
		gasneti_mutex_unlock(&gasnetc_lock_gm);
	}
}

void
gasnetc_GMSend_AMSystem(void *buf, size_t len, 
			uint16_t id, uint16_t port, gasnetc_bufdesc_t *bufd)
{
	gasneti_mutex_assertlocked(&gasnetc_lock_gm);
	gasneti_assert(buf != NULL);
	gasneti_assert(len >= 4); 
	gasneti_assert(id > 0);
	gasneti_assert(port > 0 && port < GASNETC_GM_MAXPORTS);
	gasneti_assert(len <= gm_max_length_for_size(GASNETC_AM_SIZE));

	gasnetc_token_lo_poll();

	gasneti_assert(GASNETC_AM_IS_SYSTEM(*((uint8_t *) buf)));
	gm_send_with_callback(_gmc.port, buf, GASNETC_AM_SIZE, len, 
			GM_LOW_PRIORITY, id, port, gasnetc_callback_system, 
			bufd);

	GASNETI_TRACE_PRINTF(C, 
	    ("AMSystem Send (node=%d,%d, handler_id=0x%x, len=%d)", id, port,
	     *(((uint8_t *)buf)+1), (int)len));
}


gasnetc_bufdesc_t *
gasnetc_AMRequestPool_block() 
{
	int			 bufd_idx = -1;
	gasnetc_bufdesc_t	*bufd;

	gasneti_mutex_assertunlocked(&gasnetc_lock_gm);

	/* Since every AMRequest send must go through the Pool, use this
	 * as an entry point to make progress in the Receive queue */
	gasneti_AMPoll();

	while (bufd_idx < 0) {
		while (_gmc.reqs_pool_cur < 0)
			gasnetc_AMPoll();

		gasneti_mutex_lock(&gasnetc_lock_reqpool);
		if_pt (_gmc.reqs_pool_cur >= 0) {
			bufd_idx = _gmc.reqs_pool[_gmc.reqs_pool_cur];
			GASNETI_TRACE_PRINTF(C,
			    ("AMRequestPool (%d/%d) gave bufdesc id %d\n",
	    		    _gmc.reqs_pool_cur, _gmc.reqs_pool_max,
	    		    _gmc.reqs_pool[_gmc.reqs_pool_cur]));
			_gmc.reqs_pool_cur--;
		}
		gasneti_mutex_unlock(&gasnetc_lock_reqpool);
	}
	gasneti_assert(bufd_idx < _gmc.bd_list_num);
	bufd = &(_gmc.bd_ptr[bufd_idx]);

	gasneti_assert(BUFD_ISSTATE(bufd) == BUFD_S_AMREQ);
	BUFD_SETSTATE(bufd, BUFD_S_USED);
	gasneti_assert(bufd->buf != NULL);
	gasneti_assert(bufd->id == bufd_idx);

	return bufd;
}

/* -------------------------------------------------------------------------- */
/*
 * Bootstrap operations
 */

/*
 * Gather/Send operations are used for bootstrapBarrier and bootstrapExchange.
 *
 * bootstrap Barrier essentially exchanges only a single byte whereas
 * bootstrapExchange may exchange node information.  
 *
 * In the case of a barrier, the data should be NULL and size at 0.
 * In the case of an exchange, the data can be any size.
 *
 * The current implementation limits exchanges to 4K, which is not necessarily
 * very scalable.  
 *
 * If the input buffer for the broadcast is <= 4096/nodes
 *    Use gather(mybuf) -> broadcast_from_zero(allbufs)
 * 
 * If the input buffer for the broadcast is > 4096/nodes
 *    Use broadcast_from_zero(dest_buf_addr) ->
 *            gather_to_zero(mybuf) ->
 *                 broadcast_from_zero(allbufs)
 *
 * The first version agrees to use the static Gather buffer while the second
 * requires a dynamic buffer to be allocated, which causes each node to wait
 * for a broadcast from 0 that contains the address of this buffer.
 *
 * Since the Gather/Broadcast is used before gasnet_attach(), it is not meant
 * to be thread-safe and counter increments assume only one thread is running
 * AM handlers.
 */

static int		gasnetc_bootstrapGather_phase	      = 0; 
static volatile int	gasnetc_bootstrapGatherSendInProgress = 0;

static void	       *gasnetc_bootstrapGather_buf[2]       = { NULL, NULL };
static size_t	        gasnetc_bootstrapGather_bufsz[2]     = { 0, 0 };
static volatile int	gasnetc_bootstrapGather_bufalloc[2]  = { 0, 0 };
static volatile int	gasnetc_bootstrapGather_allocdone[2] = { 0, 0 };
static volatile int	gasnetc_bootstrapGather_recvd[2]     = { 0, 0 };
static volatile int	gasnetc_bootstrapBroadcast_recvd[2]  = { 0, 0 };

#define GASNETC_BROADCASTROOT_NODE   0

#if 0
#define GASNETC_BOOTTRACE_PRINTF(t,c)	printf c
#else
#define GASNETC_BOOTTRACE_PRINTF(t,c)
#endif

void
gasnetc_AllocGatherBufs(void)
{
    size_t  allocsz;

    allocsz = MIN(GASNETC_AM_SYSTEM_MAX, 
		  2*sizeof(gasnet_seginfo_t)*gasneti_nodes);

    gasnetc_bootstrapGather_buf[0] = gasneti_malloc(allocsz);
    gasnetc_bootstrapGather_buf[1] = gasneti_malloc(allocsz);

    gasnetc_bootstrapGather_bufsz[0] = allocsz;
    gasnetc_bootstrapGather_bufsz[1] = allocsz;

}

void
gasnetc_DestroyGatherBufs(void)
{
    if (gasnetc_bootstrapGather_buf[0] != NULL)
	    gasneti_free(gasnetc_bootstrapGather_buf[0]);
    gasnetc_bootstrapGather_bufsz[0] = 0;

    if (gasnetc_bootstrapGather_buf[1] != NULL)
	    gasneti_free(gasnetc_bootstrapGather_buf[1]);
    gasnetc_bootstrapGather_bufsz[1] = 0;
}


void 
gasnetc_SysGather_reqh(gasnet_token_t token, void *addr, size_t nbytes,
		       gasnet_handlerarg_t phase)
{
    gasnet_node_t   node;
    gasnet_AMGetMsgSource(token, &node);

    /* Only the root runs the request handler */
    gasneti_assert(gasneti_mynode == GASNETC_BROADCASTROOT_NODE);
    /* We only support two phases */
    gasneti_assert(phase == 0 || phase == 1);

    if (nbytes > 0)
	memcpy((uint8_t *) gasnetc_bootstrapGather_buf[phase] + node*nbytes, 
	       addr, nbytes);

    gasnetc_bootstrapGather_recvd[phase]++;
    GASNETC_BOOTTRACE_PRINTF(C, 
	("AMSystem Gather Received (node=%d,phase=%d,cnt=%d,nbytes=%d)\n", 
	 node, phase, gasnetc_bootstrapGather_recvd[phase], nbytes));
}

/*
 * At startup, GASNet allocates a static bootstrapGather_buf large enough to
 * fit sizeof(gasnet_seginfo_t) * gasnet_numnodes().  If the exchange request
 * is larger than this buffer, node 0 broadcasts the new total size of the
 * exchange to all nodes.
 *
 */
void 
gasnetc_SysBroadcast_reqh(gasnet_token_t token, void *addr, size_t nbytes,
			  gasnet_handlerarg_t phase, gasnet_handlerarg_t exchsz)
{
    gasnet_node_t   node;
    gasnet_AMGetMsgSource(token, &node);

    /* We only support two phases */
    gasneti_assert(phase == 0 || phase == 1);

    if (nbytes > 0)
	memcpy((uint8_t *) gasnetc_bootstrapGather_buf[phase] + node*nbytes, 
	       addr, nbytes);

    gasnetc_bootstrapBroadcast_recvd[phase]++;
    gasneti_assert(gasnetc_bootstrapBroadcast_recvd[phase] == 1);

    GASNETC_BOOTTRACE_PRINTF(C, 
	("AMSystem Broadcast Received (node=%d,phase=%d,nbytes=%d)\n", 
	 gasneti_mynode, phase, nbytes));
}

/*
 * BroadcastAlloc handlers
 */
void
gasnetc_SysBroadcastAlloc_reph(gasnet_token_t token, gasnet_handlerarg_t phase)
{
    gasneti_assert(gasneti_mynode == GASNETC_BROADCASTROOT_NODE);
    gasneti_assert(phase == 0 || phase == 1);

    gasnetc_bootstrapGather_allocdone[phase]++;

    #ifdef GASNET_TRACE
      {
	gasnet_node_t   node;
	gasnet_AMGetMsgSource(token, &node);

	GASNETC_BOOTTRACE_PRINTF(C, 
	    ("AMSystem BroadcastAlloc Received (node=%d,phase=%d,cnt=%d)\n", 
	    node, phase, gasnetc_bootstrapGather_allocdone[phase]));
      }
    #endif
}
void
gasnetc_SysBroadcastAlloc_reqh(gasnet_token_t token, gasnet_handlerarg_t phase, 
			       gasnet_handlerarg_t exchsz)
{
    gasnet_node_t   node;
    gasnet_AMGetMsgSource(token, &node);

    gasneti_assert(phase == 0 || phase == 1);
    gasneti_assert(node == GASNETC_BROADCASTROOT_NODE);
    gasneti_assert(gasnetc_bootstrapGather_bufsz[phase] < exchsz);
    gasneti_assert(gasnetc_bootstrapGather_bufalloc[phase] == 0);

    if (gasnetc_bootstrapGather_buf[phase] != NULL)
	gasneti_free(gasnetc_bootstrapGather_buf[phase]);

    gasnetc_bootstrapGather_buf[phase] = gasneti_malloc(exchsz);
    gasnetc_bootstrapGather_bufsz[phase] = exchsz;
    gasnetc_bootstrapGather_bufalloc[phase]++;

    gasnetc_ReplySystem(token, gasneti_handleridx(gasnetc_SysBroadcastAlloc_reph),
	NULL, 0, 1, phase);
}

void *
gasnetc_bootstrapGatherSend(void *data, size_t len)
{
	uint8_t	 *hdr, *payload;
	uint16_t *phase_ptr;
	int	 i, phase;
	size_t	 exchsz;
	int	 sent = 0;

	if (gasnetc_bootstrapGatherSendInProgress)
	    gasneti_fatalerror(
		    "Cannot issue two successive gasnetc_bootstrapExchange");

	gasnetc_bootstrapGatherSendInProgress++;

	/* The size to exchange is the per-node payload coalesced into one
	 * contiguous buffer.  The function assumes that copying per-node
	 * buffers contiguously based on the given size will not cause any
	 * alignment problems.
	 */
	exchsz = len * gasneti_nodes;

	if (exchsz > GASNETC_AM_SYSTEM_MAX)
	    gasneti_fatalerror(
		"bootstrapGatherSend: %i bytes too large\n", (int)exchsz);

	phase = gasnetc_bootstrapGather_phase;
	gasnetc_bootstrapGather_phase ^= 1;
	gasnetc_bootstrapBroadcast_recvd[phase] = 0;

	/* First check if existing buffer is large enough */
	if (gasnetc_bootstrapGather_bufsz[phase] < exchsz) {

	    if (gasneti_mynode == GASNETC_BROADCASTROOT_NODE) {
		/* We need to reallocate the Gather buffer */
		gasnetc_bootstrapGather_allocdone[phase] = 0;
		for (i = 0; i < gasneti_nodes; i++) {
		    gasnetc_RequestSystem(i, 
			gasneti_handleridx(gasnetc_SysBroadcastAlloc_reqh), NULL,
			NULL, 0, 2, phase, exchsz);
		}
		/* Wait until allocation from all nodes is complete */
		GASNET_BLOCKUNTIL(
		    gasnetc_bootstrapGather_allocdone[phase] == gasneti_nodes);
		gasnetc_bootstrapGather_allocdone[phase] = 0;
	    }

	    GASNET_BLOCKUNTIL(gasnetc_bootstrapGather_bufalloc[phase] == 1);
	    /* Make sure the flag is reset for the next iteration */
	    gasnetc_bootstrapGather_bufalloc[phase] = 0;
	}

	/*
	 * By now, we are certain the buffer to gather into is large enough.
	 * All nodes, including GASNETC_BROADCASTROOT_NODE, send a gather
	 * request to GASNETC_BROADCASTROOT_NODE with their payload.
	 */

	sent = 0;
	gasnetc_RequestSystem(GASNETC_BROADCASTROOT_NODE, 
	    gasneti_handleridx(gasnetc_SysGather_reqh), &sent, 
	    data, len, 1, phase);

	if (gasneti_mynode == GASNETC_BROADCASTROOT_NODE) {

	    GASNET_BLOCKUNTIL(
	        gasnetc_bootstrapGather_recvd[phase] == gasneti_nodes);
	    gasnetc_bootstrapGather_recvd[phase] = 0;

	    if (gasneti_nodes == 1)
		goto barrier_done;

	    sent = 0;
	    /* Once the gather is done, Broadcast to all nodes */ 
	    for (i = 0; i < gasneti_nodes; i++) {
		gasnetc_RequestSystem(i, 
		    gasneti_handleridx(gasnetc_SysBroadcast_reqh), &sent,
		    gasnetc_bootstrapGather_buf[phase], exchsz, 1, phase);
	    }

	    /* Wait until all requests are sent */
	    GASNET_BLOCKUNTIL(sent == gasneti_nodes);
	}

	/* All nodes, including GASNETC_BROADCASTROOT_NODE, wait until the
	 * broadcast is recevied */
	GASNET_BLOCKUNTIL(gasnetc_bootstrapBroadcast_recvd[phase] == 1);
	gasnetc_bootstrapBroadcast_recvd[phase] = 0;

barrier_done:

	gasnetc_bootstrapGatherSendInProgress = 0;
	return gasnetc_bootstrapGather_buf[phase];
}

void
gasnetc_bootstrapBarrier() 
{
	gasnetc_bootstrapGatherSend(NULL, 0);
}

void
gasnetc_bootstrapExchange(void *src, size_t len, void *dest)
{
	void	*buf;

	buf = gasnetc_bootstrapGatherSend(src, len);
	memcpy(dest, buf, len*gasneti_nodes);

	return;
}

void
gasnetc_dump_tokens(void)
{
	GASNETI_TRACE_PRINTF(C,
	    ("Send tokens: lo=%3d, hi=%3d, tot=%3d, max=%3d\n",
	    _gmc.stoks.lo, _gmc.stoks.hi, _gmc.stoks.total, _gmc.stoks.max));

	GASNETI_TRACE_PRINTF(C,
	    ("Recv tokens: lo=%3d, hi=%3d, tot=%3d, max=%3d\n",
	    _gmc.rtoks.lo, _gmc.rtoks.hi, _gmc.rtoks.total, _gmc.rtoks.max));
}

int
gasnetc_alloc_nodemap(int numnodes)
{
	_gmc.gm_nodes = (gasnetc_gm_nodes_t *) 
	    gasneti_malloc(numnodes*sizeof(gasnetc_gm_nodes_t));

	_gmc.gm_nodes_rev = (gasnetc_gm_nodes_rev_t *) 
	    gasneti_malloc(numnodes * sizeof(gasnetc_gm_nodes_rev_t));

	return (_gmc.gm_nodes != NULL && _gmc.gm_nodes_rev != NULL);
}

int
gasnetc_gmport_allocate(int *board, int *port)
{
	struct gm_port	*p;
	unsigned int	port_id, board_id;
	gm_status_t	status;

	gm_init();

	for (port_id = 2; port_id < GASNETC_GM_MAXPORTS; port_id++) {
		if (port_id == 3)
			continue;

		for (board_id = 0; board_id < GASNETC_GM_MAXBOARDS; board_id++) {

			status = gm_open(&p, board_id, port_id, 
					"GASNet/GM", (enum gm_api_version)GM_API_VERSION_1_4);

			switch (status) {
				case GM_SUCCESS:
					*board = board_id;
					*port = port_id;
					_gmc.port = p;
					return 1;
					break;
				case GM_INCOMPATIBLE_LIB_AND_DRIVER:
					gasneti_fatalerror("GM library and "
					    "driver are out of sync!");
					break;
				default:
					break;
			}

		}
	}
	return 0;
}

void
gasnetc_getconf_conffile(void)
{
	FILE		*fp;
	char		line[128];
	char		gmconf[128], *gmconfenv;
	char		gmhost[128];
        const char *hostname = gasnett_gethostname();
	char		**hostnames;
	char		*homedir;
	int		lnum = 0, gmportnum, i;
	int		thisport = 0, thisid = 0, numnodes = 0, thisnode = -1;
	int		temp_id;
	gm_status_t	status;
	struct gm_port	*p;

	if ((homedir = getenv("HOME")) == NULL)
		gasneti_fatalerror("Couldn't find $HOME directory");

	if ((gmconfenv = getenv("GMPI_CONF")) != NULL)
		snprintf(gmconf, 128, "%s", gmconfenv);
	else
		snprintf(gmconf, 128, "%s/.gmpi/conf", homedir);

	if ((fp = fopen(gmconf, "r")) == NULL) {
		fprintf(stderr, "Couldn't open GMPI configuration file\n: %s", 
		    gmconf);
		return;
	}

	/* must do gm_init() from this point on since gm_host_name_to_node_id
	 * must use the port
	 */

	while (fgets(line, 128, fp)) {
	
		if (lnum == 0) {
	      		if ((sscanf(line, "%d\n", &numnodes)) < 1) 
				gasneti_fatalerror(
				    "job size not found in GMPI config file");
	      		else if (numnodes < 1) 
				gasneti_fatalerror(
				    "invalid numnodes in GMPI config file");

			if (!gasnetc_alloc_nodemap(numnodes))
				gasneti_fatalerror(
				    ("Can't allocate node mapping"));

			hostnames = (char **)
			    gasneti_malloc((numnodes+1)*sizeof(char *));
			hostnames[numnodes] = NULL;
			for (i = 0; i < numnodes; i++) {
				hostnames[i] =
				gasneti_malloc(MAXHOSTNAMELEN);
			}
			lnum++;
	      	}

		else if (lnum <= numnodes) {
			if ((sscanf(line,"%s %d\n",gmhost,&gmportnum)) == 2) {
				if (gmportnum < 1 || gmportnum > 7)
					gasneti_fatalerror("(%s) Invalid GM port: %i",
                                           gasnett_gethostname(),gmportnum);

				gasneti_assert(gmhost != NULL);

				_gmc.gm_nodes[lnum-1].port = gmportnum;
				memcpy(&hostnames[lnum-1][0], 
				    (void *)gmhost, MAXHOSTNAMELEN);

				if (strcasecmp(gmhost, hostname) == 0) {
					GASNETI_TRACE_PRINTF(C,
					    ("%s will bind to port %d\n", 
					    hostname, gmportnum) );
					thisnode = lnum-1;
					thisport = gmportnum;
				}
			}
                        else {
				fprintf(stderr, "couldn't parse: %s\n", line);
			}
			lnum++;
		}
	}
	
	fclose(fp);

	if (numnodes == 0 || thisnode == -1)
		gasneti_fatalerror("(%s) could not find myself in GMPI config file", gasnett_gethostname());
	gm_init();
	status = 
		gm_open(&p, GASNETC_DEFAULT_GM_BOARD_NUM, thisport,"GASNet/GM", 
		    (enum gm_api_version)GM_API_VERSION_1_4);
	if (status != GM_SUCCESS) 
		gasneti_fatalerror("(%s) could not open GM port %d", gasnett_gethostname(), thisport);
	status = gm_get_node_id(p, (unsigned int *) &thisid);
	if (status != GM_SUCCESS)
		gasneti_fatalerror("(%s) could not get GM node id!", gasnett_gethostname());

	_gmc.port = p;

#ifdef GASNETC_GM_2
	temp_id = thisid;

	/* GM2 only stores local node ids, so a global has to be obtained */
	if (gm_node_id_to_global_id(_gmc.port, temp_id, &(_gmc.my_id)) 
	    != GM_SUCCESS)
		gasneti_fatalerror("(%s) Couldn't get GM global node id", gasnett_gethostname());
#endif

	for (i = 0; i < numnodes; i++) {
		_gmc.gm_nodes[i].id = 
		    gm_host_name_to_node_id(p, hostnames[i]);

		if (_gmc.gm_nodes[i].id == GM_NO_SUCH_NODE_ID) {
			fprintf(stderr, "%s (%d) has no id! Check mapper\n",
			    hostnames[i],
			    _gmc.gm_nodes[i].id);
			gasneti_fatalerror("(%s) Unknown GMid or GM mapper down", gasnett_gethostname());
		}
		_gmc.gm_nodes_rev[i].port = _gmc.gm_nodes[i].port;
		_gmc.gm_nodes_rev[i].node = (gasnet_node_t) i;

		_gmc.gm_nodes_rev[i].id = _gmc.gm_nodes[i].id;

		GASNETI_TRACE_PRINTF(C, ("%d> %s (gm %d, port %d)\n", 
		    i, hostnames[i], _gmc.gm_nodes[i].id, 
		    _gmc.gm_nodes[i].port));

	}

	gasneti_mynode = thisnode;
	gasneti_nodes = numnodes;
	for (i = 0; i < numnodes; i++)
		gasneti_free(hostnames[i]);
	gasneti_free(hostnames);

	/* sort out the gm_nodes_rev for bsearch, glibc qsort uses recursion,
	 * so stack memory in order to complete the sort.  We want to minimize
	 * the number of mallocs
	 */
	qsort(_gmc.gm_nodes_rev, numnodes, sizeof(gasnetc_gm_nodes_rev_t),
	    gasnetc_gm_nodes_compare);
	_gmc.port = p;
	return;
}


#ifdef GASNETI_GM_RODATA_WORKAROUND
  /* ensure rodata object is linked into every executable */
  extern const int gasneti_dummy_rodata_writable;
  extern int gasneti_dummy_rodata_fn(void) { return gasneti_dummy_rodata_writable; }
#endif

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
  gasneti_handler_tableentry_with_bits(gasnetc_am_medcopy),
  { 0, NULL }
};

gasnet_handlerentry_t const *gasnetc_get_handlertable(void) {
  return gasnetc_handlers;
}

static gasnet_handlerentry_t const gasnetc_syshandlers[] = {
  /* ptr-width independent handlers */
  gasneti_handler_tableentry_no_bits(gasnetc_SysBroadcastAlloc_reqh),
  gasneti_handler_tableentry_no_bits(gasnetc_SysBroadcastAlloc_reph),
  gasneti_handler_tableentry_no_bits(gasnetc_SysGather_reqh),
  gasneti_handler_tableentry_no_bits(gasnetc_SysBroadcast_reqh),

  gasneti_handler_tableentry_no_bits(gasnetc_SysExitRole_reqh),
  gasneti_handler_tableentry_no_bits(gasnetc_SysExitRole_reph),
  gasneti_handler_tableentry_no_bits(gasnetc_SysExit_reqh),
  gasneti_handler_tableentry_no_bits(gasnetc_SysExit_reph),
  /* ptr-width dependent handlers */
  { 0, NULL }
};

void
gasnetc_registerSysHandlers() 
{
    int i = 0;
    gasnet_handlerentry_t *stable = 
	(gasnet_handlerentry_t *) gasnetc_syshandlers;

    while (stable[i].fnptr != NULL) {
	_gmc.syshandlers[i] = stable[i].fnptr;
	i++;
    }
}

/* ------------------------------------------------------------------------------------ */

