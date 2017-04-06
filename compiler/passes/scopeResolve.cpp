/*
 * Copyright 2004-2017 Cray Inc.
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
#include "initializerRules.h"
#include "LoopStmt.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

#include <algorithm>
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
static std::map<BlockStmt*,Vec<UseStmt*>*> moduleUsesCache;
static bool                                enableModuleUsesCache = false;

//
// The aliasFieldSet is a set of names of fields for which arrays may
// be passed in by named argument as aliases, as in new C(A=>GA) (see
// test/arrays/deitz/test_array_alias_field.chpl).
//
static Vec<const char*>                         aliasFieldSet;


// To avoid duplicate user warnings in checkIdInsideWithClause().
// Using pair<> instead of astlocT to avoid defining operator<.
typedef std::pair< std::pair<const char*,int>, const char* >  WFDIWmark;
static std::set< std::pair< std::pair<const char*,int>, const char* > > warnedForDotInsideWith;


static void     addToSymbolTable(Vec<DefExpr*>& defs);
static void     addToSymbolTable(std::vector<DefExpr*>& defs);

static void     processImportExprs();

static void     addClassToHierarchy(AggregateType* ct);

static void     addRecordDefaultConstruction();

static void     setCreationStyle(TypeSymbol* t, FnSymbol* fn);

static void     resolveGotoLabels();
static void     resolveUnresolvedSymExprs();
static void     resolveEnumeratedTypes();

static void     destroyTable();
static void     destroyModuleUsesCaches();

static void     renameDefaultTypesToReflectWidths();

static Symbol*  lookup(BaseAST* scope, const char* name);

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
    if (strncmp(ne->name, "chpl__aliasField_", 17) == 0) {
      CallExpr* pne  = toCallExpr(ne->parentExpr);
      CallExpr* ppne = (pne) ? toCallExpr(pne->parentExpr) : NULL;

      if (!ppne || !ppne->isPrimitive(PRIM_NEW)) {
        USR_FATAL(ne,
                  "alias-named-argument passing can only be used "
                  "in constructor calls");
      }

      aliasFieldSet.set_add(astr(&ne->name[17]));
    }
  }

  //
  // add implicit fields for implementing alias-named-argument passing
  //
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    for_fields(field, ct) {
      if (strcmp(field->name, "outer") == 0) {
        USR_FATAL_CONT(field,
                       "Cannot have a field named 'outer'. "
                       "'outer' is used to refer to an outer class "
                       "from within a nested class.");
      }

      if (aliasFieldSet.set_in(field->name)) {
        SET_LINENO(field);

        const char* aliasName  = astr("chpl__aliasField_", field->name);
        Symbol*     aliasField = new VarSymbol(aliasName);
        DefExpr*    def        = new DefExpr(aliasField);

        aliasField->addFlag(FLAG_CONST);
        aliasField->addFlag(FLAG_IMPLICIT_ALIAS_FIELD);

        def->init     = new UnresolvedSymExpr("false");
        def->exprType = new UnresolvedSymExpr("bool");

        ct->fields.insertAtTail(def);
      }
    }
  }

  addRecordDefaultConstruction();

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
        setCreationStyle(ts, fn);
      } else if (SymExpr* sym = toSymExpr(toArgSymbol(fn->_this)->typeExpr->body.only())) {
        fn->_this->type = sym->symbol()->type;
        fn->_this->type->methods.add(fn);
        setCreationStyle(sym->symbol()->type->symbol, fn);
      }
    } else if (fn->_this) {
      setCreationStyle(fn->_this->type->symbol, fn);
    }
  }

  //
  // build constructors (type and value versions)
  //
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    build_constructors(ct);
  }

  resolveGotoLabels();

  resolveUnresolvedSymExprs();

  resolveEnumeratedTypes();

  destroyTable();

  destroyModuleUsesCaches();

  warnedForDotInsideWith.clear();

  renameDefaultTypesToReflectWidths();

  cleanupExternC();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

void add_root_type(AggregateType* ct) {
  // make root records inherit from value
  // make root classes inherit from object
  if (ct->inherits.length == 0 && !ct->symbol->hasFlag(FLAG_NO_OBJECT)) {
    SET_LINENO(ct);

    if (isRecord(ct)) {
      ct->dispatchParents.add(dtValue);

      // Assume that this addition is unique; report if not.
      if (dtValue->dispatchChildren.add_exclusive(ct) == false) {
        INT_ASSERT(false);
      }

    } else if (isClass(ct)) {
      ct->dispatchParents.add(dtObject);

      // Assume that this addition is unique; report if not.
      if (dtObject->dispatchChildren.add_exclusive(ct) == false) {
        INT_ASSERT(false);
      }

      VarSymbol* super = new VarSymbol("super", dtObject);

      super->addFlag(FLAG_SUPER_CLASS);

      ct->fields.insertAtHead(new DefExpr(super));
    }
  }
}

/************************************* | **************************************
*                                                                             *
* addToSymbolTable adds the asts in a vector to the global symbolTable such   *
* that symbol definitions are added to entries in the table and new           *
* enclosing asts become entries                                               *
*                                                                             *
************************************** | *************************************/

static void addOneToSymbolTable(DefExpr* def);

static void addToSymbolTable(std::vector<DefExpr*>& defs) {
  for_vector(DefExpr, def, defs) {
    addOneToSymbolTable(def);
  }
}

static void addToSymbolTable(Vec<DefExpr*>& defs) {
  forv_Vec(DefExpr, def, defs) {
    addOneToSymbolTable(def);
  }
}

