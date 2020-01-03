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
// Launch assistance for the uGNI communication interface.
//

#include "chplrt.h"
#include "chpl-comm-launch.h"
#include "chpl-env.h"


void chpl_comm_preLaunch(void) {
  if (chpl_env_rt_get_bool("OVERSUBSCRIBED", false)) {
    //
    // This only applies to the sockets provider.  Here in the launcher
    // we can't tell if that will be used, but setting it superfluously
    // won't hurt anything.
    //
    chpl_env_set("FI_SOCKETS_PE_WAITTIME", "0", 0);
  }
}
