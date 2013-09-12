#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "config.h"

static bool mainReturnsInt;

static void build_chpl_entry_points(void);
static void build_getter(ClassType* ct, Symbol* field);
static void build_union_assignment_function(ClassType* ct);
static void build_enum_assignment_function(EnumType* et);
static void build_record_assignment_function(ClassType* ct);
static void build_record_cast_function(ClassType* ct);
static void build_record_copy_function(ClassType* ct);
static void build_record_hash_function(ClassType* ct);
static void build_record_equality_function(ClassType* ct);
static void build_record_inequality_function(ClassType* ct);
static void build_union_assignment_function(ClassType* ct);
static void build_enum_cast_function(EnumType* et);
static void build_enum_enumerate_function(EnumType* et);

//static void buildDefaultReadFunction(ClassType* type);
//static void buildDefaultReadFunction(EnumType* type);

static void buildDefaultReadWriteFunctions(ClassType* type);

static void buildStringCastFunction(EnumType* type);

static void buildDefaultDestructor(ClassType* ct);


void buildDefaultFunctions(void) {
  build_chpl_entry_points();
  SET_LINENO(rootModule); // todo - remove reset_ast_loc() calls below?

  Vec<BaseAST*> asts;
  collect_asts(rootModule, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (TypeSymbol* type = toTypeSymbol(ast)) {
      if (ClassType* ct = toClassType(type->type)) {
        for_fields(field, ct) {
          if (!field->hasFlag(FLAG_IMPLICIT_ALIAS_FIELD)) {
            if (isVarSymbol(field)) {
              if (strcmp(field->name, "_promotionType")) {
                build_getter(ct, field);
              }
            } else if (isEnumType(field->type)) {
              build_getter(ct, field);
            }
          }
        }
      }
      if (ClassType* ct = toClassType(type->type))
        if (!ct->symbol->hasFlag(FLAG_REF))
          buildDefaultDestructor(ct);
      if (type->hasFlag(FLAG_NO_DEFAULT_FUNCTIONS))
        continue;
      if (EnumType* et = toEnumType(type->type)) {
        //buildDefaultReadFunction(et);
        buildStringCastFunction(et);
      } else if (ClassType* ct = toClassType(type->type)) {
        buildDefaultReadWriteFunctions(ct);
      }
      if (ClassType* ct = toClassType(type->type)) {
        if (isRecord(ct)) {
          if (!isRecordWrappedType(ct)) {
            build_record_equality_function(ct);
            build_record_inequality_function(ct);
          }
          build_record_assignment_function(ct);
          build_record_cast_function(ct);
          build_record_copy_function(ct);
          build_record_hash_function(ct);
        }
        if (isUnion(ct))
          build_union_assignment_function(ct);
      }
      if (EnumType* et = toEnumType(type->type)) {
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
  forv_Vec(FnSymbol, fn, gFnSymbols) {
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
            call->replace(new CallExpr(PRIM_GET_MEMBER, arg2->remove(), new_StringSymbol(field->name)));
          }
        }
      }
    }
    return;
  }

  FnSymbol* fn = new FnSymbol(field->name);
  fn->addFlag(FLAG_NO_IMPLICIT_COPY);
  fn->addFlag(FLAG_INLINE);
