#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "qthread/qthread.h"

#include "qthread/multinode.h"
#include "spr_innards.h"
#include "qthread/spr.h"

#include "qt_multinode_innards.h"
#include "spr_innards.h"
#include "qt_asserts.h"
#include "qt_debug.h"
#include "qt_atomics.h"
#include "net/net.h"

#include "qthread/barrier.h"

/******************************************************************************
* Internal SPR remote actions                                                *
******************************************************************************/

static int initialized_flags = -1;
static int spr_initialized = 0;

static aligned_t spr_locale_barrier_op(void *arg_);
static qthread_f
spr_remote_functions[2] = {
    spr_locale_barrier_op,
    NULL};

// Locale barrier
static qt_barrier_t * locale_barrier = NULL;

static void call_fini(void)
{
    spr_fini();
}

int spr_init(unsigned int flags,
             qthread_f   *regs)
{
    qassert(setenv("QT_MULTINODE", "1", 1), 0);

    // Validate flags
    if (flags & ~(SPR_SPMD)) { return SPR_BADARGS; }
    initialized_flags = flags;

    // Initialize Qthreads on this locale
    qthread_initialize();

    // Register actions
    spr_register_actions(spr_remote_functions, 0, spr_internals_base);
    if (regs) {
        spr_register_actions(regs, 0, spr_init_base);
    }

    if (0 == spr_locale_id()) {
        // Create locale barrier
        locale_barrier = qt_barrier_create(spr_num_locales(), REGION_BARRIER);
    }

    spr_initialized = 1;

    atexit(call_fini);
    if (flags & SPR_SPMD) {
        qthread_multinode_multistart();
    } else {
        qthread_multinode_run();
    }
    return SPR_OK;
}

int spr_fini(void)
{
    static int recursion_detection = 0;

    if (initialized_flags == -1) { return SPR_NOINIT; }
    if (recursion_detection) { return SPR_OK; }
    recursion_detection = 1;

    // Destroy locale barrier
    if (NULL != locale_barrier) {
        qt_barrier_destroy(locale_barrier);
    }

    if (initialized_flags & SPR_SPMD) {
        qthread_multinode_multistop();
    }

    recursion_detection = 0;
    initialized_flags   = -1;
    return SPR_OK;
}

int spr_register_actions(qthread_f *actions,
                         size_t     count,
                         size_t     base)
{
    int rc = SPR_OK;

    assert(actions);

    if (count == 0) {
        qthread_f *cur_f = actions;
        size_t     tag   = 0;

        while (*cur_f) {
            qassert(qthread_multinode_register(tag + base, *cur_f), QTHREAD_SUCCESS);
            ++tag;
            cur_f = actions + tag;
        }
    } else {
        for (int i = 0; i < count; i++) {
            qassert(qthread_multinode_register(i + base, actions[i]), QTHREAD_SUCCESS);
        }
    }

    return rc;
}

int spr_unify(void)
{
    if (initialized_flags == -1) { return SPR_NOINIT; }
    if (initialized_flags & ~(SPR_SPMD)) { return SPR_IGN; }

    if (0 != spr_locale_id()) {
        spr_fini();
    }
    return SPR_OK;
}

int spr_num_locales(void)
{
    if (initialized_flags == -1) { return SPR_NOINIT; }
    return qthread_multinode_size();
}

int spr_locale_id(void)
{
    if (initialized_flags == -1) { return SPR_NOINIT; }
    return qthread_multinode_rank();
}

/******************************************************************************
* Data Movement: One-sided Get                                               *
******************************************************************************/

/**
 * Wait for the get operation to complete fully.
 *
 * This will cause the caller to block until acknowledgement is received
 * that the entire memory segment is ready and available at the target locale.
 *
 * param hand The handle for the get operation.
 *
 * return int Returns SPR_OK on success.
 */
int spr_get_wait(spr_get_handle_t *const hand)
{
    int const                rc = SPR_OK;
    struct spr_get_handle_s *h  = (struct spr_get_handle_s *)hand;

    qthread_readFF(&h->feb, &h->feb);

    return rc;
}

