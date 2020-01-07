/*
 * Copyright 2004-2020 Cray Inc.
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

#ifndef _AST_VISITOR_TRAVERSE_H_
#define _AST_VISITOR_TRAVERSE_H_

#include "AstVisitor.h"

//
// AstVisitor is a pure virtual base class
//
//   A careful client of AstVisitor may want to take responsibility for
//   every virtual function.  In other cases this may seem onerous; for
//   example a given transform might only operate on a single node type.
//
//   This derived class causes the children of every node to be
//   traversed but performs no other action.  This allows sub-classes
//   to implement only the relevant functions.

class AstVisitorTraverse : public AstVisitor
{
public:
                 AstVisitorTraverse();
  virtual       ~AstVisitorTraverse();

  //
  // The sub-classes of Type
  //
  virtual bool   enterAggrType       (AggregateType*     node);
  virtual void   exitAggrType        (AggregateType*     node);

  virtual bool   enterDecoratedClassType(DecoratedClassType* node);
  virtual void   exitDecoratedClassType (DecoratedClassType* node);

  virtual bool   enterEnumType       (EnumType*          node);
  virtual void   exitEnumType        (EnumType*          node);

  virtual void   visitPrimType       (PrimitiveType*     node);

  //
  // The sub-classes of Symbol
  //
  virtual bool   enterArgSym         (ArgSymbol*         node);
  virtual void   exitArgSym          (ArgSymbol*         node);

  virtual void   visitEnumSym        (EnumSymbol*        node);

  virtual bool   enterFnSym          (FnSymbol*          node);
  virtual void   exitFnSym           (FnSymbol*          node);

  virtual void   visitLabelSym       (LabelSymbol*       node);

  virtual bool   enterModSym         (ModuleSymbol*      node);
  virtual void   exitModSym          (ModuleSymbol*      node);

  virtual bool   enterTypeSym        (TypeSymbol*        node);
  virtual void   exitTypeSym         (TypeSymbol*        node);

  virtual void   visitVarSym         (VarSymbol*         node);

  //
  // The sub-classes of Expr
  //
  virtual bool   enterCallExpr       (CallExpr*          node);
  virtual void   exitCallExpr        (CallExpr*          node);

  virtual bool   enterContextCallExpr(ContextCallExpr*   node);
  virtual void   exitContextCallExpr (ContextCallExpr*   node);

  virtual bool   enterDefExpr        (DefExpr*           node);
  virtual void   exitDefExpr         (DefExpr*           node);

  virtual bool   enterNamedExpr      (NamedExpr*         node);
  virtual void   exitNamedExpr       (NamedExpr*         node);

  virtual bool   enterIfExpr         (IfExpr*            node);
  virtual void   exitIfExpr          (IfExpr*            node);

  virtual void   visitSymExpr        (SymExpr*           node);

  virtual void   visitUsymExpr       (UnresolvedSymExpr* node);

  virtual bool   enterLoopExpr     (LoopExpr*        node);
  virtual void   exitLoopExpr      (LoopExpr*        node);

  //
  // The sub-classes of Stmt
  //
  virtual void   visitUseStmt        (UseStmt*           node);

  virtual bool   enterBlockStmt      (BlockStmt*         node);
  virtual void   exitBlockStmt       (BlockStmt*         node);

  virtual bool   enterForallStmt     (ForallStmt*        node);
  virtual void   exitForallStmt      (ForallStmt*        node);

  virtual bool   enterWhileDoStmt    (WhileDoStmt*       node);
  virtual void   exitWhileDoStmt     (WhileDoStmt*       node);

  virtual bool   enterDoWhileStmt    (DoWhileStmt*       node);
  virtual void   exitDoWhileStmt     (DoWhileStmt*       node);

  virtual bool   enterCForLoop       (CForLoop*          node);
  virtual void   exitCForLoop        (CForLoop*          node);

  virtual bool   enterForLoop        (ForLoop*           node);
  virtual void   exitForLoop         (ForLoop*           node);

  virtual bool   enterParamForLoop   (ParamForLoop*      node);
  virtual void   exitParamForLoop    (ParamForLoop*      node);

  virtual bool   enterCondStmt       (CondStmt*          node);
  virtual void   exitCondStmt        (CondStmt*          node);

  virtual void   visitEblockStmt     (ExternBlockStmt*   node);

  virtual bool   enterGotoStmt       (GotoStmt*          node);
  virtual void   exitGotoStmt        (GotoStmt*          node);

  virtual bool   enterForwardingStmt (ForwardingStmt*    node);
  virtual void   exitForwardingStmt  (ForwardingStmt*    node);

  virtual bool   enterDeferStmt      (DeferStmt*         node);
  virtual void   exitDeferStmt       (DeferStmt*         node);

  virtual bool   enterTryStmt        (TryStmt*           node);
  virtual void   exitTryStmt         (TryStmt*           node);

  virtual bool   enterCatchStmt      (CatchStmt*         node);
  virtual void   exitCatchStmt       (CatchStmt*         node);
};

#endif
