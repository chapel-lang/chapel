#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <hwloc.h>

#include "chpl-topo.h"

#include "qt_subsystems.h"
#include "qt_alloc.h"
#include "qt_asserts.h" /* for qassert() */
#include "qt_affinity.h"
#include "qt_debug.h"
#include "qt_envariables.h"
#include "shufflesheps.h"

static hwloc_topology_t topology;
static uint32_t         initialized = 0;

static int            shep_depth = -1;
static hwloc_cpuset_t mccoy_thread_bindings;

static hwloc_obj_type_t wkr_type  = HWLOC_OBJ_PU;
static int              wkr_index = -1;
static int              wkr_depth = -1;
DEBUG_ONLY(static const char *typename);
const char  *typenames[] = {
    "node", "cache", "socket", "core", "pu", "L1cache", "L2cache", "L3cache",
    "L4cache"
};
const size_t numtypes = sizeof(typenames) / sizeof(char *);

#if HWLOC_API_VERSION == 0x00010000
# define WEIGHT(x)           hwloc_cpuset_weight(x)
# define ASPRINTF(x, y)      hwloc_cpuset_asprintf((x), (y))
# define FOREACH_START(x, y) hwloc_cpuset_foreach_begin((x), (y))
# define FOREACH_END()       hwloc_cpuset_foreach_end()
# define ALLOCBMAP()         hwloc_cpuset_alloc()
# define FREEBMAP(x)         hwloc_cpuset_free(x)
#else
# define WEIGHT(x)           hwloc_bitmap_weight(x)
# define ASPRINTF(x, y)      hwloc_bitmap_asprintf((x), (y))
# define FOREACH_START(x, y) hwloc_bitmap_foreach_begin((x), (y))
# define FOREACH_END()       hwloc_bitmap_foreach_end()
# define ALLOCBMAP()         hwloc_bitmap_alloc()
# define FREEBMAP(x)         hwloc_bitmap_free(x)
#endif /* if HWLOC_API_VERSION == 0x00010000 */

qthread_shepherd_id_t guess_num_shepherds(void);
qthread_worker_id_t guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds);

static unsigned int num_usable_by_depth(unsigned int depth)
{
    hwloc_const_cpuset_t allowed_cpuset = hwloc_topology_get_allowed_cpuset(topology);
    int max = hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, depth);
    unsigned int ret = 0;
    qthread_debug(AFFINITY_DETAILS, "max = %i\n", max);
    for (int i=0; i<max; ++i) {
        hwloc_obj_t obj = hwloc_get_obj_inside_cpuset_by_depth(topology, allowed_cpuset, depth, i);
        if (obj == NULL) {
            qthread_debug(AFFINITY_DETAILS, "obj %i of this type is NULL!!!\n", i);
            continue;
        }
        if (hwloc_get_nbobjs_inside_cpuset_by_type(topology, obj->cpuset, HWLOC_OBJ_PU) == 0) {
            qthread_debug(AFFINITY_DETAILS, "obj %i of this type has no PUs!!!\n", i);
            continue;
        }
        ret ++;
    }
    return ret;
}

static unsigned int num_usable_by_type(hwloc_obj_type_t tp)
{
    hwloc_const_cpuset_t allowed_cpuset = hwloc_topology_get_allowed_cpuset(topology);
    int max = hwloc_get_nbobjs_inside_cpuset_by_type(topology, allowed_cpuset, tp);
    unsigned int ret = 0;
    qthread_debug(AFFINITY_DETAILS, "max = %i\n", max);
    for (int i=0; i<max; ++i) {
        hwloc_obj_t obj = hwloc_get_obj_inside_cpuset_by_type(topology, allowed_cpuset, tp, i);
        if (obj == NULL) {
            qthread_debug(AFFINITY_DETAILS, "obj %i of this type is NULL!!!\n", i);
            continue;
        }
        if (hwloc_get_nbobjs_inside_cpuset_by_type(topology, obj->cpuset, HWLOC_OBJ_PU) == 0) {
            qthread_debug(AFFINITY_DETAILS, "obj %i of this type has no PUs!!!\n", i);
            continue;
        }
        ret ++;
    }
    return ret;
}

