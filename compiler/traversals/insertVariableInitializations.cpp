#include <typeinfo>
#include "insertVariableInitializations.h"
#include "symtab.h"
#include "symscope.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"


InsertVariableInitializations::InsertVariableInitializations() {
  processInternalModules = false;
}


void InsertVariableInitializations::postProcessStmt(Stmt* stmt) {
  VarDefStmt* def_stmt = dynamic_cast<VarDefStmt*>(stmt);

  if (!def_stmt) {
    return;
  }

  VarSymbol* var = def_stmt->var;

  while (var && !var->isNull()) {
    if (var->varClass != VAR_CONFIG &&
	typeid(*var->type) != typeid(DomainType) &&
	typeid(*var->type) != typeid(ArrayType)) {
      /***
       *** Insert default initialization
       ***/
      if (var->type->defaultVal && !var->type->defaultVal->isNull()) {
	AssignOp* assign_expr = new AssignOp(GETS_NORM,
					     new Variable(var),
					     var->type->defaultVal->copy());
	ExprStmt* assign_stmt = new ExprStmt(assign_expr);
	if (!var->parentScope->stmtContext->isNull()) {
	  if (BlockStmt* block_stmt =
	      dynamic_cast<BlockStmt*>(var->parentScope->stmtContext)) {
	    block_stmt->body->insertBefore(assign_stmt);
	  }
	  else if (ModuleDefStmt* mod_stmt =
		   dynamic_cast<ModuleDefStmt*>(var->parentScope->stmtContext)) {
	    if (BlockStmt* block_stmt =
		dynamic_cast<BlockStmt*>(mod_stmt->module->initFn->body)) {
	      block_stmt->body->insertBefore(assign_stmt);
	    }
	    else {
	      INT_FATAL(var, "SJD \"Where should I put the default init?\"");
	    }
	  }
	  else if (dynamic_cast<TypeDefStmt*>(var->parentScope->stmtContext)) {
	    // Ignore case
	  }
	  else {
	    INT_FATAL(var, "SJD \"Where should I put the default init?\"");
	  }
	}
	else {
	  INT_FATAL(var, "SJD \"Where should I put the default init?\"");
	}
      }
      /***
       *** Insert user initialization
       ***/
      if (var->init && !var->init->isNull()) {
	AssignOp* assign_expr = new AssignOp(GETS_NORM,
					     new Variable(var),
					     var->init->copy());
	ExprStmt* assign_stmt = new ExprStmt(assign_expr);
	stmt->insertBefore(assign_stmt);
      }
    }
    var = nextLink(VarSymbol, var);
  }
}
