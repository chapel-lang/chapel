//
// cleanup
//
// This pass cleans up the AST after parsing. It handles
// transformations that would be difficult to do while parsing.
//

#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "runtime.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

//
// Move the statements in a block out of the block
//
static void
flatten_scopeless_block(BlockStmt* block) {
  for_alist(Expr, stmt, block->body) {
    stmt->remove();
    block->insertBefore(stmt);
  }
  block->remove();
}


//
// Return the module imported by a use call.  The module returned could be
// nested: e.g. "use outermost.middle.innermost;"
//
ModuleSymbol* getUsedModule(CallExpr* call) {
  ModuleSymbol* mod = NULL;
  char* name = NULL;

  if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
    name = sym->var->name;
    mod = dynamic_cast<ModuleSymbol*>(sym->lookup(name));

    if (mod && call->argList->length() == 2) {
      SymExpr* sym2 = dynamic_cast<SymExpr*>(call->get(2));
      VarSymbol* sym2var = dynamic_cast<VarSymbol*>(sym2->var);
      name = sym2var->immediate->v_string;
      mod = dynamic_cast<ModuleSymbol*>(sym->lookup(name));
    }
  } else if (CallExpr* c = dynamic_cast<CallExpr*>(call->get(1))) {
    if (c->isNamed(".")) {
      mod = getUsedModule(c);
      if (mod && call->argList->length() == 2) {
        SymExpr* sym2 = dynamic_cast<SymExpr*>(call->get(2));
        VarSymbol* sym2var = dynamic_cast<VarSymbol*>(sym2->var);
        name = sym2var->immediate->v_string;
        mod = dynamic_cast<ModuleSymbol*>(mod->lookup(name));
      }
    }
  }
  if (!mod)
    USR_FATAL(call, "Cannot find module '%s'", name);
  return mod;
}


//
// Make all modules nested within outerMod import all the modules outerMod
// imports. Inner modules have access to all symbols the outer module does.
//
static void
addInnerModuleUses(ModuleSymbol* modUsed, ModuleSymbol* outerMod) {
  forv_Vec(ModuleSymbol, subModule, outerMod->subModules) {
    if (!subModule->block->modUses.in(modUsed)) {
      subModule->block->blkScope->addModuleUse(modUsed);
      addInnerModuleUses(modUsed, subModule);
    }
  }
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
  if (mod != compilerModule)
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
  if (ct->inherits->length() == 0 && !ct->symbol->hasPragma("no object")) {
    if (ct->classTag == CLASS_RECORD) {
      ct->dispatchParents.add(dtValue);
      dtValue->dispatchChildren.add(ct);
    } else if (ct->classTag == CLASS_CLASS) {
      ct->dispatchParents.add(dtObject);
      dtObject->dispatchChildren.add(ct);
    }
  }

  for_alist(Expr, expr, ct->inherits) {
    TypeSymbol* ts = dynamic_cast<TypeSymbol*>(expr->lookup(expr));
    expr->remove();
    if (!ts)
      USR_FATAL(expr, "Illegal super class");
    ClassType* pt = dynamic_cast<ClassType*>(ts->type);
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
    if (pt->inherits) {
      localSeenPtr->set_add(ct);
      add_class_to_hierarchy(pt, localSeenPtr);
    }
    ct->dispatchParents.add(pt);
    pt->dispatchChildren.add(ct);
    for_fields_backward(field, pt) {
      if (dynamic_cast<VarSymbol*>(field))
        ct->fields->insertAtHead(field->defPoint->copy());
    }
  }
}


