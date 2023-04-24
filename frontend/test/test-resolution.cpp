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

#include "test-resolution.h"

#include "chpl/resolution/resolution-queries.h"
#include "chpl/uast/Module.h"


QualifiedType
resolveTypeOfXInit(Context* context, std::string program, bool requireTypeKnown) {
  auto m = parseModule(context, std::move(program));
  assert(m->numStmts() > 0);
  const Variable* x = m->stmt(m->numStmts()-1)->toVariable();
  assert(x);
  assert(x->name() == "x");
  auto initExpr = x->initExpression();
  assert(initExpr);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(initExpr).type();
  if (requireTypeKnown) assert(qt.type());

  return qt;
}

QualifiedType
resolveQualifiedTypeOfX(Context* context, std::string program) {
  auto m = parseModule(context, std::move(program));
  assert(m->numStmts() > 0);
  const Variable* x = m->stmt(m->numStmts()-1)->toVariable();
  assert(x);
  assert(x->name() == "x");

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(x).type();
  assert(qt.type());

  return qt;
}

const Type*
resolveTypeOfX(Context* context, std::string program) {
  auto m = parseModule(context, std::move(program));
  assert(m->numStmts() > 0);
  const Variable* x = m->stmt(m->numStmts()-1)->toVariable();
  assert(x);
  assert(x->name() == "x");

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto qt = rr.byAst(x).type();
  assert(qt.kind() == QualifiedType::VAR);
  assert(qt.type());

  const Type* t = qt.type();
  return t;
}

const ResolvedExpression*
resolvedExpressionForAst(Context* context, const AstNode* ast,
                         const ResolvedFunction* inFn,
                         bool scopeResolveOnly) {
  if (!(ast->isLoop() || ast->isBlock())) {
    // compute the parent module or function
    int postorder = ast->id().postOrderId();
    if (postorder >= 0) {
      ID parentId = ast->id().parentSymbolId(context);
      auto parentAst = idToAst(context, parentId);
      if (parentAst != nullptr) {
        if (parentAst->isModule()) {
          if (scopeResolveOnly) {
            const auto& byId = scopeResolveModule(context, parentAst->id());
            return &byId.byAst(ast);
          } else {
            const auto& byId = resolveModule(context, parentAst->id());
            return &byId.byAst(ast);
          }
        } else if (auto parentFn = parentAst->toFunction()) {
          auto untyped = UntypedFnSignature::get(context, parentFn);
          // use inFn if it matches
          if (inFn && inFn->signature()->untyped() == untyped) {
            return &inFn->resolutionById().byAst(ast);
          } else {
            if (scopeResolveOnly) {
              auto rFn = scopeResolveFunction(context, parentFn->id());
              return &rFn->resolutionById().byAst(ast);
            } else {
              auto typed = typedSignatureInitial(context, untyped);
              if (!typed->needsInstantiation()) {
                auto rFn = resolveFunction(context, typed, nullptr);
                return &rFn->resolutionById().byAst(ast);
              }
            }
          }
        }
      }
    }
  }

  return nullptr;
}

void testCall(const char* testName,
              const char* program,
              const char* methodIdStr,
              const char* callIdStr,
              const char* calledFnIdStr) {
  printf("test %s\n", testName);
  Context ctx;
  Context* context = &ctx;

  auto path = UniqueString::get(context, testName);
  std::string contents = program;
  setFileText(context, path, contents);
  // parse it so that Context knowns about the IDs
  const ModuleVec& vec = parseToplevel(context, path);

  for (auto m : vec) {
    m->dump();
  }

  ID methodId = ID::fromString(context, methodIdStr);
  ID callId = ID::fromString(context, callIdStr);
  ID calledFnId;
  if (calledFnIdStr[0] != '\0') {
    calledFnId = ID::fromString(context, calledFnIdStr);
  }

  auto methodAst = parsing::idToAst(context, methodId);
  assert(methodAst);
  assert(methodAst->isFunction());
  auto callAst = parsing::idToAst(context, callId);
  assert(callAst);
  assert(callAst->isIdentifier() || callAst->isDot() || callAst->isCall());
  const AstNode* calledFnAst = nullptr;
  if (calledFnIdStr[0] != '\0') {
    calledFnAst = parsing::idToAst(context, calledFnId);
    assert(calledFnAst);
    assert(calledFnAst->isVariable() || calledFnAst->isAggregateDecl() ||
           calledFnAst->isFunction());
  }

  const ResolvedFunction* r = resolveConcreteFunction(context, methodId);

  const ResolvedExpression& re = r->byId(callAst->id());
  ID toIdentId = re.toId();
  if (!toIdentId.isEmpty()) {
    printf("full resolved fn to id %s\n", toIdentId.str().c_str());
  }
  ID toFnId;
  if (auto fn = re.mostSpecific().only()) {
    toFnId = fn->id();
    printf("full resolved fn to %s\n", toFnId.str().c_str());
  }
  if (calledFnIdStr[0] != '\0') {
    assert(toIdentId == calledFnAst->id() || toFnId == calledFnAst->id());
  } else {
    assert(toIdentId.isEmpty() && toFnId.isEmpty());
  }
  printf("\n");
}

const Variable* findVariable(const AstNode* ast, const char* name) {
  if (auto v = ast->toVariable()) {
    if (v->name() == name) {
      return v;
    }
  }

  for (auto child : ast->children()) {
    auto got = findVariable(child, name);
    if (got) return got;
  }

  return nullptr;
}

const Variable* findVariable(const ModuleVec& vec, const char* name) {
  for (auto mod : vec) {
    auto got = findVariable(mod, name);
    if (got) return got;
  }

  return nullptr;
}

std::unordered_map<std::string, QualifiedType>
resolveTypesOfVariables(Context* context,
                        std::string program,
                        const std::vector<std::string>& variables) {
  auto m = parseModule(context, std::move(program));
  auto& rr = resolveModule(context, m->id());

  std::unordered_map<std::string, QualifiedType> toReturn;
  for (auto& variable : variables) {
    auto varAst = findVariable(m, variable.c_str());
    assert(varAst != nullptr);
    toReturn[variable] = rr.byAst(varAst).type();
  }
  return toReturn;
}
