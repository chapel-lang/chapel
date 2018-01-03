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

// checkResolved.cpp

#include "passes.h"

#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "stmt.h"
#include "stlUtil.h"
#include "TryStmt.h"

#include "iterator.h"

#include <set>

// We use RefVector to store a list of symbols that are aliases for the return
// symbol being sought in isDefinedAllPaths.
// We assume that if the ref is being used then it is valid (resolution should
// ensure this).
typedef std::set<Symbol*> RefSet;

static void checkConstLoops();
static int isDefinedAllPaths(Expr* expr, Symbol* ret, RefSet& refs);
static void checkReturnPaths(FnSymbol* fn);
static void checkCalls();
static void checkExternProcs();


static void
checkConstLoops() {
  if (fWarnConstLoops == true) {
    forv_Vec(BlockStmt, block, gBlockStmts) {
      block->checkConstLoops();
    }
  }
}

void
checkResolved() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    checkReturnPaths(fn);
    if (fn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
        !fn->isIterator()) {
      IteratorInfo* ii = toAggregateType(fn->retType)->iteratorInfo;
      if (ii && ii->iterator && ii->iterator->defPoint->parentSymbol == fn)
        USR_FATAL_CONT(fn, "functions cannot return nested iterators or loop expressions");
    }
    if (fn->hasFlag(FLAG_ASSIGNOP) && fn->retType != dtVoid)
      USR_FATAL(fn, "The return value of an assignment operator must be 'void'.");
  }

  forv_Vec(TypeSymbol, type, gTypeSymbols) {
    if (EnumType* et = toEnumType(type->type)) {
      for_enums(def, et) {
        if (def->init) {
          SymExpr* sym = toSymExpr(def->init);
          if (!sym || (!sym->symbol()->hasFlag(FLAG_PARAM) &&
                       !toVarSymbol(sym->symbol())->immediate))
            USR_FATAL_CONT(def, "enumerator '%s' is not an integer param value",
                           def->sym->name);
        }
      }
    }
  }
  // check for no record deletes, no invalid PRIM_ADDR_OF
  checkCalls();
  checkConstLoops();
  checkExternProcs();
}


// Returns the smallest number of definitions of ret on any path through the
// given expression.
static int
isDefinedAllPaths(Expr* expr, Symbol* ret, RefSet& refs)
{
  if (!expr)
    return 0;

  if (isDefExpr(expr))
    return 0;

  if (isSymExpr(expr))
    return 0;

  if (CallExpr* call = toCallExpr(expr))
  {
    if (call->isResolved() &&
        call->resolvedFunction()->hasFlag(FLAG_FUNCTION_TERMINATES_PROGRAM))
      return 1;

    if (call->isPrimitive(PRIM_MOVE) ||
        call->isPrimitive(PRIM_ASSIGN))
    {
      SymExpr* lhs = toSymExpr(call->get(1));

      if (lhs->symbol() == ret)
        return 1;

      if (refs.find(lhs->symbol()) != refs.end())
        return 1;

      if (CallExpr* rhs = toCallExpr(call->get(2)))
        if (rhs->isPrimitive(PRIM_ADDR_OF))
        {
          // We expect only a SymExpr as the operand of 'addr of'.
          SymExpr* se = toSymExpr(rhs->get(1));
          if (se->symbol() == ret)
            // lhs <- ('addr of' ret)
            refs.insert(lhs->symbol());
        }
    }

    if (call->isResolved())
    {
      for_alist(e, call->argList)
      {
        if (SymExpr* se = toSymExpr(e))
        {
          ArgSymbol* arg = actual_to_formal(se);
          // If ret is passed as an out or inout argument, that's a definition.
          if (se->symbol() == ret &&
              (arg->intent == INTENT_OUT ||
               arg->intent == INTENT_INOUT ||
               arg->intent == INTENT_REF))
            return 1;

          // Treat all (non-const) refs as definitions, until we know better.
          // TODO: This may not be needed after moving insertReferenceTemps()
          // after this pass.

          // Commenting out debugging output
          //for (RefSet::iterator i = refs.begin();
          //     i != refs.end(); ++i)
          //  printf("%d\n", (*i)->id);

          if (refs.find(se->symbol()) != refs.end() &&
              arg->intent == INTENT_REF)
            return 1;
        }
      }
    }

    return 0;
  }

  if (CondStmt* cond = toCondStmt(expr))
  {
    return std::min(isDefinedAllPaths(cond->thenStmt, ret, refs),
                    isDefinedAllPaths(cond->elseStmt, ret, refs));
  }

  if (isGotoStmt(expr))
    return 0;

  // bodies of Defer statements will be moved, so treat them as
  // not defining anything
  if (isDeferStmt(expr))
    return 0;

  if (TryStmt* tryStmt = toTryStmt(expr))
    return isDefinedAllPaths(tryStmt->body(), ret, refs);

  if (BlockStmt* block = toBlockStmt(expr))
  {
    // NOAKES 2014/11/25 Transitional.  Ensure we don't call blockInfoGet()
    if (block->isWhileDoStmt()  == true ||
        block->isForLoop()      == true ||
        block->isCForLoop()     == true ||
        block->isParamForLoop() == true)
    {
      return 0;
    }

    else if (block->isDoWhileStmt() == true ||
             block->blockInfoGet()  == NULL ||
             block->blockInfoGet()->isPrimitive(PRIM_BLOCK_LOCAL))
    {
      int result = 0;

      for_alist(e, block->body)
        result += isDefinedAllPaths(e, ret, refs);

      return result;
    }

    else
    {
      return 0;
    }
  }

  if (isExternBlockStmt(expr))
    return 0;

  INT_FATAL("isDefinedAllPaths: Unhandled case.");
  return 0;
}

