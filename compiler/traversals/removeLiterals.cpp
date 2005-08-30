#include "removeLiterals.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"

void insertSymbolForLiteral(Expr* prim) {
  if (dynamic_cast<Literal*>(prim)) {
    if (Symbol* sym = Symboltable::lookupInScope(((Literal*)prim)->str, commonModule->modScope)) {
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym)) {
        SymExpr* new_var = new SymExpr(var);
        prim->replace(new_var);
        prim = new_var;
        return;
      } else { 
        INT_FATAL(prim, "This should not happen.\n");
      }
      return;
    }
 
    VarSymbol* var_sym = new VarSymbol(//glomstrings(2, "_tmp_lit_", intstring(uid++)),
                                                 ((Literal*)prim)->str,
                                                 //glomstrings(3, intstring(uid++), "_", prim->str),
                                                 prim->typeInfo());
    DefExpr* defExpr = new DefExpr(var_sym);
    var_sym->noDefaultInit = true;
    Stmt* initStmt = new ExprStmt(defExpr);
    commonModule->initFn->body->body->insertAtHead(initStmt);
    VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym);
    SymExpr* new_var = new SymExpr(var);
    prim->replace(new_var);
    prim = new_var;
  }
}


void RemoveLiterals::postProcessExpr(Expr* expr) {
  if (dynamic_cast<Literal*>(expr)){
    insertSymbolForLiteral(expr);
  }
}

