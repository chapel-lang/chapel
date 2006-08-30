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
static void build_union_assignment_function(ClassType* ct);
static void build_enum_assignment_function(EnumType* et);
static void build_record_copy_function(ClassType* ct);
static void build_record_init_function(ClassType* ct);
static void build_union_assignment_function(ClassType* ct);
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
                                 char* formalTypeName2 = NULL,
                                 char* formalTypeName3 = NULL) {
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

    if (formalTypeName3)
      if (!type_name_match(formalTypeName3, fn->formals->get(3)->sym))
        continue;

    return fn;
  }
  return NULL;
}


static void build_getter(ClassType* ct, Symbol *field) {
  if (function_exists(field->name, 2, dtMethodToken->symbol->name, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol(field->name);
  fn->defSetGet = true;
  fn->addPragma("inline");
  ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", ct);
  fn->formals->insertAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->formals->insertAtTail(_this);
  if (ct->classTag == CLASS_UNION)
    fn->insertAtTail(new CondStmt(new CallExpr("!", new CallExpr(PRIMITIVE_UNION_GETID, _this, new_IntSymbol(field->id))), new ExprStmt(new CallExpr("halt", new_StringLiteral("illegal union access")))));
  fn->insertAtTail(new ReturnStmt(new CallExpr(PRIMITIVE_GET_MEMBER, new SymExpr(_this), new SymExpr(new_StringSymbol(field->name)))));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->parentStmt->insertBefore(def);
  reset_file_info(fn, field->lineno, field->filename);
  build(fn);
  fns.add(fn);
  ct->methods.add(fn);
  fn->isMethod = true;
  fn->cname = stringcat("_", ct->symbol->cname, "_", fn->cname);
  fn->noParens = true;
  fn->_this = _this;
}


static void build_setter(ClassType* ct, Symbol* field) {
  if (FnSymbol* fn = function_exists(field->name, 4, dtMethodToken->symbol->name, ct->symbol->name, dtSetterToken->symbol->name)) {
    Vec<BaseAST*> asts;
    collect_asts(&asts, fn);
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
        if (call->isNamed(field->name) && call->argList->length() == 4) {
          if (call->get(1)->typeInfo() == dtMethodToken &&
              call->get(2)->typeInfo() == ct &&
              call->get(3)->typeInfo() == dtSetterToken) {
            Expr* arg2 = call->get(2);
            Expr* arg4 = call->get(4);
            call->replace(new CallExpr(PRIMITIVE_SET_MEMBER, arg2->remove(), new_StringLiteral(field->name), arg4->remove()));
          }
        }
      }
    }
    return;
  }

  FnSymbol* fn = new FnSymbol(field->name);
  fn->isSetter = true;
  fn->defSetGet = true;
  fn->addPragma("inline");
  fn->retType = dtVoid;

  ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", ct);
  ArgSymbol* fieldArg = new ArgSymbol(INTENT_BLANK, "_arg", dtAny);
  fn->formals->insertAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->formals->insertAtTail(_this);
  fn->formals->insertAtTail(new ArgSymbol(INTENT_BLANK, "_st", dtSetterToken));
  fn->formals->insertAtTail(fieldArg);
  VarSymbol* val = new VarSymbol("_tmp");
  val->isCompilerTemp = true;
  val->canReference = true;
  fn->insertAtTail(new DefExpr(val));
  if (ct->classTag == CLASS_UNION) {
    fn->insertAtTail(new CallExpr(PRIMITIVE_UNION_SETID, _this, new_IntSymbol(field->id)));
    fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, val, new CallExpr("_copy", fieldArg)));
  } else {
    fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, val, new CallExpr(PRIMITIVE_GET_MEMBER, _this, new_StringSymbol(field->name))));
    fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, val, new CallExpr("=", val, fieldArg)));
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, _this, new_StringSymbol(field->name), val));
  ct->symbol->defPoint->parentStmt->insertBefore(new DefExpr(fn));
  reset_file_info(fn, field->lineno, field->filename);
  build(fn);
  fns.add(fn);
  ct->methods.add(fn);
  fn->isMethod = true;
  fn->cname = stringcat("_", ct->symbol->cname, "_", fn->cname);
  fn->noParens = true;
  fn->_this = _this;
}


void build_default_functions(void) {
  collect_functions(&fns);
  build_chpl_main();

  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (TypeSymbol* type = dynamic_cast<TypeSymbol*>(ast)) {
      if (ClassType* ct = dynamic_cast<ClassType*>(type->type)) {
        for_fields(field, ct) {
          VarSymbol *cfield = dynamic_cast<VarSymbol*>(field);
          // if suppress for cobegin created arg classes
          if (cfield && !cfield->is_ref) {
            build_setter(ct, field);
            build_getter(ct, field);
          }
        }
      }
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
        if (ct->classTag == CLASS_UNION) {
          build_union_assignment_function(ct);
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
      body->insertAtTail(new CallExpr("=", new CallExpr(".", arg1, new_StringLiteral(tmp->name)), new CallExpr(".", arg2, new_StringLiteral(tmp->name))));
  body->insertAtTail(new ReturnStmt(arg1));
  BlockStmt* block_stmt = new BlockStmt(body);
  fn->body = block_stmt;
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->parentStmt->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  build(fn);
  fns.add(fn);
}


static void build_union_assignment_function(ClassType* ct) {
  if (function_exists("=", 2, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", ct);
  fn->formals->insertAtTail(arg1);
  fn->formals->insertAtTail(arg2);
  fn->retType = dtUnknown;
  AList<Stmt>* body = new AList<Stmt>();
  body->insertAtTail(new CallExpr(PRIMITIVE_UNION_SETID, arg1, new_IntSymbol(0)));
  for_fields(tmp, ct)
    if (!tmp->isTypeVariable)
      body->insertAtTail(new CondStmt(new CallExpr(PRIMITIVE_UNION_GETID, arg2, new_IntLiteral(tmp->id)), new ExprStmt(new CallExpr("=", new CallExpr(".", arg1, new_StringLiteral(tmp->name)), new CallExpr(".", arg2, new_StringLiteral(tmp->name))))));
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
