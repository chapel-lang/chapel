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

#ifdef _CHPL_GPU_GEN_INCLUDES_H
#define _CHPL_GPU_GEN_INCLUDES_H

#ifdef HAS_GPU_LOCALE

#include "chpltypes.h"
#include "chpl-comm.h"


// TODO
__device__ static inline c_sublocid_t chpl_task_getRequestedSubloc(void)
{
  // TODO
  return 1;
}

__device__ static inline chpl_localeID_t chpl_gen_getLocaleID(void)
{
  chpl_localeID_t localeID;
  localeID = {chpl_nodeID,chpl_task_getRequestedSubloc()};
  return localeID;
}
                           

__device__ static inline chpl_localeID_t chpl_rt_buildLocaleID(c_nodeid_t node,  c_sublocid_t subloc) {
  chpl_localeID_t loc = { node, subloc };
  return loc;
}

__device__ static inline c_nodeid_t get_chpl_nodeID(void) {
  return chpl_nodeID;
}

#endif // HAS_GPU_LOCALE

#endif // _CHPL_GPU_GEN_INCLUDES_H
