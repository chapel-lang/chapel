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

#ifndef CHPL_TYPES_TYPECLASSES_H
#define CHPL_TYPES_TYPECLASSES_H

namespace chpl {
namespace types {


// forward declare the various Type subclasses
// using macros and TypeClassesList.h
/// \cond DO_NOT_DOCUMENT
#define TYPE_DECL(NAME) class NAME;
#define TYPE_NODE(NAME) TYPE_DECL(NAME)
#define BUILTIN_TYPE_NODE(NAME, CHPL_NAME_STR) TYPE_DECL(NAME)
#define TYPE_BEGIN_SUBCLASSES(NAME) TYPE_DECL(NAME)
#define TYPE_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to TypeClassesList.h
#include "chpl/types/TypeClassesList.h"
// clear the macros
#undef TYPE_NODE
#undef BUILTIN_TYPE_NODE
#undef TYPE_BEGIN_SUBCLASSES
#undef TYPE_END_SUBCLASSES
#undef TYPE_DECL

// forward declare other classes
class Type;


} // end namespace types
} // end namespace chpl

#endif
