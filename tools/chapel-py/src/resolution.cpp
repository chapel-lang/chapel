/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

#include "resolution.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/framework/query-impl.h"
#include "chpl/types/ClassType.h"
#include "chpl/uast/all-uast.h"

using namespace chpl;
using namespace uast;
using namespace types;

static const ID scopeResolveViaVisibilityStmt(Context* context, const AstNode* visibilityStmt, const AstNode* node) {
  if (visibilityStmt->isUse() || visibilityStmt->isImport()) {
    auto useParent = parsing::parentAst(context, visibilityStmt);
    auto scope = resolution::scopeForId(context, useParent->id());
    auto reScope = resolution::resolveVisibilityStmts(context, scope);

    // Iterate and re-assign the ID if there are multiple visibility clauses.
    // The reason is that the first 'visibility clause ID' might be too broad
    // and contain the others.
    ID toReturn;
    for (auto visCla: reScope->visibilityClauses()) {
      if(visCla.visibilityClauseId().contains(node->id())) {
        // A visibility cluse includes the symbol-to-be-imported and the
        // 'limitations' (x as y etc.). We'll need to get them from the AST.
        auto visClaKind = visCla.kind();
        auto visClaAst =
          parsing::idToAst(context, visCla.visibilityClauseId())->toVisibilityClause();

        if (visClaKind == resolution::VisibilitySymbols::SYMBOL_ONLY ||
            visClaKind == resolution::VisibilitySymbols::ALL_CONTENTS) {
          // the cursor is over 'A' or 'B' in 'import A' or 'import A as B'.
          // The 'definition' is for the scope.
          toReturn = visCla.scope()->id();
        } else if (visClaKind == resolution::VisibilitySymbols::ONLY_CONTENTS ||
                   visClaKind == resolution::VisibilitySymbols::CONTENTS_EXCEPT) {
          if (visClaAst->symbol()->contains(node)) {
            // The cursor is over the symbol, like 'IO' in 'use IO only ioMode'.
            // Just return the scope ID.
            toReturn = visCla.scope()->id();
            continue;
          }

          // The cursor is over a limitation such as 'ioMode' in 'use IO only ioMode'.
          // Find which limitation.
          const AstNode* limitation = nullptr;
          for (auto l : visClaAst->limitations()) {
            if (l->contains(node)) {
              limitation = l;
              break;
            }
          }

          // Couldn't find one; give up.
          if (!limitation) return ID();

          // Pattern match on the limitation to see what to look up in the scope.
          // E.g., if it's 'IO only x as y', we want to look up 'x'.
          const Identifier* identToLookUp = nullptr;
          if (auto limAs = limitation->toAs()) {
            identToLookUp = limAs->symbol()->toIdentifier();
          } else if (auto limIdent = limitation->toIdentifier()) {
            identToLookUp = limIdent;
          }

          if (identToLookUp) {
            auto config = chpl::resolution::IDENTIFIER_LOOKUP_CONFIG |
                          resolution::LOOKUP_SKIP_PRIVATE_VIS;
            auto ids =
              resolution::lookupNameInScope(context, visCla.scope(),
                                            /* methodLookupHelper */ nullptr,
                                            /* receiverScopeHelper */ nullptr,
                                            identToLookUp->name(),
                                            config);

            if (ids.isEmpty()) return ID();
            toReturn = ids.firstId();
          }
        }
      }
    }
    return toReturn;
  }
  return ID();
}

static const resolution::ResolvedExpression* scopeResolveViaFunction(Context* context, const AstNode* fnNode, const AstNode* node) {
  if (auto fn = fnNode->toFunction()) {
    auto& byId =
        resolution::scopeResolveFunction(context, fn->id())->resolutionById();
    if (auto res = byId.byAstOrNull(node)) {
      return res;
    }
  }
  return nullptr;
}

static const resolution::ResolvedExpression* scopeResolveViaModule(Context* context, const AstNode* modNode, const AstNode* node) {
  if (auto mod = modNode->toModule()) {
    auto& byId = resolution::scopeResolveModule(context, mod->id());
    if (auto res = byId.byAstOrNull(node)) {
      return res;
    }
  }
  return nullptr;
}

static const resolution::ResolvedExpression*
scopeResolveViaAggregate(Context* context,
                         const AstNode* aggNode,
                         const AstNode* node) {
  if (auto agg = aggNode->toAggregateDecl()) {
    auto& byId = resolution::scopeResolveAggregate(context, agg->id());
    if (auto res = byId.byAstOrNull(node)) {
      return res;
    }
  }
  return nullptr;
}

