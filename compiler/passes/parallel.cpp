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

//
// Transformations for begin, cobegin, and on statements
//

#include "passes.h"

#include "astutil.h"
#include "driver.h"
#include "errorHandling.h"
#include "expr.h"
#include "files.h"
#include "optimizations.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "wellknown.h"

// Notes on
//   makeHeapAllocations()    //invoked from parallel()
//   insertWideReferences()
//
//------
// Terminology/abbreviations:
//
// 'on'+'begin' is considered to be both an 'on' and a 'begin'
// - see also FLAG_NON_BLOCKING
//
// A "global" is a module-level symbol, usu. a VarSymbol.
// A 'var' is a global if, equivalently:
//  isModuleSymbol(var->defPoint->parentSymbol)
//  isGlobal(var)
//
// MHA = makeHeapAllocations() and functions it invokes
// IWR = insertWideReferences() and functions it invokes
//------
//
// MHA and IWR take care of the following, among others:
// - heap allocation for remote access
// - change acces to variable -> access to its ._value
// - set up wide references
// - broadcasting of globals
//
// In more details:
//
// Heap allocation for remote access is done:
// - for globals - in IWR, if:
//    requireWideReferences()
//
// Change acces to variable -> access to its ._value
// - for globals - in MHA, if:
//    requireWideReferences()
// - for locals - in MHA, if:
//    the local is subject to heap allocation
//    in either of the above two categories
//
// Wide references are set up in IWR, if:
//    requireWideReferences()
//
// Broadcasting:
// - of certain global constants       - in MHA/findHeapVarsAndRefs, if:
//    !fLocal
// - of global arrays/domains/distribs - in MHA/findHeapVarsAndRefs, if:
//    !fLocal
// - of locations of the other globals - in IWR, if:
//    requireWideReferences()


typedef struct {
  bool firstCall;
  AggregateType* ctype;
  FnSymbol*  wrap_fn;
  std::vector<uint8_t> needsDestroy;
  bool adjustErrors;
  // TODO -- more directly record the mapping between
  // actuals, formals, and bundle class fields
} BundleArgsFnData;

// bundleArgsFnDataInit: the initial value for BundleArgsFnData
static BundleArgsFnData bundleArgsFnDataInit = { true, NULL, NULL };

static int broadcastGlobalID = 0;

static void insertEndCounts();
static void passArgsToNestedFns();
static void create_block_fn_wrapper(FnSymbol* fn, CallExpr* fcall, BundleArgsFnData &baData);
static void call_block_fn_wrapper(FnSymbol* fn, CallExpr* fcall, VarSymbol* args_buf, VarSymbol* args_buf_len, VarSymbol* tempc, FnSymbol *wrap_fn, Symbol* taskList, Symbol* taskListNode);
static void findHeapVarsAndRefs(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                Vec<Symbol*>& varSet, Vec<Symbol*>& varVec);
static bool needsAutoCopyAutoDestroyForArg(ArgSymbol* formal, Expr* arg, FnSymbol* fn);

static void replaceRecordWrappedRefs();

// Package args into a class and call a wrapper function with that
// object. The wrapper function will then call the function
// created by the previous parallel pass. This is a way to pass along
// multiple args through the limitation of one arg in the runtime's
// thread creation interface.
//
// Implemented using BundleArgsFnData and the functions:
//   create_arg_bundle_class
//   bundleArgs
//   create_block_fn_wrapper
//   call_block_fn_wrapper

// Even though the arg bundle class depends only on the iterator,
// current code unfortunately uses the call site for some information
// If there are multiple call sites, the first one is used.

static void create_arg_bundle_class(FnSymbol* fn, CallExpr* fcall, ModuleSymbol* mod, BundleArgsFnData &baData) {
  INT_ASSERT(!baData.ctype);
  SET_LINENO(fn);

// Here, 'fcall' is the first of fn's callees and so it acts as a
// representative of all the other callees, if any.
// As of this writing, this should be OK because the callees are
// obtained by duplicating the original call, which resulted in
// outlining a block into 'fn' and so is unique.
// To eliminate 'fcall' in create_arg_bundle_class(), we need
// to rely on fn's formal types instead of fcall's actual types.

  // create a new class to capture refs to locals
  AggregateType* ctype = new AggregateType(AGGREGATE_CLASS);
  TypeSymbol* new_c = new TypeSymbol(astr("_class_locals", fn->name), ctype);
  new_c->addFlag(FLAG_NO_OBJECT);
  new_c->addFlag(FLAG_NO_WIDE_CLASS);

  // Add the runtime header field
  if (fn->hasFlag(FLAG_ON)) {
    VarSymbol* field = new VarSymbol("_runtime_hdr", dtOnBundleRecord);
    ctype->fields.insertAtTail(new DefExpr(field));
  } else {
    VarSymbol* field = new VarSymbol("_runtime_hdr", dtTaskBundleRecord);
    ctype->fields.insertAtTail(new DefExpr(field));
  }

  // add the function args as fields in the class
  int i = 0;    // Fields are numbered for uniqueness.
  for_formals_actuals(formal, arg, fcall) {
    SymExpr *s = toSymExpr(arg);
    Symbol  *var = s->symbol(); // arg or var

    //
    // If we need to do an autoCopy for a BEGIN, and the result is placed on
    // the stack, then we need to copy that result into the arg bundle even
    // if the argument is passed by reference. Otherwise, that copy becomes
    // trash when the caller's scope ends.
    //
    // This was exposed by an unwrapped, privatized _array record. At this point
    // in the compiler we considered it to be a reference counted type, and
    // decided we needed an autoCopy. In the generated code, this would place
    // an integer on the caller's stack. We would then observe that we should
    // pass records by const ref, and take the reference of that local integer.
    //
    bool autoCopy = needsAutoCopyAutoDestroyForArg(formal, arg, fn);

    if (var->isRef() || isClass(var->type)) {
      // Only a variable that is passed by reference out of its current scope
      // is concurrently accessed -- which means that it has to be passed by
      // reference.
      var->addFlag(FLAG_CONCURRENTLY_ACCESSED);
    }

    VarSymbol* field = new VarSymbol(astr("_", istr(i), "_", var->name), var->getValType());

    // Mark error variables with flag, so we can remove them later.
    // the rest of parallel.cpp is less
    // confusing if there's still a slot for them.
    // Error variables aren't actually used in the bundle.
    if (var->hasFlag(FLAG_ERROR_VARIABLE)) {
      field->addFlag(FLAG_ERROR_VARIABLE);
    }

    // If it's a record-wrapped type we can just bit-copy into the arg bundle.
    if (isRecordWrappedType(var->getValType()))
      field->qual = QUAL_VAL;
    // If it's an new 'in' intent to task fn, it should be stored by value
    else if (shouldAddFormalTempAtCallSite(formal, fn))
      field->qual = QUAL_VAL;
    // If we needed to auto-copy it, it should be stored by value
    else if (autoCopy)
      field->qual = QUAL_VAL; // this is a no-op
    // If the actual or the formal is a reference, store a reference
    else if (var->isRef() ||
             // 2018-06: for a record or tuple, if the default intent is used,
             // the formal's type is non-ref and we have been using QUAL_VAL.
             // Whereas with a const ref intent, the formal's type is ref and
             // and we have been setting field->qual = QUAL_REF.
             // TODO: make the behavior consistent in both cases.
             (formal->isRef() && formal->type->isRef()
              // For a coforall index variable, pass by ref only
              // if it is a ref iterator, as indicated by ref-ness of 'var'.
              && !var->hasFlag(FLAG_COFORALL_INDEX_VAR))
            )
      field->qual = QUAL_REF;
    // BHARSH TODO: This really belongs in RVF. Note the sync/single comment
    // in 'needsAutoCopyAutoDestroyForArg'
    // If the formal is constant, store a value
    // This results in passing the actual by value into a by-ref formal,
    // which happens with RVF (for rvfDerefTmp actuals), see ex.
    //   test/multilocale/diten/needMultiLocales/DijkstraTermination.chpl
    else if (formal->isConstant())
      field->qual = QUAL_VAL;
    // Otherwise, if the formal is a reference, store a reference.
    // This is important for on-throw.chpl for example.
    else if (formal->isRef())
      field->qual = QUAL_REF;

    ctype->fields.insertAtTail(new DefExpr(field));
    i++;
  }
  // BTW 'mod' may differ from fn->defPoint->getModule()
  // e.g. due to iterator inlining.
  mod->block->insertAtHead(new DefExpr(new_c));

  baData.ctype = ctype;

  // Also set adjustErrors to 'true' for any nonblocking
  // task/on that throws.
  baData.adjustErrors = fn->throwsError() &&
                        (fn->hasFlag(FLAG_NON_BLOCKING) ||
                         fn->hasFlag(FLAG_BEGIN) ||
                         fn->hasFlag(FLAG_COBEGIN_OR_COFORALL));
}

