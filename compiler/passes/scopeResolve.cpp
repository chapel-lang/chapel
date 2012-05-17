// scopeResolve.cpp
//

#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "stmt.h"
#include "passes.h"
#include "scopeResolve.h"
#include "stringutil.h"
#include "symbol.h"


//
// static functions (forward declaration)
//
static void build_type_constructor(ClassType* ct);
static void build_constructor(ClassType* ct);


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
//   BlockStmt: defines a scope if the block is a normal block
//   for any locally defined symbols
//
// Each entry contains a map from canonicalized string pointers to the
// symbols defined in the scope.
//
typedef Map<const char*,Symbol*> SymbolTableEntry;
typedef Map<BaseAST*,SymbolTableEntry*> SymbolTable;
static SymbolTable symbolTable;

//
// The moduleUsesCache is a cache from blocks with use-statements to
// the modules that they use arranged in breadth-first order and
// separated by NULL modules to indicate that the modules are not at
// the same depth.
//
// Note that this caching is not enabled until after use expression
// have been resolved.
//
static Map<BlockStmt*,Vec<ModuleSymbol*>*> moduleUsesCache;
static bool enableModuleUsesCache = false;

//
// The aliasFieldSet is a set of names of fields for which arrays may
// be passed in by named argument as aliases, as in new C(A=>GA) (see 
// test/arrays/deitz/test_array_alias_field.chpl).
//
static Vec<const char*> aliasFieldSet;

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
    // SCOPELESS and TYPE blocks do not define scopes
    if (block && block->blockTag == BLOCK_NORMAL) {
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
addToSymbolTable(Vec<DefExpr*>& defs) {
  forv_Vec(DefExpr, def, defs) {
    if (!def->sym->hasFlag(FLAG_TEMP)) {
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
          if ((!oldFn || (oldFn && !oldFn->_this && oldFn->hasFlag(FLAG_NO_PARENS))) &&
              (!newFn || (newFn && !newFn->_this && newFn->hasFlag(FLAG_NO_PARENS)))) {
            USR_FATAL(sym, "'%s' has multiple definitions, redefined at:\n  %s", sym->name, def->sym->stringLoc());
          }
        }
        if (!newFn || (newFn && !newFn->_this && newFn->hasFlag(FLAG_NO_PARENS)))
          entry->put(def->sym->name, def->sym);
      } else {
        entry->put(def->sym->name, def->sym);
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


//
// delete the module uses cache
//
static void
destroyModuleUsesCaches() {
  Vec<Vec<ModuleSymbol*>*> values;
  moduleUsesCache.get_values(values);
  forv_Vec(Vec<ModuleSymbol*>, value, values) {
    delete value;
  }
  moduleUsesCache.clear();
}


static void
buildBreadthFirstModuleList(Vec<ModuleSymbol*>* modules,
                            Vec<ModuleSymbol*>* current = NULL,
                            Vec<ModuleSymbol*>* alreadySeen = NULL) {
  if (!alreadySeen) {
    Vec<ModuleSymbol*> seen;
    return buildBreadthFirstModuleList(modules, modules, &seen);
  }
  modules->add(NULL); // use NULL sentinel to identify modules of equal depth
  Vec<ModuleSymbol*> next;
  forv_Vec(ModuleSymbol, module, *current) {
    if (!module) {
      break;
    } else if (module->block->modUses) {
      for_actuals(expr, module->block->modUses) {
        SymExpr* se = toSymExpr(expr);
        INT_ASSERT(se);
        ModuleSymbol* mod = toModuleSymbol(se->var);
        INT_ASSERT(mod);
        if (!alreadySeen->set_in(mod)) {
          next.add(mod);
          modules->add(mod);
          alreadySeen->set_add(mod);
        }
      }
    }
  }
  if (next.n) {
    buildBreadthFirstModuleList(modules, &next, alreadySeen);
  }
}


static Symbol*
lookup(BaseAST* scope,
       const char* name,
       Vec<BaseAST*>* alreadyVisited = NULL,
       bool scanModuleUses = true) {
  Vec<BaseAST*> nestedscopes;
  if (!alreadyVisited) {
    Symbol* result = lookup(scope, name, &nestedscopes, scanModuleUses);
    return result;
  }
  if (!scanModuleUses) {
    nestedscopes.copy(*alreadyVisited);
    alreadyVisited = &nestedscopes;
  }

  if (alreadyVisited->set_in(scope))
    return NULL;

  alreadyVisited->set_add(scope);

  Vec<Symbol*> symbols;

  if (SymbolTableEntry* entry = symbolTable.get(scope))
    if (Symbol* sym = entry->get(name))
      symbols.set_add(sym);

  if (TypeSymbol* ts = toTypeSymbol(scope))
    if (ClassType* ct = toClassType(ts->type))
      if (Symbol* sym = ct->getField(name, false))
        symbols.set_add(sym);

  if (scanModuleUses && symbols.n == 0) {
    if (BlockStmt* block = toBlockStmt(scope)) {
      if (block->modUses) {
        Vec<ModuleSymbol*>* modules = moduleUsesCache.get(block); 
        if (!modules) {
          modules = new Vec<ModuleSymbol*>();
          for_actuals(expr, block->modUses) {
            SymExpr* se = toSymExpr(expr);
            INT_ASSERT(se);
            ModuleSymbol* mod = toModuleSymbol(se->var);
            INT_ASSERT(mod);
            modules->add(mod);
          }
          INT_ASSERT(modules->n);
          buildBreadthFirstModuleList(modules);
          if (enableModuleUsesCache)
            moduleUsesCache.put(block, modules);
        }
        INT_ASSERT(modules);
        forv_Vec(ModuleSymbol, mod, *modules) {
          if (mod) {
            if (mod != rootModule) {
              if (Symbol* sym = lookup(mod->initFn->body, name, alreadyVisited, false))
                symbols.set_add(sym);
            } else {
              if (Symbol* sym = lookup(mod->block, name, alreadyVisited, false))
                symbols.set_add(sym);
            }
          } else {
            //
            // break on each new depth if a symbol has been found
            //
            if (symbols.n > 0)
              break;
          }
        }
      }
    }
  }

  if (scanModuleUses && symbols.n == 0) {
    if (scope->getModule()->block == scope) {
      ModuleSymbol* mod = scope->getModule();
      Symbol* sym = NULL;
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
        Symbol* sym = lookup(ct->symbol, name, alreadyVisited, scanModuleUses);
        if (sym)
          symbols.set_add(sym);
        else {
          sym = lookup(getScope(scope), name, alreadyVisited, scanModuleUses);
          if (sym)
            symbols.set_add(sym);
        }
      } else if (getScope(scope)) {
        Symbol* sym = lookup(getScope(scope), name, alreadyVisited, scanModuleUses);
        if (sym)
          symbols.set_add(sym);
      }
    }
  }

  symbols.set_to_vec();
  if (symbols.n == 1)
    return symbols.v[0];
  else if (symbols.n == 0)
    return NULL;
  else {
    forv_Vec(Symbol, sym, symbols) {
      if (!isFnSymbol(sym))
        USR_FATAL(sym, "Symbol %s multiply defined", name);
    }
    return NULL;
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
resolveGotoLabels() {
  forv_Vec(GotoStmt, gs, gGotoStmts) {
    if (SymExpr* label = toSymExpr(gs->label)) {
      if (label->var == gNil) {
        BlockStmt* loop = find_outer_loop(gs);
        if (!loop)
          USR_FATAL(gs, "break or continue is not in a loop");
        if (gs->gotoTag == GOTO_BREAK) {
          Symbol* breakLabel = loop->breakLabel;
          INT_ASSERT(breakLabel);
          gs->label->replace(new SymExpr(breakLabel));
        } else if (gs->gotoTag == GOTO_CONTINUE) {
          Symbol* continueLabel = loop->continueLabel;
          INT_ASSERT(continueLabel);
          gs->label->replace(new SymExpr(continueLabel));
        } else
          INT_FATAL(gs, "unexpected goto type");
      }
    } else if (UnresolvedSymExpr* label = toUnresolvedSymExpr(gs->label)) {
      const char* name = label->unresolved;
      BlockStmt* loop = find_outer_loop(gs);
      while (loop && (!loop->userLabel || strcmp(loop->userLabel, name))) {
        loop = find_outer_loop(loop->parentExpr);
      }
      if (!loop) {
        USR_FATAL(gs, "bad label on break or continue");
      }
      if (gs->gotoTag == GOTO_BREAK)
        label->replace(new SymExpr(loop->breakLabel));
      else if (gs->gotoTag == GOTO_CONTINUE)
        label->replace(new SymExpr(loop->continueLabel));
      else
        INT_FATAL(gs, "unexpected goto type");
    }
  }
}


//
// resolves EnumTypeName.fieldName to the symbol named fieldName in
// the enumerated type named EnumTypeName
//
static void resolveEnumeratedTypes() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isNamed(".")) {
      if (SymExpr* first = toSymExpr(call->get(1))) {
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


/********* build constructors ***************/
void build_constructors(ClassType* ct)
{
  build_type_constructor(ct);
  build_constructor(ct);
}


static void build_type_constructor(ClassType* ct) {
  if (ct->defaultTypeConstructor)
    return;

  SET_LINENO(ct);

  FnSymbol* fn = new FnSymbol(astr("_type_construct_", ct->symbol->name));
  fn->addFlag(FLAG_TYPE_CONSTRUCTOR);
  ct->defaultTypeConstructor = fn;
  fn->cname = astr("_type_construct_", ct->symbol->cname);
  fn->addFlag(FLAG_TEMP);
  fn->retTag = RET_TYPE;

  if (ct->symbol->hasFlag(FLAG_REF))
    fn->addFlag(FLAG_REF);
  if (ct->symbol->hasFlag(FLAG_TUPLE)) {
    fn->addFlag(FLAG_TUPLE);
    fn->addFlag(FLAG_INLINE);
  }

  fn->_this = new VarSymbol("this", ct);
  fn->_this->addFlag(FLAG_ARG_THIS);
  fn->insertAtTail(new DefExpr(fn->_this));

  Vec<const char*> fieldNamesSet;
  for_fields(tmp, ct) {
    SET_LINENO(tmp);
    if (VarSymbol* field = toVarSymbol(tmp)) {

      if (field->hasFlag(FLAG_SUPER_CLASS))
        continue;

      Expr* exprType = field->defPoint->exprType;
      Expr* init = field->defPoint->init;
      if (!strcmp(field->name, "_promotionType") ||
          field->hasFlag(FLAG_OMIT_FROM_CONSTRUCTOR)) {
        fn->insertAtTail(
          new BlockStmt(
            new CallExpr(PRIM_SET_MEMBER, fn->_this, 
              new_StringSymbol(field->name),
              new CallExpr(PRIM_INIT, exprType->remove())),
            BLOCK_TYPE));
      } else {
        fieldNamesSet.set_add(field->name);

        //
        // if formal is generic
        //
        if (field->hasFlag(FLAG_TYPE_VARIABLE) || field->hasFlag(FLAG_PARAM) || (!exprType && !init)) {
          ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, field->name, field->type);
          fn->insertFormalAtTail(arg);

          if (field->hasFlag(FLAG_PARAM))
            arg->intent = INTENT_PARAM;
          else
            arg->addFlag(FLAG_TYPE_VARIABLE);

          if (init)
            arg->defaultExpr = new BlockStmt(init->copy(), BLOCK_SCOPELESS);

          if (exprType)
            arg->typeExpr = new BlockStmt(exprType->copy(), BLOCK_TYPE);

          if (!exprType && arg->type == dtUnknown)
            arg->type = dtAny;

          if (field->hasFlag(FLAG_PARAM) || field->hasFlag(FLAG_TYPE_VARIABLE))
            fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, fn->_this,
                                          new_StringSymbol(field->name), arg));
          else if (arg->type == dtAny &&
                   !ct->symbol->hasFlag(FLAG_REF))
            fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, fn->_this,
                                          new_StringSymbol(field->name),
                                          new CallExpr("chpl__initCopy",
                                            new CallExpr(PRIM_INIT, arg))));
          else
            fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, fn->_this,
                                          new_StringSymbol(field->name),
                                          new CallExpr(PRIM_INIT, arg)));
        } else if (exprType) {
          CallExpr* newInit = new CallExpr(PRIM_INIT, exprType->copy());
          CallExpr* newSet = new CallExpr(PRIM_SET_MEMBER, fn->_this,
                                          new_StringSymbol(field->name),
                                          newInit);
          fn->insertAtTail(newSet);
        } else if (init) {
          fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, fn->_this,
                                        new_StringSymbol(field->name),
                                        new CallExpr("chpl__initCopy", init->copy())));
        }
      }
    }
  }

  fn->insertAtTail(new CallExpr(PRIM_RETURN, fn->_this));
  ct->symbol->defPoint->insertBefore(new DefExpr(fn));
  fn->retType = ct;

  //
  // insert implicit uses of 'this' in constructor
  //
  Vec<BaseAST*> asts;
  collect_asts(fn->body, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (UnresolvedSymExpr* se = toUnresolvedSymExpr(ast))
      if (fieldNamesSet.set_in(se->unresolved))
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
    fn->addFlag(FLAG_METHOD);
    Expr* insertPoint = outerType->symbol->defPoint;
    while (toTypeSymbol(insertPoint->parentSymbol))
      insertPoint = insertPoint->parentSymbol->defPoint;
    insertPoint->insertBefore(fn->defPoint->remove());

    // Save the pointer to the outer class
    ct->fields.insertAtTail(new DefExpr(outer));
    fn->insertAtHead(new CallExpr(PRIM_SET_MEMBER,
                                  fn->_this,
                                  new_StringSymbol("outer"),
                                  fn->_outer));
    ct->outer = outer;
  }

  Vec<DefExpr*> defs;
  collectDefExprs(fn, defs);
  addToSymbolTable(defs);
}


