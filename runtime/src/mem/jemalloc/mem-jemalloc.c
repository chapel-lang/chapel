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

void chpl_mem_layerInit(void) {
  void* start;
  size_t size;

  chpl_comm_desired_shared_heap(&start, &size);

  //
  // TODO (EJR 12/17/15): add support for shared heaps. I think we basically
  // need to create a custom chunk allocator.
  //
  // HPX-5 did this so I think we can too:
  //     http://jemalloc-discuss.canonware.narkive.com/FzSQ4Qv4/need-help-in-porting-jemalloc
  //
  //     http://www.canonware.com/pipermail/jemalloc-discuss/2015-October/001179.html
  //
  //  TODO (EJR 12/17/15): when we support shared heaps, I need to remember to
  //  update the third-party README
  //
  if (start || size)
    chpl_error("set CHPL_MEM to a more appropriate mem type", 0, 0);

  //
  // Do a first allocation, to allow jemalloc to set up:
  //
  //   """
  //   Once, when the first call is made to one of the memory allocation
  //   routines, the allocator initializes its internals based in part on various
  //   options that can be specified at compile- or run-time.
  //   """
  //
  {
    void* p;

    if ((p = je_malloc(1)) == NULL)
      chpl_internal_error("cannot init heap: je_malloc() failed");
    je_free(p);
  }

}


void chpl_mem_layerExit(void) { }
