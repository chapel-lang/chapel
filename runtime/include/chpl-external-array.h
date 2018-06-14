/*
 * Copyright 2004-2018 Cray Inc.
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

#ifndef _chpl_external_array_H_
#define _chpl_external_array_H_

#include <stdint.h>

typedef void (*chpl_free_func)(void*);

typedef struct {
  void* elts;
  uint64_t size;

  chpl_free_func freer;
} chpl_external_array;

chpl_external_array chpl_make_external_array(uint64_t elt_size,
                                             uint64_t num_elts);
chpl_external_array chpl_make_external_array_ptr(void* elts,
                                                 uint64_t size);
void chpl_free_external_array(chpl_external_array x);

#endif
