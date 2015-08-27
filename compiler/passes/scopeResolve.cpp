/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// scopeResolve.cpp
//

#include "scopeResolve.h"

#include "astutil.h"
#include "build.h"
#include "clangUtil.h"
#include "expr.h"
#include "externCResolve.h"
#include "LoopStmt.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

#include <map>
#include <set>

#ifdef HAVE_LLVM
// TODO: Remove uses of old-style collectors from LLVM-specific code.
#include "oldCollectors.h"
#include "llvm/ADT/SmallSet.h"
#endif

typedef std::map<const char*, Symbol*>           SymbolTableEntry;
typedef std::map<BaseAST*,    SymbolTableEntry*> SymbolTable;


//
// The symbolTable maps BaseAST* pointers to entries based on scope
// definitions.  The following BaseAST subtypes define scopes:
//
//   FnSymbol: defines a scope mainly for its arguments but also for
//   identifiers that are defined via query-expressions, e.g., 't' in
//   'def f(x: ?t)'
//
//   TypeSymbol: defines a scope for EnumType and AggregateType types for
//   the enumerated type constants or the class/record fields
//
//   BlockStmt: defines a scope if the block is a normal block
//   for any locally defined symbols
//
// Each entry contains a map from canonicalized string pointers to the
// symbols defined in the scope.
//
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
static std::map<BlockStmt*,Vec<ModuleSymbol*>*> moduleUsesCache;
static bool                                     enableModuleUsesCache = false;

//
// The aliasFieldSet is a set of names of fields for which arrays may
// be passed in by named argument as aliases, as in new C(A=>GA) (see
// test/arrays/deitz/test_array_alias_field.chpl).
//
static Vec<const char*>                         aliasFieldSet;




static void     addToSymbolTable(Vec<DefExpr*>& defs); // deprecated.
static void     addToSymbolTable(std::vector<DefExpr*>& defs); // use this one in new code.

static void     processImportExprs();

static void     addClassToHierarchy(AggregateType* ct);

static void     addRecordDefaultConstruction();

static void     resolveGotoLabels();
static void     resolveUnresolvedSymExprs();
static void     resolveEnumeratedTypes();
static void     destroyTable();
static void     destroyModuleUsesCaches();

static void     renameDefaultTypesToReflectWidths();

static Symbol* lookup(BaseAST* scope, const char* name);

static BaseAST* getScope(BaseAST* ast);

void scopeResolve() {
  //
  // add all program asts to the symbol table
  //
  addToSymbolTable(gDefExprs);

  processImportExprs();

  enableModuleUsesCache = true;

  //
  // compute class hierarchy
  //
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    addClassToHierarchy(ct);
  }

  //
  // determine fields (by name) that may be passed in arrays to alias
  //
  forv_Vec(NamedExpr, ne, gNamedExprs) {
    if (!strncmp(ne->name, "chpl__aliasField_", 17)) {
      CallExpr* pne  = toCallExpr(ne->parentExpr);
      CallExpr* ppne = (pne) ? toCallExpr(pne->parentExpr) : NULL;

      if (!ppne || !ppne->isPrimitive(PRIM_NEW))
        USR_FATAL(ne, "alias-named-argument passing can only be used in constructor calls");

      aliasFieldSet.set_add(astr(&ne->name[17]));
    }
  }

  //
  // add implicit fields for implementing alias-named-argument passing
  //
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    for_fields(field, ct) {
      if (aliasFieldSet.set_in(field->name)) {
        SET_LINENO(field);

        const char* aliasName  = astr("chpl__aliasField_", field->name);
        Symbol*     aliasField = new VarSymbol(aliasName);

        aliasField->addFlag(FLAG_CONST);
        aliasField->addFlag(FLAG_IMPLICIT_ALIAS_FIELD);

        DefExpr* def = new DefExpr(aliasField);

        def->init     = new UnresolvedSymExpr("false");
        def->exprType = new UnresolvedSymExpr("bool");

        ct->fields.insertAtTail(def);
      }
    }
  }

  addRecordDefaultConstruction();

  //
  // build constructors (type and value versions)
  //
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    SET_LINENO(ct->symbol);
    build_constructors(ct);
  }

  //
  // resolve type of this for methods
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->_this && fn->_this->type == dtUnknown) {
      if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(toArgSymbol(fn->_this)->typeExpr->body.only())) {
        SET_LINENO(fn->_this);

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

  resolveUnresolvedSymExprs();

  resolveEnumeratedTypes();

  destroyTable();

  destroyModuleUsesCaches();

  renameDefaultTypesToReflectWidths();

  cleanupExternC();
}

/************************************ | *************************************
*                                                                           *
* addToSymbolTable adds the asts in a vector to the global symbolTable such *
* that symbol definitions are added to entries in the table and new         *
* enclosing asts become entries                                             *
*                                                                           *
************************************* | ************************************/
static void addOneToSymbolTable(DefExpr* def);

static void addToSymbolTable(std::vector<DefExpr*>& defs) {
  for_vector(DefExpr, def, defs)
    addOneToSymbolTable(def);
}

static void addToSymbolTable(Vec<DefExpr*>& defs) {
  forv_Vec(DefExpr, def, defs)
    addOneToSymbolTable(def);
}

static void addOneToSymbolTable(DefExpr* def)
{
    // If the symbol is a compiler-generated variable or a label,
    // do not add it to the symbol table.
    if (def->sym->hasFlag(FLAG_TEMP) ||
        isLabelSymbol(def->sym))
      return;

    BaseAST* scope = getScope(def);

    if (symbolTable.count(scope) == 0) {
      symbolTable[scope] = new SymbolTableEntry();
    }

    SymbolTableEntry* entry = symbolTable[scope];

    if (entry->count(def->sym->name) != 0) {
      Symbol*     sym       = (*entry)[def->sym->name];
      FnSymbol*   oldFn     = toFnSymbol(sym);
      FnSymbol*   newFn     = toFnSymbol(def->sym);
      TypeSymbol* typeScope = toTypeSymbol(scope);

      if (!typeScope || !isAggregateType(typeScope->type)) { // inheritance
        if ((!oldFn || (!oldFn->_this && oldFn->hasFlag(FLAG_NO_PARENS))) &&
            (!newFn || (!newFn->_this && newFn->hasFlag(FLAG_NO_PARENS)))) {
          USR_FATAL(sym,
                    "'%s' has multiple definitions, redefined at:\n  %s",
                    sym->name,
                    def->sym->stringLoc());
        }

        if (!oldFn && (newFn && !newFn->_this)) {
          // A function definition is conflicting with another named symbol
          // that isn't a function (could be a variable, a module name, etc.)
          USR_FATAL(sym,
                    "'%s' has multiple definitions, redefined at:\n  %s",
                    sym->name,
                    def->sym->stringLoc());
        } else if (!newFn && (oldFn && !oldFn->_this)) {
          // Another named symbol that isn't a function is conflicting with
          // a function definition name.
          USR_FATAL(sym,
                    "'%s' has multiple definitions, redefined at:\n  %s",
                    sym->name,
                    def->sym->stringLoc());
        }
      }

      if (!newFn || (newFn && !newFn->_this))
        (*entry)[def->sym->name] = def->sym;
    } else {
      (*entry)[def->sym->name] = def->sym;
    }
}


/************************************ | *************************************
*                                                                           *
* Transform module uses into calls to initialize functions; store the       *
* relevant scoping information in BlockStmt::modUses                        *
*                                                                           *
************************************* | ************************************/

