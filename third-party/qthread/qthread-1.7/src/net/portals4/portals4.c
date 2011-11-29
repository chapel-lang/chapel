#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>
#include <portals4.h>
#include <pthread.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "net/net.h"
#include "net/driver.h"
#include "runtime.h"
#include "qt_mpool.h"

struct recv_block_t {
    void *start;
    ptl_handle_me_t me_h;
};

struct net_pkt_t {
    int peer;
    int tag;
    void *start;
    size_t len;
};

static void* progress_function(void *data);
static void exit_handler(int tag, void *start, size_t len);
static void repost_recv_block(struct recv_block_t*);
static inline int transmit_packet(struct net_pkt_t *pkt);

static qthread_internal_net_driver_handler handlers[256];
static struct recv_block_t *recv_blocks = NULL;
static int num_recv_blocks = 0;
static int size_recv_block = 0;
static int max_msg_size = 0;
static ptl_handle_ni_t ni_h;
static ptl_handle_eq_t eq_h;
static ptl_handle_md_t md_h;
static ptl_handle_me_t overflow_me_h;
static ptl_pt_index_t data_pt;
static pthread_t progress_thread;
static qt_mpool mpool;
static int shutdown_msg = 1;

#define GET_ENV_NUM(envariable, num, dflt, zerodflt) do {                \
        char         *str = getenv(envariable);                          \
        unsigned long tmp = dflt;                                        \
        if (str && *str) {                                               \
            char *errptr;                                                \
            tmp = strtoul(str, &errptr, 0);                              \
            if (*errptr != 0) {                                          \
                fprintf(stderr, "unparsable "envariable " (%s)\n", str); \
                tmp = dflt;                                              \
            }                                                            \
            if (tmp == 0) { tmp = zerodflt; }                            \
        }                                                                \
        num = tmp;                                                       \
} while (0)

