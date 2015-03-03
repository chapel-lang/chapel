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

#ifndef _chpl_comm_compiler_macros_h_
#define _chpl_comm_compiler_macros_h_

#ifndef LAUNCHER

#include "chpl-comm.h"
#include "chpl-mem.h"
#include "error.h"
#include "chpl-wide-ptr-fns.h"

#include "chpl-prefetch.h" // for chpl_prefetch
#include "chpl-cache.h" // chpl_cache_enabled, chpl_cache_comm_get etc

// Don't warn about chpl_comm_get e.g. in this file.
#include "chpl-comm-no-warning-macros.h"

//
// Multi-locale macros used for compiler code generation
//
// Note: Macros starting with CHPL_COMM involve some kind of communication
//


static ___always_inline
void chpl_gen_comm_get(void *addr, c_nodeid_t node, void* raddr,
                       int32_t elemSize, int32_t typeIndex, int32_t len,
                       int ln, c_string fn)
{
  if (chpl_nodeID == node) {
    chpl_memcpy(addr, raddr, elemSize*len);
#ifdef HAS_CHPL_CACHE_FNS
  } else if( chpl_cache_enabled() ) {
    chpl_cache_comm_get(addr, node, raddr, elemSize, typeIndex, len, ln, fn);
#endif
  } else {
#ifdef CHPL_TASK_COMM_GET
    chpl_task_comm_get(addr, node, raddr, elemSize, typeIndex, len, ln, fn);
#else
    chpl_comm_get(addr, node, raddr, elemSize, typeIndex, len, ln, fn);
#endif
  }
}

static ___always_inline
void chpl_gen_comm_prefetch(c_nodeid_t node, void* raddr,
                            int32_t elemSize, int32_t typeIndex, int32_t len,
                            int ln, c_string fn)
{
  const int32_t MAX_BYTES_LOCAL_PREFETCH = 1024;
  int32_t offset;
  int32_t size = elemSize*len;

  if (chpl_nodeID == node) {
    // Prefetch only the first part since we don't want to blow
    // out a cache...
    for( offset = 0;
         offset < size && offset < MAX_BYTES_LOCAL_PREFETCH;
         offset += 64 ) {
      chpl_prefetch((unsigned char*)raddr + offset);
    }
#ifdef HAS_CHPL_CACHE_FNS
  } else if( chpl_cache_enabled() ) {
    chpl_cache_comm_prefetch(node, raddr, elemSize, typeIndex, len, ln, fn);
#endif
  } else {
    // Can't do anything if we don't have a remote data cache
    // and the data is remote.
  }
}


static ___always_inline
void chpl_gen_comm_put(void* addr, c_nodeid_t node, void* raddr,
                       int32_t elemSize, int32_t typeIndex, int32_t len,
                       int ln, c_string fn)
{
  if (chpl_nodeID == node) {
    chpl_memcpy(raddr, addr, elemSize*len);
#ifdef HAS_CHPL_CACHE_FNS
  } else if( chpl_cache_enabled() ) {
    chpl_cache_comm_put(addr, node, raddr, elemSize, typeIndex, len, ln, fn);
#endif
  } else {
#ifdef CHPL_TASK_COMM_PUT
    chpl_task_comm_put(addr, node, raddr, elemSize, typeIndex, len, ln, fn);
#else
    chpl_comm_put(addr, node, raddr, elemSize, typeIndex, len, ln, fn);
#endif
  }
}

static ___always_inline
void chpl_gen_comm_get_strd(void *addr, void *dststr, c_nodeid_t node, void *raddr,
                       void *srcstr, void *count, int32_t strlevels, 
                       int32_t elemSize, int32_t typeIndex,
                       int ln, c_string fn)
{
  if( 0 ) {
#ifdef HAS_CHPL_CACHE_FNS
  } else if( chpl_cache_enabled() ) {
    chpl_cache_comm_get_strd(addr, dststr, node, raddr, srcstr, count, strlevels, elemSize, typeIndex, ln, fn);
#endif
  } else {
#ifdef CHPL_TASK_COMM_GET_STRD
  chpl_task_comm_get_strd(addr, dststr, node, raddr, srcstr, count, strlevels, elemSize, typeIndex, ln, fn);
#else
  chpl_comm_get_strd(addr, dststr, node, raddr, srcstr, count, strlevels, elemSize, typeIndex, ln, fn);
#endif
  }
}

