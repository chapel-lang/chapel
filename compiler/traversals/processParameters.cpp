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
  if (formal->type != dtUnknown) {
    return formal->requiresCTmp();
  } else {
    return actual->typeInfo()->requiresCParamTmp(formal->intent);
  }
}


void ProcessParameters::postProcessExpr(Expr* expr) {
  if (typeid(*expr) == typeid(ParenOpExpr)) {
    fprintf(stderr, "ProcessParameters found a parenOpExpr:\n  ");
    expr->print(stderr);
    fprintf(stderr, "\n");
    INT_FATAL(expr, "These need to be resolved before calling this pass\n");
  }
  if (typeid(*expr) == typeid(FnCall)) {
    FnCall* fncall = dynamic_cast<FnCall*>(expr);
    if (fncall) {
      FnSymbol* fnSym = fncall->findFnSymbol();
      ParamSymbol* formal = fnSym->formals->first();
      AList<Expr>* actualList = fncall->argList;
      Expr* actual = actualList->first();
      AList<Expr>* newActuals = new AList<Expr>();
      if (formal && actual) {

        /*
        fprintf(stderr, "Found a function call with parameters: %s (%s:%d)\n", 
                fnSym->name, fncall->filename, fncall->lineno);
        */

        // BLC: pushes scope into wrong place if the current symboltable
        // scope is an expression scope?
        BlockStmt* blkStmt = Symboltable::startCompoundStmt(); 
        AList<Stmt>* body = new AList<Stmt>();
        bool tmpsRequired = false;

        /* generate copy-in statements */
        while (formal) {
          Expr* newActualUse = NULL;

          if (tmpRequired(formal, actual)) {
            tmpsRequired = true;

            Expr* initializer;
            if (formal->intent == PARAM_OUT || (actual == NULL)) {
              if (formal->init) {
                initializer = formal->init->copy();
              } else {
                initializer = formal->type->defaultVal->copy();
              }
            } else {
              initializer = actual->copy();
            }
            char* newActualName = glomstrings(2, "_", formal->name);
            DefStmt* newActualDecl = 
              Symboltable::defineSingleVarDefStmt(newActualName,
                                                  formal->type, initializer,
                                                  VAR_NORMAL, VAR_VAR);
            body->insertAtTail(newActualDecl);
            newActualDecl->varDef()->noDefaultInit = true;

            newActualUse = new Variable(newActualDecl->varDef());
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

        fncall->setArgs(newActuals);

        Stmt* origStmt = expr->getStmt();
        Stmt* newStmt = origStmt->copy();
        body->insertAtTail(newStmt);

        /* generate copy out statements */
        formal = fnSym->formals->first();
        actual = actualList->first();
        Expr* newActual = fncall->argList->first();
        if (formal && actual) {
          while (formal) {
            if (formal->requiresCopyBack() && actual) {
              Expr* copyBack = new AssignOp(GETS_NORM, actual->copy(),
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
}
