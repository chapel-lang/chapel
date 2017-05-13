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

#include "UseStmt.h"

#include "AstVisitor.h"
#include "scopeResolve.h"
#include "stlUtil.h"
#include "visibleFunctions.h"

#include <algorithm>

UseStmt::UseStmt(BaseAST* source) : Stmt(E_UseStmt) {
  src    = NULL;
  except = false;

  if (Symbol* b = toSymbol(source)) {
    src = new SymExpr(b);

  } else if (Expr* b = toExpr(source)) {
    src = b;

  } else {
    INT_FATAL(this, "Bad mod in UseStmt constructor");
  }

  gUseStmts.add(this);
}

UseStmt::UseStmt(BaseAST*                            source,
                 std::vector<const char*>*           args,
                 bool                                exclude,
                 std::map<const char*, const char*>* renames) :
  Stmt(E_UseStmt) {

  src    = NULL;
  except = exclude;

  if (Symbol* b = toSymbol(source)) {
    src = new SymExpr(b);

  } else if (Expr* b = toExpr(source)) {
    src = b;

  } else {
    INT_FATAL(this, "Bad mod in UseStmt constructor");
  }

  if (args->size() > 0) {
    // Symbols to search when going through this module's scope from an outside
    // scope
    for_vector(const char, str, *args) {
      named.push_back(str);
    }
  }

  if (renames->size() > 0) {
    // The new names of symbols in the module being used, to avoid conflicts
    // for instance.
    for (std::map<const char*, const char*>::iterator it = renames->begin();
         it != renames->end();
         ++it) {
      renamed[it->first] = it->second;
    }
  }

  gUseStmts.add(this);
}

UseStmt* UseStmt::copyInner(SymbolMap* map) {
  UseStmt *_this = 0;

  if (named.size() > 0) { // MPF: should this have || renamed.size() > 0?
    _this = new UseStmt(COPY_INT(src), &named, except, &renamed);
  } else {
    _this = new UseStmt(COPY_INT(src));
  }

  for_vector(const char, sym, relatedNames) {
    _this->relatedNames.push_back(sym);
  }

  return _this;
}

Expr* UseStmt::getFirstExpr() {
  return this;
}

Expr* UseStmt::getFirstChild() {
  return NULL;
}

void UseStmt::replaceChild(Expr* oldAst, Expr* newAst) {
  if (oldAst == src) {
    src = newAst;

  } else {
    INT_FATAL(this, "Unexpected case in UseStmt::replaceChild");
  }
}

void UseStmt::accept(AstVisitor* visitor) {
  visitor->visitUseStmt(this);
}

void UseStmt::verify() {
  Expr::verify();

  if (astTag != E_UseStmt) {
    INT_FATAL(this, "Bad NamedExpr::astTag");
  }

  if (src == NULL) {
    INT_FATAL(this, "Bad UseStmt::src");
  }

  if (relatedNames.size() != 0 &&
      named.size()        == 0 &&
      renamed.size()      == 0) {
    INT_FATAL(this,
              "Have names to avoid, but nothing was listed in the "
              "use to begin with");
  }

  verifyNotOnList(src);
}

bool UseStmt::isPlainUse() const {
  return named.size() == 0 && renamed.size() == 0;
}

bool UseStmt::hasOnlyList() const {
  return isPlainUse() == false && except == false;
}

bool UseStmt::hasExceptList() const {
  return isPlainUse() == false && except == true;
}

bool UseStmt::isARename(const char* name) const {
  return renamed.count(name) == 1;
}

const char* UseStmt::getRename(const char* name) {
  return renamed[name];
}

/************************************* | **************************************
*                                                                             *
* The parser currently accepts use statements with general expressions e.g.   *
*   use a + b;                                                                *
*   use 1.2;                                                                  *
*                                                                             *
* This method returns true if the syntax is valid.                            *
* The currnet implementation signals a FATAL error if it is not.              *
*                                                                             *
************************************** | *************************************/

bool UseStmt::isValid() const {
  return isValid(src);
}

