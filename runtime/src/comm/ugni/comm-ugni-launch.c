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
// Launch assistance for the uGNI communication interface.
//

#define _POSIX_C_SOURCE 200112L  // for setenv(3) in <stdlib.h>

#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "chplrt.h"
#include "comm-ugni-heap-pages.h"
#include "chpl-comm-launch.h"
#include "error.h"


//
// With mem=jemalloc and comm=ugni with hugepages and a dynamically
// extensible heap, we have to configure jemalloc's large chunk size.
// It would be preferable to deal with this entirely within the target
// program, but we need it before jemalloc does its constructor-based
// initialization and we don't have a dependable way to arrange that
// in the target program.
//
static
void maybe_set_jemalloc_lg_chunk(void) {
  size_t hps;

  hps = chpl_comm_ugni_getHeapPageSize();
  if (hps == chpl_comm_ugni_getSysPageSize())
    return;

  //
  // We're using the jemalloc memory layer, the heap is dynamically
  // extensible, and we've got hugepages, so we'll be registering
  // memory.  (chpl_comm_ugni_getHeapPageSize()) checks all this.)
  //
  // Arrange for jemalloc's chunk size (and alignment) to be the same
  // as what the comm layer recommends, which is the heap page size.
  // Make sure this is a power of 2, because jemalloc needs that.
  //
  int hps_log2;
  char* ev;
  char buf[1000];

  // sanity check: power of 2 and >= sys page size
  assert((hps & (hps - 1)) == 0
         && hps >= chpl_comm_ugni_getSysPageSize());

  // lg_chunk is specified as the base-2 log of the expansion chunk size
  hps_log2 = lrint(log2((double) hps));

  if ((ev = getenv("CHPL_JE_MALLOC_CONF")) == NULL) {
    snprintf(buf, sizeof(buf), "lg_chunk:%d", hps_log2);
  } else {
    // Override any user-specified lg_chunk.  Smaller or larger, it
    // would break our logic either way.
    if (snprintf(buf, sizeof(buf), "%s,lg_chunk:%d", ev, hps_log2)
        >= sizeof(buf)) {
      chpl_internal_error("setting CHPL_JE_MALLOC_CONF would truncate");
    }
  }

  if (setenv("CHPL_JE_MALLOC_CONF", buf, 1) != 0)
    chpl_internal_error("cannot setenv(CHPL_JE_MALLOC_CONF)");
}


void chpl_comm_preLaunch(void) {
  if (setenv("HUGETLB_VERBOSE", "0", 1) != 0)
    chpl_error("cannot setenv HUGETLB_VERBOSE=0", 0, 0);
  if (setenv("HUGETLB_NO_RESERVE", "yes", 0) != 0)
    chpl_error("cannot setenv HUGETLB_NO_RESERVE=yes", 0, 0);
  maybe_set_jemalloc_lg_chunk();
}
