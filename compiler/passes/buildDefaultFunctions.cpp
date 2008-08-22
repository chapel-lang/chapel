#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"


static void build_chpl_main(void);
static void build_getter(ClassType* ct, Symbol* field);
static void build_union_assignment_function(ClassType* ct);
static void build_enum_assignment_function(EnumType* et);
static void build_record_assignment_function(ClassType* ct);
static void build_record_cast_function(ClassType* ct);
static void build_record_copy_function(ClassType* ct);
static void build_record_hash_function(ClassType* ct);
static void build_record_equality_function(ClassType* ct);
static void build_record_inequality_function(ClassType* ct);
static void build_record_init_function(ClassType* ct);
static void build_union_assignment_function(ClassType* ct);
static void build_enum_cast_function(EnumType* et);
static void build_enum_enumerate_function(EnumType* et);
static void build_enum_init_function(EnumType* et);

static void buildDefaultReadFunction(ClassType* type);
static void buildDefaultReadFunction(EnumType* type);

static void buildDefaultWriteFunction(ClassType* type);
static void buildStringCastFunction(EnumType* type);



void buildDefaultFunctions(void) {
  build_chpl_main();

  Vec<BaseAST*> asts;
  collect_asts(rootModule, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (TypeSymbol* type = toTypeSymbol(ast)) {
      if (ClassType* ct = toClassType(type->type)) {
        for_fields(field, ct) {
          if (isVarSymbol(field)) {
            if (strcmp(field->name, "_promotionType")) {
              build_getter(ct, field);
            }
          } else if (isEnumType(field->type)) {
            build_getter(ct, field);
          }
        }
      }
      if (type->hasPragma(PRAG_NO_DEFAULT_FUNCTIONS))
        continue;
      if (EnumType* et = toEnumType(type->type)) {
        buildDefaultReadFunction(et);
        buildStringCastFunction(et);
      } else if (ClassType* ct = toClassType(type->type)) {
        buildDefaultReadFunction(ct);
        buildDefaultWriteFunction(ct);
      }
      if (ClassType* ct = toClassType(type->type)) {
        if (ct->classTag == CLASS_RECORD) {
          if (!(ct->symbol->hasPragma(PRAG_DOMAIN) ||
                ct->symbol->hasPragma(PRAG_ARRAY))) {
            build_record_equality_function(ct);
            build_record_inequality_function(ct);
          }
          build_record_assignment_function(ct);
          build_record_cast_function(ct);
          build_record_init_function(ct);
          build_record_copy_function(ct);
          build_record_hash_function(ct);
        }
        if (ct->classTag == CLASS_UNION) {
          build_union_assignment_function(ct);
        }
      }
      if (EnumType* et = toEnumType(type->type)) {
        build_enum_init_function(et);
        build_enum_cast_function(et);
        build_enum_assignment_function(et);
        build_enum_enumerate_function(et);
      }
    }
  }
}


// function_exists returns true iff
//  function's name matches name
//  function's number of formals matches numFormals if not -1
//  function's first formal's type matches formalType1 if not NULL
//  function's second formal's type matches formalType2 if not NULL
//  function's third formal's type matches formalType3 if not NULL

static bool type_match(Type* type, Symbol* sym) {
  if (sym->type == type)
    return true;
  SymExpr* se = toSymExpr(sym->defPoint->exprType);
  if (se && se->var->type == type)
    return true;
  return false;
}

static FnSymbol* function_exists(const char* name,
                                 int numFormals = -1,
                                 Type* formalType1 = NULL,
                                 Type* formalType2 = NULL,
                                 Type* formalType3 = NULL) {
  forv_Vec(FnSymbol, fn, gFns) {
    if (strcmp(name, fn->name))
      continue;

    if (numFormals != -1)
      if (numFormals != fn->numFormals())
        continue;

    if (formalType1)
      if (!type_match(formalType1, fn->getFormal(1)))
        continue;

    if (formalType2)
      if (!type_match(formalType2, fn->getFormal(2)))
        continue;

    if (formalType3)
      if (!type_match(formalType3, fn->getFormal(3)))
        continue;

    return fn;
  }
  return NULL;
}


