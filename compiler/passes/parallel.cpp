/*
 * Copyright 2004-2015 Cray Inc.
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
#include "expr.h"
#include "files.h"
#include "optimizations.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "CForLoop.h"

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
// - heap allocation for 'begin'
// - change acces to variable -> access to its ._value
// - set up wide references
// - broadcasting of globals
//
// In more details:
//
// Heap allocation for remote access is done:
// - for globals - in IWR, if:
//    requireWideReferences()
// - for a local - in MHA, if:
//    needHeapVars() && the local can be passed to an 'on'
//
// Heap allocation for 'begin' is done:
// - for globals - n/a
//    see above instead
// - for a local - in MHA, if:
//    the local can be passed to a 'begin'
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
} BundleArgsFnData;

// bundleArgsFnDataInit: the initial value for BundleArgsFnData
static BundleArgsFnData bundleArgsFnDataInit = { true, NULL, NULL };

static void insertEndCounts();
static void passArgsToNestedFns(Vec<FnSymbol*>& nestedFunctions);
static void create_block_fn_wrapper(FnSymbol* fn, CallExpr* fcall, BundleArgsFnData &baData);
static void call_block_fn_wrapper(FnSymbol* fn, CallExpr* fcall, VarSymbol* tempc, FnSymbol *wrap_fn);
static void findBlockRefActuals(Vec<Symbol*>& refSet, Vec<Symbol*>& refVec);
static void findHeapVarsAndRefs(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                Vec<Symbol*>& refSet, Vec<Symbol*>& refVec,
                                Vec<Symbol*>& varSet, Vec<Symbol*>& varVec);

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

  // add the function args as fields in the class
  int i = 0;    // Fields are numbered for uniqueness.
  for_actuals(arg, fcall) {
    SymExpr *s = toSymExpr(arg);
    Symbol  *var = s->var; // arg or var
    if (var->type->symbol->hasFlag(FLAG_REF) || isClass(var->type))
      // Only a variable that is passed by reference out of its current scope
      // is concurrently accessed -- which means that it has to be passed by
      // reference.
      var->addFlag(FLAG_CONCURRENTLY_ACCESSED);
    VarSymbol* field = new VarSymbol(astr("_", istr(i), "_", var->name), var->type);
    ctype->fields.insertAtTail(new DefExpr(field));
    i++;
  }
  // BTW 'mod' may differ from fn->defPoint->getModule()
  // e.g. due to iterator inlining.
  mod->block->insertAtHead(new DefExpr(new_c));

  baData.ctype = ctype;
}


//########################################################################
//# Begin cruft
//#
//# The searches and predicates below, up to the matching "End cruft" label are
//# here as compensation for the lack of direct representation in the AST of
//# cobegin and coforall blocks.
//# The implementation of these parallel constructs is rendered very early in
//# translation (in build???() actions in the parser), so they have already
//# been flattened somewhat immediately after parsing.  It would be desirable
//# to use a more high-level representation for these parallel constructs early
//# in translation, and even up to this point.  That would allow the
//# taskMayOutliveTaskArg() predicate to be computed very cheaply, rather than
//# using these somewhat involved tree searches.
//#


// Given a (cobegin or coforall) task function call, find the correponding call
// to _upEndCount().  We assume that the _upEndCount() call is at the same
// level as the task function call and precedes it in the tree.
// TODO: This general utility can be moved to expr.cpp.
static CallExpr*
findSiblingCallPreceding(const char* name, CallExpr* taskFnCall)
{
  Expr* expr = taskFnCall->prev;
  while ((expr = expr->prev))
  {
    if (CallExpr* call = toCallExpr(expr))   // expr is a call
      if (FnSymbol* fn = call->isResolved()) // that calls a function
        if (strcmp(fn->name, name) == 0)     // named <name>.
          return call;
  }

  // Returns NULL if no such call was found.
  return NULL;
}


static BlockStmt*
findTaskScope(CallExpr* taskFnCall)
{
  CallExpr* upEndCountCall = findSiblingCallPreceding("_upEndCount", taskFnCall);
  if (upEndCountCall == NULL)
    INT_FATAL(taskFnCall, "Could not find an _upEndCount call for this task function call");

  // This is the task counter for this parallel construct
  Symbol* count = toSymExpr(upEndCountCall->get(1))->var;

  // We assume that the task count is declared just inside the block
  // representing the parallel construct.
  BlockStmt* taskScope = toBlockStmt(count->defPoint->parentExpr);

  return taskScope;
}


//////////////////////////////////////////////////////////////////////////////
// Returns true if the task function fn may outlive the given task arg (passed
// to it through fcall); false otherwise.
//
// Inside a cobegin or coforall block, execution of the task will not outlive
// the _waitEndCount() call at the end of the block representing the
// corresponding parallel construct.  So, we proceed by finding the block
// containing the declaration of arg and the block containing the task call's
// corresponding _waitEndCount() call.  If the latter block properly contains
// the former, then we return true.
// Viewed another way: Variables with the same scope as the counter used in the
// _waitEndCount() call are guaranteed to outlast all tasks called by the
// cobegin or coforall.
//
static bool
taskMayOutliveTaskArg(Expr* arg, CallExpr* taskFnCall)
{
  // Get the task function.
  FnSymbol* fn = taskFnCall->isResolved();

  // "begin" functions are asynchronous -- no telling when they will terminate
  // relative to their calling context.  Therefore, a copy is always required.
  if (fn->hasFlag(FLAG_BEGIN))
    return true;

  // Ditto for non-blocking "on" functions.
  if (fn->hasFlag(FLAG_NON_BLOCKING))
    return true;

  // For cobegin or coforall functions, we are a bit more picky.
  // We only have to copy the arg if it may be deleted before the corresponding
  // parallel construct is exited.
  if (fn->hasFlag(FLAG_COBEGIN_OR_COFORALL))
  {
    DefExpr* argDef = toSymExpr(arg)->var->defPoint;
    Expr* argScope = toBlockStmt(argDef->parentExpr);
    BlockStmt* taskScope = findTaskScope(taskFnCall);

    if (argScope)
      // Ignore cases where the argument scope is not a containing block.
      if (taskScope->contains(argScope))
        return true;
  }

  return false;
}

//#
//# End cruft
//#
//########################################################################


/// Optionally autoCopies an argument being inserted into an argument bundle.
///
/// This routine optionally inserts an autoCopy ahead of each invocation of a
/// task function where it is needed.  These cases currently include:
///  - All record arguments to a begin or nonblock "on" function; and
///  - Record arguments to a cobegin or coforall task function only if the
///    argument's scope is inside the body of the coforall or cobegin.
///    (See Note #1.)
/// If such an autoCopy call is inserted, a matching autoDestroy call is placed
/// at the end of the tasking routine before the call to _downEndCount.  Since a
/// tasking function may be called from several call sites, the task function is
/// modified only when processing the first invocation.
/// The insertion of autoCopy calls is required for internally reference-counted
/// types, and also for all user-defined record types (passed by value).  For
/// internally reference-counted types, the autoCopy call increases the
/// reference count, so the internal (reference-counted) data is not reclaimed
/// before the task function exits.  For user-defined record types, the autoCopy
/// call provides a hook so the record author can ensure that the task function
/// owns its own copy of the record (including, but not limited to,
/// reference-counting it).
/// \param firstCall Should be set to \p true for the first invocation of a
/// given task function and \p false thereafter.
/// \ret Returns the result of calling autoCopy on the given arg, if necessary;
/// otherwise, just returns 
/// TODO: The autocopy/autodestroy pairs are necessary on coforalls, but
/// probably not cobegins.  We need to split that flag to handle the two cases
/// separately.
/// TODO: This routine is only necessary because the parallel pass is run after
/// insertAutoCopyAutoDestroy().  Try moving that pass after parallel and
/// removing this code.
static Symbol* insertAutoCopyDestroyForTaskArg
  (Expr* arg, ///< The actual argument being passed.
   CallExpr* fcall, ///< The call that invokes the task function.
   bool firstCall)
{
  SymExpr* s = toSymExpr(arg);
  Symbol* var = s->var;

  // Call autoCopy/autoDestroy on task arguments that potentially require deep
  // copy semantics.
  if (taskMayOutliveTaskArg(arg, fcall))
  {
    Type* baseType = arg->getValType();
    FnSymbol* autoCopyFn = getAutoCopy(baseType);
    FnSymbol* autoDestroyFn = getAutoDestroy(baseType);

    if (isRefCountedType(baseType))
    {
      // TODO: Can we consolidate these two clauses?
      // Does arg->typeInfo() != baseType mean that arg is passed by ref?
      if (arg->typeInfo() != baseType)
      {
        // For internally reference-counted types, this punches through
        // references to bump the reference count.
        VarSymbol* derefTmp = newTemp("derefTmp", baseType);
        fcall->insertBefore(new DefExpr(derefTmp));
        fcall->insertBefore(new CallExpr(PRIM_MOVE, derefTmp,
                                         new CallExpr(PRIM_DEREF, var)));
        // The result of the autoCopy call is dropped on the floor.
        // It is only called to increment the ref count.
        CallExpr* autoCopyCall = new CallExpr(autoCopyFn, derefTmp);
        fcall->insertBefore(autoCopyCall);
        insertReferenceTemps(autoCopyCall);
        // But the original var is passed through to the field assignment.
      }
      else
      {
        VarSymbol* valTmp = newTemp(baseType);
        valTmp->addFlag(FLAG_NECESSARY_AUTO_COPY);
        fcall->insertBefore(new DefExpr(valTmp));
        CallExpr* autoCopyCall = new CallExpr(autoCopyFn, var);
        fcall->insertBefore(new CallExpr(PRIM_MOVE, valTmp, autoCopyCall));
        insertReferenceTemps(autoCopyCall);
        // If the arg is not passed by reference, the result of the autoCopy is
        // passed to the field assignment.
        var = valTmp;
      }

      if (firstCall)
      {
        // The task function may be called from several call sites, so insert
        // the autodestroy call only once (when processing the first fcall).
        FnSymbol* fn = fcall->isResolved();
        Symbol* formal = actual_to_formal(arg);
        if (arg->typeInfo() != baseType)
        {
          VarSymbol* derefTmp = newTemp("derefTmp", baseType);
          fn->insertBeforeDownEndCount(new DefExpr(derefTmp));
          fn->insertBeforeDownEndCount(
            new CallExpr(PRIM_MOVE, derefTmp, new CallExpr(PRIM_DEREF,  formal)));
          formal = derefTmp;
        }
        CallExpr* autoDestroyCall = new CallExpr(autoDestroyFn, formal);
        fn->insertBeforeDownEndCount(autoDestroyCall);
        insertReferenceTemps(autoDestroyCall);
      }
    }
    else if (isRecord(baseType))
    {
      // Do this only if the record is passed by value.
      if (arg->typeInfo() == baseType)
      {
        // TODO: Find out why _RuntimeTypeInfo records do not have autoCopy
        // functions, so we can get rid of this special test.
        if (autoCopyFn == NULL) return var;

        // Insert a call to the autoCopy function ahead of the call.
        VarSymbol* valTmp = newTemp(baseType);
        fcall->insertBefore(new DefExpr(valTmp));
        CallExpr* autoCopyCall = new CallExpr(autoCopyFn, var);
        fcall->insertBefore(new CallExpr(PRIM_MOVE, valTmp, autoCopyCall));
        insertReferenceTemps(autoCopyCall);
        var = valTmp;

        if (firstCall)
        {
          // Insert a call to the autoDestroy function ahead of the return (or
          // _downEndCount() call, if present).
          // (But only once per function for each affected argument.)
          FnSymbol* fn = fcall->isResolved();
          Symbol* formal = actual_to_formal(arg);
          CallExpr* autoDestroyCall = new CallExpr(autoDestroyFn,formal);
          fn->insertBeforeDownEndCount(autoDestroyCall);
          insertReferenceTemps(autoDestroyCall);
        }
      }
    }
  }
  return var;
}


static void
bundleArgs(CallExpr* fcall, BundleArgsFnData &baData) {
  SET_LINENO(fcall);
  ModuleSymbol* mod = fcall->getModule();
  FnSymbol* fn = fcall->isResolved();

  const bool firstCall = baData.firstCall;
  if (firstCall)
    create_arg_bundle_class(fn, fcall, mod, baData);
  AggregateType* ctype = baData.ctype;

  // create the class variable instance and allocate space for it
  VarSymbol *tempc = newTemp(astr("_args_for", fn->name), ctype);
  fcall->insertBefore( new DefExpr( tempc));
  insertChplHereAlloc(fcall, false /*insertAfter*/, tempc,
                      ctype, newMemDesc("bundled args"));

  // set the references in the class instance
  int i = 1;
  for_actuals(arg, fcall) 
  {
    // Insert autoCopy/autoDestroy as needed for "begin" or "nonblocking on"
    // calls.
    Symbol  *var = insertAutoCopyDestroyForTaskArg(arg, fcall, firstCall);

    // Copy the argument into the corresponding slot in the argument bundle.
    CallExpr *setc = new CallExpr(PRIM_SET_MEMBER,
                                  tempc,
                                  ctype->getField(i),
                                  var);
    fcall->insertBefore(setc);
    i++;
  }

  // create wrapper-function that uses the class instance
  create_block_fn_wrapper(fn, fcall, baData);
  call_block_fn_wrapper(fn, fcall, tempc, baData.wrap_fn);
  baData.firstCall = false;
}


