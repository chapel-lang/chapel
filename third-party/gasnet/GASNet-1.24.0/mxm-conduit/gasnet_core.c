/*
 * Description: GASNet MXM conduit Implementation
 * Copyright (c)  2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright (c)  2012, Mellanox Technologies LTD. All rights reserved.
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <gasnet_mxm_req.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

GASNETI_IDENT(gasnetc_IdentString_Version, "$GASNetCoreLibraryVersion: " GASNET_CORE_VERSION_STR " $");
GASNETI_IDENT(gasnetc_IdentString_Name,    "$GASNetCoreLibraryName: " GASNET_CORE_NAME_STR " $");

#if HAVE_SSH_SPAWNER
GASNETI_IDENT(gasnetc_IdentString_HaveSSHSpawner, "$GASNetSSHSpawner: 1 $");
#endif
#if HAVE_MPI_SPAWNER
GASNETI_IDENT(gasnetc_IdentString_HaveMPISpawner, "$GASNetMPISpawner: 1 $");
#endif
#if HAVE_PMI_SPAWNER
GASNETI_IDENT(gasnetc_IdentString_HavePMISpawner, "$GASNetPMISpawner: 1 $");
#endif

gasnet_handlerentry_t const *gasnetc_get_handlertable(void);
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int, void*);
#else
static void gasnetc_atexit(void);
#endif
static int gasnetc_AMPoll_nocheckattach(void);
gasneti_handler_fn_t gasnetc_handler[GASNETC_MAX_NUMHANDLERS]; /* handler table (recommended impl) */

gasnet_mxm_module_t   gasnet_mxm_module;

/*#define MXM_MUTEX_AMPOLL_LOCK*/

#ifndef GASNET_SEQ
#ifndef MXM_MUTEX_AMPOLL_LOCK
#define MXM_ATOMIC_AMPOLL_LOCK
#endif
#else
#undef MXM_MUTEX_AMPOLL_LOCK
#undef MXM_ATOMIC_AMPOLL_LOCK
#endif

#ifdef MXM_MUTEX_AMPOLL_LOCK
static gasneti_mutex_t gasnetc_ampoll_lock = GASNETI_MUTEX_INITIALIZER;
#endif
#ifdef MXM_ATOMIC_AMPOLL_LOCK
static gasneti_atomic_t gasnetc_ampoll_atomic_lock = gasneti_atomic_init(1);
#endif
/* -------------------------------------------------------------------------- */

/*
 * Exit flow suff.
 * AlexM:
 * Exit flow goes:
 * Stage1: 
 * master election. First node to contact ROOT node is nominated master. 
 * Everyone else become slaves
 *
 * Stage2:
 * master sends exit request to all nodes. If node still does not know its
 * role it becomes slave node. 
 * Master waits for acks from all nodes.
 *
 * Stage3:
 * Master sends ALL_DONE message to the root node. Master and root node can exit
 * This stage is needed because root node must be present to answer role requests
 * (there is a race between node getting its role from root or from master)
 */

enum {
    SYSTEM_EXIT_REQ = 0,
    SYSTEM_EXIT_REP = 2,
    SYSTEM_EXIT_ROLE_REQ = 3,
    SYSTEM_EXIT_ROLE_REP = 4,
    SYSTEM_EXIT_ALL_DONE = 5,
};

/* root node for exit flow coordination */
#define GASNETC_EXIT_FLOW_ROOT_NODE 0

enum {
    GASNETC_EXIT_ROLE_UNKNOWN,
    GASNETC_EXIT_ROLE_MASTER,
    GASNETC_EXIT_ROLE_SLAVE
};

static int gasnetc_exit_head(int exitcode);
static void gasnetc_exit_body(void);
static void gasnetc_exit_tail(void) GASNETI_NORETURN;
static void gasnetc_exit_now(int) GASNETI_NORETURN;
static int gasnetc_SystemRequest(gasnet_node_t dest, int numargs, ...);
static int gasnetc_SystemReply(gasnet_token_t token, int numargs, ...);

/* Exit coordination timeouts */
#define GASNETC_DEFAULT_EXITTIMEOUT_MAX		360.0  /* 6 minutes! */
#define GASNETC_DEFAULT_EXITTIMEOUT_MIN		2      /* 2 seconds */
#define GASNETC_DEFAULT_EXITTIMEOUT_FACTOR	0.25   /* 1/4 second */
static double gasnetc_exittimeout = GASNETC_DEFAULT_EXITTIMEOUT_MAX;

#if GASNET_SEGMENT_FAST
/* Limit on size of pre-pinned regions */
#define GASNETC_DEFAULT_PIN_MAXSZ		0
int          gasnetc_seg_reg_count;
int          gasnetc_max_regs;
uintptr_t    gasnetc_seg_start;
uintptr_t    gasnetc_seg_end;
uint64_t     gasnetc_pin_maxsz;
unsigned int gasnetc_pin_maxsz_shift;
#endif

#ifndef GASNETI_HAVE_ATOMIC_CAS
#error "required atomic compare-and-swap is not yet implemented for your CPU/OS/compiler"
#endif

/* Exit value. value to _exit() with */
static gasneti_atomic_t gasnetc_exit_code = gasneti_atomic_init(0);
/* Counter for the number of exit requests from the root node - 0 or 1 only */
static gasneti_atomic_t gasnetc_exit_reqs = gasneti_atomic_init(0);
/* Counter for the number of exit replies the root node got */
static gasneti_atomic_t gasnetc_exit_reps = gasneti_atomic_init(0);
/* Flag to show whether exit coordination is done */
static gasneti_atomic_t gasnetc_exit_done = gasneti_atomic_init(0);
/* Exit role of the node (master or slave) */
static gasneti_atomic_t gasnetc_exit_role = gasneti_atomic_init(GASNETC_EXIT_ROLE_UNKNOWN);

/* exit roles requested vs received replies */
static gasneti_atomic_t gasnetc_exit_role_rep_out = gasneti_atomic_init(0);
/* notification to the root node from elected master */
static gasneti_atomic_t gasnetc_exit_all_done = gasneti_atomic_init(0);

/* -------------------------------------------------------------------------- */

void (*gasneti_bootstrapFini_p)(void) = NULL;
void (*gasneti_bootstrapAbort_p)(int exitcode) = NULL;
void (*gasneti_bootstrapBarrier_p)(void) = NULL;
void (*gasneti_bootstrapExchange_p)(void *src, size_t len, void *dest) = NULL;
void (*gasneti_bootstrapAlltoall_p)(void *src, size_t len, void *dest) = NULL;
void (*gasneti_bootstrapBroadcast_p)(void *src, size_t len, void *dest, int rootnode) = NULL;
void (*gasneti_bootstrapCleanup_p)(void) = NULL;

#if GASNET_TRACE
static unsigned int	gasnetc_pinned_blocks = 0;
static size_t		gasnetc_pinned_bytes = 0;
#endif

/* -------------------------------------------------------------------------- */
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
    /* using internal function instead */
    gasneti_bootstrapBarrier();
}


/* -------------------------------------------------------------------------- */

size_t gasneti_AMMaxMedium(void)
{
    /*return gasnet_mxm_module.max_am_med -*/
    /*          (sizeof(gasnet_handlerarg_t) * GASNETC_MAX_ARGS));*/
    return gasnet_mxm_module.max_am_med;
}

/* -------------------------------------------------------------------------- */

static int gasneti_bootstrapInit(
    int *argc_p, char ***argv_p,
    gasnet_node_t *nodes_p,
    gasnet_node_t *mynode_p)
{
    char *spawner = gasneti_getenv_withdefault("GASNET_IB_SPAWNER", "(not set)");
    int res = GASNET_ERR_NOT_INIT;

#if HAVE_SSH_SPAWNER
    /* Sigh.  We can't assume GASNET_IB_SPAWNER has been set except in the master.
     * However, gasneti_bootstrapInit_ssh() verifies the command line args and
     * returns GASNET_ERR_NOT_INIT on failure witout any noise on stderr.
     * So, we try ssh-based spawn first.
     */
    if (GASNET_OK != res &&
        GASNET_OK == (res = gasneti_bootstrapInit_ssh(argc_p, argv_p, nodes_p, mynode_p))) {
        gasneti_bootstrapFini_p     = &gasneti_bootstrapFini_ssh;
        gasneti_bootstrapAbort_p    = &gasneti_bootstrapAbort_ssh;
        gasneti_bootstrapBarrier_p  = &gasneti_bootstrapBarrier_ssh;
        gasneti_bootstrapExchange_p = &gasneti_bootstrapExchange_ssh;
        gasneti_bootstrapAlltoall_p = &gasneti_bootstrapAlltoall_ssh;
        gasneti_bootstrapBroadcast_p= &gasneti_bootstrapBroadcast_ssh;
        gasneti_bootstrapCleanup_p  = &gasneti_bootstrapCleanup_ssh;
    }
#endif

#if HAVE_MPI_SPAWNER
    /* Only try MPI-based spawn when spawner == "mpi".
     * Otherwise things could hang or fail in "messy" ways here.
     */
    if (GASNET_OK != res && !strcmp(spawner, "mpi") && 
        GASNET_OK == (res = gasneti_bootstrapInit_mpi(argc_p, argv_p, nodes_p, mynode_p))) {
        gasneti_bootstrapFini_p	    = &gasneti_bootstrapFini_mpi;
        gasneti_bootstrapAbort_p	= &gasneti_bootstrapAbort_mpi;
        gasneti_bootstrapBarrier_p	= &gasneti_bootstrapBarrier_mpi;
        gasneti_bootstrapExchange_p	= &gasneti_bootstrapExchange_mpi;
        gasneti_bootstrapAlltoall_p	= &gasneti_bootstrapAlltoall_mpi;
        gasneti_bootstrapBroadcast_p= &gasneti_bootstrapBroadcast_mpi;
        gasneti_bootstrapCleanup_p  = &gasneti_bootstrapCleanup_mpi;
    }
#endif

#if HAVE_PMI_SPAWNER
    /* Don't expect GASNET_IB_SPAWNER set if launched directly by srun, mpirun, yod, etc.
     * So, we try pmi-based spawn last.
     */
    if (GASNET_OK != res &&
        GASNET_OK == (res = gasneti_bootstrapInit_pmi(argc_p, argv_p, nodes_p, mynode_p))) {
        gasneti_bootstrapFini_p = &gasneti_bootstrapFini_pmi;
        gasneti_bootstrapAbort_p    = &gasneti_bootstrapAbort_pmi;
        gasneti_bootstrapBarrier_p  = &gasneti_bootstrapBarrier_pmi;
        gasneti_bootstrapExchange_p = &gasneti_bootstrapExchange_pmi;
        gasneti_bootstrapAlltoall_p = &gasneti_bootstrapAlltoall_pmi;
        gasneti_bootstrapBroadcast_p= &gasneti_bootstrapBroadcast_pmi;
        gasneti_bootstrapCleanup_p  = &gasneti_bootstrapCleanup_pmi;
    }
#endif

    if (GASNET_OK != res
#if HAVE_SSH_SPAWNER
        && strcmp(spawner, "ssh")
#endif
#if HAVE_MPI_SPAWNER
        && strcmp(spawner, "mpi")
#endif
#if HAVE_PMI_SPAWNER
        && strcmp(spawner, "pmi")
#endif
        )
    {
        gasneti_fatalerror("Requested spawner \"%s\" is unknown or not supported in this build", spawner);
    }

    return res;
}


