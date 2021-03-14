/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef _AST_COUNT_H_
#define _AST_COUNT_H_

#include "AstVisitor.h"
#include "baseAST.h"

class AstCount final : public AstVisitor
{
public:

  // Create member variables for each AST node type
  // that is handled by the baseAST macro.
#define decl_members(type) int num##type
foreach_ast(decl_members);
#undef decl_members

  // Create member variables for AST types not handled
  // by the baseAST macro.
  int numWhileDoStmt;
  int numDoWhileStmt;
  int numCForLoop;
  int numForLoop;
  int numParamForLoop;

   AstCount();
  ~AstCount() override = default;

  int total();

  //
  // The sub-classes of Type
  //
  bool   enterAggrType       (AggregateType*     node) override;
  void   exitAggrType        (AggregateType*     node) override;

  bool   enterDecoratedClassType(DecoratedClassType* node) override;
  void   exitDecoratedClassType (DecoratedClassType* node) override;

  bool   enterEnumType       (EnumType*          node) override;
  void   exitEnumType        (EnumType*          node) override;
  void   visitConstrainedType(ConstrainedType*   node) override;
  void   visitPrimType       (PrimitiveType*     node) override;

  //
  // The sub-classes of Symbol
  //
  bool   enterArgSym         (ArgSymbol*         node) override;
  void   exitArgSym          (ArgSymbol*         node) override;

  void   visitEnumSym        (EnumSymbol*        node) override;

  bool   enterFnSym          (FnSymbol*          node) override;
  void   exitFnSym           (FnSymbol*          node) override;

  bool   enterInterfaceSym   (InterfaceSymbol*   node) override;
  void   exitInterfaceSym    (InterfaceSymbol*   node) override;

  void   visitLabelSym       (LabelSymbol*       node) override;

  bool   enterModSym         (ModuleSymbol*      node) override;
  void   exitModSym          (ModuleSymbol*      node) override;

  bool   enterTypeSym        (TypeSymbol*        node) override;
  void   exitTypeSym         (TypeSymbol*        node) override;

  void   visitVarSym         (VarSymbol*         node) override;

  //
  // The sub-classes of Expr
  //
  bool   enterCallExpr       (CallExpr*          node) override;
  void   exitCallExpr        (CallExpr*          node) override;

  bool   enterContextCallExpr(ContextCallExpr*   node) override;
  void   exitContextCallExpr (ContextCallExpr*   node) override;

  bool   enterDefExpr        (DefExpr*           node) override;
  void   exitDefExpr         (DefExpr*           node) override;

  bool   enterNamedExpr      (NamedExpr*         node) override;
  void   exitNamedExpr       (NamedExpr*         node) override;

  bool   enterIfcConstraint  (IfcConstraint*     node) override;
  void   exitIfcConstraint   (IfcConstraint*     node) override;

  bool   enterIfExpr         (IfExpr*            node) override;
  void   exitIfExpr          (IfExpr*            node) override;

  void   visitSymExpr        (SymExpr*           node) override;

  void   visitUsymExpr       (UnresolvedSymExpr* node) override;

  bool   enterLoopExpr       (LoopExpr*          node) override;
  void   exitLoopExpr        (LoopExpr*          node) override;

  //
  // The sub-classes of Stmt
  //
  void   visitUseStmt        (UseStmt*           node) override;

  void   visitImportStmt     (ImportStmt*        node) override;

  bool   enterBlockStmt      (BlockStmt*         node) override;
  void   exitBlockStmt       (BlockStmt*         node) override;

  bool   enterForallStmt     (ForallStmt*        node) override;
  void   exitForallStmt      (ForallStmt*        node) override;

  bool   enterWhileDoStmt    (WhileDoStmt*       node) override;
  void   exitWhileDoStmt     (WhileDoStmt*       node) override;

  bool   enterDoWhileStmt    (DoWhileStmt*       node) override;
  void   exitDoWhileStmt     (DoWhileStmt*       node) override;

  bool   enterCForLoop       (CForLoop*          node) override;
  void   exitCForLoop        (CForLoop*          node) override;

  bool   enterForLoop        (ForLoop*           node) override;
  void   exitForLoop         (ForLoop*           node) override;

  bool   enterParamForLoop   (ParamForLoop*      node) override;
  void   exitParamForLoop    (ParamForLoop*      node) override;

  bool   enterCondStmt       (CondStmt*          node) override;
  void   exitCondStmt        (CondStmt*          node) override;

  void   visitEblockStmt     (ExternBlockStmt*   node) override;

  bool   enterForwardingStmt (ForwardingStmt*    node) override;
  void   exitForwardingStmt  (ForwardingStmt*    node) override;

  bool   enterGotoStmt       (GotoStmt*          node) override;
  void   exitGotoStmt        (GotoStmt*          node) override;

  bool   enterDeferStmt      (DeferStmt*         node) override;
  void   exitDeferStmt       (DeferStmt*         node) override;

  bool   enterTryStmt        (TryStmt*           node) override;
  void   exitTryStmt         (TryStmt*           node) override;

  bool   enterCatchStmt      (CatchStmt*         node) override;
  void   exitCatchStmt       (CatchStmt*         node) override;

  bool   enterImplementsStmt (ImplementsStmt*    node) override;
  void   exitImplementsStmt  (ImplementsStmt*    node) override;

};

#endif
