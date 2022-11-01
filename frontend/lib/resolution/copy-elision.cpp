/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/copy-elision.h"

#include "Resolver.h"

#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/all-uast.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

struct FindElidedCopies {
  using RV = ResolvedVisitor<FindElidedCopies>;

  // inputs to the process
  Context* context = nullptr;

  // result of the process
  std::set<ID> elidedCopyFromIds;

  // internal variables

  // main entry point to this code
  static void process(Resolver& resolver, std::set<ID>& elidedCopyFromIds);

  FindElidedCopies(Context* context);

  void handleMention(ID dstId);
  void handleInitOrAssign(ID dstId);
  void enterScope(const uast::AstNode* ast);
  void exitScope(const uast::AstNode* ast);

  bool enter(const VarLikeDecl* ast, RV& rv);
  void exit(const VarLikeDecl* ast, RV& rv);

  bool enter(const OpCall* ast, RV& rv);
  void exit(const OpCall* ast, RV& rv);

  bool enter(const FnCall* ast, RV& rv);
  void exit(const FnCall* ast, RV& rv);

  bool enter(const Identifier* ast, RV& rv);
  void exit(const Identifier* ast, RV& rv);

  bool enter(const uast::AstNode* node, RV& rv);
  void exit(const uast::AstNode* node, RV& rv);
};

FindElidedCopies::FindElidedCopies(Context* context)
  : context(context)
{
}

void FindElidedCopies::handleMention(ID dstId) {
  // TODO
}

void FindElidedCopies::handleInitOrAssign(ID dstId) {
  // TODO
}

void FindElidedCopies::enterScope(const AstNode* ast) {
  // TODO
}

void FindElidedCopies::exitScope(const AstNode* ast) {
  // TODO
}

bool FindElidedCopies::enter(const VarLikeDecl* ast, RV& rv) {

  enterScope(ast);

  return true;
}
void FindElidedCopies::exit(const VarLikeDecl* ast, RV& rv) {
  // TODO -- consider init= becoming a move

  exitScope(ast);
}

bool FindElidedCopies::enter(const OpCall* ast, RV& rv) {

  if (ast->op() == USTR("=")) {
    // What is the RHS and LHS of the '=' call?
    auto lhsAst = ast->actual(0);
    auto rhsAst = ast->actual(1);

    // visit the RHS first
    rhsAst->traverse(rv);

    // now consider the LHS
    ID toId;
    if (rv.hasAst(lhsAst)) {
      toId = rv.byAst(lhsAst).toId();
    }

    if (!toId.isEmpty()) {
      handleInitOrAssign(toId);
    } else {
      // visit the LHS to check for mentions
      lhsAst->traverse(rv);
    }
  }

  return false;
}

void FindElidedCopies::exit(const OpCall* ast, RV& rv) {
}

bool FindElidedCopies::enter(const FnCall* callAst, RV& rv) {

  // TODO: consider 'in' and 'out' formals

  return false;
}

void FindElidedCopies::exit(const FnCall* ast, RV& rv) {
}


bool FindElidedCopies::enter(const Identifier* ast, RV& rv) {
  return true;
}
void FindElidedCopies::exit(const Identifier* ast, RV& rv) {
  // TODO: handle mentions
}


bool FindElidedCopies::enter(const AstNode* ast, RV& rv) {
  enterScope(ast);

  return true;
}
void FindElidedCopies::exit(const AstNode* ast, RV& rv) {
  exitScope(ast);
}

std::set<ID>
computeElidedCopies(Context* context,
                    const uast::AstNode* symbol,
                    const ResolutionResultByPostorderID& byPostorder) {
  std::set<ID> elidedCopyFromIds;
  FindElidedCopies uv(context);

  ResolvedVisitor<FindElidedCopies> rv(context,
                                       symbol,
                                       uv,
                                       byPostorder);

  // Traverse formals and then the body. This is done here rather
  // than in enter(Function) because nested functions will have
  // computeElidedCopies called on them separately.
  if (auto fn = symbol->toFunction()) {
    // traverse formals and then traverse the body
    if (auto body = fn->body()) {
      // make a pretend scope for the formals
      uv.enterScope(body);

      // traverse the formals
      for (auto formal : fn->formals()) {
        formal->traverse(rv);
      }

      // traverse the real body
      body->traverse(rv);

      uv.exitScope(body);
    }
  } else {
    symbol->traverse(rv);
  }

  // swap the result into place
  elidedCopyFromIds.swap(uv.elidedCopyFromIds);

  return elidedCopyFromIds;
}


} // end namespace resolution
} // end namespace chpl
