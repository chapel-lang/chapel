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
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"


//
// Move statements in begin and cobegin blocks into nested functions
//
static void
encapsulateBegins() {
  int uid = 1;

  forv_Vec(BaseAST, ast, gAsts) {
    if (BlockStmt *b = toBlockStmt(ast)) {
      if (b->blockTag == BLOCK_BEGIN) {
        char *fname = astr("_begin_block", intstring(uid++));
        FnSymbol *fn = new FnSymbol(fname);
        fn->retType = dtVoid;
        for_alist(stmt, b->body) {
          stmt->remove();
          fn->insertAtTail(stmt);
        }
        b->insertAtTail(new DefExpr(fn));
        b->insertAtTail(new CallExpr(fname));
      } else if (b->blockTag == BLOCK_COBEGIN) {
        for_alist(stmt, b->body) {
          char *fname = astr("_cobegin_stmt", intstring(uid++));
          FnSymbol *fn = new FnSymbol(fname);
          fn->retType = dtVoid;
          b->insertAtHead(new DefExpr(fn));
          stmt->insertBefore(new CallExpr(fname));
          stmt->remove();
          fn->insertAtTail(stmt);
        }
      }
    }
  }
}


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

  if (!useCall) {
    CallExpr* call = toCallExpr(expr);
    if (!call)
      INT_FATAL(call, "Bad use statement in getUsedModule");
    if (!call->isPrimitive(PRIMITIVE_USE))
      INT_FATAL(call, "Bad use statement in getUsedModule");
    return getUsedModule(call->get(1), call);
  }

  if (SymExpr* sym = toSymExpr(expr)) {
    mod = toModuleSymbol(useCall->parentScope->lookup(sym->var->name));
    if (!mod)
      USR_FATAL(useCall, "Cannot find module '%s'", sym->var->name);
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

    mod = toModuleSymbol(lhs->lookup(rhsName));
    if (!mod)
      USR_FATAL(useCall, "Cannot find module '%s'", rhsName);
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
  if (mod == theProgram)
    USR_FATAL(call, "Cannot use module %s", mod->name);
  call->getStmtExpr()->insertBefore(new CallExpr(mod->initFn));
  if (call->getFunction() == call->getModule()->initFn)
    call->getModule()->block->blkScope->addModuleUse(mod);
  else
    call->parentScope->addModuleUse(mod);
  call->getStmtExpr()->remove();
}

//
// Compute dispatchParents and dispatchChildren vectors; add base
// class fields to subclasses; identify cyclic or illegal class or
// record hierarchies.
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
    for_fields_backward(field, pt) {
      if (toVarSymbol(field))
        ct->fields.insertAtHead(field->defPoint->copy());
    }
  }
}


static BlockStmt*
getBlock(Expr* stmt) {
  if (BlockStmt* block = toBlockStmt(stmt)) {
    if (block->blkScope)
      return block;
  }
  return getBlock(stmt->parentExpr);
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
    if (i > 1) {
      Expr *removed_expr = expr->remove();
      if (SymExpr *sym_expr = toSymExpr(removed_expr)) {
        if (!strcmp(sym_expr->var->name, "_")) {
          i++;
          continue;
        }
      }
      stmt->insertAfter(
        new CallExpr("=", removed_expr,
          new CallExpr(temp, new_IntSymbol(i-1))));
    }
    i++;
  }
}


