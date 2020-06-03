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
#include "chplrt.h"

#include "chpl-export-wrappers.h"
#include "chpl-mem.h"

#include <string.h>

void chpl_byte_buffer_free_server(chpl_byte_buffer cb) {
  if (!cb.isOwned) { return; }

  if (cb.data != NULL) {
    chpl_mem_free(cb.data, 0, 0);
  }

  return;
}

void chpl_byte_buffer_free(chpl_byte_buffer cb) {
  if (!cb.isOwned) { return; }

  if (cb.data != NULL) {
    chpl_free(cb.data);
  }

  return;
}

chpl_byte_buffer chpl_byte_buffer_make(const char* data) {
  // We can get away with this cast because we mark "isOwned" as false.
  chpl_byte_buffer result = { 0, (char*) data, strlen(data) };
  return result;
}

chpl_byte_buffer chpl_byte_buffer_make_len(const char* data, uint64_t size) {
  // We can get away with this cast because we mark "isOwned" as false.
  chpl_byte_buffer result = { 0, (char*) data, size };
  return result;
}
