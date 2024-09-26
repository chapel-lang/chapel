/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Compute node topology support: hwloc-based implementation
//
#include "chplrt.h"

#include "chpl-align.h"
#include "chpl-env.h"
#include "chpl-env-gen.h"
#include "chplcgfns.h"
#include "chplsys.h"
#include "chpl-topo.h"
#include "chpl-comm.h"
#include "chpltypes.h"
#include "error.h"
#include "chpl-mem-sys.h"
#include "chplexit.h"

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hwloc.h"

#ifdef DEBUG
// note: format arg 'f' must be a string constant
#ifdef DEBUG_NODEID
#define _DBG_P(f, ...)                                                  \
        do {                                                            \
          printf("%d:%s:%d: " f "\n", chpl_nodeID, __FILE__, __LINE__,  \
                                      ## __VA_ARGS__);                  \
        } while (0)
#else
#define _DBG_P(f, ...)                                                  \
        do {                                                            \
          printf("%s:%d: " f "\n", __FILE__, __LINE__, ## __VA_ARGS__); \
        } while (0)
#endif
static chpl_bool debug = true;
#else
#define _DBG_P(f, ...)
static chpl_bool debug = false;
#endif

static chpl_bool haveTopology = false;

static hwloc_topology_t topology;

static const struct hwloc_topology_support* topoSupport;
static chpl_bool do_set_area_membind;

static int topoDepth;

static int numNumaDomains;

// A note on core and PU numbering. As per the hwloc documentation, a cpuset
// contains OS indices of PUs. In order to use a cpuset to represent a
// collection of cores and not break this invariant, we represent a core in a
// cpuset with the smallest OS index of its PUs. For example, the physAccSet
// contains the OS indices of the smallest PU for each accessible core.

// Accessible cores and PUs.
static hwloc_cpuset_t physAccSet = NULL;
static hwloc_cpuset_t physReservedSet = NULL;
static hwloc_cpuset_t logAccSet = NULL;
static hwloc_cpuset_t logAllSet = NULL;

// This is used for runtime testing and masks the accessible PUs.
static hwloc_cpuset_t logAccMask = NULL;

static void cpuInfoInit(void);
static void partitionResources(void);
static const char *objTypeString(hwloc_obj_type_t t);

// Accessible NUMA nodes

static hwloc_nodeset_t numaSet = NULL;

static hwloc_obj_t myRoot = NULL;

// Logical CPU sets for all locales on this node. Entries are NULL if
// we don't have that info.
static hwloc_cpuset_t *logAccSets = NULL;


static hwloc_obj_t getNumaObj(c_sublocid_t);
static void alignAddrSize(void*, size_t, chpl_bool,
                          size_t*, unsigned char**, size_t*);
static void chpl_topo_setMemLocalityByPages(unsigned char*, size_t,
                                            hwloc_obj_t);

// CPU reservation must happen before CPU information is returned to other
// layers.
static const char *cantReserveCPU = NULL;

static chpl_bool oversubscribed = false;

//
// Error reporting.
//
// CHK_ERR*() must evaluate 'expr' precisely once!
//
static void chk_err_fn(const char*, int, const char*);
static void chk_err_errno_fn(const char*, int, const char*);

#define CHK_ERR(expr) \
  do { if (!(expr)) chk_err_fn(__FILE__, __LINE__, #expr); } while (0)

#define CHK_ERR_ERRNO(expr) \
  do { if (!(expr)) chk_err_errno_fn(__FILE__, __LINE__, #expr); } while (0)

#define REPORT_ERR_ERRNO(expr) \
  chk_err_errno_fn(__FILE__, __LINE__, #expr)

// Partially initialize the topology layer for use during comm initialization.
// The remainder of the initialization is done in chpl_topo_post_comm_init
// after the comm layer has been initialized and we know how many locales
// are running on this node.
//

void chpl_topo_pre_comm_init(char *accessiblePUsMask) {
  //
  // accessibleMask is a string in hwloc "bitmap list" format that
  // specifies which processing units should be considered accessible
  // to this locale. It is intended for testing purposes only and
  // should be NULL in production code.

  //
  // We only load hwloc topology information in configurations where
  // the locale model is other than "flat" or the tasking is based on
  // Qthreads (which will use the topology we load).  We don't use
  // it otherwise (so far) because loading it is somewhat expensive.
  //
  if (strcmp(CHPL_LOCALE_MODEL, "flat") != 0
      || strcmp(CHPL_TASKS, "qthreads") == 0) {
    haveTopology = true;
  } else {
    haveTopology = false;
    return;
  }

  //
  // Allocate and initialize topology object.
  //
  CHK_ERR_ERRNO(hwloc_topology_init(&topology) == 0);

  int flags = HWLOC_TOPOLOGY_FLAG_INCLUDE_DISALLOWED;
  flags |= HWLOC_TOPOLOGY_FLAG_IMPORT_SUPPORT; // for testing
  CHK_ERR_ERRNO(hwloc_topology_set_flags(topology, flags) == 0);

  CHK_ERR_ERRNO(hwloc_topology_set_all_types_filter(topology,
                                            HWLOC_TYPE_FILTER_KEEP_ALL) == 0);

  //
  // Perform the topology detection.
  //
  CHK_ERR_ERRNO(hwloc_topology_load(topology) == 0);

  //
  // What is supported?
  //
  topoSupport = hwloc_topology_get_support(topology);

#ifdef DEBUG
  struct hwloc_topology_cpubind_support *cpubind = topoSupport->cpubind;
  _DBG_P("set_thisproc_cpubind: %d", cpubind->set_thisproc_cpubind);
  _DBG_P("get_thisproc_cpubind: %d", cpubind->get_thisproc_cpubind);
  _DBG_P("set_proc_cpubind: %d", cpubind->set_proc_cpubind);
  _DBG_P("get_proc_cpubind: %d", cpubind->get_proc_cpubind);
  _DBG_P("set_thisthread_cpubind: %d", cpubind->set_thisthread_cpubind);
  _DBG_P("get_thisthread_cpubind: %d", cpubind->get_thisthread_cpubind);
  _DBG_P("set_thread_cpubind: %d", cpubind->set_thread_cpubind);
  _DBG_P("get_thread_cpubind: %d", cpubind->get_thread_cpubind);
  _DBG_P("get_thisproc_last_cpu_location: %d",
         cpubind->get_thisproc_last_cpu_location);
  _DBG_P("get_thisthread_last_cpu_location: %d",
         cpubind->get_thisthread_last_cpu_location);
#endif

  //
  // TODO: update comment
  // For now, don't support setting memory locality when comm=ugni or
  // comm=gasnet, seg!=everything.  Those are the two configurations in
  // which we use hugepages and/or memory registered with the comm
  // interface, both of which may be a problem for the set-membind call.
  // We will have other ways to achieve locality for these configs in
  // the future.
  //
  do_set_area_membind = true;
  if ((strcmp(CHPL_COMM, "gasnet") == 0
       && strcmp(CHPL_GASNET_SEGMENT, "everything") != 0)) {
      do_set_area_membind = false;
  }

  //
  // We need depth information.
  //
  topoDepth = hwloc_topology_get_depth(topology);

  if (accessiblePUsMask != NULL) {
    CHK_ERR_ERRNO((logAccMask = hwloc_bitmap_alloc()) != NULL);
    CHK_ERR(hwloc_bitmap_list_sscanf(logAccMask, accessiblePUsMask) == 0);
    if (debug) {
      char buf[1024];
      hwloc_bitmap_list_snprintf(buf, sizeof(buf), logAccMask);
      _DBG_P("logAccMask: %s", buf);
    }
  }
  cpuInfoInit();
}

//
// Finish initializing the topology layer after the comm layer has been
// initialized.
//
void chpl_topo_post_comm_init(void) {
  partitionResources();
}


void chpl_topo_exit(void) {
  if (!haveTopology) {
    return;
  }

  if (physAccSet != NULL) {
    hwloc_bitmap_free(physAccSet);
    physAccSet = NULL;
  }
  if (physReservedSet != NULL) {
    hwloc_bitmap_free(physReservedSet);
    physReservedSet = NULL;
  }
  if (logAccSet != NULL) {
    hwloc_bitmap_free(logAccSet);
    logAccSet = NULL;
  }
  if (logAllSet != NULL) {
    hwloc_bitmap_free(logAllSet);
    logAllSet = NULL;
  }
  if (numaSet != NULL) {
    hwloc_bitmap_free(numaSet);
    numaSet = NULL;
  }
  if (logAccMask != NULL) {
    hwloc_bitmap_free(logAccMask);
    logAccMask = NULL;
  }

  if (logAccSets != NULL) {
    for (int i = 0; i < chpl_get_num_locales_on_node(); i++) {
      if (logAccSets[i] != NULL) {
        hwloc_bitmap_free(logAccSets[i]);
      }
    }
    sys_free(logAccSets);
    logAccSets = NULL;
  }
  hwloc_topology_destroy(topology);
}


void* chpl_topo_getHwlocTopology(void) {
  return (haveTopology) ? topology : NULL;
}

//
// How many CPUs (cores or PUs) are there?
//
static int numCPUsPhysAcc = -1;
static int numCPUsPhysAll = -1;
static int numCPUsLogAcc  = -1;
static int numCPUsLogAll  = -1;

int chpl_topo_getNumCPUsPhysical(chpl_bool accessible_only) {
  cantReserveCPU = "chpl_topo_getNumCPUsPhysical called";
  int cpus = (accessible_only) ? numCPUsPhysAcc : numCPUsPhysAll;
  if (cpus == -1) {
    chpl_error("number of cpus is uninitialized", 0, 0);
  }
  return cpus;
}


int chpl_topo_getNumCPUsLogical(chpl_bool accessible_only) {
  cantReserveCPU = "chpl_topo_getNumCPUsLogical called";
  int cpus = (accessible_only) ? numCPUsLogAcc : numCPUsLogAll;
  if (cpus == -1) {
    chpl_error("number of cpus is uninitialized", 0, 0);
  }
  return cpus;
}


#define NEXT_OBJ(cpuset, type, obj)                                \
  hwloc_get_next_obj_inside_cpuset_by_type(topology, (cpuset),     \
                                           (type), (obj))

// Filter any PUs from the cpuset whose entry in ignoreKinds is true
static void filterPUsByKind(int numKinds, chpl_bool *ignoreKinds,
                         hwloc_cpuset_t cpuset) {

  // filtering only makes sense if there is more than one kind of PU
  if (numKinds > 1) {
    for (hwloc_obj_t pu = NEXT_OBJ(cpuset, HWLOC_OBJ_PU, NULL);
         pu != NULL;
         pu = NEXT_OBJ(cpuset, HWLOC_OBJ_PU, pu)) {
      if (debug) {
        char buf[1024];
        hwloc_bitmap_list_snprintf(buf, sizeof(buf), pu->cpuset);
      }
      int kind = hwloc_cpukinds_get_by_cpuset(topology, pu->cpuset, 0);
      CHK_ERR_ERRNO((kind >= 0) && (kind < numKinds));
      if (ignoreKinds[kind]) {
        hwloc_bitmap_andnot(cpuset, cpuset, pu->cpuset);
      }
    }
  }
}

//
// Initializes information about all CPUs (cores and PUs) from
// the topology. The accessible CPUs are initialized as a side-effect,
// but they aren't partitioned until partitionResources is called.
//

static void cpuInfoInit(void) {
  _DBG_P("cpuInfoInit");

  CHK_ERR_ERRNO((physAccSet = hwloc_bitmap_alloc()) != NULL);
  CHK_ERR_ERRNO((physReservedSet = hwloc_bitmap_alloc()) != NULL);
  CHK_ERR_ERRNO((numaSet = hwloc_bitmap_alloc()) != NULL);


  // Determine which kind(s) of PUs we are supposed to use.
  // hwloc returns kinds sorted by efficiency, least efficient
  // (more performant) last. Currently, we put them into two
  // groups, most performant ("performance") and lump all the
  // rest into "efficiency".

  int numKinds;
  CHK_ERR_ERRNO((numKinds = hwloc_cpukinds_get_nr(topology, 0)) >= 0);
  _DBG_P("There are %d kinds of PUs", numKinds);
  chpl_bool *ignoreKinds = NULL;
  if (numKinds > 1) {
    ignoreKinds = sys_calloc(numKinds, sizeof(*ignoreKinds));
    CHK_ERR(ignoreKinds);
    // there are multiple kinds of PUs
    const char *kindStr = chpl_env_rt_get("USE_PU_KIND", "performance");
    if (!strcasecmp(kindStr, "performance")) {
      // use only performance PUs. This is the default.
      _DBG_P("using only performance PUs");
      for (int i = 0; i < numKinds - 1; i++) {
        ignoreKinds[i] = true;
      }
    } else if (!strcasecmp(kindStr, "efficiency")) {
      // use only efficiency PUs
      _DBG_P("using only efficiency PUs");
      ignoreKinds[numKinds-1] = true;
    } else if (!strcasecmp(kindStr, "all")) {
      // do nothing, we'll use all kinds of PUs
      _DBG_P("using all PUs");
    } else {
      char msg[200];
      snprintf(msg, sizeof(msg),
               "\"%s\" is not a valid value for CHPL_RT_USE_PU_KIND.\n"
               "Must be one of \"performance\", \"efficiency\", or \"all\".",
               kindStr);
        chpl_error(msg, 0, 0);
    }
  }

  // accessible PUs

  logAccSet = hwloc_bitmap_dup(hwloc_topology_get_allowed_cpuset(topology));
  if (debug) {
    char buf[1024];
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), logAccSet);
    _DBG_P("logAccSet before masking: %s", buf);
  }
  if (logAccMask) {
    // Modify accessible PUs for testing purposes.
    hwloc_bitmap_and(logAccSet, logAccSet, logAccMask);
  }
  if (debug) {
    char buf[1024];
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), logAccSet);
    _DBG_P("logAccSet after masking: %s", buf);
  }

  _DBG_P("filtering logAccSet");
  filterPUsByKind(numKinds, ignoreKinds, logAccSet);
  numCPUsLogAcc = hwloc_bitmap_weight(logAccSet);
  _DBG_P("numCPUsLogAcc = %d", numCPUsLogAcc);
  if (debug) {
    char buf[1024];
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), logAccSet);
    _DBG_P("logAccSet after filtering: %s", buf);
  }

  //
  // all cores
  //

  logAllSet = hwloc_bitmap_dup(hwloc_topology_get_complete_cpuset(topology));
  numCPUsLogAll = hwloc_bitmap_weight(logAllSet);
  CHK_ERR(numCPUsLogAll > 0);
  _DBG_P("numCPUsLogAll = %d", numCPUsLogAll);

  // accessible cores

  int maxPusPerAccCore = 0;

  for (hwloc_obj_t core = NEXT_OBJ(logAllSet, HWLOC_OBJ_CORE, NULL);
       core != NULL;
       core = NEXT_OBJ(logAllSet, HWLOC_OBJ_CORE, core)) {
    // check whether this core is included in our filtered set
    if (!hwloc_bitmap_intersects(logAccSet, core->cpuset)) continue;

    // filter the core's PUs
    hwloc_cpuset_t cpuset = NULL;
    CHK_ERR_ERRNO((cpuset = hwloc_bitmap_dup(core->cpuset)) != NULL);
    // filter the core's PUs in case they are hybrid
    filterPUsByKind(numKinds, ignoreKinds, cpuset);

    // determine the max # PUs in a core
    int numPus = hwloc_bitmap_weight(cpuset);
    if (numPus > maxPusPerAccCore) {
      maxPusPerAccCore = numPus;
    }
    // use the smallest PU index to represent the core in physAccSet
    int smallest = hwloc_bitmap_first(cpuset);
    CHK_ERR(smallest != -1);
    hwloc_bitmap_set(physAccSet, smallest);
    hwloc_bitmap_free(cpuset);
  }

  if (ignoreKinds) {
    sys_free(ignoreKinds);
    ignoreKinds = NULL;
  }

  numCPUsPhysAcc = hwloc_bitmap_weight(physAccSet);
  if (numCPUsPhysAcc == 0) {
    chpl_error("No useable cores.", 0, 0);
  }

  if (numCPUsLogAll == numCPUsLogAcc) {
    // All PUs and therefore all cores are accessible
    numCPUsPhysAll = numCPUsPhysAcc;
  } else {
    // Some cores are inaccessible. We estimate their number by
    // assuming they all have the maximum number of PUs.
    numCPUsPhysAll = numCPUsLogAll / maxPusPerAccCore;
  }
  CHK_ERR(numCPUsPhysAll > 0);
  _DBG_P("numCPUsPhysAll = %d", numCPUsPhysAll);
  _DBG_P("numCPUsPhysAcc = %d", numCPUsPhysAcc);

  if (debug) {
    char buf[1024];
    _DBG_P("numCPUsLogAll: %d", numCPUsLogAll);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), logAccSet);
    _DBG_P("numCPUsLogAcc: %d logAccSet: %s", numCPUsLogAcc,
           buf);

    _DBG_P("numCPUsPhysAll: %d", numCPUsPhysAll);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), physAccSet);
    _DBG_P("numCPUsPhysAcc: %d physAccSet: %s", numCPUsPhysAcc,
           buf);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), numaSet);
    _DBG_P("numaSet: %s", buf);

    hwloc_const_cpuset_t set;

    set = hwloc_topology_get_allowed_cpuset(topology);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), set);
    _DBG_P("allowed cpuset: %s", buf);

    set = hwloc_topology_get_complete_cpuset(topology);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), set);
    _DBG_P("complete cpuset: %s", buf);

    set = hwloc_topology_get_topology_cpuset(topology);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), set);
    _DBG_P("topology cpuset: %s", buf);

  }
}

