#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <numa.h>
#include <stdio.h>

#include "qt_subsystems.h"
#include "qt_asserts.h"
#include "qt_affinity.h"
#include "qt_debug.h"

#include "shepcomp.h"
#include "shufflesheps.h"

static struct bitmask *mccoy_bitmask = NULL;

qthread_shepherd_id_t guess_num_shepherds(void);
qthread_worker_id_t   guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds);

static void qt_affinity_internal_numaV2_teardown(void)
{
    numa_run_on_node_mask(mccoy_bitmask);
}

void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers,
                               size_t                *hw_par)
{                                      /*{{{ */
    qthread_debug(AFFINITY_FUNCTIONS, "start\n");
    qassert(numa_available(), 0);
    mccoy_bitmask = numa_get_run_node_mask();
    qthread_internal_cleanup(qt_affinity_internal_numaV2_teardown);
    if (*nbshepherds == 0) {
        *nbshepherds = guess_num_shepherds();
    }
    if (*nbworkers == 0) {
        *nbworkers = guess_num_workers_per_shep(*nbshepherds);
    }
}                                      /*}}} */

void INTERNAL qt_affinity_mem_tonode(void  *addr,
                                     size_t bytes,
                                     int    node)
{                                      /*{{{ */
    numa_tonode_memory(addr, bytes, node);
}                                      /*}}} */

void INTERNAL *qt_affinity_alloc(size_t bytes)
{                                      /*{{{ */
    return numa_alloc(bytes);
}                                      /*}}} */

void INTERNAL *qt_affinity_alloc_onnode(size_t bytes,
                                        int    node)
{                                      /*{{{ */
    return numa_alloc_onnode(bytes, node);
}                                      /*}}} */

void INTERNAL qt_affinity_free(void  *ptr,
                               size_t bytes)
{                                      /*{{{ */
    numa_free(ptr, bytes);
}                                      /*}}} */

qthread_shepherd_id_t INTERNAL guess_num_shepherds(void)
{                                      /*{{{ */
    qthread_shepherd_id_t nshepherds = 1;

    if (numa_available() != 1) {
        qthread_debug(AFFINITY_FUNCTIONS, "numa_available != 1\n");
        /* this is (probably) correct if/when we have multithreaded shepherds,
         * ... BUT ONLY IF ALL NODES HAVE CPUS!!!!!! */
        nshepherds = numa_max_node() + 1;
        qthread_debug(AFFINITY_DETAILS, "numa_max_node() returned %i\n",
                      nshepherds);
    }
    if (nshepherds <= 0) {
        nshepherds = 1;
    }
    qthread_debug(AFFINITY_FUNCTIONS, "guessing %i shepherds\n", (int)nshepherds);
    return nshepherds;
}                                      /*}}} */

void INTERNAL qt_affinity_set(qthread_worker_t *me,
                              unsigned int      Q_UNUSED(nw))
{                                      /*{{{ */
    assert(me);

    qthread_shepherd_t *const myshep = me->shepherd;

    /* It would be nice if we could do something more specific than
     * "numa_run_on_node", but because sched_setaffinity() is so dangerous, we
     * really can't, in good conscience. */
    qthread_debug(AFFINITY_DETAILS, "calling numa_run_on_node(%i) for worker %i\n", myshep->node, me->packed_worker_id);
    int ret = numa_run_on_node(myshep->node);
    if (ret != 0) {
        qthread_debug(ALWAYS_OUTPUT, "numa_run_on_node() returned an error: %s\n", strerror(errno));
        abort();
    }
    numa_set_localalloc();
}                                      /*}}} */

qthread_worker_id_t INTERNAL guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    size_t       cpu_count = 1;
    unsigned int guess     = 1;

    qthread_debug(AFFINITY_DETAILS, "guessing workers for %i shepherds\n",
                  (int)nshepherds);
#ifdef HAVE_NUMA_NUM_THREAD_CPUS
    /* note: not numa_num_configured_cpus(), just in case an
     * artificial limit has been imposed. */
    cpu_count = numa_num_thread_cpus();
    qthread_debug(AFFINITY_DETAILS, "numa_num_thread_cpus returned %i\n",
                  (int)cpu_count);
#elif defined(HAVE_NUMA_BITMASK_NBYTES)
    cpu_count = 0;
    for (size_t b = 0; b < numa_bitmask_nbytes(numa_all_cpus_ptr) * 8; b++) {
        cpu_count += numa_bitmask_isbitset(numa_all_cpus_ptr, b);
    }
    qthread_debug(AFFINITY_DETAILS,
                  "after checking through the all_cpus_ptr, I counted %i cpus\n",
                  (int)cpu_count);
#else  /* ifdef HAVE_NUMA_NUM_THREAD_CPUS */
    cpu_count = numa_max_node() + 1;
    qthread_debug(AFFINITY_DETAILS, "numa_max_node() returned %i\n", (int)cpu_count);
