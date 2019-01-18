#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdio.h>
#include <stdlib.h>

/* Public Headers */
#include "qthread/qthread.h"
#include "qthread/multinode.h"

/* Internal Headers */
#include "qt_multinode_innards.h"
//#include "qthread_innards.h"
#include "qt_debug.h"
#include "qt_atomics.h"
#include "qt_subsystems.h"
#include "net/net.h"
#include "qt_hash.h" /* for qt_hash */
#include "qt_asserts.h"

static int       my_rank;
static int       world_size;
static qt_hash   uid_to_ptr_hash;
static qt_hash   ptr_to_uid_hash;
static aligned_t num_ended   = 0;
static aligned_t time_to_die = 0;
static int       initialized = 0;

struct fork_msg_t {
    uint64_t return_addr;
    uint32_t uid;
    int32_t  origin_node;
    int32_t  arg_len;
    char     args[FORK_MSG_PAYLOAD];
};

struct fork_long_msg_t {
    uint64_t return_addr;
    uint32_t uid;
    int32_t  origin_node;
    int32_t  arg_len;
    char     args[];
};

struct return_msg_t {
    uint64_t return_addr;
    uint64_t return_val;
};

struct die_msg_t {
    uint64_t my_rank;
};

#define SHORT_MSG_TAG       0x1
#define LONG_MSG_TAG        0x6
#define RETURN_MSG_TAG      0x4
#define RETURN_LONG_MSG_TAG 0x7
#define DIE_MSG_TAG         0x3

static void net_cleanup(void)
{
    qthread_debug(MULTINODE_FUNCTIONS, "[%d] begin net_cleanup\n", my_rank);

    if (my_rank == 0) {
        int i;
        for (i = 1; i < world_size; ++i) {
            struct die_msg_t msg;

            msg.my_rank = my_rank;
            qthread_debug(MULTINODE_DETAILS, "[%d] sending die message to %d\n", my_rank, i);
            qthread_internal_net_driver_send(i, DIE_MSG_TAG, &msg, sizeof(msg));
        }

        while (num_ended != world_size - 1) SPINLOCK_BODY();
    }

    qthread_internal_net_driver_finalize();

    qt_hash_destroy(uid_to_ptr_hash);
    qt_hash_destroy(ptr_to_uid_hash);

    qthread_debug(MULTINODE_FUNCTIONS, "[%d] end net_cleanup\n", my_rank);
}

static aligned_t fork_helper(void *info)
{
    struct fork_msg_t *msg = (struct fork_msg_t *)info;
    aligned_t          ret;
    qthread_f          f;

    qthread_debug(MULTINODE_FUNCTIONS, "[%d] begin fork_helper\n", my_rank);

    f = qt_hash_get(uid_to_ptr_hash, (qt_key_t)(uintptr_t)msg->uid);
    if (NULL != f) {
        ret = f(msg->args);

        if (0 != msg->return_addr) {
            struct return_msg_t ret_msg;
            ret_msg.return_addr = msg->return_addr;
            ret_msg.return_val  = ret;
            qthread_debug(MULTINODE_DETAILS, "[%d] sending return msg 0x%lx, %ld\n",
                          my_rank, ret_msg.return_addr, ret_msg.return_val);
            qthread_internal_net_driver_send(msg->origin_node, RETURN_MSG_TAG,
                                             &ret_msg, sizeof(ret_msg));
        }
    } else {
        fprintf(stderr, "action uid %d not registered at destination\n", msg->uid);
        abort();
    }

    qthread_debug(MULTINODE_FUNCTIONS, "[%d] end fork_helper\n", my_rank);

    return 0;
}