//  fn->addFlag(FLAG_TEMP); // Getter functions act as if user-defined.
  if (ct->symbol->hasFlag(FLAG_SYNC)) 
    fn->addFlag(FLAG_SYNC);
  if (ct->symbol->hasFlag(FLAG_SINGLE)) 
    fn->addFlag(FLAG_SINGLE);
  if (ct->symbol->hasFlag(FLAG_ATOMIC_TYPE))
    fn->addFlag(FLAG_ATOMIC_TYPE);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", ct);
  _this->addFlag(FLAG_ARG_THIS);
  fn->insertFormalAtTail(_this);
  if (field->isParameter())
    fn->retTag = RET_PARAM;
  else if (field->hasFlag(FLAG_TYPE_VARIABLE))
    fn->retTag = RET_TYPE;
  else if (field->hasFlag(FLAG_SUPER_CLASS)) {
    fn->retTag = RET_VALUE;
  } else {
    fn->retTag = RET_VAR;
    fn->setter = new DefExpr(new ArgSymbol(INTENT_BLANK, "setter", dtBool));
  }
  if (isUnion(ct))
    fn->insertAtTail(
      new CondStmt(
        new SymExpr(fn->setter->sym),
        new CallExpr(PRIM_SET_UNION_ID, _this, new_IntSymbol(field->id)),
        new CondStmt(
          new CallExpr("!=", 
            new CallExpr(PRIM_GET_UNION_ID, _this),
              new_IntSymbol(field->id)),
          new CallExpr("halt", new_StringSymbol("illegal union access")))));
  if (isTypeSymbol(field) && isEnumType(field->type)) {
    fn->insertAtTail(new CallExpr(PRIM_RETURN, field));
    // better flatten enumerated types now
    ct->symbol->defPoint->insertBefore(field->defPoint->remove());
  } else if (field->hasFlag(FLAG_TYPE_VARIABLE) || field->hasFlag(FLAG_SUPER_CLASS))
    fn->insertAtTail(new CallExpr(PRIM_RETURN, new CallExpr(PRIM_GET_MEMBER_VALUE, new SymExpr(_this), new SymExpr(new_StringSymbol(field->name)))));
  else
    fn->insertAtTail(new CallExpr(PRIM_RETURN, new CallExpr(PRIM_GET_MEMBER, new SymExpr(_this), new SymExpr(new_StringSymbol(field->name)))));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_ast_loc(fn, field);
  normalize(fn);
  ct->methods.add(fn);
  fn->addFlag(FLAG_METHOD);
  fn->cname = astr("_", ct->symbol->cname, "_", fn->cname);
  fn->addFlag(FLAG_NO_PARENS);
  fn->_this = _this;
}


static FnSymbol* chpl_gen_main_exists(void) {
  FnSymbol* match = NULL;
  ModuleSymbol* matchMod = NULL;
  ModuleSymbol* module = NULL;
  if (strlen(mainModuleName) != 0) {
    forv_Vec(ModuleSymbol, mod, allModules) {
      if (!strcmp(mainModuleName, mod->name))
        module = mod;
    }
    if (!module)
      USR_FATAL("Couldn't find module %s", mainModuleName);
  }
  bool firstProblem = true;
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!strcmp("main", fn->name)) {
      if (fn->numFormals() == 0 ||
          (fn->numFormals() == 1 &&
           fn->getFormal(1)->typeInfo() == dtArray) ) {
        mainHasArgs = (fn->numFormals() > 0); 

        CallExpr* ret = toCallExpr(fn->body->body.last());
        if (!ret || !ret->isPrimitive(PRIM_RETURN))
          INT_FATAL(fn, "function is not normalized");
        SymExpr* sym = toSymExpr(ret->get(1));
        if (!sym)
          INT_FATAL(fn, "function is not normalized");
        if( sym->var != gVoid ) {
          mainReturnsInt = true;
        } else {
          mainReturnsInt = false;
        }

        ModuleSymbol* fnMod = fn->getModule();
        if ((module == NULL && fnMod->modTag == MOD_MAIN) ||
            fnMod == module) {
          if (!match) {
            match = fn;
            matchMod = fnMod;
          } else {
            if (firstProblem) {
              firstProblem = false;
              USR_FATAL_CONT("Ambiguous main() function%s:",
                             fnMod == matchMod ? "" : " (use --main-module to disambiguate)");
              USR_PRINT(match, "in module %s", matchMod->name);
            }
            USR_PRINT(fn, "in module %s", fnMod->name);
          } // else, this is not a candidate for the main module
        }
      } else {
        USR_FATAL_CONT("main() function with invalid signature");
        USR_PRINT(fn, "in module %s", fn->getModule()->name);
      }
    }
  }
  if (!firstProblem) {
    USR_STOP();
  }
  return match;
}


