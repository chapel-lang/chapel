/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#include "passes.h"

#include "addAutoDestroyCalls.h"
#include "astutil.h"
#include "AstVisitorTraverse.h"
#include "errorHandling.h"
#include "DecoratedClassType.h"
#include "ForallStmt.h"
#include "iterator.h"
#include "lateConstCheck.h"
#include "lifetime.h"
#include "optimizations.h"
#include "postFold.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "resolveIntents.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "symbol.h"
#include "virtualDispatch.h"

#include <vector>
#include <algorithm>
#include <iterator>

/************************************* | **************************************
*                                                                             *
* Noakes 2015/11/23                                                           *
*                                                                             *
* The transformation beginning at returnRecordsByReferenceArguments() locates *
* a subset of the calls to functions that return a record-like type by value  *
* and creates a clone of the function that                                    *
*                                                                             *
*   a) adds a formal with ref-intent                                          *
*   b) has return type void                                                   *
*   c) integrates a small amount of code past the call site in to the tail    *
*      of the cloned function.                                                *
*                                                                             *
* and then adjusts the neighborhood near the call-site appropriately.         *
*                                                                             *
* It has been determined that this transformation should be applied uniformly *
* to record-like types.  This new implementation follows the theme of the     *
* the current transformation but                                              *
*                                                                             *
*   1) Modifies the function rather than creating a clone                     *
*   2) Does not fold any other code in to the tail                            *
*                                                                             *
* This implementation should be broadly applicable to record-like types.      *
*                                                                             *
************************************** | *************************************/

//
// Capture a function and all of the calls to it
//
class ReturnByRef
{
  //
  // Class interface
  //
public:
  static void             apply();

private:
  typedef std::map<int, ReturnByRef*> RefMap;

  static void             returnByRefCollectCalls(RefMap& calls);
  static bool             isTransformableFunction(FnSymbol* fn);
  static void             transformFunction(FnSymbol* fn);
  static ArgSymbol*       addFormal(FnSymbol* fn);
  static void             insertAssignmentToFormal(FnSymbol*  fn,
                                                   ArgSymbol* formal);
  static void             updateAssignmentsFromRefArgToValue(FnSymbol* fn);
  static void             updateAssignmentsFromRefTypeToValue(FnSymbol* fn);
  static void             updateAssignmentsFromModuleLevelValue(FnSymbol* fn);
  static void             updateReturnStatement(FnSymbol* fn);
  static void             updateReturnType(FnSymbol* fn);

  //
  // Instance interface
  //
private:

                          ReturnByRef(FnSymbol* fn);
                          ReturnByRef();

  void                    addCall(CallExpr* call);

  void                    transform();
  void                    transformMove(CallExpr* moveExpr);

  FnSymbol*               mFunction;
  std::vector<CallExpr*>  mCalls;
};

void ReturnByRef::apply()
{
  RefMap           map;
  RefMap::iterator iter;

  returnByRefCollectCalls(map);

  for (iter = map.begin(); iter != map.end(); iter++)
    iter->second->transform();

  for (int i = 0; i < virtualMethodTable.n; i++)
  {
    if (virtualMethodTable.v[i].key)
    {
      int  numFns = virtualMethodTable.v[i].value->n;

      for (int j = 0; j < numFns; j++)
      {
        FnSymbol* fn = virtualMethodTable.v[i].value->v[j];

        if (isTransformableFunction(fn))
          transformFunction(fn);
      }
    }
  }
}

//
// Collect functions that should be converted to return by ref
// and all calls to these functions.
//

void ReturnByRef::returnByRefCollectCalls(RefMap& calls)
{
  RefMap::iterator iter;

  forv_Vec(CallExpr, call, gCallExprs)
  {
    // Only transform calls that are still in the AST tree
    // (defer statement bodies have been removed at this point
    //  in this pass)
    if (call->inTree()) {

      // Only transform calls to transformable functions
      // The common case is a user-level call to a resolved function
      // Also handle the PRIMOP for a virtual method call
      if (FnSymbol* fn = call->resolvedOrVirtualFunction()) {
       if (isTransformableFunction(fn)) {
        RefMap::iterator iter = calls.find(fn->id);
        ReturnByRef*     info = NULL;

        if (iter == calls.end())
        {
          info          = new ReturnByRef(fn);
          calls[fn->id] = info;
        }
        else
        {
          info          = iter->second;
        }

        info->addCall(call);
       }
      }
    }
  }
}

bool ReturnByRef::isTransformableFunction(FnSymbol* fn)
{
  bool retval = false;

  if (AggregateType* type = toAggregateType(fn->retType))
  {
    if      (fn->hasFlag(FLAG_INIT_COPY_FN) == true)
      retval = false;

    else if (fn->hasFlag(FLAG_AUTO_COPY_FN) == true)
      retval = false;

    // Function is an iterator "helper", e.g. getValue
    // lowerIterators should make sure that getValue returns an "owned" record.
    else if (fn->hasFlag(FLAG_AUTO_II)      == true)
      retval = false;

    // Can't transform extern functions
    else if (fn->hasFlag(FLAG_EXTERN)       == true)
      retval = false;

    else if (typeNeedsCopyInitDeinit(type)  == true)
      retval = true;

    else
      retval = false;
  }

  // Task functions within iterators can yield and so also need
  // this transformation.
  // Reasonable alternative: update insertCopiesForYields to handle
  // yielding a PRIM_DEREF or yielding a reference argument.
  if (fn->hasFlag(FLAG_TASK_FN_FROM_ITERATOR_FN)) {
    if (typeNeedsCopyInitDeinit(fn->iteratorInfo->yieldedType))
      retval = true;
  }

  return retval;
}

void ReturnByRef::transformFunction(FnSymbol* fn)
{
  ArgSymbol* formal = NULL;

  // Do nothing if we already transformed this function
  if (fn->hasFlag(FLAG_FN_RETARG))
    return;

  if (fn->hasFlag(FLAG_TASK_FN_FROM_ITERATOR_FN) == false) {
    formal = addFormal(fn);
  }

  if (fn->hasFlag(FLAG_ITERATOR_FN) == false && formal != NULL) {
    insertAssignmentToFormal(fn, formal);
  }

  updateAssignmentsFromRefArgToValue(fn);
  updateAssignmentsFromRefTypeToValue(fn);
  updateAssignmentsFromModuleLevelValue(fn);

  if (formal != NULL) {
    updateReturnStatement(fn);
    updateReturnType(fn);
  }

  // Also transform coerceMove if we transform coerceCopy
  // (since later in callDestructors we might replace coerceCopy with Move)
  if (fn->name == astr_coerceCopy)
    if (FnSymbol* coerceMove = getCoerceMoveFromCoerceCopy(fn))
      transformFunction(coerceMove);
}

ArgSymbol* ReturnByRef::addFormal(FnSymbol* fn)
{
  SET_LINENO(fn);

  Type*          type    = fn->retType;
  AggregateType* refType = type->refType;
  // Note: other code does strcmps against the name _retArg
  ArgSymbol*     formal  = new ArgSymbol(INTENT_REF, "_retArg", refType);
  formal->addFlag(FLAG_RETARG);

  fn->insertFormalAtTail(formal);
  fn->addFlag(FLAG_FN_RETARG);

  return formal;
}

void ReturnByRef::insertAssignmentToFormal(FnSymbol* fn, ArgSymbol* formal)
{
  Expr*     returnPrim  = fn->body->body.tail;

  SET_LINENO(returnPrim);

  CallExpr* returnCall  = toCallExpr(returnPrim);
  Expr*     returnValue = returnCall->get(1)->remove();
  CallExpr* moveExpr    = new CallExpr(PRIM_ASSIGN, formal, returnValue);
  Expr*     expr        = returnPrim;

  // Walk backwards while the previous element is an autoDestroy call
  while (expr->prev != NULL) {
    bool stop = true;

    if (CallExpr* call = toCallExpr(expr->prev)) {
      if (FnSymbol* calledFn = call->resolvedFunction()) {
        if (calledFn->hasFlag(FLAG_AUTO_DESTROY_FN)) {
          stop = false;
        }
      }
    }

    if (stop == true) {
      break;
    }

    expr = expr->prev;
  }

  Expr* returnOrFirstAutoDestroy = expr;

  // Add the move to return before the first autoDestroy
  // At this point we could also invoke some other function
  // if that turns out to be necessary. It might well be
  // necessary in order to return array slices by value.
  returnOrFirstAutoDestroy->insertBefore(moveExpr);
}

