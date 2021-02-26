#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <hwloc.h>

#include "qt_subsystems.h"
#include "qt_asserts.h" /* for qassert() */
#include "qt_affinity.h"
#include "qt_debug.h"
#include "qt_envariables.h"
#include "qt_output_macros.h"
#include "shufflesheps.h"

static hwloc_topology_t sys_topo;
static uint32_t         initialized = 0;

static hwloc_cpuset_t mccoy_thread_bindings;

static unsigned int qt_topology_output_level = 0;

typedef struct {
    int                   uid;
    qthread_shepherd_id_t shep_id;
    qthread_worker_id_t   worker_id;
    hwloc_obj_t           bind_obj;
} topo_worker_t;

typedef struct {
    /* Shepherd mapping info */
    int         num_sheps;
    int         shep_level;
    hwloc_obj_t shep_obj;

    /* Worker mapping info */
    int         num_workers;
    int         worker_level;
    hwloc_obj_t worker_obj;

    /* linear mapping */
    topo_worker_t * worker_map;

    int num_wps;
} qt_topology_t;

static qt_topology_t qt_topo;

static void initialize_qt_topo(void)
{
    qt_topo.num_sheps    = -1;
    qt_topo.shep_level   = -1;
    qt_topo.shep_obj     = NULL;
    qt_topo.num_workers  = -1;
    qt_topo.worker_level = -1;
    qt_topo.worker_obj   = NULL;
    qt_topo.num_wps      = -1;
    qt_topo.worker_map   = NULL;
}

#define HIERARCHY_NAME_LEN 128
static hwloc_obj_type_t * topo_types;
static char            (* topo_type_names)[HIERARCHY_NAME_LEN];
static int                num_types;

static void print_system_view(hwloc_topology_t sys_topo)
{
    char str[128];
    hwloc_obj_t obj = NULL;
    hwloc_const_cpuset_t allowed_cpuset =
        hwloc_topology_get_allowed_cpuset(sys_topo);

    obj = hwloc_get_obj_inside_cpuset_by_depth(sys_topo, allowed_cpuset, 0, 0);
    while (NULL != obj) {
        int const depth = obj->depth;
        int const arity = obj->arity;
        int const num_objs =
            hwloc_get_nbobjs_inside_cpuset_by_depth(
                sys_topo, allowed_cpuset, depth);

        hwloc_obj_snprintf(str, sizeof(str), sys_topo, obj, "#", 0);

        printf("TOPO: depth: %d\n", depth);
        printf("TOPO:\tarity: %d\n", arity);
        printf("TOPO:\tnum_objs: %d\n", num_objs);
        printf("TOPO:\ttype: %s\n", str);

        obj = obj->first_child;
    }

    int const num_cores =
        hwloc_get_nbobjs_inside_cpuset_by_type(
            sys_topo, allowed_cpuset, HWLOC_OBJ_CACHE_UNIFIED);

    printf("TOPO: number of available COREs: %d\n", num_cores);
}

