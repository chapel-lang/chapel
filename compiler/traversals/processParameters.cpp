#include <typeinfo>
#include "expr.h"
#include "processParameters.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


ProcessParameters::ProcessParameters(void) {
  whichModules = MODULES_CODEGEN;
}


static bool tmpRequired(ParamSymbol* formal, Expr* actual) {
  if (actual == NULL) { // if actual is elided, temp required
    return true;
  }
  if (Variable *var = dynamic_cast<Variable*>(actual)) {
    if (dynamic_cast<TypeSymbol*>(var->var))
      return false;
  }
  if (formal->type != dtUnknown) {
    return formal->requiresCTmp();
  } else {
    return actual->typeInfo()->requiresCParamTmp(formal->intent);
  }
}


void ProcessParameters::postProcessExpr(Expr* expr) {
  if (CallExpr* fncall = dynamic_cast<CallExpr*>(expr)) {
    if (fncall->opTag != OP_NONE) {
      return;
    }
    FnSymbol* fnSym = fncall->findFnSymbol();
    DefExpr* formal = fnSym->formals->first();
    AList<Expr>* actualList = fncall->argList;
    Expr* actual = actualList->first();
    AList<Expr>* newActuals = new AList<Expr>();
    if (formal && actual) {
      // BLC: pushes scope into wrong place if the current symboltable
      // scope is an expression scope?
      BlockStmt* blkStmt = Symboltable::startCompoundStmt(); 
      AList<Stmt>* body = new AList<Stmt>();
      bool tmpsRequired = false;
      
      /* generate copy-in statements */
      while (formal) {
        Expr* newActualUse = NULL;
        
        if (tmpRequired(dynamic_cast<ParamSymbol*>(formal->sym), actual)) {
          tmpsRequired = true;
          
          Expr* initializer;
          if (dynamic_cast<ParamSymbol*>(formal->sym)->intent == PARAM_OUT || (actual == NULL)) {
            if (formal->sym->defPoint->init) {
              initializer = formal->sym->defPoint->init->copy();
            } else {
              initializer = COPY(formal->sym->type->defaultValue);
            }
          } else {
            initializer = actual->copy();
          }
          char* newActualName = glomstrings(2, "_", formal->sym->name);
          DefExpr* newActualDecl = 
            Symboltable::defineSingleVarDef(newActualName,
                                            formal->sym->type, initializer,
                                            VAR_NORMAL, VAR_VAR);
          body->insertAtTail(new ExprStmt(newActualDecl));
          VarSymbol* var = dynamic_cast<VarSymbol*>(newActualDecl->sym);
          var->noDefaultInit = true;
          newActualUse = new Variable(var);
        } else {
          newActualUse = actual->copy();
        }
        newActuals->insertAtTail(newActualUse);
        
        formal = fnSym->formals->next();
        if (actual) {
          actual = actualList->next();
        }
      }
      if (!tmpsRequired) {
        return;
      }
      
      fncall->argList = newActuals;
      
      Stmt* origStmt = expr->getStmt();
      Stmt* newStmt = origStmt->copy();
      body->insertAtTail(newStmt);

      /* generate copy out statements */
      formal = fnSym->formals->first();
      actual = actualList->first();
      Expr* newActual = fncall->argList->first();
      if (formal && actual) {
        while (formal) {
          if (dynamic_cast<ParamSymbol*>(formal->sym)->requiresCopyBack() && actual) {
            Expr* copyBack = new CallExpr(OP_GETSNORM, actual->copy(),
                                          newActual->copy());
            ExprStmt* copyBackStmt = new ExprStmt(copyBack);
            body->insertAtTail(copyBackStmt);
          }
          
          formal = fnSym->formals->next();
          if (actual) {
            actual = actualList->next();
          }
          newActual = fncall->argList->next();
        }
      }
      

      blkStmt = Symboltable::finishCompoundStmt(blkStmt, body);
      
      origStmt->replace(blkStmt);
    }
  }
}
