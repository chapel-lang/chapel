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

#include "addAutoDestroyCalls.h"



#include "astutil.h"
#include "AutoDestroyScope.h"
#include "expr.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "symbol.h"

#include <vector>

static void cullForDefaultConstructor(FnSymbol* fn);

static void walkBlock(FnSymbol*         fn,
                      AutoDestroyScope* parent,
                      BlockStmt*        block);

void addAutoDestroyCalls() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) == true) {
      cullForDefaultConstructor(fn);
    }

    walkBlock(fn, NULL, fn->body);
  }
}

//
// Retain current approach for pruning auto-destroy flags in constructors.
// We do not destroy variables that are written in to the fields of the
// object being initialized.
//

static void cullForDefaultConstructor(FnSymbol* fn) {
  if (isVarSymbol(fn->getReturnSymbol()) == true) {
    std::vector<DefExpr*>        defs;

    collectDefExprs(fn, defs);

    for_vector(DefExpr, def, defs) {
      if (VarSymbol* var = toVarSymbol(def->sym)) {
        if (var->hasFlag(FLAG_INSERT_AUTO_DESTROY) == true) {
          // Look for a use in a PRIM_SET_MEMBER where the field is a record
          // type, and remove the flag. (We don't actually check that var is
          // of record type, because chpl__autoDestroy() is a NO-OP when
          // applied to all other types.
          for_SymbolUses(se, var) {
            CallExpr* call = toCallExpr(se->parentExpr);

            if (call->isPrimitive(PRIM_SET_MEMBER) == true &&
                toSymExpr(call->get(3))->symbol()  == var) {
              var->removeFlag(FLAG_INSERT_AUTO_DESTROY);
            }
          }
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Walk the statements for a function's top-level BlockStmt                    *
*                                                                             *
*   1) Collect variables that are marked to be auto destroyed                 *
*                                                                             *
*   2) Handle recursion in to nested sub blocks                               *
*                                                                             *
*   3) Insert auto destroy calls at the end of the block                      *
*                                                                             *
*      Functions with multiple return points require additional care when     *
*      traversing a function's top-level block.                               *
*                                                                             *
*      Nested return statements are implemented as one or more PRIM_MOVEs to  *
*      the return variable followed by a GOTO to the return label. The        *
*      return value may be different at each return and so the autoDestroy    *
*      sets for the locals are handled immediately before each GOTO.          *
*                                                                             *
*      Functions that accept formals with OUT intent or INOUT intent rely on  *
*      formal temps within the function.  These formal temps are written to   *
*      the final destination using compiler generated code that appears       *
*      immediately before the return statement and will be shared by every    *
*      return path. We refer to this code as the "function epilogue".         *
*                                                                             *
*      The function epilogue is informally defined as                         *
*                                                                             *
*        a) The return label for the common exit point                        *
*        b) The first compiler-generated statement to write to a formal temp  *
*                                                                             *
*      but is not formally represented in the AST. Hence we require code to   *
*      detect the start of epilogue.                                          *
*                                                                             *
* A final complication is that the current implementation may find goto       *
* statements in the middle of a BlockStmt; this is not currently removed      *
* before this phase.  To be conservative, we arrange to ignore any such       *
* dead code.  The possibility of multiple returns makes this a little         *
* convoluted for the function's top level block statement.                    *
*                                                                             *
************************************** | *************************************/

static VarSymbol*   definesAnAutoDestroyedVariable(const Expr* stmt);
static LabelSymbol* findReturnLabel(FnSymbol* fn);
static bool         isReturnLabel(const Expr*        stmt,
                                  const LabelSymbol* returnLabel);

