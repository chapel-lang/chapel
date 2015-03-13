/*
 * Copyright 2004-2015 Cray Inc.
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

#ifndef _chpl_mem_no_warning_macros_h_
#define _chpl_mem_no_warning_macros_h_

// runtime/include/mem/*/chpl-mem-impl.h defines
// chpl_calloc, chpl_malloc, chpl_realloc, chpl_free
// with the same signatures as the standard functions
// and no additional error checking.
#include "chpl-mem-impl.h"

#undef malloc
#undef calloc
#undef free
#undef realloc

#define malloc  chpl_malloc
#define calloc  chpl_calloc
#define free    chpl_free
#define realloc chpl_realloc

#endif