/* -------------------------------------------------------------------------- */

#if MXM_API < MXM_VERSION(2,0)
static int
gasneti_mxm_get_ep_address(gasnet_mxm_ep_conn_info_t *ep_info, mxm_ptl_id_t id)
{
    size_t addrlen;
    mxm_error_t mxm_status;

    addrlen = sizeof(ep_info->ptl_addr[id]);
    mxm_status = mxm_ep_address(gasnet_mxm_module.mxm_ep, id,
                                (struct sockaddr *)&ep_info->ptl_addr[id],
                                &addrlen);
    if (mxm_status != MXM_OK)
        MXM_ERROR("Unable to extract endpoint address (%s)\n",
                  mxm_error_string(mxm_status));
    return mxm_status;
}
#endif

/* -------------------------------------------------------------------------- */

#if GASNET_SEGMENT_FAST
/*
 * Set gasnetc_pin_maxsz_shift while rounding
 * gasnetc_pin_maxsz down to a power of two
 */
static void setup_pin_maxsz(uint64_t size)
{
    MXM_DEBUG("Starting with size=%lu, gasnetc_pin_maxsz=%lu\n", size, gasnetc_pin_maxsz);
    gasneti_assert(size != 0);
    size >>= 1;
    for (gasnetc_pin_maxsz_shift=0; size != 0; ++gasnetc_pin_maxsz_shift) {
        size >>= 1;
    }
    gasnetc_pin_maxsz = ((uint64_t)1) << gasnetc_pin_maxsz_shift;
    MXM_DEBUG("Rounded down to power of two alculated gasnetc_pin_maxsz=%lu\n",
              gasnetc_pin_maxsz);
}
#endif

/* -------------------------------------------------------------------------- */