static void addOneToSymbolTable(DefExpr* def) {
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

      if ((!oldFn && (newFn && !newFn->_this)) ||
          (!newFn && (oldFn && !oldFn->_this))) {
        // A function definition is conflicting with another named symbol
        // that isn't a function (could be a variable, a module name, etc.)
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


/************************************* | **************************************
*                                                                             *
* Transform module uses into calls to initialize functions; store the         *
* relevant scoping information in BlockStmt::modUses                          *
*                                                                             *
************************************** | *************************************/

static Symbol* getUsedSymbol(UseStmt* use);
static Symbol* getUsedSymbol(Expr* expr, UseStmt* useCall);

static void processImportExprs() {
  // handle "use mod;" where mod is a module
  forv_Vec(UseStmt, use, gUseStmts) {
    SET_LINENO(use);

    Symbol* sym = getUsedSymbol(use);

    if (!sym)
      USR_FATAL(use, "Cannot find module or enum");

    // We don't need to perform any resolution on the default use of the
    // rootModule, so don't add it to the current module's use list or that
    // of its block (it should already be in the latter).
    if (sym == rootModule)
      continue;

    if (!use->parentExpr) {
      // This happens for the uses I create when traversing use chains which
      // contain excepts, so that the excepted keywords are skipped down all
      // paths from that chain head (see buildBreadthFirstModuleList).  In
      // that case, we don't need to do any further work here beyond make
      // sure the sym is a SymExpr
      use->src = new SymExpr(sym);
      continue;
    }

    use->src->replace(new SymExpr(sym));
    // Need to update the use now that we've found what it refers to

    ModuleSymbol* enclosingModule = use->getModule();

    if (ModuleSymbol* mod = toModuleSymbol(sym)) {
      // A ModuleSymbol's modUseList is intended to be for initializing modules
      // in an appropriate order.  If we're using an enum symbol, we don't
      // need to worry about this.
      enclosingModule->moduleUseAdd(mod);
    }

    BlockStmt* useParent = getVisibilityBlock(use);
    use->getStmtExpr()->remove();

    useParent->moduleUseAdd(use);

    use->validateList();
  }
}

//
// Return the module imported by a use call.  The module returned could be
// nested: e.g. "use outermost.middle.innermost;"
//
static Symbol* getUsedSymbol(UseStmt* use) {
  return getUsedSymbol(use->src, use);
}


//
// Helper routine to factor some 'use' error messages into a single place
//
static void printUseError(UseStmt* useExpr,
                          Symbol* sym = NULL) {
  if (sym && !sym->isImmediate()) {
    if (sym->name) {
      USR_FATAL(useExpr, "'use' of non-module/enum symbol %s", sym->name);
    } else {
      USR_FATAL(useExpr, "'use' of non-module/enum symbol");
    }
  } else {
    USR_FATAL(useExpr, "'use' statements must refer to module or enum symbols "
              "(e.g., 'use <module>[.<submodule>]*;')");
  }
  return;
}

static bool isValidUsedSymbol(UseStmt* useExpr, Symbol* symbol) {
  if (isModuleSymbol(symbol)) {
    return true;
  } else if (TypeSymbol* type = toTypeSymbol(symbol)) {
    if (isEnumType(type->type))
      return true;
  }
  printUseError(useExpr, symbol);
  return false;
}

//
// Return the module or enum imported by a use call.  The module returned
// could be nested: e.g. "use outermost.middle.innermost;"
//
static Symbol* getUsedSymbol(Expr* expr, UseStmt* useCall) {
  //
  // This handles the simple case of 'use <symbol>' (as well as error
  // cases that try to use non-module or non-enum symbols)
  //
  if (SymExpr* sym = toSymExpr(expr)) {
    if (Symbol* symbol = sym->symbol()) {
      if (isValidUsedSymbol(useCall, symbol)) {
        return symbol;
      }
    } else {
      printUseError(useCall);
    }
    return NULL;
  } else if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(expr)) {
    //
    // This case handles the (common) case that we're 'use'ing a
    // symbol that we have not yet resolved.
    //
    if (Symbol* symbol = lookup(useCall, sym->unresolved)) {
      if (isValidUsedSymbol(useCall, symbol)) {
        return symbol;
      }
    } else {
      USR_FATAL(useCall, "Cannot find module or enum '%s'", sym->unresolved);
    }
    return NULL;
  } else if (CallExpr* call = toCallExpr(expr)) {
    //
    // This handles the case of 'use <symbol>.<symbol>' (as well as
    // error cases in which other expressions than '.' are used)
    //

    if (!call->isNamed("."))
      printUseError(useCall);

    ModuleSymbol* lhs = toModuleSymbol(getUsedSymbol(call->get(1), useCall));

    if (!lhs)
      USR_FATAL(useCall, "Cannot find module");

    SymExpr*    rhs     = toSymExpr(call->get(2));
    const char* rhsName = 0;

    if (!rhs)
      INT_FATAL(useCall, "Bad use statement in getUsedSymbol");

    if (!get_string(rhs, &rhsName))
      INT_FATAL(useCall, "Bad use statement in getUsedSymbol");

    if (Symbol* symbol = lookup(lhs->block, rhsName)) {
      if (isValidUsedSymbol(useCall, symbol)) {
        return symbol;
      }
    } else {
      USR_FATAL(useCall, "Cannot find module '%s'", rhsName);
    }
    return NULL;
  } else {
    //
    // This is a general fall-through case that I suspect we may never
    // reach, but better safe than sorry...
    //
    printUseError(useCall);
    return NULL;
  }
}

// Verifies that all the symbols in the include and exclude lists of use
// statements refer to symbols that are visible from that module.
void UseStmt::validateList() {
  if (isPlainUse()) {
    // Trivially, if we don't have a list (are a plain use), then it must be
    // valid!
    return;
  }
  noRepeats();

  BaseAST* scopeToUse = getSearchScope();

  const char* listName = except ? "except" : "only";
  for_vector(const char, name, named) {
    if (name[0] != '\0') {
      Symbol* sym = lookup(scopeToUse, name);

      if (!sym) {
        USR_FATAL_CONT(this, "Bad identifier in '%s' clause, no known '%s'", listName, name);
      } else if (!sym->isVisible(this)) {
        USR_FATAL_CONT(this, "Bad identifier in '%s' clause, '%s' is private", listName, name);
      }

      createRelatedNames(sym);
    }
  }

  for (std::map<const char*, const char*>::iterator it = renamed.begin();
       it != renamed.end(); ++it) {
    Symbol* sym = lookup(scopeToUse, it->second);

    if (!sym) {
      SymExpr* se = toSymExpr(src);
      INT_ASSERT(se);
      USR_FATAL_CONT(this, "Bad identifier in rename, no known '%s' in '%s'", it->second, se->symbol()->name);
    } else if (!sym->isVisible(this)) {
      USR_FATAL_CONT(this, "Bad identifier in rename, '%s' is private", it->second);
    }

    createRelatedNames(sym);
  }
}

void UseStmt::noRepeats() {
  for (std::vector<const char*>::iterator it = named.begin();
       it != named.end(); ++it) {
    std::vector<const char*>::iterator next = it;
    for (++next; next != named.end(); ++next) {
      // Check rest of named for the same name
      if (!strcmp(*it, *next)) {
        USR_WARN(this, "identifier '%s' is repeated", *it);
      }
    }
    for (std::map<const char*, const char*>::iterator renamedIt = renamed.begin();
         renamedIt != renamed.end(); ++renamedIt) {
      if (!strcmp(*it, renamedIt->second)) {
        // This identifier is also used as the old name for a renaming.
        // Probably a mistake on the user's part, but not a catastrophic one
        USR_WARN(this, "identifier '%s' is repeated", *it);
      }
      if (!strcmp(*it, renamedIt->first)) {
        // The user attempted to rename a symbol to a name that was already
        // in the 'only' list.  This causes a naming conflict.
        USR_FATAL_CONT(this, "symbol '%s' multiply defined", *it);
      }
    }
  }
  for (std::map<const char*, const char*>::iterator it = renamed.begin();
       it != renamed.end(); ++it) {
    std::map<const char*, const char*>::iterator next = it;
    for (++next; next != renamed.end(); ++next) {
      if (!strcmp(it->second, next->second)) {
        // Renamed this variable twice.  Probably a mistake on the user's part,
        // but not a catastrophic one
        USR_WARN(this, "identifier '%s' is repeated", it->second);
      }
      if (!strcmp(it->second, next->first)) {
        // This name is the old_name in one rename and the new_name in another
        // Did the user actually want to cut out the middle man?
        USR_WARN(this, "identifier '%s' is repeated", it->second);
        USR_PRINT("Did you mean to rename '%s' to '%s'?", next->second, it->first);
      }
      if (!strcmp(it->first, next->second)) {
        // This name is the old_name in one rename and the new_name in another
        // Did the user actually want to cut out the middle man?
        USR_WARN(this, "identifier '%s' is repeated", it->first);
        USR_PRINT("Did you mean to rename '%s' to '%s'?", it->second, next->first);
      }
      // Two symbols can't be renamed to the same name because the map can only
      // store one entry with a given key.  We catch this case in build.cpp
      // when creating the UseStmt.  No need to check it->first matching
      // next->first.
    }
  }
}

void UseStmt::createRelatedNames(Symbol* maybeType) {
  if (TypeSymbol* ts = toTypeSymbol(maybeType)) {
    Type* type = ts->type;
    forv_Vec(FnSymbol, method, type->methods) {
      relatedNames.push_back(method->name);
    }
    if (AggregateType* at = toAggregateType(type)) {
      for_fields(sym, at) {
        relatedNames.push_back(sym->name);
      }
    }

    unsigned int constructorLen = strlen(ts->name) + strlen("_construct_") + 1;
    char * constructorName = (char *) malloc(constructorLen);
    strcpy(constructorName, "_construct_");
    strcat(constructorName, ts->name);
    relatedNames.push_back(constructorName);

    unsigned int typeConstLen = constructorLen + strlen("_type");
    char * typeConstructorName = (char *) malloc(typeConstLen);
    strcpy(typeConstructorName, "_type_construct_");
    strcat(typeConstructorName, ts->name);
    relatedNames.push_back(typeConstructorName);
  }
}

/************************************* | **************************************
*                                                                             *
* Compute dispatchParents and dispatchChildren vectors; add base class fields *
* to subclasses; identify cyclic or illegal class or record hierarchies       *
*                                                                             *
************************************** | *************************************/

static void           addClassToHierarchy(AggregateType*            ct,
                                          std::set<AggregateType*>& seen);

static AggregateType* discoverParentAndCheck(Expr*          storesName,
                                             AggregateType* child);

static void addClassToHierarchy(AggregateType* ct) {
  std::set<AggregateType*> localSeen; // classes in potential cycle

  addClassToHierarchy(ct, localSeen);
}

static void addClassToHierarchy(AggregateType*            ct,
                                std::set<AggregateType*>& localSeen) {
  // classes already in hierarchy
  static std::set<AggregateType*> globalSeen;

  if (localSeen.find(ct)  != localSeen.end())  {
    USR_FATAL(ct, "Class hierarchy is cyclic");
  }

  if (globalSeen.find(ct) != globalSeen.end()) {
    return;
  }

  globalSeen.insert(ct);

  add_root_type(ct);

  // Walk the base class list, and add parents into the class hierarchy.
  for_alist(expr, ct->inherits) {
    AggregateType* pt = discoverParentAndCheck(expr, ct);

    localSeen.insert(ct);

    addClassToHierarchy(pt, localSeen);

    ct->dispatchParents.add(pt);

    if (ct->isGeneric() == false && pt->isGeneric() == true) {
      ct->markAsGeneric();
    }

    if (pt->dispatchChildren.add_exclusive(ct) == false) {
      INT_ASSERT(false);
    }

    expr->remove();

    if (isClass(ct) == true) {
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
            if (strcmp(myfield->name, field->name) == 0) {
              alreadyContainsField = true;
              break;
            }
          }

          if (!alreadyContainsField) {
            DefExpr* def = field->defPoint->copy();

            ct->fields.insertAtHead(def);

            def->sym->addFlag(FLAG_PARENT_FIELD);
          }
        }
      }
    }
  }
}

