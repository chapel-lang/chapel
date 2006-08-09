#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "stringutil.h"
#include "runtime.h"


static Vec<FnSymbol*> fns;

static void build_chpl_main(void);
static void build_record_equality_function(ClassType* ct);
static void build_record_inequality_function(ClassType* ct);
static void build_record_assignment_function(ClassType* ct);
static void build_enum_assignment_function(EnumType* et);
static void build_record_copy_function(ClassType* ct);
static void build_record_init_function(ClassType* ct);
static void buildDefaultIOFunctions(Type* type);

// function_exists returns true iff
//  function's name matches name
//  function's number of formals matches numFormals if not -1
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
                                 char* formalTypeName1 = NULL,
                                 char* formalTypeName2 = NULL) {
  forv_Vec(FnSymbol, fn, fns) {
    if (strcmp(name, fn->name))
      continue;

    if (numFormals != -1)
      if (numFormals != fn->formals->length())
        continue;

    if (formalTypeName1)
      if (!type_name_match(formalTypeName1, fn->formals->get(1)->sym))
        continue;

    if (formalTypeName2)
      if (!type_name_match(formalTypeName2, fn->formals->get(2)->sym))
        continue;

    return fn;
  }
  return NULL;
}


void build_default_functions(void) {
  collect_functions(&fns);
  build_chpl_main();

  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (TypeSymbol* type = dynamic_cast<TypeSymbol*>(ast)) {
      if (type->hasPragma( "no default functions") ||
          type->type->instantiatedFrom)
        continue;
      buildDefaultIOFunctions(type->type);
      if (ClassType* ct = dynamic_cast<ClassType*>(type->type)) {
        if (ct->classTag == CLASS_RECORD) {
          build_record_equality_function(ct);
          build_record_inequality_function(ct);
          build_record_assignment_function(ct);
          build_record_init_function(ct);
          build_record_copy_function(ct);
        }
      }
      if (EnumType* et = dynamic_cast<EnumType*>(type->type)) {
        build_enum_assignment_function(et);
      }
    }
  }
}


static FnSymbol* chpl_main_exists(void) {
  FnSymbol* match = NULL;
  forv_Vec(FnSymbol, fn, fns) {
    if (!strcmp("main", fn->name) && !fn->formals->length()) {
      if (!match) {
        match = fn;
      } else {
        USR_FATAL(fn, "main multiply defined -- first occurrence at %s",
                  match->stringLoc());
      }
    }
  }
  return match;
}


static void build_chpl_main(void) {
  chpl_main = chpl_main_exists();
  if (!chpl_main) {
    if (userModules.n == 1) {
      chpl_main = new FnSymbol("main");
      chpl_main->retType = dtVoid;
      userModules.v[0]->stmts->insertAtTail(new DefExpr(chpl_main));
      build(chpl_main);
    } else
      USR_FATAL("Code defines multiple modules but no main function.");
  } else if (chpl_main->getModule() != chpl_main->defPoint->parentSymbol)
    USR_FATAL(chpl_main, "Main function must be defined at module scope");
  currentLineno = -1;
  chpl_main->insertAtHead(new CallExpr(chpl_main->getModule()->initFn));
  chpl_main->insertAtHead(new CallExpr(compilerModule->initFn));
}


static void build_record_equality_function(ClassType* ct) {
  FnSymbol* fn = new FnSymbol("==");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtAny);
  fn->formals->insertAtTail(arg1);
  fn->formals->insertAtTail(arg2);
  fn->retType = dtBool;
  Expr* cond = NULL;
  for_fields(tmp, ct) {
    Expr* left = new CallExpr(tmp->name, gMethodToken, arg1);
    Expr* right = new CallExpr(tmp->name, gMethodToken, arg2);
    cond = (cond)
      ? new CallExpr("&&", cond, new CallExpr("==", left, right))
      : new CallExpr("==", left, right);
  }
  fn->body = new BlockStmt(new ReturnStmt(cond));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->parentStmt->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  build(fn);
  fns.add(fn);
}


static void build_record_inequality_function(ClassType* ct) {
  FnSymbol* fn = new FnSymbol("!=");

  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtAny);
  fn->formals->insertAtTail(arg1);
  fn->formals->insertAtTail(arg2);
  fn->retType = dtBool;
  Expr* cond = NULL;
  for_fields(tmp, ct) {
    Expr* left = new CallExpr(tmp->name, gMethodToken, arg1);
    Expr* right = new CallExpr(tmp->name, gMethodToken, arg2);
    cond = (cond)
      ? new CallExpr("||", cond, new CallExpr("!=", left, right))
      : new CallExpr("!=", left, right);
  }
  BlockStmt* retStmt = new BlockStmt(new ReturnStmt(cond));
  fn->body = retStmt;
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->parentStmt->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  build(fn);
  fns.add(fn);
}