static void qt_affinity_internal_hwloc_teardown(void)
{   /*{{{*/
    DEBUG_ONLY(hwloc_topology_check(topology));
    hwloc_set_cpubind(topology, mccoy_thread_bindings, HWLOC_CPUBIND_THREAD);
    FREEBMAP(mccoy_thread_bindings);
    if (chpl_topo_getHwlocTopology() == NULL) {
        qthread_debug(AFFINITY_DETAILS, "destroy hwloc topology handle\n");
        hwloc_topology_destroy(topology);
    }
    initialized = 0;
} /*}}}*/

void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers,
                               size_t                *hw_par)
{                                      /*{{{ */
    qthread_debug(AFFINITY_CALLS, "nbshepherds:%p:%u nbworkers:%p:%u\n", nbshepherds, *nbshepherds, nbworkers, *nbworkers);
    if (qthread_cas(&initialized, 0, 1) == 0) {
        if ((topology = (hwloc_topology_t) chpl_topo_getHwlocTopology()) == NULL) {
            qassert(hwloc_topology_init(&topology), 0);
            qassert(hwloc_topology_load(topology), 0);
        }
        MACHINE_FENCE;
        initialized = 2;
    } else {
        while (initialized == 1) SPINLOCK_BODY();
    }

    DEBUG_ONLY(hwloc_topology_check(topology));
    qthread_internal_cleanup(qt_affinity_internal_hwloc_teardown);
    mccoy_thread_bindings = ALLOCBMAP();
    hwloc_get_cpubind(topology, mccoy_thread_bindings, HWLOC_CPUBIND_THREAD);
    hwloc_const_cpuset_t allowed_cpuset = hwloc_topology_get_allowed_cpuset(topology);
    int shep_type_idx = -1;
    /* BEWARE: L*cache must be consecutive of ascending levels, as the index
     * below is used to compare to the cache level */
    const size_t     shepindexofL1cache  = 5;
    hwloc_obj_type_t shep_type_options[] = {
        HWLOC_OBJ_NODE, HWLOC_OBJ_CACHE_UNIFIED, HWLOC_OBJ_SOCKET, HWLOC_OBJ_CORE,
        HWLOC_OBJ_PU,
        HWLOC_OBJ_CACHE_UNIFIED, HWLOC_OBJ_CACHE_UNIFIED, HWLOC_OBJ_CACHE_UNIFIED, HWLOC_OBJ_CACHE_UNIFIED};
{
        const char *qsh = qt_internal_get_env_str("SHEPHERD_BOUNDARY", "node");

        if (qsh) {
            for (int ti = 0; ti < numtypes; ++ti) {
                if (!strncasecmp(typenames[ti], qsh, strlen(typenames[ti]))) {
                    shep_type_idx = ti;
                }
            }
            if (shep_type_idx == -1) {
                fprintf(stderr, "unparsable shepherd boundary (%s)\n", qsh);
                exit(EXIT_FAILURE);
            }
        }

        qsh = qt_internal_get_env_str("WORKER_UNIT", "pu");
        if (qsh) {
            for (int ti = 0; ti < numtypes; ++ti) {
                if (!strncasecmp(typenames[ti], qsh, strlen(typenames[ti]))) {
                    wkr_index = ti;
                    wkr_type  = shep_type_options[ti];
                }
            }
            if (wkr_index == -1) {
                fprintf(stderr, "unparsable worker unit (%s)\n", qsh);
                exit(EXIT_FAILURE);
            }
        }
    }
    if (*nbshepherds == 0) {           /* we need to guesstimate */
        qthread_debug(AFFINITY_DETAILS, "guesstimating number of shepherds...\n");
        /* the goal here is to basically pick the number of domains over which
         * memory access is the same cost (e.g. number of sockets, if all cores on
         * a given socket share top-level cache). This will define the shepherd
         * boundary, unless the user has specified a shepherd boundary */
        if (shep_type_idx == -1) {
            do {
restart_loop:
                shep_type_idx++;
                shep_depth = hwloc_get_type_depth(topology, shep_type_options[shep_type_idx]);
                qthread_debug(AFFINITY_DETAILS, "depth of type %i (%s) = %d\n", shep_type_idx,
                              hwloc_obj_type_string(shep_type_options[shep_type_idx]), shep_depth);
                if (shep_type_idx == 3) {
                    break;
                }
                qthread_debug(AFFINITY_DETAILS, "num objs of type %i = %d\n",
                              shep_type_idx,
                              hwloc_get_nbobjs_inside_cpuset_by_type(topology, allowed_cpuset,
                                                       shep_type_options[shep_type_idx]));
                if ((shep_type_idx == 0) &&
                    (hwloc_get_nbobjs_inside_cpuset_by_type(topology, allowed_cpuset,
                                              shep_type_options[0]) == 1)) {
                    qthread_debug(AFFINITY_DETAILS,
                                  "only one node; assuming multiple shepherds\n");
                    continue;
                }
                /* check to make sure that all of these things have PUs */
                
            } while (shep_depth == HWLOC_TYPE_DEPTH_UNKNOWN ||
                     shep_depth == HWLOC_TYPE_DEPTH_MULTIPLE);

            assert(hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth) > 0);
        } else {
            shep_depth = hwloc_get_type_depth(topology, shep_type_options[shep_type_idx]);
            qthread_debug(AFFINITY_DETAILS, "depth of type %i (%s) = %d\n",
                          shep_type_idx,
                          hwloc_obj_type_string(shep_type_options[shep_type_idx]),
                          shep_depth);
        }
        DEBUG_ONLY(typename = typenames[shep_type_idx]);
        if ((shep_depth == HWLOC_TYPE_DEPTH_UNKNOWN) ||
            (shep_depth == HWLOC_TYPE_DEPTH_MULTIPLE)) {
            if ((shep_type_idx > 0) &&
                (shep_type_options[shep_type_idx] == HWLOC_OBJ_CACHE_UNIFIED)) {
                /* caches are almost always weird; so if the user asked for them, just give best effort */
                unsigned int maxdepth = hwloc_topology_get_depth(topology);
                unsigned int curdepth;
                unsigned int cacdepth = maxdepth;
                unsigned int level    = 0;
                qthread_debug(AFFINITY_DETAILS, "Trying to identify caches...\n");
                /* look backward from the PU to be able to identify cache level */
                for (curdepth = maxdepth; curdepth > 0; --curdepth) {
                    unsigned int     realdepth = curdepth - 1;
                    hwloc_obj_type_t t         = hwloc_get_depth_type(topology, realdepth);
                    if (t == HWLOC_OBJ_CACHE_UNIFIED) {
                        level++;
                        qthread_debug(AFFINITY_DETAILS,
                                      "L%u at depth %u (nbobjs is %u)\n",
                                      level, realdepth, num_usable_by_depth(realdepth));
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
                                  hwloc_get_nbobjs_inside_cpuset_by_type(topology, allowed_cpuset,
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
                              (unsigned int)hwloc_get_nbobjs_inside_cpuset_by_type(topology,
                                                allowed_cpuset, HWLOC_OBJ_PU));
                shep_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
                DEBUG_ONLY(typename = "pu");
            }
        }
        qthread_debug(AFFINITY_FUNCTIONS, "final shep_depth: %i (%s)\n", shep_depth,
                      typename);
        *nbshepherds = guess_num_shepherds();
        *nbworkers   = guess_num_workers_per_shep(*nbshepherds);
    } else {                           /* (*nbshepherds != 0) */
        qthread_debug(AFFINITY_DETAILS, "nbshepherds nonzero (%u), shep_type_idx = %i\n", *nbshepherds, shep_type_idx);
        if (shep_type_idx != -1) {
            qthread_debug(AFFINITY_DETAILS, "finding a shepherd depth...\n");
            shep_depth = hwloc_get_type_depth(topology, shep_type_options[shep_type_idx]);
            if (shep_depth <= 0) {
                qthread_debug(AFFINITY_DETAILS, "invalid shepherd type (%s), finding another one...\n", typenames[shep_type_idx]);
                if (shep_type_options[shep_type_idx] == HWLOC_OBJ_CACHE_UNIFIED) {
                    shep_type_idx = 1;
                }
                shep_type_idx++;
                while (((shep_depth = hwloc_get_type_depth(topology, shep_type_options[shep_type_idx])) <= 0) &&
                       shep_type_idx < 4) {
                    qthread_debug(AFFINITY_DETAILS, "invalid shepherd type (%s), finding another one...\n", typenames[shep_type_idx]);
                    shep_type_idx++;
                }
                assert(shep_depth > 0);
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
                unsigned int num = num_usable_by_depth(realdepth);
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
        qthread_debug(AFFINITY_DETAILS, "workers = %u\n", *nbworkers);
        {
            qthread_worker_id_t tmp = guess_num_workers_per_shep(*nbshepherds);
            if (*nbworkers == 0) {
                *nbworkers = tmp;
            }
        }
    }
}                                      /*}}} */

#ifdef QTHREAD_HAVE_MEM_AFFINITY
void INTERNAL qt_affinity_mem_tonode(void  *addr,
                                     size_t bytes,
                                     int    node)
{                                      /*{{{ */
    hwloc_nodeset_t nodeset = hwloc_bitmap_alloc();

    DEBUG_ONLY(hwloc_topology_check(topology));
    hwloc_bitmap_set(nodeset, node);
    hwloc_set_area_membind(topology, addr, bytes, nodeset,
                                   HWLOC_MEMBIND_BIND,
                                   HWLOC_MEMBIND_NOCPUBIND);
    hwloc_bitmap_free(nodeset);
}                                      /*}}} */

void INTERNAL *qt_affinity_alloc(size_t bytes)
{                                      /*{{{ */
    DEBUG_ONLY(hwloc_topology_check(topology));
    return hwloc_alloc(topology, bytes);
}                                      /*}}} */

void INTERNAL *qt_affinity_alloc_onnode(size_t bytes,
                                        int    node)
{                                      /*{{{ */
    void           *ret;
    hwloc_nodeset_t nodeset;

    DEBUG_ONLY(hwloc_topology_check(topology));
    nodeset = hwloc_bitmap_alloc();
    hwloc_bitmap_set(nodeset, node);
    ret = hwloc_alloc_membind_nodeset(topology, bytes, nodeset,
                                      HWLOC_MEMBIND_BIND,
                                      HWLOC_MEMBIND_NOCPUBIND);
    hwloc_bitmap_free(nodeset);
    return ret;
}                                      /*}}} */

void INTERNAL qt_affinity_free(void  *ptr,
                               size_t bytes)
{                                      /*{{{ */
    DEBUG_ONLY(hwloc_topology_check(topology));
    hwloc_free(topology, ptr, bytes);
}                                      /*}}} */

#endif /* ifdef QTHREAD_HAVE_MEM_AFFINITY */

qthread_shepherd_id_t INTERNAL guess_num_shepherds(void)
{                                      /*{{{ */
    qthread_shepherd_id_t ret = 1;

    DEBUG_ONLY(hwloc_topology_check(topology));
    hwloc_const_cpuset_t allowed_cpuset = hwloc_topology_get_allowed_cpuset(topology);
    ret = num_usable_by_depth(shep_depth);
    qthread_debug(AFFINITY_BEHAVIOR, "of %u %s's, only %u %s viable\n",
            hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth),
            hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth)),
            ret, (ret!=1)?"are":"is");
    qthread_debug(AFFINITY_FUNCTIONS, "guessing %u shepherds (nbobjs_by_depth, depth: %u=%s)\n",
                  (unsigned int)ret, shep_depth,
                  hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth)));
    if (ret == 0) {
        ret = 1;
    }
    return ret;
}                                      /*}}} */

