#include <typeinfo>
#include "expr.h"
#include "processParameters.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


ProcessParameters::ProcessParameters(void) {
  processInternalModules = false;
}


void ProcessParameters::postProcessExpr(Expr* &expr) {
  if (typeid(*expr) == typeid(FnCall)) {
    FnCall* fncall = dynamic_cast<FnCall*>(expr);
    if (fncall) {
      FnSymbol* fnSym = fncall->findFnSymbol();
      ParamSymbol* formal = dynamic_cast<ParamSymbol*>(fnSym->formals);
      Expr* actual = fncall->argList;
      if (formal && actual) {
	// BLC: pushes scope into wrong place
	BlockStmt* blkStmt = Symboltable::startCompoundStmt(); 
	Stmt* body = nilStmt;
	Expr* newActuals = nilExpr;
	while (formal) {
	  Symbol* newActual = new Symbol(SYMBOL, 
					 glomstrings(2, "_", formal->name));
	  VarDefStmt* newActualDecl = 
	    Symboltable::defineVarDefStmt(newActual,
					  formal->type,
					  actual,
					  VAR_NORMAL,
					  false);
	  Variable* newActualUse = new Variable(newActual);

	  body = appendLink(body, newActualDecl);
	  newActuals = appendLink(newActuals, newActualUse);
	
	  formal = nextLink(ParamSymbol, formal);
	  actual = nextLink(Expr, actual);
	}
	FnCall* newFnCall = new FnCall(fncall->baseExpr, newActuals);
	expr->stmt->preinsert(blkStmt);
	Stmt* newFnCallStmt = expr->stmt;
	newFnCallStmt->extract();
	Expr::replace(expr, newFnCall);
	
	//	Stmt* newFnCallStmt = new ExprStmt(newFnCall);
	body = appendLink(body, newFnCallStmt);
	Symboltable::finishCompoundStmt(blkStmt, body);
	//	expr->stmt->extract();
      }
      /*
      if (!fncall->argList->isNull()) {
	fprintf(stderr, "Found a function call with parameters: %s\n", 
		fnSym->name);
	//	fncall->baseExpr->print(stderr);
	//	fprintf(stderr, "\n\n");
      }
      */
    }
  }
}