static BlockStmt*
getBlock(Expr* stmt) {
  if (BlockStmt* block = dynamic_cast<BlockStmt*>(stmt)) {
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
  CallExpr* parent = dynamic_cast<CallExpr*>(call->parentExpr);
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
      if (SymExpr *sym_expr = dynamic_cast<SymExpr *>(removed_expr)) {
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

  if (ct->symbol->hasPragma("synchronization primitive"))
    ct->defaultValue = NULL;
  char* name;
  if (!dynamic_cast<ClassType*>(ct->symbol->defPoint->parentSymbol->type)) {
    name = astr("_construct_", ct->symbol->name);
  } else {
    name = astr(ct->symbol->name);
  }
  FnSymbol* fn = new FnSymbol(name);
  ct->defaultConstructor = fn;
  fn->fnClass = FN_CONSTRUCTOR;
  if (!dynamic_cast<ClassType*>(ct->symbol->defPoint->parentSymbol->type)) {
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

  ASTMap field2formal;
  for_fields(tmp, ct) {
    if (!dynamic_cast<VarSymbol*>(tmp))
      continue;
    char* name = tmp->name;
    Type* type = tmp->type;
    Expr* exprType = tmp->defPoint->exprType;
    if (exprType)
      exprType->remove();
    Expr* init = tmp->defPoint->init;
    if (init) {
      init->remove();
      if (!tmp->isTypeVariable && !exprType)
        exprType = init->copy();
    } else if (!tmp->isTypeVariable)
      init = new SymExpr(gNil);
    VarSymbol *vtmp = dynamic_cast<VarSymbol*>(tmp);
    ArgSymbol* arg = new ArgSymbol((vtmp && vtmp->consClass == VAR_PARAM) ? INTENT_PARAM : INTENT_BLANK, name, type, init);
    DefExpr* defExpr = new DefExpr(arg, NULL, exprType);
    field2formal.put(tmp, arg);
    arg->isTypeVariable = tmp->isTypeVariable;
    if (!exprType && arg->type == dtUnknown)
      arg->type = dtAny;
    fn->insertFormalAtTail(defExpr);
  }

  reset_file_info(fn, ct->symbol->lineno, ct->symbol->filename);

  // Make the line numbers for the formals point to the fields they
  // map to, not the first line of the class definition.
  for_fields(field, ct) {
    if (Symbol* formal = dynamic_cast<Symbol*>(field2formal.get(field))) {
      formal->lineno = field->lineno;
      formal->defPoint->lineno = field->defPoint->lineno;
      formal->filename = field->filename;
      formal->defPoint->filename = field->defPoint->filename;
    }
  }
  ct->symbol->defPoint->insertBefore(new DefExpr(fn));

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
  // assign formals to fields by name
  for_fields(field, ct) {
    for_formals(formal, fn) {
      if (!formal->variableExpr && !strcmp(formal->name, field->name)) {
        CallExpr* call = dynamic_cast<CallExpr*>(formal->defPoint->exprType);
        if (call && call->isNamed("_build_array_type")) {
          VarSymbol* tmp = new VarSymbol("_tmp");
          fn->insertAtTail(new DefExpr(tmp, NULL, call->copy()));
          fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this, 
                                        new_StringSymbol(field->name), tmp));
          fn->insertAtTail(new CondStmt(
            new CallExpr("!=", dtNil->symbol, formal),
            new CallExpr("=",
              new CallExpr(".",
                           fn->_this,
                           new_StringSymbol(field->name)),
                         formal)));
        } else {
          Expr* assign_expr = new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this, 
                                           new_StringSymbol(field->name), formal);
          fn->insertAtTail(assign_expr);
        }
      }
    }
  }

  Symbol* myThis = fn->_this;
  ClassType *outerType =
    dynamic_cast<ClassType*>(ct->symbol->defPoint->parentSymbol->type);
  if (outerType) {
    // Create an "outer" pointer to the outer class in the inner class
    VarSymbol* outer = new VarSymbol("outer");


    // Remove the DefPoint for this constructor, add it to the outer
    // class's method list.
    outerType->addDeclarations(new AList(fn->defPoint->remove()), true);

    // Save the pointer to the outer class
    ct->fields->insertAtHead(new DefExpr(outer));
    fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER,
                                  new SymExpr(myThis),
                                  new_StringSymbol("outer"),
                                  new SymExpr(fn->_this)));
    ct->outer = outer;
  }

  forv_Vec(FnSymbol, method, ct->methods) {
    if (method && !strcmp(method->name, "initialize")) {
      if (method->formals->length() == 2) {
        fn->insertAtTail(new CallExpr("initialize"));
        break;
      }
    }
  }

  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, myThis));
  fn->retType = ct;
}