static void build_getter(ClassType* ct, Symbol *field) {
  if (FnSymbol* fn = function_exists(field->name, 2, dtMethodToken, ct)) {
    Vec<BaseAST*> asts;
    collect_asts(fn, asts);
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = toCallExpr(ast)) {
        if (call->isNamed(field->name) && call->numActuals() == 2) {
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
  fn->addPragma(PRAG_INLINE);
  fn->addPragma(PRAG_TEMP);
  if (ct->symbol->hasPragma(PRAG_SYNC)) 
    fn->addPragma(PRAG_SYNC);
  ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", ct);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(_this);
  if (field->isParameter())
    fn->retTag = RET_PARAM;
  else if (field->hasPragma(PRAG_TYPE_VARIABLE))
    fn->retTag = RET_TYPE;
  else if (field->hasPragma(PRAG_SUPER_CLASS)) {
    fn->retTag = RET_VALUE;
  } else {
    fn->retTag = RET_VAR;
    fn->setter = new DefExpr(new ArgSymbol(INTENT_BLANK, "setter", dtBool));
  }
  if (ct->classTag == CLASS_UNION)
    fn->insertAtTail(
      new CondStmt(
        new SymExpr(fn->setter->sym),
        new CallExpr(PRIMITIVE_UNION_SETID, _this, new_IntSymbol(field->id)),
        new CondStmt(
          new CallExpr("!=", 
            new CallExpr(PRIMITIVE_UNION_GETID, _this),
              new_IntSymbol(field->id)),
          new CallExpr("halt", new_StringSymbol("illegal union access")))));
  if (isTypeSymbol(field) && isEnumType(field->type)) {
    fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, field));
    // better flatten enumerated types now
    ct->symbol->defPoint->insertBefore(field->defPoint->remove());
  } else if (field->hasPragma(PRAG_TYPE_VARIABLE) || field->hasPragma(PRAG_SUPER_CLASS))
    fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, new SymExpr(_this), new SymExpr(new_StringSymbol(field->name)))));
  else
    fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, new CallExpr(PRIMITIVE_GET_MEMBER, new SymExpr(_this), new SymExpr(new_StringSymbol(field->name)))));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_line_info(fn, field->lineno);
  normalize(fn);
  ct->methods.add(fn);
  fn->addPragma(PRAG_METHOD);
  fn->cname = astr("_", ct->symbol->cname, "_", fn->cname);
  fn->addPragma(PRAG_NO_PARENS);
  fn->_this = _this;
}


static FnSymbol* chpl_main_exists(void) {
  FnSymbol* match = NULL;
  if (strlen(mainModuleName) != 0) {
    ModuleSymbol* module = NULL;
    forv_Vec(ModuleSymbol, mod, allModules) {
      if (!strcmp(mainModuleName, mod->name))
        module = mod;
    }
    if (!module)
      USR_FATAL("Couldn't find module %s", mainModuleName);

    forv_Vec(FnSymbol, fn, gFns) {
      if (!strcmp("main", fn->name) && !fn->numFormals() && fn->getModule() == module) {
        if (!match) {
          match = fn;
        } else {
          USR_FATAL(fn, "main multiply defined in module %s -- first occurrence at %s", mainModuleName, match->stringLoc());
        }
      }
    }
  } else {
    forv_Vec(FnSymbol, fn, gFns) {
      if (!strcmp("main", fn->name) && !fn->numFormals()) {
        if (!match) {
          match = fn;
        } else {
          USR_FATAL(fn, "main multiply defined -- first occurrence at %s",
                    match->stringLoc());
        }
      }
    }
  }
  return match;
}


