/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#include "chpl/AST/BaseAST.h"

#include "chpl/AST/Decl.h"
#include "chpl/AST/Expr.h"
#include "chpl/AST/Identifier.h"
#include "chpl/AST/Symbol.h"

namespace chpl {
namespace ast {

BaseAST::BaseAST(asttags::ASTTag tag)
  : tag_(tag), id_(), children_() {
}

BaseAST::BaseAST(asttags::ASTTag tag, ASTList children)
  : tag_(tag), id_(), children_(std::move(children)) {
}


BaseAST::~BaseAST() {
}

bool BaseAST::shallowMatch(const BaseAST* other) const {
  const BaseAST* lhs = this;
  const BaseAST* rhs = other;
  if (lhs->tag() != rhs->tag())
    return false;
  if (lhs->id().symbolPath() != rhs->id().symbolPath())
    return false;
  if (!lhs->contentsMatchInner(rhs))
    return false;
  // Also check declaration names
  const Decl* lhsDecl = lhs->toDecl();
  const Decl* rhsDecl = rhs->toDecl();
  if (lhsDecl && rhsDecl &&
      lhsDecl->symbol()->name() != rhsDecl->symbol()->name())
    return false;

  return true;
}

/*
bool BaseAST::contentsMatch(const BaseAST* other) const {
  const BaseAST* lhs = this;
  const BaseAST* rhs = other;

  // check basics of the node itself
  if (!lhs->shallowMatch(other))
    return false;

  // check the children match
  int lhsN = lhs->numChildren();
  for (int i = 0; i < lhsN; i++) {
    if (!lhs->getChild(i)->contentsMatch(rhs->getChild(i)))
      return false;
  }

  return true;
}
*/

bool BaseAST::combineAST(owned<BaseAST>& keep, owned<BaseAST>& addin) {
  if (keep->shallowMatch(addin.get())) {
    // run combineAST on the child list
    return combineASTLists(keep->children_, addin->children_);
  } else {
    // swap the AST
    keep.swap(addin);
    return false;
  }
}

static void dumpHelper(const BaseAST* ast, int depth) {
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
  if (ast == nullptr) {
    printf("nullptr\n");
    return;
  }
  ID emptyId;
  if (ast->id() != emptyId) {
    if (const Symbol* sym = ast->toSymbol()) {
      printf("%s %s@%i %p %s\n",
             asttags::tagToString(ast->tag()),
             ast->id().symbolPath().c_str(),
             ast->id().postOrderId(),
             ast,
             sym->name().c_str());
    } else if (const Identifier* ident = ast->toIdentifier()) {
      printf("%s %s@%i %p %s\n",
             asttags::tagToString(ast->tag()),
             ast->id().symbolPath().c_str(),
             ast->id().postOrderId(),
             ast,
             ident->name().c_str());
    } else {
      printf("%s %s@%i %p\n",
             asttags::tagToString(ast->tag()),
             ast->id().symbolPath().c_str(),
             ast->id().postOrderId(),
             ast);
    }
  } else {
    printf("%s <no id> %p\n",
           asttags::tagToString(ast->tag()),
           ast);
  }
  int nChildren = ast->numChildren();
  for (int i = 0; i < nChildren; i++) {
    dumpHelper(ast->child(i), depth+1);
  }
}

void BaseAST::dump(const BaseAST* ast) {
  dumpHelper(ast, 0);
}


} // end namespace ast
} // end namespace chpl
