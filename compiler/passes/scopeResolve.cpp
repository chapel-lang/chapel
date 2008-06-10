#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "stmt.h"
#include "passes.h"
#include "stringutil.h"
#include "symbol.h"

//
// The symbolTable maps BaseAST* pointers to entries based on scope
// definitions.  The following BaseAST subtypes define scopes:
//
//   FnSymbol: defines a scope mainly for its arguments but also for
//   identifiers that are defined via query-expressions, e.g., 't' in
//   'def f(x: ?t)'
//
//   TypeSymbol: defines a scope for EnumType and ClassType types for
//   the enumerated type constants or the class/record fields
//
//   BlockStmt: defines a scope if the block is not BLOCK_SCOPELESS
//   for any locally defined symbols
//
// Each entry contains a map from canonicalized string pointers to the
// symbols defined in the scope.
//
typedef Map<const char*,Symbol*> SymbolTableEntry;
typedef Map<BaseAST*,SymbolTableEntry*> SymbolTable;
static SymbolTable symbolTable;

//
// getScope returns the BaseAST that corresponds to the scope where
// 'ast' exists; 'ast' must be an Expr or a Symbol.  Note that if you
// pass this a BaseAST that defines a scope, the BaseAST that defines
// the scope where it exists will be returned.  Thus if a BlockStmt
// nested in another BlockStmt is passed to getScope, the outer
// BlockStmt will be returned.
//
static BaseAST*
getScope(BaseAST* ast) {
  if (Expr* expr = toExpr(ast)) {
    BlockStmt* block = toBlockStmt(expr->parentExpr);
    if (block && block->blockTag != BLOCK_SCOPELESS) {
      return block;
    } else if (expr->parentExpr) {
      return getScope(expr->parentExpr);
    } else if (FnSymbol* fn = toFnSymbol(expr->parentSymbol)) {
      return fn;
    } else if (TypeSymbol* ts = toTypeSymbol(expr->parentSymbol)) {
      if (isEnumType(ts->type) || isClassType(ts->type)) {
        return ts;
      }
    }
    if (expr->parentSymbol == rootModule)
      return NULL;
    else
      return getScope(expr->parentSymbol->defPoint);
  } else if (Symbol* sym = toSymbol(ast)) {
    if (sym == rootModule)
      return NULL;
    else
      return getScope(sym->defPoint);
  }
  INT_FATAL(ast, "getScope expects an Expr or a Symbol");
  return NULL;
}

//
// addToSymbolTable adds the asts in a vector to the global
// symbolTable such that symbol definitions are added to entries in
// the table and new enclosing asts become entries
//
static void
addToSymbolTable(Vec<BaseAST*>& asts) {
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (!def->sym->isCompilerTemp) {
        BaseAST* scope = getScope(def);
        SymbolTableEntry* entry = symbolTable.get(scope);
        if (!entry) {
          entry = new SymbolTableEntry();
          symbolTable.put(scope, entry);
        }
        if (Symbol* sym = entry->get(def->sym->name)) {
          FnSymbol* oldFn = toFnSymbol(sym);
          FnSymbol* newFn = toFnSymbol(def->sym);
          TypeSymbol* typeScope = toTypeSymbol(scope);
          if (!typeScope || !isClassType(typeScope->type)) { // inheritance
            if ((!oldFn || (oldFn && !oldFn->_this && oldFn->noParens)) &&
                (!newFn || (newFn && !newFn->_this && newFn->noParens))) {
              USR_FATAL(sym, "'%s' has multiple definitions, redefined at:\n  %s", sym->name, def->sym->stringLoc());
            }
          }
          if (!newFn || (newFn && !newFn->_this && newFn->noParens))
            entry->put(def->sym->name, def->sym);
        } else {
          entry->put(def->sym->name, def->sym);
        }
      }
    }
  }
}

//
// delete the symbol table
//
static void
destroyTable() {
  Vec<SymbolTableEntry*> entries;
  symbolTable.get_values(entries);
  forv_Vec(SymbolTableEntry, entry, entries) {
    delete entry;
  }
  symbolTable.clear();
}


