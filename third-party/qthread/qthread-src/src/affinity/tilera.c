#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_TMC_CPUS_H
# include <tmc/cpus.h>
#endif

#include <stdio.h>

#include "qt_affinity.h"
#include "qt_asserts.h"
#include "shepcomp.h"
#include "shufflesheps.h"
#include "qt_debug.h" // for MALLOC()

qthread_shepherd_id_t guess_num_shepherds(void);
qthread_worker_id_t   guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds);

void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers,
                               size_t                *hw_par)
{                                      /*{{{ */
    if (*nbshepherds == 0) {
        *nbshepherds = guess_num_shepherds();
        if (*nbshepherds <= 0) {
            *nbshepherds = 1;
        }
    }
    if (*nbworkers == 0) {
        *nbworkers = guess_num_workers_per_shep(*nbshepherds);
        if (*nbworkers <= 0) {
            *nbworkers = 1;
        }
    }
}                                      /*}}} */

qthread_shepherd_id_t INTERNAL guess_num_shepherds(void)
{                                      /*{{{ */
    cpu_set_t online_cpus;

    qassert(tmc_cpus_get_online_cpus(&online_cpus), 0);
    return tmc_cpus_count(&online_cpus);
}                                      /*}}} */

void INTERNAL qt_affinity_set(qthread_worker_t *me,
                              unsigned int      Q_UNUSED(nw))
{                                      /*{{{ */
    if (tmc_cpus_set_my_cpu(me->packed_worker_id) < 0) {
        perror("tmc_cpus_set_my_affinity() failed");
        fprintf(stderr, "\tnode = %i\n", (int)me->packed_worker_id);
    }
}                                      /*}}} */

qthread_worker_id_t INTERNAL guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    return 1;
}                                      /*}}} */

int INTERNAL qt_affinity_gendists(qthread_shepherd_t   *sheps,
                                  qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    cpu_set_t     online_cpus;
    unsigned int *cpu_array;
    size_t        cpu_count, offset;

# warning The logic for node assignment is completely wrong for multithreaded shepherds
    qassert(tmc_cpus_get_online_cpus(&online_cpus), 0);
    cpu_count = tmc_cpus_count(&online_cpus);
    assert(cpu_count > 0);
    /* assign nodes */
    cpu_array = MALLOC(sizeof(unsigned int) * cpu_count);
    assert(cpu_array != NULL);
    qassert(tmc_cpus_to_array(&online_cpus, cpu_array, cpu_count), cpu_count);
    offset = 0;
    for (qthread_shepherd_id_t i = 0; i < nshepherds; i++) {
        sheps[i].node = cpu_array[offset];
        offset++;
        offset *= (offset < cpu_count);
    }
    FREE(cpu_array, sizeof(unsigned int) * cpu_count);
    for (qthread_shepherd_id_t i = 0; i < nshepherds; i++) {
        size_t       j, k;
        unsigned int ix, iy;
        sheps[i].shep_dists      = qt_calloc(nshepherds, sizeof(unsigned int));
        sheps[i].sorted_sheplist = qt_calloc(nshepherds - 1,
                                             sizeof(qthread_shepherd_id_t));
        assert(sheps[i].shep_dists);
        assert(sheps[i].sorted_sheplist);
        tmc_cpus_grid_cpu_to_tile(sheps[i].node, &ix, &iy);
        for (j = 0; j < nshepherds; j++) {
            unsigned int jx, jy;
            tmc_cpus_grid_cpu_to_tile(sheps[j].node, &jx, &jy);
            sheps[i].shep_dists[j] =
                abs((int)ix - (int)jx) + abs((int)iy - (int)jy);
        }
        for (j = k = 0; j < nshepherds; j++) {
            if (j != i) {
                sheps[i].sorted_sheplist[k++] = j;
            }
        }
        if (nshepherds > 1) {
            sort_sheps(sheps[i].shep_dists, sheps[i].sorted_sheplist, nshepherds);
        }
    }
    return QTHREAD_SUCCESS;
}                                      /*}}} */

/* vim:set expandtab: */
