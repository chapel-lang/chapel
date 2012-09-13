#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>

#include "qthread/qthread.h"

#include "qthread/multinode.h"
#include "qthread/spr.h"

#include "qthread_innards.h"
#include "qt_multinode_innards.h"
#include "spr_innards.h"
#include "qthread_asserts.h"
#include "qt_debug.h"
#include "qt_atomics.h"
#include "net/net.h"

/******************************************************************************
* Internal SPR remote actions                                                *
******************************************************************************/

static aligned_t spawn_wrapper(void *args_);
static aligned_t spr_get_req(void *args_);
static aligned_t spr_get_ack(void *args_);
static aligned_t spr_put_remote(void *args_);

static int const spr_remote_actions_base = 64;
static qthread_f spr_remote_actions[5]   = {
    spawn_wrapper,
    spr_get_req,
    spr_get_ack,
    spr_put_remote,
    NULL
};

static int initialized_flags = -1;

static void call_fini(void)
{
    spr_fini();
}

int spr_init(unsigned int flags,
             qthread_f   *regs)
{
    qassert(setenv("QT_MULTINODE", "1", 1), 0);
    if (flags & ~(SPR_SPMD)) { return SPR_BADARGS; }
    initialized_flags = flags;
    qthread_initialize();

    if (regs) {
        spr_register_actions(regs, 0, spr_init_base);
    }
    spr_register_actions(spr_remote_actions, 0, spr_internals_base);

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
* Remote Spawning                                                            *
******************************************************************************/

typedef struct spawn_wrapper_args_s {
    qthread_f f;
    int       src_loc;
    void     *src_addr;
    size_t    size;
} spawn_wrapper_args_t;

/**
 * Manage remote task execution context for long remote spawns.
 *
 * This is the wrapper function responsible for retrieving a copy of the
 * (long) remote argument buffer and executing the specified function.
 */
static aligned_t spawn_wrapper(void *args_)
{
    spawn_wrapper_args_t const *const args = (spawn_wrapper_args_t *)args_;

    /* Set up landing pad for argument payload */
    void *const f_args = MALLOC(args->size);

    assert(f_args);

    /* Get remote argument data */
    spr_get(f_args, args->src_loc, args->src_addr, args->size);

    /* Execute f within the wrapper's task */
    aligned_t ret = (*(args->f))(f_args);

    /* Clean up landing pad */
    FREE(f_args, args->size);

    return ret;
}

/**
 * Spawn a new task on a specific locale.
 *
 * This method supports arbitrary-length arguments.
 *
 * @param f This is a registered function that will be run to completion
 *          by a newly created task.
 * @param arg This is the argument that will be passed to the specified
 *            function.
 * @param ret This is the location where the return value of the specified
 *            function will be placed when the task completes.
 * @param tgt This is the target locale where the task will execute.
 * @param arg_len This is the length of the argument that will be passed to
 *                the specified function.
 *
 * @return int Returns SPR_OK on success.
 */
int spr_spawn(qthread_f  f,
              void      *arg,
              aligned_t *ret,
              int        tgt,
              size_t     arg_len)
{
    int const rc   = SPR_OK;
    int const here = spr_locale_id();

    if (here != tgt) {
        if (arg_len <= FORK_MSG_PAYLOAD) {
            qthread_fork_remote(f, arg, ret, tgt, arg_len);
        } else {
            spawn_wrapper_args_t const args = { f, here, arg, arg_len };
            qthread_fork_remote(spawn_wrapper, &args, ret,
                                tgt, sizeof(spawn_wrapper_args_t));
        }
    } else {
        qthread_fork_copyargs(f, arg, arg_len, ret);
    }

    return rc;
}

/******************************************************************************
* Data Movement: One-sided Get                                               *
******************************************************************************/

typedef struct spr_get_req_args_s {
    void    *get_obj;
    void    *src_addr;
    uint32_t size;
    uint32_t src;
} spr_get_req_args_t;

#define SPR_GET_BUF_SIZE (FORK_MSG_PAYLOAD - sizeof(void *))
typedef struct spr_get_ack_args_s {
    void *get_obj;
    char  buf[SPR_GET_BUF_SIZE];
} spr_get_ack_args_t;

/**
 * Receive portion of memory segment and write to destination.
 */
static aligned_t spr_get_ack(void *args_)
{
    aligned_t const rc = SPR_OK;

    /* Deconstruct payload: [dest_addr, chunk_size, buffer] */
    spr_get_t const *const get_obj = (spr_get_t *)(*(void **)args_);
    uint8_t const *const   buf     = args_ + sizeof(void *);

    memcpy(get_obj->dest_addr, buf, get_obj->size);
    qt_sinc_submit(get_obj->sinc, NULL);

    return rc;
}

/**
 * Package up a portion of the memory segment and send to destination.
 */
static aligned_t spr_get_req(void *args_)
{
    aligned_t const                 rc   = SPR_OK;
    spr_get_req_args_t const *const args = (spr_get_req_args_t *)args_;

    /* Construct payload: [dest_addr, chunk_size, buffer] */
    uint8_t payload[FORK_MSG_PAYLOAD] = { 0 };

    *(void **)payload = args->get_obj;
    memcpy(payload + sizeof(void *), args->src_addr, args->size);

    qthread_fork_remote(spr_get_ack, payload, NULL,
                        args->src, FORK_MSG_PAYLOAD);

    return rc;
}

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
    int const rc = SPR_OK;

    qt_sinc_t *const sinc = &(hand->sinc);
    spr_get_t *const objs = hand->get_objs;

    qt_sinc_wait(sinc, NULL);
    qt_sinc_fini(sinc);

    free(objs); // cannot calculate the size for scribbling

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
int spr_get(void  *dest_addr,
            int    src_loc,
            void  *src_addr,
            size_t size)
{
    spr_get_handle_t hand;
    int              rc;

    rc = spr_get_nb(dest_addr, src_loc, src_addr, size, &hand);
    if (rc != SPR_OK) { return rc; }

    rc = spr_get_wait(&hand);
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
int spr_get_nb(void             *dest_addr,
               int               src_loc,
               void             *src_addr,
               size_t            size,
               spr_get_handle_t *hand)
{
    int const rc   = SPR_OK;
    int const here = spr_locale_id();

    assert(dest_addr);
    assert(src_addr);
    assert(hand);

    if (here != src_loc) {
        size_t const num_chunks =
            (size + (SPR_GET_BUF_SIZE - 1)) / SPR_GET_BUF_SIZE;
        size_t const last_chunk_size =
            size - (SPR_GET_BUF_SIZE * (num_chunks - 1));
        assert(((num_chunks - 1) * SPR_GET_BUF_SIZE) + last_chunk_size == size);

        qt_sinc_t *const sinc = &(hand->sinc);
        qt_sinc_init(sinc, 0, NULL, NULL, num_chunks);

        spr_get_t *const get_objs = MALLOC(num_chunks * sizeof(spr_get_t));
        assert(get_objs);

        hand->get_objs = get_objs;

        for (int cid = 0; cid < num_chunks; cid++) {
            void *const chunk_dest_addr =
                ((char *)dest_addr) + (cid * SPR_GET_BUF_SIZE);
            void *const chunk_src_addr =
                ((char *)src_addr) + (cid * SPR_GET_BUF_SIZE);
            size_t const chunk_size =
                (cid < num_chunks - 1) ? SPR_GET_BUF_SIZE : last_chunk_size;

            get_objs[cid].sinc      = sinc;
            get_objs[cid].dest_addr = chunk_dest_addr;
            get_objs[cid].size      = chunk_size;

            spr_get_req_args_t req_objs;
            req_objs.get_obj  = &get_objs[cid];
            req_objs.src_addr = chunk_src_addr;
            req_objs.size     = chunk_size;
            req_objs.src      = here;

            qthread_fork_remote(spr_get_req, &req_objs, NULL,
                                src_loc, sizeof(spr_get_req_args_t));
        }
    } else {
        memcpy(dest_addr, src_addr, size);
    }

    return rc;
}

/******************************************************************************
* Data Movement: One-sided Put                                               *
******************************************************************************/

#define SPR_PUT_BUF_SIZE (FORK_MSG_PAYLOAD - sizeof(void *) - sizeof(size_t))

/**
 * Receive portion of memory segment and write to destination.
 */
static aligned_t spr_put_remote(void *args_)
{
    aligned_t const rc = SPR_OK;

    /* Deconstruct payload: [dest_addr, chunk_size, buffer] */
    void *const       dest_addr = *(void **)args_;
    size_t const      size      = *((size_t *)(args_ + sizeof(void *)));
    char const *const buf       = args_ + sizeof(void *) + sizeof(size_t);

    memcpy(dest_addr, buf, size);

    return rc;
}

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
    int const rc = SPR_OK;

    qt_sinc_t *const sinc = &(hand->sinc);

    qt_sinc_wait(sinc, NULL);
    qt_sinc_fini(sinc);

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
int spr_put(int    dest_loc,
            void  *dest_addr,
            void  *src_addr,
            size_t size)
{
    int const rc = SPR_OK;

    spr_put_handle_t hand;

    spr_put_nb(dest_loc, dest_addr, src_addr, size, &hand);
    spr_put_wait(&hand);

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
int spr_put_nb(int               dest_loc,
               void             *dest_addr,
               void             *src_addr,
               size_t            size,
               spr_put_handle_t *hand)
{
    int const rc   = SPR_OK;
    int const here = spr_locale_id();

    if (here != dest_loc) {
        size_t const num_chunks = (size < SPR_PUT_BUF_SIZE) ?
                                  1 : (size + (SPR_PUT_BUF_SIZE - 1)) / SPR_PUT_BUF_SIZE;
        size_t const last_chunk_size =
            size - (SPR_PUT_BUF_SIZE * (num_chunks - 1));
        assert(((num_chunks - 1) * SPR_PUT_BUF_SIZE) + last_chunk_size == size);

        uint8_t args[FORK_MSG_PAYLOAD] = { 0 };

        qt_sinc_init(&(hand->sinc), 0, NULL, NULL, num_chunks);

        for (int cid = 0; cid < num_chunks; cid++) {
            void *const chunk_src_addr =
                ((char *)src_addr) + (cid * SPR_PUT_BUF_SIZE);
            void *const chunk_dest_addr =
                ((char *)dest_addr) + (cid * SPR_PUT_BUF_SIZE);
            size_t const chunk_size =
                (cid < num_chunks - 1) ? SPR_PUT_BUF_SIZE : last_chunk_size;

            /* Construct payload: [dest_addr, chunk_size, buffer] */
            *(void **)args                     = chunk_dest_addr;
            *(size_t *)(args + sizeof(void *)) = chunk_size;
            memcpy(args + (sizeof(void *) + sizeof(size_t)),
                   chunk_src_addr, chunk_size);

            qthread_fork_remote_sinc(spr_put_remote, &args, &(hand->sinc),
                                     dest_loc, FORK_MSG_PAYLOAD);
        }
    } else {
        memcpy(dest_addr, src_addr, size);
    }

    return rc;
}

/* vim:set expandtab: */
