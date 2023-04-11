/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

//static inline c_sublocid_t chpl_task_getRequestedSubloc(void)
//{
  //return chpl_task_getRequestedSubloc();
//}

 //static inline chpl_localeID_t chpl_gen_getLocaleID(void)
//{
  //chpl_localeID_t localeID;
  //localeID = {0,chpl_task_getRequestedSubloc()};
  //return localeID;
//}

//__device__ static inline void* c_pointer_return(void* x) { return x; }

//__device__ static inline chpl_localeID_t chpl_rt_buildLocaleID(c_nodeid_t node,  c_sublocid_t subloc) {
  //chpl_localeID_t loc = { node, subloc };
  //return loc;
//}

//__device__ static inline c_nodeid_t get_chpl_nodeID(void) {
  //return 0;
//}

//__device__ static inline c_nodeid_t chpl_rt_nodeFromLocaleID(chpl_localeID_t loc) {
  //return loc.node;
//}

//__device__ static inline void chpl_gen_comm_get(void *addr, c_nodeid_t node,
  //void* raddr, size_t size, int32_t commID, int ln, int32_t fn)
//{
  //// TODO
//}

//__device__ static inline void chpl_gen_comm_put(void* addr, c_nodeid_t node,
  //void* raddr, size_t size, int32_t commID, int ln, int32_t fn)
//{
  //// TODO
//}

static inline void chpl_gpu_write(const char *str) { printf("%s", str); }

static inline void chpl_assert_on_gpu(int32_t lineno, int32_t filenameIdx) {
  chpl_warning("assertOnGpu() is ignored with CHPL_GPU_[CODEGEN|RUNTIME]=none", lineno, filenameIdx);
}

static inline unsigned int chpl_gpu_clock(void) {
  chpl_warning("chpl_gpu_clock was called.", 0, 0);
  return (unsigned int)clock();
}

static inline void chpl_gpu_printTimeDelta(
  const char *msg, unsigned int start, unsigned int end)
{
  printf("%s%u\n", msg, end - start);
}

static inline void chpl_gpu_force_sync() {
  chpl_warning("chpl_gpu_force_sync was called", 0, 0);
  
}

#endif // HAS_GPU_LOCALE

#endif // _CHPL_GPU_GEN_INCLUDES_H
