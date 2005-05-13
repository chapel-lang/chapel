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
    Stmt* body = fnSym->body;
    BlockStmt* blockBody = dynamic_cast<BlockStmt*>(body);
    if (blockBody == NULL) {
      INT_FATAL(fnSym, "Function symbol doesn't have blockstmt body");
    }
    
    SymScope* fnScope = blockBody->blkScope;
    if (fnScope == NULL) {
      INT_FATAL(blockBody, "Block body has NULL blkScope");
    }
    SymScope* prevScope = Symboltable::setCurrentScope(fnScope);

    Symbol* retvalSym = Symboltable::lookupInCurrentScope("_retval");
    VarSymbol* retval = dynamic_cast<VarSymbol*>(retvalSym);
    varType var_type = VAR_NORMAL;
    if (is_Value_Type(retType) && stmt->parentFunction()->_getter)
      var_type = VAR_REF;
    if (retval == NULL) {
      DefStmt* retValDefStmt = Symboltable::defineSingleVarDefStmt("_retval", 
                                                                   retType, 
                                                                   NULL, 
                                                                   var_type,
                                                                   VAR_VAR);
      retval = retValDefStmt->varDef();
      retval->noDefaultInit = true;
      blockBody->body->insertBefore(retValDefStmt);
    } else {
      if (alreadyProcessedThisReturn(retExpr, retval)) {
        Symboltable::setCurrentScope(prevScope);
        return;
      }
    }

    Variable* retVar = new Variable(retval);
    AssignOp* assignRetVar = new AssignOp(GETS_NORM, retVar, retExpr->copy());
    ExprStmt* assignStmt = new ExprStmt(assignRetVar);
    BlockStmt *retBlock = new BlockStmt(assignStmt);
    retStmt->replace(retBlock);
    Variable* newRetExpr = new Variable(retval);
    ReturnStmt *newRetStmt = new ReturnStmt(newRetExpr);
    assignStmt->insertAfter(newRetStmt);
    
    Symboltable::setCurrentScope(prevScope);
  }
}



