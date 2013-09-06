#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qthread/qthread.h"
#include "qt_visibility.h"
#include "qt_shepherd_innards.h"

#ifndef QTHREAD_SHEPHERD_TYPEDEF
# define QTHREAD_SHEPHERD_TYPEDEF
typedef struct qthread_shepherd_s qthread_shepherd_t;
#endif

#if defined(QTHREAD_HAVE_LIBNUMA)
# define QTHREAD_HAVE_MEM_AFFINITY
#endif

#if defined(QTHREAD_HAVE_HWLOC) && (HWLOC_API_VERSION > 0x00010000)
# define QTHREAD_HAVE_MEM_AFFINITY
#endif

#ifdef QTHREAD_HAVE_MEM_AFFINITY
# define MEM_AFFINITY_ONLY_ARG(x) x,
# define MEM_AFFINITY_ONLY(x)     x
#else
# define MEM_AFFINITY_ONLY_ARG(x)
# define MEM_AFFINITY_ONLY(x)
#endif

/**
 * qt_topology_init() - initialize topology layer
 * @nbshepherds:    The number of shepherds (after return).
 * @nbworkers:      The number of workers per shepherd to use (after return).
 * @hw_par:         The number of workers to use (after return).
 *
 * Process common user hints and then call qt_affinit_init() to build the
 * logical topology structure.
 *
 * Common user hints:
 * - QT_NUM_SHEPHERDS:            The number of shepherds.
 * - QT_NUM_WORKERS_PER_SHEPHERD: The number of workers per shepherd.
 * - QT_HWPAR:                    The number of workers.
 */
void INTERNAL qt_topology_init(qthread_shepherd_id_t * nshepherds,
                               qthread_worker_id_t   * nworkerspershep,
                               size_t                * hw_par);

/**
 * qt_affinity_init() - initialize affinity layer
 * @nbshepherds:    User hint for number of shepherds to use.
 * @nbworkers:      User hint for number of workers per shepherd to use.
 * @hw_par:         User hint for number of workers to use.
 *
 * Build up the logical system topology (in terms of shepherds and workers) by
 * querying the physical system topology layer.
 */
void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers,
                               size_t                *hw_par);
/**
 * qt_affinity_set() - bind a worker to a set of resources
 * @me:             The worker to bind.
 * @nworkerpershep: The number of workers per shepherd.
 *
 * Bind the specified worker to a set of resources. Usually this is a PU,
 * but might be a collection of PUs depending on the support provided by the
 * physical layer. Note that binding is actually done between the calling OS 
 * thead and the underlying hardware.
 *
 * TODO: remove `nworkerspershep` in favor of qt_topo structure.
 */
void INTERNAL qt_affinity_set(qthread_worker_t * me,
                              unsigned int nworkerspershep);

/**
 * qt_affinity_gendists() - establish distance information, per-shep
 * @sheps:      The shepherd struct containing information about all shepherds.
 * @nshepherds: The number of shepherds.
 *
 * Establish inter-shepherd distance information. If not (useful) information
 * is provided by the physical layer, set all distances to `10`. This routine
 * is only called once to set up all shepherds.
 *
 * TODO: document interpretation of distance values and significance of default
 *       value of `10`. What is the correlation between values from different
 *       physical layers?
 * TODO: remove `nshepherds` argument in favor of qt_topo structure.
 */
int qt_affinity_gendists(qthread_shepherd_t   *sheps,
                         qthread_shepherd_id_t nshepherds);

#ifdef QTHREAD_HAVE_MEM_AFFINITY
void INTERNAL *qt_affinity_alloc(size_t bytes);
void INTERNAL *qt_affinity_alloc_onnode(size_t bytes,
                                        int    node);
void INTERNAL qt_affinity_mem_tonode(void  *addr,
                                     size_t bytes,
                                     int    node);
void INTERNAL qt_affinity_free(void  *ptr,
                               size_t bytes);
#endif
/* vim:set expandtab: */