static void build_constructor(ClassType* ct) {
  if (ct->defaultConstructor)
    return;

  currentLineno = ct->lineno;
  currentFilename = ct->filename;

  if (ct->symbol->hasPragma("sync"))
    ct->defaultValue = NULL;
  char* name;
  if (!toClassType(ct->symbol->defPoint->parentSymbol->type)) {
    name = astr("_construct_", ct->symbol->name);
  } else {
    name = astr(ct->symbol->name);
  }
  FnSymbol* fn = new FnSymbol(name);
  ct->defaultConstructor = fn;
  fn->fnClass = FN_CONSTRUCTOR;
  if (!toClassType(ct->symbol->defPoint->parentSymbol->type)) {
    fn->cname = astr("_construct_", ct->symbol->cname);
  } else {
    fn->cname = astr(ct->symbol->cname);
  }
  fn->isCompilerTemp = true; // compiler inserted
  if (ct->symbol->hasPragma("ref"))
    fn->addPragma("ref");

  if (ct->symbol->hasPragma("tuple")) {
    fn->addPragma("tuple");
    fn->addPragma("inline");
  }

  fn->_this = new VarSymbol("this", ct);
  fn->insertAtTail(new DefExpr(fn->_this));
  if (ct->classTag == CLASS_CLASS)
    fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, fn->_this,
                       new CallExpr(PRIMITIVE_CHPL_ALLOC,
                         ct->symbol,
                         new_StringSymbol(astr("instance of class ",
                                               ct->symbol->name)))));
  if (ct->classTag == CLASS_CLASS)
    fn->insertAtTail(new CallExpr(PRIMITIVE_SETCID, fn->_this));

  if (ct->classTag == CLASS_UNION)
    fn->insertAtTail(new CallExpr(PRIMITIVE_UNION_SETID, fn->_this, new_IntSymbol(0)));

  ct->symbol->defPoint->insertBefore(new DefExpr(fn));

  Vec<const char*> fieldNamesSet;
  for_fields(tmp, ct) {
    VarSymbol* field = toVarSymbol(tmp);

    if (!field)
      continue;

    currentLineno = field->lineno;
    currentFilename = field->filename;

    bool isParam = field->consClass == VAR_PARAM;

    ArgSymbol* arg = new ArgSymbol((isParam) ? INTENT_PARAM : INTENT_BLANK,
                                   field->name, field->type);
    fieldNamesSet.set_add(field->name);
    Expr* exprType = field->defPoint->exprType->remove();
    Expr* init = field->defPoint->init->remove();

    bool hasType = exprType;
    if (init) {
      if (!field->isTypeVariable && !exprType) {
        exprType = init->copy();
        arg->initUsingCopy = true;
      }
    } else if (exprType && !field->isTypeVariable && field->consClass != VAR_PARAM) {
      init = new CallExpr("_init", exprType->copy());
      arg->initUsingCopy = true;
    }
    arg->defaultExpr = init;
    arg->isTypeVariable = field->isTypeVariable;
    if (!exprType && arg->type == dtUnknown)
      arg->type = dtAny;
    DefExpr* def = new DefExpr(arg, NULL, exprType);
    fn->insertFormalAtTail(def);

    if (!ct->symbol->hasPragma("ref") && !arg->isTypeVariable && arg->intent != INTENT_PARAM) {
      if (hasType) {
        VarSymbol* tmp = new VarSymbol("_tmp");
        fn->insertAtTail(new DefExpr(tmp));
        Expr* init = new CallExpr("_init", exprType->copy());
        fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, init));
        fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr("_set_field", tmp, arg)));
        fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this, 
                                      new_StringSymbol(arg->name), tmp));
      } else {
        fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this, 
                                      new_StringSymbol(arg->name),
                                      new CallExpr("_copy", arg)));
      }
    } else {
      fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this, 
                                    new_StringSymbol(arg->name),
                                    arg));
    }
  }

  Vec<BaseAST*> asts;
  collect_asts(&asts, fn->body);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* se = dynamic_cast<SymExpr*>(ast))
      if (isUnresolvedSymbol(se->var))
        if (fieldNamesSet.set_in(se->var->name))
          se->replace(new CallExpr(".", fn->_this, new_StringSymbol(se->var->name)));
  }

  currentLineno = ct->lineno;
  currentFilename = ct->filename;

  Symbol* myThis = fn->_this;
  ClassType *outerType =
    toClassType(ct->symbol->defPoint->parentSymbol->type);
  if (outerType) {
    // Create an "outer" pointer to the outer class in the inner class
    VarSymbol* outer = new VarSymbol("outer");


    // Remove the DefPoint for this constructor, add it to the outer
    // class's method list.
    outerType->addDeclarations(fn->defPoint->remove(), true);

    // Save the pointer to the outer class
    ct->fields.insertAtHead(new DefExpr(outer));
    fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER,
                                  new SymExpr(myThis),
                                  new_StringSymbol("outer"),
                                  new SymExpr(fn->_this)));
    ct->outer = outer;
  }

  forv_Vec(FnSymbol, method, ct->methods) {
    if (method && !strcmp(method->name, "initialize")) {
      if (method->numFormals() == 2) {
        fn->insertAtTail(new CallExpr("initialize"));
        break;
      }
    }
  }

  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, myThis));
  fn->retType = ct;
}


