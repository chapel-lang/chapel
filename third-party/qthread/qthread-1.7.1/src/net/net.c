#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "qthread.h"
#include "qthread_multinode.h"
#include "qthread_innards.h"
#include "qt_debug.h"
#include "qt_atomics.h"

#include "net/net.h"

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
    char     args[44];
};

struct return_msg_t {
    uint64_t return_addr;
    uint64_t return_val;
};

struct die_msg_t {
    uint64_t my_rank;
};

#define SHORT_MSG_TAG  0x1
#define RETURN_MSG_TAG 0x4
#define DIE_MSG_TAG    0x3

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

    f   = qt_hash_get(uid_to_ptr_hash, (qt_key_t)(uintptr_t)msg->uid);
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

    qthread_debug(MULTINODE_CALLS, "[%d] begin qthread_multinode_initialize\n", my_rank);

    /* initialize structures */
    initialized     = 1;
    my_rank         = world_size = -1;
    uid_to_ptr_hash = qt_hash_create(0);
    ptr_to_uid_hash = qt_hash_create(0);

    qthread_internal_net_driver_register(SHORT_MSG_TAG, fork_msg_handler);
    qthread_internal_net_driver_register(RETURN_MSG_TAG, return_msg_handler);
    qthread_internal_net_driver_register(DIE_MSG_TAG, die_msg_handler);

    qthread_empty(&time_to_die);

    /* initialize the network driver and provie barrier */
    ret = qthread_internal_net_driver_initialize();
    if (0 != ret) {
        qthread_debug(MULTINODE_FUNCTIONS, "qthread_internal_net_driver_init failed: %d\n", ret);
        return ret;
    }

    my_rank    = qthread_internal_net_driver_get_rank();
    world_size = qthread_internal_net_driver_get_size();

    /* make sure we can clean up */
    qthread_internal_cleanup_early(net_cleanup);

    qthread_debug(MULTINODE_CALLS, "[%d] end qthread_multinode_initialize\n", my_rank);

    return 0;
}

int qthread_multinode_run(void)
{
    aligned_t val;

    if (0 == initialized) { return 1; }

    qthread_debug(MULTINODE_CALLS, "[%d] begin qthread_multinode_run\n", my_rank);

    qthread_internal_net_driver_barrier();

    if (0 != my_rank) {
        struct die_msg_t msg;

        qthread_readFE(&val, &time_to_die);
        qthread_debug(MULTINODE_DETAILS, "[%d] time to die\n", my_rank);
        msg.my_rank = my_rank;
        qthread_internal_net_driver_send(0, DIE_MSG_TAG, &msg, sizeof(msg));
        qthread_finalize();
        exit(0);
    }

    qthread_debug(MULTINODE_CALLS, "[%d] end qthread_multinode_run\n", my_rank);

    return 0;
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

    ret = qt_hash_put(uid_to_ptr_hash, (qt_key_t)(uintptr_t)uid, f);
    if (ret != 1) {
        qthread_debug(MULTINODE_DETAILS, "[%d] uid -> ptr registration failed\n",
                      my_rank);
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

int qthread_fork_remote(qthread_f   f,
                        const void *arg,
                        aligned_t  *ret,
                        int         rank,
                        size_t      arg_len)
{
    struct fork_msg_t msg;

    qthread_debug(MULTINODE_CALLS, "[%d] begin qthread_fork_remote(0x%lx, 0x%lx, 0x%lx, %d, %ld)\n",
                  my_rank, (unsigned long)f, (unsigned long)arg,
                  (unsigned long)ret, rank, arg_len);

    if (NULL != ret) {
        qthread_empty(ret);
    }

    if (arg_len < sizeof(msg.args)) {
        msg.uid         = (uint64_t)qt_hash_get(ptr_to_uid_hash, f);
        msg.return_addr = (uint64_t)ret;
        msg.origin_node = my_rank;
        msg.arg_len     = arg_len;
        memcpy(msg.args, arg, arg_len);
        qthread_debug(MULTINODE_DETAILS, "[%d] remote fork %d %d 0x%lx %d\n",
                      my_rank, rank, msg.uid, msg.return_addr, msg.arg_len);
        return qthread_internal_net_driver_send(rank, SHORT_MSG_TAG, &msg, sizeof(msg));
    }

    fprintf(stderr, "long remote fork unsupported\n");
    abort();
}

/* vim:set expandtab: */
