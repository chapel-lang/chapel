#include "astutil.h"
#include "passes.h"
#include "stmt.h"

// Marker for variables listed in the 'ref' clause and so not to be converted.
// (Used instead of deleting SymbolMap elements, which does not work well.)
static Symbol* markPruned;

// Is 'sym' an index var in the coforall loop
// for which the 'fn' was created?
static bool isCorrespCoforallIndex(FnSymbol* fn, Symbol* sym)
{
  if (!sym->hasFlag(FLAG_COFORALL_INDEX_VAR))
    return false;

  // If 'sym' is for the loop that 'call' belongs to,
  // they both come from the same BlockStmt.
  BlockStmt* block = toBlockStmt(fn->defPoint->parentExpr);
  INT_ASSERT(block);

  // I conjecture that if 'sym' comes from a different block,
  // it ain't going to be from that loop.
  if (sym->defPoint->parentExpr != block)
    return false;

  // FYI: presently, for a 'coforall', the enclosing block is a for loop.
  INT_ASSERT(block->blockInfo &&
             block->blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP));

  // We could verify that 'sym' is defined via a 'move'
  // from the _indexOfInterest variable referenced by the SymExpr
  // block->blockInfo->get(1). (It's a move from a tuple component
  // of _indexOfInterest, for zippered coforall loops.)
  //
  return true;
}

// We use modified versions of these in flattenFunctions.cpp:
//  isOuterVar(), findOuterVars(), addVarsToFormals(),
//  replaceVarUsesWithFormals(), addVarsToActuals()

// Is 'sym' a non-const variable (including formals) defined outside of 'fn'?
// This is a modification of isOuterVar() from flattenFunctions.cpp.
//
static bool
isOuterVar(Symbol* sym, FnSymbol* fn) {
  Symbol* symParent = sym->defPoint->parentSymbol;
  if (symParent == fn                  || // no need to search
      sym->isParameter()               || // includes isImmediate()
      sym->hasFlag(FLAG_TEMP)          || // exclude these

      // Consts need no special semantics for begin/cobegin/coforall/on.
      // Implementation-wise, it is uniform with consts in nested functions.
      sym->hasFlag(FLAG_CONST)         ||

      // NB 'type' formals do not have INTENT_TYPE
      sym->hasFlag(FLAG_TYPE_VARIABLE)     // 'type' aliases or formals
  ) {
    // these are either not variables or not defined outside of 'fn'
    return false;
  }
  Symbol* parent = fn->defPoint->parentSymbol;
  while (true) {
    if (!isFnSymbol(parent) && !isModuleSymbol(parent))
      return false;
    if (symParent == parent)
      return true;
    if (!parent->defPoint)
      // Only happens when parent==rootModule (right?). This means symParent
      // is not in any of the lexically-enclosing functions/modules, so
      // it's gotta be within 'fn'.
      return false;

    // continue to the enclosing scope
    INT_ASSERT(parent->defPoint->parentSymbol &&
               parent->defPoint->parentSymbol != parent); // ensure termination
    parent = parent->defPoint->parentSymbol;
  }
}

static void
findOuterVars(FnSymbol* fn, SymbolMap* uses) {
  Vec<BaseAST*> asts;
  collect_asts(fn, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* symExpr = toSymExpr(ast)) {
      Symbol* sym = symExpr->var;
      if (toVarSymbol(sym) || toArgSymbol(sym))
        if (!isCorrespCoforallIndex(fn, sym) && isOuterVar(sym, fn))
          uses->put(sym,gNil);
    }
  }
}

// Mark the variables listed in 'ref' clauses, if any, with 'markPruned'.
static void
pruneOuterVars(SymbolMap* uses, CallExpr* byrefVars) {
  if (!byrefVars) return;
  for_actuals(actual, byrefVars) {
    SymExpr* se = toSymExpr(actual);
    INT_ASSERT(se); // comes as an UnresolvedSymExpr from the parser,
                    // should have been resolved in ScopeResolve
    Symbol* var = se->var;
    SymbolMapElem* elem = uses->get_record(var);
    if (elem) {
      elem->value = markPruned;
    }
  }
}

