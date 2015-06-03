/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef _chpl_comm_compiler_llvm_support_h_
#define _chpl_comm_compiler_llvm_support_h_

#ifndef LAUNCHER

#include "chpl-comm.h"
#include "chpl-mem.h"
#include "error.h"
#include "chpl-wide-ptr-fns.h"


// These are used by the LLVM wide optimization.
// Some of these functions take in a ctl parameter - this parameter controls
// atomicity loads/stores. It is produced with
//   256*sync_scope + atomic_ordering
// where sync_scope = loadOrStoreInst->getSynchScope()
//       atomic_ordering = loadOrStoreInst->getOrdering()

static inline
void chpl_gen_comm_get_ctl(void *dst_addr, wide_ptr_t src, int64_t n, int64_t ctl)
{
  c_nodeid_t src_node = chpl_wide_ptr_get_node(src);
  void* src_addr = chpl_wide_ptr_get_address(src);
  chpl_gen_comm_get(dst_addr, src_node, src_addr, sizeof(uint8_t), CHPL_TYPE_uint8_t, n, -1, "");
}

static inline
void chpl_gen_comm_put_ctl(wide_ptr_t dst, void *src_addr, int64_t n, int64_t ctl)
{
  c_nodeid_t dst_node = chpl_wide_ptr_get_node(dst);
  void* dst_addr = chpl_wide_ptr_get_address(dst);
  chpl_gen_comm_put(src_addr, dst_node, dst_addr, sizeof(uint8_t), CHPL_TYPE_uint8_t, n, -1, "");
}

// This function implements memcpy/memmove when both the source and destination
// are possibly remote. The Chapel compiler does not normally generate
// such code, but it could appear during optimization. Note that the
// dst and src regions could overlap.
static inline
void chpl_gen_comm_getput(wide_ptr_t dst, wide_ptr_t src, int64_t n)
{
  c_nodeid_t src_node = chpl_wide_ptr_get_node(src);
  void* src_addr = chpl_wide_ptr_get_address(src);
  c_nodeid_t dst_node = chpl_wide_ptr_get_node(dst);
  void* dst_addr = chpl_wide_ptr_get_address(dst);

  if (chpl_nodeID == dst_node && chpl_nodeID == src_node) {
    memmove(dst_addr, src_addr, n);
  } else if( chpl_nodeID == dst_node ) {
    chpl_gen_comm_get_ctl(dst_addr, src, n, 0);
  } else if( chpl_nodeID == src_node ) {
    chpl_gen_comm_put_ctl(dst, src_addr, n, 0);
  } else {
    char buf[1024];
    int64_t chunk;
    int64_t i;
    for( i = 0; i < n; i += chunk) {
      chunk = n - i;
      if( chunk > sizeof(buf) ) chunk = sizeof(buf);
      chpl_gen_comm_get_ctl(buf, src, chunk, 0);
      chpl_gen_comm_put_ctl(dst, buf, chunk, 0);
      src = chpl_return_wide_ptr_add(src, chunk);
      dst = chpl_return_wide_ptr_add(dst, chunk);
    }
  }
}

static inline
void chpl_gen_comm_memset(wide_ptr_t dst, int8_t src, int64_t n)
{
  c_nodeid_t dst_node = chpl_wide_ptr_get_node(dst);
  void* dst_addr = chpl_wide_ptr_get_address(dst);

  if (chpl_nodeID == dst_node) {
    memset(dst_addr, src, n);
  } else {
    char buf[1024];
    int64_t chunk;
    int64_t i;
    size_t max = sizeof(buf);
    if( n < max ) max = n;
    memset(buf, src, max);

    for( i = 0; i < n; i += chunk) {
      chunk = n - i;
      if( chunk > sizeof(buf) ) chunk = sizeof(buf);
      chpl_gen_comm_put_ctl(dst, buf, chunk, 0);
      dst = chpl_return_wide_ptr_add(dst, chunk);
    }
  }
}

#endif // LAUNCHER

#endif