static void flatten_primary_methods(FnSymbol* fn) {
  if (TypeSymbol* ts = toTypeSymbol(fn->defPoint->parentSymbol)) {
    Expr* insertPoint = ts->defPoint;
    while (toTypeSymbol(insertPoint->parentSymbol))
      insertPoint = insertPoint->parentSymbol->defPoint;
    DefExpr* def = fn->defPoint;
    def->remove();
    insertPoint->insertBefore(def);
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
  encapsulateBegins();

  initializeOuterModules(theProgram);

  Vec<BaseAST*> asts;

  // handle "use mod;" where mod is a module
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_USE))
        process_import_expr(call);
      if (call->isPrimitive(PRIMITIVE_YIELD))
        call->getFunction()->fnClass = FN_ITERATOR;
    }
  }

  // handle forall's in array type declaration with initialization
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr *call = toCallExpr(ast)) {
      if (call->isNamed("_build_array_type") && call->numActuals() == 4) {
        if (call->getStmtExpr()) {
          if (DefExpr *def = toDefExpr(call->getStmtExpr())) {
            CallExpr *tinfo = toCallExpr(def->exprType);
            Expr *indices = tinfo->get(3);
            Expr *iter = tinfo->get(4);
            indices->remove();
            iter->remove();
            if (def->init) {
              BlockStmt *forblk = build_for_expr(indices, iter, def->init->copy());
            
              FnSymbol *forall_init = new FnSymbol("_forallinit");
              forall_init->fnClass = FN_ITERATOR;
              forall_init->insertAtTail(forblk);
              def->insertBefore(new DefExpr(forall_init));
              def->init->replace(new CallExpr(forall_init));
            }
          } else {
            INT_FATAL(call, "missing parent def expr");
          }
        } else {
          INT_FATAL(call, "missing parent stmt");
        }
      }
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (FnSymbol *fn = toFnSymbol(ast)) {
      for_formals(arg, fn) {
        SymExpr *s = toSymExpr(arg->defaultExpr);
        if (!fn->instantiatedFrom &&
            s && 
            !arg->defPoint->exprType &&
            !arg->isTypeVariable) {
          if (s->var->type != dtNil) {
            arg->defPoint->exprType = new CallExpr(PRIMITIVE_TYPEOF,
                                                   arg->defaultExpr->copy());
            insert_help(arg->defPoint->exprType, arg->defPoint, arg, fn->argScope);
            arg->type = dtUnknown;
          }
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
      if (block->blockTag == BLOCK_SCOPELESS)
        flatten_scopeless_block(block);
    } else if (CallExpr* call = toCallExpr(ast)) {
      if (call->isNamed("_tuple"))
        destructure_tuple(call);
    } else if (DefExpr* def = toDefExpr(ast)) {
      if (TypeSymbol* ts = toTypeSymbol(def->sym)) {
        if (ClassType* ct = toClassType(ts->type)) {
          add_class_to_hierarchy(ct);
        }
      } else if (FnSymbol* fn = toFnSymbol(def->sym)) {
        flatten_primary_methods(fn);
        change_cast_in_where(fn);
      }
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (TypeSymbol* ts = toTypeSymbol(ast)) {
      if (ClassType* ct = toClassType(ts->type)) {
        build_constructor(ct);
        if (ct->defaultConstructor->isMethod) {
          // This is a nested class constructor
          flatten_primary_methods(ct->defaultConstructor);
        }
      }
    }
  }
}