//
// Consider a function that takes a formal of type Record by const ref
// and that returns that value from the function.  The compiler inserts
// a PRIM_MOVE operation.
//
// This work-around inserts an autoCopy to compensate
//
void ReturnByRef::updateAssignmentsFromRefArgToValue(FnSymbol* fn)
{
  std::vector<CallExpr*> callExprs;

  collectCallExprs(fn, callExprs);

  for (size_t i = 0; i < callExprs.size(); i++)
  {
    CallExpr* move = callExprs[i];

    if (move->isPrimitive(PRIM_MOVE) ||
        move->isPrimitive(PRIM_ASSIGN))
    {
      SymExpr* lhs = toSymExpr(move->get(1));
      SymExpr* rhs = toSymExpr(move->get(2));

      if (lhs != NULL && rhs != NULL)
      {
        // check if the lhs is actually the return/yield symbol
        if(lhs->symbol()->hasFlag(FLAG_RVV) ||
           lhs->symbol()->hasFlag(FLAG_YVV)) {
          VarSymbol* symLhs = toVarSymbol(lhs->symbol());
          ArgSymbol* symRhs = toArgSymbol(rhs->symbol());

          if (symLhs != NULL && symRhs != NULL)
          {
            // check if rhs is actually a formal
            bool rhsIsFormal = false;
            for_formals(formal, fn) {
              if(formal == symRhs) {
                rhsIsFormal = true;
                break;
              }
            }
            if (rhsIsFormal &&
                typeNeedsCopyInitDeinit(symLhs->type) &&
                symRhs->type == symLhs->type)
            {
              bool fromInIntent =
                (symRhs->originalIntent == INTENT_IN ||
                 symRhs->originalIntent == INTENT_CONST_IN);

              if (symLhs->hasFlag(FLAG_ARG_THIS)   == false &&
                  symLhs->hasFlag(FLAG_NO_COPY)    == false &&
                  !fromInIntent &&
                  (symRhs->intent == INTENT_REF ||
                   symRhs->intent == INTENT_CONST_REF))
              {
                SET_LINENO(move);

                CallExpr* autoCopy = NULL;

                rhs->remove();
                autoCopy = new CallExpr(getAutoCopyForType(symRhs->type),
                                        rhs,
                                        new SymExpr(gFalse));
                move->insertAtTail(autoCopy);
              }
            }
          }
        }
      }
    }
  }
}

//
// Consider a function that assigns a class-ref wrapper for a record to
// a value of that record type.  The compiler represents this as a
//
//      move dst PRIM_DEREF(src)
//
// but fails to insert the required autoCopy.
//
// This transformation adds a move/autoCopy statement immediately after
// the targeted statement.  The <dst> symbol is updated in place in the
// new statement
//
//

void ReturnByRef::updateAssignmentsFromRefTypeToValue(FnSymbol* fn)
{
  std::vector<CallExpr*> callExprs;

  collectCallExprs(fn, callExprs);

  for (size_t i = 0; i < callExprs.size(); i++)
  {
    CallExpr* move = callExprs[i];

    if (move->isPrimitive(PRIM_MOVE) || move->isPrimitive(PRIM_ASSIGN))
    {
      SymExpr*  symLhs  = toSymExpr (move->get(1));
      CallExpr* callRhs = toCallExpr(move->get(2));

      if (symLhs && callRhs && callRhs->isPrimitive(PRIM_DEREF))
      {
        VarSymbol* varLhs = toVarSymbol(symLhs->symbol());
        SymExpr*  exprRhs = toSymExpr(callRhs->get(1));
        Symbol*    symRhs = exprRhs->symbol();

        if (varLhs != NULL && symRhs != NULL)
        {
          INT_ASSERT(varLhs->isRef() == false && symRhs->isRef());
          if (typeNeedsCopyInitDeinit(varLhs->type) &&
              !varLhs->hasFlag(FLAG_NO_COPY))
          {

            // HARSHBARGER 2015-12-11:
            // `init_untyped_var` in the `normalize` pass may insert an
            // initCopy, which means that we should not insert an autocopy
            // for that same variable.
            //
            // A chpl__unref call may be inserted to implement copy-out
            // semantics for the returning of arrays.
            bool isCopied = false;

            for_SymbolUses(use, varLhs) {
              if (CallExpr* call = toCallExpr(use->parentExpr)) {
                if (FnSymbol* parentFn = call->resolvedFunction()) {
                  if (parentFn->hasFlag(FLAG_INIT_COPY_FN) ||
                      parentFn->hasFlag(FLAG_UNREF_FN)) {
                    isCopied = true;
                    break;
                  }
                }
              }
            }

            if (!isCopied) {
              SET_LINENO(move);

              FnSymbol* copyFn = getAutoCopyForType(varLhs->type);

              callRhs->remove();
              CallExpr* copyCall = new CallExpr(copyFn, exprRhs,
                                                new SymExpr(gFalse));
              move->insertAtTail(copyCall);
            }
          }
        }
      }
    }
  }
}

//
// Consider a function that returns a module-level variable as the value
// for the function.  The compiler inserts a PRIM_MOVE operation.
//
// This work-around inserts an autoCopy to compensate
//
void ReturnByRef::updateAssignmentsFromModuleLevelValue(FnSymbol* fn)
{
  std::vector<CallExpr*> callExprs;

  collectCallExprs(fn, callExprs);

  for (size_t i = 0; i < callExprs.size(); i++)
  {
    CallExpr* move = callExprs[i];

    if (move->isPrimitive(PRIM_MOVE) || move->isPrimitive(PRIM_ASSIGN))
    {
      SymExpr* lhs = toSymExpr(move->get(1));
      SymExpr* rhs = toSymExpr(move->get(2));

      if (lhs != NULL && rhs != NULL)
      {
        VarSymbol* symLhs = toVarSymbol(lhs->symbol());
        VarSymbol* symRhs = toVarSymbol(rhs->symbol());

        if (symLhs != NULL && symRhs != NULL)
        {
          if (typeNeedsCopyInitDeinit(symLhs->type) &&
              !symLhs->hasFlag(FLAG_NO_COPY) &&
              symRhs->type == symLhs->type)
          {
            DefExpr* def = symRhs->defPoint;

            if (isModuleSymbol(def->parentSymbol) == true &&
                def->parentSymbol                 != rootModule)
            {
              SET_LINENO(move);

              CallExpr* autoCopy = NULL;

              rhs->remove();
              autoCopy = new CallExpr(getAutoCopyForType(symRhs->type),
                                      rhs, new SymExpr(gFalse));
              move->insertAtTail(autoCopy);
            }
          }
        }
      }
    }
  }
}

void ReturnByRef::updateReturnStatement(FnSymbol* fn)
{
  Expr* returnPrim  = fn->body->body.tail;

  SET_LINENO(returnPrim);

  returnPrim->replace(new CallExpr(PRIM_RETURN, gVoid));
}

void ReturnByRef::updateReturnType(FnSymbol* fn)
{
  fn->retType = dtVoid;
}

ReturnByRef::ReturnByRef(FnSymbol* fn)
{
  mFunction = fn;
}

void ReturnByRef::addCall(CallExpr* call)
{
  mCalls.push_back(call);
}

void ReturnByRef::transform()
{
  // Transform all of the call sites
  for (size_t i = 0; i < mCalls.size(); i++)
  {
    CallExpr* call   = mCalls[i];
    Expr*     parent = call->parentExpr;

    if (CallExpr* parentCall = toCallExpr(parent))
    {
      if (parentCall->isPrimitive(PRIM_MOVE)   ||
          parentCall->isPrimitive(PRIM_ASSIGN) )
        transformMove(parentCall);
      else
        INT_ASSERT(false);

    } else if (isForallIterExpr(call)) {
      SET_LINENO(call);
      call->insertAtTail(gDummyRef);

    } else {
      // task functions within iterators can yield
      // but technically return void
      //INT_ASSERT(false);
    }
  }

  // Then update the function
  transformFunction(mFunction);
}

// Check for a =, PRIM_MOVE, or PRIM_ASSIGN
// with a RHS that is marked with FLAG_FORMAL_TEMP_OUT_CALLSITE.
// This reflects the writeback pattern added at the callsite
// for out/inout formals (see wrappers.cpp).
static bool isFormalTmpWriteback(Expr* e) {
  if (CallExpr* call = toCallExpr(e)) {
    if (call->isNamedAstr(astrSassign) ||
        call->isPrimitive(PRIM_MOVE) ||
        call->isPrimitive(PRIM_ASSIGN)) {
      int nActuals = call->numActuals();
      if (nActuals >= 2) {
        // check if the last argument has the appropriate flag.
        if (SymExpr* se = toSymExpr(call->get(nActuals))) {
          if (se->symbol()->hasFlag(FLAG_FORMAL_TEMP_OUT_CALLSITE))
            return true;
        }
      }
    }
  }
  return false;
}

