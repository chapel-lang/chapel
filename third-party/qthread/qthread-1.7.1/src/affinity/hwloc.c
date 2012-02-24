#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <hwloc.h>

#include "qthread_innards.h"
#include "qt_affinity.h"
#include "qt_debug.h"
#include "qt_envariables.h"
#include "shufflesheps.h"

static hwloc_topology_t topology;
static uint32_t         initialized = 0;

static int            shep_depth = -1;
static hwloc_cpuset_t mccoy_thread_bindings;

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
static hwloc_obj_type_t wkr_type  = HWLOC_OBJ_PU;
static int              wkr_index = -1;
DEBUG_ONLY(static const char *typename);
#endif

#if HWLOC_API_VERSION == 0x00010000
# define WEIGHT(x)           hwloc_cpuset_weight(x)
# define ASPRINTF(x, y)      hwloc_cpuset_asprintf((x), (y))
# define FOREACH_START(x, y) hwloc_cpuset_foreach_begin((x), (y))
# define FOREACH_END()       hwloc_cpuset_foreach_end()
# define ALLOC()             hwloc_cpuset_alloc()
# define FREE(x)             hwloc_cpuset_free(x)
#else
# define WEIGHT(x)           hwloc_bitmap_weight(x)
# define ASPRINTF(x, y)      hwloc_bitmap_asprintf((x), (y))
# define FOREACH_START(x, y) hwloc_bitmap_foreach_begin((x), (y))
# define FOREACH_END()       hwloc_bitmap_foreach_end()
# define ALLOC()             hwloc_bitmap_alloc()
# define FREE(x)             hwloc_bitmap_free(x)
#endif /* if HWLOC_API_VERSION == 0x00010000 */

qthread_shepherd_id_t guess_num_shepherds(void);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
qthread_worker_id_t guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds);
#endif

static void qt_affinity_internal_hwloc_teardown(void)
{   /*{{{*/
    hwloc_set_cpubind(topology, mccoy_thread_bindings, HWLOC_CPUBIND_THREAD);
    FREE(mccoy_thread_bindings);
    qthread_debug(AFFINITY_DETAILS, "destroy hwloc topology handle\n");
    hwloc_topology_destroy(topology);
    initialized = 0;
} /*}}}*/

