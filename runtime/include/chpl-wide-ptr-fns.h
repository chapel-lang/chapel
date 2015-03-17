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

#ifndef _chpl_wide_ptr_impl_h_
#define _chpl_wide_ptr_impl_h_

// Make sure that chpltypes will define a wide pointer type.
#ifdef CHPL_WIDE_POINTER_STRUCT
#else

#ifdef CHPL_WIDE_POINTER_PACKED
#else
#error missing either CHPL_WIDE_POINTER_STRUCT or CHPL_WIDE_POINTER_PACKED
#endif

#endif


#include "chpltypes.h"

// chpl_error needed for some of the packed wide pointer error cases.
#include "error.h"

#define WIDE_PTR_INLINE inline

// Methods for working with wide pointers (packed or not) and localeID_t.
// There are several variants of many of these in order to simplify
// different code generation tasks. The versions that take in a pointer
// to a wide_ptr_t generally do so in order to easily be used with
// a variety of wide pointer types with the structure representation,
// since one can't cast a structure...

#ifdef CHPL_WIDE_POINTER_PACKED
  #define CHPL_WIDE_PTR_MARK_BITS 1
  #define CHPL_WIDE_PTR_MARK 1

  #define CHPL_PTR_BITS (64-CHPL_WIDE_PTR_MARK_BITS-CHPL_WIDE_POINTER_NODE_BITS)

  #define CHPL_NODE_MASK ( (1ULL << CHPL_WIDE_POINTER_NODE_BITS) - 1ULL)
  #define CHPL_PTR_MASK ( (1ULL << CHPL_PTR_BITS) - 1ULL)
#endif

static WIDE_PTR_INLINE
wide_ptr_t chpl_return_wide_ptr_node(c_nodeid_t node, void* addr)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  wide_ptr_t dst;
  dst.locale = chpl_rt_buildLocaleID(node, 0);
  dst.addr = addr;
  return dst;
#else
  uint64_t uptr = (uint64_t) addr;
  uint64_t unode = node;
  uint64_t mark = CHPL_WIDE_PTR_MARK;
  uint64_t ret;
  if( (uptr & CHPL_PTR_MASK) != uptr ) {
    chpl_internal_error("Local pointer too big to fit into wide pointer");
  }
  if( (unode & CHPL_NODE_MASK) != unode) {
    chpl_internal_error("Node ID too big to fit into wide pointer");
  }
  ret = uptr;
  ret |= unode << CHPL_PTR_BITS;
  ret |= mark << (CHPL_PTR_BITS+CHPL_WIDE_POINTER_NODE_BITS);
  return (wide_ptr_t) ret;
#endif
}

static WIDE_PTR_INLINE
void chpl_check_wide_ptr(wide_ptr_t ptr)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  if( chpl_rt_nodeFromLocaleID(ptr.locale) < 0 )
    chpl_internal_error("Bad wide pointer");
#else
  uint64_t uptr = (uint64_t) ptr;
  uint64_t mark = uptr >> (CHPL_PTR_BITS+CHPL_WIDE_POINTER_NODE_BITS);
  if( uptr == 0 ) return; // NULL is always OK
  if( mark != CHPL_WIDE_PTR_MARK )
    chpl_internal_error("Bad wide pointer");
#endif
}

static WIDE_PTR_INLINE
wide_ptr_t chpl_return_wide_ptr_loc(chpl_localeID_t loc, void * addr)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  wide_ptr_t dst;
  dst.locale = loc;
  dst.addr = addr;
  return dst;
#else
  // packed wide pointers do not store sublocale,
  // so we just throw that info away
  // and reconstruct it later from the local address.
  return chpl_return_wide_ptr_node(chpl_rt_nodeFromLocaleID(loc), addr);
#endif
}

static WIDE_PTR_INLINE
wide_ptr_t chpl_return_wide_ptr_loc_ptr(const chpl_localeID_t* loc, void * addr)
{
  return chpl_return_wide_ptr_loc(*loc, addr);
}


static WIDE_PTR_INLINE
c_nodeid_t chpl_wide_ptr_get_node(wide_ptr_t ptr)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  chpl_check_wide_ptr(ptr);
  return chpl_rt_nodeFromLocaleID(ptr.locale);
#else
  uint64_t uptr = (uint64_t) ptr;
  chpl_check_wide_ptr(ptr);
  return (uptr >> CHPL_PTR_BITS) & CHPL_NODE_MASK;
#endif
}

static WIDE_PTR_INLINE
void* chpl_wide_ptr_get_address(wide_ptr_t ptr)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  chpl_check_wide_ptr(ptr);
  return ptr.addr;
#else
  uint64_t uptr = (uint64_t) ptr;
  chpl_check_wide_ptr(ptr);
  uptr &= CHPL_PTR_MASK;
  return (void*) uptr;
#endif
}

static WIDE_PTR_INLINE
chpl_localeID_t chpl_wide_ptr_get_localeID(wide_ptr_t ptr)
{
  chpl_localeID_t loc;
  chpl_check_wide_ptr(ptr);
#ifndef CHPL_WIDE_POINTER_PACKED
  loc = ptr.locale;
#else
  // packed wide pointers do not store sublocale
  loc = chpl_rt_buildLocaleID(chpl_wide_ptr_get_node(ptr), 0);
#endif
  return loc;
}

static WIDE_PTR_INLINE
void chpl_wide_ptr_read_localeID(wide_ptr_t ptr,
                                 chpl_localeID_t* loc)
{
  *loc = chpl_wide_ptr_get_localeID(ptr);
}


static WIDE_PTR_INLINE
wide_ptr_t chpl_return_wide_ptr_add(wide_ptr_t ptr, size_t amt)
{
#ifndef CHPL_WIDE_POINTER_PACKED
  ptr.addr = (void*) (((unsigned char*)ptr.addr) + amt);
  return ptr;
#else
  return (wide_ptr_t) (((unsigned char*)ptr) + amt);
#endif
}


#endif
