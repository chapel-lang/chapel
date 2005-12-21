/*** cleanup
 ***
 *** This pass and function cleans up the AST after parsing but before
 *** scope resolution.
 ***/

#include "astutil.h"
#include "buildDefaultFunctions.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "stringutil.h"

static void normalize_anonymous_record_or_forall_expression(DefExpr* def);
static void destructure_indices(ForLoopStmt* stmt);
static void destructure_tuple(CallExpr* call);
static void construct_tuple_type(int size);
static void flatten_primary_methods(FnSymbol* fn);
static void resolve_secondary_method_type(FnSymbol* fn);
static void add_this_formal_to_method(FnSymbol* fn);
static void finish_constructor(FnSymbol* fn);


void cleanup(void) {
  forv_Vec(ModuleSymbol, mod, allModules)
    cleanup(mod);
  cleanup(tupleModule); // cleanup again because tuples are put here
}


void cleanup(BaseAST* base) {
  for (int i = 1; i <= 2; i++) {
  Vec<BaseAST*> asts;
  collect_asts(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
      normalize_anonymous_record_or_forall_expression(a);
    } else if (ForLoopStmt* a = dynamic_cast<ForLoopStmt*>(ast)) {
      destructure_indices(a);
    } else if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      SymExpr* base = dynamic_cast<SymExpr*>(a->baseExpr);
      if (base && !strncmp(base->var->name, "_tuple", 6)) {
        construct_tuple_type(atoi(base->var->name+6));
        CallExpr* parent = dynamic_cast<CallExpr*>(a->parentExpr);
        if (parent && parent->isAssign() && parent->get(1) == a)
          destructure_tuple(a);
      }
    }
  }

  asts.clear();
  collect_asts(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      flatten_primary_methods(fn);
      resolve_secondary_method_type(fn);
      add_this_formal_to_method(fn);
      finish_constructor(fn);
    }
  }
  }
}


/*** normalize_anonymous_record_or_forall_expression
 ***   moves anonymous record definition into separate statement
 ***   moves anonymous forall expression into separate statement
 ***   NOTE: during parsing, these may be embedded in expressions
 ***/
static void normalize_anonymous_record_or_forall_expression(DefExpr* def) {
  if (!def->parentStmt || (!def->parentExpr && def->parentStmt->astType == STMT_EXPR))
    return;
  Stmt* stmt = def->parentStmt;
  if (stmt->getFunction() == stmt->getModule()->initFn)
    stmt = dynamic_cast<Stmt*>(stmt->getFunction()->defPoint->parentStmt->next);
  if ((!strncmp("_anon_record", def->sym->name, 12)) ||
      (!strncmp("_forallexpr", def->sym->name, 11))) {
    def->replace(new SymExpr(def->sym));
    stmt->insertBefore(def);
  } else if (!strncmp("_let_fn", def->sym->name, 7)) {
    def->replace(new CallExpr(def->sym));
    stmt->insertBefore(def);
  }
}


/*** destructure_indices
 ***
 ***   for i, j in D {    ==>    for _index_temp in D {
 ***                               i = _index_temp(1);
 ***                               j = _index_temp(2);
 ***
 ***/
static void destructure_indices(ForLoopStmt* stmt) {
  if (stmt->indices->length() > 1) {
    VarSymbol* indextmp = new VarSymbol("_index_temp");
    int i = 1;
    for_alist(DefExpr, indexDef, stmt->indices) {
      indexDef->remove();
      indexDef->init = new CallExpr(indextmp, new_IntLiteral(i++));
      stmt->innerStmt->insertAtHead(indexDef);
    }
    stmt->indices->insertAtTail(new DefExpr(indextmp));
  }
}


/*** destructure_tuple
 ***
 ***   (i,j) = expr;    ==>    i = expr(1);
 ***                           j = expr(2);
 ***
 ***   NOTE: handles recursive tuple destructuring, (i,(j,k)) = ...
 ***/