// This function helps when checking that we aren't returning
// a local variable by reference. In particular, it checks for two
// cases:
//   * returning a ref-intent argument by ref
//   * returning a const-ref-intent argument by const ref
//
static bool
returnsRefArgumentByRef(CallExpr* returnedCall, FnSymbol* fn)
{
  INT_ASSERT(returnedCall->isPrimitive(PRIM_ADDR_OF));
  if (SymExpr* rhs = toSymExpr(returnedCall->get(1))) {
    if (ArgSymbol* formal = toArgSymbol(rhs->symbol())) {
      IntentTag intent = formal->intent;
      if (fn->retTag == RET_CONST_REF && (intent & INTENT_FLAG_REF))
        return true;
      else if(fn->retTag == RET_REF &&
              (intent == INTENT_REF || intent == INTENT_REF_MAYBE_CONST))
        return true;
    }
  }

  return false;
}

// This function finds the original Symbol that a local array
// refers to (through aliasing or slicing).
// It returns the number of Exprs added to sources.
static int findOriginalArrays(FnSymbol*        fn,
                              Symbol*          sym,
                              std::set<Expr*>& sources) {
  int ret = 0;

  for_SymbolSymExprs(se, sym) {
    Expr* stmt = se->getStmtExpr();

    if (CallExpr* call = toCallExpr(stmt)) {
      if (call->isPrimitive(PRIM_MOVE)   == true  ||
          call->isPrimitive(PRIM_ASSIGN) == true) {
        Expr* lhs = call->get(1);

        if (se == lhs) {
          Expr* rhs = call->get(2);

          // Handle the following cases:
          //   rhs is a call_tmp -> recurse on the call_tmp
          //   rhs is a call to a function returning an aliasing array ->
          //      recurse into the source array argument
          if (SymExpr* rhsSe = toSymExpr(rhs)) {
            VarSymbol* rhsSym = toVarSymbol(rhsSe->symbol());
            // is RHS a local variable (user or temp)?
            // if so, find the definitions for that, and further
            // traverse if they are aliases.
            if (rhsSym                          != NULL &&
                rhsSym->defPoint->getFunction() ==   fn &&
                rhsSym->hasFlag(FLAG_TEMP)      == true) {
              ret += findOriginalArrays(fn, rhsSym, sources);
            }

          } else if (CallExpr* rhsCall = toCallExpr(rhs)) {
            FnSymbol* calledFn = rhsCall->resolvedFunction();
            SymExpr*  aliased  = NULL; // the array that is sliced or aliased

            if (calledFn && calledFn->hasFlag(FLAG_RETURNS_ALIASING_ARRAY)) {
              aliased = toSymExpr(rhsCall->get(1));
            }

            if (aliased) {
              int got = 0;

              if (aliased->symbol()->defPoint->getFunction() == fn) {
                // further traverse if aliased was a local variable.
                got = findOriginalArrays(fn, aliased->symbol(), sources);
              }

              if (got == 0) {
                // If we didn't find another local source array, add
                // aliased as the source.
                got = 1;
                sources.insert(aliased);
              }

              ret += got;
            }
          }
        }
      }
    }
  }

  return ret;
}

static void
checkBadLocalReturn(FnSymbol* fn, Symbol* retVar) {

  for_SymbolSymExprs(se, retVar) {
    // se is a use or def of 'ret'
    Expr* stmt = se->getStmtExpr();
    if (CallExpr* call = toCallExpr(stmt)) {
      if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
        Expr* lhs = call->get(1);
        Expr* rhs = call->get(2);
        CallExpr* rhsCall = toCallExpr(rhs);
        // is the se on the LHS of the PRIM_MOVE/PRIM_ASSIGN
        // i.e. is it a definition?
        if (se == lhs) {
          // Are we returning a variable by ref?
          if (rhsCall && rhsCall->isPrimitive(PRIM_ADDR_OF)) {
            // What variable are we returning with PRIM_ADDR_OF?
            SymExpr* ret = toSymExpr(rhsCall->get(1));
            // Check: Are we returning a local variable by ref?
            if (ret->symbol()->defPoint->getFunction() == fn &&
                !returnsRefArgumentByRef(rhsCall, fn)) {
              USR_FATAL_CONT(ret, "illegal expression to return by ref");
            } else {
              // Check: Are we returning a constant by ref?
              if (fn->retTag == RET_REF &&
                  (ret->symbol()->isConstant() ||
                   ret->symbol()->isParameter())) {
                USR_FATAL_CONT(rhs, "function cannot return constant by ref");
              }
            }
          }
        }
      }
    }
  }

  // if it's an array, collect the definitions for the
  // returned array. We need to go in to chains of
  // array slicing or aliasing as well as call_tmp copies.

  std::set<Expr*> sources;
  if (retVar->typeInfo()->symbol->hasFlag(FLAG_ARRAY)) {
    findOriginalArrays(fn, retVar, sources);

    for_set(Expr, source, sources) {
      // Check: Are we returning a slice or alias of a local variable
      // by value? (The above code should have found the case returning
      // such things by ref).
      SymExpr* rhsSe = toSymExpr(source);
      if (rhsSe &&
          isVarSymbol(rhsSe->symbol()) &&
          rhsSe->symbol()->defPoint->getFunction() == fn &&
          !rhsSe->isRef()) {
        USR_FATAL_CONT(rhsSe, "illegal return of array aliasing a local array");
      }
    }
  }
}

