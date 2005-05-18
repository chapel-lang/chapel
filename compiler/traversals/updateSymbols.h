#ifndef _UPDATE_SYMBOLS_H_
#define _UPDATE_SYMBOLS_H_

#include "traversal.h"
#include "baseAST.h"
#include "vec.h"

#include "map.h"
#include "baseAST.h"

/***
 *** This traversal is called by copy to update the symbols in an
 *** expression or a statement for those symbols that have themselves
 *** been copied.
 ***/
class UpdateSymbols : public Traversal {
 public:
  bool changed;
  Map<BaseAST*,BaseAST*>* copy_map;
  UpdateSymbols(Map<BaseAST*,BaseAST*>* init_copy_map);
  void preProcessExpr(Expr* expr);
  void preProcessSymbol(Symbol* symbol);
};

#endif
