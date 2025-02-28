#include <hwloc.h>

#if (HWLOC_API_VERSION < 0x00020000)
#error HWLOC version unrecognized
#endif

#include "qt_affinity.h"
#include "qt_alloc.h"
#include "qt_asserts.h" /* for qassert() */
#include "qt_envariables.h"
#include "qt_subsystems.h"
#include "shufflesheps.h"

static hwloc_topology_t topology = NULL;
static uint32_t my_topology = 0;
static uint32_t initialized = 0;

static int shep_depth = -1;
static hwloc_cpuset_t mccoy_thread_bindings;

static hwloc_obj_type_t wkr_type = HWLOC_OBJ_PU;
static int wkr_index = -1;
static int wkr_depth = -1;
char const *typenames[] = {"node",
                           "cache",
                           "socket",
                           "core",
                           "pu",
                           "L1cache",
                           "L2cache",
                           "L3cache",
                           "L4cache"};
size_t const numtypes = sizeof(typenames) / sizeof(char *);

#if HWLOC_API_VERSION == 0x00010000
#define WEIGHT(x) hwloc_cpuset_weight(x)
#define ASPRINTF(x, y) hwloc_cpuset_asprintf((x), (y))
#define FOREACH_START(x, y) hwloc_cpuset_foreach_begin((x), (y))
#define FOREACH_END() hwloc_cpuset_foreach_end()
#define ALLOCBMAP() hwloc_cpuset_alloc()
#define FREEBMAP(x) hwloc_cpuset_free(x)
#else
#define WEIGHT(x) hwloc_bitmap_weight(x)
#define ASPRINTF(x, y) hwloc_bitmap_asprintf((x), (y))
#define FOREACH_START(x, y) hwloc_bitmap_foreach_begin((x), (y))
#define FOREACH_END() hwloc_bitmap_foreach_end()
#define ALLOCBMAP() hwloc_bitmap_alloc()
#define FREEBMAP(x) hwloc_bitmap_free(x)
#endif /* if HWLOC_API_VERSION == 0x00010000 */

qthread_shepherd_id_t guess_num_shepherds(void);
qthread_worker_id_t
guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds);

static unsigned int num_usable_by_depth(unsigned int depth) {
  hwloc_const_cpuset_t allowed_cpuset =
    hwloc_topology_get_allowed_cpuset(topology);
  int max =
    hwloc_get_nbobjs_inside_cpuset_by_depth(topology, allowed_cpuset, depth);
  unsigned int ret = 0;
  for (int i = 0; i < max; ++i) {
    hwloc_obj_t obj =
      hwloc_get_obj_inside_cpuset_by_depth(topology, allowed_cpuset, depth, i);
    if (obj == NULL) { continue; }
    if (hwloc_get_nbobjs_inside_cpuset_by_type(
          topology, obj->cpuset, HWLOC_OBJ_PU) == 0) {
      continue;
    }
    ret++;
  }
  return ret;
}

static void qt_affinity_internal_hwloc_teardown(void) {
  hwloc_set_cpubind(topology, mccoy_thread_bindings, HWLOC_CPUBIND_THREAD);
  FREEBMAP(mccoy_thread_bindings);
  if (my_topology) {
    hwloc_topology_destroy(topology);
    topology = NULL;
    my_topology = 0;
  }
  initialized = 0;
}

