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

#ifndef COMPILER_INCLUDE_CLOSURES_H
#define COMPILER_INCLUDE_CLOSURES_H

#include "CallExpr.h"
#include "symbol.h"

#include <vector>

class CallExpr;
class Expr;
class FnSymbol;
class FunctionType;
class Type;

//
// This header provides functions to construct and reason about closure
// types. It also offers helper functions which are useful for resolving
// function types in general.
//
namespace closures {

/***
  Determine if the 'legacyFirstClassFunctions' config param in the
  'ChapelBase' module is set to 'true'. The module must be resolved.
*/
bool legacyFirstClassFunctions(void);

bool isFunctionCapturePrimitive(CallExpr* call);

Expr* instanceFromFnExpr(FnSymbol* fn, Expr* use);

Type* superTypeFromLegacyFuncFnCall(CallExpr* call);

const char* closureTypeToString(Type* t);

bool isClosureValidExternType(Type* t);

/*** May resolve the body if the return type is inferred. */
bool checkAndResolveSignature(FnSymbol* fn, Expr* use);

/*** Will not resolve the body if the input is generic. */
bool checkAndResolveSignatureAndBody(FnSymbol* fn, Expr* use);

/*** Wrapper around the scope-resolve lookup function. */
std::vector<FnSymbol*> lookupFunctions(const char* name, Expr* use);

/*** Compute the outer variables used by a closure in lexical order. */
const std::vector<Symbol*>& computeOuterVariables(FnSymbol* fn);

/*** Emitted whenever an error occurs when resolving function capture. */
VarSymbol* errorSink(FunctionType::Kind);

/*** Emitted whenever an error occurs when creating a closure. */
VarSymbol* closureErrorSink(void);

} // end namespace 'closures'

#endif
