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


// Leave malloc/free/etc to use the system allocator.
// malloc/free may not return memory that can be
// communicated. malloc/free might return memory
// outside of a large Chapel pre-allocated heap.
#undef malloc
#undef calloc
#undef free
#undef realloc
