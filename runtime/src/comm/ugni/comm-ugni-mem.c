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

//
// Memory management for the ugni comm layer.
//

#include <stdint.h>
#include <sys/types.h>

#include <hugetlbfs.h>  // <sys/types.h> must come first

#include "chplrt.h"

#include "chpl-comm.h"
#include "chpl-mem.h"
#include "comm-ugni-mem.h"


//
// NIC-registered memory management.
//
// The following functions manage a pool of memory that is guaranteed
// to be registered with the NIC.  Some NIC operations require that
// the referred-to memory be registered with the NIC, and some ugni
// comm layer functionality depends on such NIC operations.  As an
// example, when user code does a blocking executeOn (fork), the comm
// layer on the initiating node needs a "done" flag that the target
// node can PUT a value into to indicate that the remote executeOn has
// completed. Aries PUT operations require that the receiving
// address on the remote node be NIC-registered.  Thus the flag may
// reside on the initiator's call stack if that is NIC-registered, but
// if it isn't we need to allocate the flag dynamically.  We can't be
// sure that a heap allocation will always produce registered memory,
// so we get such flags through the functions here.
//
// Note that there is no free() function here.  We expect the usage
// model to be that callers acquire a block of memory for whatever use
// they have, and then manage dynamic allocation and deallocation out
// of that if they need to.  This vastly simplifies the mechanics of
// managing both the memory and the NIC registration.
//

//
// We align all allocations to 8-byte boundaries.
//
#define REG_MEM_ALIGNMENT 8

#define REG_MEM_ALIGN(s) \
        (((s) + REG_MEM_ALIGNMENT - 1) & ~(REG_MEM_ALIGNMENT - 1))


//
// This is the memory out of which we satisfy allocation requests.
//
static char* reg_mem_space;
static size_t reg_mem_size;

static char* reg_mem_ptr;


//
// Add a request for memory to be allocated once the pool is set up.
//
void chpl_comm_mem_reg_add_request(size_t size)
{
  //
  // Add this size (aligned upward as we will do when the actual
  // allocation is done) to the amount we'll be asked to provide
  // later.
  //
  reg_mem_size += REG_MEM_ALIGN(size);
}


//
// Initialize.
//
void chpl_comm_mem_reg_init(void)
{
  size_t page_size = gethugepagesize();

  //
  // Acquire hugepage space adequate for the requests that were added.
  //
  reg_mem_size = (reg_mem_size + page_size - 1) & ~(page_size - 1);
  reg_mem_space = get_huge_pages(reg_mem_size, GHP_DEFAULT);
  if (reg_mem_space == NULL)
    chpl_warning("cannot acquire special NIC-registered space; "
                 "will use regular space",
                 0, 0);

  reg_mem_ptr = reg_mem_space;

  //
  // We don't actually register the space -- the caller does that.
  //
}


//
// Tell about our space.
//
void chpl_comm_mem_reg_tell(void** space_p, size_t* size_p)
{
  if (space_p != NULL)
    *space_p = reg_mem_space;
  if (size_p != NULL)
    *size_p = reg_mem_size;
}


//
// Allocate.
//
void* chpl_comm_mem_reg_allocMany(size_t number, size_t size,
                                  chpl_mem_descInt_t description,
                                  int32_t lineno, int32_t filename)
{
  char* p;

  if (reg_mem_space == NULL) {
    p = chpl_mem_allocMany(number, size, description, lineno, filename);
  }
  else {
    size = REG_MEM_ALIGN(number * size);
    if (reg_mem_ptr + size > reg_mem_space + reg_mem_size)
      chpl_internal_error("out of NIC-registered space");

    p = reg_mem_ptr;
    reg_mem_ptr += size;
  }

  return p;
}


//
// Shut down.
//
void chpl_comm_mem_reg_exit(void)
{
  return;
}
