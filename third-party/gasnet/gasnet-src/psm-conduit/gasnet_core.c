/*   $Source: bitbucket.org:berkeleylab/gasnet.git/psm-conduit/gasnet_core.c $
 * Description: GASNet psm conduit Implementation
 * Copyright (c) 2013-2015 Intel Corporation. All rights reserved.
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>

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

size_t gasnetc_psm_max_request_len;
size_t gasnetc_psm_max_reply_len;

gasnetc_psm_state_t gasnetc_psm_state;

gasneti_spawnerfn_t const *gasneti_spawner = NULL;

/* Core API psm2-level handlers */
int gasnetc_handler_short(psm2_am_token_t token,
        psm2_amarg_t* args, int nargs, void* addr, uint32_t len);
int gasnetc_handler_med(psm2_am_token_t token,
        psm2_amarg_t* args, int nargs, void* addr, uint32_t len);
int gasnetc_handler_long(psm2_am_token_t token,
        psm2_amarg_t* args, int nargs, void* addr, uint32_t len);

/* Extended API psm2-level handlers */
int gasnete_handler_put(psm2_am_token_t token,
        psm2_amarg_t* args, int nargs, void* addr, uint32_t len);
int gasnete_handler_get_request(psm2_am_token_t token,
        psm2_amarg_t* args, int nargs, void* addr, uint32_t len);
int gasnete_handler_get_reply(psm2_am_token_t token,
        psm2_amarg_t* args, int nargs, void* addr, uint32_t len);
int gasnete_handler_long_put(psm2_am_token_t token,
        psm2_amarg_t* args, int nargs, void* addr, uint32_t len);
int gasnete_handler_long_get(psm2_am_token_t token,
        psm2_amarg_t* args, int nargs, void* addr, uint32_t len);
int gasnete_handler_long_put_reply(psm2_am_token_t token,
        psm2_amarg_t* args, int nargs, void* addr, uint32_t len);


/* -------------------------------------------------------------------------- */
/*
   Initialization
   ==============
   */
/* called at startup to check configuration sanity */
GASNETI_COLD
static void gasnetc_check_config(void) {
    gasneti_check_config_preinit();

    gasneti_assert(sizeof(void*) == sizeof(intptr_t)); /* For enter/leave handler */

    /* (###) add code to do some sanity checks on the number of nodes, handlers
     * and/or segment sizes */
}

GASNETI_COLD
static void gasneti_check_bug3333(int ver_major, int ver_minor) {
    gasnet_node_t pshm_width;
    gasnet_node_t node_width;

    /* Step 0. Return early if no detailed check is required.
     * TODO: check ver_major.ver_minor against fixed release when available.
     */
    if (gasneti_getenv_yesno_withdefault("GASNET_PSM_ENABLE_SHM", 0)) {
        return;
    }

    /* Step 1. Maximum width of any compute node in this job (the "ppn").
     * Replicates a portion of gasneti_nodemapInit w/o imposing the upper
     * bounds of GASNETI_PSHM_MAX_NODES or GASNET_SUPERNODE_MAXSIZE.
     */
    {
        uint32_t *allids = gasneti_malloc(gasneti_nodes * sizeof(uint32_t));
        uint32_t myid = gasneti_gethostid();
        int i, j;

        gasneti_bootstrapExchange(&myid, sizeof(uint32_t), allids);

        node_width = 0;
        for (i = 0; i < gasneti_nodes; ++i) {
            gasnet_node_t tmp = 1;
            for (j = i+1; j < gasneti_nodes; ++j) {
                tmp += (allids[j] == allids[i]);
            }
            node_width = MAX(node_width, tmp);
        }

        gasneti_free(allids);
    }

    /* Step 2. Maximum width of a pshm-domain (a "supernode"): */
#if GASNET_PSHM
    pshm_width = gasneti_getenv_int_withdefault("GASNET_SUPERNODE_MAXSIZE", 0, 0);
    pshm_width = pshm_width ? pshm_width : gasneti_nodes; /* default value of 0 means unlimited */
#else
    pshm_width = 1;
#endif

    /* Step 3. All or none exit */
    if (pshm_width < node_width) {
        if (!gasneti_mynode) {
            fprintf(stderr,
"****************************************************************************\n"
"* ERROR: This run would use PSM2's shared-memory device which is disabled  *\n"
"* by default in this release.  Please see \"Bug 3333\" in psm-conduit/README *\n"
"* (source) or README-psm (installed) for instructions to enable this run.  *\n"
"****************************************************************************\n"
);
            fflush(stderr);
        }
        gasneti_bootstrapBarrier();
        gasneti_bootstrapFini();
        _exit(1);
    }
}

