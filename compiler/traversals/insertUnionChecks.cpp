#include <typeinfo>
#include "insertUnionChecks.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"


void InsertUnionChecks::preProcessExpr(Expr* expr) {
  if (MemberAccess* union_expr = dynamic_cast<MemberAccess*>(expr)) {
    if (ClassType* union_type =
	dynamic_cast<ClassType*>(union_expr->base->typeInfo())) {
      if (union_type->union_value) {
	if (expr->isWritten()) {
	  Expr* args = union_expr->base->copy();
	  char* id_tag = glomstrings(4, "_", 
				     union_expr->base->typeInfo()->name->name,
				     "_union_id_",
				     union_expr->member->name);
	  args->append(new Variable(Symboltable::lookup(id_tag)));
	  FnCall* set_function = 
	    new FnCall(new Variable(Symboltable::lookupInternal("_UNION_SET")), args);
	  ExprStmt* set_stmt = new ExprStmt(set_function);
	  expr->stmt->insertAfter(set_stmt);
	}
	if (expr->isRead()) {
	  Expr* args = union_expr->base->copy();
	  char* id_tag = glomstrings(4, "_", 
				     union_expr->base->typeInfo()->name->name,
				     "_union_id_",
				     union_expr->member->name);
	  args->append(new Variable(Symboltable::lookup(id_tag)));
	  args->append(new StringLiteral(expr->filename));
	  args->append(new IntLiteral(intstring(expr->lineno), expr->lineno));
	  FnCall* check_function = 
	    new FnCall(new Variable(Symboltable::lookupInternal("_UNION_CHECK")), args);
	  ExprStmt* check_stmt = new ExprStmt(check_function);
	  expr->stmt->insertBefore(check_stmt);
	}
      }
    }
  }
}