static void build_chpl_main(void) {
  chpl_main = chpl_main_exists();

  if (fRuntime) {
    if (chpl_main)
      INT_FATAL(chpl_main, "'main' found when compiling runtime file");
    if (userModules.n != 1)
      INT_FATAL("expected one module when compiling runtime file");
  }

  if (!chpl_main) {
    ModuleSymbol* mainModule = NULL;
    if (strlen(mainModuleName) != 0) {
      forv_Vec(ModuleSymbol, mod, userModules) {
        if (!strcmp(mod->name, mainModuleName))
          mainModule = mod;
      }
      if (!mainModule)
        USR_FATAL("unknown module specified in '--main-module=%s'", mainModuleName);
    } else {
      for_alist(expr, theProgram->block->body) {
        if (DefExpr* def = toDefExpr(expr)) {
          if (ModuleSymbol* mod = toModuleSymbol(def->sym)) {
            if (mod->modTag == MOD_USER) {
              if (mainModule) {
                USR_FATAL_CONT("a program with multiple user modules requires a main function");
                USR_PRINT("alternatively, specify a main module with --main-module");
                USR_STOP();
              }
              mainModule = mod;
            }
          }
        }
      }
    }
    SET_LINENO(mainModule);
    chpl_main = new FnSymbol("main");
    chpl_main->retType = dtVoid;
    mainModule->block->insertAtTail(new DefExpr(chpl_main));
    normalize(chpl_main);
  } else if (!isModuleSymbol(chpl_main->defPoint->parentSymbol)) {
    USR_FATAL(chpl_main, "main function must be defined at module scope");
  }
  SET_LINENO(chpl_main);
  chpl_main->insertAtHead(new CallExpr(chpl_main->getModule()->initFn));
  if (!fRuntime) {
    VarSymbol* endCount = new VarSymbol("_endCount");
    endCount->addPragma(PRAG_TEMP);
    chpl_main->insertAtHead(new CallExpr("_startTrackingMem"));
    chpl_main->insertAtHead(new CallExpr(PRIMITIVE_SET_END_COUNT, endCount));
    chpl_main->insertAtHead(new CallExpr(PRIMITIVE_MOVE, endCount, new CallExpr("_endCountAlloc")));
    chpl_main->insertAtHead(new DefExpr(endCount));
    chpl_main->insertBeforeReturn(new CallExpr("_waitEndCount"));
  }
  chpl_main->insertAtHead(new CallExpr(theProgram->initFn));
}


static void build_record_equality_function(ClassType* ct) {
  if (function_exists("==", 2, ct))
    return;

  FnSymbol* fn = new FnSymbol("==");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  arg1->markedGeneric = true;
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtAny);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->retType = dtBool;
  for_fields(tmp, ct) {
    if (strcmp(tmp->name, "_promotionType")) {
      Expr* left = new CallExpr(tmp->name, gMethodToken, arg1);
      Expr* right = new CallExpr(tmp->name, gMethodToken, arg2);
      fn->insertAtTail(new CondStmt(new CallExpr("!=", left, right), new CallExpr(PRIMITIVE_RETURN, gFalse)));
    }
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gTrue));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_line_info(def, ct->symbol->lineno);
  normalize(fn);
}


static void build_record_inequality_function(ClassType* ct) {
  if (function_exists("!=", 2, ct))
    return;

  FnSymbol* fn = new FnSymbol("!=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  arg1->markedGeneric = true;
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtAny);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->retType = dtBool;
  for_fields(tmp, ct) {
    if (strcmp(tmp->name, "_promotionType")) {
      Expr* left = new CallExpr(tmp->name, gMethodToken, arg1);
      Expr* right = new CallExpr(tmp->name, gMethodToken, arg2);
      fn->insertAtTail(new CondStmt(new CallExpr("!=", left, right), new CallExpr(PRIMITIVE_RETURN, gTrue)));
    }
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gFalse));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_line_info(def, ct->symbol->lineno);
  normalize(fn);
}

