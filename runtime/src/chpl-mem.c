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
// Shared code for different mem implementations in mem-*/chpl_*_mem.c
//
#include "chplrt.h"

#include "chpl-mem.h"
#include "chpltypes.h"
#include "error.h"
#include "chplsys.h"

static int heapInitialized = 0;


void chpl_mem_init(void) {
  chpl_mem_layerInit();
  heapInitialized = 1;
}


void chpl_mem_exit(void) {
  chpl_mem_layerExit();
}


int chpl_mem_inited(void) {
  return heapInitialized;
}

int chpl_posix_memalign_check_valid(size_t alignment) {
  size_t tmp;
  int power;
  size_t one = 1;

  // return EINVAL if alignment not a multiple of sizeof(void*)
  tmp = alignment / sizeof(void*);
  if( alignment != tmp * sizeof(void*) ) {
    return EINVAL;
  }

  // return EINVAL if alignment not a power of 2

  // find the power of 2 that is alignment
  for( power = 0; 
       power < 8*sizeof(size_t);
       power++ ) {
    // find power of two equal to alignment.
    if( alignment == (one << power) ) break;
  }
  // return EINVAL if not a power of two.
  if( power == 8*sizeof(size_t) || alignment != (one << power) ) {
    return EINVAL; 
  }

  return 0;
}


int chpl_posix_memalign(void** ptr, size_t alignment, size_t size) {
  void* allocated;
  int err;

  *ptr = NULL;

  err = chpl_posix_memalign_check_valid(alignment);
  if( err ) return err;

  // otherwise, allocate the pointer and return 0 or ENOMEM if it failed.
  allocated = chpl_memalign(alignment, size);

  if( ! allocated ) return ENOMEM;

  *ptr = allocated;
  return 0;
}

void* chpl_valloc(size_t size)
{
  return chpl_memalign(chpl_getHeapPageSize(), size);
}

void* chpl_pvalloc(size_t size)
{
  size_t page_size = chpl_getHeapPageSize();
  size_t num_pages = (size + page_size - 1) / page_size;
  size_t rounded_up = num_pages * page_size; 
  return chpl_memalign(chpl_getHeapPageSize(), rounded_up);
}