qthread_worker_id_t INTERNAL guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    qthread_worker_id_t ret   = 0;
    size_t              total = 0;

    DEBUG_ONLY(hwloc_topology_check(topology));
    hwloc_const_cpuset_t allowed_cpuset = hwloc_topology_get_allowed_cpuset(topology);

    if (wkr_index != -1) {
        qthread_debug(AFFINITY_BEHAVIOR, "guessing number of workers based on worker type\n");
        assert(shep_depth >= 0);

        hwloc_obj_t          obj            = hwloc_get_obj_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth, 0);
        int                  workerobjs_per_shep;

        if (hwloc_compare_types(wkr_type, HWLOC_OBJ_CACHE_UNIFIED) == 0) {
            qthread_debug(AFFINITY_DETAILS, "worker type is a cache; must handle specially...\n");
            wkr_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
            qthread_debug(AFFINITY_DETAILS, "PU wkr_depth = %u\n", wkr_depth);
            for (int wkridx = 5; wkridx <= wkr_index; wkridx++) {
                hwloc_obj_t tmp;
                qthread_debug(AFFINITY_DETAILS, "wkridx=%i wkr_index=%i\n", wkridx, wkr_index);
                do {
                    wkr_depth--;
                    tmp = hwloc_get_obj_inside_cpuset_by_depth(topology, obj->cpuset, wkr_depth, 0);
                    qthread_debug(AFFINITY_DETAILS, "wkr_depth %i, type = %s %i\n", wkr_depth, hwloc_obj_type_string(tmp->type), hwloc_compare_types(tmp->type, HWLOC_OBJ_CACHE_UNIFIED));
                } while (hwloc_compare_types(tmp->type, HWLOC_OBJ_CACHE_UNIFIED) > 0);
                if (hwloc_compare_types(tmp->type, HWLOC_OBJ_CACHE_UNIFIED) != 0) {
                    fprintf(stderr, "QTHREADS: worker unit (%s) does not exist on this machine.\n", typenames[wkr_index]);
                    assert(hwloc_compare_types(tmp->type, HWLOC_OBJ_CACHE_UNIFIED) == 0);
                    goto guess_my_weight;
                }
            }
            qthread_debug(AFFINITY_DETAILS, "found the desired level of cache!!!\n");
            assert(wkr_depth >= shep_depth);
            workerobjs_per_shep = hwloc_get_nbobjs_inside_cpuset_by_depth(topology, obj->cpuset, wkr_depth);
            qthread_debug(AFFINITY_CALLS, "workerobjs type = %s, per_shep = %u\n", hwloc_obj_type_string(wkr_type), workerobjs_per_shep);
            switch (workerobjs_per_shep) {
                case 0:
                    fprintf(stderr, "QTHREADS: worker unit (%s) does not exist on this machine.\n", typenames[wkr_index]);
                    assert(workerobjs_per_shep > 0);
                    goto guess_my_weight;
                case -1:
                    fprintf(stderr, "QTHREADS: worker unit (%s) is not unique on this machine.\n", typenames[wkr_index]);
                    assert(workerobjs_per_shep > 0);
                    goto guess_my_weight;
            }
        } else {
            qthread_debug(AFFINITY_DETAILS, "worker type is NOT a cache...\n");
            workerobjs_per_shep = hwloc_get_nbobjs_inside_cpuset_by_type(topology, obj->cpuset, wkr_type);
            qthread_debug(AFFINITY_CALLS, "workerobjs = %s, per_shep = %u\n", hwloc_obj_type_string(wkr_type), workerobjs_per_shep);
            switch (workerobjs_per_shep) {
                case 0:
                    fprintf(stderr, "QTHREADS: worker unit (%s) does not exist on this machine.\n", typenames[wkr_index]);
                    assert(workerobjs_per_shep > 0);
                    goto guess_my_weight;
                case -1:
                    fprintf(stderr, "QTHREADS: worker unit (%s) is not unique on this machine.\n", typenames[wkr_index]);
                    assert(workerobjs_per_shep > 0);
                    goto guess_my_weight;
            }
            wkr_depth = hwloc_get_type_depth(topology, wkr_type);
            qthread_debug(AFFINITY_DETAILS, "wkr_depth = %u\n", wkr_depth);
            assert(wkr_depth >= shep_depth);
        }
        if (wkr_depth < shep_depth) {
            fprintf(stderr, "worker unit (%s) is not lower in hierarchy than shepherd (%s)\n",
                    hwloc_obj_type_string(wkr_type),
                    hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth)));
            wkr_type  = HWLOC_OBJ_PU;
            wkr_index = -1;
            assert(wkr_depth >= shep_depth);
            goto guess_my_weight;
        }
        ret = workerobjs_per_shep;
    } else {
guess_my_weight:
        wkr_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
        qthread_debug(AFFINITY_BEHAVIOR, "guessing number of workers based on shepherd \"WEIGHT\", wkr_depth = %u\n", wkr_depth);

        const size_t max_idx = hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth);

        for (size_t idx = 0; idx < nshepherds && idx < max_idx; ++idx) {
            hwloc_obj_t  obj    = hwloc_get_obj_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth, idx);
            unsigned int weight = WEIGHT(obj->cpuset);
            qthread_debug(AFFINITY_DETAILS, "%s %u has %u weight\n", hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth)), (unsigned int)idx, weight);
            total += weight;
            if ((idx == 0) || (ret < weight)) {
                ret = weight;
            }
        }
        if (ret * nshepherds > total) {
            qthread_debug(AFFINITY_DETAILS, "ret(%u) * nshepherds(%u) > total(%u)\n", ret, nshepherds, total);
            ret = total / nshepherds;
        }
    }
    if (ret == 0) {
        qthread_debug(AFFINITY_DETAILS, "ret == 0; assuming at least 1\n");
        ret = 1;
    }
    qthread_debug(AFFINITY_FUNCTIONS, "guessed %i workers apiece for %i sheps\n", (int)ret, (int)nshepherds);
    return ret;
}                                      /*}}} */

