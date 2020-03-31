/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "ImportStmt.h"

#include "AstVisitor.h"
#include "ResolveScope.h"
#include "stringutil.h"

#include <algorithm>

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

ImportStmt::ImportStmt(BaseAST* source,
                       bool isPrivate) : VisibilityStmt(E_ImportStmt) {
  this->isPrivate = isPrivate;
  this->modRename = astr("");
  if (Symbol* b = toSymbol(source)) {
    src = new SymExpr(b);

  } else if (Expr* b = toExpr(source)) {
    src = b;

  } else {
    INT_FATAL(this, "Bad mod in ImportStmt constructor");
  }

  gImportStmts.add(this);
}

ImportStmt::ImportStmt(BaseAST* source, const char* rename,
                       bool isPrivate) : VisibilityStmt(E_ImportStmt) {

  this->isPrivate = isPrivate;
  this->modRename = astr(rename);

  if (Symbol* b = toSymbol(source)) {
    src = new SymExpr(b);

  } else if (Expr* b = toExpr(source)) {
    src = b;

  } else {
    INT_FATAL(this, "Bad mod in ImportStmt constructor");
  }

  gImportStmts.add(this);
}

ImportStmt::ImportStmt(BaseAST* source, bool isPrivate,
                       std::vector<const char*>* namesList,
                       std::map<const char*, const char*>* renamesMap):
  VisibilityStmt(E_ImportStmt) {

  this->isPrivate = isPrivate;
  this->modRename = astr("");
  if (Symbol* b = toSymbol(source)) {
    src = new SymExpr(b);

  } else if (Expr* b = toExpr(source)) {
    src = b;

  } else {
    INT_FATAL(this, "Bad mod in ImportStmt constructor");
  }

  if (namesList->size() > 0) {
    // Symbols to enable unqualified access to
    for_vector(const char, str, *namesList) {
      unqualified.push_back(str);
    }
  }

  if (renamesMap->size() > 0) {
    // Symbols to enable unqualified access to with a different name than the
    // name with which they were originally declared
    for (std::map<const char*, const char*>::iterator it = renamesMap->begin();
         it != renamesMap->end(); ++it) {
      renamed[it->first] = it->second;
    }
  }

  gImportStmts.add(this);
}

ImportStmt* ImportStmt::copyInner(SymbolMap* map) {
  ImportStmt* _this = NULL;
  if (modRename != astr("")) {
    _this = new ImportStmt(COPY_INT(src), modRename, isPrivate);
  } else {
    _this = new ImportStmt(COPY_INT(src), isPrivate, &unqualified, &renamed);
  }

  return _this;
}

Expr* ImportStmt::getFirstExpr() {
  return src;
}

void ImportStmt::replaceChild(Expr* oldAst, Expr* newAst) {
  if (oldAst == src) {
    src = newAst;

  } else {
    INT_FATAL(this, "Unexpected case in ImportStmt::replaceChild");
  }
}

void ImportStmt::accept(AstVisitor* visitor) {
  visitor->visitImportStmt(this);
}

void ImportStmt::verify() {
  Expr::verify();

  if (astTag != E_ImportStmt) {
    INT_FATAL(this, "Bad NamedExpr::astTag");
  }

  if (src == NULL) {
    INT_FATAL(this, "Bad ImportStmt::src");
  }

  verifyNotOnList(src);
}