//
// Transform a call to a function that returns a record to be a call
// to a revised function that does not return a value and that accepts
// a reference to the destination i.e.
//
// replace
//
//     move dst func(a, b, c)
//
// with
//
//     define tmp;
//     define ref;
//
//     ref = &tmp;
//     func(a, b, c, ref);
//     dst = tmp;
//
// A tmp is created and then assigned to 'dst' to clarify that this is
// a def of 'dst'.  It is currently less clear to the compiler that this
// is also a def of 'tmp'.
//
// This is particularly important for replication of const module level
// variables (findHeapVarsAndRefs() and for wide-pointer analysis
//
// In some cases the statement after the move is another move with a RHS
// that performs a superfluous initCopy/autoCopy.  If so reduce to a simple
// move.  The called-function is responsible for performing a copy when needed.
//
//
//
//
// Noakes 2017/03/04: The implementation of deadStringLiteralElimination()
// is currently coupled to the details of this transformation
//
//    a) Assigning variables by reference currently confuses def-use
//       analysis.  This is one of the motivations for inserting a tmp
//
//    b) deadStringLiteralElimination is intended to remove the AST that
//       is used to initialize a string.  That code is modified by this
//       transformation.
//

void ReturnByRef::transformMove(CallExpr* moveExpr)
{
  SET_LINENO(moveExpr);

  Expr*     lhs       = moveExpr->get(1);

  CallExpr* callExpr  = toCallExpr(moveExpr->get(2));
  FnSymbol* fn        = callExpr->resolvedFunction();

  Expr*     nextExpr  = moveExpr->next;

  // Ignore a CondStmt containing a PRIM_CHECK_ERROR
  // so that we can still detect initCopy after a call that can throw
  //
  // Also ignore a DefExpr which might e.g. define a user variable
  // which is = initCopy(call_tmp).
  while (nextExpr &&
         (isCheckErrorStmt(nextExpr) || isDefExpr(nextExpr) ||
          isFormalTmpWriteback(nextExpr)))
    nextExpr = nextExpr->next;

  CallExpr* copyExpr  = NULL;

  Symbol*   useLhs    = toSymExpr(lhs)->symbol();

  // Noakes 2017/03/04
  // Cannot use the qualified-type here.  The formal may be still a _ref(type)
  // and using a qualified-type generates yet another temp.
  Symbol*   tmpVar    = newTemp("ret_tmp", useLhs->getValType());

  bool copiesToNoDestroy = false;
  bool isRhsInitOrAutoCopy = false;

  // Determine if
  //   a) current call is not a PRIMOP
  //   b) the subsequent statement is PRIM_MOVE for an initCopy/autoCopy
  //   c) the initCopy/autoCopy has the same argument and return type
  //      (this accounts for tuples containing refs)
  if (fn                            != NULL  &&
      nextExpr                      != NULL)
  {
    if (CallExpr* callNext = toCallExpr(nextExpr))
    {
      if (callNext->isPrimitive(PRIM_MOVE) ||
          callNext->isPrimitive(PRIM_ASSIGN))
      {
        if (CallExpr* rhsCall = toCallExpr(callNext->get(2)))
        {
          FnSymbol* rhsFn = rhsCall->resolvedFunction();

          if (rhsFn                              != NULL &&
              (rhsFn->hasFlag(FLAG_AUTO_COPY_FN) == true ||
               rhsFn->hasFlag(FLAG_INIT_COPY_FN) == true))
          {
            isRhsInitOrAutoCopy = true;
            SymExpr* copiedSe = toSymExpr(rhsCall->get(1));
            INT_ASSERT(copiedSe);
            SymExpr* dstSe = toSymExpr(callNext->get(1));
            INT_ASSERT(dstSe);

            // check that the initCopy is copying the variable we just set
            if (copiedSe->symbol() == useLhs &&
                useLhs->hasFlag(FLAG_TEMP)) {
              ArgSymbol* formalArg  = rhsFn->getFormal(1);
              Type*      formalType = formalArg->type;
              Type*      actualType = copiedSe->symbol()->getValType();
              Type*      returnType = rhsFn->retType->getValType();

              // Skip this transformation if
              //  * the type differs
              //  * it is a domain (for A.domain returning unowned)
              //  * if it's a sync or single variable (different init/auto copy)
              if (actualType == returnType &&
                  isSyncType(formalType)   == false &&
                  isSingleType(formalType) == false)
              {
                bool copyFromUnownedDomain = false;
                if (actualType->symbol->hasFlag(FLAG_DOMAIN)) {
                  if (fn->hasFlag(FLAG_NO_COPY_RETURN))
                    copyFromUnownedDomain = true;
                }

                if (copyFromUnownedDomain == false) {
                  copyExpr = rhsCall;
                  if (dstSe->symbol()->hasFlag(FLAG_NO_AUTO_DESTROY))
                    copiesToNoDestroy = true;
                }
              }
            }
          }
        }
      }
    }
  }

  // Convert the by-value call to a void call with an additional formal
  moveExpr->replace(callExpr->remove());

  callExpr->insertAtTail(tmpVar);

  callExpr->insertBefore(new DefExpr(tmpVar));
  callExpr->insertAfter(new CallExpr(PRIM_MOVE, useLhs, tmpVar));

  // Possibly reduce a copy operation to a simple move
  // the copyExpr might be a copy added when normalizing initialization
  // of user variables. *or* it might come from handling `in` intent.
  if (copyExpr) {
    if (isRhsInitOrAutoCopy) {
      removeInitOrAutoCopyPostResolution(copyExpr);
    }
    else {
      copyExpr->replace(copyExpr->get(1)->remove());
    }

    if (copiesToNoDestroy) {
      useLhs->addFlag(FLAG_NO_AUTO_DESTROY);
      // and remove any auto destroy calls we just added
      // (since ReturnByRef runs after addAutoDestroyCalls)
      for_SymbolSymExprs(se, useLhs) {
        if (CallExpr* call = toCallExpr(se->parentExpr)) {
          FnSymbol* calledFn = call->resolvedFunction();
          if (calledFn && calledFn->hasFlag(FLAG_AUTO_DESTROY_FN))
            call->remove();
        }
      }
    }
  }
}

static
bool isLocalVariable(Expr* initFrom) {
  if (SymExpr* se = toSymExpr(initFrom)) {
    Symbol* sym = se->symbol();
    // is it local?
    if (sym->defPoint->getFunction() == initFrom->getFunction())
      // is it value (not ref) ?
      if (!sym->isRef())
        return true;

  }

  return false;
}

bool isCallExprTemporary(Symbol* fromSym) {
  if (fromSym->hasFlag(FLAG_EXPR_TEMP) ||
      fromSym->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW)) {
    // It's from an auto-destroyed value that is an expression temporary
    // storing the result of a function call.
    return true;
  }

  return false;
}

bool isTemporaryFromNoCopyReturn(Symbol* fromSym) {

  bool anyNoCopyReturn = false;
  for_SymbolSymExprs(se, fromSym) {
    SymExpr* lhsSe = NULL;
    CallExpr* initOrCtor = NULL;
    if (CallExpr* call = toCallExpr(se->parentExpr)) {
      if (isInitOrReturn(call, lhsSe, initOrCtor)) {
        if (lhsSe == se) {
          if (initOrCtor != NULL) {
            if (FnSymbol* calledFn = initOrCtor->resolvedOrVirtualFunction()) {
              if (calledFn->hasFlag(FLAG_NO_COPY_RETURN)) {
                anyNoCopyReturn = true;
              }
            }
          } else {
            // Track moving from another temp.
            if (SymExpr* rhsSe = toSymExpr(call->get(2))) {
              if (isTemporaryFromNoCopyReturn(rhsSe->symbol())) {
                anyNoCopyReturn = true;
              }
            }
          }
        }
      }
    }
  }

  return anyNoCopyReturn;
}

bool doesCopyInitializationRequireCopy(Expr* initFrom) {
  Type* fromType = initFrom->getValType();
  if (typeNeedsCopyInitDeinit(fromType)) {
    // RHS is a reference, need a copy
    if (initFrom->isRef())
      return true;
    // Past here, it's a value.

    // e.g. an array view being copied into an array.
    // This copy might be copy-elided later.
    if (getCopyTypeDuringResolution(fromType) != fromType)
      return true;

    // Is it the result of a call returning by value?
    SymExpr* fromSe = toSymExpr(initFrom);
    INT_ASSERT(fromSe != NULL); // assuming normalized AST
    if (isCallExprTemporary(fromSe->symbol())) {
      // check for the sub-expression being a function marked with
      //  pragma "no copy return"
      // in that event, the copy is required.
      if (isTemporaryFromNoCopyReturn(fromSe->symbol()))
        return true;
      else
        return false;
    }

    // Is it a local variable? Or a global? or an outer?
    // Need a copy in any of these cases for variable initialization.
    return true;
  }

  return false;
}

