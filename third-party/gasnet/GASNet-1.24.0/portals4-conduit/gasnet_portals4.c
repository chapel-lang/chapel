/*   $Source: bitbucket.org:berkeleylab/gasnet.git/portals4-conduit/gasnet_portals4.c $
 * Description: Portals 4 specific configuration
 * Copyright 2012, Sandia National Laboratories
 * Terms of use are as specified in license.txt
 */



#include <gasnet_internal.h>
#include <gasnet_extended_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>
#include <pmi-spawner/gasnet_bootstrap_internal.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

#if GASNETI_USE_ALLOCA
#include <alloca.h>
#endif

#include <portals4.h>
#include <gasnet_portals4.h>
#include <gasnet_portals4_hash.h>

static ptl_handle_ni_t matching_ni_h;
static ptl_handle_ni_t nonmatching_ni_h;
#if GASNETC_PORTALS4_MAX_MD_SIZE < GASNETC_PORTALS4_MAX_VA_SIZE
static ptl_handle_md_t *am_eq_md_hs;
static ptl_handle_md_t *rdma_md_hs;
#else
static ptl_handle_md_t am_eq_md_h[1];
static ptl_handle_md_t rdma_md_hs[1];
#endif
static ptl_handle_me_t am_me_h;
static ptl_handle_le_t rdma_le_h;
static ptl_handle_eq_t coll_eq_h;
static ptl_handle_eq_t eqs_h[3];
#define am_recv_eq_h eqs_h[0]
#define am_send_eq_h eqs_h[1]
#define rdma_eq_h eqs_h[2]

static ptl_uid_t uid = PTL_UID_ANY;

static ptl_size_t bootstrap_barrier_calls = 0;
static ptl_handle_ct_t bootstrap_barrier_ct_h;
static ptl_handle_me_t bootstrap_barrier_me_h;

/* 16 blocks of 1MB each for AM reception */
static int p4_am_size = 1 * 1024 * 1024;
static int p4_am_num_entries = 16;

static int p4_am_eq_len = 8192;
static int p4_rdma_eq_len = 8192;
static int p4_poll_limit = 1024;

static p4_am_block_t *p4_am_blocks = NULL;
static gasnetc_hash p4_long_hash;

struct p4_long_match_t {
  gasneti_weakatomic_t op_count;
  void *dest_ptr;
  size_t nbytes;
  int handler;
  int numargs;
  gasnet_handlerarg_t pargs[gasnet_AMMaxArgs()];
};
typedef struct p4_long_match_t p4_long_match_t;

#define LONG_HASH(a, b) ((gasnetc_key_t)GASNETI_MAKEWORD(a,b))

static gasneti_semaphore_t p4_send_credits;
static gasneti_semaphore_t p4_rdma_credits;

static gasneti_weakatomic_t p4_op_count = gasneti_weakatomic_init(0);

#define P4_AM_MAX_DATA_LENGTH                                        \
  GASNETI_ALIGNUP(sizeof(gasnet_handlerarg_t) * gasnet_AMMaxArgs() + \
                  gasnet_AMMaxMedium(), GASNETI_MEDBUF_ALIGNMENT)
#define IS_MED_ALIGNED(x) \
  (0 == ((uintptr_t)(x) & (GASNETI_MEDBUF_ALIGNMENT-1)))

/* ---------------------------------------------------------------------------------
 * Error reporting helpers
 *
 * To reduce impact on critical code paths and code bloat from inlining, these
 * are annotated as "NEVER_INLINE" and should be called in if_pf() paths.
 * Example:
 *     if_pf (PTL_OK != ret) {
 *         return p4_failed_put(ret, "sending an AM frag");
 *     }
 */

GASNETI_NEVER_INLINE(p4_error_string,
static const char *
p4_error_string(int ret))
{
    switch (ret) {
    case PTL_OK:              return "PTL_OK";
    case PTL_ARG_INVALID:     return "PTL_ARG_INVALID";
    case PTL_CT_NONE_REACHED: return "PTL_CT_NONE_REACHED";
    case PTL_EQ_DROPPED:      return "PTL_EQ_DROPPED";
    case PTL_EQ_EMPTY:        return "PTL_EQ_EMPTY";
    case PTL_FAIL:            return "PTL_FAIL";
    case PTL_IGNORED:         return "PTL_IGNORED";
    case PTL_IN_USE:          return "PTL_IN_USE";
    case PTL_INTERRUPTED:     return "PTL_INTERRUPTED";
    case PTL_LIST_TOO_LONG:   return "PTL_LIST_TOO_LONG";
    case PTL_NO_INIT:         return "PTL_NO_INIT";
    case PTL_NO_SPACE:        return "PTL_NO_SPACE";
    case PTL_PID_IN_USE:      return "PTL_PID_IN_USE";
    case PTL_PT_FULL:         return "PTL_PT_FULL";
    case PTL_PT_EQ_NEEDED:    return "PTL_PT_EQ_NEEDED";
    case PTL_PT_IN_USE:       return "PTL_PT_IN_USE";
    }
    return "Unknown or invalid error code";
}

GASNETI_NEVER_INLINE(_p4_failed_put,
static int
_p4_failed_put(int ret, const char *descr, const char *fromfn, const char *file, int line))
{
    if (gasneti_VerboseErrors) {
        fprintf(stderr, "GASNet %s returning an error code: GASNET_ERR_BAD_ARG (%s)\n"
                        "  at %s:%i\n"
                        "  reason: PtlPut() failed with error %d (%s) while %s\n",
                        fromfn, gasnet_ErrorDesc(GASNET_ERR_BAD_ARG),
                        __FILE__, __LINE__,
                        ret, p4_error_string(ret), descr);
        fflush(stderr);
    }
    gasnett_freezeForDebuggerErr(); /* allow freeze */
    return GASNET_ERR_BAD_ARG;
}
#define p4_failed_put(_ret, _descr) \
	_p4_failed_put(_ret, _descr, GASNETI_CURRENT_FUNCTION, __FILE__, __LINE__)

static void p4_fatalerror(int ret, const char *descr) GASNETI_NORETURN;
GASNETI_NEVER_INLINE(p4_fatalerror,
static void p4_fatalerror(int ret, const char *descr))
{
    gasneti_fatalerror("[%03d] %s failed: %d (%s)", 
                       gasneti_mynode, descr, ret, p4_error_string(ret));
}
GASNETI_NORETURNP(p4_fatalerror)

static void p4_failed_eqpoll(int ret, const ptl_handle_eq_t eq_h) GASNETI_NORETURN;
GASNETI_NEVER_INLINE(p4_failed_eqpoll,
static void
p4_failed_eqpoll(int ret, const ptl_handle_eq_t eq_h))
{
    if (eq_h == am_send_eq_h) p4_fatalerror(ret, "PtlEQPoll(am_send_eq)");
    if (eq_h == am_recv_eq_h) p4_fatalerror(ret, "PtlEQPoll(am_recv_eq)");
    p4_fatalerror(ret, "PtlEQPoll(UNKNOWN)");
}
GASNETI_NORETURNP(p4_failed_eqpoll)

/*
 * Not all implementations of Portals 4 support binding a memory
 * descriptor which covers all of memory, but all support covering a
 * large fraction of memory.  Therefore, rather than working around
 * the issue by pinning per message, we use a number of memory
 * descriptors to cover all of memory.  As long as the maximum memory
 * descriptor is a large fraction of the user virtual address space
 * (like 46 bit MDs on a platform with 47 bits of user virtual address
 * space), this works fine.
 *
 * Our scheme is to create N memory descriptors which contiguously
 * cover the entire user address space, then another N-1 contiguous
 * memory descriptors offset by 1/2 the size of the MD, then a final
 * memory descriptor of 1/2 the size of the other MDs covering the top
 * of the memory space, to avoid if statements in the critical path.  This
 * scheme allows for a maximum message size of 1/2 the size of the MD
 * without ever crossing an MD boundary.  Also, because MD sizes are
 * always on a power of 2 in this scheme, computing the offsets and MD
 * selection are quick, using only bit shift and mask.
 *
 * p4_get_md() relies heavily on compiler constant folding.  "mask"
 * can be constant folded into a constant.  "which" compiler folds
 * into a bit shift of a register a constant number of times, then
 * masked by a constant (the input is, unfortunately, not constant).
 *
 * In the case where an MD can cover all of memory, p4_get_md() will
 * be compiled into two assignments.  Assuming the function inlines
 * (and it certainly should be), the two assignments should be
 * optimized into register assignments for the Portals call relatively
 * easily.
 */
