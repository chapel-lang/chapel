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

#ifndef CHPL_UAST_ASTNODE_H
#define CHPL_UAST_ASTNODE_H

#include "chpl/framework/ID.h"
#include "chpl/framework/stringify-functions.h"
#include "chpl/uast/AstList.h"
#include "chpl/uast/AstTag.h"
#include "chpl/uast/forward-declare-uast.h"
#include "chpl/util/memory.h"

#include <functional>

namespace chpl {
namespace uast {



/**
  This is the base class for AST types.

  Every AST class has:
   * a tag (indicating which AST class it is)
   * an ID (a sort of scoped location used as a key in maps)
   * a list of child AST nodes

  The list of child nodes is included in AstNode to allow for
  generic tree traversals of the AST.

  Functions like someAst->isCall() / someAst->toCall() are available and
  generated for all AST types.

  std::less is defined for every AST class and it compares IDs.

 */
class AstNode {
 friend class Builder;

 private:
  AstTag tag_;
  int attributeGroupChildNum_;
  ID id_;


 protected:
  AstList children_;

  /**
    This function needs to be defined by subclasses.
    It should check only those fields defined in subclasses
    (it should not check the AstNode fields tag, ID, or children).
    It can assume that other has the same type as the receiver.
   */
  virtual bool contentsMatchInner(const AstNode* other) const = 0;

  /**
   This function needs to be defined by subclasses.
   It should call the 'mark' method on any UniqueStrings
   stored as fields. It need not worry about the children nodes
   or the UniqueStrings stored in the ID.
   */
  virtual void markUniqueStringsInner(Context* context) const = 0;

  /**
   This function needs to be defined by subclasses.
   It should serialize any fields in the subclasses.
   It need not concern itself with AstNode's fields, including the children.
   Note that subclasses also need to provide a constructor
   accepting AstTag, Deserializer.
   */
  virtual void serializeInner(Serializer& os) const = 0;

  struct DumpSettings {
    chpl::StringifyKind kind = StringifyKind::DEBUG_DETAIL;
    bool printId = true;
    int idWidth = 0;
    std::ostream& out;
    DumpSettings(std::ostream& out) : out(out) { }
  };

  /**
    This function can be defined by subclasses to include fields
    in the uAST dump just after the tag. It should print a " "
    before any fields printed. It is not expected
    to print a newline.
   */
  virtual void dumpFieldsInner(const DumpSettings& s) const;

  /**
    This function can be defined by subclasses to emit a label for child 'i',
    for example to indicate which Block is the Then part of a Conditional.
   */
  virtual std::string dumpChildLabelInner(int i) const;

  static void dumpHelper(const DumpSettings& s,
                         const AstNode* ast,
                         int indent,
                         const AstNode* parent,
                         int parentIdx);

  AstNode(AstTag tag)
    : tag_(tag), attributeGroupChildNum_(NO_CHILD), id_(), children_() {
  }
  AstNode(AstTag tag, AstList children)
    : tag_(tag), attributeGroupChildNum_(NO_CHILD), id_(),
      children_(std::move(children)) {
  }
  AstNode(AstTag tag, AstList children, int attributeGroupChildNum)
    : tag_(tag), attributeGroupChildNum_(attributeGroupChildNum), id_(),
      children_(std::move(children)) {
    CHPL_ASSERT(NO_CHILD <= attributeGroupChildNum_ &&
                attributeGroupChildNum_ < (ssize_t)children_.size());

    if (attributeGroupChildNum_ >= 0) {
      CHPL_ASSERT(child(attributeGroupChildNum_)->isAttributeGroup());
    }
  }

  /** To be called by subclasses to set the tag and
      deserialize the AstNode fields other than the children */
  AstNode(AstTag tag, Deserializer& des);

  AstNode(const AstNode& node) {
    this->tag_ = node.tag_;
    this->attributeGroupChildNum_ = node.attributeGroupChildNum_;
    for (auto child : node.children()) {
      children_.push_back(child->copy());
    }
  }

  /** Completes the deserialization process for an AstNode
      by deserializing the children. */
  void deserializeChildren(Deserializer& des);

  /** Serializes the children, but skips comments */
  void serializeChildren(Serializer& ser) const;

  // Quick way to return an already exhausted iterator.
  template <typename T>
  AstListIteratorPair<T> emptyIterator() const {
    return AstListIteratorPair<T>(children_.end(), children_.end());
  }

