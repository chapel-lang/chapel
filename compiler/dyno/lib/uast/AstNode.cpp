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

#include "chpl/uast/AstNode.h"


#include "chpl/uast/AstNode.h"
#include "chpl/uast/Comment.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/NamedDecl.h"
#include "chpl/uast/chpl-syntax-printer.h"

#include <iomanip>

namespace chpl {
namespace uast {


AstNode::~AstNode() {
}

bool AstNode::mayContainStatements(AstTag tag) {
  switch (tag) {
    // cannot contain Chapel statements
    case asttags::As:
    case asttags::Array:
    case asttags::Attributes:
    case asttags::Break:
    case asttags::Comment:
    case asttags::Continue:
    case asttags::Delete:
    case asttags::Domain:
    case asttags::Dot:
    case asttags::EmptyStmt:
    case asttags::ErroneousExpression:
    case asttags::ExternBlock:
    case asttags::Identifier:
    case asttags::Import:
    case asttags::Include:
    case asttags::Let:
    case asttags::New:
    case asttags::Range:
    case asttags::Require:
    case asttags::Return:
    case asttags::Throw:
    case asttags::TypeQuery:
    case asttags::Use:
    case asttags::VisibilityClause:
    case asttags::WithClause:
    case asttags::Yield:
    case asttags::START_Literal:
    case asttags::BoolLiteral:
    case asttags::ImagLiteral:
    case asttags::IntLiteral:
    case asttags::RealLiteral:
    case asttags::UintLiteral:
    case asttags::START_StringLikeLiteral:
    case asttags::BytesLiteral:
    case asttags::CStringLiteral:
    case asttags::StringLiteral:
    case asttags::END_StringLikeLiteral:
    case asttags::END_Literal:
    case asttags::START_Call:
    case asttags::FnCall:
    case asttags::OpCall:
    case asttags::PrimCall:
    case asttags::Reduce:
    case asttags::Scan:
    case asttags::Tuple:
    case asttags::Zip:
    case asttags::END_Call:
    case asttags::MultiDecl:
    case asttags::TupleDecl:
    case asttags::ForwardingDecl:
    case asttags::EnumElement:
    case asttags::START_VarLikeDecl:
    case asttags::Formal:
    case asttags::TaskVar:
    case asttags::VarArgFormal:
    case asttags::Variable:
    case asttags::END_VarLikeDecl:
    case asttags::Enum:
      return false;

    // can contain statements
    case asttags::Catch:
    case asttags::Cobegin:
    case asttags::Conditional:
    case asttags::Implements:
    case asttags::Label: // contains a loop
    case asttags::Select:
    case asttags::Sync:
    case asttags::Try:
    case asttags::START_SimpleBlockLike:
    case asttags::Begin:
    case asttags::Block:
    case asttags::Defer:
    case asttags::Local:
    case asttags::Manage:
    case asttags::On:
    case asttags::Serial:
    case asttags::When:
    case asttags::END_SimpleBlockLike:
    case asttags::START_Loop:
    case asttags::DoWhile:
    case asttags::While:
    case asttags::START_IndexableLoop:
    case asttags::BracketLoop:
    case asttags::Coforall:
    case asttags::For:
    case asttags::Forall:
    case asttags::Foreach:
    case asttags::END_IndexableLoop:
    case asttags::END_Loop:
    case asttags::START_Decl:
    case asttags::START_NamedDecl:
    case asttags::START_TypeDecl:
    case asttags::Function:
    case asttags::Interface:
    case asttags::Module:
    case asttags::START_AggregateDecl:
    case asttags::Class:
    case asttags::Record:
    case asttags::Union:
    case asttags::END_AggregateDecl:
    case asttags::END_Decl:
    case asttags::END_NamedDecl:
    case asttags::END_TypeDecl:
      return true;

    // implementation details
    case asttags::NUM_AST_TAGS:
    case asttags::AST_TAG_UNKNOWN:
      return true;

    // no default to get compiler warning if any are added
  }

  assert(false && "should not be reachable");
  return true;
}

bool AstNode::shallowMatch(const AstNode* other) const {
  const AstNode* lhs = this;
  const AstNode* rhs = other;
  if (lhs->tag() != rhs->tag())
    return false;
  if (lhs->id().symbolPath() != rhs->id().symbolPath())
    return false;
  if (!lhs->contentsMatchInner(rhs))
    return false;

  return true;
}

bool AstNode::completeMatch(const AstNode* other) const {
  const AstNode* lhs = this;
  const AstNode* rhs = other;

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
    const AstNode* lhsChild = lhs->children_[i].get();
    const AstNode* rhsChild = rhs->children_[i].get();
    bool childMatch = lhsChild->completeMatch(rhsChild);
    if (!childMatch) {
      allMatch = false;
      break;
    }
  }
  return allMatch;
}

bool AstNode::update(owned<AstNode>& keep, owned<AstNode>& addin) {
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

    updateAstList(keep->children_, addin->children_);
    return true; // updated
  } else {
    // swap the AST
    keep.swap(addin);
    return true;
  }
}

void AstNode::mark(Context* context) const {
  // mark the unique string stored in the ID
  id_.mark(context);
  // run markUniqueStrings on the node
  markUniqueStringsInner(context);
  // run markAstList on the child list
  markAstList(context, children_);
}

static std::string getIdStr(const AstNode* ast) {
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

static void dumpMaxIdLen(const AstNode* ast, int& maxIdLen) {
  std::string idStr = getIdStr(ast);
  if ((int) idStr.length() > maxIdLen)
    maxIdLen = idStr.length();

  if (ast != nullptr) {
    for (const AstNode* child : ast->children()) {
      dumpMaxIdLen(child, maxIdLen);
    }
  }
}

static void dumpHelper(std::ostream& ss,
                       const AstNode* ast,
                       int maxIdLen,
                       int depth) {
  std::string idStr = getIdStr(ast);
  ss << std::setw(maxIdLen) << idStr;


  if (depth == 0) {
    ss << " ";
  }
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
  } else if (const Dot* dot = ast->toDot()) {
    ss << "." << dot->field() << " ";
  }

  //printf("(containing %i) ", ast->id().numContainedChildren());
  //printf("%p", ast);
  ss << "\n";

  for (const AstNode* child : ast->children()) {
    dumpHelper(ss, child, maxIdLen, depth+1);
  }
}

void AstNode::stringify(std::ostream& ss,
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

IMPLEMENT_DUMP(AstNode);

} // end namespace uast
} // end namespace chpl