static inline void
p4_get_md(const void *ptr, ptl_handle_md_t const * md_array,
          ptl_handle_md_t *md_h, void **base_ptr)
{
#if GASNETC_PORTALS4_MAX_MD_SIZE < GASNETC_PORTALS4_MAX_VA_SIZE
    int mask = (1ULL << (GASNETC_PORTALS4_MAX_VA_SIZE - GASNETC_PORTALS4_MAX_MD_SIZE + 1)) - 1;
    int which = (((uintptr_t) ptr) >> (GASNETC_PORTALS4_MAX_MD_SIZE - 1)) & mask;
    *md_h = md_array[which];
    *base_ptr = (void*) (which * (1ULL << (GASNETC_PORTALS4_MAX_MD_SIZE - 1)));
#else
    *md_h = md_array[0];
    *base_ptr = 0;
#endif
}


static inline int
p4_get_num_mds(void)
{
#if GASNETC_PORTALS4_MAX_MD_SIZE < GASNETC_PORTALS4_MAX_VA_SIZE
    return (1 << (GASNETC_PORTALS4_MAX_VA_SIZE - GASNETC_PORTALS4_MAX_MD_SIZE + 1));
#else
    return 1;
#endif
}


/* ---------------------------------------------------------------------------------
 * Initialize Portals 4 conduit code
 *
 * Initialized memory descriptors for sending active messages
 * (including data part of long messages), match list entries for
 * active message handling, and match list for bootstrap barrier.
 * --------------------------------------------------------------------------------- */
