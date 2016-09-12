#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* The API */
#include "qthread/qthread.h"

/* System Headers */
#include <stdlib.h> /* for random(), according to P2001 */

/* Internal Headers */
#include "qt_visibility.h"
#include "qt_debug.h"
#include "qt_asserts.h"
#include "qthread_innards.h"
#include "qt_initialized.h" // for qthread_library_initialized
#include "qt_shepherd_innards.h"
#include "qt_qthread_struct.h"
#include "qt_qthread_mgmt.h"
#include "qt_macros.h"

/* Shared Globals */
TLS_DECL_INIT(qthread_shepherd_t *, shepherd_structs);

int API_FUNC qthread_shep_ok(void)
{                      /*{{{ */
    assert(qthread_library_initialized);
    qthread_shepherd_t *ret = qthread_internal_getshep();

    if (ret == NULL) {
        return QTHREAD_PTHREAD_ERROR;
    } else {
        return QTHREAD_CASLOCK_READ_UI(ret->active);
    }
}                      /*}}} */

void API_FUNC qthread_shep_next(qthread_shepherd_id_t *shep)
{   /*{{{*/
    assert(qthread_library_initialized);
    /* This will mean something slightly different in a multinode world. */
    qthread_shepherd_id_t cur = *shep;

    assert(cur != NO_SHEPHERD);
    cur++;
    cur  *= cur < qlib->nshepherds;
    *shep = cur;
} /*}}}*/

void API_FUNC qthread_shep_prev(qthread_shepherd_id_t *shep)
{   /*{{{*/
    assert(qthread_library_initialized);
    /* This will mean something slightly different in a multinode world. */
    qthread_shepherd_id_t cur = *shep;

    assert(cur != NO_SHEPHERD);
    if (0 == cur) {
        cur = qlib->nshepherds - 1;
    } else {
        cur--;
    }
    *shep = cur;
} /*}}}*/

void API_FUNC qthread_shep_next_local(qthread_shepherd_id_t *shep)
{   /*{{{*/
    assert(qthread_library_initialized);
    /* This is node-local */
    qthread_shepherd_id_t cur = *shep;

    assert(cur != NO_SHEPHERD);
    cur++;
    cur  *= cur < qlib->nshepherds;
    *shep = cur;
} /*}}}*/

void API_FUNC qthread_shep_prev_local(qthread_shepherd_id_t *shep)
{   /*{{{*/
    assert(qthread_library_initialized);
    /* This is node-local */
    qthread_shepherd_id_t cur = *shep;

    assert(cur != NO_SHEPHERD);
    if (0 == cur) {
        cur = qlib->nshepherds - 1;
    } else {
        cur--;
    }
    *shep = cur;
} /*}}}*/

qthread_shepherd_id_t API_FUNC qthread_shep(void)
{                      /*{{{ */
    /* note that this is supposed to work even if the library is not yet initialized */
    qthread_shepherd_t *ret = qthread_internal_getshep();

    if ((qlib == NULL) || (ret == NULL)) {
        return NO_SHEPHERD;
    } else {
        return ret->shepherd_id;
    }
}                      /*}}} */

/* returns the distance between two shepherds */
int API_FUNC qthread_distance(const qthread_shepherd_id_t src,
                              const qthread_shepherd_id_t dest)
{                      /*{{{ */
    assert(qthread_library_initialized);
    assert(src < qlib->nshepherds);
    assert(dest < qlib->nshepherds);
    if ((src >= qlib->nshepherds) || (dest >= qlib->nshepherds)) {
        return QTHREAD_BADARGS;
    }
    if (qlib->shepherds[src].shep_dists == NULL) {
        return 0;
    } else {
        if (dest > src) {
            return qlib->shepherds[src].shep_dists[dest-1];
        } else if (dest == src) {
            return 0;
        } else {
            return qlib->shepherds[src].shep_dists[dest];
        }
    }
}                      /*}}} */

/* returns a list of shepherds, sorted by their distance from this qthread;
 * if NULL, then all sheps are equidistant */
const qthread_shepherd_id_t API_FUNC *qthread_sorted_sheps(void)
{                      /*{{{ */
    assert(qthread_library_initialized);
    qthread_t *t = qthread_internal_self();

    if (t == NULL) {
        return NULL;
    }
    assert(t->rdata);
    assert(t->rdata->shepherd_ptr);
    return t->rdata->shepherd_ptr->sorted_sheplist;
}                      /*}}} */

/* returns a list of shepherds, sorted by their distance from the specified shepherd;
 * if NULL, then all sheps are equidistant */
const qthread_shepherd_id_t API_FUNC *qthread_sorted_sheps_remote(const qthread_shepherd_id_t src)
{                      /*{{{ */
    assert(qthread_library_initialized);
    assert(src < qlib->nshepherds);
    if (src >= qlib->nshepherds) {
        return NULL;
    }
    return qlib->shepherds[src].sorted_sheplist;
}                      /*}}} */

/* returns the number of shepherds actively scheduling work */
qthread_shepherd_id_t API_FUNC qthread_num_shepherds(void)
{                      /*{{{ */
    assert(qthread_library_initialized);
    return (qthread_shepherd_id_t)(qlib->nshepherds_active);
}                      /*}}} */

