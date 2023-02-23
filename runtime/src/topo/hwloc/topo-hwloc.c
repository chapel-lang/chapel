/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

static void cpuInfoInit(void);

// Accessible NUMA nodes

static hwloc_nodeset_t numaSet = NULL;

// Our root within the overall topology.
static hwloc_obj_t root = NULL;

// Our socket, if applicable.
static hwloc_obj_t socket = NULL;


static hwloc_obj_t getNumaObj(c_sublocid_t);
static void alignAddrSize(void*, size_t, chpl_bool,
                          size_t*, unsigned char**, size_t*);
static void chpl_topo_setMemLocalityByPages(unsigned char*, size_t,
                                            hwloc_obj_t);

// CPU reservation must happen before CPU information is returned to other
// layers.
static chpl_bool okToReserveCPU = true;

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


void chpl_topo_init(void) {
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

  // Check hwloc API version.
  // Require at least hwloc version 1.11 (we need 1.11.5 to not crash
  // in some NUMA configurations).
  // Check both at build time and run time.
#define REQUIRE_HWLOC_VERSION 0x00010b00

#if HWLOC_API_VERSION < REQUIRE_HWLOC_VERSION
#error hwloc version 1.11.5 or newer is required
#endif

  CHK_ERR(hwloc_get_api_version() >= REQUIRE_HWLOC_VERSION);

  //
  // Allocate and initialize topology object.
  //
  CHK_ERR_ERRNO(hwloc_topology_init(&topology) == 0);

  //
  // Perform the topology detection.
  //
  CHK_ERR_ERRNO(hwloc_topology_load(topology) == 0);

  //
  // What is supported?
  //
  topoSupport = hwloc_topology_get_support(topology);

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

  //
  // By default our root is the root of the topology.
  //

  root = hwloc_get_root_obj(topology);

  cpuInfoInit();
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
  if (numaSet != NULL) {
    hwloc_bitmap_free(numaSet);
    numaSet = NULL;
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
  okToReserveCPU = false;
  int cpus = (accessible_only) ? numCPUsPhysAcc : numCPUsPhysAll;
  if (cpus == -1) {
    chpl_error("number of cpus is uninitialized", 0, 0);
  }
  return cpus;
}


int chpl_topo_getNumCPUsLogical(chpl_bool accessible_only) {
  okToReserveCPU = false;
  int cpus = (accessible_only) ? numCPUsLogAcc : numCPUsLogAll;
  if (cpus == -1) {
    chpl_error("number of cpus is uninitialized", 0, 0);
  }
  return cpus;
}


//
// Initializes information about CPUs (cores and PUs) and and NICs from the
// topology.
//

static void cpuInfoInit(void) {
  _DBG_P("cpuInfoInit");
  //
  // accessible cores and PUs
  //
  CHK_ERR_ERRNO((physAccSet = hwloc_bitmap_alloc()) != NULL);
  CHK_ERR_ERRNO((physReservedSet = hwloc_bitmap_alloc()) != NULL);
  CHK_ERR_ERRNO((logAccSet = hwloc_bitmap_alloc()) != NULL);

  // accessible NUMA nodes

  CHK_ERR_ERRNO((numaSet = hwloc_bitmap_alloc()) != NULL);

  //
  // Hwloc can't tell us the number of accessible cores directly, so
  // get that by counting the parent cores of the accessible PUs.
  //

  // accessible PUs

  //
  // We could seemingly use hwloc_topology_get_allowed_cpuset() to get
  // the set of accessible PUs here.  But that seems not to reflect the
  // schedaffinity settings, so use hwloc_get_proc_cpubind() instead.
  //
  if (topoSupport->cpubind->get_proc_cpubind) {
    int rc = hwloc_get_proc_cpubind(topology, getpid(), logAccSet, 0);
    CHK_ERR_ERRNO(rc == 0);
  } else {
    // assume all PUs are accessible
    hwloc_bitmap_fill(logAccSet);
  }
  hwloc_bitmap_and(logAccSet, logAccSet,
                   hwloc_topology_get_online_cpuset(topology));
  numCPUsLogAcc = hwloc_bitmap_weight(logAccSet);
  CHK_ERR(numCPUsLogAcc > 0);
  _DBG_P("numCPUsLogAcc = %d", numCPUsLogAcc);

  //
  // all PUs. The online set doesn't include PUs that cannot be
  // used, e.g., if hyperthreading is turned off
  //
  hwloc_const_cpuset_t onlineSet = hwloc_topology_get_online_cpuset(
                                                              topology);
  numCPUsLogAll = hwloc_bitmap_weight(onlineSet);
  CHK_ERR(numCPUsLogAll > 0);
  _DBG_P("numCPUsLogAll = %d", numCPUsLogAll);


  // accessible cores

  int pusPerCore = 0;
#define NEXT_PU(pu)                                                \
  hwloc_get_next_obj_inside_cpuset_by_type(topology, logAccSet,    \
                                           HWLOC_OBJ_PU, pu)

  for (hwloc_obj_t pu = NEXT_PU(NULL); pu != NULL; pu = NEXT_PU(pu)) {
    hwloc_obj_t core;
    CHK_ERR_ERRNO(core = hwloc_get_ancestor_obj_by_type(topology,
                                                         HWLOC_OBJ_CORE,
                                                         pu));
    int numPus = hwloc_bitmap_weight(core->cpuset);
    CHK_ERR((pusPerCore == 0) || (pusPerCore == numPus));
    pusPerCore = numPus;
    // Use the smallest PU to represent the core.
    int smallest = hwloc_bitmap_first(core->cpuset);
    CHK_ERR(smallest != -1);
    hwloc_bitmap_set(physAccSet, smallest);
  }

#undef NEXT_PU

  numCPUsPhysAcc = hwloc_bitmap_weight(physAccSet);
  CHK_ERR(numCPUsPhysAcc > 0);
  _DBG_P("numCPUsPhysAcc = %d", numCPUsPhysAcc);

  //
  // all cores
  //
  // Note: hwloc_get_nbobjs_inside_cpuset_by_type cannot be called on
  // onlineSet because inaccessible PUs and their cores do not have
  // objects in the topology. pusPerCore might vary by core, but that is
  // checked above.

  numCPUsPhysAll = numCPUsLogAll / pusPerCore;
  CHK_ERR(numCPUsPhysAll > 0);
  _DBG_P("numCPUsPhysAll = %d", numCPUsPhysAll);

  int numLocalesOnNode = chpl_get_num_locales_on_node();
  int expectedLocalesOnNode = chpl_env_rt_get_int("LOCALES_PER_NODE", 0);
  int rank = chpl_get_local_rank();
  _DBG_P("numLocalesOnNode = %d", numLocalesOnNode);
  _DBG_P("expectedLocalesOnNode = %d", expectedLocalesOnNode);
  _DBG_P("rank = %d", rank);
  if ((numLocalesOnNode > 1) || (expectedLocalesOnNode > 1)) {
    if (numLocalesOnNode > 1) {
      oversubscribed = true;
    }

    // We get our own socket if all cores are accessible, we know our local
    // rank, and the number of locales on the node is less than or equal to
    // the number of sockets. It is an error if the number of locales on the
    // node is greater than the number of sockets and CHPL_RT_LOCALES_PER_NODE
    // is set, otherwise we are oversubscribed.

    // TODO: The oversubscription determination is incorrect. A node is only
    // oversubscribed if locales are sharing cores. Need to figure out how
    // to determine this accurately.

    if (numCPUsPhysAcc == numCPUsPhysAll) {
      int numSockets = hwloc_get_nbobjs_inside_cpuset_by_type(topology,
                          root->cpuset, HWLOC_OBJ_PACKAGE);
      _DBG_P("numSockets = %d", numSockets);
      if (numLocalesOnNode <= numSockets) {
        if (rank != -1) {
          socket = hwloc_get_obj_inside_cpuset_by_type(topology,
                                    root->cpuset, HWLOC_OBJ_PACKAGE, rank);
          CHK_ERR(socket != NULL);

          // Limit the accessible cores and PUs to those in our socket.

          hwloc_bitmap_and(logAccSet, logAccSet, socket->cpuset);
          numCPUsLogAcc = hwloc_bitmap_weight(logAccSet);
          CHK_ERR(numCPUsLogAcc > 0);

          hwloc_bitmap_and(physAccSet, physAccSet, socket->cpuset);
          numCPUsPhysAcc = hwloc_bitmap_weight(physAccSet);
          CHK_ERR(numCPUsPhysAcc > 0);

          if (debug) {
            char buf[1024];
            hwloc_bitmap_list_snprintf(buf, sizeof(buf), logAccSet);
            _DBG_P("numCPUsLogAcc: %d logAccSet: %s", numCPUsLogAcc, buf);
            hwloc_bitmap_list_snprintf(buf, sizeof(buf), physAccSet);
            _DBG_P("numCPUsPhysAcc: %d physAccSet: %s", numCPUsPhysAcc, buf);
          }
          root = socket;
          oversubscribed = false;
        }
      } else if (expectedLocalesOnNode > 0) {
        char msg[100];
        snprintf(msg, sizeof(msg), "The number of locales on the node is "
                 "greater than the number of sockets (%d > %d).",
                 numLocalesOnNode, numSockets);
        chpl_error(msg, 0, 0);
      }
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

    set = hwloc_topology_get_online_cpuset(topology);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), set);
    _DBG_P("online cpuset: %s", buf);

    set = hwloc_topology_get_topology_cpuset(topology);
    hwloc_bitmap_list_snprintf(buf, sizeof(buf), set);
    _DBG_P("topology cpuset: %s", buf);

  }
}

