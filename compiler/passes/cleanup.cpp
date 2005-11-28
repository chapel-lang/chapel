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
static void destructure_tuple(CallExpr* call, Vec<BaseAST*>* asts);
static void construct_tuple_type(int size);
static void build_class_init_function(ClassType* ct);
static void flatten_primary_methods(FnSymbol* fn);
static void resolve_secondary_method_type(FnSymbol* fn);
static void add_this_formal_to_method(FnSymbol* fn);
static void finish_constructor(FnSymbol* fn);


void cleanup(void) {
  Vec<BaseAST*> asts;
  Vec<Symbol*> syms;
  Vec<FnSymbol*> fns;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
      normalize_anonymous_record_or_forall_expression(a);
    } else if (ForLoopStmt* a = dynamic_cast<ForLoopStmt*>(ast)) {
      destructure_indices(a);
    } else if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      SymExpr* base = dynamic_cast<SymExpr*>(a->baseExpr);
      CallExpr* parent = dynamic_cast<CallExpr*>(a->parentExpr);
      if (base && !strncmp(base->var->name, "_tuple", 6)) {
        construct_tuple_type(atoi(base->var->name+6));
        if (parent && OP_ISASSIGNOP(parent->opTag) && parent->get(1) == a)
          destructure_tuple(a, &asts);
      }
    }
  }
  asts.clear();
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(ast)) {
      if (TypeSymbol* type = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
        if (ClassType* ct = dynamic_cast<ClassType*>(type->definition)) {
          build_class_init_function(ct);
        }
      }
    }
  }
  collect_functions(&fns);
  forv_Vec(FnSymbol, fn, fns) {
    flatten_primary_methods(fn);
    resolve_secondary_method_type(fn);
    add_this_formal_to_method(fn);
    finish_constructor(fn);
  }
}


/*** normalize_anonymous_record_or_forall_expression
 ***   moves anonymous record definition into separate statement
 ***   moves anonymous forall expression into separate statement
 ***   NOTE: during parsing, these may be embedded in expressions
 ***/
static void normalize_anonymous_record_or_forall_expression(DefExpr* def) {
  if ((!strncmp("_anon_record", def->sym->name, 12)) ||
      (!strncmp("_forallexpr", def->sym->name, 11))) {
    Stmt* stmt = def->parentStmt;
    def->replace(new SymExpr(def->sym));
    stmt->insertBefore(new ExprStmt(def));
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
      stmt->innerStmt->insertAtHead(new ExprStmt(indexDef));
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
static void destructure_tuple(CallExpr* call, Vec<BaseAST*>* asts) {
  VarSymbol* _tuple_destruct = new VarSymbol("_tuple_destruct");
  DefExpr* def = new DefExpr(_tuple_destruct);
  call->parentStmt->insertBefore(new ExprStmt(def));
  int start = call->argList->length()/2;
  int i = 0;
  for_alist(Expr, expr, call->argList) {
    if (++i <= start)
      continue;
    Stmt* stmt = new ExprStmt(
                   new CallExpr(OP_GETSNORM, expr->copy(),
                     new CallExpr(_tuple_destruct, new_IntLiteral(i-start))));
    collect_asts(asts, stmt);
    call->parentStmt->insertAfter(stmt);
  }
  call->replace(new SymExpr(def->sym));
}


/*** construct_tuple_type
 ***   builds rank-dependent tuple type
 ***/
static void construct_tuple_type(int rank) {
  char *name = stringcat("_tuple", intstring(rank));

  if (Symboltable::lookupInScope(name, commonModule->modScope))
    return;

  AList<Stmt>* decls = new AList<Stmt>();

  // Build type declarations
  Vec<Type*> types;
  for (int i = 1; i <= rank; i++) {
    char* typeName = stringcat("_t", intstring(i));
    VariableType* type = new VariableType(getMetaType(0));
    TypeSymbol* typeSymbol = new TypeSymbol(typeName, type);
    type->addSymbol(typeSymbol);
    decls->insertAtTail(new ExprStmt(new DefExpr(typeSymbol)));
    types.add(type);
  }

  // Build field declarations
  Vec<VarSymbol*> fields;
  for (int i = 1; i <= rank; i++) {
    char* fieldName = stringcat("_f", intstring(i));
    VarSymbol* field = new VarSymbol(fieldName, types.v[i-1]);
    decls->insertAtTail(new ExprStmt(new DefExpr(field)));
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
    decls->insertAtTail(new ExprStmt(def));
  }

  // Build tuple
  ClassType* tupleType = new ClassType(CLASS_RECORD);
  TypeSymbol* tupleSym = new TypeSymbol(name, tupleType);
  tupleType->addSymbol(tupleSym);
  tupleType->addDeclarations(decls);
  commonModule->stmts->insertAtHead(new ExprStmt(new DefExpr(tupleSym)));

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
  commonModule->stmts->insertAtTail(new ExprStmt(new DefExpr(fwriteFn)));

  // Build htuple = tuple function
  {
    FnSymbol* assignFn = new FnSymbol("=");
    ArgSymbol* htupleArg = 
      new ArgSymbol(INTENT_BLANK, "_htuple", chpl_htuple->definition);
    ArgSymbol* tupleArg = new ArgSymbol(INTENT_BLANK, "val", tupleType);
    assignFn->formals = new AList<DefExpr>(new DefExpr(htupleArg),
                                           new DefExpr(tupleArg));
    assignFn->body = new BlockStmt();
    for (int i = 1; i <= rank; i++) {
      assignFn->insertAtTail(
        new ExprStmt(
          new CallExpr(OP_GETSNORM,
            new CallExpr(htupleArg, new_IntLiteral(i)),
            new CallExpr(tupleArg, new_IntLiteral(i)))));
    }
    assignFn->insertAtTail(new ReturnStmt(htupleArg));
    commonModule->stmts->insertAtTail(new ExprStmt(new DefExpr(assignFn)));
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
//           new CallExpr(OP_GETSNORM,
//             new CallExpr(tupleArg, new_IntLiteral(i)),
//             new CallExpr(secondArg, new_IntLiteral(i)))));
//     }
//     assignFn->insertAtTail(new ReturnStmt(tupleArg));
//     commonModule->stmts->insertAtTail(new ExprStmt(new DefExpr(assignFn)));
//   }

  buildDefaultClassTypeMethods(tupleType);
}


static bool stmt_defines_function(Stmt* stmt) {
  if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt))
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr))
      if (dynamic_cast<FnSymbol*>(defExpr->sym))
        return true;
  return false;
}