  // Quick way to construct an iterator over a child range.
  template <typename T>
  AstListIteratorPair<T> childRange(int start, int num) const {
    if (num <= 0) return emptyIterator<T>();
    auto itStart = children_.begin() + start;
    auto itEnd = itStart + num;
    return AstListIteratorPair<T>(itStart, itEnd);
  }

  // As above, but untemplated.
  AstListIteratorPair<AstNode> childRange(int start, int num) const {
    if (num <= 0) return emptyIterator<AstNode>();
    auto itStart = children_.begin() + start;
    auto itEnd = itStart + num;
    return AstListIteratorPair<AstNode>(itStart, itEnd);
  }

  // called by the Builder
  void setID(ID id) { id_ = id; }

 public:
  virtual ~AstNode() = 0; // this is an abstract base class

  // Magic constant to indicate no such child exists.
  static constexpr int NO_CHILD = -1;

  /**
    Returns the tag indicating which AstNode subclass this is.
   */
  AstTag tag() const {
    return tag_;
  }

  /**
    Returns the ID of this AST node.
   */
  inline const ID& id() const {
    return id_;
  }

  /**
    Returns 'true' if this AST node is a leaf node by tag.
  */
  bool isLeaf() const;

  /**
    Returns the number of child AST nodes in the tree directly under this one.
   */
  int numChildren() const {
    return children_.size();
  }

  /**
    Return a way to iterate over the children.
    */
  AstListIteratorPair<AstNode> children() const {
    return AstListIteratorPair<AstNode>(children_.begin(), children_.end());
  }

  /**
    Returns the i'th child AST node in the tree directly under this one.
    This function returns a "borrow" of the AST node. It is managed
    by this object.
   */
  const AstNode* child(int i) const {
    CHPL_ASSERT(0 <= i && i < (int) children_.size());
    return children_[i].get();
  }

  /**
    Returns the index into children of the attributeGroup child node,
    or AstNode::NO_CHILD if no attributeGroup exists on this node.
   */
  int attributeGroupChildNum() const {
    return attributeGroupChildNum_;
  }

  /*
    Attach an AttributeGroup to this AstNode after it was initially built
    without an AttributeGroup. This is used by the ParserContextImpl to
    handle parsing Variables and TupleDecls, and will result in the
    AttributeGroup being attached as the last child of this AstNode.
    TODO: We may want to consider moving the AttributeGroup to the front
    of the children list, to maintain consistency with other AstNodes. But that
    will require updating the indices of all the existing children.
  */
  void attachAttributeGroup(owned<AstNode> attributeGroup) {
    CHPL_ASSERT(attributeGroupChildNum_ == NO_CHILD);
    CHPL_ASSERT(attributeGroup->isAttributeGroup());
    attributeGroupChildNum_ = children_.size();
    children_.push_back(std::move(attributeGroup));
  }

  /**
    Return the attributeGroup associated with this AstNode, or nullptr
    if none exists. Note that it would be better to use the parsing query
    idToAttributeGroup to ensure you get the AttributeGroup when the AstNode
    is a child of a MultiDecl or TupleDecl.
   */
  const AttributeGroup* attributeGroup() const {
    if (attributeGroupChildNum_ < 0) return nullptr;
    auto ret = child(attributeGroupChildNum_);
    CHPL_ASSERT(ret->isAttributeGroup());
    return (const AttributeGroup*)ret;
  }

  /**
    Returns 'true' if this symbol contains another AST node.
    This is an operation on the IDs.
   */
  bool contains(const AstNode* other) const {
    return this->id_.contains(other->id_);
  }

  /**
    Returns 'true' if the passed type of AST node can contain statements,
    transitively.
   */
  static bool mayContainStatements(AstTag tag);

  /**
    Returns 'true' if this uAST node is a inherently a statement.
    Note that anything contained directly in a Block is also a statement.
   */
  bool isInherentlyStatement() const;

  bool shallowMatch(const AstNode* other) const;
  bool completeMatch(const AstNode* other) const;

  // 'keep' is some old AST
  // 'addin' is some new AST we wish to combine with it
  //
  // on exit, 'keep' stores the AST we need to keep, and anything
  // not kept is stored in 'addin'.
  // the function returns 'true' if anything changed in 'keep'.
  static bool update(owned<AstNode>& keep, owned<AstNode>& addin);

  void mark(Context* context) const;

  void stringify(std::ostream& ss, chpl::StringifyKind stringKind) const;

  // compute the maximum width of all of the IDs
  int computeMaxIdStringWidth() const;

  /** Serialize this uAST node to the stream stored in 'ser' */
  void serialize(Serializer& ser) const;

