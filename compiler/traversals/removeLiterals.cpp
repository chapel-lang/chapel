#include "removeLiterals.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"

void insertSymbolForLiteral(Expr* prim) {
  // jp - This doesn't work if there are multiple string representations for identical
  //        constants e.g. 1.0 1. 1.0000 1.0e0
  //      The problem is that there should only be one Symbol for a given constant so that
  //        constants can be compared by Symbol equality
  //      Instead build a separate hash table and hash on the type and then the physical
  //        representation (binary bit pattern).  See if1.cpp - if1_const for details.
  //RED: while this makes sense, there are more immediate problems which I prefer
  //to sort out before going into this; I'll do that once we are getting there;
  if (dynamic_cast<Literal*>(prim)){
    if (Symbol* sym = Symboltable::lookupInScope(((Literal*)prim)->str, commonModule->modScope)) {
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym)) {
        Variable* new_var = new Variable(var);
        //prim->replace(new Variable(var));
        prim->replace(new_var);
        prim = new_var;
        return;
      }
      else INT_FATAL(prim, "This should not happen\n");
      return;
    }
  
    VarSymbol* var = new VarSymbol(//glomstrings(2, "_tmp_lit_", intstring(uid++)),
                                                 ((Literal*)prim)->str,
                                                 //glomstrings(3, intstring(uid++), "_", prim->str),
                                                 prim->typeInfo());
    //RED: this is problematic; it means that the literal expressions will not fully disappear before
    //analysis, which leads to problems in analysis -- see gen_if1 in analysis.cpp
    DefExpr* defExpr = new DefExpr(var, prim->copy());

    Stmt* initStmt = new ExprStmt(defExpr);
    commonModule->initFn->body->body->insertAtHead(initStmt);
    VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym);
    Variable* new_var = new Variable(var);
    //prim->replace(new Variable(var));
    prim->replace(new_var);
    prim = new_var;
  }
}


void RemoveLiterals::postProcessExpr(Expr* expr) {
  if (dynamic_cast<Literal*>(expr)){
    insertSymbolForLiteral(expr);
  }
}

