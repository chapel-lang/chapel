#include <typeinfo>
#include "insertUnionChecks.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"


InsertUnionChecks::InsertUnionChecks(void) {
  writing = 0;
}


void InsertUnionChecks::preProcessStmt(Stmt* &stmt) {

}


// writing == 0, reading not writing
// writing == 1, writing not reading
// writing == 2, writing and reading  (e.g. inout parameter)
void SetWriting(int OnOff, Expr* expr, int& writing) {
  if (expr->parent && !expr->parent->isNull()) {
    Expr* parent = expr->parent;
    if (AssignOp* assignment = dynamic_cast<AssignOp*>(parent)) {
      if (assignment->left == expr) {
	writing = OnOff;
      }
    }
    if (FnCall* fn_call = dynamic_cast<FnCall*>(parent)) {
      if (typeid(*fn_call) == typeid(FnCall)) {
	FnSymbol* fn = fn_call->findFnSymbol();
	Symbol* formal = fn->formals;
	for(Expr* actual = fn_call->argList;
	    actual && !actual->isNull();
	    actual = nextLink(Expr, actual)) {
	  if (actual == expr) {
	    if (ParamSymbol* formal_param = dynamic_cast<ParamSymbol*>(formal)) {
	      if (formal_param->intent == PARAM_OUT) {
		writing = OnOff;
	      }
	      else if (formal_param->intent == PARAM_INOUT) {
		writing = OnOff * 2;  /* read&write or reset */
	      }
	    }
	  }
	  formal = nextLink(Symbol, formal);
	}
      }
    }
  }
}


void InsertUnionChecks::preProcessExpr(Expr* &expr) {
  SetWriting(1, expr, writing);
  if (MemberAccess* union_expr = dynamic_cast<MemberAccess*>(expr)) {
    if (ClassType* union_type =
	dynamic_cast<ClassType*>(union_expr->base->typeInfo())) {
      if (union_type->union_value) {
	if (writing) {
	  Expr* args = union_expr->base->copy();
	  char* id_tag = glomstrings(4, "_", 
				     union_expr->base->typeInfo()->name->name,
				     "_union_id_",
				     union_expr->member->name);
	  args->append(new Variable(new VarSymbol(id_tag)));
	  FnCall* set_function = 
	    new FnCall(new Variable(Symboltable::lookup("_UNION_SET")), args);
	  ExprStmt* set_stmt = new ExprStmt(set_function);
	  //	  expr->stmt->postinsert(set_stmt);
	  Stmt* copy_stmt = expr->stmt->copy();
	  copy_stmt->append(set_stmt);
	  Stmt::replace(expr->stmt, copy_stmt);
	}
	if (writing != 1) {
	  Expr* args = union_expr->base->copy();
	  char* id_tag = glomstrings(4, "_", 
				     union_expr->base->typeInfo()->name->name,
				     "_union_id_",
				     union_expr->member->name);
	  args->append(new Variable(new VarSymbol(id_tag)));
	  args->append(new StringLiteral(expr->filename));
	  args->append(new IntLiteral(intstring(expr->lineno), expr->lineno));
	  FnCall* check_function = 
	    new FnCall(new Variable(Symboltable::lookup("_UNION_CHECK")), args);
	  ExprStmt* check_stmt = new ExprStmt(check_function);

	  // SJD: trouble with preinsert, should preinsert/postinsert
	  // be implemented as follows instead

	  // expr->stmt->preinsert(check_stmt);
	  check_stmt->append(expr->stmt->copy());
	  Stmt::replace(expr->stmt, check_stmt);

	}
      }
    }
  }
}


void InsertUnionChecks::postProcessExpr(Expr* &expr) {
  SetWriting(0, expr, writing);
}