void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers)
{                                      /*{{{ */
    if (qthread_cas(&initialized, 0, 1) == 0) {
        qassert(hwloc_topology_init(&topology), 0);
        qassert(hwloc_topology_load(topology), 0);
        MACHINE_FENCE;
        initialized = 2;
    } else {
        while (initialized == 1) SPINLOCK_BODY();
    }

    qthread_internal_cleanup(qt_affinity_internal_hwloc_teardown);
    mccoy_thread_bindings = ALLOC();
    hwloc_get_cpubind(topology, mccoy_thread_bindings, HWLOC_CPUBIND_THREAD);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    const char  *typenames[] = {
        "node", "cache", "socket", "core", "pu", "L1cache", "L2cache", "L3cache",
        "L4cache"
    };
    const size_t numtypes      = sizeof(typenames) / sizeof(char *);
    int          shep_type_idx = -1;
    /* BEWARE: L*cache must be consecutive of ascending levels, as the index
     * below is used to compare to the cache level */
    const size_t     shepindexofL1cache  = 5;
    hwloc_obj_type_t shep_type_options[] = {
        HWLOC_OBJ_NODE, HWLOC_OBJ_CACHE, HWLOC_OBJ_SOCKET, HWLOC_OBJ_CORE,
        HWLOC_OBJ_PU,
        HWLOC_OBJ_CACHE, HWLOC_OBJ_CACHE, HWLOC_OBJ_CACHE, HWLOC_OBJ_CACHE
    };
    {
        const char *qsh = qt_internal_get_env_str("SHEPHERD_BOUNDARY");

        if (qsh) {
            for (int ti = 0; ti < numtypes; ++ti) {
                if (!strncmp(typenames[ti], qsh, strlen(typenames[ti]))) {
                    shep_type_idx = ti;
                }
            }
            if (shep_type_idx == -1) {
                fprintf(stderr, "unparsable shepherd boundary (%s)\n", qsh);
            }
        }

        qsh = qt_internal_get_env_str("WORKER_UNIT");
        if (qsh) {
            for (int ti = 0; ti < numtypes; ++ti) {
                if (!strncmp(typenames[ti], qsh, strlen(typenames[ti]))) {
                    wkr_index = ti;
                    wkr_type  = shep_type_options[ti];
                }
            }
            if (wkr_index == -1) {
                fprintf(stderr, "unparsable worker unit (%s)\n", qsh);
            }
        }
    }
    if (*nbshepherds == 0) {           /* we need to guesstimate */
        /* the goal here is to basically pick the number of domains over which
         * memory access is the same cost (e.g. number of sockets, if all cores on
         * a given socket share top-level cache). This will define the shepherd
         * boundary, unless the user has specified a shepherd boundary */
        if (shep_type_idx == -1) {
            do {
loop_top:
                shep_type_idx++;
                shep_depth = hwloc_get_type_depth(topology, shep_type_options[shep_type_idx]);
                qthread_debug(AFFINITY_DETAILS, "depth of type %i (%s) = %d\n", shep_type_idx,
                              hwloc_obj_type_string(shep_type_options[shep_type_idx]), shep_depth);
                if (shep_type_idx == 3) {
                    break;
                }
                qthread_debug(AFFINITY_DETAILS, "num objs of type %i = %d\n",
                              shep_type_idx,
                              hwloc_get_nbobjs_by_type(topology,
                                                       shep_type_options
                                                       [shep_type_idx]));
                if ((shep_type_idx == 0) &&
                    (hwloc_get_nbobjs_by_type(topology,
                                              shep_type_options[0]) == 1)) {
                    qthread_debug(AFFINITY_DETAILS,
                                  "only one node; assuming multiple shepherds\n");
                    goto loop_top;
                }
            } while (shep_depth == HWLOC_TYPE_DEPTH_UNKNOWN ||
                     shep_depth == HWLOC_TYPE_DEPTH_MULTIPLE);

            assert(hwloc_get_nbobjs_inside_cpuset_by_depth
                       (topology, hwloc_topology_get_allowed_cpuset(topology),
                       shep_depth) > 0);
        } else {
            shep_depth = hwloc_get_type_depth(topology, shep_type_options[shep_type_idx]);
            qthread_debug(AFFINITY_DETAILS, "depth of type %i (%s) = %d\n",
                          hwloc_obj_type_string(shep_type_options[shep_type_idx]),
                          shep_type_idx, shep_depth);
        }
        DEBUG_ONLY(typename = typenames[shep_type_idx]);
        if ((shep_depth == HWLOC_TYPE_DEPTH_UNKNOWN) ||
            (shep_depth == HWLOC_TYPE_DEPTH_MULTIPLE)) {
            if ((shep_type_idx > 0) &&
                (shep_type_options[shep_type_idx] == HWLOC_OBJ_CACHE)) {
                /* caches are almost always weird; so if the user asked for them, just give best effort */
                unsigned int maxdepth = hwloc_topology_get_depth(topology);
                unsigned int curdepth;
                unsigned int cacdepth = maxdepth;
                unsigned int level    = 0;
                qthread_debug(AFFINITY_DETAILS, "Trying to identify caches...\n");
                /* look backward from the PU to be able to identify cache level */
                for (curdepth = maxdepth; curdepth > 0; --curdepth) {
                    unsigned int     realdepth = curdepth - 1;
                    hwloc_obj_type_t t         =
                        hwloc_get_depth_type(topology, realdepth);
                    if (t == HWLOC_OBJ_CACHE) {
                        level++;
                        qthread_debug(AFFINITY_DETAILS,
                                      "L%u at depth %u (nbobjs is %u)\n",
                                      level, realdepth, (unsigned int)hwloc_get_nbobjs_by_depth(topology, realdepth));
                        /* default choice (1): pick the outermost layer of cache */
                        /* if user requested L-specific cache, then count & compare */
                        /* L1 is _not_ the same as 'pu' _if_ we have hyperthreading */
                        /* BEWARE: ugly compare between depth & shep_type_idx */
                        if ((shep_type_idx == 1) ||
                            (level ==
                             (shep_type_idx - (shepindexofL1cache - 1)))) {
                            cacdepth = realdepth;
                        }
                    }
                }
                if (cacdepth == maxdepth) {
                    qthread_debug(AFFINITY_DETAILS,
                                  "%s not found... nbobjs_by_type HWLOC_OBJ_PU is %u\n",
                                  typenames[shep_type_idx],
                                  (unsigned int)
                                  hwloc_get_nbobjs_by_type(topology,
                                                           HWLOC_OBJ_PU));
                    shep_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
                    DEBUG_ONLY(typename = "pu");
                } else {
                    shep_depth = cacdepth;
                }
                qthread_debug(AFFINITY_DETAILS, "final shep_depth: %i (%s)\n",
                              shep_depth, typename);
            } else {
                qthread_debug(AFFINITY_DETAILS,
                              "topology too weird... nbobjs_by_type HWLOC_OBJ_PU is %u\n",
                              (unsigned int)hwloc_get_nbobjs_by_type(topology,
                                                                     HWLOC_OBJ_PU));
                shep_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
                DEBUG_ONLY(typename = "pu");
            }
        }
        qthread_debug(AFFINITY_FUNCTIONS, "final shep_depth: %i (%s)\n", shep_depth,
                      typename);
        *nbshepherds = guess_num_shepherds();
        *nbworkers   = guess_num_workers_per_shep(*nbshepherds);
    } else {                           /* (*nbshepherds != 0) */
        if (shep_type_idx != -1) {
            shep_depth = hwloc_get_type_depth(topology, shep_type_options[shep_type_idx]);
            if (shep_depth <= 0) {
                qthread_debug(AFFINITY_DETAILS, "invalid shepherd type\n");
                shep_depth = -1;
            }
        }
        if (shep_type_idx == -1) {
            /* first, look for an exact match in width and find the proper depth */
            unsigned int maxdepth = hwloc_topology_get_depth(topology);
            unsigned int realdepth;
            unsigned int fl_depth = -1; // first depth with #objs larger than nbsheps
            qthread_debug(AFFINITY_DETAILS,
                          "Trying to identify which level to use for width = %d\n",
                          *nbshepherds);
            for (realdepth = 0; realdepth < maxdepth && shep_depth == -1; ++realdepth) {
                unsigned int num = hwloc_get_nbobjs_by_depth(topology, realdepth);
                qthread_debug(AFFINITY_DETAILS, "%s at depth %u (nbobjs is %u%s)\n",
                              hwloc_obj_type_string(hwloc_get_depth_type(topology, realdepth)), realdepth, num, (num == HWLOC_TYPE_DEPTH_UNKNOWN) ? " (unknown)" : ((num == HWLOC_TYPE_DEPTH_MULTIPLE) ? " (multiple)" : ""));
                if (num == *nbshepherds) {
                    shep_depth = realdepth;
                    DEBUG_ONLY(typename = hwloc_obj_type_string(hwloc_get_depth_type(topology, realdepth)));
                } else if ((num > *nbshepherds) && (fl_depth == -1)) {
                    fl_depth = realdepth;
                }
            }
            /* second, if we failed, try an approximate match... */
            /* should we use the last _smaller_, or the first _larger_ ? */
            /* first option means overlapping but we can use all the cores */
            /* second option means no overlapping, but cores will go unused */
            if ((shep_depth == -1) && (fl_depth != -1)) {
                /* first larger then */
                shep_depth = fl_depth;
                DEBUG_ONLY(typename = hwloc_obj_type_string(hwloc_get_depth_type(topology, fl_depth)));
                qthread_debug(AFFINITY_DETAILS, "first larger depth is %i, %s\n", fl_depth, typename);
            }
            /* third, if we use such a large value that *nothing* is larger, fall back to PU */
            if (shep_depth == -1) {
                qthread_debug(AFFINITY_DETAILS, "shep_depth == -1\n");
                shep_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
                DEBUG_ONLY(typename = "pu");
            }
        }
        DEBUG_ONLY(if (typename == NULL) {
                       typename = hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth));
                   }
                   )
        qthread_debug(AFFINITY_CALLS, "Using depth %d (%s) for %d shepherds\n", shep_depth, typename, *nbshepherds);
        if (*nbworkers == 0) {
            *nbworkers = guess_num_workers_per_shep(*nbshepherds);
        }
    }
    if (DEBUG_ONLY(1 ||) wkr_index != -1) {
        hwloc_const_cpuset_t allowed_cpuset      = hwloc_topology_get_allowed_cpuset(topology);
        hwloc_obj_t          obj                 = hwloc_get_obj_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth, 0);
        unsigned int         workerobjs_per_shep = hwloc_get_nbobjs_inside_cpuset_by_type(topology, obj->allowed_cpuset, wkr_type);
        qthread_debug(AFFINITY_CALLS, "workerobjs = %s, per_shep = %u\n", hwloc_obj_type_string(wkr_type), workerobjs_per_shep);
        assert(workerobjs_per_shep > 0);
        if (workerobjs_per_shep == 0) {
            abort();
        }
        unsigned int worker_depth = hwloc_get_type_depth(topology, wkr_type);
        assert(worker_depth >= shep_depth);
        if (worker_depth < shep_depth) {
            fprintf(stderr, "worker unit (%s) is not lower in hierarchy than shepherd (%s)\n",
                    hwloc_obj_type_string(wkr_type),
                    hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth)));
            wkr_type  = HWLOC_OBJ_PU;
            wkr_index = -1;
        }
    }