/// Optionally autoCopies an argument being inserted into an argument bundle.
///
/// These routines optionally inserts an autoCopy ahead of each invocation of a
/// task function that begins asynchronous execution (currently just "begin" and
/// "nonblocking on" functions).
/// If such an autoCopy call is inserted, a matching autoDestroy call is placed
/// at the end of the wrapper function before the call to _downEndCount.
///
/// The insertion of autoCopy calls is required for internally reference-counted
/// types, and also for all user-defined record types (passed by value).  For
/// internally reference-counted types, the autoCopy call increases the
/// reference count, so the internal (reference-counted) data is not reclaimed
/// before the task function exits.  For user-defined record types, the autoCopy
/// call provides a hook so the record author can ensure that the task function
/// owns its own copy of the record (including, but not limited to,
/// reference-counting it).
///
/// arg is an actual argument to the task fn call
/// fn  is the task function that was called
static bool needsAutoCopyAutoDestroyForArg(ArgSymbol* formal, Expr* arg,
                                           FnSymbol* fn)
{
  SymExpr* s        = toSymExpr(arg);
  Symbol*  var      = s->symbol();
  Type*    baseType = arg->getValType();

  // new-style in intents are handled elsewhere
  if (shouldAddFormalTempAtCallSite(formal, fn))
    return false;

  if (!formal->isRef() && isRecord(baseType))
    return true;

  // BHARSH TODO: Move this into RVF. If we do, then we need to handle the
  // following case:
  // ```
  // var a : sync int; // a GLOBAL variable
  // begin {
  //   writeln("inner begin");
  //   on {
  //     writeln("on-stmt");
  //     begin {
  //       a += 1;
  //     }
  //   }
  // }
  // ```
  //
  // This issue was exposed by the following test:
  //   test/multilocale/diten/needMultiLocales/DijkstraTermination.chpl
  //
  // The 'writeln's exist to make sure nothing is considered a begin-on.
  //
  // If the sync is rvf'd onto the on-statement's stack, we should not take
  // a reference to it as it is about to go out of scope with the outer begin.
  //
  // This has the effect of always bit-copying the sync record into the arg
  // bundle. This reduces comm counts and avoids issues with begin-statements
  // where we might take the reference of a sync on the stack, and that stack
  // is about to go away.
  //
  if (isSyncType(baseType) || isSingleType(baseType)) {
    return true;
  }

  // This applies only to arguments being passed to asynchronous task
  // functions. No need to increment+decrement the reference counters
  // for cobegins/coforalls. Except for the index variable for a
  // coforall - since each task needs its own copy.
  // MPF - should this logic also apply to arguments to coforall fns
  // that had the 'in' task intent?
  if (fn->hasFlag(FLAG_BEGIN) ||
      isString(baseType))
  {
    if ((isRecord(baseType) && fn->hasFlag(FLAG_BEGIN)) ||
        (isRecord(baseType) && var->hasFlag(FLAG_COFORALL_INDEX_VAR)))
    {
      if (!formal->isRef())
      {
        return true;
      }
    }
  }

  return false;
}

/// \ret Returns the result of calling autoCopy on the given arg, if necessary;
/// otherwise, just returns arg
static Symbol* insertAutoCopyForTaskArg
  (Expr* arg, ///< The actual argument being passed.
   CallExpr* fcall, ///< The call that invokes the task function.
   FnSymbol* fn) ///< The task function.
{
  SymExpr* s        = toSymExpr(arg);
  Symbol*  var      = s->symbol();
  Type*    baseType = arg->getValType();

  FnSymbol* autoCopyFn = getAutoCopyForType(baseType);

  // Normal (record) handling
  {
    // TODO: Find out why _RuntimeTypeInfo records do not have autoCopy
    // functions, so we can get rid of this special test.
    if (autoCopyFn == NULL) return var;

    // Insert a call to the autoCopy function ahead of the call.
    VarSymbol* valTmp = newTemp(baseType);
    fcall->insertBefore(new DefExpr(valTmp));
    CallExpr* autoCopyCall = new CallExpr(autoCopyFn, var);
    fcall->insertBefore(new CallExpr(PRIM_MOVE, valTmp, autoCopyCall));
    var = valTmp;
  }

  return var;
}

static void insertAutoDestroyForVar(Symbol *arg, FnSymbol* wrap_fn)
{
  Type*     baseType = arg->getValType();
  FnSymbol* autoDestroyFn = getAutoDestroy(baseType);

  if (autoDestroyFn == NULL) return;

  if (arg->isRef())
  {
    // BHARSH: This code used to be special cased for ref counted types.
    // Some changes in support of qualified refs made it possible for syncs
    // to show up here.
    VarSymbol* derefTmp = newTemp(baseType);
    wrap_fn->insertAtTail(new DefExpr(derefTmp));
    wrap_fn->insertAtTail(
      new CallExpr(PRIM_MOVE, derefTmp, new CallExpr(PRIM_DEREF,  arg)));
    arg = derefTmp;
  }

  CallExpr* autoDestroyCall = new CallExpr(autoDestroyFn, arg);
  wrap_fn->insertAtTail(autoDestroyCall);
}

