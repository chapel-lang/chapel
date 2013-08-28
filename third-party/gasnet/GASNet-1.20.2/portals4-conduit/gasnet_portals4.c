/*   $Source: /var/local/cvs/gasnet/portals4-conduit/gasnet_portals4.c,v $
 *     $Date: 2013/04/23 01:08:24 $
 * $Revision: 1.30.2.1 $
 * Description: Portals 4 specific configuration
 * Copyright 2012, Sandia National Laboratories
 * Terms of use are as specified in license.txt
 */

#include <gasnet_internal.h>
#include <gasnet_handler.h>
#include <gasnet_core_internal.h>

#include <errno.h>
#include <unistd.h>
#include <signal.h>

#if GASNETI_USE_ALLOCA
#include <alloca.h>
#endif

#include <portals4.h>
#include <portals4/pmi.h>
#include <gasnet_portals4.h>
#include <gasnet_portals4_hash.h>

static ptl_handle_ni_t matching_ni_h;
static ptl_handle_md_t am_eq_md_h;
static ptl_handle_me_t am_me_h;
static ptl_handle_eq_t coll_eq_h;
static ptl_handle_eq_t eqs_h[2];
#define am_send_eq_h eqs_h[0]
#define am_recv_eq_h eqs_h[1]

static ptl_uid_t uid = PTL_UID_ANY;

static ptl_size_t bootstrap_barrier_calls = 0;
static ptl_handle_ct_t bootstrap_barrier_ct_h;
static ptl_handle_me_t bootstrap_barrier_me_h;

static char *kvs_name = NULL, *kvs_key = NULL, *kvs_value = NULL;
static int max_name_len, max_key_len, max_val_len;

/* 16 blocks of 1MB each for AM reception
 * TODO: control these via environment variable
 */
static int p4_am_size = 1 * 1024 * 1024;
static int p4_am_num_entries = 16;

static p4_am_block_t *p4_am_blocks = NULL;
static int p4_am_enabled = 0;
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

static gasneti_weakatomic_t p4_send_credits = gasneti_weakatomic_init(0);
static gasneti_weakatomic_val_t p4_max_send_credits = 0;

static gasneti_weakatomic_t p4_op_count = gasneti_weakatomic_init(0);

#define P4_AM_MAX_DATA_LENGTH                                        \
  GASNETI_ALIGNUP(sizeof(gasnet_handlerarg_t) * gasnet_AMMaxArgs() + \
                  gasnet_AMMaxMedium(), GASNETI_MEDBUF_ALIGNMENT)
#define IS_MED_ALIGNED(x) \
  (0 == ((uintptr_t)(x) & (GASNETI_MEDBUF_ALIGNMENT-1)))


/* Possible encode/decode options include (for a 32-bit ptl_process_t):
 *   hexadecimal: 8 bytes (100% expansion) <- CURRENT
 *   base64: 6 bytes (50% expansion)
 *   asci85: 5 bytes (25% expansion)
 * See http://en.wikipedia.org/wiki/Binary-to-text_encoding
 * If we start getting concerned w/ scalability, we may consider
 * paying the added complexity of the more efficient encodings.
 * However, we can still save space by more shortening of the Keys.
 *    -PHH 2013.04.18
 */

static int
p4_encode(const void *inval, int invallen, char *outval, int outvallen)
{
    static unsigned char encodings[] = {
        '0','1','2','3','4','5','6','7', \
        '8','9','a','b','c','d','e','f' };
    int i;

    if (invallen * 2 + 1 > outvallen) {
        return 1;
    }

    for (i = 0; i < invallen; i++) {
        outval[2 * i] = encodings[((unsigned char *)inval)[i] & 0xf];
        outval[2 * i + 1] = encodings[((unsigned char *)inval)[i] >> 4];
    }

    outval[invallen * 2] = '\0';

    return 0;
}


static int
p4_decode(const char *inval, void *outval, int outvallen)
{
    int i;
    char *ret = (char*) outval;

    if (outvallen != strlen(inval) / 2) {
        return 1;
    }

    for (i = 0 ; i < outvallen ; ++i) {
        if (*inval >= '0' && *inval <= '9') {
            ret[i] = *inval - '0';
        } else {
            ret[i] = *inval - 'a' + 10;
        }
        inval++;
        if (*inval >= '0' && *inval <= '9') {
            ret[i] |= ((*inval - '0') << 4);
        } else {
            ret[i] |= ((*inval - 'a' + 10) << 4);
        }
        inval++;
    }

    return 0;
}


