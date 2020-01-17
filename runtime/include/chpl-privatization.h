/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _chpl_privatization_h_
#define _chpl_privatization_h_
#ifndef LAUNCHER
#include <stdint.h>
#include "chpltypes.h"

void chpl_privatization_init(void);

void chpl_newPrivatizedClass(void*, int64_t);

typedef struct chpl_privateObject_s {
  void* obj;
} chpl_privateObject_t;

// This is a dynamically-allocated array of chpl_privateObject_t.
extern chpl_privateObject_t* chpl_privateObjects;

// Compiler generates accesses like chpl_privateObjects[i];
// see chpl_getPrivatizedCopy.
// This allows TBAA information for chpl_privateObjects to be used.
// At the very least, inlining it would be important for performance.

void chpl_clearPrivatizedClass(int64_t);

int64_t chpl_numPrivatizedClasses(void);

#endif // LAUNCHER
#endif // _chpl_privatization_h_