static void build_chpl_entry_points(void) {

  //                                                                          
  // chpl_user_main is the (user) programmatic portion of the app             
  //                                                                          
  FnSymbol* chpl_user_main = chpl_gen_main_exists();                              

  if (fLibraryCompile) {
    if (chpl_user_main)                                                       
      INT_FATAL(chpl_user_main, "'main' found when compiling a library");
    if (mainModules.n != 1)
      INT_FATAL("expected one module when compiling a library");
  }

  if (!chpl_user_main) {
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
            if (mod->modTag == MOD_MAIN) {
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
    chpl_user_main = new FnSymbol("main");
    chpl_user_main->retType = dtVoid;
//    chpl_user_main->addFlag(FLAG_TEMP);
    mainModule->block->insertAtTail(new DefExpr(chpl_user_main));
    normalize(chpl_user_main);
  } else {
    if (!isModuleSymbol(chpl_user_main->defPoint->parentSymbol)) {
      USR_FATAL(chpl_user_main, "main function must be defined at module scope");
    }
    mainModule = chpl_user_main->getModule();
  }

  SET_LINENO(chpl_user_main);
  chpl_user_main->cname = "chpl_user_main";

  //
  // chpl_gen_main is the entry point for the compiler-generated cdoe.
  // It accounts for the initialization and memory tracking of the
  // code
  //
  chpl_gen_main = new FnSymbol("chpl_gen_main");

  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "_arg", dtMainArgument);
  chpl_gen_main->insertFormalAtTail(arg);
  chpl_gen_main->retType = dtInt[INT_SIZE_64];

  chpl_gen_main->cname = "chpl_gen_main";
  chpl_gen_main->addFlag(FLAG_EXPORT);  // chpl_gen_main is always exported.
  chpl_gen_main->addFlag(FLAG_TEMP);
  mainModule->block->insertAtTail(new DefExpr(chpl_gen_main));
  VarSymbol* main_ret = newTemp("_main_ret", dtInt[INT_SIZE_64]);
  VarSymbol* endCount = newTemp("_endCount");
  chpl_gen_main->insertAtTail(new DefExpr(main_ret));
  chpl_gen_main->insertAtTail(new DefExpr(endCount));

  chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE, endCount, new CallExpr("_endCountAlloc")));
  chpl_gen_main->insertAtTail(new CallExpr(PRIM_SET_END_COUNT, endCount));
  chpl_gen_main->insertAtTail(new CallExpr("chpl_startTrackingMemory"));

  if( ! fNoInternalModules ) {
    // We have to initialize the main module explicitly.
    // It will initialize all the modules it uses, recursively.
    chpl_gen_main->insertAtTail(new CallExpr(mainModule->initFn));
  }

  bool main_ret_set = false;

  if (!fLibraryCompile) {
    SET_LINENO(chpl_gen_main);
    if (mainHasArgs) {
      VarSymbol* converted_args = newTemp("_main_args");
      chpl_gen_main->insertAtTail(new DefExpr(converted_args));
      chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE, converted_args, 
            new CallExpr("chpl_convert_args", arg)));
      if (mainReturnsInt) {
        chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE, main_ret, 
              new CallExpr("main", converted_args)));
        main_ret_set = true;
      } else {
        chpl_gen_main->insertAtTail(new CallExpr("main", converted_args));
      }
    } else {
      if (mainReturnsInt) {
        chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE, main_ret, 
              new CallExpr("main")));
        main_ret_set = true;
      } else {
        chpl_gen_main->insertAtTail(new CallExpr("main"));
      }
    }
  }

  if (!main_ret_set) {
    chpl_gen_main->insertAtTail(new CallExpr(PRIM_MOVE, main_ret, 
          new_IntSymbol(0, INT_SIZE_64)));
  }

  chpl_gen_main->insertAtTail(new CallExpr("_waitEndCount"));
  //chpl_gen_main->insertAtTail(new CallExpr("_endCountFree", endCount));

  chpl_gen_main->insertAtTail(new CallExpr(PRIM_RETURN, main_ret));

  normalize(chpl_gen_main);
}

