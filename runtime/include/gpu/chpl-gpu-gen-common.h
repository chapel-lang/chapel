/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#ifndef _CHPL_GPU_GEN_COMMON_H
#define _CHPL_GPU_GEN_COMMON_H

#ifdef HAS_GPU_LOCALE

#include "chpltypes.h"
#include "chpl-comm.h"
#include <string.h>

// device code for reduction support
#include "chpl-gpu-dev-reduce.h"

__device__ static inline c_sublocid_t chpl_task_getRequestedSubloc(void)
{
  // TODO
  // We probably need an idea similar to `chpl_nodeID`. But, in our current
  // design, the subloc ID that this function is supposed to return is the same
  // as the device ID. So, maybe we call some device function to grab that and
  // return it?
  return 0;
}

__device__ static inline void* c_pointer_return(void* x) { return x; }
__device__ static inline const void* c_pointer_return_const(const void* x) {
  return x;
}

__device__ static inline chpl_localeID_t chpl_rt_buildLocaleID(c_nodeid_t node,  c_sublocid_t subloc) {
  chpl_localeID_t loc = { node, subloc };
  return loc;
}

__device__ static inline c_nodeid_t chpl_rt_nodeFromLocaleID(chpl_localeID_t loc) {
  return loc.node;
}

__device__ static inline c_nodeid_t chpl_rt_sublocFromLocaleID(chpl_localeID_t loc) {
  return loc.subloc;
}

__device__ static inline void chpl_gen_comm_get(void *addr, c_nodeid_t node,
  void* raddr, size_t size, int32_t commID, int ln, int32_t fn)
{
  printf("Warning: chpl_gen_comm_get called inside a GPU kernel. This shouldn't have happened.\n");
  // TODO
}

__device__ static inline void chpl_gen_comm_get_unordered(void *addr,
                                                          c_nodeid_t node,
                                                          void* raddr, size_t
                                                          size, int32_t commID,
                                                          int ln, int32_t fn)
{
  printf("Warning: chpl_gen_comm_get_unordered called inside a GPU kernel. This shouldn't have happened.\n");
  // TODO
}

__device__ static inline void chpl_gen_comm_put(void* addr, c_nodeid_t node,
  void* raddr, size_t size, int32_t commID, int ln, int32_t fn)
{
  printf("Warning: chpl_gen_comm_put called inside a GPU kernel. This shouldn't have happened.\n");
  // TODO
}

__device__ static inline void chpl_gen_comm_put_unordered(void *addr,
                                                          c_nodeid_t node,
                                                          void* raddr, size_t
                                                          size, int32_t commID,
                                                          int ln, int32_t fn)
{
  printf("Warning: chpl_gen_comm_put_unordered called inside a GPU kernel. This shouldn't have happened.\n");
  // TODO
}

__device__ static inline void chpl_gen_comm_getput_unordered(
                                            c_nodeid_t dstnode, void* dstaddr,
                                            c_nodeid_t srcnode, void* srcaddr,
                                            size_t size, int32_t commID,
                                            int ln, int32_t fn)
{
  printf("Warning: chpl_gen_comm_getput_unordered called inside a GPU kernel. This shouldn't have happened.\n");
  // TODO
}

MAYBE_GPU static inline void chpl_gpu_write(const char *str) { printf("%s", str); }

MAYBE_GPU static inline void chpl_gpu_printf0(const char *fmt) {
  printf("%s", fmt);
}
MAYBE_GPU static inline void chpl_gpu_printf1(const char *fmt,
 void *x1)
{
  printf(fmt, x1);
}
MAYBE_GPU static inline void chpl_gpu_printf2(const char *fmt,
  void *x1, void *x2)
{
  printf(fmt, x1, x2);
}
MAYBE_GPU static inline void chpl_gpu_printf3(const char *fmt,
  void *x1, void *x2, void *x3)
{
  printf(fmt, x1, x2, x3);
}
MAYBE_GPU static inline void chpl_gpu_printf4(const char *fmt,
  void *x1, void *x2, void *x3, void *x4)
{
  printf(fmt, x1, x2, x3, x4);
}
MAYBE_GPU static inline void chpl_gpu_printf5(const char *fmt,
  void *x1, void *x2, void *x3, void *x4, void *x5)
{
  printf(fmt, x1, x2, x3, x4, x5);
}
MAYBE_GPU static inline void chpl_gpu_printf6(const char *fmt,
  void *x1, void *x2, void *x3, void *x4, void *x5, void *x6)
{
  printf(fmt, x1, x2, x3, x4, x5, x6);
}
MAYBE_GPU static inline void chpl_gpu_printf7(const char *fmt,
  void *x1, void *x2, void *x3, void *x4, void *x5, void *x6, void *x7)
{
  printf(fmt, x1, x2, x3, x4, x5, x6, x7);
}
MAYBE_GPU static inline void chpl_gpu_printf8(const char *fmt,
  void *x1, void *x2, void *x3, void *x4, void *x5, void *x6, void *x7,
  void *x8)
{
  printf(fmt, x1, x2, x3, x4, x5, x6, x7, x8);
}

__device__ static inline void chpl_assert_on_gpu(int32_t lineno, int32_t filenameIdx) { /* no op */ }
__host__ static inline void chpl_assert_on_gpu(int32_t lineno, int32_t filenameIdx) {
  chpl_error("assertOnGpu() failed", lineno, filenameIdx);
}

__device__ static inline unsigned int chpl_gpu_clock(void) {
  return (unsigned int)clock();
}
__host__ static inline unsigned int chpl_gpu_clock(void) {
  return 0;
}

MAYBE_GPU static inline void chpl_gpu_printTimeDelta(
  const char *msg, unsigned int start, unsigned int end)
{
  printf("%s%u\n", msg, end - start);
}

__host__ static inline void chpl_gpu_force_sync() {
  chpl_internal_error("chpl_gpu_force_sync called from host");
}

__host__ static inline void chpl_gpu_force_warp_sync(unsigned mask) {
  chpl_internal_error("chpl_gpu_force_warp_sync called from host");
}


__device__ static inline
void chpl_gen_comm_get_from_subloc(void *addr, c_nodeid_t src_node,
                                   c_sublocid_t src_subloc, void* raddr,
                                   size_t size, int32_t commID, int ln,
                                   int32_t fn) {
  printf("Warning: chpl_gen_comm_get_from_subloc called inside a GPU kernel. This shouldn't have happened.\n");
  // TODO
}

__device__ static inline
void chpl_gen_comm_put_to_subloc(void* addr,
                                 c_nodeid_t dst_node, c_sublocid_t dst_subloc,
                                 void* raddr, size_t size, int32_t commID,
                                 int ln, int32_t fn) {
  printf("Warning: chpl_gen_comm_get_from_subloc called inside a GPU kernel. This shouldn't have happened.\n");
  // TODO

}

__device__ static inline
void chpl_internal_error(const char* message) {
  printf("%s\n", message);
  // TODO actually error
}

__device__ extern int chpl_haltFlag;

__device__ static inline
void chpl_gpu_halt(int lineno, int32_t filenameIdx) {
  chpl_haltFlag = 1;
}

#endif // HAS_GPU_LOCALE

#endif // _CHPL_GPU_GEN_COMMON_H