static ModuleSymbol* getUsedModule(Expr* expr);
static ModuleSymbol* getUsedModule(Expr* expr, CallExpr* useCall);

static void processImportExprs() {
  // handle "use mod;" where mod is a module
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_USE)) {
      SET_LINENO(call);

      ModuleSymbol* mod = getUsedModule(call);

      if (!mod)
        USR_FATAL(call, "Cannot find module");

      ModuleSymbol* enclosingModule = call->getModule();

      enclosingModule->moduleUseAdd(mod);

      getVisibilityBlock(call)->moduleUseAdd(mod);

      call->getStmtExpr()->remove();
    }
  }
}

//
// Return the module imported by a use call.  The module returned could be
// nested: e.g. "use outermost.middle.innermost;"
//
static ModuleSymbol* getUsedModule(Expr* expr) {
  CallExpr* call = toCallExpr(expr);

  if (!call)
    INT_FATAL(call, "Bad use statement in getUsedModule");

  if (!call->isPrimitive(PRIM_USE))
    INT_FATAL(call, "Bad use statement in getUsedModule");

  return getUsedModule(call->get(1), call);
}

//
// Return the module imported by a use call.  The module returned could be
// nested: e.g. "use outermost.middle.innermost;"
//
static ModuleSymbol* getUsedModule(Expr* expr, CallExpr* useCall) {
  ModuleSymbol* mod    = NULL;
  Symbol*       symbol = NULL;

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

    SymExpr*    rhs     = toSymExpr(call->get(2));
    const char* rhsName = 0;

    if (!rhs)
      INT_FATAL(useCall, "Bad use statement in getUsedModule");

    if (!get_string(rhs, &rhsName))
      INT_FATAL(useCall, "Bad use statement in getUsedModule");

    symbol = lookup(lhs->block, rhsName);
    mod    = toModuleSymbol(symbol);

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

/************************************ | *************************************
*                                                                           *
* Compute dispatchParents and dispatchChildren vectors; add base class      *
* fields to subclasses; identify cyclic or illegal class or record          *
* hierarchies                                                               *
*                                                                           *
************************************* | ************************************/

static void addClassToHierarchy(AggregateType*       ct,
                                Vec<AggregateType*>* localSeen);

static void addClassToHierarchy(AggregateType* ct) {
  Vec<AggregateType*> localSeen; // classes in potential cycle

  return addClassToHierarchy(ct, &localSeen);
}

static AggregateType* discoverParentAndCheck(Expr*          storesName,
                                             AggregateType* child);

static void addClassToHierarchy(AggregateType*       ct,
                                Vec<AggregateType*>* localSeen) {
  static Vec<AggregateType*> globalSeen; // classes already in hierarchy

  if (localSeen->set_in(ct))
    USR_FATAL(ct, "Class hierarchy is cyclic");

  if (globalSeen.set_in(ct))
    return;

  globalSeen.set_add(ct);

  add_root_type(ct);

  // Walk the base class list, and add parents into the class hierarchy.
  for_alist(expr, ct->inherits) {
    AggregateType* pt = discoverParentAndCheck(expr, ct);

    localSeen->set_add(ct);

    addClassToHierarchy(pt, localSeen);

    ct->dispatchParents.add(pt);

    bool inserted = pt->dispatchChildren.add_exclusive(ct);

    INT_ASSERT(inserted);

    expr->remove();

    if (isClass(ct)) {
      SET_LINENO(ct);
      // For a class, just add a super class pointer.
      VarSymbol* super = new VarSymbol("super", pt);

      super->addFlag(FLAG_SUPER_CLASS);

      ct->fields.insertAtHead(new DefExpr(super));
    } else {
      SET_LINENO(ct);

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

static AggregateType* discoverParentAndCheck(Expr* storesName, AggregateType* child) {
  UnresolvedSymExpr* se  = toUnresolvedSymExpr(storesName);

  INT_ASSERT(se);

  Symbol*            sym = lookup(storesName, se->unresolved);
  TypeSymbol*        ts  = toTypeSymbol(sym);

  //    printf("looking up %s\n", se->unresolved);
  if (!ts)
    USR_FATAL(storesName, "Illegal super class");

  //    printf("found it in %s\n", sym->getModule()->name);
  AggregateType* pt = toAggregateType(ts->type);

  if (!pt)
    USR_FATAL(storesName, "Illegal super class %s", ts->name);
  if (isUnion(child) && isUnion(pt))
    USR_FATAL(storesName, "Illegal inheritance involving union type");

  if (isRecord(child) && isClass(pt))
    USR_FATAL(storesName, "Record %s inherits from class %s",
              child->symbol->name, pt->symbol->name);

  if (isClass(child) && isRecord(pt))
    // <hilde> Possible language change: Allow classes to inherit
    // fields and methods from records.
    USR_FATAL(storesName,
              "Class %s inherits from record %s",
              child->symbol->name,
              pt->symbol->name);

  return pt;
}

void add_root_type(AggregateType* ct)
{
  // make root records inherit from value
  // make root classes inherit from object
  if (ct->inherits.length == 0 && !ct->symbol->hasFlag(FLAG_NO_OBJECT)) {
    SET_LINENO(ct);

    if (isRecord(ct)) {
      ct->dispatchParents.add(dtValue);

      // Assume that this addition is unique; report if not.
      bool inserted = dtValue->dispatchChildren.add_exclusive(ct);
      INT_ASSERT(inserted);
    } else if (isClass(ct)) {
      ct->dispatchParents.add(dtObject);

      // Assume that this addition is unique; report if not.
      bool inserted = dtObject->dispatchChildren.add_exclusive(ct);
      INT_ASSERT(inserted);

      VarSymbol* super = new VarSymbol("super", dtObject);
      super->addFlag(FLAG_SUPER_CLASS);
      ct->fields.insertAtHead(new DefExpr(super));
    }
  }
}

static void addRecordDefaultConstruction()
{
  forv_Vec(DefExpr, def, gDefExprs)
  {
    // We're only interested in declarations that do not have initializers.
    if (def->init)
      continue;

    if (VarSymbol* var = toVarSymbol(def->sym))
    {
      if (AggregateType* at = toAggregateType(var->type))
      {
        if (!at->isRecord())
          continue;

        // No initializer for extern records.
        if (at->symbol->hasFlag(FLAG_EXTERN))
          continue;

        SET_LINENO(def);
        CallExpr* ctor_call = new CallExpr(new SymExpr(at->symbol));
        def->init = new CallExpr(PRIM_NEW, ctor_call);
        insert_help(def->init, def, def->parentSymbol);
      }
    }
  }
}


/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void       build_type_constructor(AggregateType* ct);
static void       build_constructor(AggregateType* ct);
static ArgSymbol* create_generic_arg(VarSymbol* field);
static void       insert_implicit_this(FnSymbol*         fn,
                                       Vec<const char*>& fieldNamesSet);
static void       move_constructor_to_outer(FnSymbol*      fn,
                                            AggregateType* outerType);

void build_constructors(AggregateType* ct)
{
  if (ct->defaultInitializer)
    return;

  SET_LINENO(ct);

  build_type_constructor(ct);
  build_constructor(ct);
}


// Create the (default) type constructor for this class.
static void build_type_constructor(AggregateType* ct) {
  // Create the type constructor function,
  FnSymbol* fn = new FnSymbol(astr("_type_construct_", ct->symbol->name));

  fn->addFlag(FLAG_TYPE_CONSTRUCTOR);
  fn->cname = astr("_type_construct_", ct->symbol->cname);

  fn->addFlag(FLAG_COMPILER_GENERATED);
  fn->retTag = RET_TYPE;

  if (ct->symbol->hasFlag(FLAG_REF))
    fn->addFlag(FLAG_REF);

  if (ct->symbol->hasFlag(FLAG_TUPLE)) {
    fn->addFlag(FLAG_TUPLE);
    fn->addFlag(FLAG_INLINE);
  }

  // and insert it into the class type.
  ct->defaultTypeConstructor = fn;

  // Create "this".
  fn->_this = new VarSymbol("this", ct);
  fn->_this->addFlag(FLAG_ARG_THIS);

  fn->insertAtTail(new DefExpr(fn->_this));

  // Walk all fields and select the generic ones.
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
              new_CStringSymbol(field->name),
              new CallExpr(PRIM_TYPE_INIT, exprType->remove())),
            BLOCK_TYPE));

      } else {
        fieldNamesSet.set_add(field->name);

        //
        // if formal is generic
        //
        if (field->isType() || 
            field->hasFlag(FLAG_PARAM)         || 
            (!exprType && !init &&
             // This clause was added to handle runtime types.
             field->type == dtUnknown)) {

          ArgSymbol* arg = create_generic_arg(field);

          fn->insertFormalAtTail(arg);

          if (field->hasFlag(FLAG_PARAM) || field->isType())
            fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                          fn->_this,
                                          new_CStringSymbol(field->name), arg));

          else if (arg->type == dtAny &&
                   !ct->symbol->hasFlag(FLAG_REF))
            // It would be nice to be able to remove this case.
            fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                          fn->_this,
                                          new_CStringSymbol(field->name),
                                          new CallExpr("chpl__initCopy",
                                                       new CallExpr(PRIM_TYPE_INIT, arg))));
          #if 0
          // Leaving this case in for Tom's work.  He will remove it if it is
          // unnecessary
          else
            fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                          fn->_this,
                                          new_CStringSymbol(field->name),
                                          new CallExpr(PRIM_TYPE_INIT, arg)));
          #endif
        } else if (exprType) {
          CallExpr* newInit = new CallExpr(PRIM_TYPE_INIT, exprType->copy());
          CallExpr* newSet  = new CallExpr(PRIM_SET_MEMBER, 
                                           fn->_this,
                                           new_CStringSymbol(field->name),
                                           newInit);
          fn->insertAtTail(newSet);

        } else if (init) {
          fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                        fn->_this,
                                        new_CStringSymbol(field->name),
                                        new CallExpr("chpl__initCopy", init->copy())));
        }
      }
    }
  }

  // Add return
  fn->insertAtTail(new CallExpr(PRIM_RETURN, fn->_this));
  fn->retType = ct;

  ct->symbol->defPoint->insertBefore(new DefExpr(fn));

  // Make implicit references to 'this' explicit.
  insert_implicit_this(fn, fieldNamesSet);

  AggregateType *outerType = toAggregateType(ct->symbol->defPoint->parentSymbol->type);

  if (outerType) {
    // Create an "outer" pointer to the outer class in the inner class
    VarSymbol* outer = new VarSymbol("outer");

    move_constructor_to_outer(fn, outerType);

    // Save the pointer to the outer class
    ct->fields.insertAtTail(new DefExpr(outer));
    fn->insertAtHead(new CallExpr(PRIM_SET_MEMBER,
                                  fn->_this,
                                  new_CStringSymbol("outer"),
                                  fn->_outer));

    ct->outer = outer;
  }

  // Update the symbol table with added defs.
  std::vector<DefExpr*> defs;

  collectDefExprs(fn, defs);
  addToSymbolTable(defs);
}