// 'this' (the receiver) should *always* be passed by reference - because
// we want any updates to it in a task construct to be visible outside.
// That includes the implicit 'this' in the constructor - see
// the commit message for r21602. So we exclude those from consideration.
static void
pruneThisArg(Symbol* parent, SymbolMap* uses) {
  form_Map(SymbolMapElem, e, *uses) {
    if (Symbol* sym = e->key) {
      if (e->value != markPruned) {
        if (sym->hasFlag(FLAG_ARG_THIS)) {
          e->value = markPruned;
        }
      }
    }
  }
}

static void
addVarsToFormals(FnSymbol* fn, SymbolMap* vars) {
  form_Map(SymbolMapElem, e, *vars) {
    if (Symbol* sym = e->key) {
      if (e->value != markPruned) {
        SET_LINENO(sym);
        ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, sym->name, sym->type);
        if (ArgSymbol* symArg = toArgSymbol(sym))
          if (symArg->hasFlag(FLAG_MARKED_GENERIC))
            arg->addFlag(FLAG_MARKED_GENERIC);
        fn->insertFormalAtTail(new DefExpr(arg));
        vars->put(sym, arg);
      }
    }
  }
}

static void
replaceVarUsesWithFormals(FnSymbol* fn, SymbolMap* vars) {
  if (vars->n == 0) return;
  Vec<BaseAST*> asts;
  collect_asts(fn->body, asts);
  form_Map(SymbolMapElem, e, *vars) {
    if (Symbol* sym = e->key) {
      if (e->value != markPruned) {
        SET_LINENO(sym);
        ArgSymbol* arg = toArgSymbol(e->value);
        forv_Vec(BaseAST, ast, asts) {
          if (SymExpr* se = toSymExpr(ast)) {
            if (se->var == sym) {
              se->var = arg;
            }
          }
        }
      }
    }
  }
}

static void
addVarsToActuals(CallExpr* call, SymbolMap* vars) {
  form_Map(SymbolMapElem, e, *vars) {
    if (Symbol* sym = e->key) {
      if (e->value != markPruned) {
        SET_LINENO(sym);
        call->insertAtTail(sym);
      }
    }
  }
}

static
bool isAtomicFunctionWithOrderArgument(FnSymbol* fnSymbol, ArgSymbol** order = NULL)
{
  if( !fnSymbol ) return false;
  Symbol* _this = fnSymbol->_this;
  if( !_this ) return false;
  if( !_this->typeInfo()->symbol->hasFlag(FLAG_ATOMIC_TYPE) ) return false;
  // is the last formal the order= argument?
  // Note that it must have the type specified since inferring it happens
  // in a later pass (resolution).
  int numFormals = fnSymbol->numFormals();
  if( numFormals >= 1 ) {
    ArgSymbol* lastFormal = fnSymbol->getFormal(numFormals);
    int has_order_type = lastFormal->typeInfo()->symbol->hasFlag(FLAG_MEMORY_ORDER_TYPE);
    int has_order_name = (0 == strcmp(lastFormal->name, "order"));
    if( has_order_name && ! has_order_type ) {
      INT_FATAL(lastFormal, "atomic method has order without type");
    }
    if( has_order_type ) {
      if( order ) *order = lastFormal;
      return true;
    }
  }
  return false;
}