#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
    if (*nbshepherds == 0) {
        *nbshepherds = guess_num_shepherds();
    }
    shep_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
    qthread_debug(AFFINITY_CALLS, "Using depth %d (%s) for %d shepherds\n", shep_depth,
                  hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth)), *nbshepherds);
    if (*nbworkers == 0) {
        *nbworkers = 1;
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
}                                      /*}}} */

#ifdef QTHREAD_HAVE_MEM_AFFINITY
void INTERNAL qt_affinity_mem_tonode(void  *addr,
                                     size_t bytes,
                                     int    node)
{                                      /*{{{ */
    hwloc_nodeset_t nodeset = hwloc_bitmap_alloc();

    hwloc_bitmap_set(nodeset, node);
    hwloc_set_area_membind_nodeset(topology, addr, bytes, nodeset,
                                   HWLOC_MEMBIND_BIND,
                                   HWLOC_MEMBIND_NOCPUBIND);
    hwloc_bitmap_free(nodeset);
}                                      /*}}} */

void INTERNAL *qt_affinity_alloc(size_t bytes)
{                                      /*{{{ */
    return hwloc_alloc(topology, bytes);
}                                      /*}}} */

void INTERNAL *qt_affinity_alloc_onnode(size_t bytes,
                                        int    node)
{                                      /*{{{ */
    void           *ret;
    hwloc_nodeset_t nodeset = hwloc_bitmap_alloc();

    hwloc_bitmap_set(nodeset, node);
    ret =
        hwloc_alloc_membind_nodeset(topology, bytes, nodeset,
                                    HWLOC_MEMBIND_BIND,
                                    HWLOC_MEMBIND_NOCPUBIND);
    hwloc_bitmap_free(nodeset);
    return ret;
}                                      /*}}} */

