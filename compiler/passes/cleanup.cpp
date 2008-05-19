//
// cleanup
//
// This pass cleans up the AST after parsing. It handles
// transformations that would be difficult to do while parsing.
//

#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"


//
// Move the statements in a block out of the block
//
static void
flatten_scopeless_block(BlockStmt* block) {
  for_alist(stmt, block->body) {
    stmt->remove();
    block->insertBefore(stmt);
  }
  block->remove();
}


//
// Return the module imported by a use call.  The module returned could be
// nested: e.g. "use outermost.middle.innermost;"
//
static ModuleSymbol* getUsedModule(Expr* expr, CallExpr* useCall = NULL) {
  ModuleSymbol* mod = NULL;
  Symbol* symbol = NULL;
  if (!useCall) {
    CallExpr* call = toCallExpr(expr);
    if (!call)
      INT_FATAL(call, "Bad use statement in getUsedModule");
    if (!call->isPrimitive(PRIMITIVE_USE))
      INT_FATAL(call, "Bad use statement in getUsedModule");
    return getUsedModule(call->get(1), call);
  }

  if (SymExpr* sym = toSymExpr(expr)) {
    symbol = useCall->parentScope->lookup(sym->getName());
    mod = toModuleSymbol(symbol);
    if (symbol && !mod) {
      USR_FATAL(useCall, "Illegal use of non-module %s", symbol->name);
    } else if (!symbol) {
      USR_FATAL(useCall, "Cannot find module '%s'", sym->getName());
    }
  } else if(CallExpr* call = toCallExpr(expr)) {
    if (!call->isNamed("."))
      USR_FATAL(useCall, "Bad use statement in getUsedModule");
    ModuleSymbol* lhs = getUsedModule(call->get(1), useCall);
    if (!lhs)
      USR_FATAL(useCall, "Cannot find module");
    SymExpr* rhs = toSymExpr(call->get(2));
    const char* rhsName;
    if (!rhs)
      INT_FATAL(useCall, "Bad use statement in getUsedModule");

    if (!get_string(rhs, &rhsName))
      INT_FATAL(useCall, "Bad use statement in getUsedModule");
    symbol = lhs->lookup(rhsName);
    mod = toModuleSymbol(symbol);
    if (symbol && !mod) {
      USR_FATAL(useCall, "Illegal use of non-module %s", symbol->name);
    } else if (!symbol) {
      USR_FATAL(useCall, "Cannot find module '%s'", rhsName);
    }
  } else {
    INT_FATAL(useCall, "Bad use statement in getUsedModule");
  }

  return mod;
}

//
// Transform module uses into calls to initialize functions; store the
// relevant scoping information in BlockStmt::modUses
//
static void
process_import_expr(CallExpr* call) {
  ModuleSymbol* mod = getUsedModule(call);
  if (!mod)
    USR_FATAL(call, "Cannot find module");
  call->getStmtExpr()->insertBefore(new CondStmt(new SymExpr(mod->guard), buildOnStmt(new CallExpr(PRIMITIVE_ON_LOCALE_NUM, new SymExpr(new_IntSymbol(0))), new CallExpr(mod->initFn))));
  call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, mod->guard, gFalse));
  if (call->getFunction() == call->getModule()->initFn)
    call->getModule()->block->modUses.add(mod);
  else
    getVisibilityBlock(call)->modUses.add(mod);
  call->getStmtExpr()->remove();
}

