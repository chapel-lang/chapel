/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#ifndef TEST_RESOLUTION_H
#define TEST_RESOLUTION_H

#include "test-parsing.h"

#include "chpl/resolution/resolution-types.h"
#include "chpl/types/QualifiedType.h"

// forward declare classes and namespaces
namespace chpl {
  namespace resolution {
  }
  namespace types {
  }
}

using namespace resolution;
using namespace types;

// assumes the last statement is a variable declaration for x
// with an initialization expression.
// Returns the type of the initializer expression.
QualifiedType resolveTypeOfXInit(Context* context,
                                 std::string program,
                                 bool requireTypeKnown = true);

QualifiedType resolveQualifiedTypeOfX(Context* context, std::string program);

const Type* resolveTypeOfX(Context* context, std::string program);

const ResolvedExpression*
resolvedExpressionForAst(Context* context, const AstNode* ast,
                         const ResolvedFunction* inFn,
                         bool scopeResolveOnly);

// check that in method methodIdStr, the call with id callIdStr resolves
// to a function with id calledFnIdStr.
// if calledFnIdStr == "", expect no match (e.g. ambiguity)
void testCall(const char* testName,
              const char* program,
              const char* methodIdStr,
              const char* callIdStr,
              const char* calledFnIdStr);

const Variable* findVariable(const AstNode* ast, const char* name);
const Variable* findVariable(const ModuleVec& vec, const char* name);

std::unordered_map<std::string, QualifiedType>
resolveTypesOfVariables(Context* context, std::string program, const std::vector<std::string>& variables);

#endif
