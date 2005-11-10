#include "buildDefaultFunctions.h"
#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"
#include "stringutil.h"
#include "filesToAST.h"
#include "runtime.h"


static void build_chpl_main(Vec<ModuleSymbol*>* modules);
static void build_constructor(ClassType* ct);
static void build_init(ClassType* ct);
static void build_getter(ClassType* ct, Symbol *tmp);
static void build_setters_and_getters(ClassType* ct);
static void build_record_equality_function(ClassType* ct);
static void build_record_inequality_function(ClassType* ct);
static void build_record_assignment_function(ClassType* ct);
static void buildDefaultClassTypeMethods(ClassType* ct);
static void buildDefaultIOFunctions(Type* type);
static void finishConstructor(FnSymbol* fn);


// function_exists returns true iff
//  function's name matches name
//  function's number of formals matches numFormals if not -1
//  function's typeBinding matches typeBinding if not NULL
//  function's first formal's type's name matches formalTypeName1 if not NULL
//  function's second formal's type's name matches formalTypeName2 if not NULL

static bool type_name_match(char* name, Symbol* sym) {
  if (!strcmp(name, sym->type->symbol->name))
    return true;
  SymExpr* symExpr = dynamic_cast<SymExpr*>(sym->defPoint->exprType);
  if (symExpr && !strcmp(name, symExpr->var->name))
    return true;
  return false;
}

static FnSymbol* function_exists(char* name,
                                 int numFormals = -1,
                                 Type* typeBinding = NULL,
                                 char* formalTypeName1 = NULL,
                                 char* formalTypeName2 = NULL) {
  Vec<FnSymbol*> fns;
  collect_functions(&fns);
  FnSymbol* match = NULL;
  forv_Vec(FnSymbol, fn, fns) {
    if (strcmp(name, fn->name))
      continue;

    if (numFormals != -1)
      if (numFormals != fn->formals->length())
        continue;

    if (typeBinding)
      if (typeBinding != fn->typeBinding->definition)
        continue;

    if (formalTypeName1)
      if (!type_name_match(formalTypeName1, fn->formals->get(1)->sym))
        continue;

    if (formalTypeName2)
      if (!type_name_match(formalTypeName2, fn->formals->get(2)->sym))
        continue;

    if (!match) {
      match = fn;
    } else if (!strcmp("main", fn->name)) {
      USR_FATAL(fn, "main multiply defined -- first occurrence at %s",
                match->stringLoc());
    }
  }
  return match;
}


void BuildDefaultFunctions::run(Vec<ModuleSymbol*>* modules) {
  build_chpl_main(modules);

  Vec<BaseAST*> asts;
  collect_asts(&asts);

  forv_Vec(BaseAST, ast, asts) {
    if (TypeSymbol* type = dynamic_cast<TypeSymbol*>(ast)) {
      buildDefaultIOFunctions(type->definition);
      if (ClassType* ct = dynamic_cast<ClassType*>(type->definition)) {
        buildDefaultClassTypeMethods(ct);
      }

    }
  }

  asts.clear();
  collect_asts(&asts);

  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      if (fn->fnClass == FN_CONSTRUCTOR)
        finishConstructor(fn);
    }
  }
}


static ExprStmt* buildCallExprStmt(FnSymbol* fn) {
  SymExpr* variable = new SymExpr(fn);
  variable->lineno = -1;
  CallExpr* fnCall = new CallExpr(variable);
  fnCall->lineno = -1;
  ExprStmt* exprStmt = new ExprStmt(fnCall);
  exprStmt->lineno = -1;
  return exprStmt;
}


static ModuleSymbol* findUniqueUserModule(Vec<ModuleSymbol*>* modules) {
  ModuleSymbol* userModule = NULL;

  forv_Vec(ModuleSymbol, mod, *modules) {
    if (mod->modtype == MOD_USER) {
      if (userModule == NULL) {
        userModule = mod;
      } else {
        return NULL;  // two user modules defined
      }
    }
  }
  return userModule;
}