void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t *nbworkers,
                               size_t *hw_par) {
  if (qthread_cas(&initialized, 0, 1) == 0) {
    if (topology == NULL) {
      qassert(hwloc_topology_init(&topology), 0);
      qassert(hwloc_topology_load(topology), 0);
      my_topology = 1;
    }
    MACHINE_FENCE;
    initialized = 2;
  } else {
    while (initialized == 1) SPINLOCK_BODY();
  }

  qthread_internal_cleanup(qt_affinity_internal_hwloc_teardown);
  mccoy_thread_bindings = ALLOCBMAP();
  hwloc_get_cpubind(topology, mccoy_thread_bindings, HWLOC_CPUBIND_THREAD);
  hwloc_const_cpuset_t allowed_cpuset =
    hwloc_topology_get_allowed_cpuset(topology);
  int shep_type_idx = -1;
  /* BEWARE: L*cache must be consecutive of ascending levels, as the index
   * below is used to compare to the cache level */
  size_t const shepindexofL1cache = 5;
  hwloc_obj_type_t shep_type_options[] = {HWLOC_OBJ_NODE,
                                          HWLOC_OBJ_CACHE_UNIFIED,
                                          HWLOC_OBJ_SOCKET,
                                          HWLOC_OBJ_CORE,
                                          HWLOC_OBJ_PU,
                                          HWLOC_OBJ_CACHE_UNIFIED,
                                          HWLOC_OBJ_CACHE_UNIFIED,
                                          HWLOC_OBJ_CACHE_UNIFIED,
                                          HWLOC_OBJ_CACHE_UNIFIED};
  {
    char const *qsh = qt_internal_get_env_str("SHEPHERD_BOUNDARY", "node");

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
          wkr_type = shep_type_options[ti];
        }
      }
      if (wkr_index == -1) {
        fprintf(stderr, "unparsable worker unit (%s)\n", qsh);
        exit(EXIT_FAILURE);
      }
    }
  }
  if (*nbshepherds == 0) { /* we need to guesstimate */
    /* the goal here is to basically pick the number of domains over which
     * memory access is the same cost (e.g. number of sockets, if all cores on
     * a given socket share top-level cache). This will define the shepherd
     * boundary, unless the user has specified a shepherd boundary */
    if (shep_type_idx == -1) {
      do {
        shep_type_idx++;
        shep_depth =
          hwloc_get_type_depth(topology, shep_type_options[shep_type_idx]);
        if (shep_type_idx == 3) { break; }
        if ((shep_type_idx == 0) &&
            (hwloc_get_nbobjs_inside_cpuset_by_type(
               topology, allowed_cpuset, shep_type_options[0]) == 1)) {
          continue;
        }
        /* check to make sure that all of these things have PUs */

      } while (shep_depth == HWLOC_TYPE_DEPTH_UNKNOWN ||
               shep_depth == HWLOC_TYPE_DEPTH_MULTIPLE);

      assert(hwloc_get_nbobjs_inside_cpuset_by_depth(
               topology, allowed_cpuset, shep_depth) > 0);
    } else {
      shep_depth =
        hwloc_get_type_depth(topology, shep_type_options[shep_type_idx]);
    }
    if ((shep_depth == HWLOC_TYPE_DEPTH_UNKNOWN) ||
        (shep_depth == HWLOC_TYPE_DEPTH_MULTIPLE)) {
      if ((shep_type_idx > 0) &&
          (shep_type_options[shep_type_idx] == HWLOC_OBJ_CACHE_UNIFIED)) {
        /* caches are almost always weird; so if the user asked for them, just
         * give best effort */
        unsigned int maxdepth = hwloc_topology_get_depth(topology);
        unsigned int curdepth;
        unsigned int cacdepth = maxdepth;
        unsigned int level = 0;
        /* look backward from the PU to be able to identify cache level */
        for (curdepth = maxdepth; curdepth > 0; --curdepth) {
          unsigned int realdepth = curdepth - 1;
          hwloc_obj_type_t t = hwloc_get_depth_type(topology, realdepth);
          if (t == HWLOC_OBJ_CACHE_UNIFIED) {
            level++;
            /* default choice (1): pick the outermost layer of cache */
            /* if user requested L-specific cache, then count & compare */
            /* L1 is _not_ the same as 'pu' _if_ we have hyperthreading */
            /* BEWARE: ugly compare between depth & shep_type_idx */
            if ((shep_type_idx == 1) ||
                (level == (shep_type_idx - (shepindexofL1cache - 1)))) {
              cacdepth = realdepth;
            }
          }
        }
        if (cacdepth == maxdepth) {
          shep_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
        } else {
          shep_depth = cacdepth;
        }
      } else {
        shep_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
      }
    }
    *nbshepherds = guess_num_shepherds();
    *nbworkers = guess_num_workers_per_shep(*nbshepherds);
  } else { /* (*nbshepherds != 0) */
    if (shep_type_idx != -1) {
      shep_depth =
        hwloc_get_type_depth(topology, shep_type_options[shep_type_idx]);
      if (shep_depth <= 0) {
        if (shep_type_options[shep_type_idx] == HWLOC_OBJ_CACHE_UNIFIED) {
          shep_type_idx = 1;
        }
        shep_type_idx++;
        while (((shep_depth = hwloc_get_type_depth(
                   topology, shep_type_options[shep_type_idx])) <= 0) &&
               shep_type_idx < 4) {
          shep_type_idx++;
        }
        assert(shep_depth > 0);
      }
    }
    if (shep_type_idx == -1) {
      /* first, look for an exact match in width and find the proper depth */
      unsigned int maxdepth = hwloc_topology_get_depth(topology);
      unsigned int realdepth;
      unsigned int fl_depth =
        (unsigned int)-1; // first depth with #objs larger than nbsheps
      for (realdepth = 0; realdepth < maxdepth && shep_depth == -1;
           ++realdepth) {
        unsigned int num = num_usable_by_depth(realdepth);
        if (num == *nbshepherds) {
          shep_depth = realdepth;
        } else if ((num > *nbshepherds) && (fl_depth == (unsigned int)-1)) {
          fl_depth = realdepth;
        }
      }
      /* second, if we failed, try an approximate match... */
      /* should we use the last _smaller_, or the first _larger_ ? */
      /* first option means overlapping but we can use all the cores */
      /* second option means no overlapping, but cores will go unused */
      if ((shep_depth == -1) && (fl_depth != (unsigned int)-1)) {
        /* first larger then */
        shep_depth = fl_depth;
      }
      /* third, if we use such a large value that *nothing* is larger, fall back
       * to PU */
      if (shep_depth == -1) {
        shep_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
      }
    }
    {
      qthread_worker_id_t tmp = guess_num_workers_per_shep(*nbshepherds);
      if (*nbworkers == 0) { *nbworkers = tmp; }
    }
  }
}