/* Process defaults and the environment to get configuration settings */
static int gasneti_load_settings(void)
{
    const char *tmp;

#define GASNETC_ENVINT(program_var, env_key, default_val, minval, is_mem) do {     \
        int64_t _tmp = gasneti_getenv_int_withdefault(#env_key, default_val, is_mem);  \
        if (_tmp < minval)                                                             \
            GASNETI_RETURN_ERRR(BAD_ARG, "("#env_key" < "#minval") in environment");     \
        program_var = _tmp;                                                            \
     } while (0)

#if GASNET_SEGMENT_FAST
    GASNETC_ENVINT(gasnetc_pin_maxsz, GASNET_PIN_MAXSZ, GASNETC_DEFAULT_PIN_MAXSZ, 0, 1);
    if (!gasnetc_pin_maxsz) {
        /* 0=automatic.  Will setup later */
    } else if (!GASNETI_POWEROFTWO(gasnetc_pin_maxsz)) {
        gasneti_fatalerror("GASNET_PIN_MAXSZ (%llu) is not a power of 2", (unsigned long long) gasnetc_pin_maxsz);
    } else if (gasnetc_pin_maxsz < GASNET_PAGESIZE) {
        gasneti_fatalerror("GASNET_PIN_MAXSZ (%lu) is less than GASNET_PAGESIZE (%lu)",
                           (unsigned long) gasnetc_pin_maxsz, (unsigned long) GASNET_PAGESIZE);
    } else {
        setup_pin_maxsz(gasnetc_pin_maxsz);
    }
#endif

#if GASNET_PSHM
    /* Max Medium is compile-time constant */
    gasnet_mxm_module.max_am_med = GASNETC_MAX_MEDIUM;
#else
    GASNETC_ENVINT(gasnet_mxm_module.max_am_med, GASNET_AM_MAX_MED,
                   GASNETC_DEFAULT_AM_MAX_MED, 0, 1);
    if (!gasnet_mxm_module.max_am_med) {
        gasnet_mxm_module.max_am_med = GASNETC_DEFAULT_AM_MAX_MED;
    } else if (gasnet_mxm_module.max_am_med < 512) {
        gasneti_fatalerror("GASNET_AM_MAX_MED (%lu) is less than 512",
                           (unsigned long) gasnet_mxm_module.max_am_med);
    }
    MXM_DEBUG("GASNET_AM_MAX_MED is set to %lu\n", gasnet_mxm_module.max_am_med);
#endif

    GASNETC_ENVINT(gasnet_mxm_module.strict_api, GASNET_STRICT_API, 1, 0, 1);
    MXM_DEBUG("GASNET_STRICT_API is set to %u\n", gasnet_mxm_module.strict_api);

    /* Report */
    GASNETI_TRACE_PRINTF(I, ("mxm-conduit run time configuration settings = {"));

#if GASNET_SEGMENT_FAST
    GASNETI_TRACE_PRINTF(I, ("  GASNET_PIN_MAXSZ                = %lu%s",
                             (unsigned long) gasnetc_pin_maxsz,
                             (!gasnetc_pin_maxsz ? " (automatic)" : "")));
#endif

#if !GASNET_PSHM
    GASNETI_TRACE_PRINTF(I, ("  GASNET_AM_MAX_MED               = %lu",
                             (unsigned long) gasnet_mxm_module.max_am_med));
#endif

    GASNETI_TRACE_PRINTF(I, ("  GASNET_STRICT_API               = %u",
                             (unsigned) gasnet_mxm_module.strict_api));

    GASNETI_TRACE_PRINTF(I, ("}"));
    gasnetc_exittimeout = gasneti_get_exittimeout(
                              GASNETC_DEFAULT_EXITTIMEOUT_MAX,
                              GASNETC_DEFAULT_EXITTIMEOUT_MIN,
                              GASNETC_DEFAULT_EXITTIMEOUT_FACTOR,
                              GASNETC_DEFAULT_EXITTIMEOUT_MIN);
    return GASNET_OK;
}

/* -------------------------------------------------------------------------- */

/* Maximum pinning capabilities */
typedef struct gasnetc_pin_info_t_ {
    uintptr_t    memory;    /* How much pinnable (per proc) */
    /*uint32_t   regions;*/
    int          ppn;       /* How many procs on the same host */
} gasnetc_pin_info_t;
static gasnetc_pin_info_t gasnetc_pin_info;

/* -------------------------------------------------------------------------- */

#if GASNET_ALIGNED_SEGMENTS  /* Unused otherwise */
/* Reproduce the mmap()/munmap() steps to keep compatible VM spaces */
static void gasnetc_fakepin(uintptr_t limit, uintptr_t step)
{
    if (limit != 0) {
        void *addr;
        step = MIN(limit, step);
        addr = gasneti_mmap(step);
        if (addr != MAP_FAILED) {
            gasnetc_fakepin(limit - step, step);
            gasneti_munmap(addr, step);
        }
    }
}
#endif

/* -------------------------------------------------------------------------- */

extern int gasnetc_pin(void *addr, size_t size, gasnetc_memreg_t *reg)
{
    mxm_error_t res;
    gasneti_assert(((uintptr_t)addr % GASNET_PAGESIZE) == 0);
    gasneti_assert(((uintptr_t)size % GASNET_PAGESIZE) == 0);

#if MXM_API >= MXM_VERSION(2,0)
    res = mxm_mem_map(gasnet_mxm_module.mxm_context, &addr, &size, 0, 0, 0); 
#elif MXM_API < MXM_VERSION(1,5)
    res = mxm_reg_mr(gasnet_mxm_module.mxm_ep, MXM_PTL_RDMA,
                     addr, size, &reg->lkey, &reg->rkey);
#elif MXM_API == MXM_VERSION(1,5)
    res = mxm_mem_register(gasnet_mxm_module.mxm_context,
                           addr, size, &reg->memh);
#endif

    if_pf (MXM_OK != res)
        return 1;

    reg->addr     = (uintptr_t)addr;
    reg->len      = size;
    reg->end      = (uintptr_t)addr + (size - 1);
#if MXM_API >= MXM_VERSION(2,0)
    mxm_mem_get_key(gasnet_mxm_module.mxm_context, addr, &reg->m_key);
#endif

#if GASNET_TRACE
    gasnetc_pinned_blocks += 1;
    gasnetc_pinned_bytes += reg->len;
#endif

    return 0;
}

/* -------------------------------------------------------------------------- */

static void gasnetc_unpin(gasnetc_memreg_t *reg)
{
#if MXM_API >= MXM_VERSION(2,0)
    if_pf (MXM_OK != mxm_mem_unmap(gasnet_mxm_module.mxm_context, (void *)reg->addr, reg->len, 0)) {
        MXM_ERROR("Failed deregistering memory region\n");
    }

#elif MXM_API < MXM_VERSION(1,5)
    if_pf (MXM_OK != mxm_dereg_mr(gasnet_mxm_module.mxm_ep, MXM_PTL_RDMA,
                                  (void *)reg->addr, reg->len)) {
        MXM_ERROR("Failed deregistering memory region\n");
    }
#elif MXM_API == MXM_VERSION(1,5)
    mxm_mem_destroy(reg->memh);
#endif

#if GASNET_TRACE
    gasnetc_pinned_blocks -= 1;
    gasnetc_pinned_bytes -= reg->len;
#endif
}

/* -------------------------------------------------------------------------- */

static void *gasnetc_try_pin_inner(size_t size, gasnetc_memreg_t *reg)
{
    void *addr;

    addr = gasneti_mmap(size);
    if (addr != MAP_FAILED) {
        /*MXM_DEBUG("MMap for size %lu succeeded, trying to register\n", size);*/
        if (gasnetc_pin(addr, size, reg)) {
            MXM_DEBUG("Registering for size %lu failed\n", size);
            gasnetc_unpin(reg);
            gasneti_munmap(addr, size);
            return NULL;
        } else {
            /*MXM_DEBUG("Registering for size %lu succeeded\n", size);*/
        }
    } else {
        MXM_DEBUG("MMap for size %lu failed\n", size);
        addr = NULL;
    }
    return addr;
}

/* -------------------------------------------------------------------------- */


/* Try to pin up to 'limit' in chunks no larger than size 'step' */
static uintptr_t gasnetc_trypin(uintptr_t limit, uintptr_t step)
{
    while (limit >= GASNETI_MMAP_GRANULARITY) {
        gasnetc_memreg_t reg;
        /* step non-zero means try linear growth, else bisection */
        uintptr_t size = step ? MIN(limit, step) : limit;
        uintptr_t half = GASNETI_PAGE_ALIGNDOWN(size / 2);
        if (gasnetc_try_pin_inner(size, &reg) != NULL) {
            /* Success - recurse to try another chunk */
            size += gasnetc_trypin(step ? (limit - size) : half, step);
            gasnetc_unpin(&reg);
            gasnetc_unmap(&reg);
            return size;
        }
        limit = half;
        step = 0;
    }

    return 0;
}

/* -------------------------------------------------------------------------- */

static void gasnetc_init_pin_info(int first_local, int ppn)
{
    gasnetc_pin_info_t *all_info = gasneti_malloc(gasneti_nodes * sizeof(gasnetc_pin_info_t));
    unsigned long limit;
    int i;
    unsigned long tmp;

    /*
     * We bound our search by the smallest of:
     *   - 2/3 of physical memory
     *   - env(GASNET_PHYSMEM_MAX)
     */
    MXM_DEBUG("Initializing pinning info\n");
    limit = 2 * (gasneti_getPhysMemSz(1) / 3);
    MXM_LOG("Physical memory size: %lu, limit: %lu\n", gasneti_getPhysMemSz(1), limit);

    /* Honor PHYSMEM_MAX if set */
    tmp = gasneti_getenv_int_withdefault("GASNET_PHYSMEM_MAX", 0, 1);
    if (tmp) {
        MXM_DEBUG("GASNET_PHYSMEM_MAX is set to %lu\n", tmp);
        limit = tmp;
        MXM_DEBUG("Updated limit: %lu\n", limit);
    } else {
        MXM_DEBUG("GASNET_PHYSMEM_MAX is not set\n");
    }

    limit = GASNETI_PAGE_ALIGNDOWN(limit);
    MXM_DEBUG("Limit aligned down to page size: %lu\n", limit);
    if (limit == 0) {
        gasneti_fatalerror("Failed to determine the available physical memory");
        return;
    }

    gasnetc_pin_info.memory = ~((uintptr_t)0);
    gasnetc_pin_info.ppn = ppn;

    if (gasneti_getenv_yesno_withdefault("GASNET_PHYSMEM_PROBE", 0)) {
        /* Now search for largest pinnable memory, on one process per machine */
        unsigned long step = GASNETI_MMAP_GRANULARITY;
        MXM_DEBUG("GASNET_PHYSMEM_NOPROBE is set\n");
        MXM_DEBUG("Step set to %lu (GASNETI_MMAP_GRANULARITY)\n", step);
#if GASNET_SEGMENT_FAST
        if (step > gasnetc_pin_maxsz) {
            step = gasnetc_pin_maxsz;
            MXM_DEBUG("GASNET_SEGMENT_FAST: Step decreased to %lu (gasnetc_pin_maxsz)\n", step);
        }
#endif
        step = GASNETI_PAGE_ALIGNDOWN(step);
        if (gasneti_mynode == first_local) {
            uintptr_t size;
            MXM_DEBUG("First local node - probing pinning: limit=%lu, step=%lu\n",
                      limit, step);
            size = gasnetc_trypin(limit, step);
            if_pf (!size) {
                gasneti_fatalerror("ERROR: Failure to determine the max pinnable memory.  " GASNET_CONDUIT_NAME_STR " may be misconfigured.");
            }
            gasnetc_pin_info.memory = size;
            MXM_DEBUG("Done probing - using size %lu\n", size);
        } else {
            MXM_DEBUG("Not first local node - no pinning probing\n");
        }

        gasneti_bootstrapExchange(&gasnetc_pin_info,
                                  sizeof(gasnetc_pin_info_t),
                                  all_info);

#if GASNET_ALIGNED_SEGMENTS  /* Just a waste of time otherwise */
        if (gasneti_mynode != first_local) {
            /* Extra mmap traffic to ensure compatible VM spaces */
            gasnetc_fakepin(all_info[first_local].memory, step);
        }
        gasneti_bootstrapBarrier(); /* Ensure fakepin completes unmap before continuing */
#endif
    } else {
        MXM_LOG("No pinning probing required - using size %lu\n", limit);
        /* Note that README says PHYSMEM_NOPROBE must be equal on all nodes */
        gasnetc_pin_info.memory = limit;
        gasneti_bootstrapExchange(&gasnetc_pin_info,
                                  sizeof(gasnetc_pin_info_t),
                                  all_info);
    }

    /* Determine the global values (min of maxes) from the local values */
    for (i = 0; i < gasneti_nodes; i++) {
        gasnetc_pin_info_t *info = &all_info[i];
        info->memory = GASNETI_PAGE_ALIGNDOWN(info->memory / info->ppn);
        gasnetc_pin_info.memory  = MIN(gasnetc_pin_info.memory, info->memory);
    }

    MXM_LOG("Using max pinnable memory: %lu\n", gasnetc_pin_info.memory);
    gasneti_free(all_info);
}

/* -------------------------------------------------------------------------- */

static inline int gasnetc_post_recv(gasnet_mxm_recv_req_t *r_req)
{
    mxm_recv_req_t * p_req = &r_req->mxm_rreq;
    mxm_req_base_t * p_base = &r_req->mxm_rreq.base;
    p_req->tag = 0;
    p_req->tag_mask = 0; /* match any tag */
    p_base->completed_cb = NULL;
    p_base->conn = NULL; /* receive from any source */
    p_base->mq = gasnet_mxm_module.mxm_mq;
    p_base->state = MXM_REQ_NEW;
    p_base->data_type = MXM_REQ_DATA_BUFFER;
#if MXM_API < MXM_VERSION(1,5)
    p_base->flags = 0;
    p_base->data.buffer.mkey = MXM_MKEY_NONE;
#elif MXM_API == MXM_VERSION(1,5)
    p_base->flags = 0;
    p_base->data.buffer.memh = NULL;
#endif
    return mxm_req_recv(&r_req->mxm_rreq);
}

/* -------------------------------------------------------------------------- */

static int gasnetc_init(int *argc, char ***argv)
{
#if MXM_API < MXM_VERSION(1,5)
    struct sockaddr_mxm_local_proc  sockaddr_bind_self;
    struct sockaddr_mxm_ib_local    sockaddr_bind_rdma;
    mxm_ep_opts_t                   mxm_ep_opts;
    mxm_context_opts_t              mxm_opts;
#else
    mxm_ep_opts_t                 * mxm_ep_opts;
    mxm_context_opts_t            * mxm_opts;
#endif
    gasnet_mxm_ep_conn_info_t       local_ep;
    mxm_error_t                     mxm_status;
    int                             i;
    uint32_t                        res;
#if MXM_API >= MXM_VERSION(2,0)
    size_t                          mxm_addr_len;
#endif

    uint32_t jobid = 0;
    unsigned long cur_ver;
    gasnet_mxm_recv_req_t *r, *prev_r;

    /*  check system sanity */
    gasnetc_check_config();

    if (gasneti_init_done)
        GASNETI_RETURN_ERRR(NOT_INIT, "GASNet already initialized");

    gasneti_freezeForDebugger();

    memset(&gasnet_mxm_module, 0, sizeof(gasnet_mxm_module));

#if GASNET_DEBUG_VERBOSE
    /* note - can't call trace macros during gasnet_init because trace system not yet initialized */
    fprintf(stderr,"gasnetc_init(): about to spawn...\n");
    fflush(stderr);
#endif

    /* (###) add code here to bootstrap the nodes for your conduit */

    /*
     * Using existing bootstrap init via SSH.
     * This also initializes gasneti_nodes and gasneti_mynode globals.
     */
    res = gasneti_bootstrapInit(argc, argv, &gasneti_nodes, &gasneti_mynode);
    if (res != GASNET_OK) {
        return res;
    }

    gasneti_init_done = 1; /* enable early to allow tracing */

    /* Now enable tracing of all the following steps */
    gasneti_trace_init(argc, argv);

    /*
     * Process the environment for configuration/settings.
     */
    res = gasneti_load_settings();
    if (res != GASNET_OK) {
        return res;
    }

    /*
     * Initialize MXM
     *
     * There are three modes:
     *   - MXM_ASYNC_MODE_NONE
     *   - MXM_ASYNC_MODE_SIGNAL
     *   - MXM_ASYNC_MODE_THREAD
     * We need a progress thread or at least signal - otherwise AM won't work.
     */

    cur_ver = mxm_get_version();
    if (cur_ver != MXM_API) 
    {
        MXM_ERROR("Current MXM version %ld.%ld does not match to MXM version %d.%d UPC was compiled with.\n", (cur_ver >> MXM_MAJOR_BIT)& 0xff, (cur_ver >> MXM_MINOR_BIT) & 0xff, MXM_VERNO_MAJOR, MXM_VERNO_MINOR);
        return GASNET_ERR_NOT_INIT;
    }

#if MXM_API < MXM_VERSION(1,5)
    mxm_fill_context_opts(&mxm_opts);
    mxm_opts.async_mode = MXM_ASYNC_MODE_THREAD;
    mxm_status = mxm_init(&mxm_opts, &gasnet_mxm_module.mxm_context);
#else 
#if MXM_API < MXM_VERSION(2,1)
    res = mxm_config_read_context_opts(&mxm_opts);
#else
    res = mxm_config_read_opts(&mxm_opts, &mxm_ep_opts, "GASNET", NULL, 0);
#endif
    if_pf (res != MXM_OK) {
        MXM_ERROR("Failed to parse MXM configuration");
        return GASNET_ERR_NOT_INIT;
    }
    mxm_opts->async_mode = MXM_ASYNC_MODE_THREAD;
#if MXM_API == MXM_VERSION(1,5)
    mxm_opts->ptl_bitmap = MXM_BIT(MXM_PTL_SELF) | MXM_BIT(MXM_PTL_RDMA);
#endif
    mxm_status = mxm_init(mxm_opts, &gasnet_mxm_module.mxm_context);
#endif

    if (mxm_status != MXM_OK) {
        if (mxm_status == MXM_ERR_NO_DEVICE)
            MXM_ERROR("No supported device found, disqualifying MXM\n");
        else
            MXM_ERROR("MXM init failed (%s)\n", mxm_error_string(mxm_status));
        return GASNET_ERR_NOT_INIT;
    }

#if (0)
#if GASNET_DEBUG_VERBOSE
    {
        /*
         * set MXM verbosity level
         */
        mxm_global_opts_t  mxm_global_opts;
        mxm_fill_global_opts(&mxm_global_opts);
        mxm_global_opts.log_level = MXM_LOG_LEVEL_INFO;
        mxm_global_opts.debug_signo = SIGUSR1;
        mxm_global_opts.log_buffer_size = 1024;
        mxm_set_global_opts(&mxm_global_opts);
    }
#endif
#endif
    jobid = gasneti_mynode;

    /*
     * Setup the endpoint options and local addresses to bind to
     */

#if MXM_API < MXM_VERSION(1,5)
    sockaddr_bind_self.sa_family = AF_MXM_LOCAL_PROC;
    sockaddr_bind_self.context_id = jobid;
    sockaddr_bind_self.process_id = getpid();

    sockaddr_bind_rdma.sa_family = AF_MXM_IB_LOCAL;
    sockaddr_bind_rdma.lid = 0;
    sockaddr_bind_rdma.pkey = 0;
    sockaddr_bind_rdma.qp_num = 0;
    sockaddr_bind_rdma.sl = 0;

    /*
     * Initialize MXM endpoint
     */

    memset(&mxm_ep_opts, 0, sizeof(mxm_ep_opts_t));
    mxm_fill_ep_opts(&mxm_ep_opts);

    mxm_ep_opts.ptl_bind_addr[MXM_PTL_SELF] = (struct sockaddr*) &sockaddr_bind_self;
    mxm_ep_opts.ptl_bind_addr[MXM_PTL_RDMA] = (struct sockaddr*) &sockaddr_bind_rdma;

    mxm_status = mxm_ep_create(gasnet_mxm_module.mxm_context,
                               &mxm_ep_opts, &gasnet_mxm_module.mxm_ep);
#else
#if MXM_API < MXM_VERSION(2,1)
    mxm_status = mxm_config_read_ep_opts(&mxm_ep_opts);
    if (mxm_status != MXM_OK) {
        MXM_ERROR("Failed to parse MXM configuration (%s)\n",
                  mxm_error_string(mxm_status));
        return GASNET_ERR_NOT_INIT;
    }
#endif
#if MXM_API == MXM_VERSION(1,5)
    mxm_ep_opts->job_id = jobid;
    mxm_ep_opts->local_rank = gasneti_nodemap_local_rank;
    mxm_ep_opts->num_local_procs = gasneti_nodemap_local_count;
#endif
    mxm_status = mxm_ep_create(gasnet_mxm_module.mxm_context,
                        mxm_ep_opts, &gasnet_mxm_module.mxm_ep);
#endif

    if (mxm_status != MXM_OK) {
        MXM_ERROR("Unable to create MXM endpoint (%s)\n",
                  mxm_error_string(mxm_status));
        return GASNET_ERR_NOT_INIT;
    }

#if MXM_API < MXM_VERSION(1,5)
    gasnet_mxm_module.zcopy_thresh = mxm_ep_opts.rdma.zcopy_thresh;
#elif MXM_API == MXM_VERSION(1,5) 
    gasnet_mxm_module.zcopy_thresh = mxm_ep_opts->rdma.zcopy_thresh;
    mxm_config_free(mxm_ep_opts);
#else
    mxm_config_free_ep_opts(mxm_ep_opts);
#endif

    /*
     * Get address for each PTL on this endpoint and exchange it
     */
#if MXM_API < MXM_VERSION(2,0)
    if (MXM_OK != gasneti_mxm_get_ep_address(&local_ep, MXM_PTL_SELF))
        return GASNET_ERR_NOT_INIT;
    if (MXM_OK != gasneti_mxm_get_ep_address(&local_ep, MXM_PTL_RDMA))
        return GASNET_ERR_NOT_INIT;
#else
    mxm_addr_len = sizeof(local_ep.ep_addr);
    if (MXM_OK != (mxm_status = mxm_ep_get_address(gasnet_mxm_module.mxm_ep, local_ep.ep_addr, &mxm_addr_len))) {
        MXM_ERROR("Unable to extract endpoint address (%s)\n",
                                  mxm_error_string(mxm_status));
        return GASNET_ERR_NOT_INIT;
    }
#endif
    /*
     * MXM fills in addresses for MXM_PTL_SELF and MXM_PTL_RDMA.
     * We don't care about the rest - tell valgrind to ignore it.
     */
    /*VALGRIND_MAKE_MEM_DEFINED(&local_ep, sizeof (local_ep));*/

    gasnet_mxm_module.remote_eps =
        gasneti_calloc(gasneti_nodes, sizeof(local_ep));

    gasneti_bootstrapExchange(&local_ep, sizeof (local_ep),
                              gasnet_mxm_module.remote_eps);

    /*
     * Allocate pool of receive requests
     */

    GASNETC_ENVINT(gasnet_mxm_module.am_max_depth, GASNET_NETWORKDEPTH, 16, 1, 1);

    gasnet_mxm_module.am_recv_pool = gasneti_malloc(gasnet_mxm_module.am_max_depth*sizeof(gasnet_mxm_recv_req_t*));

    prev_r = NULL;
    for (i = 0; i < gasnet_mxm_module.am_max_depth; i++) {
        r = gasnetc_alloc_recv_req();
        gasnet_mxm_module.am_recv_pool[i] = r;
        r->next = prev_r;
        prev_r = r;
    }
    gasnet_mxm_module.am_recv_tail = gasnet_mxm_module.am_recv_pool[0];
    gasnet_mxm_module.am_recv_head = gasnet_mxm_module.am_recv_pool[gasnet_mxm_module.am_max_depth-1];

    /* (###) Add code here to determine which GASNet nodes may share memory.
    The collection of nodes sharing memory are known as a "supernode".
    The (first) data structure to describe this is gasneti_nodemap[]:
      For all i: gasneti_nodemap[i] is the lowest node number collocated w/ node i
    where nodes are considered collocated if they have the same node "ID".
    Or in English:
     "gasneti_nodemap[] maps from node to first node on the same supernode."

    If the conduit has already communicated endpoint address information or
    a similar identifier that is unique per shared-memory compute node, then
    that info can be passed via arguments 2 through 4.
    Otherwise the conduit should pass a non-null gasnetc_bootstrapExchange
    as argument 1 to use platform-specific IDs, such as gethostid().
    See gasneti_nodemapInit() in gasnet_internal.c for more usage documentation.
    See below for info on gasnetc_bootstrapExchange()

    If the conduit can build gasneti_nodemap[] w/o assistance, it should
    call gasneti_nodemapParse() after constructing it (instead of nodemapInit()).
    */

    gasneti_nodemapInit(&gasneti_bootstrapExchange, NULL, 0, 0);

#if GASNET_PSHM
    /* (###) If your conduit will support PSHM, you should initialize it here.
     * The 1st argument is normally "&gasnetc_bootstrapExchange" (described below).
     * The 2nd argument is the amout of shared memory space needed for any
     * conduit-specific uses.  The return value is a pointer to the space
     * requested by the 2nd argument.
     */
    /*
     * ### = gasneti_pshm_init(###, ###);
     */
    gasneti_pshm_init(&gasneti_bootstrapExchange, 0);
#endif

    /*
     * Determine gasnetc_max_msg_sz and dependent variables
     */
#if GASNET_SEGMENT_FAST
    {
        uintptr_t gasnetc_max_msg_sz = GASNETC_MXM_MAX_MSG_SIZE;
        if (!gasnetc_pin_maxsz || (gasnetc_pin_maxsz > gasnetc_max_msg_sz)) {
            uint64_t orig = gasnetc_pin_maxsz;
            char newval[16];

            setup_pin_maxsz(gasnetc_max_msg_sz);

            gasneti_format_number(gasnetc_pin_maxsz, newval, sizeof(newval), 1);
            GASNETI_TRACE_PRINTF(I, ("Final/effective GASNET_PIN_MAXSZ = %s", newval));

            if (orig != 0) {
                char oldval[16];
                gasneti_format_number(orig, oldval, sizeof(oldval), 1);
                fprintf(stderr,
                        "WARNING: Requested GASNET_PIN_MAXSZ %s reduced by HCA's max_msg_sz to %s\n",
                        oldval, newval);
            }
        }
    }
#endif

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    {
        /* (###) Add code here to determine optimistic maximum segment size */
        /*
         * gasneti_MaxLocalSegmentSize = ###;
         */

        /* (###) Add code here to find the MIN(MaxLocalSegmentSize) over all nodes */
        /*
         * gasneti_MaxGlobalSegmentSize = ###;
         */

        /* it may be appropriate to use gasneti_segmentInit() here to set
           gasneti_MaxLocalSegmentSize and gasneti_MaxGlobalSegmentSize,
           if your conduit can use memory anywhere in the address space
           (you may want to tune GASNETI_MMAP_MAX_SIZE to limit the max size)

           it may also be appropriate to first call gasneti_mmapLimit() to
           account for limitations imposed by having multiple GASNet nodes
           per shared-memory compute node
        */

        /*
         * Find max pinnable size before we start carving up memory w/ mmap()s.
         *
         * Take care that only one process per host performs the probe.
         * The result is then divided by the number of processes on the
         * host, which is easily determined from the connection
         * information we exchanged above.
         */
        GASNETI_TRACE_PRINTF(C,("I am node %d of %d on-node peers",
                                gasneti_nodemap_local_rank,
                                gasneti_nodemap_local_count));

        gasnetc_init_pin_info(gasneti_nodemap_local[0],
                              gasneti_nodemap_local_count);

        gasneti_assert(gasnetc_pin_info.memory != 0);
        gasneti_assert(gasnetc_pin_info.memory != (uintptr_t)(-1));
        /*gasneti_assert(gasnetc_pin_info.regions != 0);*/


#if GASNET_SEGMENT_FAST
        gasneti_segmentInit((gasnetc_pin_info.memory /*- reserved_mem*/),
                            &gasneti_bootstrapExchange);
#elif GASNET_SEGMENT_LARGE
        {
            uintptr_t limit = gasneti_mmapLimit((uintptr_t)-1, (uint64_t)-1,
                                                &gasneti_bootstrapExchange,
                                                &gasneti_bootstrapBarrier);
            gasneti_segmentInit(limit, &gasneti_bootstrapExchange);
        }
#endif
    }
#elif GASNET_SEGMENT_EVERYTHING
    /* segment is everything - nothing to do */
#else
#error Bad segment config
#endif

#if (0) /* The ssh or mpi spwaner code takes care of this */
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
    /*
     * gasneti_setupGlobalEnvironment(gasneti_nodes, gasneti_mynode,
     *                                gasnetc_bootstrapExchange, gasnetc_bootstrapBroadcast);
     */
    gasneti_setupGlobalEnvironment(gasneti_nodes, gasneti_mynode,
                                   gasneti_bootstrapExchange, gasneti_bootstrapBroadcast);
#endif

    /*
     * Establish connections to all the nodes
     */
    res = gasnetc_connect_init();
    if (res != GASNET_OK) {
        return res;
    }

    /*
     * Allocate and clear buffer of flags that would denote whether
     * this connection needs FENCE operation as part of barrier
     */
    gasnet_mxm_module.need_fence = gasneti_calloc(gasneti_nodes, sizeof(uint8_t));
    if (gasnet_mxm_module.need_fence == NULL) {
        MXM_ERROR("Out of memory");
        return GASNET_ERR_NOT_INIT;
    }

    /*
     * Post all receive requests
     */
    for (r = gasnet_mxm_module.am_recv_head; r != NULL; r = r->next) {
        if (gasnetc_post_recv(r)) {
            MXM_ERROR("Unable to post receive\n");
            return GASNET_ERR_NOT_INIT;
        }
    }

    /*
     * wait for all the nodes to complete bootstrap
     */
    gasnetc_bootstrapBarrier();

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
    gasnet_node_t i;
    size_t maxsize = 0;

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
    {   int i;
        for (i = 0; i < GASNETC_MAX_NUMHANDLERS; i++)
            gasnetc_handler[i] = (gasneti_handler_fn_t)&gasneti_defaultAMHandler;
    }
    {   /*  core API handlers */
        gasnet_handlerentry_t *ctable = (gasnet_handlerentry_t *)gasnetc_get_handlertable();
        int len = 0;
        int numreg = 0;
        gasneti_assert(ctable);
        while (ctable[len].fnptr) len++; /* calc len */
        if (gasnetc_reghandlers(ctable, len, 1, 63, 0, &numreg) != GASNET_OK)
            GASNETI_RETURN_ERRR(RESOURCE,"Error registering core API handlers");
        gasneti_assert(numreg == len);
    }

    {   /*  extended API handlers */
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

    /* Handler for non-collective returns from main() */
#if HAVE_ON_EXIT
    on_exit(gasnetc_on_exit, NULL);
#else
    atexit(gasnetc_atexit);
#endif

    /* ------------------------------------------------------------------------------------ */
    /*  register segment  */

    gasneti_seginfo = (gasnet_seginfo_t *) gasneti_malloc(gasneti_nodes * sizeof (gasnet_seginfo_t));

#if GASNET_SEGMENT_FAST
    if (segsize == 0) segbase = NULL; /* no segment */
    else {
        /* (###) add code here to choose and register a segment
           (ensuring alignment across all nodes if this conduit sets GASNET_ALIGNED_SEGMENTS==1)
           you can use gasneti_segmentAttach() here if you used gasneti_segmentInit() above
        */
        gasneti_assert(((uintptr_t)segbase) % GASNET_PAGESIZE == 0);
        gasneti_assert(segsize % GASNET_PAGESIZE == 0);

        /* allocate the segment and exchange seginfo */
        gasneti_segmentAttach(segsize, minheapoffset, gasneti_seginfo, &gasneti_bootstrapExchange);
        segbase = gasneti_seginfo[gasneti_mynode].addr;
        segsize = gasneti_seginfo[gasneti_mynode].size;

        gasnetc_seg_start = (uintptr_t)segbase;
        gasnetc_seg_end   = (uintptr_t)segbase + (segsize - 1);

        /* Find the largest number of pinned regions required */
        for (i=0; i < gasneti_nodes; ++i) {
            maxsize = MAX(maxsize, gasneti_seginfo[i].size);
        }
        gasnetc_max_regs = (maxsize + gasnetc_pin_maxsz - 1) >> gasnetc_pin_maxsz_shift;
        /* pin the segment and exchange the RKeys */
        {
            uintptr_t         addr;
            size_t            remain;
            int               j;
            int gasnetc_my_reg_offset = gasnetc_max_regs*gasneti_mynode;
            /* gasnet_mxm_module.reg is an array of info objects of
             * all the registered segments on all the nodes.
             * Its size is O((max number of local segments) x (number of nodes)) */
            gasnet_mxm_module.reg = gasneti_calloc(
                                        gasneti_nodes * gasnetc_max_regs, sizeof(gasnetc_memreg_t));

            for (j = 0, addr = gasnetc_seg_start, remain = segsize; remain != 0; ++j) {
                size_t len = MIN(remain, gasnetc_pin_maxsz);
                if (gasnetc_pin((void *)addr, len, &gasnet_mxm_module.reg[j+gasnetc_my_reg_offset]))
                    gasneti_fatalerror("failed registering the segment");
                addr += len;
                remain -= len;
                gasneti_assert(j <= gasnetc_max_regs);
            }

            /* ToDo:
             * + When using PSHM we could store mkeys just once per supernode
             * + When not fully connected, we could utilize sparse storage
             */

            gasneti_bootstrapExchange(gasnet_mxm_module.reg + gasnetc_my_reg_offset,
                                      gasnetc_max_regs*sizeof(gasnetc_memreg_t),
                                      gasnet_mxm_module.reg);

            gasnetc_seg_reg_count = j;
        }
    }

#elif GASNET_SEGMENT_LARGE
    {
        /* just allocate the segment (don't pin it) and exchange seginfo */
        gasneti_segmentAttach(segsize, minheapoffset, gasneti_seginfo, gasneti_bootstrapExchange);
        segbase = gasneti_seginfo[gasneti_mynode].addr;
        segsize = gasneti_seginfo[gasneti_mynode].size;
    }

#else /* GASNET_SEGMENT_EVERYTHING */
    {
        segbase = (void *)0;
        segsize = (uintptr_t)-1;
        /* (###) add any code here needed to setup GASNET_SEGMENT_EVERYTHING support */
        {
            int i;
            for (i = 0; i < gasneti_nodes; i++) {
                gasneti_seginfo[i].addr = (void *)0;
                gasneti_seginfo[i].size = (uintptr_t)-1;
            }
        }
    }
#endif

    /* ------------------------------------------------------------------------------------ */
    /*  gather segment information */

    /* (###) add code here to gather the segment assignment info into
             gasneti_seginfo on each node (may be possible to use AMShortRequest here)
     */
    /*
     * Done
     */

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

    gasneti_bootstrapCleanup();

    return GASNET_OK;
}

/* ------------------------------------------------------------------------------------ */

/* gasnetc_atexit OR gasnetc_on_exit
 *
 * This is a simple (at,on_}exit() handler to achieve a hopefully graceful exit.
 * We use the functions gasnetc_exit_{head,body}() to coordinate the shutdown.
 * Note that we don't call gasnetc_exit_tail() since we anticipate the normal
 * exit() procedures to shutdown the multi-threaded process nicely and also
 * because with atexit() we don't have access to the exit code!
 */
#if HAVE_ON_EXIT
static void gasnetc_on_exit(int exitcode, void *arg)
{
    /* Check return from _head to avoid reentrance */
    if (gasnetc_exit_head(exitcode)) {
        gasnetc_exit_body();
    }
}
#else
static void gasnetc_atexit(void)
{
    /* Check return from _head to avoid reentrance */
    if (gasnetc_exit_head(0)) { /* real exit code is outside our control */
        gasnetc_exit_body();
    }
}
#endif

/* ------------------------------------------------------------------------------------ */

static void gasneti_mxm_finalize(void)
{
    int i;
    mxm_error_t mxm_res;

    MXM_LOG("Cleaning up MXM conduit resources\n");

    if (gasnet_mxm_module.connections) {
        for (i = 0; i < gasneti_nodes; i++) {
            if (gasnet_mxm_module.connections[i]) {
                mxm_res = mxm_ep_disconnect(gasnet_mxm_module.connections[i]);
                if (mxm_res != MXM_OK) {
                    MXM_ERROR("Failed disconnecting endpoint (%s)\n",
                              mxm_error_string(mxm_res));
                }
            }
        }
        gasneti_free(gasnet_mxm_module.connections);
    }

    mxm_mq_destroy(gasnet_mxm_module.mxm_mq);

    if (gasnet_mxm_module.mxm_ep)
        mxm_ep_destroy(gasnet_mxm_module.mxm_ep);

    for(i = 0; i < gasnet_mxm_module.am_max_depth; i++) {
        gasnetc_free_recv_req(gasnet_mxm_module.am_recv_pool[i]);
    }
    gasneti_free(gasnet_mxm_module.am_recv_pool);

#ifdef GASNET_SEGMENT_FAST
    if (gasneti_attach_done) {
        size_t remain = gasneti_seginfo[gasneti_mynode].size;
        int j = gasnetc_max_regs*gasneti_mynode;
        while (remain) {
            gasnetc_unpin(&gasnet_mxm_module.reg[j++]);
            remain -= MIN(remain, gasnetc_pin_maxsz);
        }
    }
#endif

    if (gasnet_mxm_module.mxm_context)
        mxm_cleanup(gasnet_mxm_module.mxm_context);

    if (gasnet_mxm_module.need_fence)
        gasneti_free(gasnet_mxm_module.need_fence);
    if (gasnet_mxm_module.remote_eps)
        gasneti_free(gasnet_mxm_module.remote_eps);
    if (gasnet_mxm_module.reg)
        gasneti_free(gasnet_mxm_module.reg);
}

/* -------------------------------------------------------------------------- */

/*
 * Disabling AM handlers when exiting.
 * We need this because we must call AMPoll for system messages.
 * Note that system messages do not call any gasnet handlers.
 * Instead, they are processed in the callback function itself.
 */
static void gasnetc_noop(void) {
    return;
}

static void gasnetc_disable_AMs(void)
{
    int i;
    for (i = 0; i < GASNETC_MAX_NUMHANDLERS; ++i)
        gasnetc_handler[i] = (gasneti_handler_fn_t)&gasnetc_noop;
}


/* -------------------------------------------------------------------------- */

/* gasnetc_exit_now
 *
 * First we set the atomic variable gasnetc_exit_done to allow
 * the exit of any threads which are still spinning on it.
 * Then this function tries hard to actually terminate the calling thread.
 * If for some unlikely reason the _exit() call returns, we abort().
 *
 * DOES NOT RETURN
 */
static void gasnetc_exit_now(int exitcode)
{
    /* If anybody is still waiting, let them go */
    gasneti_atomic_set(&gasnetc_exit_done, 1, GASNETI_ATOMIC_WMB_POST);

    MXM_DEBUG_EXIT_FLOW("Completing exit flow [%s, pid %d, exit code %d] - calling killmyprocesses()\n",
                        gasnett_gethostname(), getpid(), exitcode);

    gasneti_killmyprocess(exitcode);

    /* NOT REACHED */

    gasneti_reghandler(SIGABRT, SIG_DFL);
    gasneti_fatalerror("gasnetc_exit_now aborting...");
    /* NOT REACHED */
}

/* -------------------------------------------------------------------------- */

/*
 * This function handler is responsible for receiving the remote exit
 * requests from the master node and replying. We call gasnetc_exit_head()
 * with the exitcode seen in the remote exit request. If this remote request
 * is seen before any local exit requests (normal or signal), then we are
 * also responsible for starting the exit procedure, via
 * gasnetc_exit_{body,tail}().
 * Additionally, we are responsible for firing off a SIGQUIT to let the
 * user's handler, if any, run before we begin to exit.
 */
void
gasnetc_HandleSystemExitReq(gasnet_token_t token, gasnet_handlerarg_t exitcode)
{
    int rc;

    /* The master will send this AM, but should _never_ receive it */
    gasneti_assert(gasneti_atomic_read(&gasnetc_exit_role, 0) != GASNETC_EXIT_ROLE_MASTER);

    /* We should never receive this AM multiple times */
    gasneti_assert(gasneti_atomic_read(&gasnetc_exit_reqs, 0) == 0);

    MXM_DEBUG_EXIT_FLOW("Handling exit request - increasing gasnetc_exit_reqs\n");
    /* Count the exit requests, so gasnetc_exit_wait() knows when to return */
    gasneti_atomic_increment(&gasnetc_exit_reqs, 0);

    /* If we didn't already know, we are now certain our role is "slave" */
    (void) gasneti_atomic_compare_and_swap(&gasnetc_exit_role,
                                           GASNETC_EXIT_ROLE_UNKNOWN, GASNETC_EXIT_ROLE_SLAVE, 0);

    /* Wait for reply from root node if we already requested our role */
    MXM_DEBUG_EXIT_FLOW("Waiting for role rep before sending responce to master: current count=%d\n", gasneti_atomic_read(&gasnetc_exit_role_rep_out, 0));
    while (gasneti_atomic_read(&gasnetc_exit_role_rep_out, 0) != 0) {
        gasnetc_AMPoll_nocheckattach(); 
    }

    MXM_DEBUG_EXIT_FLOW("Handling exit request - sending response to master\n");
    /* Send a reply so the master knows we are reachable */
    rc = gasnetc_SystemReply(token, 1,
                             (gasnet_handlerarg_t) SYSTEM_EXIT_REP);
    gasneti_assert(rc == GASNET_OK);

    /*
     * Initiate an exit IFF this is the first we've heard of it.
     * gasnetc_exit_head() returns non-zero on first call only.
     */
    if (gasnetc_exit_head(exitcode)) {
        gasneti_sighandlerfn_t handler;
        MXM_DEBUG_EXIT_FLOW("Got exit request from master node - starting exit flow...\n");
        /* IMPORTANT NOTE
         * When we reach this point we are in a request handler which
         * will never return.
         * Care should be taken to ensure this doesn't wedge the AM recv logic.
         *
         * This is currently safe because:
         * 1) request handlers are run w/ no locks held
         * 2) we always have an extra thread to recv AM requests <--??? NOT TRUE FOR MXM
         */

        /* To try and be reasonably robust, want to avoid performing the
         * shutdown and exit from signal context if we can avoid it.
         * However, we must raise SIGQUIT if the user has registered a handler.
         * Therefore we inspect what registered before calling raise().
         *
         * XXX we don't do this atomically w.r.t the signal
         * XXX we don't do the right thing w/ SIG_ERR and SIG_HOLD
         */
        handler = gasneti_reghandler(SIGQUIT, SIG_IGN);
        if ((handler != gasneti_defaultSignalHandler) &&
#ifdef SIG_HOLD
                (handler != (gasneti_sighandlerfn_t) SIG_HOLD) &&
#endif
                (handler != (gasneti_sighandlerfn_t) SIG_ERR) &&
                (handler != (gasneti_sighandlerfn_t) SIG_IGN) &&
                (handler != (gasneti_sighandlerfn_t) SIG_DFL)) {
            (void) gasneti_reghandler(SIGQUIT, handler);
#if 1
            raise(SIGQUIT);
            /* Note: Both ISO C and POSIX assure us that raise() won't return until
             * after the signal handler (if any) has executed.  However, if that
             * handler calls gasnetc_exit(), we'll never return here. */
#elif 0
            kill(getpid(), SIGQUIT);
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

    MXM_DEBUG_EXIT_FLOW("No need to initiate exit sequence, exit already in progress\n");
    return;
}

/* -------------------------------------------------------------------------- */

/*
 * This request handler (invoked only on the "root" node) handles the election
 * of a single exit "master", who will coordinate an orderly shutdown.
 */
static void
gasnetc_HandleSystemExitRoleReq(gasnet_token_t token)
{
    int result, rc;
    int local_role;
    gasnet_node_t src;

    rc = gasnet_AMGetMsgSource(token, &src);
    gasneti_assert(rc == GASNET_OK);

    /* May only send this request to the root node */
    gasneti_assert(gasneti_mynode == GASNETC_EXIT_FLOW_ROOT_NODE);

#if (1)
    /* What role would the local node get if the requester is made the master? */
    local_role = (src == GASNETC_EXIT_FLOW_ROOT_NODE) ?
                 GASNETC_EXIT_ROLE_MASTER : GASNETC_EXIT_ROLE_SLAVE;

    /*
     * Try atomically to assume the proper role.
     * Result determines role of requester:
     *   - if this is the first time we're getting this request,
     *     the requester will receive MASTER response
     *   - else the requester will receive SLAVE
     * The trick here is that if the requester is ROOT_NODE,
     * then its role will be set as well, and not only returned
     * in response.
     */
    result = gasneti_atomic_compare_and_swap(
                 &gasnetc_exit_role, GASNETC_EXIT_ROLE_UNKNOWN, local_role, 0) ?
             GASNETC_EXIT_ROLE_MASTER : GASNETC_EXIT_ROLE_SLAVE;
#else
    /*
     * Logically, this is much simpler - don't try to be smart,
     * just return MASTER to the first caller.
     * However, this is a bit less efficient, because
     * the root node will know its role only after it gets
     * a query from itself and responds to itself.
     *
     * Good to have this option for debugging exit flow though.
     */
    {
        static gasneti_atomic_t first_caller = gasneti_atomic_init(1);

        /*
         * Decrement first_caller value, return non-zero IFF the new value is 0.
         * This means that first caller will get MASTER, the rest will get SLAVE.
         */
        result = (gasneti_atomic_decrement_and_test(&first_caller, 0)) ?
                 GASNETC_EXIT_ROLE_MASTER : GASNETC_EXIT_ROLE_SLAVE;
    }
#endif

    MXM_DEBUG_EXIT_FLOW("Informing requester node %d: role is is %s\n", (int)src,
                        (result == GASNETC_EXIT_ROLE_MASTER) ? "MASTER" : "SLAVE");

    /* Inform the requester of the outcome. */
    rc = gasnetc_SystemReply(token, 2,
                             (gasnet_handlerarg_t)SYSTEM_EXIT_ROLE_REP,
                             (gasnet_handlerarg_t)result);
    gasneti_assert(rc == GASNET_OK);
}

/* -------------------------------------------------------------------------- */

/*
 * This reply handler receives the result of the election of an exit "master".
 * The reply contains the exit "role" this node should assume.
 */
static void
gasnetc_HandleSystemExitRoleRep(gasnet_token_t token,
                                gasnet_handlerarg_t role)
{
    gasnet_node_t src;
    int rc = gasnet_AMGetMsgSource(token, &src);

    gasneti_assert(rc == GASNET_OK);
    /* May only receive this reply from the root node */
    gasneti_assert(src == GASNETC_EXIT_FLOW_ROOT_NODE);

    gasneti_assert((role == GASNETC_EXIT_ROLE_MASTER) ||
                   (role == GASNETC_EXIT_ROLE_SLAVE));

    /* Set the role if not yet set.
     * Then assert that the assigned role has been assumed.
     * This way the assertion is checking that if the role
     * was obtained by other means (namely by receiving an
     * exit request) it must match the election result.
     */
    (void) gasneti_atomic_compare_and_swap(&gasnetc_exit_role,
                                           GASNETC_EXIT_ROLE_UNKNOWN, role, 0);
    gasneti_assert(gasneti_atomic_read(&gasnetc_exit_role, 0) == role);
    MXM_DEBUG_EXIT_FLOW("got exit role reply: current count=%d\n", gasneti_atomic_read(&gasnetc_exit_role_rep_out, 0));
    gasneti_atomic_decrement(&gasnetc_exit_role_rep_out, 0);
}

/* -------------------------------------------------------------------------- */

void gasnetc_HandleSystemExitMessage(gasnetc_am_token_t * token,
                                     gasnet_handlerarg_t * args, uint8_t numargs)
{

    /*
     * System exit message has to have one or two args
     * First argument is always the exit message type.
     */
    gasneti_assert(numargs);
    switch (args[0]) {

    case SYSTEM_EXIT_REQ:
        /* second argument is exit code */
        gasneti_assert(numargs == 2);
        gasnetc_HandleSystemExitReq(token, args[1]);
        break;

    case SYSTEM_EXIT_REP:
        gasneti_assert(numargs == 1);
        /* just count this reply message */
        gasneti_atomic_increment(&gasnetc_exit_reps, 0);
        break;

    case SYSTEM_EXIT_ROLE_REQ:
        gasneti_assert(numargs == 1);
        gasnetc_HandleSystemExitRoleReq(token);
        break;

    case SYSTEM_EXIT_ROLE_REP:
        /* second argument is exit role */
        gasneti_assert(numargs == 2);
        gasnetc_HandleSystemExitRoleRep(token, args[1]);
        break;

    case SYSTEM_EXIT_ALL_DONE:
        gasneti_assert(numargs == 1);
        MXM_DEBUG_EXIT_FLOW("all done notification from master\n");
        gasneti_atomic_increment(&gasnetc_exit_all_done, 0);
        break;

    default:
        gasneti_fatalerror("invalid system message type - %d", args[0]);
    }
}

/* -------------------------------------------------------------------------- */

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

        /*
         * Don't know our role yet.
         * Send a system-category AM Request to determine our role.
         */
        rc = gasnetc_SystemRequest(GASNETC_EXIT_FLOW_ROOT_NODE, /* destination*/
                                   1,                           /* nargs*/
                                   (gasnet_handlerarg_t)SYSTEM_EXIT_ROLE_REQ);

        gasneti_assert(rc == GASNET_OK);
        MXM_DEBUG_EXIT_FLOW("sending exit role request: current count=%d\n", gasneti_atomic_read(&gasnetc_exit_role_rep_out, 0));

        gasneti_atomic_increment(&gasnetc_exit_role_rep_out, 0);

        /* Now spin until somebody tells us what our role is */
        do {
            gasnetc_AMPoll_nocheckattach();
            role = gasneti_atomic_read(&gasnetc_exit_role, 0);
        } while (role == GASNETC_EXIT_ROLE_UNKNOWN);
    }

    return role;
}

/* -------------------------------------------------------------------------- */

/* gasnetc_exit_sighandler
 *
 * This signal handler is for a last-ditch exit when a signal arrives while
 * attempting the graceful exit.  That includes SIGALRM if we get wedged.
 *
 * Just a signal-handler wrapper for gasnetc_exit_now().
 *
 * DOES NOT RETURN
 */
static void gasnetc_exit_sighandler(int sig)
{
    int exitcode = (int)gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);
    static gasneti_atomic_t once = gasneti_atomic_init(1);

    /* always print this */
    /* note - can't call trace macros here, or even sprintf */
    if (sig == SIGALRM) {
        static const char msg1[] = "gasnetc_exit_sighandler(): WARNING: timeout during exit... goodbye\n";
        write(STDERR_FILENO, msg1, sizeof (msg1) - 1);
    } else {
        static const char msg1[] = "gasnetc_exit_sighandler(): ERROR: signal ";
        static const char msg2[] = " received during exit... goodbye\n";
        char digit;

        write(STDERR_FILENO, msg1, sizeof (msg1) - 1);

        /* assume sig < 100 */
        if (sig > 9) {
            digit = '0' + ((sig / 10) % 10);
            write(STDERR_FILENO, &digit, 1);
        }
        digit = '0' + (sig % 10);
        write(STDERR_FILENO, &digit, 1);

        write(STDERR_FILENO, msg2, sizeof (msg2) - 1);
    }

    if (gasneti_atomic_decrement_and_test(&once, 0)) {
        /* We ask the bootstrap support to kill us, but only once */
        gasneti_reghandler(SIGALRM, gasnetc_exit_sighandler);
        alarm(5);
        gasneti_bootstrapAbort(exitcode);
    } else {
        gasnetc_exit_now(exitcode);
    }
    /* NOT REACHED */
}

/* -------------------------------------------------------------------------- */

/* gasnetc_exit_tail
 *
 * This the final exit code for the cases of local or remote requested exits.
 * It is not used for the return-from-main case.  Nor is this code used if
 * a fatal signal (including SIGALRM on timeout) is encountered while
 * trying to shutdown.
 *
 * Just a wrapper around gasnetc_exit_now() to actually terminate.
 *
 * DOES NOT RETURN
 */
static void gasnetc_exit_tail(void)
{
    gasnetc_exit_now((int)gasneti_atomic_read(&gasnetc_exit_code,
                     GASNETI_ATOMIC_RMB_PRE));
    /* NOT REACHED */
}

/* -------------------------------------------------------------------------- */

/* gasnetc_exit_master
 *
 * We say a polite goodbye to our peers and then listen for their replies.
 * This forms the root nodes portion of a barrier for graceful shutdown.
 *
 * The "goodbyes" are just a system-category AM containing the desired
 * exit code.
 * The AM helps ensure that on non-collective exits the "other" nodes know
 * to exit.
 * If we see a "goodbye" from all of our peers we know we've managed to
 * coordinate an orderly shutdown.  If not, then in gasnetc_exit_body()
 * we can ask the bootstrap support to kill the job in a less graceful way.
 *
 * Takes the exitcode and a timeout in us as arguments
 *
 * Returns 0 on success, non-zero on any sort of failure including timeout.
 */
static int gasnetc_exit_master(int exitcode, int64_t timeout_us)
{
    int i, rc;
    gasneti_tick_t start_time;

    gasneti_assert(timeout_us > 0);

    start_time = gasneti_ticks_now();

    /* Notify phase */
    for (i = 0; i < gasneti_nodes; ++i) {
        if (i == gasneti_mynode)
            continue; /* no need to send to self */
        if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us)
            return -1;

        MXM_DEBUG_EXIT_FLOW("Master sending exit request to slave node %d\n", i);
        rc = gasnetc_SystemRequest(i, /* destination*/
                                   2, /* nargs*/
                                   (gasnet_handlerarg_t) SYSTEM_EXIT_REQ,
                                   (gasnet_handlerarg_t) exitcode);
        if (rc != GASNET_OK)
            return -1;
    }

    /* Wait phase - wait for replies from our N-1 peers */
    MXM_DEBUG_EXIT_FLOW("Master waiting to receive %d exit replies...\n",
                        gasneti_nodes - 1);
    while (gasneti_atomic_read(&gasnetc_exit_reps, 0) < (gasneti_nodes - 1)) {
        if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us)
            return -1;
        gasnetc_AMPoll_nocheckattach();
    }
    MXM_DEBUG_EXIT_FLOW("Master received %d exit replies - done polling\n",
                        gasneti_nodes - 1);

    rc = gasnetc_SystemRequest(GASNETC_EXIT_FLOW_ROOT_NODE, /* destination*/
            1, /* nargs*/
            (gasnet_handlerarg_t) SYSTEM_EXIT_ALL_DONE);
    if (rc != GASNET_OK)
        return -1;
    MXM_DEBUG_EXIT_FLOW("Master sent ALL_DONE message to root node(%d)\n", GASNETC_EXIT_FLOW_ROOT_NODE);
    return 0;
}


