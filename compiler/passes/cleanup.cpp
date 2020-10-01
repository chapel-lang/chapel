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

static void cleanup(ModuleSymbol* module);

static void normalizeNestedFunctionExpressions(FnSymbol* fn);

static void replaceIsSubtypeWithPrimitive(CallExpr* call,
                                          bool proper, bool coerce);
static void addIntentRefMaybeConst(ArgSymbol* arg);

static void flattenPrimaryMethod(TypeSymbol* ts, FnSymbol* fn);

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

static bool areMultiDefExprsInAList(AList& list) { 
   int numStmts = 0;

    for_alist(stmt, list){
      if (isDefExpr(stmt)) numStmts++;
    }
    
    return numStmts > 1;
}

static void setAstHelp(Expr* parent, Expr*& lhs, Expr* rhs) {
  if(lhs){
    lhs->remove();
  }
  lhs = rhs;
  parent_insert_help(parent, lhs);
}

static void backPropagateInFunction(BlockStmt* block) {

  Expr* init = NULL;
  DefExpr* prev = NULL;

  if (!areMultiDefExprsInAList(block->body)) return;

  SET_LINENO(block);

  VarSymbol* typeTmp = NULL;
  BlockStmt* tmpBlock = new BlockStmt();

  for_alist_backward(stmt, block->body) {
    if (DefExpr* def = toDefExpr(stmt)) {

      //1. set local variableis -- analysis
      if (def->init || def->exprType) {

        if(def->init != NULL) {
          init = def->init;
        } else {
          init = NULL;
        }

        typeTmp = NULL;

      }

      //2. update prev if necessary. Since the defExprs in a block statement are
      //iterated in a reverse order, we use prev for cases like var x, y = 1.0, so
      //that x can be initialized with 1.0 and y is initialized with the x symbol.
      if (prev != NULL && def->init == NULL && def->exprType == NULL) {
        SET_LINENO(prev);
        if(prev->exprType != NULL && typeTmp == NULL){
          typeTmp = newTemp("type_tmp");
          typeTmp->addFlag(FLAG_TYPE_VARIABLE);
          DefExpr* tmpDef = new DefExpr(typeTmp, prev->exprType->copy());
          prev->exprType->replace(new SymExpr(typeTmp));
          tmpBlock->insertAtTail(tmpDef);
        }

        if(prev->init != NULL && init != NULL){
          if (init->isNoInitExpr()){
            setAstHelp(prev, prev->init, init->copy());
          } else if (typeTmp) {
            setAstHelp(prev, prev->init, new CallExpr("chpl__readXX", new SymExpr(def->sym)));
          } else {
            setAstHelp(prev, prev->init, new SymExpr(def->sym));
          }
        }
      }
    
      //3. update def, type then init
      {
        SET_LINENO(def);
        if(typeTmp != NULL && def->exprType == NULL) {
          setAstHelp(def, def->exprType, prev->exprType->copy());
        }

        if(init != NULL && def->init == NULL){
          setAstHelp(def, def->init, init->copy());
        }
      }
      prev = def;
    }
  }

  if(tmpBlock) {
    block->insertAtHead(tmpBlock);
    tmpBlock->flattenAndRemove();
  }
}

static void backPropagate(BaseAST* ast) {
  if (BlockStmt* block = toBlockStmt(ast)) {
    //Looking for scopeless blocks containing only DefExprs
    //and end of statement markers since these are how
    //multivariable declarations are represented after parsing
    if (block->blockTag == BLOCK_SCOPELESS){
      for_alist(stmt, block->body){
        if(DefExpr* def = toDefExpr(stmt)){
          if(isVarSymbol(def->sym)) {
          } else {
            return;
          }
        } else if(isEndOfStatementMarker(stmt)){
        
        } else {
          return;
        }
      }
      backPropagateInFunction(block);
    }
  }
}

