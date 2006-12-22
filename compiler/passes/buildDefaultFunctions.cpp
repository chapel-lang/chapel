#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"


static void build_chpl_main(void);
static void build_getter(ClassType* ct, Symbol* field);
static void build_setter(ClassType* ct, Symbol* field);
static void build_union_assignment_function(ClassType* ct);
static void build_enum_assignment_function(EnumType* et);
static void build_record_assignment_function(ClassType* ct);
static void build_record_copy_function(ClassType* ct);
static void build_record_hash_function(ClassType* ct);
static void build_record_equality_function(ClassType* ct);
static void build_record_inequality_function(ClassType* ct);
static void build_record_init_function(ClassType* ct);
static void build_union_assignment_function(ClassType* ct);

static void buildDefaultReadFunction(ClassType* type);
static void buildDefaultReadFunction(EnumType* type);

static void buildDefaultWriteFunction(ClassType* type);
static void buildDefaultWriteFunction(EnumType* type);



void build_default_functions(void) {
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
      if (type->hasPragma("no default functions"))
        continue;
      if (!fnostdincs) {
        if (EnumType* et = dynamic_cast<EnumType*>(type->type)) {
          buildDefaultReadFunction(et);
          buildDefaultWriteFunction(et);
        } else if (ClassType* ct = dynamic_cast<ClassType*>(type->type)) {
          buildDefaultReadFunction(ct);
          buildDefaultWriteFunction(ct);
        }
      }
      if (ClassType* ct = dynamic_cast<ClassType*>(type->type)) {
        if (ct->classTag == CLASS_RECORD) {
          if (!(ct->symbol->hasPragma("domain") ||
                ct->symbol->hasPragma("array") ||
                ct->symbol->hasPragma("seq"))) {
            build_record_equality_function(ct);
            build_record_inequality_function(ct);
          }
          build_record_assignment_function(ct);
          build_record_init_function(ct);
          build_record_copy_function(ct);
          build_record_hash_function(ct);
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
  forv_Vec(FnSymbol, fn, gFns) {
    if (strcmp(name, fn->name))
      continue;

    if (numFormals != -1)
      if (numFormals != fn->formals->length())
        continue;

    if (formalTypeName1)
      if (!type_name_match(formalTypeName1, fn->getFormal(1)))
        continue;

    if (formalTypeName2)
      if (!type_name_match(formalTypeName2, fn->getFormal(2)))
        continue;

    if (formalTypeName3)
      if (!type_name_match(formalTypeName3, fn->getFormal(3)))
        continue;

    return fn;
  }
  return NULL;
}


static void build_getter(ClassType* ct, Symbol *field) {
  if (FnSymbol* fn = function_exists(field->name, 2, dtMethodToken->symbol->name, ct->symbol->name)) {
    Vec<BaseAST*> asts;
    collect_asts(&asts, fn);
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
        if (call->isNamed(field->name) && call->argList->length() == 2) {
          if (call->get(1)->typeInfo() == dtMethodToken &&
              call->get(2)->typeInfo() == ct) {
            Expr* arg2 = call->get(2);
            call->replace(new CallExpr(PRIMITIVE_GET_MEMBER, arg2->remove(), new_StringSymbol(field->name)));
          }
        }
      }
    }
    return;
  }

  FnSymbol* fn = new FnSymbol(field->name);
  fn->defSetGet = true;
  fn->addPragma("inline");
  if (ct->symbol->hasPragma( "synchronization primitive")) 
    fn->addPragma( "synchronization primitive");
  ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", ct);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(_this);
  if (ct->classTag == CLASS_UNION)
    fn->insertAtTail(new CondStmt(new CallExpr("!", new CallExpr(PRIMITIVE_UNION_GETID, _this, new_IntSymbol(field->id))), new CallExpr("halt", new_StringSymbol("illegal union access"))));
  fn->insertAtTail(new ReturnStmt(new CallExpr(PRIMITIVE_GET_MEMBER, new SymExpr(_this), new SymExpr(new_StringSymbol(field->name)))));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_file_info(fn, field->lineno, field->filename);
  normalize(fn);
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
            call->replace(new CallExpr(PRIMITIVE_SET_MEMBER, arg2->remove(), new_StringSymbol(field->name), arg4->remove()));
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
  if (ct->symbol->hasPragma( "synchronization primitive")) 
    fn->addPragma( "synchronization primitive");

  ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", ct);
  ArgSymbol* fieldArg = new ArgSymbol(INTENT_BLANK, "_arg", dtAny);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(_this);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_st", dtSetterToken));
  fn->insertFormalAtTail(fieldArg);
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
  ct->symbol->defPoint->insertBefore(new DefExpr(fn));
  reset_file_info(fn, field->lineno, field->filename);
  normalize(fn);
  ct->methods.add(fn);
  fn->isMethod = true;
  fn->cname = stringcat("_", ct->symbol->cname, "_", fn->cname);
  fn->noParens = true;
  fn->_this = _this;
}