static void print_logical_view(void)
{
    char shep_level[128];
    char worker_level[128];
    char str[128];

    printf("QT_TOPO: shep_level: %d\n", qt_topo.shep_level);
    if (NULL != qt_topo.shep_obj) {
        hwloc_obj_type_snprintf(shep_level, sizeof(shep_level), qt_topo.shep_obj, 0);
        printf("QT_TOPO: shep type: %s\n", shep_level);
    }

    printf("QT_TOPO: worker_level: %d\n", qt_topo.worker_level);
    if (NULL != qt_topo.worker_obj) {
        hwloc_obj_type_snprintf(worker_level, sizeof(worker_level), qt_topo.worker_obj, 0);
        printf("QT_TOPO: worker type: %s\n", worker_level);
    }

    printf("QT_TOPO: #(sheps): %d\n", qt_topo.num_sheps);
    printf("QT_TOPO: #(workers): %d\n", qt_topo.num_workers);
    printf("QT_TOPO: #(wps): %d\n", qt_topo.num_wps);

    if (!strcmp(worker_level, "Core")) {
        hwloc_obj_snprintf(
            str, sizeof(str), sys_topo, qt_topo.worker_map[0].bind_obj->first_child, "#", 0);
        printf("worker_map: {%d(%d,%d, Core containing %s)", qt_topo.worker_map[0].uid, qt_topo.worker_map[0].shep_id, qt_topo.worker_map[0].worker_id, str);
    } else {
        hwloc_obj_snprintf(
            str, sizeof(str), sys_topo, qt_topo.worker_map[0].bind_obj, "#", 0);
        printf("worker_map: {%d(%d,%d,%s)", qt_topo.worker_map[0].uid, qt_topo.worker_map[0].shep_id, qt_topo.worker_map[0].worker_id, str);
    }

    for (int i = 1; i < qt_topo.num_workers; i++) {
        if (!strcmp(worker_level, "Core")) {
            hwloc_obj_snprintf(
                str, sizeof(str), sys_topo, qt_topo.worker_map[i].bind_obj->first_child, "#", 0);
            printf(", %d(%d,%d, Core containing %s)", qt_topo.worker_map[i].uid, qt_topo.worker_map[i].shep_id, qt_topo.worker_map[i].worker_id, str);
        } else {
            hwloc_obj_snprintf(
                str, sizeof(str), sys_topo, qt_topo.worker_map[i].bind_obj, "#", 0);
            printf(", %d(%d,%d,%s)", qt_topo.worker_map[i].uid, qt_topo.worker_map[i].shep_id, qt_topo.worker_map[i].worker_id, str);
        }
    }
    printf("}\n");

}

static void fini_type_options(void)
{
    qthread_debug(AFFINITY_CALLS, "destroying type options\n");

    qt_free(topo_types);
    qt_free(topo_type_names);
}

static void init_type_options(void)
{
    qthread_debug(AFFINITY_CALLS, "creating type options\n");

    hwloc_const_cpuset_t allowed_cpuset =
        hwloc_topology_get_allowed_cpuset(sys_topo);
    hwloc_obj_t obj =
        hwloc_get_obj_inside_cpuset_by_depth(
            sys_topo, allowed_cpuset, 0, 0);

    /* Walk down tree */
    while (NULL != obj) {
        num_types += 1;

        obj = obj->first_child;
    }

    topo_types = qt_malloc(num_types * sizeof(hwloc_obj_type_t));
    topo_type_names = qt_malloc(num_types * HIERARCHY_NAME_LEN * sizeof(char));
    assert(NULL != topo_types);
    assert(NULL != topo_type_names);

    /* Walk up tree */
    obj = hwloc_get_obj_inside_cpuset_by_depth(sys_topo, allowed_cpuset,
                                               num_types-1, 0);
    int cache_level = 1;
    int type_id = num_types-1;
    while (NULL != obj) {
        topo_types[type_id] = obj->type;

        if (0 == hwloc_compare_types(HWLOC_OBJ_CACHE_UNIFIED, obj->type)) {
            snprintf(topo_type_names[type_id], 8, "L%dcache", cache_level);
            cache_level += 1;
        } else {
            strncpy(topo_type_names[type_id],
                    hwloc_obj_type_string(obj->type),
                    HIERARCHY_NAME_LEN);
        }

        type_id -= 1;
        obj = obj->parent;
    }
}

static void print_type_options(void)
{
    printf("type_options: {%s", topo_type_names[0]);
    for (int i = 1; i < num_types; i++) {
        printf(", %s", topo_type_names[i]);
    }
    printf("}\n");
}

static hwloc_obj_type_t wkr_type  = HWLOC_OBJ_CORE;
static int              wkr_index = -1;
static int              wkr_depth = -1;

#if HWLOC_API_VERSION < 0x00010100
# define hwloc_bitmap_weight        hwloc_cpuset_weight
# define hwloc_bitmap_asprintf      hwloc_cpuset_asprintf
# define hwloc_bitmap_foreach_begin hwloc_cpuset_foreach_begin
# define hwloc_bitmap_foreach_end   hwloc_cpuset_foreach_end
# define hwloc_bitmap_alloc         hwloc_cpuset_alloc
# define hwloc_bitmap_free          hwloc_cpuset_free
#endif /* if HWLOC_API_VERSION < 0x00010100 */