// For the given class type, this builds the compiler-generated constructor
// which is also called by user-defined constructors to pre-initialize all 
// fields to their declared or type-specific initial values.
static void build_constructor(AggregateType* ct) {
  if (isSyncType(ct))
    ct->defaultValue = NULL;

  // Create the default constructor function symbol,
  FnSymbol* fn = new FnSymbol(astr("_construct_", ct->symbol->name));
  fn->cname = fn->name;

  fn->addFlag(FLAG_DEFAULT_CONSTRUCTOR);
  fn->addFlag(FLAG_CONSTRUCTOR);
  fn->addFlag(FLAG_COMPILER_GENERATED);

  if (ct->symbol->hasFlag(FLAG_REF))
    fn->addFlag(FLAG_REF);

  if (ct->symbol->hasFlag(FLAG_TUPLE)) {
    fn->addFlag(FLAG_TUPLE);
    fn->addFlag(FLAG_INLINE);
  }

  // And insert it into the class type.
  ct->defaultInitializer = fn;

  // Create "this".
  fn->_this = new VarSymbol("this", ct);
  fn->_this->addFlag(FLAG_ARG_THIS);

  fn->insertAtTail(new DefExpr(fn->_this));

  // Walk the fields in the class type.
  std::map<VarSymbol*,ArgSymbol*> fieldArgMap;

  Vec<const char*> fieldNamesSet;

  for_fields(tmp, ct) {
    SET_LINENO(tmp);
    if (VarSymbol* field = toVarSymbol(tmp)) {
      // Filter inherited fields and other special cases.
      // "outer" is used internally to supply a pointer to
      // the outer parent of a nested class.
      if (!field->hasFlag(FLAG_SUPER_CLASS) &&
          !field->hasFlag(FLAG_OMIT_FROM_CONSTRUCTOR) &&
          strcmp(field->name, "_promotionType") &&
          strcmp(field->name, "outer")) {
        // Create an argument to the default constructor
        // corresponding to the field.
        ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, field->name, field->type);

        fieldArgMap[field] = arg;
        fieldNamesSet.set_add(field->name);
      }
    }
  }

  ArgSymbol* meme      = NULL;
  CallExpr*  superCall = NULL;
  CallExpr*  allocCall = NULL;

  if (ct->symbol->hasFlag(FLAG_REF) ||
      isSyncType(ct)) {
    // For ref, sync and single classes, just allocate space.
    allocCall = callChplHereAlloc(fn->_this);
    fn->insertAtTail(new CallExpr(PRIM_MOVE, fn->_this, allocCall));
  } else if (!ct->symbol->hasFlag(FLAG_TUPLE)) {
    // Create a meme (whatever that is).
    meme = new ArgSymbol(INTENT_BLANK,
                         "meme",
                         ct,
                         NULL,
                         new SymExpr(gTypeDefaultToken));
    meme->addFlag(FLAG_IS_MEME);

    // Move the meme into "this".
    fn->insertAtTail(new CallExpr(PRIM_MOVE, fn->_this, meme));

    if (isClass(ct)) {
      if (ct->dispatchParents.n > 0 && !ct->symbol->hasFlag(FLAG_EXTERN)) {
        // This class has a parent class.
        if (!ct->dispatchParents.v[0]->defaultInitializer) {
          // If it doesn't yet have an initializer, make one.
          build_constructors(toAggregateType(ct->dispatchParents.v[0]));
        }

        // Get the parent constructor.
        // Note that since we only pay attention to the first entry in the
        // dispatchParents list, we are effectively implementing
        // single class inheritance, multiple interface inheritance.
        FnSymbol* superCtor = ct->dispatchParents.v[0]->defaultInitializer;

        // Create a call to the superclass constructor.
        superCall = new CallExpr(superCtor->name);

        // Walk the formals of the default super class constructor
        for_formals_backward(formal, superCtor) {
          if (formal->hasFlag(FLAG_IS_MEME))
            continue;

          DefExpr* superArg = formal->defPoint->copy();

          // Omit the arguments shadowed by this class's fields.
          if (fieldNamesSet.set_in(superArg->sym->name))
            continue;

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
          new CallExpr(PRIM_MOVE,
                       tmp,
                       new CallExpr(PRIM_GET_MEMBER_VALUE,
                                    fn->_this,
                                    new_CStringSymbol("super"))));
      }
    }
  }

  if (isUnion(ct))
    fn->insertAtTail(new CallExpr(PRIM_SET_UNION_ID, fn->_this, new_IntSymbol(0)));

  ct->symbol->defPoint->insertBefore(new DefExpr(fn));

  for_fields(tmp, ct) {
    VarSymbol* field = toVarSymbol(tmp);

    if (!field)
      continue;

    if (fieldArgMap.count(field) == 0)
      continue;

    ArgSymbol* arg = fieldArgMap[field];

    SET_LINENO(field);

    if (field->hasFlag(FLAG_PARAM))
      arg->intent = INTENT_PARAM;

    Expr* exprType = field->defPoint->exprType;
    Expr* init     = field->defPoint->init;

    bool  hadType  = exprType;
    bool  hadInit  = init;

    if (exprType != NULL) {
      exprType->remove();
    }

    if (init != NULL) {
      init->remove();
    }

    if (init) {
      if (!field->isType() && !exprType) {
        // init && !exprType
        VarSymbol* tmp = newTemp();
#ifndef HILDE_MM
        tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
#endif
        tmp->addFlag(FLAG_MAYBE_PARAM);
        tmp->addFlag(FLAG_MAYBE_TYPE);

        exprType = new BlockStmt(new DefExpr(tmp), BLOCK_TYPE);

        toBlockStmt(exprType)->insertAtTail(new CallExpr(PRIM_MOVE,
                                                         tmp,
                                                         new CallExpr("chpl__initCopy",
                                                                      init->copy())));

        toBlockStmt(exprType)->insertAtTail(new CallExpr(PRIM_TYPEOF, tmp));
      }

    } else if (hadType &&
               !field->isType() &&
               !field->hasFlag(FLAG_PARAM)) {
      init = new CallExpr(PRIM_INIT, exprType->copy());
    }

    if (!field->isType() && !field->hasFlag(FLAG_PARAM)) {
      if (hadType)
        init = new CallExpr("_createFieldDefault", exprType->copy(), init);
      else if (init)
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

    if (field->isType())
      // Args with this flag are removed after resolution.
      // Note that in the default type constructor, this flag is also applied
      // (along with FLAG_GENERIC) to arguments whose type is unknown, but would
      // not be pruned in resolution.
      arg->addFlag(FLAG_TYPE_VARIABLE);

    if (!exprType && arg->type == dtUnknown)
      arg->type = dtAny;

    fn->insertFormalAtTail(arg);

    if (arg->type == dtAny && !arg->hasFlag(FLAG_TYPE_VARIABLE) &&
        !arg->hasFlag(FLAG_PARAM) && !ct->symbol->hasFlag(FLAG_REF))
      fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, 
                                    fn->_this, 
                                    new_CStringSymbol(arg->name),
                                    new CallExpr("chpl__initCopy", arg)));
    else
      // Since we don't copy the argument before stuffing it in a field,
      // we will have to remove the autodestroy flag for specific cases.
      // Namely, if the function is a default constructor and the target
      // of a PRIM_SET_MEMBER is a record, then the INSERT_AUTO_DESTROY
      // flag must be removed.
      // (See NOTE 1 in callDestructors.cpp.)
      fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, 
                                    fn->_this, 
                                    new_CStringSymbol(arg->name),
                                    arg));
  }

  if (meme)
    fn->insertFormalAtTail(meme);

  insert_implicit_this(fn, fieldNamesSet);

  AggregateType *outerType = toAggregateType(ct->symbol->defPoint->parentSymbol->type);

  if (outerType) {
    move_constructor_to_outer(fn, outerType);

    // Save the pointer to the outer class
    fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER,
                                  fn->_this,
                                  new_CStringSymbol("outer"),
                                  fn->_outer));
  }

  //
  // Insert a call to the "initialize()" method if one is defined.
  // The return value of this method (if any) is ignored.
  //
  forv_Vec(FnSymbol, method, ct->methods) {
    // Select a method named "initialize" and taking no arguments
    // (aside from _mt and the implicit 'this').
    if (method && !strcmp(method->name, "initialize")) {
      if (method->numFormals() == 2) {
        CallExpr* init = new CallExpr("initialize", gMethodToken, fn->_this);
        fn->insertAtTail(init);
        // If a type has an initialize method, it's not Plain Old Data.
        ct->symbol->addFlag(FLAG_NOT_POD);
        break;
      }
    }
  }

  fn->insertAtTail(new CallExpr(PRIM_RETURN, fn->_this));

  std::vector<DefExpr*> defs;
  collectDefExprs(fn, defs);
  addToSymbolTable(defs);
}