static aligned_t fork_long_helper(void *info)
{
    struct fork_long_msg_t *msg = (struct fork_long_msg_t *)info;
    aligned_t          ret;
    qthread_f          f;

    qthread_debug(MULTINODE_FUNCTIONS, "[%d] begin fork_helper\n", my_rank);

    f = qt_hash_get(uid_to_ptr_hash, (qt_key_t)(uintptr_t)msg->uid);
    if (NULL != f) {
        ret = f(msg->args);

        if (0 != msg->return_addr) {
            struct return_msg_t ret_msg;
            ret_msg.return_addr = msg->return_addr;
            ret_msg.return_val  = ret;
            qthread_debug(MULTINODE_DETAILS, "[%d] sending return msg 0x%lx, %ld\n",
                          my_rank, ret_msg.return_addr, ret_msg.return_val);
            qthread_internal_net_driver_send(msg->origin_node, RETURN_MSG_TAG,
                                             &ret_msg, sizeof(ret_msg));
        }
    } else {
        fprintf(stderr, "action uid %d not registered at destination\n", msg->uid);
        abort();
    }

    qthread_debug(MULTINODE_FUNCTIONS, "[%d] end fork_helper\n", my_rank);

    return 0;
}

static void fork_msg_handler(int    tag,
                             void  *start,
                             size_t len)
{
    qthread_debug(MULTINODE_FUNCTIONS, "[%d] begin fork_msg_handler\n", my_rank);

    if (sizeof(struct fork_msg_t) > len) { abort(); }

    qthread_fork_copyargs(fork_helper, start, len, NULL);

    qthread_debug(MULTINODE_FUNCTIONS, "[%d] end fork_msg_handler\n", my_rank);
}

static void fork_long_msg_handler(int    tag,
                                  void  *start,
                                  size_t len)
{
    qthread_debug(MULTINODE_FUNCTIONS, "[%d] begin fork_long_msg_handler\n", my_rank);

    qthread_fork_copyargs(fork_long_helper, start, len, NULL);

    qthread_debug(MULTINODE_FUNCTIONS, "[%d] end fork_long_msg_handler\n", my_rank);
}

static void return_msg_handler(int    tag,
                               void  *start,
                               size_t len)
{
    struct return_msg_t *msg = (struct return_msg_t *)start;

    qthread_debug(MULTINODE_FUNCTIONS, "[%d] begin return_msg_handler 0x%lx, %ld\n",
                  my_rank, (unsigned long)msg->return_addr, msg->return_val);

    qthread_writeF_const((aligned_t *)msg->return_addr, msg->return_val);

    qthread_debug(MULTINODE_FUNCTIONS, "[%d] end return_msg_handler\n",
                  my_rank);
}

static void return_long_msg_handler(int    tag,
                                    void  *start,
                                    size_t len)
{
    struct return_msg_t *msg = (struct return_msg_t *)start;

    qthread_debug(MULTINODE_FUNCTIONS, "[%d] begin return_long_msg_handler 0x%lx, %ld\n",
                  my_rank, (unsigned long)msg->return_addr, msg->return_val);

    qthread_writeF_const((aligned_t *)msg->return_addr, msg->return_val);

    // TODO: figure out how to free long message. This is only called when there is
    //       a return value.

    qthread_debug(MULTINODE_FUNCTIONS, "[%d] end return_long_msg_handler\n",
                  my_rank);
}

static void die_msg_handler(int    tag,
                            void  *start,
                            size_t len)
{
    qthread_debug(MULTINODE_FUNCTIONS, "[%d] begin die_msg_handler\n", my_rank);

    if (my_rank == 0) {
        num_ended++;
    } else {
        qthread_writeF_const(&time_to_die, 1);
    }

    qthread_debug(MULTINODE_FUNCTIONS, "[%d] end die_msg_handler\n", my_rank);
}

