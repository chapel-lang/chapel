#include <typeinfo>
#include "eliminateReturns.h"
#include "stmt.h"
#include "symtab.h"

EliminateReturns::EliminateReturns(void) {
  whichModules = MODULES_CODEGEN;
}


static bool alreadyProcessedThisReturn(Expr* retExpr, Symbol* retval) {
  if (typeid(*retExpr) == typeid(Variable)) {
    Variable* retVar = dynamic_cast<Variable*>(retExpr);
    if (retVar->var == retval) {
      return true;
    }
  }
  return false;
}


void EliminateReturns::preProcessStmt(Stmt* stmt) {
  if (typeid(*stmt) == typeid(ReturnStmt)) {
    ReturnStmt* retStmt = dynamic_cast<ReturnStmt*>(stmt);
    Expr* retExpr = retStmt->expr;

    if (retExpr == NULL) {
      return;
    }

    //    fprintf(stderr, "\n\n\nFound a return statement: ");
    //    retStmt->println(stderr);

    FnSymbol* fnSym = retStmt->parentFunction();
    if (fnSym == NULL) {
      INT_FATAL(stmt, "Return statement doesn't have parent function");
    }

    //    fprintf(stderr, "Parent function is: %s\n", fnSym->name);
    Type* retType = fnSym->retType;
    //    fprintf(stderr, "Return type is: ");
    //    retType->println(stderr);
    BlockStmt* body = fnSym->body;
    
    SymScope* fnScope = body->blkScope;
    if (fnScope == NULL) {
      INT_FATAL(body, "Block body has NULL blkScope");
    }
    SymScope* prevScope = Symboltable::setCurrentScope(fnScope);

    Symbol* retvalSym = Symboltable::lookupInCurrentScope("_retval");
    VarSymbol* retval = dynamic_cast<VarSymbol*>(retvalSym);
    if (retval == NULL) {
      DefExpr* retValDef = Symboltable::defineSingleVarDef("_retval", 
                                                           retType, 
                                                           NULL, 
                                                           VAR_NORMAL,
                                                           VAR_VAR);
      retval = dynamic_cast<VarSymbol*>(retValDef->sym);
      retval->noDefaultInit = true;
      body->body->insertAtHead(new ExprStmt(retValDef));
    } else {
      if (alreadyProcessedThisReturn(retExpr, retval)) {
        Symboltable::setCurrentScope(prevScope);
        return;
      }
    }

    Variable* retVar = new Variable(retval);
    ParenOpExpr* assignRetVar = new ParenOpExpr(OP_GETSNORM, retVar, retExpr->copy());
    ExprStmt* assignStmt = new ExprStmt(assignRetVar);
    BlockStmt *retBlock = new BlockStmt(new AList<Stmt>(assignStmt));
    retStmt->replace(retBlock);
    Variable* newRetExpr = new Variable(retval);
    ReturnStmt *newRetStmt = new ReturnStmt(newRetExpr);
    assignStmt->insertAfter(newRetStmt);
    
    Symboltable::setCurrentScope(prevScope);
  }
}



