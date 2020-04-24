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

#include "ParamForLoop.h"

#include "AstVisitor.h"
#include "build.h"
#include "passes.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "stringutil.h"

/************************************ | *************************************
*                                                                           *
* Factory methods for the Parser                                            *
*                                                                           *
************************************* | ************************************/

BlockStmt* ParamForLoop::buildParamForLoop(VarSymbol* indexVar,
                                           Expr*      range,
                                           BlockStmt* stmts)
{
  VarSymbol*   lowVar     = newParamVar();
  VarSymbol*   highVar    = newParamVar();
  VarSymbol*   strideVar  = newParamVar();

  LabelSymbol* breakLabel = new LabelSymbol("_breakLabel");
  LabelSymbol* continueLabel  = new LabelSymbol("_unused_continueLabel");

  CallExpr*    call       = toCallExpr(range);
  Expr*        low        = NULL;
  Expr*        high       = NULL;
  Expr*        stride     = NULL;
  Expr*        count      = NULL;

  BlockStmt*   outer      = new BlockStmt();

  if (call && call->isNamed("chpl_by"))
  {
    stride = call->get(2)->remove();
    call   = toCallExpr(call->get(1));
  }
  else
  {
    stride = new SymExpr(new_IntSymbol(1));
  }

  if (call && call->isNamed("#"))
  {
    count       = new CallExpr("chpl_compute_count_param_loop", call->get(2)->remove());
    call        = toCallExpr(call->get(1));
  }

  if(call) {
    if(count == NULL)
    {
      // high..low
      if (call->isNamed("chpl_build_bounded_range"))
      {
        low    = call->get(1)->remove();
        high   = call->get(1)->remove();
      }
    }
    else
    {
      // high..low#count
      if(call->isNamed("chpl_build_bounded_range"))
      {
        Expr* temp_low  = call->get(1)->remove();
        Expr* temp_high = call->get(1)->remove();

        // It is necessary that low is calculated first, because it also applies check for bound size.
        low  = new CallExpr("chpl_bounded_count_for_param_loop_low", temp_low->copy(), temp_high->copy(), count->copy());
        high = new CallExpr("chpl_bounded_count_for_param_loop_high", temp_low->copy(), temp_high->copy(), count);
      }
      // low..#count
      else if (call->isNamed("chpl_build_low_bounded_range"))
      {
        low  = call->get(1)->remove();
        high = new CallExpr("chpl_high_bound_count_for_param_loop", low->copy(), count);
      }
      // ..high#count
      else if (call->isNamed("chpl_build_high_bounded_range"))
      {
        high = call->get(1)->remove();
        low  = new CallExpr("chpl_low_bound_count_for_param_loop", high->copy(), count);
      }
    }
  }

  // When no case is satisfied, then show error
  if(high == NULL || low == NULL)
  {
    USR_FATAL(range, "param for-loops currently only support range expressions with well-defined param integral bounds");
  }

  outer->insertAtTail(new DefExpr(indexVar, new_IntSymbol((int64_t) 0)));

  outer->insertAtTail(new DefExpr(lowVar));
  // Allows for proper coercion rules for param loops, and eliminates types like
  // string from consideration (which had caused internal errors in the past)
  CallExpr* computeLow = new CallExpr("chpl_compute_low_param_loop_bound", low,
                                      high);
  outer->insertAtTail(new CallExpr(PRIM_MOVE, lowVar, computeLow));

  outer->insertAtTail(new DefExpr(highVar));
  // Allows for proper coercion rules for param loops, and eliminates types like
  // string from consideration (which had caused internal errors in the past)
  CallExpr* computeHigh = new CallExpr("chpl_compute_high_param_loop_bound",
                                       low->copy(), high->copy());
  outer->insertAtTail(new CallExpr(PRIM_MOVE, highVar, computeHigh));

  outer->insertAtTail(new DefExpr(strideVar));
  outer->insertAtTail(new CallExpr(PRIM_MOVE, strideVar, stride));

  outer->insertAtTail(new ParamForLoop(indexVar,
                                       lowVar,
                                       highVar,
                                       strideVar,
                                       continueLabel,
                                       breakLabel,
                                       stmts));

  // this continueLabel will be replaced by a per-iteration one.
  outer->insertAtTail(new DefExpr(continueLabel));
  outer->insertAtTail(new DefExpr(breakLabel));

  return buildChapelStmt(outer);
}