/**
 * Get the specified arbitrary-length remote memory segment.
 * Note: this is a native implementation using SPR remote-spawning.
 *
 * This method blocks the calling task until the entire memory segment is
 * available at the local destination.
 *
 * @param dest_addr The pointer to the local memory segment destination.
 *
 * @param src_loc The locale where the memory segment resides.
 *
 * @param src_addr The pointer to the memory segment on the specified locale.
 *
 * @param size The size of the memory segment.
 *
 * @return int Returns SPR_OK on success.
 */
int spr_get(void *restrict       dest_addr,
            int                  src_loc,
            const void *restrict src_addr,
            size_t               size)
{
    int rc = SPR_OK;

    qthread_debug(MULTINODE_CALLS, "[%d] begin spr_get(%d, %p, %p, %d)\n", spr_locale_id(), src_loc, src_addr, dest_addr, size);

    struct spr_get_handle_s hand;

    spr_get_nb(dest_addr, src_loc, src_addr, size, (spr_get_handle_t *)&hand);
    spr_get_wait((spr_get_handle_t *)&hand);

    qthread_debug(MULTINODE_CALLS, "[%d] end spr_get(%d, %p, %p, %d)\n", spr_locale_id(), src_loc, src_addr, dest_addr, size);

    return rc;
}

/**
 * Get the specified arbitrary-length remote memory segment.
 * Note: this is a native implementation using SPR remote-spawning.
 *
 * This method returns after the data transfer was initiated; the handle
 * must be used to synchronize on the completion of the transfer.
 *
 * @param dest_addr The pointer to the local memory segment destination.
 *
 * @param src_loc The locale where the memory segment resides.
 *
 * @param src_addr The pointer to the memory segment on the specified locale.
 *
 * @param size The size of the memory segment.
 *
 * @param hand The pointer to the handle that must be used later to
 *             synchronize on the completion of the operation.
 *
 * @return int Returns SPR_OK on success.
 */
int spr_get_nb(void *restrict             dest_addr,
               int                        src_loc,
               const void *restrict       src_addr,
               size_t                     size,
               spr_get_handle_t *restrict hand)
{
    int                      rc   = SPR_OK;
    int const                here = spr_locale_id();
    struct spr_get_handle_s *h    = (struct spr_get_handle_s *)hand;

    qthread_debug(MULTINODE_CALLS, "[%d] begin spr_get_nb(%d, %p, %p, %d, %p)\n", spr_locale_id(), src_loc, src_addr, dest_addr, size, h);
    assert(src_loc < qthread_multinode_size());
    assert(size > 0);
    assert(dest_addr);
    assert(src_addr);
    assert(hand);

    qthread_empty(&h->feb);
    rc = qthread_internal_net_driver_get(dest_addr, src_loc, src_addr, size, &h->feb);

    qthread_debug(MULTINODE_CALLS, "[%d] end spr_get_nb(%d, %p, %p, %d)\n", spr_locale_id(), src_loc, src_addr, dest_addr, size);

    return rc;
}

/******************************************************************************
* Data Movement: One-sided Put                                               *
******************************************************************************/

/**
 * Wait until all data is available at the destination.
 *
 * This will cause the caller to block until acknowledgement is received
 * that the entire memory segment is ready and available at the target locale.
 *
 * param hand The handle for the get operation.
 *
 * return int Returns SPR_OK on success.
 */
int spr_put_wait(spr_put_handle_t *const hand)
{
    int const                rc = SPR_OK;
    struct spr_put_handle_s *h  = (struct spr_put_handle_s *)hand;

    qthread_readFF(&h->feb, &h->feb);

    return rc;
}

/**
 * Put a copy of the arbitrary-length local memory segment at the specified
 * location on the remote locale.
 *
 * This method blocks the calling task until the entire memory segment is
 * available at the remote locale.
 *
 * @param dest_loc The locale where the remote memory segment resides.
 *
 * @param dest_addr The pointer to the memory segment on the specified locale.
 *
 * @param src_add The pointer to the local memory segment.
 *
 * @param size The size of the memory segment.
 *
 * @return int Returns SPR_OK on success.
 */