bool doesValueReturnRequireCopy(Expr* initFrom) {
  if (typeNeedsCopyInitDeinit(initFrom->getValType())) {
    // RHS is a reference, need a copy
    if (initFrom->isRef())
      return true;
    // Past here, it's a value.

    // Is it the result of a call returning by value?
    SymExpr* fromSe = toSymExpr(initFrom);
    if (isCallExprTemporary(fromSe->symbol())) {
      // check for the sub-expression being a function marked with
      //  pragma "no copy return"
      // in that event, the copy is required.
      if (isTemporaryFromNoCopyReturn(fromSe->symbol()))
        return true;
      else
        return false;
    }

    // Is it a local variable?
    if (isLocalVariable(initFrom))
      return false;

    // Or a global? or an outer?
    // Need a copy in any of these cases for return.
    return true;
  }

  return false;
}

/************************************* | **************************************
*                                                                             *
* Code to implement original style of return record by ref formal             *
*                                                                             *
************************************** | *************************************/

//
// Cache to avoid cloning functions that return records if the copy
// of the returned argument is done in the same way as at another
// call site; the key into the cache is the old function, the values
// are stored in a vector based on the copy function (copy function
// 1, new function 1, copy function 2, new function 2, ...)
//
static Map<FnSymbol*,Vec<FnSymbol*>*> retToArgCache;

static void
fixupDestructors() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_DESTRUCTOR)) {
      if (fn->_this == NULL) {
        continue;
      }
      AggregateType* ct = toAggregateType(fn->_this->getValType());
      INT_ASSERT(ct);

      if (ct->isUnion())
        continue;

      //
      // insert calls to destructors for all 'value' fields
      //
      for_fields_backward(field, ct) {
        SET_LINENO(field);

        if (field->type->hasDestructor() == true) {
          AggregateType* fct = toAggregateType(field->type);

          INT_ASSERT(fct);

          if (!isClass(fct) && !field->hasFlag(FLAG_NO_AUTO_DESTROY)) {
            bool       useRefType = !isRecordWrappedType(fct);
            VarSymbol* tmp        = newTemp("_field_destructor_tmp_",
                                            useRefType ? fct->refType : fct);

            fn->insertIntoEpilogue(new DefExpr(tmp));

            fn->insertIntoEpilogue(new CallExpr(PRIM_MOVE, tmp,
              new CallExpr(useRefType ? PRIM_GET_MEMBER : PRIM_GET_MEMBER_VALUE, fn->_this, field)));

            FnSymbol* autoDestroyFn = autoDestroyMap.get(field->type);

            if (autoDestroyFn &&
                autoDestroyFn->hasFlag(FLAG_REMOVABLE_AUTO_DESTROY)) {
              fn->insertIntoEpilogue(new CallExpr(autoDestroyFn, tmp));
            } else {
              fn->insertIntoEpilogue(new CallExpr(field->type->getDestructor(),
                                                  tmp));
            }
          }

        } else if (FnSymbol* autoDestroyFn = autoDestroyMap.get(field->type)) {
          VarSymbol* tmp = newTemp("_field_destructor_tmp_", field->type);

          fn->insertIntoEpilogue(new DefExpr(tmp));
          fn->insertIntoEpilogue(new CallExpr(PRIM_MOVE,
                                              tmp,
                                              new CallExpr(PRIM_GET_MEMBER_VALUE, fn->_this, field)));
          fn->insertIntoEpilogue(new CallExpr(autoDestroyFn, tmp));
        }
      }

      //
      // insert call to parent destructor
      //
      INT_ASSERT(ct->dispatchParents.n <= 1);

      if (ct->dispatchParents.n == 1 && isClass(ct) == true) {
        AggregateType* parType = ct->dispatchParents.v[0];

        if (FnSymbol* parDestructor = parType->getDestructor()) {
          SET_LINENO(fn);

          VarSymbol* tmp   = newTemp("_parent_destructor_tmp_", parType);
          Symbol*    _this = fn->_this;
          CallExpr*  cast  = new CallExpr(PRIM_CAST, parType->symbol, _this);

          fn->insertIntoEpilogue(new DefExpr(tmp));
          fn->insertIntoEpilogue(new CallExpr(PRIM_MOVE,     tmp, cast));
          fn->insertIntoEpilogue(new CallExpr(parDestructor, tmp));
        }
      }
    }
  }
}


static void ensureModuleDeinitFnAnchor(ModuleSymbol* mod, Expr*& anchor) {
  if (anchor)
    return;

  SET_LINENO(mod);

  FnSymbol* deinitFn = mod->deinitFn;

  if (!deinitFn) {
    deinitFn = new FnSymbol(astr("chpl__deinit_", mod->name));

    mod->block->insertAtTail(new DefExpr(deinitFn));

    normalize(deinitFn);
    resolveFunction(deinitFn);

    mod->deinitFn = deinitFn;
  }

  anchor = new CallExpr(PRIM_NOOP);
  deinitFn->insertIntoEpilogue(anchor);
}

static void cleanupModuleDeinitAnchor(Expr*& anchor) {
  if (anchor) {
    anchor->remove();
    anchor = NULL;
  }
}

static void noteGlobalInitialization(ModuleSymbol* mod,
                                     VarSymbol* var,
                                     std::vector<VarSymbol*>& inited,
                                     std::set<VarSymbol*>& initedSet) {
  // Only consider module-scope variables needing destruction
  if (isAutoDestroyedVariable(var) && var->defPoint->parentSymbol == mod) {
    // Try to insert into the initedSet
    if (initedSet.insert(var).second) {
      // An insertion occurred, meaning this was the first
      // thing that looked like initialization for this variable.
      inited.push_back(var);
    }
  }
}

// Collects global variables in initialization order.
// Does not do any checking of that order (that should already
// be handled in addAutoDestroyCalls on the module init fn).
// This is similar to walkBlock in addAutoDestroyCalls but very trimmed down.
static void collectGlobals(ModuleSymbol* mod,
                           BlockStmt* block,
                           std::vector<VarSymbol*>& inited,
                           std::set<VarSymbol*>& initedSet) {

  for (Expr* stmt = block->body.first(); stmt != NULL; stmt = stmt->next) {

    // Look for a variable initialization.
    if (isCallExpr(stmt)) {
      // note that these cases will run also when setting the variable
      // after the 1st initialization. That should be OK though because
      // once a variable is initialized, it stays initialized, until
      // it is destroyed.

      CallExpr* fCall = NULL;
      // Check for returned variable
      if (VarSymbol* v = initsVariable(stmt, fCall))
        noteGlobalInitialization(mod, v, inited, initedSet);

      if (fCall != NULL) {
        // Check also for out intent in a called function
        for_formals_actuals(formal, actual, fCall) {
          if (VarSymbol* v = initsVariableOut(formal, actual))
            noteGlobalInitialization(mod, v, inited, initedSet);
        }

        // Recurse into calls to compiler generated functions that
        // return array types (forall expression functions).
        // (Search for globalTemps in normalize.cpp to see related code).
        if (FnSymbol* calledFn = fCall->resolvedFunction())
          if (calledFn->hasFlag(FLAG_MAYBE_ARRAY_TYPE))
            if (calledFn->hasFlag(FLAG_COMPILER_GENERATED))
              collectGlobals(mod, calledFn->body, inited, initedSet);
      }

    // Recurse in to a BlockStmt (or sub-classes of BlockStmt e.g. a loop)
    } else if (BlockStmt* subBlock = toBlockStmt(stmt)) {
      collectGlobals(mod, subBlock, inited, initedSet);

    // Recurse in to the BlockStmt(s) of a CondStmt
    } else if (CondStmt*  cond     = toCondStmt(stmt))  {
      collectGlobals(mod, cond->thenStmt, inited, initedSet);
    }
  }
}