int
gasnetc_p4_init(gasnet_node_t *rank_p, gasnet_node_t *size_p)
{
    int initialized, ret, i;
    ptl_ni_limits_t ni_req_limits, ni_limits;
    ptl_process_t my_id;
    ptl_process_t *desired = NULL;
    ptl_md_t md;
    ptl_me_t me;
    ptl_pt_index_t pt;

    /* TODO:
       + Distinct env vars for send and recv eq lengths
       + Distinct env var for poll limit
     */
    p4_am_size = gasneti_getenv_int_withdefault("GASNET_AM_BUFFER_SIZE", p4_am_size, 0);
    p4_am_num_entries = gasneti_getenv_int_withdefault("GASNET_AM_NUM_ENTRIES", p4_am_num_entries, 0);
    p4_am_eq_len = gasneti_getenv_int_withdefault("GASNET_AM_EVENT_QUEUE_LENGTH", p4_am_eq_len, 0);
    p4_rdma_eq_len = gasneti_getenv_int_withdefault("GASNET_EXTENDED_EVENT_QUEUE_LENGTH", p4_rdma_eq_len, 0);
    p4_poll_limit = p4_am_eq_len / 8; /* arbitrary */

    ret = gasneti_bootstrapInit_pmi(NULL, NULL, size_p, rank_p);
    if (GASNET_OK != ret) return ret;

    /* Setup data structures */
    p4_long_hash = gasnetc_hash_create();
    if (NULL == p4_long_hash) gasneti_fatalerror("gasnetc_hash_create");

    /* Initialize Portals */
    ret = PtlInit();
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlInit()");

    /* Initialize network */
    ni_req_limits.max_entries = 1024;
    ni_req_limits.max_unexpected_headers = 1024;
    ni_req_limits.max_mds = 1024;
    ni_req_limits.max_eqs = 1024;
    ni_req_limits.max_cts = 1024;
    ni_req_limits.max_pt_index = 64;
    ni_req_limits.max_iovecs = 1024;
    ni_req_limits.max_list_size = 1024;
    ni_req_limits.max_triggered_ops = 1024;
    ni_req_limits.max_msg_size = LONG_MAX;
    ni_req_limits.max_atomic_size = LONG_MAX;
    ni_req_limits.max_fetch_atomic_size = LONG_MAX;
    ni_req_limits.max_waw_ordered_size = LONG_MAX;
    ni_req_limits.max_war_ordered_size = LONG_MAX;
    ni_req_limits.max_volatile_size = LONG_MAX;
#if GASNET_SEGMENT_EVERYTHING
    ni_req_limits.features = PTL_TARGET_BIND_INACCESSIBLE;
#else
    ni_req_limits.features = 0;
#endif

    ret = PtlNIInit(PTL_IFACE_DEFAULT,
                    PTL_NI_MATCHING | PTL_NI_LOGICAL,
                    PTL_PID_ANY,
                    &ni_req_limits,
                    &ni_limits,
                    &matching_ni_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlNiInit(matching)");
#if GASNET_SEGMENT_EVERYTHING
    if ((ni_limits.features & PTL_TARGET_BIND_INACCESSIBLE) == 0) {
        gasneti_fatalerror("[%03d] Portals reports it doesn't support SEGMENT_EVERYTHING",
                           gasneti_mynode);
    }
#endif

    ret = PtlNIInit(PTL_IFACE_DEFAULT,
                    PTL_NI_NO_MATCHING | PTL_NI_LOGICAL,
                    PTL_PID_ANY,
                    &ni_req_limits,
                    &ni_limits,
                    &nonmatching_ni_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlNiInit(nonmatching)");
#if GASNET_SEGMENT_EVERYTHING
    if ((ni_limits.features & PTL_TARGET_BIND_INACCESSIBLE) == 0) {
        gasneti_fatalerror("[%03d] Portals reports it doesn't support SEGMENT_EVERYTHING",
                           gasneti_mynode);
    }
#endif

    /* NOTE: the Portals 4 specification says that your Physical ID
       must be the same on every logical NI, so this is safe (ie, we
       don't have to call GetPhysId on the non-matching ni) */
    ret = PtlGetPhysId(matching_ni_h, &my_id);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlGetPhysId()");

    /* build id map */
    desired = gasneti_malloc(sizeof(ptl_process_t) * gasneti_nodes);
    gasneti_bootstrapExchange_pmi(&my_id, sizeof(ptl_process_t), desired);

    gasneti_nodemapInit(NULL, &desired[0].phys.nid,
                        sizeof(desired[0].phys.nid),
                        sizeof(desired[0]));

    ret = PtlSetMap(matching_ni_h,
                    gasneti_nodes,
                    desired);
    if_pf (PTL_OK != ret && PTL_IGNORED != ret) p4_fatalerror(ret, "PtlSetMap(matching)");

    ret = PtlSetMap(nonmatching_ni_h,
                    gasneti_nodes,
                    desired);
    if_pf (PTL_OK != ret && PTL_IGNORED != ret) p4_fatalerror(ret, "PtlSetMap(nonmatching)");

    gasneti_free(desired);

    /* uid is similar to physical id in terms of multiple network
       interfaces, so just getting the uid off the matching ni is
       ok */
    ret = PtlGetUid(matching_ni_h, &uid);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlGetUid()");

    ret = PtlEQAlloc(matching_ni_h, p4_am_eq_len, &am_send_eq_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlEQAlloc(send)");
    /* Every send generates the ACK event, so initialize the send credits to size of eq */
    gasneti_semaphore_init(&p4_send_credits, p4_am_eq_len - 1, p4_am_eq_len - 1);

    ret = PtlEQAlloc(matching_ni_h, p4_am_eq_len, &am_recv_eq_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlEQAlloc(recv)");

    ret = PtlEQAlloc(matching_ni_h, 1024, &coll_eq_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlEQAlloc(coll)");

    ret = PtlEQAlloc(nonmatching_ni_h, p4_rdma_eq_len, &rdma_eq_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlEQAlloc(rdma)");
    gasneti_semaphore_init(&p4_rdma_credits, p4_rdma_eq_len, p4_rdma_eq_len);

    /* allocate portal table entries */
    ret = PtlPTAlloc(matching_ni_h,
                     0,
                     coll_eq_h,
                     bootstrap_idx,
                     &pt);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlPTAlloc(bootstrap)");

    ret = PtlPTAlloc(matching_ni_h,
                     PTL_PT_FLOWCTRL,
                     am_recv_eq_h,
                     am_idx,
                     &pt);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlPTAlloc(active message)");

    ret = PtlPTAlloc(matching_ni_h,
                     PTL_PT_FLOWCTRL,
                     am_recv_eq_h,
                     long_data_idx,
                     &pt);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlPTAlloc(long data)");

    ret = PtlPTAlloc(nonmatching_ni_h,
                     0,
                     rdma_eq_h,
                     rdma_idx,
                     &pt);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlPTAlloc(rdma)");

    /* Allocate memory descriptor for active message transfer (including long transfers) */
    md.options   = PTL_MD_EVENT_SEND_DISABLE;
    md.eq_handle = am_send_eq_h;
    md.ct_handle = PTL_CT_NONE;
#if GASNETC_PORTALS4_MAX_MD_SIZE < GASNETC_PORTALS4_MAX_VA_SIZE
    {
        int num_mds;
        ptl_size_t size = (1ULL << GASNETC_PORTALS4_MAX_MD_SIZE) - 1;
        ptl_size_t offset_unit = (1ULL << GASNETC_PORTALS4_MAX_MD_SIZE) / 2;

        num_mds = p4_get_num_mds();
        am_eq_md_hs = gasneti_malloc(sizeof(ptl_handle_md_t) * num_mds);
        for (i = 0 ; i < num_mds ; ++i) {
            md.start = (char*) (offset_unit * i);
            md.length = (i - 1 == num_mds) ? size / 2 : size;
            ret = PtlMDBind(matching_ni_h,
                            &md,
                            &am_eq_md_hs[i]);
            if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlMDBind(AM)");
        }
    }
#else
    md.start     = 0;
    md.length    = PTL_SIZE_MAX;
    ret = PtlMDBind(matching_ni_h,
                    &md,
                    &am_eq_md_h[0]);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlMDBind(AM)");
#endif

    /* Allocate memory descriptor for rdma operations */
    md.options   = PTL_MD_EVENT_SEND_DISABLE;
    md.eq_handle = rdma_eq_h;
    md.ct_handle = PTL_CT_NONE;
#if GASNETC_PORTALS4_MAX_MD_SIZE < GASNETC_PORTALS4_MAX_VA_SIZE
    {
        int num_mds;
        ptl_size_t size = (1ULL << GASNETC_PORTALS4_MAX_MD_SIZE) - 1;
        ptl_size_t offset_unit = (1ULL << GASNETC_PORTALS4_MAX_MD_SIZE) / 2;

        num_mds = p4_get_num_mds();
        rdma_md_hs = gasneti_malloc(sizeof(ptl_handle_md_t) * num_mds);
        for (i = 0 ; i < num_mds ; ++i) {
            md.start = (char*) (offset_unit * i);
            md.length = (i - 1 == num_mds) ? size / 2 : size;
            ret = PtlMDBind(nonmatching_ni_h,
                            &md,
                            &rdma_md_hs[i]);
            if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlMDBind(rdma)");
        }
    }
#else
    md.start     = 0;
    md.length    = PTL_SIZE_MAX;
    ret = PtlMDBind(nonmatching_ni_h,
                    &md,
                     &rdma_md_hs[0]);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlMDBind(rdma)");
#endif

    /* Configure the active message receive resources now, since
       there's no harm in not waiting until the segment code is
       figured out */
    p4_am_blocks = gasneti_malloc(sizeof(p4_am_block_t) * p4_am_num_entries);
    for (i = 0 ; i < p4_am_num_entries ; ++i) {
        void * data = gasneti_malloc(p4_am_size);
        gasneti_assert(IS_MED_ALIGNED(data));
        p4_am_blocks[i].data = data;

#ifdef GASNET_PAR
        ret = PtlCTAlloc(matching_ni_h,
                         &p4_am_blocks[i].ct_h);
        if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlCTAlloc(AM block)");
        gasneti_weakatomic_set(&p4_am_blocks[i].op_count, 0, 0);
#endif

        me.start = data;
        me.length = p4_am_size;
        me.min_free = P4_AM_MAX_DATA_LENGTH;
        me.uid = uid;
        me.options = PTL_ME_OP_PUT | PTL_ME_MANAGE_LOCAL |
            PTL_ME_MAY_ALIGN | PTL_ME_IS_ACCESSIBLE | 
            PTL_ME_EVENT_LINK_DISABLE;
#ifdef GASNET_PAR
        me.ct_handle = p4_am_blocks[i].ct_h;
        me.options |= PTL_ME_EVENT_CT_COMM;
#else
        me.ct_handle = PTL_CT_NONE;
#endif
        me.match_id.rank = PTL_RANK_ANY;
        me.match_bits = ACTIVE_MESSAGE;
        me.ignore_bits = AM_REQREP_BITS;
        ret = PtlMEAppend(matching_ni_h,
                          am_idx,
                          &me,
                          PTL_PRIORITY_LIST,
                          &(p4_am_blocks[i]),
                          &p4_am_blocks[i].me_h);
        if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlMEAppend(AM block)");
    }

    /* Configure the barrier memory descriptor now, so that we can
       rely on it working from here on out... */
    ret = PtlCTAlloc(matching_ni_h, &bootstrap_barrier_ct_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlCTAlloc()");

    me.start = NULL;
    me.length = 0;
    me.min_free = 0;
    me.ct_handle = bootstrap_barrier_ct_h;
    me.uid = uid;
    me.options = PTL_ME_OP_PUT |
        PTL_ME_EVENT_SUCCESS_DISABLE | PTL_ME_EVENT_CT_COMM | 
        PTL_ME_IS_ACCESSIBLE | 
        PTL_ME_EVENT_LINK_DISABLE | PTL_ME_EVENT_UNLINK_DISABLE;
    me.match_id.rank = PTL_RANK_ANY;
    me.match_bits = BOOTSTRAP_BARRIER_MB;
    me.ignore_bits = 0;
    ret = PtlMEAppend(matching_ni_h,
                      bootstrap_idx,
                      &me,
                      PTL_PRIORITY_LIST,
                      NULL,
                      &bootstrap_barrier_me_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlMEAppend(barrier)");

    /* One last barrier using PMI (the other being in the information
       exchange), so that we can be sure everyone is ready to receive
       the barrier messages.  

       TODO: This could probably suck less.  One way to avoid this PMI
       barrier would be to use physical addressing for the bootstrap
       collectives so that we could initialize the receive resources
       for the bootstrapBarrier before the info exchange and therefore
       not care about synchronization until the segmentInit later in
       gasnet_init(). */
    gasneti_bootstrapBarrier_pmi();

    gasneti_bootstrapCleanup_pmi(); /* no more PMI-based collectives */

    return GASNET_OK;
}


/* ---------------------------------------------------------------------------------
 * Attach transfer segment
 *
 * Create match list entry for long data transfer
 * --------------------------------------------------------------------------------- */
int
gasnetc_p4_attach(void *segbase, uintptr_t segsize)
{
    ptl_me_t me;
    ptl_le_t le;
    int ret;

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    gasneti_assert((void*) 0 != segbase && (uintptr_t)-1 != segsize);
    me.start = le.start = segbase;
    me.length = le.length = segsize;
#elif GASNET_SEGMENT_EVERYTHING
    gasneti_assert((void*) 0 == segbase && (uintptr_t)-1 == segsize);
    me.start = le.start = 0;
    me.length = le.start = PTL_SIZE_MAX;
#endif

    me.min_free = 0;
    me.ct_handle = PTL_CT_NONE;
    me.uid = uid;
    me.options = PTL_ME_OP_PUT | PTL_ME_EVENT_LINK_DISABLE;
    me.match_id.rank = PTL_RANK_ANY;
    me.match_bits = LONG_DATA;
    me.ignore_bits = AM_REQREP_BITS;
    ret = PtlMEAppend(matching_ni_h,
                      long_data_idx,
                      &me,
                      PTL_PRIORITY_LIST,
                      NULL,
                      &am_me_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlMEAppend(attach)");

    le.ct_handle = PTL_CT_NONE;
    le.uid = uid;
    le.options = PTL_LE_OP_PUT | PTL_LE_OP_GET | 
        PTL_LE_EVENT_SUCCESS_DISABLE | PTL_LE_EVENT_LINK_DISABLE;
    ret = PtlLEAppend(nonmatching_ni_h,
                      rdma_idx,
                      &le,
                      PTL_PRIORITY_LIST,
                      NULL,
                      &rdma_le_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlLEAppend(attach)");

    return GASNET_OK;
}


/* ---------------------------------------------------------------------------------
 * Exit
 *
 * Clean up as many resources as possible.  Try not to do any more communication.
 * --------------------------------------------------------------------------------- */
void
gasnetc_p4_exit(void)
{
    int i, ret;

    /* assume that there's nothing to clean up in the hash, since
       hopefully all long messages are processed before we exit. */
    gasnetc_hash_destroy(p4_long_hash);

    PtlMEUnlink(am_me_h);
    PtlLEUnlink(rdma_le_h);

    PtlMEUnlink(bootstrap_barrier_me_h);
    PtlCTFree(bootstrap_barrier_ct_h);

    for (i = 0 ; i < p4_am_num_entries ; ++i) {
#ifdef GASNET_PAR
        PtlCTFree(p4_am_blocks[i].ct_h);
#endif

        ret = PtlMEUnlink(p4_am_blocks[i].me_h);
        if (PTL_OK == ret) {
            /* if we didn't unlink, don't free the data, as the unlink
               likely failed because there's a communication in
               progress.  Blocking here seems bad, so just let it
               leak. */
            gasneti_free(p4_am_blocks[i].data);
        }
    }
    gasneti_free(p4_am_blocks);

#if GASNETC_PORTALS4_MAX_MD_SIZE < GASNETC_PORTALS4_MAX_VA_SIZE
    {
        int num_mds = p4_get_num_mds();
        for (i = 0 ; i < num_mds ; ++i) {
            PtlMDRelease(am_eq_md_hs[i]);
            PtlMDRelease(rdma_md_hs[i]);
        }
        gasneti_free(am_eq_md_hs);
        gasneti_free(rdma_md_hs);
    }
#else
    PtlMDRelease(am_eq_md_h[0]);
    PtlMDRelease(rdma_md_hs[0]);
#endif
    PtlPTFree(matching_ni_h, long_data_idx);
    PtlPTFree(matching_ni_h, am_idx);
    PtlPTFree(matching_ni_h, bootstrap_idx);
    PtlPTFree(matching_ni_h, rdma_idx);

    PtlEQFree(coll_eq_h);
    PtlEQFree(am_recv_eq_h);
    PtlEQFree(am_send_eq_h);
    PtlEQFree(rdma_eq_h);

    /* TODO: are these safe to do here? */
    PtlNIFini(matching_ni_h);
    PtlNIFini(nonmatching_ni_h);
    PtlFini();

    /* Currently, regardless whether make this call, non-collective exits hang */
    gasneti_bootstrapFini_pmi();
}


/* ---------------------------------------------------------------------------------
 * freelists for fragments and long matches
 * --------------------------------------------------------------------------------- */
static gasneti_lifo_head_t p4_am_frag_pool = GASNETI_LIFO_INITIALIZER;

GASNETI_INLINE(p4_alloc_am_frag)
p4_frag_am_t *p4_alloc_am_frag(void)
{
    p4_frag_am_t *frag = gasneti_lifo_pop(&p4_am_frag_pool);
    if_pf (NULL == frag) {
        frag = gasneti_malloc(offsetof(p4_frag_am_t,data) + P4_AM_MAX_DATA_LENGTH);
        gasneti_leak(frag);
    }
    frag->data_length = 0;
    return frag;
}

GASNETI_INLINE(p4_free_am_frag)
void p4_free_am_frag(p4_frag_am_t *frag)
{
    gasneti_lifo_push(&p4_am_frag_pool, frag);
}

static gasneti_lifo_head_t p4_data_frag_pool = GASNETI_LIFO_INITIALIZER;

GASNETI_INLINE(p4_alloc_data_frag)
p4_frag_data_t *p4_alloc_data_frag(void)
{
    p4_frag_data_t *frag = gasneti_lifo_pop(&p4_data_frag_pool);
    if_pf (NULL == frag) {
        frag = gasneti_malloc(sizeof(p4_frag_data_t));
        gasneti_leak(frag);
    }
    return frag;
}

GASNETI_INLINE(p4_free_data_frag)
void p4_free_data_frag(p4_frag_data_t *frag)
{
    gasneti_lifo_push(&p4_data_frag_pool, frag);
}

static gasneti_lifo_head_t p4_long_match_pool = GASNETI_LIFO_INITIALIZER;

GASNETI_INLINE(p4_find_long_match)
p4_long_match_t *p4_find_long_match(const gasnetc_key_t key)
{
    p4_long_match_t *match, *tmp;
    match = (p4_long_match_t*) gasnetc_hash_get(p4_long_hash, key);
    if (NULL == match) {
        match = gasneti_lifo_pop(&p4_long_match_pool);
        if_pf (NULL == match) {
            match = gasneti_malloc(sizeof(p4_long_match_t));
            gasneti_leak(match);
        }
        gasneti_weakatomic_set(&match->op_count, 0, 0);
        tmp = gasnetc_hash_put_find(p4_long_hash, key, match);
        if (NULL != tmp) {
            gasneti_lifo_push(&p4_long_match_pool, match);
            match = tmp;
        }
    }
    return match;
}

GASNETI_INLINE(p4_free_long_match)
void p4_free_long_match(p4_long_match_t *match)
{
    gasneti_lifo_push(&p4_long_match_pool, match);
}

/* ---------------------------------------------------------------------------------
 * Internal polling progress loop
 * --------------------------------------------------------------------------------- */
static int
p4_poll(const ptl_handle_eq_t *eq_handles, unsigned int size, unsigned int limit)
{
    ptl_event_t ev;
    int ret;
    unsigned int which;
    int empty = 0;
    int idx = 0;

    while (--limit) {
        /* round-robin to prevent first eq from starving the other(s) when size > 1 */
        const ptl_handle_eq_t *eq_ptr = &eq_handles[idx++];
	if (idx == size) idx = 0;

        ret = PtlEQPoll(eq_ptr, 1, 0, &ev, &which);
        if_pt (PTL_EQ_EMPTY == ret) {
            if (++empty != size) continue;
            break;
        } else if_pf (PTL_OK != ret) {
	    if_pf ((PTL_EQ_DROPPED == ret) && *eq_ptr == am_recv_eq_h) {
                /* fall through and treat as PTL_OK */
            } else {
                p4_failed_eqpoll(ret, *eq_ptr);
            }
        }
        empty = 0;

        switch (ev.type) {
        case PTL_EVENT_PUT:
            /* AM or long data have arrived */
            if_pf (PTL_OK != ev.ni_fail_type) {
                gasneti_fatalerror("[%03d] event of type %d failed %d", 
                                   gasneti_mynode, ev.type, ev.ni_fail_type);
            } else {
                if (IS_ACTIVE_MESSAGE(ev.match_bits)) {
                    int isReq = ((IS_AM_REQUEST(ev.match_bits)) ? 1 : 0);
                    int nbytes = GET_PAYLOAD_LENGTH(ev.match_bits);
                    int numargs = GET_ARG_COUNT(ev.match_bits);
                    int handler = GET_HANDLERID(ev.match_bits);
#if GASNET_PAR
                    p4_am_block_t *block = (p4_am_block_t*) ev.user_ptr;
#endif

                    gasneti_assert(IS_MED_ALIGNED(ev.mlength));

                    if (IS_AM_SHORT(ev.match_bits)) {
                        gasnetc_p4_token_t token;
                        gasnet_handlerarg_t *pargs = (gasnet_handlerarg_t*) ev.start;
                        gasnetc_p4_token_t *tokenp = &token;
                        
                        token.reply_sent = 0;
                        token.sourceid = ev.initiator.rank;

                        gasneti_assert(0 == nbytes);
                        gasneti_assert(ev.mlength ==
                                       GASNETI_ALIGNUP(numargs * sizeof(gasnet_handlerarg_t),
                                                       GASNETI_MEDBUF_ALIGNMENT));

#ifdef P4_DEBUG
                        fprintf(stderr, "%d: firing short handler %d from %d\n",
                                (int) gasneti_mynode, handler, (int) token.sourceid);
#endif
                        GASNETI_RUN_HANDLER_SHORT(isReq, handler, gasnetc_handler[handler],
                                                  tokenp, pargs, numargs);
                    } else if (IS_AM_MEDIUM(ev.match_bits)) {
                        gasnetc_p4_token_t token;
                        gasnet_handlerarg_t *pargs = (gasnet_handlerarg_t*) ev.start;
                        gasnetc_p4_token_t *tokenp = &token;
#if !GASNETI_USE_ALLOCA
                        void *free_buf = NULL;
#endif

                        /* Medium payload is after args, with padding to GASNETI_MEDBUF_ALIGNMENT */
                        void *buf = (void *)(pargs +
                                             GASNETI_ALIGNUP(numargs,
                                                             (GASNETI_MEDBUF_ALIGNMENT /
                                                                sizeof(gasnet_handlerarg_t))));

                        token.reply_sent = 0;
                        token.sourceid = ev.initiator.rank;

                        gasneti_assert(ev.mlength ==
                                       GASNETI_ALIGNUP((uintptr_t)buf + nbytes - (uintptr_t)ev.start,
                                                       GASNETI_MEDBUF_ALIGNMENT));
                        gasneti_assert(nbytes <= gasnet_AMMaxMedium());

                        /* We must allow for PTL_ME_MAY_ALIGN to produce even senseless alignments */
                        if_pf (! IS_MED_ALIGNED(buf)) {
#if GASNETI_USE_ALLOCA
                            buf = memcpy(alloca(nbytes), buf, nbytes);
#else
                            buf = memcpy(gasneti_malloc(nbytes), buf, nbytes);
                            free_buf = buf;
#endif
                        }

#ifdef P4_DEBUG
                        fprintf(stderr, "%d: firing medium handler %d from %d, nbytes: %d\n",
                                (int) gasneti_mynode, handler, (int) token.sourceid, nbytes);
#endif
                        GASNETI_RUN_HANDLER_MEDIUM(isReq, handler, gasnetc_handler[handler],
                                                   tokenp, pargs, numargs, buf, nbytes);
                        
#if !GASNETI_USE_ALLOCA
                        gasneti_free(free_buf);
#endif
                    } else if (IS_AM_LONG_PACKED(ev.match_bits)) {
                        gasnetc_p4_token_t token;
                        gasnet_handlerarg_t *pargs = (gasnet_handlerarg_t*) ev.start;
                        gasnetc_p4_token_t *tokenp = &token;
			char *buf = (void*) (pargs + numargs);
			void *dest_ptr;
                        uint64_t tmp;

                        token.reply_sent = 0;
                        token.sourceid = ev.initiator.rank;

                        gasneti_assert(ev.mlength ==
                                       GASNETI_ALIGNUP(numargs * sizeof(gasnet_handlerarg_t)
                                                         + sizeof(uint64_t) + nbytes,
                                                       GASNETI_MEDBUF_ALIGNMENT));

                        memcpy(&tmp, buf, sizeof(uint64_t));
                        dest_ptr = (void*) tmp;
			buf += sizeof(uint64_t);

			memcpy(dest_ptr, buf, nbytes);

#ifdef P4_DEBUG
                        fprintf(stderr, "%d: firing long (packed) handler from %d\n",
                                (int) gasneti_mynode, (int) token.sourceid);
#endif
                        GASNETI_RUN_HANDLER_LONG(isReq, handler, gasnetc_handler[handler],
                                                 tokenp, pargs, numargs, dest_ptr, nbytes);
		    } else if (IS_AM_LONG(ev.match_bits)) {
		      /* the header part of a long message */
		      const gasnetc_key_t key = LONG_HASH(ev.hdr_data, ev.initiator.rank);
		      p4_long_match_t *match = p4_find_long_match(key);

                      gasneti_assert(ev.mlength ==
                                     GASNETI_ALIGNUP(numargs * sizeof(gasnet_handlerarg_t),
                                                     GASNETI_MEDBUF_ALIGNMENT));

		      match->handler = handler;
		      memcpy(match->pargs, ev.start, sizeof(gasnet_handlerarg_t) * numargs);
		      match->numargs = numargs;

		      if (2 == gasneti_weakatomic_add(&match->op_count, 1, 0)) {
			gasnetc_p4_token_t token;
			gasnetc_p4_token_t *tokenp = &token;

			gasnetc_hash_remove(p4_long_hash, key);

			token.reply_sent = 0;
			token.sourceid = ev.initiator.rank;

#ifdef P4_DEBUG
                        fprintf(stderr, "%d: firing long handler from %d\n",
                                (int) gasneti_mynode, (int) token.sourceid);
#endif
			GASNETI_RUN_HANDLER_LONG(isReq, match->handler, 
						 gasnetc_handler[match->handler],
						 tokenp, match->pargs, match->numargs,
						 match->dest_ptr, match->nbytes);
		        p4_free_long_match(match);
                      }
                    } else {
                        gasneti_fatalerror("unknown active message type.  MB: 0x%lx",
                                           (unsigned long)ev.match_bits);
                    }
#ifdef GASNET_PAR
                    gasneti_weakatomic_add(&block->op_count, 1, 0);
#endif

                } else {
		  /* the payload part of a long message */
		  int isReq = ((IS_AM_REQUEST(ev.match_bits)) ? 1 : 0);
		  const gasnetc_key_t key = LONG_HASH(ev.hdr_data, ev.initiator.rank);
		  p4_long_match_t *match = p4_find_long_match(key);

		  match->dest_ptr = ev.start;
		  match->nbytes = ev.mlength;

		  if (2 == gasneti_weakatomic_add(&match->op_count, 1, 0)) {
		    gasnetc_p4_token_t token;
		    gasnetc_p4_token_t *tokenp = &token;

		    gasnetc_hash_remove(p4_long_hash, key);

		    token.reply_sent = 0;
		    token.sourceid = ev.initiator.rank;

#ifdef P4_DEBUG
                    fprintf(stderr, "%d: firing long handler from %d\n",
                            (int) gasneti_mynode, (int) token.sourceid);
#endif
		    GASNETI_RUN_HANDLER_LONG(isReq, match->handler, 
					     gasnetc_handler[match->handler],
					     tokenp, match->pargs, match->numargs,
					     match->dest_ptr, match->nbytes);
		    p4_free_long_match(match);
		  }
                }
            }
            break;

        case PTL_EVENT_SEND:
            /* SEND has finished.  In general, we disable the send
               events, but some implementations will generate them
               during error conditions.  Ignore except when there is
               an error. */
            if_pf (PTL_OK != ev.ni_fail_type) {
                gasneti_fatalerror("[%03d] event of type %d failed %d", 
                                   gasneti_mynode, ev.type, ev.ni_fail_type);
            }
            break;

        case PTL_EVENT_ACK:
            if_pf (PTL_NI_PT_DISABLED == ev.ni_fail_type) {
                ptl_process_t proc;
                ptl_handle_md_t md_h;
                void *base;

                if (OP_TYPE_AM == OP_GET_TYPE(ev.user_ptr)) {
                    p4_frag_am_t *am_frag = (p4_frag_am_t*) OP_GET_PTR(ev.user_ptr);
                    proc.rank = am_frag->rank;
#ifdef P4_DEBUG
                    fprintf(stderr, "%d: retransmitting am message to %d\n",
                            (int) gasneti_mynode, (int) proc.rank);
#endif
                    p4_get_md(am_frag->data, am_eq_md_h, &md_h, &base);
                    ret = PtlPut(md_h,
                                 (ptl_size_t) ((char*) am_frag->data - (char*) base),
                                 am_frag->data_length,
                                 PTL_ACK_REQ,
                                 proc,
                                 am_idx,
                                 am_frag->match_bits,
                                 0,
                                 OP_USER_PTR_BUILD(OP_TYPE_AM, am_frag),
                                 am_frag->hdr_data);
                    if_pf (PTL_OK != ret) {
                        return p4_failed_put(ret, "resending an AM frag");
                    }
                } else {
                    p4_frag_data_t *data_frag = (p4_frag_data_t*) OP_GET_PTR(ev.user_ptr);
                    proc.rank = data_frag->am_frag->rank;
#ifdef P4_DEBUG
                    fprintf(stderr, "%d: retransmitting data message to %d\n",
                            (int) gasneti_mynode, (int) proc.rank);
#endif
                    ret = PtlPut(data_frag->md_h,
                                 data_frag->local_offset,
                                 data_frag->length,
                                 PTL_ACK_REQ,
                                 proc,
                                 long_data_idx,
                                 data_frag->match_bits,
                                 data_frag->remote_offset,
                                 OP_USER_PTR_BUILD(OP_TYPE_AM_DATA, data_frag),
                                 data_frag->am_frag->hdr_data);
                    if_pf (PTL_OK != ret) {
                        return p4_failed_put(ret, "resending a data frag");
                    }
                }
                /* RDMA operations will never hit a NI_PT_DISABLED state */
            } else if (PTL_OK != ev.ni_fail_type) {
                gasneti_fatalerror("[%03d] event of type %d failed %d", 
                                   gasneti_mynode, ev.type, ev.ni_fail_type);
            } else {
                switch(OP_GET_TYPE(ev.user_ptr)) {
                case OP_TYPE_AM:
                    {
                        p4_frag_am_t *am_frag = (p4_frag_am_t*) OP_GET_PTR(ev.user_ptr);
#ifdef P4_DEBUG
                        fprintf(stderr, "%d: got ack for am to %d\n",
                                (int) gasneti_mynode, (int) am_frag->rank);
#endif
                        gasneti_semaphore_up(&p4_send_credits);
                        if (!IS_AM_LONG(am_frag->match_bits) ||
                            (2 == gasneti_weakatomic_add(&am_frag->op_count, 1, 0))) {
                            p4_free_am_frag(am_frag);
                        }
                    }
                    break;
                case OP_TYPE_AM_DATA:
                    {
                        p4_frag_data_t *data_frag = (p4_frag_data_t*) OP_GET_PTR(ev.user_ptr);
                        p4_frag_am_t *am_frag = data_frag->am_frag;
#ifdef P4_DEBUG
                        fprintf(stderr, "%d: got ack for data to %d\n",
                                (int) gasneti_mynode, (int) am_frag->rank);
#endif
                        gasneti_semaphore_up(&p4_send_credits);

                        if (data_frag->send_complete_ptr) {
                            *(data_frag->send_complete_ptr) = 1;
                        }
                        p4_free_data_frag(data_frag);
                        if (!IS_AM_LONG(am_frag->match_bits) ||
                            (2 == gasneti_weakatomic_add(&am_frag->op_count, 1, 0))) {
                            p4_free_am_frag(am_frag);
                        }
                    }
                    break;
                case OP_TYPE_EOP:
                    {
                        gasnete_eop_t *eop = (gasnete_eop_t*) OP_GET_PTR(ev.user_ptr);
#ifdef P4_DEBUG
                        fprintf(stderr, "%d: got ack for eop %p\n",
                                (int) gasneti_mynode, eop);
#endif
                        gasneti_semaphore_up(&p4_rdma_credits);
                        GASNETE_EOP_MARKDONE(eop);
                    }
                    break;
                case OP_TYPE_IOP:
                    {
                        gasnete_iop_t *iop = (gasnete_iop_t*) OP_GET_PTR(ev.user_ptr);
#ifdef P4_DEBUG
                        fprintf(stderr, "%d: got ack for iop %p\n",
                                (int) gasneti_mynode, iop);
#endif
                        gasneti_semaphore_up(&p4_rdma_credits);
                        gasneti_weakatomic_increment(&(iop->completed_put_cnt), 0);
                    }
                    break;
                }
            }
            
            break;

        case PTL_EVENT_REPLY:
            {
                gasneti_assert(OP_TYPE_IOP == OP_GET_TYPE(ev.user_ptr) ||
                               OP_TYPE_EOP == OP_GET_TYPE(ev.user_ptr));

                switch (OP_GET_TYPE(ev.user_ptr)) {
                case OP_TYPE_EOP:
                    {
                        gasnete_eop_t *eop = (gasnete_eop_t*) OP_GET_PTR(ev.user_ptr);
                        gasneti_semaphore_up(&p4_rdma_credits);
                        GASNETE_EOP_MARKDONE(eop);
                    }
                    break;
                case OP_TYPE_IOP:
                    {
                        gasnete_iop_t *iop = (gasnete_iop_t*) OP_GET_PTR(ev.user_ptr);
                        gasneti_semaphore_up(&p4_rdma_credits);
                        gasneti_weakatomic_increment(&(iop->completed_get_cnt), 0);
                    }
                    break;
                }
            }
            break;

        case PTL_EVENT_GET:
            if_pf (PTL_OK != ev.ni_fail_type) {
                gasneti_fatalerror("[%03d] event of type %d failed %d", 
                                   gasneti_mynode, ev.type, ev.ni_fail_type);
            } else {
                gasneti_fatalerror("[%03d] unexpected event of type %d", 
                                   gasneti_mynode, ev.type);
            }
            break;

        case PTL_EVENT_PT_DISABLED:
            /* Either the receive queue ran out of space or the am
               space filled (note that while the long_data pt can be
               disabled due to the event queue, it can not fill,
               because it's a persistent remote managed put/get ME).
               Drain the receive queue.  If the receive queue
               overflowed, we might end up getting one (and only one)
               more PT_DISABLED, recursing again, but that won't cause
               any problems.  Once the queue is drained, re-enable the
               PT that generated the disabled (if both PTs went down,
               we'll get two events, handling that case
               automagically). */
            ret = p4_poll(eq_ptr, 1, 0); /* Fully drain this queue */
            if_pf (GASNET_OK != ret) gasneti_fatalerror("failed p4_poll() call to drain am_recv_eq");
            ret = PtlPTEnable(matching_ni_h, ev.pt_index);
            if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlPTEnable()");
            break;

        case PTL_EVENT_AUTO_UNLINK:
            {
                p4_am_block_t *block = (p4_am_block_t*) ev.user_ptr;
                ptl_me_t me;
#ifdef GASNET_PAR
                ptl_ct_event_t ct;
                gasneti_weakatomic_val_t val;

                PtlCTGet(block->ct_h, &ct);
                val = gasneti_weakatomic_read(&block->op_count, 0);
                while (ct.success + ct.failure > val) {
                    /* If we're here, there are messages that have
                       been delivered into the ME that have not yet
                       been processed.  Therefore, we need to wait
                       until the other thread finishes processing the
                       events associated with this ME */
                    fprintf(stderr, "%03d: spinning waiting for block to finish %ld, %ld\n",
                            gasneti_mynode, ct.success + ct.failure, (long)val);
                    GASNETI_WAITHOOK();
                    val = gasneti_weakatomic_read(&block->op_count, 0);
                }
                ct.success = ct.failure = 0;
                PtlCTSet(block->ct_h, ct);
                gasneti_weakatomic_set(&block->op_count, 0, 0);
#endif
                
                me.start = block->data;
                me.length = p4_am_size;
                me.min_free = P4_AM_MAX_DATA_LENGTH;
                me.uid = uid;
                me.options = PTL_ME_OP_PUT | PTL_ME_MANAGE_LOCAL |
                    PTL_ME_MAY_ALIGN | PTL_ME_IS_ACCESSIBLE | 
                    PTL_ME_EVENT_LINK_DISABLE;
#ifdef GASNET_PAR
                me.ct_handle = block->ct_h;
                me.options |= PTL_LE_EVENT_CT_COMM;
#else
                me.ct_handle = PTL_CT_NONE;
#endif
                me.match_id.rank = PTL_RANK_ANY;
                me.match_bits = ACTIVE_MESSAGE;
                me.ignore_bits = AM_REQREP_BITS;
                ret = PtlMEAppend(matching_ni_h,
                                  am_idx,
                                  &me,
                                  PTL_PRIORITY_LIST,
                                  block,
                                  &block->me_h);
                if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlMEAppend(relink)");
            }
            break;

        default:
            gasneti_fatalerror("[%03d] Unexpected event type %d",
                               gasneti_mynode, ev.type);
        }
    }

    return GASNET_OK;
}


/* ---------------------------------------------------------------------------------
 * General progress loop
 *
 * Try to progress as many events as possible.  Safe to trigger
 * request or reply handlers from this call.
 * --------------------------------------------------------------------------------- */
void
gasnetc_p4_poll(void)
{
    int ret = p4_poll(eqs_h, 3, p4_poll_limit);
    if_pf (GASNET_OK != ret) {
        gasneti_fatalerror("[%03d] p4_poll returned %d (%s)",
                           gasneti_mynode, ret, gasnet_ErrorName(ret));
    }
}


/* ---------------------------------------------------------------------------------
 * Generic request send
 *
 * Send message of type category to dest, triggering active message on reception
 * --------------------------------------------------------------------------------- */
int
gasnetc_p4_TransferGeneric(int category, ptl_match_bits_t req_type, gasnet_node_t dest, 
                          gasnet_handler_t handler, void *source_addr, size_t nbytes,
                          void *dest_addr, int numargs, va_list argptr)
{
    int ret, i;
    p4_frag_am_t *frag;
    ptl_process_t proc;
    ptl_handle_md_t md_h;
    void *base;
    gasnet_handlerarg_t *arglist;
    uint64_t protocol = 0;
    size_t payload_length = 0;
    int num_credits = (gasnetc_Long == category || gasnetc_LongAsync == category) ? 2 : 1;
    volatile int32_t long_send_complete  = 0;

    /* attempt to get the right number of send credits */
    while (0 == gasneti_semaphore_trydown_n(&p4_send_credits, num_credits)) {
#ifdef P4_DEBUG
        fprintf(stderr, "%d: ran out of send credits\n", (int) gasneti_mynode);
#endif
        /* drain the send queue (make sure to get as many slots as
           possible) */
        ret = p4_poll(&am_send_eq_h, 1, p4_am_eq_len); /* TODO: send_eq len when distinct */
        if (GASNET_OK != ret) return ret;
    }

#ifdef P4_DEBUG
    fprintf(stderr, "%d: TransferGeneric to %d, nbytes = %d, category = %d\n",
            (int) gasneti_mynode, (int) dest, (int) nbytes, (int) category);
#endif

    /* Allocate a send fragment */
    frag = p4_alloc_am_frag();

    /* copy the arguments */
    arglist = (gasnet_handlerarg_t*) frag->data;
    for (i = 0 ; i < numargs ; ++i) {
        arglist[i] = va_arg(argptr, gasnet_handlerarg_t);
        frag->data_length += sizeof(gasnet_handlerarg_t);
    }

    frag->hdr_data = gasneti_weakatomic_add(&p4_op_count, 1, 0);
    proc.rank = frag->rank = dest;

    switch (category) {
    case gasnetc_Short:
        protocol = AM_SHORT;
        payload_length = 0;
        break;

    case gasnetc_Medium:
        protocol = AM_MEDIUM;
        gasneti_assert (nbytes <= gasnet_AMMaxMedium());
        if (nbytes > 0) {
            /* copy payload with appropriate alignment relative to 'data' field */
            const int offset = frag->data_length & (GASNETI_MEDBUF_ALIGNMENT - 1);
#if (GASNETI_MEDBUF_ALIGNMENT == 8)
            /* Use the fact that alignment must be at least 4-bytes */
            gasneti_assert(!offset || (offset == 4));
            frag->data_length += offset;
#else
            /* This is the fully general case */
            frag->data_length += offset ? (GASNETI_MEDBUF_ALIGNMENT - offset) : 0;
#endif
            memcpy(frag->data + frag->data_length, source_addr, nbytes);
	    frag->data_length += nbytes;
        }
        payload_length = nbytes;
        break;
    case gasnetc_Long:
    case gasnetc_LongAsync:
        if ((frag->data_length + nbytes) <= (P4_AM_MAX_DATA_LENGTH - sizeof(uint64_t))) {
            uint64_t tmp = (uintptr_t) dest_addr;
            protocol = AM_LONG_PACKED;

	    /* copy in address where we should deliver the data */
            memcpy(frag->data + frag->data_length, &tmp, sizeof(uint64_t));
	    frag->data_length += sizeof(uint64_t);

            /* copy the payload */
            memcpy(frag->data + frag->data_length, source_addr, nbytes);
            frag->data_length += nbytes;
            payload_length = nbytes;

            /* release the credits for the long put, since we ended up
               not needing them.  also, mark the long send complete
               for the same reason */
            long_send_complete = 1;
            gasneti_semaphore_up(&p4_send_credits);
        } else {
            p4_frag_data_t *data_frag = p4_alloc_data_frag();

            protocol = AM_LONG;
            payload_length = 0;

            p4_get_md(source_addr, am_eq_md_h, &md_h, &base);

            data_frag->am_frag = frag;
            gasneti_weakatomic_set(&frag->op_count, 0, GASNETI_ATOMIC_NONE);
            data_frag->send_complete_ptr = (category == gasnetc_Long) ? &long_send_complete : NULL;
            data_frag->md_h = md_h;
            data_frag->local_offset = (ptl_size_t) ((char*) source_addr - (char*) base);
            data_frag->length = nbytes;
            data_frag->match_bits = CREATE_MATCH_BITS(LONG_DATA, req_type, AM_LONG, 0, 0, 0);
            data_frag->remote_offset = (char*) dest_addr - (char*) gasneti_seginfo[dest].addr;

            /* transfer the payload */
            ret = PtlPut(data_frag->md_h,
                         data_frag->local_offset,
                         data_frag->length,
                         PTL_ACK_REQ,
                         proc,
                         long_data_idx,
                         data_frag->match_bits,
                         data_frag->remote_offset,
                         OP_USER_PTR_BUILD(OP_TYPE_AM_DATA, data_frag),
                         frag->hdr_data);
            if_pf (PTL_OK != ret) {
                return p4_failed_put(ret, "sending a data frag");
            }
        }
        break;
    default:
        gasneti_fatalerror("[%03d]: Unknown message category %d", 
                           (int) gasneti_mynode, category);
    }

    frag->match_bits = CREATE_MATCH_BITS(ACTIVE_MESSAGE, req_type, protocol, numargs, handler, payload_length);

    /* maintain alignment at target */
    frag->data_length = GASNETI_ALIGNUP(frag->data_length, GASNETI_MEDBUF_ALIGNMENT);
    gasneti_assert (frag->data_length <= P4_AM_MAX_DATA_LENGTH);

    /* send the fragment */
    p4_get_md(frag->data, am_eq_md_h, &md_h, &base);
    ret = PtlPut(md_h,
                 (ptl_size_t) ((char*) frag->data - (char*) base),
                 frag->data_length,
                 PTL_ACK_REQ,
                 proc,
                 am_idx,
                 frag->match_bits,
                 0,
                 OP_USER_PTR_BUILD(OP_TYPE_AM, frag),
                 frag->hdr_data);
    if_pf (PTL_OK != ret) {
        return p4_failed_put(ret, "sending an AM frag");
    }

    /* if long, block until the send event */
    while (gasnetc_Long == category && long_send_complete == 0) {
#if GASNET_PSHM
        /* Progress shared-memory Request and Reply queues while we wait */
        gasneti_AMPSHMPoll(0);
#endif
        ret = p4_poll(&am_send_eq_h, 1, p4_am_eq_len);
        if (GASNET_OK != ret) return ret;
        GASNETI_WAITHOOK();
    }

    return GASNET_OK;
}


void
gasnetc_rdma_put(gasnet_node_t node, void *dest, void *src, size_t nbytes,
                 int type, gasnet_handle_t op)
{
    int ret;
    ptl_handle_md_t md_h;
    ptl_process_t proc;
    void *base;

    proc.rank = node;

    while (0 == gasneti_semaphore_trydown_n(&p4_rdma_credits, 1)) {
        /* drain the rdma queue (make sure to get as many slots as
           possible) */
        ret = p4_poll(&rdma_eq_h, 1, p4_rdma_eq_len); /* TODO: send_eq len when distinct */
        if (GASNET_OK != ret) abort();
    }

    p4_get_md(src, rdma_md_hs, &md_h, &base);
    ret = PtlPut(md_h,
                 (ptl_size_t) ((char*) src - (char*) base),
                 nbytes,
                 PTL_ACK_REQ,
                 proc,
                 rdma_idx,
                 0,
                 (char*) dest - (char*) gasneti_seginfo[node].addr,
                 OP_USER_PTR_BUILD(type, op),
                 0);
    if_pf (PTL_OK != ret) {
        p4_failed_put(ret, "put(implicit)");
    }
}


void
gasnetc_rdma_put_wait(gasnet_handle_t oph)
{
    int ret;
    gasnete_op_t *op = (gasnete_op_t*) oph;

    if (OPTYPE(op) == OPTYPE_EXPLICIT) {
        gasnete_eop_t *eop = (gasnete_eop_t *)op;
        while (!GASNETE_EOP_DONE(eop)) {
#if GASNET_PSHM
            /* Progress shared-memory Request and Reply queues while we wait */
            gasneti_AMPSHMPoll(0);
#endif
            ret = p4_poll(&rdma_eq_h, 1, p4_rdma_eq_len);
            if (GASNET_OK != ret) abort();
            GASNETI_WAITHOOK();
        }
    } else {
        gasnete_iop_t *iop = (gasnete_iop_t *)op;
        while (!(GASNETE_IOP_CNTDONE(iop,get) && GASNETE_IOP_CNTDONE(iop,put))) {
#if GASNET_PSHM
            /* Progress shared-memory Request and Reply queues while we wait */
            gasneti_AMPSHMPoll(0);
#endif
            ret = p4_poll(&rdma_eq_h, 1, p4_rdma_eq_len);
            if (GASNET_OK != ret) abort();
            GASNETI_WAITHOOK();
        }
    }
}


void
gasnetc_rdma_get(void *dest, gasnet_node_t node, void * src, size_t nbytes,
                 int type, gasnet_handle_t op)
{
    int ret;
    ptl_handle_md_t md_h;
    ptl_process_t proc;
    void *base;

    proc.rank = node;

    while (0 == gasneti_semaphore_trydown_n(&p4_rdma_credits, 1)) {
        /* drain the rdma queue (make sure to get as many slots as
           possible) */
        ret = p4_poll(&rdma_eq_h, 1, p4_rdma_eq_len); /* TODO: send_eq len when distinct */
        if (GASNET_OK != ret) abort();
    }

    p4_get_md(dest, rdma_md_hs, &md_h, &base);
    ret = PtlGet(md_h,
                 (ptl_size_t) ((char*) dest - (char*) base),
                 nbytes,
                 proc,
                 rdma_idx,
                 0,
                 (char*) src - (char*) gasneti_seginfo[node].addr,
                 OP_USER_PTR_BUILD(type, op));
    if_pf (PTL_OK != ret) {
        p4_failed_put(ret, "put(implicit)");
    }
}


/* ---------------------------------------------------------------------------------
 * Bootstrap barrier function over portals
 * After the network has been initialized, but before all the conduit resources
 * have been allocated, can use this function to perform the equivelent of
 * an MPI_Barrier.  Assumes that barrier has been setup during gasnetc_init
 * --------------------------------------------------------------------------------- */
static void p4_barrier_send(ptl_handle_md_t md_h, int peer)
{
    ptl_process_t proc;
    int ret;

    proc.rank = peer;
    ret = PtlPut(md_h,
                 0,
                 0,
                 PTL_NO_ACK_REQ,
                 proc,
                 bootstrap_idx,
                 BOOTSTRAP_BARRIER_MB,
                 0,
                 NULL,
                 0);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "gasnetc_bootstrapBarrier() PtlPut()");
}
void
gasnetc_bootstrapBarrier(void)
{
    int my_root = (gasneti_mynode == 0) ? 0 : (gasneti_mynode - 1) / 2;
    int left = ((2 * gasneti_mynode + 1) < gasneti_nodes) ? (2 * gasneti_mynode + 1) : -1;
    int right = ((2 * gasneti_mynode + 2) < gasneti_nodes) ? (2 * gasneti_mynode + 2) : -1;
    int count = ((left == -1) ? 0 : 1) + ((right == -1) ? 0 : 1);
    ptl_ct_event_t ct;
    int ret;
    ptl_handle_md_t md_h;
    ptl_md_t md;

    bootstrap_barrier_calls++;

    md.start = NULL;
    md.length = 0;
    md.options = PTL_MD_UNORDERED;
    md.eq_handle = PTL_EQ_NONE;
    md.ct_handle = PTL_CT_NONE;

    ret = PtlMDBind(matching_ni_h, &md, &md_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "gasnetc_bootstrapBarrier() PtlMDBind()");

    if (my_root == gasneti_mynode) {
        /* root */
        ret = PtlCTWait(bootstrap_barrier_ct_h, 
                        bootstrap_barrier_calls * count,
                        &ct);
        if_pf (PTL_OK != ret) p4_fatalerror(ret, "gasnetc_bootstrapBarrier() PtlCTWait(root)");

        if (-1 != left ) { p4_barrier_send(md_h,left ); }
        if (-1 != right) { p4_barrier_send(md_h,right); }
    } else if (0 == count) {
        /* leaf */
        p4_barrier_send(md_h,my_root);

        ret = PtlCTWait(bootstrap_barrier_ct_h, 
                        bootstrap_barrier_calls,
                        &ct);
        if_pf (PTL_OK != ret) p4_fatalerror(ret, "gasnetc_bootstrapBarrier() PtlCTWait(leaf)");
    } else {
        /* middle node */
        const int goal = bootstrap_barrier_calls * (count + 1);

        /* wait for up part */
        ret = PtlCTWait(bootstrap_barrier_ct_h, 
                        goal - 1,
                        &ct);
        if_pf (PTL_OK != ret) p4_fatalerror(ret, "gasnetc_bootstrapBarrier() PtlCTWait(up)");

        p4_barrier_send(md_h,my_root);

        /* wait for down part */
        ret = PtlCTWait(bootstrap_barrier_ct_h, 
                        goal,
                        &ct);
        if_pf (PTL_OK != ret) p4_fatalerror(ret, "gasnetc_bootstrapBarrier() PtlCTWait(down)");

        if (-1 != left ) { p4_barrier_send(md_h,left ); }
        if (-1 != right) { p4_barrier_send(md_h,right); }
    }
    
    ret = PtlMDRelease(md_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "gasnetc_bootstrapBarrier() PtlMDRelease()");
}


/* ---------------------------------------------------------------------------------
 * Bootstrap exchange function over portals
 * After the network has been initialized, but before all the conduit resources
 * have been allocated, can use this function to perform the equivelent of
 * an MPI_Allgather.  Bruck's concatenation algorithm is used here.
 * --------------------------------------------------------------------------------- */
void
gasnetc_bootstrapExchange(void *src, size_t len, void *dest)
{
    ptl_md_t md;
    ptl_me_t me;
    ptl_handle_me_t me_h;
    ptl_handle_md_t md_h;
    ptl_event_t ev;
    ptl_hdr_data_t rcvd = 0;
    ptl_hdr_data_t goal = 0;
    ptl_hdr_data_t hdr_data = 1;
    ptl_size_t offset = len;
    void *temp;
    uint32_t distance;
    int ret, sends = 0;

    GASNETI_TRACE_PRINTF(C,("bootExch with len = %d, src = %p dest = %p",(int)len,src,dest));

    temp = gasneti_malloc(len * gasneti_nodes);

    md.start = temp;
    md.length = len * gasneti_nodes;
    md.options = PTL_MD_UNORDERED;
    md.eq_handle = coll_eq_h;
    md.ct_handle = PTL_CT_NONE;
    ret = PtlMDBind(matching_ni_h, &md, &md_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "gasnetc_bootstrapExchange() PtlMDBind()");

    /* register the temp md with an EQ on a match list */
    me.start = temp;
    me.length = len * gasneti_nodes;
    me.ct_handle = PTL_CT_NONE;
    me.uid = uid;
    me.options = PTL_ME_OP_PUT;
    me.match_id.rank = PTL_RANK_ANY;
    me.match_bits = BOOTSTRAP_EXCHANGE_MB;
    me.ignore_bits = 0;
    me.min_free = 0;
    ret = PtlMEAppend(matching_ni_h, bootstrap_idx, &me, PTL_PRIORITY_LIST, NULL, &me_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "gasnetc_bootstrapExchange() PtlMEAppend()");

    /* ensure ME is linked before the barrier */
    ret = PtlEQWait(coll_eq_h, &ev);
    if (ret != PTL_OK) p4_fatalerror(ret, "gasnetc_bootstrapExchange() PtlEQWait(link)");
    gasneti_assert( ev.type == PTL_EVENT_LINK );

    gasnetc_bootstrapBarrier();

    /* Bruck's Concatenation Algorithm */
    memcpy(temp, src, len);
    for (distance = 1; distance < gasneti_nodes; distance *= 2) {
        ptl_size_t to_xfer;
        gasnet_node_t peer;
        ptl_process_t proc;

        if (gasneti_mynode >= distance) {
            peer = gasneti_mynode - distance;
        } else {
            peer = gasneti_mynode + (gasneti_nodes - distance);
        }

        to_xfer = len * MIN(distance, gasneti_nodes - distance);
        proc.rank = peer;
        ret = PtlPut(md_h, 
                     0, 
                     to_xfer, 
                     PTL_NO_ACK_REQ, 
                     proc,
                     bootstrap_idx,
                     BOOTSTRAP_EXCHANGE_MB,
                     offset,
                     NULL,
                     hdr_data);
        if_pf (PTL_OK != ret) p4_fatalerror(ret, "gasnetc_bootstrapExchange() PtlPut()");

        sends += 1;

        /* wait for completion of the proper receive, and keep count
           of uncompleted sends.  "rcvd" is an accumulator to deal
           with out-of-order receives, which are IDed by the
           hdr_data */
        goal |= hdr_data;
        while ((rcvd & goal) != goal) {
            ret = PtlEQWait(coll_eq_h, &ev);
            switch (ret) {
            case PTL_OK:
                if (ev.type == PTL_EVENT_SEND) {
                    sends -= 1;
                } else {
                    rcvd |= ev.hdr_data;
                    gasneti_assert(ev.type == PTL_EVENT_PUT);
                    gasneti_assert(ev.rlength == ev.mlength);
                    gasneti_assert((ev.rlength == to_xfer) || (ev.hdr_data != hdr_data));
                }
                break;
            default:
                p4_fatalerror(ret, "gasnetc_bootstrapExchange() PtlEQWait(loop)");
            }
        }
        
        hdr_data <<= 1;
        offset += to_xfer;
    }

    /* wait for any SEND_END events not yet seen */
    while (sends) {
        ret = PtlEQWait(coll_eq_h, &ev);
        switch (ret) {
        case PTL_OK:
            gasneti_assert( ev.type == PTL_EVENT_SEND );
            sends -= 1;
            break;
        default:
            p4_fatalerror(ret, "gasnetc_bootstrapExchange() PtlEQWait(final)");
        }
    }

    ret = PtlMDRelease(md_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "gasnetc_bootstrapExchange() PtlMDRelease()");
#if 1
    /* There appears to be a race condition in the reference implementation
     * where we're getting PTL_IN_USE from MEUnlink even though there are no
     * events pending (and if you try to wait for an event, you'll wait
     * forever).  For now, just wait for the implementation to right itself and
     * move on.  -BWB 2013.04.12 */
    /* portals4 issue #28 "Unlink / in_use race" was fixed in svn r2182.
       For now we retain the retry loop "just in case" -PHH 2013.04.18 */
    do {
      ret = PtlMEUnlink(me_h);
      if (PTL_IN_USE == ret) sleep(1);
    } while (PTL_IN_USE == ret);
#else
    ret = PtlMEUnlink(me_h);
#endif
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "gasnetc_bootstrapExchange() PtlMEUnlink()");

    /* now rotate into final position */
    memcpy(dest, (uint8_t*)temp + len * (gasneti_nodes - gasneti_mynode), len * gasneti_mynode);
    memcpy((uint8_t*)dest + len * gasneti_mynode, temp, len * (gasneti_nodes - gasneti_mynode));
    gasneti_free(temp);

    /* should not need a barrier here */
    GASNETI_TRACE_PRINTF(C,("bootExch exit"));
}
