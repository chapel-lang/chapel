/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#define ADVANCE_PRESERVING_STANDARD_MODULES_(ctx__) \
  do { \
    ctx__->advanceToNextRevision(false); \
    setupModuleSearchPaths(ctx__, false, false, {}, {}); \
  } while (0)

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
resolveTypesOfVariables(Context* context,
                        const Module* mod,
                        const std::vector<std::string>& variables);

std::unordered_map<std::string, QualifiedType>
resolveTypesOfVariables(Context* context, std::string program, const std::vector<std::string>& variables);

std::unordered_map<std::string, QualifiedType>
resolveTypesOfVariablesInit(Context* context, std::string program, const std::vector<std::string>& variables);

void ensureParamInt(const QualifiedType& type, int64_t expectedValue);
void ensureParamUint(const QualifiedType& type, uint64_t expectedValue);
void ensureParamBool(const QualifiedType& type, bool expectedValue);
void ensureParamString(const QualifiedType& type, const std::string& expectedValue);
void ensureParamEnumStr(const QualifiedType& type, const std::string& expectedName);
void ensureErroneousType(const QualifiedType& type);

QualifiedType getTypeForFirstStmt(Context* context, const std::string& program);

Context::Configuration getConfigWithHome();

/**
  Returns the ResolvedFunction called by a particular
  ResolvedExpression, if there was exactly one candidate.
  Otherwise, it returns nullptr.

  This function does not handle return intent overloading.
 */
const ResolvedFunction* resolveOnlyCandidate(Context* context,
                                             const ResolvedExpression& r);

QualifiedType findVarType(const Module* m,
                          const ResolutionResultByPostorderID& rr,
                          std::string name);

/**
  Test resolution of a domain literal.
 */
void testDomainLiteral(Context* context, std::string domainLiteral,
                       DomainType::Kind domainKind);

void testDomainIndex(Context* context, std::string domainType,
                     std::string expectedType);

void testDomainBadPass(Context* context, std::string argType,
                       std::string actualType);

#endif