static void
bundleArgs(CallExpr* fcall, BundleArgsFnData &baData) {
  SET_LINENO(fcall);
  ModuleSymbol* mod = fcall->getModule();
  FnSymbol* fn = fcall->resolvedFunction();

  const bool firstCall = baData.firstCall;
  if (firstCall)
    create_arg_bundle_class(fn, fcall, mod, baData);
  AggregateType* ctype = baData.ctype;

  // create the class variable instance and allocate space for it
  VarSymbol *tempc = newTemp(astr("_args_for", fn->name), ctype);
  fcall->insertBefore( new DefExpr( tempc));

  // allocate the argument bundle on the stack
  Expr* alloc = new CallExpr(PRIM_STACK_ALLOCATE_CLASS, ctype->symbol);
  Expr* move = new CallExpr(PRIM_MOVE, tempc, alloc);
  fcall->insertBefore(move);

  // Don't destroy rt hdr.
  baData.needsDestroy.push_back(false);

  // set the references in the class instance
  int i = 1;
  for_formals_actuals(formal, arg, fcall)
  {
    // For anything nonblocking, don't bundle error variables,
    // handle them directly
    if (baData.adjustErrors) {
      if (SymExpr* se = toSymExpr(arg)) {
        if (se->symbol()->hasFlag(FLAG_ERROR_VARIABLE)) {
          baData.needsDestroy.push_back(false);
          i++;
          continue;
        }
      }
    }

    // Insert autoCopy/autoDestroy as needed for "begin" or "nonblocking on"
    // calls (and some other cases).
    Symbol  *var = NULL;
    bool autoCopy = needsAutoCopyAutoDestroyForArg(formal, arg, fn);
    if (autoCopy)
      var = insertAutoCopyForTaskArg(arg, fcall, fn);
    else
      var = toSymExpr(arg)->symbol();

    bool autoDestroy = autoCopy;
    // New "in" intent handling includes destruction in called
    // function (i.e. in the task function not in the wrapper)
    if (autoDestroy)
      autoDestroy = !shouldAddFormalTempAtCallSite(formal, fn);

    baData.needsDestroy.push_back(autoDestroy);

    Symbol* field = ctype->getField(i+1); // +1 for rt header

    if (field->isRef()) {
      VarSymbol* tmp = newTemp(field->qualType());
      fcall->insertBefore(new DefExpr(tmp));
      fcall->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_SET_REFERENCE, var)));
      var = tmp; // use 'tmp' in PRIM_SET_MEMBER
    }

    // Copy the argument into the corresponding slot in the argument bundle.
    CallExpr *setc = new CallExpr(PRIM_SET_MEMBER,
                                  tempc,
                                  field,
                                  var);
    fcall->insertBefore(setc);
    i++;
  }



  // Put the bundle into a void* argument
  VarSymbol *allocated_args = newTemp(astr("_args_vfor", fn->name), dtCVoidPtr);
  fcall->insertBefore(new DefExpr(allocated_args));
  fcall->insertBefore(new CallExpr(PRIM_MOVE, allocated_args,
                               new CallExpr(PRIM_CAST_TO_VOID_STAR, tempc)));

  // Put the size of the bundle into the next argument
  VarSymbol *tmpsz = newTemp(astr("_args_size", fn->name),
                             dtInt[INT_SIZE_DEFAULT]);
  fcall->insertBefore(new DefExpr(tmpsz));
  fcall->insertBefore(new CallExpr(PRIM_MOVE, tmpsz,
                                   new CallExpr(PRIM_SIZEOF_BUNDLE,
                                                ctype->symbol)));

  // Find the _EndCount argument so we can pass that explicitly as the
  // first argument to a task launch function.
  Symbol* endCount = NULL;
  VarSymbol *taskList = NULL;
  VarSymbol *taskListNode = NULL;

  if (!fn->hasFlag(FLAG_ON)) {
    for_actuals(arg, fcall) {

      Type* baseType = arg->getValType();
      if (baseType->symbol->hasFlag(FLAG_END_COUNT)) {
        SymExpr* symexp = toSymExpr(arg);
        endCount = symexp->symbol();

        // Turns out there can be more than one such field. See e.g.
        //   spectests:Task_Parallelism_and_Synchronization/singleVar.chpl
        // INT_ASSERT(endCountField == 0);
        // We have historically picked the first such field.
        break;
      }
    }

    if (!fn->hasFlag(FLAG_BEGIN))
      INT_ASSERT(endCount);

    if (endCount != NULL) {
      // Now get the taskList field out of the end count.

      taskList = newTemp(astr("_taskList", fn->name), QualifiedType(QUAL_REF, dtCVoidPtr));

      // If the end count is a reference, dereference it.
      // EndCount is a class.
      if (endCount->isRef()) {
        VarSymbol *endCountDeref = newTemp(astr("_end_count_deref", fn->name),
                                           endCount->getValType());
        fcall->insertBefore(new DefExpr(endCountDeref));
        fcall->insertBefore(new CallExpr(PRIM_MOVE, endCountDeref,
                                           new CallExpr(PRIM_DEREF, endCount)));
        endCount = endCountDeref;
      }

      fcall->insertBefore(new DefExpr(taskList));
      fcall->insertBefore(new CallExpr(PRIM_MOVE, taskList,
                                       new CallExpr(PRIM_GET_MEMBER,
                                                    endCount,
                                                    endCount->typeInfo()->getField("taskList"))));


      // Now get the node ID field for the end count,
      // which is where the task list is stored.
      taskListNode = newTemp(astr("_taskListNode", fn->name), dtInt[INT_SIZE_DEFAULT]);
      fcall->insertBefore(new DefExpr(taskListNode));
      fcall->insertBefore(new CallExpr(PRIM_MOVE, taskListNode,
                                       new CallExpr(PRIM_WIDE_GET_NODE,
                                                    endCount)));
    }
  }

  // create wrapper-function that uses the class instance
  if (firstCall)
    create_block_fn_wrapper(fn, fcall, baData);

  // call wrapper-function
  call_block_fn_wrapper(fn, fcall, allocated_args, tmpsz, tempc, baData.wrap_fn, taskList, taskListNode);
  baData.firstCall = false;
}

static CallExpr* helpFindDownEndCount(BlockStmt* block)
{
  Expr* cur = block->body.last();

  while (cur && (isCallExpr(cur) || isDefExpr(cur) || isBlockStmt(cur))) {
    if (CallExpr* call = toCallExpr(cur)) {
      if (call->isResolved())
        if (call->resolvedFunction()->hasFlag(FLAG_DOWN_END_COUNT_FN))
          return call;
    } else if (BlockStmt* inner = toBlockStmt(cur)) {
      // Need to handle local blocks since the compiler
      // sometimes places end count inside one.

      // Stop for any actual loop
      if (block->isLoopStmt()) break;

      // Check inside the block.
      CallExpr* got = helpFindDownEndCount(inner);
      if (got) return got;
    }

    cur = cur->prev;
  }

  return NULL;
}

// Finds downEndCount CallExpr or returns NULL.
CallExpr* findDownEndCount(FnSymbol* fn)
{
  return helpFindDownEndCount(fn->body);
}

static bool isGetDynamicEndCount(CallExpr* call)
{
  if (call->isPrimitive(PRIM_GET_DYNAMIC_END_COUNT))
    return true;
  FnSymbol* fn = call->resolvedFunction();
  if (fn && fn == gGetDynamicEndCount)
    return true;

  return false;
}

// TODO: Could we represent the AST differently so that
// the work of this pass is easier? E.g. don't add downEndCount until
// this point?