// Convert hwloc_obj_type_t into better names than hwloc_obj_type_string.
static const char *objTypeString(hwloc_obj_type_t t) {
  const char *str = NULL;
  switch(t) {
    case HWLOC_OBJ_MACHINE: str = "machine"; break;
    case HWLOC_OBJ_PACKAGE: str = "socket"; break;
    case HWLOC_OBJ_NUMANODE: str = "NUMA domain"; break;
    case HWLOC_OBJ_CORE: str = "core"; break;
    case HWLOC_OBJ_L1CACHE: str = "L1 cache"; break;
    case HWLOC_OBJ_L2CACHE: str = "L2 cache"; break;
    case HWLOC_OBJ_L3CACHE: str = "L3 cache"; break;
    case HWLOC_OBJ_L4CACHE: str = "L4 cache"; break;
    case HWLOC_OBJ_L5CACHE: str = "L5 cache"; break;
    default: str = "unknown"; break;
  }
  return str;
}

//
// Partitions resources when running with co-locales.
//

static void partitionResources(void) {
  hwloc_obj_t root = hwloc_get_root_obj(topology);
  hwloc_obj_type_t myRootType = HWLOC_OBJ_TYPE_MAX;
  int numLocalesOnNode = chpl_get_num_locales_on_node();
  int numColocales = chpl_env_rt_get_int("LOCALES_PER_NODE", 0);
  int unusedCores = 0;

  const char *t = chpl_env_rt_get("COLOCALE_OBJ_TYPE", NULL);
  if (t != NULL) {
    // The type of root object was specified on the command-line.
    if (!strcmp(t , "socket")) {
      myRootType = HWLOC_OBJ_PACKAGE;
    } else if (!strcmp(t , "numa")) {
      myRootType = HWLOC_OBJ_NUMANODE;
    } else if (!strcmp(t , "core")) {
      myRootType = HWLOC_OBJ_CORE;
    } else if (!strcmp(t , "cache")) {
      hwloc_obj_type_t cacheTypes[] = {HWLOC_OBJ_L5CACHE, HWLOC_OBJ_L4CACHE,
                                       HWLOC_OBJ_L3CACHE, HWLOC_OBJ_L2CACHE,
                                       HWLOC_OBJ_L1CACHE, HWLOC_OBJ_TYPE_MAX};
      for (int i = 0; cacheTypes[i] != HWLOC_OBJ_TYPE_MAX; i++) {
        if (hwloc_get_nbobjs_by_type(topology, cacheTypes[i]) > 0) {
          myRootType = cacheTypes[i];
          break;
        }
      }
    } else {
      char msg[200];
      snprintf(msg, sizeof(msg),
               "CHPL_RT_COLOCALE_OBJ_TYPE is not a valid type: \"%s\"", t);
      chpl_error(msg, 0, 0);
    }
  }


  int rank = chpl_get_local_rank();
  _DBG_P("numLocalesOnNode: %d", numLocalesOnNode);
  _DBG_P("numColocales: %d", numColocales);
  _DBG_P("rank: %d", rank);
  if (numLocalesOnNode > 1) {
    oversubscribed = true;
  }
  logAccSets = sys_calloc(numLocalesOnNode, sizeof(hwloc_cpuset_t));
  if (numColocales > 0) {
    // We get our own socket/NUMA/cache/core object if we have exclusive
    // access to the node, we know our local rank, and the number of locales
    // on the node is less than or equal to the number of objects. It is an
    // error if the number of locales on the node is greater than the number
    // of objects and CHPL_RT_LOCALES_PER_NODE is set, otherwise we are
    // oversubscribed.

    // TODO: The oversubscription determination is incorrect. A node is only
    // oversubscribed if locales are sharing cores. Need to figure out how
    // to determine this accurately.

    if (rank != -1) {
      if (myRootType == HWLOC_OBJ_TYPE_MAX) {
        // Chose a root object if the number of them matches the number of
        // locales.
        hwloc_obj_type_t rootTypes[] = {HWLOC_OBJ_MACHINE, HWLOC_OBJ_PACKAGE,
                                        HWLOC_OBJ_NUMANODE,
                                        HWLOC_OBJ_L5CACHE, HWLOC_OBJ_L4CACHE,
                                        HWLOC_OBJ_L3CACHE, HWLOC_OBJ_L2CACHE,
                                        HWLOC_OBJ_L1CACHE, HWLOC_OBJ_CORE,
                                        HWLOC_OBJ_TYPE_MAX};
        for (int i = 0; rootTypes[i] != HWLOC_OBJ_TYPE_MAX; i++) {
          int numObjs = hwloc_get_nbobjs_by_type(topology, rootTypes[i]);
          if (numObjs == numColocales) {
            myRootType = rootTypes[i];
            break;
          }
        }
      }
      if (myRootType != HWLOC_OBJ_TYPE_MAX) {
        _DBG_P("myRootType: %s", objTypeString(myRootType));
        int numCores = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_CORE);
        int numObjs = hwloc_get_nbobjs_by_type(topology, myRootType);
        if (numObjs < numLocalesOnNode) {
          char msg[200];
          snprintf(msg, sizeof(msg), "Node only has %d %s(s)", numObjs,
                   objTypeString(myRootType));
          chpl_error(msg, 0, 0);
        }
        if (numObjs > numLocalesOnNode) {
          int coresPerLocale = numCores / numObjs;
          unusedCores = (numObjs - numLocalesOnNode) * coresPerLocale;
        }

        // Use the object whose logical index corresponds to our local rank.
        CHK_ERR(myRoot = hwloc_get_obj_inside_cpuset_by_type(topology,
                                  root->cpuset, myRootType, rank));

        _DBG_P("confining ourself to %s %d", objTypeString(myRootType), rank);

        // Compute the accessible PUs for all locales on this node based on
        // the object each occupies. This is used to determine which NIC each
        // locale should use.

        for (int i = 0; i < numLocalesOnNode; i++) {
          hwloc_obj_t obj;
          CHK_ERR(obj = hwloc_get_obj_inside_cpuset_by_type(topology,
                                  root->cpuset, myRootType, i));
          hwloc_cpuset_t s = hwloc_bitmap_dup(obj->cpuset);
          hwloc_bitmap_and(s, s, logAccSet);
          logAccSets[i] = s;
        }
      } else {
        // Cores not tied to a root object
        int coresPerLocale = numCPUsPhysAcc / numLocalesOnNode;
        if (coresPerLocale < 1) {
          char msg[200];
          snprintf(msg, sizeof(msg), "Cannot run %d co-locales on %d cores.",
                   numLocalesOnNode, numCPUsPhysAcc);
          chpl_error(msg, 0, 0);
        }
        unusedCores = numCPUsPhysAcc % numLocalesOnNode;
        int count = 0;
        int locale = -1;
        int id;
        hwloc_bitmap_foreach_begin(id, physAccSet) {
          if (count == 0) {
            locale++;
            if (locale == numLocalesOnNode) {
              break;
            }
            CHK_ERR_ERRNO(logAccSets[locale] = hwloc_bitmap_alloc());
          }
          hwloc_obj_t pu;
          hwloc_obj_t core;
          CHK_ERR(pu = hwloc_get_pu_obj_by_os_index(topology, id));
          CHK_ERR(core = hwloc_get_ancestor_obj_by_type(topology,
                                                    HWLOC_OBJ_CORE, pu));
          hwloc_bitmap_or(logAccSets[locale], logAccSets[locale],
                          core->cpuset);
          count = (count + 1) % coresPerLocale;
        } hwloc_bitmap_foreach_end();
      }
      if (unusedCores != 0) {
        char msg[200];
        snprintf(msg, sizeof(msg), "%d cores are unused", unusedCores);
        chpl_warning(msg, 0, 0);
      }

      // Limit our accessible cores and PUs to those in our cpuset.

      hwloc_bitmap_and(logAccSet, logAccSet, logAccSets[rank]);
      numCPUsLogAcc = hwloc_bitmap_weight(logAccSet);
      CHK_ERR(numCPUsLogAcc > 0);

      hwloc_bitmap_and(physAccSet, physAccSet, logAccSets[rank]);
      numCPUsPhysAcc = hwloc_bitmap_weight(physAccSet);
      CHK_ERR(numCPUsPhysAcc > 0);

      if (debug) {
        char buf[1024];
        hwloc_bitmap_list_snprintf(buf, sizeof(buf), logAccSet);
        _DBG_P("numCPUsLogAcc: %d logAccSet: %s", numCPUsLogAcc, buf);
        hwloc_bitmap_list_snprintf(buf, sizeof(buf), physAccSet);
        _DBG_P("numCPUsPhysAcc: %d physAccSet: %s", numCPUsPhysAcc, buf);
      }
      oversubscribed = false;
    }
  } else {
    // We don't know which PUs other locales on the same node are using,
    // so just set our own.
    logAccSets[0] = hwloc_bitmap_dup(logAccSet);
  }
  if (debug) {
    for (int i = 0; i < numLocalesOnNode; i++) {
      char buf[1024];
      if (logAccSets[i] != NULL) {
        hwloc_bitmap_list_snprintf(buf, sizeof(buf), logAccSets[i]);
      } else {
        strncpy(buf, "unknown", sizeof(buf));
      }
      _DBG_P("logAccSets[%d]: %s", i, buf);
    }
  }

  // CHPL_RT_OVERSUBSCRIBED overrides oversubscription determination

  oversubscribed = chpl_env_rt_get_bool("OVERSUBSCRIBED", oversubscribed);

  if ((verbosity >= 2) && (chpl_nodeID == 0)) {
    printf("oversubscribed = %s\n", oversubscribed ? "True" : "False");
  }

  // Find the NUMA nodes.

  hwloc_cpuset_to_nodeset(topology, logAccSet, numaSet);
  numNumaDomains = hwloc_bitmap_weight(numaSet);
  _DBG_P("numNumaDomains %d", numNumaDomains);
  if (debug) {
    char buf[1024];
    _DBG_P("numCPUsLogAll: %d", numCPUsLogAll);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), logAccSet);
    _DBG_P("numCPUsLogAcc: %d logAccSet: %s", numCPUsLogAcc,
           buf);

    _DBG_P("numCPUsPhysAll: %d", numCPUsPhysAll);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), physAccSet);
    _DBG_P("numCPUsPhysAcc: %d physAccSet: %s", numCPUsPhysAcc,
           buf);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), numaSet);
    _DBG_P("numaSet: %s", buf);

    hwloc_const_cpuset_t set;

    set = hwloc_topology_get_allowed_cpuset(topology);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), set);
    _DBG_P("allowed cpuset: %s", buf);

    set = hwloc_topology_get_complete_cpuset(topology);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), set);
    _DBG_P("complete cpuset: %s", buf);

    set = hwloc_topology_get_topology_cpuset(topology);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), set);
    _DBG_P("topology cpuset: %s", buf);
  }
}

