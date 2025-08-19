/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

//
// cleanup
//
// This pass cleans up the AST after parsing. It handles
// transformations that would be difficult to do while parsing.
//

#include "passes.h"

#include "astutil.h"
#include "build.h"
#include "CatchStmt.h"
#include "expr.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "wellknown.h"

#include "convert-help.h"

static void cleanup(ModuleSymbol* module);

static void normalizeNestedFunctionExpressions(FnSymbol* fn);

static void replaceIsSubtypeWithPrimitive(CallExpr* call,
                                          bool proper, bool coerce);
static void addIntentRefMaybeConst(ArgSymbol* arg);

static void applyAtomicTypeToPrimaryMethod(TypeSymbol* ts, FnSymbol* fn);

static void fixupVoidReturnFn(FnSymbol* fn);

void cleanup() {
  std::vector<ModuleSymbol*> mods;

  ModuleSymbol::getTopLevelModules(mods);

  for_vector(ModuleSymbol, mod, mods) {
    cleanup(mod);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void setAstHelp(Expr* parent, Expr*& lhs, Expr* rhs) {
  if(lhs){
    lhs->remove();
  }
  lhs = rhs;
  parent_insert_help(parent, lhs);
}

static void handleNonTypedAndNonInitedVar(DefExpr* def) {
  if (isVarSymbol(def->sym) && !isShadowVarSymbol(def->sym)) {
    bool needsInit = false;
    // The test for FLAG_TEMP allows compiler-generated (temporary) variables
    // to be declared without an explicit type or initializer expression.
    if ((!def->init || def->init->isNoInitExpr())
        && !def->exprType && !def->sym->hasFlag(FLAG_TEMP))
      if (isBlockStmt(def->parentExpr) && !isArgSymbol(def->parentSymbol) &&
          !isShadowVarSymbol(def->sym))
        if (def->parentExpr != rootModule->block && def->parentExpr != stringLiteralModule->block)
          if (!def->sym->hasFlag(FLAG_INDEX_VAR))
            needsInit = true;

    if (needsInit) {
      if ((def->init && def->init->isNoInitExpr()) ||
          def->sym->hasFlag(FLAG_CONFIG)) {
        if (!def->sym->hasFlag(FLAG_RESOLVED_EARLY)) {
          USR_FATAL_CONT(def->sym,
                         "Variable '%s' is not initialized and has no type",
                         def->sym->name);
        }
      } else {
        bool skip = false;
        if (FnSymbol* inFn = toFnSymbol(def->parentSymbol)) {
          if (inFn->isNormalized()) {
            skip = true;
          }
        } else if (def->sym->hasFlag(FLAG_RESOLVED_EARLY)) {
          skip = true;
        }

        if (!skip) {
          SET_LINENO(def);
          setAstHelp(def, def->init, new SymExpr(gSplitInit));
        }
      }
    }
  }
}

static void cleanup(ModuleSymbol* module) {
  std::vector<BaseAST*> asts;

  collect_asts(module, asts);

  for_vector(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (def->sym->hasFlag(FLAG_RESOLVED_EARLY)) continue;

      if (def->sym->hasFlag(FLAG_DOCS_ONLY) == true) {
        // Delete functions/variables that are for docs only
        def->remove();
      } else if (FnSymbol* fn = toFnSymbol(def->sym)) {
        SET_LINENO(def);
        if (fn->hasFlag(FLAG_COMPILER_NESTED_FUNCTION) == true) {
          normalizeNestedFunctionExpressions(fn);
        }

        if (fn->name == astrScolon) {
          int extraMethodArgs = 2*fn->isMethod(); // 2 extra args: this & _mt
          int numFormals = fn->numFormals() - extraMethodArgs;
          if (numFormals != 2) {
            USR_FATAL_CONT(fn, "cast operator should have two formals, not %d",
                           numFormals);
          } else {
            // skip 'this' argument, if method
            ArgSymbol* arg2 = fn->getFormal(2 + extraMethodArgs);
            if (arg2->intent != INTENT_TYPE &&
                !arg2->hasFlag(FLAG_TYPE_VARIABLE)) {
              USR_FATAL_CONT(arg2, "second formal for cast should have type intent");
            }
            if (arg2->typeExpr == NULL &&
                fn->getModule()->modTag != MOD_USER) {
              USR_WARN(arg2, "cast type formal should be constrained");
            }
          }
        }
      }
    }
  }

  for_vector(BaseAST, ast, asts) {
    SET_LINENO(ast);

    if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->blockTag == BLOCK_SCOPELESS && block->list != NULL) {
        // If the scopeless block is for applying GPU attributes to promoted
        // expressions, do not flatten it. The bounds of the block denote
        // where the GPU attribute is applied.
        if (!block->isGpuMetadata()) {
          block->flattenAndRemove();
        }
      }

    } else if (CallExpr* call = toCallExpr(ast)) {
      if (call->isNamed("isSubtype"))
        replaceIsSubtypeWithPrimitive(call, false, false);
      else if (call->isNamed("isProperSubtype"))
        replaceIsSubtypeWithPrimitive(call, true, false);
      else if (call->isNamed("isCoercible"))
        replaceIsSubtypeWithPrimitive(call, false, true);

    } else if (DefExpr* def = toDefExpr(ast)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        // Is this function defined within a type i.e. is it a method?
        if (TypeSymbol* ts = toTypeSymbol(fn->defPoint->parentSymbol)) {
          flattenPrimaryMethod(ts, fn);
          applyAtomicTypeToPrimaryMethod(ts, fn);
        }
        fixupVoidReturnFn(fn);
      } else {
        handleNonTypedAndNonInitedVar(def);
      }
    } else if (CatchStmt* catchStmt = toCatchStmt(ast)) {
      catchStmt->cleanup();
    } else if (ArgSymbol* arg = toArgSymbol(ast)) {
      addIntentRefMaybeConst(arg);
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Moves expressions that are parsed as nested function definitions into their *
* own statement; during parsing, these are embedded in call expressions       *
*                                                                             *
************************************** | *************************************/

static void normalizeNestedFunctionExpressions(FnSymbol* fn) {
  DefExpr* def = fn->defPoint;

  // Replace with a 'SymExpr', not a 'UnresolvedSymExpr', here. We know
  // exactly what definition this name refers to - we have it on hand!
  if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol)) {
    AggregateType* ct = toAggregateType(ts->type);
    INT_ASSERT(ct);
    def->replace(new SymExpr(fn));
    ct->addDeclarations(def);
  } else {
    Expr* stmt = def->getStmtExpr();
    def->replace(new SymExpr(fn));
    stmt->insertBefore(def);
  }
}