void INTERNAL qt_affinity_free(void  *ptr,
                               size_t bytes)
{                                      /*{{{ */
    hwloc_free(topology, ptr, bytes);
}                                      /*}}} */

#endif /* ifdef QTHREAD_HAVE_MEM_AFFINITY */

qthread_shepherd_id_t INTERNAL guess_num_shepherds(void)
{                                      /*{{{ */
    qthread_shepherd_id_t ret = 1;

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    ret = hwloc_get_nbobjs_by_depth(topology, shep_depth);
    qthread_debug(AFFINITY_FUNCTIONS, "guessing %u shepherds (nbobjs_by_depth, depth: %u=%s)\n",
                  (unsigned int)ret, shep_depth,
                  hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth)));
#else
    /* the goal here is to pick the number of independent processing units
     * (i.e. cores) */
    ret = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PU);
    qthread_debug(AFFINITY_FUNCTIONS, "guessing %u shepherds (nbobjs_by_type, HWLOC_OBJ_PU)\n",
                  (unsigned int)ret);
#endif
    if (ret == 0) {
        ret = 1;
    }
    return ret;
}                                      /*}}} */

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
qthread_worker_id_t INTERNAL guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    qthread_worker_id_t ret        = 0;
    size_t              total      = 0;
    const size_t        max_socket = hwloc_get_nbobjs_by_depth(topology, shep_depth);

    for (size_t socket = 0; socket < nshepherds && socket < max_socket;
         ++socket) {
        hwloc_obj_t obj =
            hwloc_get_obj_by_depth(topology, shep_depth, socket);
        unsigned int weight = WEIGHT(obj->allowed_cpuset);
        qthread_debug(AFFINITY_DETAILS, "%s %u has %u weight\n", typename,
                      (unsigned int)socket, weight);
        total += weight;
        if ((socket == 0) || (ret < weight)) {
            ret = weight;
        }
    }
    if (ret * nshepherds > total) {
        ret = total / nshepherds;
    }
    if (ret == 0) {
        ret = 1;
    }
    qthread_debug(AFFINITY_FUNCTIONS, "guessed %i workers apiece for %i sheps\n", (int)ret,
                  (int)nshepherds);
    return ret;
}                                      /*}}} */