VarSymbol* ParamForLoop::newParamVar()
{
  VarSymbol* retval = newTemp();

  retval->addFlag(FLAG_MAYBE_PARAM);

  return retval;
}

/************************************ | *************************************
*                                                                           *
* Instance methods                                                          *
*                                                                           *
************************************* | ************************************/

ParamForLoop::ParamForLoop() : LoopStmt(0)
{
  mResolveInfo = 0;
}

ParamForLoop::ParamForLoop(VarSymbol*   indexVar,
                           VarSymbol*   lowVar,
                           VarSymbol*   highVar,
                           VarSymbol*   strideVar,
                           LabelSymbol* continueLabel,
                           LabelSymbol* breakLabel,
                           BlockStmt*   initBody) : LoopStmt(initBody)
{
  continueLabelSet(continueLabel);
  breakLabelSet(breakLabel);

  mResolveInfo = new CallExpr(PRIM_BLOCK_PARAM_LOOP,
                              indexVar,
                              lowVar,
                              highVar,
                              strideVar);
}

ParamForLoop::~ParamForLoop()
{

}

ParamForLoop* ParamForLoop::copy(SymbolMap* mapRef, bool internal)
{
  SymbolMap     localMap;
  SymbolMap*    map       = (mapRef != 0) ? mapRef : &localMap;
  ParamForLoop* retval    = new ParamForLoop();

  retval->astloc         = astloc;
  retval->blockTag       = blockTag;
  retval->mBreakLabel    = mBreakLabel;
  retval->mContinueLabel = mContinueLabel;

  if (mResolveInfo != 0)
    retval->mResolveInfo = mResolveInfo->copy(map, true);

  for_alist(expr, body)
    retval->insertAtTail(expr->copy(map, true));

  if (internal == false)
    update_symbols(retval, map);

  return retval;
}

bool ParamForLoop::isParamForLoop() const
{
  return true;
}

SymExpr* ParamForLoop::indexExprGet() const
{
  SymExpr* retval = 0;

  if (mResolveInfo != 0)
  {
    retval = toSymExpr(mResolveInfo->get(1));

    INT_ASSERT(retval);
  }

  return retval;
}

SymExpr* ParamForLoop::lowExprGet() const
{
  SymExpr* retval = 0;

  if (mResolveInfo != 0)
  {
    retval = toSymExpr(mResolveInfo->get(2));

    INT_ASSERT(retval);
  }

  return retval;
}

SymExpr* ParamForLoop::highExprGet() const
{
  SymExpr* retval = 0;

  if (mResolveInfo != 0)
  {
    retval = toSymExpr(mResolveInfo->get(3));

    INT_ASSERT(retval);
  }

  return retval;
}

SymExpr* ParamForLoop::strideExprGet() const
{
  SymExpr* retval = 0;

  if (mResolveInfo != 0)
  {
    retval = toSymExpr(mResolveInfo->get(4));

    INT_ASSERT(retval);
  }

  return retval;
}

CallExpr* ParamForLoop::resolveInfo() const
{
  return mResolveInfo;
}

CallExpr* ParamForLoop::blockInfoGet() const
{
  printf("Migration: ParamForLoop   %12d Unexpected call to blockInfoGet()\n", id);

  return 0;
}

CallExpr* ParamForLoop::blockInfoSet(CallExpr* expr)
{
  printf("Migration: ParamForLoop   %12d Unexpected call to blockInfoSet()\n", id);

  return 0;
}

BlockStmt* ParamForLoop::copyBody(SymbolMap* map)
{
  BlockStmt* retval = new BlockStmt();

  retval->astloc        = astloc;
  retval->blockTag      = blockTag;

  for_alist(expr, body)
    retval->insertAtTail(expr->copy(map, true));

  update_symbols(retval, map);

  return retval;
}