// Returns the EndCount variable used in the wrapper.
static
void moveDownEndCountToWrapper(FnSymbol* fn, FnSymbol* wrap_fn, Symbol* wrap_c, AggregateType* ctype, Symbol* error)
{
  if (fn->hasFlag(FLAG_NON_BLOCKING) ||
      fn->hasEitherFlag(FLAG_BEGIN, FLAG_COBEGIN_OR_COFORALL)) {
    CallExpr* downEndCount = findDownEndCount(fn);
    // We should have found a downEndCount for non-blocking task/on fns
    INT_ASSERT(downEndCount);

    // Move the downEndCount to the wrapper function.

    FnSymbol* downEndCountFn = downEndCount->resolvedFunction();

    if (downEndCount->numActuals() == 1) {
      // Call downEndCount in the wrapper.
      CallExpr* call = new CallExpr(downEndCountFn);
      if (error != NULL)
        call->insertAtTail(error);
      else
        call->insertAtTail(gNil);

      wrap_fn->insertAtTail(call);

      // Remove downEndCount from the task fn since it
      // is now in the wrapper.
      downEndCount->remove();

      return;
    }

    Expr* endCountTmp = downEndCount->get(1);

    QualifiedType endCountType = endCountTmp->qualType();

    Expr* cur = downEndCount->prev;
    ArgSymbol* whichArg = NULL;
    bool getDynamicEndCount = false;
    // Which argument is passed to the downEndCount?
    // Or is it gotten dynamically?
    // This loop is meant to handle control-free regions only.
    while (true) {
      SymExpr* se = toSymExpr(endCountTmp);
      if (ArgSymbol* arg = toArgSymbol(se->symbol())) {
        whichArg = arg;
        break;
      }
      if (cur == NULL)
        break; // out of AST
      if (CallExpr* call = toCallExpr(cur))
        if (call->isPrimitive(PRIM_MOVE))
          if (SymExpr* dst = toSymExpr(call->get(1)))
            if (dst->symbol() == se->symbol()) {
              if (SymExpr* src = toSymExpr(call->get(2)))
                endCountTmp = src;
              else if (CallExpr* subcall = toCallExpr(call->get(2))) {
                if (subcall->isPrimitive(PRIM_DEREF))
                  endCountTmp = subcall->get(1);
                else if (isGetDynamicEndCount(subcall)) {
                  getDynamicEndCount = true;
                  break;
                }
              }
            }
      cur = cur->prev;
    }

    INT_ASSERT(whichArg != NULL || getDynamicEndCount == true);

    VarSymbol* localEndCount = NULL;

    if (whichArg != NULL) {
      // figure out which arg is the i'th arg
      int i = 1;
      for_formals(formal, fn) {
        if (formal == whichArg) break;
        i++;
      }
      INT_ASSERT(i <= fn->numFormals());
      // Now get the i'th class member. It should be an end count.
      // Change that to the downEndCount call.

      Symbol *field = ctype->getField(i+1); // +1 for rt header
      INT_ASSERT(field->getValType()->symbol->hasFlag(FLAG_END_COUNT));

      localEndCount = newTemp("endcount", field->qualType());
      wrap_fn->insertAtTail(new DefExpr(localEndCount));
      wrap_fn->insertAtTail(
          new CallExpr(PRIM_MOVE, localEndCount,
          new CallExpr(PRIM_GET_MEMBER_VALUE, wrap_c, field)));

      if (field->isRef()) {
        VarSymbol* derefTmp = newTemp("endcountDeref", field->type->getValType());
        wrap_fn->insertAtTail(new DefExpr(derefTmp));
        wrap_fn->insertAtTail(
            new CallExpr(PRIM_MOVE, derefTmp,
            new CallExpr(PRIM_DEREF, localEndCount)));

        localEndCount = derefTmp;
      }
    } else if (getDynamicEndCount == true) {
      localEndCount = newTemp("endcount", endCountType);
      wrap_fn->insertAtTail(new DefExpr(localEndCount));
      wrap_fn->insertAtTail(
          new CallExpr(PRIM_MOVE, localEndCount,
            new CallExpr(PRIM_GET_DYNAMIC_END_COUNT)));
    } else {
      INT_FATAL("error");
    }

    // Call downEndCount in the wrapper.
    CallExpr* call = new CallExpr(downEndCountFn, localEndCount);
    if (error != NULL)
      call->insertAtTail(error);
    else
      call->insertAtTail(gNil);

    wrap_fn->insertAtTail(call);

    // Remove downEndCount from the task fn since it
    // is now in the wrapper.
    downEndCount->remove();

    INT_ASSERT(localEndCount != NULL);
  }
}

static void create_block_fn_wrapper(FnSymbol* fn, CallExpr* fcall, BundleArgsFnData &baData)
{
  ModuleSymbol* mod = fcall->getModule();
  INT_ASSERT(fn == fcall->resolvedFunction());

  AggregateType* ctype = baData.ctype;
  FnSymbol *wrap_fn = new FnSymbol( astr("wrap", fn->name));

  Symbol* error = NULL;

  // Create error variable, but not for blocking on statement
  // This function will handle errors for non-blocking tasks/on
  if (baData.adjustErrors) {
    // Create an error variable
    error = newTemp("error", dtError);
    wrap_fn->insertAtTail(new DefExpr(error));
  }

  // Add a special flag to the wrapper-function as appropriate.
  // These control aspects of code generation.
  if (fn->hasFlag(FLAG_ON))                     wrap_fn->addFlag(FLAG_ON_BLOCK);
  if (fn->hasFlag(FLAG_NON_BLOCKING))           wrap_fn->addFlag(FLAG_NON_BLOCKING);
  if (fn->hasFlag(FLAG_COBEGIN_OR_COFORALL))    wrap_fn->addFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK);
  if (fn->hasFlag(FLAG_BEGIN))                  wrap_fn->addFlag(FLAG_BEGIN_BLOCK);
  if (fn->hasFlag(FLAG_LOCAL_ON))               wrap_fn->addFlag(FLAG_LOCAL_ON);

  if (fn->hasFlag(FLAG_ON)) {
    // The wrapper function for 'on' block has an additional argument, which
    // passes the new wide locale pointer to the fork function.
    // This argument is stripped from the wrapper function during code generation.
    // As far as the compiler knows, the call looks like:
    //  wrapon_fn(new_locale, wrapped_args)
    // and the wrapon_fn has a matching signature.  But at codegen time, this is
    // translated to:
    //  fork(new_locale.locale.node, wrapon_fn, wrapped_args)
    // The fork function effective generates the call
    //  wrapon_fn(wrapped_args)
    // (without the locale arg).

    // The locale arg is originally attached to the on_fn, but we copy it
    // into the wrapper here, and then later on remove it completely.
    // The on_fn does not need this extra argument, and can find out its locale
    // by reading the task-private "here" pointer.
    ArgSymbol *localeArg = fn->getFormal(1)->copy();
    // The above copy() used to be a remove(), based on the assumption that there was
    // exactly one wrapper for each on.  Now, the on_fn is outlined early and has
    // several callers, therefore severall wrapon_fns are generated.
    // So, we leave the extra locale arg in place here and remove it later
    // (see the last if (fn->hasFlag(FLAG_ON)) clause in passArgsToNestedFns()).
    localeArg->addFlag(FLAG_NO_CODEGEN);
    wrap_fn->insertFormalAtTail(localeArg);
  } else {
    // create a task list argument.
    ArgSymbol *taskListArg = new ArgSymbol( INTENT_IN, "dummy_taskList",
                                            dtCVoidPtr->refType );
    taskListArg->addFlag(FLAG_NO_CODEGEN);
    wrap_fn->insertFormalAtTail(taskListArg);
    ArgSymbol *taskListNode = new ArgSymbol( INTENT_IN, "dummy_taskListNode",
                                             dtInt[INT_SIZE_DEFAULT]);
    taskListNode->addFlag(FLAG_NO_CODEGEN);
    wrap_fn->insertFormalAtTail(taskListNode);
  }

  ArgSymbol *allocated_args = new ArgSymbol( INTENT_IN, "buf", dtCVoidPtr);
  wrap_fn->insertFormalAtTail(allocated_args);
  allocated_args->addFlag(FLAG_NO_CODEGEN);
  ArgSymbol *allocated_sz = new ArgSymbol( INTENT_IN, "buf_size",  dtInt[INT_SIZE_DEFAULT]);
  allocated_sz->addFlag(FLAG_NO_CODEGEN);
  wrap_fn->insertFormalAtTail(allocated_sz);
  ArgSymbol *wrap_c = new ArgSymbol( INTENT_IN, "c", ctype);
  //wrap_c->addFlag(FLAG_NO_CODEGEN);
  wrap_fn->insertFormalAtTail(wrap_c);

  mod->block->insertAtTail(new DefExpr(wrap_fn));

  // Add start fence to wrapper if it was requested
  // This supports --cache-remote and is set in createTaskFunctions
  if (fn->hasFlag(FLAG_WRAPPER_NEEDS_START_FENCE)) {
    wrap_fn->addFlag(FLAG_WRAPPER_NEEDS_START_FENCE); // helps optimizeOnClauses
    wrap_fn->insertAtTail(new CallExpr(PRIM_START_RMEM_FENCE));
  }

  // Create a call to the original function
  CallExpr *call_orig = new CallExpr(fn);
  bool first = true;
  int i = 0;
  for_fields(field, ctype)
  {
    // Skip runtime header
    if (i > 0) {
      if (error != NULL && field->hasFlag(FLAG_ERROR_VARIABLE)) {
        // Add the error argument
        INT_ASSERT(error != NULL);
        call_orig->insertAtTail(error);
      } else {
        // insert args
        VarSymbol* tmp = newTemp(field->name, field->qualType());
        wrap_fn->insertAtTail(new DefExpr(tmp));
        wrap_fn->insertAtTail(
            new CallExpr(PRIM_MOVE, tmp,
            new CallExpr(PRIM_GET_MEMBER_VALUE, wrap_c, field)));

        // Special case:
        // If this is an on block, remember the first field,
        // but don't add to the list of actuals passed to the original on_fn.
        // It contains the locale on which the new task is launched.
        if (first && fn->hasFlag(FLAG_ON))
          /* no-op */;
        else
          call_orig->insertAtTail(tmp);

        first = false;
      }
    }

    i++;
  }

  wrap_fn->retType = dtVoid;
  wrap_fn->insertAtTail(call_orig);     // add new call

  // Destroy any fields that we should be destroying.
  i = 0;
  for_fields(field, ctype)
  {
    // Skip runtime header
    if (i > 0 && baData.needsDestroy[i]) {
      // insert auto destroy calls
      VarSymbol* tmp = newTemp(field->name, field->qualType());
      wrap_fn->insertAtTail(new DefExpr(tmp));
      wrap_fn->insertAtTail(
          new CallExpr(PRIM_MOVE, tmp,
          new CallExpr(PRIM_GET_MEMBER_VALUE, wrap_c, field)));

      insertAutoDestroyForVar(tmp, wrap_fn);
    }

    i++;
  }

  // Move the downEndCount at the tail of fn, if any,
  // to the wrapper.
  moveDownEndCountToWrapper(fn, wrap_fn, wrap_c, ctype, error);

  // Add finish fence to wrapper if it was requested
  // This supports --cache-remote and is set in createTaskFunctions
  if (fn->hasFlag(FLAG_WRAPPER_NEEDS_FINISH_FENCE)) {
    wrap_fn->addFlag(FLAG_WRAPPER_NEEDS_FINISH_FENCE); // helps optimizeOnClauses
    wrap_fn->insertAtTail(new CallExpr(PRIM_FINISH_RMEM_FENCE));
  }

  wrap_fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  // 'fn' has already been flattened and hoisted to the top level.
  // We leave 'fn' in the module where it was placed originally,
  // whereas 'wrap_fn' is in fcall's module.
  // These two modules may be different, e.g. due to iterator inlining.
  INT_ASSERT(isGlobal(fn));

  baData.wrap_fn = wrap_fn;
}