static void build_class_init_function(ClassType* ct) {
  if (ct->initFn)
    INT_FATAL("ahhh");
  ct->initFn = new FnSymbol(stringcat("_init_", ct->symbol->name));
  ct->initFn->isInitFn = true;
  ct->initFn->formals = new AList<DefExpr>();
  ct->initFn->method_type = PRIMARY_METHOD;
  ct->initFn->typeBinding = ct->symbol;
  if (use_class_init) {
    ct->initFn->body = new BlockStmt();
    ct->initFn->body->blkScope = ct->structScope;
    for_alist(Stmt, stmt, ct->declarationList) {
      stmt->remove();
      if (stmt_defines_function(stmt))
        ct->symbol->defPoint->parentStmt->insertBefore(stmt);
      else
        ct->initFn->insertAtTail(stmt);
    }
  }
  ct->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(new DefExpr(ct->initFn)));
  ct->methods.add(ct->initFn);
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
  if (TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(fn->typeBinding)) {
    fn->cname = stringcat("_", typeSym->cname, "_", fn->cname);
    if (fn->fnClass != FN_CONSTRUCTOR) {
      ArgSymbol* this_insert = new ArgSymbol(INTENT_REF, "this", typeSym->definition);
      fn->formals->insertAtHead(new DefExpr(this_insert));
      fn->_this = this_insert;
      bool isThisMethod = !strcmp(fn->name, "this");
      if (!isThisMethod) {
        ArgSymbol* token_dummy = new ArgSymbol(INTENT_REF, "_methodTokenDummy",
                                               dtMethodToken);
        fn->formals->insertAtHead(new DefExpr(token_dummy));
      }
      if (fn->isSetter) {
        ArgSymbol* setter_dummy = new ArgSymbol(INTENT_REF, "_setterTokenDummy", 
                                                dtSetterToken);
        int len = fn->formals->length();
        fn->formals->get(len)->insertBefore(new DefExpr(setter_dummy));
      }
    }
  }
}


static void
finish_constructor(FnSymbol* fn) {
  if (fn->fnClass != FN_CONSTRUCTOR)
    return;

  ClassType* ct = dynamic_cast<ClassType*>(fn->typeBinding->definition);

  fn->_this = new VarSymbol("this", ct);
  dynamic_cast<VarSymbol*>(fn->_this)->noDefaultInit = true;

  char* description = stringcat("instance of class ", ct->symbol->name);
  Expr* alloc_rhs = new CallExpr(Symboltable::lookupInternal("_chpl_alloc"),
                                 ct->symbol,
                                 new_StringLiteral(description));
  CallExpr* alloc_expr = new CallExpr(OP_GETSNORM, fn->_this, alloc_rhs);
  if (no_infer) {
    alloc_expr->baseExpr = new SymExpr("_move");
    alloc_expr->opTag = OP_NONE;
  }
  Stmt* alloc_stmt = new ExprStmt(alloc_expr);

  AList<Stmt>* stmts = new AList<Stmt>();

  stmts->insertAtTail(new ExprStmt(new DefExpr(fn->_this)));
  stmts->insertAtTail(alloc_stmt);

  if (use_class_init)
    stmts->insertAtTail(new ExprStmt(new CallExpr(ct->initFn->name)));
  else
    stmts->insertAtTail(new ExprStmt(new CallExpr(ct->initFn, Symboltable::lookupInternal("_methodToken"), fn->_this)));

  // assign formals to fields by name
  forv_Vec(Symbol, field, ct->fields) {
    for_alist(DefExpr, formalDef, fn->formals) {
      if (ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym)) {
        if (!strcmp(formal->name, field->name)) {
          Expr* lhs = new MemberAccess(fn->_this, field);
          Expr* assign_expr = new CallExpr(OP_GETSNORM, lhs, formal);
          Stmt* assign_stmt = new ExprStmt(assign_expr);
          stmts->insertAtTail(assign_stmt);
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
