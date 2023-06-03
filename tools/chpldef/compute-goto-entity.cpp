/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#include "./Message.h"
#include "./Server.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/resolution-types.h"

namespace chpldef {

static const chpl::uast::BuilderResult&
parseFromPath(chpl::Context* chapel, chpl::UniqueString path) {
  using namespace chpl::parsing;
  chpl::UniqueString empty;
  auto& ret = parseFileToBuilderResultAndCheck(chapel, path, empty);
  return ret;
}

namespace {
  using LineToIdsMap = std::map<int, std::vector<chpl::ID>>;
}

static bool isSymbolOfInterest(const chpl::uast::AstNode* ast) {
  if (ast->isIdentifier()) return true;
  if (ast->isDot()) return true;
  return false;
}

// Preorder traversal to collect identifiers in lexical order.
static void doMapLinesInModule(chpl::Context* chapel,
                               const chpl::uast::BuilderResult& br,
                               LineToIdsMap& m,
                               const chpl::uast::AstNode* node) {
  for (auto ast : node->children()) {
    if (ast->isComment()) continue;

    if (isSymbolOfInterest(ast)) {
      auto loc = br.idToLocation(ast->id(), chpl::UniqueString());
      CHPL_ASSERT(!loc.isEmpty());
      auto& v = m[loc.firstLine()];
      v.push_back(ast->id());
    }

    bool recurse = !ast->isModule() && !ast->isLeaf();
    if (recurse) {
      doMapLinesInModule(chapel, br, m, ast);
    }
  }
}

static LineToIdsMap
mapLinesToIdsInModule(chpl::Context* chapel, chpl::UniqueString path) {
  auto& br = parseFromPath(chapel, path);
  LineToIdsMap ret;
  auto it = br.topLevelExpressions();
  for (auto ast : it) doMapLinesInModule(chapel, br, ret, ast);
  return ret;
}

static const LineToIdsMap&
mapLinesToIdsInModuleQuery(chpl::Context* chapel, chpl::UniqueString path) {
  using namespace chpl;
  QUERY_BEGIN(mapLinesToIdsInModuleQuery, chapel, path);
  auto ret = mapLinesToIdsInModule(chapel, path);
  return QUERY_END(ret);
}

static chpl::Location
chplLocFromUriAndPos(chpl::Context* chapel, const std::string& uri,
                     Position pos) {
  auto path = chpl::UniqueString::get(chapel, uri);
  int l1 = pos.line + 1;
  int c1 = pos.character + 1;
  int l2 = pos.line + 1;
  int c2 = pos.character + 1;
  return chpl::Location(path, l1, c1, l2, c2);
}

static const chpl::uast::AstNode*
chooseAstClosestToLocation(chpl::Context* chapel,
                           const chpl::uast::AstNode* astOld,
                           const chpl::uast::AstNode* astNew,
                           chpl::Location loc) {
  using namespace chpl::parsing;
  CHPL_ASSERT(astNew);
  CHPL_ASSERT(astOld != astNew);

  auto locOld = astOld ? locateAst(chapel, astOld) : chpl::Location();
  auto locNew = locateAst(chapel, astNew);
  if (!locNew.contains(loc)) return astOld;
  if (!astOld) return astNew;

  // Prefer the location furthest towards the end of the range.
  auto ret = (locNew > locOld) ? astNew : astOld;
  return ret;
}

static const chpl::uast::AstNode*
astAtLocation(Server* ctx, chpl::Location loc) {
  const auto& m = ctx->withChapel(mapLinesToIdsInModuleQuery,
                                  loc.path());
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
    if (ret == last) break;
  }

  return ret;
}

static const chpl::uast::Module*
closestModule(chpl::Context* chapel, const chpl::uast::AstNode* ast) {
  if (auto mod = ast->toModule()) return mod;
  if (auto id = chpl::parsing::idToParentModule(chapel, ast->id())) {
    auto mod = chpl::parsing::idToAst(chapel, id);
    return mod->toModule();
  }
  return nullptr;
}

static const chpl::resolution::ResolutionResultByPostorderID&
resolveModule(Server* ctx, chpl::ID idMod) {
  return ctx->withChapel(chpl::resolution::resolveModule, idMod);
}

static std::vector<chpl::ID>
sourceAstToIds(Server* ctx, const chpl::uast::AstNode* ast) {
  std::vector<chpl::ID> ret;

  if (!ast) return ret;

  auto mod = ctx->withChapel(closestModule, ast);
  CHPL_ASSERT(mod);

  if (mod == ast) CHPLDEF_TODO();

  ctx->trace("Target '%s' in containing module '%s'\n",
             ctx->fmt(ast).c_str(),
             ctx->fmt(mod).c_str());

  auto& rr = resolveModule(ctx, mod->id());
  auto& re = rr.byAst(ast);

  if (re.isBuiltin()) CHPLDEF_TODO();

  ctx->trace("Encountered '%zu' errors resolving\n",
             ctx->errorHandler()->numErrors());

  // First do a simple check for target location.
  if (auto idTarget = re.toId()) {
    ret.push_back(idTarget);

  // Else, more work is required...
  } else {
    CHPLDEF_TODO();
  }

  return ret;
}

static bool chplLocIsValid(const chpl::Location& loc) {
  return !loc.isEmpty() && loc.firstLine() >= 1 &&
        loc.firstColumn() >= 1 &&
        loc.lastLine() >= 1 &&
        loc.lastColumn() >= 1;
}

static std::variant<LocationArray, LocationLinkArray>
computeDeclarationPoints(Server* ctx, const TextDocumentPositionParams& p) {
  auto& uri = p.textDocument.uri;
  auto& pos = p.position;
  auto loc = ctx->withChapel(chplLocFromUriAndPos, uri, pos);

  ctx->verbose("Looking for AST at '%s'\n", ctx->fmt(loc).c_str());

  auto ast = astAtLocation(ctx, loc);
  auto ids = sourceAstToIds(ctx, ast);

  ctx->verbose("Found '%zu' target IDs\n", ids.size());

  // TODO: Use this info to construct a 'LocationLinkArray' instead.
  LocationArray ret;
  for (auto& id : ids) {
    auto loc = ctx->withChapel(chpl::parsing::locateId, id);

    ctx->trace("Found entity at '%s'\n", ctx->fmt(loc).c_str());

    if (chplLocIsValid(loc)) {
      Position start = { static_cast<uint64_t>(loc.firstLine()-1),
                         static_cast<uint64_t>(loc.firstColumn()-1) };
      Position end = { static_cast<uint64_t>(loc.lastLine()-1),
                       static_cast<uint64_t>(loc.lastColumn()-1) };
      Location out;
      out.range = { start, end };
      out.uri = loc.path().str();

      ret.push_back(std::move(out));
    }
  }

  return ret;
}

Declaration::ComputedResult
Declaration::compute(Server* ctx, const Params& p) {
  DeclarationResult ret;
  ret.result = computeDeclarationPoints(ctx, p);
  return ret;
}

/** This functionality is turned off in the server capabilities tab,
    because Chapel does not distinguish between a definition and a
    declaration in the way that some languages such as C/C++ do. */
Definition::ComputedResult
Definition::compute(Server* ctx, const Params& p) {
  DefinitionResult ret;
  ret.result = computeDeclarationPoints(ctx, p);
  return ret;
}

} // end namespace 'chpldef'
