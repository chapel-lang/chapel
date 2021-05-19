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

#include "chpl/uast/ASTNode.h"

#include "chpl/uast/NamedDecl.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"

namespace chpl {
namespace uast {


ASTNode::~ASTNode() {
}

bool ASTNode::shallowMatch(const ASTNode* other) const {
  const ASTNode* lhs = this;
  const ASTNode* rhs = other;
  if (lhs->tag() != rhs->tag())
    return false;
  if (lhs->id().symbolPath() != rhs->id().symbolPath())
    return false;
  if (!lhs->contentsMatchInner(rhs))
    return false;

  return true;
}

bool ASTNode::completeMatch(const ASTNode* other) const {
  const ASTNode* lhs = this;
  const ASTNode* rhs = other;

  // check the node itself
  if (!lhs->shallowMatch(rhs)) {
    return false;
  }

  // check the numeric elements of the id and the number of children
  if (lhs->id().postOrderId() != rhs->id().postOrderId() ||
      lhs->id().numContainedChildren() != rhs->id().numContainedChildren() ||
      lhs->children_.size() != rhs->children_.size()) {
    return false;
  }

  // check the child nodes recursively
  bool allMatch = true;
  size_t nChildren = lhs->children_.size();
  for (size_t i = 0; i < nChildren; i++) {
    const ASTNode* lhsChild = lhs->children_[i].get();
    const ASTNode* rhsChild = rhs->children_[i].get();
    bool childMatch = lhsChild->completeMatch(rhsChild);
    if (!childMatch) {
      allMatch = false;
      break;
    }
  }
  return allMatch;
}

bool ASTNode::updateAST(owned<ASTNode>& keep, owned<ASTNode>& addin) {
  // If any of the children changed, it's important to create
  // a new AST node for the parent, so that any queries referring
  // to that AST node get a new version.
  //
  // This might come up with a Module where one declaration in its body
  // changed.
  //
  // However, a Function contained in that module that did not change
  // is OK to reuse that. In other words, nodes can be reused up to
  // the level that there are no changes contained within them, as a tree.
  //
  // This matches a sort of "isolated from above" property.

  if (keep->completeMatch(addin.get())) {
    // no changes are necessary
    return false;
  } else if (keep->shallowMatch(addin.get())) {
    // always get a new pointer for the node
    keep.swap(addin);
    // after that, keep.children_ is the child nodes from addin

    keep->children_.swap(addin->children_);
    // now keep is a new pointer
    // keep.children are the child nodes from keep

    updateASTList(keep->children_, addin->children_);
    return true; // updated
  } else {
    // swap the AST
    keep.swap(addin);
    return true;
  }
}

void ASTNode::markAST(Context* context, const ASTNode* keep) {
  if (keep == nullptr) return;
  // mark the unique string stored in the ID
  keep->id_.markUniqueStrings(context);
  // run markUniqueStrings on the node
  keep->markUniqueStringsInner(context);
  // run markASTList on the child list
  markASTList(context, keep->children_);
}

static void dumpHelper(const ASTNode* ast, int depth) {
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
  if (ast == nullptr) {
    printf("nullptr\n");
    return;
  }
  ID emptyId;
  if (ast->id() != emptyId) {
    if (const NamedDecl* named = ast->toNamedDecl()) {
      printf("%s %s %p %s\n",
             asttags::tagToString(ast->tag()),
             ast->id().symbolPath().c_str(),
             ast,
             named->name().c_str());
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
  for (const ASTNode* child : ast->children()) {
    dumpHelper(child, depth+1);
  }
}

void ASTNode::dump(const ASTNode* ast, int leadingSpaces) {
  dumpHelper(ast, leadingSpaces);
}


} // end namespace uast
} // end namespace chpl
