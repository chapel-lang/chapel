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

#ifndef _chpl_prefetch_h_
#define _chpl_prefetch_h_

#include "chpl-comp-detect-macros.h"

static inline void chpl_prefetch(void* addr) {
#if (RT_COMP_CC & (~RT_COMP_PGI))
  // cray, intel, gcc, clang
  __builtin_prefetch(addr);
#elif (RT_COMP_CC & RT_COMP_PGI)
  // PGI seems to only support prefetch through a pragma. The dereference is
  // necessary. This does not do the exact the same thing as the other
  // compilers, but I'm not certain how to get perfectly equivalent
  // functionality outside of emitting the asm myself.
  #pragma mem prefetch *((char *)addr)
#else
  // Unknown compiler, we don't know how to generate a prefetch, just NOOP
  // It's not that much of a difference anyways
  (void)0;
#endif
}

#endif // _chpl_prefetch_h_