static void qt_affinity_internal_hwloc_teardown(void)
{   /*{{{*/
    DEBUG_ONLY(hwloc_topology_check(sys_topo));

    fini_type_options();

    /* Reestablish mccoy thread bindings */
    hwloc_set_cpubind(sys_topo, mccoy_thread_bindings, HWLOC_CPUBIND_THREAD);
    hwloc_bitmap_free(mccoy_thread_bindings);

    qt_free(qt_topo.worker_map);
    hwloc_topology_destroy(sys_topo);
    initialized = 0;
} /*}}}*/

/*
 * User hints:
 * - QT_TOPO_OUTPUT_LEVEL: The amount of topology information to print. Level
 *                         '2' will print a synopsis of the topology reported
 *                         by hwloc. Currently only level '2' is used.
 * - QT_SHEPHERD_BOUNDARY: The level in the hierarchy to associate with
 *                         shepherds.
 * - QT_WORKER_UNIT:       The level in the hierarchy to use for binding
 *                         workers.
 *                         The worker will be bound to the set of COREs under this
 *                         level.
 */
void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers,
                               size_t                *hw_par)
{                                      /*{{{ */
    qthread_debug(AFFINITY_CALLS, "nbshepherds=%u, nbworkers=%u, hw_par=%u\n", *nbshepherds, *nbworkers, *hw_par);

    /* Collect user hints */
    qthread_shepherd_id_t num_sheps_hint   = *nbshepherds;
    qthread_worker_id_t   num_wps_hint     = *nbworkers;
    size_t                num_workers_hint = *hw_par;

    /* Set up logical sys_topo */
    initialize_qt_topo();

    /* Initialize hwloc setup */
    if (qthread_cas(&initialized, 0, 1) == 0) {
        qassert(hwloc_topology_init(&sys_topo), 0);
        qassert(hwloc_topology_load(sys_topo), 0);
        MACHINE_FENCE;
        initialized = 2;
    } else {
        while (initialized == 1) SPINLOCK_BODY();
    }
    DEBUG_ONLY(hwloc_topology_check(sys_topo));
    qthread_internal_cleanup(qt_affinity_internal_hwloc_teardown);

    /* Record mccoy thread bindings so we can reestablish during finalization */
    mccoy_thread_bindings = hwloc_bitmap_alloc();
    hwloc_get_cpubind(sys_topo, mccoy_thread_bindings, HWLOC_CPUBIND_THREAD);

    hwloc_const_cpuset_t allowed_cpuset =
        hwloc_topology_get_allowed_cpuset(sys_topo);

    /* Collect CORE info */
    hwloc_obj_t core_obj =
        hwloc_get_obj_inside_cpuset_by_type(
            sys_topo, allowed_cpuset, HWLOC_OBJ_CORE, 0);
    if (NULL == core_obj) {
        print_error("failed to locate CORE#0\n");
        exit(EXIT_FAILURE);
    }
    int const num_cores =
        hwloc_get_nbobjs_inside_cpuset_by_depth(
            sys_topo, allowed_cpuset, core_obj->depth);
    if (0 >= core_obj) {
        print_error("failed to count COREs\n");
        exit(EXIT_FAILURE);
    }

    /* Process boundary and unit environment variables */
    int shep_type_id   = -1;
    int worker_type_id = -1;

    /* Build up sys_topo options */
    init_type_options();

    /* Collect environment variables */
    {
        qt_topology_output_level =
            qt_internal_get_env_num("TOPO_OUTPUT_LEVEL", 0, 0);
    }
    {
        const char *qsh = qt_internal_get_env_str("SHEPHERD_BOUNDARY", "node");
        if (qsh) {
            for (int ti = 0; ti < num_types; ++ti) {
                if (!strncasecmp(topo_type_names[ti], qsh,
                                 strlen(topo_type_names[ti]))) {
                    shep_type_id = ti;
                }
            }
            if (shep_type_id == -1) {
                fprintf(stderr, "unparsable shepherd boundary (%s)\n", qsh);
                exit(EXIT_FAILURE);
            }
        }

        qsh = qt_internal_get_env_str("WORKER_UNIT", "pu");
        if (qsh) {
            for (int ti = 0; ti < num_types; ++ti) {
                if (!strncasecmp(topo_type_names[ti], qsh,
                                 strlen(topo_type_names[ti]))) {
                    worker_type_id = ti;
                }
            }
            if (worker_type_id == -1) {
                fprintf(stderr, "unparsable worker unit (%s)\n", qsh);
                exit(EXIT_FAILURE);
            }
        }
    }

    /* Print system view */
    if (2 == qt_topology_output_level) {
        print_system_view(sys_topo);
        print_type_options();
    }

    /**************************************************************************/

    qthread_debug(AFFINITY_DETAILS, "HINTS: max-sheps: %d\n", num_sheps_hint);
    qthread_debug(AFFINITY_DETAILS, "HINTS: max-wps: %d\n", num_wps_hint);
    qthread_debug(AFFINITY_DETAILS, "HINTS: max-workers: %d\n", num_workers_hint);
    qthread_debug(AFFINITY_DETAILS, "HINTS: shep-boundary: %d\n", shep_type_id);
    qthread_debug(AFFINITY_DETAILS, "HINTS: worker-unit: %d\n", worker_type_id);

    /* Process boundary and units requests */
    if (-1 != worker_type_id) {
        /* User specified worker unit */

        hwloc_obj_t worker_obj =
            hwloc_get_obj_inside_cpuset_by_type(
                sys_topo, allowed_cpuset, topo_types[worker_type_id], 0);
        if (NULL == worker_obj) {
            print_error("failed to locate worker unit object\n");
            exit(EXIT_FAILURE);
        }

        /* Calculate number of these objects */
        int num_objs =
            hwloc_get_nbobjs_inside_cpuset_by_type(
                sys_topo, allowed_cpuset, topo_types[worker_type_id]);

        qthread_debug(AFFINITY_DETAILS, "found %d %s unit obj(s)\n", num_objs, topo_type_names[worker_type_id]);

        /* Update logical sys_topo info */
        qt_topo.worker_obj = worker_obj;
        qt_topo.worker_level = worker_obj->depth;

        /* Update hints */
    }
    if (-1 != shep_type_id) {
        /* User specified shepherd boundary */

        hwloc_obj_t shep_obj =
            hwloc_get_obj_inside_cpuset_by_type(
                sys_topo, allowed_cpuset, topo_types[shep_type_id], 0);
        if (NULL == shep_obj) {
            print_error("failed to locate shepherd boundary object\n");
            exit(EXIT_FAILURE);
        }

        /* Calculate number of these objects */
        int const num_shep_objs =
            hwloc_get_nbobjs_inside_cpuset_by_type(
                sys_topo, allowed_cpuset, topo_types[shep_type_id]);

        qthread_debug(AFFINITY_DETAILS, "found %d %s shep obj(s)\n", num_shep_objs, topo_type_names[shep_type_id]);

        /* Calculate number of CORE within boundary: this is max num-workers */
        int const num_shep_cores = num_cores / num_shep_objs;

        /* Update logical sys_topo info */
        qt_topo.shep_obj = shep_obj;
        qt_topo.shep_level = shep_obj->depth;

        /* Update hints */
        if (0 == num_sheps_hint || num_shep_objs < num_sheps_hint) {
            qthread_debug(AFFINITY_DETAILS, "%s shep obj => max-sheps=%d\n", topo_type_names[shep_type_id], num_shep_objs);
            num_sheps_hint = num_shep_objs;
        }
        if (0 == num_wps_hint || num_shep_cores < num_wps_hint) {
            qthread_debug(AFFINITY_DETAILS, "%s shep obj => max-wps=%d\n", topo_type_names[shep_type_id], num_shep_cores);
            num_wps_hint = num_shep_cores;
        }
    }

    /* Establish boundary and unit */
    if (NULL == qt_topo.worker_obj) {
        qthread_debug(AFFINITY_DETAILS, "No worker unit hint, choosing Core unit obj\n");

        /* Assume unit is CORE; handle boundary selection below. */
        qt_topo.worker_obj   = core_obj;
        qt_topo.worker_level = core_obj->depth;
    }

    if (NULL == qt_topo.shep_obj) {
        /* Have only unit, must find boundary */

        /* Policy:
         * - If have num-sheps and num-wps hints, then select first shared level
         *   that accomodates both values.
         * - If have only num-wps hint, then select first shared level that
         *   accomodates that values.
         * - If have only num-sheps hint, then select first shared level that
         *   accomodates that values.
         * - Otherwise select first shared level at or above the unit level. */

        /* Policy: select the first shared level at or above the unit with
         *         at least num-wps shared COREs (if requested) and at most
         *         num-sheps boundary objects (if requested); num-wps should
         *         take precedence over num-sheps */
        hwloc_obj_t shep_obj = core_obj;

        /* Walk up tree counting shared COREs */
        int shared = 1;
        while (NULL != shep_obj->parent) {
            shared *= (shep_obj->arity == 0) ? 1 : shep_obj->arity;

            if (0 != num_sheps_hint && 0 != num_wps_hint) {
                /* Have both num-sheps and num-wps hints */
                int num_shep_objs =
                    hwloc_get_nbobjs_inside_cpuset_by_depth(
                        sys_topo, allowed_cpuset, shep_obj->depth);
                if (num_shep_objs == num_sheps_hint && shared >= num_wps_hint) {
                    /* Choose this level */
                    break;
                }
            } else if (0 != num_sheps_hint) {
                /* Have only num-sheps hint */
                int num_shep_objs =
                    hwloc_get_nbobjs_inside_cpuset_by_depth(
                        sys_topo, allowed_cpuset, shep_obj->depth);
                if (num_shep_objs == num_sheps_hint) {
                    /* Choose this level */
                    break;
                }
            } else if (0 != num_wps_hint) {
                /* Have only num-wps hint */
                if (shared >= num_wps_hint) {
                    /* Choose this level */
                    break;
                }
            } else {
                /* No hints */
                if (shep_obj->depth <= qt_topo.worker_obj->depth && 1<shared) {
                    /* Choose this level */
                    break;
                }
            }

            shep_obj = shep_obj->parent;
        }

        /* Update logical sys_topo info */
        qt_topo.shep_obj = shep_obj;
        qt_topo.shep_level = shep_obj->depth;

        qthread_debug(AFFINITY_DETAILS, "chose %s shep obj\n", topo_type_names[qt_topo.shep_level]);

        /* Update hints */
    }

    /* Sanity-check that boundary and units are reasonable */
    assert(NULL != qt_topo.shep_obj && NULL != qt_topo.worker_obj);
    if (qt_topo.shep_level > qt_topo.worker_level) {
        print_warning("shepherd boundary smaller than worker unit\n");
    }

    /**************************************************************************/

    /* Establish num-sheps and num-wps */
    if (0 == num_sheps_hint && 0 == num_wps_hint) {
        /* No sys_topo hints */
        qt_topo.num_sheps =
            hwloc_get_nbobjs_inside_cpuset_by_depth(
                sys_topo, allowed_cpuset, qt_topo.shep_obj->depth);
        qt_topo.num_wps = num_cores / qt_topo.num_sheps;
    } else if (0 == num_wps_hint) {
        /* Only sheps hint given */
        int num_sheps_objs =
            hwloc_get_nbobjs_inside_cpuset_by_depth(
                sys_topo, allowed_cpuset, qt_topo.shep_obj->depth);

        if (num_sheps_hint <= num_sheps_objs) {
            qt_topo.num_sheps = num_sheps_hint;
        } else {
            print_warning("disregarding num-sheps hint (%d) in favor of maximum number of objects at shepherd boundary level (%d)\n", num_sheps_hint, num_sheps_objs);
            qt_topo.num_sheps = num_sheps_objs;
        }
        qt_topo.num_wps = num_cores / qt_topo.num_sheps;
    } else if (0 == num_sheps_hint) {
        /* Only WPS hint given */
        qt_topo.num_sheps =
            hwloc_get_nbobjs_inside_cpuset_by_depth(
                sys_topo, allowed_cpuset, qt_topo.shep_obj->depth);
        qt_topo.num_wps = num_wps_hint;
    } else {
        /* Both hints */
        qt_topo.num_sheps = num_sheps_hint;
        qt_topo.num_wps   = num_wps_hint;
    }

    /* Establish num-workers */
    if (0 != num_workers_hint) {
        /* Adjust num-wps count to account for requested num-workers */
        if ((num_workers_hint / num_sheps_hint) > num_wps_hint) {
            print_error("invalid HWPAR, too many workers\n");
            exit(EXIT_FAILURE);
        }
        if (0 != num_workers_hint % num_sheps_hint) {
            print_error("invalid HWPAR, odd workers\n");
            exit(EXIT_FAILURE);
        }

        qt_topo.num_workers = num_workers_hint;
        qt_topo.num_wps = num_workers_hint / qt_topo.num_sheps;
    } else {
        qt_topo.num_workers = qt_topo.num_sheps * qt_topo.num_wps;
    }

    /* Construct worker map */
    qt_topo.worker_map = qt_malloc(qt_topo.num_workers * sizeof(topo_worker_t));
    assert(qt_topo.worker_map);

    for (int i = 0; i < qt_topo.num_sheps; i++) {
        for (int j = 0; j < qt_topo.num_wps; j++) {
            int uid = (i * qt_topo.num_wps) + j;
            qt_topo.worker_map[uid].uid       = uid;
            qt_topo.worker_map[uid].shep_id   = i;
            qt_topo.worker_map[uid].worker_id = j;

            /* Set binding location */
            hwloc_obj_t shep_obj =
                hwloc_get_obj_inside_cpuset_by_depth(
                    sys_topo, allowed_cpuset, qt_topo.shep_level, i);
            hwloc_obj_t logical_core_obj =
                hwloc_get_obj_inside_cpuset_by_type(
                    sys_topo, shep_obj->cpuset, HWLOC_OBJ_CORE, j);
            qt_topo.worker_map[uid].bind_obj =
                hwloc_get_ancestor_obj_by_depth(
                    sys_topo, qt_topo.worker_obj->depth, logical_core_obj);
        }
    }

    /* Print logical sys_topo */
    if (2 == qt_topology_output_level) {
        print_logical_view();
    }

    /* Set "outputs" */
    *nbshepherds = qt_topo.num_sheps;
    *nbworkers   = qt_topo.num_wps;

    wkr_type     = qt_topo.worker_obj->type;
    wkr_depth    = qt_topo.worker_level;

    return;
}                                      /*}}} */

