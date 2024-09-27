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

#ifndef CHPL_RESOLUTION_RESOLVED_VISITOR_H
#define CHPL_RESOLUTION_RESOLVED_VISITOR_H

#include "chpl/resolution/resolution-types.h"
#include "chpl/uast/all-uast.h"

namespace chpl {
namespace resolution {

using namespace uast;

template <typename ResolvedVisitorImpl>
static bool resolvedVisitorEnterFor(ResolvedVisitorImpl& v,
                                    const uast::For* loop) {
  if (loop->isParam()) {
    // Enter the param for-loop with the current resolution results, so that
    // the user-defined visitor can choose to do something custom if it
    // wants.
    bool goInto = v.userVisitor().enter(loop, v);

    if (goInto) {
      const ResolvedExpression& rr = v.byAst(loop);
      const ResolvedParamLoop* resolvedLoop = rr.paramLoop();

      if (resolvedLoop == nullptr) return false;

      const AstNode* iterand = loop->iterand();
      iterand->traverse(v);

      // TODO: Should there be some kind of function the UserVisitor can
      // implement to observe a new iteration of the loop body?
      for (const auto& loopBody : resolvedLoop->loopBodies()) {
        ResolvedVisitorImpl loopVis(v.rc(), loop,
                                    v.userVisitor(), loopBody);

        for (const AstNode* child : loop->children()) {
          // Written to visit "all but the iterand" in case we add more
          // fields/children to the For class later.
          if (child != iterand) {
            child->traverse(loopVis);
          }
        }
      }
    }

    return false;
  } else {
    return v.userVisitor().enter(loop, v);
  }
}

template <typename ResolvedVisitorImpl>
static bool resolvedVisitorEnterAst(ResolvedVisitorImpl& v,
                                    const uast::AstNode* ast) {
  switch (ast->tag()) {
    #define CASE_LEAF(NAME) \
      case asttags::NAME: \
      { \
        const NAME* casted = (const NAME*) ast; \
        v.userVisitor().enter(casted, v); \
        CHPL_ASSERT(ast->numChildren() == 0); \
        v.userVisitor().exit(casted, v); \
        break; \
      }

    #define CASE_NODE(NAME) \
      case asttags::NAME: \
      { \
        const NAME* casted = (const NAME*) ast; \
        bool goInToIt = v.userVisitor().enter(casted, v); \
        if (goInToIt) { \
          for (const AstNode* child : ast->children()) { \
            child->traverse(v); \
          } \
        } \
        v.userVisitor().exit(casted, v); \
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

  return false;

}

/**
  This class enables visiting resolved uAST nodes.
  It is a kind of adapter that converts untyped visiting (traversing uAST nodes
  with a ResolvedVisitor) to typed visiting (traversing uAST nodes with the
  provided User Visitor (UV)).

  The enter/exit calls invoke enter/exit on the User Visitor while passing in a
  reference to the current ResolvedVisitor. It is possible to get the type of a
  uAST node from the current ResolvedVisitor.

  To use this, create a custom class, and within it, declare enter/exit
  calls like so:

    class MyResolvedVisitor {
      ...
      bool enter(const AstNode* ast, RV& rv);
      void exit(const AstNode* ast, RV& rv);
      ...
    }

  Then, use this pattern to visit:

    ResolvedVisitor<MyResolvedVisitor> rv(context, symbol,
                                          myResolvedVisitor, byPostorder);
    symbol->traverse(rv);

  */
template <typename UV>
class ResolvedVisitor {
  using UserVisitorType = UV;
  ResolutionContext* rc_ = nullptr;
  const uast::AstNode* ast_ = nullptr;
  UV& userVisitor_;

  // the resolution results for the contained AstNodes
  const ResolutionResultByPostorderID& byPostorder_;

public:
  ResolvedVisitor(ResolutionContext* rc,
           const uast::AstNode* ast,
           UV& userVisitor,
           const ResolutionResultByPostorderID& byPostorder)
    : rc_(rc),
      ast_(ast),
      userVisitor_(userVisitor),
      byPostorder_(byPostorder) {
  }

  /** Return the ResolutionContext used by this ResolvedVisitor */
  ResolutionContext* rc() const { return rc_; }

  /** Return the context used by this ResolvedVisitor */
  Context* context() const {
    return rc_->context();
  }
  /** Return the uAST node being visited by this ResolvedVisitor */
  const uast::AstNode* ast() const {
    return ast_;
  }
  /** Return the user visitor that this ResolvedVisitor invokes */
  UV& userVisitor() {
    return userVisitor_;
  }
  /** Return the user visitor that this ResolvedVisitor invokes */
  const UV& userVisitor() const {
    return userVisitor_;
  }
  /** Return the current ResolutionResultByPostorderID which can be used to
      gather type information */
  const ResolutionResultByPostorderID& byPostorder() const {
    return byPostorder_;
  }
  /** Returns if the ResolutionResultByPostorderID has a result for
      a particular uAST node */
  bool hasAst(const uast::AstNode* ast) const {
    return byPostorder_.hasAst(ast);
  }
  /** Return the ResolvedExpression for a particular uAST node */
  const ResolvedExpression& byAst(const uast::AstNode* ast) const {
    return byPostorder_.byAst(ast);
  }
  /** Returns if the ResolutionResultByPostorderID has a result for
      a particular ID */
  bool hasId(const ID& id) const {
    return byPostorder_.hasId(id);
  }
  /** Return the ResolvedExpression for a particular ID */
  const ResolvedExpression& byId(const ID& id) const {
    return byPostorder_.byId(id);
  }


  /*
   * Visiting a param for-loop has special behavior. The user's visitor
   * will be invoked with the current resolution results. These results will
   * contain valid information for the iterand, and the means to access the
   * per-iteration resolution results in case the ResolvedVisitor's default
   * behavior is insufficient.
   *
   * If the user's visitor returns true, then the ResolvedVisitor will invoke
   * the user's visitor once on the iterand, and will then invoke the user's
   * visitor on the loop index and loop body for each iteration of the param
   * for-loop.
   */
  bool enter(const uast::For* loop) {
    return resolvedVisitorEnterFor(*this, loop);
  }
  void exit(const uast::For* loop) {
    userVisitor_.exit(loop, *this);
  }

  // if none of the above is called, fall back on this one
  bool enter(const uast::AstNode* ast) {
    return resolvedVisitorEnterAst(*this, ast);
  }
  void exit(const uast::AstNode* ast) { };
};

/** Similar to ResolvedVisitor but this one works with a mutable
    ResolutionResultByPostorderID.
 */
template <typename UV>
class MutatingResolvedVisitor {
  using UserVisitorType = UV;
  ResolutionContext* rc_ = nullptr;
  const uast::AstNode* ast_ = nullptr;
  UV& userVisitor_;

  // the resolution results for the contained AstNodes
  ResolutionResultByPostorderID& byPostorder_;

public:
  MutatingResolvedVisitor(ResolutionContext* rc,
                          const uast::AstNode* ast,
                          UV& userVisitor,
                          const ResolutionResultByPostorderID& byPostorder)
    : rc_(rc),
      ast_(ast),
      userVisitor_(userVisitor),
      byPostorder_(const_cast<ResolutionResultByPostorderID&>(byPostorder)) {
  }

  /** Return the ResolutionContext used by this ResolvedVisitor */
  ResolutionContext* rc() const { return rc_; }

  /** Return the context used by this ResolvedVisitor */
  Context* context() const {
    return rc_->context();;
  }
  /** Return the uAST node being visited by this ResolvedVisitor */
  const uast::AstNode* ast() const {
    return ast_;
  }
  /** Return the user visitor that this ResolvedVisitor invokes */
  UV& userVisitor() {
    return userVisitor_;
  }
  /** Return the user visitor that this ResolvedVisitor invokes */
  const UV& userVisitor() const {
    return userVisitor_;
  }
  /** Return the current ResolutionResultByPostorderID which can be used to
      gather type information */
  ResolutionResultByPostorderID& byPostorder() const {
    return byPostorder_;
  }
  /** Returns if the ResolutionResultByPostorderID has a result for
      a particular uAST node */
  bool hasAst(const uast::AstNode* ast) const {
    return byPostorder_.hasAst(ast);
  }
  /** Return the ResolvedExpression for a particular uAST node */
  ResolvedExpression& byAst(const uast::AstNode* ast) const {
    return byPostorder_.byAst(ast);
  }
  /** Returns if the ResolutionResultByPostorderID has a result for
      a particular ID */
  bool hasId(const ID& id) const {
    return byPostorder_.hasId(id);
  }
  /** Return the ResolvedExpression for a particular ID */
  ResolvedExpression& byId(const ID& id) const {
    return byPostorder_.byId(id);
  }


  /*
   * Visiting a param for-loop has special behavior. The user's visitor
   * will be invoked with the current resolution results. These results will
   * contain valid information for the iterand, and the means to access the
   * per-iteration resolution results in case the ResolvedVisitor's default
   * behavior is insufficient.
   *
   * If the user's visitor returns true, then the ResolvedVisitor will invoke
   * the user's visitor once on the iterand, and will then invoke the user's
   * visitor on the loop index and loop body for each iteration of the param
   * for-loop.
   */
  bool enter(const uast::For* loop) {
    return resolvedVisitorEnterFor(*this, loop);
  }

  void exit(const uast::For* loop) {
    userVisitor_.exit(loop, *this);
  }

  // if none of the above is called, fall back on this one
  bool enter(const uast::AstNode* ast) {
    return resolvedVisitorEnterAst(*this, ast);
  }
  void exit(const uast::AstNode* ast) { };
};



} // end namespace resolution


} // end namespace chpl

#endif
