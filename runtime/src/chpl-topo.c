/*
 * Copyright 2004-2018 Cray Inc.
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
// Compute node topology support
//
#include "chplrt.h"

#include "chpl-align.h"
#include "chpl-env-gen.h"
#include "chplcgfns.h"
#include "chplsys.h"
#include "chpl-topo.h"
#include "chpltypes.h"
#include "error.h"

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if defined(CHPL_HWLOC_HWLOC) || defined(CHPL_HWLOC_SYSTEM)
#define CHPL_HAS_HWLOC
#endif

#ifdef CHPL_HAS_HWLOC
//
// We have hwloc, so provide a real implementation of the topology
// interface.
//
#include "hwloc.h"

#ifdef DEBUG
// note: format arg 'f' must be a string constant
#define _DBG_P(f, ...)                                                  \
        do {                                                            \
          printf("%s:%d: " f "\n", __FILE__, __LINE__, ## __VA_ARGS__); \
        } while (0)
#else
#define _DBG_P(f, ...)
#endif

static chpl_bool haveTopology;

static hwloc_topology_t topology;

static const struct hwloc_topology_support* topoSupport;
static chpl_bool do_set_area_membind;

static int topoDepth;

static int numaLevel;
static int numNumaDomains;


static hwloc_obj_t getNumaObj(c_sublocid_t);
static void alignAddrSize(void*, size_t, chpl_bool,
                          size_t*, unsigned char**, size_t*);
static void chpl_topo_setMemLocalityByPages(unsigned char*, size_t,
                                            hwloc_obj_t);
static void report_error(const char*, int);


void chpl_topo_init(void) {
  //
  // For now we don't load topology information for locModel=flat, since
  // we won't use it in that case and loading it is somewhat expensive.
  // Eventually we will probably load it even for locModel=flat and use
  // it as the information source for what's currently in chplsys, and
  // also pass it to Qthreads when we use that (so it doesn't load it
  // again), but that's work for the future.
  //
  haveTopology = (strcmp(CHPL_LOCALE_MODEL, "flat") != 0) ? true : false;
  if (!haveTopology) {
    return;
  }

  // Check hwloc API version.
  // Require at least hwloc version 1.11 (we need 1.11.5 to not crash
  // in some NUMA configurations).
  // Check both at build time and run time.
#define REQUIRE_HWLOC_VERSION 0x00010b00

#if HWLOC_API_VERSION < REQUIRE_HWLOC_VERSION
#error hwloc version 1.11.5 or newer is required
#else
  {
    unsigned version = hwloc_get_api_version();
    // check that the version is at least REQUIRE_HWLOC_VERSION
    if (version < REQUIRE_HWLOC_VERSION)
      chpl_internal_error("hwloc version 1.11.5 or newer is required");
  }
#endif

  //
  // Allocate and initialize topology object.
  //
  if (hwloc_topology_init(&topology)) {
    report_error("hwloc_topology_init()", errno);
  }

  //
  // Perform the topology detection.
  //
  if (hwloc_topology_load(topology)) {
    report_error("hwloc_topology_load()", errno);
  }

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
  // How many NUMA domains do we have?
  //
  {
    int level;

    //
    // Note: If there are multiple levels with NUMA nodes, this finds
    //       only the uppermost.
    //
    for (level = 0, numaLevel = -1;
         level < topoDepth && numaLevel == -1;
         level++) {
      if (hwloc_get_depth_type(topology, level) == HWLOC_OBJ_NUMANODE) {
        numaLevel = level;
      }
    }
  }

  //
  // Find the NUMA nodes, that is, the objects at numaLevel that also
  // have CPUs.  This is as opposed to things like Xeon Phi HBM, which
  // is memory-only, no CPUs.
  //
  {
    const hwloc_cpuset_t cpusetAll = hwloc_get_root_obj(topology)->cpuset;
    numNumaDomains =
      hwloc_get_nbobjs_inside_cpuset_by_depth(topology, cpusetAll, numaLevel);
  }
}


void chpl_topo_exit(void) {
  if (!haveTopology) {
    return;
  }

  hwloc_topology_destroy(topology);
}


int chpl_topo_getNumNumaDomains(void) {
  return numNumaDomains;
}


void chpl_topo_setThreadLocality(c_sublocid_t subloc) {
  hwloc_cpuset_t cpuset;
  int flags;

  _DBG_P("chpl_topo_setThreadLocality(%d)\n", (int) subloc);

  if (!haveTopology) {
    return;
  }

  if (!topoSupport->cpubind->set_thread_cpubind)
    return;

  if ((cpuset = hwloc_bitmap_alloc()) == NULL) {
    report_error("hwloc_bitmap_alloc()", errno);
  }

  hwloc_cpuset_from_nodeset(topology, cpuset,
                            getNumaObj(subloc)->allowed_nodeset);

  flags = HWLOC_CPUBIND_THREAD | HWLOC_CPUBIND_STRICT;
  if (hwloc_set_cpubind(topology, cpuset, flags)) {
    report_error("hwloc_set_cpubind()", errno);
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

  if ((cpuset = hwloc_bitmap_alloc()) == NULL) {
    report_error("hwloc_bitmap_alloc()", errno);
  }

  if ((nodeset = hwloc_bitmap_alloc()) == NULL) {
    report_error("hwloc_bitmap_alloc()", errno);
  }

  flags = HWLOC_CPUBIND_THREAD;
  if (hwloc_get_cpubind(topology, cpuset, flags)) {
    report_error("hwloc_get_cpubind()", errno);
  }

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

  _DBG_P("chpl_topo_setMemLocality(%p, %#zx, onlyIn=%s, %d)\n",
         p, size, (onlyInside ? "T" : "F"), (int) subloc);

  if (!haveTopology) {
    return;
  }

  alignAddrSize(p, size, onlyInside, &pgSize, &pPgLo, &nPages);

  _DBG_P("    localize %p, %#zx bytes (%#zx pages)\n",
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

  _DBG_P("chpl_topo_setMemSubchunkLocality(%p, %#zx, onlyIn=%s)\n",
         p, size, (onlyInside ? "T" : "F"));

  if (!haveTopology) {
    return;
  }

  alignAddrSize(p, size, onlyInside, &pgSize, &pPgLo, &nPages);

  _DBG_P("    localize %p, %#zx bytes (%#zx pages)\n",
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

  _DBG_P("chpl_topo_touchMemFromSubloc(%p, %#zx, onlyIn=%s, %d)\n",
         p, size, (onlyInside ? "T" : "F"), (int) subloc);

  if (!haveTopology
      || !topoSupport->cpubind->get_thread_cpubind
      || !topoSupport->cpubind->set_thread_cpubind) {
    return;
  }

  alignAddrSize(p, size, onlyInside, &pgSize, &pPgLo, &nPages);

  _DBG_P("    localize %p, %#zx bytes (%#zx pages)\n",
         pPgLo, nPages * pgSize, nPages);

  if (nPages == 0)
    return;

  if ((cpuset = hwloc_bitmap_alloc()) == NULL) {
    report_error("hwloc_bitmap_alloc()", errno);
  }

  flags = HWLOC_CPUBIND_THREAD;
  if (hwloc_get_cpubind(topology, cpuset, flags)) {
    report_error("hwloc_get_cpubind()", errno);
  }

  chpl_topo_setThreadLocality(subloc);

  {
    size_t pg;
    for (pg = 0; pg < nPages; pg++) {
      pPgLo[pg * pgSize] = 0;
    }
  }

  flags = HWLOC_CPUBIND_THREAD | HWLOC_CPUBIND_STRICT;
  if (hwloc_set_cpubind(topology, cpuset, flags)) {
    report_error("hwloc_set_cpubind()", errno);
  }

  hwloc_bitmap_free(cpuset);
}


static inline
hwloc_obj_t getNumaObj(c_sublocid_t subloc) {
  // could easily imagine this being a bit slow, but it's okay for now
  return
    hwloc_get_obj_inside_cpuset_by_depth(topology,
                                         hwloc_get_root_obj(topology)->cpuset,
                                         numaLevel,
                                         subloc);
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

  _DBG_P("hwloc_set_area_membind_nodeset(%p, %#zx, %d)\n", p, size,
         (int) hwloc_bitmap_first(numaObj->allowed_nodeset));

  flags = HWLOC_MEMBIND_MIGRATE | HWLOC_MEMBIND_STRICT;
  if (hwloc_set_area_membind_nodeset(topology, p, size,
                                     numaObj->allowed_nodeset,
                                     HWLOC_MEMBIND_BIND, flags)) {
    report_error("hwloc_set_area_membind_nodeset()", errno);
  }
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

  if ((nodeset = hwloc_bitmap_alloc()) == NULL) {
    report_error("hwloc_bitmap_alloc()", errno);
  }

  flags = HWLOC_MEMBIND_BYNODESET;
  if (hwloc_get_area_memlocation(topology, p, 1, nodeset, flags)) {
    report_error("hwloc_get_area_memlocation()", errno);
  }

  node = hwloc_bitmap_first(nodeset);
  if (!isActualSublocID(node)) {
    node = c_sublocid_any;
  }

  hwloc_bitmap_free(nodeset);

  return node;
}


static
void report_error(const char* what, int errnum) {
  char buf[100];

  snprintf(buf, sizeof(buf), "%s: %s", what, strerror(errnum));
  chpl_internal_error(buf);
}

#else
//
// We do not have hwloc, so provide a vacuous implementation of the
// topology interface.
//

void chpl_topo_init(void) { }
void chpl_topo_exit(void) { }
int chpl_topo_getNumNumaDomains(void) { return 1; }
void chpl_topo_setThreadLocality(c_sublocid_t subloc) { }
c_sublocid_t chpl_topo_getThreadLocality(void) { return c_sublocid_any; }
void chpl_topo_setMemLocality(void* p, size_t size, chpl_bool onlyInside,
                              c_sublocid_t subloc) { }
void chpl_topo_setMemSubchunkLocality(void* p, size_t size,
                                      chpl_bool onlyInside,
                                      size_t* subchunkSizes) { }
void chpl_topo_touchMemFromSubloc(void* p, size_t size, chpl_bool onlyInside,
                                  c_sublocid_t subloc) { }
c_sublocid_t chpl_topo_getMemLocality(void* p) { return c_sublocid_any; }

#endif // if defined(CHPL_HAS_HWLOC)
