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

ImportStmt::ImportStmt(BaseAST* source) : VisibilityStmt(E_ImportStmt) {
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

ImportStmt::ImportStmt(BaseAST* source,
                       const char* rename) : VisibilityStmt(E_ImportStmt) {

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
  ImportStmt* _this = new ImportStmt(COPY_INT(src), modRename);

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
  // 2017/05/28 The parser inserts a normalized UseStmt of ChapelBase
  if (isSymExpr(src)) {
    INT_FATAL("This should only happen for a UseStmt");

  } else if (Symbol* sym = scope->lookup(src, /*isUse=*/ true)) {
    SET_LINENO(this);

    if (ModuleSymbol* modSym = toModuleSymbol(sym)) {
      if (modSym->defPoint->parentSymbol != theProgram) {
        USR_FATAL_CONT(this, "currently unable to import nested modules");
      }
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
