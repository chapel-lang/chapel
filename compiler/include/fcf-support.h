/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#ifndef COMPILER_INCLUDE_FCF_SUPPORT_H
#define COMPILER_INCLUDE_FCF_SUPPORT_H

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
// This header provides functions to construct and reason about first-class
// function and closure types.
//
namespace fcfs {

/***
  Determine if the 'fcfsUsePointerImplementation' config param in the
  'ChapelBase' module is set to 'true'. The module must be resolved.
*/
bool usePointerImplementation(void);

/***
  Create a function class instance given a function and a point-of-mention.
  The function class created is a thin wrapper around a function pointer
  and will never be a closure (an environment will not be created and the
  presence of outer variables will be ignored).

  The result will already be inserted into the tree, replacing 'use'.
*/
Expr* createFunctionClassInstance(FnSymbol* fn, Expr* use);

/*** Convert a function type to the corresponding function class type. */
Type* functionClassSuperTypeFromFunctionType(FunctionType* ft);

/***
  Used to implement the legacy 'func()' type constructor function. The type
  constructed by this function always returns by value, and each formal
  has the blank intent.
*/
Type* functionClassSuperTypeForFuncConstructor(CallExpr* call);

/***
  Computes a string for a function class type. The builtin FunctionType
  AST node should use its 'toString()' method instead.
*/
const char* functionClassTypeToString(Type* t);

/***
  May resolve the body of 'fn' if its return type is inferred. Will not
  resolve the body of 'fn' if the function is generic. Will not attempt
  resolution if the function is marked 'isResolved()'. Returns 'true'
  if the body had to be resolved as a side effect of this call.
*/
bool checkAndResolveSignature(FnSymbol* fn, Expr* use);

/***
  Will not try to resolve the function body if the function is generic.
  Will always try to resolve the body otherwise. Will not attempt
  resolution if the function is marked 'isResolved()'.
*/
void checkAndResolveSignatureAndBody(FnSymbol* fn, Expr* use);

/***
  Wrapper around the scope-resolve lookup function that returns a
  C++ std::vector. For convenience, since it is not trivial to
  change the underlying lookup function.
*/
std::vector<FnSymbol*> lookupFunctions(const char* name, Expr* use);

/***
  This class stores the result of computing all outer variables mentioned
  within a function in lexical order.
*/
class ClosureEnv {
 private:
  FnSymbol* owner_ = nullptr;
  std::vector<Symbol*> outerVariables_;
  std::map<Symbol*, std::vector<SymExpr*>> outerVariableToMentions_;
  std::vector<FnSymbol*> childFunctions_;

 public:
  ClosureEnv() = default;
  ~ClosureEnv() = default;
  ClosureEnv(FnSymbol* owner);

  FnSymbol* owner() const;
  bool isEmpty() const;
  int numOuterVariables() const;
  Symbol* outerVariable(int idx) const;
  int numMentions(Symbol* sym) const;
  SymExpr* firstMention(Symbol* sym) const;
  SymExpr* mention(Symbol* sym, int idx) const;
  const std::vector<FnSymbol*>& childFunctions() const;
};

/*** Compute the outer variables mentioned by a function in lexical order. */
const ClosureEnv& computeOuterVariables(FnSymbol* fn);

/*** Use to ensure a clean error for failed capture without other context. */
VarSymbol* errorSink(FunctionType::Kind);

/*** When a function is being captured but it's a statement in and of itself, warn. */
void emitWarningForStandaloneCapture(Expr* expr, const char* name);

} // end namespace 'fcfs'

#endif