#endif  /* ifdef HAVE_NUMA_NUM_THREAD_CPUS */
    guess = cpu_count / nshepherds;
    if (guess == 0) {
        guess = 1;
    }
    qthread_debug(AFFINITY_FUNCTIONS, "guessing %i workers per shepherd\n",
                  (int)guess);
    return guess;
}                                      /*}}} */

static void assign_nodes(qthread_shepherd_t *sheps,
                         size_t              nsheps)
{                                      /*{{{ */
    const size_t    num_extant_nodes   = numa_max_node() + 1;
    struct bitmask *nmask              = numa_get_run_node_mask();
    struct bitmask *cmask              = numa_allocate_cpumask();
    size_t         *cpus_left_per_node = qt_calloc(num_extant_nodes,
                                                   sizeof(size_t)); // handle heterogeneous core counts
    int             over_subscribing   = 0;

    assert(cmask);
    assert(nmask);
    assert(cpus_left_per_node);
    numa_bitmask_clearall(cmask);
    /* get the # cpus for each node */
    for (size_t i = 0; i < numa_bitmask_nbytes(nmask) * 8; ++i) {
        if (numa_bitmask_isbitset(nmask, i)) {
            numa_node_to_cpus(i, cmask);
            for (size_t j = 0; j < numa_bitmask_nbytes(cmask) * 8; j++) {
                cpus_left_per_node[i] +=
                    numa_bitmask_isbitset(cmask, j) ? 1 : 0;
            }
            qthread_debug(AFFINITY_DETAILS, "there are %i CPUs on node %i\n",
                          (int)cpus_left_per_node[i], (int)i);
        }
    }
    /* assign nodes by iterating over cpus_left_per_node array (which is of
     * size num_extant_nodes rather than of size nodes_i_can_use) */
    int node = 0;
    for (size_t i = 0; i < nsheps; ++i) {
        switch (over_subscribing) {
            case 0:
            {
                int count = 0;
                while (count < num_extant_nodes &&
                       cpus_left_per_node[node] == 0) {
                    node++;
                    node *= (node < num_extant_nodes);
                    count++;
                }
                if (count < num_extant_nodes) {
                    cpus_left_per_node[node]--;
                    break;
                }
            }
                over_subscribing = 1;
        }
        qthread_debug(AFFINITY_DETAILS, "setting shep %i to numa node %i\n",
                      (int)i, (int)node);
        sheps[i].node = node;
        node++;
        node *= (node < num_extant_nodes);
    }
    numa_bitmask_free(nmask);
    numa_bitmask_free(cmask);
    FREE(cpus_left_per_node, num_extant_nodes * sizeof(size_t));
}                                      /*}}} */

int INTERNAL qt_affinity_gendists(qthread_shepherd_t   *sheps,
                                  qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    qthread_debug(AFFINITY_FUNCTIONS, "sheps:%p, nsheps:%u\n", sheps, nshepherds);
    if (numa_available() == -1) {
        return QTHREAD_THIRD_PARTY_ERROR;
    }
    assign_nodes(sheps, nshepherds);
#ifdef HAVE_NUMA_DISTANCE
    qthread_debug(AFFINITY_DETAILS, "querying distances...\n");
    /* truly ancient versions of libnuma (in the changelog, this is
     * considered "pre-history") do not have numa_distance() */
    for (unsigned int i = 0; i < nshepherds; i++) {
        const unsigned int node_i = sheps[i].node;
        size_t             j, k;
        sheps[i].shep_dists      = qt_calloc(nshepherds, sizeof(unsigned int));
        sheps[i].sorted_sheplist = qt_calloc(nshepherds - 1,
                                             sizeof(qthread_shepherd_id_t));
        assert(sheps[i].shep_dists);
        assert(sheps[i].sorted_sheplist);
        for (j = 0; j < nshepherds; j++) {
            const unsigned int node_j = sheps[j].node;

            if ((node_i != QTHREAD_NO_NODE) && (node_j != QTHREAD_NO_NODE)) {
                sheps[i].shep_dists[j] = numa_distance(node_i, node_j);
            } else {
                /* XXX too arbitrary */
                if (i == j) {
                    sheps[i].shep_dists[j] = 0;
                } else {
                    sheps[i].shep_dists[j] = 20;
                }
            }
            qthread_debug(AFFINITY_DETAILS, "shep %u to shep %u distance: %u\n", i, j, sheps[i].shep_dists[j]);
        }
        k = 0;
        for (j = 0; j < nshepherds; j++) {
            if (j != i) {
                sheps[i].sorted_sheplist[k++] = j;
            }
        }
        if (nshepherds > 1) {
            sort_sheps(sheps[i].shep_dists, sheps[i].sorted_sheplist, nshepherds);
        }
    }
#endif /* ifdef HAVE_NUMA_DISTANCE */
    return QTHREAD_SUCCESS;
}                                      /*}}} */

/* vim:set expandtab: */