GASNETI_COLD
static int gasnetc_init(int *argc, char ***argv) {
    psm2_error_t ret;

    /*  check system sanity */
    gasnetc_check_config();

    if (gasneti_init_done)
        GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");

    gasneti_freezeForDebugger();

#if GASNET_DEBUG_VERBOSE
    /* note - can't call trace macros during gasnet_init because trace system
       not yet initialized */
    fprintf(stderr,"gasnetc_init(): about to spawn...\n"); fflush(stderr);
#endif

    /*
     * Using existing bootstrap init.
     * This also initializes gasneti_nodes and gasneti_mynode globals.
     */
    gasneti_spawner = gasneti_spawnerInit(argc, argv, NULL, &gasneti_nodes, &gasneti_mynode);
    if (!gasneti_spawner) GASNETI_RETURN_ERRR(NOT_INIT, "GASNet job spawn failed");

    /* Must init timers after global env, and preferably before tracing */
    GASNETI_TICKS_INIT();

    /* Ensure uniform PSM2_* env vars */
    gasneti_propagate_env("PSM2_", GASNETI_PROPAGATE_ENV_PREFIX);

    /* (###) Add code here to determine which GASNet nodes may share memory.
       The collection of nodes sharing memory are known as a "supernode".  The
       (first) data structure to describe this is gasneti_nodemap[]: For all i:
       gasneti_nodemap[i] is the lowest node number collocated w/ node i where
       nodes are considered collocated if they have the same node "ID".
       Or in English:
       "gasneti_nodemap[] maps from node to first node on the same supernode."

       If the conduit has already communicated endpoint address information or
       a similar identifier that is unique per shared-memory compute node, then
       that info can be passed via arguments 2 through 4.  Otherwise the
       conduit should pass a non-null gasnetc_bootstrapExchange as argument 1
       to use platform-specific IDs, such as gethostid().  See
       gasneti_nodemapInit() in gasnet_internal.c for more usage documentation.
       See below for info on gasnetc_bootstrapExchange()

       If the conduit can build gasneti_nodemap[] w/o assistance, it should
       call gasneti_nodemapParse() after constructing it (instead of
       nodemapInit()).
       */
    gasneti_nodemapInit(&gasneti_bootstrapExchange, NULL, 0, 0);


    /* psm2 initialization */
    memset(&gasnetc_psm_state, 0, sizeof(gasnetc_psm_state_t));

    gasneti_spinlock_init(&gasnetc_psm_state.psm_lock);
#if GASNET_PSHM
    gasneti_spinlock_init(&gasnetc_psm_state.pshm_lock);
#endif
    gasnete_long_msg_init();

    /* Determine shutdown timeout */
    gasnetc_psm_state.exit_timeout =
        gasneti_get_exittimeout(300.0, 5.0, 0.01, 5.0);

    {
        int ver_major = PSM2_VERNO_MAJOR;
        int ver_minor = PSM2_VERNO_MINOR;
        psm2_uuid_t uuid;

        gasneti_check_bug3333(ver_major, ver_minor);

#if GASNET_PSHM && 0 /* DISABLED: see bug 3334 */
        /* Save memory (and maybe performance?) by disabling psm2's self and
           shared-memory communication models.  PSHM will be used instead. */
        setenv("PSM2_DEVICES", "hfi", 1);
#endif

        /* Set environment variables so psm2 allocates hardware contexts
         * correctly. */
        {
            char s[255] = {0};
            snprintf(s, sizeof(s), "%d", gasneti_myhost.node_rank);
            setenv("MPI_LOCALRANKID", s, 0);

            snprintf(s, sizeof(s), "%d", gasneti_myhost.node_count);
            setenv("MPI_LOCALNRANKS", s, 0);
        }

        ret = psm2_init(&ver_major, &ver_minor);
        if(ret != PSM2_OK) {
            char s[255] = {0};
            snprintf(s, sizeof(s), "psm2_init failure: %s\n",
                    psm2_error_get_string(ret));
            GASNETI_RETURN_ERRR(NOT_INIT, s);
        }

        if(gasneti_mynode == 0) {
            psm2_uuid_generate(uuid);
        }

        gasneti_bootstrapBroadcast(&uuid, sizeof(psm2_uuid_t), &uuid, 0);

        ret = psm2_ep_open(uuid, NULL,
                &gasnetc_psm_state.ep, &gasnetc_psm_state.epid);
        if(ret != PSM2_OK) {
            char s[255] = {0};
            snprintf(s, sizeof(s), "psm2_ep_open failure: %s\n",
                    psm2_error_get_string(ret));
            GASNETI_RETURN_ERRR(NOT_INIT, s);
        }

    ret = psm2_mq_init(gasnetc_psm_state.ep, PSM2_MQ_ORDERMASK_NONE,
            NULL, 0, &gasnetc_psm_state.mq);
    }

    {
        struct psm2_am_parameters params = {0};
        size_t size = 0;

        ret = psm2_am_get_parameters(gasnetc_psm_state.ep, &params,
                sizeof(struct psm2_am_parameters), &size);
        if(ret != PSM2_OK) {
            char s[255] = {0};
            snprintf(s, sizeof(s), "psm2_am_get_parameters failure: %s\n",
                    psm2_error_get_string(ret));
            GASNETI_RETURN_ERRR(NOT_INIT, s);
        }

        if(params.max_nargs * 2 < gasnet_AMMaxArgs()) {
            char s[255] = {0};
            snprintf(s, sizeof(s), "PSM/AM reports support for %u 32-bit arguments; GASNet requires %lu\n",
                    params.max_nargs * 2, (long)gasnet_AMMaxArgs());
            GASNETI_RETURN_ERRR(NOT_INIT, s);
        }

        gasnetc_psm_max_request_len = params.max_request_short;
        gasnetc_psm_max_reply_len = params.max_reply_short;

        gasneti_assert_always(gasnetc_psm_max_request_len <= GASNETC_MAX_MEDIUM_PSHM);
        gasneti_assert_always(gasnetc_psm_max_request_len <= gasnetc_psm_max_reply_len);
        gasneti_assert_always(gasnetc_psm_max_request_len >= 512);
    }

    {
        int i;
        psm2_am_handler_fn_t handlers[AM_HANDLER_NUM];

        handlers[AM_HANDLER_SHORT]   = gasnetc_handler_short;
        handlers[AM_HANDLER_MED]     = gasnetc_handler_med;
        handlers[AM_HANDLER_LONG]    = gasnetc_handler_long;
        handlers[AM_HANDLER_PUT]     = gasnete_handler_put;
        handlers[AM_HANDLER_GET_REQUEST] = gasnete_handler_get_request;
        handlers[AM_HANDLER_GET_REPLY] = gasnete_handler_get_reply;
        handlers[AM_HANDLER_LONG_PUT] = gasnete_handler_long_put;
        handlers[AM_HANDLER_LONG_GET] = gasnete_handler_long_get;
        handlers[AM_HANDLER_LONG_PUT_REPLY] = gasnete_handler_long_put_reply;

        ret = psm2_am_register_handlers(gasnetc_psm_state.ep,
                handlers, AM_HANDLER_NUM, gasnetc_psm_state.am_handlers);
        if(ret != PSM2_OK) {
            char s[255] = {0};
            snprintf(s, sizeof(s), "psm2_am_register_handlers failure: %s",
                    psm2_error_get_string(ret));
            GASNETI_RETURN_ERRR(BAD_ARG, s);
        }
    }

#if GASNET_DEBUG_VERBOSE
    fprintf(stderr,
            "gasnetc_init(): spawn successful - node %i/%i starting...\n",
            gasneti_mynode, gasneti_nodes); fflush(stderr);
#endif

#if GASNET_PSHM
    /* (###) If your conduit will support PSHM, you should initialize it here.
     * The 1st argument is normally "&gasnetc_bootstrapExchange" (described
     * below).  The 2nd argument is the amount of shared memory space needed
     * for any conduit-specific uses.  The return value is a pointer to the
     * space requested by the 2nd argument.
     */

    gasneti_pshm_init(gasneti_bootstrapSNodeBroadcast, 0);
#endif

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    {
        /* it may be appropriate to use gasneti_segmentInit() here to set
           gasneti_MaxLocalSegmentSize and gasneti_MaxGlobalSegmentSize,
           if your conduit can use memory anywhere in the address space
           (you may want to tune GASNETI_MMAP_MAX_SIZE to limit the max size)

           it may also be appropriate to first call gasneti_mmapLimit() to
           get a good value for the first argument to gasneti_segmentInit(), to
           account for limitations imposed by having multiple GASNet nodes
           per shared-memory compute node (this is recommended for all
           systems with virtual memory unless there can be only one
           process per compute node).
           */

        uintptr_t limit = gasneti_mmapLimit((uintptr_t)-1, (uint64_t)-1,
                &gasneti_bootstrapExchange, &gasneti_bootstrapBarrier);
        gasneti_segmentInit(limit, &gasneti_bootstrapExchange);
    }
#elif GASNET_SEGMENT_EVERYTHING
    /* segment is everything - nothing to do */
#else
#error Bad segment config
#endif

    /* Enable this if you wish to use the default GASNet services for
       broadcasting the environment from one compute node to all the others
       (for use in gasnet_getenv(), which needs to return environment variable
       values from the "spawning console").  You need to provide two functions
       (gasnetc_bootstrapExchange and gasnetc_bootstrapBroadcast) which the
       system can safely and immediately use to broadcast and exchange
       information between nodes (gasnetc_bootstrapBroadcast is optional but
       highly recommended).
       See gasnet/other/mpi-spawner/gasnet_bootstrap_mpi.c for definitions of
       these two functions in terms of MPI collective operations.  This system
       assumes that at least one of the compute nodes has a copy of the full
       environment from the "spawning console" (if this is not true, you'll
       need to implement something yourself to get the values from the spawning
       console) If your job system already always propagates environment
       variables to all the compute nodes, then you probably don't need this.
       */

    gasneti_setupGlobalEnvironment(gasneti_nodes, gasneti_mynode,
            gasneti_bootstrapExchange, gasneti_bootstrapBroadcast);

    gasneti_init_done = 1;

    gasneti_auxseg_init(); /* adjust max seg values based on auxseg */

    return GASNET_OK;
}