static void create_block_fn_wrapper(FnSymbol* fn, CallExpr* fcall, BundleArgsFnData &baData)
{
  ModuleSymbol* mod = fcall->getModule();
  INT_ASSERT(fn == fcall->isResolved());

  INT_ASSERT(baData.firstCall == !baData.wrap_fn);
  if (!baData.firstCall) return;

  AggregateType* ctype = baData.ctype;
  FnSymbol *wrap_fn = new FnSymbol( astr("wrap", fn->name));

  // Add a special flag to the wrapper-function as appropriate.
  // These control aspects of code generation.
  if (fn->hasFlag(FLAG_ON))                     wrap_fn->addFlag(FLAG_ON_BLOCK);
  if (fn->hasFlag(FLAG_NON_BLOCKING))           wrap_fn->addFlag(FLAG_NON_BLOCKING);
  if (fn->hasFlag(FLAG_COBEGIN_OR_COFORALL))    wrap_fn->addFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK);
  if (fn->hasFlag(FLAG_BEGIN))                  wrap_fn->addFlag(FLAG_BEGIN_BLOCK);

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
    DefExpr* localeArg = toDefExpr(fn->formals.get(1)->copy());
    // The above copy() used to be a remove(), based on the assumption that there was
    // exactly one wrapper for each on.  Now, the on_fn is outlined early and has
    // several callers, therefore severall wrapon_fns are generated.
    // So, we leave the extra locale arg in place here and remove it later 
    // (see the last if (fn->hasFlag(FLAG_ON)) clause in passArgsToNestedFns()).
    wrap_fn->insertFormalAtTail(localeArg);
  }

  ArgSymbol *wrap_c = new ArgSymbol( INTENT_CONST_REF, "c", ctype);
  wrap_fn->insertFormalAtTail(wrap_c);

  mod->block->insertAtTail(new DefExpr(wrap_fn));

  // Create a call to the original function
  CallExpr *call_orig = new CallExpr(fn);
  bool first = true;
  for_fields(field, ctype)
  {
    // insert args
    VarSymbol* tmp = newTemp(field->name, field->type);
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

  wrap_fn->retType = dtVoid;
  wrap_fn->insertAtTail(call_orig);     // add new call

  if (fn->hasFlag(FLAG_ON))
    ; // the caller will free the actual
  else
    wrap_fn->insertAtTail(callChplHereFree(wrap_c));

  wrap_fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  // 'fn' has already been flattened and hoisted to the top level.
  // We leave 'fn' in the module where it was placed originally,
  // whereas 'wrap_fn' is in fcall's module.
  // These two modules may be different, e.g. due to iterator inlining.
  INT_ASSERT(isGlobal(fn));

  baData.wrap_fn = wrap_fn;
}

