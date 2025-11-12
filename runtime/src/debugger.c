/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#include "chplrt.h"

#include "debugger.h"

#include <stdio.h>

#include "chpl-mem-sys.h"
#include "chpl-comm-compiler-macros.h"


void debuggerBreakHere(void) {printf("%s", "");}

void* chpl_debug_get(void* locale, void* addr, size_t size) {
  chpl_localeID_t* localeID = (chpl_localeID_t*)locale;
  size_t numBytes = sizeof(uint8_t)*size;
  void* ret = sys_malloc(numBytes);
  if (ret == NULL) {
    chpl_error("Out of memory in chpl_debug_get", 0, 0);
  }
  chpl_gen_comm_get(ret, chpl_rt_nodeFromLocaleID(*localeID), addr,
                    numBytes, CHPL_COMM_UNKNOWN_ID, -1, 0);
  return ret;
}
void chpl_debug_free(void* ptr) {
  // free the memory allocated in chpl_debug_get
  sys_free(ptr);
}
