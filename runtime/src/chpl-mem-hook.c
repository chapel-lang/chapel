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
// Error checking functions for memory debugging support.
//
#include "chplrt.h"

#include "chpl-mem.h"
#include "chpl-mem-hook.h"
#include "chpltypes.h"
#include "error.h"

#include <stdio.h>
#include <stdint.h>


void chpl_memhook_check_pre(size_t number, size_t size,
                            chpl_mem_descInt_t description,
                            int32_t lineno, int32_t filename) {
  if (!chpl_mem_inited())
    chpl_error("memory routine called before the memory layer is initialized",
               lineno, filename);

  if (number > 0 && size > SIZE_MAX/number)
    chpl_error("Attempting to allocate > max(size_t) bytes of memory",
               lineno, filename);
}


void chpl_memhook_check_post(void* memAlloc,
                             chpl_mem_descInt_t description,
                             int32_t lineno, int32_t filename) {
  if (memAlloc == NULL) {
    const int messageSize = 1024;
    char message[messageSize];

    snprintf(message, messageSize, "Out of memory allocating \"%s\"",
             chpl_mem_descString(description));
    chpl_error(message, lineno, filename);
  }
}