static void call_block_fn_wrapper(FnSymbol* fn, CallExpr* fcall, VarSymbol* tempc, FnSymbol *wrap_fn)
{
  // The wrapper function is called with the bundled argument list.
  if (fn->hasFlag(FLAG_ON)) {
    // For an on block, the first argument is also passed directly
    // to the wrapper function.
    // The forking function uses this to fork a task on the target locale.
    fcall->insertBefore(new CallExpr(wrap_fn, fcall->get(1)->remove(), tempc));
  } else
    fcall->insertBefore(new CallExpr(wrap_fn, tempc));

  if (fn->hasFlag(FLAG_ON))
    fcall->insertAfter(callChplHereFree(tempc));
  else
    ; // wrap_fn will free the formal

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
    ArgSymbol* arg = new ArgSymbol(INTENT_CONST_REF, "_endCount", endCountType);
    fn->insertFormalAtTail(arg);
    VarSymbol* var = newTemp("_endCount", endCountType);
    fn->insertAtHead(new CallExpr(PRIM_MOVE, var, arg));
    fn->insertAtHead(new DefExpr(var));
    endCountMap.put(fn, var);
    queue.add(fn);
  }
}


static void
replicateGlobalRecordWrappedVars(DefExpr *def) {
  ModuleSymbol* mod = toModuleSymbol(def->parentSymbol);
  Expr* found_stmt = NULL;
  Expr* useFirst = NULL;
  Symbol *currDefSym = def->sym;
  bool found = false;
  // Try to find the first definition of this variable in the
  //   module initialization function
  std::vector<Expr*> stmts;
  collect_stmts(mod->initFn->body, stmts);
  for_vector(Expr, stmt, stmts)
  {
    // Ignore all but CallExprs.
    if (! isCallExpr(stmt))
      continue;

    std::vector<SymExpr*> symExprs;
    collectSymExprs(stmt, symExprs);
    for_vector(SymExpr, se, symExprs) {
      if (se->var == currDefSym) {
        INT_ASSERT(se->parentExpr);
        int result = isDefAndOrUse(se);
        if (result & 1) {
          // first use/def of the variable is a def (normal case)
          INT_ASSERT(useFirst==NULL);
          found_stmt = stmt;
          found = true;
          break;
        } else if (result & 2) {
          if (useFirst == NULL) {
            // This statement captures a reference to the variable
            // to pass it to the function that builds the initializing
            // expression
            CallExpr *parent = toCallExpr(se->parentExpr);
            INT_ASSERT(parent);
            INT_ASSERT(parent->isPrimitive(PRIM_ADDR_OF));
            INT_ASSERT(isCallExpr(parent->parentExpr));
            // Now start looking for the first use of the captured
            // reference
            currDefSym = toSymExpr(toCallExpr(parent->parentExpr)->get(1))->var;
            INT_ASSERT(currDefSym);
            // This is used to flag that we have found the first use
            // of the variable
            useFirst = stmt;
          } else {
            // This statement builds the initializing expression, so
            // we can insert the broadcast after this statement

            // These checks may need to change if we change the way
            // we handle domain literals, forall expressions, and/or
            // depending on how we add array literals to the language
            INT_ASSERT(toCallExpr(stmt));
            INT_ASSERT(toCallExpr(stmt)->primitive==NULL);
            found_stmt = stmt;
            found = true;
            break;
          }
        }
      }
    }
    if (found)
      break;
  }

  if (found)
    found_stmt->insertAfter(new CallExpr(PRIM_PRIVATE_BROADCAST, def->sym));
  else
    mod->initFn->insertBeforeReturn(new CallExpr
                                    (PRIM_PRIVATE_BROADCAST, def->sym));
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


static void
freeHeapAllocatedVars(Vec<Symbol*> heapAllocatedVars) {
  Vec<FnSymbol*> fnsContainingTaskll;

  // start with the functions created from begin, cobegin, and coforall statements
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_BEGIN) || fn->hasFlag(FLAG_COBEGIN_OR_COFORALL) ||
        fn->hasFlag(FLAG_NON_BLOCKING))
      fnsContainingTaskll.add(fn);
  }
  // add any functions that call the functions added so far
  forv_Vec(FnSymbol, fn, fnsContainingTaskll) {
    forv_Vec(CallExpr, call, *fn->calledBy) {
      if (call->parentSymbol) {
        FnSymbol* caller = toFnSymbol(call->parentSymbol);
        INT_ASSERT(caller);
        fnsContainingTaskll.add_exclusive(caller);
      }
    }
  }

  Vec<Symbol*> symSet;
  std::vector<BaseAST*> asts;
  Vec<SymExpr*> symExprs;
  collect_asts(rootModule, asts);
  for_vector(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (def->parentSymbol) {
        if (isLcnSymbol(def->sym)) {
          symSet.set_add(def->sym);
        }
      }
    } else if (SymExpr* se = toSymExpr(ast)) {
      symExprs.add(se);
    }
  }
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(symSet, symExprs, defMap, useMap);

  forv_Vec(Symbol, var, heapAllocatedVars) {
    // find out if a variable that was put on the heap could be passed in as an
    // argument to a function created from a begin, cobegin, or coforall statement;
    // if not, free the heap memory just allocated at the end of the block
    Vec<SymExpr*>* defs = defMap.get(var);
    if (defs == NULL) {
      INT_FATAL(var, "Symbol is never defined.");
    }
    if (defs->n == 1) {
      bool freeVar = true;
      Vec<Symbol*> varsToTrack;
      varsToTrack.add(var);
      forv_Vec(Symbol, v, varsToTrack) {
        if (useMap.get(v)) {
          forv_Vec(SymExpr, se, *useMap.get(v)) {
            if (CallExpr* call = toCallExpr(se->parentExpr)) {
              if (call->isPrimitive(PRIM_ADDR_OF) ||
                  call->isPrimitive(PRIM_GET_MEMBER) ||
                  call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                  call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                  call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
                  call->isPrimitive(PRIM_WIDE_GET_NODE))
                // Treat the use of these primitives as a use of their arguments.
                call = toCallExpr(call->parentExpr);
              if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN))
                varsToTrack.add(toSymExpr(call->get(1))->var);
              else if (fnsContainingTaskll.in(call->isResolved())) {
                freeVar = false;
                break;
              }
            }
          }
          if (!freeVar) break;
        }
      }
      if (freeVar) {
        CallExpr* move = toCallExpr(defs->v[0]->parentExpr);
        INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
        Expr* innermostBlock = NULL;
        // find the innermost block that contains all uses of var
        INT_ASSERT(useMap.get(var));
        forv_Vec(SymExpr, se, *useMap.get(var)) {
          bool useInInnermostBlock = false;
          BlockStmt* curInnermostBlock = toBlockStmt(se->parentExpr);
          INT_ASSERT(!curInnermostBlock); // assumed to be NULL at this point
          for (Expr* block = se->parentExpr->parentExpr;
               block && !useInInnermostBlock;
               block = block->parentExpr) {
            if (!curInnermostBlock)
              curInnermostBlock = toBlockStmt(block);
            if (!innermostBlock) {
              innermostBlock = toBlockStmt(block);
              if (innermostBlock)
                useInInnermostBlock = true;
            } else if (block == innermostBlock)
              useInInnermostBlock = true;
          }
          if (!useInInnermostBlock) {
            // the current use is not contained within innermostBlock,
            // so find out if the innermost block that contains the current use
            // also contains innermostBlock
            Expr* block = innermostBlock;
            while (block && block != curInnermostBlock)
              block = block->parentExpr;
            if (block)
              innermostBlock = curInnermostBlock;
            else {
              // the innermost block that contains the current use is disjoint
              // from the innermost block that contains previously encountered use(s)
              INT_ASSERT(innermostBlock && !block);
              while ((innermostBlock = innermostBlock->parentExpr)) {
                for (block = curInnermostBlock->parentExpr; block && block != innermostBlock;
                     block = block->parentExpr)
                  /* do nothing */;
                if (block) break;
              }
              if (!innermostBlock)
                INT_FATAL(move, "cannot find a block that contains all uses of var\n");
            }
          }
        }
        FnSymbol* fn = toFnSymbol(move->parentSymbol);
        SET_LINENO(var);
        if (fn && innermostBlock == fn->body)
          // The block is a function body.
          fn->insertBeforeReturnAfterLabel(callChplHereFree(move->get(1)->copy()));
        else {
          // The block is some other kind of block.
          if (CForLoop* cfl = toCForLoop(innermostBlock))
          {
            // The logical end of the loop block is as at the end of the
            // increment clause.  It is assumed here, that if the innerMost
            // block contained the entire for loop, then that block would not
            // be the CForLoop block itself.  If a block contains nothing but a
            // CForLoop and then the block structure is smashed flat, we lose
            // the ability to distinguish these two cases.
            BlockStmt* incr = cfl->incrBlockGet();
            incr->insertAtTailBeforeFlow(callChplHereFree(move->get(1)->copy()));
          }
          // Other cases may need to be added here.
          else
          {
            // A "normal" block.
            BlockStmt* block = toBlockStmt(innermostBlock);
            INT_ASSERT(block);
            block->insertAtTailBeforeFlow(callChplHereFree(move->get(1)->copy()));
          }
        }
      }
      else
      {
        // Free the variable at the end of the task function.
      }
    }
    // else ... 
    // TODO: After the new constructor story is implemented, every declaration
    // should have exactly one definition associated with it, so the
    // (defs-> == 1) test above can be replaced by an assertion.
  }
}