int
qthread_internal_net_driver_initialize(void)
{
    int ret, i;
    ptl_process_t *desired = NULL;
    ptl_uid_t uid = PTL_UID_ANY;
    ptl_ni_limits_t ni_limits, ni_req_limits;
    ptl_md_t md;
    ptl_me_t me;

    GET_ENV_NUM("QTHREAD_PORTALS_RECV_BLOCKS", num_recv_blocks, 3, 3);
    GET_ENV_NUM("QTHREAD_PORTALS_RECV_BLOCK_SIZE", size_recv_block, 1024 * 1024, 1024 * 1024);
    GET_ENV_NUM("QTHREAD_PORTALS_MAX_MSG_SIZE", max_msg_size, 4096, 4096);

    fprintf(stderr, "begin net_driver_init\n");

    printf("max msg size %d\n", max_msg_size);
    printf("recv_block_size %d\n", size_recv_block);

    mpool = qt_mpool_create(sizeof(struct net_pkt_t));

    ret = PtlInit();
    if (PTL_OK != ret) {
        fprintf(stderr, "PtlInit: %d\n", ret);
        return ret;
    }

    /* initialize runtime */
    ret = qthread_internal_net_driver_runtime_init();
    if (0 != ret) {
        fprintf(stderr, "internal_net_driver_runtime_init: %d\n", ret);
        return ret;
    }

    desired = qthread_internal_net_driver_runtime_get_mapping();
    if (NULL == desired) {
        fprintf(stderr, "internal_net_driver_runtime_get_mapping: %d\n", errno);
        return -1;
    }

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
    ni_req_limits.max_atomic_size = 512;
    ni_req_limits.max_fetch_atomic_size = 512;
    ni_req_limits.max_waw_ordered_size = 512;
    ni_req_limits.max_war_ordered_size = 512;
    ni_req_limits.max_volatile_size = 512;
    ni_req_limits.features = 0;

    ret = PtlNIInit(PTL_IFACE_DEFAULT,
                    PTL_NI_MATCHING | PTL_NI_LOGICAL,
                    PTL_PID_ANY,
                    &ni_req_limits,
                    &ni_limits,
                    &ni_h);
    if (PTL_OK != ret) {
        fprintf(stderr, "PtlNIInit: %d\n", ret);
        return ret;
    }

    for (i = 0 ; i < qthread_internal_net_driver_get_size() ; ++i) {
        fprintf(stderr, "map[%d] = %d,%d\n", i, desired[i].phys.nid, desired[i].phys.pid);
    }

    ret = PtlSetMap(ni_h,
                    qthread_internal_net_driver_get_size(),
                    desired);
    if (PTL_OK != ret && PTL_IGNORED != ret) {
        fprintf(stderr, "PtlSetMap: %d\n", ret);
        return ret;
    }

    ret = PtlGetUid(ni_h, &uid);
    if (PTL_OK != ret) {
        fprintf(stderr, "PtlGetUid: %d\n", ret);
        return ret;
    }

    /* create completion EQ/CT and portal table entry */
    ret = PtlEQAlloc(ni_h, 1024, &eq_h);
    if (PTL_OK != ret) {
        fprintf(stderr, "PtlEQAlloc: %d\n", ret);
        return ret;
    }

    ret = PtlPTAlloc(ni_h,
                     0,
                     eq_h,
                     19,
                     &data_pt);
    if (PTL_OK != ret) {
        fprintf(stderr, "PtlPTAlloc: %d\n", ret);
        return ret;
    }

    /* post truncating me */
    me.start = NULL;
    me.length = 0;
    me.ct_handle = PTL_CT_NONE;
    me.min_free = 0;
    me.ac_id.uid = PTL_UID_ANY;
    me.options = PTL_ME_OP_PUT | 
        PTL_ME_UNEXPECTED_HDR_DISABLE;
    me.match_id.rank = PTL_RANK_ANY;
    me.match_bits = 0x0;
    me.ignore_bits = ~(0x0ULL);
    ret = PtlMEAppend(ni_h,
                      data_pt,
                      &me,
                      PTL_OVERFLOW,
                      NULL,
                      &overflow_me_h);
    if (PTL_OK != ret) {
        fprintf(stderr, "PtlMEAppend: %d\n", ret);
        return ret;
    }

    /* post receive mes */
    recv_blocks = malloc(sizeof(struct recv_block_t) * num_recv_blocks);
    if (NULL == recv_blocks) return -1;
    for (i = 0 ; i < num_recv_blocks ; ++i) {
        recv_blocks[i].start = malloc(size_recv_block);
        if (NULL == recv_blocks[i].start) return -1;
        recv_blocks[i].me_h = PTL_INVALID_HANDLE;
        repost_recv_block(&recv_blocks[i]);
    }

    md.start = 0;
    md.length = SIZE_MAX;
    md.options = PTL_MD_UNORDERED;
    md.eq_handle = eq_h;
    md.ct_handle = PTL_CT_NONE;
    ret = PtlMDBind(ni_h,
                    &md,
                    &md_h);
    if (PTL_OK != ret) {
        fprintf(stderr, "PtlMDBind: %d\n", ret);
        return ret;
    }
    
    /* spawn the management thread */
    ret = pthread_create(&progress_thread, NULL, progress_function, NULL);
    if (0 != ret) {
        fprintf(stderr, "pthread_create: %d\n", ret);
        return ret;
    }

    handlers[0] = exit_handler;

    /* finish up */
    qthread_internal_net_driver_runtime_barrier();

    fprintf(stderr, "end net_driver_init\n");

    return 0;
}


int
qthread_internal_net_driver_send(int peer, int tag, 
                                 void *start, size_t len)
{
    struct net_pkt_t *pkt;

    if (tag <= 0 || tag >= 256) { return -1; }

    pkt = qt_mpool_alloc(mpool);
    pkt->peer = peer;
    pkt->tag = tag;
    pkt->start = start;
    pkt->len = len;

    return transmit_packet(pkt);
}


int
qthread_internal_net_driver_register(int tag, 
                                     qthread_internal_net_driver_handler handler)
{
    if (tag <= 0 || tag >= 256) { return -1; }
    handlers[tag] = handler;
    return 0;
}