static void call_block_fn_wrapper(FnSymbol* fn, CallExpr* fcall, VarSymbol*
    args_buf, VarSymbol* args_buf_len, VarSymbol* tempc, FnSymbol *wrap_fn,
    Symbol* taskList, Symbol* taskListNode)
{
  // The wrapper function is called with the bundled argument list.
  if (fn->hasFlag(FLAG_ON)) {
    // For an on block, the first argument is also passed directly
    // to the wrapper function.
    // The forking function uses this to fork a task on the target locale.
    fcall->insertBefore(new CallExpr(wrap_fn, fcall->get(1)->remove(), args_buf, args_buf_len, tempc));
  } else {
    // For non-on blocks, the task list is passed directly to the function
    // (so that codegen can find it).
    // We need the taskList.
    INT_ASSERT(taskList);
    fcall->insertBefore(new CallExpr(wrap_fn, new SymExpr(taskList), new SymExpr(taskListNode), args_buf, args_buf_len, tempc));
  }

  fcall->remove();                     // rm orig. call
}


static void
insertEndCount(FnSymbol* fn,
               Type* endCountType,
               Vec<FnSymbol*>& queue,
               Map<FnSymbol*,Symbol*>& endCountMap) {
  if (fn == chpl_gen_main) {
    VarSymbol* var = newTemp("_endCount", endCountType);
    fn->insertAtHead(new DefExpr(var));
    endCountMap.put(fn, var);
    queue.add(fn);
  } else {
    ArgSymbol* arg = new ArgSymbol(INTENT_CONST_IN, "_endCount", endCountType);
    fn->insertFormalAtTail(arg);
    VarSymbol* var = newTemp("_endCount", endCountType);
    fn->insertAtHead(new CallExpr(PRIM_MOVE, var, arg));
    fn->insertAtHead(new DefExpr(var));
    endCountMap.put(fn, var);
    queue.add(fn);
  }
}

static void insertBroadcast(Expr* beforeExpr, Symbol* sym) {
  // Disable serialized broadcasting for internal globals until have a way of
  // finding the point where the runtime is sufficiently 'set up' such that
  // we can use the Locales array.
  bool isInternalModule = sym->getModule()->modTag == MOD_INTERNAL;

  bool isVariableRecordWrappedType = isRecordWrappedType(sym->getValType()) && sym->hasFlag(FLAG_CONST) == false;

  if (isInternalModule == false &&
      isVariableRecordWrappedType == false &&
      serializeMap.find(sym->type) != serializeMap.end()) {

    Serializers ser       = serializeMap[sym->type];
    FnSymbol* broadcastFn = ser.broadcaster;
    FnSymbol* destroyFn   = ser.destroyer;
    INT_ASSERT(broadcastFn != NULL && destroyFn != NULL);

    VarSymbol* broadcastID = new_IntSymbol(broadcastGlobalID++);
    beforeExpr->insertAfter(new CallExpr(broadcastFn, sym, broadcastID));

    FnSymbol* autoDestroyFn = autoDestroyMap.get(sym->type);

    if (autoDestroyFn != NULL) {
      Expr* destroyExpr = NULL;

      // We expect to find an autoDestroy call inserted by callDestructors'
      // 'insertGlobalAutoDestroyCalls' function.
      for_SymbolSymExprs(se, sym) {
        CallExpr* call = toCallExpr(se->parentExpr);
        if (call && call->isResolved()) {
          FnSymbol* fn = call->resolvedFunction();
          if (fn == autoDestroyFn) {
            // Fail if multiple destroys are found
            INT_ASSERT(destroyExpr == NULL);
            destroyExpr = call->getStmtExpr();
          }
        }
      }
      INT_ASSERT(destroyExpr != NULL);

      // Destroy broadcasted copies before the original.
      destroyExpr->insertBefore(new CallExpr(destroyFn, sym, broadcastID));
    }
  } else {
    beforeExpr->insertAfter(new CallExpr(PRIM_PRIVATE_BROADCAST, sym));
  }
}