void chpl_topo_post_args_init(void) {
  if ((verbosity >= 2) && (socket != NULL)) {
    printf("%d: using socket %d\n", chpl_nodeID,
           socket->logical_index);
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
  okToReserveCPU = false;
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
                            getNumaObj(subloc)->allowed_nodeset);

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
    return c_sublocid_any;
  }

  if (!topoSupport->cpubind->get_thread_cpubind) {
    return c_sublocid_any;
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
  set = hwloc_bitmap_dup(root->cpuset);

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

  _DBG_P("hwloc_set_area_membind_nodeset(%p, %#zx, %d)", p, size,
         (int) hwloc_bitmap_first(numaObj->allowed_nodeset));

  flags = HWLOC_MEMBIND_MIGRATE | HWLOC_MEMBIND_STRICT;
  CHK_ERR_ERRNO(hwloc_set_area_membind_nodeset(topology, p, size,
                                               numaObj->allowed_nodeset,
                                               HWLOC_MEMBIND_BIND, flags)
                == 0);
}


c_sublocid_t chpl_topo_getMemLocality(void* p) {
  int flags;
  hwloc_nodeset_t nodeset;
  int node;

  if (!haveTopology) {
    return c_sublocid_any;
  }

  if (!topoSupport->membind->get_area_memlocation) {
    return c_sublocid_any;
  }

  if (p == NULL) {
    return c_sublocid_any;
  }

  CHK_ERR_ERRNO((nodeset = hwloc_bitmap_alloc()) != NULL);

  flags = HWLOC_MEMBIND_BYNODESET;
  CHK_ERR_ERRNO(hwloc_get_area_memlocation(topology, p, 1, nodeset, flags)
                == 0);

  node = hwloc_bitmap_first(nodeset);
  if (!isActualSublocID(node)) {
    node = c_sublocid_any;
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
  if (okToReserveCPU) {
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
    _DBG_P("okToReserveCPU is false");
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
  _DBG_P("chpl_topo_bindCPUPhysical id: %d status: %d", id, status);
  return status;
}

chpl_bool chpl_topo_isOversubscribed(void) {
  _DBG_P("oversubscribed = %s", oversubscribed ? "True" : "False");
  return oversubscribed;
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
