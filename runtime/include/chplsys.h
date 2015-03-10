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

#ifndef _CHPL_SYS_H_
#define _CHPL_SYS_H_

#include "chpltypes.h" // for c_string

#include <stdint.h>

size_t chpl_getSysPageSize(void);
size_t chpl_getHeapPageSize(void);
uint64_t chpl_bytesPerLocale(void);
size_t chpl_bytesAvailOnThisLocale(void);
int chpl_getNumPhysicalCpus(chpl_bool accessible_only);
int chpl_getNumLogicalCpus(chpl_bool accessible_only);

//
// returns the name of a locale via uname -n or the like
//
c_string chpl_nodeName(void);

#endif