/* ------------------------------------------------------------------------------------ */
GASNETI_COLD
extern int gasnet_init(int *argc, char ***argv) {
    int retval = gasnetc_init(argc, argv);
    if (retval != GASNET_OK) GASNETI_RETURN(retval);
    gasneti_trace_init(argc, argv);
    return GASNET_OK;
}


/* -------------------------------------------------------------------------- */
#if GASNETI_CLIENT_THREADS
  /* The first field of threadinfo is a void* reserved for use by the "core".
   * Since "handler_depth" is the only thread-specific state required, we
   * just use it as an intptr_t without any additional indirection.
   */
  #define gasnetc_handler_depth (*(intptr_t*)gasnete_mythread())
#else
  static intptr_t gasnetc_handler_depth = 0;
#endif
#define gasnetc_enter_hander() ((void)++gasnetc_handler_depth)
#define gasnetc_leave_hander() do {             \
    --gasnetc_handler_depth;                    \
    gasneti_assert(gasnetc_handler_depth >= 0); \
  } while (0)

int gasnetc_handler_short(psm2_am_token_t token,
        psm2_amarg_t* args, int nargs, void* addr, uint32_t len)
{
    int is_request;
    int handler_idx;
    int gasnet_nargs;
    gasnet_token_t user_token = (gasnet_token_t)token;

    gasnetc_enter_hander();

    gasneti_assert(nargs >= 1);

    is_request = args[0].u32w0 & REQUEST_BIT;
    handler_idx = args[0].u32w0 & ~REQUEST_BIT;
    gasnet_nargs = args[0].u32w1;

    gasneti_assert((nargs - 1) * 2 >= gasnet_nargs);
    gasneti_assert(handler_idx >= 0 && handler_idx < 256);

    GASNETI_RUN_HANDLER_SHORT(is_request, handler_idx,
            gasnetc_handler[handler_idx],
            user_token, (uint32_t*)&args[1], gasnet_nargs);

    gasnetc_leave_hander();
    return 0;
}


int gasnetc_handler_med(psm2_am_token_t token,
        psm2_amarg_t* args, int nargs, void* addr, uint32_t len)
{
    int is_request;
    int handler_idx;
    int gasnet_nargs;
    void* userload;
    gasnet_token_t user_token = (gasnet_token_t)token;

    gasnetc_enter_hander();

    gasneti_assert(nargs >= 1);

    is_request = args[0].u32w0 & REQUEST_BIT;
    handler_idx = args[0].u32w0 & ~REQUEST_BIT;
    gasnet_nargs = args[0].u32w1;

    gasneti_assert((nargs - 1) * 2 >= gasnet_nargs);
    gasneti_assert(handler_idx >= 0 && handler_idx < 256);

    /* The payload buffer handed up from psm2 is read-only.
       GASNet handlers must be able to modify the buffer, so copy the data. */
    if(len > 0) {
        userload = alloca(len);
        memcpy(userload, addr, len);
    } else {
        userload = NULL;
    }

    GASNETI_RUN_HANDLER_MEDIUM(is_request, handler_idx,
            gasnetc_handler[handler_idx],
            user_token, (uint32_t*)&args[1], gasnet_nargs, userload, len);

    gasnetc_leave_hander();
    return 0;
}