void INTERNAL qt_affinity_deinit(void) {}

#ifdef USE_HWLOC_MEM_AFFINITY
void INTERNAL qt_affinity_mem_tonode(void *addr, size_t bytes, int node) {
  hwloc_nodeset_t nodeset = hwloc_bitmap_alloc();

  hwloc_bitmap_set(nodeset, node);
  hwloc_set_area_membind(topology,
                         addr,
                         bytes,
                         nodeset,
                         HWLOC_MEMBIND_BIND,
                         HWLOC_MEMBIND_NOCPUBIND);
  hwloc_bitmap_free(nodeset);
}

void INTERNAL *qt_affinity_alloc(size_t bytes) {
  return hwloc_alloc(topology, bytes);
}

void INTERNAL *qt_affinity_alloc_onnode(size_t bytes, int node) {
  void *ret;
  hwloc_nodeset_t nodeset;

  nodeset = hwloc_bitmap_alloc();
  hwloc_bitmap_set(nodeset, node);
  ret = hwloc_alloc_membind(
    topology, bytes, nodeset, HWLOC_MEMBIND_BIND, HWLOC_MEMBIND_NOCPUBIND);
  hwloc_bitmap_free(nodeset);
  return ret;
}

void INTERNAL qt_affinity_free(void *ptr, size_t bytes) {
  hwloc_free(topology, ptr, bytes);
}

#endif /* ifdef USE_HWLOC_MEM_AFFINITY */

qthread_shepherd_id_t INTERNAL guess_num_shepherds(void) {
  qthread_shepherd_id_t ret = 1;

  ret = num_usable_by_depth(shep_depth);
  if (ret == 0) { ret = 1; }
  return ret;
}