static void destructure_tuple(CallExpr* call) {
  Stmt* stmt = call->parentStmt;
  VarSymbol* temp = new VarSymbol("_tuple_destruct");
  stmt->insertBefore(new DefExpr(temp));
  call->replace(new SymExpr(temp));
  int i = 1;
  for_alist(Expr, expr, call->argList) {
    if (CallExpr* callExpr = dynamic_cast<CallExpr*>(expr))
      if (callExpr->isNamed("typeof"))
        continue;
    stmt->insertAfter(
      new CallExpr(OP_MOVE, expr->remove(),
        new CallExpr(temp, new_IntLiteral(i++))));
  }
}


/*** construct_tuple_type
 ***   builds rank-dependent tuple type
 ***/
static void construct_tuple_type(int rank) {
  currentLineno = 0;

  char *name = stringcat("_tuple", intstring(rank));

  if (Symboltable::lookupInScope(name, tupleModule->modScope))
    return;

  AList<Stmt>* decls = new AList<Stmt>();

  // Build type declarations
  Vec<Type*> types;
  for (int i = 1; i <= rank; i++) {
    char* typeName = stringcat("_t", intstring(i));
    VariableType* type = new VariableType(getMetaType(0));
    TypeSymbol* typeSymbol = new TypeSymbol(typeName, type);
    type->addSymbol(typeSymbol);
    decls->insertAtTail(new DefExpr(typeSymbol));
    types.add(type);
  }

  // Build field declarations
  Vec<VarSymbol*> fields;
  for (int i = 1; i <= rank; i++) {
    char* fieldName = stringcat("_f", intstring(i));
    VarSymbol* field = new VarSymbol(fieldName, types.v[i-1]);
    decls->insertAtTail(new DefExpr(field));
    fields.add(field);
  }

  // Build this methods
  for (int i = 1; i <= rank; i++) {
    FnSymbol* fn = new FnSymbol("this");
    if (no_infer) {
      ArgSymbol* arg = new ArgSymbol(INTENT_PARAM, "index", dtInteger);
      fn->formals = new AList<DefExpr>(new DefExpr(arg));
      fn->whereExpr = new CallExpr(OP_EQUAL, arg, new_IntLiteral(i));
    } else {
      ArgSymbol* arg = new ArgSymbol(INTENT_PARAM, "index", new_LiteralType(new_IntSymbol(i)));
      fn->formals = new AList<DefExpr>(new DefExpr(arg));
    }

    fn->retRef = true;
    fn->body = new BlockStmt(new ReturnStmt(fields.v[i-1]->name));
    DefExpr* def = new DefExpr(fn);
    if (no_infer)
      def->exprType = new SymExpr(types.v[i-1]->symbol);
    decls->insertAtTail(def);
  }

  // Build tuple
  ClassType* tupleType = new ClassType(CLASS_RECORD);
  TypeSymbol* tupleSym = new TypeSymbol(name, tupleType);
  tupleType->addSymbol(tupleSym);
  tupleType->addDeclarations(decls);
  tupleModule->stmts->insertAtHead(new DefExpr(tupleSym));
  cleanup(tupleSym);

  if (!fnostdincs) {
    // Build write function
    FnSymbol* fwriteFn = new FnSymbol("fwrite");
    TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(Symboltable::lookupInFileModuleScope("file"));
    ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", fileType->definition);
    ArgSymbol* fwriteArg = new ArgSymbol(INTENT_BLANK, "val", tupleType);
    fwriteFn->formals = new AList<DefExpr>(new DefExpr(fileArg), new DefExpr(fwriteArg));
    AList<Expr>* actuals = new AList<Expr>();
    actuals->insertAtTail(new_StringLiteral(stringcpy("(")));
    for (int i = 1; i <= rank; i++) {
      if (i != 1)
        actuals->insertAtTail(new_StringLiteral(stringcpy(", ")));
      actuals->insertAtTail(
        new MemberAccess(
          new UnresolvedSymbol("val"),
          stringcat("_f", intstring(i))));
    }
    actuals->insertAtTail(new_StringLiteral(stringcpy(")")));
    Expr* fwriteCall = new CallExpr("fwrite", new SymExpr(fileArg), actuals);
    fwriteFn->body = new BlockStmt(new ExprStmt(fwriteCall));
    tupleModule->stmts->insertAtTail(new DefExpr(fwriteFn));
    cleanup(fwriteFn);
  }

  // Build htuple = tuple function
  if (!fnostdincs && !fnostdincs_but_file) {
    FnSymbol* assignFn = new FnSymbol("=");
    ArgSymbol* htupleArg = 
      new ArgSymbol(INTENT_BLANK, "_htuple", chpl_htuple->definition);
    ArgSymbol* tupleArg = new ArgSymbol(INTENT_BLANK, "val", tupleType);
    assignFn->formals = new AList<DefExpr>(new DefExpr(htupleArg),
                                           new DefExpr(tupleArg));
    assignFn->body = new BlockStmt();
    for (int i = 1; i <= rank; i++) {
      assignFn->insertAtTail(
        new CallExpr("=",
          new CallExpr(htupleArg, new_IntLiteral(i)),
          new CallExpr(tupleArg, new_IntLiteral(i))));
    }
    assignFn->insertAtTail(new ReturnStmt(htupleArg));
    tupleModule->stmts->insertAtTail(new DefExpr(assignFn));
    cleanup(assignFn);
  }

  // Build tuple = _ function
//   {
//     FnSymbol* assignFn = new FnSymbol("=");
//     ArgSymbol* tupleArg = new ArgSymbol(INTENT_BLANK, "tuple", tupleType);
//     ArgSymbol* secondArg = new ArgSymbol(INTENT_BLANK, "val", dtUnknown);
//     assignFn->formals = new AList<DefExpr>(new DefExpr(tupleArg),
//                                            new DefExpr(secondArg));
//     assignFn->body = new BlockStmt();
//     for (int i = 1; i <= rank; i++) {
//       assignFn->insertAtTail(
//         new ExprStmt(
//           new CallExpr(OP_MOVE,
//             new CallExpr(tupleArg, new_IntLiteral(i)),
//             new CallExpr(secondArg, new_IntLiteral(i)))));
//     }
//     assignFn->insertAtTail(new ReturnStmt(tupleArg));
//     tupleModule->stmts->insertAtTail(new ExprStmt(new DefExpr(assignFn)));
//   }

  buildDefaultClassTypeMethods(tupleType);
}


