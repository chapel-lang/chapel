#include "updateSymbols.h"
#include "expr.h"
#include "symbol.h"
#include "stmt.h"
#include "stringutil.h"


UpdateSymbols::UpdateSymbols(ASTMap* iMap) :
  map(iMap) { }


#define XSUB(_x, _t)                               \
  if (_x) {                                        \
    if (BaseAST *b = map->get(_x)) {         \
      if (_t* _y = dynamic_cast<_t*>(b)) {         \
        _x = _y;                                   \
      } else {                                     \
        INT_FATAL("Major error in UpdateSymbols"); \
      }                                            \
    }                                              \
  }


void UpdateSymbols::preProcessExpr(Expr* expr) {
  if (SymExpr* sym_expr = dynamic_cast<SymExpr*>(expr)) {
    XSUB(sym_expr->var, Symbol);
  } else if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    XSUB(defExpr->sym->type, Type);
  } else if (CastExpr* castExpr = dynamic_cast<CastExpr*>(expr)) {
    XSUB(castExpr->type, Type);
  } else if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(expr)) {
    XSUB(memberAccess->member, Symbol);
  }
}


void UpdateSymbols::preProcessStmt(Stmt* stmt) {
  if (GotoStmt* goto_stmt = dynamic_cast<GotoStmt*>(stmt))
    XSUB(goto_stmt->label, LabelSymbol);
}


void UpdateSymbols::preProcessSymbol(Symbol* sym) {
  if (!dynamic_cast<TypeSymbol*>(sym)) {
    XSUB(sym->type, Type);
  }
  if (FnSymbol* ps = dynamic_cast<FnSymbol*>(sym)) {
    XSUB(ps->retType, Type);
    XSUB(ps->_this, Symbol);
    XSUB(ps->_setter, Symbol);
    XSUB(ps->_getter, Symbol);
  } else if (ArgSymbol* ps = dynamic_cast<ArgSymbol*>(sym)) {
    if (ps->isGeneric && ps->genericSymbol) {
      BaseAST *b = map->get(ps->genericSymbol);
      if (b) {
        if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(b)) {
          if (ts->definition->astType != TYPE_VARIABLE)
            ps->isGeneric = 0;
          ps->genericSymbol = ts;
        } else {
          INT_FATAL("Major error in UpdateSymbols");
        }
      }
    }
  }
}