qthread_worker_id_t INTERNAL
guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds) {
  qthread_worker_id_t ret = 0;
  size_t total = 0;

  hwloc_const_cpuset_t allowed_cpuset =
    hwloc_topology_get_allowed_cpuset(topology);

  if (wkr_index != -1) {
    assert(shep_depth >= 0);

    hwloc_obj_t obj = hwloc_get_obj_inside_cpuset_by_depth(
      topology, allowed_cpuset, shep_depth, 0);
    int workerobjs_per_shep;

    if (hwloc_compare_types(wkr_type, HWLOC_OBJ_CACHE_UNIFIED) == 0) {
      wkr_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);
      for (int wkridx = 5; wkridx <= wkr_index; wkridx++) {
        hwloc_obj_t tmp;
        do {
          wkr_depth--;
          tmp = hwloc_get_obj_inside_cpuset_by_depth(
            topology, obj->cpuset, wkr_depth, 0);
        } while (hwloc_compare_types(tmp->type, HWLOC_OBJ_CACHE_UNIFIED) > 0);
        if (hwloc_compare_types(tmp->type, HWLOC_OBJ_CACHE_UNIFIED) != 0) {
          fprintf(
            stderr,
            "QTHREADS: worker unit (%s) does not exist on this machine.\n",
            typenames[wkr_index]);
          assert(hwloc_compare_types(tmp->type, HWLOC_OBJ_CACHE_UNIFIED) == 0);
          goto guess_my_weight;
        }
      }
      assert(wkr_depth >= shep_depth);
      workerobjs_per_shep = hwloc_get_nbobjs_inside_cpuset_by_depth(
        topology, obj->cpuset, wkr_depth);
      switch (workerobjs_per_shep) {
        case 0:
          fprintf(
            stderr,
            "QTHREADS: worker unit (%s) does not exist on this machine.\n",
            typenames[wkr_index]);
          assert(workerobjs_per_shep > 0);
          goto guess_my_weight;
        case -1:
          fprintf(stderr,
                  "QTHREADS: worker unit (%s) is not unique on this machine.\n",
                  typenames[wkr_index]);
          assert(workerobjs_per_shep > 0);
          goto guess_my_weight;
      }
    } else {
      workerobjs_per_shep =
        hwloc_get_nbobjs_inside_cpuset_by_type(topology, obj->cpuset, wkr_type);
      switch (workerobjs_per_shep) {
        case 0:
          fprintf(
            stderr,
            "QTHREADS: worker unit (%s) does not exist on this machine.\n",
            typenames[wkr_index]);
          assert(workerobjs_per_shep > 0);
          goto guess_my_weight;
        case -1:
          fprintf(stderr,
                  "QTHREADS: worker unit (%s) is not unique on this machine.\n",
                  typenames[wkr_index]);
          assert(workerobjs_per_shep > 0);
          goto guess_my_weight;
      }
      wkr_depth = hwloc_get_type_depth(topology, wkr_type);
      assert(wkr_depth >= shep_depth);
    }
    if (wkr_depth < shep_depth) {
      fprintf(
        stderr,
        "worker unit (%s) is not lower in hierarchy than shepherd (%s)\n",
        hwloc_obj_type_string(wkr_type),
        hwloc_obj_type_string(hwloc_get_depth_type(topology, shep_depth)));
      wkr_type = HWLOC_OBJ_PU;
      wkr_index = -1;
      assert(wkr_depth >= shep_depth);
      goto guess_my_weight;
    }
    ret = workerobjs_per_shep;
  } else {
  guess_my_weight:
    wkr_depth = hwloc_get_type_depth(topology, HWLOC_OBJ_PU);

    size_t const max_idx = hwloc_get_nbobjs_inside_cpuset_by_depth(
      topology, allowed_cpuset, shep_depth);

    for (size_t idx = 0; idx < nshepherds && idx < max_idx; ++idx) {
      hwloc_obj_t obj = hwloc_get_obj_inside_cpuset_by_depth(
        topology, allowed_cpuset, shep_depth, idx);
      unsigned int weight = WEIGHT(obj->cpuset);
      total += weight;
      if ((idx == 0) || (ret < weight)) { ret = weight; }
    }
    if (ret * nshepherds > total) { ret = total / nshepherds; }
  }
  if (ret == 0) { ret = 1; }
  return ret;
}