//
// Compute dispatchParents and dispatchChildren vectors; add base
// class fields to subclasses; identify cyclic or illegal class or
// record hierarchies; add dummy field to empty records and unions
//
static void
add_class_to_hierarchy(ClassType* ct, Vec<ClassType*>* localSeenPtr = NULL) {
  static Vec<ClassType*> globalSeen; // classes already in hierarchy
  Vec<ClassType*> localSeen;         // classes in potential cycle

  if (!localSeenPtr)
    localSeenPtr = &localSeen;

  if (localSeenPtr->set_in(ct))
    USR_FATAL(ct, "Class hierarchy is cyclic");

  if (globalSeen.set_in(ct))
    return;
  globalSeen.set_add(ct);

  // make root records inherit from value
  // make root classes inherit from object
  if (ct->inherits.length() == 0 && !ct->symbol->hasPragma("no object")) {
    if (ct->classTag == CLASS_RECORD) {
      ct->dispatchParents.add(dtValue);
      dtValue->dispatchChildren.add(ct);
    } else if (ct->classTag == CLASS_CLASS) {
      ct->dispatchParents.add(dtObject);
      dtObject->dispatchChildren.add(ct);
      VarSymbol* super = new VarSymbol("super", dtObject);
      super->addPragma("super class");
      ct->fields.insertAtHead(new DefExpr(super));
    }
  }

  for_alist(expr, ct->inherits) {
    TypeSymbol* ts = toTypeSymbol(expr->lookup(expr));
    expr->remove();
    if (!ts)
      USR_FATAL(expr, "Illegal super class");
    ClassType* pt = toClassType(ts->type);
    if (!pt)
      USR_FATAL(expr, "Illegal super class %s", ts->name);
    if (ct->classTag == CLASS_UNION || pt->classTag == CLASS_UNION)
      USR_FATAL(expr, "Illegal inheritance involving union type");
    if (ct->classTag == CLASS_RECORD && pt->classTag == CLASS_CLASS)
      USR_FATAL(expr, "Record %s inherits from class %s",
                ct->symbol->name, pt->symbol->name);
    if (ct->classTag == CLASS_CLASS && pt->classTag == CLASS_RECORD)
      USR_FATAL(expr, "Class %s inherits from record %s",
                ct->symbol->name, pt->symbol->name);
    localSeenPtr->set_add(ct);
    add_class_to_hierarchy(pt, localSeenPtr);
    ct->dispatchParents.add(pt);
    pt->dispatchChildren.add(ct);
    if (ct->classTag != CLASS_CLASS) {
      for_fields_backward(field, pt) {
        if (toVarSymbol(field) && !field->hasPragma("super class")) {
          bool alreadyContainsField = false;
          for_fields(myfield, ct) {
            if (!strcmp(myfield->name, field->name)) {
              alreadyContainsField = true;
              break;
            }
          }
          if (!alreadyContainsField) {
            ct->fields.insertAtHead(field->defPoint->copy());
          }
        }
      }
    } else {
      VarSymbol* super = new VarSymbol("super", pt);
      super->addPragma("super class");
      ct->fields.insertAtHead(new DefExpr(super));
    }
  }
}


//
// Moves expressions that are parsed as nested function definitions
// into their own statement; during parsing, these are embedded in
// call expressions
//
static void normalize_nested_function_expressions(DefExpr* def) {
  if ((!strncmp("_anon_record", def->sym->name, 12)) ||
      (!strncmp("_forallexpr", def->sym->name, 11)) ||
      (!strncmp("_let_fn", def->sym->name, 7)) ||
      (!strncmp("_if_fn", def->sym->name, 6)) ||
      (!strncmp("_reduce_scan", def->sym->name, 12)) ||
      (!strncmp("_forif_fn", def->sym->name, 9))) {
    Expr* stmt = def->getStmtExpr();
    if (!stmt) {
      if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol)) {
        if (ClassType* ct = toClassType(ts->type)) {
          def->replace(new SymExpr(def->sym->name));
          ct->addDeclarations(def, true);
          return;
        }
      }
    }
    def->replace(new SymExpr(def->sym->name));
    stmt->insertBefore(def);
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
  CallExpr* parent = toCallExpr(call->parentExpr);
  if (!parent || !parent->isNamed("=") || parent->get(1) != call)
    return;
  Expr* stmt = parent->getStmtExpr();
  VarSymbol* temp = new VarSymbol("_tuple_destruct");
  stmt->insertBefore(new DefExpr(temp));
  stmt = new CallExpr(PRIMITIVE_MOVE, temp, parent->get(2)->remove());
  parent->replace(stmt);
  int i = 1;
  for_actuals(expr, call) {
    Expr *removed_expr = expr->remove();
    if (SymExpr *sym_expr = toSymExpr(removed_expr)) {
      if (sym_expr->isNamed("_")) {
        i++;
        continue;
      }
    }
    stmt->insertAfter(
      new CallExpr("=", removed_expr,
        new CallExpr(temp, new_IntSymbol(i))));
    i++;
  }
}


static bool
isSparseDomain(Expr* exprType) {
  if (CallExpr* callExprType = toCallExpr(exprType))
    if (callExprType->isNamed("_build_sparse_subdomain_type"))
      return true;
  return false;
}


