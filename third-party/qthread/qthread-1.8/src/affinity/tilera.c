#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_TMC_CPUS_H
# include <tmc/cpus.h>
#endif

#include <stdio.h>

#include "qthread_innards.h"
#include "qt_affinity.h"
#include "shepcomp.h"
#include "shufflesheps.h"

qthread_shepherd_id_t guess_num_shepherds(void);
qthread_worker_id_t   guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds);

void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers)
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

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
void INTERNAL qt_affinity_set(qthread_worker_t *me)
{                                      /*{{{ */
    if (tmc_cpus_set_my_cpu(me->packed_worker_id) < 0) {
        perror("tmc_cpus_set_my_affinity() failed");
        fprintf(stderr, "\tnode = %i\n", (int)me->packed_worker_id);
    }
}                                      /*}}} */

#else
void INTERNAL qt_affinity_set(qthread_shepherd_t *me)
{                                      /*{{{ */
    if (tmc_cpus_set_my_cpu(me->node) < 0) {
        perror("tmc_cpus_set_my_affinity() failed");
        fprintf(stderr, "\tnode = %i\n", (int)me->node);
    }
}                                      /*}}} */

#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

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

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
# warning The logic for node assignment is completely wrong for multithreaded shepherds
#endif
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
        sheps[i].shep_dists = calloc(nshepherds, sizeof(unsigned int));
        assert(sheps[i].shep_dists);
        tmc_cpus_grid_cpu_to_tile(sheps[i].node, &ix, &iy);
        for (j = 0; j < nshepherds; j++) {
            unsigned int jx, jy;
            tmc_cpus_grid_cpu_to_tile(sheps[j].node, &jx, &jy);
            sheps[i].shep_dists[j] =
                abs((int)ix - (int)jx) + abs((int)iy - (int)jy);
        }
        sheps[i].sorted_sheplist =
            calloc(nshepherds - 1, sizeof(qthread_shepherd_id_t));
        assert(sheps[i].sorted_sheplist);
        for (j = k = 0; j < nshepherds; j++) {
            if (j != i) {
                sheps[i].sorted_sheplist[k++] = j;
            }
        }
#if defined(HAVE_QSORT_R)
# if defined(QTHREAD_QSORT_BSD)
        assert(sheps[i].sorted_sheplist);
        qsort_r(sheps[i].sorted_sheplist, nshepherds - 1,
                sizeof(qthread_shepherd_id_t), (void *)(intptr_t)i,
                &qthread_internal_shepcomp);
# elif defined(QTHREAD_QSORT_GLIBC)
        assert(sheps[i].sorted_sheplist);
        qsort_r(sheps[i].sorted_sheplist, nshepherds - 1,
                sizeof(qthread_shepherd_id_t), &qthread_internal_shepcomp,
                (void *)(intptr_t)i);
# else
#  error BAD QSORT
# endif /* if defined(QTHREAD_QSORT_BSD) */
#else /* if defined(HAVE_QSORT_R) */
        shepcomp_src = (qthread_shepherd_id_t)i;
        qsort(sheps[i].sorted_sheplist, nshepherds - 1,
              sizeof(qthread_shepherd_id_t), qthread_internal_shepcomp);
#endif  /* if defined(HAVE_QSORT_R) && defined(QTHREAD_QSORT_BSD) */
        {
            int    prev_dist = qthread_distance(i, sheps[i].sorted_sheplist[0]);
            size_t count     = 1;
            for (size_t j = 1; j < nshepherds - 1; ++j) {
                if (qthread_distance(i, sheps[i].sorted_sheplist[j]) == prev_dist) {
                    count++;
                } else {
                    if (count > 1) {
                        shuffle_sheps(sheps[i].sorted_sheplist + (j - count), count);
                    }
                    count     = 1;
                    prev_dist = qthread_distance(i, sheps[i].sorted_sheplist[j]);
                }
            }
            if (count > 1) {
                shuffle_sheps(sheps[i].sorted_sheplist + (nshepherds - 1 - count), count);
            }
        }
    }
    return QTHREAD_SUCCESS;
}                                      /*}}} */

/* vim:set expandtab: */