#undef NEXT_OBJ

void chpl_topo_post_args_init(void) {
  char buf[1024];
  if (verbosity >= 2) {
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), physAccSet);
    printf("%d: using core(s) %s", chpl_nodeID, buf);
    if (myRoot && (myRoot->type != HWLOC_OBJ_MACHINE)) {
      printf(" in %s %d\n", objTypeString(myRoot->type),
             myRoot->logical_index);
    } else {
      putchar('\n');
    }
  }
}

//
// Fills the "cpus" array with the hwloc "cpuset" (a bitmap whose bits are
// set according to CPU physical OS indexes).
//
static
int getCPUs(hwloc_cpuset_t cpuset, int *cpus, int size) {
  int count = 0;
  int id;
  hwloc_bitmap_foreach_begin(id, cpuset) {
    if (count == size) {
      break;
    }
    cpus[count++] = id;
  } hwloc_bitmap_foreach_end();
  return count;
}


//
// Fills the "cpus" array with up to "count" physical OS indices of the
// accessible cores or PUs. If "physical" is true, then "cpus" contains
// core indices, otherwise it contains PU indices. Returns the number
// of indices in the "cpus" array.
//
int chpl_topo_getCPUs(chpl_bool physical, int *cpus, int count) {
  // Initializes CPU information.
  cantReserveCPU = "chpl_topo_getCPUs called";
  return getCPUs(physical ? physAccSet : logAccSet, cpus, count);
}