static AggregateType* discoverParentAndCheck(Expr*          storesName,
                                             AggregateType* child) {
  UnresolvedSymExpr* se  = toUnresolvedSymExpr(storesName);
  Symbol*            sym = lookup(storesName, se->unresolved);
  TypeSymbol*        ts  = toTypeSymbol(sym);

  //    printf("looking up %s\n", se->unresolved);
  if (!ts) {
    USR_FATAL(storesName, "Illegal super class");
  }

  //    printf("found it in %s\n", sym->getModule()->name);
  AggregateType* pt = toAggregateType(ts->type);

  if (!pt) {
    USR_FATAL(storesName, "Illegal super class %s", ts->name);
  }

  if (isUnion(child) && isUnion(pt)) {
    USR_FATAL(storesName, "Illegal inheritance involving union type");
  }

  if (isRecord(child) && isClass(pt)) {
    USR_FATAL(storesName,
              "Record %s inherits from class %s",
              child->symbol->name,
              pt->symbol->name);
  }

  if (isClass(child) && isRecord(pt)) {
    // <hilde> Possible language change: Allow classes to inherit
    // fields and methods from records.
    USR_FATAL(storesName,
              "Class %s inherits from record %s",
              child->symbol->name,
              pt->symbol->name);
  }

  return pt;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void addRecordDefaultConstruction() {
  forv_Vec(DefExpr, def, gDefExprs) {
    // We're only interested in declarations that do not have initializers.
    if (def->init != NULL) {

    } else if (VarSymbol* var = toVarSymbol(def->sym)) {
      if (AggregateType* at = toAggregateType(var->type)) {
        if (at->isRecord() == false) {

        // No initializer for extern records.
        } else if (at->symbol->hasFlag(FLAG_EXTERN) == true) {

        } else {
          SET_LINENO(def);

          CallExpr* ctor_call = new CallExpr(new SymExpr(at->symbol));

          def->init = new CallExpr(PRIM_NEW, ctor_call);

          insert_help(def->init, def, def->parentSymbol);
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void setCreationStyle(TypeSymbol* t, FnSymbol* fn) {
  bool isCtor = (0 == strcmp(t->name, fn->name));
  bool isInit = (0 == strcmp(fn->name, "init"));

  if (!isCtor && !isInit)
    return;

  AggregateType* ct = toAggregateType(t->type);
  if (!ct)
    INT_FATAL(fn, "initializer on non-class type");

  if (fn->hasFlag(FLAG_NO_PARENS)) {
    USR_FATAL(fn, "a%s cannot be declared without parentheses", isCtor ? " constructor" : "n initializer");
  }

  if (ct->initializerStyle == DEFINES_NONE_USE_DEFAULT) {
    // We hadn't previously seen a constructor or initializer definition.
    // Update the field on the type appropriately.
    if (isInit) {
      ct->initializerStyle = DEFINES_INITIALIZER;
    } else if (isCtor) {
      ct->initializerStyle = DEFINES_CONSTRUCTOR;
    } else {
      // Should never reach here, but just in case...
      INT_FATAL(fn, "Function was neither a constructor nor an initializer");
    }
  } else if ((ct->initializerStyle == DEFINES_CONSTRUCTOR && !isCtor) ||
             (ct->initializerStyle == DEFINES_INITIALIZER && !isInit)) {
    // We've previously seen a constructor but this new method is an initializer
    // or we've previously seen an initializer but this new method is a
    // constructor.  We don't allow both to be defined on a type.
    USR_FATAL_CONT(fn, "Definition of both constructor '%s' and initializer 'init'.  Please choose one.", ct->symbol->name);
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

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
  // Do nothing if it is already built
  if (ct->defaultTypeConstructor)
    return;

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
    gGenericTupleTypeCtor = fn;
  }

  // and insert it into the class type.
  ct->defaultTypeConstructor = fn;

  // Create "this".
  fn->_this = new VarSymbol("this", ct);
  fn->_this->addFlag(FLAG_ARG_THIS);

  fn->insertAtTail(new DefExpr(fn->_this));

  Vec<const char*> fieldNamesSet;

  CallExpr* superCall = NULL;

  // Copy arguments from superclass type constructor
  // (supporting inheritance from generic classes)
  if (isClass(ct) && ct->dispatchParents.n > 0) {

    if(AggregateType *parentTy = toAggregateType(ct->dispatchParents.v[0])){

      // This class/record has a parent class/record
      if (!parentTy->defaultTypeConstructor) {
        // If it doesn't yet have an type constructor, make one
        build_type_constructor(parentTy);
      }
      FnSymbol* superTypeCtor = parentTy->defaultTypeConstructor;

      if (superTypeCtor->numFormals() > 0) {

        superCall = new CallExpr(parentTy->symbol->name);

        // Now walk through arguments in super class type constructor
        for_formals(formal, superTypeCtor) {

          DefExpr* superArg = formal->defPoint->copy();

          // Add a formal to the current class type constructor.

          ArgSymbol* arg = toArgSymbol(superArg->sym->copy());
          bool fieldInThisClass = false;
          for_fields(sym, ct) {
            if (0 == strcmp(sym->name, arg->name)) {
              fieldInThisClass = true;
            }
          }

          if (fieldInThisClass) {
            // If the field is also present in the child, adjust the field
            // name in the super. Otherwise it would not be possible to
            // type construct the super.

            // ?
            // Should we omit them?
            // Or pass the child field to the super constructor?
            continue;
          }
          arg->addFlag(FLAG_PARENT_FIELD);
          fn->insertFormalAtTail(arg);
          superCall->insertAtTail(new SymExpr(arg));
        }
      }
    }
  }

  for_fields(tmp, ct) {
    SET_LINENO(tmp);

    if (VarSymbol* field = toVarSymbol(tmp)) {
      if (field->hasFlag(FLAG_SUPER_CLASS)) {
        // supporting inheritance from generic classes
        if (superCall) {
          CallExpr* newInit = new CallExpr(PRIM_TYPE_INIT, superCall);
          CallExpr* newSet  = new CallExpr(PRIM_SET_MEMBER,
                                           fn->_this,
                                           new_CStringSymbol(field->name),
                                           newInit);
          fn->insertAtTail(newSet);
        }
        continue;
      }


      Expr* exprType = field->defPoint->exprType;
      Expr* init = field->defPoint->init;

      if (!strcmp(field->name, "_promotionType")) {

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
            (!exprType && !init)) {

          ArgSymbol* arg = create_generic_arg(field);

          // Indicate which type constructor args are also for super class
          // This helps us to call the superclass type constructor in resolution
          if (field->hasFlag(FLAG_PARENT_FIELD)) {
            arg->addFlag(FLAG_PARENT_FIELD);
          }

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
                                          new CallExpr(PRIM_TYPE_INIT, arg)));
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
  if (ct->initializerStyle == DEFINES_INITIALIZER) {
    // Don't want to create the default constructor if we have seen initializers
    // defined.  The work is completely unnecessary, since we won't call the
    // default constructor, and it mutates information about the fields that
    // we would rather stayed unmutated.
    return;
  }

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
    gGenericTupleInit = fn;
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

  if (ct->symbol->hasFlag(FLAG_REF)) {
    // For ref, sync and single classes, just allocate space.
    allocCall = callChplHereAlloc(fn->_this->type);

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
          // reverse-lexical order results in all of the arguments appearing
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

        tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
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
      if (hadType) {
        init = new CallExpr("_createFieldDefault", exprType->copy(), init);
      } else if (init)
        init = new CallExpr("chpl__initCopy", init);
    }

    if (exprType) {
      if (!isBlockStmt(exprType))
        arg->typeExpr = new BlockStmt(exprType, BLOCK_TYPE);
      else
        arg->typeExpr = toBlockStmt(exprType);
    }

    if (init) {
      if (hadInit)
        arg->defaultExpr = new BlockStmt(init, BLOCK_SCOPELESS);
      else {
        Expr* initVal = new SymExpr(gTypeDefaultToken);
        arg->defaultExpr = new BlockStmt(initVal);
      }
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
        // If a record type has an initialize method, it's not Plain Old Data.
        if (!isClass(ct)) {
          ct->symbol->addFlag(FLAG_NOT_POD);
        }
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


/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveGotoLabels() {
  forv_Vec(GotoStmt, gs, gGotoStmts) {
    SET_LINENO(gs);

    if (SymExpr* label = toSymExpr(gs->label)) {
      if (label->symbol() == gNil) {
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

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void resolveUnresolvedSymExpr(UnresolvedSymExpr*            usymExpr,
                                     std::set<UnresolvedSymExpr*>& skipSet);

static void updateMethod(UnresolvedSymExpr*            usymExpr,
                         std::set<UnresolvedSymExpr*>& skipSet);

static void updateMethod(UnresolvedSymExpr*            usymExpr,
                         std::set<UnresolvedSymExpr*>& skipSet,
                         Symbol*                       sym);

static void updateMethod(UnresolvedSymExpr*            usymExpr,
                         std::set<UnresolvedSymExpr*>& skipSet,
                         Symbol*                       sym,
                         SymExpr*                      symExpr);

static void insertFieldAccess(FnSymbol*          method,
                              UnresolvedSymExpr* usymExpr,
                              Symbol*            sym,
                              Expr*              expr);

static int  computeNestedDepth(const char* name,
                               Type*       type);

static void resolveModuleCall(CallExpr*                     call,
                              std::set<UnresolvedSymExpr*>& skipSet);

static bool isMethodName(const char* name, Type* type);
static bool isMethodNameLocal(const char* name, Type* type);

static void checkIdInsideWithClause(Expr*              exprInAst,
                                    UnresolvedSymExpr* origUSE);

#ifdef HAVE_LLVM
static bool tryCResolve(ModuleSymbol* module, const char* name);
static bool tryCResolve_set(ModuleSymbol*                     module,
                            const char*                       name,
                            llvm::SmallSet<ModuleSymbol*, 24> already_checked);
#endif

static void resolveUnresolvedSymExprs() {
  std::set<UnresolvedSymExpr*> skipSet;

  //
  // Translate M.x where M is a ModuleSymbol into just x where x is
  // the symbol in module M; for functions, insert a "module=" token
  // that is used to determine visible functions.
  //

  int maxResolved = 0;
  int i           = 0;

  forv_Vec(UnresolvedSymExpr, unresolvedSymExpr, gUnresolvedSymExprs) {
    resolveUnresolvedSymExpr(unresolvedSymExpr, skipSet);

    maxResolved++;
  }

  forv_Vec(CallExpr, call, gCallExprs) {
    resolveModuleCall(call, skipSet);
  }

  // Note that the extern C resolution might add new UnresolvedSymExprs, and it
  // might do that within resolveModuleCall, so we try resolving unresolved
  // symbols a second time as the extern C block support might have added some.
  // Alternatives include:
  //  - have the extern C wrapper-builder directly call resolveUnresolved
  //      (but that complicates the way scopeResolve works now)
  //  - import all C symbols at an earlier point
  //      (but that might add lots of unused garbage to the Chapel AST
  //       for e.g. #include <stdio.h>; and it might cause the C-to-Chapel
  //       translator to need to handle more platform/compiler-specific stuff,
  //       and it might lead to extra naming conflicts).
  forv_Vec(UnresolvedSymExpr, unresolvedSymExpr, gUnresolvedSymExprs) {
    // Only try resolving symbols that are new after last attempt.
    if (i >= maxResolved) {
      resolveUnresolvedSymExpr(unresolvedSymExpr, skipSet);
    }

    i++;
  }
}

static void resolveUnresolvedSymExpr(UnresolvedSymExpr*            usymExpr,
                                     std::set<UnresolvedSymExpr*>& skipSet) {
  SET_LINENO(usymExpr);

  const char* name = usymExpr->unresolved;

  if (skipSet.find(usymExpr) != skipSet.end() ||
      strcmp(name, ".")      == 0             ||
      usymExpr->parentSymbol == NULL) {

  } else if (Symbol* sym = lookup(usymExpr, name)) {
    FnSymbol* fn = toFnSymbol(sym);

    if (fn == NULL) {
      SymExpr* symExpr = new SymExpr(sym);

      usymExpr->replace(symExpr);

      updateMethod(usymExpr, skipSet, sym, symExpr);

    // sjd: stopgap to avoid shadowing variables or functions by methods
    } else if (fn->hasFlag(FLAG_METHOD) == true) {
      updateMethod(usymExpr, skipSet);

    // handle function call without parentheses
    } else if (fn->hasFlag(FLAG_NO_PARENS) == true) {
      checkIdInsideWithClause(usymExpr, usymExpr);
      usymExpr->replace(new CallExpr(fn));

    } else if (Expr* parent = usymExpr->parentExpr) {
      CallExpr* call = toCallExpr(parent);

      if (call == NULL || call->baseExpr != usymExpr) {
        CallExpr* primFn = NULL;

        // Wrap the FN in the appropriate way
        if (call != NULL && call->isNamed("c_ptrTo") == true) {
          primFn = new CallExpr(PRIM_CAPTURE_FN_FOR_C);
        } else {
          primFn = new CallExpr(PRIM_CAPTURE_FN_FOR_CHPL);
        }

        usymExpr->replace(primFn);

        primFn->insertAtTail(usymExpr);

        skipSet.insert(usymExpr);

      } else {
        updateMethod(usymExpr, skipSet, sym);
      }

    } else {
      updateMethod(usymExpr, skipSet, sym);
    }

  } else {
    updateMethod(usymExpr, skipSet);

#ifdef HAVE_LLVM
    if (externC == true && tryCResolve(usymExpr->getModule(), name) == true) {
      // Try resolution again since the symbol should exist now
      resolveUnresolvedSymExpr(usymExpr, skipSet);
    }
#endif
  }
}

// Apply 'this' and 'outer' in methods where necessary
static void updateMethod(UnresolvedSymExpr*            usymExpr,
                         std::set<UnresolvedSymExpr*>& skipSet) {
  updateMethod(usymExpr, skipSet, NULL);
}

static void updateMethod(UnresolvedSymExpr*            usymExpr,
                         std::set<UnresolvedSymExpr*>& skipSet,
                         Symbol*                       sym) {
  updateMethod(usymExpr, skipSet, sym, NULL);
}

static void updateMethod(UnresolvedSymExpr*            usymExpr,
                         std::set<UnresolvedSymExpr*>& skipSet,
                         Symbol*                       sym,
                         SymExpr*                      symExpr) {
  const char* name   = usymExpr->unresolved;
  Expr*       expr   = (symExpr != NULL) ? (Expr*) symExpr : (Expr*) usymExpr;
  Symbol*     parent = expr->parentSymbol;
  bool        isAggr = false;

  if (sym != NULL) {
    if (TypeSymbol* cts = toTypeSymbol(sym->defPoint->parentSymbol)) {
      isAggr = isAggregateType(cts->type);
    }
  }

  while (isModuleSymbol(parent) == false) {
    if (FnSymbol* method = toFnSymbol(parent)) {
      // stopgap bug fix: do not let methods shadow symbols
      // that are more specific than methods
      if (sym != NULL && sym->defPoint->getFunction() == method) {
        break;

      } else if (method->_this != NULL) {
        if (symExpr == NULL || symExpr->symbol() != method->_this) {
          Type* type = method->_this->type;

          if (isAggr == true || isMethodName(name, type) == true) {
            CallExpr* call = toCallExpr(expr->parentExpr);

            if (call                              != NULL &&
                call->baseExpr                    == expr &&
                call->numActuals()                >= 2    &&
                isSymExpr(call->get(1))           == true &&
                toSymExpr(call->get(1))->symbol() == gMethodToken) {
              UnresolvedSymExpr* use = new UnresolvedSymExpr(name);

              expr->replace(use);

              skipSet.insert(use);

            } else {
              insertFieldAccess(method, usymExpr, sym, expr);
            }
          }

          break;
        }
      }
    }

    parent = parent->defPoint->parentSymbol;
  }
}

// Apply implicit this pointers and outer this pointers
static void insertFieldAccess(FnSymbol*          method,
                              UnresolvedSymExpr* usymExpr,
                              Symbol*            sym,
                              Expr*              expr) {
  const char* name      = usymExpr->unresolved;
  int         nestDepth = computeNestedDepth(name, method->_this->type);
  Expr*       dot       = new SymExpr(method->_this);

  checkIdInsideWithClause(expr, usymExpr);

  if (nestDepth > 0) {
    for (int i = 0; i < nestDepth; i++) {
      dot = new CallExpr(".", dot, new_CStringSymbol("outer"));
    }
  }

  if (isTypeSymbol(sym) == true)
    dot = new CallExpr(".", dot, sym);
  else
    dot = new CallExpr(".", dot, new_CStringSymbol(name));

  expr->replace(dot);
}

static int computeNestedDepth(const char* name, Type* type) {
  int retval = 0;

  if (isMethodName(name, type) == true) {
    AggregateType* ct = toAggregateType(type);

    // count how many classes out from current depth that
    // this method is first defined in
    while (ct != NULL && isMethodNameLocal(name, ct) == false) {
      retval = retval + 1;
      ct     = toAggregateType(ct->symbol->defPoint->parentSymbol->type);
    }

  } else {
    // count how many classes out from current depth that
    // this symbol is first defined in
    AggregateType* ct = toAggregateType(type);

    while (ct != NULL && ct->getField(name, false) == NULL) {
      retval = retval + 1;
      ct     = toAggregateType(ct->symbol->defPoint->parentSymbol->type);
    }
  }

  return retval;
}

//
// isMethodName returns true iff 'name' names a method of 'type'
//
static bool isMethodName(const char* name, Type* type) {
  if (strcmp(name, type->symbol->name) == 0) {
    return false;
  }

  forv_Vec(Symbol, method, type->methods) {
    if (method != NULL && strcmp(name, method->name) == 0) {
      return true;
    }
  }

  forv_Vec(Type, pt, type->dispatchParents) {
    if (isMethodName(name, pt) == true) {
      return true;
    }
  }

  if (AggregateType* ct = toAggregateType(type)) {
    Type* outerType = ct->symbol->defPoint->parentSymbol->type;

    if (AggregateType* outer = toAggregateType(outerType)) {
      if (isMethodName(name, outer) == true) {
        return true;
      }
    }
  }

  return false;
}


//
// isMethodNameLocal returns true iff 'name' names a method of 'type'
// excluding methods of an outer type
//
static bool isMethodNameLocal(const char* name, Type* type) {
  if (strcmp(name, type->symbol->name) == 0) {
    return false;
  }

  forv_Vec(Symbol, method, type->methods) {
    if (method != NULL && strcmp(name, method->name) == 0) {
      return true;
    }
  }

  forv_Vec(Type, pt, type->dispatchParents) {
    if (isMethodName(name, pt) == true) {
      return true;
    }
  }

  return false;
}


static void errorDotInsideWithClause(UnresolvedSymExpr* origUSE,
                                     const char*        construct) {
  // As of this writing, a with-clause can be duplicated in the AST.
  // This code avoids multiple error messages for the same symbol.

  std::pair<const char*, int> markLoc(origUSE->astloc.filename,
                                      origUSE->astloc.lineno);

  WFDIWmark                   mark(markLoc, origUSE->unresolved);

  if (warnedForDotInsideWith.count(mark) == 0) {
    USR_FATAL_CONT(origUSE,
                   "cannot reference a field or function '%s' "
                   "in a with-clause of this %s",
                   origUSE->unresolved,
                   construct);

    warnedForDotInsideWith.insert(mark);
  }
}

//
// 'expr' ended up being a field reference (or perhaps a method call).
// If we are inside a 'with' clause, report an error.
//
static void checkIdInsideWithClause(Expr*              exprInAst,
                                    UnresolvedSymExpr* origUSE) {
  // A 'with' clause for a forall loop.
  if (BlockStmt* parent = toBlockStmt(exprInAst->parentExpr)) {
    if (ForallIntents* fi = parent->forallIntents) {
      for_vector(Expr, fiVar, fi->fiVars) {
        if (exprInAst == fiVar) {
          errorDotInsideWithClause(origUSE, "forall loop");
          return;
        }
      }
    }
  }

  // A 'with' clause for a task construct.
  if (Expr* parent1 = exprInAst->parentExpr) {
    if (BlockStmt* parent2 = toBlockStmt(parent1->parentExpr)) {
      if (parent1 == parent2->byrefVars) {
        CallExpr* blockInfo = parent2->blockInfoGet();

        // Ensure that an issue, indeed, occurred a task construct.
        INT_ASSERT(blockInfo->isPrimitive(PRIM_BLOCK_COBEGIN)  ||
                   blockInfo->isPrimitive(PRIM_BLOCK_COFORALL) ||
                   blockInfo->isPrimitive(PRIM_BLOCK_BEGIN));

        errorDotInsideWithClause(origUSE, blockInfo->primitive->name);
      }
    }
  }
}

static void resolveModuleCall(CallExpr*                     call,
                              std::set<UnresolvedSymExpr*>& skipSet) {
  if (call->isNamed(".") == true) {
    if (SymExpr* se = toSymExpr(call->get(1))) {
      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        SET_LINENO(call);

        ModuleSymbol* enclosingModule = call->getModule();
        Symbol*       sym             = NULL;
        const char*   mbrName         = get_string(call->get(2));

        enclosingModule->moduleUseAdd(mod);

        // Can the identifier be mapped to something at this scope?
        if (symbolTable.count(mod->block) != 0) {
          SymbolTableEntry* entry = symbolTable[mod->block];

          if (entry->count(mbrName) != 0) {
            sym = (*entry)[mbrName];
          }
        }

        if (sym != NULL) {
          if (sym->isVisible(call) == false) {
            // The symbol is not visible at this scope because it is
            // private to mod!  Error out
            USR_FATAL(call,
                      "Cannot access '%s', '%s' is private to '%s'",
                      mbrName,
                      mbrName,
                      mod->name);

          } else if (FnSymbol* fn = toFnSymbol(sym)) {
            if (fn->_this == NULL && fn->hasFlag(FLAG_NO_PARENS)) {
              call->replace(new CallExpr(fn));
            } else {
              UnresolvedSymExpr* se = new UnresolvedSymExpr(mbrName);

              skipSet.insert(se);

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
        } else if (externC                                 == true &&
                   tryCResolve(call->getModule(), mbrName) == true) {
          // Try to resolve again now that the symbol should
          // be in the table
          resolveModuleCall(call, skipSet);
#endif

        } else {
          USR_FATAL_CONT(call,
                         "Symbol '%s' undeclared in module '%s'",
                         mbrName,
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
        std::vector<DefExpr*> v;

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


/************************************* | **************************************
*                                                                             *
* resolves EnumTypeName.fieldName to the symbol named fieldName in the        *
* enumerated type named EnumTypeName                                          *
*                                                                             *
************************************** | *************************************/

static void resolveEnumeratedTypes() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isNamed(".")) {
      SET_LINENO(call);

      if (SymExpr* first = toSymExpr(call->get(1))) {
        if (EnumType* type = toEnumType(first->symbol()->type)) {
          if (SymExpr* second = toSymExpr(call->get(2))) {
            const char* name;

            INT_ASSERT(get_string(second, &name));

            for_enums(constant, type) {
              if (!strcmp(constant->sym->name, name)) {
                call->replace(new SymExpr(constant->sym));
              }
            }
            // Unresolved enum symbols are now either resolved or throw an error
            // during the function resolution pass. Permits paren-less methods.
          }
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* delete the symbol table and module uses cache                               *
*                                                                             *
************************************** | *************************************/

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
  std::map<BlockStmt*,Vec<UseStmt*>*>::iterator use;

  for (use = moduleUsesCache.begin(); use != moduleUsesCache.end(); use++) {
    delete use->second;
  }

  moduleUsesCache.clear();
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

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

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void lookup(BaseAST* scope, const char * name,
                   std::vector<Symbol* >& symbols,
                   Vec<BaseAST*>& alreadyVisited,
                   std::set<int> rejectedPrivateIds,
                   BaseAST* callingContext);



static void    buildBreadthFirstModuleList(Vec<UseStmt*>* modules);

static void    buildBreadthFirstModuleList(Vec<UseStmt*>* modules,
                                           Vec<UseStmt*>* current,
                                           std::map<Symbol*, std::vector<UseStmt*> >* alreadySeen);

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
        Vec<UseStmt*>* moduleUses = NULL;

        if (moduleUsesCache.count(block) == 0) {
          moduleUses = new Vec<UseStmt*>();

          for_actuals(expr, block->modUses) {
            UseStmt* use = toUseStmt(expr);
            INT_ASSERT(use);

            moduleUses->add(use);
          }

          INT_ASSERT(moduleUses->n);

          buildBreadthFirstModuleList(moduleUses);

          if (enableModuleUsesCache)
            moduleUsesCache[block] = moduleUses;
        } else {
          moduleUses = moduleUsesCache[block];
        }

        forv_Vec(UseStmt, use, *moduleUses) {
          if (use) {
            if (!use->skipSymbolSearch(name)) {
              const char* nameToUse = use->isARename(name) ? use->getRename(name) : name;
              BaseAST* scopeToUse = use->getSearchScope();

              if (Symbol* sym = inSymbolTable(scopeToUse, nameToUse)) {
                if (sym->hasFlag(FLAG_PRIVATE)) {
                  if (rejectedPrivateIds.find(sym->id) ==
                      rejectedPrivateIds.end()) {
                    // The symbol found was not one of the already rejected
                    // private symbols
                    if (!sym->isVisible(callingContext)) {
                      rejectedPrivateIds.insert(sym->id);
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

static bool skipUse(std::map<Symbol*, std::vector<UseStmt*> >* seen,
                    UseStmt* current);

static void buildBreadthFirstModuleList(Vec<UseStmt*>* modules) {
  std::map<Symbol*, std::vector<UseStmt* > > seen;

  return buildBreadthFirstModuleList(modules, modules, &seen);
}

// If the uses of a particular module are considered its level 1 uses, then
// this function will only add level 2 and lower uses to the modules vector
// argument.
static void buildBreadthFirstModuleList(Vec<UseStmt*>* modules,
                                        Vec<UseStmt*>* current,
                                        std::map<Symbol*, std::vector<UseStmt*> >* alreadySeen) {
  modules->add(NULL); // use NULL sentinel to identify modules of equal depth

  Vec<UseStmt*> next;

  forv_Vec(UseStmt, source, *current) {
    if (!source) {
      break;
    } else {
      SymExpr* se = toSymExpr(source->src);
      INT_ASSERT(se);
      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        if (mod->block->modUses) {
          for_actuals(expr, mod->block->modUses) {
            UseStmt* use = toUseStmt(expr);
            INT_ASSERT(use);

            SymExpr* useSE = toSymExpr(use->src);
            INT_ASSERT(useSE);

            UseStmt* useToAdd = NULL;
            if (!useSE->symbol()->hasFlag(FLAG_PRIVATE)) {
              // Uses of private modules are not transitive - the symbols in the
              // private modules are only visible to itself and its immediate
              // parent.  Therefore, if the symbol is private, we will not
              // traverse it further and will merely add it to the alreadySeen
              // map.
              useToAdd = use->applyOuterUse(source);
              if (useToAdd != NULL && !skipUse(alreadySeen, useToAdd)) {
                next.add(useToAdd);
                modules->add(useToAdd);
              }
              // if applyOuterUse returned NULL, the number of symbols that
              // could be provided from this use was 0, so it didn't need to be
              // added to the alreadySeen map.
              if (useToAdd != NULL) {
                (*alreadySeen)[useSE->symbol()].push_back(useToAdd);
              }

            } else {
              (*alreadySeen)[useSE->symbol()].push_back(use);
            }
          }
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

// If the outer use statement would limit this one, return a new use of our
// module with the additional symbols accounted for.  This new use will not be
// added to the AST, but will be reused in scopeResolution if the same use path
// is followed.
// If the outer use does not require us to alter ourself, return ourself.  If
// the combination of the two uses results in no new symbols being provided by
// this module, return NULL.
UseStmt* UseStmt::applyOuterUse(UseStmt* outer) {
  if (outer->isPlainUse()) {
    // The outer use would not modify us, return ourself.
    return this;
  }
  if (outer->except) {
    // The outer use specifies an 'except' list
    if (!except && !isPlainUse()) {
      // The most complicated case is if we specified an 'only' list.
      // If that happened, we want to check if any of the identifiers
      // in the 'except' list are specified by the 'only' list, and
      // not place them in the new 'only' list.
      std::vector<const char*> newOnlyList;
      for_vector(const char, includeMe, named) {
        if (std::find(outer->named.begin(), outer->named.end(), includeMe) == outer->named.end()) {
          // We didn't find this symbol in the list to exclude, so
          // add it.
          newOnlyList.push_back(includeMe);
        }
      }
      std::map<const char*, const char*> newRenamed;
      for (std::map<const char*, const char*>::iterator it = renamed.begin();
          it != renamed.end(); ++it) {
        if (std::find(outer->named.begin(), outer->named.end(), it->first) ==
            outer->named.end()) {
          // We didn't find the new name in the list to exclude, so the rename
          // is still interesting.  Add it.
          newRenamed[it->first] = it->second;
        }
      }

      if (newOnlyList.size() == named.size() &&
          newRenamed.size() == renamed.size()) {
        // The except list didn't cut down on our only list.
        // No need to create a new UseStmt, just return ourself.
        return this;
      } else if (newOnlyList.size() == 0 && newRenamed.size() == 0) {
        // All of the 'only' list was in the 'except' list, so we don't provide
        // new symbols.
        return NULL;
      } else {
        // The only list will be shorter, create a new UseStmt with it.
        SET_LINENO(this);
        return new UseStmt(src, &newOnlyList, false, &newRenamed);
        // Note: we don't populate the relatedNames vector for the new use,
        // since we don't have a way to connect the names in it back to the
        // types we did or didn't include in the shorter 'only' list.
      }
    } else {
      // Handles case where inner use has an 'except' list, or is
      // just a plain use.  The use returned will have a (longer) 'except'
      // list.
      SET_LINENO(this);
      UseStmt* newUse = copy();
      for_vector(const char, toExclude, outer->named) {
        newUse->named.push_back(toExclude);
      }
      newUse->except = true;
      return newUse;
    }
  } else {
    // The outer use has an 'only' list
    if (!isPlainUse()) {
      if (except) {
        // The more complicated case arises if we have an 'except' list
        // The inner use should turn into a use with an 'only' list if anything
        // remains.
        std::vector<const char*> newOnlyList;
        for_vector(const char, includeMe, outer->named) {
          if (std::find(named.begin(), named.end(), includeMe) == named.end()) {
            // We didn't find this symbol in our 'except' list, so
            // add it.
            newOnlyList.push_back(includeMe);
          }
        }
        std::map<const char*, const char*> newRenamed;
        for(std::map<const char*, const char*>::iterator it = outer->renamed.begin();
            it != outer->renamed.end(); ++it) {
          if (std::find(named.begin(), named.end(), it->second) == named.end()) {
            // We didn't find the old name of the renamed symbol in our
            // 'except' list, so add it.
            newRenamed[it->first] = it->second;
          }
        }
        if (newOnlyList.size() > 0 || newRenamed.size() > 0) {
          // At least some of the identifiers in the 'only' list
          // weren't in the inner 'except' list.  Modify the use to
          // 'only' include those from the original 'only' list which
          // weren't in the inner 'except' list (could be all of the
          // outer 'only' list)
          SET_LINENO(this);
          return new UseStmt(src, &newOnlyList, false, &newRenamed);
        } else {
          // all the 'only' identifiers were in the 'except'
          // list so this module use will give us nothing.
          return NULL;
        }
      } else {
        // We had an 'only' list, so we need to narrow that list down to just
        // the names that are in both lists.
        SET_LINENO(this);
        std::vector<const char*> newOnlyList;
        std::map<const char*, const char*> newRenamed;
        for_vector(const char, includeMe, outer->named) {
          if (std::find(named.begin(), named.end(), includeMe) != named.end()) {
            // We found this symbol in both 'only' lists, so add it
            // to the union of them.
            newOnlyList.push_back(includeMe);
          } else {
            std::map<const char*, const char*>::iterator it = renamed.find(includeMe);
            if (it != renamed.end()) {
              // We found this symbol in the renamed list and the outer 'only'
              // list so add it to the new renamed list.
              newRenamed[it->first] = it->second;
            }
          }
        }
        for (std::map<const char*, const char*>::iterator it = outer->renamed.begin();
             it != outer->renamed.end(); ++it) {
          if (std::find(named.begin(), named.end(), it->second) != named.end()) {
            // The old name was in our 'only' list.  We need to rename it.
            newRenamed[it->first] = it->second;
          } else {
            std::map<const char*, const char*>::iterator innerIt = renamed.find(it->second);
            if (innerIt != renamed.end()) {
              // We found this symbol in the renamed list and the outer
              // renamed list so add the outer use's new name as the key, and
              // our old name as the old name to use.
              newRenamed[it->first] = innerIt->second;
            }
          }
        }
        if (newOnlyList.size() > 0 || newRenamed.size() > 0) {
          // There were symbols that were in both 'only' lists, so
          // this module use is still interesting.
          SET_LINENO(this);
          return new UseStmt(src, &newOnlyList, false, &newRenamed);
        } else {
          // all of the 'only' identifiers in the outer use
          // were missing from the inner use's 'only' list, so this
          // module use will give us nothing.
          return NULL;
        }
      }
    } else {
      // The inner use did not specify an 'except' or 'only' list,
      // so propagate our 'only' list and/or renamed list to it.
      SET_LINENO(this);
      UseStmt* newUse = copy();
      for_vector(const char, toInclude, outer->named) {
        newUse->named.push_back(toInclude);
      }
      for (std::map<const char*, const char*>::iterator it = outer->renamed.begin();
          it != outer->renamed.end(); ++it) {
        newUse->renamed[it->first] = it->second;
      }
      newUse->except = false;
      return newUse;
    }
  }
}

// Returns true if the current use statement has the possibility of allowing
// symbols that weren't already covered by 'other'
//
// Assumes that other->mod == this->mod.  Will not verify that fact.
bool UseStmt::providesNewSymbols(UseStmt* other) {
  if (other->isPlainUse()) {
    // Other is a general use, without an 'only' or 'except' list.  It covers
    // everything we could possibly cover, so we don't provide new symbols.
    return false;
  }
  if (isPlainUse()) {
    // We're a general use.  We know the other one isn't, so we provide symbols
    // it doesn't.
    return true;
  }

  if (except) {
    // We have an 'except' list.  This may be more general than other, so
    // we might want to dive into it.
    if (other->except) {
      // Other also has an 'except' list.
      if (other->named.size() <= named.size()) {
        // We are excluding more symbols than other, or the same number
        unsigned int numSame = 0;
        for_vector(const char, exclude, other->named) {
          if (std::find(named.begin(), named.end(), exclude) != named.end())
            numSame++;
        }
        // If all of other's excludes are in our list, we provide no new
        // symbols. If we don't cover all of the other's 'except' list, then we
        // know we provide the missing symbols.
        return numSame != other->named.size();
      } else {
        // Our 'except' list is smaller, so by definition we must provide
        // symbols that 'other' does not.
        return true;
      }
    } else {
      // Other has an 'only' list.  'Only' lists are usually more
      // restrictive than 'except' lists, and determining whether a
      // long 'only' list is less restrictive than a long 'except' list
      // doesn't seem beneficial in the long run.  So err on the side of
      // assuming we provide something new
      return true;
    }
  } else {
    // We have an 'only' list.  This is likely more specific than other, but
    // we should still check.
    if (other->except) {
      // Other has an 'except' list
      if (renamed.size() > 0) {
        // If we renamed any symbols, then we provide something new
        return true;
      }

      // If there were no renamed symbols and there's overlap in the two
      // lists then we provide new symbols
      int numSame = 0;
      for_vector(const char, include, named) {
        if (std::find(other->named.begin(), other->named.end(), include) != other->named.end()) {
          numSame++;
        }
      }
      // If numSame > 0, some of the names in our 'only' list were present in
      // other's 'except' list, which means we definitely provide new symbols
      return numSame > 0;
    } else if (other->named.size() + other->renamed.size() < named.size() +
               renamed.size()) {
      // Other has a smaller 'only' list.  By definition, this means we are
      // providing symbols not available in other.
      return true;
    } else {
      unsigned int numSame = 0;
      for_vector(const char, include, named) {
        if (std::find(other->named.begin(), other->named.end(), include) != other->named.end()) {
          numSame++;
        }
        // Don't check against other's renamed list, since a renamed version
        // of something in our only list is a new symbol
      }
      for(std::map<const char*, const char*>::iterator it = renamed.begin();
          it != renamed.end(); ++it) {
        // Don't check against other's only list.  A renamed version of
        // something in their only list is a new symbol
        // Do check against other's renamed list.  If both uses cause the exact
        // same rename to occur, we should count it.
        for (std::map<const char*, const char*>::iterator otherIt =
               other->renamed.begin();
             otherIt != other->renamed.end(); ++otherIt) {
          if (!strcmp(it->first, otherIt->first) &&
              !strcmp(it->second, otherIt->second)) {
            numSame++;
          }
        }
      }

      // If all of our 'only' list was in the 'only' list of other, we don't
      // provide anything new.
      return numSame != named.size() + renamed.size();
    }
  }
}

// Returns true if we should skip looking at this use, because the symbols it
// provides have already been covered by a previous use.
static bool skipUse(std::map<Symbol*, std::vector<UseStmt*> >* seen,
                    UseStmt* current) {
  SymExpr* useSE = toSymExpr(current->src);
  INT_ASSERT(useSE);

  std::vector<UseStmt*> vec = (*seen)[useSE->symbol()];
  if (vec.size() > 0) {
    // We've already seen at least one use of this module, but it might not be
    // thorough enough to justify skipping the newest 'use'.
    for_vector(UseStmt, use, vec) {
      if (!current->providesNewSymbols(use)) {
        // We found a prior use that covered all the symbols available from
        // current.  We can skip looking at current
        return true;
      }
    }
  }
  // We didn't have a prior use, or all the prior uses we missing at least one
  // of the symbols current provides.  Don't skip current.
  return false;
}

/************************************* | **************************************
*                                                                             *
* getScope returns the BaseAST that corresponds to the scope where 'ast'      *
* exists; 'ast' must be an Expr or a Symbol.  Note that if you pass this a    *
* BaseAST that defines a scope, the BaseAST that defines the scope where it   *
* exists will be returned.                                                    *
*                                                                             *
* Thus if a BlockStmt nested in another BlockStmt is passed to getScope,      *
* the outer BlockStmt will be returned.                                       *
*                                                                             *
************************************** | *************************************/

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