/* -------------------------------------------------------------------------- */

/* gasnetc_exit_slave
 *
 * We wait for a polite goodbye from the exit master.
 *
 * Takes a timeout in us as arguments
 *
 * Returns 0 on success, non-zero on timeout.
 */
static int gasnetc_exit_slave(int64_t timeout_us)
{
    gasneti_tick_t start_time;
    gasneti_assert(timeout_us > 0);
    start_time = gasneti_ticks_now();

    /* wait until the exit request is received from the master */
    MXM_DEBUG_EXIT_FLOW("Slave waiting for exit request from master. Exit reqs=%d\n", gasneti_atomic_read(&gasnetc_exit_reqs, 0));
    while (gasneti_atomic_read(&gasnetc_exit_reqs, 0) == 0) {
        if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) {
            MXM_DEBUG_EXIT_FLOW("Slave exit by timeout on exit_reqs\n");
            return -1;
        }
        gasnetc_AMPoll_nocheckattach(); /* works even before _attach */
    }

    /* wait till we get role replies from master. Else we will cause master to timeout on sending exit rep */
    MXM_DEBUG_EXIT_FLOW("Slave waiting for role rep: current count=%d\n", gasneti_atomic_read(&gasnetc_exit_role_rep_out, 0));
    while (gasneti_atomic_read(&gasnetc_exit_role_rep_out, 0) != 0) {
        if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) {
            MXM_DEBUG_EXIT_FLOW("Slave exit by timeout on exit_role_count\n");
            return -1;
        }
        gasnetc_AMPoll_nocheckattach(); /* works even before _attach */
    }


    return 0;
}