static void flatten_primary_methods(FnSymbol* fn) {
  if (dynamic_cast<TypeSymbol*>(fn->defPoint->parentSymbol)) {
    Stmt* insertPoint = fn->typeBinding->defPoint->parentStmt;
    if (!fn->typeBinding)
      INT_FATAL(fn, "Primary method has no typeBinding");
    while (dynamic_cast<TypeSymbol*>(insertPoint->parentSymbol))
      insertPoint = insertPoint->parentSymbol->defPoint->parentStmt;
    Stmt* stmt = fn->defPoint->parentStmt;
    stmt->remove();
    insertPoint->insertBefore(stmt);
  }
}


static void resolve_secondary_method_type(FnSymbol* fn) {
  if (fn->typeBinding && fn->typeBinding->isUnresolved) {
    Symbol* typeBindingSymbol = Symboltable::lookupFromScope(fn->typeBinding->name, fn->parentScope);
    assert(!typeBindingSymbol->isUnresolved);
    if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(typeBindingSymbol)) {
      Type* typeBinding = ts->definition;
      fn->typeBinding = ts;
      if (fn->fnClass != FN_CONSTRUCTOR) {
        fn->method_type = SECONDARY_METHOD;
      }
      typeBinding->methods.add(fn);
    } else {
      USR_FATAL(fn, "Function is not bound to type");
    }
  }
}


