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
  // TODO: Use 'inFn' to reconstruct the correct 'ResolutionContext' state.
  ResolutionContext rcval(context);
  auto rc = &rcval;

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
              auto typed = typedSignatureInitial(rc, untyped);
              if (!typed->needsInstantiation()) {
                // TODO: Detect if this is an interior call or not.
                // If it's not, we can rewind frames.
                ResolutionContext rcval(context);
                auto rFn = resolveFunction(&rcval, typed, nullptr);
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
  if (auto c = re.mostSpecific().only()) {
    toFnId = c.fn()->id();
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
                        const Module* mod,
                        const std::vector<std::string>& variables) {
  std::unordered_map<std::string, QualifiedType> toReturn;
  auto& rr = resolveModule(context, mod->id());
  for (auto& variable : variables) {
    if (auto varAst = findVariable(mod, variable.c_str())) {
      toReturn[variable] = rr.byAst(varAst).type();
    }
  }
  assert(variables.size() == toReturn.size());
  return toReturn;
}

std::unordered_map<std::string, QualifiedType>
resolveTypesOfVariables(Context* context,
                        std::string program,
                        const std::vector<std::string>& variables) {
  auto path = UniqueString::get(context, "input.chpl");
  setFileText(context, path, std::move(program));
  const ModuleVec& vec = parseToplevel(context, path);
  std::unordered_map<std::string, QualifiedType> toReturn;

  for (auto m : vec) {
    auto& rr = resolveModule(context, m->id());

    for (auto& variable : variables) {
      if (auto varAst = findVariable(m, variable.c_str())) {
        toReturn[variable] = rr.byAst(varAst).type();
      }
    }
  }
  assert(variables.size() == toReturn.size());
  return toReturn;
}

std::unordered_map<std::string, QualifiedType>
resolveTypesOfVariablesInit(Context* context,
                        std::string program,
                        const std::vector<std::string>& variables) {
  auto m = parseModule(context, std::move(program));
  auto& rr = resolveModule(context, m->id());

  std::unordered_map<std::string, QualifiedType> toReturn;
  for (auto& variable : variables) {
    auto varAst = findVariable(m, variable.c_str());
    assert(varAst != nullptr);
    assert(varAst->initExpression());
    toReturn[variable] = rr.byAst(varAst->initExpression()).type();
  }
  return toReturn;
}

void ensureParamInt(const QualifiedType& type, int64_t expectedValue) {
  assert(type.kind() == QualifiedType::PARAM);
  assert(type.type() != nullptr);
  assert(type.type()->isIntType());
  assert(type.param() != nullptr);
  assert(type.param()->isIntParam());
  assert(type.param()->toIntParam()->value() == expectedValue);
}

void ensureParamUint(const QualifiedType& type, uint64_t expectedValue) {
  assert(type.kind() == QualifiedType::PARAM);
  assert(type.type() != nullptr);
  assert(type.type()->isUintType());
  assert(type.param() != nullptr);
  assert(type.param()->isUintParam());
  assert(type.param()->toUintParam()->value() == expectedValue);
}

void ensureParamBool(const QualifiedType& type, bool expectedValue) {
  assert(type.kind() == QualifiedType::PARAM);
  assert(type.type() != nullptr);
  assert(type.type()->isBoolType());
  assert(type.param() != nullptr);
  assert(type.param()->isBoolParam());
  assert(type.param()->toBoolParam()->value() == expectedValue);
}

void ensureParamString(const QualifiedType& type, const std::string& expectedValue) {
  assert(type.kind() == QualifiedType::PARAM);
  assert(type.type() != nullptr);
  assert(type.type()->isStringType());
  assert(type.param() != nullptr);
  assert(type.param()->isStringParam());
  assert(type.param()->toStringParam()->value() == expectedValue);
}

void ensureParamEnumStr(const QualifiedType& type, const std::string& expectedName) {
  assert(type.kind() == QualifiedType::PARAM);
  assert(type.type() != nullptr);
  assert(type.type()->isEnumType());
  assert(type.param() != nullptr);
  assert(type.param()->isEnumParam());
  assert(type.param()->toEnumParam()->value().str == expectedName);
}

void ensureErroneousType(const QualifiedType& type) {
  assert(type.type() != nullptr);
  assert(type.type()->isErroneousType());
}


QualifiedType getTypeForFirstStmt(Context* context,
                                  const std::string& program) {
  auto path = UniqueString::get(context, "input.chpl");
  setFileText(context, path, program);

  const ModuleVec& vec = parseToplevel(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 1);
  auto stmt = m->stmt(0);
  assert(stmt);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  const auto& resolvedExpr = rr.byAst(stmt);

  return resolvedExpr.type();
}

Context::Configuration getConfigWithHome() {
  std::string chpl_home;
  if (const char* chpl_home_env = getenv("CHPL_HOME")) {
    chpl_home = chpl_home_env;
  } else {
    printf("CHPL_HOME must be set");
    exit(1);
  }

  Context::Configuration config;
  config.chplHome = chpl_home;

  return config;
}

const ResolvedFunction* resolveOnlyCandidate(Context* context,
                                             const ResolvedExpression& r) {
  ResolutionContext rcval(context);
  auto rc = &rcval;
  auto msc = r.mostSpecific().only();
  if (!msc) return nullptr;

  const TypedFnSignature* sig = msc.fn();
  const PoiScope* poiScope = r.poiScope();

  return resolveFunction(rc, sig, poiScope);
}

QualifiedType findVarType(const Module* m,
                          const ResolutionResultByPostorderID& rr,
                          std::string name) {
  const Variable* var = findOnlyNamed(m, name)->toVariable();
  assert(var != nullptr);
  return rr.byAst(var).type();
}

void testDomainLiteral(Context* context, std::string domainLiteral,
                       DomainType::Kind domainKind) {
  printf("Testing: %s\n", domainLiteral.c_str());

  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  ErrorGuard guard(context);

  std::string program =
      R"""(
module M {
  var d = )""" +
      domainLiteral + R"""(;

  type i = d.idxType;
  param rk = d.isRectangular();
  param ak = d.isAssociative();
}
)""";

  auto path = UniqueString::get(context, "input.chpl");
  setFileText(context, path, std::move(program));

  const ModuleVec& vec = parseToplevel(context, path);
  const Module* m = vec[0];

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  const Variable* d = m->stmt(0)->toVariable();
  assert(d);
  assert(d->name() == "d");

  QualifiedType dQt = rr.byAst(d).type();
  assert(dQt.type());
  auto dType = dQt.type()->toDomainType();
  assert(dType);

  assert(findVarType(m, rr, "i") == dType->idxType());

  assert(dType->kind() == domainKind);
  bool isRectangular = domainKind == DomainType::Kind::Rectangular;
  ensureParamBool(findVarType(m, rr, "rk"), isRectangular);
  ensureParamBool(findVarType(m, rr, "ak"), !isRectangular);

  assert(guard.realizeErrors() == 0);
}

void testDomainIndex(Context* context, std::string domainType,
                     std::string expectedType) {
  printf("Testing: index(%s) == %s\n", domainType.c_str(),
         expectedType.c_str());

  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  ErrorGuard guard(context);

  std::string program =
      R"""(
module M {
  var d : )""" +
      domainType + R"""(;
  type t = )""" +
      expectedType + R"""(;
  type i = index(d);

  param equal = i == t;
}
)""";

  auto path = UniqueString::get(context, "input.chpl");
  setFileText(context, path, std::move(program));

  const ModuleVec& vec = parseToplevel(context, path);
  const Module* m = vec[0];

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  assert(!findVarType(m, rr, "d").isUnknownOrErroneous());
  assert(!findVarType(m, rr, "t").isUnknownOrErroneous());
  assert(!findVarType(m, rr, "i").isUnknownOrErroneous());

  ensureParamBool(findVarType(m, rr, "equal"), true);

  assert(guard.realizeErrors() == 0);
}

void testDomainBadPass(Context* context, std::string argType,
                       std::string actualType) {
  printf("Testing: cannot pass %s to %s\n", actualType.c_str(),
         argType.c_str());

  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  ErrorGuard guard(context);

  std::string program =
      R"""(
module M {
  proc foo(arg: )""" +
      argType + R"""() {
    return 42;
  }

  var d : )""" +
      actualType + R"""(;
  var c_ret = foo(d);
}
)""";
  // TODO: generic checks

  auto path = UniqueString::get(context, "input.chpl");
  setFileText(context, path, std::move(program));

  const ModuleVec& vec = parseToplevel(context, path);
  const Module* m = vec[0];

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  auto c_ret = findOnlyNamed(m, "c_ret")->toVariable();
  assert(rr.byAst(c_ret).type().isErroneousType());
  assert(guard.errors().size() == 1);
  auto& e = guard.errors()[0];
  assert(e->type() == chpl::NoMatchingCandidates);

  // 'clear' rather than 'realize' to simplify test output
  guard.clearErrors();
}

void testArrayAssign(Context* context, const char* prelude, const char* typeExpr, const char* iterable, int expectedRank, const char* expectedStride, AssociatedAction::Action actionKind, const char* expectedCopyInitFn) {
  std::string wholeString = std::string(prelude) + "\n\n" + "proc main() { var A" + typeExpr + " = " + iterable + "; }\n";
  printf("=== Resolving program: ===\n");
  printf("%s\n", wholeString.c_str());

  ErrorGuard guard(context);

  static int inputIndex = 0;

  auto filename = UniqueString::get(context, std::string("input") + std::to_string(inputIndex++) + ".chpl");
  setFileText(context, filename, wholeString);
  auto mods = parse(context, filename, UniqueString());
  assert(mods.size() == 1);
  auto& byPostorder = resolveConcreteFunction(context, mods[0]->stmt(mods[0]->numStmts()-1)->id())->resolutionById();
  auto var = findVariable(mods[0], "A");
  assert(var);
  auto& rr = byPostorder.byAst(var);

  assert(!rr.type().isUnknownOrErroneous());
  assert(rr.type().type()->isArrayType());
  auto arrT = rr.type().type()->toArrayType();
  auto domain = arrT->domainType();
  assert(!domain.isUnknownOrErroneous());
  assert(domain.type()->isDomainType());
  auto domT = domain.type()->toDomainType();
  ensureParamInt(domT->rank(), expectedRank);
  ensureParamEnumStr(domT->strides(), expectedStride);
  assert(!domT->idxType().isUnknownOrErroneous());
  assert(domT->idxType().type()->isIntType());

  bool foundCall = false;
  for (auto& aa : rr.associatedActions()) {
    if (aa.action() == actionKind) {
      foundCall = true;
      assert(aa.fn()->id().symbolPath() == expectedCopyInitFn);
    }
  }
  assert(foundCall);
  assert(!guard.realizeErrors());
}

void testArrayMaterialize(Context* context, const char* prelude, const char* iterable, int expectedRank, const char* expectedStride, const char* expectedCopyInitFn) {
  testArrayAssign(context, prelude, "", iterable, expectedRank, expectedStride, AssociatedAction::CUSTOM_COPY_INIT, expectedCopyInitFn);
}

void testArrayCoerce(Context* context, const char* prelude, const char* typeExpr, const char* iterable, int expectedRank, const char* expectedStride, const char* expectedCopyInitFn) {
  testArrayAssign(context, prelude, typeExpr, iterable, expectedRank, expectedStride, AssociatedAction::INIT_OTHER, expectedCopyInitFn);
}