int chpl_topo_getNumNumaDomains(void) {
  return numNumaDomains;
}


void chpl_topo_setThreadLocality(c_sublocid_t subloc) {
  hwloc_cpuset_t cpuset;
  int flags;

  _DBG_P("chpl_topo_setThreadLocality(%d)", (int) subloc);

  if (!haveTopology) {
    return;
  }

  if (!topoSupport->cpubind->set_thread_cpubind)
    return;

  CHK_ERR_ERRNO((cpuset = hwloc_bitmap_alloc()) != NULL);

  hwloc_cpuset_from_nodeset(topology, cpuset,
                            getNumaObj(subloc)->nodeset);

  // Only use accessible CPUs.

  hwloc_bitmap_and(cpuset, cpuset, logAccSet);

  flags = HWLOC_CPUBIND_THREAD | HWLOC_CPUBIND_STRICT;
  CHK_ERR_ERRNO(hwloc_set_cpubind(topology, cpuset, flags) == 0);
  if (debug) {
    char buf[1024];
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), cpuset);
    _DBG_P("chpl_topo_setThreadLocality(%d): %s", (int) subloc, buf);
  }
  hwloc_bitmap_free(cpuset);
}


c_sublocid_t chpl_topo_getThreadLocality(void) {
  hwloc_cpuset_t cpuset;
  hwloc_nodeset_t nodeset;
  int flags;
  int node;

  if (!haveTopology) {
    return c_sublocid_none;
  }

  if (!topoSupport->cpubind->get_thread_cpubind) {
    return c_sublocid_none;
  }

  CHK_ERR_ERRNO((cpuset = hwloc_bitmap_alloc()) != NULL);
  CHK_ERR_ERRNO((nodeset = hwloc_bitmap_alloc()) != NULL);

  flags = HWLOC_CPUBIND_THREAD;
  CHK_ERR_ERRNO(hwloc_get_cpubind(topology, cpuset, flags) == 0);

  hwloc_cpuset_to_nodeset(topology, cpuset, nodeset);

  node = hwloc_bitmap_first(nodeset);

  hwloc_bitmap_free(nodeset);
  hwloc_bitmap_free(cpuset);

  return node;
}


