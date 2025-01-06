/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/uast/all-uast.h"
#include "chpl/uast/AstNode.h"


#include "chpl/uast/Comment.h"
#include "chpl/uast/Conditional.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/IndexableLoop.h"
#include "chpl/uast/NamedDecl.h"
#include "chpl/uast/Try.h"
#include "chpl/uast/chpl-syntax-printer.h"

#include <iomanip>

namespace chpl {
namespace uast {


void AstNode::dumpFieldsInner(const DumpSettings& s) const {
}

std::string AstNode::dumpChildLabelInner(int i) const {
  if (i == attributeGroupChildNum_) {
    return "attributeGroup";
  }

  return "";
}

AstNode::~AstNode() {
}

bool AstNode::isLeaf() const {
  bool ret = false;
  switch (this->tag()) {
  #define AST_NODE(NAME__)
  #define AST_BEGIN_SUBCLASSES(NAME__)
  #define AST_END_SUBCLASSES(NAME__)
  #define AST_LEAF(NAME__) \
    case asttags::NAME__: \
      ret = true; \
      break;
    #include "chpl/uast/uast-classes-list.h"
    AST_NODE(NUM_AST_TAGS)
    AST_NODE(AST_TAG_UNKNOWN)
    default: break;
  #undef AST_NODE
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
  #undef AST_LEAF
  }

  if (ret) {
    CHPL_ASSERT(numChildren() == 0);
  }

  return ret;
}

bool AstNode::mayContainStatements(AstTag tag) {
  switch (tag) {
    // cannot contain Chapel statements
    case asttags::AnonFormal:
    case asttags::As:
    case asttags::Array:
    case asttags::Attribute:
    case asttags::AttributeGroup:
    case asttags::Break:
    case asttags::Comment:
    case asttags::Continue:
    case asttags::Delete:
    case asttags::Domain:
    case asttags::Dot:
    case asttags::EmptyStmt:
    case asttags::ErroneousExpression:
    case asttags::ExternBlock:
    case asttags::FunctionSignature:
    case asttags::Identifier:
    case asttags::Import:
    case asttags::Include:
    case asttags::Init:
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
    case asttags::ReduceIntent:
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

  CHPL_ASSERT(false && "should not be reachable");
  return true;
}

bool AstNode::isInherentlyStatement() const {
  switch (tag_) {
    // cannot contain Chapel statements
    case asttags::AnonFormal:
    case asttags::As:
    case asttags::Array:
    case asttags::Attribute:
    case asttags::AttributeGroup:
    case asttags::Break:
    case asttags::Comment:
    case asttags::Continue:
    case asttags::Delete:
    case asttags::Domain:
    case asttags::Dot:
    case asttags::EmptyStmt:
    case asttags::ErroneousExpression:
    case asttags::ExternBlock:
    case asttags::FunctionSignature:
    case asttags::Identifier:
    case asttags::Import:
    case asttags::Include:
    case asttags::Init:
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
    case asttags::ReduceIntent:
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

    // might be a statement and might be an expr
    case asttags::Conditional:
      return !toConditional()->isExpressionLevel();

    case asttags::Try:
      return !toTry()->isExpressionLevel();

    case asttags::START_IndexableLoop:
    case asttags::BracketLoop:
    case asttags::Coforall:
    case asttags::For:
    case asttags::Forall:
    case asttags::Foreach:
    case asttags::END_IndexableLoop:
      return toIndexableLoop()->isExpressionLevel();

    // is a statement
    case asttags::Catch:
    case asttags::Cobegin:
    case asttags::Implements:
    case asttags::Label: // contains a loop
    case asttags::Select:
    case asttags::Sync:
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

  CHPL_ASSERT(false && "should not be reachable");
  return true;
}


bool AstNode::shallowMatch(const AstNode* other) const {
  const AstNode* lhs = this;
  const AstNode* rhs = other;
  if (lhs->tag() != rhs->tag())
    return false;
  if (lhs->id().symbolPath() != rhs->id().symbolPath())
    return false;
  if (lhs->attributeGroupChildNum() != rhs->attributeGroupChildNum())
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

  // check the numeric elements of the id and the number of children and
  // the attributeGroupChildNum
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

    // TODO: This should only be possible while we're implementing
    // serialization...
    if (lhsChild == nullptr || rhsChild == nullptr) {
      return false;
    }

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
  if (ast == nullptr || ast->id().isEmpty()) {
    return "<no id>";
  } else {
    return ast->id().str();
  }
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

int AstNode::computeMaxIdStringWidth() const {
  int maxIdLen = 0;
  dumpMaxIdLen(this, maxIdLen);
  return maxIdLen;
}

void AstNode::dumpHelper(const DumpSettings& s,
                         const AstNode* ast,
                         int indent,
                         const AstNode* parent,
                         int parentIdx) {

  // output the id if desired
  if (s.printId) {
    std::string idStr = getIdStr(ast);
    // save the stream format state
    std::ios state(nullptr);
    state.copyfmt(s.out);
    // output ID left justified and padded
    s.out << std::setw(s.idWidth) << std::left << idStr;
    // restore the stream format state
    s.out.copyfmt(state);
  }

  if (s.kind == StringifyKind::DEBUG_DETAIL) {
    // add spacing according to depth
    // (includes one space to separate id from the rest)
    for (int i = 0; i <= indent; i++) {
      s.out << "  ";
    }
  }

  // output the label if there is one
  if (parent != nullptr) {
    std::string label = parent->dumpChildLabelInner(parentIdx);
    if (!label.empty()) {
      s.out << label << " ";
    }
  }

  // output the tag
  s.out << asttags::tagToString(ast->tag());

  // output the fields
  ast->dumpFieldsInner(s);

  if (s.kind == StringifyKind::DEBUG_DETAIL) {
    s.out << "\n";
  }

  // output the child nodes
  int i = 0;
  for (const AstNode* child : ast->children()) {
    if (child != nullptr) {
      dumpHelper(s, child, indent+1, ast, i);
    } else {
      s.out << "nullptr";
    }
    i++;
  }

  return;
}

void AstNode::stringify(std::ostream& ss,
                        StringifyKind stringKind) const {

  switch (stringKind) {
    case StringifyKind::CHPL_SYNTAX:
      // use the Chapel Syntax printer
      printChapelSyntax(ss, this);
      break;
    case StringifyKind::DEBUG_SUMMARY:
      // just print the ID
      this->id().stringify(ss, stringKind);
      break;
    case StringifyKind::DEBUG_DETAIL:
      {
        auto s = DumpSettings(ss);
        s.kind = stringKind;
        s.printId = true;
        // compute the maximum id width so it's a nice column
        int maxIdLen = computeMaxIdStringWidth();
        s.idWidth = maxIdLen;
        dumpHelper(s, this, 0, /*parent*/ nullptr, /*parentIdx*/NO_CHILD);
      }
      break;
  }
}

void AstNode::serialize(Serializer& ser) const {
  ser.beginAst(this);
  ser.write(tag_);
  ser.writeVInt(attributeGroupChildNum_);
  // id_ not serialized; it is recomputed after reading
  serializeInner(ser);
  serializeChildren(ser);
  ser.endAst(this);
}

AstNode::AstNode(AstTag tag, Deserializer& des)
  : tag_(tag) {
  // Note: Assumes that the tag was already deserialized in order to invoke
  // the correct class' deserializer.
  attributeGroupChildNum_ = des.readVInt();
  // id_ not deserialized; it is recomputed after reading
}

void AstNode::serializeChildren(Serializer& ser) const {
  // count the number of children ignoring comments
  uint64_t count = 0;
  for (const AstNode* child : children()) {
    if (!child->isComment()) {
      count++;
    }
  }

  // write the count
  ser.writeVU64(count);

  // store the children ignoring comments
  for (const AstNode* child : children()) {
    if (!child->isComment()) {
      child->serialize(ser);
    }
  }
}

void AstNode::deserializeChildren(Deserializer& des) {
  uint64_t len = des.readVU64();
  // note: this check assumes 1 byte per child node, and it's
  // true that each child node will be at least one byte.
  if (des.checkStringLengthAvailable(len)) {
    children_.resize(len);
    for (uint64_t i = 0; i < len; i++) {
      children_[i] = deserializeWithoutIds(des);
    }
  }
}

owned<AstNode> AstNode::deserializeWithoutIds(Deserializer& des) {
  uint64_t pos = des.position();

  AstTag tag = des.read<AstTag>();

  // Deserialize using the specific type constructor
  // which will call AstNode::AstNode(AstTag tag, Deserializer& des) above
  // to deserialize AstNode's fields (but not the children)
  // and then deserialize the subclass fields through its constructor.
  // Then, register the deserialized Ast with the Deserializer
  // Finally, deserialize the children with deserializeChildren.

  switch (tag) {
    #define CASE_LEAF(NAME) \
      case asttags::NAME: \
      { \
        auto ret = toOwned(new NAME(des)); \
        des.registerAst(ret.get(), pos); \
        ret->deserializeChildren(des); \
        return ret; \
      }

    #define CASE_NODE(NAME) \
      case asttags::NAME: \
      { \
        auto ret = toOwned(new NAME(des)); \
        des.registerAst(ret.get(), pos); \
        ret->deserializeChildren(des); \
        return ret; \
      }

    #define CASE_OTHER(NAME) \
      case asttags::NAME: \
      { \
        assert(false && "this code should never be run"); \
        break; \
      }

    #define AST_NODE(NAME) CASE_NODE(NAME)
    #define AST_LEAF(NAME) CASE_LEAF(NAME)
    #define AST_BEGIN_SUBCLASSES(NAME) CASE_OTHER(START_##NAME)
    #define AST_END_SUBCLASSES(NAME) CASE_OTHER(END_##NAME)

    // Apply the above macros to uast-classes-list.h
    // to fill in the cases
    #include "chpl/uast/uast-classes-list.h"
    // and also for NUM_AST_TAGS
    CASE_OTHER(NUM_AST_TAGS)
    CASE_OTHER(AST_TAG_UNKNOWN)

    // clear the macros
    #undef AST_NODE
    #undef AST_LEAF
    #undef AST_BEGIN_SUBCLASSES
    #undef AST_END_SUBCLASSES
    #undef CASE_LEAF
    #undef CASE_NODE
    #undef CASE_OTHER
  }

  assert(false && "this code should never be run"); \

  owned<AstNode> ret = nullptr;
  return ret;
}

owned<AstNode> AstNode::copy() const {
  switch (this->tag()) {
    #define CASE_LEAF(NAME) \
      case asttags::NAME: \
      { \
        const NAME* casted = (const NAME*) this; \
        auto ret = toOwned(new NAME(*casted)); \
        ret->setID(ID()); \
        return ret; \
        break; \
      }

    #define CASE_NODE(NAME) \
      case asttags::NAME: \
      { \
        const NAME* casted = (const NAME*) this; \
        auto ret = toOwned(new NAME(*casted)); \
        ret->setID(ID()); \
        for (auto& child : ret->children_) { \
          child = child->copy(); \
        } \
        return ret; \
        break; \
      }

    #define CASE_OTHER(NAME) \
      case asttags::NAME: \
      { \
        CHPL_ASSERT(false && "this code should never be run"); \
        break; \
      }

    #define AST_NODE(NAME) CASE_NODE(NAME)
    #define AST_LEAF(NAME) CASE_LEAF(NAME)
    #define AST_BEGIN_SUBCLASSES(NAME) CASE_OTHER(START_##NAME)
    #define AST_END_SUBCLASSES(NAME) CASE_OTHER(END_##NAME)

    // Apply the above macros to uast-classes-list.h
    // to fill in the cases
    #include "chpl/uast/uast-classes-list.h"
    // and also for NUM_AST_TAGS
    CASE_OTHER(NUM_AST_TAGS)
    CASE_OTHER(AST_TAG_UNKNOWN)

    // clear the macros
    #undef AST_NODE
    #undef AST_LEAF
    #undef AST_BEGIN_SUBCLASSES
    #undef AST_END_SUBCLASSES
    #undef CASE_LEAF
    #undef CASE_NODE
    #undef CASE_OTHER
  }

  owned<AstNode> ret;
  return ret;
}

IMPLEMENT_DUMP(AstNode);

} // end namespace uast
} // end namespace chpl
