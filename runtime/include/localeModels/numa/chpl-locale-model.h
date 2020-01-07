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

#ifndef _chpl_locale_model_h_
#define _chpl_locale_model_h_

#include "sys_basic.h"
#include "chpltypes.h"

//
// This is the type of a global locale ID.
//
typedef struct {
  int32_t node;
  int32_t subloc;
} chpl_localeID_t;

//
// This is the initializer for a chpl_localeID_t.  This macro is
// referenced explicitly in the compiler, in symbol.cpp.
//
#define CHPL_LOCALEID_T_INIT  {0, 0}

//
// This is the external copy constructor for a chpl_localeID_t, specified
// by the module code for a numa locale model.
//
static inline
chpl_localeID_t chpl__initCopy_chpl_rt_localeID_t(chpl_localeID_t initial) {
  return initial;
}

//
// These functions are used by the module code to assemble and
// disassemble global locale IDs.
//
static inline
chpl_localeID_t chpl_rt_buildLocaleID(c_nodeid_t node, c_sublocid_t subloc) {
  chpl_localeID_t loc = { node, subloc };
  return loc;
}

static inline
c_nodeid_t chpl_rt_nodeFromLocaleID(chpl_localeID_t loc) {
  return loc.node;
}

static inline
c_sublocid_t chpl_rt_sublocFromLocaleID(chpl_localeID_t loc) {
  return loc.subloc;
}

//
// These functions are exported from the locale model for use by
// the tasking layer to convert between a full sublocale and an
// execution sublocale.
//
extern
c_sublocid_t chpl_localeModel_sublocToExecutionSubloc(
                  c_sublocid_t full_subloc);

extern
c_sublocid_t chpl_localeModel_sublocMerge(c_sublocid_t full_subloc,
                  c_sublocid_t execution_subloc);

#endif // _chpl_locale_model_h_