static void build_chpl_main(Vec<ModuleSymbol*>* modules) {
  // find main function if it exists; create one if not
  chpl_main = function_exists("main", 0);

  BlockStmt* mainBody = 0;
  ModuleSymbol* mainModule = 0;
  if (!chpl_main) {
    mainModule = findUniqueUserModule(modules);
    if (mainModule) {
      mainBody = new BlockStmt();
      chpl_main = new FnSymbol("main", NULL, new AList<DefExpr>(), dtVoid, NULL, mainBody);
      mainModule->stmts->insertAtTail(new ExprStmt(new DefExpr(chpl_main)));
    } else {
      USR_FATAL("Code defines multiple modules but no main function.");
    }
  } else {
    // tack call to main fn module's init call onto main fn's body
    mainModule = dynamic_cast<ModuleSymbol*>(chpl_main->parentScope->astParent);
    if (!mainModule) {
      INT_FATAL(chpl_main, "main function's parent scope wasn't a module scope");
    }
    mainBody = chpl_main->body;
  }
  mainBody->insertAtHead(buildCallExprStmt(mainModule->initFn));
  mainBody->insertAtHead(buildCallExprStmt(commonModule->initFn));
  mainBody->insertAtHead(buildCallExprStmt(prelude->initFn));
}


static void build_init(ClassType* ct) {
  ct->initFn = new FnSymbol(stringcat("_init_", ct->symbol->name));
  ct->initFn->formals = new AList<DefExpr>();

//   if (use_init_expr) {
//     forv_Vec(Symbol, field, ct->fields) {
//       Expr* type = NULL;
//       if (field->defPoint->exprType)
//         type = field->defPoint->exprType->copy();
//       else if (field->defPoint->init)
//         type = new CallExpr("typeof", field->defPoint->init->copy());
//       ct->initFn->insertAtTail(new ExprStmt(new InitExpr(field, type)));
//     }
//   }

  reset_file_info(ct->initFn, ct->symbol->lineno, ct->symbol->filename);
  ct->symbol->defPoint->parentStmt->insertBefore
    (new ExprStmt(new DefExpr(ct->initFn)));
  ct->methods.add(ct->initFn);
  ct->initFn->method_type = PRIMARY_METHOD;
  ct->initFn->typeBinding = ct->symbol;
}


static void build_constructor(ClassType* ct) {
  ct->defaultConstructor = function_exists("initialize", -1, ct);
  if (ct->defaultConstructor)
    return;

  char* name = stringcat("_construct_", ct->symbol->name);
  FnSymbol* fn = new FnSymbol(name);
  ct->defaultConstructor = fn;
  fn->fnClass = FN_CONSTRUCTOR;
  fn->cname = stringcat("_construct_", ct->symbol->cname);

  AList<DefExpr>* args = new AList<DefExpr>();

  forv_Vec(TypeSymbol, tmp, ct->types) {
    if (VariableType *tv = dynamic_cast<VariableType*>(tmp->definition)) {
      char* name = tmp->name;
      Type* type = tv->type;
      ArgSymbol* arg = new ArgSymbol(INTENT_TYPE, name, type);
      arg->isGeneric = true;
      arg->genericSymbol = dynamic_cast<TypeSymbol*>(tv->symbol);
      args->insertAtTail(new DefExpr(arg, new SymExpr(dtUnknown->symbol)));
    }
  }

  forv_Vec(Symbol, tmp, ct->fields) {
    char* name = tmp->name;
    Type* type = tmp->type;
    Expr* init = (tmp->defPoint->init) 
      ? tmp->defPoint->init->copy()
      : new SymExpr(gNil);
    Expr* exprType = (tmp->defPoint->exprType)
      ? tmp->defPoint->exprType->copy()
      : NULL;
    if (tmp->defPoint->init) {
      tmp->defPoint->init->remove();
    }
    VarSymbol *vtmp = dynamic_cast<VarSymbol*>(tmp);
    ArgSymbol* arg = new ArgSymbol((vtmp && vtmp->consClass == VAR_PARAM) ? INTENT_PARAM : INTENT_BLANK, name, type);
    DefExpr* defExpr = new DefExpr(arg, init, exprType);
    args->insertAtTail(defExpr);
  }

  fn->formals = args;

  reset_file_info(fn, ct->symbol->lineno, ct->symbol->filename);
  ct->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(new DefExpr(fn)));
  ct->methods.add(fn);
  if (ct->symbol->hasPragma("data class")) {
    fn->addPragma("rename _data_construct");
    fn->addPragma("keep types");
  }
  fn->typeBinding = ct->symbol;
}


