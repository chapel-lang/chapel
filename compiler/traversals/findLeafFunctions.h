#ifndef _FIND_LEAF_FUNCTIONS_
#define _FIND_LEAF_FUNCTIONS_

#include "traversal.h"
#include "symbol.h"

// compiler optimization, precursor to function inlining
struct FuncLeaves {
  FnSymbol* first;
  FuncLeaves* next;
};


class FindLeafFunctions : public Traversal {
  bool _inside_leaf_func;
  void addLeaf(FnSymbol* fsym);
  FuncLeaves* _leaf_functions;
public :
  virtual ~FindLeafFunctions();
  void preProcessExpr(Expr* expr);
  void postProcessExpr(Expr* expr);
  bool isLeafFunction(FnSymbol* fs);
  void printLeaves(void);
};

#endif