int qthread_multinode_initialize(void)
{
    int ret;

    qthread_debug(MULTINODE_CALLS, "begin qthread_multinode_initialize\n");

    /* initialize structures */
    initialized     = 1;
    my_rank         = world_size = -1;
    uid_to_ptr_hash = qt_hash_create(0);
    ptr_to_uid_hash = qt_hash_create(0);

    qthread_internal_net_driver_register(SHORT_MSG_TAG, fork_msg_handler);
    qthread_internal_net_driver_register(LONG_MSG_TAG, fork_long_msg_handler);
    qthread_internal_net_driver_register(RETURN_MSG_TAG, return_msg_handler);
    qthread_internal_net_driver_register(RETURN_LONG_MSG_TAG, return_long_msg_handler);
    qthread_internal_net_driver_register(DIE_MSG_TAG, die_msg_handler);

    /* initialize the network driver and provie barrier */
    ret = qthread_internal_net_driver_initialize();
    if (0 != ret) {
        qthread_debug(MULTINODE_FUNCTIONS, "qthread_internal_net_driver_init failed: %d\n", ret);
        return ret;
    }

    my_rank    = qthread_internal_net_driver_get_rank();
    world_size = qthread_internal_net_driver_get_size();

    if (0 != my_rank) {
        qthread_empty(&time_to_die);
    }

    /* make sure we can clean up */
    qthread_internal_cleanup_early(net_cleanup);

    qthread_debug(MULTINODE_CALLS, "[%d] end qthread_multinode_initialize\n", my_rank);

    return QTHREAD_SUCCESS;
}

int qthread_multinode_run(void)
{
    aligned_t val;

    if (0 == initialized) { return 1; }

    qthread_debug(MULTINODE_CALLS, "[%d] begin qthread_multinode_run\n", my_rank);

    qthread_internal_net_driver_barrier();

    if (0 != my_rank) {
        struct die_msg_t msg;

        qthread_readFF(&val, &time_to_die);
        qthread_debug(MULTINODE_DETAILS, "[%d] time to die\n", my_rank);
        msg.my_rank = my_rank;
        qthread_internal_net_driver_send(0, DIE_MSG_TAG, &msg, sizeof(msg));
        qthread_finalize();
        exit(0);
    }

    qthread_debug(MULTINODE_CALLS, "[%d] end qthread_multinode_run\n", my_rank);

    return QTHREAD_SUCCESS;
}

int qthread_multinode_multistart(void)
{
    if (0 == initialized) { return 1; }

    qthread_debug(MULTINODE_CALLS, "[%d] begin qthread_multinode_multistart\n", my_rank);

    qthread_internal_net_driver_barrier();

    qthread_debug(MULTINODE_CALLS, "[%d] end qthread_multinode_multistart\n", my_rank);

    return QTHREAD_SUCCESS;
}

int qthread_multinode_multistop(void)
{
    aligned_t val;

    qthread_debug(MULTINODE_CALLS, "[%d] begin qthread_multinode_multistop\n", my_rank);

    if (0 != my_rank) {
        struct die_msg_t msg;

        qthread_readFF(&val, &time_to_die);
        qthread_debug(MULTINODE_DETAILS, "[%d] time to die\n", my_rank);
        msg.my_rank = my_rank;
        qthread_internal_net_driver_send(0, DIE_MSG_TAG, &msg, sizeof(msg));

        exit(0); // triggers atexit(net_cleanup)
    }

    qthread_debug(MULTINODE_CALLS, "[%d] end qthread_multinode_multistop\n", my_rank);

    return QTHREAD_SUCCESS;
}

int qthread_multinode_rank(void)
{
    return my_rank;
}

int qthread_multinode_size(void)
{
    return world_size;
}