  /** Deserialize this uAST node from the stream in 'des'. Note
      that uAST nodes deserialized in this way will not have IDs assigned.
      To assign IDs, it's necessary to use a Builder. */
  static owned<AstNode> deserializeWithoutIds(Deserializer& des);

  /// \cond DO_NOT_DOCUMENT
  DECLARE_DUMP;
  /// \endcond DO_NOT_DOCUMENT

  // define is__ methods for the various AST types
  // using macros and uast-classes-list.h
  /// \cond DO_NOT_DOCUMENT
  #define AST_IS(NAME) \
    bool is##NAME() const { \
      return asttags::is##NAME(this->tag_); \
    }
  #define AST_NODE(NAME) AST_IS(NAME)
  #define AST_LEAF(NAME) AST_IS(NAME)
  #define AST_BEGIN_SUBCLASSES(NAME) AST_IS(NAME)
  #define AST_END_SUBCLASSES(NAME)
  // Used for macro-based casting
  bool isAstNode() const { return true; }
  /// \endcond
  // Apply the above macros to uast-classes-list.h
  #include "chpl/uast/uast-classes-list.h"
  // clear the macros
  #undef AST_NODE
  #undef AST_LEAF
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
  #undef AST_IS

  // define to__ methods for the various AST types
  // using macros and uast-classes-list.h
  // Note: these offer equivalent functionality to C++ dynamic_cast<DstType*>
  /// \cond DO_NOT_DOCUMENT
  #define AST_TO(NAME) \
    const NAME * to##NAME() const { \
      return this->is##NAME() ? (const NAME *)this : nullptr; \
    } \
    NAME * to##NAME() { \
      return this->is##NAME() ? (NAME *)this : nullptr; \
    }
  #define AST_NODE(NAME) AST_TO(NAME)
  #define AST_LEAF(NAME) AST_TO(NAME)
  #define AST_BEGIN_SUBCLASSES(NAME) AST_TO(NAME)
  #define AST_END_SUBCLASSES(NAME)
  // Used for macro-based casting
  AST_TO(AstNode)
  /// \endcond
  // Apply the above macros to uast-classes-list.h
  #include "chpl/uast/uast-classes-list.h"
  // clear the macros
  #undef AST_NODE
  #undef AST_LEAF
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
  #undef AST_TO

 private:

  template <typename ReturnType, typename Visitor>
  struct Dispatcher {
    static ReturnType doDispatch(const AstNode* ast, Visitor& v) {

      switch (ast->tag()) {
        #define CONVERT(NAME) \
          case chpl::uast::asttags::NAME: \
          { \
            return v.visit((const chpl::uast::NAME*) ast); \
          }

        #define IGNORE(NAME) \
          case chpl::uast::asttags::NAME: \
          { \
            CHPL_ASSERT(false && "this code should never be run"); \
          }

        #define AST_NODE(NAME) CONVERT(NAME)
        #define AST_LEAF(NAME) CONVERT(NAME)
        #define AST_BEGIN_SUBCLASSES(NAME) IGNORE(START_##NAME)
        #define AST_END_SUBCLASSES(NAME) IGNORE(END_##NAME)

        #include "chpl/uast/uast-classes-list.h"

        IGNORE(NUM_AST_TAGS)
        IGNORE(AST_TAG_UNKNOWN)

        #undef AST_NODE
        #undef AST_LEAF
        #undef AST_BEGIN_SUBCLASSES
        #undef AST_END_SUBCLASSES
        #undef CONVERT
        #undef IGNORE
      }

      CHPL_ASSERT(false && "this code should never be run");
      return ReturnType();
    }
  };
  template <typename Visitor>
  struct Dispatcher<void, Visitor> {
    static void doDispatch(const AstNode* ast, Visitor& v) {

      switch (ast->tag()) {
        #define CONVERT(NAME) \
          case chpl::uast::asttags::NAME: \
          { \
            v.visit((const chpl::uast::NAME*) ast); \
            return; \
          }

        #define IGNORE(NAME) \
          case chpl::uast::asttags::NAME: \
          { \
            CHPL_ASSERT(false && "this code should never be run"); \
          }

        #define AST_NODE(NAME) CONVERT(NAME)
        #define AST_LEAF(NAME) CONVERT(NAME)
        #define AST_BEGIN_SUBCLASSES(NAME) IGNORE(START_##NAME)
        #define AST_END_SUBCLASSES(NAME) IGNORE(END_##NAME)

        #include "chpl/uast/uast-classes-list.h"

        IGNORE(NUM_AST_TAGS)
        IGNORE(AST_TAG_UNKNOWN)

        #undef AST_NODE
        #undef AST_LEAF
        #undef AST_BEGIN_SUBCLASSES
        #undef AST_END_SUBCLASSES
        #undef CONVERT
        #undef IGNORE
      }

      CHPL_ASSERT(false && "this code should never be run");
    }
  };

 public:

  /**
     The dispatch function supports calling a method according to the tag
     (aka runtime type) of a uast node. It does not itself visit
     children of the uast node (but of course the called visit function
     is free to do so).

     It is a template and the Visitor argument should provide functions
     like

        MyReturnType MyVisitor::visit(const uast::AstNode* ast);
        MyReturnType MyVisitor::visit(const uast::Variable* ast);

     and these will be invoked according to C++ overload resolution
     (where in particular an exact match will be preferred).

     It is generally necessary to specify the ReturnType when calling it, e.g.

       ast->dispatch<MyReturnType>(myVisitor);

     The return type currently needs to be default constructable.
   */
  template <typename ReturnType, typename Visitor>
  ReturnType dispatch(Visitor& v) const {

    return Dispatcher<ReturnType, Visitor>::doDispatch(this, v);
  }

  /**
     The traverse function supports calling a method according to the tag
     (aka runtime type) of a uast node and calling that method also
     on the children of the uast node.

     It is a template and the Visitor argument should provide functions
     like

        bool MyTraverser::enter(const uast::AstNode* ast);
        void MyTraverser::exit(const uast::AstNode* ast);
        bool MyTraverser::enter(const uast::Variable* ast);
        void MyTraverser::exit(const uast::Variable* ast);

     and these will be invoked according to C++ overload resolution
     (where in particular an exact match will be preferred).

     The enter method returns whether or not the children should
     be visited. In particular, when visiting a node:

       * First, the enter method is called.
       * If enter returns true, the children are visited.
       * Then the exit method is called (whether or not enter returned true).

     Unlike `dispatch`, this function doesn't support returning a value.

     The traverse function can be called like this:

       traverse(myTraverser, ast);
   */

  template <typename Visitor>
  void traverse(Visitor& v) const {

    switch (this->tag()) {
      #define CASE_LEAF(NAME) \
        case asttags::NAME: \
        { \
          const NAME* casted = (const NAME*) this; \
          v.enter(casted); \
          CHPL_ASSERT(this->numChildren() == 0); \
          v.exit(casted); \
          break; \
        }

      #define CASE_NODE(NAME) \
        case asttags::NAME: \
        { \
          const NAME* casted = (const NAME*) this; \
          bool goInToIt = v.enter(casted); \
          if (goInToIt) { \
            for (const AstNode* child : this->children()) { \
              child->traverse(v); \
            } \
          } \
          v.exit(casted); \
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
  }

  owned<AstNode> copy() const;
};
} // end namespace uast

