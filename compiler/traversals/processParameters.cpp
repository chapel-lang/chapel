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
      Expr* actual = fncall->argList;
      if (formal && actual) {
	/*
	fprintf(stderr, "Found a function call with parameters: %s\n", 
		fnSym->name);
	*/
	// BLC: pushes scope into wrong place
	BlockStmt* blkStmt = Symboltable::startCompoundStmt(); 
	Stmt* body = nilStmt;
	Expr* newActuals = nilExpr;
	bool tmpsRequired = false;
	while (formal) {
	  if (formal->requiresCTmp()) {
	    tmpsRequired = true;
	    Symbol* newActual = new Symbol(SYMBOL, 
					   glomstrings(2, "_", formal->name));
	    Expr* initializer;
	    if (formal->intent == PARAM_OUT) {
	      initializer = nilExpr;
	    } else {
	      initializer = actual->copy();
	    }
	    VarDefStmt* newActualDecl = 
	      Symboltable::defineVarDefStmt(newActual,
					    formal->type,
					    initializer,
					    VAR_NORMAL,
					    false);
	    Variable* newActualUse = new Variable(newActual);

	    body = appendLink(body, newActualDecl);
	    newActuals = appendLink(newActuals, newActualUse);
	  } else {
	    newActuals = appendLink(newActuals, actual->copy());
	  }
	
	  formal = nextLink(ParamSymbol, formal);
	  actual = nextLink(Expr, actual);
	}
	if (!tmpsRequired) {
	  return;
	}

	FnCall* newFnCall = new FnCall(fncall->baseExpr, newActuals);

	/* This is what we'd like to do (along with the code below
	expr->replace(newFnCall);
	Stmt* origStmt = expr->stmt;
	Stmt* newStmt = origStmt->copy();
	body = appendLink(body, newStmt);
	*/

	/* so instead we do this */
	Stmt* origStmt = expr->stmt;
	Stmt* prevStmt = prevLink(Stmt, origStmt);
	Stmt* nextStmt = nextLink(Stmt, origStmt);
	origStmt->extract();
	expr->replace(newFnCall);
	body = appendLink(body, origStmt);
	
	formal = dynamic_cast<ParamSymbol*>(fnSym->formals);
	actual = fncall->argList;
	Expr* newActual = newActuals;
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

	/* we'd like to do this, but Stmt::replace doesn't currently know
	   how to replace all the pointers, just the first argument
	Stmt::replace(origStmt, blkStmt);
	*/

	/* so instead we do this */
	if (nextStmt && !nextStmt->isNull()) {
	  nextStmt->insertBefore(blkStmt);
	} else if (prevStmt && !prevStmt->isNull()) {
	  prevStmt->insertAfter(blkStmt);
	} else {
	  INT_FATAL("Brad really needs to implement statement lists as "
		    "a template class");
	}

	/*
	expr->stmt->insertBefore(blkStmt);
	Stmt* newFnCallStmt = expr->stmt;
	newFnCallStmt->extract();
	Expr::replace(expr, newFnCall);
	
	//	Stmt* newFnCallStmt = new ExprStmt(newFnCall);
	body = appendLink(body, newFnCallStmt);
	Symboltable::finishCompoundStmt(blkStmt, body);
	//	expr->stmt->extract();
	*/
      }
      /*
      if (!fncall->argList->isNull()) {
	//	fncall->baseExpr->print(stderr);
	//	fprintf(stderr, "\n\n");
      }
      */
    }
  }
}
