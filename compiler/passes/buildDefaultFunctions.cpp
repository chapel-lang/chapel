#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"


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
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (TypeSymbol* type = toTypeSymbol(ast)) {
      if (ClassType* ct = toClassType(type->type)) {
        for_fields(field, ct) {
          VarSymbol *cfield = toVarSymbol(field);
          // if suppress for cobegin created arg classes
          if (cfield && strcmp(field->name, "_promotionType")) {
            build_getter(ct, field);
          }
        }
      }
      if (type->hasPragma("no default functions"))
        continue;
      if (!fNoStdIncs) {
        if (EnumType* et = toEnumType(type->type)) {
          buildDefaultReadFunction(et);
          buildStringCastFunction(et);
        } else if (ClassType* ct = toClassType(type->type)) {
          buildDefaultReadFunction(ct);
          buildDefaultWriteFunction(ct);
        }
      }
      if (ClassType* ct = toClassType(type->type)) {
        if (ct->classTag == CLASS_RECORD) {
          if (!(ct->symbol->hasPragma("domain") ||
                ct->symbol->hasPragma("array"))) {
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
//  function's first formal's type's name matches formalTypeName1 if not NULL
//  function's second formal's type's name matches formalTypeName2 if not NULL

static bool type_name_match(const char* name, Symbol* sym) {
  if (!strcmp(name, sym->type->symbol->name))
    return true;
  SymExpr* symExpr = toSymExpr(sym->defPoint->exprType);
  if (symExpr && !strcmp(name, symExpr->var->name))
    return true;
  return false;
}

static FnSymbol* function_exists(const char* name,
                                 int numFormals = -1,
                                 const char* formalTypeName1 = NULL,
                                 const char* formalTypeName2 = NULL,
                                 const char* formalTypeName3 = NULL) {
  forv_Vec(FnSymbol, fn, gFns) {
    if (strcmp(name, fn->name))
      continue;

    if (numFormals != -1)
      if (numFormals != fn->numFormals())
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
  fn->defSetGet = true;
  fn->addPragma("inline");
  fn->isCompilerTemp = true;
  if (ct->symbol->hasPragma( "sync")) 
    fn->addPragma( "sync");
  ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", ct);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(_this);
  if (field->isParam())
    fn->retTag = RET_PARAM;
  else if (field->isTypeVariable)
    fn->retTag = RET_TYPE;
  else {
    fn->retTag = RET_VAR;
    fn->setter = new DefExpr(new ArgSymbol(INTENT_BLANK, "setter", dtBool));
  }
  if (ct->classTag == CLASS_UNION)
    fn->insertAtTail(new CondStmt(new SymExpr(fn->setter->sym), new CallExpr(PRIMITIVE_UNION_SETID, _this, new_IntSymbol(field->id)), new CondStmt(new CallExpr("!", new CallExpr(PRIMITIVE_UNION_GETID, _this, new_IntSymbol(field->id))), new CallExpr("halt", new_StringSymbol("illegal union access")))));
  if (field->isTypeVariable)
    fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, new SymExpr(_this), new SymExpr(new_StringSymbol(field->name)))));
  else
    fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, new CallExpr(PRIMITIVE_GET_MEMBER, new SymExpr(_this), new SymExpr(new_StringSymbol(field->name)))));
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


static FnSymbol* chpl_main_exists(void) {
  FnSymbol* match = NULL;
  if (strlen(mainModuleName) != 0) {
    SymScope* progScope = theProgram->block->blkScope;
    ModuleSymbol* module = toModuleSymbol(progScope->table.get(mainModuleName));
    if (!module)
      USR_FATAL("Couldn't find module %s", mainModuleName);
    Vec<FnSymbol*>* mainfns = module->block->blkScope->visibleFunctions.get(canonicalize_string("main"));
    if (!mainfns)
      return NULL;
    forv_Vec(FnSymbol, fn, *mainfns) {
      if (!fn->numFormals()) {
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
  if (!chpl_main) {
    int rootUserModuleCount = 0;
    ModuleSymbol* rootUserModule = NULL;
    for_alist(expr, theProgram->block->body) {
      if (DefExpr* def = toDefExpr(expr)) {
        if (ModuleSymbol* mod = toModuleSymbol(def->sym)) {
          if (mod->modTag == MOD_USER) {
            rootUserModuleCount++;
            rootUserModule = mod;
          }
        }
      }
    }
    if (rootUserModuleCount == 1) {
      chpl_main = new FnSymbol("main");
      chpl_main->retType = dtVoid;
      rootUserModule->block->insertAtTail(new DefExpr(chpl_main));
      normalize(chpl_main);
    } else if (strlen(mainModuleName) != 0) { 
      SymScope* progScope = theProgram->block->blkScope;
      ModuleSymbol* module = toModuleSymbol(progScope->table.get(mainModuleName));
      if (!module)
        USR_FATAL("Couldn't find module %s at program scope", mainModuleName);
      chpl_main = new FnSymbol("main");
      chpl_main->retType = dtVoid;
      module->block->insertAtTail(new DefExpr(chpl_main));
      normalize(chpl_main);
    } else {
      USR_FATAL("Code defines multiple modules but no main function.");
    }
  } else if (chpl_main->getModule() != chpl_main->defPoint->parentSymbol)
    USR_FATAL(chpl_main, "Main function must be defined at module scope");
  currentLineno = -1;
  chpl_main->insertAtHead(new CallExpr(chpl_main->getModule()->initFn));
  chpl_main->insertAtHead(new CallExpr(theProgram->initFn));
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
    if (strcmp(tmp->name, "_promotionType")) {
      Expr* left = new CallExpr(tmp->name, gMethodToken, arg1);
      Expr* right = new CallExpr(tmp->name, gMethodToken, arg2);
      fn->insertAtTail(new CondStmt(new CallExpr("!=", left, right), new CallExpr(PRIMITIVE_RETURN, gFalse)));
    }
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gTrue));
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
    if (strcmp(tmp->name, "_promotionType")) {
      Expr* left = new CallExpr(tmp->name, gMethodToken, arg1);
      Expr* right = new CallExpr(tmp->name, gMethodToken, arg2);
      fn->insertAtTail(new CondStmt(new CallExpr("!=", left, right), new CallExpr(PRIMITIVE_RETURN, gTrue)));
    }
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gFalse));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  normalize(fn);
}