template<> struct serialize<uast::AstList> {
  void operator()(Serializer& ser, const uast::AstList& list) {
    ser.writeVU64(list.size());
    for (const auto& node : list) {
      node->serialize(ser);
    }
  }
};

template<> struct deserialize<uast::AstList> {
  uast::AstList operator()(Deserializer& des) {
    uast::AstList ret;
    uint64_t len = des.readVU64();
    for (uint64_t i = 0; i < len; i++) {
      ret.push_back(uast::AstNode::deserializeWithoutIds(des));
    }
    return ret;
  }
};
} // end namespace chpl

/// \cond DO_NOT_DOCUMENT
namespace std {

// define std::less for the various AST types
// using macros and uast-classes-list.h
/// \cond DO_NOT_DOCUMENT
#define AST_LESS(NAME) \
  template<> struct less<chpl::uast::NAME*> { \
    bool operator()(const chpl::uast::NAME* lhs, \
                    const chpl::uast::NAME* rhs) const { \
      if (lhs == nullptr && rhs != nullptr) return true; \
      if (rhs == nullptr) return false; \
      std::less<chpl::ID> lessID; \
      /* cast in the next line is so it compiles with only forward decls */ \
      return lessID(((const chpl::uast::AstNode*)lhs)->id(), \
                    ((const chpl::uast::AstNode*)rhs)->id()); \
    } \
  };
#define AST_NODE(NAME) AST_LESS(NAME)
#define AST_LEAF(NAME) AST_LESS(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) AST_LESS(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to uast-classes-list.h
#include "chpl/uast/uast-classes-list.h"
// Additionally, apply the macro to AstNode
AST_LESS(AstNode)
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef AST_LESS
/// \endcond

} // end namespace std

#endif