static void build_constructor(ClassType* ct) {
  if (ct->initializer)
    return;

  SET_LINENO(ct);

  if (isSyncType(ct))
    ct->defaultValue = NULL;

  // Create the default constructor.
  FnSymbol* fn = new FnSymbol(astr("_construct_", ct->symbol->name));
  fn->addFlag(FLAG_DEFAULT_CONSTRUCTOR);
  fn->addFlag(FLAG_CONSTRUCTOR);
  ct->initializer = fn;
  fn->cname = astr("_construct_", ct->symbol->cname);
  fn->addFlag(FLAG_TEMP); // compiler inserted

  if (ct->symbol->hasFlag(FLAG_REF))
    fn->addFlag(FLAG_REF);
  if (ct->symbol->hasFlag(FLAG_TUPLE)) {
    fn->addFlag(FLAG_TUPLE);
    fn->addFlag(FLAG_INLINE);
  }

  // Create "this".
  fn->_this = new VarSymbol("this", ct);
  fn->_this->addFlag(FLAG_ARG_THIS);
  fn->insertAtTail(new DefExpr(fn->_this));

  // Walk the fields in the class type.
  Map<VarSymbol*,ArgSymbol*> fieldArgMap;
  Vec<const char*> fieldNamesSet;
  for_fields(tmp, ct) {
    SET_LINENO(tmp);
    if (VarSymbol* field = toVarSymbol(tmp)) {
      // Filter inherited fields and other special cases.
      // "outer" is used internally to supply a pointer to the outer parent of a nested class.
      if (!field->hasFlag(FLAG_SUPER_CLASS) &&
          !field->hasFlag(FLAG_OMIT_FROM_CONSTRUCTOR) &&
          strcmp(field->name, "_promotionType") &&
          strcmp(field->name, "outer")) {
        // Create an argument to the default constructor
        // corresponding to the field.
        ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, field->name, field->type);
        fieldArgMap.put(field, arg);
        fieldNamesSet.set_add(field->name);
      }
    }
  }

  ArgSymbol* meme = NULL;
  CallExpr* superCall = NULL;
  CallExpr* allocCall = NULL;
  if (ct->symbol->hasFlag(FLAG_REF) ||
      isSyncType(ct)) {
    // For ref, sync and single classes, just allocate space.
    allocCall = new CallExpr(PRIM_CHPL_ALLOC, fn->_this,
                         newMemDesc(ct->symbol->name));
    fn->insertAtTail(new CallExpr(PRIM_MOVE, fn->_this, allocCall));
  } else if (!ct->symbol->hasFlag(FLAG_TUPLE)) {
    // Create a meme (whatever that is).
    meme = new ArgSymbol(INTENT_BLANK, "meme", ct, NULL, new SymExpr(gTypeDefaultToken));
    meme->addFlag(FLAG_IS_MEME);
    // Move the meme into "this".
    fn->insertAtTail(new CallExpr(PRIM_MOVE, fn->_this, meme));
    if (isClass(ct)) {
      if (ct->dispatchParents.n > 0 && !ct->symbol->hasFlag(FLAG_EXTERN)) {
        // This class has a parent class.
        if (!ct->dispatchParents.v[0]->initializer) {
          // If it doesn't yet have an initializer, make one.
          build_constructors(toClassType(ct->dispatchParents.v[0]));
        }

        // Get the parent constructor.
        // Note that since we only pay attention to the first entry in the
        // dispatchParents list, we are effectively implementing
        // single class inheritance, multiple interface inheritance.
        FnSymbol* superCtor = ct->dispatchParents.v[0]->initializer;

        // Create a call to the superclass constructor.
        superCall = new CallExpr(superCtor->name);
        int shadowID = 1;
        // Walk the formals of the default super class constructor
        for_formals_backward(formal, superCtor) {
          if (formal->hasFlag(FLAG_IS_MEME))
            continue;
          DefExpr* superArg = formal->defPoint->copy();
          // Rename the arg if it clashes with a field name already seen,
          // starting with those in the most-derived class in lexical order
          // and then in successive ancestors in reverse-lexical order.
          // Field names within a given class are guaranteed to be unique,
          // so the order in which the names are visited at each ancestral 
          // level is immaterial.
          if (fieldNamesSet.set_in(superArg->sym->name))
            superArg->sym->name =
              astr("_shadow_", istr(shadowID++), "_", superArg->sym->name);
          fieldNamesSet.set_add(superArg->sym->name);
          // Inserting each successive ancestor argument at the head in 
          // reverse-lexcial order results in all of the arguments appearing
          // in lexical order, starting with those in the most ancient class
          // and ending with those in the most-derived class.
          fn->insertFormalAtHead(superArg);
          superCall->insertAtHead(superArg->sym);
        }

        // Create a temp variable and add it to the actual argument list
        // in the superclass constructor call.  This temp will hold 
        // the pointer to the parent subobject.
        VarSymbol* tmp = newTemp();
        superCall->insertAtTail(new NamedExpr("meme", new SymExpr(tmp)));

        // Add super call to the constructor function.
        fn->insertAtTail(superCall);

        // Declare that variable in the scope of this constructor.
        // And initialize it with the super class pointer.
        superCall->insertBefore(new DefExpr(tmp));
        superCall->insertBefore(
          new CallExpr(PRIM_MOVE, tmp,
            new CallExpr(PRIM_GET_MEMBER_VALUE,
                         fn->_this, new_StringSymbol("super"))));
      }
    }
  }

  if (isUnion(ct))
    fn->insertAtTail(new CallExpr(PRIM_UNION_SETID, fn->_this, new_IntSymbol(0)));

  ct->symbol->defPoint->insertBefore(new DefExpr(fn));

  for_fields(tmp, ct) {
    VarSymbol* field = toVarSymbol(tmp);

    if (!field)
      continue;

    ArgSymbol* arg = fieldArgMap.get(field);

    if (!arg)
      continue;

    SET_LINENO(field);

    if (field->hasFlag(FLAG_PARAM))
      arg->intent = INTENT_PARAM;

    Expr* exprType = field->defPoint->exprType->remove();
    Expr* init = field->defPoint->init->remove();

    bool hadType = exprType;
    bool hadInit = init;

    if (init) {
      if (!field->hasFlag(FLAG_TYPE_VARIABLE) && !exprType) {
        // init && !exprType
        VarSymbol* tmp = newTemp();
        tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
        tmp->addFlag(FLAG_MAYBE_PARAM);
        tmp->addFlag(FLAG_MAYBE_TYPE);
        exprType = new BlockStmt(new DefExpr(tmp), BLOCK_TYPE);
        toBlockStmt(exprType)->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr("chpl__initCopy", init->copy())));
        toBlockStmt(exprType)->insertAtTail(new CallExpr(PRIM_TYPEOF, tmp));
      }
    } else if (hadType && !field->hasFlag(FLAG_TYPE_VARIABLE) && !field->hasFlag(FLAG_PARAM)) {
      init = new CallExpr(PRIM_INIT, exprType->copy());
    }
    if (!field->hasFlag(FLAG_TYPE_VARIABLE) && !field->hasFlag(FLAG_PARAM)) {
      if (hadType)
        init = new CallExpr("_createFieldDefault", exprType->copy(), init);
      else
        if (init)
          init = new CallExpr("chpl__initCopy", init);
    }
    if (init) {
      if (hadInit)
        arg->defaultExpr = new BlockStmt(init, BLOCK_SCOPELESS);
      else
        arg->defaultExpr = new BlockStmt(new SymExpr(gTypeDefaultToken));
    }
    if (exprType) {
      if (!isBlockStmt(exprType))
        arg->typeExpr = new BlockStmt(exprType, BLOCK_TYPE);
      else
        arg->typeExpr = toBlockStmt(exprType);
    }
    if (field->hasFlag(FLAG_TYPE_VARIABLE))
      arg->addFlag(FLAG_TYPE_VARIABLE);
    if (!exprType && arg->type == dtUnknown)
      arg->type = dtAny;
    fn->insertFormalAtTail(arg);
    if (arg->type == dtAny && !arg->hasFlag(FLAG_TYPE_VARIABLE) &&
        !arg->hasFlag(FLAG_PARAM) && !ct->symbol->hasFlag(FLAG_REF))
      fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, fn->_this, 
                                    new_StringSymbol(arg->name),
                                    new CallExpr("chpl__initCopy", arg)));
    else
      fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, fn->_this, 
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
    if (UnresolvedSymExpr* se = toUnresolvedSymExpr(ast))
      if (fieldNamesSet.set_in(se->unresolved))
        se->replace(buildDotExpr(fn->_this, se->unresolved));
  }

  SET_LINENO(ct);

  ClassType *outerType = toClassType(ct->symbol->defPoint->parentSymbol->type);
  if (outerType) {
    // Remove the DefPoint for this constructor, add it to the outer
    // class's method list.
    outerType->methods.add(fn);
    fn->_outer = new ArgSymbol(INTENT_BLANK, "outer", outerType);
    fn->insertFormalAtHead(new DefExpr(fn->_outer));
    fn->insertFormalAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken)));
    fn->addFlag(FLAG_METHOD);
    Expr* insertPoint = outerType->symbol->defPoint;
    while (toTypeSymbol(insertPoint->parentSymbol))
      insertPoint = insertPoint->parentSymbol->defPoint;
    insertPoint->insertBefore(fn->defPoint->remove());

    // Save the pointer to the outer class
    fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
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
            if (call->isPrimitive(PRIM_RETURN)) {
              if (call->get(1) && call->get(1)->typeInfo() != dtVoid) {
                init = new CallExpr(PRIM_RETURN, init);
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
    fn->insertAtTail(new CallExpr(PRIM_RETURN, fn->_this));

  Vec<DefExpr*> defs;
  collectDefExprs(fn, defs);
  addToSymbolTable(defs);
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
    if (!call->isPrimitive(PRIM_USE))
      INT_FATAL(call, "Bad use statement in getUsedModule");
    return getUsedModule(call->get(1), call);
  }

  if (SymExpr* sym = toSymExpr(expr)) {
    Symbol* symbol = sym->var;
    mod = toModuleSymbol(symbol);
    if (symbol && !mod) {
      USR_FATAL(useCall, "Illegal use of non-module %s", symbol->name);
    }
  } else if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(expr)) {
    Symbol* symbol = lookup(useCall, sym->unresolved);
    mod = toModuleSymbol(symbol);
    if (symbol && !mod) {
      USR_FATAL(useCall, "Illegal use of non-module %s", symbol->name);
    } else if (!symbol) {
      USR_FATAL(useCall, "Cannot find module '%s'", sym->unresolved);
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
  SET_LINENO(call);
  ModuleSymbol* mod = getUsedModule(call);
  if (!mod)
    USR_FATAL(call, "Cannot find module");
  ModuleSymbol* enclosingModule = call->getModule();
  //  printf("Adding %s to %s's use list\n", mod->name, enclosingModule->name);
  if (!enclosingModule->modUseSet.set_in(mod)) {
    enclosingModule->modUseSet.set_add(mod);
    enclosingModule->modUseList.add(mod);
  }
  if (call->getStmtExpr()->parentExpr == call->getModule()->initFn->body)
    call->getModule()->block->addUse(mod);
  else
    getVisibilityBlock(call)->addUse(mod);
  call->getStmtExpr()->remove();
}

//
// Compute dispatchParents and dispatchChildren vectors; add base
// class fields to subclasses; identify cyclic or illegal class or
// record hierarchies
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
  if (ct->inherits.length == 0 && !ct->symbol->hasFlag(FLAG_NO_OBJECT)) {
    if (isRecord(ct)) {
      ct->dispatchParents.add(dtValue);
      dtValue->dispatchChildren.add(ct);
    } else if (isClass(ct)) {
      ct->dispatchParents.add(dtObject);
      dtObject->dispatchChildren.add(ct);
      VarSymbol* super = new VarSymbol("super", dtObject);
      super->addFlag(FLAG_SUPER_CLASS);
      ct->fields.insertAtHead(new DefExpr(super));
    }
  }

  // Walk the base class list, and add parents into the class hierarchy.
  for_alist(expr, ct->inherits) {
    UnresolvedSymExpr* se = toUnresolvedSymExpr(expr);
    INT_ASSERT(se);
    //    printf("looking up %s\n", se->unresolved);
    Symbol* sym = lookup(expr, se->unresolved);
    TypeSymbol* ts = toTypeSymbol(sym);
    if (!ts)
      USR_FATAL(expr, "Illegal super class");
    //    printf("found it in %s\n", sym->getModule()->name);
    ClassType* pt = toClassType(ts->type);
    if (!pt)
      USR_FATAL(expr, "Illegal super class %s", ts->name);
    if (isUnion(ct) && isUnion(pt))
      USR_FATAL(expr, "Illegal inheritance involving union type");
    if (isRecord(ct) && isClass(pt))
      USR_FATAL(expr, "Record %s inherits from class %s",
                ct->symbol->name, pt->symbol->name);
    if (isClass(ct) && isRecord(pt))
      // <hilde> Possible language change: Allow classes to inherit fields and methods from records.
      USR_FATAL(expr, "Class %s inherits from record %s",
                ct->symbol->name, pt->symbol->name);
    localSeenPtr->set_add(ct);
    add_class_to_hierarchy(pt, localSeenPtr);
    ct->dispatchParents.add(pt);
    pt->dispatchChildren.add(ct);
    expr->remove();
    if (isClass(ct)) {
      // For a class, just add a super class pointer.
      VarSymbol* super = new VarSymbol("super", pt);
      super->addFlag(FLAG_SUPER_CLASS);
      ct->fields.insertAtHead(new DefExpr(super));
    } else {
      // For records and unions, scan the fields in the parent type.
      for_fields_backward(field, pt) {
        if (toVarSymbol(field) && !field->hasFlag(FLAG_SUPER_CLASS)) {
          // If not already in derived class (by name), copy it.
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
    }
  }
}


static void renameDefaultType(Type* type, const char* newname) {
  if (strchr(type->symbol->name, '(') != NULL) {
    INT_FATAL("Renaming a default type that already seems to have a width");
  }
  type->symbol->name = astr(newname);
}


static void renameDefaultTypesToReflectWidths(void) {
  renameDefaultType(dtInt[INT_SIZE_DEFAULT], "int(64)");
  renameDefaultType(dtUInt[INT_SIZE_DEFAULT], "uint(64)");
  renameDefaultType(dtReal[FLOAT_SIZE_DEFAULT], "real(64)");
  renameDefaultType(dtImag[FLOAT_SIZE_DEFAULT], "imag(64)");
  renameDefaultType(dtComplex[COMPLEX_SIZE_DEFAULT], "complex(128)");
}


void scopeResolve(void) {
  //
  // add all program asts to the symbol table
  //
  addToSymbolTable(gDefExprs);

  //
  // handle "use mod;" where mod is a module
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_USE)) {
      process_import_expr(call);
    }
  }

  enableModuleUsesCache = true;

  //
  // compute class hierarchy
  //
  forv_Vec(ClassType, ct, gClassTypes) {
    add_class_to_hierarchy(ct);
  }

  //
  // determine fields (by name) that may be passed in arrays to alias
  //
  forv_Vec(NamedExpr, ne, gNamedExprs) {
    if (!strncmp(ne->name, "chpl__aliasField_", 17)) {
      CallExpr* pne = toCallExpr(ne->parentExpr);
      CallExpr* ppne = (pne) ? toCallExpr(pne->parentExpr) : NULL;
      if (!ppne || !ppne->isPrimitive(PRIM_NEW))
        USR_FATAL(ne, "alias-named-argument passing can only be used in constructor calls");
      aliasFieldSet.set_add(astr(&ne->name[17]));
    }
  }

  //
  // add implicit fields for implementing alias-named-argument passing
  //
  forv_Vec(ClassType, ct, gClassTypes) {
    for_fields(field, ct) {
      if (aliasFieldSet.set_in(field->name)) {
        Symbol* aliasField = new VarSymbol(astr("chpl__aliasField_", field->name));
        aliasField->addFlag(FLAG_CONST);
        aliasField->addFlag(FLAG_IMPLICIT_ALIAS_FIELD);
        DefExpr* def = new DefExpr(aliasField);
        def->init = new UnresolvedSymExpr("false");
        def->exprType = new UnresolvedSymExpr("bool");
        ct->fields.insertAtTail(def);
      }
    }
  }

  //
  // build constructors (type and value versions)
  //
  forv_Vec(ClassType, ct, gClassTypes) {
    SET_LINENO(ct->symbol);
    build_constructors(ct);
  }

  //
  // resolve type of this for methods
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->_this && fn->_this->type == dtUnknown) {
      if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(toArgSymbol(fn->_this)->typeExpr->body.only())) {
        TypeSymbol* ts = toTypeSymbol(lookup(sym, sym->unresolved));
        
        if (!ts) {
          USR_FATAL(fn, "cannot resolve base type for method '%s'", fn->name);
        }
        sym->replace(new SymExpr(ts));
        fn->_this->type = ts->type;
        fn->_this->type->methods.add(fn);
      } else if (SymExpr* sym = toSymExpr(toArgSymbol(fn->_this)->typeExpr->body.only())) {
        fn->_this->type = sym->var->type;
        fn->_this->type->methods.add(fn);
      }
    }
  }

  resolveGotoLabels();


  Vec<UnresolvedSymExpr*> skipSet;

  //
  // Translate M.x where M is a ModuleSymbol into just x where x is
  // the symbol in module M; for functions, insert a "module=" token
  // that is used to determine visible functions.
  //

  forv_Vec(UnresolvedSymExpr, unresolvedSymExpr, gUnresolvedSymExprs) {
    if (skipSet.set_in(unresolvedSymExpr))
      continue;

    const char* name = unresolvedSymExpr->unresolved;
    if (!strcmp(name, "."))
      continue;

    if (!unresolvedSymExpr->parentSymbol)
      continue;

    SET_LINENO(unresolvedSymExpr);

    Symbol* sym = lookup(unresolvedSymExpr, name); //, NULL, false);

    //
    // handle function call without parentheses
    //
    if (FnSymbol* fn = toFnSymbol(sym)) {
      if (!fn->_this && fn->hasFlag(FLAG_NO_PARENS)) {
        unresolvedSymExpr->replace(new CallExpr(fn));
        continue;
      }
    }

    // sjd: stopgap to avoid shadowing variables or functions by methods
    if (FnSymbol* fn = toFnSymbol(sym))
      if (fn->hasFlag(FLAG_METHOD))
        sym = NULL;

    SymExpr* symExpr = NULL;

    if (sym) {
      if (!isFnSymbol(sym)) {
        symExpr = new SymExpr(sym);
        unresolvedSymExpr->replace(symExpr);
      }
      else if (isFnSymbol(sym)) {
        Expr *parent = unresolvedSymExpr->parentExpr;
        if (parent) {
          CallExpr *call = toCallExpr(parent);
          if (((call) && (call->baseExpr != unresolvedSymExpr)) || (!call)) {
            //If the function is being used as a first-class value, handle this with a primitive and unwrap the primitive later in functionResolution
            CallExpr *prim_capture_fn = new CallExpr(PRIM_CAPTURE_FN);
            unresolvedSymExpr->replace(prim_capture_fn);
            prim_capture_fn->insertAtTail(unresolvedSymExpr);
            continue;
          }
        }
      }
    }

    // Apply 'this' and 'outer' in methods where necessary
    {
      Expr* expr = symExpr;
      if (!expr)
        expr = unresolvedSymExpr;

      Symbol* parent = expr->parentSymbol;
      while (!toModuleSymbol(parent)) {
        if (FnSymbol* method = toFnSymbol(parent)) {

          // stopgap bug fix: do not let methods shadow symbols
          // that are more specific than methods
          if (sym && sym->defPoint->getFunction() == method)
            break;

          if (method->_this && (!symExpr || symExpr->var != method->_this)) {
            Type* type = method->_this->type;
            TypeSymbol* cts =
              (sym) ? toTypeSymbol(sym->defPoint->parentSymbol) : NULL;
            if ((cts && isClassType(cts->type)) ||
                isMethodName(name, type)) {
              CallExpr* call = toCallExpr(expr->parentExpr);
              if (call && call->baseExpr == expr &&
                  call->numActuals() >= 2 &&
                  toSymExpr(call->get(1)) &&
                  toSymExpr(call->get(1))->var == gMethodToken) {
                UnresolvedSymExpr* use = new UnresolvedSymExpr(name);
                expr->replace(use);
                skipSet.set_add(use);
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
                  while (ct && !ct->getField(name, false)) {
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
                expr->replace(dot);
              }
            }
            break;
          }
        }
        parent = parent->defPoint->parentSymbol;
      }
    }
  }

  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isNamed(".")) {
      if (SymExpr* se = toSymExpr(call->get(1))) {
        if (ModuleSymbol* mod = toModuleSymbol(se->var)) { 
          ModuleSymbol* enclosingModule = call->getModule();
          if (!enclosingModule->modUseSet.set_in(mod)) {
            enclosingModule->modUseSet.set_add(mod);
            enclosingModule->modUseList.add(mod);
          }
          SET_LINENO(call);
          SymbolTableEntry* entry = symbolTable.get(mod->initFn->body);
          Symbol* sym = (entry) ? entry->get(get_string(call->get(2))) : 0;
          if (!sym) {
            entry = symbolTable.get(mod->block);
            sym = (entry) ? entry->get(get_string(call->get(2))) : 0;
          }
          if (FnSymbol* fn = toFnSymbol(sym)) {
            if (!fn->_this && fn->hasFlag(FLAG_NO_PARENS)) {
              call->replace(new CallExpr(fn));
            } else {
              UnresolvedSymExpr* se = new UnresolvedSymExpr(get_string(call->get(2)));
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

  resolveEnumeratedTypes();

  destroyTable();

  destroyModuleUsesCaches();

  renameDefaultTypesToReflectWidths();
}
