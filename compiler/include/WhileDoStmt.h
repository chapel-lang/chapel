#ifndef _WHILE_DO_STMT_H_
#define _WHILE_DO_STMT_H_

#include "WhileStmt.h"

class WhileDoStmt : public WhileStmt
{
  //
  // Class interface
  //
public:
  static BlockStmt*      build(Expr* cond, BlockStmt* body);

private:
  static bool            isPrimitiveCForLoop(Expr* cond);


  //
  // Instance interface
  //
public:
  virtual               ~WhileDoStmt();

  virtual WhileDoStmt*   copy(SymbolMap* map = NULL, bool internal = false);

  virtual bool           isWhileDoStmt()                              const;

  virtual GenRet         codegen();
  virtual void           accept(AstVisitor* visitor);

  virtual Expr*          getFirstExpr();
  virtual Expr*          getNextExpr(Expr* expr);

private:
                         WhileDoStmt();

                         WhileDoStmt(Expr*      cond, BlockStmt* body);
                         WhileDoStmt(VarSymbol* var,  BlockStmt* body);
};

#endif