void chpl_topo_setMemLocality(void* p, size_t size, chpl_bool onlyInside,
                              c_sublocid_t subloc) {
  size_t pgSize;
  unsigned char* pPgLo;
  size_t nPages;

  _DBG_P("chpl_topo_setMemLocality(%p, %#zx, onlyIn=%s, %d)",
         p, size, (onlyInside ? "T" : "F"), (int) subloc);

  if (!haveTopology) {
    return;
  }

  alignAddrSize(p, size, onlyInside, &pgSize, &pPgLo, &nPages);

  _DBG_P("    localize %p, %#zx bytes (%#zx pages)",
         pPgLo, nPages * pgSize, nPages);

  if (nPages == 0)
    return;

  chpl_topo_setMemLocalityByPages(pPgLo, nPages * pgSize, getNumaObj(subloc));
}


void chpl_topo_setMemSubchunkLocality(void* p, size_t size,
                                      chpl_bool onlyInside,
                                      size_t* subchunkSizes) {
  size_t pgSize;
  unsigned char* pPgLo;
  size_t nPages;
  int i;
  size_t pg;
  size_t pgNext;

  _DBG_P("chpl_topo_setMemSubchunkLocality(%p, %#zx, onlyIn=%s)",
         p, size, (onlyInside ? "T" : "F"));

  if (!haveTopology) {
    return;
  }

  alignAddrSize(p, size, onlyInside, &pgSize, &pPgLo, &nPages);

  _DBG_P("    localize %p, %#zx bytes (%#zx pages)",
         pPgLo, nPages * pgSize, nPages);

  if (nPages == 0)
    return;

  for (i = 0, pg = 0; i < numNumaDomains; i++, pg = pgNext) {
    if (i == numNumaDomains - 1)
      pgNext = nPages;
    else
      pgNext = 1 + (nPages * (i + 1) - 1) / numNumaDomains;
    chpl_topo_setMemLocalityByPages(pPgLo + pg * pgSize,
                                    (pgNext - pg) * pgSize, getNumaObj(i));
    if (subchunkSizes != NULL) {
      subchunkSizes[i] = (pgNext - pg) * pgSize;
    }
  }
}


void chpl_topo_touchMemFromSubloc(void* p, size_t size, chpl_bool onlyInside,
                                  c_sublocid_t subloc) {
  size_t pgSize;
  unsigned char* pPgLo;
  size_t nPages;
  hwloc_cpuset_t cpuset;
  int flags;

  _DBG_P("chpl_topo_touchMemFromSubloc(%p, %#zx, onlyIn=%s, %d)",
         p, size, (onlyInside ? "T" : "F"), (int) subloc);

  if (!haveTopology
      || !topoSupport->cpubind->get_thread_cpubind
      || !topoSupport->cpubind->set_thread_cpubind) {
    return;
  }

  alignAddrSize(p, size, onlyInside, &pgSize, &pPgLo, &nPages);

  _DBG_P("    localize %p, %#zx bytes (%#zx pages)",
         pPgLo, nPages * pgSize, nPages);

  if (nPages == 0)
    return;

  CHK_ERR_ERRNO((cpuset = hwloc_bitmap_alloc()) != NULL);

  flags = HWLOC_CPUBIND_THREAD;
  CHK_ERR_ERRNO(hwloc_set_cpubind(topology, cpuset, flags) == 0);

  chpl_topo_setThreadLocality(subloc);

  {
    size_t pg;
    for (pg = 0; pg < nPages; pg++) {
      pPgLo[pg * pgSize] = 0;
    }
  }

  flags = HWLOC_CPUBIND_THREAD | HWLOC_CPUBIND_STRICT;
  CHK_ERR_ERRNO(hwloc_set_cpubind(topology, cpuset, flags) == 0);

  hwloc_bitmap_free(cpuset);
}


static inline
hwloc_obj_t getNumaObj(c_sublocid_t subloc) {
  int id;
  int count = 0;
  hwloc_bitmap_foreach_begin(id, numaSet) {
    if (count == subloc) {
      break;
    }
    count++;
  } hwloc_bitmap_foreach_end();
  return hwloc_get_numanode_obj_by_os_index(topology, id);
}


