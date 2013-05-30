//
// Shared code for different comm implementations in
//  comm/<commlayer>/comm-<commlayer>.c
//
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-mem.h"

#include <stdint.h>
#include <string.h>
#include <unistd.h>

int32_t chpl_nodeID = -1;
int32_t chpl_numNodes = -1;
int32_t chpl_numPrivateObjects = 0;
static int32_t chpl_capPrivateObjects = 0;
void** chpl_privateObjects;

int chpl_verbose_comm;
int chpl_comm_diagnostics;
int chpl_verbose_mem;

void chpl_newPrivatizedClass(void* v) {
  chpl_numPrivateObjects += 1;
  if (chpl_numPrivateObjects == 1) {
    chpl_capPrivateObjects = 8;
    chpl_privateObjects = chpl_mem_allocMany(chpl_capPrivateObjects, sizeof(void*), CHPL_RT_MD_COMM_PRIVATE_OBJECTS_ARRAY, 0, "");
  } else {
    if (chpl_numPrivateObjects > chpl_capPrivateObjects) {
      void** tmp;
      chpl_capPrivateObjects *= 2;
      tmp = chpl_mem_allocMany(chpl_capPrivateObjects, sizeof(void*), CHPL_RT_MD_COMM_PRIVATE_OBJECTS_ARRAY, 0, "");
      memcpy((void*)tmp, (void*)chpl_privateObjects, (chpl_numPrivateObjects-1)*sizeof(void*));
      chpl_privateObjects = tmp;
      // purposely leak old copies of chpl_privateObject to avoid the need to
      // lock chpl_getPrivatizedClass; to do: fix with lock free data structure
    }
  }
  chpl_privateObjects[chpl_numPrivateObjects-1] = v;
}

extern void* chpl_getPrivatizedClass(int32_t i) {
  return chpl_privateObjects[i];
}

#include "chpl-wide-ptr-fns.h"

// These are used by the LLVM wide optimization.
// They need to be symbols in order to prevent LLVM optimizations from
// deciding that they are unused code and deleting them before we add
// calls to them late in the optimization.

// Some of these functions take in a ctl parameter - this parameter controls
// atomicity loads/stores. It is produced with
//   256*sync_scope + atomic_ordering
// where sync_scope = loadOrStoreInst->getSynchScope()
//       atomic_ordering = loadOrStoreInst->getOrdering()

// Calls chpl_gen_comm_get. It the future, it should respect the atomicity flag.
void chpl_gen_comm_get_ctl_sym(void *dst_addr, wide_ptr_t src, int64_t n, int64_t ctl);
// Calls chpl_gen_comm_put. It the future, it should respect the atomicity flag.
void chpl_gen_comm_put_ctl_sym(wide_ptr_t dst, void *src_addr, int64_t n, int64_t ctl);
// Implements memcpy by calling chpl_gen_comm_get and chpl_gen_comm_put.
void chpl_gen_comm_getput_sym(wide_ptr_t dst, wide_ptr_t src, int64_t n);
// Implements memset by repeatedly using chpl_gen_comm_put with zeros.
void chpl_gen_comm_memset_sym(wide_ptr_t dst, int8_t src, int64_t n);

void chpl_gen_comm_get_ctl_sym(void *dst_addr, wide_ptr_t src, int64_t n, int64_t ctl)
{
  c_nodeid_t src_node = chpl_wide_ptr_get_node(src);
  void* src_addr = chpl_wide_ptr_get_address(src);
  if (chpl_nodeID == src_node) {
    memcpy(dst_addr, src_addr, n);
  } else {
#ifdef CHPL_TASK_COMM_GET
    chpl_task_comm_get(dst_addr, src_node, src_addr, sizeof(uint8_t), CHPL_TYPE_uint8_t, n, -1, "");
#else
    chpl_comm_get(dst_addr, src_node, src_addr, sizeof(uint8_t), CHPL_TYPE_uint8_t, n, -1, "");
#endif
  }
}

void chpl_gen_comm_put_ctl_sym(wide_ptr_t dst, void *src_addr, int64_t n, int64_t ctl)
{
  c_nodeid_t dst_node = chpl_wide_ptr_get_node(dst);
  void* dst_addr = chpl_wide_ptr_get_address(dst);
  if (chpl_nodeID == dst_node) {
    memcpy(dst_addr, src_addr, n);
  } else {
#ifdef CHPL_TASK_COMM_PUT
    chpl_task_comm_put(src_addr, dst_node, dst_addr, sizeof(uint8_t), CHPL_TYPE_uint8_t, n, -1, "");
#else
    chpl_comm_put(src_addr, dst_node, dst_addr, sizeof(uint8_t), CHPL_TYPE_uint8_t, n, -1, "");
#endif
  }
}

void chpl_gen_comm_getput_sym(wide_ptr_t dst, wide_ptr_t src, int64_t n)
{
  c_nodeid_t src_node = chpl_wide_ptr_get_node(src);
  void* src_addr = chpl_wide_ptr_get_address(src);
  c_nodeid_t dst_node = chpl_wide_ptr_get_node(dst);
  void* dst_addr = chpl_wide_ptr_get_address(dst);

  if (chpl_nodeID == dst_node && chpl_nodeID == src_node) {
    memcpy(dst_addr, src_addr, n);
  } else if( chpl_nodeID == dst_node ) {
    chpl_gen_comm_get_ctl_sym(dst_addr, src, n, 0);
  } else if( chpl_nodeID == src_node ) {
    chpl_gen_comm_put_ctl_sym(dst, src_addr, n, 0);
  } else {
    char buf[1024];
    int64_t chunk;
    int64_t i;
    for( i = 0; i < n; i += chunk) {
      chunk = n - i;
      if( chunk > sizeof(buf) ) chunk = sizeof(buf);
      chpl_gen_comm_get_ctl_sym(buf, src, chunk, 0);
      chpl_gen_comm_put_ctl_sym(dst, buf, chunk, 0);
      src = chpl_return_wide_ptr_add(src, chunk);
      dst = chpl_return_wide_ptr_add(dst, chunk);
    }
  }
}


void chpl_gen_comm_memset_sym(wide_ptr_t dst, int8_t src, int64_t n)
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
      chpl_gen_comm_put_ctl_sym(dst, buf, chunk, 0);
      dst = chpl_return_wide_ptr_add(dst, chunk);
    }
  }
}

