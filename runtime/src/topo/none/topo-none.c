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

//
// Compute node topology support: vacuous implementation
//
#include "chplrt.h"

#include "chplsys.h"
#include "chpl-topo.h"
#include "chpltypes.h"
#include "error.h"

#include <stdint.h>


void chpl_topo_init(void) { }


void chpl_topo_exit(void) { }


void* chpl_topo_getHwlocTopology(void) {
  return NULL;
}


int chpl_topo_getNumCPUsPhysical(chpl_bool accessible_only) {
  return chpl_sys_getNumCPUsPhysical(accessible_only);
}


int chpl_topo_getNumCPUsLogical(chpl_bool accessible_only) {
  return chpl_sys_getNumCPUsLogical(accessible_only);
}


int chpl_topo_getNumNumaDomains(void) {
  return 1;
}


void chpl_topo_setThreadLocality(c_sublocid_t subloc) { }


c_sublocid_t chpl_topo_getThreadLocality(void) {
  return c_sublocid_any;
}


void chpl_topo_setMemLocality(void* p, size_t size, chpl_bool onlyInside,
                              c_sublocid_t subloc) { }


void chpl_topo_setMemSubchunkLocality(void* p, size_t size,
                                      chpl_bool onlyInside,
                                      size_t* subchunkSizes) { }


void chpl_topo_touchMemFromSubloc(void* p, size_t size, chpl_bool onlyInside,
                                  c_sublocid_t subloc) { }


c_sublocid_t chpl_topo_getMemLocality(void* p) {
  return c_sublocid_any;
}