void INTERNAL qt_affinity_set(qthread_worker_t *me)
{                                                                                                /*{{{ */
    hwloc_const_cpuset_t      allowed_cpuset      = hwloc_topology_get_allowed_cpuset(topology); // where am I allowed to run?
    qthread_shepherd_t *const myshep              = me->shepherd;
    unsigned int              maxshepobjs         = hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth);
    hwloc_obj_t               obj                 = hwloc_get_obj_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth, myshep->node);
    hwloc_obj_t               worker0             = hwloc_get_obj_inside_cpuset_by_type(topology, obj->allowed_cpuset, wkr_type, 0);
    unsigned int              workerobjs_per_shep = hwloc_get_nbobjs_inside_cpuset_by_type(topology, obj->allowed_cpuset, wkr_type);

    assert(workerobjs_per_shep > 0);

    qthread_debug(AFFINITY_DETAILS,
                  "shep %i(%i) worker %i [%i], there are %u %s [%i pu]\n",
                  (int)myshep->shepherd_id,
                  (int)myshep->node,
                  (int)me->worker_id,
                  (int)me->packed_worker_id,
                  maxshepobjs,
                  hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth)),
                  (int)hwloc_get_nbobjs_inside_cpuset_by_type(topology, allowed_cpuset, HWLOC_OBJ_PU));
    unsigned int shep_pus           = hwloc_get_nbobjs_inside_cpuset_by_type(topology, obj->allowed_cpuset, HWLOC_OBJ_PU);
    unsigned int worker_pus         = hwloc_get_nbobjs_inside_cpuset_by_type(topology, worker0->allowed_cpuset, HWLOC_OBJ_PU);
    unsigned int wraparounds        = me->packed_worker_id / (maxshepobjs * qlib->nworkerspershep);
    unsigned int worker_wraparounds = me->worker_id / workerobjs_per_shep;
    hwloc_obj_t  sub_obj            =
        hwloc_get_obj_inside_cpuset_by_type(topology, obj->allowed_cpuset,
                                            HWLOC_OBJ_PU,
                                            ((me->worker_id * worker_pus) +
                                             (wraparounds * qlib->nworkerspershep) +
                                             worker_wraparounds) % shep_pus);