static void walkBlock(FnSymbol*         fn,
                      AutoDestroyScope* parent,
                      BlockStmt*        block) {
  AutoDestroyScope scope(parent, block);

  LabelSymbol*     retLabel   = (parent == NULL) ? findReturnLabel(fn) : NULL;
  bool             isDeadCode = false;

  for_alist(stmt, block->body) {
    //
    // Handle the current statement
    //

    // AutoDestroy primary locals at start of function epilogue (1)
    if (isReturnLabel(stmt, retLabel) == true) {
      scope.insertAutoDestroys(fn, stmt);

    } else if (isGotoStmt(stmt) == true) {
      isDeadCode = true;

    // Be conservative about unreachable code before the epilogue
    } else if (isDeadCode == false) {
      // Collect variables that should be autoDestroyed
      if (VarSymbol* var = definesAnAutoDestroyedVariable(stmt)) {
        scope.variableAdd(var);

      // AutoDestroy primary locals at start of function epilogue (2)
      } else if (scope.handlingFormalTemps(stmt) == true) {
        scope.insertAutoDestroys(fn, stmt);

      // Recurse in to a BlockStmt (or sub-classes of BlockStmt e.g. a loop)
      } else if (BlockStmt* subBlock = toBlockStmt(stmt)) {
        walkBlock(fn, &scope, subBlock);

      // Recurse in to the BlockStmt(s) of a CondStmt
      } else if (CondStmt*  cond     = toCondStmt(stmt))  {
        walkBlock(fn, &scope, cond->thenStmt);

        if (cond->elseStmt != NULL)
          walkBlock(fn, &scope, cond->elseStmt);
      }
    }

    //
    // Handle the end of a block
    //
    if (stmt->next == NULL) {
      GotoStmt* gotoStmt = toGotoStmt(stmt);

      // The main block for a function or a simple sub-block
      if (parent == NULL || gotoStmt == NULL) {
        scope.insertAutoDestroys(fn, stmt);

      // Currently unprepared for a nested scope that ends in a goto
      } else {
        switch (gotoStmt->gotoTag) {
          case GOTO_RETURN:
          case GOTO_CONTINUE:
          case GOTO_BREAK:
            scope.insertAutoDestroys(fn, stmt);
            break;

          case GOTO_NORMAL:
          case GOTO_GETITER_END:
          case GOTO_ITER_RESUME:
          case GOTO_ITER_END:
          case GOTO_ERROR_HANDLING:
           // MDN 2016/03/18 Need to revisit these cases
           break;
        }
      }
    }
  }
}

// Is this a DefExpr that defines a variable that might be autoDestroyed?
static VarSymbol* definesAnAutoDestroyedVariable(const Expr* stmt) {
  VarSymbol* retval = NULL;

  if (const DefExpr* expr = toConstDefExpr(stmt)) {
    if (VarSymbol* var = toVarSymbol(expr->sym))
      retval = (isAutoDestroyedVariable(var) == true) ? var : NULL;
  }

  return retval;
}

//
// The return label is one of the logical markers for the start of
// the function epilogue but is not currently well marked.
//
// Identify it by inspecting any goto statements.  If there are
// multiple jumps to the return label then verify they are all
// the same.
//

static LabelSymbol* findReturnLabel(FnSymbol* fn) {
  std::vector<GotoStmt*> gotoStmts;
  BlockStmt*             block  = fn->body;
  LabelSymbol*           retval = NULL;

  collectGotoStmts(block, gotoStmts);

  for_vector(GotoStmt, gotoStmt, gotoStmts) {
    if (gotoStmt->isGotoReturn() == true) {
      LabelSymbol* target = gotoStmt->gotoTarget();

      if (retval == NULL)
        retval = target;
      else
        INT_ASSERT(retval == target);
    }
  }

  return retval;
}

// Is this the definition of the return label?
static bool isReturnLabel(const Expr* stmt, const LabelSymbol* returnLabel) {
  bool retval = false;

  if (returnLabel != NULL) {
    if (const DefExpr*  expr = toConstDefExpr(stmt)) {
      if (LabelSymbol* label = toLabelSymbol(expr->sym)) {
        retval = (label == returnLabel) ? true : false;
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* A shared utilty function for resolution.                                    *
*                                                                             *
************************************** | *************************************/

bool isAutoDestroyedVariable(Symbol* sym) {
  bool retval = false;

  if (VarSymbol* var = toVarSymbol(sym)) {
    if ((var->hasFlag(FLAG_INSERT_AUTO_DESTROY) == true &&
         var->hasFlag(FLAG_NO_AUTO_DESTROY)     == false) ||

        (var->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW) == true  &&
         var->type->symbol->hasFlag(FLAG_ITERATOR_RECORD)        == false &&
         // TODO - can we remove this isRefCountedType?
         isRefCountedType(var->type)                             == false)) {

      retval = (var->isType() == false && autoDestroyMap.get(var->type) != 0);
    }
  }

  return retval;
}

