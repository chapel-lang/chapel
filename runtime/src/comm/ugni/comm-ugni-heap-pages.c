/*
 * Copyright 2004-2020 Cray Inc.
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

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

#include "chplrt.h"
#include "chplcgfns.h"
#include "chpl-env.h"
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
static pthread_once_t hps_once = PTHREAD_ONCE_INIT;
static size_t hps = 0;


static
void set_hps(void)
{
  hps = chpl_comm_ugni_getSysPageSize();

  //
  // If we have mem=jemalloc, a dynamically extensible heap, and
  // hugepages, then the heap page size is the hugepage size.
  //
  char* ev;
  if (strcmp(CHPL_MEM, "jemalloc") == 0
      && chpl_env_rt_get("MAX_HEAP_SIZE", NULL) == NULL
      && (ev = getenv("HUGETLB_DEFAULT_PAGE_SIZE")) != NULL) {
    hps = chpl_env_str_to_size("HUGETLB_DEFAULT_PAGE_SIZE", ev, hps);
  }
}


size_t chpl_comm_ugni_getHeapPageSize(void) {
  if (hps == 0
      && pthread_once(&hps_once, set_hps) != 0) {
    chpl_internal_error("pthread_once(&hps_once) failed");
  }

  return hps;
}