static int gasnetc_exit_root_node(int64_t timeout_us)
{
    gasneti_tick_t start_time;
    gasneti_assert(timeout_us > 0);
    start_time = gasneti_ticks_now();

    /* wait until the exit request is received from the master */
    MXM_DEBUG_EXIT_FLOW("root waiting for all done request from master. current=%d\n", gasneti_atomic_read(&gasnetc_exit_all_done, 0));
    while (gasneti_atomic_read(&gasnetc_exit_all_done, 0) == 0) {
        if (gasneti_ticks_to_ns(gasneti_ticks_now() - start_time) / 1000 > timeout_us) {
            MXM_DEBUG_EXIT_FLOW("root node exit by timeout on exit_all_done\n");
            return -1;
        }
        gasnetc_AMPoll_nocheckattach(); /* works even before _attach */
    }
    return 0;
}

/* -------------------------------------------------------------------------- */

/* gasnetc_exit_body
 *
 * This code is common to all the exit paths and is used to perform a
 * hopefully graceful exit in all cases.  To coordinate a graceful
 * shutdown gasnetc_get_exit_role() will select one node as the "master".
 * That master node will then send a remote exit request to each of its
 * peers to ensure they know that it is time to exit.
 *
 * If we fail to coordinate the shutdown, we ask the bootstrap to shut us
 * down aggressively.  Otherwise we return to our caller.  Unless our caller
 * is the at-exit handler, we are typically followed by a call to
 * gasnetc_exit_tail() to perform the actual termination.
 *
 * Note also that this function will block all calling threads other than
 * the first until the shutdown code has been completed.
 *
 * XXX: timeouts contained here are entirely arbitrary
 */