void ParamForLoop::accept(AstVisitor* visitor)
{
  if (visitor->enterParamForLoop(this) == true)
  {
    if (indexExprGet() != 0)
      indexExprGet()->accept(visitor);

    if (lowExprGet() != 0)
      lowExprGet()->accept(visitor);

    if (highExprGet() != 0)
      highExprGet()->accept(visitor);

    if (strideExprGet() != 0)
      strideExprGet()->accept(visitor);

    for_alist(next_ast, body)
      next_ast->accept(visitor);

    if (useList)
      useList->accept(visitor);

    if (byrefVars)
      byrefVars->accept(visitor);

    visitor->exitParamForLoop(this);
  }
}

void ParamForLoop::verify()
{
  BlockStmt::verify();

  if (mResolveInfo              == NULL)
    INT_FATAL(this, "ParamForLoop::verify. mResolveInfo is NULL");

  if (BlockStmt::blockInfoGet() != NULL)
    INT_FATAL(this, "ParamForLoop::verify. blockInfo is not NULL");

  if (useList                   != NULL)
    INT_FATAL(this, "ParamForLoop::verify. useList   is not NULL");

  if (byrefVars                 != NULL)
    INT_FATAL(this, "ParamForLoop::verify. byrefVars is not NULL");
}

GenRet ParamForLoop::codegen()
{
  GenRet ret;

  INT_FATAL(this, "ParamForLoop::codegen This should be unreachable");

  return ret;
}

//
// The following functions support function resolution.

// The first two functions support a post-order iteration over the AST.
// It is important that the "loop header" is traversed before the body
// could be visited.
//
// The second two functions are used to unroll the body of the loop
// and then replace the loop with a NOP.
//

Expr* ParamForLoop::getFirstExpr()
{
  INT_ASSERT(mResolveInfo != 0);

  return mResolveInfo->getFirstExpr();
}

Expr* ParamForLoop::getNextExpr(Expr* expr)
{
  Expr* retval = this;

  if (expr == mResolveInfo && body.head != NULL)
    retval = body.head->getFirstExpr();

  return retval;
}


/* This function copies the body of a param for loop while
   adjusting it slightly - to stamp out each iteration.

   * Inserts the body before the expression beforeHere
   * i should be a loop variable index (used to label iterations)
   * Assumes that map already contains the mapping redefining
     the index variable.
   * continueSym is the symbol for the loop's continue label.
     This function will replace that with a new continue label
     local to this iteration.
*/
static void copyBodyHelper(Expr* beforeHere, int64_t i,
                           SymbolMap* map, ParamForLoop* loop,
                           Symbol* continueSym)
{
  // Replace the continue label with a per-iteration label
  // that is at the end of that iteration.
  LabelSymbol* continueLabel = new
    LabelSymbol(astr("_continueLabel", istr(i)));
  Expr* defContinueLabel = new DefExpr(continueLabel);

  beforeHere->insertBefore(defContinueLabel);

  map->put(continueSym, continueLabel);

  defContinueLabel->insertBefore(loop->copyBody(map));
}

