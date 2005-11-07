#include <typeinfo>
#include "eliminateReturns.h"
#include "stmt.h"
#include "symtab.h"
#include "stringutil.h"

EliminateReturns::EliminateReturns(void) {
  whichModules = MODULES_CODEGEN;
  uid = 1;
  label_ret_map = new Map<Symbol*,LabelSymbol*>();
}


static bool alreadyProcessedThisReturn(Expr* retExpr, Symbol* retval) {
  if (SymExpr* retVar = dynamic_cast<SymExpr*>(retExpr)) {
    if (retVar->var == retval) {
      return true;
    }
  }
  return false;
}


void EliminateReturns::preProcessStmt(Stmt* stmt) {

  if (ReturnStmt* retStmt = dynamic_cast<ReturnStmt*>(stmt)) {
    Expr* retExpr = retStmt->expr;

     //    fprintf(stderr, "\n\n\nFound a return statement: ");
    //    retStmt->println(stderr);
    FnSymbol* fnSym = retStmt->parentFunction();

    if (fnSym->fnClass == FN_CONSTRUCTOR)
      return;
       
    if (retExpr == NULL) {
      //replace all returns except the last one in a function with a goto
      if (!isLastStmtInFunc(retStmt, fnSym->body)) {
        replaceRetWithGoto(retStmt, fnSym);
      }
      return;
    }

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
    //Symbol* retvalSym = Symboltable::lookupInCurrentScope("_retval");
    Symbol* retvalSym = Symboltable::lookupInScope("_retval", fnScope);
    VarSymbol* retval = dynamic_cast<VarSymbol*>(retvalSym);
    if (retval == NULL) {
      retval = new VarSymbol("_retval", retType);
      retval->cname = stringcat(retval->name, "_", intstring(uid++));
      retval->noDefaultInit = true;
      DefExpr* retDef = new DefExpr(retval);
      if (fnSym->defPoint->exprType)
        retDef->exprType = fnSym->defPoint->exprType->copy();
      body->insertAtHead(new ExprStmt(retDef));
    } else {
      if (alreadyProcessedThisReturn(retExpr, retval)) {
        //replace all returns except the last one in a function with a goto
        if (!isLastStmtInFunc(retStmt, fnSym->body))
          replaceRetWithGoto(retStmt, fnSym);
        return;
      }
    }

    SymExpr* retVar = new SymExpr(retval);
    CallExpr* assignRetVar = new CallExpr(OP_GETSNORM, retVar, retExpr->copy());
    ExprStmt* assignStmt = new ExprStmt(assignRetVar);
    BlockStmt *retBlock = new BlockStmt(assignStmt);
    retStmt->replace(retBlock);
    SymExpr* newRetExpr = new SymExpr(retval);
    ReturnStmt *newRetStmt = new ReturnStmt(newRetExpr);
    assignStmt->insertAfter(newRetStmt);
    
    //replace all returns except the last one in a function with a goto
    if (!isLastStmtInFunc(newRetStmt, fnSym->body)) {
      replaceRetWithGoto(newRetStmt, fnSym);
    }
      
  }
}

bool EliminateReturns::isLastStmtInFunc(ReturnStmt* ret_stmt, BlockStmt* block) {
  Stmt* last_stmt = block->body->last();
  if (BlockStmt* block_stmt = dynamic_cast<BlockStmt*>(last_stmt))
    return isLastStmtInFunc(ret_stmt, block_stmt);
  else
    return (ret_stmt == last_stmt);
}

void EliminateReturns::replaceRetWithGoto(ReturnStmt* ret_stmt, FnSymbol* fn_sym) {
  LabelSymbol* label_sym = label_ret_map->get(fn_sym);
  GotoStmt* goto_stmt = new GotoStmt(goto_normal, label_sym);
  //already created label
  if (label_sym)
    //replace return goto   
    ret_stmt->replace(goto_stmt);
  //create label
  else {
    createLabelWithRetAtFuncEnd(ret_stmt, fn_sym);
    replaceRetWithGoto(ret_stmt, fn_sym);
  }
}

void EliminateReturns::createLabelWithRetAtFuncEnd(ReturnStmt* ret_stmt, FnSymbol* fn_sym) {
  LabelSymbol* label_sym = new LabelSymbol("_end_fn_label");
  label_sym->cname = stringcat(label_sym->name, "_", intstring(uid++));
  label_ret_map->put(fn_sym, label_sym);
  
  fn_sym->insertAtTail(new LabelStmt(new DefExpr(label_sym))); 
  fn_sym->insertAtTail(ret_stmt->copy());
}