int gasnetc_handler_long(psm2_am_token_t token,
        psm2_amarg_t* args, int nargs, void* addr, uint32_t len)
{
    int is_request;
    int handler_idx;
    int gasnet_nargs;
    void* dest_addr;
    gasnet_token_t user_token = (gasnet_token_t)token;

    gasnetc_enter_hander();

    gasneti_assert(nargs >= 2);

    is_request = args[0].u32w0 & REQUEST_BIT;
    handler_idx = args[0].u32w0 & ~REQUEST_BIT;
    gasnet_nargs = args[0].u32w1;
    dest_addr = (void*)args[1].u64w0;

    gasneti_assert((nargs - 2) * 2 >= gasnet_nargs);
    gasneti_assert(handler_idx >= 0 && handler_idx < 256);

    if(len > 0) {
        gasneti_assert(addr != NULL);
        gasneti_assert(dest_addr != NULL);
        memcpy(dest_addr, addr, len);
    }

    GASNETI_RUN_HANDLER_LONG(is_request, handler_idx,
            gasnetc_handler[handler_idx],
            user_token, (uint32_t*)&args[2], gasnet_nargs, dest_addr, len);

    gasnetc_leave_hander();
    return 0;
}


static void gasnetc_handler_exit2(gasnet_token_t token,
        gasnet_handlerarg_t arg0)
{
    /* Store the provided exit code and indicate this process should exit.
       psm2 can't be finalized in an AM handler, so the actual exit must
       be handled outside the psm2 progress path. */
    gasnetc_psm_state.exit_code = arg0;
    gasnetc_psm_state.should_exit = 1;
}


static volatile int32_t gasnetc_psm_counter = 0;

static void gasnetc_handler_barrier2(gasnet_token_t token,
        gasnet_handlerarg_t arg0)
{
    /* Counter is incremented during initialization to form a simple
       AM-based barrier.  Counter is decremented during shutdown for the
       same purpose. */
    gasnetc_psm_counter += arg0;
}


/* Call to increment or decrement the counter on all nodes. */
/* This barrier assumes all nodes are reachable via psm2. */
int gasnetc_psm_request_barrier_all(int value)
{
    psm2_error_t ret;
    int i;

    for(i = 0; i < gasneti_nodes; i++) {
        int dest = (gasneti_mynode + i) % gasneti_nodes;

        ret = gasnetc_AMRequestShortM(dest,
                gasneti_handleridx(gasnetc_handler_barrier2), 1, value);
        if (ret != GASNET_OK) {
            gasneti_fatalerror("AMRequestShortM (from barrier) failure: %s\n",
                    psm2_error_get_string(ret));
        }
    }

    return GASNET_OK;
}


/* -------------------------------------------------------------------------- */
GASNETI_COLD
static void gasneti_check_bug3419(void) {
    if (GASNET_OK != gasnet_barrier(gasnetc_psm_max_request_len, 0)) {
        if (!gasneti_mynode) {
            fprintf(stderr,
"***********************************************************************\n"
"* ERROR: The nodes in this run are reporting unequal values of PSM2's *\n"
"* max_request_short.  Please see \"Bug 3419\" in psm-conduit/README     *\n"
"* (source) or README-psm (installed) for more information.            *\n"
"***********************************************************************\n"
);
            fflush(stderr);
        }
        gasneti_bootstrapBarrier();
        gasneti_bootstrapFini();
        _exit(1);
    }
}

