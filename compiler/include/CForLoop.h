#ifndef _CFOR_LOOP_H_
#define _CFOR_LOOP_H_

#include "LoopStmt.h"

class ForLoop;

class CForLoop : public LoopStmt
{
  //
  // Class interface
  //
public:
  static BlockStmt*      buildCForLoop(CallExpr*  cforInfo,
                                       BlockStmt* body);

  static CForLoop*       buildWithBodyFrom(ForLoop* forLoop);

  static CForLoop*       loopForClause(BlockStmt* clause);

  //
  // Instance Interface
  //
public:
  virtual               ~CForLoop();

  virtual CForLoop*      copy(SymbolMap* map = NULL, bool internal = false);

  virtual GenRet         codegen();
  virtual void           verify();
  virtual void           accept(AstVisitor* visitor);

  virtual Expr*          getFirstExpr();
  virtual Expr*          getNextExpr(Expr* expr);

  virtual bool           isCForLoop()                                 const;

  virtual bool           deadBlockCleanup();

  void                   loopHeaderSet(BlockStmt* initBlock,
                                       BlockStmt* testBlock,
                                       BlockStmt* incrBlock);

  BlockStmt*             initBlockGet()                               const;
  BlockStmt*             testBlockGet()                               const;
  BlockStmt*             incrBlockGet()                               const;

  virtual CallExpr*      blockInfoGet()                               const;
  virtual CallExpr*      blockInfoSet(CallExpr* expr);

private:
                         CForLoop();

                         CForLoop(BlockStmt* body);

  std::string            codegenCForLoopHeader   (BlockStmt* block);
  GenRet                 codegenCForLoopCondition(BlockStmt* block);

  BlockStmt*             mInitClause;
  BlockStmt*             mTestClause;
  BlockStmt*             mIncrClause;
};

#endif

