#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* The API */
#include "qthread/qthread.h"

/* System Headers */

/* Internal Headers */
#include "qt_visibility.h"
#include "qt_debug.h"
#include "qt_asserts.h"
#include "qthread_innards.h" /* for qlib */
#include "qt_initialized.h"  // for qthread_library_initialized
#include "qt_shepherd_innards.h"
// #include "qt_qthread_struct.h"


int API_FUNC qthread_disable_worker(const qthread_worker_id_t w)
{   /*{{{*/
    assert(qthread_library_initialized);

    unsigned int shep   = w % qlib->nshepherds;
    unsigned int worker = w / qlib->nshepherds;

    qassert_ret((shep < qlib->nshepherds), QTHREAD_BADARGS);
    qassert_ret((worker < qlib->nworkerspershep), QTHREAD_BADARGS);
    if ((worker == 0) & (shep == 0)) {
        /* currently, the "real mccoy" original thread cannot be migrated
         * (because I don't know what issues that could cause on all
         * architectures). For similar reasons, therefore, the original
         * shepherd cannot be disabled. One of the nice aspects of this is that
         * therefore it is impossible to disable ALL shepherds.
         *
         * ... it's entirely possible that I'm being overly cautious. This is a
         * policy based on gut feeling rather than specific issues. */
        return QTHREAD_NOT_ALLOWED;
    }
    qthread_debug(SHEPHERD_CALLS, "began on worker(%i-%i)\n", shep, worker);

    (void)QT_CAS(qlib->shepherds[shep].workers[worker].active, 1, 0);
    qlib->nworkers_active--; // decrement active count

    if (worker == 0) { qthread_disable_shepherd(shep); }

    return QTHREAD_SUCCESS;
}   /*}}}*/

void API_FUNC qthread_enable_worker(const qthread_worker_id_t w)
{                      /*{{{ */
    assert(qthread_library_initialized);

    unsigned int shep   = w % qlib->nshepherds;
    unsigned int worker = w / qlib->nshepherds;

    assert(shep < qlib->nshepherds);

    if (worker == 0) { qthread_enable_shepherd(shep); }
    qthread_debug(SHEPHERD_CALLS, "began on shep(%i)\n", shep);
    if (worker < qlib->nworkerspershep) {
        qthread_internal_incr(&(qlib->nworkers_active), &(qlib->nworkers_active_lock), 1);
        (void)QT_CAS(qlib->shepherds[shep].workers[worker].active, 0, 1);
    }
}                      /*}}} */

qthread_worker_id_t API_FUNC qthread_worker(qthread_shepherd_id_t *shepherd_id)
{                                      /*{{{ */
    assert(qthread_library_initialized);

    qthread_worker_t *worker = (qthread_worker_t *)TLS_GET(shepherd_structs);

    if((shepherd_id != NULL) && (worker != NULL)) {
        *shepherd_id = worker->shepherd->shepherd_id;
    }
    return worker ? (worker->packed_worker_id) : NO_WORKER;
}                                      /*}}} */

qthread_worker_id_t API_FUNC qthread_worker_unique(qthread_shepherd_id_t *shepherd_id)
{                     /*{{{ */
    assert(qthread_library_initialized);

    qthread_worker_t *worker = (qthread_worker_t *)TLS_GET(shepherd_structs);

    if((shepherd_id != NULL) && (worker != NULL)) {
        *shepherd_id = worker->shepherd->shepherd_id;
    }
    return worker ? (worker->unique_id) : NO_WORKER;
}                      /*}}} */

/* returns worker ID within the shepherd */
qthread_worker_id_t API_FUNC qthread_worker_local(qthread_shepherd_id_t *shepherd_id)
{                                      /*{{{ */
    assert(qthread_library_initialized);

    qthread_worker_t *worker = (qthread_worker_t *)TLS_GET(shepherd_structs);

    if((shepherd_id != NULL) && (worker != NULL)) {
        *shepherd_id = worker->shepherd->shepherd_id;
    }
    return worker ? (worker->worker_id) : NO_WORKER;
}

/* returns the number of workers actively scheduling work */
qthread_worker_id_t API_FUNC qthread_num_workers(void)
{                      /*{{{ */
    assert(qthread_library_initialized);
    return (qthread_worker_id_t)qthread_readstate(ACTIVE_WORKERS);
}                      /*}}} */

/* returns the number of workers within a shepherd */
qthread_worker_id_t API_FUNC qthread_num_workers_local(qthread_shepherd_id_t shepherd_id)
{                      /*{{{ */
    assert(qthread_library_initialized);

    return qlib->nworkerspershep;
}

/* vim:set expandtab: */
