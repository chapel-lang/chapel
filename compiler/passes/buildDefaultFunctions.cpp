#include "buildDefaultFunctions.h"
#include "astutil.h"
#include "expr.h"
#include "../passes/filesToAST.h"
#include "stmt.h"
#include "symtab.h"
#include "stringutil.h"
#include "filesToAST.h"


static void build_chpl_main(Vec<ModuleSymbol*>* modules);
static void build_constructor(ClassType* classType);
static void build_union_id_enum(ClassType* classType);
static void build_getter(ClassType* classType, Symbol *tmp);
static void build_setters_and_getters(ClassType* classType);
static void build_record_equality_function(ClassType* classType);
static void build_record_inequality_function(ClassType* classType);
static void build_record_assignment_function(ClassType* classType);
static void buildDefaultClassTypeMethods(ClassType* classType);
static void buildDefaultIOFunctions(Type* type);


void BuildDefaultFunctions::run(Vec<ModuleSymbol*>* modules) {
  build_chpl_main(modules);

  Vec<Symbol*> syms;
  collect_symbols(&syms);

  forv_Vec(Symbol, sym, syms) {
    if (TypeSymbol* type = dynamic_cast<TypeSymbol*>(sym)) {
      buildDefaultIOFunctions(type->definition);
      if (ClassType* ct = dynamic_cast<ClassType*>(type->definition)) {
        buildDefaultClassTypeMethods(ct);
      }
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
  Vec<FnSymbol*> fns;
  collect_functions(&fns);
  forv_Vec(FnSymbol, fn, fns) {
    if (!strcmp(fn->name, "main") && fn->formals->isEmpty()) {
      if (!chpl_main) {
        chpl_main = fn;
      } else {
        USR_FATAL(fn, "main multiply defined -- first occurrence at %s",
                  chpl_main->stringLoc());
      }
    }
  }

  BlockStmt* mainBody;
  ModuleSymbol* mainModule;
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


static void build_constructor(ClassType* classType) {
  Symbol* tmp = Symboltable::lookupInScope("initialize", classType->symbol->parentScope);
  while (tmp) {
    if (FnSymbol* userDefaultFn = dynamic_cast<FnSymbol*>(tmp)) {
      if (userDefaultFn->retType == classType) {
        classType->defaultConstructor = userDefaultFn;
        return;
      }
    }
    tmp = tmp->overload;
  }
  char* name = stringcat("_construct_", classType->symbol->name);
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol(name));
  classType->defaultConstructor = fn;
  fn->fnClass = FN_CONSTRUCTOR;
  fn->cname = stringcat("_construct_", classType->symbol->cname);

  AList<DefExpr>* args = new AList<DefExpr>();

  forv_Vec(TypeSymbol, tmp, classType->types) {
    if (VariableType *tv = dynamic_cast<VariableType*>(tmp->definition)) {
      char* name = tmp->name;
      Type* type = tv->type;
      ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, name, type);
      arg->isGeneric = true;
      arg->genericSymbol = dynamic_cast<TypeSymbol*>(tv->symbol);
      args->insertAtTail(new DefExpr(arg, new SymExpr(dtUnknown->symbol)));
    }
  }

  forv_Vec(Symbol, tmp, classType->fields) {
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

  Symboltable::continueFnDef(fn, args, classType);

  AList<Stmt>* stmts = new AList<Stmt>;
  fn->_this = new VarSymbol("this", classType);
  dynamic_cast<VarSymbol*>(fn->_this)->noDefaultInit = true;

  DefExpr* def_expr = new DefExpr(fn->_this);
  stmts->insertAtTail(new ExprStmt(def_expr));
  char* description = stringcat("instance of class ", classType->symbol->name);
  Expr* alloc_rhs = new CallExpr(Symboltable::lookupInternal("_chpl_alloc"),
                                 new SymExpr(classType->symbol),
                                 new_StringLiteral(description));
  Expr* alloc_lhs = new SymExpr(fn->_this);
  Expr* alloc_expr = new CallExpr(OP_GETSNORM, alloc_lhs, alloc_rhs);
  Stmt* alloc_stmt = new ExprStmt(alloc_expr);
  stmts->insertAtTail(alloc_stmt);
  classType->buildConstructorBody(stmts, fn->_this, args);

  stmts->insertAtTail(new ReturnStmt(new SymExpr(fn->_this)));
  BlockStmt* body = new BlockStmt(stmts);
  DefExpr* fn_def = new DefExpr(Symboltable::finishFnDef(fn, body));
  reset_file_info(fn_def, classType->symbol->lineno, classType->symbol->filename);
  classType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(fn_def));
  classType->methods.add(fn);
  if (classType->symbol->hasPragma("codegen data")) {
    fn->addPragma("rename _data_construct");
    fn->addPragma("keep types");
  }
}


static void build_union_id_enum(ClassType* classType) {
  if (classType->classTag == CLASS_UNION) {
    classType->buildFieldSelector();
  }
}


static void build_getter(ClassType* classType, Symbol *tmp) {
  FnSymbol* getter_fn = Symboltable::startFnDef(new FnSymbol(tmp->name));
  getter_fn->addPragma("inline");
  getter_fn->cname =
    stringcat("_", classType->symbol->cname, "_get_", tmp->cname);
  getter_fn->_getter = tmp;
  ArgSymbol* getter_this = new ArgSymbol(INTENT_REF, "this", classType);
  AList<DefExpr>* getter_args = new AList<DefExpr>(new DefExpr(getter_this));
  getter_args->insertAtHead(new DefExpr(new ArgSymbol(INTENT_REF, "_methodTokenDummy", 
                                                      dtMethodToken)));
  Symboltable::continueFnDef(getter_fn, getter_args, tmp->type);
  Expr* getter_expr = new MemberAccess(new SymExpr(getter_this), tmp);
  BlockStmt* getter_return = new BlockStmt(new ReturnStmt(getter_expr));
  DefExpr* getter_def_expr = 
    new DefExpr(Symboltable::finishFnDef(getter_fn, getter_return));
  classType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(getter_def_expr));
  reset_file_info(getter_def_expr, tmp->lineno, tmp->filename);
  classType->methods.add(getter_fn);
  getter_fn->method_type = PRIMARY_METHOD;
  getter_fn->typeBinding = classType->symbol;
  getter_fn->_this = getter_this;
  getter_fn->noParens = true;
}