void INTERNAL qt_affinity_set(qthread_worker_t *me,
                              unsigned int      nworkerspershep)
{   /*{{{ */
    ASSERT_ONLY(hwloc_topology_check(sys_topo));

    hwloc_obj_t target_obj = qt_topo.worker_map[me->unique_id - 1].bind_obj;
    if (hwloc_set_cpubind(sys_topo, target_obj->cpuset,
                          HWLOC_CPUBIND_THREAD)) {
        char *str;
        int   i = errno;
#ifdef __APPLE__
        if (i == ENOSYS) {
            return;
        }
#endif
        hwloc_bitmap_asprintf(&str, target_obj->cpuset);
        fprintf(stderr, "Couldn't bind to cpuset %s because %s (%i)\n", str,
                strerror(i), i);
        FREE(str, strlen(str));
    }
}                                      /*}}} */

int INTERNAL qt_affinity_gendists(qthread_shepherd_t   *sheps,
                                  qthread_shepherd_id_t nshepherds)
{   /*{{{ */
    qthread_debug(AFFINITY_CALLS, "generating distances for %i sheps (%p)\n", (int)qt_topo.num_sheps, sheps);

    for (size_t i = 0; i < qt_topo.num_sheps; i++) {
        sheps[i].node            = i % qt_topo.num_sheps;
        sheps[i].sorted_sheplist = qt_calloc(qt_topo.num_sheps - 1,
                                             sizeof(qthread_shepherd_id_t));
        sheps[i].shep_dists      = qt_calloc(qt_topo.num_sheps,
                                             sizeof(unsigned int));
    }

#ifdef QTHREAD_HAVE_HWLOC_DISTS
    hwloc_const_cpuset_t allowed_cpuset =
        hwloc_topology_get_allowed_cpuset(sys_topo);
    /* XXX: should this really find the obj closest to the shep level that
     *      has a distance matrix? */
    const struct hwloc_distances_s * matrix =
        hwloc_get_whole_distance_matrix_by_type(
            sys_topo, HWLOC_OBJ_NODE);

    if (matrix) {
        qthread_debug(AFFINITY_DETAILS, "matrix is %p, type at this depth: %s, relative_depth: %u(%s), nbobj: %u\n", matrix, hwloc_obj_type_string(qt_topo.shep_obj->type), matrix->relative_depth, hwloc_obj_type_string(hwloc_get_depth_type(sys_topo, matrix->relative_depth)), matrix->nbobjs);
        assert(matrix->latency);
    } else {
        qthread_debug(AFFINITY_DETAILS, "matrix is %p, type at this depth: %s\n", matrix, hwloc_obj_type_string(HWLOC_OBJ_NODE));
    }
    size_t node_to_NUMAnode[qt_topo.num_sheps];
    for (size_t i = 0; i < qt_topo.num_sheps; ++i) {
        hwloc_obj_t node_obj = hwloc_get_obj_inside_cpuset_by_depth(sys_topo, allowed_cpuset, qt_topo.shep_level, i);
        while (node_obj->type > HWLOC_OBJ_NODE) {
            node_obj = node_obj->parent;
            assert(node_obj);
        }
        node_to_NUMAnode[i] = node_obj->logical_index;
        qthread_debug(AFFINITY_DETAILS, "obj %i maps to node %i\n", i, node_to_NUMAnode[i]);
    }
#endif /* ifdef QTHREAD_HAVE_HWLOC_DISTS */

    for (size_t i = 0; i < qt_topo.num_sheps; ++i) {
        for (size_t j = 0, k = 0; j < qt_topo.num_sheps; ++j) {
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
        if (qt_topo.num_sheps > 1) {
            sort_sheps(sheps[i].shep_dists, sheps[i].sorted_sheplist,
                       qt_topo.num_sheps);
        }
    }
    /* there does not seem to be a way to extract distances... <sigh> */
    return QTHREAD_SUCCESS;
}                                      /*}}} */

#ifdef QTHREAD_HAVE_MEM_AFFINITY
void INTERNAL qt_affinity_mem_tonode(void  *addr,
                                     size_t bytes,
                                     int    node)
{                                      /*{{{ */
    hwloc_nodeset_t nodeset = hwloc_bitmap_alloc();

    DEBUG_ONLY(hwloc_topology_check(sys_topo));
    hwloc_bitmap_set(nodeset, node);
    hwloc_set_area_membind(sys_topo, addr, bytes, nodeset,
                                   HWLOC_MEMBIND_BIND,
                                   HWLOC_MEMBIND_NOCPUBIND);
    hwloc_bitmap_free(nodeset);
}                                      /*}}} */

void INTERNAL *qt_affinity_alloc(size_t bytes)
{                                      /*{{{ */
    DEBUG_ONLY(hwloc_topology_check(sys_topo));
    return hwloc_alloc(sys_topo, bytes);
}                                      /*}}} */

void INTERNAL *qt_affinity_alloc_onnode(size_t bytes,
                                        int    node)
{                                      /*{{{ */
    void           *ret;
    hwloc_nodeset_t nodeset;

    DEBUG_ONLY(hwloc_topology_check(sys_topo));
    nodeset = hwloc_bitmap_alloc();
    hwloc_bitmap_set(nodeset, node);
    ret = hwloc_alloc_membind_nodeset(sys_topo, bytes, nodeset,
                                      HWLOC_MEMBIND_BIND,
                                      HWLOC_MEMBIND_NOCPUBIND);
    hwloc_bitmap_free(nodeset);
    return ret;
}                                      /*}}} */

void INTERNAL qt_affinity_free(void  *ptr,
                               size_t bytes)
{                                      /*{{{ */
    DEBUG_ONLY(hwloc_topology_check(sys_topo));
    hwloc_free(sys_topo, ptr, bytes);
}                                      /*}}} */

#endif /* ifdef QTHREAD_HAVE_MEM_AFFINITY */

/* vim:set expandtab: */