static FnSymbol* chpl_main_exists(void) {
  FnSymbol* match = NULL;
  forv_Vec(FnSymbol, fn, gFns) {
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
      normalize(chpl_main);
    } else
      USR_FATAL("Code defines multiple modules but no main function.");
  } else if (chpl_main->getModule() != chpl_main->defPoint->parentSymbol)
    USR_FATAL(chpl_main, "Main function must be defined at module scope");
  currentLineno = -1;
  chpl_main->insertAtHead(new CallExpr(chpl_main->getModule()->initFn));
  chpl_main->insertAtHead(new CallExpr(compilerModule->initFn));
}


static void build_record_equality_function(ClassType* ct) {
  if (function_exists("==", 2, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("==");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtAny);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->retType = dtBool;
  for_fields(tmp, ct) {
    Expr* left = new CallExpr(tmp->name, gMethodToken, arg1);
    Expr* right = new CallExpr(tmp->name, gMethodToken, arg2);
    fn->insertAtTail(new CondStmt(new CallExpr("!=", left, right), new ReturnStmt(new SymExpr(gFalse))));
  }
  fn->insertAtTail(new ReturnStmt(new SymExpr(gTrue)));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  normalize(fn);
}


static void build_record_inequality_function(ClassType* ct) {
  if (function_exists("!=", 2, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("!=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtAny);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->retType = dtBool;
  for_fields(tmp, ct) {
    Expr* left = new CallExpr(tmp->name, gMethodToken, arg1);
    Expr* right = new CallExpr(tmp->name, gMethodToken, arg2);
    fn->insertAtTail(new CondStmt(new CallExpr("!=", left, right), new ReturnStmt(new SymExpr(gTrue))));
  }
  fn->insertAtTail(new ReturnStmt(new SymExpr(gFalse)));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  normalize(fn);
}


static void build_enum_assignment_function(EnumType* et) {
  if (function_exists("=", 2, et->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", et);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", et);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->insertAtTail(new ReturnStmt(arg2));
  DefExpr* def = new DefExpr(fn);
  et->symbol->defPoint->insertBefore(def);
  reset_file_info(def, et->symbol->lineno, et->symbol->filename);
  normalize(fn);
}


static void build_record_assignment_function(ClassType* ct) {
  if (function_exists("=", 2, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtAny);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->retType = dtUnknown;
  for_fields(tmp, ct)
    if (!tmp->isTypeVariable)
      fn->insertAtTail(new CallExpr("=", new CallExpr(".", arg1, new_StringSymbol(tmp->name)), new CallExpr(".", arg2, new_StringSymbol(tmp->name))));
  fn->insertAtTail(new ReturnStmt(arg1));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  normalize(fn);
}


static void build_union_assignment_function(ClassType* ct) {
  if (function_exists("=", 2, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", ct);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->retType = dtUnknown;
  fn->insertAtTail(new CallExpr(PRIMITIVE_UNION_SETID, arg1, new_IntSymbol(0)));
  for_fields(tmp, ct)
    if (!tmp->isTypeVariable)
      fn->insertAtTail(new CondStmt(new CallExpr(PRIMITIVE_UNION_GETID, arg2, new_IntSymbol(tmp->id)), new CallExpr("=", new CallExpr(".", arg1, new_StringSymbol(tmp->name)), new CallExpr(".", arg2, new_StringSymbol(tmp->name)))));
  fn->insertAtTail(new ReturnStmt(arg1));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  normalize(fn);
}


static void build_record_copy_function(ClassType* ct) {
  if (function_exists("_copy", 1, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("_copy");
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "x", ct);
  fn->insertFormalAtTail(arg);
  CallExpr* call = new CallExpr(ct->defaultConstructor->name);
  for_fields(tmp, ct)
    call->insertAtTail(new CallExpr(".", arg, new_StringSymbol(tmp->name)));
  fn->insertAtTail(new ReturnStmt(call));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  normalize(fn);
  if (ct->symbol->hasPragma("tuple"))
    fn->addPragma("tuple copy");
}


static void build_record_init_function(ClassType* ct) {
  if (function_exists("_init", 1, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("_init");
  fn->addPragma("inline");
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "x", ct);
  fn->insertFormalAtTail(arg);
  CallExpr* call = new CallExpr(ct->defaultConstructor->name);
  for_fields(tmp, ct) {
    VarSymbol* var = dynamic_cast<VarSymbol*>(tmp);
    if (var->consClass == VAR_PARAM || var->isTypeVariable)
      call->insertAtTail(new NamedExpr(tmp->name, new CallExpr(".", arg, new_StringSymbol(tmp->name))));
  }
  fn->insertAtTail(new ReturnStmt(call));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  normalize(fn);
  if (ct->symbol->hasPragma("tuple"))
    fn->addPragma("tuple init");
}


static void build_record_hash_function(ClassType *ct) {
  if (function_exists("_indefinite_hash", 1, ct->symbol->name))
    return;

  FnSymbol *fn = new FnSymbol("_indefinite_hash");
  fn->addPragma("inline");
  ArgSymbol *arg = new ArgSymbol(INTENT_BLANK, "r", ct);
  fn->insertFormalAtTail(arg);

  if (ct->fields->length() < 0) {
    fn->insertAtTail(new ReturnStmt(new_IntSymbol(0)));
  } else {
    CallExpr *call;
    bool first = true;
    for_fields(field, ct) {
      CallExpr *field_access = new CallExpr(field->name, gMethodToken, arg); 
      if (first) {
        call = new CallExpr("_indefinite_hash", field_access);
        first = false;
      } else {
        call = new CallExpr("^", 
                            new CallExpr("_indefinite_hash",
                                         field_access),
                            new CallExpr("<<",
                                         call,
                                         new_IntSymbol(17)));
      }
    }
    fn->insertAtTail(new ReturnStmt(call));
  }
  DefExpr *def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  normalize(fn);
  if (ct->symbol->hasPragma("tuple"))
    fn->addPragma("tuple hash function");
}


static void buildDefaultReadFunction(ClassType* ct) {
  if (function_exists("read", 3, dtMethodToken->symbol->name, "file", ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("read");
  fn->cname = stringcat("_auto_", ct->symbol->name, "_read");
  TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(fileModule->lookup("file"));
  ArgSymbol* arg = new ArgSymbol(INTENT_INOUT, "x", ct);
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", fileType->type);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(fn->_this);
  fn->insertFormalAtTail(arg);
  fn->retType = dtVoid;

  Symbol* ignoreWhiteSpace = new VarSymbol("ignoreWhiteSpace");
  fn->insertAtTail(new DefExpr(ignoreWhiteSpace, new SymExpr(gTrue)));
  Symbol* matchingCharWasRead = new VarSymbol("matchingCharWasRead");
  fn->insertAtTail(new DefExpr(matchingCharWasRead, new_IntSymbol((int64)0)));
  Expr* fileArgFP = buildDot(fn->_this, "_fp");
  CallExpr* readOpenBrace = new CallExpr("_readLitChar", fileArgFP, new_StringSymbol("{"), ignoreWhiteSpace);
  fn->insertAtTail(new CallExpr("=", matchingCharWasRead, readOpenBrace));
  CallExpr* notRead = new CallExpr("==", matchingCharWasRead, new_IntSymbol(0));
  Expr* readError = new CallExpr("halt", new_StringSymbol("Read of the class failed: "), new CallExpr("_get_errno"));
  CondStmt* readErrorCond = new CondStmt(notRead, readError);
  fn->insertAtTail(readErrorCond);
  bool first = true;
  for_fields(tmp, ct) {
    if (tmp->isTypeVariable)
      continue;
    if (!first) {
      CallExpr* readComma = new CallExpr("_readLitChar", fileArgFP->copy(), new_StringSymbol(","), ignoreWhiteSpace);
      fn->insertAtTail(new CallExpr("=", matchingCharWasRead, readComma));
      fn->insertAtTail(readErrorCond->copy());
    }  
    Symbol* fieldName = new VarSymbol("fieldName");
    fn->insertAtTail(new DefExpr(fieldName, new_StringSymbol("")));
    CallExpr* readFieldName = new CallExpr(buildDot(fn->_this, "read"), fieldName);
    fn->insertAtTail(readFieldName);
    Symbol* name = new_StringSymbol(tmp->name);
    Expr* confirmFieldName = new CallExpr("!=", fieldName, name);
    CondStmt* fieldNameCond = new CondStmt(confirmFieldName, readError->copy());
    fn->insertAtTail(fieldNameCond);
    CallExpr* readEqualSign = new CallExpr("_readLitChar", fileArgFP->copy(), new_StringSymbol("="), ignoreWhiteSpace);
    fn->insertAtTail(new CallExpr("=", matchingCharWasRead, readEqualSign));
    fn->insertAtTail(readErrorCond->copy());
    CallExpr* argName = new CallExpr(".", arg, name);
    CallExpr* readValue = new CallExpr(buildDot(fn->_this, "read"), argName);
    fn->insertAtTail(readValue);
    first = false;
  }
  CallExpr* readCloseBrace = new CallExpr("_readLitChar", fileArgFP->copy(), new_StringSymbol("}"), ignoreWhiteSpace);
  fn->insertAtTail(new CallExpr("=", matchingCharWasRead, readCloseBrace));
  fn->insertAtTail(readErrorCond->copy());

  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  ct->methods.add(fn);
  fn->isMethod = true;
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  normalize(fn);
  ct->methods.add(fn);
}


static void buildDefaultReadFunction(EnumType* et) {
  if (function_exists("read", 3, dtMethodToken->symbol->name, "file", et->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("read");
  fn->cname = stringcat("_auto_", et->symbol->name, "_read");
  TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(fileModule->lookup("file"));
  ArgSymbol* arg = new ArgSymbol(INTENT_INOUT, "x", et);
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", fileType->type);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(fn->_this);
  fn->insertFormalAtTail(arg);
  fn->retType = dtVoid;

  Symbol* valString = new VarSymbol("valString");
  fn->insertAtTail(new DefExpr(valString, new_StringSymbol("")));
  fn->insertAtTail(new CallExpr(buildDot(fn->_this, "read"), valString));
  Expr* elseStmt = new CallExpr("halt", 
                                new_StringSymbol("***Error: Not of "), 
                                new_StringSymbol(et->symbol->name), 
                                new_StringSymbol(" type***"));
  for_alist_backward(DefExpr, constant, et->constants) {
    Expr* cond = new CallExpr("==", valString, new_StringSymbol(constant->sym->name));
    Expr* thenStmt = new CallExpr("=", arg, constant->sym);
    elseStmt = new CondStmt(cond, thenStmt, elseStmt);
    
  }
  fn->insertAtTail(elseStmt);

  DefExpr* def = new DefExpr(fn);
  et->symbol->defPoint->insertBefore(def);
  et->methods.add(fn);
  fn->isMethod = true;
  reset_file_info(def, et->symbol->lineno, et->symbol->filename);
  normalize(fn);
  et->methods.add(fn);
}


static void buildDefaultWriteFunction(ClassType* ct) {
  if (function_exists("write", 3, dtMethodToken->symbol->name, ct->symbol->name, "file"))
    return;

  FnSymbol* fn = new FnSymbol("write");
  fn->cname = stringcat("_auto_", ct->symbol->name, "_write");
  TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(fileModule->lookup("file"));
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", ct);
  ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", fileType->type);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(fn->_this);
  fn->insertFormalAtTail(fileArg);
  fn->retType = dtVoid;

  if (ct->classTag == CLASS_CLASS) {
    BlockStmt* fwriteNil = new BlockStmt();
    fwriteNil->insertAtTail(new CallExpr(buildDot(fileArg, "write"), new_StringSymbol("nil")));
    fwriteNil->insertAtTail(new ReturnStmt());
    fn->insertAtTail(new CondStmt(new CallExpr("==", fn->_this, gNil),
                                    fwriteNil));
  }

  if (ct->classTag == CLASS_CLASS) {
    fn->insertAtTail(new CallExpr(buildDot(fileArg, "write"), new_StringSymbol("{")));
  } else {
    fn->insertAtTail(new CallExpr(buildDot(fileArg, "write"), new_StringSymbol("(")));
  }

  if (ct->classTag == CLASS_UNION) {
    CondStmt* cond = NULL;
    for_fields(tmp, ct) {
      BlockStmt* writeFieldBlock = new BlockStmt();
      writeFieldBlock->insertAtTail(new CallExpr(buildDot(fileArg, "write"), new_StringSymbol(tmp->name)));
      writeFieldBlock->insertAtTail(new CallExpr(buildDot(fileArg, "write"), new_StringSymbol(" = ")));
      writeFieldBlock->insertAtTail(new CallExpr(buildDot(fileArg, "write"), 
                                                 buildDot(fn->_this, tmp->name)));
      cond = new CondStmt(new CallExpr(PRIMITIVE_UNION_GETID, fn->_this, new_IntSymbol(tmp->id)), writeFieldBlock, cond);
    }
    fn->insertAtTail(cond);
  } else {
    bool first = true;
    for_fields(tmp, ct) {
      if (tmp->isTypeVariable)
        continue;
      if (!first) {
        fn->insertAtTail(new CallExpr(buildDot(fileArg, "write"), new_StringSymbol(", ")));
      }
      fn->insertAtTail(new CallExpr(buildDot(fileArg, "write"), new_StringSymbol(tmp->name)));
      fn->insertAtTail(new CallExpr(buildDot(fileArg, "write"), new_StringSymbol(" = ")));
      fn->insertAtTail(new CallExpr(buildDot(fileArg, "write"), 
                                      buildDot(fn->_this, tmp->name)));
      first = false;
    }
  }

  if (ct->classTag == CLASS_CLASS) {
    fn->insertAtTail(new CallExpr(buildDot(fileArg, "write"), new_StringSymbol("}")));
  } else {
    fn->insertAtTail(new CallExpr(buildDot(fileArg, "write"), new_StringSymbol(")")));
  }

  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  fn->isMethod = true;
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  normalize(fn);
  ct->methods.add(fn);
}


static void buildDefaultWriteFunction(EnumType* et) {
  if (function_exists("write", 3, dtMethodToken->symbol->name, et->symbol->name, "file"))
    return;

  FnSymbol* fn = new FnSymbol("write");
  fn->cname = stringcat("_auto_", et->symbol->name, "_write");
  TypeSymbol* fileType = dynamic_cast<TypeSymbol*>(fileModule->lookup("file"));
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", et);
  ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", fileType->type);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(fn->_this);
  fn->insertFormalAtTail(fileArg);
  fn->retType = dtVoid;

  CondStmt* body = NULL;
  for_alist(DefExpr, constant, et->constants) {
    body = new CondStmt(
             new CallExpr("==", fn->_this, constant->sym),
             new CallExpr(buildDot(fileArg, "write"),
                          new_StringSymbol(constant->sym->name)),
             body);
  }
  fn->insertAtTail(body);

  DefExpr* def = new DefExpr(fn);
  et->symbol->defPoint->insertBefore(def);
  fn->isMethod = true;
  reset_file_info(def, et->symbol->lineno, et->symbol->filename);
  normalize(fn);
  et->methods.add(fn);
}