static void build_record_equality_function(ClassType* ct) {
  if (function_exists("==", 2, ct))
    return;

  FnSymbol* fn = new FnSymbol("==");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  arg1->markedGeneric = true;
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", ct);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->retType = dtBool;
  for_fields(tmp, ct) {
    if (!tmp->hasFlag(FLAG_IMPLICIT_ALIAS_FIELD)) {
      if (strcmp(tmp->name, "_promotionType")) {
        Expr* left = new CallExpr(tmp->name, gMethodToken, arg1);
        Expr* right = new CallExpr(tmp->name, gMethodToken, arg2);
        fn->insertAtTail(new CondStmt(new CallExpr("!=", left, right), new CallExpr(PRIM_RETURN, gFalse)));
      }
    }
  }
  fn->insertAtTail(new CallExpr(PRIM_RETURN, gTrue));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_ast_loc(def, ct->symbol);
  normalize(fn);
}


static void build_record_inequality_function(ClassType* ct) {
  if (function_exists("!=", 2, ct))
    return;

  FnSymbol* fn = new FnSymbol("!=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  arg1->markedGeneric = true;
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", ct);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->retType = dtBool;
  for_fields(tmp, ct) {
    if (!tmp->hasFlag(FLAG_IMPLICIT_ALIAS_FIELD)) {
      if (strcmp(tmp->name, "_promotionType")) {
        Expr* left = new CallExpr(tmp->name, gMethodToken, arg1);
        Expr* right = new CallExpr(tmp->name, gMethodToken, arg2);
        fn->insertAtTail(new CondStmt(new CallExpr("!=", left, right), new CallExpr(PRIM_RETURN, gTrue)));
      }
    }
  }
  fn->insertAtTail(new CallExpr(PRIM_RETURN, gFalse));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_ast_loc(def, ct->symbol);
  normalize(fn);
}

static void build_enum_enumerate_function(EnumType* et) {
  // Build a function that returns a tuple of the enum's values
  // Each enum type has its own _enum_enumerate function.
  FnSymbol* fn = new FnSymbol("_enum_enumerate");
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  arg->addFlag(FLAG_TYPE_VARIABLE);
  fn->insertFormalAtTail(arg);
  fn->where = new BlockStmt(new CallExpr("==", arg, et->symbol));

  et->symbol->defPoint->insertAfter(new DefExpr(fn));

  // Generate the tuple of enum values for the given enum type
  CallExpr* call = new CallExpr("_construct__tuple");
  for_enums(constant, et) {
    call->insertAtTail(constant->sym);
  }
  call->insertAtHead(new_IntSymbol(call->numActuals()));

  fn->insertAtTail(new CallExpr(PRIM_RETURN, call));

  normalize(fn);
}

static void build_enum_cast_function(EnumType* et) {
  // integral value to enumerated type cast function
  FnSymbol* fn = new FnSymbol("_cast");
  fn->addFlag(FLAG_TEMP);
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  arg1->addFlag(FLAG_TYPE_VARIABLE);
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtIntegral);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->where = new BlockStmt(new CallExpr("==", arg1, et->symbol));
  if (fNoBoundsChecks) {
    fn->insertAtTail(new CallExpr(PRIM_RETURN, new CallExpr(PRIM_CAST, et->symbol, arg2)));
  } else {
    // Generate a select statement with when clauses for each of the
    // enumeration constants, and an otherwise clause that calls halt.
    int64_t count = 0;
    BlockStmt* whenstmts = buildChapelStmt();
    for_enums(constant, et) {
      if (!get_int(constant->init, &count)) {
        count++;
      }
      CondStmt* when =
        new CondStmt(new CallExpr(PRIM_WHEN,
                                  new SymExpr(new_IntSymbol(count))),
                     new CallExpr(PRIM_RETURN,
                                  new CallExpr(PRIM_CAST,
                                               et->symbol, arg2)));
      whenstmts->insertAtTail(when);
    }
    const char * errorString = "enumerated type out of bounds";
    CondStmt* otherwise =
      new CondStmt(new CallExpr(PRIM_WHEN),
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
  reset_ast_loc(def, et->symbol);
  normalize(fn);

  // string to enumerated type cast function
  fn = new FnSymbol("_cast");
  fn->addFlag(FLAG_TEMP);
  arg1 = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  arg1->addFlag(FLAG_TYPE_VARIABLE);
  arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", dtString);
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);

  CondStmt* cond = NULL;
  for_enums(constant, et) {
    cond = new CondStmt(
             new CallExpr("==", arg2, new_StringSymbol(constant->sym->name)),
             new CallExpr(PRIM_RETURN, constant->sym),
             cond);
  }
  fn->insertAtTail(cond);
  fn->insertAtTail(new CallExpr("halt", new_StringSymbol("illegal conversion of string \\\""), arg2, new_StringSymbol("\\\" to "), new_StringSymbol(et->symbol->name)));
  fn->insertAtTail(new CallExpr(PRIM_RETURN, 
                                toDefExpr(et->constants.first())->sym));

  fn->where = new BlockStmt(new CallExpr("==", arg1, et->symbol));
  def = new DefExpr(fn);
  //
  // these cast functions need to go in the base module because they
  // are automatically inserted to handle implicit coercions
  //
  baseModule->block->insertAtTail(def);
  reset_ast_loc(def, et->symbol);
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
  fn->insertAtTail(new CallExpr(PRIM_RETURN, arg2));
  DefExpr* def = new DefExpr(fn);
  et->symbol->defPoint->insertBefore(def);
  reset_ast_loc(def, et->symbol);
  normalize(fn);
}


static void build_record_assignment_function(ClassType* ct) {
  if (function_exists("=", 2, ct))
    return;

  FnSymbol* fn = new FnSymbol("=");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", ct);
  arg1->markedGeneric = true;

  bool externRecord = ct->symbol->hasFlag(FLAG_EXTERN);
  // If the LHS is extern, the RHS must be of matching type; otherwise
  // Chapel permits matches that have the same names
  ArgSymbol* arg2 = new ArgSymbol(INTENT_BLANK, "_arg2", 
                                  (externRecord ? ct : dtAny));
  fn->insertFormalAtTail(arg1);
  fn->insertFormalAtTail(arg2);
  fn->retType = dtUnknown;

  if (externRecord) {
    fn->insertAtTail(new CallExpr(PRIM_MOVE, arg1, arg2));
  } else {
    for_fields(tmp, ct) {
      if (!tmp->hasFlag(FLAG_IMPLICIT_ALIAS_FIELD)) {
        if (!tmp->hasFlag(FLAG_TYPE_VARIABLE) && !tmp->isParameter() && strcmp(tmp->name, "_promotionType"))
          fn->insertAtTail(new CallExpr("=", new CallExpr(".", arg1, new_StringSymbol(tmp->name)), new CallExpr(".", arg2, new_StringSymbol(tmp->name))));
      }
    }
  }
  fn->insertAtTail(new CallExpr(PRIM_RETURN, arg1));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_ast_loc(def, ct->symbol);
  normalize(fn);
}


static void build_record_cast_function(ClassType* ct) {
  FnSymbol* fn = new FnSymbol("_cast");
  ArgSymbol* t = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  t->addFlag(FLAG_TYPE_VARIABLE);
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "arg", dtAny);
  fn->insertFormalAtTail(t);
  fn->insertFormalAtTail(arg);
  fn->where = new BlockStmt(new CallExpr(PRIM_IS_SUBTYPE, ct->symbol, t));
  VarSymbol* ret = newTemp();
  VarSymbol* tmp = newTemp();
  fn->insertAtTail(new DefExpr(ret));
  fn->insertAtTail(new DefExpr(tmp));
  fn->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_INIT, t)));
  fn->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr("=", tmp, arg)));
  fn->insertAtTail(new CallExpr(PRIM_MOVE, ret, tmp));
  fn->insertAtTail(new CallExpr(PRIM_RETURN, ret));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_ast_loc(def, ct->symbol);
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
  fn->insertAtTail(new CallExpr(PRIM_SET_UNION_ID, arg1, new_IntSymbol(0)));
  for_fields(tmp, ct)
    if (!tmp->hasFlag(FLAG_IMPLICIT_ALIAS_FIELD))
      if (!tmp->hasFlag(FLAG_TYPE_VARIABLE))
        fn->insertAtTail(
          new CondStmt(
            new CallExpr("==",
              new CallExpr(PRIM_GET_UNION_ID, arg2),
              new_IntSymbol(tmp->id)),
            new CallExpr("=",
              new CallExpr(".", arg1, new_StringSymbol(tmp->name)),
              new CallExpr(".", arg2, new_StringSymbol(tmp->name)))));
  fn->insertAtTail(new CallExpr(PRIM_RETURN, arg1));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_ast_loc(def, ct->symbol);
  normalize(fn);
}


