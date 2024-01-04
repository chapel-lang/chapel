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
#ifndef _chplconstargcheck_H_
#define _chplconstargcheck_H_

#include <stdint.h>
#include <stdlib.h>

// FNV-1a hash function, a "string hash"
uint64_t const_arg_hash(void* ptr, size_t size);

void check_const_hash_matches(uint64_t start_val, uint64_t end_val,
                              const char* arg_name, int32_t lineno,
                              int32_t filenameIdx);

#endif