static void build_enum_enumerate_function(EnumType* et) {
  // Build a function that returns a tuple of the enum's values
  // Each enum type has its own _enum_enumerate function.
  FnSymbol* fn = new FnSymbol("_enum_enumerate");
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  arg->addPragma(PRAG_TYPE_VARIABLE);
  fn->insertFormalAtTail(arg);
  fn->where = new BlockStmt(new CallExpr("==", arg, et->symbol));

  et->symbol->defPoint->insertAfter(new DefExpr(fn));

  // Generate the tuple of enum values for the given enum type
  CallExpr* call = new CallExpr("_construct__tuple");
  for_enums(constant, et) {
    call->insertAtTail(constant->sym);
  }
  call->insertAtHead(new_IntSymbol(call->numActuals()));

  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, call));
  normalize(fn);
}


static void build_enum_cast_function(EnumType* et) {
  // integral value to enumerated type cast function
  FnSymbol* fn = new FnSymbol("_cast");
  fn->addPragma(PRAG_TEMP);
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  arg1->addPragma(PRAG_TYPE_VARIABLE);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtIntegral);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->where = new BlockStmt(new CallExpr("==", arg1, et->symbol));
  if (fNoBoundsChecks) {
    fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, new CallExpr(PRIMITIVE_CAST, et->symbol, arg2)));
  } else {
    // Generate a select statement with when clauses for each of the
    // enumeration constants, and an otherwise clause that calls halt.
    long count = 0;
    BlockStmt* whenstmts = buildChapelStmt();
    for_enums(constant, et) {
      if (!get_int(constant->init, &count)) {
        count++;
      }
      CondStmt* when =
        new CondStmt(new CallExpr(PRIMITIVE_WHEN,
                                  new SymExpr(new_IntSymbol(count))),
                     new CallExpr(PRIMITIVE_RETURN,
                                  new CallExpr(PRIMITIVE_CAST,
                                               et->symbol, arg2)));
      whenstmts->insertAtTail(when);
    }
    const char * errorString = "enumerated type out of bounds";
    CondStmt* otherwise =
      new CondStmt(new CallExpr(PRIMITIVE_WHEN),
                   new BlockStmt(new CallExpr("halt",
                                 new_StringSymbol(errorString))));
    whenstmts->insertAtTail(otherwise);
    fn->insertAtTail(buildSelectStmt(new SymExpr(arg2), whenstmts));
  }
  DefExpr* def = new DefExpr(fn);
  //
  // these cast functions need to go in the base module because they
  // are automatically inserted to handle implicit coercions
  //
  baseModule->block->insertAtTail(def);
  reset_line_info(def, et->symbol->lineno);
  normalize(fn);

  // string to enumerated type cast function
  fn = new FnSymbol("_cast");
  fn->addPragma(PRAG_TEMP);
  arg1 = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  arg1->addPragma(PRAG_TYPE_VARIABLE);
  arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtString);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);

  CondStmt* cond = NULL;
  for_enums(constant, et) {
    cond = new CondStmt(
             new CallExpr("==", arg2, new_StringSymbol(constant->sym->name)),
             new CallExpr(PRIMITIVE_RETURN, constant->sym),
             cond);
  }
  fn->insertAtTail(cond);
  fn->insertAtTail(new CallExpr("halt", new_StringSymbol("illegal conversion of string \\\""), arg2, new_StringSymbol("\\\" to "), new_StringSymbol(et->symbol->name)));
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, 
                                toDefExpr(et->constants.first())->sym));

  fn->where = new BlockStmt(new CallExpr("==", arg1, et->symbol));
  def = new DefExpr(fn);
  //
  // these cast functions need to go in the base module because they
  // are automatically inserted to handle implicit coercions
  //
  baseModule->block->insertAtTail(def);
  reset_line_info(def, et->symbol->lineno);
  normalize(fn);
}


static void build_enum_init_function(EnumType* et) {
  if (function_exists("_init", 1, et))
    return;

  FnSymbol* fn = new FnSymbol("_init");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", et);
  fn->insertFormalAtTail(arg1);
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, toDefExpr(et->constants.first())->sym));
  DefExpr* def = new DefExpr(fn);
  et->symbol->defPoint->insertBefore(def);
  reset_line_info(def, et->symbol->lineno);
  normalize(fn);
}