static void
buildBreadthFirstUseTree(Vec<ModuleSymbol*>* current,
                         Vec<Vec<ModuleSymbol*>*> &queue,
                         Vec<ModuleSymbol*>* alreadySeen = NULL) {
  if (!alreadySeen) {
    Vec<ModuleSymbol*> seen;
    return buildBreadthFirstUseTree(current, queue, &seen);
  }
  queue.add(current);
  Vec<ModuleSymbol*>* next = new Vec<ModuleSymbol*>;
  forv_Vec(ModuleSymbol, module, *current) {
    forv_Vec(ModuleSymbol, mod, module->block->modUses) {
      if (!alreadySeen->set_in(mod)) {
        next->add(mod);
        alreadySeen->set_add(mod);
      }
    }
  }
  if (next->n > 0) {
    buildBreadthFirstUseTree(next, queue, alreadySeen);
  } else {
    delete next;
  }
}


static Symbol*
lookup(BaseAST* scope,
       const char* name,
       Vec<BaseAST*>* alreadyVisited = NULL,
       bool scanModuleUses = true) {
  Vec<BaseAST*> nestedscopes;
  if (!alreadyVisited) {
    return lookup(scope, name, &nestedscopes, scanModuleUses);
  }
  if (!scanModuleUses) {
    nestedscopes.copy(*alreadyVisited);
    alreadyVisited = &nestedscopes;
  }

  if (alreadyVisited->set_in(scope))
    return NULL;
  alreadyVisited->set_add(scope);

  Vec<Symbol*> symbols;
  Symbol* sym = NULL;
  SymbolTableEntry* entry = symbolTable.get(scope);
  if (entry)
    sym = entry->get(name);
  if (sym) {
    symbols.set_add(sym);
  }
  if (TypeSymbol* ts = toTypeSymbol(scope)) {
    if (ClassType* ct = toClassType(ts->type)) {
      sym = ct->getField(name);
      if (sym)
        symbols.set_add(sym);
    }
  }
  if (symbols.n == 0 && scanModuleUses) {
    BlockStmt* block = toBlockStmt(scope);
    Vec<ModuleSymbol*>* modules = (block) ? &block->modUses : NULL;
    if (modules && modules->n > 0) {
      Vec<Vec<ModuleSymbol*>*> moduleQueue;
      buildBreadthFirstUseTree(modules, moduleQueue);
      forv_Vec(Vec<ModuleSymbol*>, layer, moduleQueue) {
        forv_Vec(ModuleSymbol, mod, *layer) {
          if (mod == rootModule)
            sym = lookup(mod->block, name, alreadyVisited, false);
          else
            sym = lookup(mod->initFn->body, name, alreadyVisited, false);
          if (sym) {
            symbols.set_add(sym);
          }
        }
        if (symbols.n > 0)
          break;
      }
      bool first = true;
      forv_Vec(Vec<ModuleSymbol*>, layer, moduleQueue) {
        if (first) {
          first = false;
          continue;
        }
        delete layer;
      }
    }
  }
  if (scanModuleUses && symbols.n == 0) {
    if (scope->getModule()->block == scope) {
      ModuleSymbol* mod = scope->getModule();
      if (mod == rootModule)
        sym = lookup(mod->block, name, alreadyVisited, scanModuleUses);
      else
        sym = lookup(mod->initFn->body, name, alreadyVisited, scanModuleUses);
      if (sym)
        symbols.set_add(sym);
        
      if (symbols.n == 0 && getScope(scope)) {
        sym = lookup(getScope(scope), name, alreadyVisited, scanModuleUses);
        if (sym)
          symbols.set_add(sym);
      }
    } else {
      FnSymbol* fn = toFnSymbol(scope);
      if (fn && fn->_this && toClassType(fn->_this->type)) {
        ClassType* ct = toClassType(fn->_this->type);
        sym = lookup(ct->symbol, name, alreadyVisited, scanModuleUses);
        if (sym)
          symbols.set_add(sym);
        else {
          sym = lookup(getScope(scope), name, alreadyVisited, scanModuleUses);
          if (sym)
            symbols.set_add(sym);
        }
      } else if (getScope(scope)) {
        sym = lookup(getScope(scope), name, alreadyVisited, scanModuleUses);
        if (sym)
          symbols.set_add(sym);
      }
    }
  }
  symbols.set_to_vec();
  if (symbols.n > 1) {
    forv_Vec(Symbol, sym, symbols) {
      if (!toFnSymbol(sym)) {
        USR_FATAL(sym, "Symbol %s multiply defined", name);
      }
    }
  }
  if (symbols.n == 0) {
    return NULL;
  } else {
    return symbols.v[0];
  }
}