// Returns false if 
//  fLocal == true
// or
//  CHPL_COMM == "ugni"
// of
//  CHPL_COMM == "gasnet" && CHPL_GASNET_SEGMENT == "everything";
// true otherwise.
static bool
needHeapVars() {
  if (fLocal) return false;

  if (!strcmp(CHPL_COMM, "ugni") ||
      (!strcmp(CHPL_COMM, "gasnet") &&
       !strcmp(CHPL_GASNET_SEGMENT, "everything")))
    return false;

  return true;
}

//
// In the following, through makeHeapAllocations():
//   refSet, refVec - symbols whose referencees need to be heap-allocated
//   varSet, varVec - symbols that themselves need to be heap-allocated
//

// Traverses all 'begin' or 'on' task functions flagged as needing heap
// allocation (for its formals) or flagged as nonblockikng.
// Traverses all ref formals of these functions and adds them to the refSet and
// refVec.
static void findBlockRefActuals(Vec<Symbol*>& refSet, Vec<Symbol*>& refVec)
{
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_BEGIN) ||
        (fn->hasFlag(FLAG_ON) &&
         (needHeapVars() || fn->hasFlag(FLAG_NON_BLOCKING)))) {
      for_formals(formal, fn) {
        if (formal->type->symbol->hasFlag(FLAG_REF)) {
          refSet.set_add(formal);
          refVec.add(formal);
        }
      }
    }
  }
}