# ifdef QTHREAD_DEBUG_AFFINITY
    {
        char *str;
        qthread_debug(AFFINITY_DETAILS, "%u: shep_pus=%u, wraparounds=%u\n",
                      (unsigned)me->packed_worker_id,
                      shep_pus, wraparounds);
        qthread_debug(AFFINITY_DETAILS, "%u: (%i*%i) + ((%i + (%i * %i)) % %i)\n",
                      (unsigned)me->packed_worker_id,
                      (int)shep_pus, (int)myshep->node, (int)me->worker_id, (int)wraparounds, (int)qlib->nworkerspershep, (int)shep_pus);
        qthread_debug(AFFINITY_DETAILS, "%u: (%i*%i) + (((%i * %i) + (%i * %i) + %i) % %i)\n",
                      (unsigned)me->packed_worker_id,
                      (int)shep_pus, (int)myshep->node, (int)me->worker_id, (int)worker_pus, (int)wraparounds, (int)qlib->nworkerspershep, (int)worker_wraparounds, (int)shep_pus);
        ASPRINTF(&str, sub_obj->allowed_cpuset);
        qthread_debug(AFFINITY_CALLS,
                      "binding shep %i worker %i (%i) to PU %i, newPU %i, mask %s\n",
                      (int)myshep->shepherd_id, (int)me->worker_id,
                      (int)me->packed_worker_id,
                      (shep_pus * myshep->node) + ((me->worker_id + (wraparounds * qlib->nworkerspershep)) % shep_pus),
                      (shep_pus * myshep->node) + (((me->worker_id * worker_pus) + (wraparounds * qlib->nworkerspershep) + (worker_wraparounds)) % shep_pus),
                      str);
        free(str);
    }
# endif /* ifdef QTHREAD_DEBUG_AFFINITY */
    if (hwloc_set_cpubind(topology, sub_obj->allowed_cpuset, HWLOC_CPUBIND_THREAD)) {
        char *str;
        int   i = errno;
# ifdef __APPLE__
        if (i == ENOSYS) {
            return;
        }
# endif
        ASPRINTF(&str, sub_obj->allowed_cpuset);
        fprintf(stderr, "Couldn't bind to cpuset %s because %s (%i)\n", str,
                strerror(i), i);
        free(str);
    }
}                                      /*}}} */

#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
void INTERNAL qt_affinity_set(qthread_shepherd_t *me)
{                                                                                      /*{{{ */
    hwloc_const_cpuset_t allowed_cpuset = hwloc_topology_get_allowed_cpuset(topology); // where am I allowed to run?
    hwloc_obj_t          obj            = hwloc_get_obj_inside_cpuset_by_depth(topology, allowed_cpuset,
                                                                               shep_depth, me->node);

# ifdef QTHREAD_DEBUG
    unsigned int maxshepobjs = hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth);
# endif
    // unsigned int weight      = WEIGHT(obj->allowed_cpuset);
    // unsigned int wraparounds = me->shepherd_id / maxshepobjs;

    assert(hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth) >= 1);

    qthread_debug(AFFINITY_CALLS,
                  "shep %i(%i) worker 0 [%i], there are %u %s [%i pu]\n",
                  (int)me->shepherd_id,
                  (int)me->node,
                  (int)me->shepherd_id,
                  maxshepobjs,
                  hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth)),
                  (int)hwloc_get_nbobjs_inside_cpuset_by_type(topology, allowed_cpuset, HWLOC_OBJ_PU));
# ifdef QTHREAD_DEBUG_AFFINITY
    {
        char *str;
        ASPRINTF(&str, obj->allowed_cpuset);
        qthread_debug(AFFINITY_DETAILS,
                      "binding shep %i worker 0 (%i) to %s %i, mask %s\n",
                      (int)me->shepherd_id,
                      (int)me->shepherd_id,
                      hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth)),
                      me->node,
                      str);
        free(str);
    }
# endif /* ifdef QTHREAD_DEBUG_AFFINITY */
    if (hwloc_set_cpubind(topology, obj->allowed_cpuset, HWLOC_CPUBIND_THREAD)) {
        char *str;
        int   i = errno;
# ifdef __APPLE__
        if (i == ENOSYS) {
            return;
        }
# endif
        ASPRINTF(&str, obj->allowed_cpuset);
        fprintf(stderr, "Couldn't bind to cpuset %s because %s (%i)\n", str,
                strerror(i), i);
        free(str);
    }
}                                      /*}}} */

#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */

