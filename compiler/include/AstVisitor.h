/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#ifndef _AST_VISITOR_H_
#define _AST_VISITOR_H_

class AggregateType;
class DecoratedClassType;
class EnumType;
class PrimitiveType;

class ArgSymbol;
class EnumSymbol;
class FnSymbol;
class LabelSymbol;
class ModuleSymbol;
class TypeSymbol;
class VarSymbol;

class CallExpr;
class ContextCallExpr;
class DefExpr;
class NamedExpr;
class IfExpr;
class SymExpr;
class UnresolvedSymExpr;
class LoopExpr;

class UseStmt;
class ImportStmt;
class BlockStmt;
class ForallStmt;
class WhileDoStmt;
class DoWhileStmt;
class CForLoop;
class ForLoop;
class ParamForLoop;
class ExternBlockStmt;
class CondStmt;
class GotoStmt;
class ForwardingStmt;
class DeferStmt;
class TryStmt;
class CatchStmt;

class AstVisitor
{
public:
                 AstVisitor();
  virtual       ~AstVisitor();

  // Generally an AST visitor has one or two routine per type of AST node.
  // For nodes that can contain other nodes, it has 2 routines:
  //   bool enterSomething(Something* node)
  //   void exitSomething(Something* node)
  // And for nodes that do not contain other nodes, it has
  //   void visitSomething(Something* node)
  //
  // If enterSomething returns `true`, it indicates that:
  //   * nested nodes should be visited
  //   * exitSomething should be called for that node

  //
  // The first implementation of this pattern used a traditional naming
  // convention that relied on overloading of virtual functions.  Although
  // this was functionally safe for this use-case, one of our configurations
  // causes GCC to be more particular and issue warnings.
  //
  // Switched to a more robust naming convention that avoids this issue.
  //

  //
  // The sub-classes of Type
  //
  virtual bool   enterAggrType       (AggregateType*     node) = 0;
  virtual void   exitAggrType        (AggregateType*     node) = 0;

  virtual bool   enterDecoratedClassType(DecoratedClassType*     node) = 0;
  virtual void   exitDecoratedClassType (DecoratedClassType*     node) = 0;

  virtual bool   enterEnumType       (EnumType*          node) = 0;
  virtual void   exitEnumType        (EnumType*          node) = 0;

  virtual void   visitPrimType       (PrimitiveType*     node) = 0;

  //
  // The sub-classes of Symbol
  //
  virtual bool   enterArgSym         (ArgSymbol*         node) = 0;
  virtual void   exitArgSym          (ArgSymbol*         node) = 0;

  virtual void   visitEnumSym        (EnumSymbol*        node) = 0;

  virtual bool   enterFnSym          (FnSymbol*          node) = 0;
  virtual void   exitFnSym           (FnSymbol*          node) = 0;

  virtual void   visitLabelSym       (LabelSymbol*       node) = 0;

  virtual bool   enterModSym         (ModuleSymbol*      node) = 0;
  virtual void   exitModSym          (ModuleSymbol*      node) = 0;

  virtual bool   enterTypeSym        (TypeSymbol*        node) = 0;
  virtual void   exitTypeSym         (TypeSymbol*        node) = 0;

  virtual void   visitVarSym         (VarSymbol*         node) = 0;

  //
  // The sub-classes of Expr
  //
  virtual bool   enterCallExpr       (CallExpr*          node) = 0;
  virtual void   exitCallExpr        (CallExpr*          node) = 0;

  virtual bool   enterContextCallExpr(ContextCallExpr*   node) = 0;
  virtual void   exitContextCallExpr (ContextCallExpr*   node) = 0;

  virtual bool   enterDefExpr        (DefExpr*           node) = 0;
  virtual void   exitDefExpr         (DefExpr*           node) = 0;

  virtual bool   enterNamedExpr      (NamedExpr*         node) = 0;
  virtual void   exitNamedExpr       (NamedExpr*         node) = 0;

  virtual bool   enterIfExpr         (IfExpr*            node) = 0;
  virtual void   exitIfExpr          (IfExpr*            node) = 0;

  virtual void   visitSymExpr        (SymExpr*           node) = 0;

  virtual void   visitUsymExpr       (UnresolvedSymExpr* node) = 0;

  virtual bool   enterLoopExpr     (LoopExpr*        node) = 0;
  virtual void   exitLoopExpr      (LoopExpr*        node) = 0;

  //
  // The sub-classes of Stmt
  //
  virtual void   visitUseStmt        (UseStmt*           node) = 0;

  virtual void   visitImportStmt     (ImportStmt*        node) = 0;

  virtual bool   enterBlockStmt      (BlockStmt*         node) = 0;
  virtual void   exitBlockStmt       (BlockStmt*         node) = 0;

  virtual bool   enterForallStmt     (ForallStmt*        node) = 0;
  virtual void   exitForallStmt      (ForallStmt*        node) = 0;

  virtual bool   enterWhileDoStmt    (WhileDoStmt*       node) = 0;
  virtual void   exitWhileDoStmt     (WhileDoStmt*       node) = 0;

  virtual bool   enterDoWhileStmt    (DoWhileStmt*       node) = 0;
  virtual void   exitDoWhileStmt     (DoWhileStmt*       node) = 0;

  virtual bool   enterCForLoop       (CForLoop*          node) = 0;
  virtual void   exitCForLoop        (CForLoop*          node) = 0;

  virtual bool   enterForLoop        (ForLoop*           node) = 0;
  virtual void   exitForLoop         (ForLoop*           node) = 0;

  virtual bool   enterParamForLoop   (ParamForLoop*      node) = 0;
  virtual void   exitParamForLoop    (ParamForLoop*      node) = 0;

  virtual bool   enterCondStmt       (CondStmt*          node) = 0;
  virtual void   exitCondStmt        (CondStmt*          node) = 0;

  virtual bool   enterForwardingStmt (ForwardingStmt*    node) = 0;
  virtual void   exitForwardingStmt  (ForwardingStmt*    node) = 0;

  virtual void   visitEblockStmt     (ExternBlockStmt*   node) = 0;

  virtual bool   enterGotoStmt       (GotoStmt*          node) = 0;
  virtual void   exitGotoStmt        (GotoStmt*          node) = 0;

  virtual bool   enterDeferStmt      (DeferStmt*         node) = 0;
  virtual void   exitDeferStmt       (DeferStmt*         node) = 0;

  virtual bool   enterTryStmt        (TryStmt*           node) = 0;
  virtual void   exitTryStmt         (TryStmt*           node) = 0;

  virtual bool   enterCatchStmt      (CatchStmt*         node) = 0;
  virtual void   exitCatchStmt       (CatchStmt*         node) = 0;
};

#endif
