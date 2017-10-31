/*
 * Copyright 2004-2017 Cray Inc.
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
// System and heap page sizes, for the uGNI communication interface.
//

#include <stdint.h>
#include <stdlib.h>

#include "chplrt.h"
#include "chplcgfns.h"
#include "comm-ugni-heap-pages.h"
#include "error.h"


//
// This is really just a utility function, but exposing it makes things
// easier for chpl_comm_ugni_getHeapPageSize() callers in the launcher,
// where the regular chplsys.c support isn't available.
//
size_t chpl_comm_ugni_getSysPageSize(void) {
  static size_t sps = 0;

  if (sps != 0)
    return sps;

#if defined _SC_PAGESIZE
  sps = (size_t) sysconf(_SC_PAGESIZE);
#elif defined _SC_PAGE_SIZE
  sps = (size_t) sysconf(_SC_PAGE_SIZE);
#endif

  if (sps == 0)
    chpl_internal_error("can't get system page size");

  return sps;
}


//
// Figure out what the heap page size will be.
//
size_t chpl_comm_ugni_getHeapPageSize(void) {
  static size_t hps = 0;

  if (hps != 0)
    return hps;

  //
  // Do we have mem=jemalloc and a dynamically extensible heap?
  //
  if (strcmp(CHPL_MEM, "jemalloc") != 0
      || getenv("CHPL_RT_MAX_HEAP_SIZE") != NULL) {
    return (hps = chpl_comm_ugni_getSysPageSize());
  }

  //
  // Do we have hugepages?
  //
  const char* ev = getenv("HUGETLB_DEFAULT_PAGE_SIZE");

  if (ev == NULL) {
    return (hps = chpl_comm_ugni_getSysPageSize());
  }

  //
  // Figure out the heap page size.
  //
  int num_scanned;
  char units;

  if ((num_scanned = sscanf(ev, "%zi%c", &hps, &units)) != 1) {
    if (num_scanned == 2 && strchr("kKmMgG", units) != NULL) {
      switch (units) {
      case 'k' : case 'K': hps <<= 10; break;
      case 'm' : case 'M': hps <<= 20; break;
      case 'g' : case 'G': hps <<= 30; break;
      }
    }
    else {
      chpl_error("Cannot parse HUGETLB_DEFAULT_PAGE_SIZE environment variable",
                 0, 0);
    }
  }

  return hps;
}