static void build_type_constructor(ClassType* ct) {
  if (ct->defaultTypeConstructor)
    return;

  currentLineno = ct->lineno;
  currentFilename = ct->filename;

  FnSymbol* fn = new FnSymbol(astr("_type_construct_", ct->symbol->name));
  fn->addPragma("type constructor");
  ct->defaultTypeConstructor = fn;
  fn->cname = astr("_type_construct_", ct->symbol->cname);
  fn->isCompilerTemp = true;
  fn->retTag = RET_TYPE;

  if (ct->symbol->hasPragma("ref"))
    fn->addPragma("ref");
  if (ct->symbol->hasPragma("heap"))
    fn->addPragma("heap");
  if (ct->symbol->hasPragma("tuple")) {
    fn->addPragma("tuple");
    fn->addPragma("inline");
  }

  fn->_this = new VarSymbol("this", ct);
  fn->insertAtTail(new DefExpr(fn->_this));

  Vec<const char*> fieldNamesSet;
  for_fields(tmp, ct) {
    currentLineno = tmp->lineno;
    currentFilename = tmp->filename;
    if (VarSymbol* field = toVarSymbol(tmp)) {

      if (field->hasPragma("super class"))
        continue;

      Expr* exprType = field->defPoint->exprType;
      Expr* init = field->defPoint->init;
      if (!strcmp(field->name, "_promotionType") ||
          field->hasPragma("omit from constructor")) {
        fn->insertAtTail(
          new BlockStmt(
            new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this, 
              new_StringSymbol(field->name),
              new CallExpr(PRIMITIVE_INIT, exprType->remove())),
            BLOCK_TYPE));
      } else {
        fieldNamesSet.set_add(field->name);

        //
        // if formal is generic
        //
        if (field->isTypeVariable || field->isParam || (!exprType && !init)) {
          ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, field->name, field->type);
          fn->insertFormalAtTail(arg);

          if (field->isParam)
            arg->intent = INTENT_PARAM;
          else
            arg->isTypeVariable = true;

          if (init)
            arg->defaultExpr = new BlockStmt(init->copy(), BLOCK_SCOPELESS);

          if (exprType)
            arg->typeExpr = new BlockStmt(exprType->copy(), BLOCK_SCOPELESS);

          if (!exprType && arg->type == dtUnknown)
            arg->type = dtAny;

          if (field->isParam || field->isTypeVariable)
            fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this,
                                          new_StringSymbol(field->name), arg));
          else
            fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this,
                                          new_StringSymbol(field->name),
                                          new CallExpr(PRIMITIVE_INIT, arg)));
        } else if (exprType) {
          if (isSparseDomain(exprType))
            fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this,
                                          new_StringSymbol(field->name),
                                          exprType->copy()));
          else
            fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this,
                                          new_StringSymbol(field->name),
                                          new CallExpr(PRIMITIVE_INIT, exprType->copy())));
        } else if (init) {
          fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this,
                                        new_StringSymbol(field->name),
                                        new CallExpr("_copy", init->copy())));
        }
      }
    }
  }

  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, fn->_this));
  ct->symbol->defPoint->insertBefore(new DefExpr(fn));
  fn->retType = ct;

  //
  // insert implicit uses of 'this' in constructor
  //
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn->body);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* se = toSymExpr(ast))
      if (!se->var && fieldNamesSet.set_in(se->unresolved))
        se->replace(buildDotExpr(fn->_this, se->unresolved));
  }

  ClassType *outerType = toClassType(ct->symbol->defPoint->parentSymbol->type);
  if (outerType) {
    // Create an "outer" pointer to the outer class in the inner class
    VarSymbol* outer = new VarSymbol("outer");


    // Remove the DefPoint for this constructor, add it to the outer
    // class's method list.
    outerType->methods.add(fn);
    fn->_outer = new ArgSymbol(INTENT_BLANK, "outer", outerType);
    fn->insertFormalAtHead(new DefExpr(fn->_outer));
    fn->insertFormalAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken)));
    fn->isMethod = true;
    Expr* insertPoint = outerType->symbol->defPoint;
    while (toTypeSymbol(insertPoint->parentSymbol))
      insertPoint = insertPoint->parentSymbol->defPoint;
    insertPoint->insertBefore(fn->defPoint->remove());

    // Save the pointer to the outer class
    ct->fields.insertAtTail(new DefExpr(outer));
    fn->insertAtHead(new CallExpr(PRIMITIVE_SET_MEMBER,
                                  fn->_this,
                                  new_StringSymbol("outer"),
                                  fn->_outer));
    ct->outer = outer;
  }
}