static void build_enum_assignment_function(EnumType* et) {
  if (function_exists("=", 2, et))
    return;

  FnSymbol* fn = new FnSymbol("=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", et);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", et);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, arg2));
  DefExpr* def = new DefExpr(fn);
  et->symbol->defPoint->insertBefore(def);
  reset_line_info(def, et->symbol->lineno);
  normalize(fn);
}


static void build_record_assignment_function(ClassType* ct) {
  if (function_exists("=", 2, ct))
    return;

  FnSymbol* fn = new FnSymbol("=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  arg1->markedGeneric = true;
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtAny);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->retType = dtUnknown;
  for_fields(tmp, ct) {
    if (!tmp->hasPragma(PRAG_TYPE_VARIABLE) && !tmp->isParameter() && strcmp(tmp->name, "_promotionType"))
      fn->insertAtTail(new CallExpr("=", new CallExpr(".", arg1, new_StringSymbol(tmp->name)), new CallExpr(".", arg2, new_StringSymbol(tmp->name))));
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, arg1));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_line_info(def, ct->symbol->lineno);
  normalize(fn);
}


static void build_record_cast_function(ClassType* ct) {
  FnSymbol* fn = new FnSymbol("_cast");
  ArgSymbol* t = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  t->addPragma(PRAG_TYPE_VARIABLE);
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "arg", dtAny);
  fn->insertFormalAtTail(t);
  fn->insertFormalAtTail(arg);
  fn->where = new BlockStmt(new CallExpr(PRIMITIVE_ISSUBTYPE, ct->symbol, t));
  VarSymbol* ret = new VarSymbol("_ret");
  VarSymbol* tmp = new VarSymbol("_tmp");
  fn->insertAtTail(new DefExpr(ret));
  fn->insertAtTail(new DefExpr(tmp));
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_INIT, t)));
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr("=", tmp, arg)));
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, ret, tmp));
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, ret));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_line_info(def, ct->symbol->lineno);
  normalize(fn);
}

static void build_union_assignment_function(ClassType* ct) {
  if (function_exists("=", 2, ct))
    return;

  FnSymbol* fn = new FnSymbol("=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", ct);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->retType = dtUnknown;
  fn->insertAtTail(new CallExpr(PRIMITIVE_UNION_SETID, arg1, new_IntSymbol(0)));
  for_fields(tmp, ct)
    if (!tmp->hasPragma(PRAG_TYPE_VARIABLE))
      fn->insertAtTail(
        new CondStmt(
          new CallExpr("==",
            new CallExpr(PRIMITIVE_UNION_GETID, arg2),
            new_IntSymbol(tmp->id)),
          new CallExpr("=",
            new CallExpr(".", arg1, new_StringSymbol(tmp->name)),
            new CallExpr(".", arg2, new_StringSymbol(tmp->name)))));
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, arg1));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_line_info(def, ct->symbol->lineno);
  normalize(fn);
}


static void build_record_copy_function(ClassType* ct) {
  if (function_exists("_copy", 1, ct))
    return;

  INT_ASSERT(!ct->symbol->hasPragma(PRAG_TUPLE));

  FnSymbol* fn = new FnSymbol("_copy");
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "x", ct);
  arg->markedGeneric = true;
  fn->insertFormalAtTail(arg);
  CallExpr* call = new CallExpr(ct->defaultConstructor->name);
  for_fields(tmp, ct) {
    if (strcmp("_promotionType", tmp->name))
      call->insertAtTail(new NamedExpr(tmp->name, new CallExpr(".", arg, new_StringSymbol(tmp->name))));
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, call));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_line_info(def, ct->symbol->lineno);
  normalize(fn);
}


