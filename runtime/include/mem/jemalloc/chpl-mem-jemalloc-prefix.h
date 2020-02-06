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

/* jemalloc memory function implementation */
#ifndef _chpl_mem_jemalloc_prefix_H_
#define _chpl_mem_jemalloc_prefix_H_

// If the build environment doesn't supply a jemalloc symbol prefix
// then just use "je_"
#ifndef CHPL_JEMALLOC_PREFIX
#define CHPL_JEMALLOC_PREFIX je_
// Ask jemalloc.h to provide the je_ functions
// (i.e. je_malloc rather than just malloc)
#define JEMALLOC_NO_DEMANGLE
#endif

#endif