static void
p4_info_put(const char *key, void *value, size_t valuelen) 
{
    snprintf(kvs_key, max_key_len, "gsnt-%lx-%s", (long unsigned) gasneti_mynode, key);
    if (0 != p4_encode(value, valuelen, kvs_value, max_val_len)) {
        gasneti_fatalerror("gasnetc_info_put() encode failed");
    }
    if (PMI_SUCCESS != PMI_KVS_Put(kvs_name, kvs_key, kvs_value)) {
        gasneti_fatalerror("gasnetc_info_put() PMI_KVS_Put() failed");
    }
}


static void
p4_info_get(int pe, const char *key, void *value, size_t valuelen)
{
    snprintf(kvs_key, max_key_len, "gsnt-%lx-%s", (long unsigned) pe, key);
    if (PMI_SUCCESS != PMI_KVS_Get(kvs_name, kvs_key, kvs_value, max_val_len)) {
        gasneti_fatalerror("gasnetc_info_get() PMI_KVS_Get() failed");
    }
    if (0 != p4_decode(kvs_value, value, valuelen)) {
        gasneti_fatalerror("gasnetc_info_get() decode failed");
    }
}


static void
p4_info_exchange(void)
{
    if (PMI_SUCCESS != PMI_KVS_Commit(kvs_name)) {
        gasneti_fatalerror("gasnetc_info_exchange() PMI_KVS_Commit() failed");
    }

    if (PMI_SUCCESS != PMI_Barrier()) {
        gasneti_fatalerror("gasnetc_info_exchange() PMI_Barrier() failed");
    }
}

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

/* ---------------------------------------------------------------------------------
 * Initialize Portals 4 conduit code
 *
 * Initialized memory descriptors for sending active messages
 * (including data part of long messages), match list entries for
 * active message handling, and match list for bootstrap barrier.
 * --------------------------------------------------------------------------------- */