GASNETI_COLD
extern int gasnetc_attach(gasnet_handlerentry_t *table, int numentries,
        uintptr_t segsize, uintptr_t minheapoffset) {
    void *segbase = NULL;

    GASNETI_TRACE_PRINTF(C,("gasnetc_attach(table (%i entries), segsize=%"PRIuPTR", minheapoffset=%"PRIuPTR")",
                numentries, segsize, minheapoffset));

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
    if ((minheapoffset % GASNET_PAGESIZE) != 0) {
        /* round up the minheapoffset to page sz */
        minheapoffset =
            ((minheapoffset / GASNET_PAGESIZE) + 1) * GASNET_PAGESIZE;
    }
#else
    segsize = 0;
    minheapoffset = 0;
#endif

    /* adjust segsize for auxseg reqts */
    segsize = gasneti_auxseg_preattach(segsize);

    /* ---------------------------------------------------------------------- */
    /*  register handlers */
    { int i;
        for (i = 0; i < GASNETC_MAX_NUMHANDLERS; i++)
            gasnetc_handler[i] =
                (gasneti_handler_fn_t)&gasneti_defaultAMHandler;
    }
    { /*  core API handlers */
        gasnet_handlerentry_t *ctable =
            (gasnet_handlerentry_t *)gasnetc_get_handlertable();
        int len = 0;
        int numreg = 0;
        gasneti_assert(ctable);
        while (ctable[len].fnptr) len++; /* calc len */
        if (gasneti_amregister(ctable, len, 1, 63, 0, &numreg) != GASNET_OK)
            GASNETI_RETURN_ERRR(RESOURCE,"Error registering core API handlers");
        gasneti_assert(numreg == len);
    }

    { /*  extended API handlers */
        gasnet_handlerentry_t *etable =
            (gasnet_handlerentry_t *)gasnete_get_handlertable();
        int len = 0;
        int numreg = 0;
        gasneti_assert(etable);
        while (etable[len].fnptr) len++; /* calc len */
        if (gasneti_amregister(etable, len, 64, 127, 0, &numreg) != GASNET_OK)
            GASNETI_RETURN_ERRR(RESOURCE,"Error registering extended API handlers");
        gasneti_assert(numreg == len);
    }

    if (table) { /*  client handlers */
        int numreg1 = 0;
        int numreg2 = 0;

        /*  first pass - assign all fixed-index handlers */
        if (gasneti_amregister(table, numentries, 128, 255, 0, &numreg1) !=
                GASNET_OK) {
            GASNETI_RETURN_ERRR(RESOURCE,
                    "Error registering fixed-index client handlers");
        }

        /*  second pass - fill in dontcare-index handlers */
        if (gasneti_amregister(table, numentries, 128, 255, 1, &numreg2) !=
                GASNET_OK) {
            GASNETI_RETURN_ERRR(RESOURCE,
                    "Error registering variable-index client handlers");
        }

        gasneti_assert(numreg1 + numreg2 == numentries);
    }

    /* ---------------------------------------------------------------------- */
    /*  register fatal signal handlers */

    /* catch fatal signals and convert to SIGQUIT */
    gasneti_registerSignalHandlers(gasneti_defaultSignalHandler);

    /*  (###) register any custom signal handlers required by your conduit
     *        (e.g. to support interrupt-based messaging)
     */


#if HAVE_ON_EXIT
    on_exit(gasnetc_on_exit, NULL);
#else
    atexit(gasnetc_atexit);
#endif

    /* ---------------------------------------------------------------------- */
    /*  register segment  */

    gasneti_seginfo = (gasnet_seginfo_t *)gasneti_malloc(
            gasneti_nodes*sizeof(gasnet_seginfo_t));
    gasneti_leak(gasneti_seginfo);

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    if (segsize == 0) {
        segbase = NULL; /* no segment */
        segsize = 0;
    } else {
        /* (###) add code here to choose and register a segment
           (ensuring alignment across all nodes if this conduit sets
           GASNET_ALIGNED_SEGMENTS==1) you can use gasneti_segmentAttach() here
           if you used gasneti_segmentInit() above
           */
        gasneti_segmentAttach(segsize, minheapoffset,
                gasneti_seginfo, gasneti_bootstrapExchange);

        segbase = gasneti_seginfo[gasneti_mynode].addr;
        segsize = gasneti_seginfo[gasneti_mynode].size;
        gasneti_assert(((uintptr_t)segbase) % GASNET_PAGESIZE == 0);
        gasneti_assert(segsize % GASNET_PAGESIZE == 0);

#if GASNET_ALIGNED_SEGMENTS
        {
            /* Check that all segments are aligned. */
            void** all_segbases = gasneti_malloc(sizeof(void*) * gasneti_nodes);
            gasnet_node_t i;

            gasneti_bootstrapExchange(&segbase, sizeof(void*), all_segbases);
            for(i = 0; i < gasneti_nodes; i++) {
                if(all_segbases[i] != segbase) {
                    char s[255] = {0};
                    snprintf(s, sizeof(s), "Node %d segbase %p does not match remote node %d segbase %p",
                            gasneti_mynode, segbase, i, all_segbases[i]);
                    GASNETI_RETURN_ERRR(BAD_ARG, s);
                }
            }
            gasneti_free(all_segbases);
        }
#endif
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
        /* (###) add any code here needed to setup GASNET_SEGMENT_EVERYTHING support */
    }
#endif

    /* After local segment is attached, call optional client-provided hook
       should call BEFORE any conduit-specific pinning/registration of the
       segment
       */
    if (gasnet_client_attach_hook) {
        gasnet_client_attach_hook(segbase, segsize);
    }

    /* ---------------------------------------------------------------------- */
    /*  mark attach done: safe to use AMs past this point. */
    gasneti_attach_done = 1;

    /* ---------------------------------------------------------------------- */
    /*  establish psm2 connections with peers */

    /* Exchange epids */
    {
        psm2_epid_t *peer_epids;
        psm2_error_t *peer_errs;
        int *peer_mask = NULL;
        psm2_error_t ret;
        uintptr_t i;

        gasnetc_psm_state.peer_epaddrs =
                gasneti_malloc(sizeof(psm2_epaddr_t) * gasneti_nodes);
        gasneti_leak(gasnetc_psm_state.peer_epaddrs);

        peer_epids = gasneti_malloc(sizeof(psm2_epid_t) * gasneti_nodes);
        peer_errs =
                gasneti_malloc(sizeof(psm2_error_t) * gasneti_nodes);

        gasneti_bootstrapExchange(
                &gasnetc_psm_state.epid, sizeof(psm2_epid_t), peer_epids);

#if GASNET_PSHM
        /* If PSHM is enabled, don't connect to procs in the same node. */
        peer_mask = gasneti_malloc(sizeof(int) * gasneti_nodes);

        for (i = 0; i < gasneti_nodes; i++) {
            if (gasneti_pshm_in_supernode(i)) {
                peer_mask[i] = 0;
                gasnetc_psm_state.peer_epaddrs[i] = NULL;
            } else {
                peer_mask[i] = 1;
            }
        }
#endif

        ret = psm2_ep_connect(gasnetc_psm_state.ep, gasneti_nodes,
                peer_epids, peer_mask, peer_errs,
                gasnetc_psm_state.peer_epaddrs,
                (int64_t)(gasnetc_psm_state.exit_timeout*1e9));
        if(ret != PSM2_OK) {
            char s[255] = {0};
            snprintf(s, sizeof(s), "psm2_ep_connect failure: %s",
                    psm2_error_get_string(ret));
            GASNETI_RETURN_ERRR(BAD_ARG, s);
        }

        /* Stuff the GASNet node ID for each epaddr into the context pointer.
           This is used by GetMsgSource to convert an epaddr into a node ID. */
        for(i = 0; i < gasneti_nodes; i++) {
            psm2_epaddr_setctxt(gasnetc_psm_state.peer_epaddrs[i], (void*)i);
        }

        gasneti_free(peer_epids);
        gasneti_free(peer_mask);
        gasneti_free(peer_errs);

        gasnetc_psm_request_barrier_all(1);
        while(gasnetc_psm_counter < gasneti_nodes) {
            gasnetc_AMPoll();
        }

    }

    {
    int ret = gasnetc_progress_thread_init();
    if(ret != GASNET_OK)
        return ret;
    }

    /* ---------------------------------------------------------------------- */
    /*  gather segment information */

    /* (###) add code here to gather the segment assignment info into
       gasneti_seginfo on each node (may be possible to use AMShortRequest here)
       */

    /* ---------------------------------------------------------------------- */
    /*  primary attach complete */
    gasneti_bootstrapBarrier();

    GASNETI_TRACE_PRINTF(C,("gasnetc_attach(): primary attach complete"));

    gasneti_assert(gasneti_seginfo[gasneti_mynode].addr == segbase);
    gasneti_assert(gasneti_seginfo[gasneti_mynode].size == segsize);

    /* (###) exchange_fn is optional (may be NULL) and is only used with GASNET_SEGMENT_EVERYTHING
             if your conduit has an optimized bootstrapExchange pass it in place of NULL
     */
    gasneti_auxseg_attach(NULL); /* provide auxseg */

    gasnete_init(); /* init the extended API */

    gasneti_nodemapFini();

    /* ensure extended API is initialized across nodes */
    gasneti_bootstrapBarrier();

    /* Detection for bug 3419 */
    gasneti_check_bug3419();

    return GASNET_OK;
}


