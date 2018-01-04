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
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "chplrt.h"
#include "comm-ugni-heap-pages.h"
#include "chpl-comm-launch.h"
#include "error.h"

#ifdef CHPL_TARGET_MEM_JEMALLOC
#include "chpl-mem-jemalloc-prefix.h"
#endif


//
// With mem=jemalloc and comm=ugni with hugepages and a dynamically
// extensible heap, we have to configure jemalloc's large chunk size.
// It would be preferable to deal with this entirely within the target
// program, but we need it before jemalloc does its constructor-based
// initialization and we don't have a dependable way to arrange that
// in the target program.
//
static
char* jemalloc_conf_ev_name(void) {
#define _STRFY(s)              #s
#define _EXPAND_THEN_STRFY(s)  _STRFY(s)
#ifdef CHPL_JEMALLOC_PREFIX
  #define EVN_PREFIX_STR         _EXPAND_THEN_STRFY(CHPL_JEMALLOC_PREFIX)
#else
  #define EVN_PREFIX_STR         ""
#endif

  static char evn[100] = "";

  // safe because always called serially
  if (evn[0] != '\0')
    return evn;

  if (snprintf(evn, sizeof(evn), "%sMALLOC_CONF", EVN_PREFIX_STR)
      >= sizeof(evn)) {
    chpl_internal_error("jemalloc conf env var name buffer is too small");
  }

  for (int i = 0; evn[i] != '\0'; i++) {
    if (islower(evn[i]))
      evn[i] = toupper(evn[i]);
  }

  return evn;

#undef _STRFY
#undef _EXPAND_THEN_STRFY
#undef EVN_PREFIX_STR
}


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

  //
  // Now, set the jemalloc environment variable.  We also must include
  // the "purge:" setting we specify at jemalloc configuration time,
  // because the env var overrides config-time settings.
  //
  if ((ev = getenv(jemalloc_conf_ev_name())) == NULL) {
    const char* fmt = "purge:decay,lg_chunk:%d";
    if (snprintf(buf, sizeof(buf), fmt, hps_log2) >= sizeof(buf)) {
      chpl_internal_error("setting jemalloc conf env var would truncate");
    }
  } else {
    // Override any user-specified lg_chunk.  Smaller or larger, it
    // would break our logic either way.
    const char* fmt = ((strstr(ev, "purge:") == NULL)
                       ? "%s,purge:decay,lg_chunk:%d"
                       : "%s,lg_chunk:%d");
    if (snprintf(buf, sizeof(buf), fmt, ev, hps_log2) >= sizeof(buf)) {
      chpl_internal_error("setting jemalloc conf env var would truncate");
    }
  }

  if (setenv(jemalloc_conf_ev_name(), buf, 1) != 0)
    chpl_internal_error("cannot setenv jemalloc conf env var");
}


void chpl_comm_preLaunch(void) {
  if (setenv("HUGETLB_VERBOSE", "0", 1) != 0)
    chpl_error("cannot setenv HUGETLB_VERBOSE=0", 0, 0);
  if (setenv("HUGETLB_NO_RESERVE", "yes", 0) != 0)
    chpl_error("cannot setenv HUGETLB_NO_RESERVE=yes", 0, 0);
  maybe_set_jemalloc_lg_chunk();
}