CallExpr* ParamForLoop::foldForResolve()
{
  SymExpr*   idxExpr   = indexExprGet();
  SymExpr*   lse       = lowExprGet();
  SymExpr*   hse       = highExprGet();
  SymExpr*   sse       = strideExprGet();

  VarSymbol* lvar      = toVarSymbol(lse->symbol());
  VarSymbol* hvar      = toVarSymbol(hse->symbol());
  VarSymbol* svar      = toVarSymbol(sse->symbol());

  CallExpr*  noop      = new CallExpr(PRIM_NOOP);

  validateLoop(lvar, hvar, svar);

  Symbol*      idxSym  = idxExpr->symbol();
  Symbol*      continueSym = continueLabelGet();
  Type*        idxType = indexType();
  IF1_int_type idxSize = (is_bool_type(idxType) || get_width(idxType) == 32)
                           ? INT_SIZE_32 : INT_SIZE_64;

  // Insert an "insertion marker" for loop unrolling
  insertAfter(noop);

  bool emptyLoop = true;

  if (is_int_type(idxType))
  {
    int64_t low    = lvar->immediate->to_int();
    int64_t high   = hvar->immediate->to_int();
    int64_t stride = svar->immediate->to_int();

    if (stride <= 0)
    {
      for (int64_t i = high; i >= low; i += stride)
      {
        SymbolMap map;

        map.put(idxSym, new_IntSymbol(i, idxSize));
        copyBodyHelper(noop, i, &map, this, continueSym);
        emptyLoop = false;
      }
    }
    else
    {
      for (int64_t i = low; i <= high; i += stride)
      {
        SymbolMap map;

        map.put(idxSym, new_IntSymbol(i, idxSize));
        copyBodyHelper(noop, i, &map, this, continueSym);
        emptyLoop = false;
      }
    }
  }
  else
  {
    INT_ASSERT(is_uint_type(idxType) || is_bool_type(idxType));

    uint64_t low    = lvar->immediate->to_uint();
    uint64_t high   = hvar->immediate->to_uint();
    int64_t  stride = svar->immediate->to_int();

    if (stride <= 0)
    {
      for (uint64_t i = high; i >= low; i += stride)
      {
        SymbolMap map;

        if (is_bool_type(idxType)) {
          map.put(idxSym, new_BoolSymbol(i, BOOL_SIZE_SYS));
        } else {
          map.put(idxSym, new_UIntSymbol(i, idxSize));
        }

        copyBodyHelper(noop, i, &map, this, continueSym);
        emptyLoop = false;
      }
    }
    else
    {
      for (uint64_t i = low; i <= high; i += stride)
      {
        SymbolMap map;

        if (is_bool_type(idxType)) {
          map.put(idxSym, new_BoolSymbol(i, BOOL_SIZE_SYS));
        } else {
          map.put(idxSym, new_UIntSymbol(i, idxSize));
        }

        copyBodyHelper(noop, i, &map, this, continueSym);
        emptyLoop = false;
      }
    }
  }

  if (emptyLoop)
    addMentionToEndOfStatement(this, NULL);

  // Remove the "insertion marker"
  noop->remove();

  // Replace the paramLoop with the NO-OP
  replace(noop);

  return noop;
}

// Checks things like bounding and paramness of the range.
// The calls to chpl_compute_low_param_loop_bound and
// chpl_compute_high_param_loop_bound in buildParamForLoop should ensure that
// the appropriate type is used for these loops
void ParamForLoop::validateLoop(VarSymbol* lvar,
                                VarSymbol* hvar,
                                VarSymbol* svar) {
  if (!lvar            || !hvar            || !svar)
    USR_FATAL(this,
              "param for-loops must be defined over a bounded param range");

  if (!lvar->immediate || !hvar->immediate || !svar->immediate)
    USR_FATAL(this,
              "param for-loops must be defined over a bounded param range");

  if (!is_int_type(svar->type) && !is_uint_type(svar->type)) {
    USR_FATAL(this, "Range stride must be an int");
  }
}

//
// Determine the index type for a ParamForLoop.
//
// This implementation creates a range with low/high values and then
// asks for its type.
//
Type* ParamForLoop::indexType()
{
  SymExpr*  lse     = lowExprGet();
  SymExpr*  hse     = highExprGet();
  CallExpr* range    = new CallExpr("chpl_build_bounded_range",
                                    lse->copy(),
                                    hse->copy());
  Type*     idxType = 0;

  insertBefore(range);

  resolveCall(range);

  if (FnSymbol* sym = range->resolvedFunction())
  {
    resolveSignature(sym);

    DefExpr* formal = toDefExpr(sym->formals.get(1));

    if (toArgSymbol(formal->sym)->typeExpr != NULL) {
      idxType = toArgSymbol(formal->sym)->typeExpr->body.tail->typeInfo();
    } else {
      idxType = formal->sym->type;
    }

    range->remove();
  }

  else
  {
    INT_FATAL("unresolved range");
  }

  return idxType;
}
