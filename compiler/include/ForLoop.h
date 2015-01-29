#ifndef _FOR_LOOP_H_
#define _FOR_LOOP_H_

#include "LoopStmt.h"

class ForLoop : public LoopStmt
{
  //
  // Class interface
  //
public:
  static BlockStmt*      buildForLoop (Expr*      indices,
                                       Expr*      iteratorExpr,
                                       BlockStmt* body,
                                       bool       coforall,
                                       bool       zippered);

  //
  // Instance Interface
  //
public:
                         ForLoop(VarSymbol* index,
                                 VarSymbol* iterator,
                                 BlockStmt* initBody);
  virtual               ~ForLoop();

  virtual ForLoop*       copy(SymbolMap* map      = NULL,
                              bool       internal = false);

  virtual GenRet         codegen();
  virtual void           verify();
  virtual void           accept(AstVisitor* visitor);

  virtual Expr*          getFirstExpr();
  virtual Expr*          getNextExpr(Expr* expr);

  virtual bool           isForLoop()                                  const;

  virtual bool           deadBlockCleanup();

  BlockStmt*             copyBody();
  BlockStmt*             copyBody(SymbolMap* map);

  SymExpr*               indexGet()                                   const;
  SymExpr*               iteratorGet()                                const;

  virtual CallExpr*      blockInfoGet()                               const;
  virtual CallExpr*      blockInfoSet(CallExpr* expr);

private:
                         ForLoop();

  SymExpr*               mIndex;
  SymExpr*               mIterator;
};

#endif