static void build_record_init_function(ClassType* ct) {
  if (function_exists("_init", 1, ct))
    return;

  FnSymbol* fn = new FnSymbol("_init");
  fn->addPragma(PRAG_INLINE);
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "x", ct);
  arg->markedGeneric = true;
  fn->insertFormalAtTail(arg);
  CallExpr* call = new CallExpr(ct->defaultConstructor->name);
  for_formals(formal, ct->defaultConstructor) {
    if (formal->hasPragma(PRAG_TYPE_VARIABLE) || formal->intent == INTENT_PARAM) {
      call->insertAtTail(new NamedExpr(formal->name, new CallExpr(".", arg, new_StringSymbol(formal->name))));
    } else if (!formal->defaultExpr) {
      call->insertAtTail(new NamedExpr(formal->name, new CallExpr(PRIMITIVE_INIT, new CallExpr(".", arg, new_StringSymbol(formal->name)))));
    }
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, call));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_line_info(def, ct->symbol->lineno);
  normalize(fn);
  if (ct->symbol->hasPragma(PRAG_TUPLE))
    fn->addPragma(PRAG_TUPLE_INIT);
}


static void build_record_hash_function(ClassType *ct) {
  if (function_exists("_associative_hash", 1, ct))
    return;

  FnSymbol *fn = new FnSymbol("_associative_hash");
  fn->addPragma(PRAG_INLINE);
  ArgSymbol *arg = new ArgSymbol(INTENT_BLANK, "r", ct);
  arg->markedGeneric = true;
  fn->insertFormalAtTail(arg);

  if (ct->fields.length() == 0) {
    fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, new_IntSymbol(0)));
  } else {
    CallExpr *call = NULL;
    bool first = true;
    for_fields(field, ct) {
      CallExpr *field_access = new CallExpr(field->name, gMethodToken, arg); 
      if (first) {
        call = new CallExpr("_associative_hash", field_access);
        first = false;
      } else {
        call = new CallExpr("^", 
                            new CallExpr("_associative_hash",
                                         field_access),
                            new CallExpr("<<",
                                         call,
                                         new_IntSymbol(17)));
      }
    }
    fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, call));
  }
  DefExpr *def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_line_info(def, ct->symbol->lineno);
  normalize(fn);
  if (ct->symbol->hasPragma(PRAG_TUPLE))
    fn->addPragma(PRAG_TUPLE_HASH_FUNCTION);
}


static void buildDefaultReadFunction(ClassType* ct) {
  if (function_exists("read", 3, dtMethodToken, dtChapelFile, ct))
    return;

  FnSymbol* fn = new FnSymbol("read");
  fn->cname = astr("_auto_", ct->symbol->name, "_read");
  ArgSymbol* arg = new ArgSymbol(INTENT_INOUT, "x", ct);
  arg->markedGeneric = true;
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", dtChapelFile);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(fn->_this);
  fn->insertFormalAtTail(arg);
  fn->retType = dtVoid;

  BlockStmt* body = new BlockStmt();
  Symbol* ignoreWhiteSpace = new VarSymbol("ignoreWhiteSpace");
  body->insertAtTail(new DefExpr(ignoreWhiteSpace, new SymExpr(gTrue)));
  Symbol* matchingCharWasRead = new VarSymbol("matchingCharWasRead");
  body->insertAtTail(new DefExpr(matchingCharWasRead, new_IntSymbol((int64)0)));
  Expr* fileArgFP = buildDotExpr(fn->_this, "_fp");
  CallExpr* readOpenBrace = new CallExpr("_readLitChar", fileArgFP, new_StringSymbol("{"), ignoreWhiteSpace);
  body->insertAtTail(new CallExpr("=", matchingCharWasRead, readOpenBrace));
  CallExpr* notRead = new CallExpr("==", matchingCharWasRead, new_IntSymbol(0));
  Expr* readError = new CallExpr("halt", new_StringSymbol("Read of the class failed: "), new CallExpr(PRIMITIVE_GET_ERRNO));
  CondStmt* readErrorCond = new CondStmt(notRead, readError);
  body->insertAtTail(readErrorCond);
  bool first = true;
  for_fields(tmp, ct) {
    if (tmp->hasPragma(PRAG_TYPE_VARIABLE) || tmp->hasPragma(PRAG_SUPER_CLASS))
      continue;
    if (!first) {
      CallExpr* readComma = new CallExpr("_readLitChar", fileArgFP->copy(), new_StringSymbol(","), ignoreWhiteSpace);
      body->insertAtTail(new CallExpr("=", matchingCharWasRead, readComma));
      body->insertAtTail(readErrorCond->copy());
    }  
    Symbol* fieldName = new VarSymbol("fieldName");
    body->insertAtTail(new DefExpr(fieldName, new_StringSymbol("")));
    CallExpr* readFieldName = new CallExpr(buildDotExpr(fn->_this, "read"), fieldName);
    body->insertAtTail(readFieldName);
    Symbol* name = new_StringSymbol(tmp->name);
    Expr* confirmFieldName = new CallExpr("!=", fieldName, name);
    CondStmt* fieldNameCond = new CondStmt(confirmFieldName, readError->copy());
    body->insertAtTail(fieldNameCond);
    CallExpr* readEqualSign = new CallExpr("_readLitChar", fileArgFP->copy(), new_StringSymbol("="), ignoreWhiteSpace);
    body->insertAtTail(new CallExpr("=", matchingCharWasRead, readEqualSign));
    body->insertAtTail(readErrorCond->copy());
    CallExpr* argName = new CallExpr(".", arg, name);
    CallExpr* readValue = new CallExpr(buildDotExpr(fn->_this, "read"), argName);
    body->insertAtTail(readValue);
    first = false;
  }
  CallExpr* readCloseBrace = new CallExpr("_readLitChar", fileArgFP->copy(), new_StringSymbol("}"), ignoreWhiteSpace);
  body->insertAtTail(new CallExpr("=", matchingCharWasRead, readCloseBrace));
  body->insertAtTail(readErrorCond->copy());
  fn->insertAtTail(buildOnStmt(new SymExpr(fn->_this), body));

  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  ct->methods.add(fn);
  fn->addPragma(PRAG_METHOD);
  reset_line_info(def, ct->symbol->lineno);
  normalize(fn);
  ct->methods.add(fn);
}


