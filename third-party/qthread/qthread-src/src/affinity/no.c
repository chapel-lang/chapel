#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt_alloc.h"
#include "qt_affinity.h"
#include "qt_asserts.h"
#include "qt_debug.h"
#include "qt_envariables.h"
#include "shufflesheps.h"

void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers,
                               size_t                *hw_par)
{                                      /*{{{ */
    if (*nbshepherds == 0) {
        *nbshepherds = 1;
    }
    if (*nbworkers == 0) {
        *nbworkers = 1;
    }
}                                      /*}}} */

void INTERNAL qt_affinity_set(qthread_worker_t *me, unsigned int Q_UNUSED(nw))
{}

int INTERNAL qt_affinity_gendists(qthread_shepherd_t   *sheps,
                                  qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    qthread_debug(AFFINITY_CALLS, "start (%p, %i)\n", sheps, (int)nshepherds);
    for (size_t i = 0; i < nshepherds; ++i) {
        sheps[i].sorted_sheplist = qt_calloc(nshepherds - 1,
                                             sizeof(qthread_shepherd_id_t));
        sheps[i].shep_dists      = qt_calloc(nshepherds, sizeof(unsigned int));
        for (size_t j = 0, k = 0; j < nshepherds; ++j) {
            if (j != i) {
                assert(k < (nshepherds - 1));
                sheps[i].shep_dists[j]        = 10;
                sheps[i].sorted_sheplist[k++] = j;
            }
        }
        // no need to sort; they're all equidistant
        shuffle_sheps(sheps[i].sorted_sheplist, nshepherds - 1);
    }
    return QTHREAD_SUCCESS;
}                                      /*}}} */

/* vim:set expandtab: */
