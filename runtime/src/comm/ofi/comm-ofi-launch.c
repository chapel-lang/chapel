/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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
// Launch assistance for the uGNI communication interface.
//

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "chplrt.h"
#include "chpl-comm-launch.h"
#include "chpl-env.h"


void chpl_comm_preLaunch(int32_t numLocales) {
#ifdef COMM_OFI_OOB_PMI2
  //
  // Set the maximum number of PMI key-value store entries to an amount
  // adequate for our launch needs.  We'll actually only need this if we
  // end up using the PMI2_KVS_*() interface for out-of-band support
  // during comm layer initialization.  At this point we cannot tell
  // whether we'll do so, but it won't hurt anything to set it anyway.
  //
  // Based on limited experimentation, it looks like the actual value we
  // need is ((numLocales * 5) + 3).  (numLocales * 7) is a ceiling.
  //
  {
    int32_t newVal = numLocales * 7;

    const char* evName = "PMI_MAX_KVS_ENTRIES";
    char* evStr;
    int32_t oldVal;
    if ((evStr = getenv(evName)) == NULL
        || sscanf(evStr, "%" SCNi32, &oldVal) < 1
        || oldVal < newVal) {
      char buf[10];
      (void) snprintf(buf, sizeof(buf), "%" PRId32, newVal);
      chpl_env_set(evName, buf, 1);
    }
  }
#endif

  if (chpl_env_rt_get_bool("OVERSUBSCRIBED", false)) {
    //
    // This only applies to the sockets provider.  Here in the launcher
    // we can't tell if that will be used, but setting it superfluously
    // won't hurt anything.
    //
    chpl_env_set("FI_SOCKETS_PE_WAITTIME", "0", 0);
  }
}