static const resolution::ResolvedExpression*
scopeResolveViaEnum(Context* context, const AstNode* enumNode, const AstNode* node) {
  if (auto enumDecl = enumNode->toEnum()) {
    auto& byId = resolution::scopeResolveEnum(context, enumDecl->id());
    if (auto res = byId.byAstOrNull(node)) {
      return res;
    }
  }
  return nullptr;
}

static const ID scopeResolveViaInclude(Context* context,
                                 const AstNode* includeNode,
                                 const AstNode* node) {
  if (auto include = includeNode->toInclude())
    if (auto submodule = parsing::getIncludedSubmodule(context, include->id()))
      return submodule->id();
  return ID();
}

// For scope resolution, we handle AST nodes that don't necessarily get
// their own ResolvedExpression (specifically, uses/imports), so return an ID
// instead of a ResolvedExpression.
static const ID scopeResolveToIdForNode(Context* context, const AstNode* node) {
  const AstNode* search = node;
  while (search) {
    if (auto rr = scopeResolveViaFunction(context, search, node)) {
      return rr->toId();
    } else if (auto rr = scopeResolveViaAggregate(context, search, node)) {
      return rr->toId();
    } else if (auto rr = scopeResolveViaEnum(context, search, node)) {
      return rr->toId();
    } else if (auto rr = scopeResolveViaModule(context, search, node)) {
      return rr->toId();
    } else if(auto id = scopeResolveViaVisibilityStmt(context, search, node)) {
      return id;
    } else if (auto id = scopeResolveViaInclude(context, search, node)) {
      return id;
    }
    search = parsing::parentAst(context, search);
  }
  return ID();
}

const resolution::ResolvedExpression* scopeResolveResultsForNode(Context* context, const AstNode* node) {
  const AstNode* search = node;
  while (search) {
    if (auto rr = scopeResolveViaFunction(context, search, node)) {
      return rr;
    } else if (auto rr = scopeResolveViaModule(context, search, node)) {
      return rr;
    }
    search = parsing::parentAst(context, search);
  }
  return nullptr;
}

static const AstNode* idOrEmptyToAstNodeOrNull(Context* context, const ID& id) {
  if (id.isEmpty()) return nullptr;

  return parsing::idToAst(context, id);
}

const AstNode* const& nodeOrNullFromToId(Context* context, const AstNode* node) {
  QUERY_BEGIN(nodeOrNullFromToId, context, node);
  auto id = scopeResolveToIdForNode(context, node);
  auto nodeOrNull = idOrEmptyToAstNodeOrNull(context, id);
  return QUERY_END(nodeOrNull);
}

static const resolution::ResolvedExpression* resolveViaFunction(Context* context, const AstNode* fnNode, const AstNode* node) {
  if (auto fn = fnNode->toFunction()) {

    auto resolvedFn = resolution::resolveConcreteFunction(context, fn->id());
    if (!resolvedFn) return nullptr;

    auto& byId = resolvedFn->resolutionById();
    if (auto res = byId.byAstOrNull(node)) {
      return res;
    }
  }
  return nullptr;
}

static const resolution::ResolvedExpression* resolveViaModule(Context* context, const AstNode* modNode, const AstNode* node) {
  if (auto mod = modNode->toModule()) {
    auto& byId = resolution::resolveModule(context, mod->id());
    if (auto res = byId.byAstOrNull(node)) {
      return res;
    }
  }
  return nullptr;
}

const resolution::ResolvedExpression* resolveResultsForNode(Context* context, const AstNode* node) {
  const AstNode* search = node;
  while (search) {
    auto rr = resolveViaFunction(context, search, node);
    if (rr) { return rr; }

    rr = resolveViaModule(context, search, node);
    if (rr) { return rr; }

    search = parsing::parentAst(context, search);
  }
  return nullptr;
}

QualifiedType const& typeForNode(Context* context, const AstNode* node) {
  QUERY_BEGIN(typeForNode, context, node);

  auto rr = resolveResultsForNode(context, node);
  QualifiedType qt;
  if (rr) {
    qt = rr->type();
  }

  return QUERY_END(qt);
}

const AstNode* const& calledFnForNode(Context* context, const AstNode* node) {
  QUERY_BEGIN(calledFnForNode, context, node);

  auto rr = resolveResultsForNode(context, node);
  ID id;
  if (rr) {
    if (const auto& only = rr->mostSpecific().only()) {
      id = only.fn()->id();
    }
  }

  auto res = idOrEmptyToAstNodeOrNull(context, id);
  return QUERY_END(res);
}