/* -------------------------------------------------------------------------- */

#if HAVE_ON_EXIT
GASNETI_COLD
static void gasnetc_on_exit(int exitcode, void *arg) {
    if(gasnetc_psm_state.exit_in_progress == 0) {
        gasnetc_exit(exitcode);
    }
}
#else
GASNETI_COLD
static void gasnetc_atexit(void) {
    if(gasnetc_psm_state.exit_in_progress == 0) {
        gasnetc_exit(0);
    }
}
#endif

GASNETI_COLD
extern void gasnetc_exit(int exitcode) {
    int i;
    psm2_error_t ret;

    /* once we start a shutdown, ignore all future SIGQUIT signals or we risk
     * reentrancy */
    gasneti_reghandler(SIGQUIT, SIG_IGN);

    {  /* ensure only one thread ever continues past this point */
        static gasneti_mutex_t exit_lock = GASNETI_MUTEX_INITIALIZER;
        gasneti_mutex_lock(&exit_lock);
    }

    gasnetc_psm_state.exit_in_progress = 1;

    GASNETI_TRACE_PRINTF(C,("gasnet_exit(%i)\n", exitcode));

    gasneti_flush_streams();
    gasneti_trace_finish();
    gasneti_sched_yield();

    /* Prior to attach we cannot send AMs to coordinate the exit */
    if (! gasneti_attach_done) {
        fprintf(stderr, "WARNING: GASNet psm-conduit may not shutdown cleanly when gasnet_exit() is called before gasnet_attach()\n");
        gasneti_bootstrapAbort(exitcode);
        gasneti_killmyprocess(exitcode);
    }

    /* If called from a handler, there is no way gasnetc_exit can notify other
     * processes to exit (except the one that invoked this handler).  Cleaning
     * up properly will also be difficult due to psm2 already holding locks at
     * this point.  So, just abort. */
    if (0 != gasnetc_handler_depth) {
        fprintf(stderr, "WARNING: GASNet psm-conduit may not shutdown cleanly when gasnet_exit() is called from an AM handler\n");
        gasneti_bootstrapAbort(exitcode);
        gasneti_killmyprocess(exitcode);
    }

    /* (###) add code here to terminate the job across _all_ nodes
       with gasneti_killmyprocess(exitcode) (not regular exit()), preferably
       after raising a SIGQUIT to inform the client of the exit
       */
    /* TODO: in the common (collective exit) case this is N^2 communication! */
    for(i = 1; i < gasneti_nodes; i++) {
        int j = (gasneti_mynode + i) % gasneti_nodes;
        ret = gasnetc_AMRequestShortM(j,
                gasneti_handleridx(gasnetc_handler_exit2), 1, exitcode);
        if (ret != GASNET_OK) {
            gasneti_fatalerror("AMRequestShortM (from gasnetc_exit) failure: %s\n",
                    psm2_error_get_string(ret));
        }
    }

    /* Poll for 60 seconds for at least one exit2 msg, then give up and exit. */
    const uint64_t timeout_ns = 60 * 1000000000L;
    const gasneti_tick_t t_start = gasneti_ticks_now();
    while (gasneti_ticks_to_ns(gasneti_ticks_now() - t_start) < timeout_ns) {
        if (gasnetc_psm_state.exit_in_progress) break;
        gasneti_sched_yield();
        gasnetc_AMPoll();
    }

    gasnetc_psm_state.exit_code = exitcode;
    gasnetc_do_exit();
}


/* -------------------------------------------------------------------------- */
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
        psm2_epaddr_t source_addr = NULL;
        psm2_am_get_source((psm2_am_token_t)token, &source_addr);

        *srcindex = (int)(uintptr_t)psm2_epaddr_getctxt(source_addr);
        return GASNET_OK;
    }

    gasneti_assert(sourceid < gasneti_nodes);
    *srcindex = sourceid;
    return GASNET_OK;
}

/* -------------------------------------------------------------------------- */
/*
   Active Message Request Functions
   ================================
   */

extern int gasnetc_AMRequestShortM(
        gasnet_node_t dest,       /* destination node */
        gasnet_handler_t handler, /* index into dest endpoint's handler table */
        int numargs, ...)
{
    int retval;
    va_list argptr;

    gasneti_assert(dest >= 0 && dest < gasneti_nodes);
    GASNETI_COMMON_AMREQUESTSHORT(dest,handler,numargs);
    gasneti_AMPoll(); /* (###) poll at least once, to assure forward progress */

    va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
    /* (###) If your conduit will support PSHM, let it check the dest first. */
    if_pt (gasneti_pshm_in_supernode(dest)) {
        GASNETC_PSM_PSHM_LOCK();
        retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Short, dest, handler,
                0, 0, 0,
                numargs, argptr);
        GASNETC_PSM_PSHM_UNLOCK();
    } else
#endif
    {
        int i;
        psm2_amarg_t psm2_args[9];
        psm2_error_t ret;

        /* Pack args into an array. */
        gasnet_handlerarg_t* int_args = (gasnet_handlerarg_t*)&psm2_args[1];

        /* This is a request, so set the request bit */
        psm2_args[0].u32w0 = handler | REQUEST_BIT;
        psm2_args[0].u32w1 = numargs;

        for(i = 0; i < numargs; i++) {
            int_args[i] = va_arg(argptr, gasnet_handlerarg_t);
        }


        /* numargs + 3 because two are used above, and 1 to round up for an
           odd number of user arguments. */
        GASNETC_PSM_LOCK();
        ret = psm2_am_request_short(gasnetc_psm_state.peer_epaddrs[dest],
                gasnetc_psm_state.am_handlers[AM_HANDLER_SHORT],
                psm2_args, (numargs + 3) >> 1, NULL, 0, PSM2_AM_FLAG_NONE,
                NULL, NULL);
        GASNETC_PSM_UNLOCK();
        if(ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                    psm2_error_get_string(ret));
        }

        retval = GASNET_OK;
    }

    va_end(argptr);
    GASNETI_RETURN(retval);
}


