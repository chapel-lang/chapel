#ifndef _AST_LOGGGER_H_
#define _AST_LOGGGER_H_

#include "AstVisitor.h"

class AstLogger : public AstVisitor {
public:
                   AstLogger();
  virtual         ~AstLogger();

  virtual bool     visitEnter(AggregateType*     node);
  virtual void     visitExit (AggregateType*     node);

  virtual bool     visitEnter(EnumType*          node);
  virtual void     visitExit (EnumType*          node);

  virtual void     visit     (PrimitiveType*     node);

  virtual bool     visitEnter(ArgSymbol*         node);
  virtual void     visitExit (ArgSymbol*         node);

  virtual void     visit     (EnumSymbol*        node);

  virtual bool     visitEnter(FnSymbol*          node);
  virtual void     visitExit (FnSymbol*          node);

  virtual void     visit     (LabelSymbol*       node);

  virtual bool     visitEnter(ModuleSymbol*      node);
  virtual void     visitExit (ModuleSymbol*      node);

  virtual bool     visitEnter(TypeSymbol*        node);
  virtual void     visitExit (TypeSymbol*        node);

  virtual void     visit     (VarSymbol*         node);

  virtual bool     visitEnter(CallExpr*          node);
  virtual void     visitExit (CallExpr*          node);

  virtual bool     visitEnter(DefExpr*           node);
  virtual void     visitExit (DefExpr*           node);

  virtual bool     visitEnter(NamedExpr*         node);
  virtual void     visitExit (NamedExpr*         node);

  virtual void     visit     (SymExpr*           node);

  virtual void     visit     (UnresolvedSymExpr* node);

  virtual bool     visitEnter(BlockStmt*         node);
  virtual void     visitExit (BlockStmt*         node);

  virtual bool     visitEnter(CondStmt*          node);
  virtual void     visitExit (CondStmt*          node);

  virtual void     visit     (ExternBlockStmt*   node);

  virtual bool     visitEnter(GotoStmt*          node);
  virtual void     visitExit (GotoStmt*          node);
};

#endif
