#ifndef _ELIMINATE_RETURNS_H_
#define _ELIMINATE_RETURNS_H_

#include "traversal.h"
#include "symbol.h"
#include "map.h"
#include "stmt.h"
#include "symscope.h"

class EliminateReturns : public Traversal {
 public:
  int uid;
  Map<Symbol*, LabelSymbol*>* label_ret_map;
  EliminateReturns(void);
 
  void preProcessStmt(Stmt* stmt);
  void createLabelWithRetAtFuncEnd(ReturnStmt* ret_stmt, FnSymbol* fn_sym);
  bool isLastStmtInFunc(ReturnStmt* ret_stmt, BlockStmt* block);
  void replaceRetWithGoto(ReturnStmt* ret_stmt, FnSymbol* fn_sym);
};

void eliminateReturns(void);

#endif