static void build_constructor(ClassType* ct) {
  if (ct->defaultConstructor)
    return;

  currentLineno = ct->lineno;
  currentFilename = ct->filename;

  if (ct->symbol->hasPragma("sync"))
    ct->defaultValue = NULL;

  FnSymbol* fn = new FnSymbol(astr("_construct_", ct->symbol->name));
  fn->addPragma("default constructor");
  ct->defaultConstructor = fn;
  fn->cname = astr("_construct_", ct->symbol->cname);
  fn->isCompilerTemp = true; // compiler inserted

  if (ct->symbol->hasPragma("ref"))
    fn->addPragma("ref");
  if (ct->symbol->hasPragma("heap"))
    fn->addPragma("heap");
  if (ct->symbol->hasPragma("tuple")) {
    fn->addPragma("tuple");
    fn->addPragma("inline");
  }

  fn->_this = new VarSymbol("this", ct);
  fn->insertAtTail(new DefExpr(fn->_this));

  Map<VarSymbol*,ArgSymbol*> fieldArgMap;
  for_fields(tmp, ct) {
    currentLineno = tmp->lineno;
    currentFilename = tmp->filename;
    if (VarSymbol* field = toVarSymbol(tmp)) {
      if (!field->hasPragma("super class") &&
          !field->hasPragma("omit from constructor") &&
          strcmp(field->name, "_promotionType") &&
          strcmp(field->name, "outer")) {
        ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, field->name, field->type);
        fieldArgMap.put(field, arg);
      }
    }
  }

  ArgSymbol* meme = NULL;
  if (ct->classTag == CLASS_CLASS) {
    if (ct->symbol->hasPragma("ref") || ct->symbol->hasPragma("sync")) {
      fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, fn->_this,
                         new CallExpr(PRIMITIVE_CHPL_ALLOC, fn->_this,
                           new_StringSymbol(astr("instance of class ", ct->symbol->name)))));
    } else {
      meme = new ArgSymbol(INTENT_BLANK, "meme", ct, NULL, new SymExpr(gNil));
      meme->addPragma("is meme");
      BlockStmt* allocate = new BlockStmt();
      allocate->insertAtTail(new CallExpr(PRIMITIVE_MOVE, fn->_this,
                               new CallExpr(PRIMITIVE_CHPL_ALLOC, fn->_this,
                                 new_StringSymbol(astr("instance of class ", ct->symbol->name)))));
      allocate->insertAtTail(new CallExpr(PRIMITIVE_SETCID, fn->_this));
      CondStmt* cond = new CondStmt(
        new CallExpr(PRIMITIVE_PTR_EQUAL, gNil, meme),
        allocate,
        new CallExpr(PRIMITIVE_MOVE, fn->_this, meme));
      fn->insertAtTail(cond);
      if (ct->dispatchParents.n > 0) {
        if (!ct->dispatchParents.v[0]->defaultConstructor) {
          build_type_constructor(toClassType(ct->dispatchParents.v[0]));
          build_constructor(toClassType(ct->dispatchParents.v[0]));
        }
        FnSymbol* superCtor = ct->dispatchParents.v[0]->defaultConstructor;
        CallExpr* superCall = new CallExpr(superCtor->name);
        for_formals_backward(formal, superCtor) {
          if (formal->hasPragma("is meme"))
            continue;
          DefExpr* superArg = formal->defPoint->copy();
          fn->insertFormalAtHead(superArg);
          superCall->insertAtHead(superArg->sym);
        }
        VarSymbol* tmp = new VarSymbol("_tmp");
        superCall->insertAtTail(new NamedExpr("meme", new SymExpr(tmp)));
        cond->insertAfter(superCall);
        superCall->insertBefore(new DefExpr(tmp));
        superCall->insertBefore(
          new CallExpr(PRIMITIVE_MOVE, tmp,
            new CallExpr(PRIMITIVE_GET_MEMBER_VALUE,
                         fn->_this, new_StringSymbol("super"))));
      }
    }
  }

  if (ct->classTag == CLASS_UNION)
    fn->insertAtTail(new CallExpr(PRIMITIVE_UNION_SETID, fn->_this, new_IntSymbol(0)));

  ct->symbol->defPoint->insertBefore(new DefExpr(fn));

  Vec<const char*> fieldNamesSet;
  for_fields(tmp, ct) {
    VarSymbol* field = toVarSymbol(tmp);

    if (!field)
      continue;

    ArgSymbol* arg = fieldArgMap.get(field);

    if (!arg)
      continue;

    currentLineno = field->lineno;
    currentFilename = field->filename;

    if (field->isParam)
      arg->intent = INTENT_PARAM;
    fieldNamesSet.set_add(field->name);
    Expr* exprType = field->defPoint->exprType->remove();
    Expr* init = field->defPoint->init->remove();

    bool hasType = exprType;
    bool hasInit = init;
    if (init) {
      if (!field->isTypeVariable && !exprType) {
        exprType = new CallExpr(PRIMITIVE_TYPEOF, new CallExpr("_copy", init->copy()));
      }
    } else if (exprType && !field->isTypeVariable && !field->isParam) {
      if (isSparseDomain(exprType))
        init = exprType->copy();
      else
        init = new CallExpr(PRIMITIVE_INIT, exprType->copy());
    }
    if (hasType && !field->isTypeVariable && !field->isParam) {
      if (!isSparseDomain(exprType))
        init = new CallExpr("_createFieldDefault", exprType->copy(), init);
    }
    if (!hasType && !field->isTypeVariable && !field->isParam) {
      init = new CallExpr("_copy", init);
    }
    if (init) {
      if (hasInit)
        arg->defaultExpr = new BlockStmt(init, BLOCK_SCOPELESS);
      else
        arg->defaultExpr = new BlockStmt(new SymExpr(gNil));
    }
    if (exprType)
      arg->typeExpr = new BlockStmt(exprType, BLOCK_SCOPELESS);
    arg->isTypeVariable = field->isTypeVariable;
    if (!exprType && arg->type == dtUnknown)
      arg->type = dtAny;
    fn->insertFormalAtTail(arg);
    fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this, 
                                  new_StringSymbol(arg->name),
                                  arg));
  }

  if (meme)
    fn->insertFormalAtTail(meme);

  //
  // insert implicit uses of 'this' in constructor
  //
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn->body);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* se = toSymExpr(ast))
      if (!se->var && fieldNamesSet.set_in(se->unresolved))
        se->replace(buildDotExpr(fn->_this, se->unresolved));
  }

  currentLineno = ct->lineno;
  currentFilename = ct->filename;

  ClassType *outerType = toClassType(ct->symbol->defPoint->parentSymbol->type);
  if (outerType) {
    // Remove the DefPoint for this constructor, add it to the outer
    // class's method list.
    outerType->methods.add(fn);
    fn->_outer = new ArgSymbol(INTENT_BLANK, "outer", outerType);
    fn->insertFormalAtHead(new DefExpr(fn->_outer));
    fn->insertFormalAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken)));
    fn->isMethod = true;
    Expr* insertPoint = outerType->symbol->defPoint;
    while (toTypeSymbol(insertPoint->parentSymbol))
      insertPoint = insertPoint->parentSymbol->defPoint;
    insertPoint->insertBefore(fn->defPoint->remove());

    // Save the pointer to the outer class
    fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER,
                                  fn->_this,
                                  new_StringSymbol("outer"),
                                  fn->_outer));
  }

  //
  // insert call to initialize method if one is defined; make the
  // constructor return the result of the initialize method if it
  // returns a value (otherwise return this)
  //
  bool insertedReturn = false;
  forv_Vec(FnSymbol, method, ct->methods) {
    if (method && !strcmp(method->name, "initialize")) {
      if (method->numFormals() == 2) {
        CallExpr* init = new CallExpr("initialize", gMethodToken, fn->_this);
        Vec<BaseAST*> asts;
        collect_top_asts(&asts, method->body);
        forv_Vec(BaseAST, ast, asts) {
          if (CallExpr* call = toCallExpr(ast)) {
            if (call->isPrimitive(PRIMITIVE_RETURN)) {
              if (call->get(1) && call->get(1)->typeInfo() != dtVoid) {
                init = new CallExpr(PRIMITIVE_RETURN, init);
                insertedReturn = true;
                break;
              }
            }
          }
        }
        fn->insertAtTail(init);
        break;
      }
    }
  }
  if (!insertedReturn)
    fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, fn->_this));
}


