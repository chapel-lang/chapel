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

static QualifiedType resolveViaFunction(Context* context, const AstNode* fnNode, const AstNode* node) {
  if (auto fn = fnNode->toFunction()) {

    auto resolvedFn = resolution::resolveConcreteFunction(context, fn->id());
    if (!resolvedFn) return QualifiedType();

    auto& byId = resolvedFn->resolutionById();
    if (auto res = byId.byAstOrNull(node)) {
      return res->type();
    }
  }
  return QualifiedType();
}

static QualifiedType resolveViaModule(Context* context, const AstNode* modNode, const AstNode* node) {
  if (auto mod = modNode->toModule()) {
    auto byId = resolution::resolveModule(context, mod->id());
    if (auto res = byId.byAstOrNull(node)) {
      return res->type();
    }
  }
  return QualifiedType();
}

static QualifiedType resolveResultsForNode(Context* context, const AstNode* node) {
  const AstNode* search = node;
  while (search) {
    auto qt = resolveViaFunction(context, search, node);
    if (!qt.isUnknown()) { return qt; }

    qt = resolveViaModule(context, search, node);
    if (!qt.isUnknown()) { return qt; }

    search = parsing::parentAst(context, search);
  }
  return QualifiedType();
}

QualifiedType const& typeForNode(Context* context, const AstNode* node) {
  QUERY_BEGIN(typeForNode, context, node);
  auto qt = resolveResultsForNode(context, node);
  return QUERY_END(qt);
}
