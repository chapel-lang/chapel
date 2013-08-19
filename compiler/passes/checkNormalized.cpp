// checkNormalized.cpp

#include "passes.h"

#include "expr.h"
#include "astutil.h"

void
checkNormalized(void) {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ITERATOR_FN)) {
      if (fn->retTag == RET_TYPE)
        USR_FATAL_CONT(fn, "iterators may not yield or return types");
      if (fn->retTag == RET_PARAM)
        USR_FATAL_CONT(fn, "iterators may not yield or return parameters");
    } else if (fn->hasFlag(FLAG_CONSTRUCTOR) &&
               !fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)) {
      if (fn->retExprType)
        USR_FATAL_CONT(fn, "constructors may not declare a return type");
      for_formals(formal, fn) {
        Vec<SymExpr*> symExprs;
        collectSymExprs(formal, symExprs);
        forv_Vec(SymExpr, se, symExprs) {
          if (se->var == fn->_this) {
            USR_FATAL_CONT(se, "invalid access of class member in constructor header");
            break;
          }
        }
      }
    } else if (fn->hasFlag(FLAG_DESTRUCTOR)) {
      if (fn->retExprType)
        USR_FATAL_CONT(fn, "destructors may not declare a return type");
    }
  }
}