static void build_record_copy_function(ClassType* ct) {
  if (function_exists("chpl__initCopy", 1, ct))
    return;

  INT_ASSERT(!ct->symbol->hasFlag(FLAG_TUPLE));

  FnSymbol* fn = new FnSymbol("chpl__initCopy");
  fn->addFlag(FLAG_INIT_COPY_FN);
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "x", ct);
  arg->markedGeneric = true;
  fn->insertFormalAtTail(arg);
  CallExpr* call = new CallExpr(ct->initializer);
  for_fields(tmp, ct) {
    if (!tmp->hasFlag(FLAG_IMPLICIT_ALIAS_FIELD))
      if (strcmp("_promotionType", tmp->name))
        call->insertAtTail(new NamedExpr(tmp->name, new CallExpr(".", arg, new_StringSymbol(tmp->name))));
  }
  fn->insertAtTail(new CallExpr(PRIM_RETURN, call));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_ast_loc(def, ct->symbol);
  normalize(fn);
}


static void build_record_hash_function(ClassType *ct) {
  if (function_exists("chpl__defaultHash", 1, ct))
    return;

  FnSymbol *fn = new FnSymbol("chpl__defaultHash");
  fn->addFlag(FLAG_INLINE);
  ArgSymbol *arg = new ArgSymbol(INTENT_BLANK, "r", ct);
  arg->markedGeneric = true;
  fn->insertFormalAtTail(arg);

  if (ct->fields.length == 0) {
    fn->insertAtTail(new CallExpr(PRIM_RETURN, new_IntSymbol(0)));
  } else {
    CallExpr *call = NULL;
    bool first = true;
    for_fields(field, ct) {
      if (!field->hasFlag(FLAG_IMPLICIT_ALIAS_FIELD)) {
        CallExpr *field_access = new CallExpr(field->name, gMethodToken, arg); 
        if (first) {
          call = new CallExpr("chpl__defaultHash", field_access);
          first = false;
        } else {
          call = new CallExpr("^", 
                              new CallExpr("chpl__defaultHash",
                                           field_access),
                              new CallExpr("<<",
                                           call,
                                           new_IntSymbol(17)));
        }
      }
    }
    fn->insertAtTail(new CallExpr(PRIM_RETURN, call));
  }
  DefExpr *def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_ast_loc(def, ct->symbol);
  normalize(fn);
}