//
// Resolve the module symbol referred to by the ImportStmt
//
void ImportStmt::scopeResolve(ResolveScope* scope) {
  // 2020/03/02: checkValid() does not currently return on failure, to generate
  // good error messages
  if (checkValid(src) == true) {
    // 2017/05/28 The parser inserts a normalized UseStmt of ChapelBase
    if (isSymExpr(src)) {
      INT_FATAL("This should only happen for a UseStmt");

    } else if (Symbol* sym = scope->lookupForImport(src, false)) {
      SET_LINENO(this);

      if (ModuleSymbol* modSym = toModuleSymbol(sym)) {
        scope->enclosingModule()->moduleUseAdd(modSym);

        updateEnclosingBlock(scope, sym);

        validateList();

      } else {
        if (sym->isImmediate() == true) {
          USR_FATAL(this,
                    "'import' statements must include a module symbol "
                    "(e.g., 'import <module>;')");

        } else if (sym->name != NULL) {
          if (isCallExpr(src) == false) {
            // We found a symbol that wasn't a module, but the import statement
            // wasn't looking in a path with one or more `.`s in it.  That
            // means this symbol is already available for unqualified access
            USR_FATAL(this, "Can't 'import' without naming a module");
          }

          if (unqualified.size() != 0) {
            // We already have listed unqualified access for this import, which
            // means this is the last symbol prior to the curly braces (e.g.
            // this is `B` of `import A.B.{C, D};`).  This symbol is required
            // to be a module
            USR_FATAL(this, "Last symbol prior to `{` in import must be a "
                      "module, symbol '%s' is not", sym->name);
          }

          if (modRename[0] != '\0') {
            // The user wanted to rename this symbol when bringing it in.
            // Move the module rename to be the rename for the symbol
            renamed[modRename] = sym->name;
            modRename = "";
          } else {
            // We want to only enable unqualified access of this particular
            // symbol in the module
            this->unqualified.push_back(sym->name);
          }

          ModuleSymbol* parentSym = toModuleSymbol(sym->defPoint->parentSymbol);
          if (parentSym == NULL) {
            INT_ASSERT(sym->defPoint->parentSymbol != NULL);
            USR_FATAL_CONT(this, "only the last symbol in an 'import' "
                           "statement's path can be something other than a "
                           "module");
            USR_PRINT(this, "'%s' is not a module",
                      sym->defPoint->parentSymbol->name);
            USR_STOP();
          }
          scope->enclosingModule()->moduleUseAdd(parentSym);
          updateEnclosingBlock(scope, parentSym);

          validateList();

        } else {
          INT_FATAL(this, "'import' of non-module symbol");
        }
      }
    } else {
      if (UnresolvedSymExpr* import = toUnresolvedSymExpr(src)) {
        USR_FATAL(this, "Cannot find module or symbol '%s'",
                  import->unresolved);
      } else {
        INT_FATAL(this, "Cannot find module");
      }
    }
  }
}

