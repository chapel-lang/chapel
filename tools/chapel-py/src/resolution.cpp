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
    for (auto visCla: reScope->visibilityClauses()) {
      if(visCla.visibilityClauseId().contains(node->id())) {
        return visCla.scope()->id();
      }
    }
  }
  return ID();
}

static const ID scopeResolveViaFunction(Context* context, const AstNode* fnNode, const AstNode* node) {
  if (auto fn = fnNode->toFunction()) {
    auto& byId =
        resolution::scopeResolveFunction(context, fn->id())->resolutionById();
    if (auto res = byId.byAstOrNull(node)) {
      return res->toId();
    }
  }
  return ID();
}

static const ID scopeResolveViaModule(Context* context, const AstNode* modNode, const AstNode* node) {
  if (auto mod = modNode->toModule()) {
    auto& byId = resolution::scopeResolveModule(context, mod->id());
    if (auto res = byId.byAstOrNull(node)) {
      return res->toId();
    }
  }
  return ID();
}

static const ID scopeResolveResultsForNode(Context* context, const AstNode* node) {
  const AstNode* search = node;
  while (search) {
    if (auto id = scopeResolveViaFunction(context, search, node)) {
      return id;
    } else if (auto id = scopeResolveViaModule(context, search, node)) {
      return id;
    } else if(auto id = scopeResolveViaVisibilityStmt(context, search, node)) {
      return id;
    }
    search = parsing::parentAst(context, search);
  }
  return ID();
}

static const AstNode* idOrEmptyToAstNodeOrNull(Context* context, const ID& id) {
  if (id.isEmpty()) return nullptr;

  return parsing::idToAst(context, id);
}

const AstNode* const& nodeOrNullFromToId(Context* context, const AstNode* node) {
  QUERY_BEGIN(nodeOrNullFromToId, context, node);
  auto id = scopeResolveResultsForNode(context, node);
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
