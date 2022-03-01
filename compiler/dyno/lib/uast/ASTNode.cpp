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

#include "chpl/uast/ASTNode.h"


#include "chpl/uast/chpl-syntax-printer.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Expression.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/NamedDecl.h"

#include <iomanip>

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

bool ASTNode::update(owned<ASTNode>& keep, owned<ASTNode>& addin) {
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

void ASTNode::mark(Context* context) const {
  // mark the unique string stored in the ID
  id_.mark(context);
  // run markUniqueStrings on the node
  markUniqueStringsInner(context);
  // run markASTList on the child list
  markASTList(context, children_);
}

static std::string getIdStr(const ASTNode* ast) {
  std::string idStr;
  if (ast == nullptr || ast->id().isEmpty()) {
    idStr = "<no id>";
  } else {
    std::ostringstream ss;
    ast->id().stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
    idStr = ss.str();
  }

  return idStr;
}

static void dumpMaxIdLen(const ASTNode* ast, int& maxIdLen) {
  std::string idStr = getIdStr(ast);
  if ((int) idStr.length() > maxIdLen)
    maxIdLen = idStr.length();

  if (ast != nullptr) {
    for (const ASTNode* child : ast->children()) {
      dumpMaxIdLen(child, maxIdLen);
    }
  }
}

static void dumpHelper(std::ostream& ss,
                       const ASTNode* ast,
                       int maxIdLen,
                       int depth) {
  std::string idStr = getIdStr(ast);
  ss << std::setw(maxIdLen) << idStr;


  for (int i = 0; i < depth; i++) {
    ss << "  ";
  }

  if (ast == nullptr) {
    ss << "nullptr\n";
    return;
  }

  ss << asttags::tagToString(ast->tag()) << " ";

  if (const NamedDecl* named = ast->toNamedDecl()) {
    ss << named->name().str() << " ";
  } else if (const Identifier* ident = ast->toIdentifier()) {
    ss << ident->name().str() << " ";
  } else if (const Comment* comment = ast->toComment()) {
    ss << comment->str() << " ";
  }

  //printf("(containing %i) ", ast->id().numContainedChildren());
  //printf("%p", ast);
  ss << "\n";

  for (const ASTNode* child : ast->children()) {
    dumpHelper(ss, child, maxIdLen, depth+1);
  }
}

void ASTNode::stringify(std::ostream& ss,
                        StringifyKind stringKind) const {

  if (stringKind == StringifyKind::CHPL_SYNTAX) {
    printChapelSyntax(ss, this);
  }
  else {
    int maxIdLen = 0;
    int leadingSpaces = 0;
    dumpMaxIdLen(this, maxIdLen);
    dumpHelper(ss, this, maxIdLen, leadingSpaces);
  }
}

IMPLEMENT_DUMP(ASTNode);

} // end namespace uast
} // end namespace chpl
