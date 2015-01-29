#ifndef _AST_VISITOR_H_
#define _AST_VISITOR_H_

class AggregateType;
class EnumType;
class PrimitiveType;

class ArgSymbol;
class EnumSymbol;
class FnSymbol;
class IpeSymbol;
class LabelSymbol;
class ModuleSymbol;
class TypeSymbol;
class VarSymbol;

class CallExpr;
class DefExpr;
class NamedExpr;
class SymExpr;
class UnresolvedSymExpr;

class BlockStmt;
class WhileDoStmt;
class DoWhileStmt;
class CForLoop;
class ForLoop;
class ParamForLoop;
class ExternBlockStmt;
class CondStmt;
class GotoStmt;

class AstVisitor
{
public:
                 AstVisitor();
  virtual       ~AstVisitor();

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
  virtual bool   enterAggrType    (AggregateType*     node) = 0;
  virtual void   exitAggrType     (AggregateType*     node) = 0;

  virtual bool   enterEnumType    (EnumType*          node) = 0;
  virtual void   exitEnumType     (EnumType*          node) = 0;

  virtual void   visitPrimType    (PrimitiveType*     node) = 0;

  //
  // The sub-classes of Symbol
  //
  virtual bool   enterArgSym      (ArgSymbol*         node) = 0;
  virtual void   exitArgSym       (ArgSymbol*         node) = 0;

  virtual void   visitEnumSym     (EnumSymbol*        node) = 0;

  virtual bool   enterFnSym       (FnSymbol*          node) = 0;
  virtual void   exitFnSym        (FnSymbol*          node) = 0;

  virtual void   visitIpeSym      (IpeSymbol*         node) = 0;

  virtual void   visitLabelSym    (LabelSymbol*       node) = 0;

  virtual bool   enterModSym      (ModuleSymbol*      node) = 0;
  virtual void   exitModSym       (ModuleSymbol*      node) = 0;

  virtual bool   enterTypeSym     (TypeSymbol*        node) = 0;
  virtual void   exitTypeSym      (TypeSymbol*        node) = 0;

  virtual void   visitVarSym      (VarSymbol*         node) = 0;

  //
  // The sub-classes of Expr
  //
  virtual bool   enterCallExpr    (CallExpr*          node) = 0;
  virtual void   exitCallExpr     (CallExpr*          node) = 0;

  virtual bool   enterDefExpr     (DefExpr*           node) = 0;
  virtual void   exitDefExpr      (DefExpr*           node) = 0;

  virtual bool   enterNamedExpr   (NamedExpr*         node) = 0;
  virtual void   exitNamedExpr    (NamedExpr*         node) = 0;

  virtual void   visitSymExpr     (SymExpr*           node) = 0;

  virtual void   visitUsymExpr    (UnresolvedSymExpr* node) = 0;

  //
  // The sub-classes of Stmt
  //
  virtual bool   enterBlockStmt   (BlockStmt*         node) = 0;
  virtual void   exitBlockStmt    (BlockStmt*         node) = 0;

  virtual bool   enterWhileDoStmt (WhileDoStmt*       node) = 0;
  virtual void   exitWhileDoStmt  (WhileDoStmt*       node) = 0;

  virtual bool   enterDoWhileStmt (DoWhileStmt*       node) = 0;
  virtual void   exitDoWhileStmt  (DoWhileStmt*       node) = 0;

  virtual bool   enterCForLoop    (CForLoop*          node) = 0;
  virtual void   exitCForLoop     (CForLoop*          node) = 0;

  virtual bool   enterForLoop     (ForLoop*           node) = 0;
  virtual void   exitForLoop      (ForLoop*           node) = 0;

  virtual bool   enterParamForLoop(ParamForLoop*      node) = 0;
  virtual void   exitParamForLoop (ParamForLoop*      node) = 0;

  virtual bool   enterCondStmt    (CondStmt*          node) = 0;
  virtual void   exitCondStmt     (CondStmt*          node) = 0;

  virtual void   visitEblockStmt  (ExternBlockStmt*   node) = 0;

  virtual bool   enterGotoStmt    (GotoStmt*          node) = 0;
  virtual void   exitGotoStmt     (GotoStmt*          node) = 0;
};

#endif
