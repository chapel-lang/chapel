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

#include "chpl/uast/ASTBase.h"

#include "chpl/uast/Decl.h"
#include "chpl/uast/Exp.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Sym.h"

namespace chpl {
namespace uast {

ASTBase::ASTBase(ASTTag tag)
  : tag_(tag), id_(), children_() {
}

ASTBase::ASTBase(ASTTag tag, ASTList children)
  : tag_(tag), id_(), children_(std::move(children)) {
}


ASTBase::~ASTBase() {
}

bool ASTBase::shallowMatch(const ASTBase* other) const {
  const ASTBase* lhs = this;
  const ASTBase* rhs = other;
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
      lhsDecl->sym()->name() != rhsDecl->sym()->name())
    return false;

  return true;
}

bool ASTBase::updateAST(owned<ASTBase>& keep, owned<ASTBase>& addin) {
  printf("IN updateAST keep is\n");
  ASTBase::dump(keep.get(), 2);
  printf("IN updateAST addin is\n");
  ASTBase::dump(addin.get(), 2);
  printf("\n");

  if (keep->shallowMatch(addin.get())) {
    printf("Shallow Match\n");
    // run updateASTList on the child list
    return updateASTList(keep->children_, addin->children_);
  } else {
    // swap the AST
    keep.swap(addin);
    return false;
  }
}

void ASTBase::markAST(Context* context, const ASTBase* keep) {
  if (keep == nullptr) return;
  // mark the unique string stored in the ID
  keep->id_.markUniqueStrings(context);
  // run markUniqueStrings on the node
  keep->markUniqueStringsInner(context);
  // run markASTList on the child list
  markASTList(context, keep->children_);
}

static void dumpHelper(const ASTBase* ast, int depth) {
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
  if (ast == nullptr) {
    printf("nullptr\n");
    return;
  }
  ID emptyId;
  if (ast->id() != emptyId) {
    if (const Sym* sym = ast->toSym()) {
      printf("%s %s %p %s\n",
             asttags::tagToString(ast->tag()),
             ast->id().symbolPath().c_str(),
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
  for (const ASTBase* child : ast->children()) {
    dumpHelper(child, depth+1);
  }
}

void ASTBase::dump(const ASTBase* ast, int leadingSpaces) {
  dumpHelper(ast, leadingSpaces);
}


} // end namespace uast
} // end namespace chpl
