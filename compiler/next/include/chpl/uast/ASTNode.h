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

#ifndef CHPL_UAST_ASTNODE_H
#define CHPL_UAST_ASTNODE_H

#include "chpl/queries/ID.h"
#include "chpl/uast/ASTList.h"
#include "chpl/uast/ASTTag.h"
#include "chpl/uast/ASTTypes.h"
#include "chpl/util/memory.h"

#include <cassert>

namespace chpl {
namespace uast {


/**
  This is the base class for AST types.

  Every AST class has:
   * a tag (indicating which AST class it is)
   * an ID (a sort of scoped location used as a key in maps)
   * a list of child AST nodes

  The list of child nodes is included in ASTNode to allow for
  generic tree traversals of the AST.

  Functions like someAst->isCall() / someAst->toCall() are available and
  generated for all AST types.

  std::less is defined for every AST class and it compares IDs.

 */
class ASTNode {
 friend class Builder;

 private:
  ASTTag tag_;
  ID id_;

 protected:
  ASTList children_;

  /**
    This function needs to be defined by subclasses.
    It should check only those fields defined in subclasses
    (it should not check the ASTNode fields tag, ID, or children).
    It can assume that other has the same type as the receiver.
   */
  virtual bool contentsMatchInner(const ASTNode* other) const = 0;

  /**
   This function needs to be defined by subclasses.
   It should call the 'mark' method on any UniqueStrings
   stored as fields. It need not worry about the children nodes
   or the UniqueStrings stored in the ID.
   */
  virtual void markUniqueStringsInner(Context* context) const = 0;

 protected:
  ASTNode(ASTTag tag);
  ASTNode(ASTTag tag, ASTList children);
  // called by the Builder
  void setID(ID id) { id_ = id; }

 public:
  virtual ~ASTNode() = 0; // this is an abstract base class

  /**
    Returns the tag indicating which ASTNode subclass this is.
   */
  ASTTag tag() const {
    return tag_;
  }

  /**
    Returns the ID of this AST node.
   */
  ID id() const {
    return id_; 
  }

  /**
    Returns the number of child AST nodes in the tree directly under this one.
   */
  int numChildren() const {
    return children_.size();
  }

  /**
    Return a way to iterate over the children.
    */
  ASTListIteratorPair<ASTNode> children() const {
    return ASTListIteratorPair<ASTNode>(children_.begin(), children_.end());
  }

  /**
    Returns the i'th child AST node in the tree directly under this one.
    This function returns a "borrow" of the AST node. It is managed
    by this object.
   */
  const ASTNode* child(int i) const {
    assert(0 <= i && i < children_.size());
    return children_[i].get();
  }

  /**
    Returns 'true' if this symbol contains another AST node.
    This is an operation on the IDs.
   */
  bool contains(const ASTNode* other) const {
    return this->id_.contains(other->id_);
  }

  bool shallowMatch(const ASTNode* other) const;
  bool completeMatch(const ASTNode* other) const;

  // keep is some old AST
  // addin is some new AST we wish to combine with it
  //
  // on exit, keep stores the AST we need to keep, and anything
  // not kept is stored in 'addin'.
  // the function returns 'true' if anything changed in 'keep'.
  static bool updateAST(owned<ASTNode>& keep, owned<ASTNode>& addin);

  static void markAST(Context* context, const ASTNode* keep);

  static void dump(const ASTNode* ast, int leadingSpaces=0);

  // define is__ methods for the various AST types
  // using macros and ASTClassesList.h
  /// \cond DO_NOT_DOCUMENT
  #define AST_IS(NAME) \
    bool is##NAME() const { \
      return asttags::is##NAME(this->tag_); \
    }
  #define AST_NODE(NAME) AST_IS(NAME)
  #define AST_LEAF(NAME) AST_IS(NAME)
  #define AST_BEGIN_SUBCLASSES(NAME) AST_IS(NAME)
  #define AST_END_SUBCLASSES(NAME)
  /// \endcond
  // Apply the above macros to ASTClassesList.h
  #include "chpl/uast/ASTClassesList.h"
  // clear the macros
  #undef AST_NODE
  #undef AST_LEAF
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
  #undef AST_IS

  // define to__ methods for the various AST types
  // using macros and ASTClassesList.h
  // Note: these offer equivalent functionality to C++ dynamic_cast<DstType*>
  /// \cond DO_NOT_DOCUMENT
  #define AST_TO(NAME) \
    const NAME * to##NAME() const { \
      return this->is##NAME() ? (NAME *)this : nullptr; \
    } \
    NAME * to##NAME() { \
      return this->is##NAME() ? (NAME *)this : nullptr; \
    }
  #define AST_NODE(NAME) AST_TO(NAME)
  #define AST_LEAF(NAME) AST_TO(NAME)
  #define AST_BEGIN_SUBCLASSES(NAME) AST_TO(NAME)
  #define AST_END_SUBCLASSES(NAME)
  /// \endcond
  // Apply the above macros to ASTClassesList.h
  #include "chpl/uast/ASTClassesList.h"
  // clear the macros
  #undef AST_NODE
  #undef AST_LEAF
  #undef AST_BEGIN_SUBCLASSES
  #undef AST_END_SUBCLASSES
  #undef AST_TO
};


} // end namespace uast
} // end namespace chpl

/// \cond DO_NOT_DOCUMENT
namespace std {

// define std::less for the various AST types
// using macros and ASTClassesList.h
/// \cond DO_NOT_DOCUMENT
#define AST_LESS(NAME) \
  template<> struct less<chpl::uast::NAME*> { \
    bool operator()(const chpl::uast::NAME* lhs, \
                    const chpl::uast::NAME* rhs) const { \
      if (lhs == nullptr && rhs != nullptr) return true; \
      if (rhs == nullptr) return false; \
      std::less<chpl::ID> lessID; \
      /* cast in the next line is so it compiles with only forward decls */ \
      return lessID(((const chpl::uast::ASTNode*)lhs)->id(), \
                    ((const chpl::uast::ASTNode*)rhs)->id()); \
    } \
  };
#define AST_NODE(NAME) AST_LESS(NAME)
#define AST_LEAF(NAME) AST_LESS(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) AST_LESS(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to ASTClassesList.h
#include "chpl/uast/ASTClassesList.h"
// Additionally, apply the macro to ASTNode
AST_LESS(ASTNode)
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef AST_LESS


} // end namespace std
/// \endcond

#endif
