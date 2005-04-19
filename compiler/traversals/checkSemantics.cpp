#include "checkSemantics.h"
#include "expr.h"

static void checkFnCall(FnCall* fncall) {
  
}


static void checkAssignOp(AssignOp* assign) {
  if (assign->left->isConst()) {
    USR_FATAL(assign, "Assigning to a constant expression");
  }
}

//Roxana: added support for legal parameter initializers
static void checkVar(Variable* var){
  if (var->isParam()){
    //printf("Parameter expression.\n");
    VarSymbol* vs = dynamic_cast<VarSymbol*>(var->var);
    if (vs){
      //printf("Var symbol: %s.\n", vs->name);
      Expr* init = vs->init;
      if (!init->isComputable()){
        //printf("Expression is computable.\n");
        USR_FATAL(init, "Initializing parameter to a variable expression.");
      }
    }
  }
}


void CheckSemantics::preProcessExpr(Expr* expr) {
  FnCall* fncall = dynamic_cast<FnCall*>(expr);
  if (fncall) checkFnCall(fncall);

  AssignOp* assign = dynamic_cast<AssignOp*>(expr);
  if (assign){
    //printf("Assignment expression.\n"); 
    checkAssignOp(assign);
  }
  Variable* var = dynamic_cast<Variable*>(expr);
  if (var) {
    //printf("Variable.\n");
    checkVar(var);
  }
}