void INTERNAL qt_affinity_set(qthread_worker_t *me,
                              unsigned int      nworkerspershep)
{                                                                                                /*{{{ */
    ASSERT_ONLY(hwloc_topology_check(topology));

    const hwloc_const_cpuset_t allowed_cpuset = hwloc_topology_get_allowed_cpuset(topology);           // where am I allowed to run?
    qthread_shepherd_t *const  myshep         = me->shepherd;
    unsigned int               maxshepobjs    = hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth);
    hwloc_obj_t                obj            = hwloc_get_obj_inside_cpuset_by_depth(topology, allowed_cpuset,
                                                                                     shep_depth, myshep->node);
    qthread_debug(AFFINITY_DETAILS, "node = %u\n", myshep->node);
    assert(wkr_depth >= 0);
            qthread_debug(AFFINITY_DETAILS, "wkr_depth = %u\n", wkr_depth);
            qthread_debug(AFFINITY_DETAILS, "num_wkrs = %u\n", hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, wkr_depth));
            qthread_debug(AFFINITY_DETAILS, "num_wkrs = %u\n", hwloc_get_nbobjs_inside_cpuset_by_depth(topology, obj->cpuset, wkr_depth));
    hwloc_obj_t  worker0             = hwloc_get_obj_inside_cpuset_by_depth(topology, obj->cpuset, wkr_depth, 0);
    unsigned int workerobjs_per_shep = hwloc_get_nbobjs_inside_cpuset_by_depth(topology, obj->cpuset, wkr_depth);

    assert(workerobjs_per_shep > 0);
    assert(worker0);

    qthread_debug(AFFINITY_CALLS,
                  "shep %i(%i) worker %i [%i], there are %u %s [%i pu]\n",
                  (int)myshep->shepherd_id,
                  (int)myshep->node,
                  (int)me->worker_id,
                  (int)me->packed_worker_id,
                  maxshepobjs,
                  hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth)),
                  (int)hwloc_get_nbobjs_inside_cpuset_by_type(topology, allowed_cpuset, HWLOC_OBJ_PU));
    int shep_pus = hwloc_get_nbobjs_inside_cpuset_by_type(topology, obj->cpuset, HWLOC_OBJ_PU);
    assert(shep_pus > 0);
    unsigned int worker_pus         = hwloc_get_nbobjs_inside_cpuset_by_type(topology, worker0->cpuset, HWLOC_OBJ_PU);
    unsigned int wraparounds        = me->packed_worker_id / (maxshepobjs * nworkerspershep);
    unsigned int worker_wraparounds = me->worker_id / workerobjs_per_shep;
    hwloc_obj_t  sub_obj            =
        hwloc_get_obj_inside_cpuset_by_type(topology, obj->cpuset,
                                            HWLOC_OBJ_PU,
                                            ((me->worker_id * worker_pus) +
                                             (wraparounds * nworkerspershep) +
                                             worker_wraparounds) % shep_pus);

