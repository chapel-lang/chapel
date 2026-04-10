/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_RT_UNWIND_H
#define CHPL_RT_UNWIND_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LAUNCHER
  // These symbols do nothing in launcher code, so define stubs here.
  static inline char* chpl_stack_unwind_to_string(char sep) { return NULL; }
  static inline void chpl_stack_unwind(FILE* out, char sep) {}
#else
  // Otherwise, call out to implementations in 'chpl-unwind.c'...
  char* chpl_stack_unwind_to_string(char sep);
  void chpl_stack_unwind(FILE* out, char sep);
#endif

#ifdef __cplusplus
}
#endif

#endif