void INTERNAL qt_affinity_set(qthread_worker_t *me,
                              unsigned int nworkerspershep) {
  ASSERT_ONLY(hwloc_topology_check(topology));

  hwloc_const_cpuset_t const allowed_cpuset =
    hwloc_topology_get_allowed_cpuset(topology); // where am I allowed to run?
  qthread_shepherd_t *const myshep = me->shepherd;
  unsigned int maxshepobjs = hwloc_get_nbobjs_inside_cpuset_by_depth(
    topology, allowed_cpuset, shep_depth);
  hwloc_obj_t obj = hwloc_get_obj_inside_cpuset_by_depth(
    topology, allowed_cpuset, shep_depth, myshep->node);
  assert(wkr_depth >= 0);
  hwloc_obj_t worker0 =
    hwloc_get_obj_inside_cpuset_by_depth(topology, obj->cpuset, wkr_depth, 0);
  unsigned int workerobjs_per_shep =
    hwloc_get_nbobjs_inside_cpuset_by_depth(topology, obj->cpuset, wkr_depth);

  assert(workerobjs_per_shep > 0);
  assert(worker0);

  int shep_pus =
    hwloc_get_nbobjs_inside_cpuset_by_type(topology, obj->cpuset, HWLOC_OBJ_PU);
  assert(shep_pus > 0);
  unsigned int worker_pus = hwloc_get_nbobjs_inside_cpuset_by_type(
    topology, worker0->cpuset, HWLOC_OBJ_PU);
  unsigned int wraparounds =
    me->packed_worker_id / (maxshepobjs * nworkerspershep);
  unsigned int worker_wraparounds = me->worker_id / workerobjs_per_shep;
  hwloc_obj_t sub_obj = hwloc_get_obj_inside_cpuset_by_type(
    topology,
    obj->cpuset,
    HWLOC_OBJ_PU,
    ((me->worker_id * worker_pus) + (wraparounds * nworkerspershep) +
     worker_wraparounds) %
      shep_pus);

  if (hwloc_set_cpubind(topology, sub_obj->cpuset, HWLOC_CPUBIND_THREAD)) {
    char *str;
    int i = errno;
#ifdef __APPLE__
    if (i == ENOSYS) { return; }
#endif
    ASPRINTF(&str, sub_obj->cpuset);
    fprintf(stderr,
            "Couldn't bind to cpuset %s because %s (%i)\n",
            str,
            strerror(i),
            i);
    FREE(str, strlen(str));
  }
}

int INTERNAL qt_affinity_gendists(qthread_shepherd_t *sheps,
                                  qthread_shepherd_id_t nshepherds) {
  hwloc_const_cpuset_t allowed_cpuset =
    hwloc_topology_get_allowed_cpuset(topology); // where am I allowed to run?
  size_t num_extant_objs;

  num_extant_objs = hwloc_get_nbobjs_inside_cpuset_by_depth(
    topology, allowed_cpuset, shep_depth);
  assert(num_extant_objs != 0);

  for (size_t i = 0; i < nshepherds; ++i) {
    sheps[i].node = i % num_extant_objs;
    sheps[i].sorted_sheplist =
      qt_calloc(nshepherds - 1, sizeof(qthread_shepherd_id_t));
    sheps[i].shep_dists = qt_calloc(nshepherds, sizeof(unsigned int));
  }
  for (size_t i = 0; i < nshepherds; ++i) {
    for (size_t j = 0, k = 0; j < nshepherds; ++j) {
      if (j != i) {
        sheps[i].shep_dists[j] = 10;
        sheps[i].sorted_sheplist[k++] = j;
      }
    }
    if (nshepherds > 1) {
      sort_sheps(sheps[i].shep_dists, sheps[i].sorted_sheplist, nshepherds);
    }
  }
  /* there does not seem to be a way to extract distances... <sigh> */
  return QTHREAD_SUCCESS;
}

/* vim:set expandtab: */
