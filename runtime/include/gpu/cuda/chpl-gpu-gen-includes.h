/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

#ifndef _CHPL_GPU_GEN_INCLUDES_H
#define _CHPL_GPU_GEN_INCLUDES_H

#ifdef HAS_GPU_LOCALE

#include "chpltypes.h"
#include "chpl-comm.h"


// General TODO
// This file is included in the application executable only. It mirrors
// chpl-gen-include in providing these runtime interfaces to the Chapel module
// and the compiler-generated, communication-related code. We need these
// functions to be executing correctly on the GPU to have GPU-driven
// communication, with the assumption that GPU-driven communication will look
// like regular communication to the compiler. But until we have proper
// implementation for the GPU-driven communication, we just need __device__
// versions of these functions so that we can compile Chapel applications.

// TODO
// This file might need a `chpl_nodeID` analogue. We need the GPU kernels to be
// aware of which locale they're executing on.

__device__ static inline c_sublocid_t chpl_task_getRequestedSubloc(void)
{
  // TODO
  // We probably need an idea similar to `chpl_nodeID`. But, in our current
  // design, the subloc ID that this function is supposed to return is the same
  // as the device ID. So, maybe we call some device function to grab that and
  // return it?
  return 0;
}

// TODO Rest of the functions are relatively boilerplate once we have everything
__device__ static inline chpl_localeID_t chpl_gen_getLocaleID(void)
{
  chpl_localeID_t localeID;
  localeID = {0,chpl_task_getRequestedSubloc()};
  return localeID;
}

__device__ static inline void* c_pointer_return(void* x) { return x; }

__device__ static inline chpl_localeID_t chpl_rt_buildLocaleID(c_nodeid_t node,  c_sublocid_t subloc) {
  chpl_localeID_t loc = { node, subloc };
  return loc;
}

__device__ static inline c_nodeid_t get_chpl_nodeID(void) {
  return 0;
}

#endif // HAS_GPU_LOCALE

#endif // _CHPL_GPU_GEN_INCLUDES_H