std::vector<int> const& actualOrderForNode(Context* context, const AstNode* node) {
  QUERY_BEGIN(actualOrderForNode, context, node);

  auto rr = resolveResultsForNode(context, node);
  std::vector<int> res;
  if (rr) {
    if (const auto& only = rr->mostSpecific().only()) {
      int i = 0;
      while (auto formalActual = only.formalActualMap().byActualIdx(i++)) {
        res.push_back(formalActual->formalIdx());
      }
    }
  }

  return QUERY_END(res);
}


static inline ID parseAndGetSymbolIdFromTopLevelModule(Context* context,
                                                       const char* modName,
                                                       const char* symName) {
  using namespace parsing;
  // make sure the module is parsed before we try to look up the symbol
  if (!getToplevelModule(context, UniqueString::get(context, modName))) {
    return ID();
  }
  if (auto TestID = getSymbolIdFromTopLevelModule(context, modName, symName)) {
    return TestID;
  }
  return ID();
}

struct TestFunctionFinder {
  // Inputs
  Context* context;
  const QualifiedType& testType;

  // Outputs
  std::vector<const Function*> fns;

  TestFunctionFinder(Context* context, const QualifiedType& testType)
    : context(context), testType(testType), fns() {}

  bool enter(const Function* fn) {
    // We're looking for a non-method function that throws and accepts
    // a single argument (which we will soon validate to be of the testType).
    if (!fn->throws() || fn->isMethod() || fn->numFormals() != 1) return false;
    // Skip non-user functions.
    if (parsing::idIsInBundledModule(context, fn->id())) return false;

    using namespace resolution;

    // The function also needs to be concrete.
    ResolutionContext rcval(context);
    const UntypedFnSignature* uSig = UntypedFnSignature::get(context, fn);
    const TypedFnSignature* sig = typedSignatureInitial(&rcval, uSig);
    if (sig == nullptr || sig->needsInstantiation()) return false;

    if (canPass(context, testType, sig->formalType(0)).passes()) {
      // One formal of 'testType', which constitutes a test.
      fns.push_back(fn);
    }

    return false;
  }
  void exit(const Function* fn) {}

  bool enter(const AstNode* node) { return true; }
  void exit(const AstNode* node) {}
};

std::vector<const Function*> const&
findTestFunctionsForModule(Context* context, const Module* mod) {
  QUERY_BEGIN(findTestFunctionsForModule, context, mod);
  std::vector<const Function*> result;

  if (auto TestID =
    parseAndGetSymbolIdFromTopLevelModule(context, "UnitTest", "Test")) {
    auto TestTy = resolution::initialTypeForTypeDecl(context, TestID);
    if (TestTy && TestTy->isClassType()) {
      auto borrowedDec = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
      auto borrowedTestTy = TestTy->toClassType()->withDecorator(context, borrowedDec);
      auto borrowedTestQualTy = QualifiedType(QualifiedType::DEFAULT_INTENT, borrowedTestTy);

      TestFunctionFinder tff(context, borrowedTestQualTy);
      mod->traverse(tff);
      result = tff.fns;
    }
  }
  return QUERY_END(result);
}

struct TestFunctionMainFinder {
  // Inputs
  Context* context;
  ID mainID;

  // Outputs
  const FnCall* callsMain;

  TestFunctionMainFinder(Context* context, ID mainID)
    : context(context), mainID(mainID), callsMain(nullptr) {}

  bool enter(const FnCall* call) {
    if (callsMain != nullptr) return false;
    auto resolvedID = scopeResolveToIdForNode(context, call->calledExpression());
    if (resolvedID == mainID) {
      callsMain = call;
      return false;
    }
    return true;
  }
  void exit(const Function* fn) {}

  bool enter(const AstNode* node) { return callsMain == nullptr; }
  void exit(const AstNode* node) {}
};

const FnCall* const&
findUnitTestMainForModule(Context* context, const Module* mod) {
  QUERY_BEGIN(findUnitTestMainForModule, context, mod);
  const FnCall* result = nullptr;

  if (auto mainID =
      parseAndGetSymbolIdFromTopLevelModule(context, "UnitTest", "main")) {
    TestFunctionMainFinder tfmf(context, mainID);
    mod->traverse(tfmf);
    result = tfmf.callsMain;
  }
  return QUERY_END(result);
}