static void add_this_formal_to_method(FnSymbol* fn) {
  if (fn->_this) // already added
    return;
  if (fn->typeBinding && fn->fnClass != FN_CONSTRUCTOR) {
    fn->cname = stringcat("_", fn->typeBinding->cname, "_", fn->cname);
    ArgSymbol* this_insert = new ArgSymbol(INTENT_REF, "this", fn->typeBinding->definition);
    fn->formals->insertAtHead(new DefExpr(this_insert));
    fn->_this = this_insert;
    if (strcmp(fn->name, "this")) {
      ArgSymbol* token_dummy = new ArgSymbol(INTENT_REF, "_methodTokenDummy",
                                             dtMethodToken);
      fn->formals->insertAtHead(new DefExpr(token_dummy));
    }
    if (fn->isSetter) {
      ArgSymbol* setter_dummy = new ArgSymbol(INTENT_REF, "_setterTokenDummy", 
                                              dtSetterToken);
      fn->formals->last()->insertBefore(new DefExpr(setter_dummy));
    }
  }
}


static void
finish_constructor(FnSymbol* fn) {
  if (fn->fnClass != FN_CONSTRUCTOR)
    return;
  CallExpr *inner = 0;

  if (ReturnStmt* stmt = dynamic_cast<ReturnStmt*>(fn->body->body->last())) {
    if (stmt->expr)
      return; // already finished
  }

  ClassType* ct = dynamic_cast<ClassType*>(fn->typeBinding->definition);

  fn->_this = new VarSymbol("this");

  if (fn->body->body->length() > 0) {
    FnSymbol* user_fn = new FnSymbol(stringcat("_user_", fn->name));
    user_fn->formals = new AList<DefExpr>();
    user_fn->typeBinding = ct->symbol;
    BlockStmt* user_body = fn->body;
    fn->body->replace(new BlockStmt());
    fn->insertAtHead(new DefExpr(user_fn));
    user_fn->body->replace(user_body);
    fn->insertAtHead(new CallExpr((inner = new CallExpr(user_fn, methodToken, fn->_this))));
    inner->partialTag = PARTIAL_OK;
    ct->methods.add(user_fn);
    cleanup(user_fn);
  }

  dynamic_cast<VarSymbol*>(fn->_this)->noDefaultInit = true;

  char* description = stringcat("instance of class ", ct->symbol->name);
  Expr* alloc_rhs = new CallExpr(Symboltable::lookupInternal("_chpl_alloc"),
                                 ct->symbol,
                                 new_StringLiteral(description));
  CallExpr* alloc_expr = new CallExpr(OP_MOVE, fn->_this, alloc_rhs);

  AList<Stmt>* stmts = new AList<Stmt>();

  stmts->insertAtTail(new DefExpr(fn->_this));
  stmts->insertAtTail(alloc_expr);

  // assign formals to fields by name
  forv_Vec(Symbol, field, ct->fields) {
    for_alist(DefExpr, formalDef, fn->formals) {
      if (ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym)) {
        if (!strcmp(formal->name, field->name)) {
          Expr* lhs = new MemberAccess(fn->_this, field);
          Expr* assign_expr =
            (no_infer)
            ? new CallExpr("=", lhs, formal)
            : new CallExpr(OP_MOVE, lhs, formal);
          stmts->insertAtTail(assign_expr);
        }
      }
    }
  }

  fn->insertAtHead(stmts);

  // fix type variables, associate by name
  for_alist(DefExpr, formalDef, fn->formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (formal->intent == INTENT_TYPE) {
      forv_Vec(TypeSymbol, type, ct->types) {
        if (VariableType* variableType = dynamic_cast<VariableType*>(type->definition)) {
          if (!strcmp(type->name, formal->name)) {
            formal->type = variableType->type;
            formal->isGeneric = true;
            formal->genericSymbol = type;
          }
        }
      }
    }
  }

  fn->insertAtTail(new ReturnStmt(fn->_this));
  fn->retType = ct;
}
