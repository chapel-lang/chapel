#include "updateSymbols.h"
#include "expr.h"
#include "symbol.h"
#include "stringutil.h"

UpdateSymbols::UpdateSymbols(Map<BaseAST*,BaseAST*>* init_updateMap,
                             Map<BaseAST*,BaseAST*>* init_copyMap) {
  changed = false;
  updateMap = init_updateMap;
  copyMap = init_copyMap;
  /** Prune for symbols? **/ /** Performance reasons --SJD */
  /** If so, prune a local copy! **/ /** Also make it a hash */
  /** types too now **/
}


#define XSUB(_x, _t)                                  \
  if (_x) {                                           \
    BaseAST *b = updateMap->get(_x);                  \
    if (b) {                                          \
      if (_t new_sym = dynamic_cast<_t>(b)) {         \
        _x = new_sym;                                 \
        changed = true;                               \
      } else {                                        \
        INT_FATAL("Major error in UpdateSymbols");    \
      }                                               \
    }                                                 \
  }                                                   \
  

void UpdateSymbols::preProcessExpr(Expr* expr) {
  if (Variable* sym_expr = dynamic_cast<Variable*>(expr)) {
    Expr* newExpr = dynamic_cast<Expr*>(updateMap->get(sym_expr->var));
    if (newExpr) {
      sym_expr->replace(newExpr->copy(false, copyMap));
    } else {
      XSUB(sym_expr->var, Symbol*);
    }
  } else if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    XSUB(defExpr->sym->type, Type*);
  } else if (CastExpr* castExpr = dynamic_cast<CastExpr*>(expr)) {
    XSUB(castExpr->type, Type*);
  } else if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(expr)) {
    XSUB(memberAccess->member, Symbol*);
  }
}


void UpdateSymbols::preProcessSymbol(Symbol* sym) {
  if (!dynamic_cast<TypeSymbol*>(sym)) {
    XSUB(sym->type, Type*);
  }
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    XSUB(fn->retType, Type*);
    XSUB(fn->_this, Symbol*);
    XSUB(fn->_setter, Symbol*);
    XSUB(fn->_getter, Symbol*);
  }
  if (ParamSymbol* p = dynamic_cast<ParamSymbol*>(sym)) {
    if (p->isGeneric && p->typeVariable) {
      BaseAST *b = updateMap->get(p->typeVariable);
      if (b) {
        if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(b)) {
          if (ts->type->astType != TYPE_VARIABLE)
            p->isGeneric = 0;
          p->typeVariable = ts;
        } else {
          INT_FATAL("Major error in UpdateSymbols");
        }
      }
    }
  }
}
