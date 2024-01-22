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
#include "chpl-const-arg-check.h"

#include "chplrt.h"
#include "chpl-linefile-support.h"
#include "chpl-mem.h"
#include "error.h"

// FNV-1a hash function, a "string hash"
uint64_t const_arg_hash(void* ptr, size_t size) {
  uint64_t hashValue = 0xcbf29ce484222325; // starting seed
  uint64_t FNV_prime = 0x100000001b3;

  unsigned char* fakeCharPtr = (unsigned char*)ptr;
  size_t i;
  for (i = 0; i < size; i++) {
    hashValue = (hashValue ^ *fakeCharPtr) * FNV_prime;
    fakeCharPtr++;
  }
  return hashValue;
}

void check_const_hash_matches(uint64_t start_val, uint64_t end_val,
                              const char* arg_name, int32_t lineno,
                              int32_t filenameIdx) {
  if (start_val != end_val) {
    char* warning_msg = chpl_glom_strings(8, "The argument '", arg_name,
                      "' was modified indirectly during this function, this ",
                      "behavior is unstable and may change in the future.\n",
                      "If this behavior is intentional, declaring the argument",
                      " as 'const ref' will silence this warning.  If not, ",
                      "declaring the argument as 'const in' will prevent ",
                      "indirect modifications");
    chpl_warning(warning_msg, lineno, filenameIdx);
    chpl_mem_free(warning_msg, 0, 0);
  }
}
