/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

/************************************* | **************************************
*                                                                             *
* A ResolveScope supports the first phase in mapping a name to a lexically    *
* scoped symbol. A scope is a set of bindings where a binding is a mapping    *
* from a name to a single symbol.  The name must be unique within a given     *
* scope.                                                                      *
*                                                                             *
*                                                                             *
* May 8, 2017:                                                                *
*   Until recently this state was implemented using a std::map and a handful  *
* of file static function.  This first implementation simply wraps that code. *
*                                                                             *
* This representation is able to detect disallowed reuse of a name within a   *
* scope but does not fully support function overloading; a name can only be   *
* mapped to a single AST node.                                                *
*                                                                             *
* Scopes are created for 4 kinds of AST nodes                                 *
*    1) A  BlockStmt  with tag BLOCK_NORMAL                                   *
*    2) An FnSymbol   defines a scope for its formals and query variables     *
*    3) A  TypeSymbol for an enum type                                        *
*    4) A  TypeSymbol for an aggregate type                                   *
*                                                                             *
************************************** | *************************************/

#include "ResolveScope.h"

#include "DecoratedClassType.h"
#include "ForallStmt.h"
#include "ImportStmt.h"
#include "LoopExpr.h"
#include "scopeResolve.h"

ResolveScope* rootScope;

static std::map<BaseAST*, ResolveScope*> sScopeMap;

ResolveScope* ResolveScope::getRootModule() {
  ResolveScope* retval = new ResolveScope(theProgram, NULL);

  retval->addBuiltIns();

  return retval;
}

ResolveScope* ResolveScope::findOrCreateScopeFor(DefExpr* def) {
  BaseAST*      ast    = getScope(def);
  ResolveScope* retval = NULL;

  if (ast == rootModule->block) {
    ast = theProgram->block;
  }

  retval = getScopeFor(ast);

  if (retval == NULL) {
    if (BlockStmt* blockStmt = toBlockStmt(ast)) {
      retval = new ResolveScope(blockStmt, NULL);

    } else if (FnSymbol*  fnSymbol = toFnSymbol(ast)) {
      retval = new ResolveScope(fnSymbol, NULL);

    } else if (TypeSymbol* typeSymbol = toTypeSymbol(ast)) {
      retval = new ResolveScope(typeSymbol, NULL);

    } else if (LoopExpr* fe = toLoopExpr(ast)) {
      retval = new ResolveScope(fe, NULL);

    } else if (ForallStmt* fs = toForallStmt(ast)) {
      retval = new ResolveScope(fs, NULL);

    } else {
      INT_ASSERT(false);
    }

    sScopeMap[ast] = retval;
  }

  return retval;
}

ResolveScope* ResolveScope::getScopeFor(BaseAST* ast) {
  std::map<BaseAST*, ResolveScope*>::iterator it;
  ResolveScope*                               retval = NULL;

  it = sScopeMap.find(ast);

  if (it != sScopeMap.end()) {
    retval = it->second;
  }

  return retval;
}