int
qthread_internal_net_driver_finalize(void)
{
    int ret;
    struct net_pkt_t *pkt;
    void *dummy;
    

    /* mark progress thread as time to go away, notify it, and wait */
    pkt = qt_mpool_alloc(mpool);
    pkt->peer = qthread_internal_net_driver_get_rank();
    pkt->tag = 0;
    pkt->start = &shutdown_msg;
    pkt->len = sizeof(shutdown_msg);

    ret = transmit_packet(pkt);
    if (0 != ret) {
        fprintf(stderr, "shutdown internal send: %d\n", ret);
        return ret;
    }

    pthread_join(progress_thread, &dummy);

    qthread_internal_net_driver_runtime_fini();

    PtlFini();

    qt_mpool_destroy(mpool);

    return 0;
}


static void*
progress_function(void *data)
{
    int ret;
    ptl_event_t ev;

    fprintf(stderr, "begin progress function\n");
    while (1) {
        ret = PtlEQWait(eq_h, &ev);
        if (PTL_OK != ret) {
            fprintf(stderr, "PtlEQWait: %d\n", ret);
        }
        fprintf(stderr, "Handling event type %d\n", ev.type);
        switch (ev.type) {
        case PTL_EVENT_PUT:
            assert(ev.mlength == ev.rlength);
            handlers[ev.hdr_data](ev.hdr_data, ev.start, ev.mlength);
            break;
        case PTL_EVENT_SEND:
            if (PTL_OK != ev.ni_fail_type) {
                fprintf(stderr, "SEND event with fail type %d\n", ev.ni_fail_type);
            }
            break;
        case PTL_EVENT_ACK:
            if (PTL_OK != ev.ni_fail_type) {
                fprintf(stderr, "SEND event with fail type %d\n", ev.ni_fail_type);
            } else {
                struct net_pkt_t *pkt = (struct net_pkt_t*) ev.user_ptr;
                if (ev.mlength != pkt->len) {
                    ret = transmit_packet(pkt);
                    if (PTL_OK != ret) {
                        fprintf(stderr, "transmit packet failed: %d\n", ret);
                    }
                } else {
                    qt_mpool_destroy(mpool);
                }
            }
            break;
        default:
            fprintf(stderr, "Unexpected event type %d\n", ev.type);
        }
    }
    return NULL;
}


static void
exit_handler(int tag, void *start, size_t len)
{
    fprintf(stderr, "end progress function\n");
    pthread_exit(NULL);
}


static void
repost_recv_block(struct recv_block_t* block)
{
    ptl_me_t me;
    int ret;

    printf("recv block posted\n");
    me.start = block->start;
    me.length = size_recv_block;
    me.ct_handle = PTL_CT_NONE;
    me.min_free = max_msg_size;
    me.ac_id.uid = PTL_UID_ANY;
    me.options = PTL_ME_OP_PUT | 
        PTL_ME_MANAGE_LOCAL | 
        PTL_ME_NO_TRUNCATE | 
        PTL_ME_MAY_ALIGN;
    me.match_id.rank = PTL_RANK_ANY;
    me.match_bits = 0;
    me.ignore_bits = ~(0ULL);

    ret = PtlMEAppend(ni_h,
                      data_pt,
                      &me,
                      PTL_PRIORITY_LIST,
                      block,
                      &block->me_h);
    if (PTL_OK != ret) abort();
}

static inline int
transmit_packet(struct net_pkt_t *pkt)
{
    int ret;
    ptl_process_t peer;

    peer.rank = pkt->peer;
    printf("sending to rank %d %d\n", pkt->peer, (int) pkt->len);
    ret = PtlPut(md_h,
                 (ptl_size_t) pkt->start,
                 pkt->len,
                 PTL_ACK_REQ,
                 peer,
                 data_pt,
                 0,
                 0,
                 pkt,
                 pkt->tag);
    if (PTL_OK != ret) {
        fprintf(stderr, "PtlPut failed: %d\n", ret);
    }

    return ret;
}
