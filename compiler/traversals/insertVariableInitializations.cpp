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
  AList<Expr>* args = new AList<Expr>(new IntLiteral(intstring(rank), rank));
  args->insertAtTail(new Variable(var));
  args->insertAtTail(array_type->domain->copy());
  args->insertAtTail(new Variable(array_type->elementType->symbol));
  Symbol* init_array = Symboltable::lookupInternal("_INIT_ARRAY");
  FnCall* call = new FnCall(new Variable(init_array), args);
  ExprStmt* call_stmt = new ExprStmt(call);
  stmt->insertBefore(call_stmt);

  ForallExpr* domain = dynamic_cast<ForallExpr*>(array_type->domain);
  
  IndexType* index_type = NULL;
  AList<Symbol>* indices = new AList<Symbol>();

  //RED We create one index per (anonymous) domain assuming Index Type. 
  //However, this is not complete since index variables
  //in forall are not handled well enough yet.
  if (_dtinteger_IndexType_switch) {
    if (domain->indices->isEmpty()) {
      DomainType* domain_type = dynamic_cast<DomainType*>(domain->typeInfo());
      if (!domain_type) {
        INT_FATAL(domain, "Domain has no type");
      }
      index_type = domain_type->idxType;
      if (!index_type) {
        INT_FATAL(stmt, "Domain index has no type");
      }
      char* name = glomstrings(1, "_idx_init");
      indices->insertAtTail(new Symbol(SYMBOL, name));
    }  
  } else {
    // Here we create some indices (assuming arithmetic domain)
    // This should eventually use the IndexType when that is in. --SJD  
    if (domain->indices->isEmpty()) {
      DomainType* domain_type = dynamic_cast<DomainType*>(domain->typeInfo());
      if (!domain_type) {
        INT_FATAL(domain, "Domain has no type");
      }
      for (int i = 0; i < domain_type->numdims; i++) {
        char* name = glomstrings(2, "_ind_", intstring(i));
        indices->insertAtTail(new Symbol(SYMBOL, name));
      }
    } else {
      DefExpr* def_expr = domain->indices->first();
      while (def_expr) {
        indices->insertAtTail(new Symbol(SYMBOL, copystring(def_expr->sym->name)));
        def_expr = domain->indices->next();
      }
    }
  }    
  ForLoopStmt* loop
    = Symboltable::startForLoop(true, indices, array_type->domain->copy());
  BlockStmt* block_stmt = Symboltable::startCompoundStmt();
  NoOpStmt* noop_stmt = new NoOpStmt();
  Symboltable::finishCompoundStmt(block_stmt, new AList<Stmt>(noop_stmt));
  loop = Symboltable::finishForLoop(loop, block_stmt);
  stmt->insertBefore(loop);
  insert_init(noop_stmt, var, array_type->elementType);
  AList<DefExpr>* indices_change = loop->indices;
  TRAVERSE(loop->body, new InsertElidedIndices(indices_change), true);
}


static void insert_domain_init(Stmt* stmt, VarSymbol* var, Type* type) {
  DomainType* domain_type = dynamic_cast<DomainType*>(type);

  if (!domain_type) {
    INT_FATAL(var, "Domain type expected");
  }

  int rank = domain_type->numdims;

  SimpleSeqExpr* seq_init;

  ForallExpr* forall_init = dynamic_cast<ForallExpr*>(var->defPoint->init->expr);
  if (forall_init) {
    seq_init = dynamic_cast<SimpleSeqExpr*>(forall_init->domains->first());
  } else {
    seq_init = dynamic_cast<SimpleSeqExpr*>(var->defPoint->init->expr);
  }

  if (!seq_init) {
    INT_FATAL(var, "Invalid domain initialization expression");
  }

  for (int i = 0; i < rank; i++) {
    AList<Expr>* args = new AList<Expr>(new Variable(var));
    args->insertAtTail(new IntLiteral(intstring(i), i));
    args->insertAtTail(seq_init->lo->copy());
    args->insertAtTail(seq_init->hi->copy());
    args->insertAtTail(seq_init->str->copy());
    Symbol* init_domain_dim = Symboltable::lookupInternal("_INIT_DOMAIN_DIM");
    FnCall* call = new FnCall(new Variable(init_domain_dim), args);
    ExprStmt* call_stmt = new ExprStmt(call);
    stmt->insertBefore(call_stmt);
    if (forall_init) {
      seq_init = dynamic_cast<SimpleSeqExpr*>(forall_init->domains->next());
    }
  }
}


