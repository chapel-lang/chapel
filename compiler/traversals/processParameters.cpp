#include <typeinfo>
#include "expr.h"
#include "processParameters.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


ProcessParameters::ProcessParameters(void) {
  processInternalModules = false;
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
      ParamSymbol* formal = dynamic_cast<ParamSymbol*>(fnSym->formals);
      Expr* actualList = fncall->argList;
      Expr* actual = actualList;
      Expr* newActuals = NULL;
      if (formal && actual) {

	/*
	fprintf(stderr, "Found a function call with parameters: %s\n", 
		fnSym->name);
	*/

	// BLC: pushes scope into wrong place if the current symboltable
	// scope is an expression scope?
	BlockStmt* blkStmt = Symboltable::startCompoundStmt(); 
	Stmt* body = NULL;
	bool tmpsRequired = false;

	/* generate copy-in statements */
	while (formal) {
	  Expr* newActualUse = NULL;

	  bool actualElided = !actual;
	  if (formal->requiresCTmp() || actualElided) {
	    tmpsRequired = true;

	    Expr* initializer;
	    if (formal->intent == PARAM_OUT || actualElided) {
	      initializer = formal->init;
	    } else {
	      initializer = actual->copy();
	    }
	    char* newActualName = glomstrings(2, "_", formal->name);
	    DefStmt* newActualDecl = 
	      Symboltable::defineSingleVarDefStmt(newActualName,
						  formal->type, initializer,
						  VAR_NORMAL, VAR_VAR);
	    body = appendLink(body, newActualDecl);

	    newActualUse = new Variable(newActualDecl->varDef());
	  } else {
	    newActualUse = actual->copy();
	  }
	  newActuals = appendLink(newActuals, newActualUse);
	
	  formal = nextLink(ParamSymbol, formal);
	  if (!actualElided) {
	    actual = nextLink(Expr, actual);
	  }
	}
	if (!tmpsRequired) {
	  return;
	}

	fncall->setArgs(newActuals);

	Stmt* origStmt = expr->stmt;
	Stmt* newStmt = origStmt->copy();
	body = appendLink(body, newStmt);

	/* generate copy out statements */
	formal = dynamic_cast<ParamSymbol*>(fnSym->formals);
	actual = actualList;
	Expr* newActual = fncall->argList;
	if (formal && actual) {
	  while (formal) {
	    if (formal->requiresCopyBack() && actual) {
	      Expr* copyBack = new AssignOp(GETS_NORM, actual->copy(),
					    newActual->copy());
	      ExprStmt* copyBackStmt = new ExprStmt(copyBack);
	      body = appendLink(body, copyBackStmt);
	    }

	    formal = nextLink(ParamSymbol, formal);
	    if (actual) {
	      actual = nextLink(Expr, actual);
	    }
	    newActual = nextLink(Expr, newActual);
	  }
	}
	

	blkStmt = Symboltable::finishCompoundStmt(blkStmt, body);

	origStmt->replace(blkStmt);
      }
    }
  }
}