static void build_enum_enumerate_function(EnumType* et) {
  // Build a function that returns a tuple of the enum's values
  // Each enum type has its own _enum_enumerate function.
  FnSymbol* fn = new FnSymbol("_enum_enumerate");
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  arg->isGeneric = false;
  arg->isTypeVariable = true;
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
  fn->isCompilerTemp = true;
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  arg1->isGeneric = false;
  arg1->isTypeVariable = true;
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
    BlockStmt* whenstmts = build_chpl_stmt();
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
    fn->insertAtTail(build_select(new SymExpr(arg2), whenstmts));
  }
  DefExpr* def = new DefExpr(fn);
  et->symbol->defPoint->insertBefore(def);
  reset_file_info(def, et->symbol->lineno, et->symbol->filename);
  normalize(fn);

  // string to enumerated type cast function
  fn = new FnSymbol("_cast");
  fn->isCompilerTemp = true;
  arg1 = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  arg1->isGeneric = false;
  arg1->isTypeVariable = true;
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
  et->symbol->defPoint->insertBefore(def);
  reset_file_info(def, et->symbol->lineno, et->symbol->filename);
  normalize(fn);
}


static void build_enum_init_function(EnumType* et) {
  if (function_exists("_init", 1, et->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("_init");
  ArgSymbol* arg1 = new ArgSymbol(INTENT_BLANK, "_arg1", et);
  fn->insertFormalAtTail(arg1);
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, toDefExpr(et->constants.first())->sym));
  DefExpr* def = new DefExpr(fn);
  et->symbol->defPoint->insertBefore(def);
  reset_file_info(def, et->symbol->lineno, et->symbol->filename);
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
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, arg2));
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
  for_fields(tmp, ct) {
    if (!tmp->isTypeVariable && !tmp->isParam() && strcmp(tmp->name, "_promotionType"))
      fn->insertAtTail(new CallExpr("=", new CallExpr(".", arg1, new_StringSymbol(tmp->name)), new CallExpr(".", arg2, new_StringSymbol(tmp->name))));
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, arg1));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  normalize(fn);
}