// Traverses all DefExprs.
//  If the symbol is a coforall index expression,
//   If it is of reference type,
//    Add it to refSet and refVec.
//   Otherwise, if it is not of primitive type or other undesired cases,
//    Add it to varSet and varVec.
//  Otherwise, select module-level vars that are not private or extern.
//   If the var is const and has value semantics except record-wrapped types,
//    Insert a prim_private_broadcast call after the def.
//   Otherwise, if it is a record-wrapped type, replicate it.
//   Otherwise,
//    Add it to varSet and varVec, so it will be put on the heap.
static void findHeapVarsAndRefs(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                Vec<Symbol*>& refSet, Vec<Symbol*>& refVec,
                                Vec<Symbol*>& varSet, Vec<Symbol*>& varVec)
{
  forv_Vec(DefExpr, def, gDefExprs) {
    SET_LINENO(def);
    if (def->sym->hasFlag(FLAG_COFORALL_INDEX_VAR)) {
      if (def->sym->type->symbol->hasFlag(FLAG_REF)) {
        refSet.set_add(def->sym);
        refVec.add(def->sym);
      } else if (!isPrimitiveType(def->sym->type) ||
                 toFnSymbol(def->parentSymbol)->retTag==RET_REF) {
        varSet.set_add(def->sym);
        varVec.add(def->sym);
      }
    } else if (!fLocal &&
               isModuleSymbol(def->parentSymbol) &&
               def->parentSymbol != rootModule &&
               isVarSymbol(def->sym) &&
               !def->sym->hasFlag(FLAG_LOCALE_PRIVATE) &&
               !def->sym->hasFlag(FLAG_EXTERN)) {
      if (def->sym->hasFlag(FLAG_CONST) &&
          (is_bool_type(def->sym->type) ||
           is_enum_type(def->sym->type) ||
           is_int_type(def->sym->type) ||
           is_uint_type(def->sym->type) ||
           is_real_type(def->sym->type) ||
           is_imag_type(def->sym->type) ||
           is_complex_type(def->sym->type) ||
           (isRecord(def->sym->type) &&
            !isRecordWrappedType(def->sym->type) &&
            // sync/single are currently classes, so this shouldn't matter
            !isSyncType(def->sym->type)))) {
        // replicate global const of primitive type
        INT_ASSERT(defMap.get(def->sym) && defMap.get(def->sym)->n == 1);
        for_defs(se, defMap, def->sym) {
          se->getStmtExpr()->insertAfter(new CallExpr(PRIM_PRIVATE_BROADCAST, def->sym));
        }
      } else if (isRecordWrappedType(def->sym->type)) {
        // replicate address of global arrays, domains, and distributions
        replicateGlobalRecordWrappedVars(def);
      } else {
        // put other global constants and all global variables on the heap
        varSet.set_add(def->sym);
        varVec.add(def->sym);
      }
    }
  }
}