static void flatten_primary_methods(FnSymbol* fn) {
  if (TypeSymbol* ts = toTypeSymbol(fn->defPoint->parentSymbol)) {
    Expr* insertPoint = ts->defPoint;
    while (toTypeSymbol(insertPoint->parentSymbol))
      insertPoint = insertPoint->parentSymbol->defPoint;
    DefExpr* def = fn->defPoint;
    def->remove();
    insertPoint->insertBefore(def);
    if (fn->userString && fn->name != ts->name)
      fn->userString = astr(ts->name, ".", fn->userString);
    if (ts->hasPragma("sync"))
      fn->addPragma("sync");
  }
}


static void change_cast_in_where(FnSymbol* fn) {
  if (fn->where) {
    Vec<BaseAST*> asts;
    collect_asts(&asts, fn->where);
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = toCallExpr(ast)) {
        if (call->isNamed("_cast")) {
          call->primitive = primitives[PRIMITIVE_ISSUBTYPE];
          call->baseExpr->remove();
        }
      }
    }
  }
}


//
// Make all modules call their parent module's init function to make sure
// it has been run and the module's symbols are initialized.  This is NOT
// the same as using the outer module.
//
static void initializeOuterModules(ModuleSymbol* mod) {
  for_alist(stmt, mod->block->body) {
    if (BlockStmt* b = toBlockStmt(stmt))
      stmt = b->body.first();
    if (DefExpr* def = toDefExpr(stmt)) {
      if (ModuleSymbol* m = toModuleSymbol(def->sym)) {
        if (mod != theProgram) {
          if (!m->initFn || !mod->initFn)
            INT_FATAL("Module with no initialization function");
          m->initFn->insertAtHead(new CallExpr(mod->initFn));
        }
        initializeOuterModules(m);
      }
    }
  }
}