int spr_put(int                  dest_loc,
            void *restrict       dest_addr,
            const void *restrict src_addr,
            size_t               size)
{
    int rc = SPR_OK;

    qthread_debug(MULTINODE_CALLS, "[%d] begin spr_put(%d, %p, %p, %d)\n", spr_locale_id(), dest_loc, dest_addr, src_addr, size);

    struct spr_put_handle_s hand;

    spr_put_nb(dest_loc, dest_addr, src_addr, size, (spr_put_handle_t *)&hand);
    spr_put_wait((spr_put_handle_t *)&hand);

    qthread_debug(MULTINODE_CALLS, "[%d] end spr_put(%d, %p, %p, %d)\n", spr_locale_id(), dest_loc, dest_addr, src_addr, size);

    return rc;
}

/**
 * Put a copy of the arbitrary-length local memory segment at the specified
 * location on the remote locale.
 *
 * This method returns after the data transfer was initiated; the handle
 * must be used to synchronize on the completion of the transfer.
 *
 * @param dest_loc The locale where the remote memory segment resides.
 *
 * @param dest_addr The pointer to the memory segment on the specified locale.
 *
 * @param src_add The pointer to the local memory segment.
 *
 * @param size The size of the memory segment.
 *
 * @param hand The pointer to the handle that must be used later to
 *             synchronize on the completion of the operation.
 *
 * @return int Returns SPR_OK on success.
 */
int spr_put_nb(int                        dest_loc,
               void *restrict             dest_addr,
               const void *restrict       src_addr,
               size_t                     size,
               spr_put_handle_t *restrict hand)
{
    int                      rc   = SPR_OK;
    int const                here = spr_locale_id();
    struct spr_put_handle_s *h    = (struct spr_put_handle_s *)hand;

    qthread_debug(MULTINODE_CALLS, "[%d] begin spr_put_nb(%d, %p, %p, %d)\n", spr_locale_id(), dest_loc, dest_addr, src_addr, size);
    assert(dest_loc < qthread_multinode_size());
    assert(size > 0);
    assert(dest_addr);
    assert(src_addr);
    assert(hand);

    qthread_empty(&h->feb);
    rc = qthread_internal_net_driver_put(dest_loc, dest_addr, src_addr, size, &h->feb);

    qthread_debug(MULTINODE_CALLS, "[%d] end spr_put_nb(%d, %p, %p, %d)\n", spr_locale_id(), dest_loc, dest_addr, src_addr, size);

    return rc;
}

/******************************************************************************
* Locale-level Collectives: Barrier                                          *
******************************************************************************/

aligned_t spr_locale_barrier_op(void *arg_)
{
    aligned_t result = 0;

    qthread_debug(MULTINODE_CALLS, "[%d] enter task-level barrier.\n", spr_locale_id());

    assert(NULL != locale_barrier); // Must run on L0
    qt_barrier_enter(locale_barrier);

    qthread_debug(MULTINODE_CALLS, "[%d] exit task-level barrier.\n", spr_locale_id());
    
    return result;
}

int spr_locale_barrier(void)
{
    int rc = SPR_OK;

    if (0 == spr_initialized) {
        // Perform (thread-level) barrier using the network driver runtime
        qthread_debug(MULTINODE_BEHAVIOR, "[%d] enter network driver runtime  barrier.\n", spr_locale_id());

        rc = qthread_internal_net_driver_barrier();

        qthread_debug(MULTINODE_BEHAVIOR, "[%d] exit network driver runtime  barrier.\n", spr_locale_id());
    } else {
        // Perform task-level barrier
        qthread_debug(MULTINODE_BEHAVIOR, "[%d] enter task-level barrier.\n", spr_locale_id());

        aligned_t ret;
        qthread_fork_remote(spr_locale_barrier_op,
                            NULL, // arg
                            &ret,
                            0,    // rank
                            0);   // arg_len
        qthread_readFF(&ret, &ret);

        qthread_debug(MULTINODE_BEHAVIOR, "[%d] exit task-level barrier.\n", spr_locale_id());
    }

    return rc;
}

/* vim:set expandtab: */
