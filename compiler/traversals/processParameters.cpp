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
      Expr* origActuals = fncall->argList->copyList();
      Expr* actual = fncall->argList;
      if (formal && actual) {

	/*
	fprintf(stderr, "Found a function call with parameters: %s\n", 
		fnSym->name);
	*/

	// BLC: pushes scope into wrong place if the current symboltable
	// scope is an expression scope?
	BlockStmt* blkStmt = Symboltable::startCompoundStmt(); 
	Stmt* body = nilStmt;
	bool tmpsRequired = false;

	/* generate copy-in statements */
	while (formal) {
	  tmpsRequired = true;

	  Symbol* newActual;
	  Expr* initializer;
	  if (formal->intent == PARAM_OUT) {
	    initializer = nilExpr;
	  } else {
	    initializer = actual->copy();
	  }
	  char* actualName = glomstrings(2, "_", formal->name);
	  VarDefStmt* newActualDecl = 
	    Symboltable::defineSingleVarDefStmt(actualName,
						formal->type, initializer,
						VAR_NORMAL, false, &newActual);
	  Variable* newActualUse = new Variable(newActual);
	  actual->replace(newActualUse);

	  body = appendLink(body, newActualDecl);
	
	  formal = nextLink(ParamSymbol, formal);
	  actual = nextLink(Expr, actual);
	}
	if (!tmpsRequired) {
	  return;
	}

	Stmt* origStmt = expr->stmt;
	Stmt* newStmt = origStmt->copy();
	body = appendLink(body, newStmt);

	/* generate copy out statements */
	formal = dynamic_cast<ParamSymbol*>(fnSym->formals);
	actual = origActuals;
	Expr* newActual = fncall->argList;
	if (formal && actual) {
	  while (formal) {
	    if (formal->requiresCPtr()) {
	      Expr* copyBack = new AssignOp(GETS_NORM, actual->copy(),
					    newActual->copy());
	      ExprStmt* copyBackStmt = new ExprStmt(copyBack);
	      body = appendLink(body, copyBackStmt);
	    }

	    formal = nextLink(ParamSymbol, formal);
	    actual = nextLink(Expr, actual);
	    newActual = nextLink(Expr, newActual);
	  }
	}
	

	blkStmt = Symboltable::finishCompoundStmt(blkStmt, body);

	origStmt->replace(blkStmt);
      }
    }
  }
}
