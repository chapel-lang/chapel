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

#include <map>
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

/***
  Create an instance of a closure given a function and a point-of-use.
  The expression will not be inserted into the tree and is resolved.

  This is currently only supported for legacy closures.
*/
Expr* createClassInstance(FnSymbol* fn, Expr* use);

/*** This always computes a legacy closure type. */
Type* legacySuperTypeForFuncConstructor(CallExpr* call);

/*** Currently only works for legacy closures. */
const char* closureTypeToString(Type* t);

/*** May resolve body if return type is inferred, but not if generic. */
bool checkAndResolveSignature(FnSymbol* fn, Expr* use);

/*** Will not resolve body if the input is generic. */
bool checkAndResolveSignatureAndBody(FnSymbol* fn, Expr* use);

/*** Wrapper around the scope-resolve lookup function. */
std::vector<FnSymbol*> lookupFunctions(const char* name, Expr* use);

class ClosureEnv {
 private:
  FnSymbol* owner_ = nullptr;
  std::vector<Symbol*> outerVariables_;
  std::map<Symbol*, std::vector<SymExpr*>> outerVariableToUses_;
  std::vector<FnSymbol*> childFunctions_;

 public:
  ClosureEnv() = default;
  ~ClosureEnv() = default;
  ClosureEnv(FnSymbol* owner);

  FnSymbol* owner() const;
  bool isEmpty() const;
  int numOuterVariables() const;
  Symbol* outerVariable(int idx) const;
  int numUsesOf(Symbol* sym) const;
  SymExpr* firstUseOf(Symbol* sym) const;
  SymExpr* useOf(Symbol* sym, int idx) const;
};

/*** Compute the outer variables used by a function in lexical order. */
const ClosureEnv& computeOuterVariables(FnSymbol* fn);

/*** Emitted whenever an error occurs when resolving function capture. */
VarSymbol* errorSink(FunctionType::Kind);

/*** Emitted whenever an error occurs when creating a closure. */
VarSymbol* closureErrorSink(void);

} // end namespace 'closures'

#endif