static void insertGlobalAutoDestroyCalls() {
  std::vector<VarSymbol*> inited;
  std::set<VarSymbol*> initedSet;

  forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
    if (isAlive(mod)) {
      Expr* anchor = NULL;

      inited.clear();
      initedSet.clear();
      if (mod->initFn != NULL) {
        collectGlobals(mod, mod->initFn->body, inited, initedSet);
      } else {
        // Collect variables from modules without initFn
        // (e.g. the rootModule)
        // TODO: can we remove this case?
        for_alist(expr, mod->block->body) {
          if (DefExpr* def = toDefExpr(expr))
            if (VarSymbol* var = toVarSymbol(def->sym))
              if (isAutoDestroyedVariable(var))
                inited.push_back(var);
        }
      }

      // Now go through inited in reverse order.
      for_vector(VarSymbol, var, inited) {
        if (isAutoDestroyedVariable(var)) {
          FnSymbol* autoDestroy = autoDestroyMap.get(var->type);
          SET_LINENO(var);

          ensureModuleDeinitFnAnchor(mod, anchor);

          // destroys go after anchor in reverse order of decls
          anchor->insertAfter(new CallExpr(autoDestroy, var));
        }
      }
      cleanupModuleDeinitAnchor(anchor);
    }
  }
}


static void lowerAutoDestroyRuntimeType(CallExpr* call) {
  if (SymExpr* rttSE = toSymExpr(call->get(1))) {
    Symbol *rttSym = rttSE->symbol();
    // toAggregateType() filters out calls in unresolved generic functions.
    if (AggregateType* rttAG = toAggregateType(rttSym->type)) {
      if (rttAG->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {

        for_fields(field, rttAG) {
          bool destroyField = false;
          bool destroyFieldRTT = false;
          FnSymbol *destroyFn = autoDestroyMap.get(field->getValType());

          if (destroyFn != NULL) {
            INT_ASSERT(call->getStmtExpr() == call);
            destroyField = true;
          }
          else if (AggregateType* fieldAG = toAggregateType(field->type)) {
            if (fieldAG->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
              destroyFieldRTT = true;
            }
          }

          if (destroyField || destroyFieldRTT) {
            SET_LINENO(call);
            VarSymbol* fieldTemp = newTemp("fieldTemp", field->getValType());
            call->insertBefore(new DefExpr(fieldTemp));
            call->insertBefore("'move'(%S,'.v'(%E,%S))", fieldTemp,
              new SymExpr(rttSym), field);

            CallExpr *destroyCall = NULL;
            if (destroyField) {
              // Invoke destroyFn on the field
              destroyCall = new CallExpr(destroyFn, fieldTemp);
            }
            else {
              // Add another PRIM_AUTO_DESTROY_RUNTIME_TYPE for the field
              destroyCall = new CallExpr(PRIM_AUTO_DESTROY_RUNTIME_TYPE, fieldTemp);
            }
            
            call->insertBefore(destroyCall);
          }
        }
      }
    }
  }

  call->remove();
}

static void insertDestructorCalls() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_CALL_DESTRUCTOR)) {
      Type* type = call->get(1)->typeInfo();

      if (type->hasDestructor() == false) {
        call->remove();
      } else {
        SET_LINENO(call);

        call->replace(new CallExpr(type->getDestructor(),
                                   call->get(1)->remove()));
      }
    } else if (call->isPrimitive(PRIM_AUTO_DESTROY_RUNTIME_TYPE)) {
      lowerAutoDestroyRuntimeType(call);
    }
  }
}

// This routine inserts autoCopy calls ahead of yield statements as necessary,
// so the calling routine "owns" the yielded value.
// The copy is necessary when an iterator yields a variable (rather than an
// expression temporary).
//
// For example
//
// iter ex1() {
//   var x:SomeRecord;
//   yield x;  //  should yield a copy of x
// }
// iter ex2() {
//   yield returnsSomeRecord();  // no need to copy
// }
//
// Note that for parallel iterators, yields can occur in task
// functions (that aren't iterators themselves).
static void insertCopiesForYields()
{
  // Examine all calls.
  forv_Vec(CallExpr, call, gCallExprs)
  {
    // Select only yield primitives.
    if (! call->isPrimitive(PRIM_YIELD))
      continue;

    // Filter out calls that are not in the tree.
    if (! call->parentSymbol)
      continue;

    // This is the symbol passed back in the yield.
    SymExpr* yieldedSe = toSymExpr(call->get(1));
    Symbol* yieldedSym = yieldedSe->symbol();

    FnSymbol* inFn = toFnSymbol(call->parentSymbol);
    IteratorInfo* ii = inFn->iteratorInfo;

    // coforall functions in iterators don't seem to have ii
    RetTag iteratorRetTag = RET_VALUE;
    if (ii)
      iteratorRetTag = ii->iteratorRetTag;


    // If the yielded symbol is subject to auto-copy/destroy discipline
    // and it's returned by value (not by reference)
    // and the yielded value is not an expression temporary
    //  (e.g. for yield someCall(), the result of someCall() doesn't need copy)
    // then we need to copy initialize into the yielded value.
    if (typeNeedsCopyInitDeinit(yieldedSym->getValType()) &&
        iteratorRetTag == RET_VALUE) {

      SymExpr* foundSe = findSourceOfYield(call);

      // Now foundSe is in the last simple PRIM_MOVE that set some
      // chain of symbols (leading to yield) and in particular it
      // is the RHS of that move.
      //
      // Or foundSe is the argument to PRIM_YIELD.

      // TODO - is the check for FLAG_INSERT_AUTO_DESTROY
      // necessary here? Could this use doesValueReturnRequireCopy?
      if (foundSe->symbol()->hasFlag(FLAG_INSERT_AUTO_DESTROY) &&
          !foundSe->symbol()->hasFlag(FLAG_EXPR_TEMP)) {
        // Add an auto-copy here.
        SET_LINENO(call);
        Type* type = foundSe->symbol()->getValType();
        Symbol* tmp = newTemp("_yield_expr_tmp_", type);
        Expr* stmt = foundSe->getStmtExpr();
        stmt->insertBefore(new DefExpr(tmp));
        stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp,
                           new CallExpr(getAutoCopyForType(type),
                                        foundSe->copy(),
                                        new SymExpr(gFalse))));

        foundSe->replace(new SymExpr(tmp));
      }
    }
  }
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool traceGlobalChecking = false;

static bool isCheckedModuleScopeVariable(VarSymbol* var) {
  if (DefExpr* def = var->defPoint) {
    ModuleSymbol* mod = def->getModule();
    if (mod && def->parentExpr == mod->block) {
      if (var->hasFlag(FLAG_TYPE_VARIABLE) == false &&
          var->hasFlag(FLAG_EXTERN) == false &&
          var->type->symbol->hasFlag(FLAG_EXTERN) == false &&
          var->hasFlag(FLAG_GLOBAL_VAR_BUILTIN) == false &&
          var->hasFlag(FLAG_TEMP) == false && // for loop exprs->array types
          var->isParameter() == false)
        return true;
    }
  }
  return false;
}

static VarSymbol* theCheckedModuleScopeVariable(Expr* actual) {
  if (SymExpr* se = toSymExpr(actual))
    if (VarSymbol* var = toVarSymbol(se->symbol()))
      if (isCheckedModuleScopeVariable(var))
        return var;

  return NULL;
}

// There should be a single instance of this class per compilation.
class GatherGlobalsReferredTo final : public AstVisitorTraverse {
  public:
    // these are set and "returned" by visiting a function
    FnSymbol* thisFunction;
    std::set<FnSymbol*> calledThisFunction;     // calls from this function
    std::set<VarSymbol*> mentionedThisFunction; // globals mentioned directly

    // this is global state storing results of analysis
    std::set<FnSymbol*> visited;
    std::map<FnSymbol*, std::set<VarSymbol*> > directGlobalMentions;
    std::map<FnSymbol*, std::set<FnSymbol*> > callGraph;

    GatherGlobalsReferredTo()
      : thisFunction(NULL)
    { }
    bool callUsesGlobal(CallExpr* c, std::set<VarSymbol*>& globals);
    bool fnUsesGlobal(FnSymbol* fn, std::set<VarSymbol*>& globals);
    bool enterFnSym(FnSymbol* fn) override;
    void visitSymExpr(SymExpr* se) override;
    void exitFnSym(FnSymbol* fn) override;
};

bool GatherGlobalsReferredTo::enterFnSym(FnSymbol* fn) {
  if (visited.insert(fn).second) {
    // if the function symbol hasn't already been visited,
    // put it in the visited set and do the visiting.
    // do the analysis in the other visitors

    // don't worry about 'halt' calling deinit functions
    // or about functions opting out of this analysis.
    // this helps with certain patters where globals are used in 'deinit'
    if (fn->hasFlag(FLAG_FUNCTION_TERMINATES_PROGRAM) ||
        fn->hasFlag(FLAG_IGNORE_IN_GLOBAL_ANALYSIS))
      return false;

    thisFunction = fn;
    calledThisFunction.clear();
    mentionedThisFunction.clear();
    return true;
  }

  return false;
}

