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

#include "chplrt.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chplmemtrack.h"
#include "chpltypes.h"
#include "error.h"
#include "dlmalloc.h"

mspace chpl_dlmalloc_heap;


void chpl_mem_layerInit(void) {
  void*  heap_base;
  size_t heap_size;

  chpl_comm_desired_shared_heap(&heap_base, &heap_size);
  if (heap_base == NULL || heap_size == 0)
    chpl_dlmalloc_heap = create_mspace(0, 1);
  else
    chpl_dlmalloc_heap = create_mspace_with_base(heap_base, heap_size, 1);
}


void chpl_mem_layerExit(void) { }