#ifdef QTHREAD_DEBUG_AFFINITY
    {
        char *str;
        qthread_debug(AFFINITY_DETAILS, "%u: shep_pus=%u, wraparounds=%u\n",
                      (unsigned)me->packed_worker_id,
                      shep_pus, wraparounds);
        qthread_debug(AFFINITY_DETAILS, "%u: (%i*%i) + ((%i + (%i * %i)) % %i)\n",
                      (unsigned)me->packed_worker_id,
                      (int)shep_pus, (int)myshep->node, (int)me->worker_id, (int)wraparounds, (int)nworkerspershep, (int)shep_pus);
        qthread_debug(AFFINITY_DETAILS, "%u: (%i*%i) + (((%i * %i) + (%i * %i) + %i) % %i)\n",
                      (unsigned)me->packed_worker_id,
                      (int)shep_pus, (int)myshep->node, (int)me->worker_id, (int)worker_pus, (int)wraparounds, (int)nworkerspershep, (int)worker_wraparounds, (int)shep_pus);
        ASPRINTF(&str, sub_obj->cpuset);
        qthread_debug(AFFINITY_BEHAVIOR,
                      "binding shep %i worker %i (%i) to PU %i, newPU %i, mask %s\n",
                      (int)myshep->shepherd_id, (int)me->worker_id,
                      (int)me->packed_worker_id,
                      (shep_pus * myshep->node) + ((me->worker_id + (wraparounds * nworkerspershep)) % shep_pus),
                      (shep_pus * myshep->node) + (((me->worker_id * worker_pus) + (wraparounds * nworkerspershep) + (worker_wraparounds)) % shep_pus),
                      str);
        FREE(str, strlen(str));
    }