void GatherGlobalsReferredTo::visitSymExpr(SymExpr* se) {
  // handle function calls or mentions
  if (FnSymbol* fn = toFnSymbol(se->symbol())) {
    // handle root virtual method or non-virtual call
    if (calledThisFunction.insert(fn).second) {

      if (traceGlobalChecking)
        USR_PRINT("fn '%s'[%i] calls fn '%s'[%i]",
                  thisFunction->name, thisFunction->id, fn->name, fn->id);

      bool inVirtualMethodCall = false;
      if (CallExpr* pCall = toCallExpr(se->parentExpr))
        if (pCall->isPrimitive(PRIM_VIRTUAL_METHOD_CALL))
          if (FnSymbol* vFn = toFnSymbol(toSymExpr(pCall->get(1))->symbol()))
            if (vFn == fn)
              inVirtualMethodCall = true;

      if (inVirtualMethodCall) {
        // if we added something, also add virtual children callable
        // handle additional children callable by virtual method
        if (Vec<FnSymbol*>* children = virtualChildrenMap.get(fn)) {
          forv_Vec(FnSymbol*, childFn, *children) {

            if (traceGlobalChecking)
              USR_PRINT("fn '%s'[%i] virtual calls fn '%s'[%i]",
                        thisFunction->name, thisFunction->id,
                        childFn->name, childFn->id);

            calledThisFunction.insert(childFn);
          }
        }
      }
    }
  }

  // handle global variable mentions
  if (VarSymbol* var = toVarSymbol(se->symbol())) {
    if (isCheckedModuleScopeVariable(var)) {
      // it's a module-scope variable, so add it to directGlobalMentions
      if (mentionedThisFunction.insert(var).second) {

        if (traceGlobalChecking)
          USR_PRINT("fn '%s'[%i] mentions global '%s'[%i]",
                    thisFunction->name, thisFunction->id,
                    var->name, var->id);
      }
    }
  }
}

void GatherGlobalsReferredTo::exitFnSym(FnSymbol* fn) {
  INT_ASSERT(fn == thisFunction);
  // update the global maps based on calledThisFunction / mentionedThisFunction
  std::set<FnSymbol*>& fnCalls = callGraph[thisFunction];
  std::set<VarSymbol*>& directMentions = directGlobalMentions[thisFunction];

  for_set(FnSymbol, called, calledThisFunction) {
    fnCalls.insert(called);
  }

  for_set(VarSymbol, mention, mentionedThisFunction) {
    directMentions.insert(mention);
  }
}


// There will be one instance of this per module, but these will
// share a single GatherGlobalsReferredTo.
class FindInvalidGlobalUses final : public AstVisitorTraverse {
  public:
    GatherGlobalsReferredTo& gatherVisitor;
    std::set<VarSymbol*> invalidGlobals;
    std::map<VarSymbol*, CallExpr*> copyElidedGlobals;
    std::vector<VarSymbol*> errorGlobalVariables;

    FindInvalidGlobalUses(GatherGlobalsReferredTo& gatherVisitor)
      : gatherVisitor(gatherVisitor)
    { }
    void issueError(VarSymbol* g, BaseAST* loc);
    void gatherModuleVariables(ModuleSymbol* thisModule);
    // stores in errorGlobalVariables the invalid variables that were used
    bool checkIfCalledUsesInvalid(CallExpr* c, bool error);
    // stores in errorGlobalVariables the invalid variables that were used
    bool checkIfFnUsesInvalid(FnSymbol* fn);
    bool errorIfFnUsesInvalid(FnSymbol* fn, BaseAST* loc,
                              std::set<FnSymbol*>& visited);
    bool enterCallExpr(CallExpr* call) override;
    bool enterCondStmt(CondStmt* cond) override;
};


void FindInvalidGlobalUses::issueError(VarSymbol* g, BaseAST* loc) {
  if (copyElidedGlobals.count(g) != 0) {
    USR_PRINT(loc,
              "mentions module-scope variable '%s'",
              g->name);
    USR_PRINT(copyElidedGlobals[g], "which is dead due to copy elided here");
  } else {
    USR_PRINT(loc,
             "mentions module-scope variable '%s' not initialized yet",
             g->name);
  }
}

void FindInvalidGlobalUses::gatherModuleVariables(ModuleSymbol* thisModule) {
  // initialize invalidGlobals with all global variables from this module
  for_alist (expr, thisModule->block->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (VarSymbol* var = toVarSymbol(def->sym)) {
        if (isCheckedModuleScopeVariable(var)) {
          invalidGlobals.insert(var);
        }
      }
    }
  }
}

// the error argument indicates if we should run slow error reporting.
bool FindInvalidGlobalUses::checkIfCalledUsesInvalid(CallExpr* c, bool error) {

  if (FnSymbol* calledFn = c->resolvedOrVirtualFunction()) {
    // consider root virtual method or non-virtual call

    if (checkIfFnUsesInvalid(calledFn)) {
      if (error) {
        std::set<FnSymbol*> visited;
        errorIfFnUsesInvalid(calledFn, c, visited);
      }
      return true;
    }

    // consider additional children callable by virtual method
    if (c->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
      if (Vec<FnSymbol*>* children = virtualChildrenMap.get(calledFn)) {
        forv_Vec(FnSymbol*, childFn, *children) {
          if (checkIfFnUsesInvalid(childFn)) {
            if (error) {
              std::set<FnSymbol*> visited;
              errorIfFnUsesInvalid(childFn, c, visited);
            }
            return true;
          }
        }
      }
    }
  }

  return false;
}

// returns true if there was any intersection
// returns intersecting elements in outVector
static
bool computeIntersection(std::set<VarSymbol*>& a, std::set<VarSymbol*>& b,
                         std::vector<VarSymbol*>& outVector)
{
  outVector.clear();
  std::set_intersection(a.begin(), a.end(),
                        b.begin(), b.end(),
                        std::back_inserter(outVector));

  return outVector.size() != 0;
}

// This function is intended to be relatively optimized
bool FindInvalidGlobalUses::checkIfFnUsesInvalid(FnSymbol* startFn) {
  std::map<FnSymbol*, std::set<VarSymbol*> >& directGlobalMentions =
    gatherVisitor.directGlobalMentions;
  std::map<FnSymbol*, std::set<FnSymbol*> >& callGraph =
    gatherVisitor.callGraph;

  std::set<FnSymbol*> everBeenInWork;
  std::vector<FnSymbol*> work;

  everBeenInWork.insert(startFn);
  work.push_back(startFn);

  while (!work.empty()) {
    // process a thing from the work set
    FnSymbol* fn = work.back();
    work.pop_back();

    // compute the call graph and globals used by this function
    // (if we haven't already)
    fn->accept(&gatherVisitor);

    // check for direct mentions of the globals in question
    // compute the intersection of
    // directGlobalMentions[fn] and invalidGlobals
    if (computeIntersection(invalidGlobals, directGlobalMentions[fn],
                            errorGlobalVariables))
    {
      return true;
    }

    // add called functions to the work queue
    std::set<FnSymbol*>& fnCalls = callGraph[fn];
    for_set (FnSymbol, called, fnCalls) {
      if (everBeenInWork.insert(called).second) {
        // 1st time visiting called; already added it to everBeenInWork
        work.push_back(called);
      }
    }
  }

  return false;
}

// This function is slow but handles the case in which an error needs
// to be reported. In that event it gives a stack trace.
bool FindInvalidGlobalUses::errorIfFnUsesInvalid(FnSymbol* fn, BaseAST* loc,
                                                 std::set<FnSymbol*>& visited) {
  std::map<FnSymbol*, std::set<VarSymbol*> >& directGlobalMentions =
    gatherVisitor.directGlobalMentions;
  std::map<FnSymbol*, std::set<FnSymbol*> >& callGraph =
    gatherVisitor.callGraph;
  std::vector<VarSymbol*> inV;

  if (visited.insert(fn).second == false)
    return false; // already visited

  if (developer || printsUserLocation(fn))
    USR_PRINT(loc, "calls function '%s'", fn->name);

  // check for direct mentions of the globals in question
  // compute the intersection of
  // directGlobalMentions[otherFn] and invalidGlobals
  if (computeIntersection(invalidGlobals, directGlobalMentions[fn], inV)) {
    for_vector (VarSymbol, g, inV) {
      SymExpr* se = findSymExprFor(fn, g);
      INT_ASSERT(se);
      issueError(g, se);
    }
    return true;
  }

  // check the call graph
  std::set<FnSymbol*>& fnCalls = callGraph[fn];
  for_set (FnSymbol, called, fnCalls) {
    if (checkIfFnUsesInvalid(called)) {
      BaseAST* useLoc = findSymExprFor(fn, called);
      if (useLoc == NULL)
        useLoc = fn;
      if (!developer && !printsUserLocation(useLoc))
        useLoc = loc;

      errorIfFnUsesInvalid(called, useLoc, visited);
      return true;
    }
  }
  return false;
}


