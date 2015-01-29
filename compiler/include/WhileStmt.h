#ifndef _WHILE_STMT_H_
#define _WHILE_STMT_H_

#include "LoopStmt.h"

class WhileStmt : public LoopStmt
{
public:
  Expr*                  condExprGet()                                const;
  SymExpr*               condExprForTmpVariableGet()                  const;

protected:
                         WhileStmt(Expr*      sym,
                                   BlockStmt* initBody);

                         WhileStmt(VarSymbol* sym,
                                   BlockStmt* initBody);

  virtual               ~WhileStmt();

  void                   copyShare(const WhileStmt& ref,
                                   SymbolMap*       mapRef,
                                   bool             internal);

  // Interface to BaseAST
  virtual void           verify();

  // Interface to Expr
  virtual void           replaceChild(Expr* oldAst, Expr* newAst);

  // New interface
  virtual bool           isWhileStmt()                                const;

  virtual void           checkConstLoops();

  virtual bool           deadBlockCleanup();

  virtual CallExpr*      blockInfoGet()                               const;
  virtual CallExpr*      blockInfoSet(CallExpr* expr);

private:
                         WhileStmt();

  // Helper functions for checkConstLoops()
  SymExpr*               getWhileCondDef(VarSymbol* condSym);
  void                   checkWhileLoopCondition(Expr* condExp);
  bool                   symDeclaredInBlock(Symbol* condSym);
  void                   checkConstWhileLoop();
  bool                   loopBodyHasExits();

  Expr*                  mCondExpr;
};

#endif
