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

#include "compiler-gadgets.h"
#include "Message.h"
#include "Server.h"

namespace chpldef {

static const chpl::uast::AstNode*
chooseAstClosestToLocation(chpl::Context* chapel,
                           const chpl::uast::AstNode* astOld,
                           const chpl::uast::AstNode* astNew,
                           chpl::Location loc) {
  using namespace chpl::parsing;
  CHPL_ASSERT(astNew);
  CHPL_ASSERT(astOld != astNew);

  // If there is no new candidate then bail early.
  if (!astNew) return astOld;

  // TODO: This can run 'locateAst' on the same AST node O(n) times.
  auto locOld = astOld ? locateAst(chapel, astOld) : chpl::Location();
  auto locNew = locateAst(chapel, astNew);

  // If the new candidate has an empty location, bail early (and if the
  // old candidate's location is empty, clear it as well).
  if (!locNew) return !locOld ? nullptr : astOld;

  // A valid candidate must contain the range of interest (note that 'astOld'
  // can stay 'nullptr' along this path as long as no new AST's location
  // range contains 'loc').
  if (!locNew.contains(loc)) return astOld;

  // If there is no old candidate, the new one trivially wins because it
  // contains the range of interest.
  if (!astOld) return astNew;

  // We know that both candidates contain the range of interest. If either
  // candidate is a tighter range than the other, then it trivially wins
  // (because it must be closer to the range of interest).
  if (locNew.contains(locOld)) return astOld;
  if (locOld.contains(locNew)) return astNew;

  // Construct two LSP ranges to use for comparison.
  Range rangeOld(std::move(locOld));
  Range rangeNew(std::move(locNew));

  // At this point, candidates only partially overlap, so we have to
  // determine distance another way. Since AST are pretty granular,
  // try just preferring the lesser location (the one earlier in the
  // source code).
  auto ret = (rangeNew < rangeOld) ? astNew : astOld;

  return ret;
}

static const chpl::uast::AstNode*
astAtLocation(Server* ctx, chpl::Location loc) {
  const auto& m = ctx->withChapel(mapLinesToIdsInModule, loc.path());
  auto it = m.find(loc.firstLine());
  if (it == m.end()) return nullptr;

  ctx->trace("Found '%zu' AST nodes on line '%d'\n",
             it->second.size(),
             loc.firstLine());

  const chpl::uast::AstNode* ret = nullptr;
  for (auto id : it->second) {
    auto last = ret;
    ctx->withChapel([&](auto chapel) {
      auto ast = chpl::parsing::idToAst(chapel, id);

      if (ctx->isLogTrace()) {
        ctx->trace("Considering '%s' and '%s'\n",
                   ctx->fmt(ret).c_str(),
                   ctx->fmt(ast).c_str());
      }

      ret = chooseAstClosestToLocation(chapel, ret, ast, loc);

      if (ctx->isLogTrace()) {
        ctx->trace("Chose '%s'\n", ctx->fmt(ret).c_str());
      }

    });

    // TODO: Are we at a fixed point?
    if (ret && ret == last) break;
  }

  // Do final filtering. E.g., for 'x.   foo', was the cursor on 'foo'?
  // If we were not clicking on the field part then clear the result.
  ctx->withChapel([&](auto chapel) {
    if (auto dot = ret->toDot()) {
      auto locField = chpl::parsing::locateDotFieldWithAst(chapel, dot);
      if (!locField.contains(loc)) ret = nullptr;
    }
  });

  return ret;
}

static std::vector<chpl::ID>
sourceAstToIds(Server* ctx, const chpl::uast::AstNode* ast) {
  using namespace chpl::resolution;

  if (!ast) return {};

  const ResolvedFunction* rf = nullptr;
  const ResolutionResultByPostorderID* rr = nullptr;
  auto parentCall = ctx->withChapel(parentCallIfBaseExpression, ast);
  const bool isCallBaseExpr = (parentCall != nullptr);
  const chpl::uast::AstNode* sym = nullptr;
  bool canScopeResolveFunction = false;
  std::vector<chpl::ID> ret;

  // We need to look at the ID in order to determine the closest entity to
  // resolve, and then we need to fetch some sort of 'ResolutionResult'.
  ctx->withChapel([&](auto chapel) {
    if (auto idParent = ast->id().parentSymbolId(chapel)) {
      auto astParent = chpl::parsing::idToAst(chapel, idParent);
      CHPL_ASSERT(astParent);

      if (chpl::parsing::idIsFunction(chapel, idParent)) {
        rf = resolveConcreteFunction(chapel, idParent);
        // TODO: What do we do if the symbol is a generic function?
        if (rf == nullptr) CHPLDEF_TODO();
        canScopeResolveFunction = true;
        rr = &rf->resolutionById();
        sym = astParent;

      } else if (astParent->isModule()) {
        rr = &resolveModule(chapel, idParent);
        sym = astParent;

      } else if (astParent && astParent->isAggregateDecl()) {
        CHPLDEF_TODO();
      }
    }
  });

  if (!sym || sym == ast) CHPLDEF_TODO();

  // If we are a call base expression, inspect the parent call.
  const auto node = isCallBaseExpr ? parentCall : ast;

  // In most cases, just inspect the resolution results we were given.
  if (auto re = rr->byAstOrNull(node)) {
    auto& ms = re->mostSpecific();

    if (re->isBuiltin()) CHPLDEF_TODO();

    if (auto id = re->toId()) {
      ret.push_back(std::move(id));
    } else if (auto tfs = ms.only()) {
      ret.push_back(tfs->id());
    } else {
      CHPLDEF_TODO();
    }

  // If we failed to find something and our symbol is a function, try again
  // with just scope resolving since the info for this AST may have been
  // discarded if it is e.g., a formal's type.
  // TODO: This branch is not sufficient to cover all cases and should be
  // replaced with a new dyno query that populates the ResolutionResult
  // for formal components.
  } else if (canScopeResolveFunction) {
    auto rf = ctx->withChapel(scopeResolveFunction, sym->id());
    auto& rr = rf->resolutionById();
    if (auto re = rr.byAstOrNull(node)) {
      if (auto id = re->toId()) ret.push_back(std::move(id));
    }
  }

  return ret;
}

static std::variant<LocationArray, LocationLinkArray>
computeDeclarationPoints(Server* ctx, const TextDocumentPositionParams& p) {
  auto& uri = p.textDocument.uri;
  auto& pos = p.position;
  auto loc = ctx->withChapel(locationFromUriAndPosition, uri, pos);

  ctx->verbose("Looking for AST at '%s'\n", ctx->fmt(loc).c_str());

  auto ast = astAtLocation(ctx, loc);
  auto ids = sourceAstToIds(ctx, ast);

  ctx->verbose("Found '%zu' target IDs\n", ids.size());

  // TODO: Use this info to construct a 'LocationLinkArray' instead.
  LocationArray ret;
  for (auto& id : ids) {
    auto loc = ctx->withChapel(chpl::parsing::locateId, id);

    ctx->trace("Found entity at '%s'\n", ctx->fmt(loc).c_str());

    if (loc.isValid()) {
      Location out(loc);
      ret.push_back(std::move(out));
    }
  }

  return ret;
}

template <>
Declaration::ComputeResult
Declaration::compute(Server* ctx, ComputeParams p) {
  DeclarationResult ret;
  ret.result = computeDeclarationPoints(ctx, p);
  return ret;
}

/** This functionality is turned off in the server capabilities tab,
    because Chapel does not distinguish between a definition and a
    declaration in the way that some languages such as C/C++ do. */
template <>
Definition::ComputeResult
Definition::compute(Server* ctx, ComputeParams p) {
  DefinitionResult ret;
  ret.result = computeDeclarationPoints(ctx, p);
  return ret;
}

} // end namespace 'chpldef'