#endif /* ifdef QTHREAD_DEBUG_AFFINITY */
    if (hwloc_set_cpubind(topology, sub_obj->cpuset, HWLOC_CPUBIND_THREAD)) {
        char *str;
        int   i = errno;
#ifdef __APPLE__
        if (i == ENOSYS) {
            return;
        }
#endif
        ASPRINTF(&str, sub_obj->cpuset);
        fprintf(stderr, "Couldn't bind to cpuset %s because %s (%i)\n", str,
                strerror(i), i);
        FREE(str, strlen(str));
    }
}                                      /*}}} */

int INTERNAL qt_affinity_gendists(qthread_shepherd_t   *sheps,
                                  qthread_shepherd_id_t nshepherds)
{                                                                                      /*{{{ */
    DEBUG_ONLY(hwloc_topology_check(topology));

    hwloc_const_cpuset_t allowed_cpuset = hwloc_topology_get_allowed_cpuset(topology); // where am I allowed to run?
    size_t               num_extant_objs;

    qthread_debug(AFFINITY_CALLS, "generating distances for %i sheps (%p)\n", (int)nshepherds, sheps);

    num_extant_objs = hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth);
    qthread_debug(AFFINITY_DETAILS, "found %u objects at shep_depth %u\n", num_extant_objs, shep_depth);
    assert(num_extant_objs != 0);

    for (size_t i = 0; i < nshepherds; ++i) {
        sheps[i].node            = i % num_extant_objs;
        sheps[i].sorted_sheplist = qt_calloc(nshepherds - 1,
                                             sizeof(qthread_shepherd_id_t));
        sheps[i].shep_dists      = qt_calloc(nshepherds, sizeof(unsigned int));
    }