static inline
void alignAddrSize(void* p, size_t size, chpl_bool onlyInside,
                   size_t* p_pgSize, unsigned char** p_pPgLo,
                   size_t* p_nPages) {
  unsigned char* pCh = (unsigned char*) p;
  size_t pgSize = chpl_getHeapPageSize();
  size_t pgMask = pgSize - 1;
  unsigned char* pPgLo;
  size_t nPages;

  if (onlyInside) {
    pPgLo = round_up_to_mask_ptr(pCh, pgMask);
    if (size < pPgLo - pCh)
      nPages = 0;
    else
      nPages = round_down_to_mask(size - (pPgLo - pCh), pgMask) / pgSize;
  } else {
    pPgLo = round_down_to_mask_ptr(pCh, pgMask);
    nPages = round_up_to_mask(size + (pCh - pPgLo), pgMask) / pgSize;
  }

  *p_pgSize = pgSize;
  *p_pPgLo = pPgLo;
  *p_nPages = nPages;
}


void chpl_topo_interleaveMemLocality(void* p, size_t size) {
  int flags;

  if (!haveTopology) {
    return;
  }

  if (!topoSupport->membind->set_area_membind ||
      !topoSupport->membind->interleave_membind) {
    return;
  }
  hwloc_bitmap_t set;
  set = hwloc_bitmap_dup(hwloc_get_root_obj(topology)->cpuset);

  flags = 0;
  CHK_ERR_ERRNO(hwloc_set_area_membind(topology, p, size, set, HWLOC_MEMBIND_INTERLEAVE, flags) == 0);
}


//
// p must be page aligned and the page size must evenly divide size
//
static
void chpl_topo_setMemLocalityByPages(unsigned char* p, size_t size,
                                     hwloc_obj_t numaObj) {
  int flags;

  if (!haveTopology) {
    return;
  }

  if (!topoSupport->membind->set_area_membind
      || !do_set_area_membind)
    return;

  _DBG_P("hwloc_set_area_membind(%p, %#zx, %d)", p, size,
         (int) hwloc_bitmap_first(numaObj->nodeset));

  flags = HWLOC_MEMBIND_MIGRATE | HWLOC_MEMBIND_STRICT;
  CHK_ERR_ERRNO(hwloc_set_area_membind(topology, p, size,
                                               numaObj->nodeset,
                                               HWLOC_MEMBIND_BIND, flags)
                == 0);
}


c_sublocid_t chpl_topo_getMemLocality(void* p) {
  int flags;
  hwloc_nodeset_t nodeset;
  int node;

  if (!haveTopology) {
    return c_sublocid_none;
  }

  if (!topoSupport->membind->get_area_memlocation) {
    return c_sublocid_none;
  }

  if (p == NULL) {
    return c_sublocid_none;
  }

  CHK_ERR_ERRNO((nodeset = hwloc_bitmap_alloc()) != NULL);

  flags = HWLOC_MEMBIND_BYNODESET;
  CHK_ERR_ERRNO(hwloc_get_area_memlocation(topology, p, 1, nodeset, flags)
                == 0);

  node = hwloc_bitmap_first(nodeset);
  if (!isActualSublocID(node)) {
    node = c_sublocid_none;
  }

  hwloc_bitmap_free(nodeset);

  return node;
}


//
// Reserves a physical CPU (core) and returns its hwloc OS index. The core and
// its PUs will not be returned by chpl_topo_getCPUs,
// chpl_topo_getNumCPUsPhysical, and chpl_topo_getNumCPUsLogical. Must be
// called before those functions. Will not reserve a core if CPU binding is
// not supported on this platform or if there is only one unreserved core.
//
// Returns OS index of reserved core, -1 otherwise
//
int
chpl_topo_reserveCPUPhysical(void) {
  int id = -1;
  _DBG_P("topoSupport->cpubind->set_thisthread_cpubind: %d",
         topoSupport->cpubind->set_thisthread_cpubind);
  _DBG_P("numCPUsPhysAcc: %d", numCPUsPhysAcc);
  if (!cantReserveCPU) {
    if ((topoSupport->cpubind->set_thisthread_cpubind) &&
        (numCPUsPhysAcc > 1)) {

      if (debug) {
        char buf[1024];
        _DBG_P("chpl_topo_reserveCPUPhysical before");
        hwloc_bitmap_list_snprintf(buf, sizeof(buf), physAccSet);
        _DBG_P("physAccSet: %s", buf);
        hwloc_bitmap_list_snprintf(buf, sizeof(buf), physReservedSet);
        _DBG_P("physReservedSet: %s", buf);
        hwloc_bitmap_list_snprintf(buf, sizeof(buf), logAccSet);
        _DBG_P("logAccSet: %s", buf);
      }
      // Reserve the highest-numbered core.
      id = hwloc_bitmap_last(physAccSet);
      if (id >= 0) {

        // Find the core's object in the topology so we can reserve its PUs.
        hwloc_obj_t pu, core;
        CHK_ERR_ERRNO(pu = hwloc_get_pu_obj_by_os_index(topology, id));
        CHK_ERR_ERRNO(core = hwloc_get_ancestor_obj_by_type(topology,
                                                            HWLOC_OBJ_CORE,
                                                            pu));
        // Reserve the core.
        hwloc_bitmap_andnot(physAccSet, physAccSet, pu->cpuset);
        numCPUsPhysAcc = hwloc_bitmap_weight(physAccSet);
        hwloc_bitmap_or(physReservedSet, physReservedSet, pu->cpuset);
        CHK_ERR(numCPUsPhysAcc > 0);

        // Reserve the core's PUs.
        hwloc_bitmap_andnot(logAccSet, logAccSet, core->cpuset);
        numCPUsLogAcc = hwloc_bitmap_weight(logAccSet);
        CHK_ERR(numCPUsLogAcc > 0);

        _DBG_P("reserved core %d", id);
      }
    }
  } else {
    _DBG_P("cantReserveCPU: %s", cantReserveCPU);
  }

  if (debug) {
    char buf[1024];
    _DBG_P("chpl_topo_reserveCPUPhysical %d", id);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), physAccSet);
    _DBG_P("physAccSet: %s", buf);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), physReservedSet);
    _DBG_P("physReservedSet: %s", buf);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), logAccSet);
    _DBG_P("logAccSet: %s", buf);
  }
  return id;
}


//
// Binds the current thread to the specified CPU. The CPU must
// have previously been reserved via chpl_topo_reserveCPUPhysical.
//
// Returns 0 on success, 1 otherwise
//
int chpl_topo_bindCPU(int id) {
  int status = 1;
  if (hwloc_bitmap_isset(physReservedSet, id) &&
      (topoSupport->cpubind->set_thisthread_cpubind)) {
    int flags = HWLOC_CPUBIND_THREAD | HWLOC_CPUBIND_STRICT;
    hwloc_cpuset_t cpuset;
    CHK_ERR_ERRNO((cpuset = hwloc_bitmap_alloc()) != NULL);
    hwloc_bitmap_set(cpuset, id);
    CHK_ERR_ERRNO(hwloc_set_cpubind(topology, cpuset, flags) == 0);
    hwloc_bitmap_free(cpuset);
    status = 0;
  }
  _DBG_P("chpl_topo_bindCPU id: %d status: %d", id, status);
  return status;
}

//
// Binds the current thread to the accessible logical CPUs (PUs). This
// restricts the thread to the locale's PUs (i.e., the progress thread should
// use the same PUs as the locale).
//
// Returns 0 on success, 1 otherwise
//
int chpl_topo_bindLogAccCPUs(void) {
  int status = 0; // default to success if binding not supported
  if (topoSupport->cpubind->set_thisthread_cpubind) {
    int flags = HWLOC_CPUBIND_THREAD | HWLOC_CPUBIND_STRICT;
    CHK_ERR_ERRNO(hwloc_set_cpubind(topology, logAccSet, flags) == 0);
    status = 0;
  }
  if (debug) {
    char buf[1024];
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), logAccSet);
    _DBG_P("chpl_topo_bindLogAccCPUs %s status: %d", buf, status);
  }
  return status;
}

