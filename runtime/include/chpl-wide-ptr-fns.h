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

#ifndef _chpl_wide_ptr_impl_h_
#define _chpl_wide_ptr_impl_h_

#include "chpltypes.h"

// chpl_error needed for some of the packed wide pointer error cases.
#include "error.h"

// Methods for working with wide pointers and localeID_t.
// There are several variants of many of these in order to simplify
// different code generation tasks. The versions that take in a pointer
// to a wide_ptr_t generally do so in order to easily be used with
// a variety of wide pointer types with the structure representation,
// since one can't cast a structure...

static inline
wide_ptr_t chpl_return_wide_ptr_node(c_nodeid_t node, void* addr)
{
  wide_ptr_t dst;
  dst.locale = chpl_rt_buildLocaleID(node, 0);
  dst.addr = addr;
  return dst;
}

static inline
void chpl_check_wide_ptr(wide_ptr_t ptr)
{
  //if( chpl_rt_nodeFromLocaleID(ptr.locale) < 0 )
  //  chpl_internal_error("Bad wide pointer");
}

static inline
wide_ptr_t chpl_return_wide_ptr_loc(chpl_localeID_t loc, void * addr)
{
  wide_ptr_t dst;
  dst.locale = loc;
  dst.addr = addr;
  return dst;
}

static inline
wide_ptr_t chpl_return_wide_ptr_loc_ptr(const chpl_localeID_t* loc, void * addr)
{
  return chpl_return_wide_ptr_loc(*loc, addr);
}


static inline
c_nodeid_t chpl_wide_ptr_get_node(wide_ptr_t ptr)
{
  chpl_check_wide_ptr(ptr);
  return chpl_rt_nodeFromLocaleID(ptr.locale);
}

static inline
void* chpl_wide_ptr_get_address(wide_ptr_t ptr)
{
  chpl_check_wide_ptr(ptr);
  return ptr.addr;
}

static inline
chpl_localeID_t chpl_wide_ptr_get_localeID(wide_ptr_t ptr)
{
  chpl_localeID_t loc;
  chpl_check_wide_ptr(ptr);
  loc = ptr.locale;
  return loc;
}

static inline
void chpl_wide_ptr_read_localeID(wide_ptr_t ptr,
                                 chpl_localeID_t* loc)
{
  *loc = chpl_wide_ptr_get_localeID(ptr);
}


static inline
wide_ptr_t chpl_return_wide_ptr_add(wide_ptr_t ptr, size_t amt)
{
  ptr.addr = (void*) (((unsigned char*)ptr.addr) + amt);
  return ptr;
}


#endif
