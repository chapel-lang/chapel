/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

// Since aligned_alloc is C++17 or C11 and we're currently
// requiring C++14 and C11, this file arranges to use the C11
// version.
//
// It can be removed when C++17 is required.

#ifndef MY_ALIGNED_ALLOC_H
#define MY_ALIGNED_ALLOC_H

// note that size_t has to be defined already by whatever includes this.
// that is intentional to simplify including this from both C and C++

#ifdef __cplusplus
extern "C" {
#endif

void *my_aligned_alloc(size_t alignment, size_t size);

#ifdef __cplusplus
}
#endif

#endif