static ___always_inline
void chpl_gen_comm_put_strd(void *addr, void *dststr, c_nodeid_t node, void *raddr,
                       void *srcstr, void *count, int32_t strlevels, 
                       int32_t elemSize, int32_t typeIndex,
                       int ln, c_string fn)
{
  if( 0 ) {
#ifdef HAS_CHPL_CACHE_FNS
  } else if( chpl_cache_enabled() ) {
    chpl_cache_comm_put_strd(addr, dststr, node, raddr, srcstr, count, strlevels, elemSize, typeIndex, ln, fn);
#endif
  } else {
#ifdef CHPL_TASK_COMM_PUT_STRD
  chpl_task_comm_put_strd(addr, dststr, node, raddr, srcstr, count, strlevels, elemSize, typeIndex, ln, fn);
#else
  chpl_comm_put_strd(addr, dststr, node, raddr, srcstr, count, strlevels, elemSize, typeIndex, ln, fn);
#endif
  }
}

// Returns true if the given node ID matches the ID of the currently node,
// false otherwise.
static ___always_inline
chpl_bool chpl_is_node_local(c_nodeid_t node)
{ return node == chpl_nodeID; }

// Assert that the given node ID matches that of the currently-running image
// If not, format the given error message with the given filename and line number
// and then halt the current task.  (The exact behavior is dictated by 
// chpl_error()).
static ___always_inline
void chpl_check_local(c_nodeid_t node, int32_t ln, const char* file, const char* error)
{
  if (! chpl_is_node_local(node))
    chpl_error(error, ln, file);
}

static ___always_inline
void chpl_heap_register_global_var(int i, wide_ptr_t *ptr_to_wide_ptr)
{
  chpl_globals_registry[i] = ptr_to_wide_ptr;
  CHPL_HEAP_REGISTER_GLOBAL_VAR_EXTRA(i, *ptr_to_wide_ptr);
}

#ifdef DEBUG_COMM_INIT
#define CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals) \
  for (int i = 0; i < numGlobals; i++) \
    printf("[%d] chpl_globals_registry[%d] = %p\n", chpl_nodeID, i, *((void **)chpl_globals_registry[i]));
#else
#define CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals) ;
#endif

static ___always_inline
void chpl_gen_comm_broadcast_global_vars(int numGlobals)
{
  chpl_comm_barrier("barrier before broadcasting globals"); 
  chpl_comm_broadcast_global_vars(numGlobals);
  CHPL_COMM_DEBUG_BROADCAST_GLOBAL_VARS(numGlobals);
  chpl_comm_barrier("barrier after broadcasting globals");
}

static ___always_inline
void chpl_check_nil(void* ptr, int32_t lineno, const char* filename)
{
  if (ptr == nil)
    chpl_error("attempt to dereference nil", lineno, filename);
}

static ___always_inline
void* chpl_array_alloc(size_t nmemb, size_t eltSize, int32_t lineno, const char* filename) {
  return chpl_mem_allocManyZero(nmemb, eltSize, CHPL_RT_MD_ARRAY_ELEMENTS, lineno, filename);
}

static ___always_inline
void* chpl_wide_array_alloc(int32_t dstNode, size_t nmemb, size_t eltSize, int32_t lineno, const char* filename) {
  if (dstNode != chpl_nodeID)
    chpl_error("array vector data is not local", lineno, filename);;
  return chpl_array_alloc(nmemb, eltSize, lineno, filename);
}

static ___always_inline
void chpl_array_free(void* x, int32_t lineno, const char* filename)
{
  chpl_mem_free(x, lineno, filename);
}

static ___always_inline
void chpl_wide_array_free(int32_t dstNode, void* x, int32_t lineno, const char* filename)
{
  if (dstNode != chpl_nodeID)
    chpl_error("array vector data is not local", lineno, filename);
  chpl_array_free(x, lineno, filename);
}

// Include LLVM support functions for --llvm-wide-opt
#include "chpl-comm-compiler-llvm-support.h"

#endif // LAUNCHER

// Turn warnings for chpl_comm_get back on
#include "chpl-comm-warning-macros.h"

#endif