static void
makeHeapAllocations() {
  Vec<Symbol*> refSet;
  Vec<Symbol*> refVec;
  Vec<Symbol*> varSet;
  Vec<Symbol*> varVec;

  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap);

  findBlockRefActuals(refSet, refVec);
  findHeapVarsAndRefs(defMap, refSet, refVec, varSet, varVec);

  forv_Vec(Symbol, ref, refVec) {
    if (ArgSymbol* arg = toArgSymbol(ref)) {
      FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);
      forv_Vec(CallExpr, call, *fn->calledBy) {
        if (! call->parentSymbol)
          continue;

        SymExpr* se = NULL;
        for_formals_actuals(formal, actual, call) {
          if (formal == arg)
            se = toSymExpr(actual);
        }
        INT_ASSERT(se);
        // Previous passes mean that we should always get a formal SymExpr
        // to match the ArgSymbol.  And that formal should have the
        // ref flag, since we obtained it through the refVec.
        INT_ASSERT(se->var->type->symbol->hasFlag(FLAG_REF));
        if (!refSet.set_in(se->var)) {
          refSet.set_add(se->var);
          refVec.add(se->var);
        }
      }
    } else if (VarSymbol* var = toVarSymbol(ref)) {
      //      INT_ASSERT(defMap.get(var)->n == 1);
      for_defs(def, defMap, var) {
        INT_ASSERT(def);
        if (CallExpr* call = toCallExpr(def->parentExpr)) {
          if (call->isPrimitive(PRIM_MOVE)) {
            if (CallExpr* rhs = toCallExpr(call->get(2))) {
              if (rhs->isPrimitive(PRIM_ADDR_OF)) {
                SymExpr* se = toSymExpr(rhs->get(1));
                INT_ASSERT(se);
                if (!varSet.set_in(se->var)) {
                  varSet.set_add(se->var);
                  varVec.add(se->var);
                }
              } else if (rhs->isPrimitive(PRIM_GET_MEMBER) ||
                         rhs->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                         rhs->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                         rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
                SymExpr* se = toSymExpr(rhs->get(1));
                INT_ASSERT(se);
                if (se->var->type->symbol->hasFlag(FLAG_REF)) {
                  if (!refSet.set_in(se->var)) {
                    refSet.set_add(se->var);
                    refVec.add(se->var);
                  }
                } else if (!varSet.set_in(se->var)) {
                  varSet.set_add(se->var);
                  varVec.add(se->var);
                }
              }
              //
              // Otherwise assume reference is to something that is
              // already on the heap!  This is concerning...  SJD:
              // Build a future that returns a reference in an
              // iterator to something that is not on the heap
              // (including not in an array).
              //
              // The alternative to making this assumption is to
              // follow the returned reference (assuming this is a
              // function call) through the function and make sure
              // that whatever it returns is on the heap.  Then if we
              // eventually see a GET_ARRAY primitive, we know it is
              // already on the heap.
              //
              // To debug this case, add an else INT_FATAL here.
              //
            } else if (SymExpr* rhs = toSymExpr(call->get(2))) {
              INT_ASSERT(rhs->var->type->symbol->hasFlag(FLAG_REF));
              if (!refSet.set_in(rhs->var)) {
                refSet.set_add(rhs->var);
                refVec.add(rhs->var);
              }
            } else
              INT_FATAL(ref, "unexpected case");
          } else { // !call->isPrimitive(PRIM_MOVE)
            // This definition is created by passing the variable to a function
            // by ref, out or inout intent.  We then assume that the function
            // updates the reference.

            // If the definition of the ref var does not appear in this
            // function, then most likely it was established in an calling
            // routine.
            // We may need to distinguish between definition of the reference
            // var itself (i.e. the establishment of an alias) as compared to
            // when the variable being referenced is updated....
            // In any case, it is safe to ignore this case, because either the
            // value of the ref variable was established elsewhere, or it will
            // appear in another def associated with the ref var.
//            INT_FATAL(ref, "unexpected case");
          }
        } else
          INT_FATAL(ref, "unexpected case");
      }
    }
  }

  Vec<Symbol*> heapAllocatedVars;

  forv_Vec(Symbol, var, varVec) {
    INT_ASSERT(var->hasFlag(FLAG_REF_VAR) || !var->type->symbol->hasFlag(FLAG_REF));

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

    // Special case for string literals: Do not heap-allocate them.
    // Instead, string literal values are moved into local temporaries that are
    // widened.
    if (var->type == dtString)
      if (VarSymbol* v = toVarSymbol(var))
        if (v->immediate)
          continue;

    SET_LINENO(var);

    if (ArgSymbol* arg = toArgSymbol(var)) {
      VarSymbol* tmp = newTemp(var->type);
      varSet.set_add(tmp);
      varVec.add(tmp);
      SymExpr* firstDef = new SymExpr(tmp);
      arg->getFunction()->insertAtHead(new CallExpr(PRIM_MOVE, firstDef, arg));
      addDef(defMap, firstDef);
      arg->getFunction()->insertAtHead(new DefExpr(tmp));
      for_defs(def, defMap, arg) {
        def->var = tmp;
        addDef(defMap, def);
      }
      for_uses(use, useMap, arg) {
        use->var = tmp;
        addUse(useMap, use);
      }
      continue;
    }
    AggregateType* heapType = buildHeapType(var->type);

    //
    // allocate local variables on the heap; global variables are put
    // on the heap during program startup
    //
    if (!isModuleSymbol(var->defPoint->parentSymbol) &&
        ((useMap.get(var) && useMap.get(var)->n > 0) ||
         (defMap.get(var) && defMap.get(var)->n > 0))) {
      SET_LINENO(var->defPoint);
      insertChplHereAlloc(var->defPoint->getStmtExpr(),
                          true /*insertAfter*/,var, heapType,
                          newMemDesc("local heap-converted data"));
      heapAllocatedVars.add(var);
    }

    for_defs(def, defMap, var) {
      if (CallExpr* call = toCallExpr(def->parentExpr)) {
        SET_LINENO(call);
        // Do we need a case for PRIM_ASSIGN?
        if (call->isPrimitive(PRIM_MOVE)) {
          VarSymbol* tmp = newTemp(var->type);
          call->insertBefore(new DefExpr(tmp));
          Expr* value = call->get(2)->remove();
          call->insertBefore(new CallExpr(PRIM_MOVE, tmp, value));
          call->replace(new CallExpr(PRIM_SET_MEMBER, call->get(1)->copy(), heapType->getField(1), tmp));
        } else if (call->isResolved() &&
                   call->isResolved()->hasFlag(FLAG_AUTO_DESTROY_FN)) {
          call->remove();
        } else {
          VarSymbol* tmp = newTemp(var->type);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, def->var, heapType->getField(1))));
          def->replace(new SymExpr(tmp));
        }
      } else
        INT_FATAL(var, "unexpected case");
    }

    for_uses(use, useMap, var) {
      if (CallExpr* call = toCallExpr(use->parentExpr)) {
        if (call->isPrimitive(PRIM_ADDR_OF)) {
          CallExpr* move = toCallExpr(call->parentExpr);
          INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
          if (move->get(1)->typeInfo() == heapType) {
            call->replace(use->copy());
          } else {
            call->replace(new CallExpr(PRIM_GET_MEMBER, use->var, heapType->getField(1)));
          }
        } else if (call->isResolved()) {
          if (call->isResolved()->hasFlag(FLAG_AUTO_DESTROY_FN_SYNC)) {
            //
            // We don't move sync vars to the heap and don't do the
            // analysis to determine whether or not they outlive a
            // task that refers to them, so conservatively remove
            // their autodestroy calls to avoid freeing them before
            // all tasks are done with them.  While this is
            // unfortunate and needs to be fixed in the future to
            // avoid leaks (TODO), it is better than the previous
            // version of this code that would remove all autodestroy
            // calls in this conditional.  See the commit message for
            // this commenet for more detail.
            //
            call->remove();
          } else if (actual_to_formal(use)->type == heapType) {
            // do nothing
          } else {
            VarSymbol* tmp = newTemp(var->type);
            call->getStmtExpr()->insertBefore(new DefExpr(tmp));
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, use->var, heapType->getField(1))));
            use->replace(new SymExpr(tmp));
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
          VarSymbol* tmp = newTemp(var->type->refType);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, use->var, heapType->getField(1))));
          use->replace(new SymExpr(tmp));
        } else {
          VarSymbol* tmp = newTemp(var->type);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, use->var, heapType->getField(1))));
          use->replace(new SymExpr(tmp));
        }
      } else if (use->parentExpr)
        INT_FATAL(var, "unexpected case");
    }

    var->type = heapType;
  }

  freeHeapAllocatedVars(heapAllocatedVars);
}