//
// isMethodName returns true iff 'name' names a method of 'type'
//
static bool
isMethodName(const char* name, Type* type) {
  if (!strcmp(name, type->symbol->name))
    return false;
  forv_Vec(Symbol, method, type->methods) {
    if (method && !strcmp(name, method->name))
      return true;
  }
  forv_Vec(Type, pt, type->dispatchParents) {
    if (isMethodName(name, pt))
      return true;
  }
  if (ClassType* ct = toClassType(type)) {
    Type *outerType = ct->symbol->defPoint->parentSymbol->type;
    if (ClassType* outer = toClassType(outerType))
      if (isMethodName(name, outer))
        return true;
  }
  return false;
}


//
// isMethodNameLocal returns true iff 'name' names a method of 'type'
// excluding methods of an outer type
//
static bool
isMethodNameLocal(const char* name, Type* type) {
  if (!strcmp(name, type->symbol->name))
    return false;
  forv_Vec(Symbol, method, type->methods) {
    if (method && !strcmp(name, method->name))
      return true;
  }
  forv_Vec(Type, pt, type->dispatchParents) {
    if (isMethodName(name, pt))
      return true;
  }
  return false;
} 


static BlockStmt*
find_outer_loop(Expr* stmt) {
  if (BlockStmt* block = toBlockStmt(stmt))
    if (block->isLoop())
      return block;
  if (stmt->parentExpr)
    return find_outer_loop(stmt->parentExpr);
  return NULL;
}


static void
resolveGotoLabel(GotoStmt* gotoStmt) {
  if (gotoStmt->label->var == gNil) {
    BlockStmt* loop = find_outer_loop(gotoStmt);
    if (!loop)
      USR_FATAL(gotoStmt, "break or continue is not in a loop");
    if (gotoStmt->gotoTag == GOTO_BREAK)
      gotoStmt->label->replace(new SymExpr(loop->post_loop));
    else if (gotoStmt->gotoTag == GOTO_CONTINUE)
      gotoStmt->label->replace(new SymExpr(loop->pre_loop));
    else
      INT_FATAL(gotoStmt, "Unexpected goto type");
  } else if (!gotoStmt->label->var) {
    const char* name = gotoStmt->label->unresolved;
    if (gotoStmt->gotoTag == GOTO_BREAK)
      name = astr("_post", name);
    Vec<BaseAST*> asts;
    collect_asts(gotoStmt->parentSymbol, asts);
    forv_Vec(BaseAST, ast, asts) {
      if (LabelSymbol* ls = toLabelSymbol(ast)) {
        if (!strcmp(ls->name, name))
          gotoStmt->label->replace(new SymExpr(ls));
      }
    }
  }
}


//
// resolves EnumTypeName.fieldName to the symbol named fieldName in
// the enumerated type named EnumTypeName
//
static void resolveEnumeratedTypes() {
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isNamed(".")) {
        if (SymExpr* first = toSymExpr(call->get(1))) {
          if (first->unresolved)
            continue; // EnumTypeName should be resolved already
          if (EnumType* type = toEnumType(first->var->type)) {
            if (SymExpr* second = toSymExpr(call->get(2))) {
              const char* name;
              bool found = false;
              INT_ASSERT(get_string(second, &name));
              for_enums(constant, type) {
                if (!strcmp(constant->sym->name, name)) {
                  call->replace(new SymExpr(constant->sym));
                  found = true;
                }
              }
              if (!found) {
                USR_FATAL(call, "unresolved enumerated type symbol \"%s\"", name);
              }
            }
          }
        }
      }
    }
  }
}


static bool
isTypeAlias(Symbol* sym) {
  return
    isVarSymbol(sym) &&
    sym->isTypeVariable &&
    isFnSymbol(sym->defPoint->parentSymbol);
}


/********* build constructor ***************/
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
  collect_asts(fn->body, asts);
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

  asts.clear();
  collect_asts(fn, asts);
  addToSymbolTable(asts);
}