static void buildDefaultReadFunction(EnumType* et) {
  if (function_exists("read", 3, dtMethodToken, dtChapelFile, et))
    return;

  FnSymbol* fn = new FnSymbol("read");
  fn->cname = astr("_auto_", et->symbol->name, "_read");
  ArgSymbol* arg = new ArgSymbol(INTENT_INOUT, "x", et);
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", dtChapelFile);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(fn->_this);
  fn->insertFormalAtTail(arg);
  fn->retType = dtVoid;

  Symbol* valString = new VarSymbol("valString");
  fn->insertAtTail(new DefExpr(valString, new_StringSymbol("")));
  fn->insertAtTail(new CallExpr(buildDotExpr(fn->_this, "read"), valString));
  Expr* elseStmt = new CallExpr("halt", 
                                new_StringSymbol("***Error: Not of "), 
                                new_StringSymbol(et->symbol->name), 
                                new_StringSymbol(" type***"));
  for_enums_backward(constant, et) {
    Expr* cond = new CallExpr("==", valString, new_StringSymbol(constant->sym->name));
    Expr* thenStmt = new CallExpr("=", arg, constant->sym);
    elseStmt = new CondStmt(cond, thenStmt, elseStmt);
    
  }
  fn->insertAtTail(elseStmt);

  DefExpr* def = new DefExpr(fn);
  et->symbol->defPoint->insertBefore(def);
  et->methods.add(fn);
  fn->addPragma(PRAG_METHOD);
  reset_line_info(def, et->symbol->lineno);
  normalize(fn);
  et->methods.add(fn);
}