extern int gasnetc_AMRequestMediumM(
        gasnet_node_t dest,      /* destination node */
        gasnet_handler_t handler, /* index into destination endpoint's handler table */
        void *source_addr, size_t nbytes,   /* data payload */
        int numargs, ...) {
    va_list argptr;
    int retval;

    gasneti_assert(dest >= 0 && dest < gasneti_nodes);

    GASNETI_COMMON_AMREQUESTMEDIUM(dest,handler,source_addr,nbytes,numargs);
    gasneti_AMPoll(); /* (###) poll at least once, to assure forward progress */
    va_start(argptr, numargs); /*  pass in last argument */

#if GASNET_PSHM
    /* (###) If your conduit will support PSHM, let it check the dest first. */
    if_pt (gasneti_pshm_in_supernode(dest)) {
        GASNETC_PSM_PSHM_LOCK();
        retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Medium, dest, handler,
                source_addr, nbytes, 0,
                numargs, argptr);
        GASNETC_PSM_PSHM_UNLOCK();
    } else
#endif
    {
        psm2_amarg_t psm2_args[9];
        psm2_error_t ret;
        int i;

        /* Pack args into an array. */
        gasnet_handlerarg_t* int_args = (gasnet_handlerarg_t*)&psm2_args[1];

        /* This is a request, so set the request bit */
        psm2_args[0].u32w0 = handler | REQUEST_BIT;
        psm2_args[0].u32w1 = numargs;

        for(i = 0; i < numargs; i++) {
            int_args[i] = va_arg(argptr, gasnet_handlerarg_t);
        }

        /* numargs + 3 because two are used above, and 1 to round up for an
           odd number of user arguments. */
        GASNETC_PSM_LOCK();
        ret = psm2_am_request_short(gasnetc_psm_state.peer_epaddrs[dest],
                gasnetc_psm_state.am_handlers[AM_HANDLER_MED],
                psm2_args, (numargs + 3) >> 1, source_addr, nbytes,
                PSM2_AM_FLAG_NONE,
                NULL, NULL);
        GASNETC_PSM_UNLOCK();
        if(ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                    psm2_error_get_string(ret));
        }

        retval = GASNET_OK;
    }

    va_end(argptr);
    GASNETI_RETURN(retval);
}


extern int gasnetc_AMRequestLongM( gasnet_node_t dest,        /* destination node */
        gasnet_handler_t handler, /* index into destination endpoint's handler table */
        void *source_addr, size_t nbytes,   /* data payload */
        void *dest_addr,                    /* data destination on destination node */
        int numargs, ...) {
    va_list argptr;
    int retval;

    gasneti_assert(dest >= 0 && dest < gasneti_nodes);

    GASNETI_COMMON_AMREQUESTLONG(dest,handler,source_addr,nbytes,dest_addr,numargs);
    gasneti_AMPoll(); /* (###) poll at least once, to assure forward progress */
    va_start(argptr, numargs); /*  pass in last argument */

#if GASNET_PSHM
    /* (###) If your conduit will support PSHM, let it check the dest first. */
    if_pt (gasneti_pshm_in_supernode(dest)) {
        GASNETC_PSM_PSHM_LOCK();
        retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                source_addr, nbytes, dest_addr,
                numargs, argptr);
        GASNETC_PSM_PSHM_UNLOCK();
    } else
#endif
    {
        psm2_amarg_t psm2_args[10];
        psm2_error_t ret;
        int i;

        /* Pack args into an array. */
        gasnet_handlerarg_t* int_args = (gasnet_handlerarg_t*)&psm2_args[2];

        /* This is a request, so set the request bit */
        psm2_args[0].u32w0 = handler | REQUEST_BIT;
        psm2_args[0].u32w1 = numargs;
        psm2_args[1].u64w0 = (uintptr_t)dest_addr;

        for(i = 0; i < numargs; i++) {
            int_args[i] = va_arg(argptr, gasnet_handlerarg_t);
        }

        GASNETC_PSM_LOCK();
        ret = psm2_am_request_short(gasnetc_psm_state.peer_epaddrs[dest],
                gasnetc_psm_state.am_handlers[AM_HANDLER_LONG],
                psm2_args, (numargs + 5) >> 1, source_addr, nbytes,
                PSM2_AM_FLAG_NONE, NULL, NULL);
        GASNETC_PSM_UNLOCK();
        if(ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                    psm2_error_get_string(ret));
        }

        retval = GASNET_OK;
    }

    va_end(argptr);
    GASNETI_RETURN(retval);
}


extern int gasnetc_AMRequestLongAsyncM(gasnet_node_t dest, /* dest node */
        gasnet_handler_t handler, /* index into dest endpoint's handler table */
        void *source_addr, size_t nbytes,   /* data payload */
        void *dest_addr,                    /* data destination on dest node */
        int numargs, ...) {
    va_list argptr;
    int retval;

    gasneti_assert(dest >= 0 && dest < gasneti_nodes);

    GASNETI_COMMON_AMREQUESTLONGASYNC(dest,handler,source_addr,nbytes,dest_addr,numargs);
    gasneti_AMPoll(); /* (###) poll at least once, to assure forward progress */
    va_start(argptr, numargs); /*  pass in last argument */

#if GASNET_PSHM
    /* (###) If your conduit will support PSHM, let it check the dest first. */
    if_pt (gasneti_pshm_in_supernode(dest)) {
        GASNETC_PSM_PSHM_LOCK();
        retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                source_addr, nbytes, dest_addr,
                numargs, argptr);
        GASNETC_PSM_PSHM_UNLOCK();
    } else