static void handleNonTypedAndNonInitedVar(DefExpr* def) {
  if (toVarSymbol(def->sym)) {
    bool needsInit = false;
    // The test for FLAG_TEMP allows compiler-generated (temporary) variables
    // to be declared without an explicit type or initializer expression.
    if ((!def->init || def->init->isNoInitExpr())
        && !def->exprType && !def->sym->hasFlag(FLAG_TEMP))
      if (isBlockStmt(def->parentExpr) && !isArgSymbol(def->parentSymbol))
        if (def->parentExpr != rootModule->block && def->parentExpr != stringLiteralModule->block)
          if (!def->sym->hasFlag(FLAG_INDEX_VAR))
            needsInit = true;

    if (needsInit) {
      if ((def->init && def->init->isNoInitExpr()) ||
          def->sym->hasFlag(FLAG_CONFIG)) {
        USR_FATAL_CONT(def->sym,
                       "Variable '%s' is not initialized and has no type",
                       def->sym->name);
      } else {
        SET_LINENO(def);
        setAstHelp(def, def->init, new SymExpr(gSplitInit));
      }
    }
  }
}

static void cleanup(ModuleSymbol* module) {
  std::vector<BaseAST*> asts;

  collect_asts(module, asts);

  for_vector(BaseAST, ast, asts) {
    backPropagate(ast);
    if (DefExpr* def = toDefExpr(ast)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        SET_LINENO(def);
        if (fn->hasFlag(FLAG_COMPILER_NESTED_FUNCTION) == true) {
          normalizeNestedFunctionExpressions(fn);
        }
      }
    }
  }

  for_vector(BaseAST, ast, asts) {
    SET_LINENO(ast);

    if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->blockTag == BLOCK_SCOPELESS && block->list != NULL) {
        block->flattenAndRemove();
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

  if (module == stringLiteralModule && !fMinimalModules) {
    // Fix calls to chpl_createStringWithLiteral to use resolved expression.
    // For compiler performance reasons, we'd like to have new_StringSymbol
    // emit calls to a resolved function; however new_StringSymbol might
    // run before that function is parsed. So fix up any literals created
    // during parsing here.
    INT_ASSERT(gChplCreateStringWithLiteral != NULL);
    const char* name = gChplCreateStringWithLiteral->name;

    for_vector(BaseAST, ast, asts) {
      if (CallExpr* call = toCallExpr(ast)) {
        if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(call->baseExpr)) {
          if (urse->unresolved == name) {
            SET_LINENO(urse);
            urse->replace(new SymExpr(gChplCreateStringWithLiteral));
          }
        }
      }
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

  if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol)) {
    AggregateType* ct = toAggregateType(ts->type);

    INT_ASSERT(ct);

    def->replace(new UnresolvedSymExpr(fn->name));

    ct->addDeclarations(def);

  } else if (isArgSymbol(def->parentSymbol)) {
    Expr* stmt = def->getStmtExpr();

    def->replace(new UnresolvedSymExpr(fn->name));

    stmt->insertBefore(def);

  } else {
    Expr* stmt = def->getStmtExpr();

    def->replace(new UnresolvedSymExpr(fn->name));

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
  std::vector<CallExpr*> callExprs;
  collectCallExprs(fn, callExprs);
  bool foundReturn = false;
  // Pass expandExternArrayCalls builds a wrapper for the extern function
  // and returns the value the extern function returned.  It marks the
  // extern function with FLAG_EXTERN_FN_WITH_ARRAY_ARG, which tells us
  // that we need to be able to handle the wrapper returning the result
  // of a call to it.  If the extern function had a 'void' return, treat
  // it as a void value.
  if (fn->hasFlag(FLAG_EXTERN_FN_WITH_ARRAY_ARG)) {
    return;
  }

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

static void flattenPrimaryMethod(TypeSymbol* ts, FnSymbol* fn) {
  Expr*    insertPoint = ts->defPoint;
  DefExpr* def         = fn->defPoint;

  while (isTypeSymbol(insertPoint->parentSymbol)) {
    insertPoint = insertPoint->parentSymbol->defPoint;
  }

  insertPoint->insertBefore(def->remove());

  if (fn->userString != NULL && fn->name != ts->name) {
    if (strncmp(fn->userString, "ref ", 4) == 0) {
      // fn->userString of "ref foo()"
      // Move "ref " before the type name so we end up with "ref Type.foo()"
      // instead of "Type.ref foo()"
      fn->userString = astr("ref ", ts->name, ".", fn->userString + 4);

    } else {
      fn->userString = astr(ts->name, ".", fn->userString);
    }
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
