/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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
// See chpl-init.c for details on the initialization functions
//

// accessiblePUsMask masks the accessible PUs and is used by runtime
// unit testing. It should be NULL in production code.

void chpl_topo_pre_comm_init(char *accessiblePUsMask);
void chpl_topo_post_comm_init(void);
void chpl_topo_post_args_init(void);
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
// Fills the "cpus" array with up to "count" physical OS indices of the
// accessible cores or PUs. If "physical" is true, then "cpus" contains
// core indices, otherwise it contains PU indices. Returns the number
// of indices in the "cpus" array.
//
int chpl_topo_getCPUs(chpl_bool physical, int *cpus, int count);

//
// Reserves a physical CPU (core) and returns its hwloc OS index. The
// core and its PUs will not be returned by chpl_topo_getCPUs,
// chpl_topo_getNumCPUsPhysical, and chpl_topo_getNumCPUsLogical. Must
// be called before those functions. Will not reserve a core if CPU
// binding is not supported on this platform or if there is only one
// unreserved core.
//
// Returns -1 if the reservation failed.
//
int chpl_topo_reserveCPUPhysical(void);

// Binds the current thread to the PU specified by the hwloc OS index.
// The index must have previously been returned by
// chpl_topo_reserveCPUPhysical.
//
// Returns 0 on success, 1 otherwise
//
int chpl_topo_bindCPU(int id);

// Binds the current thread to the accessible logical CPUs (PUs). This
// restricts the thread to the locale's PUs (i.e., the progress thread should
// use the same PUs as the locale).
//
// Returns 0 on success, 1 otherwise
//
int chpl_topo_bindLogAccCPUs(void);

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
// Set the locality of a block of memory to interleave (round-robin) between
// NUMA domains. This may only set the policy and does not necessarily fault
// memory in
//
void chpl_topo_interleaveMemLocality (void*, size_t);

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

typedef struct chpl_topo_pci_addr {
    uint16_t domain;
    uint8_t bus;
    uint8_t device;
    uint8_t function;
} chpl_topo_pci_addr_t;

#define CHPL_TOPO_PCI_ADDR_EQUAL(a, b) \
    (((a)->domain == (b)->domain) && \
     ((a)->bus == (b)->bus) && \
     ((a)->device == (b)->device) && \
     ((a)->function == (b)->function))

chpl_topo_pci_addr_t *chpl_topo_selectNicByType(chpl_topo_pci_addr_t *inAddr,
                                                chpl_topo_pci_addr_t *outAddr);

int chpl_topo_selectMyDevices(chpl_topo_pci_addr_t *inAddrs,
                              chpl_topo_pci_addr_t *outAddrs, int *count);
//
// Returns True if the node is oversubscribed (locales are sharing
// cores).
chpl_bool chpl_topo_isOversubscribed(void);

#ifdef __cplusplus
} // end extern "C"
#endif

#endif
