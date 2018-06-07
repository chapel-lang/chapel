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
#include "chplrt.h"

#include "chpl-external-array.h"
#include "chpl-mem.h"

const chpl_free_func CHPL_FREE_FUNC_NIL = NULL;
const chpl_free_func CHPL_FREE_FUNC_CHAPEL_WRAP = chpl_wrap_chapel_free_call;

void chpl_call_free(chpl_external_array x) {
  if (x.freer != CHPL_FREE_FUNC_NIL) {
    x.freer(x.elts);
  }
}

void chpl_wrap_chapel_free_call(void* mem) {
  chpl_mem_free(mem, 0, 0);
}