static ArgSymbol* create_generic_arg(VarSymbol* field)
{
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, field->name, field->type);

  // We take it as a param argument if it is marked as a param field.
  if (field->hasFlag(FLAG_PARAM))
    arg->intent = INTENT_PARAM;
  else
    // Both type arguments and arguments of unspecified type get this flag.
    arg->addFlag(FLAG_TYPE_VARIABLE);

  // Copy the field type if it exists.
  Expr* exprType = field->defPoint->exprType;
  if (exprType)
    arg->typeExpr = new BlockStmt(exprType->copy(), BLOCK_TYPE);

  // Copy the initialization expression if it exists.
  Expr* init = field->defPoint->init;
  if (init)
    arg->defaultExpr = new BlockStmt(init->copy(), BLOCK_SCOPELESS);

  // Translate an unknown field type into an unspecified arg type.
  if (!exprType && arg->type == dtUnknown)
  {
    if (! field->isType())
      arg->addFlag(FLAG_GENERIC);
    arg->type = dtAny;
  }

  return arg;
}

/// Replace implicit references to 'this' in the body of this 
/// type constructor with explicit member reference (dot) expressions.
static void insert_implicit_this(FnSymbol* fn, Vec<const char*>& fieldNamesSet)
{
  std::vector<BaseAST*> asts;

  collect_asts(fn->body, asts);

  for_vector(BaseAST, ast, asts) {
    if (UnresolvedSymExpr* se = toUnresolvedSymExpr(ast))
      if (fieldNamesSet.set_in(se->unresolved))
        // The name of this UnresolvedSymExpr matches a field name.
        // So replace it with a dot expression.
        se->replace(buildDotExpr(fn->_this, se->unresolved));
  }
}

static void move_constructor_to_outer(FnSymbol* fn, AggregateType* outerType)
{
  // Remove the DefPoint for this constructor, add it to the outer
  // class's method list.
  outerType->methods.add(fn);

  fn->_outer = new ArgSymbol(INTENT_BLANK, "outer", outerType);
  fn->_outer->addFlag(FLAG_GENERIC); // Arg expects a real object :-P.
  fn->insertFormalAtHead(new DefExpr(fn->_outer));
  fn->insertFormalAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK,
                                                   "_mt",
                                                   dtMethodToken)));
  fn->addFlag(FLAG_METHOD);

  fn->addFlag(FLAG_METHOD_PRIMARY);

  Expr* insertPoint = outerType->symbol->defPoint;

  while (toTypeSymbol(insertPoint->parentSymbol))
    insertPoint = insertPoint->parentSymbol->defPoint;

  insertPoint->insertBefore(fn->defPoint->remove());
}


