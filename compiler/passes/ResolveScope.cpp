/*
 * Copyright 2004-2018 Cray Inc.
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

#include "scopeResolve.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"

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

  INT_ASSERT(getScopeFor(modSymbol->block) == NULL);

  sScopeMap[modSymbol->block] = this;
}

ResolveScope::ResolveScope(FnSymbol*           fnSymbol,
                           const ResolveScope* parent) {
  mAstRef = fnSymbol;
  mParent = parent;

  INT_ASSERT(getScopeFor(fnSymbol) == NULL);

  sScopeMap[fnSymbol] = this;
}

ResolveScope::ResolveScope(TypeSymbol*         typeSymbol,
                           const ResolveScope* parent) {
  Type* type = typeSymbol->type;

  INT_ASSERT(isEnumType(type) || isAggregateType(type));

  mAstRef = typeSymbol;
  mParent = parent;

  INT_ASSERT(getScopeFor(typeSymbol) == NULL);

  sScopeMap[typeSymbol] = this;
}

ResolveScope::ResolveScope(BlockStmt*          blockStmt,
                           const ResolveScope* parent) {
  mAstRef = blockStmt;
  mParent = parent;

  INT_ASSERT(getScopeFor(blockStmt) == NULL);

  sScopeMap[blockStmt] = this;
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
  extend(dtVoid->symbol);
  extend(dtStringC->symbol);

  extend(gFalse);
  extend(gTrue);

  extend(gTryToken);

  extend(dtNil->symbol);
  extend(gNil);

  extend(gNoInit);

  extend(dtUnknown->symbol);
  extend(dtValue->symbol);

  extend(gUnknown);
  extend(gVoid);

  extend(dtBools[BOOL_SIZE_SYS]->symbol);
  extend(dtBools[BOOL_SIZE_1]->symbol);
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
  extend(gCVoidPtr);
  extend(dtSymbol->symbol);

  extend(dtFile->symbol);
  extend(gFile);

  extend(dtOpaque->symbol);
  extend(gOpaque);

  extend(dtTaskID->symbol);
  extend(gTaskID);

  extend(dtSyncVarAuxFields->symbol);
  extend(gSyncVarAuxFields);

  extend(dtSingleVarAuxFields->symbol);
  extend(gSingleVarAuxFields);

  extend(dtAny->symbol);
  extend(dtIntegral->symbol);
  extend(dtAnyComplex->symbol);
  extend(dtNumeric->symbol);

  extend(dtIteratorRecord->symbol);
  extend(dtIteratorClass->symbol);

  extend(dtMethodToken->symbol);
  extend(gMethodToken);

  extend(dtTypeDefaultToken->symbol);
  extend(gTypeDefaultToken);

  extend(dtModuleToken->symbol);
  extend(gModuleToken);

  extend(dtAnyEnumerated->symbol);

  extend(gBoundsChecking);
  extend(gCastChecking);
  extend(gDivZeroChecking);
  extend(gPrivatization);
  extend(gLocal);
  extend(gNodeID);
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

bool ResolveScope::extend(Symbol* newSym) {
  const char* name   = newSym->name;
  bool        retval = false;

  if (Symbol* oldSym = lookupNameLocally(name)) {
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

    // If oldSym is a constructor and newSym is a type, update with the type
    if (newFn == NULL || newFn->_this == NULL) {
      mBindings[name] = newSym;
    }

  } else {
    mBindings[name] = newSym;
    retval          = true;
  }

  return retval;
}

bool ResolveScope::extend(const UseStmt* stmt) {
  mUseList.push_back(stmt);

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

Symbol* ResolveScope::lookup(Expr* expr) const {
  Symbol* retval = NULL;

  // A lexical lookup from the current scope
  if (UnresolvedSymExpr* uSym = toUnresolvedSymExpr(expr)) {
    retval = lookup(uSym);

  // A dotted reference (<object>.<field>) to a field in an object
  } else if (CallExpr* call = toCallExpr(expr)) {
    if (call->isNamedAstr(astrSdot) == true) {
      retval = getFieldFromPath(call);

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

Symbol* ResolveScope::lookup(UnresolvedSymExpr* usymExpr) const {
  const ResolveScope* ptr    = NULL;
  Symbol*             retval = NULL;

  for (ptr = this; ptr != NULL && retval == NULL; ptr = ptr->mParent) {
    retval = ptr->lookupWithUses(usymExpr);
  }

  return retval;
}

Symbol* ResolveScope::lookupWithUses(UnresolvedSymExpr* usymExpr) const {
  const char* name   = usymExpr->unresolved;
  Symbol*     retval = lookupNameLocally(name);

  if (retval == NULL && mUseList.size() > 0) {
    UseList useList = mUseList;
    SymList symbols;

    buildBreadthFirstUseList(useList);

    // Do not use for_vector(); it terminates on a NULL
    for (size_t i = 0; i < useList.size(); i++) {
      if (const UseStmt* use = useList[i]) {
        if (use->skipSymbolSearch(name) == false) {
          BaseAST*    scopeToUse = use->getSearchScope();
          const char* nameToUse  = name;

          if (use->isARename(name) == true) {
            nameToUse = use->getRename(name);
          }

          if (ResolveScope* next = getScopeFor(scopeToUse)) {
            if (Symbol* sym = next->lookupNameLocally(nameToUse)) {
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

Symbol* ResolveScope::getFieldFromPath(CallExpr* dottedExpr) const {
  Expr*   lhsExpr = dottedExpr->get(1);
  Symbol* retval  = NULL;

  if (Symbol* symbol = lookup(lhsExpr)) {
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
Symbol* ResolveScope::lookupNameLocally(const char* name) const {
  Bindings::const_iterator it     = mBindings.find(name);
  Symbol*                  retval = NULL;

  if (it != mBindings.end()) {
    retval = it->second;
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
}

bool ResolveScope::getFieldsWithUses(const char* fieldName,
                                     SymList&    symbols) const {
  if (Symbol* sym = lookupNameLocally(fieldName)) {
    symbols.push_back(sym);

  } else {
    if (mUseList.size() > 0) {
      std::vector<const UseStmt*> useList = mUseList;

      buildBreadthFirstUseList(useList);

      // Do not use for_vector(); it terminates on a NULL
      for (size_t i = 0; i < useList.size(); i++) {
        const UseStmt* use = useList[i];

        if (use != NULL) {
          if (use->skipSymbolSearch(fieldName) == false) {
            BaseAST*    scopeToUse = use->getSearchScope();
            const char* nameToUse  = NULL;

            if (use->isARename(fieldName) == true) {
              nameToUse = use->getRename(fieldName);
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

void ResolveScope::buildBreadthFirstUseList(UseList& useList) const {
  UseMap visited;

  buildBreadthFirstUseList(useList, useList, visited);
}

void ResolveScope::buildBreadthFirstUseList(UseList& useList,
                                            UseList& current,
                                            UseMap&  visited) const {
  UseList next;

  // use NULL as a sentinel to identify modules of equal depth
  useList.push_back(NULL);

  for (size_t i = 0; i < current.size(); i++) {
    const UseStmt* source = current[i];

    if (source == NULL) {
      break;

    } else {
      SymExpr* se = toSymExpr(source->src);

      INT_ASSERT(se);

      if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
        if (mod->block->useList != NULL) {
          for_actuals(expr, mod->block->useList) {
            UseStmt* use      = toUseStmt(expr);
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
                useList.push_back(useToAdd);
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
          }
        }
      }
    }
  }

  if (next.size() > 0) {
    buildBreadthFirstUseList(useList, next, visited);
  }
}

// Returns true if we should skip looking at this use, because the symbols it
// provides have already been covered by a previous use.
bool ResolveScope::skipUse(UseMap& visited, const UseStmt* current) const {
  SymExpr* useSE  = toSymExpr(current->src);
  UseList  vec    = visited[useSE->symbol()];
  bool     retval = false;

  for (size_t i = 0; i < vec.size() && retval == false; i++) {
    const UseStmt* use = vec[i];

    if (current->providesNewSymbols(use) == false) {
      retval = true;
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