static void gasnetc_exit_body(void)
{
    int role, exitcode;
    int graceful = 0;
    int tok_drain = 0;
    int64_t timeout_us;

    /*
     * Once we start a shutdown, ignore all future
     * SIGQUIT signals or we risk reentrancy.
     */
    (void) gasneti_reghandler(SIGQUIT, SIG_IGN);

    /*
     * Ensure only one thread ever continues past this point.
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
            MXM_DEBUG_EXIT_FLOW("Another thread will handle exit - this one is spinning ...\n");
            while (!gasneti_atomic_read(&gasnetc_exit_done, GASNETI_ATOMIC_RMB_PRE)) {
                gasneti_sched_yield(); /* NOT safe to use sleep() here - conflicts with alarm() */
            }
            gasnetc_exit_tail();
            /* NOT REACHED */
        }
    }

    /* read exit code, stored by first caller to gasnetc_exit_head() */
    exitcode = gasneti_atomic_read(&gasnetc_exit_code, GASNETI_ATOMIC_RMB_PRE);

    MXM_DEBUG_EXIT_FLOW("Handling exit flow with exit code %d\n", exitcode);

    /* clear any scheduled SIGALRM events */
    alarm(0);

    /* Establish a last-ditch signal handler in case of failure. */
    gasneti_reghandler(SIGALRM, gasnetc_exit_sighandler);