static void
finishConstructor(FnSymbol* fn) {
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
  stmts->insertAtTail(new ExprStmt(new CallExpr(ct->initFn, Symboltable::lookupInternal("_methodToken"), fn->_this)));

  // assign formals to fields by name
  forv_Vec(Symbol, field, ct->fields) {
    for_alist(DefExpr, formalDef, fn->formals) {
      if (ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym)) {
        if (!strcmp(formal->name, field->name)) {
          Expr* lhs = new MemberAccess(new SymExpr(fn->_this), field);
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

  fn->insertAtTail(new ReturnStmt(new SymExpr(fn->_this)));
  fn->retType = ct;
}


static void build_getter(ClassType* ct, Symbol *field) {
  FnSymbol* fn = new FnSymbol(field->name);
  fn->addPragma("inline");
  fn->_getter = field;
  fn->retType = field->type;
  fn->formals = new AList<DefExpr>();
  fn->body = new BlockStmt(new ReturnStmt(new SymExpr(field->name)));
  DefExpr* def = new DefExpr(fn);
  if (no_infer && field->defPoint->exprType)
    def->exprType = field->defPoint->exprType->copy();
  ct->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
  reset_file_info(fn, field->lineno, field->filename);
  ct->methods.add(fn);
  fn->method_type = PRIMARY_METHOD;
  fn->typeBinding = ct->symbol;
  fn->noParens = true;
}


static void build_setter(ClassType* ct, Symbol* field) {
  FnSymbol* fn = new FnSymbol(field->name);
  fn->addPragma("inline");
  fn->_setter = field;
  fn->retType = dtVoid;

  fn->formals = new AList<DefExpr>(new DefExpr(new ArgSymbol(INTENT_REF, "_setterTokenDummy", dtSetterToken)));
  ArgSymbol* fieldArg = new ArgSymbol(INTENT_BLANK, "_arg", (no_infer) ? field->type : dtUnknown);
  DefExpr* argDef = new DefExpr(fieldArg);
  if (no_infer && field->defPoint->exprType)
    argDef->exprType = field->defPoint->exprType->copy();
  fn->formals->insertAtTail(argDef);
  fn->body->insertAtTail(new ExprStmt(new CallExpr(OP_GETSNORM, field, fieldArg)));

  ct->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(new DefExpr(fn)));
  reset_file_info(fn, field->lineno, field->filename);

  ct->methods.add(fn);
  fn->method_type = PRIMARY_METHOD;
  fn->typeBinding = ct->symbol;
  fn->noParens = true;
}


static void build_setters_and_getters(ClassType* ct) {
  forv_Vec(Symbol, field, ct->fields) {
    build_setter(ct, field);
    build_getter(ct, field);
  }
  forv_Vec(TypeSymbol, tmp, ct->types) {
    if (tmp->type->astType == TYPE_USER || 
        (fnewvardef && tmp->type->astType == TYPE_VARIABLE))
      build_getter(ct, tmp);
  }
}


static void build_record_equality_function(ClassType* ct) {
  FnSymbol* fn = new FnSymbol("==");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(arg1));
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", ct);
  args->insertAtTail(new DefExpr(arg2));
  Symboltable::continueFnDef(fn, args, dtBoolean);
  Expr* cond = NULL;
  forv_Vec(Symbol, tmp, ct->fields) {
    Expr* left = new MemberAccess(new SymExpr(arg1), tmp);
    Expr* right = new MemberAccess(new SymExpr(arg2), tmp);
    cond = (cond)
      ? new CallExpr(OP_LOGAND, cond, new CallExpr(OP_EQUAL, left, right))
      : new CallExpr(OP_EQUAL, left, right);
  }
  BlockStmt* retStmt = new BlockStmt(new ReturnStmt(cond));
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, retStmt));
  ct->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
}


