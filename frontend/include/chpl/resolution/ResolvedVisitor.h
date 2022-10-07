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

#ifndef CHPL_RESOLUTION_RESOLVED_VISITOR_H
#define CHPL_RESOLUTION_RESOLVED_VISITOR_H

#include "chpl/resolution/resolution-types.h"
#include "chpl/uast/all-uast.h"

namespace chpl {
namespace resolution {

using namespace uast;

/**
  This class enables visiting resolved uAST nodes.
  It is a kind of adapter that converts untyped visiting (traversing uAST nodes
  with a ResolvedVisitor) to typed visiting (traversing uAST nodes with the
  provided User Visitor (UV)).

  The enter/exit calls invoke enter/exit on the User Visitor while passing in a
  reference to the current ResolvedVisitor. It is possible to get the type of a
  uAST node from the current ResolvedVisitor.
  */
template <typename UV>
class ResolvedVisitor {
  Context* context_ = nullptr;
  const uast::AstNode* ast_ = nullptr;
  UV& userVisitor_;

  // the resolution results for the contained AstNodes
  const ResolutionResultByPostorderID& byPostorder_;

public:
  ResolvedVisitor(Context* context,
           const uast::AstNode* ast,
           UV& userVisitor,
           const ResolutionResultByPostorderID& byPostorder)
    : context_(context),
      ast_(ast),
      userVisitor_(userVisitor),
      byPostorder_(byPostorder) {
  }

  /** Return the context used by this ResolvedVisitor */
  Context* context() const {
    return context_;
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
    if (loop->isParam()) {
      // Enter the param for-loop with the current resolution results, so that
      // the user-defined visitor can choose to do something custom if it
      // wants.
      bool goInto = userVisitor_.enter(loop, *this);

      if (goInto) {
        const ResolvedExpression& rr = byPostorder_.byAst(loop);
        const ResolvedParamLoop* resolvedLoop = rr.paramLoop();

        const AstNode* iterand = loop->iterand();
        iterand->traverse(*this);

        // TODO: Should there be some kind of function the UserVisitor can
        // implement to observe a new iteration of the loop body?
        for (auto loopBody : resolvedLoop->loopBodies()) {
          ResolvedVisitor<UV> loopVis(context_, loop, userVisitor_, loopBody);

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
      return userVisitor_.enter(loop, *this);
    }
  }

  void exit(const uast::For* loop) {
    userVisitor_.exit(loop, *this);
  }

  // if none of the above is called, fall back on this one
  bool enter(const uast::AstNode* ast) {
    switch (ast->tag()) {
      #define CASE_LEAF(NAME) \
        case asttags::NAME: \
        { \
          const NAME* casted = (const NAME*) ast; \
          userVisitor_.enter(casted, *this); \
          assert(ast->numChildren() == 0); \
          userVisitor_.exit(casted, *this); \
          break; \
        }

      #define CASE_NODE(NAME) \
        case asttags::NAME: \
        { \
          const NAME* casted = (const NAME*) ast; \
          bool goInToIt = userVisitor_.enter(casted, *this); \
          if (goInToIt) { \
            for (const AstNode* child : ast->children()) { \
              child->traverse(*this); \
            } \
          } \
          userVisitor_.exit(casted, *this); \
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
  void exit(const uast::AstNode* ast) { };
};

/** Similar to ResolvedVisitor but this one works with a mutable
    ResolutionResultByPostorderID.
 */
template <typename UV>
class MutatingResolvedVisitor {
  Context* context_ = nullptr;
  const uast::AstNode* ast_ = nullptr;
  UV& userVisitor_;

  // the resolution results for the contained AstNodes
  ResolutionResultByPostorderID& byPostorder_;

public:
  MutatingResolvedVisitor(Context* context,
           const uast::AstNode* ast,
           UV& userVisitor,
           ResolutionResultByPostorderID& byPostorder)
    : context_(context),
      ast_(ast),
      userVisitor_(userVisitor),
      byPostorder_(byPostorder) {
  }

  /** Return the context used by this ResolvedVisitor */
  Context* context() const {
    return context_;
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
    if (loop->isParam()) {
      // Enter the param for-loop with the current resolution results, so that
      // the user-defined visitor can choose to do something custom if it
      // wants.
      bool goInto = userVisitor_.enter(loop, *this);

      if (goInto) {
        ResolvedExpression& rr = byPostorder_.byAst(loop);
        const ResolvedParamLoop* resolvedLoop = rr.paramLoop();

        const AstNode* iterand = loop->iterand();
        iterand->traverse(*this);

        // TODO: Should there be some kind of function the UserVisitor can
        // implement to observe a new iteration of the loop body?
        for (auto loopBody : resolvedLoop->loopBodies()) {
          MutatingResolvedVisitor<UV> loopVis(context_, loop, userVisitor_, loopBody);

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
      return userVisitor_.enter(loop, *this);
    }
  }

  void exit(const uast::For* loop) {
    userVisitor_.exit(loop, *this);
  }

  // if none of the above is called, fall back on this one
  bool enter(const uast::AstNode* ast) {
    switch (ast->tag()) {
      #define CASE_LEAF(NAME) \
        case asttags::NAME: \
        { \
          const NAME* casted = (const NAME*) ast; \
          userVisitor_.enter(casted, *this); \
          assert(ast->numChildren() == 0); \
          userVisitor_.exit(casted, *this); \
          break; \
        }

      #define CASE_NODE(NAME) \
        case asttags::NAME: \
        { \
          const NAME* casted = (const NAME*) ast; \
          bool goInToIt = userVisitor_.enter(casted, *this); \
          if (goInToIt) { \
            for (const AstNode* child : ast->children()) { \
              child->traverse(*this); \
            } \
          } \
          userVisitor_.exit(casted, *this); \
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
  void exit(const uast::AstNode* ast) { };
};



} // end namespace resolution


} // end namespace chpl

#endif