bool FindInvalidGlobalUses::enterCallExpr(CallExpr* call) {

  if (call->isPrimitive(PRIM_ASSIGN_ELIDED_COPY)) {
    if (SymExpr* rhsSe = toSymExpr(call->get(2))) {
      if (VarSymbol* var = toVarSymbol(rhsSe->symbol())) {
        if (isCheckedModuleScopeVariable(var)) {
          invalidGlobals.insert(var);
          copyElidedGlobals[var] = call;
        }
      }
    }
  }

  // Nothing to do if there are no invalid globals
  if (invalidGlobals.size() == 0)
    return false;

  CallExpr* fCall = NULL;
  VarSymbol* retVar = initsVariable(call, fCall);

  // First, check any actuals not being initialized in the call.
  if (fCall != NULL) {
    // check an actual function call
    for_formals_actuals(formal, actual, fCall) {
      SymExpr* actualSe = toSymExpr(actual);
      if (actualSe && actualSe->symbol() == retVar) {
        // OK, ret-arg e.g.
      } else if (initsVariableOut(formal, actual)) {
        // OK, out intents will count as initialization
      } else if (VarSymbol* var = theCheckedModuleScopeVariable(actual)) {
        if (invalidGlobals.count(var) != 0)
          issueError(var, actual);
      }
    }
  } else {
    // check actuals used in primitives
    for_actuals(actual, call) {
      SymExpr* actualSe = toSymExpr(actual);
      if (actualSe && actualSe->symbol() != retVar) {
        if (VarSymbol* var = theCheckedModuleScopeVariable(actual))
          if (invalidGlobals.count(var) != 0)
            issueError(var, actual);
      }
    }
  }

  CallExpr* checkCall = fCall ? fCall : call;
  // Then, check any called functions
  if (checkIfCalledUsesInvalid(checkCall, false)) {
    for_vector (VarSymbol, var, errorGlobalVariables) {
      USR_FATAL_CONT(checkCall,
                     "module-scope variable '%s' may be used "
                     "before it is initialized",
                     var->name);
      printUseBeforeInitDetails(var);
    }
    checkIfCalledUsesInvalid(checkCall, true);
  }

  // Then, note the variables initialized by the call

  // move/return arg
  if (retVar && isCheckedModuleScopeVariable(retVar))
    invalidGlobals.erase(retVar);
  // out intent
  if (fCall != NULL) {
    for_formals_actuals(formal, actual, fCall) {
      if (VarSymbol* outVar = initsVariableOut(formal, actual))
        if (isCheckedModuleScopeVariable(outVar))
          invalidGlobals.erase(outVar);
    }
  }

  return false;
}

bool FindInvalidGlobalUses::enterCondStmt(CondStmt* cond) {
  if (cond->elseStmt) {
    std::set<VarSymbol*> saveInvalidGlobals = invalidGlobals;
    std::map<VarSymbol*, CallExpr*> saveElidedGlobals = copyElidedGlobals;

    cond->elseStmt->accept(this);

    // put the initializations back for considering the if clause
    invalidGlobals = saveInvalidGlobals;
    copyElidedGlobals = saveElidedGlobals;
  }

  // visiting the if clause will make note of any inits
  cond->thenStmt->accept(this);

  return false;
}

// A global variable might be used in a function called to initialize it.
// This function detects that case and raises an error.
//
// Sometimes this results in resolution errors, but that depends on the
// order of resolution and isn't reliable.
//
// This analysis is interprocedural. In a separate compilation setting,
// simply noting which global variables might be referred to in which
// functions does not seem prohibitive.
//
static void checkForInvalidGlobalUses() {
  GatherGlobalsReferredTo gatherVisitor;

  forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
    if (mod->initFn != NULL &&
        (mod->modTag != MOD_INTERNAL && mod->modTag != MOD_STANDARD)) {
      // if we expand to internal modules, have to contend with
      // the possibility of writeln being called in a class deinitializer
      // and using stdout.

      FindInvalidGlobalUses checkVisitor(gatherVisitor);
      checkVisitor.gatherModuleVariables(mod);
      mod->initFn->body->accept(&checkVisitor);
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// This function checks to see if the actual argument for a given formal is
// a temporary that has been copy-initialized. If so, the function tries
// to return the original actual. Look for a pattern like:
//    move temp (call chpl__copyInit actual)
// And return 'actual'.
static SymExpr* getActualBeforeCopyInit(CallExpr* call, ArgSymbol* formal) {
  Expr* actual = formal_to_actual(call, formal);
  SymExpr* se = toSymExpr(actual);

  if (se && se->symbol()->hasFlag(FLAG_TEMP)) {
    for_SymbolSymExprs(use, se->symbol()) {
      CallExpr* def = toCallExpr(use->parentExpr);

      if (def && (def->isPrimitive(PRIM_ASSIGN) ||
                  def->isPrimitive(PRIM_MOVE))) {
        SymExpr* lhs = toSymExpr(def->get(1));
        CallExpr* rhs = toCallExpr(def->get(2));

        if (lhs && rhs && (lhs->symbol() == se->symbol())) {
          FnSymbol* rhsFn = rhs->resolvedFunction();

          if (rhsFn && rhsFn->hasFlag(FLAG_INIT_COPY_FN)) {
            if (SymExpr* result = toSymExpr(rhs->get(1))) {
              return result;
            }
          }
        }
      }
    }
  }

  return NULL;
}

// Function resolution adds "dummy" initCopy functions for types
// that cannot be copied. These "dummy" initCopy functions are marked
// with the flag FLAG_ERRONEOUS_COPY. This pattern enables
// the compiler to continue to operate with its current structure
// even for types that cannot be copied. In particular, this pass
// has the ability to remove initCopy calls in some cases.
//
// This function simply checks that no function marked with that
// flag is ever called and raises an error if so.
//
// Additionally, also collect in formals that are marked with
// "error on copy", and check their actuals to see if they are
// initialized with calls to initCopy().
static void checkForErroneousInitCopies() {

  std::map<FnSymbol*, const char*> errors;
  std::set<ArgSymbol*> errorOnCopyFormals;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ERRONEOUS_COPY)) {
      // Store the error in the local map
      if (const char* err = getErroneousCopyError(fn))
        errors[fn] = err;
    }

    // Collect in intent formals that are marked "error on copy".
    for_formals(formal, fn) {
      if (formal->hasFlag(FLAG_ERROR_ON_COPY)) {
        if (formal->intent & INTENT_IN ||
            formal->originalIntent & INTENT_IN) {
          errorOnCopyFormals.insert(formal);
        } else {
          INT_FATAL(formal, "is marked with %s but is not %s",
                            "error on copy",
                            intentDescrString(INTENT_IN));
        }
      }
    }
  }

  // Iterate through "error on copy" formals and check their callsites.
  // Emit an error if an actual is a copy.
  for_set(ArgSymbol, formal, errorOnCopyFormals) {
    FnSymbol* fn = formal->getFunction();
    INT_ASSERT(fn != NULL);

    if (!fn->inTree())
      continue;

    computeAllCallSites(fn);

    forv_Vec(CallExpr, call, *fn->calledBy) {
      if (SymExpr* actual = getActualBeforeCopyInit(call, formal)) {

        // TODO: Tweak output if it prints something like 'with <temp>'.
        // Not sure if that can ever happen, thanks to copy elision, but
        // leave this comment just in case.
        USR_FATAL_CONT(call, "calling '%s' with actual '%s' would result "
                             "in a copy",
                             fn->name,
                             toString(actual->symbol(), false));
      }
    }
  }

  // Mark initCopy/autoCopy functions calling functions marked with
  // FLAG_ERRONEOUS_COPY with the same
  // flag. This situation can come up with the compiler-generated
  // tuple copy functions.
  bool changed;
  do {
    changed = false;
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->hasFlag(FLAG_ERRONEOUS_COPY)) {
        for_SymbolSymExprs(se, fn) {
          if (FnSymbol* callInFn = se->getFunction()) {
            bool inCopyIsh = callInFn->hasFlag(FLAG_INIT_COPY_FN) ||
                             callInFn->hasFlag(FLAG_AUTO_COPY_FN) ||
                             callInFn->hasFlag(FLAG_COERCE_FN) ||
                             (callInFn->hasFlag(FLAG_COPY_INIT) &&
                              callInFn->hasFlag(FLAG_COMPILER_GENERATED));
            if (inCopyIsh && !callInFn->hasFlag(FLAG_ERRONEOUS_COPY)) {
              callInFn->addFlag(FLAG_ERRONEOUS_COPY);
              changed = true;

              // propagate error if present
              if (errors.count(fn) != 0)
                errors[callInFn] = errors[fn];
            }
          }
        }
      }
    }
  } while(changed);

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ERRONEOUS_COPY)) {
      // Error on each call site
      for_SymbolSymExprs(se, fn) {
        if (FnSymbol* callInFn = se->getFunction()) {
          bool inCopyIsh = callInFn->hasFlag(FLAG_INIT_COPY_FN) ||
                           callInFn->hasFlag(FLAG_AUTO_COPY_FN) ||
                           callInFn->hasFlag(FLAG_COERCE_FN) ||
                           (callInFn->hasFlag(FLAG_COPY_INIT) &&
                            callInFn->hasFlag(FLAG_COMPILER_GENERATED));
          if (inCopyIsh == false) {

            USR_FATAL_CONT(se, "invalid copy-initialization");

            if (errors.count(fn) != 0)
              USR_FATAL_CONT(se, "%s", errors[fn]);

            Type* t = fn->getFormal(1)->getValType();
            if (fn->hasFlag(FLAG_COERCE_FN)) {
              t = fn->getFormal(2)->getValType();
            }

            if (printsUserLocation(t)) {
              USR_PRINT(t, "%s does not have a valid init=", toString(t));
            } else if (t->symbol->userInstantiationPointLoc.filename != NULL) {
              USR_PRINT(t->symbol->userInstantiationPointLoc,
                        "%s does not have a valid init=", toString(t));
            }
          } else {
            // Should have been propagated above
            INT_ASSERT(callInFn->hasFlag(FLAG_ERRONEOUS_COPY));
          }
        }
      }
    }
  }

  //
  // Mark functions that are only called by FLAG_ERRONEOUS_COPY fns
  // with the same flag as well (to avoid errors on these in some cases).
  // These will generally be removed by the prune pass.
  do {
    changed = false;
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->hasFlag(FLAG_ERRONEOUS_COPY)) {
        // do nothing
      } else {
        // check if all calls are inside FLAG_ERRONEOUS_COPY fns
        bool allMentionsInErroneousFns = true;
        bool anyMentionsInErroneousFns = false;
        for_SymbolSymExprs(se, fn) {
          if (FnSymbol* inFn = se->getFunction()) {
            if (inFn->hasFlag(FLAG_ERRONEOUS_COPY)) {
              anyMentionsInErroneousFns = true;
              // continue to make sure they all are
            } else {
              allMentionsInErroneousFns = false;
              break;
            }
          }
        }
        if (anyMentionsInErroneousFns && allMentionsInErroneousFns) {
          fn->addFlag(FLAG_ERRONEOUS_COPY);
          changed = true;
        }
      }
    }
  } while(changed);
}

