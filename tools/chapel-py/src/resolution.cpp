/*
 * Copyright 2024 Hewlett Packard Enterprise Development LP
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
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/framework/query-impl.h"

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

// For scope resolution, we handle AST nodes that don't necessarily get
// their own ResolvedExpression (specificlaly, uses/imports), so return an ID
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