//
// Go into the module referred to by the import statement
//
BaseAST* ImportStmt::getSearchScope() const {
  BaseAST* retval = NULL;

  if (SymExpr* se = toSymExpr(src)) {
    if (ModuleSymbol* module = toModuleSymbol(se->symbol())) {
      retval = module->block;

    } else {
      INT_FATAL(this, "Import invalid, not applied to module");
    }

  } else {
    INT_FATAL(this, "getSearchScope called before this import was processed");
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* The parser currently accepts import statements with general expressions     *
* e.g.                                                                        *
*                                                                             *
*   import a + b;                                                             *
*   import 1.2;                                                               *
*                                                                             *
* This method returns true if the syntax is valid.                            *
* The current implementation signals a FATAL error if it is not.              *
*                                                                             *
************************************** | *************************************/
// Note that this implementation is expected to diverge from that of UseStmt,
// due to ImportStmts supporting things like `import M.x;` where x is not a
// module symbol, and due to UseStmts supporting enums.  ImportStmts may support
// enums in the future, but that will wait on user requests
bool ImportStmt::checkValid(Expr* expr) const {
  bool retval = false;

  if (isUnresolvedSymExpr(expr) == true) {
    retval = true;

  } else if (CallExpr* call = toCallExpr(expr)) {
    if (call->isNamedAstr(astrSdot) == true) {
      if (checkValid(call->get(1)) == true) {
        if (SymExpr* rhs = toSymExpr(call->get(2))) {
          VarSymbol* v = toVarSymbol(rhs->symbol());

          if (v                        != NULL &&
              v->immediate             != NULL &&
              v->immediate->const_kind == CONST_KIND_STRING) {
            retval = true;

          } else {
            INT_FATAL(this, "Bad import statement");
          }

        } else {
          INT_FATAL(this, "Bad import statement");
        }
      }

    } else {
      USR_FATAL(this,
                "'import' statement paths must start with at least one module "
                "symbol (e.g., 'import <module>[.<submodule>]*;')");
    }

  } else if (SymExpr* symExpr = toSymExpr(expr)) {
    if (symExpr->symbol()->isImmediate() == false) {
      retval = true;

    } else {
      // This probably should be an INT_FATAL
      USR_FATAL(this,
                "'import' statement paths must start with at least one module "
                "symbol (e.g., 'import <module>[.<submodule>]*;')");
    }

  } else {
    INT_FATAL(this, "Unexpected import stmt");
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Verifies that all the symbols in the list for unqualified access of import  *
* statements refer to symbols that are visible from that module.              *
*                                                                             *
************************************** | *************************************/
void ImportStmt::validateList() {
  noRepeats();

  validateUnqualified();
  validateRenamed();
}

void ImportStmt::noRepeats() const {
  std::vector<const char*>::const_iterator           it1;

  for (it1 = unqualified.begin(); it1 != unqualified.end(); ++it1) {
    std::vector<const char*>::const_iterator next = it1;
    std::map<const char*, const char*>::const_iterator rit;

    for (++next; next != unqualified.end(); ++next) {
      // Check rest of named for the same name
      if (strcmp(*it1, *next) == 0) {
        USR_WARN(this, "identifier '%s' is repeated", *it1);
      }
    }

    for (rit = renamed.begin(); rit != renamed.end(); ++rit) {
      if (strcmp(*it1, rit->second) == 0) {
        // This identifier is also used as the old name for a renaming.
        // Probably a mistake on the user's part, but not a catastrophic one
        USR_WARN(this, "identifier '%s' is repeated", *it1);
      }

      if (strcmp(*it1, rit->first) == 0) {
        // The user attempted to rename a symbol to a name that was already
        // in the 'only' list.  This causes a naming conflict.
        USR_FATAL_CONT(this, "symbol '%s' multiply defined", *it1);
      }
    }
  }

  noRepeatsInRenamed();
}

void ImportStmt::validateUnqualified() {
    BaseAST*            scopeToUse = getSearchScope();
  const ResolveScope* scope      = ResolveScope::getScopeFor(scopeToUse);

  for_vector(const char, name, unqualified) {
    if (name[0] != '\0') {
      std::vector<Symbol*> symbols;

      scope->getFields(name, symbols);

      if (symbols.size() == 0) {
        SymExpr* srcExpr = toSymExpr(src);
        INT_ASSERT(srcExpr); // should have been resolved by this point
        USR_FATAL_CONT(this,
                       "Bad identifier, no known '%s' defined in '%s'",
                       name,
                       srcExpr->symbol()->name);

      } else {
        for_vector(Symbol, sym, symbols) {
          if (sym->hasFlag(FLAG_PRIVATE) == true) {
            USR_FATAL_CONT(this,
                           "Bad identifier, '%s' is private",
                           name);
          }
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Determine whether the import permits us to search for a symbol with the     *
* given name.  Returns true ("should skip") if the import doesn't specify any *
* symbols for unqualified access, or if the name provided does not match any  *
* of them                                                                     *
*                                                                             *
************************************** | *************************************/

bool ImportStmt::skipSymbolSearch(const char* name) {
  // We don't define any symbols for unqualified access, so we should skip this
  // import
  if (unqualified.size() == 0 && renamed.size() == 0) {
    return true;
  } else {
    // Otherwise, look through the list of unqualified symbol names to see if
    // this one was listed
    for_vector(const char, toCheck, unqualified) {
      if (toCheck == name)
        return false;
    }

    for(std::map<const char*, const char*>::const_iterator it = renamed.begin();
        it != renamed.end();
        ++it) {
      if (astr(name) == astr(it->first)) {
        return false;
      }
    }
    return true;
  }
}

/************************************* | **************************************
*                                                                             *
* Determine if the imported module should be considered for supporting        *
* qualified access.                                                           *
*                                                                             *
************************************** | *************************************/

bool ImportStmt::providesQualifiedAccess() const {
  /* Today, ImportStmts only support qualified access of all symbols in the
     imported modules, or supports unqualified access of certain symbols in it,
     but not both.  There are plans for enabling both if we desire it (by
     allowing `this` in ImportStmt's unqualified access lists)
  */
  if (unqualified.size() == 0 && renamed.size() == 0) {
    return true;
  } else {
    return false;
  }
}

bool ImportStmt::providesUnqualifiedAccess() const {
  if (unqualified.size() != 0 || renamed.size() != 0) {
    return true;
  } else {
    return false;
  }
}

/************************************* | **************************************
*                                                                             *
* Returns true if the current import statement has the possibility of         *
* allowing symbols that weren't already covered by 'other'                    *
*                                                                             *
* Assumes that other->mod == this->mod.  Will not verify that fact.           *
*                                                                             *
************************************** | *************************************/

bool ImportStmt::providesNewSymbols(const UseStmt* other) const {
  if (other->isPlainUse()) {
    // Other is a general use, without an 'only' or 'except' list.  It covers
    // everything we could possibly cover, so we don't provide new symbols.
    return false;
  }

  if (providesQualifiedAccess() && !providesUnqualifiedAccess()) {
    // UseStmts always provide qualified access, and we don't provide any
    // symbols for unqualified access, so by definition we are covered!
    return false;
  }

  // Otherwise, we provide symbols for unqualified access, so we might provide
  // something that the limited UseStmt doesn't.
  if (other->hasExceptList()) {
    if (renamed.size() > 0) {
      // If we renamed any symbols, then we provide something new
      return true;
    }
    // If there were no renamed symbols and there's overlap between our symbols
    // and the other's except list, then we provide new symbols
    unsigned int numSame = 0;
    for_vector(const char, name, unqualified) {
      if (std::find(other->named.begin(), other->named.end(),
                    name) != other->named.end()) {
        numSame++;
      }
    }
    return numSame > 0;

  } else if (other->named.size() + other->renamed.size() < unqualified.size() +
             renamed.size()) {
    // Other has an 'only' list and it has less symbols in it than our list of
    // unqualified or renamed symbols.  By definition, this means we are
    // providing symbols not available in other.
    return true;

  } else {
    unsigned int numSame = 0;

    for_vector(const char, name, unqualified) {
      if (std::find(other->named.begin(), other->named.end(),
                    name) != other->named.end()) {
        numSame++;
      }
    }

    for(std::map<const char*, const char*>::const_iterator it = renamed.begin();
        it != renamed.end(); ++it) {
      // Don't check against other's only list.  A renamed version of
      // something in their only list is a new symbol
      // Do check against other's renamed list.  If both uses cause the exact
      // same rename to occur, we should count it.
      for (std::map<const char*, const char*>::const_iterator otherIt =
             other->renamed.begin();
           otherIt != other->renamed.end(); ++otherIt) {
        if (astr(it->first) ==  astr(otherIt->first) &&
            astr(it->second) == astr(otherIt->second)) {
          numSame++;
        }
      }
    }

    // If all of our symbols for unqualified access were in the other's 'only'
    // list, then we don't provide anything new.
    return numSame != unqualified.size() + renamed.size();
  }
}

bool ImportStmt::providesNewSymbols(const ImportStmt* other) const {
  bool hasUnqual = providesQualifiedAccess();
  bool otherHasUnqual = other->providesQualifiedAccess();

  if (hasUnqual != otherHasUnqual) {
    // One of us provides qualified access and the other provides unqualified
    // access, so we provide new symbols!
    return true;
  } else if (!hasUnqual) {
    // We both provide qualified access, so the current statement doesn't
    // provide new symbols
    return false;
  } else {
    // We both provide unqualified access to at least some of the symbols in the
    // module.  It's possible that we overlap somewhat, so check to be sure

    if (other->unqualified.size() + other->renamed.size() < unqualified.size() +
        renamed.size()) {
      // We defined more unqualified symbols than the other import, so we
      // definitely provide more
      return true;
    } else {
      unsigned int numSame = 0;

      for_vector(const char, name, unqualified) {
        if (std::find(other->unqualified.begin(), other->unqualified.end(),
                      name) != other->unqualified.end()) {
          numSame++;
        }
      }

      for(std::map<const char*, const char*>::const_iterator it =
            renamed.begin(); it != renamed.end(); ++it) {
        // Don't check against other's unqualified list.  A renamed version of
        // something in their unqualified list is a new symbol
        // Do check against other's renamed list.  If both uses cause the exact
        // same rename to occur, we should count it.
        for (std::map<const char*, const char*>::const_iterator otherIt =
               other->renamed.begin();
             otherIt != other->renamed.end(); ++otherIt) {
          if (strcmp(it->first,  otherIt->first)  == 0 &&
              strcmp(it->second, otherIt->second) == 0) {
            numSame++;
          }
        }
      }

      // If all of our provided unqualified symbols were already provided by
      // this import, then obviously we provide no new symbols.
      return numSame != unqualified.size() + renamed.size();
    }
  }
}

/************************************* | **************************************
*                                                                             *
* If the outer use statement would limit this one, return a new import of our *
* module with the additional symbols accounted for.  This new import will not *
* be added to the AST, but will be reused in scopeResolution if the same use  *
* path is followed.                                                           *
*                                                                             *
* If the outer use does not require us to alter ourself, return ourself.      *
* If the combination of the two stmts results in no new symbols being         *
* provided by this module, return NULL.                                       *
*                                                                             *
************************************** | *************************************/

ImportStmt* ImportStmt::applyOuterUse(const UseStmt* outer) {
  if (outer->isPlainUse() == true) {
    // The outer use would not modify us, return ourself.
    return this;
  } else if (outer->hasExceptList() == true) {
    // The outer use specifies an 'except' list
    // We want to check if any of the identifiers in the 'except' list are
    // specified by the 'only' list, and not place them in the new 'only' list.
    std::vector<const char*> newUnqualifiedList;

    for_vector(const char, includeMe, unqualified) {
      if (std::find(outer->named.begin(), outer->named.end(),
                    includeMe) == outer->named.end()) {

        // We didn't find this symbol in the list to exclude, so
        // add it.
        newUnqualifiedList.push_back(includeMe);
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

    if (newUnqualifiedList.size() == unqualified.size() &&
        newRenamed.size() == renamed.size()) {
      // The except list didn't cut down on our list.
      // No need to create a new ImportStmt, just return ourself.
      return this;

    } else if (newUnqualifiedList.size() == 0 && newRenamed.size() == 0) {
      // All of our list was in the 'except' list,
      // so we don't provide new symbols.
      return NULL;

    } else {
      // The list will be shorter, create a new ImportStmt with it.
      SET_LINENO(this);

      return new ImportStmt(src, isPrivate, &newUnqualifiedList, &newRenamed);
    }

  } else {
    // The outer use has an 'only' list
    // We need to narrow that list down to just the names that are in both
    // lists.

    SET_LINENO(this);

    std::vector<const char*> newUnqualifiedList;
    std::map<const char*, const char*> newRenamed;

    for_vector(const char, includeMe, outer->named) {
      if (std::find(unqualified.begin(), unqualified.end(),
                    includeMe) != unqualified.end()) {
        // We found this symbol in both lists, so add it
        // to the union of them.
        newUnqualifiedList.push_back(includeMe);

      } else {
        std::map<const char*, const char*>::iterator it =
          renamed.find(includeMe);

        if (it != renamed.end()) {
          // We found this symbol in the renamed list and the outer 'only'
          // list so add it to the new renamed list.
          newRenamed[it->first] = it->second;
        }
      }
    }

    for (std::map<const char*, const char*>::const_iterator it =
           outer->renamed.begin(); it != outer->renamed.end(); ++it) {
      if (std::find(unqualified.begin(), unqualified.end(),
                    it->second) != unqualified.end()) {
        // The old name was in our list.  We need to rename it.
        newRenamed[it->first] = it->second;
      } else {

        std::map<const char*, const char*>::const_iterator innerIt =
          renamed.find(it->second);

        if (innerIt != renamed.end()) {
          // We found this symbol in the renamed list and the outer
          // renamed list so add the outer use's new name as the key, and
          // our old name as the old name to use.
          newRenamed[it->first] = innerIt->second;
        }
      }
    }

    if (newUnqualifiedList.size() > 0 || newRenamed.size() > 0) {
      // There were symbols that were in both lists, so this module use is still
      // interesting.
      SET_LINENO(this);
      return new ImportStmt(src, isPrivate, &newUnqualifiedList, &newRenamed);

    } else {
      // all of the 'only' identifiers in the outer use
      // were missing from the inner import's list, so this
      // module use will give us nothing.
      return NULL;
    }
  }
}

ImportStmt* ImportStmt::applyOuterImport(const ImportStmt* outer) {
  if (outer->providesQualifiedAccess()) {
    // The outer import provides qualified access.

    // This assert is for if/when we switch to enabling both unqualified and
    // qualified access in a single import statement.
    INT_ASSERT(!outer->providesUnqualifiedAccess());

    // Unqualified access means we shouldn't follow the imports within this
    // scope.
    return NULL;
  } else {
    // The outer import provides unqualified access.

    if (providesQualifiedAccess()) {
      // The inner import provides qualified access.  This won't be relevant,
      // because the outer import only enables unqualified access.

      // This assert is for if/when we switch to enabling both unqualified and
      // qualified access in a single import statement.
      INT_ASSERT(!providesUnqualifiedAccess());

      return NULL;

    } else {
      // The inner import also provides unqualified access. We should see how
      // much overlap there is, if any, and reduce ourselves if necessary.

      SET_LINENO(this);

      std::vector<const char*> newUnqualifiedList;
      std::map<const char*, const char*> newRenamed;

      for_vector(const char, includeMe, outer->unqualified) {
        if (std::find(unqualified.begin(), unqualified.end(),
                      includeMe) != unqualified.end()) {
          // We found this symbol in both lists, so add it
          // to the union of them.
          newUnqualifiedList.push_back(includeMe);

        } else {
          std::map<const char*, const char*>::iterator it =
            renamed.find(includeMe);

          if (it != renamed.end()) {
            // We found this symbol in the renamed list and the outer
            // list so add it to the new renamed list.
            newRenamed[it->first] = it->second;
          }
        }
      }

      for (std::map<const char*, const char*>::const_iterator it =
             outer->renamed.begin(); it != outer->renamed.end(); ++it) {
        if (std::find(unqualified.begin(), unqualified.end(),
                      it->second) != unqualified.end()) {
          // The old name was in our list.  We need to rename it.
          newRenamed[it->first] = it->second;
        } else {

          std::map<const char*, const char*>::const_iterator innerIt =
            renamed.find(it->second);

          if (innerIt != renamed.end()) {
            // We found this symbol in the renamed list and the outer
            // renamed list so add the outer use's new name as the key, and
            // our old name as the old name to use.
            newRenamed[it->first] = innerIt->second;
          }
        }
      }

      if (newUnqualifiedList.size() > 0 || newRenamed.size() > 0) {
        // There were symbols that were in both lists, so this module use is
        // still interesting.
        SET_LINENO(this);
        return new ImportStmt(src, isPrivate, &newUnqualifiedList, &newRenamed);

      } else {
        // all of the identifiers in the outer import were missing from the
        // inner import's list, so this module import will give us nothing.
        return NULL;
      }
    }
  }
}