/*

 A coforall index variable should transfer ownership of a
 yielded record from the iterator's yielded value to the
 task function.

 However, normal record memory management strategies would
 lead the coforall index variable to be destroyed in the loop
 creating tasks.

 This function moves the destruction of the coforall index
 variable to the end of the task body (which is represented
 as a coforall function).

 It's likely that this issue would be clearer to implement in
 the compiler if coforall loops had their own AST node instead
 of being represented as ForLoop.

 */
static void destroyFormalInTaskFn(ArgSymbol* formal, FnSymbol* taskFn);

static void adjustCoforallIndexVariables() {

  std::set<ArgSymbol*> handledFormals;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_COBEGIN_OR_COFORALL)) {

      // For coforall functions, find the actual and formal corresponding
      // to the coforall index variable.
      for_SymbolSymExprs(fnSe, fn) {
        if (CallExpr* call = toCallExpr(fnSe->parentExpr)) {
          if (call->baseExpr == fnSe) {
            for_formals_actuals(formal, actualExpr, call) {
              SymExpr* actualSe = toSymExpr(actualExpr);
              INT_ASSERT(actualSe);
              Symbol* actual = actualSe->symbol();
              if (actual->hasFlag(FLAG_COFORALL_INDEX_VAR) &&
                  actual->hasFlag(FLAG_INSERT_AUTO_DESTROY) &&
                  typeNeedsCopyInitDeinit(actual->type)) {

                // Remove FLAG_INSERT_AUTO_DESTROY so it will not
                // be destroyed in the loop creating tasks.
                actual->removeFlag(FLAG_INSERT_AUTO_DESTROY);

                // instead, add the destruction at the end of
                // the coforall body / task function.
                if (handledFormals.count(formal) == 0) {
                  destroyFormalInTaskFn(formal, fn);
                  handledFormals.insert(formal);
                }
              }
            }
          }
        }
      }
    }
  }
}
static void destroyFormalInTaskFn(ArgSymbol* formal, FnSymbol* taskFn) {
  SET_LINENO(formal);

  CallExpr* downEndCount = findDownEndCount(taskFn);
  INT_ASSERT(downEndCount);
  FnSymbol* autoDestroyFn = autoDestroyMap.get(formal->type);
  INT_ASSERT(autoDestroyFn);
  CallExpr* autoDestroyCall = new CallExpr(autoDestroyFn, formal);
  downEndCount->insertBefore(autoDestroyCall);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/


static void removeEndOfStatementMarkersElidedCopyPrimsZips() {
  for_alive_in_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_END_OF_STATEMENT))
      call->remove();
    if (call->isPrimitive(PRIM_ASSIGN_ELIDED_COPY))
      call->primitive = primitives[PRIM_ASSIGN];

    // we keep PRIM_ZIPs after resolution as markers to avoid copy elision for
    // symbols that are used in zip clauses. At this point, we no longer need
    // those
    if (call->isPrimitive(PRIM_ZIP))
      call->remove();
  }
}

static void removeElidedOnBlocks() {
  for_alive_in_Vec(BlockStmt, block, gBlockStmts) {
    if (block->isLoopStmt() == false) {
      if (CallExpr* const info = block->blockInfoGet()) {
        if (info->isPrimitive(PRIM_BLOCK_ELIDED_ON)) {
          // Turn it into a regular block.
          info->remove();
        }
      }
    }
  }
}

static void insertAutoDestroyPrimsForLoopExprTemps() {
  // below is a workaround for stopping the leaks coming from forall exprs that
  // are array types. This leak only occurs if the expression is not assigned to
  // a type variable, and it uses ranges and not domains.
  //
  // can we cache the CallExprs we care about in resolveCall etc?
  for_alive_in_Vec(CallExpr, call, gCallExprs) {
    // don't need to touch ArgSymbols
    if (!isArgSymbol(call->parentSymbol)) {
      // are we calling a resolved call_forallexpr?
      if (FnSymbol *callee = call->resolvedFunction()) {
        if (callee->hasFlag(FLAG_FN_RETURNS_ITERATOR)) {
          if (startsWith(callee->name, astr_forallexpr)) {
            // is the argument a range? -- if so, this call will create a domain
            // that we need to clean in the calling scope
            if (SymExpr *argSE = toSymExpr(call->get(1))) {
              if (argSE->symbol()->type->symbol->hasFlag(FLAG_RANGE)) {
                // are we moving the result of the call to an expr temp (so that
                // it is not a user variable) that is a runtime type value?
                if (CallExpr *parentCall = toCallExpr(call->parentExpr)) {
                  if (parentCall->isPrimitive(PRIM_MOVE)) {
                    if (SymExpr *targetSE = toSymExpr(parentCall->get(1))) {
                      if (targetSE->symbol()->hasFlag(FLAG_EXPR_TEMP) &&
                          targetSE->symbol()->type->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
                        SET_LINENO(call);
                        call->getFunction()->insertBeforeEpilogue(
                              new CallExpr(PRIM_AUTO_DESTROY_RUNTIME_TYPE,
                              new SymExpr(targetSE->symbol())));

                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}


/************************************* | **************************************
*                                                                             *
* Entry point                                                                 *
*                                                                             *
************************************** | *************************************/

void callDestructors() {

  adjustCoforallIndexVariables();

  createIteratorBreakBlocks();

  fixupDestructors();

  insertAutoDestroyPrimsForLoopExprTemps();

  insertDestructorCalls();

  ReturnByRef::apply();

  insertCopiesForYields();

  lateConstCheck(NULL);

  addAutoDestroyCalls();

  insertGlobalAutoDestroyCalls();

  checkForInvalidGlobalUses();

  checkForErroneousInitCopies();

  checkLifetimesAndNilDereferences();

  convertClassTypesToCanonical();

  removeEndOfStatementMarkersElidedCopyPrimsZips();
  removeElidedOnBlocks();
}