static void buildDefaultReadWriteFunctions(ClassType* ct) {
  bool hasReadWriteThis = false;
  bool hasReadThis = false;
  bool hasWriteThis = false;
  bool makeReadThisAndWriteThis = true;

  // If we have a readWriteThis, we'll call it from readThis/writeThis.
  if (function_exists("readWriteThis", 3, dtMethodToken, ct, NULL)) {
    hasReadWriteThis = true;
  }
  // We'll make a writeThis and a readThis if neither exist.
  // If only one exists, we leave just one (as some types
  // can be written but not read, for example).
  if (function_exists("writeThis", 3, dtMethodToken, ct, dtWriter)) {
    hasWriteThis = true;
    makeReadThisAndWriteThis = false;
  }
  if (function_exists("readThis", 3, dtMethodToken, ct, dtReader)) {
    hasReadThis = true;
    makeReadThisAndWriteThis = false;
  }

  // Make writeThis if we have neither writeThis nor readThis.
  if ( makeReadThisAndWriteThis && ! hasWriteThis ) {
    FnSymbol* fn = new FnSymbol("writeThis");
    fn->cname = astr("_auto_", ct->symbol->name, "_write");
    fn->_this = new ArgSymbol(INTENT_BLANK, "this", ct);
    fn->_this->addFlag(FLAG_ARG_THIS);
    ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", dtWriter);
    fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
    fn->insertFormalAtTail(fn->_this);
    fn->insertFormalAtTail(fileArg);
    fn->retType = dtVoid;

    if( hasReadWriteThis ) {
      fn->insertAtTail(new CallExpr(buildDotExpr(fn->_this, "readWriteThis"), fileArg));
    } else {
      fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "writeThisDefaultImpl"), fn->_this));
    }

    DefExpr* def = new DefExpr(fn);
    ct->symbol->defPoint->insertBefore(def);
    fn->addFlag(FLAG_METHOD);
    reset_ast_loc(def, ct->symbol);
    normalize(fn);
    ct->methods.add(fn);
  }
  if ( makeReadThisAndWriteThis && ! hasReadThis ) {
    FnSymbol* fn = new FnSymbol("readThis");
    fn->cname = astr("_auto_", ct->symbol->name, "_read");
    fn->_this = new ArgSymbol(INTENT_BLANK, "this", ct);
    fn->_this->addFlag(FLAG_ARG_THIS);
    ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", dtReader);
    fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
    fn->insertFormalAtTail(fn->_this);
    fn->insertFormalAtTail(fileArg);
    fn->retType = dtVoid;

    if( hasReadWriteThis ) {
      fn->insertAtTail(new CallExpr(buildDotExpr(fn->_this, "readWriteThis"), fileArg));
    } else {
      fn->insertAtTail(new CallExpr(buildDotExpr(fileArg, "readThisDefaultImpl"), fn->_this));
    }

    DefExpr* def = new DefExpr(fn);
    ct->symbol->defPoint->insertBefore(def);
    // ? ct->methods.add(fn) ? in old code
    fn->addFlag(FLAG_METHOD);
    reset_ast_loc(def, ct->symbol);
    normalize(fn);
    ct->methods.add(fn);
  }
}


