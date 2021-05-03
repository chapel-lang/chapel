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

#ifndef CHPL_UAST_ASTBASE_H
#define CHPL_UAST_ASTBASE_H

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

  The list of child nodes is included in ASTBase to allow for
  generic tree traversals of the AST.

  Functions like someAst->isCall() / someAst->toCall() are available and
  generated for all AST types.

  std::less is defined for every AST class and it compares IDs.

 */
class ASTBase {
 friend class Builder;

 private:
  ASTTag tag_;
  ID id_;

 protected:
  ASTList children_;

  /**
    This function needs to be defined by subclasses.
    It should check only those fields defined in subclasses
    (it should not check the ASTBase fields tag, ID, or children).
    It can assume that other has the same type as the receiver.
   */
  virtual bool contentsMatchInner(const ASTBase* other) const = 0;

  /**
   This function need sto be defined by subclasses.
   It should call the 'mark' method on any UniqueStrings
   stored as fields. It need not worry about the children nodes
   or the UniqueStrings stored in the ID.
   */
  virtual void markUniqueStringsInner(Context* context) const = 0;

 protected:
  ASTBase(ASTTag tag);
  ASTBase(ASTTag tag, ASTList children);
  // called by the Builder
  void setID(ID id) { id_ = id; }

 public:
  virtual ~ASTBase() = 0; // this is an abstract base class

  /**
    Returns the tag indicating which ASTBase subclass this is.
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
  ASTListIteratorPair<ASTBase> children() const {
    return ASTListIteratorPair<ASTBase>(children_.begin(), children_.end());
  }

  /**
    Returns the i'th child AST node in the tree directly under this one.
    This function returns a "borrow" of the AST node. It is managed
    by this object.
   */
  const ASTBase* child(int i) const {
    assert(0 <= i && i < children_.size());
    return children_[i].get();
  }

  /**
    Returns 'true' if this symbol contains another AST node.
    This is an operation on the IDs.
   */
  bool contains(const ASTBase* other) const {
    return this->id_.contains(other->id_);
  }

  bool shallowMatch(const ASTBase* other) const;

  // keep is some old AST
  // addin is some new AST we wish to combine with it
  //
  // on exit, keep stores the AST we need to keep, and anything
  // not kept is stored in 'addin'.
  // the function returns 'true' if anything changed in 'keep'.
  static bool updateAST(owned<ASTBase>& keep, owned<ASTBase>& addin);

  static void markAST(Context* context, const ASTBase* keep);

  static void dump(const ASTBase* ast, int leadingSpaces=0);

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

#if 0
/// \cond DO_NOT_DOCUMENT
#define AST_MARK(NAME) \
  template<> struct mark<uast::NAME*> { \
    void operator()(Context* context, const uast::NAME*& keep) const { \
      /* cast in the next line is so it compiles with only forward decls */ \
      uast::ASTBase::markAST(context, (const uast::ASTBase*) keep); \
    } \
  };
#define AST_NODE(NAME) AST_MARK(NAME)
#define AST_LEAF(NAME) AST_MARK(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) AST_MARK(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to ASTClassesList.h
#include "chpl/ast/ASTClassesList.h"
// Additionally, apply the macro to ASTBase
AST_MARK(ASTBase)
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef AST_MARK
/// \endcond
#endif


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
      return lessID(((const chpl::uast::ASTBase*)lhs)->id(), \
                    ((const chpl::uast::ASTBase*)rhs)->id()); \
    } \
  };
#define AST_NODE(NAME) AST_LESS(NAME)
#define AST_LEAF(NAME) AST_LESS(NAME)
#define AST_BEGIN_SUBCLASSES(NAME) AST_LESS(NAME)
#define AST_END_SUBCLASSES(NAME)
/// \endcond
// Apply the above macros to ASTClassesList.h
#include "chpl/uast/ASTClassesList.h"
// Additionally, apply the macro to ASTBase
AST_LESS(ASTBase)
// clear the macros
#undef AST_NODE
#undef AST_LEAF
#undef AST_BEGIN_SUBCLASSES
#undef AST_END_SUBCLASSES
#undef AST_LESS


} // end namespace std
/// \endcond

#endif
