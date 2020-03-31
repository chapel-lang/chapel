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

#ifndef _chpl_topo_h_
#define _chpl_topo_h_


#include "chpltypes.h"

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


//
// chpl_topo_init() must be called before anything else here, to
// initialize the topology support
//
void chpl_topo_init(void);
void chpl_topo_exit(void);

//
// retrieve the hwloc topology (if any) for other packages to use
//
void* chpl_topo_getHwlocTopology(void);

//
// How many CPUs are there?
//
int chpl_topo_getNumCPUsPhysical(chpl_bool /*accessible_only*/);
int chpl_topo_getNumCPUsLogical(chpl_bool /*accessible_only*/);

//
// how many NUMA domains are there?
//
int chpl_topo_getNumNumaDomains(void);

//
// set the sublocale where the current thread is running
//
void chpl_topo_setThreadLocality(c_sublocid_t);

//
// get the sublocale where the current thread is running
//
c_sublocid_t chpl_topo_getThreadLocality(void);

//
// set the locality of a block of memory, to a specific NUMA domain
//
// args:
//   base address
//   size (bytes)
//   onlyInside?  true: only localize pages strictly within the memory
//                false: also localize partial pages at edges
//   desired sublocale (NUMA domain)
//
void chpl_topo_setMemLocality(void*, size_t, chpl_bool, c_sublocid_t);

//
// set the locality of the sub-blocks of a block of memory, to each
// of the NUMA domains in order
//
// args:
//   base address
//   size (bytes)
//   onlyInside?  true: only localize pages strictly within the memory
//                false: also localize partial pages at edges
//   (optional) address of result vector of subchunk sizes
//
void chpl_topo_setMemSubchunkLocality(void*, size_t, chpl_bool, size_t*);

//
// touch a block of memory, while running on a given NUMA domain
//
// args:
//   base address
//   size (bytes)
//   onlyInside?  true: only localize pages strictly within the memory
//                false: also localize partial pages at edges
//   desired sublocale (NUMA domain)
//
void chpl_topo_touchMemFromSubloc(void*, size_t, chpl_bool, c_sublocid_t);

//
// get memory locality of (the page containing) an address
//
// args:
//   address
//
c_sublocid_t chpl_topo_getMemLocality(void*);


#ifdef __cplusplus
} // end extern "C"
#endif

#endif
