/*
 * Copyright 2023-2026 Hewlett Packard Enterprise Development LP
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

//
// This file defines nearly all the methods exposed to python from the frontend.
//
// To use these definitions, define the macro and then `#include` this file.
// See `chapel.cpp` for examples. If the macros below are not defined, they are
// defined to be empty. Only the ones that are used at the time of include need
// to be defined.
//

//
// Defines the beginning of an ast node
//
#ifndef CLASS_BEGIN
#define CLASS_BEGIN(TAG)
#endif

//
// Defines the end of an ast node
//
#ifndef CLASS_END
#define CLASS_END(TAG)
#endif

//
// Defines a simple getter that takes no arguments
//
// BODY is an inline definition of the getter. Available variables:
// - contextObject: the current frontend context
// - node: the ast node
// TYPEFN is the type of the method, not including the receiver. It should
//        be written as the C++ function type, ReturnType(ArgTypes...).
//
#ifndef METHOD
#define METHOD(NODE, NAME, DOCSTR, TYPEFN, BODY)
#endif

#ifndef PLAIN_GETTER
#define PLAIN_GETTER(NODE, NAME, DOCSTR, RETTYPE, BODY) \
  METHOD(NODE, NAME, DOCSTR, RETTYPE(), (void) args; BODY)
#endif

//
// Declares a simple getter that takes no arguments. The definition is expected
// elsewhere in a function named `NODEObject_NAME`. See the `ACTUAL_ITERATOR`
// macro for an example
//
#ifndef METHOD_PROTOTYPE
#define METHOD_PROTOTYPE(NODE, NAME, DOCSTR)
#endif

//
// Forward declares a prototype for __iter__, which due to current limitations
// cannot be implemented in method tables. The TYPE argument is the (C++)
// type yielded by the iterator.
#ifndef ITER_PROTOTYPE
#define ITER_PROTOTYPE(NODE, TYPE)
#endif

//
// Forward declares a prototype for an operator method, which due to current
//  limitations cannot be implemented in method tables.
// The TYPEFN argument is the full C++ function type of the operator,
// including the receiver.
#ifndef OPERATOR_PROTOTYPE
#define OPERATOR_PROTOTYPE(NODE, NAME, DOCSTR, TYPEFN)
#endif

#include "method-tables/core-methods.h"
#include "method-tables/param-methods.h"
#include "method-tables/type-methods.h"
#include "method-tables/uast-methods.h"
#include "method-tables/error-methods.h"

//
// Cleanup and undefine all macros
//
#undef CLASS_BEGIN
#undef CLASS_END
#undef METHOD
#undef PLAIN_GETTER
#undef METHOD_PROTOTYPE
#undef ITER_PROTOTYPE
#undef OPERATOR_PROTOTYPE
