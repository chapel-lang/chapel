#ifndef _DEFINE_SYMBOLS_H_
#define _DEFINE_SYMBOLS_H_

/*** DEAD -- SJD [12/9/2004] ***/

#include "baseAST.h"
#include "traversal.h"
#include "symscope.h"

class DefineSymbols : public Traversal {
 public:
  SymScope* scope;
  DefineSymbols::DefineSymbols(SymScope* init_scope);
  void preProcessStmt(Stmt* &stmt);
};

#endif
