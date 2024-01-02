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

namespace chpldef {

const chpl::uast::BuilderResult&
parseFromUri(chpl::Context* chapel, chpl::UniqueString uri) {
  using namespace chpl::parsing;
  chpl::UniqueString empty;
  auto& ret = parseFileToBuilderResultAndCheck(chapel, uri, empty);
  return ret;
}

const chpl::uast::BuilderResult&
parseFromUri(chpl::Context* chapel, const std::string& uri) {
  auto ustr = chpl::UniqueString::get(chapel, uri);
  return parseFromUri(chapel, ustr);
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

    // Push back the occupied lines. This is usually just one line but can
    // be up to 3 in the case of a dot expression.
    if (isSymbolOfInterest(ast)) {
      auto loc = br.idToLocation(ast->id(), chpl::UniqueString());
      CHPL_ASSERT(!loc.isEmpty());
      for (int i = loc.firstLine(); i <= loc.lastLine(); i++) {
        auto& v = m[i];
        v.push_back(ast->id());
      }
    }

    bool recurse = !ast->isModule() && !ast->isLeaf();
    if (recurse) {
      doMapLinesInModule(chapel, br, m, ast);
    }
  }
}

static LineToIdsMap
mapLinesToIdsInModuleImpl(chpl::Context* chapel, chpl::UniqueString uri) {
  auto& br = parseFromUri(chapel, uri);
  LineToIdsMap ret;
  auto it = br.topLevelExpressions();
  for (auto ast : it) doMapLinesInModule(chapel, br, ret, ast);
  return ret;
}

const LineToIdsMap&
mapLinesToIdsInModule(chpl::Context* chapel, chpl::UniqueString uri) {
  using namespace chpl;
  QUERY_BEGIN(mapLinesToIdsInModule, chapel, uri);
  auto ret = mapLinesToIdsInModuleImpl(chapel, uri);
  return QUERY_END(ret);
}

chpl::Location
locationFromUriAndPosition(chpl::Context* chapel, const std::string& uri,
                           const Position& pos) {
  auto ustr = chpl::UniqueString::get(chapel, uri);
  int l1 = pos.line + 1;
  int c1 = pos.character + 1;
  int l2 = pos.line + 1;
  int c2 = pos.character + 1;
  return chpl::Location(ustr, l1, c1, l2, c2);
}

const chpl::uast::FnCall*
parentCallIfBaseExpression(chpl::Context* chapel,
                           const chpl::uast::AstNode* ast) {
  if (!ast) return nullptr;
  auto p = chpl::parsing::parentAst(chapel, ast);
  if (!p) return nullptr;

  if (auto call = p->toFnCall())
    if (auto ce = call->calledExpression())
      if (ce == ast) return call;
  return nullptr;
}

} // end namespace 'chpldef'
