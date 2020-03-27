/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include "chplrt.h"
#include "chpl-comm-launch.h"
#include "chpl-env.h"

void chpl_comm_preLaunch() {
  const char* chpl_rt_masterip = chpl_env_rt_get("MASTERIP", NULL);
  if (chpl_rt_masterip != NULL) {
    chpl_env_set("GASNET_MASTERIP", chpl_rt_masterip, 1);
  }
  const char* chpl_rt_workerip = chpl_env_rt_get("WORKERIP", NULL);
  if (chpl_rt_workerip != NULL) {
    chpl_env_set("GASNET_WORKERIP", chpl_rt_workerip, 1);
  }
}