chpl_bool chpl_topo_isOversubscribed(void) {
  _DBG_P("oversubscribed = %s", oversubscribed ? "True" : "False");
  return oversubscribed;
}

//
// Returns the shortest path from obj0 to obj1 in the topology. The
// hwloc_get_common_ancestor_obj function does not work on objects that do
// not have a cpuset, such as I/O objects, so first find a non-I/O ancestor
// of the specified objects. The checks for NULL are because it isn't clear
// when hwloc might return NULL for various (potentially future) topologies
// and the hwloc routines seg fault when passed NULL. This function returns
// zero if it cannot compute the distance.
//
static int
distance(hwloc_topology_t topology, hwloc_obj_t obj0, hwloc_obj_t obj1) {
  int dist = 0;
  if ((obj0 != NULL) && (obj1 != NULL)) {
    hwloc_obj_t nonio0 = hwloc_get_non_io_ancestor_obj(topology, obj0);
    hwloc_obj_t nonio1 = hwloc_get_non_io_ancestor_obj(topology, obj1);
    if ((nonio0 != NULL) && (nonio1 != NULL)) {
      hwloc_obj_t common = hwloc_get_common_ancestor_obj(topology, nonio0,
                                                 nonio1);
      if (common != NULL) {
        dist = (nonio0->depth - common->depth) +
               (nonio1->depth - common->depth);
        if (dist < 0) {
          dist = 0;
        }
      }
    }
  }
  return dist;
}

//
// Comparison function for sort. Sorts objects based on PCI bus address.
//
static int comparePCIObjs(const void *a, const void *b)
{
  hwloc_obj_t objA = *((hwloc_obj_t *) a);
  hwloc_obj_t objB = *((hwloc_obj_t *) b);

  assert(objA->type == HWLOC_OBJ_PCI_DEVICE);
  assert(objB->type == HWLOC_OBJ_PCI_DEVICE);

  struct hwloc_pcidev_attr_s *attrA = &(objA->attr->pcidev);
  struct hwloc_pcidev_attr_s *attrB = &(objB->attr->pcidev);

  // Compare the PCI bus addresses
  int result = 0;
  if (attrA->domain != attrB->domain) {
    result = attrA->domain < attrB->domain ? -1 : 1;
  } else if (attrA->bus != attrB->bus) {
    result = attrA->bus < attrB->bus ? -1 : 1;
  } else if (attrA->dev != attrB->dev) {
    result = attrA->dev < attrB->dev ? -1 : 1;
  } else if (attrA->func != attrB->func) {
    result = attrA->func < attrB->func ? -1 : 1;
  } else {
    result = 0;
  }
  return result;
}

static void fillDistanceMatrix(int numObjs, hwloc_obj_t *objs,
                               int distances[][numObjs]) {

  // Build a distance matrix between locales and objects.

  int numLocales = chpl_get_num_locales_on_node();
  _DBG_P("numLocales = %d numObjs = %d", numLocales, numObjs);

  hwloc_obj_t locales[numLocales];

  for (int i = 0; i < numLocales; i++) {
    if (logAccSets[i] != NULL) {
      CHK_ERR(locales[i] =  hwloc_get_obj_covering_cpuset(topology,
                                                        logAccSets[i]));
      char buf[1024];
      hwloc_obj_attr_snprintf(buf, sizeof(buf), locales[i], ",", 1);
      _DBG_P("locales[%d]: %s", i, buf);
    } else {
      locales[i] = NULL;
    }
  }

  for (int i = 0; i < numObjs; i++) {
    char buf[1024];
    hwloc_obj_attr_snprintf(buf, sizeof(buf), objs[i], ",", 1);
    _DBG_P("objs[%d]: %s", i, buf);
  }

  // Compute the distances between locales and objects. If locales[j]
  // is NULL then we don't know which PUs that locale is using, so
  // we ignore it by setting its distances to infinite.

  for (int i = 0; i < numLocales; i++) {
    for (int j = 0; j < numObjs; j++) {
      if (locales[i] != NULL) {
        distances[i][j] = distance(topology, objs[j], locales[i]);
      } else {
        distances[i][j] = INT32_MAX;
      }
    }
  }
#ifdef DEBUG
  printf("distances:\n");
  for (int i = 0; i < numLocales; i++) {
    for (int j = 0; j < numObjs; j++) {
      printf("%02d ", distances[i][j]);
    }
    printf("\n");
  }
#endif
}


//
// Given a NIC with the specified PCI address, determines which NIC of the
// same type (same PCI Vendor ID and PCI Device ID) is the best to use under
// the following constraints:
//  * every locale must be assigned exactly one NIC
//  * a NIC is be shared between locales only if necessary
//  * a locale should use the NIC that is closest to it in the hwloc topology,
//    subject to the above constraints
//
// Returns the PCI address of the NIC that should be used, NULL if there
// was an error.
//