static bool buildWriteSuperClass(ArgSymbol* fileArg, FnSymbol* fn, Expr* dot, Type* type, bool first=true) {
  ClassType* ct = toClassType(type);
  bool printedSomething = false;
  if (!ct)
    return false; // Maybe error out?
  for_fields(tmp, ct) {
    if (tmp->hasPragma(PRAG_TYPE_VARIABLE))
      continue;
    if (tmp->hasPragma(PRAG_SUPER_CLASS)) {
      printedSomething = buildWriteSuperClass(fileArg, fn, buildDotExpr(dot, tmp->name), ct->dispatchParents.v[0], first);
      if (printedSomething)
        first = false;
      continue;
    }
    if (!first) {
      fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol(", ")));
    }
    fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol(tmp->name)));
    fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol(" = ")));
    fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"),
                                    buildDotExpr(dot->copy(), tmp->name)));
    first = false;
    printedSomething = true;
  }
  return printedSomething;
}
static void buildDefaultWriteFunction(ClassType* ct) {
  if (function_exists("writeThis", 3, dtMethodToken, ct, dtWriter))
    return;

  FnSymbol* fn = new FnSymbol("writeThis");
  fn->cname = astr("_auto_", ct->symbol->name, "_write");
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", ct);
  ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", dtWriter);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(fn->_this);
  fn->insertFormalAtTail(fileArg);
  fn->retType = dtVoid;

  if (ct->classTag == CLASS_CLASS) {
    BlockStmt* fwriteNil = new BlockStmt();
    fwriteNil->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol("nil")));
    fwriteNil->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
    fn->insertAtTail(new CondStmt(new CallExpr("==", fn->_this, gNil),
                                    fwriteNil));
  }

  if (ct->classTag == CLASS_CLASS) {
    fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol("{")));
  } else {
    fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol("(")));
  }

  if (ct->classTag == CLASS_UNION) {
    CondStmt* cond = NULL;
    for_fields(tmp, ct) {
      BlockStmt* writeFieldBlock = new BlockStmt();
      writeFieldBlock->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol(tmp->name)));
      writeFieldBlock->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol(" = ")));
      writeFieldBlock->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), 
                                                 buildDotExpr(fn->_this, tmp->name)));
      cond = new CondStmt(new CallExpr("==", new CallExpr(PRIMITIVE_UNION_GETID, fn->_this), new_IntSymbol(tmp->id)), writeFieldBlock, cond);
    }
    fn->insertAtTail(cond);
  } else {
    bool first = true;
    for_fields(tmp, ct) {
      if (tmp->hasPragma(PRAG_TYPE_VARIABLE))
        continue;
      if (!strcmp("outer", tmp->name))
        continue;
      if (tmp->hasPragma(PRAG_SUPER_CLASS)) {
        bool printedSomething =
          buildWriteSuperClass(fileArg, fn, buildDotExpr(fn->_this, tmp->name), fn->_this->type->dispatchParents.v[0]);
        if (printedSomething)
          first = false;
        continue;
      }
      if (!first) {
        fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol(", ")));
      }
      fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol(tmp->name)));
      fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol(" = ")));
      fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), 
                                      buildDotExpr(fn->_this, tmp->name)));
      first = false;
    }
  }

  if (ct->classTag == CLASS_CLASS) {
    fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol("}")));
  } else {
    fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "write"), new_StringSymbol(")")));
  }

  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  fn->addPragma(PRAG_METHOD);
  reset_line_info(def, ct->symbol->lineno);
  normalize(fn);
  ct->methods.add(fn);
}


static void buildStringCastFunction(EnumType* et) {
  if (function_exists("_cast", 2, dtString, et))
    return;

  FnSymbol* fn = new FnSymbol("_cast");
  ArgSymbol* t = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  t->addPragma(PRAG_TYPE_VARIABLE);
  fn->insertFormalAtTail(t);
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "this", et);
  fn->insertFormalAtTail(arg);
  fn->where = new BlockStmt(new CallExpr("==", t, dtString->symbol));

  for_enums(constant, et) {
    fn->insertAtTail(
      new CondStmt(
        new CallExpr("==", arg, constant->sym),
        new CallExpr(PRIMITIVE_RETURN, new_StringSymbol(constant->sym->name))));
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, new_StringSymbol("")));

  DefExpr* def = new DefExpr(fn);
  //
  // these cast functions need to go in the base module because they
  // are automatically inserted to handle implicit coercions
  //
  baseModule->block->insertAtTail(def);
  reset_line_info(def, et->symbol->lineno);
  normalize(fn);
}