static void build_record_inequality_function(ClassType* ct) {
  FnSymbol* fn = new FnSymbol("!=");

  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(arg1));
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", ct);
  args->insertAtTail(new DefExpr(arg2));
  Symboltable::continueFnDef(fn, args, dtBoolean);
  Expr* cond = NULL;
  forv_Vec(Symbol, tmp, ct->fields) {
    Expr* left = new MemberAccess(new SymExpr(arg1), tmp);
    Expr* right = new MemberAccess(new SymExpr(arg2), tmp);
    cond = (cond)
      ? new CallExpr(OP_LOGOR, cond, new CallExpr(OP_NEQUAL, left, right))
      : new CallExpr(OP_NEQUAL, left, right);
  }
  BlockStmt* retStmt = new BlockStmt(new ReturnStmt(cond));
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, retStmt));
  ct->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
}


static void build_record_assignment_function(ClassType* ct) {
  if (function_exists("=", 2, NULL, ct->symbol->name))
    return;

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("="));
  ArgSymbol* _arg1 = 
    f_equal_method ? new ArgSymbol(INTENT_REF, "this", ct)
    : new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(_arg1));
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtUnknown);
  args->insertAtTail(new DefExpr(arg2));
  Symboltable::continueFnDef(fn, args, dtUnknown);
  AList<Stmt>* body = new AList<Stmt>();
  forv_Vec(Symbol, tmp, ct->fields) {
    Expr* left = new MemberAccess(new SymExpr(_arg1), tmp);
    Expr* right = new MemberAccess(new SymExpr(arg2), tmp);
    Expr* assign_expr = new CallExpr(OP_GETSNORM, left, right);
    body->insertAtTail(new ExprStmt(assign_expr));
  }

  body->insertAtTail(new ReturnStmt(new SymExpr(_arg1)));
  BlockStmt* block_stmt = new BlockStmt(body);
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, block_stmt));
  ct->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  if (f_equal_method) {
    ct->methods.add(fn);
    fn->method_type = PRIMARY_METHOD;
    fn->typeBinding = ct->symbol;
    fn->_this = _arg1;
  }
  if (no_infer)
    fn->retType = ct;
}


static void buildDefaultClassTypeMethods(ClassType* ct) {
  build_setters_and_getters(ct);
  build_init(ct);
  build_constructor(ct);
  if (ct->classTag == CLASS_RECORD || ct->classTag == CLASS_VALUECLASS) {
    build_record_equality_function(ct);
    build_record_inequality_function(ct);
    build_record_assignment_function(ct);
  }
}


static void buildDefaultIOFunctions(Type* type) {
  if (type->hasDefaultWriteFunction()) {
    if (!function_exists("fwrite", 2, NULL, "file", type->symbol->name)) {
      FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("fwrite"));
      fn->cname = stringcat("_auto_", type->symbol->name, "_fwrite");
      TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(Symboltable::lookupInFileModuleScope("file"));
      ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", fileType->definition);
      ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "val", type);
      Symboltable::continueFnDef(fn, new AList<DefExpr>(new DefExpr(fileArg), new DefExpr(arg)), dtVoid);
      AList<Stmt>* body = type->buildDefaultWriteFunctionBody(fileArg, arg);
      BlockStmt* block_stmt = new BlockStmt(body);
      DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, block_stmt));
      type->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
      reset_file_info(def, type->symbol->lineno, type->symbol->filename);
    }
  }

  if (type->hasDefaultReadFunction()) {
    if (!function_exists("read", 1, NULL, type->symbol->name)) {
      FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("read"));
      fn->cname = stringcat("_auto_", type->symbol->name, "_read");
      ArgSymbol* arg = new ArgSymbol(INTENT_INOUT, "val", type);
      Symboltable::continueFnDef(fn, new AList<DefExpr>(new DefExpr(arg)), dtVoid);
      AList<Stmt>* body = type->buildDefaultReadFunctionBody(arg);
      BlockStmt* block_stmt = new BlockStmt(body);
      DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, block_stmt));
      type->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
      reset_file_info(def, type->symbol->lineno, type->symbol->filename);
    }
  }
}
