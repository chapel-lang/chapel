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

#ifndef _FIRST_CLASS_FUNCTIONS_H_
#define _FIRST_CLASS_FUNCTIONS_H_

#include "alist.h"

class CallExpr;
class FnSymbol;
class Type;

// This creates a FCF wrapper instance from the 'PRIM_CAPTURE_FN_FOR_CHPL'
// primitive call. This is the only method of creating a first class
// function wrapper instance today, but in the future we might like for
// for them to be created directly from an expression rather than use a
// primitive call.
//
// LEGACY FCFs:
//
// If the 'legacyFirstClassFunctions' toggle is set, then the following
// holds for the wrapper class instance that is created:
//
//    - Formal names will not be considered as part of the function's type
//      when inserting the new wrapper type into the FCF class hierarchy
//      (in other words - all formals are considered anonymous)
//    - The intent for each formal will be considered to be 'INTENT_BLANK',
//      and a compiler warning will be issued if this is not the case
//    - The formals will be named 'x1'..'xn'
//    - The originating function may 'throw'. Note that this is not
//      possible to express with the legacy 'func' type constructor.
//      Instead, users express this by writing a named function with the
//      appropriate type that throws, create a FCF from that function,
//      and then take the type of that FCF instance, e.g,
//      ```
//      proc foo(x: int, y: int): int throws {}
//      var f = foo;
//      type T = f.type;
//      ```
Expr* fcfWrapperInstanceFromPrimCall(CallExpr* call);

// This takes a 'PRIM_CAPTURE_FN_FOR_C' primitive and produces a void/void
// pointer as a result. It is used to plumb things through to the runtime.
// In the future, function types can probably be used directly instead.
Expr* fcfRawFunctionPointerFromPrimCall(CallExpr* call);

// This is currently unused, but its intention is to sidestep any prim call.
Expr* fcfWrapperInstanceFromFnExpr(FnSymbol* fn);

// This implements the legacy 'func()' type constructor (which is a set of
// functions that live in ChapelBase). Function types constructed in this
// fashion will never have named formals, and their argument intents will
// always be 'INTENT_BLANK'. In the future, it will be deprecated in favor
// of the built-in function type constructor.
//
// LEGACY FCFs:
//
// This function is not impacted by the 'legacyFirstClassFunctions' toggle.
// Its semantics are already significantly impacted by it originating
// from a function call.
Type* fcfWrapperSuperTypeFromFuncFnCall(CallExpr* call);

// This is the current interface for the new procedure type constructor.
// In the future the compiler will reason about procedure types directly
// (by creating a new instance of a 'Type').
//
// This constructs a 'FCF super type' representing the passed in function.
// For example, if the function has the type 'proc(_: int): void', then
// the super type in the FCF wrapper hierarchy representing such a function
// type might be 'chpl__fcf_int_void'. All FCFs of the same type will be
// subclasses of such a superclass.
//
// LEGACY FCFs:
//
// If 'legacyFirstClassFunctions' is set, any calls to this function will
// trigger a 'INT_FATAL' error. This is because the modern FCF type
// constructor was not supported in the legacy world, and it seems like
// more trouble than it's worth to provide cross-compat.
//
Type* fcfWrapperSuperTypeFromFnType(FnSymbol* fn);

// This is intended to print out a user-facing type for the wrapper.
const char* fcfWrapperTypeToString(Type* t);

// Recursively consider if the formals and return type of this function
// type are themselves valid extern types.
bool fcfIsValidExternType(Type* t);

#endif