//
// Converts blocks implementing various task constructs into
// functions, so they can be invoked by a separate task.
//
// The body of the original block becomes the body of the function,
// and the inline location of the block is replaced by a call to the
// implementing function.
//
// A subsequent step (flattenNesteFunctions) adds arguments to these
// functions to pass in values or references from the context which
// are used in the body of the block.
//
// As a special case, the target locale is prepended to the arguments passed 
// to the "on" function.
//
void createTaskFunctions(void) {
  // one-time initialization
  markPruned = gVoid;

  if( fCacheRemote ) {
    // Add fences to Atomics methods
    //  -- or do it with a flag on the network atomic impl fns
    //  for each method in an atomics type that has an order= argument,
    //   and which does not start/end with chpl_rmem_consist_maybe_release,
    //   add chpl_rmem_consist_maybe_release(order)
    //   add chpl_rmem_consist_maybe_acquire(order)
    //  only do this when the remote data cache is enabled.
    // Go through TypeSymbols looking for flag ATOMIC_TYPE
    forv_Vec(ModuleSymbol, module, gModuleSymbols) {
      if( module->hasFlag(FLAG_ATOMIC_MODULE) ) {
        // we could do this with for_alist ... as in getFunctions()
        // instead of creating a copy of the list of functions here.
        Vec<FnSymbol*> moduleFunctions = module->getTopLevelFunctions(false);
        forv_Vec(FnSymbol, fnSymbol, moduleFunctions) {
          ArgSymbol* order = NULL;
          // Does this function have an order= argument?
          // If so, add memory consistency functions (future - if they are not
          // already there).
          if( isAtomicFunctionWithOrderArgument(fnSymbol, &order) ) {
            SET_LINENO(fnSymbol);
            fnSymbol->insertAtHead(
                new CallExpr("chpl_rmem_consist_maybe_release", order));
            fnSymbol->insertBeforeReturn(
                new CallExpr("chpl_rmem_consist_maybe_acquire", order));
          }
        }
      }
    }
  }
 
  // Process task-creating constructs. We include 'on' blocks, too.
  // This code used to be in parallel().
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (CallExpr* info = block->blockInfo) {
      SET_LINENO(block);
      FnSymbol* fn = NULL;
      if (info->isPrimitive(PRIM_BLOCK_BEGIN)) {
        fn = new FnSymbol("begin_fn");
        fn->addFlag(FLAG_BEGIN);
      } else if (info->isPrimitive(PRIM_BLOCK_COBEGIN)) {
        fn = new FnSymbol("cobegin_fn");
        fn->addFlag(FLAG_COBEGIN_OR_COFORALL);
      } else if (info->isPrimitive(PRIM_BLOCK_COFORALL)) {
        fn = new FnSymbol("coforall_fn");
        fn->addFlag(FLAG_COBEGIN_OR_COFORALL);
      } else if (info->isPrimitive(PRIM_BLOCK_ON) ||
                 info->isPrimitive(PRIM_BLOCK_BEGIN_ON) ||
                 info->isPrimitive(PRIM_BLOCK_COBEGIN_ON) ||
                 info->isPrimitive(PRIM_BLOCK_COFORALL_ON)) {
        fn = new FnSymbol("on_fn");
        fn->addFlag(FLAG_ON);
        if (info->isPrimitive(PRIM_BLOCK_BEGIN_ON)) {
          fn->addFlag(FLAG_NON_BLOCKING);
          fn->addFlag(FLAG_BEGIN);
        }
        if (info->isPrimitive(PRIM_BLOCK_COBEGIN_ON) ||
            info->isPrimitive(PRIM_BLOCK_COFORALL_ON)) {
          fn->addFlag(FLAG_NON_BLOCKING);
          fn->addFlag(FLAG_COBEGIN_OR_COFORALL);
        }

        ArgSymbol* arg = new ArgSymbol(INTENT_CONST_IN, "dummy_locale_arg", dtLocaleID);
        fn->insertFormalAtTail(arg);
      }
      else if (info->isPrimitive(PRIM_BLOCK_PARAM_LOOP) || // resolution will remove this case.
               info->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP) ||
               info->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP) ||
               info->isPrimitive(PRIM_BLOCK_FOR_LOOP) ||
               info->isPrimitive(PRIM_BLOCK_LOCAL) ||
               info->isPrimitive(PRIM_BLOCK_UNLOCAL))
        ; // Not a parallel block construct, so do nothing special.
      else
        INT_FATAL(block, "Unhandled blockInfo case.");

      if (fn) {
        INT_ASSERT(isTaskFun(fn));
        CallExpr* call = new CallExpr(fn);

        // These variables are only used if fCacheRemote is set.
        bool needsMemFence = true;
        bool isBlockingOn = false;

        if( fCacheRemote ) {
          Symbol* parent = block->parentSymbol;
          if( parent ) {
            if( parent->hasFlag( FLAG_NO_REMOTE_MEMORY_FENCE ) ) {
              // Do not add remote memory barriers.
              needsMemFence = false;
            } else {
              FnSymbol* fnSymbol = parent->getFnSymbol();
              // For methods on atomic types, we do not add the memory
              // barriers, because these functions have an 'order'
              // argument, which needs to get passed to the memory barrier,
              // so they are handled above.
              needsMemFence = ! isAtomicFunctionWithOrderArgument(fnSymbol);
            }
          }
        }

        if (fn->hasFlag(FLAG_ON)) {
          // This puts the target locale expression "onExpr" at the start of the call.
          call->insertAtTail(block->blockInfo->get(1)->remove());
        }

        block->insertBefore(new DefExpr(fn));

        if( fCacheRemote ) {
          if( block->blockInfo->isPrimitive(PRIM_BLOCK_ON) ) {
            isBlockingOn = true;
          }

          /* We don't need to add a fence for the parent side of
             PRIM_BLOCK_BEGIN_ON
             PRIM_BLOCK_COBEGIN_ON
             PRIM_BLOCK_COFORALL_ON
             PRIM_BLOCK_BEGIN
               since upEndCount takes care of it. */

          // If we need memory barriers, put them around the call to the
          // task function. These memory barriers are ensuring memory
          // consistency.  Spawn barrier (release) is needed for any
          // blocking on statement. Other statements, including cobegin,
          // coforall, begin handle this in upEndCount.
          if( needsMemFence && isBlockingOn )
            block->insertBefore(new CallExpr("chpl_rmem_consist_release"));
        }
        // Add the call to the outlined task function.
        block->insertBefore(call);
        if( fCacheRemote ) {
          // Join barrier (acquire) is needed for a blocking on, and it
          // will make sure that writes in the on statement are available
          // to the caller. Nonblocking on or begin don't block so it
          // doesn't make sense to acquire barrier after running them. 
          // coforall, cobegin, and sync blocks do this in waitEndCount.
          if( needsMemFence && isBlockingOn )
            block->insertBefore(new CallExpr("chpl_rmem_consist_acquire"));
           // block->insertBefore(new CallExpr(PRIM_JOIN_RMEM_FENCE));
        }

        block->blockInfo->remove();

        // Now build the fn for the task or on statement.

        if( fCacheRemote ) {
          // We do a 'start' (acquire) memory barrier to prevent the task
          // from re-using cached elements from another task. This could
          // conceivably be handled by the tasking layer, but they already
          // have enough to worry about...
          fn->insertAtTail(new CallExpr(PRIM_START_RMEM_FENCE));
        }

        // This block becomes the body of the new function.
        // It is flattened so _downEndCount appears in the same scope as the
        // function formals added below.
        for_alist(stmt, block->body)
          fn->insertAtTail(stmt->remove());

        if( fCacheRemote ) {
          // In order to make sure that any 'put' from the task is completed,
          // we do a 'finish' (release) barrier. If it's a begin,
          // nonblocking on, coforall, or cobegin though this will be
          // handled in _downEndCount -- so we just need to add the barrier
          // here for a blocking on statement. We don't add it redundantly
          // because other parts of the compiler rely on finding _downEndCount
          // at the end of certain functions.
          if( isBlockingOn )
            fn->insertAtTail(new CallExpr(PRIM_FINISH_RMEM_FENCE));
        }
        
        fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
        fn->retType = dtVoid;

        if (needsCapture(fn)) {
          // Convert referenced variables to explicit arguments.
          // Note: this collects only the variables, including globals,
          // that are referenced directly within in the task-parallel block.
          SymbolMap* uses = new SymbolMap();
          findOuterVars(fn, uses);

          // Dunno what the "right" place to eliminate these variables is.
          // Will need to do more when we have a variety of ref-like clauses.
          pruneOuterVars(uses, block->byrefVars);
          block->byrefVars->remove();

          pruneThisArg(call->parentSymbol, uses);

          // Cf. flattenNestedFunctions - here we don't (seem to) need:
          // (a) the 'while (change)' loop - as we are not flattenning 'fn';
          // (b) the 'outerCall' computation - because the (single) call
          //     and the callee's defPoint are in the same scope, so they see
          //     the defPoints of the same set of vars.
          addVarsToActuals(call, uses);

          addVarsToFormals(fn, uses);
          replaceVarUsesWithFormals(fn, uses);
        }
      } // if fn
    } // if blockInfo

    // 'byrefVars' should have been eliminated for those blocks where it is
    // syntactically allowed, and should be always empty for anything else.
    INT_ASSERT(!block->byrefVars);

  } // for block

}  // createTaskFunctions()
