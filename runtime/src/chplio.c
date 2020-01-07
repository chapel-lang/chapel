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

#include "chplrt.h"

#include "chplio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// These should be moved to chpl-string.c and eventually go away.
chpl_string chpl_refToString(void* ref) {
  char buff[32];
  (void) snprintf(buff, sizeof(buff), "%p", ref);
  return string_copy(buff, 0, 0);
}


chpl_string chpl_wideRefToString(c_nodeid_t node, void* addr) {
  char buff[32];
  (void) snprintf(buff, sizeof(buff), "%" PRI_c_nodeid_t ":%p", node, addr);
  return string_copy(buff, 0, 0);
}