int
gasnetc_p4_init(int *rank, int *size)
{
    int initialized, ret, i;
    ptl_ni_limits_t ni_req_limits, ni_limits;
    ptl_process_t my_id;
    ptl_process_t *desired = NULL;
    ptl_md_t md;
    ptl_me_t me;
    ptl_pt_index_t pt;

    if (PMI_SUCCESS != PMI_Initialized(&initialized)) {
        gasneti_fatalerror("PMI_Initialized() failed");
    }

    if (!initialized) {
        if (PMI_SUCCESS != PMI_Init(&initialized)) {
            gasneti_fatalerror("PMI_Init() failed");
        }
    }

    if (PMI_SUCCESS != PMI_Get_rank(rank)) {
        gasneti_fatalerror("PMI_Get_rank() failed");
    }

    if (PMI_SUCCESS != PMI_Get_size(size)) {
        gasneti_fatalerror("PMI_Get_size() failed");
    }

    /* this is a bit of an abstraction break, but makes debugging
       output so much nicer... */
    gasneti_mynode = *rank;
    gasneti_nodes = *size;

    if (PMI_SUCCESS != PMI_KVS_Get_name_length_max(&max_name_len)) {
        gasneti_fatalerror("PMI_KVS_Get_name_length_max() failed");
    }
    kvs_name = (char*) gasneti_malloc(max_name_len);

    if (PMI_SUCCESS != PMI_KVS_Get_key_length_max(&max_key_len)) {
        gasneti_fatalerror("PMI_KVS_Get_key_length_max() failed");
    }
    kvs_key = (char*) gasneti_malloc(max_key_len);

    if (PMI_SUCCESS != PMI_KVS_Get_value_length_max(&max_val_len)) {
        gasneti_fatalerror("PMI_KVS_Get_value_length_max() failed");
    }
    kvs_value = (char*) gasneti_malloc(max_val_len);

    if (PMI_SUCCESS != PMI_KVS_Get_my_name(kvs_name, max_name_len)) {
        gasneti_fatalerror("PMI_KVS_Get_my_name() failed");
    }

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
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlNiInit()");

#if GASNET_SEGMENT_EVERYTHING
    if ((ni_limits.features & PTL_TARGET_BIND_INACCESSIBLE) == 0) {
        gasneti_fatalerror("[%03d] Portals reports it doesn't support SEGMENT_EVERYTHING",
                           gasneti_mynode);
    }
#endif

    ret = PtlGetPhysId(matching_ni_h, &my_id);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlGetPhysId()");

    /* build id map */
    p4_info_put("ptl4-pid", &my_id, sizeof(my_id));
    p4_info_exchange();
    desired = gasneti_malloc(sizeof(ptl_process_t) * gasneti_nodes);
    for (i = 0 ; i < gasneti_nodes; ++i) {
        p4_info_get(i, "ptl4-pid",
                         &desired[i], sizeof(ptl_process_t));
    }

    gasneti_nodemapInit(NULL, &desired[0].phys.nid,
                        sizeof(desired[0].phys.nid),
                        sizeof(desired[0]));

    ret = PtlSetMap(matching_ni_h,
                    gasneti_nodes,
                    desired);
    if_pf (PTL_OK != ret && PTL_IGNORED != ret) p4_fatalerror(ret, "PtlSetMap()");

    gasneti_free(desired);
    gasneti_free(kvs_name);
    gasneti_free(kvs_key);
    gasneti_free(kvs_value);

    ret = PtlGetUid(matching_ni_h, &uid);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlGetUid()");

    ret = PtlEQAlloc(matching_ni_h, 8192, &am_send_eq_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlEQAlloc(send)");
    /* Every send will generate two events, the SEND event and the ACK
       event.  We don't track them seperately because the retransmit
       logical essentially means we either 1) need to hold both
       credits until the ACK comes in or 2) reacquire credits in the
       retransmit case, which has a whole host of deadlock
       possibilities.  Since most hardware implementations are likely
       to generate the SEND only slightly before the ACK (due to
       end-to-end retransmit), this isn't that big of an optimization
       loss anyway. */
    p4_max_send_credits = 8192 / 2;

    ret = PtlEQAlloc(matching_ni_h, 8192, &am_recv_eq_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlEQAlloc(recv)");

    ret = PtlEQAlloc(matching_ni_h, 1024, &coll_eq_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlEQAlloc(coll)");

    /* allocate portal table entries */
    ret = PtlPTAlloc(matching_ni_h,
                     0,
                     coll_eq_h,
                     COLLECTIVE_PT,
                     &pt);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlPTAlloc(COLLECTIVE)");
    if (COLLECTIVE_PT != pt) {
        gasneti_fatalerror("[%03d] PtlPTAlloc() found bad PT", gasneti_mynode);
    }

    ret = PtlPTAlloc(matching_ni_h,
                     PTL_PT_FLOWCTRL,
                     am_recv_eq_h,
                     AM_PT,
                     &pt);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlPTAlloc(AM)");
    if (AM_PT != pt) {
        gasneti_fatalerror("[%03d] PtlPTAlloc() found bad PT", gasneti_mynode);
    }

    /* Allocate memory descriptor for active message transfer (including long transfers) */
    md.start     = 0;
    md.length    = PTL_SIZE_MAX;
    md.options   = 0;
    md.eq_handle = am_send_eq_h;
    md.ct_handle = PTL_CT_NONE;
    ret = PtlMDBind(matching_ni_h,
                    &md,
                    &am_eq_md_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlMDBind(AM)");

    /* Configure the active message receive resources now, since
       there's no harm in not waiting until the segment code is
       figured out */
    p4_am_blocks = gasneti_malloc(sizeof(p4_am_block_t) * p4_am_num_entries);
    for (i = 0 ; i < p4_am_num_entries ; ++i) {
        void * data = gasneti_malloc(p4_am_size);
        gasneti_assert(IS_MED_ALIGNED(data));
        p4_am_blocks[i].data = data;
        me.start = data;
        me.length = p4_am_size;
        me.min_free = P4_AM_MAX_DATA_LENGTH;
        me.ct_handle = PTL_CT_NONE;
        me.uid = uid;
        me.options = PTL_ME_OP_PUT | PTL_ME_MANAGE_LOCAL |
            PTL_ME_MAY_ALIGN | PTL_ME_IS_ACCESSIBLE | 
            PTL_ME_EVENT_LINK_DISABLE;
        me.match_id.rank = PTL_RANK_ANY;
        me.match_bits = ACTIVE_MESSAGE;
        me.ignore_bits = AM_REQREP_BITS;
        ret = PtlMEAppend(matching_ni_h,
                          AM_PT,
                          &me,
                          PTL_PRIORITY_LIST,
                          &(p4_am_blocks[i]),
                          &p4_am_blocks[i].me_h);
        if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlMEAppend(AM block)");
    }
    p4_am_enabled = 1;

    /* Configure the barrier memory descriptor now, so that we can
       rely on it working from here on out... */
    ret = PtlCTAlloc(matching_ni_h, &bootstrap_barrier_ct_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlCTAlloc()");

    me.start = NULL;
    me.length = 0;
    me.min_free = 0;
    me.ct_handle = bootstrap_barrier_ct_h;
    me.uid = uid;
    me.options = PTL_ME_OP_PUT | PTL_ME_UNEXPECTED_HDR_DISABLE | 
        PTL_ME_EVENT_SUCCESS_DISABLE | PTL_ME_EVENT_CT_COMM | 
        PTL_ME_IS_ACCESSIBLE | 
        PTL_ME_EVENT_LINK_DISABLE | PTL_ME_EVENT_UNLINK_DISABLE;
    me.match_id.rank = PTL_RANK_ANY;
    me.match_bits = BOOTSTRAP_BARRIER_MB;
    me.ignore_bits = 0;
    ret = PtlMEAppend(matching_ni_h,
                      COLLECTIVE_PT,
                      &me,
                      PTL_OVERFLOW_LIST,
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
    ret = PMI_Barrier();
    if (PMI_SUCCESS != ret) gasneti_fatalerror("[%03d] PMI_Barrier() failed: %d", 
                                               gasneti_mynode, ret);

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
    int ret;

#if GASNET_SEGMENT_FAST || GASNET_SEGMENT_LARGE
    gasneti_assert((void*) 0 != segbase && (uintptr_t)-1 != segsize);
    me.start = segbase;
    me.length = segsize;
#elif GASNET_SEGMENT_EVERYTHING
    gasneti_assert((void*) 0 == segbase && (uintptr_t)-1 == segsize);
    me.start = 0;
    me.length = PTL_SIZE_MAX;
#endif

    me.min_free = 0;
    me.ct_handle = PTL_CT_NONE;
    me.uid = uid;
    me.options = PTL_ME_OP_PUT | PTL_ME_EVENT_LINK_DISABLE | 
        PTL_ME_UNEXPECTED_HDR_DISABLE;
    me.match_id.rank = PTL_RANK_ANY;
    me.match_bits = LONG_DATA;
    me.ignore_bits = AM_REQREP_BITS;
    ret = PtlMEAppend(matching_ni_h,
                      AM_PT,
                      &me,
                      PTL_OVERFLOW_LIST,
                      NULL,
                      &am_me_h);
    if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlMEAppend(attach)");

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

    PtlMEUnlink(bootstrap_barrier_me_h);
    PtlCTFree(bootstrap_barrier_ct_h);

    for (i = 0 ; i < p4_am_num_entries ; ++i) {
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

    PtlMDRelease(am_eq_md_h);
    PtlPTFree(matching_ni_h, AM_PT);
    PtlPTFree(matching_ni_h, COLLECTIVE_PT);

    PtlEQFree(coll_eq_h);
    PtlEQFree(am_recv_eq_h);
    PtlEQFree(am_send_eq_h);

#if 0 /* TODO: are these safe to do here? */
    PtlNIFini(matching_ni_h);
    PtlFini();
#endif

#if 0 /* If we make this call then non-collective exits will hang */
    PMI_Finalize();
#endif
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
    frag->base.type = P4_FRAG_TYPE_AM; /* lost while on freelist */
    frag->data_length = 0;
    return frag;
}

GASNETI_INLINE(p4_free_am_frag)
void p4_free_am_frag(p4_frag_am_t *frag)
{
    gasneti_assert(P4_FRAG_TYPE_AM == frag->base.type);
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
    frag->base.type = P4_FRAG_TYPE_DATA; /* lost while on freelist */
    return frag;
}

GASNETI_INLINE(p4_free_data_frag)
void p4_free_data_frag(p4_frag_data_t *frag)
{
    gasneti_assert(P4_FRAG_TYPE_DATA == frag->base.type);
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

static int
p4_poll(const ptl_handle_eq_t *eq_handles, unsigned int size)
{
    ptl_event_t ev;
    int ret;
    unsigned int which;

    while (1) {
        ret = PtlEQPoll(eq_handles, size, 0, &ev, &which);
        if_pt (PTL_EQ_EMPTY == ret) {
            break;
        } else if_pf (PTL_OK != ret) {
            p4_failed_eqpoll(ret, eq_handles[which]);
        }

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

        case PTL_EVENT_SEND:
            /* AM or long data send has arrived.  Don't need to do
               anything if there wasn't an error, since we're doing
               flow control stuff and have to wait for the ack... */
            if_pf (PTL_OK != ev.ni_fail_type) {
                gasneti_fatalerror("[%03d] event of type %d failed %d", 
                                   gasneti_mynode, ev.type, ev.ni_fail_type);
            }
            break;

        case PTL_EVENT_ACK:
            /* AM data has been acked */
            if (PTL_NI_PT_DISABLED == ev.ni_fail_type) {
                p4_frag_t *frag = (p4_frag_t*) ev.user_ptr;
                ptl_process_t proc;

                gasneti_assert(P4_FRAG_TYPE_AM == frag->type ||
                               P4_FRAG_TYPE_DATA == frag->type);

                if (P4_FRAG_TYPE_AM == frag->type) {
                    p4_frag_am_t *am_frag = (p4_frag_am_t*) frag;
                    proc.rank = am_frag->rank;
#ifdef P4_DEBUG
                    fprintf(stderr, "%d: retransmitting am message to %d\n",
                            (int) gasneti_mynode, (int) proc.rank);
#endif
                    ret = PtlPut(am_eq_md_h,
                                 (ptl_size_t) am_frag->data,
                                 am_frag->data_length,
                                 PTL_ACK_REQ,
                                 proc,
                                 AM_PT,
                                 am_frag->match_bits,
                                 0,
                                 am_frag,
                                 am_frag->hdr_data);
                    if_pf (PTL_OK != ret) {
                        return p4_failed_put(ret, "resending an AM frag");
                    }
                } else {
                    p4_frag_data_t *data_frag = (p4_frag_data_t*) frag;
                    proc.rank = data_frag->am_frag->rank;
#ifdef P4_DEBUG
                    fprintf(stderr, "%d: retransmitting data message to %d\n",
                            (int) gasneti_mynode, (int) proc.rank);
#endif
                    ret = PtlPut(am_eq_md_h,
                                 data_frag->local_offset,
                                 data_frag->length,
                                 PTL_ACK_REQ,
                                 proc,
                                 AM_PT,
                                 data_frag->match_bits,
                                 data_frag->remote_offset,
                                 data_frag,
                                 data_frag->am_frag->hdr_data);
                    if_pf (PTL_OK != ret) {
                        return p4_failed_put(ret, "resending a data frag");
                    }
                }
            } else if_pf (PTL_OK != ev.ni_fail_type) {
                gasneti_fatalerror("[%03d] event of type %d failed %d", 
                                   gasneti_mynode, ev.type, ev.ni_fail_type);
            } else {
                p4_frag_t *frag = (p4_frag_t*) ev.user_ptr;
                p4_frag_am_t *am_frag;

                gasneti_assert(P4_FRAG_TYPE_AM == frag->type ||
                               P4_FRAG_TYPE_DATA == frag->type);

                (void) gasneti_weakatomic_subtract(&p4_send_credits, 1, 0);

                if (frag->type == P4_FRAG_TYPE_AM) {
                    am_frag = (p4_frag_am_t*) frag;
#ifdef P4_DEBUG
                    fprintf(stderr, "%d: got ack for am to %d\n",
                            (int) gasneti_mynode, (int) am_frag->rank);
#endif
                } else {
                    p4_frag_data_t *data_frag = (p4_frag_data_t*) frag;
                    am_frag = data_frag->am_frag;
#ifdef P4_DEBUG
                    fprintf(stderr, "%d: got ack for data to %d\n",
                            (int) gasneti_mynode, (int) am_frag->rank);
#endif
                    if (data_frag->send_complete_ptr) {
                        *(data_frag->send_complete_ptr) = 1;
                    }
                    p4_free_data_frag(data_frag);
                }

                if (!IS_AM_LONG(am_frag->match_bits) ||
                    (2 == gasneti_weakatomic_add(&am_frag->op_count, 1, 0))) {
                    p4_free_am_frag(am_frag);
                }
            }
            
            break;

        case PTL_EVENT_PT_DISABLED:
            /* delay doing anything until we're done draining the
               queue (ie, until we get the auto_unlink events) */
            p4_am_enabled = 0;
            break;

        case PTL_EVENT_AUTO_UNLINK:
            {
                p4_am_block_t *block = (p4_am_block_t*) ev.user_ptr;
                ptl_me_t me;
                
                me.start = block->data;
                me.length = p4_am_size;
                me.min_free = P4_AM_MAX_DATA_LENGTH;
                me.ct_handle = PTL_CT_NONE;
                me.uid = uid;
                me.options = PTL_ME_OP_PUT | PTL_ME_MANAGE_LOCAL |
                    PTL_ME_MAY_ALIGN | PTL_ME_IS_ACCESSIBLE | 
                    PTL_ME_EVENT_LINK_DISABLE;
                me.match_id.rank = PTL_RANK_ANY;
                me.match_bits = ACTIVE_MESSAGE;
                me.ignore_bits = AM_REQREP_BITS;
                ret = PtlMEAppend(matching_ni_h,
                                  AM_PT,
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
    if (0 == p4_am_enabled) {
        /* if the pt was disabled, we either ran out of bounce buffers
           or eq space.  We've drained the eq.  We've theoretically
           reposted the buffer.  Let's get going again. */
        ret = PtlPTEnable(matching_ni_h, AM_PT);
        if_pf (PTL_OK != ret) p4_fatalerror(ret, "PtlPTEnable()");
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
    int ret = p4_poll(eqs_h, 2);
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
    gasneti_weakatomic_val_t tmp;
    p4_frag_am_t *frag;
    ptl_process_t proc;
    gasnet_handlerarg_t *arglist;
    uint64_t protocol = 0;
    size_t payload_length = 0;
    int num_credits = (gasnetc_Long == category || gasnetc_LongAsync == category) ? 2 : 1;
    volatile int32_t long_send_complete  = 0;

    /* TODO: Count credits downward and use gasneti_semaphore_trydown_n()?
     *       -PHH 2013.04.18 */
    /* attempt to get the right number of send credits */
 retry:
    tmp = gasneti_weakatomic_add(&p4_send_credits, num_credits, 0);
    if (tmp > p4_max_send_credits) {
#ifdef P4_DEBUG
        fprintf(stderr, "%d: ran out of send credits\n", (int) gasneti_mynode);
#endif
        (void) gasneti_weakatomic_subtract(&p4_send_credits, num_credits, 0);

        /* drain the send queue (make sure to get as many slots as
           possible) */
        ret = p4_poll(&am_send_eq_h, 1);
        if (GASNET_OK != ret) return ret;
        goto retry;
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
            (void) gasneti_weakatomic_subtract(&p4_send_credits, 1, 0);
        } else {
            p4_frag_data_t *data_frag = p4_alloc_data_frag();

            protocol = AM_LONG;
            payload_length = 0;

            data_frag->am_frag = frag;
            gasneti_weakatomic_set(&frag->op_count, 0, GASNETI_ATOMIC_NONE);
            data_frag->send_complete_ptr = (category == gasnetc_Long) ? &long_send_complete : NULL;
            data_frag->local_offset = (ptl_size_t) source_addr;
            data_frag->length = nbytes;
            data_frag->match_bits = CREATE_MATCH_BITS(LONG_DATA, req_type, AM_LONG, 0, 0, 0);
            data_frag->remote_offset = (char*) dest_addr - (char*) gasneti_seginfo[dest].addr;

            /* transfer the payload */
            ret = PtlPut(am_eq_md_h,
                         data_frag->local_offset,
                         data_frag->length,
                         PTL_ACK_REQ,
                         proc,
                         AM_PT,
                         data_frag->match_bits,
                         data_frag->remote_offset,
                         data_frag,
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
    ret = PtlPut(am_eq_md_h,
                 (ptl_size_t) frag->data,
                 frag->data_length,
                 PTL_ACK_REQ,
                 proc,
                 AM_PT,
                 frag->match_bits,
                 0,
                 frag,
                 frag->hdr_data);
    if_pf (PTL_OK != ret) {
        return p4_failed_put(ret, "sending an AM frag");
    }

    /* if long, block until the send event */
    while (gasnetc_Long == category && long_send_complete == 0) {
        ret = p4_poll(&am_send_eq_h, 1);
        if (GASNET_OK != ret) return ret;
#if GASNET_PSHM
        /* Progress shared-memory Request and Reply queues while we wait */
        gasneti_AMPSHMPoll(0);
#endif
        GASNETI_WAITHOOK();
    }

    return GASNET_OK;
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
                 COLLECTIVE_PT,
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
    ret = PtlMEAppend(matching_ni_h, COLLECTIVE_PT, &me, PTL_PRIORITY_LIST, NULL, &me_h);
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
                     COLLECTIVE_PT,
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
