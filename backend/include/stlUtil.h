/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

//# stl.h -*- C++ -*-
//
// For loop macros for use with STL container classes
//
//########################################################################

#ifndef STL_UTIL_H
#define STL_UTIL_H

#include "chpl/util/iteration.h"

//
// -- Historical note --
//
// These macros were intended to mimic the operation of the former
// forv_Vec-like macros. They have been replaced by range-for in two cases, and
// a suitable adapter adding for the other. Next steps on this front should
// remove all uses of these macros and then this file.
//
// NOTE: unlike forv_Vec, these macros do not allow iterating over an
// expanding vector because they take begin() and end() before the
// loop begins

#define for_vector(TYPE, VAL, VEC) for (TYPE* VAL : chpl::uptoNullptrIterator(VEC))

#define for_vector_allowing_0s(TYPE, VAL, VEC) for (TYPE* VAL : VEC)

// NOTE: this does not have an uptoNullptrIterator because an std::unordered_set
// doesn't have a well-defined iteration order, so I presume any uses of that
// did not exist. And over a regular std::set, a nullptr would be the first
// entry and the loop would never fire, so I presume no uses like this exist
// either
#define for_set(TYPE, VAL, SET) for (TYPE * VAL : SET)

#endif