//
// re-privatize privatized object fields in iterator classes
//
static void
reprivatizeIterators() {
  if (fLocal)
    return; // no need for privatization

  Vec<Symbol*> privatizedFields;

  forv_Vec(AggregateType, ct, gAggregateTypes) {
    for_fields(field, ct) {
      if (ct->symbol->hasFlag(FLAG_ITERATOR_CLASS) &&
          field->type->symbol->hasFlag(FLAG_PRIVATIZED_CLASS)) {
        privatizedFields.set_add(field);
      }
    }
  }

  forv_Vec(SymExpr, se, gSymExprs) {
    if (privatizedFields.set_in(se->var)) {
      SET_LINENO(se);
      if (CallExpr* call = toCallExpr(se->parentExpr)) {
        if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
          CallExpr* move = toCallExpr(call->parentExpr);
          INT_ASSERT(move->isPrimitive(PRIM_MOVE));
          SymExpr* lhs = toSymExpr(move->get(1));
          AggregateType* ct = toAggregateType(se->var->type);
          VarSymbol* tmp = newTemp(ct->getField("pid")->type);
          move->insertBefore(new DefExpr(tmp));
          lhs->replace(new SymExpr(tmp));
          move->insertAfter(new CallExpr(PRIM_MOVE, lhs->var, new CallExpr(PRIM_GET_PRIV_CLASS, lhs->var->type->symbol, tmp)));
        } else if (call->isPrimitive(PRIM_GET_MEMBER)) {
          CallExpr* move = toCallExpr(call->parentExpr);
          INT_ASSERT(move->isPrimitive(PRIM_MOVE));
          SymExpr* lhs = toSymExpr(move->get(1));
          AggregateType* ct = toAggregateType(se->var->type);
          VarSymbol* tmp = newTemp(ct->getField("pid")->type);
          move->insertBefore(new DefExpr(tmp));
          lhs->replace(new SymExpr(tmp));
          call->primitive = primitives[PRIM_GET_MEMBER_VALUE];
          VarSymbol* valTmp = newTemp(lhs->getValType());
          move->insertBefore(new DefExpr(valTmp));
          move->insertAfter(new CallExpr(PRIM_MOVE, lhs, new CallExpr(PRIM_ADDR_OF, valTmp)));
          move->insertAfter(new CallExpr(PRIM_MOVE, valTmp, new CallExpr(PRIM_GET_PRIV_CLASS, lhs->getValType()->symbol, tmp)));
        } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
          AggregateType* ct = toAggregateType(se->var->type);
          VarSymbol* tmp = newTemp(ct->getField("pid")->type);
          call->insertBefore(new DefExpr(tmp));
          call->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, call->get(3)->remove(), ct->getField("pid"))));
          call->insertAtTail(new SymExpr(tmp));
        } else
          INT_FATAL(se, "unexpected case in re-privatization in iterator");
      } else
        INT_FATAL(se, "unexpected case in re-privatization in iterator");
    }
  }

  forv_Vec(Symbol, sym, privatizedFields) if (sym) {
    sym->type = dtInt[INT_SIZE_DEFAULT];
  }
}