int qthread_multinode_register(uint32_t  uid,
                               qthread_f f)
{
    int ret = 0;

    if (0 == initialized) { return 1; }

    qthread_debug(MULTINODE_CALLS, "[%d] begin qthread_multinode_register(uid=%d, ptr=0x%lx)\n",
                  my_rank, uid, (unsigned long)f);

    // Check that UID is strictly positive
    if (0 == uid) {
        fprintf(stderr, "invalid action id %d: must be > 0", uid);
        return 1;
    }

    // Check that UID is not used more than once
    if (NULL != qt_hash_get(uid_to_ptr_hash, (qt_key_t)(uintptr_t)uid)) {
        fprintf(stderr, "duplicate registration of action uid %d\n", uid);
        return 1;
    }

    ret = qt_hash_put(uid_to_ptr_hash, (qt_key_t)(uintptr_t)uid, f);
    if (ret != 1) {
        qthread_debug(MULTINODE_DETAILS, "[%d] uid -> ptr registration failed\n",
                      my_rank);
        return 1;
    }

    // Check that function pointer is not used more than once
    if (NULL != qt_hash_get(ptr_to_uid_hash, f)) {
        fprintf(stderr, "duplicate registration of action function 0x%lx\n",
                (unsigned long)f);
        return 1;
    }

    ret = qt_hash_put(ptr_to_uid_hash, f, (void *)(uintptr_t)uid);
    if (ret != 1) {
        qthread_debug(MULTINODE_DETAILS, "[%d] ptr -> uid registration failed\n",
                      my_rank);
        return 1;
    }

    qthread_debug(MULTINODE_CALLS, "[%d] end qthread_multinode_register\n",
                  my_rank);

    return 0;
}

int qthread_multinode_action_uid(qthread_f f,
                                 uint32_t *uid)
{
    int ret = 0;

    if (0 == initialized) { return 1; }

    *uid = (uint64_t)qt_hash_get(ptr_to_uid_hash, f);

    return ret;
}

int qthread_multinode_action_ptr(uint32_t   uid,
                                 qthread_f *f)
{
    int ret = 0;

    if (0 == initialized) { return 1; }

    *f = qt_hash_get(uid_to_ptr_hash, (qt_key_t)(uintptr_t)uid);

    return ret;
}

int qthread_fork_remote(qthread_f   f,
                        const void *arg,
                        aligned_t  *ret,
                        int         rank,
                        size_t      arg_len)
{
    qthread_debug(MULTINODE_CALLS, "[%d] begin f=0x%lx, arg=0x%lx, ret=0x%lx, rank=%d, arg_len=%ld)\n",
                  my_rank, (unsigned long)f, (unsigned long)arg,
                  (unsigned long)ret, rank, arg_len);

    uint64_t const uid = (uint64_t)qt_hash_get(ptr_to_uid_hash, f);
    if (qt_hash_get(uid_to_ptr_hash, (qt_key_t)(uintptr_t)uid) != f) {
        fprintf(stderr, "action not registered at source\n");
        abort();
    }

    if (NULL != ret) {
        qthread_empty(ret);
    }

    if (arg_len <= FORK_MSG_PAYLOAD) {
        struct fork_msg_t msg;

        msg.uid         = uid;
        msg.return_addr = (uint64_t)ret;
        msg.origin_node = my_rank;
        msg.arg_len     = arg_len;
        memcpy(msg.args, arg, arg_len);
        qthread_debug(MULTINODE_DETAILS, "[%d] remote fork %d %d 0x%lx %d\n",
                      my_rank, rank, msg.uid, msg.return_addr, msg.arg_len);
        return qthread_internal_net_driver_send(rank, SHORT_MSG_TAG, &msg, sizeof(msg));
    } else {

        struct fork_long_msg_t * long_msg;

        size_t long_msg_size = sizeof(struct fork_long_msg_t) + arg_len;
        long_msg = qt_malloc(long_msg_size);
        assert(NULL != long_msg);

        long_msg->uid         = uid;
        long_msg->return_addr = (uint64_t)ret;
        long_msg->origin_node = my_rank;
        long_msg->arg_len     = arg_len;
        memcpy(&long_msg->args, arg, arg_len);

        qthread_debug(MULTINODE_DETAILS, "[%d] remote long fork rank=%d uid=%d return_addr=0x%lx arg_len=%d\n",
                      my_rank, rank, long_msg->uid, long_msg->return_addr, long_msg->arg_len);
        int const rc = qthread_internal_net_driver_send(rank, LONG_MSG_TAG, long_msg, long_msg_size);

        qt_free(long_msg);

        return rc;
    }
}

/* vim:set expandtab: */