static void build_enum_assignment_function(EnumType* et) {
  if (function_exists("=", 2, et->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", et);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", et);
  fn->formals->insertAtTail(arg1);
  fn->formals->insertAtTail(arg2);
  fn->insertAtTail(new ReturnStmt(arg2));
  DefExpr* def = new DefExpr(fn);
  et->symbol->defPoint->parentStmt->insertBefore(def);
  reset_file_info(def, et->symbol->lineno, et->symbol->filename);
  build(fn);
  fns.add(fn);
}


static void build_record_assignment_function(ClassType* ct) {
  if (function_exists("=", 2, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtAny);
  fn->formals->insertAtTail(arg1);
  fn->formals->insertAtTail(arg2);
  fn->retType = dtUnknown;
  AList<Stmt>* body = new AList<Stmt>();
  for_fields(tmp, ct)
    if (!tmp->isTypeVariable)
      body->insertAtTail(new CallExpr(tmp->name, gMethodToken, arg1, gSetterToken,
                                      new CallExpr(tmp->name, gMethodToken, arg2)));
  body->insertAtTail(new ReturnStmt(arg1));
  BlockStmt* block_stmt = new BlockStmt(body);
  fn->body = block_stmt;
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->parentStmt->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  build(fn);
  fns.add(fn);
}


static void build_record_copy_function(ClassType* ct) {
  if (function_exists("_copy", 1, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("_copy");
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "x", ct);
  fn->formals->insertAtTail(arg);
  CallExpr* call = new CallExpr(ct->defaultConstructor->name);
  for_fields(tmp, ct)
    call->insertAtTail(new CallExpr(".", arg, new_StringLiteral(tmp->name)));
  fn->insertAtTail(new ReturnStmt(call));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->parentStmt->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  build(fn);
  fns.add(fn);
  if (ct->symbol->hasPragma("tuple"))
    fn->addPragma("tuple copy");
}


static void build_record_init_function(ClassType* ct) {
  if (function_exists("_init", 1, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("_init");
  fn->addPragma("inline");
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "x", ct);
  fn->formals->insertAtTail(arg);
  CallExpr* call = new CallExpr(ct->defaultConstructor->name);
  for_fields(tmp, ct) {
    VarSymbol* var = dynamic_cast<VarSymbol*>(tmp);
    if (var->consClass == VAR_PARAM || var->isTypeVariable)
      call->insertAtTail(new NamedExpr(tmp->name, new CallExpr(".", arg, new_StringLiteral(tmp->name))));
  }
  fn->insertAtTail(new ReturnStmt(call));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->parentStmt->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  build(fn);
  fns.add(fn);
  if (ct->symbol->hasPragma("tuple"))
    fn->addPragma("tuple init");
}


void buildDefaultIOFunctions(Type* type) {
  if (fnostdincs)
    return;
  if (type->hasDefaultWriteFunction()) {
    if (!function_exists("fwrite", 2, "file", type->symbol->name)) {
      FnSymbol* fn = new FnSymbol("fwrite");
      fn->cname = stringcat("_auto_", type->symbol->name, "_fwrite");
      TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(fileModule->lookup("file"));
      ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", fileType->type);
      ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "val", type);
      fn->formals->insertAtTail(fileArg);
      fn->formals->insertAtTail(arg);
      fn->retType = dtVoid;
      AList<Stmt>* body = type->buildDefaultWriteFunctionBody(fileArg, arg);
      BlockStmt* block_stmt = new BlockStmt(body);
      fn->body = block_stmt;
      DefExpr* def = new DefExpr(fn);
      type->symbol->defPoint->parentStmt->insertBefore(def);
      reset_file_info(def, type->symbol->lineno, type->symbol->filename);
      build(fn);
      fns.add(fn);
    }
  }

  if (type->hasDefaultReadFunction()) {
    if (!function_exists("fread", 2, "file", type->symbol->name)) {
      FnSymbol* fn = new FnSymbol("fread");
      fn->cname = stringcat("_auto_", type->symbol->name, "_fread");
      TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(fileModule->lookup("file"));
      ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", fileType->type);
      ArgSymbol* arg = new ArgSymbol(INTENT_INOUT, "val", type);
      fn->formals->insertAtTail(fileArg);
      fn->formals->insertAtTail(arg);
      fn->retType = dtVoid;
      AList<Stmt>* body = type->buildDefaultReadFunctionBody(fileArg, arg);
      BlockStmt* block_stmt = new BlockStmt(body);
      fn->body = block_stmt;
      DefExpr* def = new DefExpr(fn);
      type->symbol->defPoint->parentStmt->insertBefore(def);
      reset_file_info(def, type->symbol->lineno, type->symbol->filename);
      build(fn);
    }
  }
}