void
parallel(void) {

#ifdef HILDE_MM
  // This is here just because it depends on the cleanup after lowerIterators
  // having been performed, and it depends on lower iterators to get the basic
  // block structure of iterator functions right.
  // In other words, it could be moved back to the end of callDestructors.cpp
  // if basic block analysis were modified to treat a PRIM_YIELD as an
  // end-of-block and treat all blocks within an iterator function as
  // successors of a faked-in start block.
  insertAutoCopyAutoDestroy();
#endif

  Vec<FnSymbol*> taskFunctions;

  // Collect the task functions for processing.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (isTaskFun(fn)) {
      taskFunctions.add(fn);
      // Would need to flatten them if they are not already.
      INT_ASSERT(isGlobal(fn));
    }
  }

  compute_call_sites();

  // TODO: Move this into a separate pass.
  remoteValueForwarding(taskFunctions);

  reprivatizeIterators();

  makeHeapAllocations();

  insertEndCounts();

  passArgsToNestedFns(taskFunctions);
}


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
      call->replace(new CallExpr(PRIM_MOVE, endCountMap.get(pfn), call->get(1)->remove()));
    }
  }

  forv_Vec(FnSymbol, fn, queue) {
    forv_Vec(CallExpr, call, *fn->calledBy) {
      SET_LINENO(call);
      Type* endCountType = endCountMap.get(fn)->type;
      FnSymbol* pfn = call->getFunction();
      if (!endCountMap.get(pfn))
        insertEndCount(pfn, endCountType, queue, endCountMap);
      call->insertAtTail(endCountMap.get(pfn));
    }
  }
}


// For each "nested" function created to represent remote execution, 
// bundle args so they can be passed through a fork function.
// Fork functions in general have the signature
//  fork(int32_t destNode, void (*)(void* args), void* args, ...);
// In Chapel, we wrap the arguments passed to the nested function in an object
// whose type is just a list of the arguments passed to the nested function.
// Those arguments consist of variables in the scope of the nested function call
// that are accessed within the body of the nested function (recursively, of course).
static void passArgsToNestedFns(Vec<FnSymbol*>& nestedFunctions)
{
  forv_Vec(FnSymbol, fn, nestedFunctions) {

    BundleArgsFnData baData = bundleArgsFnDataInit;

    forv_Vec(CallExpr, call, *fn->calledBy) {
      SET_LINENO(call);
      bundleArgs(call, baData);
    }

    if (fn->hasFlag(FLAG_ON))
    {
      // Now we can remove the dummy locale arg from the on_fn
      DefExpr* localeArg = toDefExpr(fn->formals.get(1));
      std::vector<SymExpr*> symExprs;
      collectSymExprs(fn->body, symExprs);
      for_vector(SymExpr, sym, symExprs)
      {
        if (sym->var->defPoint == localeArg)
          sym->getStmtExpr()->remove();
      }
      localeArg->remove();
    }
  }
}


Type* getOrMakeRefTypeDuringCodegen(Type* type) {
  Type* refType;
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

////////////////////////////////////////////////////////////////////////////////
// NOTES
//
// Note #1:
//  The need for an autoCopy call arises because a variable scoped to the body
//  of a coforall (e.g.) will be deleted when one traversal of the body
//  completes.  All the body of a coforall does in reality is to queue up a
//  task to be run when the coforall loop completes.
//  Which means that the original variable is long gone by the time the task
//  runs.  A shallow (i.e. bitwise) copy will not suffice, because class
//  variables in the record might point to class objects that have since been
//  reclaimed.
//
//  The cases in which the autoCopy/autoDestroy on task args can be avoided can
//  be considered an optimization:  As long is the lifetime of the variable
//  used as an argument is guaranteed to exceed the lifetime of the task, then
//  a deep copy (through autoCopy) is not necessary.  The implicit sync at the
//  end of cobegins and coforalls means that all tasks generated by these
//  constructs will complete before the construct is exited.  Therefore a
//  variable declared outside of a cobegin or coforall is guaranteed to outlast
//  any task it spawns.  Therefore it is legal to apply the optimization in
//  this case.