static void buildStringCastFunction(EnumType* et) {
  if (function_exists("_cast", 2, dtString, et))
    return;

  FnSymbol* fn = new FnSymbol("_cast");
  ArgSymbol* t = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  t->addFlag(FLAG_TYPE_VARIABLE);
  fn->insertFormalAtTail(t);
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "this", et);
  arg->addFlag(FLAG_ARG_THIS);
  fn->insertFormalAtTail(arg);
  fn->where = new BlockStmt(new CallExpr("==", t, dtString->symbol));

  for_enums(constant, et) {
    fn->insertAtTail(
      new CondStmt(
        new CallExpr("==", arg, constant->sym),
        new CallExpr(PRIM_RETURN, new_StringSymbol(constant->sym->name))));
  }
  fn->insertAtTail(new CallExpr(PRIM_RETURN, new_StringSymbol("")));

  DefExpr* def = new DefExpr(fn);
  //
  // these cast functions need to go in the base module because they
  // are automatically inserted to handle implicit coercions
  //
  baseModule->block->insertAtTail(def);
  reset_ast_loc(def, et->symbol);
  normalize(fn);
}


static void buildDefaultDestructor(ClassType* ct) {
  if (function_exists("~chpl_destroy", 2, dtMethodToken, ct))
    return;

  SET_LINENO(ct->symbol);

  FnSymbol* fn = new FnSymbol("~chpl_destroy");
  fn->addFlag(FLAG_DESTRUCTOR);
  fn->addFlag(FLAG_INLINE);
  fn->cname = astr("chpl__auto_destroy_", ct->symbol->name);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", ct);
  fn->_this->addFlag(FLAG_ARG_THIS);
  fn->insertFormalAtTail(fn->_this);
  fn->retType = dtVoid;
  fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  ct->symbol->defPoint->insertBefore(new DefExpr(fn));
  fn->addFlag(FLAG_METHOD);
  ct->methods.add(fn);
}