#if GASNET_DEBUG
    gasneti_reghandler(SIGABRT, SIG_DFL);
#else
    gasneti_reghandler(SIGABRT, gasnetc_exit_sighandler);
#endif
    gasneti_reghandler(SIGILL, gasnetc_exit_sighandler);
    gasneti_reghandler(SIGSEGV, gasnetc_exit_sighandler);
    gasneti_reghandler(SIGFPE, gasnetc_exit_sighandler);
    gasneti_reghandler(SIGBUS, gasnetc_exit_sighandler);

    /* Disable processing of AMs, except system-level ones */
    MXM_DEBUG_EXIT_FLOW("Disabling AM handlers\n");
    gasnetc_disable_AMs();

    GASNETI_TRACE_PRINTF(C, ("gasnet_exit(%i)\n", exitcode));

    /* Try to flush out all the output, allowing at least 30s */
    alarm(MAX(30, (int)gasnetc_exittimeout));
    {
        gasneti_flush_streams();
        gasneti_trace_finish();
        alarm(0);
        gasneti_sched_yield();
    }

    /*
     * Determining our role (master or slave) in the
     * coordination of this shutdown.
     */
    alarm(MAX(120, (int)gasnetc_exittimeout));
    MXM_DEBUG_EXIT_FLOW("Determining node role in shutdown sequence...\n");
    role = gasnetc_get_exit_role();
    MXM_DEBUG_EXIT_FLOW("Role in shutdown sequence is %s\n",
                        (role == GASNETC_EXIT_ROLE_MASTER) ? "MASTER" : "SLAVE");

    /* Attempt a coordinated shutdown */
    timeout_us = gasnetc_exittimeout * 1.0e6;
    alarm(1 + (int) gasnetc_exittimeout);
    switch (role) {
    case GASNETC_EXIT_ROLE_MASTER:
        /* send all the remote exit requests and wait for the replies */
        graceful = (gasnetc_exit_master(exitcode, timeout_us) == 0);
        break;

    case GASNETC_EXIT_ROLE_SLAVE:
        /* wait for the exit request and reply before proceeding */
        graceful = (gasnetc_exit_slave(timeout_us) == 0);
        /*
         * A sleep is insufficient to rely on our reply being out on
         * the wire.  We must actually verify that we have drained
         * enough tokens before deregistering pinned buffers.
         */
        alarm(0);
        break;

    default:
        gasneti_fatalerror("invalid exit role");
    }
    if (gasneti_mynode == GASNETC_EXIT_FLOW_ROOT_NODE) {
        graceful = (gasnetc_exit_root_node(timeout_us) == 0);
    }

    MXM_DEBUG_EXIT_FLOW("%s is exiting %sgracefully\n",
                        (role == GASNETC_EXIT_ROLE_MASTER) ? "Master" : "Slave",
                        (graceful) ? "" : " NOT");

    /* Clean up transport resources, allowing at least 30s */
    alarm(MAX(30, (int)gasnetc_exittimeout));
    {
#if defined(GASNET_SEGMENT_FAST)
        if (gasneti_attach_done &&
                gasneti_seginfo[gasneti_mynode].size > 0) {
            /*
             * Deregister memory...
             */
            if (0) {
                fprintf(stderr, "%d> Couldn't deregister prepinned segment",
                        gasneti_mynode);
            }
        }
#endif

#if GASNET_SEQ /* NOT thread-safe */
        /*
         * ToDo: Destroy PinnedBufs ...
         */
#endif

        gasneti_flush_streams();

        if (gasneti_init_done) { /* ToDo: what to close here? */
            if (gasneti_attach_done) { /* ToDo: what to close here? */
            }
        }

        /* Clean all the MXM conduit-related resources */
        gasneti_mxm_finalize();
    }

    /* Try again to flush out any recent output, allowing at least 5s */
    alarm(MAX(5, (int)gasnetc_exittimeout));
    {
        gasneti_flush_streams();
#if !GASNET_DEBUG_VERBOSE
        gasneti_close_streams();
#endif
    }

    /* XXX potential problems here if exiting from the
     * "Wrong" thread, or from a signal handler */
    alarm(MAX(60, (int)gasnetc_exittimeout));
    {
        if (graceful) {
#if GASNET_DEBUG_VERBOSE
            fprintf(stderr, "Graceful exit initiated by node %d (exit code %d)\n",
                    (int)gasneti_mynode, exitcode);
#endif
            gasneti_bootstrapFini();

        } else {
            /* We couldn't reach our peers, so hope the
             * bootstrap code can kill the entire job */
#if GASNET_DEBUG_VERBOSE
            fprintf(stderr, "Ungraceful exit initiated by node %d\n", (int)gasneti_mynode);
#endif
            gasneti_bootstrapAbort(exitcode);
            /* NOT REACHED */
        }
    }
    alarm(0);
}