static AggregateType*
buildHeapType(Type* type) {
  static Map<Type*,AggregateType*> heapTypeMap;
  if (heapTypeMap.get(type))
    return heapTypeMap.get(type);

  SET_LINENO(type->symbol);
  AggregateType* heap = new AggregateType(AGGREGATE_CLASS);
  TypeSymbol* ts = new TypeSymbol(astr("heap_", type->symbol->cname), heap);
  ts->addFlag(FLAG_NO_OBJECT);
  ts->addFlag(FLAG_HEAP);
  theProgram->block->insertAtTail(new DefExpr(ts));
  heap->fields.insertAtTail(new DefExpr(new VarSymbol("value", type)));
  heapTypeMap.put(type, heap);
  return heap;
}


//
// In the following, through makeHeapAllocations():
//   varSet, varVec - symbols that themselves need to be heap-allocated
//

// Traverses all DefExprs.
//  If the symbol is not of primitive type or other undesired cases,
//    Add it to varSet and varVec.
//  Otherwise, select module-level vars that are not private or extern.
//   If the var is const and has value semantics except record-wrapped types,
//    Insert a prim_private_broadcast call after the def.
//   Otherwise, if it is a record-wrapped type, replicate it.
//   Otherwise,
//    Add it to varSet and varVec, so it will be put on the heap.
static void findHeapVarsAndRefs(Map<Symbol*, Vec<SymExpr*>*>& defMap,

                                Vec<Symbol*>& varSet,
                                Vec<Symbol*>& varVec)
{
  forv_Vec(DefExpr, def, gDefExprs) {
    SET_LINENO(def);

    // BHARSH TODO: Add a check to only continue if def is not a reference
    if (!fLocal                                 &&
        isModuleSymbol(def->parentSymbol)       &&
        def->parentSymbol != rootModule         &&
        isVarSymbol(def->sym)                   &&
        !def->sym->hasFlag(FLAG_TEMP)           &&
        !def->sym->hasFlag(FLAG_LOCALE_PRIVATE) &&
        !def->sym->hasFlag(FLAG_EXTERN)) {
      if (def->sym->hasFlag(FLAG_CONST) &&
          (is_bool_type(def->sym->type)    ||
           is_enum_type(def->sym->type)    ||
           is_int_type(def->sym->type)     ||
           is_uint_type(def->sym->type)    ||
           is_real_type(def->sym->type)    ||
           is_imag_type(def->sym->type)    ||
           is_complex_type(def->sym->type) ||
           (isRecord(def->sym->type)             &&
            !isRecordWrappedType(def->sym->type) &&
            !isSyncType(def->sym->type)          &&
            !isSingleType(def->sym->type)        &&
            // Dont try to broadcast string literals, they'll get fixed in
            // another manner
            !(def->sym->type == dtString && def->sym->isImmediate())))) {

        // replicate global const of primitive/record type

        Expr* initialization = def->sym->getInitialization();

        INT_ASSERT(initialization);
        insertBroadcast(initialization, def->sym);

      } else if (isRecordWrappedType(def->sym->type)) {
        // replicate address of global arrays, domains, and distributions
        Expr* initialization = def->sym->getInitialization();

        INT_ASSERT(initialization);
        insertBroadcast(initialization, def->sym);

      } else {
        // put other global constants and all global variables on the heap
        // ... but not type variables without a runtime type component
        Symbol* sym = def->sym;
        if (sym->hasFlag(FLAG_HAS_RUNTIME_TYPE) ||
            !sym->hasFlag(FLAG_TYPE_VARIABLE)) {
          // Don't add type variables without runtime type
          varSet.set_add(sym);
          varVec.add(sym);
        }
      }
    }
  }
}


static void
makeHeapAllocations() {
  Vec<Symbol*> varSet;
  Vec<Symbol*> varVec;

  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap);

  findHeapVarsAndRefs(defMap, varSet, varVec);

  forv_Vec(Symbol, var, varVec) {
    // MPF: I'm disabling the below assert because PR #5692
    // can create call_tmp variables that are refs. Since these
    // are temps, they aren't marked with FLAG_REF_VAR.
    //INT_ASSERT(var->hasFlag(FLAG_REF_VAR) || !var->isRef());

    if (var->hasFlag(FLAG_EXTERN)) {
      // don't widen external variables
      continue;
    }

    if (var->hasFlag(FLAG_PRINT_MODULE_INIT_INDENT_LEVEL)) {
      // don't widen PrintModuleInitOrder variables
      continue;
    }

    if (isModuleSymbol(var->defPoint->parentSymbol)) {
      if (!requireWideReferences()) {
        // don't heap-allocate globals
        continue;
      }
    }

    if (isString(var) && var->isImmediate()) {
      // String immediates are privatized; do not widen them
      continue;
    }

    SET_LINENO(var);

    AggregateType* heapType = buildHeapType(var->type);

    for_defs(def, defMap, var) {
      if (CallExpr* call = toCallExpr(def->parentExpr)) {
        SET_LINENO(call);
        if (call->isPrimitive(PRIM_MOVE)) {
          VarSymbol* tmp = newTemp(var->type);
          call->insertBefore(new DefExpr(tmp));
          call->insertBefore(new CallExpr(PRIM_MOVE, tmp, call->get(2)->remove()));
          call->replace(new CallExpr(PRIM_SET_MEMBER, call->get(1)->copy(), heapType->getField(1), tmp));
        } else if (call->isPrimitive(PRIM_ASSIGN)) {
          // ensure what we assign into is what we expect
          INT_ASSERT(toSymExpr(call->get(1))->symbol() == var);
          VarSymbol* tmp = newTemp(var->qualType().toRef());
          call->insertBefore(new DefExpr(tmp));
          call->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, var, heapType->getField(1))));
          def->replace(new SymExpr(tmp));
        } else if (call->isResolved()) {
            ArgSymbol* formal = actual_to_formal(def);
            if (formal->isRef()) {
              VarSymbol* tmp = newTemp(var->type);
              tmp->qual = QUAL_REF;
              PrimitiveTag op = PRIM_GET_MEMBER;
              if (heapType->getField(1)->isRef()) {
                op = PRIM_GET_MEMBER_VALUE;
              }
              call->getStmtExpr()->insertBefore(new DefExpr(tmp));
              call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(op, def->symbol(), heapType->getField(1))));
              def->replace(new SymExpr(tmp));
            }
        } else {
          VarSymbol* tmp = newTemp(var->type);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, def->symbol(), heapType->getField(1))));
          def->replace(new SymExpr(tmp));
        }
      } else
        INT_FATAL(var, "unexpected case");
    }

    for_uses(use, useMap, var) {
      if (CallExpr* call = toCallExpr(use->parentExpr)) {
        if (call->isPrimitive(PRIM_ADDR_OF) || call->isPrimitive(PRIM_SET_REFERENCE)) {
          CallExpr* move = toCallExpr(call->parentExpr);
          INT_ASSERT(move && (move->isPrimitive(PRIM_MOVE)));
          if (move->get(1)->typeInfo() == heapType) {
            call->replace(use->copy());
          } else {
            call->replace(new CallExpr(PRIM_GET_MEMBER, use->symbol(), heapType->getField(1)));
          }
        } else if (call->isResolved()) {
          ArgSymbol* formal = actual_to_formal(use);
          if (formal->type != heapType) {

            if (formal->isRef()) {
              // Handle value arguments passed by blank intent where
              // blank intent is 'const ref' without making a copy.
              // Instead, pass the address of the heap-allocated variable.

              // TODO  - this code is almost the same as the
              // def case above..
              VarSymbol* tmp = newTemp(var->type);
              tmp->qual = QUAL_REF;
              PrimitiveTag op = PRIM_GET_MEMBER;
              if (heapType->getField(1)->isRef()) {
                op = PRIM_GET_MEMBER_VALUE;
              }
              call->getStmtExpr()->insertBefore(new DefExpr(tmp));
              call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(op, use->symbol(), heapType->getField(1))));
              use->replace(new SymExpr(tmp));
            } else {
              // formal takes in argument by value, so read from the
              // heap-allocated global.
              VarSymbol* tmp = newTemp(var->type);

              call->getStmtExpr()->insertBefore(new DefExpr(tmp));
              call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, use->symbol(), heapType->getField(1))));
              use->replace(new SymExpr(tmp));
            }
          }
        } else if ((call->isPrimitive(PRIM_GET_MEMBER) ||
                    call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                    call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                    call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) ||
                    call->isPrimitive(PRIM_WIDE_GET_LOCALE) || //I'm not sure this is cricket.
                    call->isPrimitive(PRIM_WIDE_GET_NODE) ||// what member are we extracting?
                    call->isPrimitive(PRIM_SET_SVEC_MEMBER) ||
                    call->isPrimitive(PRIM_SET_MEMBER)) &&
                   call->get(1) == use) {
          VarSymbol* tmp = newTemp(var->qualType().toRef());
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          PrimitiveTag op = PRIM_GET_MEMBER;
          if (heapType->getField(1)->isRef()) {
            op = PRIM_GET_MEMBER_VALUE;
          }
          call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(op, use->symbol(), heapType->getField(1))));
          use->replace(new SymExpr(tmp));
        } else {
          VarSymbol* tmp = newTemp(var->type);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, use->symbol(), heapType->getField(1))));
          use->replace(new SymExpr(tmp));
        }
      } else if (use->parentExpr)
        INT_FATAL(var, "unexpected case");
    }

    var->type = heapType;
    var->qual = QUAL_VAL;
  }
}


