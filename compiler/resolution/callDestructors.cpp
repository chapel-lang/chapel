/*
 * Copyright 2004-2020 Cray Inc.
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
#include "errorHandling.h"
#include "DecoratedClassType.h"
#include "ForallStmt.h"
#include "iterator.h"
#include "lateConstCheck.h"
#include "lifetime.h"
#include "postFold.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "resolveIntents.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "virtualDispatch.h"

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

    if (move->isPrimitive(PRIM_MOVE) == true)
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
                autoCopy = new CallExpr(getAutoCopyForType(symRhs->type), rhs);
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

    if (move->isPrimitive(PRIM_MOVE) == true)
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
              CallExpr* copyCall = new CallExpr(copyFn, exprRhs);
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

    if (move->isPrimitive(PRIM_MOVE) == true)
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
              autoCopy = new CallExpr(getAutoCopyForType(symRhs->type), rhs);
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
  while (nextExpr && (isCheckErrorStmt(nextExpr) || isDefExpr(nextExpr)))
    nextExpr = nextExpr->next;

  CallExpr* copyExpr  = NULL;

  Symbol*   useLhs    = toSymExpr(lhs)->symbol();

  // Noakes 2017/03/04
  // Cannot use the qualified-type here.  The formal may be still a _ref(type)
  // and using a qualified-type generates yet another temp.
  Symbol*   tmpVar    = newTemp("ret_tmp", useLhs->type);

  FnSymbol* unaliasFn = NULL;

  bool copiesToNoDestroy = false;

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
      if (callNext->isPrimitive(PRIM_MOVE) == true)
      {
        if (CallExpr* rhsCall = toCallExpr(callNext->get(2)))
        {
          FnSymbol* rhsFn = rhsCall->resolvedFunction();

          if (rhsFn                              != NULL &&
              (rhsFn->hasFlag(FLAG_AUTO_COPY_FN) == true ||
               rhsFn->hasFlag(FLAG_INIT_COPY_FN) == true))
          {
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

              unaliasFn = getUnalias(useLhs->type);

              // Cannot reduce initCopy/autoCopy when types differ
              //   (unless there is an unaliasFn available)
              // Cannot reduce initCopy/autoCopy for sync variables
              bool typesOK = unaliasFn != NULL || actualType == returnType;

              if (typesOK                  == true  &&
                  isSyncType(formalType)   == false &&
                  isSingleType(formalType) == false)
              {
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

  // Convert the by-value call to a void call with an additional formal
  moveExpr->replace(callExpr->remove());

  callExpr->insertAtTail(tmpVar);

  callExpr->insertBefore(new DefExpr(tmpVar));
  callExpr->insertAfter(new CallExpr(PRIM_MOVE, useLhs, tmpVar));

  // Possibly reduce a copy operation to a simple move
  // the copyExpr might be a copy added when normalizing initialization
  // of user variables. *or* it might come from handling `in` intent.
  if (copyExpr) {
    FnSymbol* rhsFn = copyExpr->resolvedFunction();

    // Use an unalias call if possible
    if (rhsFn->hasFlag(FLAG_INIT_COPY_FN) && unaliasFn != NULL) {
      // BHARSH: It seems important that there's a temporary to store the
      // result of the unaliasFn call. Otherwise we'll move into a variable
      // that has multiple uses, which seems to cause a variety of problems.
      //
      // In particular, I noticed that `changeRetToArgAndClone` generates
      // bad AST if I simply did this:
      //   copyExpr->replace(new CallExpr(unaliasFn, refVar));
      VarSymbol* unaliasTemp = newTemp("unaliasTemp", unaliasFn->retType);
      CallExpr*  unaliasCall = new CallExpr(unaliasFn, tmpVar);
      Expr* anchor = copyExpr->getStmtExpr();

      // The call needs to be inserted before it is needed but
      // after any error handling conditional that might be after callExpr.
      anchor->insertBefore(new DefExpr(unaliasTemp));
      anchor->insertBefore(new CallExpr(PRIM_MOVE, unaliasTemp, unaliasCall));

      copyExpr->replace(new SymExpr(unaliasTemp));
    } else {
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

static
bool isCallExprTemporary(Expr* initFrom) {
  SymExpr* fromSe = toSymExpr(initFrom);
  INT_ASSERT(fromSe);
  Symbol* fromSym = fromSe->symbol();
  if (fromSym->hasFlag(FLAG_EXPR_TEMP) ||
      fromSym->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW)) {
    // It's from an auto-destroyed value that is an expression temporary
    // storing the result of a function call.
    return true;
  }

  return false;
}

bool doesCopyInitializationRequireCopy(Expr* initFrom) {
  if (typeNeedsCopyInitDeinit(initFrom->getValType())) {
    // RHS is a reference, need a copy
    if (initFrom->isRef())
      return true;
    // Past here, it's a value.

    // Is it the result of a call returning by value?
    if (isCallExprTemporary(initFrom))
      return false;

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
    if (isCallExprTemporary(initFrom))
      return false;

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

      //
      // insert calls to destructors for all 'value' fields
      //
      for_fields_backward(field, ct) {
        SET_LINENO(field);

        if (field->type->hasDestructor() == true) {
          AggregateType* fct = toAggregateType(field->type);

          INT_ASSERT(fct);

          if (!isClass(fct)) {
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

static void insertGlobalAutoDestroyCalls() {
  forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
    if (isAlive(mod)) {
      Expr* anchor = NULL;

      for_alist(expr, mod->block->body) {
        if (DefExpr* def = toDefExpr(expr)) {
          if (VarSymbol* var = toVarSymbol(def->sym)) {
            if (isAutoDestroyedVariable(var)) {
              FnSymbol* autoDestroy = autoDestroyMap.get(var->type);
              SET_LINENO(var);

              ensureModuleDeinitFnAnchor(mod, anchor);

              // destroys go after anchor in reverse order of decls
              anchor->insertAfter(new CallExpr(autoDestroy, var));
            }
          }
        }
      }
      cleanupModuleDeinitAnchor(anchor);
    }
  }
}


static void lowerAutoDestroyRuntimeType(CallExpr* call) {
 if (SymExpr* rttSE = toSymExpr(call->get(1)))
  // toAggregateType() filters out calls in unresolved generic functions.
  if (AggregateType* rttAG = toAggregateType(rttSE->symbol()->type))
   if (rttAG->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE))
    // Todo: the same for the element type component and
    // for the case of a runtime type for a domain.
    // Todo: avoid hard-coding the field names.
    if (Symbol* domField = rttAG->getField("dom", false))
     if (FnSymbol* destroyFn = autoDestroyMap.get(domField->getValType()))
      {
       // Invoke destroyFn on rttSE->dom.
       INT_ASSERT(call->getStmtExpr() == call);
       SET_LINENO(call);
       VarSymbol* domTemp = newTemp("domTemp", domField->getValType());
       call->insertBefore(new DefExpr(domTemp));
       call->insertBefore("'move'(%S,'.v'(%E,%S))", domTemp,
                          rttSE->remove(), domField);
       call->insertBefore(new CallExpr(destroyFn, domTemp));
      }
 // Whether we expanded it above or it is a no-op, we are done with it.
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
        stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(getAutoCopyForType(type), foundSe->copy())));

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

// Function resolution adds "dummy" initCopy functions for types
// that cannot be copied. These "dummy" initCopy functions are marked
// with the flag FLAG_ERRONEOUS_INITCOPY. This pattern enables
// the compiler to continue to operate with its current structure
// even for types that cannot be copied. In particular, this pass
// has the ability to remove initCopy calls in some cases.
//
// This function simply checks that no function marked with that
// flag is ever called and raises an error if so.
static void checkForErroneousInitCopies() {

  // Mark initCopy/autoCopy functions calling functions marked with
  // FLAG_ERRONEOUS_INITCOPY/FLAG_ERRONEOUS_AUTOCOPY with the same
  // flag. This situation can come up with the compiler-generated
  // tuple copy functions.
  bool changed;
  do {
    changed = false;
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->hasFlag(FLAG_ERRONEOUS_INITCOPY)) {
        for_SymbolSymExprs(se, fn) {
          if (FnSymbol* callInFn = se->getFunction()) {
            if (callInFn->hasFlag(FLAG_INIT_COPY_FN) &&
                !callInFn->hasFlag(FLAG_ERRONEOUS_INITCOPY)) {
              callInFn->addFlag(FLAG_ERRONEOUS_INITCOPY);
              changed = true;
            }
          }
        }
      }

      if (fn->hasFlag(FLAG_ERRONEOUS_AUTOCOPY)) {
        for_SymbolSymExprs(se, fn) {
          if (FnSymbol* callInFn = se->getFunction()) {
            if (callInFn->hasFlag(FLAG_AUTO_COPY_FN) &&
                !callInFn->hasFlag(FLAG_ERRONEOUS_AUTOCOPY)) {
              callInFn->addFlag(FLAG_ERRONEOUS_AUTOCOPY);
              changed = true;
            }
          }
        }
      }
    }
  } while(changed);

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ERRONEOUS_INITCOPY)) {
      // Error on each call site
      for_SymbolSymExprs(se, fn) {
        if (FnSymbol* callInFn = se->getFunction()) {
          if (!callInFn->hasFlag(FLAG_INIT_COPY_FN)) {
            USR_FATAL_CONT(se,
                           "copy-initialization invoked for a type "
                           "that does not have a copy initializer");
          } else {
            // Should have been propagated above
            INT_ASSERT(callInFn->hasFlag(FLAG_ERRONEOUS_INITCOPY));
          }
        }
      }
    }

    if (fn->hasFlag(FLAG_ERRONEOUS_AUTOCOPY)) {
      // Error on each call site
      for_SymbolSymExprs(se, fn) {
        if (FnSymbol* callInFn = se->getFunction()) {
          if (!callInFn->hasFlag(FLAG_AUTO_COPY_FN)) {
            USR_FATAL_CONT(se,
                           "implicit copy-initialization invoked for a type "
                           "that does not allow it");
          } else {
            // Should have been propagated above
            INT_ASSERT(callInFn->hasFlag(FLAG_ERRONEOUS_AUTOCOPY));
          }
        }
      }
    }
  }
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
* Entry point                                                                 *
*                                                                             *
************************************** | *************************************/

void callDestructors() {

  adjustCoforallIndexVariables();

  createIteratorBreakBlocks();

  fixupDestructors();

  insertDestructorCalls();

  // Execute this before conversion to return by ref
  // May fail to handle reference variables as desired
  addAutoDestroyCalls();

  ReturnByRef::apply();

  insertCopiesForYields();

  checkLifetimes();
  // Note - checkLifetimes adds flags to mark when a variable is dead:
  //  FLAG_DEAD_END_OF_BLOCK and FLAG_DEAD_LAST_MENTION
  // to local variables. Other parts of this pass use this.

  lateConstCheck(NULL);

  insertGlobalAutoDestroyCalls();

  checkForErroneousInitCopies();

  convertClassTypesToCanonical();
}