static void build_setters_and_getters(ClassType* classType) {
  forv_Vec(Symbol, tmp, classType->fields) {
    char* setter_name = tmp->name;
    FnSymbol* setter_fn = Symboltable::startFnDef(new FnSymbol(setter_name));
    setter_fn->addPragma("inline");
    setter_fn->cname = stringcat("_", classType->symbol->name, "_set_", tmp->name);
    setter_fn->_setter = tmp;
    ArgSymbol* setter_this = new ArgSymbol(INTENT_REF, "this", classType);
    AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(setter_this));
    args->insertAtHead(new DefExpr(new ArgSymbol(INTENT_REF, "_methodTokenDummy", 
                                                 dtMethodToken)));
    args->insertAtTail(new DefExpr(new ArgSymbol(INTENT_REF, "_setterTokenDummy", 
                                                 dtSetterToken)));

    ArgSymbol* setter_arg = new ArgSymbol(INTENT_BLANK, "_arg", dtUnknown);
    args->insertAtTail(new DefExpr(setter_arg));
    Symboltable::continueFnDef(setter_fn, args, dtVoid);
    Expr* setter_lhs = new MemberAccess(new SymExpr(setter_this), tmp);
    Expr* setter_rhs = new SymExpr(setter_arg);
    Expr* setter_assignment = new CallExpr(OP_GETSNORM, setter_lhs, setter_rhs);
    BlockStmt* setter_stmt = new BlockStmt(new ExprStmt(setter_assignment));
    DefExpr* setter_def_expr = new DefExpr(
      Symboltable::finishFnDef(setter_fn, setter_stmt));
    classType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(setter_def_expr));
    reset_file_info(setter_def_expr, tmp->lineno, tmp->filename);
    classType->methods.add(setter_fn);
    setter_fn->method_type = PRIMARY_METHOD;
    setter_fn->typeBinding = classType->symbol;
    setter_fn->_this = setter_this;
    setter_fn->noParens = true;

    build_getter(classType, tmp);
  }
  forv_Vec(TypeSymbol, tmp, classType->types) {
    if (tmp->type->astType == TYPE_USER || 
        (fnewvardef && tmp->type->astType == TYPE_VARIABLE))
      build_getter(classType, tmp);
  }
}


static void build_record_equality_function(ClassType* classType) {
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("=="));
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", classType);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(arg1));
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", classType);
  args->insertAtTail(new DefExpr(arg2));
  Symboltable::continueFnDef(fn, args, dtBoolean);
  Expr* cond = NULL;
  forv_Vec(Symbol, tmp, classType->fields) {
    Expr* left = new MemberAccess(new SymExpr(arg1), tmp);
    Expr* right = new MemberAccess(new SymExpr(arg2), tmp);
    cond = (cond)
      ? new CallExpr(OP_LOGAND, cond, new CallExpr(OP_EQUAL, left, right))
      : new CallExpr(OP_EQUAL, left, right);
  }
  BlockStmt* retStmt = new BlockStmt(new ReturnStmt(cond));
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, retStmt));
  classType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
  reset_file_info(def, classType->symbol->lineno, classType->symbol->filename);
}