static void build_record_cast_function(ClassType* ct) {
  FnSymbol* fn = new FnSymbol("_cast");
  ArgSymbol* t = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  t->isTypeVariable = true;
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "arg", dtAny);
  fn->insertFormalAtTail(t);
  fn->insertFormalAtTail(arg);
  fn->where = new BlockStmt(new CallExpr(PRIMITIVE_ISSUBTYPE, ct->symbol, t));
  VarSymbol* ret = new VarSymbol("_ret");
  VarSymbol* tmp = new VarSymbol("_tmp");
  fn->insertAtTail(new DefExpr(ret));
  fn->insertAtTail(new DefExpr(tmp));
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr("_init", t)));
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr("=", tmp, arg)));
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, ret, tmp));
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, ret));
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
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, arg1));
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
  for_fields(tmp, ct) {
    if (strcmp("_promotionType", tmp->name))
      call->insertAtTail(new NamedExpr(tmp->name, new CallExpr(".", arg, new_StringSymbol(tmp->name))));
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, call));
  DefExpr* def = new DefExpr(fn);
  ct->symbol->defPoint->insertBefore(def);
  reset_file_info(def, ct->symbol->lineno, ct->symbol->filename);
  normalize(fn);
  if (ct->symbol->hasPragma("tuple")) {
    fn->addPragma("tuple copy");
    fn->addPragma("inline");
  }
}


static void build_record_init_function(ClassType* ct) {
  if (function_exists("_init", 1, ct->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("_init");
  fn->addPragma("inline");
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "x", ct);
  fn->insertFormalAtTail(arg);
  CallExpr* call = new CallExpr(ct->defaultConstructor->name);
  for_formals(formal, ct->defaultConstructor) {
    if (formal->isTypeVariable || formal->intent == INTENT_PARAM) {
      call->insertAtTail(new NamedExpr(formal->name, new CallExpr(".", arg, new_StringSymbol(formal->name))));
    } else if (!formal->defaultExpr) {
      call->insertAtTail(new NamedExpr(formal->name, new CallExpr("_init", new CallExpr(".", arg, new_StringSymbol(formal->name)))));
    }
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, call));
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

  if (ct->fields.length() == 0) {
    fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, new_IntSymbol(0)));
  } else {
    CallExpr *call = NULL;
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
    fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, call));
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
  TypeSymbol* fileType = toTypeSymbol(fileModule->lookup("file"));
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
  TypeSymbol* fileType = toTypeSymbol(fileModule->lookup("file"));
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
  for_enums_backward(constant, et) {
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
  if (function_exists("writeThis", 3, dtMethodToken->symbol->name, ct->symbol->name, "Writer"))
    return;

  FnSymbol* fn = new FnSymbol("writeThis");
  fn->cname = stringcat("_auto_", ct->symbol->name, "_write");
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", ct);
  ArgSymbol* fileArg = new ArgSymbol(INTENT_BLANK, "f", dtWriter);
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->insertFormalAtTail(fn->_this);
  fn->insertFormalAtTail(fileArg);
  fn->retType = dtVoid;

  if (ct->classTag == CLASS_CLASS) {
    BlockStmt* fwriteNil = new BlockStmt();
    fwriteNil->insertAtTail(new CallExpr(buildDot(fileArg, "write"), new_StringSymbol("nil")));
    fwriteNil->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
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


static void buildStringCastFunction(EnumType* et) {
  if (function_exists("_cast", 2, "string", et->symbol->name))
    return;

  FnSymbol* fn = new FnSymbol("_cast");
  ArgSymbol* t = new ArgSymbol(INTENT_BLANK, "t", dtAny);
  t->isTypeVariable = true;
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
  et->symbol->defPoint->insertBefore(def);
  reset_file_info(def, et->symbol->lineno, et->symbol->filename);
  normalize(fn);
}