static void build_constructor(ClassType* ct) {
  if (ct->defaultConstructor)
    return;

  currentLineno = ct->lineno;

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
  Vec<const char*> fieldNamesSet;
  for_fields(tmp, ct) {
    currentLineno = tmp->lineno;
    if (VarSymbol* field = toVarSymbol(tmp)) {
      if (!field->hasPragma("super class") &&
          !field->hasPragma("omit from constructor") &&
          strcmp(field->name, "_promotionType") &&
          strcmp(field->name, "outer")) {
        ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, field->name, field->type);
        fieldArgMap.put(field, arg);
        fieldNamesSet.set_add(field->name);
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
        int shadowID = 1;
        for_formals_backward(formal, superCtor) {
          if (formal->hasPragma("is meme"))
            continue;
          DefExpr* superArg = formal->defPoint->copy();
          if (fieldNamesSet.set_in(superArg->sym->name))
            superArg->sym->name =
              astr("_shadow_", istr(shadowID++), "_", superArg->sym->name);
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

  for_fields(tmp, ct) {
    VarSymbol* field = toVarSymbol(tmp);

    if (!field)
      continue;

    ArgSymbol* arg = fieldArgMap.get(field);

    if (!arg)
      continue;

    currentLineno = field->lineno;

    if (field->isParam)
      arg->intent = INTENT_PARAM;
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
  collect_asts(fn->body, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* se = toSymExpr(ast))
      if (!se->var && fieldNamesSet.set_in(se->unresolved))
        se->replace(buildDotExpr(fn->_this, se->unresolved));
  }

  currentLineno = ct->lineno;

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
        collect_top_asts(method->body, asts);
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

  asts.clear();
  collect_asts(fn, asts);
  addToSymbolTable(asts);
}
/********* end build constructor ***************/


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
    Symbol* symbol = lookup(useCall, sym->getName());
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
    symbol = lookup(lhs->block, rhsName);
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
    SymExpr* se = toSymExpr(expr);
    INT_ASSERT(se);
    Symbol* sym = lookup(expr, se->getName());
    TypeSymbol* ts = toTypeSymbol(sym);
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
    expr->remove();
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



void scopeResolve(void) {
  //
  // add all program asts to the symbol table
  //
  addToSymbolTable(gAsts);

  // handle "use mod;" where mod is a module
  forv_Vec(BaseAST, ast, gAsts) {
    currentLineno = ast->lineno;
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
      build_type_constructor(ct);
      build_constructor(ct);
    }
  }

  //
  // resolve type of this for methods
  //
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->_this && fn->_this->type == dtUnknown) {
      if (SymExpr* sym = toSymExpr(toArgSymbol(fn->_this)->typeExpr->body.only())) {
        if (!sym->var) {
          TypeSymbol* ts = toTypeSymbol(lookup(sym, sym->unresolved));

          if (!ts) {
            USR_FATAL(fn, "cannot resolve base type for method '%s'", fn->name);
          }
          sym->var = ts;
          sym->unresolved = NULL;
          sym->remove();
        }
        fn->_this->type = sym->var->type;
        fn->_this->type->methods.add(fn);
      }
    }
  }

  Vec<SymExpr*> skipSet;

  forv_Vec(BaseAST, ast, gAsts) {
    currentLineno = ast->lineno;

    // Translate M.x where M is a ModuleSymbol into just x where x is
    // the symbol in module M; for functions, insert a "module=" token
    // that is used to determine visible functions.
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isNamed(".")) {
        if (SymExpr* se1 = toSymExpr(call->get(1))) {
          if (ModuleSymbol* mod = toModuleSymbol(se1->var)) {
            SymbolTableEntry* entry = symbolTable.get(mod->initFn->body);
            Symbol* sym = (entry) ? entry->get(get_string(call->get(2))) : 0;
            if (!sym) {
              entry = symbolTable.get(mod->block);
              sym = (entry) ? entry->get(get_string(call->get(2))) : 0;
            }
            if (FnSymbol* fn = toFnSymbol(sym)) {
              if (!fn->_this && fn->noParens) {
                call->replace(new CallExpr(fn));
              } else {
                SymExpr* se = new SymExpr(get_string(call->get(2)));
                skipSet.set_add(se);
                call->replace(se);
                CallExpr* parent = toCallExpr(se->parentExpr);
                INT_ASSERT(parent);
                parent->insertAtHead(mod);
                parent->insertAtHead(gModuleToken);
              }
            } else if (sym) {
              call->replace(new SymExpr(sym));
            } else {
              USR_FATAL_CONT(call, "Symbol '%s' undeclared in module '%s'",
                             get_string(call->get(2)), mod->name);
            }
          }
        }
      }
    }

    if (SymExpr* symExpr = toSymExpr(ast)) {
      if (symExpr->unresolved) {

        if (skipSet.set_in(symExpr))
          continue;

        const char* name = symExpr->unresolved;
        if (symExpr->isNamed(".") || !symExpr->parentSymbol)
          continue;

        if (!symExpr->parentSymbol)
          continue;

        Symbol* sym = lookup(symExpr, name); //, NULL, false);

        //
        // handle function call without parentheses
        //
        if (FnSymbol* fn = toFnSymbol(sym)) {
          if (!fn->_this && fn->noParens) {
            symExpr->replace(new CallExpr(fn));
            continue;
          }
        }

        // sjd: stopgap to avoid shadowing variables or functions by methods
        if (FnSymbol* fn = toFnSymbol(sym))
          if (fn->isMethod)
            sym = NULL;

        if (sym) {
          if (!isFnSymbol(sym)) {
            symExpr->var = sym;
            symExpr->unresolved = NULL;
          }
          if (isTypeAlias(sym)) {
            ASTMap map;
            Expr* init = sym->defPoint->init->copy(&map);

            // detect recursively defined type aliases
            Vec<BaseAST*> asts;
            map.get_values(asts);
            forv_Vec(BaseAST, ast, asts) {
              if (SymExpr* se = toSymExpr(ast))
                if (isTypeAlias(se->var))
                  USR_FATAL(sym, "type alias is recursive");
            }

            symExpr->replace(init);
            continue;
          }
        }

        // Apply 'this' and 'outer' in methods where necessary
        {
          Symbol* parent = symExpr->parentSymbol;
          while (!toModuleSymbol(parent)) {
            if (FnSymbol* method = toFnSymbol(parent)) {

              // stopgap bug fix: do not let methods shadow symbols
              // that are more specific than methods
              if (sym && sym->defPoint->getFunction() == method)
                break;

              if (method->_this && symExpr->var != method->_this) {
                Type* type = method->_this->type;
                TypeSymbol* cts =
                  (sym) ? toTypeSymbol(sym->defPoint->parentSymbol) : NULL;
                if ((cts && isClassType(cts->type)) ||
                    isMethodName(name, type)) {
                  CallExpr* call = toCallExpr(symExpr->parentExpr);
                  if (call && call->baseExpr == symExpr &&
                      call->numActuals() >= 2 &&
                      toSymExpr(call->get(1)) &&
                      toSymExpr(call->get(1))->var == gMethodToken) {
                    symExpr->var = NULL;
                    symExpr->unresolved = name;
                  } else {
                    ClassType* ct = toClassType(type);
                    int nestDepth = 0;
                    if (isMethodName(name, type)) {
                      while (ct && !isMethodNameLocal(name, ct)) {
                        // count how many classes out from current depth that
                        // this method is first defined in
                        nestDepth += 1;
                        ct = toClassType
                          (ct->symbol->defPoint->parentSymbol->type);
                      }
                    } else {
                      while (ct && !ct->getField(name)) {
                        // count how many classes out from current depth that
                        // this symbol is first defined in
                        nestDepth += 1;
                        ct = toClassType(ct->symbol->defPoint->parentSymbol->type);
                      }
                    }

                    Expr *dot = NULL;
                    for (int i=0; i<=nestDepth; i++) {
                      // Apply implicit this pointers and outer this pointers
                      if (i == 0) {
                        if (i < nestDepth) {
                          dot = new CallExpr(".",
                                             method->_this,
                                             new_StringSymbol("outer"));
                        } else {
                          if (isTypeSymbol(sym))
                            dot = new CallExpr(".", method->_this, sym);
                          else
                            dot = new CallExpr(".",
                                               method->_this,
                                               new_StringSymbol(name));
                        }
                      } else {
                        if (i < nestDepth) {
                          dot = new CallExpr(".",
                                             dot, new_StringSymbol("outer"));
                        } else {
                          if (isTypeSymbol(sym))
                            dot = new CallExpr(".", dot, sym);
                          else
                            dot = new CallExpr(".", dot, new_StringSymbol(name));
                        }
                      }
                    }
                    symExpr->replace(dot);
                  }
                }
                break;
              }
            }
            parent = parent->defPoint->parentSymbol;
          }
        }
      }
    } else if (GotoStmt* gs = toGotoStmt(ast)) {
      resolveGotoLabel(gs);
    }
  }

  resolveEnumeratedTypes();

  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = toDefExpr(ast))
      if (isTypeAlias(def->sym))
        def->remove();
  }

  destroyTable();
}
