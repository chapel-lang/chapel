#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <numa.h>
#include <stdio.h>

#include "qthread_innards.h"
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
                               qthread_worker_id_t   *nbworkers)
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
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
        /* this is (probably) correct if/when we have multithreaded shepherds,
         * ... BUT ONLY IF ALL NODES HAVE CPUS!!!!!! */
        nshepherds = numa_max_node() + 1;
        qthread_debug(AFFINITY_DETAILS, "numa_max_node() returned %i\n",
                      nshepherds);
#else
# ifdef HAVE_NUMA_NUM_THREAD_CPUS
        /* note: not numa_num_configured_cpus(), just in case an
         * artificial limit has been imposed. */
        nshepherds = numa_num_thread_cpus();
        qthread_debug(AFFINITY_DETAILS, "numa_num_thread_cpus returned %i\n",
                      nshepherds);
# elif defined(HAVE_NUMA_BITMASK_NBYTES)
        nshepherds = 0;
        for (size_t b = 0; b < numa_bitmask_nbytes(numa_all_cpus_ptr) * 8;
             b++) {
            nshepherds += numa_bitmask_isbitset(numa_all_cpus_ptr, b);
        }
        qthread_debug(AFFINITY_DETAILS,
                      "after checking through the all_cpus_ptr, I counted %i cpus\n",
                      nshepherds);
# else  /* ifdef HAVE_NUMA_NUM_THREAD_CPUS */
        nshepherds = numa_max_node() + 1;
        qthread_debug(AFFINITY_DETAILS, "numa_max_node() returned %i\n",
                      nshepherds);
# endif /* ifdef HAVE_NUMA_NUM_THREAD_CPUS */
#endif  /* MULTITHREADED */
    }
    if (nshepherds <= 0) {
        nshepherds = 1;
    }
    qthread_debug(AFFINITY_FUNCTIONS, "guessing %i shepherds\n", (int)nshepherds);
    return nshepherds;
}                                      /*}}} */

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
void INTERNAL qt_affinity_set(qthread_worker_t *me)
{                                      /*{{{ */
    assert(me);

    qthread_shepherd_t *const myshep      = me->shepherd;
    struct bitmask           *shep_cpuset = numa_allocate_cpumask();
    assert(shep_cpuset);
    unsigned int maxshepobjs         = guess_num_shepherds();
    unsigned int workerobjs_per_shep = 0;

    numa_node_to_cpus(myshep->node, shep_cpuset);
    for (size_t j = 0; j < numa_bitmask_nbytes(shep_cpuset) * 8; j++) {
        workerobjs_per_shep += numa_bitmask_isbitset(shep_cpuset, j) ? 1 : 0;
    }
# ifdef QTHREAD_DEBUG_AFFINITY
    {
        char bitmask_str[numa_bitmask_nbytes(shep_cpuset) * 2];
        for (size_t byte = 0; byte < numa_bitmask_nbytes(shep_cpuset); byte++) {
            int byte_mask = 0;
            int cnt       = 0;
            for (int bit = 0; bit < 8; bit++) {
                if (numa_bitmask_isbitset(shep_cpuset, (byte * 8) + bit)) {
                    byte_mask |= 1 << bit;
                    qthread_debug(AFFINITY_DETAILS, "byte %i, bit %i\n", byte, bit);
                    cnt++;
                }
            }
            sprintf(&bitmask_str[byte * 2], "%02x", byte_mask);
        }
        qthread_debug(AFFINITY_CALLS, "set %i(%i) worker %i [%i], there are %u PUs, shepmask = %s\n",
                      (int)myshep->shepherd_id,
                      (int)myshep->node,
                      (int)me->worker_id,
                      (int)me->packed_worker_id,
                      workerobjs_per_shep,
                      bitmask_str);
    }
# endif /* ifdef QTHREAD_DEBUG_AFFINITY */

    unsigned int    shep_pus           = workerobjs_per_shep;
    unsigned int    worker_pus         = 1;
    unsigned int    wraparounds        = me->packed_worker_id / (maxshepobjs * qlib->nworkerspershep);
    unsigned int    worker_wraparounds = me->worker_id / workerobjs_per_shep;
    struct bitmask *wkr_cpuset         = numa_allocate_cpumask();
    assert(wkr_cpuset);
    {
        size_t cnt_within_shep = ((me->worker_id * worker_pus) +
                                  (wraparounds * qlib->nworkerspershep) +
                                  worker_wraparounds) % shep_pus;
        size_t bit_offset = 0;
        for (size_t i = 0; i <= cnt_within_shep; i++) {
            while (numa_bitmask_isbitset(shep_cpuset, bit_offset) == 0) {
                bit_offset++;
            }
        }
        qthread_debug(AFFINITY_DETAILS, "wkr bit = %i, cnt_within_shep = %i\n", (int)bit_offset, (int)cnt_within_shep);
        numa_bitmask_setbit(wkr_cpuset, bit_offset);
    }

# ifdef QTHREAD_DEBUG_AFFINITY
    {
        char bitmask_str[numa_bitmask_nbytes(wkr_cpuset) * 2];
        for (size_t byte = 0; byte < numa_bitmask_nbytes(wkr_cpuset); byte++) {
            int byte_mask = 0;
            int cnt       = 0;
            for (int bit = 0; bit < 8; bit++) {
                if (numa_bitmask_isbitset(wkr_cpuset, (byte * 8) + bit)) {
                    byte_mask |= 1 << bit;
                    cnt++;
                }
            }
            sprintf(&bitmask_str[byte * 2], "%02x", byte_mask);
        }
        qthread_debug(AFFINITY_CALLS,
                      "binding shep %i worker %i (%i) to node %i, PU %i, mask %s\n",
                      (int)myshep->shepherd_id, (int)me->worker_id,
                      (int)me->packed_worker_id,
                      myshep->node,
                      (shep_pus * myshep->node) + (((me->worker_id * worker_pus) + (wraparounds * qlib->nworkerspershep) + (worker_wraparounds)) % shep_pus),
                      bitmask_str);
    }
# endif /* ifdef QTHREAD_DEBUG_AFFINITY */

    numa_bind(wkr_cpuset);
    numa_bitmask_free(shep_cpuset);
    numa_bitmask_free(wkr_cpuset);
}                                      /*}}} */

