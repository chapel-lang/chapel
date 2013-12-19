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
          if (symArg->markedGeneric)
            arg->markedGeneric = true;
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
void createTaskFunctions(void) {
  // one-time initialization
  markPruned = gVoid;

  // Process task-creating constructs. We include 'on' blocks, too.
  // This code used to be in parallel().
  forv_Vec(BlockStmt, block, gBlockStmts) {
    bool neededCapture = false;
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
                 info->isPrimitive(PRIM_BLOCK_ON_NB)) {
        fn = new FnSymbol("on_fn");
        fn->addFlag(FLAG_ON);
        if (block->blockInfo->isPrimitive(PRIM_BLOCK_ON_NB))
          fn->addFlag(FLAG_NON_BLOCKING);

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
        if (block->blockInfo->isPrimitive(PRIM_BLOCK_ON) ||
            block->blockInfo->isPrimitive(PRIM_BLOCK_ON_NB))
          // This puts the target locale expression "onExpr" at the start of the call.
          call->insertAtTail(block->blockInfo->get(1)->remove());

        block->insertBefore(new DefExpr(fn));
        block->insertBefore(call);
        block->blockInfo->remove();

        // This block becomes the body of the new function.
        // It is flattened so _downEndCount appears in the same scope as the
        // function formals added below.
        for_alist(stmt, block->body)
          fn->insertAtTail(stmt->remove());

        fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
        fn->retType = dtVoid;

        if (needsCapture(fn)) {
          neededCapture = true;
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

    if (!neededCapture) {
      // A 'ref' clause is allowed syntactically only on certain blocks,
      // so byrefVars should be empty for anything else.
      INT_ASSERT(!block->byrefVars);
    }
  } // for block

}  // createTaskFunctions()
