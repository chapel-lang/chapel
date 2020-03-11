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

    } else if (Symbol* sym = scope->lookupForImport(src)) {
      SET_LINENO(this);

      if (ModuleSymbol* modSym = toModuleSymbol(sym)) {
        scope->enclosingModule()->moduleUseAdd(modSym);

        updateEnclosingBlock(scope, sym);

      } else {
        if (sym->isImmediate() == true) {
          USR_FATAL(this,
                    "'import' statements must refer to module symbols "
                    "(e.g., 'import <module>;')");

        } else if (sym->name != NULL) {
          USR_FATAL_CONT(this,
                         "'import' of non-module symbol %s",
                         sym->name);
          USR_FATAL_CONT(sym,  "Definition of symbol %s", sym->name);
          USR_STOP();

        } else {
          INT_FATAL(this, "'import' of non-module symbol");
        }
      }
    } else {
      if (UnresolvedSymExpr* import = toUnresolvedSymExpr(src)) {
        USR_FATAL(this, "Cannot find module '%s'", import->unresolved);
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
                "'import' statements must refer to module symbols "
                "(e.g., 'import <module>[.<submodule>]*;')");
    }

  } else if (SymExpr* symExpr = toSymExpr(expr)) {
    if (symExpr->symbol()->isImmediate() == false) {
      retval = true;

    } else {
      USR_FATAL(this,
                "'import' statements must refer to module symbols "
                "(e.g., 'import <module>[.<submodule>]*;')");
    }

  } else {
    INT_FATAL(this, "Unexpected import stmt");
  }

  return retval;
}