//
// A helper function for replaceRecordWrappedRefs that updates the type and
// Qualifier for the LHS of the move and adds it to a list of Symbols whose
// uses may need updating.
//
static void fixLHS(CallExpr* move, std::vector<Symbol*>& todo) {
  Symbol* LHS = toSymExpr(move->get(1))->symbol();
  if (LHS->isRef()) {
    LHS->type = LHS->getValType();
    LHS->qual = QUAL_VAL;
    todo.push_back(LHS);
  }
}


//
// Replaces reference fields to record-wrapped types with QUAL_VAL fields of
// the same type. This transformation results in less communication as these
// record-wrapped fields will be bulk-copied across the network such that
// accessing the 'pid' or '_instance' fields will be a local operation.
//
// This is valid because the record-wrapped types (e.g. _array) are immutable.
// Currently the difference between a ref-array and a val-array is used by the
// compiler to determine when to copy or destroy these objects. This logic is
// handled in callDestructors, so by this point the distinction is no longer
// important.
//
// After the fields are transformed, a number of primitives may be used
// incorrectly. For example, PRIM_GET_MEMBER_VALUE will return a reference if
// the field is a reference. After this transformation this primitive will
// return a QUAL_VAL, meaning the LHS of the parent PRIM_MOVE should also
// become a QUAL_VAL.
//
static void replaceRecordWrappedRefs() {
  std::vector<Symbol*> todo;

  // Changes reference fields with a record-wrapped type into value fields.
  // Note that this will modify arg bundle classes.
  forv_Vec(AggregateType, aggType, gAggregateTypes) {
    if (!aggType->symbol->hasFlag(FLAG_REF)) {
      for_fields(field, aggType) {
        if (field->isRef() && isRecordWrappedType(field->getValType())) {
          field->type = field->getValType();
          field->qual = QUAL_VAL;
          todo.push_back(field);
        }
      }
    }
  }

  // I'd like to be able to just iterate over the uses of tuple fields, but
  // we don't have a good way of doing that today. The case to worry about
  // is when we're indexing into a tuple with an integer (param or otherwise).
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
      CallExpr* move = toCallExpr(call->parentExpr);
      INT_ASSERT(isMoveOrAssign(move));

      if (!call->isRef()) {
        INT_ASSERT(isRecordWrappedType(call->typeInfo()->getValType()));
        fixLHS(move, todo);
      }
    }
  }

  // Try and find references that were initialized with the field reference,
  // and fix them to be QUAL_VAL
  //
  // These primitives were chosen because without fixing them we would see
  // testing failures.
  //
  // Currently it is not necessary to insert a PRIM_SET_REFERENCE when passing
  // a value to a reference-formal because codegen will handle that implicitly.
  // BHARSH TODO: I'm not sure if that's the desired behavior, but that's what
  // was done for the initial qualified refs merge.
  //
  while (todo.size() > 0) {
    Symbol* sym = todo.back();
    todo.pop_back();
    INT_ASSERT(!sym->isRef() && isRecordWrappedType(sym->type));

    for_SymbolSymExprs(se, sym) {
      CallExpr* call = toCallExpr(se->parentExpr);
      INT_ASSERT(call);

      if (call->isPrimitive(PRIM_MOVE)) {
        if (se == call->get(2)) {
          fixLHS(call, todo);
        }
      }
      else if (call->isPrimitive(PRIM_DEREF)) {
        SET_LINENO(call);
        call->replace(se->copy());
      }
      else if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
        if (se == call->get(2)) {
          CallExpr* move = toCallExpr(call->parentExpr);
          INT_ASSERT(isMoveOrAssign(move));
          fixLHS(move, todo);
        }
      }
      else if (call->isPrimitive(PRIM_RETURN)) {
        FnSymbol* fn = toFnSymbol(call->parentSymbol);
        INT_ASSERT(fn);
        fn->retType = sym->type;

        forv_Vec(CallExpr, fncall, *fn->calledBy) {
          CallExpr* move = toCallExpr(fncall->parentExpr);
          if (move && isMoveOrAssign(move)) {
            fixLHS(move, todo);
          }
        }
      }
    }
  }
}


void parallel() {
  compute_call_sites();

  replaceRecordWrappedRefs();

  remoteValueForwarding();

  if (requireOutlinedOn()) {
    makeHeapAllocations();
  }

  insertEndCounts();

  passArgsToNestedFns();

  // Lower error handling check error primitives
  // now that callDestructors and parallel have
  // had the benefit of more straightforward error-handling AST.
  lowerCheckErrorPrimitive();
}


/* Lowers PRIM_GET_END_COUNT / PRIM_SET_END_COUNT for
   managing the end-counts for cobegin and coforall.

   End counts for sync/begin are managed by
   PRIM_GET_DYNAMIC_END_COUNT / PRIM_SET_DYNAMIC_END_COUNT.
   These are lowered here in to the corresponding calls.
 */
