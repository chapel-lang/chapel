#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <numa.h>

#include "qt_subsystems.h"
#include "qt_asserts.h"
#include "qt_affinity.h"
#include "qt_debug.h"

#include "shepcomp.h"
#include "shufflesheps.h"

static nodemask_t *mccoy_bitmask = NULL;

static qthread_shepherd_id_t guess_num_shepherds(void);
qthread_worker_id_t          guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds);

static void qt_affinity_internal_numa_teardown(void)
{
    numa_run_on_node_mask(mccoy_bitmask);
    FREE(mccoy_bitmask, sizeof(nodemask_t));
}

void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers)
{                                      /*{{{ */
    mccoy_bitmask  = MALLOC(sizeof(nodemask_t));
    *mccoy_bitmask = numa_get_run_node_mask();
    qthread_internal_cleanup(qt_affinity_internal_numa_teardown);
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

#define BMASK_WORDS 16

static qthread_shepherd_id_t guess_num_shepherds(void)
{                                      /*{{{ */
    qthread_shepherd_id_t nshepherds = 1;

    if (numa_available() != 1) {
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
        unsigned long bmask[BMASK_WORDS];
        unsigned long count = 0;

        nshepherds = numa_max_node() + 1;
        qthread_debug(AFFINITY_DETAILS, "numa_max_node() returned %i\n",
                      nshepherds);
        qthread_debug(AFFINITY_DETAILS, "bmask is %i bytes\n", (int)sizeof(bmask));
        memset(bmask, 0, sizeof(bmask));
        for (size_t shep = 0; shep < nshepherds; ++shep) {
            int ret = numa_node_to_cpus(shep, bmask, sizeof(bmask));
            qthread_debug(AFFINITY_DETAILS,
                          "bmask for shep %i is %x,%x,%x,%x (%i)\n",
                          (int)shep, (unsigned)bmask[0], (unsigned)bmask[1],
                          (unsigned)bmask[2], (unsigned)bmask[3], ret);
            if (ret != 0) {
                break;
            }
            for (size_t word = 0;
                 word < sizeof(bmask) / sizeof(unsigned long); ++word) {
                for (size_t j = 0; j < sizeof(unsigned long) * 8; ++j) {
                    if (bmask[word] & (1UL << j)) {
                        ++count;
                    }
                }
            }
        }
        qthread_debug(AFFINITY_DETAILS,
                      "counted %i CPUs via numa_node_to_cpus()\n",
                      (int)count);
        if (count > 0) {
            nshepherds = count;
        }
# endif /* ifdef HAVE_NUMA_NUM_THREAD_CPUS */
#endif  /* MULTITHREADED */
    }
    if (nshepherds <= 0) {
        nshepherds = 1;
    }
    return nshepherds;
}                                      /*}}} */

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
void INTERNAL qt_affinity_set(qthread_worker_t *me,
                              unsigned int      Q_UNUSED(nw))
{                                      /*{{{ */
    assert(me);

    qthread_shepherd_t *const myshep = me->shepherd;

    /* It would be nice if we could do something more specific than
     * "numa_run_on_node", but because sched_etaffinity() is so dangerous, we
     * really can't, in good conscience. */
    qthread_debug(AFFINITY_FUNCTIONS, "calling numa_run_on_node(%i) for worker %i\n", myshep->node, me->packed_worker_id);
    int ret = numa_run_on_node(myshep->node);
    if (ret != 0) {
        numa_error("setting thread affinity");
	abort();
    }
    numa_set_localalloc();
}                                      /*}}} */

#else
void INTERNAL qt_affinity_set(qthread_shepherd_t *me,
                              unsigned int        Q_UNUSED(nw))
{                                      /*{{{ */
    assert(me);

    /* It would be nice if we could do something more specific than
     * "numa_run_on_node", but because sched_etaffinity() is so dangerous, we
     * really can't, in good conscience. */
    qthread_debug(AFFINITY_FUNCTIONS, "calling numa_run_on_node(%i) for worker %i\n", me->node, me->shepherd_id);
    int ret = numa_run_on_node(me->node);
    if (ret != 0) {
        numa_error("setting thread affinity");
	abort();
    }
    numa_set_localalloc();
}                                      /*}}} */

#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

qthread_worker_id_t INTERNAL guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    size_t       cpu_count = 1;
    unsigned int guess     = 1;

    qthread_debug(AFFINITY_CALLS, "guessing workers for %i shepherds\n",
                  (int)nshepherds);
#ifdef HAVE_NUMA_NUM_THREAD_CPUS
    /* note: not numa_num_configured_cpus(), just in case an
     * artificial limit has been imposed. */
    cpu_count = numa_num_thread_cpus();
    qthread_debug(AFFINITY_DETAILS, "numa_num_thread_cpus returned %i\n",
                  nshepherds);
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
    qthread_debug(AFFINITY_DETAILS, "numa_max_node() returned %i\n", nshepherds);
#endif  /* ifdef HAVE_NUMA_NUM_THREAD_CPUS */
    guess = cpu_count / nshepherds;
    if (guess == 0) {
        guess = 1;
    }
    qthread_debug(AFFINITY_DETAILS, "guessing %i workers per shepherd\n",
                  (int)guess);
    return guess;
}                                      /*}}} */

