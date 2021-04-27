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

#include "chpl/AST/ASTBase.h"

#include "chpl/AST/Decl.h"
#include "chpl/AST/Exp.h"
#include "chpl/AST/Identifier.h"
#include "chpl/AST/Sym.h"

namespace chpl {
namespace ast {

ASTBase::ASTBase(asttags::ASTTag tag)
  : tag_(tag), id_(), children_() {
}

ASTBase::ASTBase(asttags::ASTTag tag, ASTList children)
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
  if (keep->shallowMatch(addin.get())) {
    // run updateASTList on the child list
    return updateASTList(keep->children_, addin->children_);
  } else {
    // swap the AST
    keep.swap(addin);
    return false;
  }
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
  for (const ASTBase* child : ast->children()) {
    dumpHelper(child, depth+1);
  }
}

void ASTBase::dump(const ASTBase* ast) {
  dumpHelper(ast, 0);
}


} // end namespace ast
} // end namespace chpl