void cleanup(void) {
  initializeOuterModules(theProgram);

  Vec<BaseAST*> asts;
  collect_asts(&asts);

  // handle forall's in array type declaration with initialization
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr *call = toCallExpr(ast)) {
      if (call->isNamed("_build_array_type") && call->numActuals() == 4) {
        if (DefExpr *def = toDefExpr(call->parentExpr)) {
          CallExpr *tinfo = toCallExpr(def->exprType);
          Expr *indices = tinfo->get(3);
          Expr *iter = tinfo->get(4);
          indices->remove();
          iter->remove();
          if (def->init) {
            BlockStmt *forblk = buildForLoopExpr(indices, iter, def->init->copy());
            
            FnSymbol *forall_init = new FnSymbol("_forallinit");
            forall_init->insertAtTail(forblk);
            def->insertBefore(new DefExpr(forall_init));
            def->init->replace(new CallExpr(forall_init));
          }
        } else {
          USR_FATAL(call, "unhandled case of array type loop expression");
        }
      }
    }
  }

  asts.clear();
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (DefExpr* def = toDefExpr(ast)) {
      normalize_nested_function_expressions(def);
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->blockTag == BLOCK_SCOPELESS && block->list)
        flatten_scopeless_block(block);
    } else if (CallExpr* call = toCallExpr(ast)) {
      if (call->isNamed("_build_tuple"))
        destructure_tuple(call);
    } else if (DefExpr* def = toDefExpr(ast)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        flatten_primary_methods(fn);
        change_cast_in_where(fn);
      }
    }
  }

  // handle "use mod;" where mod is a module
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_USE))
        process_import_expr(call);
    }
  }

  forv_Vec(TypeSymbol, ts, gTypes) {
    if (ClassType* ct = toClassType(ts->type)) {
      add_class_to_hierarchy(ct);

      //
      // add field to empty records and unions
      //
      if (ct->classTag != CLASS_CLASS && ct->fields.isEmpty())
        ct->fields.insertAtHead(
          new DefExpr(
            new VarSymbol("emptyRecordPlaceholder"),
              new SymExpr(new_IntSymbol(0))));
    }
  }

  forv_Vec(TypeSymbol, ts, gTypes) {
    if (ClassType* ct = toClassType(ts->type)) {
      currentLineno = ts->lineno;
      currentFilename = ts->filename;
      build_type_constructor(ct);
      build_constructor(ct);
    }
  }
}