int INTERNAL qt_affinity_gendists(qthread_shepherd_t   *sheps,
                                  qthread_shepherd_id_t nshepherds)
{                                                                                      /*{{{ */
    hwloc_const_cpuset_t allowed_cpuset = hwloc_topology_get_allowed_cpuset(topology); // where am I allowed to run?
    size_t               num_extant_objs;

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    num_extant_objs =
        hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset,
                                                shep_depth);
    for (size_t i = 0; i < nshepherds; ++i) {
        sheps[i].node            = i % num_extant_objs;
        sheps[i].sorted_sheplist = calloc(nshepherds - 1, sizeof(qthread_shepherd_id_t));
        sheps[i].shep_dists      = calloc(nshepherds - 1, sizeof(qthread_shepherd_id_t));
        for (size_t j = 0, k = 0; j < nshepherds; ++j) {
            if (j != i) {
                sheps[i].shep_dists[k]        = 10;
                sheps[i].sorted_sheplist[k++] = j;
            }
        }
        shuffle_sheps(sheps[i].sorted_sheplist, nshepherds - 1);
    }
#else /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
      /* Find the number of bigger-than-PU objects in the CPU set */
    assert(hwloc_get_nbobjs_inside_cpuset_by_type
               (topology, allowed_cpuset, HWLOC_OBJ_PU) >= 1);
    size_t *cpus_left_per_obj;  // handle heterogeneous core counts
    int     over_subscribing = 0;
    shep_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
    do {
        qthread_debug(AFFINITY_DETAILS, "#objs at shep_depth %i is: %i\n",
                      shep_depth,
                      hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth));
        if (hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth) <= nshepherds) {
            // shep_depth++;
            break;
        } else {
            shep_depth--;
        }
    } while (1);
    qthread_debug(AFFINITY_BEHAVIOR, "top shep_depth is %i\n", shep_depth);
    num_extant_objs = hwloc_get_nbobjs_inside_cpuset_by_depth(topology,
                                                              allowed_cpuset,
                                                              shep_depth);
    cpus_left_per_obj = calloc(num_extant_objs, sizeof(size_t));
    /* Count how many PUs are in each obj */
    for (size_t i = 0; i < num_extant_objs; ++i) {
        hwloc_obj_t  obj =
            hwloc_get_obj_inside_cpuset_by_depth(topology, allowed_cpuset,
                                                 shep_depth, i);
        /* count how many PUs in this obj */
        cpus_left_per_obj[i] = hwloc_get_nbobjs_inside_cpuset_by_type(topology,
                                                                      obj->allowed_cpuset,
                                                                      HWLOC_OBJ_PU);
        qthread_debug(AFFINITY_DETAILS, "count[%i] = %i\n", (int)i, (int)cpus_left_per_obj[i]);
    }
    /* assign nodes by iterating over cpus_left_per_node array (which is of
     * size num_extant_nodes rather than of size nodes_i_can_use) */
    unsigned int obj = 0;
    for (size_t i = 0; i < nshepherds; ++i) {
        switch (over_subscribing) {
            case 0:
            {
                unsigned int count = 0;
                while (count < num_extant_objs && cpus_left_per_obj[obj] == 0) {
                    obj++;
                    obj *= (obj < num_extant_objs);
                    count++;
                }
                if (count < num_extant_objs) {
                    cpus_left_per_obj[obj]--;
                    break;
                }
            }
                over_subscribing = 1;
        }
        qthread_debug(AFFINITY_DETAILS, "setting shep %i to numa obj %i\n", (int)i,
                      (int)obj);
        sheps[i].node = obj;
        obj++;
        obj *= (obj < num_extant_objs);
    }
    free(cpus_left_per_obj);
    for (size_t i = 0; i < nshepherds; ++i) {
        sheps[i].sorted_sheplist = calloc(nshepherds - 1, sizeof(qthread_shepherd_id_t));
        assert(sheps[i].sorted_sheplist);
        sheps[i].shep_dists = calloc(nshepherds - 1, sizeof(unsigned int));
        for (size_t j = 0, k = 0; j < nshepherds; ++j) {
            if (j != i) {
                sheps[i].shep_dists[k]        = 10;
                sheps[i].sorted_sheplist[k++] = j;
            }
        }
        shuffle_sheps(sheps[i].sorted_sheplist, nshepherds - 1);
    }
#endif /* ifdef QTHREAD_MULTITHREADED_SHEPHERDS */
       /* there does not seem to be a way to extract distances... <sigh> */
    return QTHREAD_SUCCESS;
}                                      /*}}} */

/* vim:set expandtab: */