/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void resolveGotoLabels() {
  forv_Vec(GotoStmt, gs, gGotoStmts) {
    SET_LINENO(gs);

    if (SymExpr* label = toSymExpr(gs->label)) {
      if (label->var == gNil) {
        LoopStmt* loop = LoopStmt::findEnclosingLoop(gs);

        if (!loop)
          USR_FATAL(gs, "break or continue is not in a loop");

        if (gs->gotoTag == GOTO_BREAK) {
          Symbol* breakLabel = loop->breakLabelGet();

          INT_ASSERT(breakLabel);
          gs->label->replace(new SymExpr(breakLabel));

        } else if (gs->gotoTag == GOTO_CONTINUE) {
          Symbol* continueLabel = loop->continueLabelGet();
          INT_ASSERT(continueLabel);

          gs->label->replace(new SymExpr(continueLabel));

        } else
          INT_FATAL(gs, "unexpected goto type");
      }

    } else if (UnresolvedSymExpr* label = toUnresolvedSymExpr(gs->label)) {
      const char* name = label->unresolved;
      LoopStmt*   loop = LoopStmt::findEnclosingLoop(gs);

      while (loop && (!loop->userLabel || strcmp(loop->userLabel, name))) {
        loop = LoopStmt::findEnclosingLoop(loop->parentExpr);
      }

      if (!loop) {
        USR_FATAL(gs, "bad label on break or continue");
      }

      if (gs->gotoTag == GOTO_BREAK)
        label->replace(new SymExpr(loop->breakLabelGet()));

      else if (gs->gotoTag == GOTO_CONTINUE)
        label->replace(new SymExpr(loop->continueLabelGet()));

      else
        INT_FATAL(gs, "unexpected goto type");
    }
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void resolveUnresolvedSymExpr(UnresolvedSymExpr*       unresolvedSymExpr,
                                     Vec<UnresolvedSymExpr*>& skipSet);

static void resolveModuleCall(CallExpr* call, Vec<UnresolvedSymExpr*>& skipSet);
static bool isMethodName(const char* name, Type* type);
static bool isMethodNameLocal(const char* name, Type* type);

#ifdef HAVE_LLVM
static bool tryCResolve(ModuleSymbol* module, const char* name);
static bool tryCResolve_set(ModuleSymbol*                     module,
                            const char*                       name,
                            llvm::SmallSet<ModuleSymbol*, 24> already_checked);
#endif

static void resolveUnresolvedSymExprs()
{
  Vec<UnresolvedSymExpr*> skipSet;

  //
  // Translate M.x where M is a ModuleSymbol into just x where x is
  // the symbol in module M; for functions, insert a "module=" token
  // that is used to determine visible functions.
  //

  int max_resolved = 0;

  forv_Vec(UnresolvedSymExpr, unresolvedSymExpr, gUnresolvedSymExprs) {
    resolveUnresolvedSymExpr(unresolvedSymExpr, skipSet);
    max_resolved++;
  }

  forv_Vec(CallExpr, call, gCallExprs) {
    resolveModuleCall(call, skipSet);
  }

  int i = 0;

  // Note that the extern C resolution might add new UnresolvedSymExprs, and it
  // might do that within resolveModuleCall, so we try resolving unresolved
  // symbols a second time as the extern C block support might have added some.
  // Alternatives include:
  //  - have the extern C wrapper-builder directly call resolveUnresolved
  //      (but that complicates the way scopeResolve works now)
  //  - import all C symbols at an earlier point
  //      (but that might add lots of unused garbage to the Chapel AST
  //       for e.g. #include <stdio.h>; and it might cause the C-to-Chapel
  //       translater to need to handle more platform/compiler-specific stuff,
  //       and it might lead to extra naming conflicts).
  forv_Vec(UnresolvedSymExpr, unresolvedSymExpr, gUnresolvedSymExprs) {
    // Only try resolving symbols that are new after last attempt.
    if( i >= max_resolved ) {
      resolveUnresolvedSymExpr(unresolvedSymExpr, skipSet);
    }
    i++;
  }

  skipSet.clear();
}

static void resolveUnresolvedSymExpr(UnresolvedSymExpr* unresolvedSymExpr,
                                     Vec<UnresolvedSymExpr*>& skipSet) {
  if (skipSet.set_in(unresolvedSymExpr))
    return;

  const char* name = unresolvedSymExpr->unresolved;
  if (!strcmp(name, "."))
    return;

  // Skip unresolveds that are not in the tree.
  if (!unresolvedSymExpr->parentSymbol)
    return;

  SET_LINENO(unresolvedSymExpr);

  Symbol* sym = lookup(unresolvedSymExpr, name);

  //
  // handle function call without parentheses
  //
  if (FnSymbol* fn = toFnSymbol(sym)) {
    if (!fn->_this && fn->hasFlag(FLAG_NO_PARENS)) {
      unresolvedSymExpr->replace(new CallExpr(fn));
      return;
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
      Expr* parent = unresolvedSymExpr->parentExpr;

      if (parent) {
        CallExpr *call = toCallExpr(parent);

        if (((call) && (call->baseExpr != unresolvedSymExpr)) || (!call)) {
          //If the function is being used as a first-class value, handle
          // this with a primitive and unwrap the primitive later in
          // functionResolution
          CallExpr *prim_capture_fn = new CallExpr(PRIM_CAPTURE_FN);

          unresolvedSymExpr->replace(prim_capture_fn);
          prim_capture_fn->insertAtTail(unresolvedSymExpr);

          // Don't do it again if for some reason we return
          // to trying to resolve this symbol.
          skipSet.set_add(unresolvedSymExpr);
          return;
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
          Type*       type = method->_this->type;
          TypeSymbol* cts  =
            (sym) ? toTypeSymbol(sym->defPoint->parentSymbol) : NULL;

          if ((cts && isAggregateType(cts->type)) ||
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
              AggregateType* ct        = toAggregateType(type);
              int            nestDepth = 0;

              if (isMethodName(name, type)) {
                while (ct && !isMethodNameLocal(name, ct)) {
                  // count how many classes out from current depth that
                  // this method is first defined in
                  nestDepth += 1;
                  ct = toAggregateType
                    (ct->symbol->defPoint->parentSymbol->type);
                }
              } else {
                while (ct && !ct->getField(name, false)) {
                  // count how many classes out from current depth that
                  // this symbol is first defined in
                  nestDepth += 1;
                  ct = toAggregateType(ct->symbol->defPoint->parentSymbol->type);
                }
              }

              Expr *dot = NULL;

              for (int i=0; i<=nestDepth; i++) {
                // Apply implicit this pointers and outer this pointers
                if (i == 0) {
                  if (i < nestDepth) {
                    dot = new CallExpr(".",
                                       method->_this,
                                       new_CStringSymbol("outer"));
                  } else {
                    if (isTypeSymbol(sym))
                      dot = new CallExpr(".", method->_this, sym);
                    else
                      dot = new CallExpr(".",
                                         method->_this,
                                         new_CStringSymbol(name));
                  }
                } else {
                  if (i < nestDepth) {
                    dot = new CallExpr(".",
                                       dot, new_CStringSymbol("outer"));
                  } else {
                    if (isTypeSymbol(sym))
                      dot = new CallExpr(".", dot, sym);
                    else
                      dot = new CallExpr(".", dot, new_CStringSymbol(name));
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

#ifdef HAVE_LLVM
  if (!sym && externC && tryCResolve(unresolvedSymExpr->getModule(), name)) {
    //try resolution again since the symbol should exist now
    resolveUnresolvedSymExpr(unresolvedSymExpr, skipSet);
  }
#endif
}

//
// isMethodName returns true iff 'name' names a method of 'type'
//
static bool isMethodName(const char* name, Type* type) {
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

  if (AggregateType* ct = toAggregateType(type)) {
    Type *outerType = ct->symbol->defPoint->parentSymbol->type;

    if (AggregateType* outer = toAggregateType(outerType))
      if (isMethodName(name, outer))
        return true;
  }

  return false;
}


//
// isMethodNameLocal returns true iff 'name' names a method of 'type'
// excluding methods of an outer type
//
static bool isMethodNameLocal(const char* name, Type* type) {
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

static void resolveModuleCall(CallExpr* call, Vec<UnresolvedSymExpr*>& skipSet) {
  if (call->isNamed(".")) {
    if (SymExpr* se = toSymExpr(call->get(1))) {
      if (ModuleSymbol* mod = toModuleSymbol(se->var)) { 
        ModuleSymbol* enclosingModule = call->getModule();

        enclosingModule->moduleUseAdd(mod);

        SET_LINENO(call);

        Symbol*           sym      = NULL;
        const char*       mbr_name = get_string(call->get(2));

        // Can the identifier be mapped to something at this scope?
        if (symbolTable.count(mod->block) != 0) {
          SymbolTableEntry* entry = symbolTable[mod->block];

          if (entry->count(mbr_name) != 0) {
            sym = (*entry)[mbr_name];
          }
        }

        if (sym) {
          if (!sym->isVisible(call)) {
            // The symbol is not visible at this scope because it is
            // private to mod!  Error out
            USR_FATAL(call, "Cannot access '%s', '%s' is private to '%s'", mbr_name, mbr_name, mod->name);
          } else if (FnSymbol* fn = toFnSymbol(sym)) {
            if (!fn->_this && fn->hasFlag(FLAG_NO_PARENS)) {
              call->replace(new CallExpr(fn));
            } else {
              UnresolvedSymExpr* se = new UnresolvedSymExpr(mbr_name);

              skipSet.set_add(se);
              call->replace(se);

              CallExpr* parent = toCallExpr(se->parentExpr);
              INT_ASSERT(parent);

              parent->insertAtHead(mod);
              parent->insertAtHead(gModuleToken);
            }
          } else {
            call->replace(new SymExpr(sym));
          }
#ifdef HAVE_LLVM
        } else if (!sym && externC && tryCResolve(call->getModule(),mbr_name)) {
          // Try to resolve again now that the symbol should
          // be in the table
          resolveModuleCall(call, skipSet);
#endif

        } else {
          USR_FATAL_CONT(call,
                         "Symbol '%s' undeclared in module '%s'",
                         mbr_name,
                         mod->name);
        }
      }
    }
  }
}

#ifdef HAVE_LLVM
static bool tryCResolve(ModuleSymbol* module, const char* name) {
  llvm::SmallSet<ModuleSymbol*, 24> already_checked;

  return tryCResolve_set(module, name, already_checked);
}

static bool tryCResolve_set(ModuleSymbol* module, const char* name,
  llvm::SmallSet<ModuleSymbol*, 24> already_checked) {

  if (! module) return false;

  if (llvm_small_set_insert(already_checked, module)) {
   // already_checked.insert(module)) {
    // we added it to the set, so continue.
  } else {
    // It was already in the set.
    return false;
  }

  // Is it resolveable in this module?
  if (module->extern_info) {
    // Try resolving it
    Vec<Expr*> c_exprs;

    // Try to create an extern declaration for name,
    //  if it exists in the module's extern blocks.
    // The resulting Chapel extern declarations are put into
    //  c_exprs and will need to be resolved.
    convertDeclToChpl(module, name, c_exprs);

    if (c_exprs.count()) {
      forv_Vec(Expr*, c_expr, c_exprs) {
        Vec<DefExpr*> v;

        collectDefExprs(c_expr, v);

        addToSymbolTable(v);

        if (DefExpr* de = toDefExpr(c_expr)) {
          if (TypeSymbol* ts = toTypeSymbol(de->sym)) {
            if (AggregateType* ct = toAggregateType(ts->type)) {
              SET_LINENO(ct->symbol);
              // If this is a class DefExpr,
              //  make sure its initializer gets created.
              build_constructors(ct);
            }
          }
        }
      }

      //any new UnresolvedSymExprs will be called in another for loop
      // in scopeResolve.
      return true;
    }
  }

  // Otherwise, try the modules used by this module.
  forv_Vec(ModuleSymbol, usedMod, module->modUseList) {
    bool got = tryCResolve_set(usedMod, name, already_checked);

    if( got ) return true;
  }

  return false;
}

#endif


/************************************ | *************************************
*                                                                           *
* resolves EnumTypeName.fieldName to the symbol named fieldName in the      *
* enumerated type named EnumTypeName                                        *
*                                                                           *
************************************* | ************************************/

static void resolveEnumeratedTypes() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isNamed(".")) {
      SET_LINENO(call);

      if (SymExpr* first = toSymExpr(call->get(1))) {
        if (EnumType* type = toEnumType(first->var->type)) {
          if (SymExpr* second = toSymExpr(call->get(2))) {
            const char* name;
            bool found = false;

            CallExpr* parent = toCallExpr(call->parentExpr);
            if( parent && parent->baseExpr == call ) {
              // This is a call e.g.
              // myenum.method( a )
              // aka call(call(. myenum "method") a)
              // so that call needs to go through normalize and resolve
              continue;
            }


            INT_ASSERT(get_string(second, &name));

            for_enums(constant, type) {
              if (!strcmp(constant->sym->name, name)) {
                call->replace(new SymExpr(constant->sym));
                found = true;
              }
            }

            if (!found) {
              USR_FATAL(call, 
                        "unresolved enumerated type symbol \"%s\"",
                        name);
            }
          }
        }
      }
    }
  }
}

/************************************ | *************************************
*                                                                           *
* delete the symbol table and module uses cache                             *
*                                                                           *
************************************* | ************************************/

static void destroyTable() {
  SymbolTable::iterator entry;

  for (entry = symbolTable.begin(); entry != symbolTable.end(); entry++) {
    delete entry->second;
  }

  symbolTable.clear();
}


//
// delete the module uses cache
//
static void destroyModuleUsesCaches() {
  std::map<BlockStmt*,Vec<ModuleSymbol*>*>::iterator use;

  for (use = moduleUsesCache.begin(); use != moduleUsesCache.end(); use++) {
    delete use->second;
  }

  moduleUsesCache.clear();
}


/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void renameDefaultType(Type* type, const char* newname);

static void renameDefaultTypesToReflectWidths(void) {
  renameDefaultType(dtInt[INT_SIZE_DEFAULT],         "int(64)");
  renameDefaultType(dtUInt[INT_SIZE_DEFAULT],        "uint(64)");
  renameDefaultType(dtReal[FLOAT_SIZE_DEFAULT],      "real(64)");
  renameDefaultType(dtImag[FLOAT_SIZE_DEFAULT],      "imag(64)");
  renameDefaultType(dtComplex[COMPLEX_SIZE_DEFAULT], "complex(128)");
}

static void renameDefaultType(Type* type, const char* newname) {
  if (strchr(type->symbol->name, '(') != NULL) {
    INT_FATAL("Renaming a default type that already seems to have a width");
  }

  type->symbol->name = astr(newname);
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void lookup(BaseAST* scope, const char * name,
                   std::vector<Symbol* >& symbols,
                   Vec<BaseAST*>& alreadyVisited,
                   std::set<int> rejectedPrivateIds,
                   BaseAST* callingContext);



static void    buildBreadthFirstModuleList(Vec<ModuleSymbol*>* modules);

static void    buildBreadthFirstModuleList(Vec<ModuleSymbol*>* modules,
                                           Vec<ModuleSymbol*>* current,
                                           Vec<ModuleSymbol*>* alreadySeen);

// Given a name and a scope, determine the symbol referred by that name in the
// context of that scope.
static Symbol* lookup(BaseAST* scope, const char* name) {
  Symbol * symbolResult = NULL;
  std::vector<Symbol * > symbolOptions;
  Vec<BaseAST*> nestedscopes;

  std::set<int> rejectedPrivateIds;
  // rejectedPrivateIds is a set of all ids for private symbols that we have
  // encountered and determined are not visible to us.  Though it adds storage
  // and requires a check of all later symbol matches once it has any contents,
  // it allows us to go back to a scope we may have partially traversed (and
  // left early due to finding the incorrect private symbol match).  It also
  // means that, in the event a private symbol lives in the internal or commonly
  // used standard modules, the user will not be flooded with multiple output
  // for the same incorrect match.

  // Lydia note: I would like to investigate the effect of traversing all call
  // expressions of the "." accessor.


  // Call inner lookup on scope, the name, the symbols return vector, and the
  // vector of ASTs already visited.
  lookup(scope, name, symbolOptions, nestedscopes, rejectedPrivateIds, scope);

  int numFound = symbolOptions.size();
  if (numFound == 0) {
    // No symbols found for this name
    symbolResult = NULL;
  } else if (numFound == 1) {
    // A unique symbol found for this name
      symbolResult = symbolOptions.front();
  } else {
    // Multiple symbols found for this name.  If at least one of them isn't
    // a function, we need to handle it now so error out.  Otherwise, function
    // resolution will handle it.
    for_vector(Symbol, sym, symbolOptions) {
      if (!isFnSymbol(sym))
        USR_FATAL_CONT(sym, "Symbol %s multiply defined", name);
    }
    USR_STOP();
    symbolResult = NULL;
  }

  return symbolResult;
  // return symbol matching the name
}

// Determines and obtains a method by the given name on the given type
//
// This function uses the same methodology as isMethodName but returns the
// symbol found instead of just a boolean
static FnSymbol* getMethod(const char* name, Type* type) {
  if (strcmp(name, type->symbol->name) == 0)
    return NULL;

  // Looks for name in methods defined directly on this type
  forv_Vec(FnSymbol, method, type->methods) {
    if (method && !strcmp(name, method->name))
      return method;
  }

  // Looks for name in methods defined on parent types
  forv_Vec(Type, pt, type->dispatchParents) {
    if (FnSymbol *sym = getMethod(name, pt))
      return sym;
  }

  // Looks for name in types wrapping this type definition
  if (AggregateType* ct = toAggregateType(type)) {
    Type *outerType = ct->symbol->defPoint->parentSymbol->type;

    if (AggregateType* outer = toAggregateType(outerType))
      if (FnSymbol *sym = getMethod(name, outer))
        return sym;
  }

  return NULL;
}

// For a scope and a given method, determine if the method is visible in this
// scope
// Lydia note (2015/06/26)
// Semantic issue not handled by this function: when a parenthesis-less method
// is defined by an outside module and the use of that module is at the same
// scope as another symbol of the same name as the method, which symbol should
// take precedent?  When the use is not present, both the prior version of
// scopeResolve and the current version don't resolve the symbol, leaving the
// decision to function resolution, which thinks it should have gotten the
// method that wasn't available
static bool methodMatched(BaseAST* scope, FnSymbol* method) {
  if (method->_this->type->symbol == scope) {
    return true;
  } else {
    BaseAST* curScope = getScope(scope);
    // Traverse up the scopes either until we find this method or until there
    // are no more scopes to traverse
    while (curScope) {
      if (TypeSymbol* ts = toTypeSymbol(scope)) {
        // Are we in a type symbol?
        if (Symbol* sym = getMethod(method->name, ts->type)) {
          // Does that type symbol have a method with the same name?
          if (sym == method) {
            // Is it us?
            return true;
          } else {
            // We are not in scope
            return false;
          }
        }
      }
      curScope = getScope(curScope);
    }
    return false;
  }
}

// inSymbolTable returns a Symbol* if there was an entry for this scope
// that matched this name, NULL otherwise.
static Symbol* inSymbolTable(BaseAST* scope, const char* name) {
  if (symbolTable.count(scope) != 0) {
    SymbolTableEntry* entry = symbolTable[scope];
    if (entry->count(name) != 0) {
      Symbol* sym = (*entry)[name];
      // If the symbol found isn't a method, or it was a method and we are
      // in the appropriate scope to add it (as determined by calling
      // methodMatched), then return the symbol
      FnSymbol* fn = toFnSymbol(sym);
      if (sym && (!sym->hasFlag(FLAG_METHOD) ||
                  (fn && (methodMatched(scope, fn)))))
        return sym;
    }
  }
  return NULL;
}

// If the current scope is an aggregate type, checks if the name refers to a
// field or method on that type.  If a match is found, return it.  Otherwise
// return NULL.
static Symbol* inType(BaseAST* scope, const char* name) {
  if (TypeSymbol* ts = toTypeSymbol(scope)) {
    if (AggregateType* ct = toAggregateType(ts->type)) {
      if (Symbol* sym = ct->getField(name, false)) {
        return sym;
      } else if (Symbol* fn = getMethod(name, ct)) {
        // There is a method of that name, is it visible?
        if (methodMatched(scope, toFnSymbol(fn))) {
          return fn;
        }
      }
    }
  }
  return NULL;
}

// Returns true if the symbol is already present in the vector, false otherwise
static bool isRepeat(std::vector<Symbol* >& symbols, Symbol* toAdd) {
  for (std::vector<Symbol* >::iterator it = symbols.begin();
       it != symbols.end(); ++it) {
    if (*it == toAdd) {
      return true;
    }
  }
  return false;
}

// Assumes that symbols contains nothing before entering this function
static bool lookupThisScopeAndUses(BaseAST* scope, const char * name,
                                   std::vector<Symbol* >& symbols,
                                   std::set<int> rejectedPrivateIds,
                                   BaseAST* callingContext) {
  INT_ASSERT(symbols.size() == 0);

  if (Symbol* sym = inSymbolTable(scope, name)) {
    if (sym->hasFlag(FLAG_PRIVATE)) {
      if (rejectedPrivateIds.find(sym->id) == rejectedPrivateIds.end()) {
        // The symbol found was not one of the already rejected private
        // symbols
        if (!sym->isVisible(callingContext)) {
          rejectedPrivateIds.insert(sym->id);
          USR_WARN(callingContext, "A visible '%s' is an inaccessible private symbol, defined at:\n %s", name, sym->stringLoc());
        } else {
          symbols.push_back(sym);
        }
      } // If it was already rejected, we definitely don't want to add it.
    } else {
      symbols.push_back(sym);
    }
  }

  if (Symbol* sym = inType(scope, name)) {
    if (isRepeat(symbols, sym)) {
      // If we're looking at the exact same Symbol, there's no need to add it
      // and we can just return.
      return true;
    }
    // When methods and fields can be private, need to check against the
    // rejected private symbols here.  But that's in the future.
    symbols.push_back(sym);
  }

  if (symbols.size() == 0) {
    // Nothing found so far, look into the uses.
    if (BlockStmt* block = toBlockStmt(scope)) {
      if (block->modUses) {
        Vec<ModuleSymbol*>* modules = NULL;

        if (moduleUsesCache.count(block) == 0) {
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
            moduleUsesCache[block] = modules;
        } else {
          modules = moduleUsesCache[block];
        }

        forv_Vec(ModuleSymbol, mod, *modules) {
          if (mod) {
            if (Symbol* sym = inSymbolTable(mod->block, name)) {
              if (sym->hasFlag(FLAG_PRIVATE)) {
                if (rejectedPrivateIds.find(sym->id) ==
                    rejectedPrivateIds.end()) {
                  // The symbol found was not one of the already rejected
                  // private symbols
                  if (!sym->isVisible(callingContext)) {
                    rejectedPrivateIds.insert(sym->id);
                    USR_WARN(callingContext, "A visible '%s' is an inaccessible private symbol, defined at:\n %s", name, sym->stringLoc());
                  } else {
                    if (!isRepeat(symbols, sym)) {
                      symbols.push_back(sym);
                    }
                  }
                }
                // If it was already rejected, we don't want to add it.

              } else if (!isRepeat(symbols, sym)) {
                // Don't want to add if the symbol itself was already present.
                symbols.push_back(sym);
              }
            }
          } else {
            //
            // break on each new depth if a symbol has been found
            //
            if (symbols.size() > 0)
              break;
          }
        }

        if (symbols.size() > 0) {
          // We found a symbol in the module use.  This could conflict with the
          // function symbol's arguments if we are at the top level scope
          // within a function.  Note that we'd check the next scope up if
          // size() == 0, so we only need to do this check here because the
          // module case would hide it otherwise
          if (FnSymbol* fn = toFnSymbol(getScope(block))) {
            // The next scope up from the block statement is a function
            // symbol. That means that we need to check the arguments
            if (Symbol* sym = inSymbolTable(fn, name)) {
              // We found it in the arguments.  This should cause a conflict,
              // because it is probably an error that the user had the same
              // name as a module level variable.
              USR_WARN(sym, "Module level symbol is hiding function argument '%s'", name);
              //symbols.push_back(sym);
              // If we wanted this to be an error case, uncomment the above line
            }
          }
        }
      }
    }
  }

  return symbols.size() != 0;
}


// Recursive look up - separates the checks which occur in the scope from the
// steps that occur to get the next scope.
//
// Note that having this set up would make it easier to check the entirety of
// an access call (M1.M2.M3, for instance) as the recursion does not occur in
// the innermost scope.
static void lookup(BaseAST* scope, const char * name,
                   std::vector<Symbol* >& symbols,
                   Vec<BaseAST*>& alreadyVisited,
                   std::set<int> rejectedPrivateIds,
                   BaseAST* callingContext) {
  if (!alreadyVisited.set_in(scope)) {
    alreadyVisited.set_add(scope);

    if (lookupThisScopeAndUses(scope, name, symbols, rejectedPrivateIds, callingContext)) {
      // We've found an instance here.
      // Lydia note: in the access call case, we'd want to look in our
      // surrounding scopes for the symbols on the left and right part of the
      // call (if any) to verify we were finding anything in particular.
      // A symbol could be visible in the innermost scope because it was defined
      // in an outer scope (for instance, if M1 defines foo, M2 doesn't shadow
      // it and we're looking for M1.M2.foo), so that is something to keep in
      // mind as well.

      return;
    }

    if (scope->getModule()->block == scope) {
      if (getScope(scope))
        lookup(getScope(scope), name, symbols, alreadyVisited, rejectedPrivateIds, callingContext);
    } else {
      // Otherwise, look in the next scope up.
      FnSymbol* fn = toFnSymbol(scope);
      if (fn && fn->_this) {
        // If we're currently in a method, the next scope up is anything visible
        // within the aggregate type
        AggregateType* ct = toAggregateType(fn->_this->type);
        if (ct)
          lookup(ct->symbol, name, symbols, alreadyVisited, rejectedPrivateIds, callingContext);
      }
      // Check if found something in last lookup call
      if (symbols.size() == 0) {
        // If we didn't find something in the aggregate type that matched, or we
        // weren't in an aggregate type method, so look at next scope up.
        lookup(getScope(scope), name, symbols, alreadyVisited, rejectedPrivateIds, callingContext);
      }
    }
  }
}

static void buildBreadthFirstModuleList(Vec<ModuleSymbol*>* modules) {
  Vec<ModuleSymbol*> seen;

  return buildBreadthFirstModuleList(modules, modules, &seen);
}

// If the uses of a particular module are considered its level 1 uses, then
// this function will only add level 2 and lower uses to the modules vector
// argument.
static void buildBreadthFirstModuleList(Vec<ModuleSymbol*>* modules,
                                        Vec<ModuleSymbol*>* current,
                                        Vec<ModuleSymbol*>* alreadySeen) {
  modules->add(NULL); // use NULL sentinel to identify modules of equal depth

  Vec<ModuleSymbol*> next;

  forv_Vec(ModuleSymbol, module, *current) {
    if (!module) {
      break;
    } else if (module->block->modUses) {
      for_actuals(expr, module->block->modUses) {
        SymExpr*      se  = toSymExpr(expr);
        INT_ASSERT(se);

        ModuleSymbol* mod = toModuleSymbol(se->var);
        INT_ASSERT(mod);

        if (!alreadySeen->set_in(mod)) {
          if (!mod->hasFlag(FLAG_PRIVATE)) {
            // Uses of private modules are not transitive - the symbols in the
            // private modules are only visible to itself and its immediate
            // parent.  Therefore, if the symbol is private, we will not
            // traverse it further and will merely add it to the alreadySeen
            // vector.
            next.add(mod);
            modules->add(mod);
          }
          alreadySeen->set_add(mod);
        }
      }
    }
  }

  if (next.n) {
    buildBreadthFirstModuleList(modules, &next, alreadySeen);
  }
}

// Returns true if this module is capable of being used or traversed as part of
// an access in the provided scope, false if the module is private and the
// scope is not in its direct parent
bool Symbol::isVisible(BaseAST* scope) const {
  if (!hasFlag(FLAG_PRIVATE)) {
    // If it isn't public, it is trivially visible.
    return true;
  } else {
    BaseAST* parentScope = getScope(defPoint);
    INT_ASSERT(parentScope != NULL); // Should be true, given we found this
    // module symbol.

    // We need to walk up scopes until we either find our parent scope (in
    // which case, we're visible if it "use"s us) or we run out of scope to
    // check against (in which case we are most certainly *not* visible)
    BaseAST* searchScope = scope;
    while (searchScope != NULL) {
      if (searchScope == parentScope) {
        return true;
      }

      searchScope = getScope(searchScope);
      // Keep walkin', we didn't find the parent scope yet.
    }

    // We got to the top of the scope without finding the parent.
    return false;
  }
}

/************************************ | *************************************
*                                                                           *
* getScope returns the BaseAST that corresponds to the scope where 'ast'    *
* exists; 'ast' must be an Expr or a Symbol.  Note that if you pass this a  *
* BaseAST that defines a scope, the BaseAST that defines the scope where it *
* exists will be returned.                                                  *
*                                                                           *
* Thus if a BlockStmt nested in another BlockStmt is passed to getScope,    *
* the outer BlockStmt will be returned.                                     *
*                                                                           *
************************************* | ************************************/

static BaseAST* getScope(BaseAST* ast) {
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
      if (isEnumType(ts->type) || isAggregateType(ts->type)) {
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