static void build_record_inequality_function(ClassType* classType) {
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("!="));

  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", classType);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(arg1));
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", classType);
  args->insertAtTail(new DefExpr(arg2));
  Symboltable::continueFnDef(fn, args, dtBoolean);
  Expr* cond = NULL;
  forv_Vec(Symbol, tmp, classType->fields) {
    Expr* left = new MemberAccess(new SymExpr(arg1), tmp);
    Expr* right = new MemberAccess(new SymExpr(arg2), tmp);
    cond = (cond)
      ? new CallExpr(OP_LOGOR, cond, new CallExpr(OP_NEQUAL, left, right))
      : new CallExpr(OP_NEQUAL, left, right);
  }
  BlockStmt* retStmt = new BlockStmt(new ReturnStmt(cond));
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, retStmt));
  classType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
  reset_file_info(def, classType->symbol->lineno, classType->symbol->filename);
}


static void build_record_assignment_function(ClassType* classType) {
  Vec<FnSymbol*> fns;
  collect_functions(&fns);

  forv_Vec(FnSymbol, fn, fns) {
    if (!strcmp(fn->name, "="))
      if (fn->formals->first()->sym->type == classType)
        return;
  }

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("="));
  ArgSymbol* _arg1 = 
    f_equal_method ? new ArgSymbol(INTENT_REF, "this", classType)
    : new ArgSymbol(INTENT_BLANK, "_arg1", classType);
  AList<DefExpr>* args = new AList<DefExpr>(new DefExpr(_arg1));
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtUnknown);
  args->insertAtTail(new DefExpr(arg2));
  Symboltable::continueFnDef(fn, args, dtUnknown);
  AList<Stmt>* body = new AList<Stmt>();
  forv_Vec(Symbol, tmp, classType->fields) {
    Expr* left = new MemberAccess(new SymExpr(_arg1), tmp);
    Expr* right = new MemberAccess(new SymExpr(arg2), tmp);
    Expr* assign_expr = new CallExpr(OP_GETSNORM, left, right);
    body->insertAtTail(new ExprStmt(assign_expr));
  }

  body->insertAtTail(new ReturnStmt(new SymExpr(_arg1)));
  BlockStmt* block_stmt = new BlockStmt(body);
  DefExpr* def = new DefExpr(Symboltable::finishFnDef(fn, block_stmt));
  classType->symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def));
  reset_file_info(def, classType->symbol->lineno, classType->symbol->filename);
  if (f_equal_method) {
    classType->methods.add(fn);
    fn->method_type = PRIMARY_METHOD;
    fn->typeBinding = classType->symbol;
    fn->_this = _arg1;
  }
}


static void buildDefaultClassTypeMethods(ClassType* classType) {
  build_setters_and_getters(classType);
  build_union_id_enum(classType);
  build_constructor(classType);
  if (classType->classTag == CLASS_RECORD ||
      classType->classTag == CLASS_VALUECLASS) {
    build_record_equality_function(classType);
    build_record_inequality_function(classType);
    build_record_assignment_function(classType);
  }
}


static void buildDefaultIOFunctions(Type* type) {
  if (type->hasDefaultWriteFunction()) {
    bool userWriteDefined = false;
    Symbol* fwrite = Symboltable::lookupInScope("fwrite", type->symbol->parentScope);
    TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(Symboltable::lookupInFileModuleScope("file"));
    while (fwrite) {
      if (fwrite->getFnSymbol() && 
          fwrite->getFnSymbol()->formals->length() == 2 &&
          fwrite->getFnSymbol()->formals->first()->sym->type == fileType->definition &&
          fwrite->getFnSymbol()->formals->get(2)->sym->type == type) {
        userWriteDefined = true;
        fwrite->cname = stringcat("_user_", type->symbol->name, "_fwrite");
        break;
      }
      fwrite = fwrite->overload;
    }
    if (!userWriteDefined) {
      FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("fwrite"));
      fn->cname = stringcat("_auto_", type->symbol->name, "_fwrite");
      if (fileType == NULL) {
        INT_FATAL("Couldn't find file");
      }
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
    bool userReadDefined = false;
    Symbol* read = Symboltable::lookupInScope("read", type->symbol->parentScope);
    while (read) {
      if (read->getFnSymbol() && read->getFnSymbol()->formals->only()->sym->type == type) {
        userReadDefined = true;
        read->cname = stringcat("_user_", type->symbol->name, "_read");
        break;
      }
      read = read->overload;
    }
    if (!userReadDefined) {
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
