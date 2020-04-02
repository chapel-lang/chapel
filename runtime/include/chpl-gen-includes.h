/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

// chpl-gen-includes.h
//
// Inline functions used in code generation.
//
// TODO: Check if any of these are redundant with functions in chpl-wide-ptr-fns.h.
//

#include "chpl-comm-compiler-macros.h"
#include "chplcgfns.h"
#include "chpl-locale-model.h"
#include "chpl-tasks.h"
#include "chpltypes.h"

//
// Call a function in the compiler-produced function table, passing it
// one argument.
//
static inline
void chpl_ftable_call(chpl_fn_int_t fid, void* bundle)
{
  (*chpl_ftable[fid])(bundle);
}


// used for converting between the Chapel idea of a locale ID: chpl_localeID_t
// and the runtime idea of a locale ID: c_localeid_t.
static inline
c_localeid_t id_pub2rt(chpl_localeID_t s)
{
  return
    ((c_localeid_t) chpl_rt_nodeFromLocaleID(s) << 32) |
    ((c_localeid_t) chpl_rt_sublocFromLocaleID(s) & 0xffffffff);
}

static inline
chpl_localeID_t id_rt2pub(c_localeid_t i)
{
  return chpl_rt_buildLocaleID(i >> 32, i & 0xffffffff);
}
extern void chpl_getLocaleID (chpl_localeID_t* localeID,  int64_t _ln, int32_t _fn);
static inline
chpl_localeID_t chpl_gen_getLocaleID(void)
{
  chpl_localeID_t localeID;
  chpl_getLocaleID(&localeID, 0, 0);
  return localeID;
}