static void
checkReturnPaths(FnSymbol* fn) {
  // Check to see if the function returns a value.
  if (fn->isIterator() ||
      !strcmp(fn->name, "=") || // TODO: Remove this to enforce new signature.
      !strcmp(fn->name, "chpl__buildArrayRuntimeType") ||
      fn->retType == dtVoid ||
      fn->retTag == RET_TYPE ||
      fn->hasFlag(FLAG_EXTERN) ||
      fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) ||
      fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) ||
      fn->hasFlag(FLAG_AUTO_II))
    return; // No.

  // Check to see if the returned value is initialized.
  Symbol* ret = fn->getReturnSymbol();
  VarSymbol* var = toVarSymbol(ret);
  if (var)
  {
    // If it has an immediate initializer, it is initialized.
    if (var->immediate)
      return;
  }

  if (isEnumSymbol(ret))
    return;

  RefSet refs;
  int result = isDefinedAllPaths(fn->body, ret, refs);

  //
  // Issue a warning if there is a path that has zero definitions.
  //
  if (result == 0)
    USR_FATAL_CONT(fn->body, "control reaches end of function that returns a value");

  //
  // Issue an error if returning a local variable by ref.
  // Issue an error if returning a slice of a local variable.
  //
  if (!fn->hasFlag(FLAG_WRAPPER)) {
    // Add returned aliases to refs.
    checkBadLocalReturn(fn, ret);
    for_set(Symbol, alias, refs) {
      checkBadLocalReturn(fn, alias);
    }
  }
}

static void
checkBadAddrOf(CallExpr* call)
{
  if (call->isPrimitive(PRIM_ADDR_OF)) {
    // This test is turned off if we are in a wrapper function.
    FnSymbol* fn = call->getFunction();
    if (!fn->hasFlag(FLAG_WRAPPER)) {
      SymExpr* lhs = NULL;

      if (CallExpr* move = toCallExpr(call->parentExpr))
        if (move->isPrimitive(PRIM_MOVE))
          lhs = toSymExpr(move->get(1));

      //
      // check that the operand of 'addr of' is a legal lvalue.
      if (SymExpr* rhs = toSymExpr(call->get(1))) {

        bool lhsRef   = lhs && lhs->symbol()->hasFlag(FLAG_REF_VAR);
        bool lhsConst = lhs && lhs->symbol()->hasFlag(FLAG_CONST);

        bool rhsType = rhs->symbol()->hasFlag(FLAG_TYPE_VARIABLE);
        bool rhsParam = rhs->symbol()->isParameter();
        // Also detect runtime type variables
        if (rhs->symbol()->type->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE))
          rhsType = true;

        if (lhsRef && rhsType) {
          USR_FATAL_CONT(call, "Cannot set a reference to a type variable.");
        } else if (lhsRef && rhsParam) {
          USR_FATAL_CONT(call, "Cannot set a reference to a param variable.");
        } else if (lhsRef && !lhsConst) {
          if (rhs->symbol()->hasFlag(FLAG_EXPR_TEMP) ||
              rhs->symbol()->isConstant()) {
            USR_FATAL_CONT(call, "Cannot set a non-const reference to a const variable.");
          }
        }
      }
    }
  }
}


static void
checkCalls()
{
  forv_Vec(CallExpr, call, gCallExprs)
    checkBadAddrOf(call);
}


static void checkExternProcs() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!fn->hasFlag(FLAG_EXTERN))
      continue;

    for_formals(formal, fn) {
      if (formal->typeInfo() == dtString) {
        if (!fn->hasFlag(FLAG_INSTANTIATED_GENERIC)) {
          USR_FATAL_CONT(fn, "extern procedures should not take arguments of "
                             "type string, use c_string instead");
        } else {
          // This is a generic instantiation of an extern proc that is using
          // string, so we want to report the call sites causing this
          USR_FATAL_CONT(fn, "extern procedure has arguments of type string");
          forv_Vec(CallExpr, call, *fn->calledBy) {
            USR_PRINT(call, "when instantiated from here");
          }
          USR_PRINT(fn, "use c_string instead");
        }
        break;
      }
    }
  }
}