static void replaceIsSubtypeWithPrimitive(CallExpr* call,
                                          bool proper, bool coerce) {
  Expr* sub = call->get(1);
  Expr* sup = call->get(2);
  sub->remove();
  sup->remove();

  PrimitiveTag prim = proper ? PRIM_IS_PROPER_SUBTYPE : PRIM_IS_SUBTYPE;
  if (coerce)
    prim = PRIM_IS_COERCIBLE;

  call->replace(new CallExpr(prim, sup, sub));
}


static void addIntentRefMaybeConst(ArgSymbol* arg) {
  if (arg->hasFlag(FLAG_INTENT_REF_MAYBE_CONST_FORMAL)) {
    arg->intent = INTENT_REF_MAYBE_CONST;
  }
}

//
// If call is an empty return statement, e.g. "return;"
// then change it into a return of the 'void' symbol: "return _void;"
// and mark the function it is in with FLAG_VOID_NO_RETURN_VALUE.
//
static void insertVoidReturnSymbols(CallExpr* call) {
  INT_ASSERT(call->isPrimitive(PRIM_RETURN));

  if (call->numActuals() == 0) {
    FnSymbol* fn = call->getFunction();
    INT_ASSERT(fn);
    call->insertAtTail(gVoid);
    fn->addFlag(FLAG_VOID_NO_RETURN_VALUE);
  }
}

//
// Mark functions with no return statements and functions with only empty
// return statements with FLAG_VOID_NO_RETURN_VALUE. Change empty return
// statements to return the value '_void'.
//
static void fixupVoidReturnFn(FnSymbol* fn) {

  // Pass expandExternArrayCalls builds a wrapper for the extern function
  // and returns the value the extern function returned.  It marks the
  // extern function with FLAG_EXTERN_FN_WITH_ARRAY_ARG, which tells us
  // that we need to be able to handle the wrapper returning the result
  // of a call to it.  If the extern function had a 'void' return, treat
  // it as a void value.
  if (fn->hasFlag(FLAG_EXTERN_FN_WITH_ARRAY_ARG)) return;

  // Do not do this lowering on functions without a body.
  if (fn->hasFlag(FLAG_NO_FN_BODY)) return;

  std::vector<CallExpr*> callExprs;
  collectCallExprs(fn, callExprs);
  bool foundReturn = false;

  for_vector(CallExpr, call, callExprs) {
    if (call->isPrimitive(PRIM_RETURN)) {
      foundReturn = true;
      insertVoidReturnSymbols(call);
    }
  }
  if (!foundReturn) {
    fn->addFlag(FLAG_VOID_NO_RETURN_VALUE);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void applyAtomicTypeToPrimaryMethod(TypeSymbol* ts, FnSymbol* fn) {
  if (ts->hasFlag(FLAG_ATOMIC_TYPE)) {
    fn->addFlag(FLAG_ATOMIC_TYPE);
  }
}