bool UseStmt::isValid(Expr* expr) const {
  bool retval = false;

  if (isUnresolvedSymExpr(expr) == true) {
    retval = true;

  } else if (CallExpr* call = toCallExpr(expr)) {
    if (call->isNamed(".") == true) {
      if (isValid(call->get(1)) == true) {
        if (SymExpr* rhs = toSymExpr(call->get(2))) {
          VarSymbol* v = toVarSymbol(rhs->symbol());

          if (v                        != NULL &&
              v->immediate             != NULL &&
              v->immediate->const_kind == CONST_KIND_STRING) {
            retval = true;

          } else {
            INT_FATAL(this, "Bad use statement in getUsedSymbol");
          }

        } else {
          INT_FATAL(this, "Bad use statement in getUsedSymbol");
        }
      }

    } else {
      USR_FATAL(this,
                "'use' statements must refer to module or enum symbols "
                "(e.g., 'use <module>[.<submodule>]*;')");
    }

  } else if (SymExpr* symExpr = toSymExpr(expr)) {
    if (symExpr->symbol()->isImmediate() == false) {
      retval = true;

    } else {
      USR_FATAL(this,
                "'use' statements must refer to module or enum symbols "
                "(e.g., 'use <module>[.<submodule>]*;')");
    }

  } else {
    INT_FATAL(this, "Unexpected use stmt");
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void UseStmt::scopeResolve() {
  SET_LINENO(this);

  if (Symbol* sym = getUsedSymbol(src)) {
    if (sym == rootModule) {

    // This may happen during symbol renaming
    } else if (parentExpr == NULL) {
      src = new SymExpr(sym);

    } else {
      ModuleSymbol* enclosingModule = getModule();
      BlockStmt*    enclosingBlock  = getVisibilityBlock(this);

      src->replace(new SymExpr(sym));

      if (ModuleSymbol* mod = toModuleSymbol(sym)) {
        enclosingModule->moduleUseAdd(mod);
      }

      remove();
      enclosingBlock->useListAdd(this);

      validateList();
    }

  } else {
    USR_FATAL(this, "Cannot find module or enum");
  }
}

//
// Return the module or enum imported by a use call.
// The module could be nested: e.g. "use outermost.middle.innermost;"
//
Symbol* UseStmt::getUsedSymbol(Expr* expr) {
  Symbol* retval = NULL;

  // 1) The common case of                'use <name>;'
  // 2) Also invoked when recursing for   'use <name>.<name>;'
  if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(expr)) {
    //
    // This case handles the (common) case that we're 'use'ing a
    // symbol that we have not yet resolved.
    //
    if (Symbol* symbol = lookup(sym->unresolved, this)) {
      if (isValidUsedSymbol(symbol) == true) {
        retval = symbol;
      }

    } else {
      USR_FATAL(this, "Cannot find module or enum '%s'", sym->unresolved);
    }

  // This handles the case of 'use <symbol>.<symbol>'
  } else if (CallExpr* call = toCallExpr(expr)) {
    if (ModuleSymbol* lhs = toModuleSymbol(getUsedSymbol(call->get(1)))) {
      if (SymExpr* rhs = toSymExpr(call->get(2))) {
        const char* rhsName = NULL;

        if (get_string(rhs, &rhsName) == true) {
          if (Symbol* symbol = lookup(rhsName, lhs->block)) {
            if (isValidUsedSymbol(symbol) == true) {
              retval = symbol;
            }

          } else {
            USR_FATAL(this, "Cannot find module '%s'", rhsName);
          }

        } else {
          INT_FATAL(this, "Bad use statement in getUsedSymbol");
        }

      } else {
        INT_FATAL(this, "Bad use statement in getUsedSymbol");
      }

    } else {
      USR_FATAL(this, "Cannot find module");
    }

  //
  // The parser generates 1 instance of this to represent the dependency
  // betweeen ChapelBase and rootModule
  //
  // The remaining cases are generated by symbol renaming
  //
  } else if (SymExpr* sym = toSymExpr(expr)) {
    if (Symbol* symbol = sym->symbol()) {
      if (isValidUsedSymbol(symbol) == true) {
        retval = symbol;
      }

    } else {
      printUseError();
    }

  } else {
    INT_FATAL(this, "Bad use statement in getUsedSymbol");
  }

  return retval;
}

bool UseStmt::isValidUsedSymbol(Symbol* symbol) const {
  bool retval = false;

  if (isModuleSymbol(symbol) == true) {
    retval = true;

  } else if (TypeSymbol* type = toTypeSymbol(symbol)) {
    if (isEnumType(type->type) == true) {
      retval = true;

    } else {
      printUseError(symbol);
    }

  } else {
    printUseError(symbol);
  }

  return retval;
}

// Verifies that all the symbols in the include and exclude lists of use
// statements refer to symbols that are visible from that module.
void UseStmt::validateList() {
  if (isPlainUse() == false) {
    BaseAST*    scopeToUse = getSearchScope();
    const char* listName   = except ? "except" : "only";

    noRepeats();

    for_vector(const char, name, named) {
      if (name[0] != '\0') {
        if (Symbol* sym = lookup(name, scopeToUse)) {
          if (sym->isVisible(this) == true) {
            createRelatedNames(sym);

          } else {
            USR_FATAL_CONT(this,
                           "Bad identifier in '%s' clause, '%s' is private",
                           listName,
                           name);
          }

        } else {
          USR_FATAL_CONT(this,
                         "Bad identifier in '%s' clause, no known '%s'",
                         listName, name);
        }
      }
    }

    for (std::map<const char*, const char*>::iterator it = renamed.begin();
         it != renamed.end();
         ++it) {
      if (Symbol* sym = lookup(it->second, scopeToUse)) {
        if (sym->isVisible(this) == true) {
          createRelatedNames(sym);

        } else {
          USR_FATAL_CONT(this,
                         "Bad identifier in rename, '%s' is private",
                         it->second);
        }

      } else {
        SymExpr* se = toSymExpr(src);

        INT_ASSERT(se);

        USR_FATAL_CONT(this,
                       "Bad identifier in rename, no known '%s' in '%s'",
                       it->second,
                       se->symbol()->name);
      }
    }
  }
}

// Should only be called when the mod field has been resolved
BaseAST* UseStmt::getSearchScope() const {
  BaseAST* retval = NULL;

  if (SymExpr* se = toSymExpr(src)) {
    if (ModuleSymbol* module = toModuleSymbol(se->symbol())) {
      retval = module->block;

    } else if (TypeSymbol* enumTypeSym = toTypeSymbol(se->symbol())) {
      retval = enumTypeSym;

    } else {
      INT_FATAL(this, "Use invalid, not applied to module or enum");
    }

  } else {
    INT_FATAL(this, "getSearchScope called before this use was processed");
  }

  return retval;
}

void UseStmt::noRepeats() const {
  for (std::vector<const char*>::const_iterator it = named.begin();
       it != named.end();
       ++it) {
    std::vector<const char*>::const_iterator next = it;

    for (++next; next != named.end(); ++next) {
      // Check rest of named for the same name
      if (strcmp(*it, *next) == 0) {
        USR_WARN(this, "identifier '%s' is repeated", *it);
      }
    }

    for (std::map<const char*, const char*>::const_iterator
           renamedIt = renamed.begin();
         renamedIt != renamed.end();
         ++renamedIt) {

      if (strcmp(*it, renamedIt->second) == 0) {
        // This identifier is also used as the old name for a renaming.
        // Probably a mistake on the user's part, but not a catastrophic one
        USR_WARN(this, "identifier '%s' is repeated", *it);
      }

      if (strcmp(*it, renamedIt->first) == 0) {
        // The user attempted to rename a symbol to a name that was already
        // in the 'only' list.  This causes a naming conflict.
        USR_FATAL_CONT(this, "symbol '%s' multiply defined", *it);
      }
    }
  }

  for (std::map<const char*, const char*>::const_iterator it = renamed.begin();
       it != renamed.end();
       ++it) {
    std::map<const char*, const char*>::const_iterator next = it;

    for (++next; next != renamed.end(); ++next) {
      if (strcmp(it->second, next->second) == 0) {
        // Renamed this variable twice.  Probably a mistake on the user's part,
        // but not a catastrophic one
        USR_WARN(this, "identifier '%s' is repeated", it->second);
      }

      if (strcmp(it->second, next->first) == 0) {
        // This name is the old_name in one rename and the new_name in another
        // Did the user actually want to cut out the middle man?
        USR_WARN(this, "identifier '%s' is repeated", it->second);
        USR_PRINT("Did you mean to rename '%s' to '%s'?",
                  next->second,
                  it->first);
      }

      if (strcmp(it->first, next->second) == 0) {
        // This name is the old_name in one rename and the new_name in another
        // Did the user actually want to cut out the middle man?
        USR_WARN(this, "identifier '%s' is repeated", it->first);
        USR_PRINT("Did you mean to rename '%s' to '%s'?",
                  it->second,
                  next->first);
      }
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

    unsigned int constrLen      = strlen(ts->name) + strlen("_construct_") + 1;
    unsigned int typeConstrLen  = constrLen        + strlen("_type");

    char*        constrName     = (char*) malloc(constrLen);
    char*        typeConstrName = (char*) malloc(typeConstrLen);

    strcpy(constrName,     "_construct_");
    strcat(constrName,     ts->name);

    strcpy(typeConstrName, "_type_construct_");
    strcat(typeConstrName, ts->name);

    relatedNames.push_back(constrName);
    relatedNames.push_back(typeConstrName);
  }
}

void UseStmt::printUseError() const {
  USR_FATAL(this,
            "'use' statements must refer to module or enum symbols "
            "(e.g., 'use <module>[.<submodule>]*;')");
}

void UseStmt::printUseError(Symbol* sym) const {
  if (sym->isImmediate() == true) {
    USR_FATAL(this,
              "'use' statements must refer to module or enum symbols "
              "(e.g., 'use <module>[.<submodule>]*;')");

  } else if (sym->name != NULL) {
    USR_FATAL(this, "'use' of non-module/enum symbol %s", sym->name);

  } else {
    USR_FATAL(this, "'use' of non-module/enum symbol");
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void UseStmt::writeListPredicate(FILE* mFP) const {
  if (hasOnlyList() == true) {
    fprintf(mFP, " 'only' ");

  } else if (hasExceptList() == true) {
    fprintf(mFP, " 'except' ");
  }
}

/************************************* | **************************************
*                                                                             *
* Determine whether the use permits us to search for a symbol with the given  *
* name.  Returns true ("should skip") if the name is related to our 'except'  *
* list, or not present when we've been given an 'only' list.                  *
*                                                                             *
************************************** | *************************************/

bool UseStmt::skipSymbolSearch(const char* name) const {
  bool retval = false;

  if (isPlainUse() == true) {
    retval = false;

  } else if (except == true) {
    if (matchedNameOrConstructor(name) == true) {
      retval =  true;

    } else if (inRelatedNames(name)) {
      retval =  true;

    } else {
      retval = false;
    }

  } else {
    if (matchedNameOrConstructor(name) == true) {
      retval = false;

    } else if (inRelatedNames(name) == true) {
      retval = false;

    } else {
      retval =  true;
    }
  }

  return retval;
}

bool UseStmt::matchedNameOrConstructor(const char* name) const {
  for_vector(const char, toCheck, named) {
    if (strcmp(name, toCheck) == 0) {
      return true;
    }
  }

  for(std::map<const char*, const char*>::const_iterator it = renamed.begin();
      it != renamed.end();
      ++it) {
    if (strcmp(name, it->first) == 0) {
      return true;
    }
  }

  return false;
}

// Returns true if the name was in the relatedNames field, false otherwise.
bool UseStmt::inRelatedNames(const char* name) const {
  for_vector(const char, toCheck, relatedNames) {
    if (strcmp(name, toCheck) == 0) {
      return true;
    }
  }

  return false;
}

/************************************* | **************************************
*                                                                             *
* If the outer use statement would limit this one, return a new use of our    *
* module with the additional symbols accounted for.  This new use will not    *
* be added to the AST, but will be reused in scopeResolution if the same use  *
* path is followed.                                                           *
*                                                                             *
* If the outer use does not require us to alter ourself, return ourself.      *
* If the combination of the two uses results in no new symbols being provided *
* by this module, return NULL.                                                *
*                                                                             *
************************************** | *************************************/

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
        if (std::find(outer->named.begin(),
                      outer->named.end(),
                      includeMe) == outer->named.end()) {

          // We didn't find this symbol in the list to exclude, so
          // add it.
          newOnlyList.push_back(includeMe);
        }
      }

      std::map<const char*, const char*> newRenamed;

      for (std::map<const char*, const char*>::iterator it = renamed.begin();
          it != renamed.end();
           ++it) {
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
        // All of the 'only' list was in the 'except' list,
        // so we don't provide new symbols.
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

/************************************* | **************************************
*                                                                             *
* Returns true if the current use statement has the possibility of allowing   *
* symbols that weren't already covered by 'other'                             *
*                                                                             *
* Assumes that other->mod == this->mod.  Will not verify that fact.           *
*                                                                             *
************************************** | *************************************/

bool UseStmt::providesNewSymbols(UseStmt* other) const {
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
      }

      for(std::map<const char*, const char*>::const_iterator it =
            renamed.begin();
          it != renamed.end();
          ++it) {
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
