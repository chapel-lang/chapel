#include <typeinfo>
#include "insertVariableInitializations.h"
#include "insertElidedLoops.h"
#include "symtab.h"
#include "symscope.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"


static void insert_init(Stmt* stmt, VarSymbol* var, Type* type);
static void insert_init_at_stmt(Stmt* stmt, VarSymbol* var, Type* type);
//static void insert_default_init(Stmt* stmt, VarSymbol* var, Type* type);
static void insert_user_default_init(Stmt* stmt, VarSymbol* var, Type* type);
static void insert_user_init(Stmt* stmt, VarSymbol* var);

void insert_default_init_stmt(VarSymbol* var, Stmt* init_stmt) {
  if (var->parentScope->stmtContext) {
    if (BlockStmt* block_stmt =
        dynamic_cast<BlockStmt*>(var->parentScope->stmtContext)) {
      block_stmt->body->insertBefore(init_stmt);
    }
    else if (DefStmt* def_stmt =
             dynamic_cast<DefStmt*>(var->parentScope->stmtContext)) {
      if (ModuleSymbol* mod = def_stmt->moduleDef()) {
        if (BlockStmt* block_stmt =
            dynamic_cast<BlockStmt*>(mod->initFn->body)) {
          block_stmt->body->insertBefore(init_stmt);
        }
        else {
          INT_FATAL(var, "SJD \"Where should I put the default init?\"");
        }
      }
      else if (def_stmt->typeDef()) {
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
  else if (DefExpr* def_expr = dynamic_cast<DefExpr*>(var->parentScope->exprContext)) {
    if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(def_expr->sym)) {
      if (BlockStmt* block_stmt =
          dynamic_cast<BlockStmt*>(mod->initFn->body)) {
        block_stmt->body->insertBefore(init_stmt);
      }
      else {
        INT_FATAL(var, "SJD \"Where should I put the default init?\"");
      }
    }
    else if (dynamic_cast<TypeSymbol*>(def_expr->sym)) {
      // Ignore case
    }
    else if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
      /** basically for __init_fn which has no block_stmt scope **/
      if (BlockStmt* block_stmt = dynamic_cast<BlockStmt*>(fn->body)) {
        block_stmt->body->insertBefore(init_stmt);
      }
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
  insert_user_init_stmt(stmt, call_stmt);

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
  insert_user_init_stmt(stmt, loop);
  insert_init_at_stmt(noop_stmt, var, array_type->elementType);
  Symbol* indices_change = dynamic_cast<DefExpr*>(loop->indices)->sym;
  TRAVERSE(loop->body, new InsertElidedIndices(indices_change), true);
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


// static void insert_string_default_init(Stmt* stmt, VarSymbol* var, Type* type) {
//   if (type->defaultVal) {
//     Expr* args = new Variable(var);
//     args->append(new Variable(var));
//     args->append(type->defaultVal->copy());
//     Symbol* init_string = Symboltable::lookupInternal("_init_string");
//     FnCall* call = new FnCall(new Variable(init_string), args);
//     ExprStmt* call_stmt = new ExprStmt(call);
//     insert_default_init_stmt(var, call_stmt);
//   }
// }


static void insert_string_user_default_init(Stmt* stmt, VarSymbol* var, Type* type) {
  if (type->defaultVal) {
    Expr* args = new Variable(var);
    args->append(new Variable(var));
    args->append(type->defaultVal->copy());
    Symbol* init_string = Symboltable::lookupInternal("_init_string");
    FnCall* call = new FnCall(new Variable(init_string), args);
    ExprStmt* call_stmt = new ExprStmt(call);
    stmt->insertBefore(call_stmt);
  }
}


static void insert_config_init(Stmt* stmt, VarSymbol* var) {

  // SJD: Note I want this to be a single macro INIT_CONFIG with the
  // default expression at the end and the if statement tucked away,
  // but because complex literals can't be passed, can't do this.
  // Need a traversal to change complex literals into complex variable
  // temporaries for function calls.

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


// static void insert_default_init(Stmt* stmt, VarSymbol* var, Type* type) {

//   //
//   // SJD: This code squelches inserting the default init statement for
//   // classes within their own constructor.  This avoids generating
//   // code like the following which crashed my whole system:
//   //
//   //   ClassName _construct_ClassName(void) {
//   //     ClassName this;
//   //     this = _construct_ClassName();  <=== Squelched
//   //
//   if (StructuralType* class_type = dynamic_cast<StructuralType*>(type)) {
//     if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(class_type->constructor)) {
//       if (DefExpr* def_expr = dynamic_cast<DefExpr*>(def_stmt->defExprList)) {
//         if (def_expr->sym == stmt->parentSymbol) {
//           return;
//         }
//       }
//     }
//   }

//   if (type->defaultVal) {
//     AssignOp* assign_expr = new AssignOp(GETS_NORM,
//                                          new Variable(var),
//                                          type->defaultVal->copy());
//     ExprStmt* assign_stmt = new ExprStmt(assign_expr);
//     insert_default_init_stmt(var, assign_stmt);
//   }
// }


static void insert_user_default_init(Stmt* stmt, VarSymbol* var, Type* type) {
  if (type->defaultVal) {
    AssignOp* assign_expr = new AssignOp(GETS_NORM,
                                         new Variable(var),
                                         type->defaultVal->copy());
    ExprStmt* assign_stmt = new ExprStmt(assign_expr);
    stmt->insertBefore(assign_stmt);
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


static void insert_init(Stmt* stmt, VarSymbol* var, Type* type) {
//   //
//   // Default initialization (at the top of the scope)
//   //
//   if (dynamic_cast<ArrayType*>(type)) {
//     // Default initialization for arrays done at the DefStmt for now
//   } else if (dynamic_cast<DomainType*>(type)) {
//     // Default initialization for domains not done
//   } else if (type == dtString) {
//     insert_string_default_init(stmt, var, type);
//   } else {
//     insert_default_init(stmt, var, type);
//   }

  //
  // User initialization (at the DefStmt)
  //
  if (dynamic_cast<ArrayType*>(type)) {
    insert_array_init(stmt, var, type);
  } else if (dynamic_cast<DomainType*>(type)) {
    insert_domain_init(stmt, var);
  } else if (var->varClass == VAR_CONFIG) {
    insert_config_init(stmt, var);
  } else {
    insert_user_init(stmt, var);
  }
}


//
// This procedure is like insert_init, except the default
// initialization is done at the DefStmt rather than at the top of the
// scope.  This is how it is currently handled for arrays.
//
static void insert_init_at_stmt(Stmt* stmt, VarSymbol* var, Type* type) {
  //
  // Default initialization (at the DefStmt)
  //
  if (dynamic_cast<ArrayType*>(type)) {
    // Default initialization for arrays done at the DefStmt for now
  } else if (dynamic_cast<DomainType*>(type)) {
    // Default initialization for domains not done
  } else if (type == dtString) {
    insert_string_user_default_init(stmt, var, type);
  } else {
    insert_user_default_init(stmt, var, type);
  }

  //
  // User initialization (at the DefStmt)
  //
  if (dynamic_cast<ArrayType*>(type)) {
    insert_array_init(stmt, var, type);
  } else if (dynamic_cast<DomainType*>(type)) {
    insert_domain_init(stmt, var);
  } else if (var->varClass == VAR_CONFIG) {
    INT_FATAL(stmt, "Array of configs?! or something");
  } else {
    insert_user_init(stmt, var);
  }
}


InsertVariableInitializations::InsertVariableInitializations() {
  whichModules = MODULES_COMMON_AND_USER;
}


void InsertVariableInitializations::postProcessStmt(Stmt* stmt) {
  DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt);

  if (!def_stmt) {
    return;
  }

  if (dynamic_cast<TypeSymbol*>(def_stmt->parentSymbol)) {
    return;
  }

  if (!def_stmt->varDef()) {
    return;
  }

  DefExpr* def_expr = dynamic_cast<DefExpr*>(def_stmt->defExprList);
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