static void insertEndCounts()
{
  Vec<FnSymbol*> queue;
  Map<FnSymbol*,Symbol*> endCountMap;

  forv_Vec(CallExpr, call, gCallExprs) {
    SET_LINENO(call);
    if (call->isPrimitive(PRIM_GET_END_COUNT)) {
      FnSymbol* pfn = call->getFunction();
      if (!endCountMap.get(pfn))
        insertEndCount(pfn, call->typeInfo(), queue, endCountMap);
      call->replace(new SymExpr(endCountMap.get(pfn)));
    } else if (call->isPrimitive(PRIM_SET_END_COUNT)) {
      FnSymbol* pfn = call->getFunction();
      if (!endCountMap.get(pfn))
        insertEndCount(pfn, call->get(1)->typeInfo(), queue, endCountMap);
      INT_ASSERT(call->numActuals() >= 1);
      call->replace(new CallExpr(PRIM_MOVE, endCountMap.get(pfn), call->get(1)->remove()));
    } else if (call->isPrimitive(PRIM_GET_DYNAMIC_END_COUNT)) {
      call->replace(new CallExpr(gGetDynamicEndCount));
    } else if (call->isPrimitive(PRIM_SET_DYNAMIC_END_COUNT)) {
      INT_ASSERT(call->numActuals() >= 1);
      call->replace(new CallExpr(gSetDynamicEndCount, call->get(1)->remove()));
    }
  }

  forv_Vec(FnSymbol, fn, queue) {
    forv_Vec(CallExpr, call, *fn->calledBy) {
      SET_LINENO(call);
      Type* endCountType = endCountMap.get(fn)->type;
      FnSymbol* pfn = call->getFunction();
      if (!endCountMap.get(pfn))
        insertEndCount(pfn, endCountType, queue, endCountMap);
      Symbol* endCount = endCountMap.get(pfn);
      call->insertAtTail(endCount);
    }
  }
}

// Generate an if statement
// if ( chpl_doDirectExecuteOn( targetLocale ) )
//     call un-wrapped task function
// else
//     proceed as with chpl_executeOn/chpl_executeOnFast
//     fid call to wrapper function on a remote local
//     (possibly just a different sublocale)
//
// This is called the "direct on" optimization.
//
// Returns the call in the 'else' block (which will need
// argument bundling as usual).
static
CallExpr* createConditionalForDirectOn(CallExpr* call, FnSymbol* fn)
{

  VarSymbol* isDirectTmp = newTemp("isdirect", dtBool);
  CallExpr* isCall;
  isCall = new CallExpr(gChplDoDirectExecuteOn,
                        call->get(1)->copy() // target
                       );

  DefExpr* def = new DefExpr(isDirectTmp);
  call->insertBefore(def);

  // Remove call (we will add it back in a conditional)
  call->remove();

  CallExpr* move = new CallExpr(PRIM_MOVE, isDirectTmp, isCall);
  def->insertAfter(move);

  // Build comparison
  SymExpr* isTrue = new SymExpr(isDirectTmp);

  // Build true branch
  CallExpr* directcall = call->copy();

  // Remove the first arg (destination locale)
  // not needed for direct call
  directcall->get(1)->remove();

  // False branch is call.

  // Build condition
  CondStmt* cond = new CondStmt(isTrue, directcall, call);

  // Add the condition which calls the outlined task function somehow.
  move->insertAfter(cond);

  return call;
}

// For each "nested" function created to represent remote execution,
// bundle args so they can be passed through a fork function.
//
// Fork functions in general have the signature
//    fork(int32_t destNode, void (*)(void* args), void* args, ...);
//
// In Chapel, we wrap the arguments passed to the nested function in an object
// whose type is just a list of the arguments passed to the nested function.
// Those arguments consist of variables in the scope of the nested function
// call that are accessed within the body of the nested function (recursively,
// of course).
static void passArgsToNestedFns() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (isTaskFun(fn)) {
      BundleArgsFnData baData = bundleArgsFnDataInit;

      // Would need to flatten them if they are not already.
      INT_ASSERT(isGlobal(fn));

      forv_Vec(CallExpr, call, *fn->calledBy) {
        SET_LINENO(call);

        if (fn->hasFlag(FLAG_ON) && !fn->hasFlag(FLAG_NON_BLOCKING)) {
          // create conditional for direct-on optimization
          call = createConditionalForDirectOn(call, fn);
        }

        bundleArgs(call, baData);
      }

      if (fn->hasFlag(FLAG_ON)) {
        // Now we can remove the dummy locale arg from the on_fn
        DefExpr*              localeArg = toDefExpr(fn->formals.get(1));
        std::vector<SymExpr*> symExprs;

        collectSymExprsFor(fn->body, localeArg->sym, symExprs);

        for_vector(SymExpr, sym, symExprs) {
            sym->getStmtExpr()->remove();
        }

        localeArg->remove();
      }

      // Remove any error fields - they shouldn't be code-generated
      // since the errors are handled directly.
      if (baData.adjustErrors) {
        AggregateType* ctype = baData.ctype;
        Symbol* toRemove = NULL;
        for_fields(field, ctype) {
          if (field->hasFlag(FLAG_ERROR_VARIABLE))
            toRemove = field;
        }
        if (toRemove != NULL) {
          INT_ASSERT(toRemove->firstSymExpr() == NULL);
          toRemove->defPoint->remove();
        }
      }
    }
  }
}


Type* getOrMakeRefTypeDuringCodegen(Type* type) {
  Type* refType;
  // BHARSH TODO: This check causes a failure for the following test:
  //   execflags/tmacd/config_ref
  //
  // For some reason this test wants a reference to a reference. To make
  // progress I'm commenting out the check for now, but I think we'll
  // eventually want it when we complete the qualified refs work.
  //
  // if (type->symbol->hasFlag(FLAG_REF)) return type;
  refType = type->refType;
  if( ! refType ) {
    SET_LINENO(type->symbol);
    AggregateType* ref = new AggregateType(AGGREGATE_RECORD);
    TypeSymbol* refTs = new TypeSymbol(astr("_ref_", type->symbol->cname), ref);
    refTs->addFlag(FLAG_REF);
    refTs->addFlag(FLAG_NO_DEFAULT_FUNCTIONS);
    refTs->addFlag(FLAG_NO_OBJECT);
    theProgram->block->insertAtTail(new DefExpr(refTs));
    ref->fields.insertAtTail(new DefExpr(new VarSymbol("_val", type)));
    refType = ref;
    type->refType = ref;
  }
  return refType;
}

// This function is called if the wide reference type does not already
// exist to cause it to be code generated even though it was not
// needed by earlier passes.
Type* getOrMakeWideTypeDuringCodegen(Type* refType) {
  Type* wideType;
  INT_ASSERT(refType == dtNil ||
             isClass(refType) ||
             refType->symbol->hasFlag(FLAG_REF));
  // First, check if the wide type already exists.
  if( isClass(refType) ) {
    wideType = wideClassMap.get(refType);
    if( wideType ) return wideType;
  }
  // For a ref to a class, isClass seems to return true...
  wideType = wideRefMap.get(refType);
  if( wideType ) return wideType;

  // Now, create a wide pointer type.
  AggregateType* wide = new AggregateType(AGGREGATE_RECORD);
  TypeSymbol* wts = new TypeSymbol(astr("chpl____wide_", refType->symbol->cname), wide);
  if( refType->symbol->hasFlag(FLAG_REF) || refType == dtNil )
    wts->addFlag(FLAG_WIDE_REF);
  else
    wts->addFlag(FLAG_WIDE_CLASS);
  theProgram->block->insertAtTail(new DefExpr(wts));
  wide->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtLocaleID)));
  wide->fields.insertAtTail(new DefExpr(new VarSymbol("addr", refType)));
  if( isClass(refType) ) {
    wideClassMap.put(refType, wide);
  } else {
    wideRefMap.put(refType, wide);
  }
  return wide;
}

