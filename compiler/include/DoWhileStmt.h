#ifndef _DO_WHILE_STMT_H_
#define _DO_WHILE_STMT_H_

#include "WhileStmt.h"

class DoWhileStmt : public WhileStmt
{
  //
  // Class interface
  //
public:
  static BlockStmt*      build(Expr* cond, BlockStmt* body);


  //
  // Instance interface
  //
public:
  virtual               ~DoWhileStmt();

  virtual DoWhileStmt*   copy(SymbolMap* map = NULL, bool internal = false);

  virtual bool           isDoWhileStmt()                              const;

  virtual GenRet         codegen();
  virtual void           accept(AstVisitor* visitor);

  virtual Expr*          getFirstExpr();
  virtual Expr*          getNextExpr(Expr* expr);

private:
                         DoWhileStmt();

                         DoWhileStmt(Expr*      cond, BlockStmt* body);
                         DoWhileStmt(VarSymbol* var,  BlockStmt* body);
};

#endif