void ResolveScope::destroyAstMap() {
  std::map<BaseAST*, ResolveScope*>::iterator it;

  for (it = sScopeMap.begin(); it != sScopeMap.end(); it++) {
    delete it->second;
  }

  sScopeMap.clear();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

ResolveScope::ResolveScope(ModuleSymbol*       modSymbol,
                           const ResolveScope* parent) {
  mAstRef = modSymbol;
  mParent = parent;

  // Use modSymbol->block for sScopeMap
  INT_ASSERT(getScopeFor(modSymbol->block) == NULL);
  sScopeMap[modSymbol->block] = this;
}

ResolveScope::ResolveScope(BaseAST*            ast,
                           const ResolveScope* parent) {
  mAstRef = ast;
  mParent = parent;

  INT_ASSERT(getScopeFor(ast) == NULL);
  sScopeMap[ast] = this;
}

/************************************* | **************************************
*                                                                             *
* Historically, definitions have been mapped to scopes by                     *
*   1) Walking gDefExprs                                                      *
*   2) Determining the "scope" for a given DefExpr by walking upwards         *
*      through the AST.                                                       *
*   3) Validating that the definition is valid.                               *
*   4) Extending the scope with the definition                                *
*                                                                             *
* As a special case the built-in symbols, which are defined in rootModule,    *
* are mapped as if they were defined in chpl_Program.                         *
*                                                                             *
* 2017/05/23:                                                                 *
*   Begin to modify this process so that scopes and definitions are managed   *
* using a traditional top-down traversal of the AST starting at chpl_Program. *
*                                                                             *
* This process will overlook the compiler defined built-ins.  This function   *
* is responsible for pre-allocating the scope for chpl_Program and then       *
* inserting the built-ins.
*                                                                             *
************************************** | *************************************/

void ResolveScope::addBuiltIns() {
  extend(dtNothing->symbol);
  extend(dtVoid->symbol);
  extend(dtStringC->symbol);

  extend(gFalse);
  extend(gTrue);

  extend(dtNil->symbol);
  extend(gNil);

  extend(gNoInit);
  extend(gSplitInit);

  extend(dtUnknown->symbol);
  extend(dtAnyRecord->symbol);

  extend(gUnknown);
  extend(gNone);
  extend(gVoid);

  extend(dtBools[BOOL_SIZE_SYS]->symbol);
  extend(dtBools[BOOL_SIZE_8]->symbol);
  extend(dtBools[BOOL_SIZE_16]->symbol);
  extend(dtBools[BOOL_SIZE_32]->symbol);
  extend(dtBools[BOOL_SIZE_64]->symbol);

  extend(dtInt[INT_SIZE_8]->symbol);
  extend(dtInt[INT_SIZE_16]->symbol);
  extend(dtInt[INT_SIZE_32]->symbol);
  extend(dtInt[INT_SIZE_64]->symbol);

  extend(dtUInt[INT_SIZE_8]->symbol);
  extend(dtUInt[INT_SIZE_16]->symbol);
  extend(dtUInt[INT_SIZE_32]->symbol);
  extend(dtUInt[INT_SIZE_64]->symbol);

  extend(dtReal[FLOAT_SIZE_32]->symbol);
  extend(dtReal[FLOAT_SIZE_64]->symbol);

  extend(dtImag[FLOAT_SIZE_32]->symbol);
  extend(dtImag[FLOAT_SIZE_64]->symbol);

  extend(dtComplex[COMPLEX_SIZE_64]->symbol);
  extend(dtComplex[COMPLEX_SIZE_128]->symbol);

  extend(dtCVoidPtr->symbol);
  extend(dtCFnPtr->symbol);

  extend(dtFile->symbol);

  extend(dtOpaque->symbol);
  extend(gOpaque);

  extend(dtTaskID->symbol);
  extend(gTaskID);

  extend(dtSyncVarAuxFields->symbol);
  extend(gSyncVarAuxFields);

  extend(dtSingleVarAuxFields->symbol);
  extend(gSingleVarAuxFields);

  extend(dtAny->symbol);
  extend(dtAnyBool->symbol);
  extend(dtAnyComplex->symbol);
  extend(dtAnyEnumerated->symbol);
  extend(dtAnyImag->symbol);
  extend(dtAnyReal->symbol);
  extend(dtAnyPOD->symbol);

  extend(dtIntegral->symbol);
  extend(dtNumeric->symbol);

  extend(dtIteratorRecord->symbol);
  extend(dtIteratorClass->symbol);
  extend(dtBorrowed->symbol);
  extend(dtBorrowedNonNilable->symbol);
  extend(dtBorrowedNilable->symbol);
  extend(dtUnmanaged->symbol);
  extend(dtUnmanagedNonNilable->symbol);
  extend(dtUnmanagedNilable->symbol);
  extend(dtAnyManagementAnyNilable->symbol);
  extend(dtAnyManagementNonNilable->symbol);
  extend(dtAnyManagementNilable->symbol);

  extend(dtMethodToken->symbol);
  extend(gMethodToken);

  extend(dtTypeDefaultToken->symbol);
  extend(gTypeDefaultToken);

  extend(dtModuleToken->symbol);
  extend(gModuleToken);

  extend(gBoundsChecking);
  extend(gCastChecking);
  extend(gNilChecking);
  extend(gOverloadSetsChecks);
  extend(gDivZeroChecking);
  extend(gPrivatization);
  extend(gLocal);
  extend(gWarnUnstable);
  extend(gNodeID);

  extend(gInfinity);
  extend(gNan);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

std::string ResolveScope::name() const {
  std::string retval = "";

  if        (ModuleSymbol* modSym  = toModuleSymbol(mAstRef)) {
    retval = modSym->name;

  } else if (FnSymbol*     fnSym   = toFnSymbol(mAstRef))     {
    retval = fnSym->name;

  } else if (TypeSymbol*   typeSym = toTypeSymbol(mAstRef))   {
    retval = typeSym->name;

  } else if (BlockStmt*    block   = toBlockStmt(mAstRef))    {
    char buff[1024];

    sprintf(buff, "BlockStmt %9d", block->id);

    retval = buff;

  } else {
    INT_ASSERT(false);
  }

  return retval;
}

int ResolveScope::depth() const {
  const ResolveScope* ptr    = mParent;
  int                 retval =       0;

  while (ptr != NULL) {
    retval = retval + 1;
    ptr    = ptr->mParent;
  }

  return retval;
}

int ResolveScope::numBindings() const {
  Bindings::const_iterator it;
  int                      retval = 0;

  for (it = mBindings.begin(); it != mBindings.end(); it++) {
    retval = retval + 1;
  }

  return retval;
}

BlockStmt* ResolveScope::asBlockStmt() const {
  BlockStmt* retval = NULL;

  if (ModuleSymbol* modSym = toModuleSymbol(mAstRef)) {
    retval = modSym->block;

  } else if (BlockStmt* block = toBlockStmt(mAstRef)) {
    retval = block;

  } else {
    retval = NULL;
  }

  return retval;
}

ModuleSymbol* ResolveScope::enclosingModule() const {
  const ResolveScope* ptr    = NULL;
  ModuleSymbol*       retval = NULL;

  for (ptr = this; ptr != NULL && retval == NULL; ptr = ptr->mParent) {
    retval = toModuleSymbol(ptr->mAstRef);
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool ResolveScope::extend(Symbol* newSym, bool isTopLevel) {
  const char* name   = newSym->name;
  bool        retval = false;

  // If this is a top-level module, we look up the symbol's name as
  // though we were resolving a 'use' in order to take module symbols
  // (not visible through normal lexical scoping) into consideration.
  // Without this, we no longer get duplicate symbol definition
  // warnings when two top-level modules share the same name and a
  // subsequent lookup of that module name picks the first module it
  // finds (since we don't have a concept of overloading for modules
  // or most symbol types other than functions.
  //
  if (Symbol* oldSym = lookupNameLocally(name, isTopLevel)) {
    FnSymbol* oldFn = toFnSymbol(oldSym);
    FnSymbol* newFn = toFnSymbol(newSym);

    // Do not complain if they are both functions
    if (oldFn != NULL && newFn != NULL) {
      retval = true;

    // e.g. record String and proc String(...)
    } else if (isAggregateTypeAndConstructor(oldSym, newSym) == true ||
               isAggregateTypeAndConstructor(newSym, oldSym) == true) {
      retval = true;

    // Methods currently leak their scope and can conflict with variables
    } else if (isSymbolAndMethod(oldSym, newSym)             == true ||
               isSymbolAndMethod(newSym, oldSym)             == true) {
      retval = true;

    } else {
      USR_FATAL(oldSym,
                "'%s' has multiple definitions, redefined at:\n  %s",
                name,
                newSym->stringLoc());
    }

    // Prefer storage of non-functions over functions,
    //                   functions over methods,
    //                   public functions over private functions
    if (newFn == NULL || (newFn->_this == NULL &&
                          newFn->hasFlag(FLAG_PRIVATE) == false)) {
      mBindings[name] = newSym;
    }

  } else {
    mBindings[name] = newSym;
    retval          = true;
  }

  return retval;
}

bool ResolveScope::extend(VisibilityStmt* stmt) {
  mUseImportList.push_back(stmt);

  return true;
}

bool ResolveScope::isAggregateTypeAndConstructor(Symbol* sym0, Symbol* sym1) {
  TypeSymbol* typeSym = toTypeSymbol(sym0);
  FnSymbol*   funcSym = toFnSymbol(sym1);
  bool        retval  = false;

  if (typeSym != NULL && funcSym != NULL && funcSym->_this != NULL) {
    AggregateType* type0 = toAggregateType(typeSym->type);
    AggregateType* type1 = toAggregateType(funcSym->_this->type);

    retval = (type0 == type1) ? true : false;
  }

  return retval;
}

bool ResolveScope::isSymbolAndMethod(Symbol* sym0, Symbol* sym1) {
  FnSymbol* fun0   = toFnSymbol(sym0);
  FnSymbol* fun1   = toFnSymbol(sym1);
  bool      retval = false;

  if (fun0 == NULL && fun1 != NULL && fun1->_this != NULL) {
    retval = true;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Lookup an object based on an expression i.e. <name>                         *
*   1. <name> is a simple lexically scoped name                               *
*   2. <name> is a dotted path to a field in some object                      *
*                                                                             *
************************************** | *************************************/

Symbol* ResolveScope::lookup(Expr* expr, bool isUse) const {
  Symbol* retval = NULL;

  // A lexical lookup from the current scope
  if (UnresolvedSymExpr* uSym = toUnresolvedSymExpr(expr)) {
    retval = lookup(uSym, isUse);

  // A dotted reference (<object>.<field>) to a field in an object
  } else if (CallExpr* call = toCallExpr(expr)) {
    if (call->isNamedAstr(astrSdot) == true) {
      retval = getFieldFromPath(call, isUse);

    } else {
      INT_FATAL(expr, "Not a dotted expr");
    }

  } else {
    INT_FATAL(expr, "Unsupported expr");
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Perform a lexical lookup for a name starting from a specific scope          *
*                                                                             *
************************************** | *************************************/

Symbol* ResolveScope::lookup(UnresolvedSymExpr* usymExpr, bool isUse) const {
  const ResolveScope* ptr    = NULL;
  Symbol*             retval = NULL;

  for (ptr = this; ptr != NULL && retval == NULL; ptr = ptr->mParent) {
    retval = ptr->lookupWithUses(usymExpr, isUse);
  }

  return retval;
}

Symbol* ResolveScope::lookupWithUses(UnresolvedSymExpr* usymExpr, bool isUse) const {
  const char* name   = usymExpr->unresolved;
  Symbol*     retval = lookupNameLocally(name, isUse);

  if (retval == NULL && mUseImportList.size() > 0) {
    UseImportList useImportList = mUseImportList;
    SymList symbols;

    buildBreadthFirstUseImportList(useImportList);

    // Do not use for_vector(); it terminates on a NULL
    for (size_t i = 0; i < useImportList.size(); i++) {
      if (UseStmt* use = toUseStmt(useImportList[i])) {
        if (use->skipSymbolSearch(name, true) == false) {
          BaseAST*    scopeToUse = use->getSearchScope();
          const char* nameToUse  = name;

          if (use->isARenamedSym(name) == true) {
            nameToUse = use->getRenamedSym(name);
          }

          if (ResolveScope* next = getScopeFor(scopeToUse)) {
            if (Symbol* sym = next->lookupNameLocally(nameToUse, isUse)) {
              if (isRepeat(sym, symbols) == false) {
                if (FnSymbol* fn = toFnSymbol(sym)) {
                  if (fn->isMethod() == false) {
                    symbols.push_back(fn);
                  }

                } else {
                  symbols.push_back(sym);
                }
              }
            }
          }
        }

      } else if (ImportStmt* import = toImportStmt(useImportList[i])) {
        BaseAST* scopeToUse = import->getSearchScope();

        if (ResolveScope* next = getScopeFor(scopeToUse)) {
          if (Symbol* sym = next->lookupNameLocally(name, isUse)) {
            if (isRepeat(sym, symbols) == false) {
              if (FnSymbol* fn = toFnSymbol(sym)) {
                if (fn->isMethod() == false) {
                  symbols.push_back(fn);
                }

              } else {
                symbols.push_back(sym);
              }
            }
          }
        }

      // Found a NULL sentinel.  Break if there are results.
      } else {
        if (symbols.size() > 0) {
          break;
        }
      }
    }

    if (symbols.size() == 1) {
      retval = symbols[0];
    }
  }

  return retval;
}

// Returns true if the symbol is present in the vector, false otherwise
bool ResolveScope::isRepeat(Symbol* toAdd, const SymList& symbols) const {
  SymList::const_iterator it;
  bool                    retval = false;

  for (it = symbols.begin(); it != symbols.end() && retval == false; ++it) {
    if (*it == toAdd) {
      retval = true;
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* The name is a dotted expression (i.e. <objectExpr>.<fieldName>).            *
*                                                                             *
* This is a qualified access to a field defined within the object.            *
*                                                                             *
************************************** | *************************************/

Symbol* ResolveScope::getFieldFromPath(CallExpr* dottedExpr, bool isUse) const {
  Expr*   lhsExpr = dottedExpr->get(1);
  Symbol* retval  = NULL;

  if (Symbol* symbol = lookup(lhsExpr, isUse)) {
    if (ModuleSymbol* module = toModuleSymbol(symbol)) {
      if (SymExpr* rhs = toSymExpr(dottedExpr->get(2))) {
        const char* rhsName = NULL;

        if (get_string(rhs, &rhsName) == true) {
          ResolveScope* scope = getScopeFor(module->block);

          if (Symbol* symbol = scope->getField(rhsName)) {
            retval = symbol;

          } else {
            USR_FATAL(dottedExpr, "Cannot find field '%s'", rhsName);
          }

        } else {
          INT_FATAL(dottedExpr, "Bad qualified name");
        }

      } else {
        INT_FATAL(dottedExpr, "Bad qualified name");
      }

    } else {
      INT_ASSERT(false);
    }

  } else {
    if (UnresolvedSymExpr* obj = toUnresolvedSymExpr(lhsExpr)) {
      USR_FATAL(dottedExpr, "Cannot find object '%s'", obj->unresolved);

    } else {
      USR_FATAL(dottedExpr, "Cannot find object");
    }
  }

  return retval;
}

// 2017/06/02: Future updates will avoid returning PRIVATE fields
Symbol* ResolveScope::getField(const char* fieldName) const {
  const ResolveScope* ptr    = this;
  Symbol*             retval = NULL;

  for ( ; ptr != NULL && retval == NULL; ptr = ptr->mParent) {
    retval = ptr->getFieldLocally(fieldName);
  }

  return retval;
}

// 2017/06/02: Future updates will avoid returning PRIVATE fields
Symbol* ResolveScope::getFieldLocally(const char* fieldName) const {
  Bindings::const_iterator it     = mBindings.find(fieldName);
  Symbol*                  retval = NULL;

  if (it != mBindings.end()) {
    retval = it->second;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// 2017/06/02 Used by scopeResolve.
Symbol* ResolveScope::lookupNameLocally(const char* name, bool isUse) const {
  Bindings::const_iterator it     = mBindings.find(name);
  Symbol*                  retval = NULL;

  if (it != mBindings.end()) {
    Symbol* sym = it->second;

    // don't consider top-level modules to be visible unless this is a use
    if (toModuleSymbol(sym) == NULL || this != rootScope || isUse) {
      retval = sym;
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Entry point for UseStmt to check except/only lists.                         *
*                                                                             *
************************************** | *************************************/

void ResolveScope::getFields(const char* fieldName,
                             SymList&    symbols) const {
  std::set<const ResolveScope*> visited;

  getFields(fieldName, visited, symbols);
}

void ResolveScope::getFields(const char* fieldName,
                             ScopeSet&   visited,
                             SymList&    symbols) const {
  if (visited.find(this) == visited.end()) {
    if (getFieldsWithUses(fieldName, symbols) == true) {

    } else if (isModuleSymbol(mAstRef) == true) {
      if (mParent != NULL) {
        visited.insert(this);

        mParent->getFields(fieldName, visited, symbols);
      }

    } else {
      INT_ASSERT(false);
    }
  }

  if (symbols.size() > 0) {
    for (std::vector<Symbol*>::iterator it = symbols.begin();
         it != symbols.end();
         it++) {
      if (*it == mAstRef) {
        // Only and except lists should not return the original module name.
        symbols.erase(it);
        break;
      }
    }
  }
}

bool ResolveScope::getFieldsWithUses(const char* fieldName,
                                     SymList&    symbols) const {
  if (Symbol* sym = lookupNameLocally(fieldName)) {
    symbols.push_back(sym);

  } else {
    if (mUseImportList.size() > 0) {
      std::vector<VisibilityStmt*> useImportList = mUseImportList;

      buildBreadthFirstUseImportList(useImportList);

      // Do not use for_vector(); it terminates on a NULL
      for (size_t i = 0; i < useImportList.size(); i++) {
        UseStmt* use = toUseStmt(useImportList[i]);

        if (use != NULL) {
          if (use->skipSymbolSearch(fieldName, true) == false) {
            BaseAST*    scopeToUse = use->getSearchScope();
            const char* nameToUse  = NULL;

            if (use->isARenamedSym(fieldName) == true) {
              nameToUse = use->getRenamedSym(fieldName);
            } else {
              nameToUse = fieldName;
            }

            if (ResolveScope* next = getScopeFor(scopeToUse)) {
              if (Symbol* sym = next->lookupNameLocally(nameToUse)) {
                symbols.push_back(sym);
              }
            }
          }

        } else if (symbols.size() > 0) {
          break;
        }
      }
    }
  }

  return (symbols.size() != 0) ? true : false;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void ResolveScope::buildBreadthFirstUseImportList(UseImportList& useImportList) const {
  UseImportMap visited;

  buildBreadthFirstUseImportList(useImportList, useImportList, visited);
}

void
ResolveScope::buildBreadthFirstUseImportList(UseImportList& useImportList,
                                             UseImportList& current,
                                             UseImportMap&  visited) const {
  UseImportList next;

  // use NULL as a sentinel to identify modules of equal depth
  useImportList.push_back(NULL);

  for (size_t i = 0; i < current.size(); i++) {
    UseStmt* source = toUseStmt(current[i]);

    if (source == NULL) {
      break;

    } else {
      SymExpr* se = toSymExpr(source->src);

      INT_ASSERT(se);

      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        if (mod->block->useList != NULL) {
          for_actuals(expr, mod->block->useList) {
            if (UseStmt* use = toUseStmt(expr)) {
              SymExpr* useSE    = toSymExpr(use->src);
              UseStmt* useToAdd = NULL;

              if (useSE->symbol()->hasFlag(FLAG_PRIVATE) == false) {
                // Uses of private modules are not transitive -
                // the symbols in the private modules are only visible to
                // itself and its immediate parent.  Therefore, if the symbol
                // is private, we will not traverse it further and will merely
                // add it to the alreadySeen map.
                useToAdd = use->applyOuterUse(source);

                if (useToAdd                   != NULL &&
                    skipUse(visited, useToAdd) == false) {
                  next.push_back(useToAdd);
                  useImportList.push_back(useToAdd);
                }

                // If applyOuterUse returned NULL, the number of symbols
                // that could be provided from this use was 0,
                // so it didn't need to be added to the alreadySeen map.
                if (useToAdd != NULL) {
                  visited[useSE->symbol()].push_back(useToAdd);
                }

              } else {
                visited[useSE->symbol()].push_back(use);
              }
            } else if (isImportStmt(expr)) {
              // Don't do anything, imports only apply to qualified access
            } else {
              INT_ASSERT("Unexpected expr, expected ImportStmt or UseStmt");
            }
          }
        }
      }
    }
  }

  if (next.size() > 0) {
    buildBreadthFirstUseImportList(useImportList, next, visited);
  }
}

// Returns true if we should skip looking at this use, because the symbols it
// provides have already been covered by a previous use.
bool ResolveScope::skipUse(UseImportMap& visited, const UseStmt* current) const {
  SymExpr* useSE  = toSymExpr(current->src);
  UseImportList  vec    = visited[useSE->symbol()];
  bool     retval = false;

  for (size_t i = 0; i < vec.size() && retval == false; i++) {
    UseStmt* use = toUseStmt(vec[i]);

    if (use != NULL) {
      if (current->providesNewSymbols(use) == false) {
        retval = true;
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void ResolveScope::describe() const {
  Bindings::const_iterator it;
  const char*              blockParent = "";
  int                      index       = 0;

  if (BlockStmt* block = toBlockStmt(mAstRef)) {
    blockParent = block->parentSymbol->name;
  }

  printf("#<ResolveScope %s %s\n", name().c_str(), blockParent);
  printf("  Depth:       %19d\n", depth());
  printf("  NumBindings: %19d\n", numBindings());

  for (it = mBindings.begin(); it != mBindings.end(); it++, index++) {
    printf("    %3d: %s\n", index, it->first);
  }

  printf(">\n\n");
}