static void flatten_primary_methods(FnSymbol* fn) {
  if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(fn->defPoint->parentSymbol)) {
    Expr* insertPoint = ts->defPoint;
    while (dynamic_cast<TypeSymbol*>(insertPoint->parentSymbol))
      insertPoint = insertPoint->parentSymbol->defPoint;
    DefExpr* def = fn->defPoint;
    def->remove();
    insertPoint->insertBefore(def);
    if (ts->hasPragma( "synchronization primitive"))
      fn->addPragma( "synchronization primitive");
  }
}


static void add_this_formal_to_method(FnSymbol* fn) {
  return;
  if (fn->hasPragma("ref this")) {
    DefExpr* thisDef = fn->_this->defPoint;
    Expr* type = thisDef->exprType;
    CallExpr* ref = new CallExpr("ref");
    thisDef->exprType->replace(ref);
    ref->insertAtTail(type);
  }
}


static void change_cast_in_where(FnSymbol* fn) {
  if (fn->where) {
    Vec<BaseAST*> asts;
    collect_asts(&asts, fn->where);
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
        if (call->isNamed("_cast")) {
          call->primitive = primitives[PRIMITIVE_ISSUBTYPE];
          call->baseExpr->remove();
        }
      }
    }
  }
}


void cleanup(void) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);

  // handle "use mod;" where mod is a module
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isPrimitive(PRIMITIVE_USE))
        process_import_expr(call);
      if (call->isPrimitive(PRIMITIVE_YIELD))
        call->getFunction()->fnClass = FN_ITERATOR;
    }
  }

  // Make nested modules use all the modules the outer module uses
  forv_Vec(ModuleSymbol, mod, allModules) {
    forv_Vec(ModuleSymbol, usedModule, mod->block->modUses) {
      addInnerModuleUses(usedModule, mod);
    }
  }

  // handle forall's in array type declaration with initialization
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr *call = dynamic_cast<CallExpr*>( ast)) {
      if (call->isNamed( "_build_array_type") && call->argList->length() == 4) {
        if (call->getStmtExpr()) {
          if (DefExpr *def = dynamic_cast<DefExpr*>(call->getStmtExpr())) {
            CallExpr *tinfo = dynamic_cast<CallExpr*>(def->exprType);
            Expr *indices = tinfo->get(3);
            Expr *iter = tinfo->get(4);
            indices->remove();
            iter->remove();
            BlockStmt *forblk = build_for_expr(indices, iter, def->init->copy());
            
            FnSymbol *forall_init = new FnSymbol( "_forallinit");
            forall_init->fnClass = FN_ITERATOR;
            forall_init->insertAtTail( forblk);
            def->insertBefore( new DefExpr( forall_init));
            def->init->replace( new CallExpr( forall_init));
          } else {
            INT_FATAL( call, "missing parent def expr");
          }
        } else {
          INT_FATAL( call, "missing parent stmt");
        }
      }
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (FnSymbol *fn = dynamic_cast<FnSymbol*>(ast)) {
      for_formals( arg, fn) {
        SymExpr *s = dynamic_cast<SymExpr*>(arg->defaultExpr);
        if (!fn->instantiatedFrom &&
            s && 
            !arg->defPoint->exprType &&
            !arg->isTypeVariable) {
          if (s->var->type != dtNil) {
            arg->defPoint->exprType = new CallExpr( PRIMITIVE_TYPEOF,
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
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      normalize_nested_function_expressions(def);
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (BlockStmt* block = dynamic_cast<BlockStmt*>(ast)) {
      if (block->blockTag == BLOCK_SCOPELESS)
        flatten_scopeless_block(block);
    } else if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isNamed("_tuple"))
        destructure_tuple(call);
    } else if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(def->sym)) {
        if (ClassType* ct = dynamic_cast<ClassType*>(ts->type)) {
          add_class_to_hierarchy(ct);
        }
      } else if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def->sym)) {
        flatten_primary_methods(fn);
        add_this_formal_to_method(fn);
        change_cast_in_where(fn);
      }
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(ast)) {
      if (ClassType* ct = dynamic_cast<ClassType*>(ts->type)) {
        build_constructor(ct);
        if (ct->defaultConstructor->isMethod) {
          // This is a nested class constructor
          flatten_primary_methods(ct->defaultConstructor);
        }
      }
    }
  }
}