/* -------------------------------------------------------------------------- */

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
static int gasnetc_exit_head(int exitcode)
{
    static gasneti_atomic_t once = gasneti_atomic_init(1);
    int retval;

    retval = gasneti_atomic_decrement_and_test(&once, 0);
    if (retval) {
        /* Store the exit code for later use */
        MXM_DEBUG_EXIT_FLOW("First function call - storing exit code %d\n", exitcode);
        gasneti_atomic_set(&gasnetc_exit_code, exitcode, GASNETI_ATOMIC_WMB_POST);
    }
    else {
        MXM_DEBUG_EXIT_FLOW("Subsequent function call - exit code ignored\n");
    }

    return retval;
}

/* -------------------------------------------------------------------------- */

/* gasnetc_exit
 *
 * This is the start of a locally requested exit from GASNet.
 * The caller might be the user, some part of the conduit which has detected an error,
 * or possibly gasneti_defaultSignalHandler() responding to a termination signal.
 */
void gasnetc_exit(int exitcode)
{
    gasnetc_exit_head(exitcode);
    gasnetc_exit_body();
    gasnetc_exit_tail();
    /* NOT REACHED */
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
 *   anything special.  Othwerwise, #define GASNETC_GET_TOKEN in
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
/*  GASNETI_CHECKATTACH();*/
    GASNETI_CHECK_ERRR((!token),BAD_ARG,"bad token");
    GASNETI_CHECK_ERRR((!srcindex),BAD_ARG,"bad src ptr");

#if GASNET_PSHM
    /* (###) If your conduit will support PSHM, let the PSHM code
     * have a chance to recognize the token first, as shown here. */
    if (gasneti_AMPSHMGetMsgSource(token, &sourceid) != GASNET_OK)
#endif
    {
        /* (###) add code here to write the source index into sourceid. */
        sourceid = ((gasnetc_am_token_t*)token)->src_node;
    }

    gasneti_assert(sourceid < gasneti_nodes);
    *srcindex = sourceid;
    return GASNET_OK;
}

extern void gasnetc_ProcessRecv(gasnet_mxm_recv_req_t *r);

static int gasnetc_AMPoll_nocheckattach(void) {
#ifdef MXM_MUTEX_AMPOLL_LOCK
    if (gasneti_mutex_trylock(&gasnetc_ampoll_lock) == 0) {
#endif
#ifdef MXM_ATOMIC_AMPOLL_LOCK
        if (gasneti_atomic_decrement_and_test(&gasnetc_ampoll_atomic_lock,GASNETI_ATOMIC_ACQ)) {
#endif


#if GASNET_PSHM
            /* (###) If your conduit will support PSHM, let it make progress here. */
            gasneti_AMPSHMPoll(0);
#endif
            /* (###) add code here to run your AM progress engine */
            {
                gasnet_mxm_recv_req_t *r;

                r = gasnet_mxm_module.am_recv_head;
                if (r == NULL) {
                    gasneti_fatalerror("no posted reqs... oops");
                    return GASNET_ERR_RESOURCE;
                }

                mxm_progress(gasnet_mxm_module.mxm_context);

                /* receive request is completed - process the message */
                if (mxm_req_test(&r->mxm_rreq.base)) {
                    gasnet_mxm_module.am_recv_head = r->next;
                    gasnetc_ProcessRecv(r);
                    gasnet_mxm_module.am_recv_tail->next = r;
                    r->next = 0;
                    gasnet_mxm_module.am_recv_tail = r;
                    if (gasnet_mxm_module.am_recv_head == NULL) {
                        gasnet_mxm_module.am_recv_head = r;
                    }
                    gasnetc_post_recv(r);
                }

            }
#ifdef MXM_MUTEX_AMPOLL_LOCK
            gasneti_mutex_unlock(&gasnetc_ampoll_lock);
        }
#endif
#ifdef MXM_ATOMIC_AMPOLL_LOCK
        gasneti_atomic_set(&gasnetc_ampoll_atomic_lock,1,GASNETI_ATOMIC_REL);
    }
#endif
    return GASNET_OK;
}

extern int gasnetc_AMPoll(void) {
    GASNETI_CHECKATTACH();
    return gasnetc_AMPoll_nocheckattach();
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
    va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
    /* (###) If your conduit will support PSHM, let it check the dest first. */
    if_pt (gasneti_pshm_in_supernode(dest)) {
        retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Short, dest, handler,
                                               0, 0, 0,
                                               numargs, argptr);
    }
    else
#endif
    {
        /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t)
                 and send the active message
         */

        retval = gasnetc_RequestGeneric(gasnetc_Short, dest, handler,
                                        NULL,        /* void *src_addr*/
                                        0,           /* int nbytes*/
                                        NULL,        /* void *dst_addr*/
                                        1,           /* uint8_t is_sync*/
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
    va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
    /* (###) If your conduit will support PSHM, let it check the dest first. */
    if_pt (gasneti_pshm_in_supernode(dest)) {
        retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Medium, dest, handler,
                                               source_addr, nbytes, 0,
                                               numargs, argptr);
    }
    else
#endif
    {
        /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t)
                 and send the active message
         */
        retval = gasnetc_RequestGeneric(gasnetc_Medium, dest, handler,
                                        source_addr, /* void *src_addr*/
                                        nbytes,      /* int nbytes*/
                                        NULL,        /* void *dst_addr*/
                                        1,           /* uint8_t is_sync*/
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
    va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
    /* (###) If your conduit will support PSHM, let it check the dest first. */
    if_pt (gasneti_pshm_in_supernode(dest)) {
        retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                                               source_addr, nbytes, dest_addr,
                                               numargs, argptr);
    }
    else
#endif
    {
        /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t)
                 and send the active message
         */

        retval = gasnetc_RequestGeneric(gasnetc_Long, dest, handler,
                                        source_addr, /* void *src_addr*/
                                        nbytes,      /* int nbytes*/
                                        dest_addr,   /* void *dst_addr*/
                                        1,           /* uint8_t is_sync*/
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
    va_start(argptr, numargs); /*  pass in last argument */
#if GASNET_PSHM
    /* (###) If your conduit will support PSHM, let it check the dest first. */
    if_pt (gasneti_pshm_in_supernode(dest)) {
        retval = gasneti_AMPSHM_RequestGeneric(gasnetc_Long, dest, handler,
                                               source_addr, nbytes, dest_addr,
                                               numargs, argptr);
    }
    else
#endif
    {
        /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t)
                 and send the active message
         */

        retval = gasnetc_RequestGeneric(gasnetc_Long, dest, handler,
                                        source_addr, /* void *src_addr*/
                                        nbytes,      /* int nbytes*/
                                        dest_addr,   /* void *dst_addr*/
                                        0,           /* uint8_t is_sync*/
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
    /* (###) If your conduit will support PSHM, let it check the token first. */
    if_pt (gasnetc_token_is_pshm(token)) {
        retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Short, token, handler,
                                             0, 0, 0,
                                             numargs, argptr);
    }
    else
#endif
    {
        /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t)
                 and send the active message
         */

        retval = gasnetc_ReplyGeneric(gasnetc_Short, /* gasnetc_category_t category*/
                                      token,         /* gasnet_token_t token*/
                                      handler,       /* gasnet_handler_t handler*/
                                      NULL, 0,       /* void *src_addr, int nbytes*/
                                      NULL,          /* void *dst_addr*/
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
    /* (###) If your conduit will support PSHM, let it check the token first. */
    if_pt (gasnetc_token_is_pshm(token)) {
        retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Medium, token, handler,
                                             source_addr, nbytes, 0,
                                             numargs, argptr);
    }
    else
#endif
    {
        /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t)
                 and send the active message
         */

        retval = gasnetc_ReplyGeneric(gasnetc_Medium,/* gasnetc_category_t category*/
                                      token,         /* gasnet_token_t token*/
                                      handler,       /* gasnet_handler_t handler*/
                                      source_addr,   /* void *src_addr*/
                                      nbytes,        /* int nbytes*/
                                      NULL,          /* void *dst_addr*/
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
    /* (###) If your conduit will support PSHM, let it check the token first. */
    if_pt (gasnetc_token_is_pshm(token)) {
        retval = gasneti_AMPSHM_ReplyGeneric(gasnetc_Long, token, handler,
                                             source_addr, nbytes, dest_addr,
                                             numargs, argptr);
    }
    else
#endif
    {
        /* (###) add code here to read the arguments using va_arg(argptr, gasnet_handlerarg_t)
                 and send the active message
         */

        retval = gasnetc_ReplyGeneric(gasnetc_Long,  /* gasnetc_category_t category*/
                                      token,         /* gasnet_token_t token*/
                                      handler,       /* gasnet_handler_t handler*/
                                      source_addr,   /* void *src_addr*/
                                      nbytes,        /* int nbytes*/
                                      dest_addr,     /* void *dst_addr*/
                                      numargs, argptr);
    }
    va_end(argptr);
    GASNETI_RETURN(retval);
}

/* -------------------------------------------------------------------------- */

static int gasnetc_SystemRequest(gasnet_node_t dest,
                                 int numargs, ...)
{
    int retval;
    va_list argptr;
    va_start(argptr, numargs);
    retval = gasnetc_RequestGeneric(gasnetc_System,/* gasnetc_category_t category*/
                                    dest,          /* destination*/
                                    0,             /* gasnet_handler_t handler*/
                                    NULL,          /* void *src_addr*/
                                    0,             /* int nbytes*/
                                    NULL,          /* void *dst_addr*/
                                    1,             /* uint8_t is_sync*/
                                    numargs, argptr);
    va_end(argptr);
    GASNETI_RETURN(retval);
}

/* -------------------------------------------------------------------------- */

int gasnetc_SystemReply(gasnet_token_t token,
                        int numargs, ...)
{
    int retval;
    va_list argptr;
    va_start(argptr, numargs);
    retval = gasnetc_ReplyGeneric(gasnetc_System, /* gasnetc_category_t category*/
                                  token,          /* gasnet_token_t token*/
                                  0,              /* gasnet_handler_t handler*/
                                  NULL, 0,        /* void *src_addr, int nbytes*/
                                  NULL,           /* void *dst_addr*/
                                  numargs, argptr);
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
