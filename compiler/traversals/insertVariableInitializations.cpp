#include <typeinfo>
#include "insertVariableInitializations.h"
#include "symtab.h"
#include "symscope.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"


static void insert_default_init_stmt(VarSymbol* var, Stmt* init_stmt) {
  if (var->parentScope->stmtContext) {
    if (BlockStmt* block_stmt =
	dynamic_cast<BlockStmt*>(var->parentScope->stmtContext)) {
      block_stmt->body->insertBefore(init_stmt);
    }
    else if (ModuleDefStmt* mod_stmt =
	     dynamic_cast<ModuleDefStmt*>(var->parentScope->stmtContext)) {
      if (BlockStmt* block_stmt =
	  dynamic_cast<BlockStmt*>(mod_stmt->module->initFn->body)) {
	block_stmt->body->insertBefore(init_stmt);
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


static void insert_user_init_stmt(Stmt* stmt, Stmt* init_stmt) {
  stmt->insertBefore(init_stmt);
}


static void insert_array_init(Stmt* stmt, VarSymbol* var) {
  ArrayType* array_type = dynamic_cast<ArrayType*>(var->type);

  if (!array_type) {
    INT_FATAL(var, "Array type expected");
  }

  int rank = array_type->domainType->numdims;
  Expr* args = new IntLiteral(intstring(rank), rank);
  args->append(new Variable(var));
  args->append(array_type->domain->copy());
  args->append(new Variable(array_type->elementType->symbol));
  Symbol* init_array = Symboltable::lookupInternal("_INIT_ARRAY");
  FnCall* call = new FnCall(new Variable(init_array), args);
  ExprStmt* call_stmt = new ExprStmt(call);
  insert_user_init_stmt(stmt, call_stmt);
}


static void insert_domain_init(Stmt* stmt, VarSymbol* var) {
  DomainType* domain_type = dynamic_cast<DomainType*>(var->type);

  if (!domain_type) {
    INT_FATAL(var, "Domain type expected");
  }

  int rank = domain_type->numdims;

  SimpleSeqExpr* seq_init;

  if (ForallExpr* forall_init = dynamic_cast<ForallExpr*>(var->init)) {
    seq_init = dynamic_cast<SimpleSeqExpr*>(forall_init->domains);
  }
  else {
    seq_init = dynamic_cast<SimpleSeqExpr*>(var->init);
  }

  if (!seq_init) {
    INT_FATAL(var, "Invalid domain initialization expression");
  }

  for (int i = 0; i < rank; i++) {
    Expr* args = new Variable(var);
    args->append(new IntLiteral(intstring(i), i));
    args->append(seq_init->lo->copy());
    args->append(seq_init->hi->copy());
    args->append(seq_init->str->copy());
    Symbol* init_domain_dim = Symboltable::lookupInternal("_INIT_DOMAIN_DIM");
    FnCall* call = new FnCall(new Variable(init_domain_dim), args);
    ExprStmt* call_stmt = new ExprStmt(call);
    insert_user_init_stmt(stmt, call_stmt);
    seq_init = nextLink(SimpleSeqExpr, seq_init);
  }
}


static void insert_config_init(Stmt* stmt, VarSymbol* var) {

  // SJD: Note I want this to be a single macro INIT_CONFIG with the
  // default expression at the end and the if statement tucked away,
  // but because complex literals can't be passed, can't do this.
  // Need a traversal to change complex literals into complex variable
  // temporaries for function calls.

  // Idea: rewrite ProcessParameters and InsertParameterTemps using
  // Let expressions.  These should be useful so we might want to
  // implement it now.

  Expr* init_expr = var->init ? var->init : var->type->defaultVal;
  Expr* args = new Variable(var);
  args->append(new Variable(var->type->symbol));
  args->append(new StringLiteral(copystring(var->name)));
  args->append(new StringLiteral(var->parentScope->symContext->name));
  //  args->append(init_expr->copy());
  Symbol* init_config = Symboltable::lookupInternal("_INIT_CONFIG");
  FnCall* call = new FnCall(new Variable(init_config), args);
  AssignOp* assign = new AssignOp(GETS_NORM, new Variable(var), init_expr->copy());
  ExprStmt* assign_stmt = new ExprStmt(assign);
  CondStmt* cond_stmt = new CondStmt(call, assign_stmt);
  insert_user_init_stmt(stmt, cond_stmt);
}


static void insert_default_init(Stmt* stmt, VarSymbol* var) {
  if (var->type->defaultVal) {
    AssignOp* assign_expr = new AssignOp(GETS_NORM,
					 new Variable(var),
					 var->type->defaultVal->copy());
    ExprStmt* assign_stmt = new ExprStmt(assign_expr);
    insert_default_init_stmt(var, assign_stmt);
  }
}


static void insert_user_init(Stmt* stmt, VarSymbol* var) {
  if (var->init) {
    AssignOp* assign_expr = new AssignOp(GETS_NORM,
					 new Variable(var),
					 var->init->copy());
    ExprStmt* assign_stmt = new ExprStmt(assign_expr);
    stmt->insertBefore(assign_stmt);
  }
}


InsertVariableInitializations::InsertVariableInitializations() {
  processInternalModules = false;
}


void InsertVariableInitializations::postProcessStmt(Stmt* stmt) {
  VarDefStmt* def_stmt = dynamic_cast<VarDefStmt*>(stmt);

  if (!def_stmt) {
    return;
  }

  if (dynamic_cast<TypeSymbol*>(def_stmt->parentSymbol)) {
    return;
  }

  VarSymbol* var = def_stmt->var;

  while (var) {
    if (dynamic_cast<ArrayType*>(var->type)) {
      insert_array_init(stmt, var);
    }
    else if (dynamic_cast<DomainType*>(var->type)) {
      insert_domain_init(stmt, var);
    }
    else if (var->varClass == VAR_CONFIG) {
      insert_default_init(stmt, var);
      insert_config_init(stmt, var);
    }
    else {
      insert_default_init(stmt, var);
      insert_user_init(stmt, var);
    }
    var = nextLink(VarSymbol, var);
  }

  def_stmt->extract();
}