int API_FUNC qthread_disable_shepherd(const qthread_shepherd_id_t shep)
{                      /*{{{ */
    assert(qthread_library_initialized);
    qassert_ret((shep < qlib->nshepherds), QTHREAD_BADARGS);
    if (shep == 0) {
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
    qthread_debug(SHEPHERD_CALLS, "began on shep(%i)\n", shep);
    qthread_internal_incr(&(qlib->nshepherds_active), &(qlib->nshepherds_active_lock), -1);
    (void)QT_CAS(qlib->shepherds[shep].active, 1, 0);
    return QTHREAD_SUCCESS;
}                      /*}}} */

void API_FUNC qthread_enable_shepherd(const qthread_shepherd_id_t shep)
{                      /*{{{ */
    assert(qthread_library_initialized);
    assert(shep < qlib->nshepherds);
    qthread_debug(SHEPHERD_CALLS, "began on shep(%i)\n", shep);
    qthread_internal_incr(&(qlib->nshepherds_active), &(qlib->nshepherds_active_lock), 1);
    (void)QT_CAS(qlib->shepherds[shep].active, 0, 1);
}                      /*}}} */

/***************************************************************************
* Internal Functions                                                      *
***************************************************************************/

unsigned int INTERNAL qthread_internal_shep_to_node(const qthread_shepherd_id_t shep)
{                      /*{{{ */
    return qlib->shepherds[shep].node;
}                      /*}}} */

qthread_shepherd_t INTERNAL *qthread_find_active_shepherd(qthread_shepherd_id_t *l,
                                                          unsigned int          *d)
{                      /*{{{ */
    qthread_shepherd_id_t       target = 0;
    qthread_shepherd_t         *sheps  = qlib->shepherds;
    const qthread_shepherd_id_t nsheps =
        (qthread_shepherd_id_t)qlib->nshepherds;

    qthread_debug(SHEPHERD_FUNCTIONS, "l(%p): from %i sheps\n", l, (int)nsheps);
    if (l == NULL) {
        /* if l==NULL, there's no locality info, so just find the least-busy active shepherd */
        saligned_t busyness = 0;
        int        found    = 0;

        for (size_t i = 0; i < nsheps; i++) {
            if (QTHREAD_CASLOCK_READ_UI(sheps[i].active)) {
                ssize_t shep_busy_level = qt_threadqueue_advisory_queuelen(sheps[i].ready);

                if (found == 0) {
                    found = 1;
                    qthread_debug(SHEPHERD_FUNCTIONS,
                                  "l(%p): shep %i is the least busy (%i) so far\n",
                                  l, (int)i, shep_busy_level);
                    busyness = shep_busy_level;
                    target   = i;
                } else if ((shep_busy_level < busyness) ||
                           ((shep_busy_level == busyness) &&
                            (random() % 2 == 0))) {
                    qthread_debug(SHEPHERD_FUNCTIONS,
                                  "l(%p): shep %i is the least busy (%i) so far\n",
                                  l, (int)i, shep_busy_level);
                    busyness = shep_busy_level;
                    target   = i;
                }
            }
        }
        assert(found);
        if (found == 0) {
            qthread_debug(SHEPHERD_FUNCTIONS,
                          "l(%p): DID NOT FIND ANY ACTIVE SHEPHERDS!!!\n", l);
            return NULL;
        } else {
            qthread_debug(SHEPHERD_FUNCTIONS,
                          "l(%p): found bored target %i\n",
                          l, (int)target);
            return &(sheps[target]);
        }
    } else {
        /* if we have locality info, use it to identify the closest shepherd(s)
         * and if there's more than one that is equidistant, pick the least busy
         */
        qthread_shepherd_id_t alt;
        saligned_t            busyness;
        unsigned int          target_dist;

        while (target < (nsheps - 1) && QTHREAD_CASLOCK_READ_UI(sheps[l[target]].active) == 0) {
            target++;
        }
        if (target >= (nsheps - 1)) {
            return NULL;
        }
        target_dist = d[l[target]];
        qthread_debug(SHEPHERD_FUNCTIONS,
                      "l(%p): nearest active shepherd (%i) is %u away\n",
                      l, (int)l[target], target_dist);
        busyness = qt_threadqueue_advisory_queuelen(sheps[l[target]].ready);
        for (alt = target + 1; alt < (nsheps - 1) && d[l[alt]] == target_dist;
             alt++) {
            saligned_t shep_busy_level = qt_threadqueue_advisory_queuelen(sheps[l[alt]].ready);
            if ((shep_busy_level < busyness) ||
                ((shep_busy_level == busyness) && (random() % 2 == 0))) {
                qthread_debug(SHEPHERD_FUNCTIONS,
                              "l(%p): shep %i is the least busy (%i) so far\n",
                              l, l[alt], shep_busy_level);
                busyness    = shep_busy_level;
                target      = alt;
                target_dist = d[l[alt]];
            }
        }
        qthread_debug(SHEPHERD_FUNCTIONS, "l(%p): found target %i\n", l, (int)target);
        return &(sheps[l[target]]);
    }
}                      /*}}} */

/* vim:set expandtab: */
