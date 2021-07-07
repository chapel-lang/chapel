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
#include <functional>

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
  ASTNode(ASTTag tag)
    : tag_(tag), id_(), children_() {
  }
  ASTNode(ASTTag tag, ASTList children)
    : tag_(tag), id_(), children_(std::move(children)) {
  }

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
    assert(0 <= i && i < (int) children_.size());
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

  // 'keep' is some old AST
  // 'addin' is some new AST we wish to combine with it
  //
  // on exit, 'keep' stores the AST we need to keep, and anything
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
      return this->is##NAME() ? (const NAME *)this : nullptr; \
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

  /**
     The dispatch function supports calling a method according to the tag
     (aka runtime type) of a uast node. It does not itself visit
     children of the uast node (but of course the called visit function
     is free to do so).

     It is a template and the Visitor argument should provide functions
     like

        MyReturnType MyVisitor::visit(const uast::Expression* ast);
        MyReturnType MyVisitor::visit(const uast::Variable* ast);

     and these will be invoked according to C++ overload resolution
     (where in particular an exact match will be preferred).

     It is generally necessary to specify the ReturnType when calling it, e.g.

       ast->dispatch<MyReturnType>(myVisitor);

     The return type currently needs to be default constructable.
   */
  template <typename ReturnType, typename Visitor>
  ReturnType dispatch(Visitor& v) const {

    switch (this->tag()) {
      #define CONVERT(NAME) \
        case chpl::uast::asttags::NAME: \
        { \
          return v.visit((const chpl::uast::NAME*) this); \
        }

      #define IGNORE(NAME) \
        case chpl::uast::asttags::NAME: \
        { \
          assert(false && "this code should never be run"); \
        }

      #define AST_NODE(NAME) CONVERT(NAME)
      #define AST_LEAF(NAME) CONVERT(NAME)
      #define AST_BEGIN_SUBCLASSES(NAME) IGNORE(START_##NAME)
      #define AST_END_SUBCLASSES(NAME) IGNORE(END_##NAME)

      #include "chpl/uast/ASTClassesList.h"

      IGNORE(NUM_AST_TAGS)

      #undef AST_NODE
      #undef AST_LEAF
      #undef AST_BEGIN_SUBCLASSES
      #undef AST_END_SUBCLASSES
      #undef CONVERT
      #undef IGNORE
    }

    ReturnType dummy;
    assert(false && "this code should never be run");
    return dummy;
  }
  /**
    Similar to dispatch but for visitors that return void.
   */
  template <typename Visitor>
  void dispatchVoid(Visitor& v) const {

    switch (this->tag()) {
      #define CONVERT(NAME) \
        case chpl::uast::asttags::NAME: \
        { \
          v.visit((const chpl::uast::NAME*) this); \
          return; \
        }

      #define IGNORE(NAME) \
        case chpl::uast::asttags::NAME: \
        { \
          assert(false && "this code should never be run"); \
        }

      #define AST_NODE(NAME) CONVERT(NAME)
      #define AST_LEAF(NAME) CONVERT(NAME)
      #define AST_BEGIN_SUBCLASSES(NAME) IGNORE(START_##NAME)
      #define AST_END_SUBCLASSES(NAME) IGNORE(END_##NAME)

      #include "chpl/uast/ASTClassesList.h"

      IGNORE(NUM_AST_TAGS)

      #undef AST_NODE
      #undef AST_LEAF
      #undef AST_BEGIN_SUBCLASSES
      #undef AST_END_SUBCLASSES
      #undef CONVERT
      #undef IGNORE
    }

    assert(false && "this code should never be run");
  }


  /**
     The traverse function supports calling a method according to the tag
     (aka runtime type) of a uast node and calling that method also
     on the children of the uast node.

     It is a template and the Visitor argument should provide functions
     like

        bool MyTraverser::enter(const uast::Expression* ast);
        void MyTraverser::exit(const uast::Expression* ast);
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
          v.exit(casted); \
          break; \
        }

      #define CASE_NODE(NAME) \
        case asttags::NAME: \
        { \
          const NAME* casted = (const NAME*) this; \
          bool goInToIt = v.enter(casted); \
          if (goInToIt) { \
            for (const ASTNode* child : this->children()) { \
              child->traverse(v); \
            } \
          } \
          v.exit(casted); \
          break; \
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

      // Apply the above macros to ASTClassesList.h
      // to fill in the cases
      #include "chpl/uast/ASTClassesList.h"
      // and also for NUM_AST_TAGS
      CASE_OTHER(NUM_AST_TAGS)

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