#ifdef QTHREAD_HAVE_HWLOC_DISTS
    const struct hwloc_distances_s *matrix = hwloc_get_whole_distance_matrix_by_type(topology, HWLOC_OBJ_NODE);
    if (matrix) {
        qthread_debug(AFFINITY_DETAILS, "matrix is %p, type at this depth: %s, relative_depth: %u(%s), nbobj: %u\n", matrix, hwloc_obj_type_string(HWLOC_OBJ_NODE), matrix->relative_depth, hwloc_obj_type_string(hwloc_get_depth_type(topology, matrix->relative_depth)), matrix->nbobjs);
        assert(matrix->latency);
    } else {
        qthread_debug(AFFINITY_DETAILS, "matrix is %p, type at this depth: %s\n", matrix, hwloc_obj_type_string(HWLOC_OBJ_NODE));
    }
    size_t node_to_NUMAnode[num_extant_objs];
    for (size_t i = 0; i < num_extant_objs; ++i) {
        hwloc_obj_t node_obj = hwloc_get_obj_inside_cpuset_by_depth(topology, allowed_cpuset, shep_depth, i);
        while (node_obj->type > HWLOC_OBJ_NODE) {
            node_obj = node_obj->parent;
            assert(node_obj);
        }
        node_to_NUMAnode[i] = node_obj->logical_index;
        qthread_debug(AFFINITY_DETAILS, "obj %i maps to node %i\n", i, node_to_NUMAnode[i]);
    }