#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
void INTERNAL qt_affinity_set(qthread_shepherd_t *me)
{                                      /*{{{ */
    struct bitmask *cmask       = numa_allocate_cpumask();
    unsigned int    maxshepobjs = 0;

    numa_node_to_cpus(me->node, cmask);
    for (size_t j = 0; j < numa_bitmask_nbytes(cmask) * 8; j++) {
        maxshepobjs += numa_bitmask_isbitset(cmask, j) ? 1 : 0;
    }
    numa_bind(cmask);
    qthread_debug(AFFINITY_CALLS, "set %i(%i) worker 0 [%i], there are %u PUs\n",
                  (int)me->shepherd_id,
                  (int)me->node,
                  (int)me->shepherd_id,
                  maxshepobjs);
    numa_bitmask_free(cmask);
}                                      /*}}} */

#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

unsigned int INTERNAL guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds)
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
    size_t         *cpus_left_per_node = calloc(num_extant_nodes, sizeof(size_t)); // handle heterogeneous core counts
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
    free(cpus_left_per_node);
}                                      /*}}} */

int INTERNAL qt_affinity_gendists(qthread_shepherd_t   *sheps,
                                  qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    if (numa_available() == -1) {
        return QTHREAD_THIRD_PARTY_ERROR;
    }
    assign_nodes(sheps, nshepherds);
#ifdef HAVE_NUMA_DISTANCE
    /* truly ancient versions of libnuma (in the changelog, this is
     * considered "pre-history") do not have numa_distance() */
    for (unsigned int i = 0; i < nshepherds; i++) {
        const unsigned int node_i = sheps[i].node;
        size_t             j, k;
        sheps[i].shep_dists = calloc(nshepherds, sizeof(unsigned int));
        assert(sheps[i].shep_dists);
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
        }
        sheps[i].sorted_sheplist =
            calloc(nshepherds - 1, sizeof(qthread_shepherd_id_t));
        assert(sheps[i].sorted_sheplist);
        k = 0;
        for (j = 0; j < nshepherds; j++) {
            if (j != i) {
                sheps[i].sorted_sheplist[k++] = j;
            }
        }
# if defined(HAVE_QSORT_R) && defined(QTHREAD_QSORT_BSD)
        assert(sheps[i].sorted_sheplist);
        qsort_r(sheps[i].sorted_sheplist, nshepherds - 1,
                sizeof(qthread_shepherd_id_t), (void *)(intptr_t)i,
                &qthread_internal_shepcomp);
# elif defined(HAVE_QSORT_R) && defined(QTHREAD_QSORT_GLIBC)
        /* what moron in the linux community decided to implement BSD's
         * qsort_r with the arguments reversed??? */
        assert(sheps[i].sorted_sheplist);
        qsort_r(sheps[i].sorted_sheplist, nshepherds - 1,
                sizeof(qthread_shepherd_id_t), &qthread_internal_shepcomp,
                (void *)(intptr_t)i);
# else  /* if defined(HAVE_QSORT_R) && defined(QTHREAD_QSORT_BSD) */
        shepcomp_src = (qthread_shepherd_id_t)i;
        qsort(sheps[i].sorted_sheplist, nshepherds - 1,
              sizeof(qthread_shepherd_id_t), qthread_internal_shepcomp);
# endif /* if defined(HAVE_QSORT_R) && defined(QTHREAD_QSORT_BSD) */
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
#endif /* ifdef HAVE_NUMA_DISTANCE */
    return QTHREAD_SUCCESS;
}                                      /*}}} */

/* vim:set expandtab: */
