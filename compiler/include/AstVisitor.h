#ifndef _AST_VISITOR_H_
#define _AST_VISITOR_H_

class AggregateType;
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
class DefExpr;
class NamedExpr;
class SymExpr;
class UnresolvedSymExpr;

class BlockStmt;
class ExternBlockStmt;
class CondStmt;
class GotoStmt;

class AstVisitor {
public:
                 AstVisitor();
  virtual       ~AstVisitor();

  //
  // The sub-classes of Type
  //
  virtual bool   visitEnter(AggregateType*     node) = 0;
  virtual void   visitExit (AggregateType*     node) = 0;

  virtual bool   visitEnter(EnumType*          node) = 0;
  virtual void   visitExit (EnumType*          node) = 0;

  virtual void   visit     (PrimitiveType*     node) = 0;

  //
  // The sub-classes of Symbol
  //
  virtual bool   visitEnter(ArgSymbol*         node) = 0;
  virtual void   visitExit (ArgSymbol*         node) = 0;

  virtual void   visit     (EnumSymbol*        node) = 0;

  virtual bool   visitEnter(FnSymbol*          node) = 0;
  virtual void   visitExit (FnSymbol*          node) = 0;

  virtual void   visit     (LabelSymbol*       node) = 0;

  virtual bool   visitEnter(ModuleSymbol*      node) = 0;
  virtual void   visitExit (ModuleSymbol*      node) = 0;

  virtual bool   visitEnter(TypeSymbol*        node) = 0;
  virtual void   visitExit (TypeSymbol*        node) = 0;

  virtual void   visit     (VarSymbol*         node) = 0;

  //
  // The sub-classes of Expr
  //
  virtual bool   visitEnter(CallExpr*          node) = 0;
  virtual void   visitExit (CallExpr*          node) = 0;

  virtual bool   visitEnter(DefExpr*           node) = 0;
  virtual void   visitExit (DefExpr*           node) = 0;

  virtual bool   visitEnter(NamedExpr*         node) = 0;
  virtual void   visitExit (NamedExpr*         node) = 0;

  virtual void   visit     (SymExpr*           node) = 0;

  virtual void   visit     (UnresolvedSymExpr* node) = 0;

  virtual bool   visitEnter(BlockStmt*         node) = 0;
  virtual void   visitExit (BlockStmt*         node) = 0;

  virtual bool   visitEnter(CondStmt*          node) = 0;
  virtual void   visitExit (CondStmt*          node) = 0;

  virtual void   visit     (ExternBlockStmt*   node) = 0;

  virtual bool   visitEnter(GotoStmt*          node) = 0;
  virtual void   visitExit (GotoStmt*          node) = 0;
};

#endif
