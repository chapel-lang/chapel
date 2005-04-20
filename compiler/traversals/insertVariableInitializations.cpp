#include <typeinfo>
#include "insertVariableInitializations.h"
#include "insertElidedLoops.h"
#include "symtab.h"
#include "symscope.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"


#define INDEX_ANON_TUPLE_STOPGAP


static void insert_init(Stmt* stmt, VarSymbol* var, Type* type);


static void insert_array_init(Stmt* stmt, VarSymbol* var, Type* type) {
  ArrayType* array_type = dynamic_cast<ArrayType*>(type);

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
  stmt->insertBefore(call_stmt);

  ForallExpr* domain = dynamic_cast<ForallExpr*>(array_type->domain);
  Symbol* indices = dynamic_cast<DefExpr*>(domain->indices)->sym;
  
  // Here we create some indices (assuming arithmetic domain)
  // This should eventually use the IndexType when that is in. --SJD
  if (!indices) {
    DomainType* domain_type = dynamic_cast<DomainType*>(domain->typeInfo());
    if (!domain_type) {
      INT_FATAL(domain, "Domain has no type");
    }
    for (int i = 0; i < domain_type->numdims; i++) {
      char* name = glomstrings(2, "_ind_", intstring(i));
      indices = appendLink(indices, new Symbol(SYMBOL, name));
    }
  }

  ForLoopStmt* loop =
    Symboltable::startForLoop(true, indices, array_type->domain->copy());
  NoOpStmt* noop_stmt = new NoOpStmt();
  BlockStmt* block_stmt = new BlockStmt(noop_stmt);
  loop = Symboltable::finishForLoop(loop, block_stmt);
  stmt->insertBefore(loop);
  insert_init(noop_stmt, var, array_type->elementType);
  Symbol* indices_change = dynamic_cast<DefExpr*>(loop->indices)->sym;
  TRAVERSE(loop->body, new InsertElidedIndices(indices_change), true);
}


static void insert_domain_init(Stmt* stmt, VarSymbol* var, Type* type) {
  DomainType* domain_type = dynamic_cast<DomainType*>(type);

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
    stmt->insertBefore(call_stmt);
    seq_init = nextLink(SimpleSeqExpr, seq_init);
  }
}


static Stmt* basic_default_init_stmt(Stmt* stmt, VarSymbol* var, Type* type) {
  Stmt* init_stmt = NULL;
  if (type->defaultVal) {
    Expr* lhs = new Variable(var);
    Expr* rhs = type->defaultVal->copy();
    Expr* init_expr = new AssignOp(GETS_NORM, lhs, rhs);
    init_stmt = new ExprStmt(init_expr);
  } else if (type->defaultConstructor) {
    if (var->noDefaultInit) {
      return new NoOpStmt();
    }
    Expr* lhs = new Variable(var);
    Expr* constructor_variable = new Variable(type->defaultConstructor);
    Expr* rhs = new FnCall(constructor_variable, NULL);
    Expr* init_expr = new AssignOp(GETS_NORM, lhs, rhs);
    init_stmt = new ExprStmt(init_expr);
  } else {
    if (dynamic_cast<IndexType*>(type)) {
      return new NoOpStmt();
    }
    INT_FATAL(var, "Failed to insert default initialization");
  }
  return init_stmt;
}


static void insert_config_init(Stmt* stmt, VarSymbol* var, Type* type) {

  // SJD: Note I want this to be a single macro INIT_CONFIG with the
  // default expression at the end and the if statement tucked away,
  // but because complex literals can't be passed, can't do this.
  // Need a traversal to change complex literals into complex variable
  // temporaries for function calls.

  Expr* init_expr = var->init ? var->init : type->defaultVal;
  Expr* args = new Variable(var);
  args->append(new Variable(type->symbol));
  args->append(new StringLiteral(copystring(var->name)));
  args->append(new StringLiteral(var->parentScope->symContext->name));
  Symbol* init_config = Symboltable::lookupInternal("_INIT_CONFIG");
  FnCall* call = new FnCall(new Variable(init_config), args);
  Expr* assign = new AssignOp(GETS_NORM, new Variable(var), init_expr->copy());
  ExprStmt* assign_stmt = new ExprStmt(assign);
  CondStmt* cond_stmt = new CondStmt(call, assign_stmt);
  stmt->insertBefore(cond_stmt);
}


static void insert_basic_init(Stmt* stmt, VarSymbol* var, Type* type) {
  Stmt* init_stmt = basic_default_init_stmt(stmt, var, type);

  if (var->init) {
    Expr* lhs = new Variable(var);
    Expr* rhs = var->init->copy();
    Expr* init_expr = new AssignOp(GETS_NORM, lhs, rhs);
    init_stmt->append(new ExprStmt(init_expr));
  }

  stmt->insertBefore(init_stmt);
}


static void insert_init(Stmt* stmt, VarSymbol* var, Type* type) {
  if (dynamic_cast<ArrayType*>(type)) {
    insert_array_init(stmt, var, type);
  } else if (dynamic_cast<DomainType*>(type)) {
    insert_domain_init(stmt, var, type);
  } else if (var->varClass == VAR_CONFIG) {
    insert_config_init(stmt, var, type);
  } else {
    insert_basic_init(stmt, var, type);
  }
}


InsertVariableInitializations::InsertVariableInitializations() {
  whichModules = MODULES_COMMON_AND_USER;
}


void InsertVariableInitializations::postProcessStmt(Stmt* stmt) {
  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {

    if (dynamic_cast<TypeSymbol*>(def_stmt->parentSymbol)) {
      return;
    }

    if (!def_stmt->varDef()) {
      return;
    }

    DefExpr* def_expr = def_stmt->defExprls;
    while (def_expr) {
      VarSymbol* var = dynamic_cast<VarSymbol*>(def_expr->sym);
      while (var) {
        insert_init(stmt, var, var->type);
        var = nextLink(VarSymbol, var);
      }
      def_expr = nextLink(DefExpr, def_expr);
    }
    
    def_stmt->extract();
  }
}