int INTERNAL qt_affinity_gendists(qthread_shepherd_t   *sheps,
                                  qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    const size_t num_extant_nodes = numa_max_node() + 1;
    nodemask_t   bmask;

    qthread_debug(AFFINITY_FUNCTIONS, "sheps(%p), nshepherds(%u), num_extant_nodes:%u\n", sheps, nshepherds, (unsigned)num_extant_nodes);
    if (numa_available() == -1) {
        return QTHREAD_THIRD_PARTY_ERROR;
    }
    nodemask_zero(&bmask);
    /* assign nodes */
    qthread_debug(AFFINITY_DETAILS, "assign nodes...\n");
    for (size_t i = 0; i < nshepherds; ++i) {
        sheps[i].node = i % num_extant_nodes;
	qthread_debug(AFFINITY_DETAILS, "set bit %u in bmask\n", i % num_extant_nodes);
        nodemask_set(&bmask, i % num_extant_nodes);
    }
    qthread_debug(AFFINITY_DETAILS, "numa_set_interleave_mask\n");
    numa_set_interleave_mask(&bmask);
#ifdef QTHREAD_NUMA_DISTANCE_WORKING
    qthread_debug(AFFINITY_DETAILS, "querying distances...\n");
    /* truly ancient versions of libnuma (in the changelog, this is
     * considered "pre-history") do not have numa_distance() */
    for (qthread_shepherd_id_t i = 0; i < nshepherds; i++) {
	qthread_debug(AFFINITY_DETAILS, "i = %u < %u...\n", i, nshepherds);
        const unsigned int node_i = sheps[i].node;
        size_t             j, k;
        sheps[i].shep_dists      = calloc(nshepherds, sizeof(unsigned int));
        sheps[i].sorted_sheplist = calloc(nshepherds - 1, sizeof(qthread_shepherd_id_t));
	qthread_debug(AFFINITY_DETAILS, "allocs %p %p\n", sheps[i].shep_dists, sheps[i].sorted_sheplist);
        assert(sheps[i].shep_dists);
        assert(sheps[i].sorted_sheplist);
        for (j = 0; j < nshepherds; j++) {
            const unsigned int node_j = sheps[j].node;

            if ((node_i != QTHREAD_NO_NODE) && (node_j != QTHREAD_NO_NODE) && (node_i != node_j)) {
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
#endif /* ifdef QTHREAD_NUMA_DISTANCE_WORKING */
    return QTHREAD_SUCCESS;
}                                      /*}}} */

/* vim:set expandtab */
