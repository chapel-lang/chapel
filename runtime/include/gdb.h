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

#ifndef _GDB_H_
#define _GDB_H_

#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

void gdbShouldBreakHere(void);  // must be in separate file to avoid elimination

inline static void chpl_debugtrap(void) {
  #ifdef __has_builtin
    #if __has_builtin (__builtin_debugtrap)
      #define call_debugtrap() __builtin_debugtrap()
    #endif
  #endif
  #ifndef call_debugtrap
    #define call_debugtrap() raise(SIGTRAP)
  #endif
  call_debugtrap();
  #undef call_debugtrap
}

#ifdef __cplusplus
}
#endif

#endif
