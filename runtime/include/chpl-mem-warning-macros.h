/*
 * Copyright 2004-2017 Cray Inc.
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


// NOTE: there is no include guard. This is so that the mem-warning and
// mem-no-warning headers can be included an arbitrary number of times to
// disable and then re-enable the macros

#define malloc  dont_use_malloc_use_chpl_mem_allocMany_instead
#define calloc  dont_use_calloc_use_chpl_mem_allocMany_instead
#define free    dont_use_free_use_chpl_mem_free_instead
#define realloc dont_use_realloc_use_chpl_mem_realloc_instead
