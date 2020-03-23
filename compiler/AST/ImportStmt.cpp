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
                       std::vector<const char*>* namesList):
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

  gImportStmts.add(this);
}

ImportStmt* ImportStmt::copyInner(SymbolMap* map) {
  ImportStmt* _this = new ImportStmt(COPY_INT(src), modRename, isPrivate);

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

          // We want to only enable unqualified access of this particular symbol
          // in the module
          this->unqualified.push_back(sym->name);

          ModuleSymbol* parentSym = toModuleSymbol(sym->defPoint->parentSymbol);
          if (parentSym == NULL) {
            INT_ASSERT(sym->defPoint->parentSymbol != NULL);
            USR_FATAL_CONT(this, "only the last symbol in an 'import' "
                           "statement's path can be something other than a "
                           "module");
            USR_PRINT(this, "'%s' is not a module",
                      sym->defPoint->parentSymbol->name);
            USR_STOP();
          } else if (this->isARename()) {
            // This shouldn't be too hard, though.
            USR_FATAL(this, "renaming imported symbols that aren't modules is "
                      "not currently supported");
          }
          scope->enclosingModule()->moduleUseAdd(parentSym);
          updateEnclosingBlock(scope, parentSym);

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
* Determine whether the import permits us to search for a symbol with the     *
* given name.  Returns true ("should skip") if the import doesn't specify any *
* symbols for unqualified access, or if the name provided does not match any  *
* of them                                                                     *
*                                                                             *
************************************** | *************************************/

bool ImportStmt::skipSymbolSearch(const char* name) {
  // We don't define any symbols for unqualified access, so we should skip this
  // import
  if (unqualified.size() == 0) {
    return true;
  } else {
    // Otherwise, look through the list of unqualified symbol names to see if
    // this one was listed
    for_vector(const char, toCheck, unqualified) {
      if (toCheck == name)
        return false;
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
  if (unqualified.size() == 0) {
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

  if (!providesQualifiedAccess()) {
    // UseStmts always provide qualified access, and we don't provide any
    // symbols for unqualified access, so by definition we are covered!
    return false;
  }

  // Otherwise, we provide symbols for unqualified access, so we might provide
  // something that the limited UseStmt doesn't.
  if (other->hasExceptList()) {
    // If there's overlap between our symbols and the other's except list, then
    // we provide new symbols
    unsigned int numSame = 0;
    for_vector(const char, name, unqualified) {
      if (std::find(other->named.begin(), other->named.end(),
                    name) != other->named.end()) {
        numSame++;
      }
    }
    return numSame > 0;

  } else if (other->named.size() + other->renamed.size() < unqualified.size()) {
    // Other has an 'only' list and it has less symbols in it than our list of
    // unqualified symbols.  By definition, this means we are providing symbols
    // not available in other.
    return true;

  } else {
    unsigned int numSame = 0;

    for_vector(const char, name, unqualified) {
      if (std::find(other->named.begin(), other->named.end(),
                    name) != other->named.end()) {
        numSame++;
      }
    }

    // If all of our symbols for unqualified access were in the other's 'only'
    // list, then we don't provide anything new.
    return numSame != unqualified.size();
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

    if (other->unqualified.size() < unqualified.size()) {
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

      // If all of our provided unqualified symbols were already provided by
      // this import, then obviously we provide no new symbols.
      return numSame != unqualified.size();
    }
  }
}