static Stmt* basic_default_init_stmt(Stmt* stmt, VarSymbol* var, Type* type) {
  if (var->noDefaultInit && var->type != dtString) {
    return new NoOpStmt();
  } else if (type->defaultVal) {
    Expr* lhs = new Variable(var);
    Expr* rhs = type->defaultVal->copy();
    Expr* init_expr = new AssignOp(GETS_NORM, lhs, rhs);
    return new ExprStmt(init_expr);
  } else if (type->defaultConstructor) {
    Expr* lhs = new Variable(var);
    Expr* constructor_variable = new Variable(type->defaultConstructor);
    Expr* rhs = new FnCall(constructor_variable);
    Expr* init_expr = new AssignOp(GETS_NORM, lhs, rhs);
    return new ExprStmt(init_expr);
  } else if (dynamic_cast<IndexType*>(type)) {
    return new NoOpStmt();
  } else {
    INT_FATAL(var, "Failed to insert default initialization");
    return new NoOpStmt();
  }
}


static void insert_config_init(Stmt* stmt, VarSymbol* var, Type* type) {

  // SJD: Note I want this to be a single macro INIT_CONFIG with the
  // default expression at the end and the if statement tucked away,
  // but because complex literals can't be passed, can't do this.
  // Need a traversal to change complex literals into complex variable
  // temporaries for function calls.

  Expr* init_expr = var->defPoint->init ? var->defPoint->init->expr : type->defaultVal;
  AList<Expr>* args = new AList<Expr>(new Variable(var));
  args->insertAtTail(new Variable(type->symbol));
  args->insertAtTail(new StringLiteral(copystring(var->name)));
  args->insertAtTail(new StringLiteral(var->parentScope->symContext->name));
  Symbol* init_config = Symboltable::lookupInternal("_INIT_CONFIG");
  FnCall* call = new FnCall(new Variable(init_config), args);
  Expr* assign = new AssignOp(GETS_NORM, new Variable(var), init_expr->copy());
  ExprStmt* assign_stmt = new ExprStmt(assign);
  CondStmt* cond_stmt = new CondStmt(call, new BlockStmt(new AList<Stmt>(assign_stmt)));
  stmt->insertBefore(cond_stmt);
}


static void insert_basic_init(Stmt* stmt, VarSymbol* var, Type* type) {
  if (!is_Scalar_Type(type) || !var->defPoint->init) {
    stmt->insertBefore(basic_default_init_stmt(stmt, var, type));
  }

  if (var->defPoint->init) {
    Expr* lhs = new Variable(var);
    Expr* rhs = var->defPoint->init->expr->copy();
    Expr* init_expr = new AssignOp(GETS_NORM, lhs, rhs);
    stmt->insertBefore(new ExprStmt(init_expr));
  }
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
  whichModules = MODULES_CODEGEN;
}


void InsertVariableInitializations::postProcessStmt(Stmt* stmt) {
  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
    if (dynamic_cast<TypeSymbol*>(def_stmt->parentSymbol)) {
      return;
    }

    if (!def_stmt->varDef()) {
      return;
    }

    DefExpr* def_expr = def_stmt->defExprls->first();
    while (def_expr) {
      VarSymbol* var = dynamic_cast<VarSymbol*>(def_expr->sym);
      insert_init(stmt, var, var->type);
      def_expr = def_stmt->defExprls->next();
    }
    
    //    def_stmt->remove();
  }
}