#endif /* ifdef QTHREAD_HAVE_HWLOC_DISTS */
    for (size_t i = 0; i < nshepherds; ++i) {
        for (size_t j = 0, k = 0; j < nshepherds; ++j) {
            if (j != i) {
#ifdef QTHREAD_HAVE_HWLOC_DISTS
                if (matrix) {
                    sheps[i].shep_dists[j] = matrix->latency[node_to_NUMAnode[sheps[i].node] + matrix->nbobjs * node_to_NUMAnode[sheps[j].node]] * 10;
                    qthread_debug(AFFINITY_DETAILS, "distance from %i(%i) to %i(%i) is %i\n",
                                  (int)i, (int)sheps[i].node,
                                  (int)j, (int)sheps[j].node,
                                  (int)(sheps[i].shep_dists[j]));
                } else {
                    // handle what is fundamentally a bug in old versions of hwloc
                    sheps[i].shep_dists[j] = 10;
                    qthread_debug(AFFINITY_DETAILS, "pretending distance from %i to %i is %i\n", (int)i, (int)j, (int)(sheps[i].shep_dists[j]));
                }
#else          /* ifdef QTHREAD_HAVE_HWLOC_DISTS */
                sheps[i].shep_dists[j] = 10;
                qthread_debug(AFFINITY_DETAILS, "pretending distance from %i to %i is %i\n", (int)i, (int)j, (int)(sheps[i].shep_dists[j]));
#endif         /* ifdef QTHREAD_HAVE_HWLOC_DISTS */
                sheps[i].sorted_sheplist[k++] = j;
            }
        }
        if (nshepherds > 1) {
            sort_sheps(sheps[i].shep_dists, sheps[i].sorted_sheplist, nshepherds);
        }
    }
       /* there does not seem to be a way to extract distances... <sigh> */
    return QTHREAD_SUCCESS;
}                                      /*}}} */

/* vim:set expandtab: */
