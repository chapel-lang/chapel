#ifndef _AST_LOGGGER_H_
#define _AST_LOGGGER_H_

#include "AstVisitor.h"

class AstLogger : public AstVisitor {
public:
                   AstLogger();
  virtual         ~AstLogger();

  //
  // The sub-classes of Type
  //
  virtual bool     enterAggrType  (AggregateType*     node);
  virtual void     exitAggrType   (AggregateType*     node);

  virtual bool     enterEnumType  (EnumType*          node);
  virtual void     exitEnumType   (EnumType*          node);

  virtual void     visitPrimType  (PrimitiveType*     node);

  //
  // The sub-classes of Symbol
  //
  virtual bool     enterArgSym    (ArgSymbol*         node);
  virtual void     exitArgSym     (ArgSymbol*         node);

  virtual void     visitEnumSym   (EnumSymbol*        node);

  virtual bool     enterFnSym     (FnSymbol*          node);
  virtual void     exitFnSym      (FnSymbol*          node);

  virtual void     visitLabelSym  (LabelSymbol*       node);

  virtual bool     enterModSym    (ModuleSymbol*      node);
  virtual void     exitModSym     (ModuleSymbol*      node);

  virtual bool     enterTypeSym   (TypeSymbol*        node);
  virtual void     exitTypeSym    (TypeSymbol*        node);

  virtual void     visitVarSym    (VarSymbol*         node);

  //
  // The sub-classes of Expr
  //
  virtual bool     enterCallExpr  (CallExpr*          node);
  virtual void     exitCallExpr   (CallExpr*          node);

  virtual bool     enterDefExpr   (DefExpr*           node);
  virtual void     exitDefExpr    (DefExpr*           node);

  virtual bool     enterNamedExpr (NamedExpr*         node);
  virtual void     exitNamedExpr  (NamedExpr*         node);

  virtual void     visitSymExpr   (SymExpr*           node);

  virtual void     visitUsymExpr  (UnresolvedSymExpr* node);

  virtual bool     enterBlockStmt (BlockStmt*         node);
  virtual void     exitBlockStmt  (BlockStmt*         node);

  virtual bool     enterCondStmt  (CondStmt*          node);
  virtual void     exitCondStmt   (CondStmt*          node);

  virtual void     visitEblockStmt(ExternBlockStmt*   node);

  virtual bool     enterGotoStmt  (GotoStmt*          node);
  virtual void     exitGotoStmt   (GotoStmt*          node);
};

#endif
