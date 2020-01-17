/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _chpl_export_wrappers_h_
#define _chpl_export_wrappers_h_

#include <stdbool.h>
#include <stdint.h>

typedef struct chpl_byte_buffer {
  int8_t isOwned;
  char* data;
  uint64_t size;
} chpl_byte_buffer;

void chpl_byte_buffer_free(chpl_byte_buffer cb);

chpl_byte_buffer chpl_byte_buffer_make(const char* data);
chpl_byte_buffer chpl_byte_buffer_make_len(const char* data, uint64_t size);

#endif
