#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <numa.h>

#include "qthread_innards.h"
#include "qt_affinity.h"
#include "qt_debug.h"

#include "shepcomp.h"
#include "shufflesheps.h"

static nodemask_t *mccoy_bitmask = NULL;

static qthread_shepherd_id_t guess_num_shepherds(void);
qthread_worker_id_t guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds);

static void qt_affinity_internal_numa_teardown(void)
{
    numa_run_on_node_mask(mccoy_bitmask);
    FREE(mccoy_bitmask, sizeof(nodemask_t));
}

void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t *nbworkers)
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
void INTERNAL qt_affinity_set(qthread_worker_t *me)
{                                      /*{{{ */
    if (numa_run_on_node(me->shepherd->node) != 0) {
        numa_error("setting thread affinity");
    }
    numa_set_preferred(me->shepherd->node);
}                                      /*}}} */

#else
void INTERNAL qt_affinity_set(qthread_shepherd_t *me)
{                                      /*{{{ */
    if (numa_run_on_node(me->node) != 0) {
        numa_error("setting thread affinity");
    }
    numa_set_preferred(me->node);
}                                      /*}}} */

#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

qthread_worker_id_t INTERNAL guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    size_t       cpu_count = 1;
    unsigned int guess     = 1;

    qthread_debug(AFFINITY_DETAILS, "guessing workers for %i shepherds\n",
                  (int)nshepherds);
# ifdef HAVE_NUMA_NUM_THREAD_CPUS
    /* note: not numa_num_configured_cpus(), just in case an
     * artificial limit has been imposed. */
    cpu_count = numa_num_thread_cpus();
    qthread_debug(AFFINITY_DETAILS, "numa_num_thread_cpus returned %i\n",
                  nshepherds);
# elif defined(HAVE_NUMA_BITMASK_NBYTES)
    cpu_count = 0;
    for (size_t b = 0; b < numa_bitmask_nbytes(numa_all_cpus_ptr) * 8; b++) {
        cpu_count += numa_bitmask_isbitset(numa_all_cpus_ptr, b);
    }
    qthread_debug(AFFINITY_DETAILS,
                  "after checking through the all_cpus_ptr, I counted %i cpus\n",
                  (int)cpu_count);
# else /* ifdef HAVE_NUMA_NUM_THREAD_CPUS */
    cpu_count = numa_max_node() + 1;
    qthread_debug(AFFINITY_DETAILS, "numa_max_node() returned %i\n", nshepherds);
# endif /* ifdef HAVE_NUMA_NUM_THREAD_CPUS */
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

    if (numa_available() == -1) {
        return QTHREAD_THIRD_PARTY_ERROR;
    }
    nodemask_zero(&bmask);
    /* assign nodes */
    for (size_t i = 0; i < nshepherds; ++i) {
        sheps[i].node = i % num_extant_nodes;
        nodemask_set(&bmask, i % num_extant_nodes);
    }
    numa_set_interleave_mask(&bmask);
#ifdef HAVE_NUMA_DISTANCE
    /* truly ancient versions of libnuma (in the changelog, this is
     * considered "pre-history") do not have numa_distance() */
    for (qthread_shepherd_id_t i = 0; i < nshepherds; i++) {
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
            int prev_dist = qthread_distance(i, sheps[i].sorted_sheplist[0]);
            size_t count = 1;
            for (size_t j = 1; j < nshepherds-1; ++j) {
                if (qthread_distance(i, sheps[i].sorted_sheplist[j]) == prev_dist) {
                    count++;
                } else {
                    if (count > 1) {
                        shuffle_sheps(sheps[i].sorted_sheplist + (j - count), count);
                    }
                    count = 1;
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

/* vim:set expandtab */