#endif
    {
        psm2_amarg_t psm2_args[10];
        psm2_error_t ret;
        int i;

        /* Pack args into an array. */
        gasnet_handlerarg_t* int_args = (gasnet_handlerarg_t*)&psm2_args[2];

        /* This is a request, so set the request bit */
        psm2_args[0].u32w0 = handler | REQUEST_BIT;
        psm2_args[0].u32w1 = numargs;
        psm2_args[1].u64w0 = (uintptr_t)dest_addr;

        for(i = 0; i < numargs; i++) {
            int_args[i] = va_arg(argptr, gasnet_handlerarg_t);
        }

        GASNETC_PSM_LOCK();
        ret = psm2_am_request_short(gasnetc_psm_state.peer_epaddrs[dest],
                gasnetc_psm_state.am_handlers[AM_HANDLER_LONG],
                psm2_args, (numargs + 5) >> 1, source_addr, nbytes, PSM2_AM_FLAG_NONE,
                NULL, NULL);
        GASNETC_PSM_UNLOCK();
        if(ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_request_short failure: %s\n",
                    psm2_error_get_string(ret));
        }

        retval = GASNET_OK;
    }

    va_end(argptr);
    GASNETI_RETURN(retval);
}


extern int gasnetc_AMReplyShortM(
        gasnet_token_t token,     /* token provided on handler entry */
        gasnet_handler_t handler, /* index into dest endpoint's handler table */
        int numargs, ...) {
    va_list argptr;
    int retval;

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
        psm2_amarg_t psm2_args[9];
        psm2_error_t ret;
        int i;

        /* Pack args into an array. */
        gasnet_handlerarg_t* int_args = (gasnet_handlerarg_t*)&psm2_args[1];

        psm2_args[0].u32w0 = handler;
        psm2_args[0].u32w1 = numargs;

        for(i = 0; i < numargs; i++) {
            int_args[i] = va_arg(argptr, gasnet_handlerarg_t);
        }

        /* numargs + 3 because two are used above, and 1 to round up for an
           odd number of user arguments. */
        ret = psm2_am_reply_short((psm2_am_token_t)token,
                gasnetc_psm_state.am_handlers[AM_HANDLER_SHORT],
                psm2_args, (numargs + 3) >> 1, NULL, 0, PSM2_AM_FLAG_NONE,
                NULL, NULL);
        if(ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_reply_short failure: %s\n",
                    psm2_error_get_string(ret));
        }

        retval = GASNET_OK;
    }

    va_end(argptr);
    GASNETI_RETURN(retval);
}


extern int gasnetc_AMReplyMediumM(
        gasnet_token_t token,       /* token provided on handler entry */
        gasnet_handler_t handler, /* index into dest endpoint's handler table */
        void *source_addr, size_t nbytes,   /* data payload */
        int numargs, ...) {
    va_list argptr;
    int retval;

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
        psm2_amarg_t psm2_args[9];
        psm2_error_t ret;
        int i;

        /* Pack args into an array. */
        gasnet_handlerarg_t* int_args = (gasnet_handlerarg_t*)&psm2_args[1];

        psm2_args[0].u32w0 = handler;
        psm2_args[0].u32w1 = numargs;

        for(i = 0; i < numargs; i++) {
            int_args[i] = va_arg(argptr, gasnet_handlerarg_t);
        }

        /* numargs + 3 because two are used above, and 1 to round up for an
           odd number of user arguments. */
        ret = psm2_am_reply_short((psm2_am_token_t)token,
                gasnetc_psm_state.am_handlers[AM_HANDLER_MED],
                psm2_args, (numargs + 3) >> 1, source_addr, nbytes,
                PSM2_AM_FLAG_NONE,
                NULL, NULL);
        if(ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_reply_short failure: %s\n",
                    psm2_error_get_string(ret));
        }
        retval = GASNET_OK;
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
    va_list argptr;
    int retval;

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
        psm2_amarg_t psm2_args[10];
        psm2_error_t ret;
        int i;

        /* Pack args into an array. */
        gasnet_handlerarg_t* int_args = (gasnet_handlerarg_t*)&psm2_args[2];

        psm2_args[0].u32w0 = handler;
        psm2_args[0].u32w1 = numargs;
        psm2_args[1].u64w0 = (uintptr_t)dest_addr;

        for(i = 0; i < numargs; i++) {
            int_args[i] = va_arg(argptr, gasnet_handlerarg_t);
        }

        /* numargs + 3 because two are used above, and 1 to round up for an
           odd number of user arguments. */
        ret = psm2_am_reply_short((psm2_am_token_t)token,
                gasnetc_psm_state.am_handlers[AM_HANDLER_LONG],
                psm2_args, (numargs + 5) >> 1, source_addr, nbytes,
                PSM2_AM_FLAG_NONE, NULL, NULL);
        if(ret != PSM2_OK) {
            gasneti_fatalerror("psm2_am_reply_short failure: %s\n",
                    psm2_error_get_string(ret));
        }

        retval = GASNET_OK;
    }
    va_end(argptr);
    GASNETI_RETURN(retval);
}

/* ------------------------------------------------------------------------------------ */
/*
   No-interrupt sections
   =====================
   This section is only required for conduits that may use interrupt-based handler dispatch
   See the GASNet spec and http://gasnet.lbl.gov/dist/docs/gasnet.html for
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
GASNETI_COLD
extern void gasnetc_hsl_init   (gasnet_hsl_t *hsl) {
    GASNETI_CHECKATTACH();
    gasneti_mutex_init(&(hsl->lock));

#if GASNETC_USE_INTERRUPTS
    /* add code here to init conduit-specific HSL state */
#error interrupts not implemented
#endif
}

GASNETI_COLD
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
    gasneti_handler_tableentry_no_bits(gasnetc_handler_barrier2),
    gasneti_handler_tableentry_no_bits(gasnetc_handler_exit2),

    /* ptr-width dependent handlers */

    { 0, NULL }
};

gasnet_handlerentry_t const *gasnetc_get_handlertable(void) {
    return gasnetc_handlers;
}

/* ------------------------------------------------------------------------------------ */