chpl_topo_pci_addr_t *chpl_topo_selectNicByType(chpl_topo_pci_addr_t *inAddr,
                                                chpl_topo_pci_addr_t *outAddr)
{

  hwloc_obj_t           nic = NULL;
  chpl_topo_pci_addr_t  *result = NULL;
  int numLocales = chpl_get_num_locales_on_node();
  struct hwloc_pcidev_attr_s *nicAttr;
  int localRank = chpl_get_local_rank();

  _DBG_P("chpl_topo_selectNicByType: %04x:%02x:%02x.%x", inAddr->domain,
             inAddr->bus, inAddr->device, inAddr->function);
  _DBG_P("numLocales %d rank %d", numLocales, localRank);

  // find the PCI object corresponding to the specified NIC
  nic = hwloc_get_pcidev_by_busid(topology, (unsigned) inAddr->domain,
                                  (unsigned) inAddr->bus,
                                  (unsigned) inAddr->device,
                                  (unsigned) inAddr->function);
  if (nic != NULL) {
    if ((numLocales > 1) && (localRank >= 0)) {
      // Find all the NICS with the same vendor and device as the specified NIC.
      nicAttr = &(nic->attr->pcidev);
      int maxNics = hwloc_get_nbobjs_by_type(topology, HWLOC_OBJ_PCI_DEVICE);
      hwloc_obj_t nics[maxNics];
      int numNics = 0;

      for (hwloc_obj_t obj = hwloc_get_next_pcidev(topology, NULL);
           obj != NULL;
           obj = hwloc_get_next_pcidev(topology, obj)) {

        if (obj->type == HWLOC_OBJ_PCI_DEVICE) {
          struct hwloc_pcidev_attr_s *attr = &(obj->attr->pcidev);
          if ((attr->vendor_id == nicAttr->vendor_id) &&
              (attr->device_id == nicAttr->device_id)) {
              nics[numNics++] = obj;
          }
        }
      }
      if (numNics > 1) {
        //
        // Sort the NICs so that all locales have them in the same order,
        // should hwloc return them in different orders to different locales.
        //
        qsort(nics, numNics, sizeof(*nics), comparePCIObjs);

        int distances[numLocales][numNics];
        fillDistanceMatrix(numNics, nics, distances);


        // Search the distance matrix for the shortest distance and assign the
        // NIC to the locale. Repeat for all unassigned NICs and locales
        // until either all locales have a NIC or there are no more
        // unassigned NICs. In the latter situation locales will have to
        // share NICs, so mark all NICs as unassigned and repeat the
        // process.

        hwloc_obj_t assigned[numLocales]; // NIC assigned to the locale
        int numAssigned = 0;

        for (int i = 0; i < numLocales; i++) {
          assigned[i] = NULL;
        }

        chpl_bool finished = false;
        while (!finished && (numAssigned < numLocales)) {

          _DBG_P("outer loop: numAssigned %d", numAssigned);
          // The used array keeps track of NICs that have been assigned in
          // this iteration.
          chpl_bool used[numNics];
          for (int i = 0; i < numNics; i++) {
            used[i] = false;
          }
          // Find the minimum distance between a locale that hasn't been
          // assigned a NIC and a NIC that hasn't been assigned in this
          // iteration ("used") and assign that NIC to that locale.
          int numAvail = numNics;
          while((numAvail > 0) && (numAssigned < numLocales)) {
            _DBG_P("inner loop: numAssigned %d numAvail %d", numAssigned, numAvail);
              int minimum = INT32_MAX;
              int minNic = -1;
              int minLoc = -1;
              for (int i = 0; i < numLocales; i++) {
                _DBG_P("assigned[%d] = %p", i, assigned[i]);
                _DBG_P("minimum = %d", minimum);
                if (!assigned[i]) {
                  for (int j = 0; j < numNics; j++) {
                    _DBG_P("used[%d] = %d, distances[%d][%d] = %d",
                           j, used[j], i, j, distances[i][j]);
                    if ((!used[j]) && (distances[i][j] <= minimum)) {
                      minimum = distances[i][j];
                      minLoc = i;
                      minNic = j;
                    }
                  }
                }
              }
              assert((minLoc >= 0) && (minNic >= 0));
              assigned[minLoc] = nics[minNic];
              used[minNic] = true;
              if (minLoc == localRank) {
                // We are done once we find our NIC.
                nic = nics[minNic];
                finished = true;
                break;
              }
              numAssigned++;
              numAvail--;
          }
        }
      }
    }
  } else {
    _DBG_P("Could not find NIC %04x:%02x:%02x.%x", inAddr->domain,
           inAddr->bus, inAddr->device, inAddr->function);
  }

  if (nic != NULL) {
    nicAttr = &(nic->attr->pcidev);
    if (outAddr != NULL) {
      outAddr->domain = nicAttr->domain;
      outAddr->bus = nicAttr->bus;
      outAddr->device = nicAttr->dev;
      outAddr->function = nicAttr->func;
      result = outAddr;
    }
  }
  return result;
}

// Given the PCI bus addresses of a set of devices, determine which of those
// devices the calling locale should use. Each co-locale is assigned the same
// number of devices and each device is assigned to at most one locale. This
// function uses a greedy algorithm to assign devices to locales. The
// distance matrix records the distance between each device and the locale's
// CPU set. The device/locale pair with the minimum distance are assigned to
// each other and the device is removed from consideration. The process then
// repeats until all co-locales have been assigned the proper number of
// devices.
//
// Note that cores are assigned to co-locales during initialization of the
// topology layer before this function is called. As a result, the assignment
// of cores and devices to co-locales may not be optimal, especially if the
// machine topology is asymmetric. For example, if there are two co-locales
// on a machine with four NUMA domains, one co-locale will be assigned cores
// in the first two NUMA domains and the other the second two domains. If
// there are two devices each connected to one of the first two domains then
// the second co-locale will use a device that is relatively far from it.

int chpl_topo_selectMyDevices(chpl_topo_pci_addr_t *inAddrs,
                              chpl_topo_pci_addr_t *outAddrs,
                              int *count)
{
  int result = 0;
  int numLocales = chpl_get_num_locales_on_node();
  _DBG_P("count = %d", *count);
  _DBG_P("numLocales = %d", numLocales);
  int numColocales = chpl_env_rt_get_int("LOCALES_PER_NODE", 0);
  if (numColocales > 1) {
    int numDevs = *count;
    int owners[numDevs]; // locale that owns each device
    hwloc_obj_t objs[numDevs]; // the device objects
    int devsPerLocale = numDevs / numColocales;
    _DBG_P("devsPerLocale = %d", devsPerLocale);
    int owned[numColocales]; // number of devices each co-locale owns

    for (int i = 0; i < numDevs; i++) {
      owners[i] = -1;
      objs[i] = NULL;
    }

    for (int i = 0; i < numColocales; i++) {
      owned[i] = 0;
    }

    int rank = chpl_get_local_rank();
    if (rank >= 0) {
      for (int i = 0; i < numDevs; i++) {
        hwloc_obj_t obj;
        // find the PCI object corresponding to the specified bus address
        obj = hwloc_get_pcidev_by_busid(topology,
                                        (unsigned) inAddrs[i].domain,
                                        (unsigned) inAddrs[i].bus,
                                        (unsigned) inAddrs[i].device,
                                        (unsigned) inAddrs[i].function);
        if (obj == NULL) {
          _DBG_P("Could not find PCI %04x:%02x:%02x.%x", inAddrs[i].domain,
                 inAddrs[i].bus, inAddrs[i].device, inAddrs[i].function);
          if (debug) {
            _DBG_P("PCI devices:");
            for (hwloc_obj_t obj = hwloc_get_next_pcidev(topology, NULL);
                 obj != NULL;
                 obj = hwloc_get_next_pcidev(topology, obj)) {
              _DBG_P("%04x:%02x:%02x.%x", obj->attr->pcidev.domain,
                 obj->attr->pcidev.bus, obj->attr->pcidev.dev,
                 obj->attr->pcidev.func);
            }
          }
          result = 1;
          goto done;
        }
        objs[i] = obj;
      }
      int distances[numLocales][numDevs];
      fillDistanceMatrix(numDevs, objs, distances);
      while (owned[rank] < devsPerLocale) {

        // Find the minimum distance between a locale that needs more devices
        // and a device that doesn't have an owner and assign that device to
        // that locale.

        int minimum = INT32_MAX;
        int minDev = -1;
        int minLoc = -1;
        for (int i = 0; i < numLocales; i++) {
          if (owned[i] < devsPerLocale) {
            for (int j = 0; j < numDevs; j++) {
              if ((owners[j] == -1) && (distances[i][j] <= minimum)) {
                minimum = distances[i][j];
                minLoc = i;
                minDev = j;
              }
            }
          }
        }
        assert((minDev >= 0) && (minLoc >= 0));
        owners[minDev] = minLoc;
        owned[minLoc]++;
      }
    }
    // Return the addresses of our devices
    int j = 0;
    for (int i = 0; i < numDevs; i++) {
      if (owners[i] == rank) {
          outAddrs[j++] = inAddrs[i];
      }
    }
    assert(j == devsPerLocale);
    *count = devsPerLocale;
  } else {
    // No co-locales, use all the devices.
    for (int i = 0; i < *count; i++) {
      outAddrs[i] = inAddrs[i];
    }
  }
done:
  _DBG_P("returning %d, count = %d", result, *count);
  return result;
}

static
void chk_err_fn(const char* file, int lineno, const char* what) {
  chpl_internal_error_v("%s: %d: !(%s)", file, lineno, what);
}


static
void chk_err_errno_fn(const char* file, int lineno, const char* what) {
  chpl_internal_error_v("%s: %d: !(%s): %s", file, lineno, what,
                        strerror(errno));
}
