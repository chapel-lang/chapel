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

#include "passes.h"

#include "alist.h"
#include "astutil.h"
#include "expr.h"
#include "resolveIntents.h"
#include "stmt.h"
#include "stlUtil.h"

static void markTaskFunctionsInIterators(Vec<FnSymbol*>& nestedFunctions);

void flattenFunctions() {
  Vec<FnSymbol*> nestedFunctions;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (isFnSymbol(fn->defPoint->parentSymbol)) {
      nestedFunctions.add(fn);
    }
  }

  markTaskFunctionsInIterators(nestedFunctions);
  flattenNestedFunctions(nestedFunctions);
}


void flattenNestedFunction(FnSymbol* nestedFunction) {
  if (isFnSymbol(nestedFunction->defPoint->parentSymbol)) {
    Vec<FnSymbol*> nestedFunctions;

    nestedFunctions.add(nestedFunction);

    flattenNestedFunctions(nestedFunctions);
  }
}

static void markTaskFunctionsInIterators(Vec<FnSymbol*>& nestedFunctions) {

  forv_Vec(FnSymbol, fn, nestedFunctions) {
    FnSymbol* curFn = fn;
    while (curFn && isTaskFun(curFn)) {
      curFn = toFnSymbol(curFn->defPoint->parentSymbol);
    }
    // Now curFn is NULL or the first not-a-task function
    if (curFn && curFn->isIterator()) {
      // Mark all of the inner task functions
      IteratorInfo* ii = curFn->iteratorInfo;
      curFn = fn;
      while (curFn && isTaskFun(curFn)) {
        curFn->addFlag(FLAG_TASK_FN_FROM_ITERATOR_FN);
        curFn->iteratorInfo = ii;
        curFn = toFnSymbol(curFn->defPoint->parentSymbol);
      }
    }
  }
}


//
// returns true if the symbol is defined in an outer function to fn
// third argument not used at call site
//
static bool
isOuterVar(Symbol* sym, FnSymbol* fn, Symbol* parent = NULL) {
  if (!parent) {
    parent = fn->defPoint->parentSymbol;

    // If the symbol is at module scope and the type should always be
    // RVF'd and the symbol doesn't have the "locale private" flag
    // applied to it then we should RVF it, so add it to the
    // function's argument list so that it can be considered in the
    // remoteValueForwarding pass (Otherwise, symbols at module scope
    // tend not to be RVF'd... but maybe they should be?  A
    // disadvantage to doing so is that they have to be added as
    // arguments in the flattenFunctions pass to be considered, but if
    // they're not going to be RVF'd, this is unnecessary...  And if
    // they're const, they'll be broadcast proactively at module
    // initialization time).)
    //
    // Why skip "locale private" variables?  Because these variables
    // already have special handling to localize them, and RVFing them
    // causes the `Locales` to be RVF'd which caused extra
    // communications to take place (and generally seems confusing).
    //
    if (isModuleSymbol(sym->defPoint->parentSymbol) &&
        sym->getValType()->symbol->hasFlag(FLAG_ALWAYS_RVF) &&
        !sym->hasFlag(FLAG_LOCALE_PRIVATE)) {
      return true;
    }
  }

  if (!isFnSymbol(parent))
    return false;

  else if (sym->defPoint->parentSymbol == parent)
    return true;

  else
    return isOuterVar(sym, fn, parent->defPoint->parentSymbol);
}


//
// finds outer vars directly used in a function
//
static void
findOuterVars(FnSymbol* fn, SymbolMap* uses) {
  std::vector<SymExpr*> SEs;
  collectLcnSymExprs(fn, SEs);

  for_vector(SymExpr, symExpr, SEs) {
      Symbol* sym = symExpr->symbol();

      if (isOuterVar(sym, fn)) {
        uses->put(sym,gNil);
      }
  }
}


// Is this type OK to pass by value (e.g. it's reasonably-sized)?
static bool
passableByVal(Type* type) {
  if (is_bool_type(type)    ||
      is_int_type(type)     ||
      is_uint_type(type)    ||
      is_real_type(type)    ||
      is_imag_type(type)    ||
      is_complex_type(type) ||
      is_enum_type(type)    ||
      isClass(type)         ||
      type == dtTaskID      ||
      // For now, allow ranges as a special case, not records in general.
      type->symbol->hasFlag(FLAG_RANGE) ||
      0)
    return true;

  // TODO: allow reasonably-sized records. NB this-in-taskfns-in-ctors.chpl
  // TODO: allow reasonably-sized tuples - heterogeneous and homogeneous.

  return false;
}


// Should we pass 'sym' by reference? This is needed if 'sym' may be modified.
// Otherwise passing by value is more efficient.
static bool
passByRef(Symbol* sym) {
  //
  // If it's constant (in the sense that the value will not change),
  // there's no need to pass it by reference
  //
  if (sym->isConstValWillNotChange()) {
    return false;
  }

  if (sym->isRef()) return true;

  Type* type = sym->type;

  //
  // BHARSH: 2017-10-13: By passing ICs by value, it will be easier later in
  // the pipeline to scalar replace or hoist them. This is hardly the ideal
  // place for this 'optimization', but it's easier than revamping
  // scalarReplacement or iterators.
  //
  if (type->symbol->hasFlag(FLAG_ITERATOR_CLASS)) {
    return false;
  }

  if (sym->hasFlag(FLAG_ARG_THIS) == true &&
      passableByVal(type)         == true) {
    // This is also constant. TODO: mark with FLAG_CONST.
    // TODO: join with the passableByVal(type) test below.
    return false;
  }

  // These simply document the current state.
  INT_ASSERT(type->symbol->hasFlag(FLAG_REF) == (type->refType == NULL));

  // Coforall vars are constant, but are not marked so.
  // todo - mark them with FLAG_CONST and remove this assert,
  //        as well as the special case for FLAG_COFORALL_INDEX_VAR.
  INT_ASSERT(!sym->hasFlag(FLAG_COFORALL_INDEX_VAR) ||
             !sym->hasFlag(FLAG_CONST));

  if (sym->hasFlag(FLAG_CONST) ||
      sym->hasFlag(FLAG_COFORALL_INDEX_VAR)) {  // These are constant, too.
    if (passableByVal(type)) {
       return false;
    }
  }

  // If the above did not fire, pass it by ref if there is a refType.
  //
  // Here is why. We used to create the refType only if we needed it
  // for passing by ref.  It may be that we now add ref types by default
  // for various primitive types (and maybe other stuff), so this check
  // below might be too conservative.  But, it was (and still is) a way to
  // avoid doing more analysis (i.e., distinguishing between defs and uses).
  // Some of unnecessary by-ref passing never happens because of the above
  // checks for const-ness. Some others are reverted to by-value passing
  // in the remoteValueForwarding pass.
  //
  if (type->refType) {
    return true;
  } else {
    return false;
  }
}


static void
addVarsToFormals(FnSymbol* fn, SymbolMap* vars) {
  form_Map(SymbolMapElem, e, *vars) {
    if (Symbol* sym = e->key) {
      Type* type = sym->type;
      IntentTag intent = INTENT_BLANK;

        /* NOTE: This is still conservative.  This avoids passing
           coforall index vars by reference for non-var iterators.
           David came up with an example with nested functions and no
           iterators that would unnecessarily pass coforall index vars
           by reference.  With further analysis, we could figure out
           whether this variable is actually going to be returned as
           an LHS expr. */
      //
      // BHARSH: TODO: The arg intent set here can have a large impact on
      // RVF later on. For RVF to be more effective, this might be a good
      // place to do some analysis and mark arguments as 'const in' and
      // 'const ref', even if the actual is not marked with FLAG_CONST.
      //
      // Prior to the QualifiedType changes this section would make the type
      // something like _ref_int, but the intent would be INTENT_CONST_IN and
      // RVF would fire in some situations.
      //
      if (passByRef(sym)) {
        // The task function can take in its argument by REF_MAYBE_CONST
        // no matter the type. This enables e.g. a task function processing
        // array elements to correctly set array argument intent.
        IntentTag temp = INTENT_REF_MAYBE_CONST;
        if (sym->isConstant()) {
          temp = INTENT_CONST_REF;
        }
        intent = concreteIntent(temp, type);
        type = type->getValType()->refType;
      } else {
        IntentTag temp = INTENT_BLANK;
        if (sym->isConstant() && sym->isRef()) {
          // Allows for RVF later
          temp = INTENT_CONST_REF;
        }
        intent = concreteIntent(temp, type);
      }

      SET_LINENO(sym);
      ArgSymbol* arg = new ArgSymbol(intent, sym->name, type);
      if (sym->hasFlag(FLAG_ARG_THIS))
          arg->addFlag(FLAG_ARG_THIS);
      if (sym->hasFlag(FLAG_REF_TO_IMMUTABLE))
          arg->addFlag(FLAG_REF_TO_IMMUTABLE);
      if (sym->hasFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT))
          arg->addFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT);
      if (sym->hasFlag(FLAG_COFORALL_INDEX_VAR))
          arg->addFlag(FLAG_COFORALL_INDEX_VAR);
      arg->addFlag(FLAG_OUTER_VARIABLE);

      fn->insertFormalAtTail(new DefExpr(arg));
      vars->put(sym, arg);
    }
  }
}

static void
replaceVarUsesWithFormals(FnSymbol* fn, SymbolMap* vars) {
  if (vars->n == 0) return;

  std::vector<SymExpr*> symExprs;

  collectSymExprs(fn->body, symExprs);

  size_t firstInLifetimeConstraint = symExprs.size();
  if (fn->lifetimeConstraints)
    collectSymExprs(fn->lifetimeConstraints, symExprs);

  form_Map(SymbolMapElem, e, *vars) {
    if (Symbol* sym = e->key) {
      ArgSymbol* arg  = toArgSymbol(e->value);
      Type*      type = arg->type;

      size_t i = 0;
      for_vector(SymExpr, se, symExprs) {
        if (se->symbol() == sym) {
          if (type == sym->type) {
            se->setSymbol(arg);

          } else if (CallExpr* call = toCallExpr(se->parentExpr)) {
            FnSymbol* fnc         = call->resolvedFunction();
            bool      canPassToFn = false;

            if (fnc) {
              ArgSymbol* form = actual_to_formal(se);

              if (arg->isRef()                            &&
                  form->isRef()                           &&
                  arg->getValType() == form->getValType()) {
                canPassToFn = true;
              } else if (arg->type == form->type) {
                canPassToFn = true;
              }
            }

            // check if call is in a lifetime clause
            if (i >= firstInLifetimeConstraint)
              canPassToFn = true;

            if (( (call->isPrimitive(PRIM_MOVE)       ||
                   call->isPrimitive(PRIM_ASSIGN)     ||
                   call->isPrimitive(PRIM_SET_MEMBER) )
                  && call->get(1) == se)                                   ||
                call->isPrimitive(PRIM_GET_MEMBER)                         ||
                call->isPrimitive(PRIM_GET_MEMBER_VALUE)                   ||
                call->isPrimitive(PRIM_WIDE_GET_LOCALE)                    ||
                call->isPrimitive(PRIM_WIDE_GET_NODE)                      ||
                call->isPrimitive(PRIM_END_OF_STATEMENT)                   ||
                canPassToFn) {
              se->setSymbol(arg); // do not dereference argument in these cases

            } else if (call->isPrimitive(PRIM_ADDR_OF)) {
              SET_LINENO(se);
              call->replace(new SymExpr(arg));

            } else {
              SET_LINENO(se);

              VarSymbol* tmp   = newTemp(sym->type);
              CallExpr*  deref = new CallExpr(PRIM_DEREF, arg);
              CallExpr*  move  = new CallExpr(PRIM_MOVE,  tmp, deref);

              se->getStmtExpr()->insertBefore(new DefExpr(tmp));
              se->getStmtExpr()->insertBefore(move);

              se->setSymbol(tmp);
            }

          } else {
            // So far, the only other known case is when 'se' is some
            // shadow variable's outer sym. If so, just replace the symbol.
            ShadowVarSymbol* svar = toShadowVarSymbol(se->parentSymbol);
            INT_ASSERT(svar && se == svar->outerVarSE);
            se->setSymbol(arg);
          }
        }
        i++;
      }
    }
  }
}


static void
addVarsToActuals(CallExpr* call, SymbolMap* vars, bool outerCall) {
  form_Map(SymbolMapElem, e, *vars) {
    if (Symbol* sym = e->key) {
      SET_LINENO(sym);
      call->insertAtTail(sym);
    }
  }
}

static void deleteCalledby(FnSymbol* fn) {
  if (fn->calledBy != NULL)  { delete fn->calledBy; fn->calledBy = NULL; }
}
static void deleteAllCalledby() {
  for_alive_in_Vec(FnSymbol, fn, gFnSymbols)  deleteCalledby(fn);
}

void flattenNestedFunctions(Vec<FnSymbol*>& nestedFunctions) {
  if (fVerify) deleteAllCalledby();

  Vec<FnSymbol*> outerFunctionSet;
  Vec<FnSymbol*> nestedFunctionSet;

  forv_Vec(FnSymbol, fn, nestedFunctions)
    nestedFunctionSet.set_add(fn);

  Map<FnSymbol*,SymbolMap*> args_map;

  forv_Vec(FnSymbol, fn, nestedFunctions) {
    SymbolMap* uses = new SymbolMap();

    findOuterVars(fn, uses);

    args_map.put(fn, uses);
  }

  // iterate to get outer vars in a function based on outer vars in
  // functions it calls
  // Also handle finding outer functions that are calling an
  // inner function, since these will also need the new arguments.
  bool change;

  do {
    change = false;

    forv_Vec(FnSymbol, fn, nestedFunctions) {
      if (!fVerify) deleteCalledby(fn);
      computeAllCallSites(fn);

      std::vector<BaseAST*> asts;
      collect_top_asts(fn, asts);

      SymbolMap* uses = args_map.get(fn);

      for_vector(BaseAST, ast, asts) {
        if (CallExpr* call = toCallExpr(ast)) {
          if (call->isResolved()) {
            if (FnSymbol* fcall = call->findFnSymbol()) {
              SymbolMap* call_uses = args_map.get(fcall);

              if (call_uses) {
                form_Map(SymbolMapElem, e, *call_uses) {
                  if (isOuterVar(e->key, fn) && !uses->get(e->key)) {
                    uses->put(e->key, gNil);
                    change = true;
                  }
                }
              }
            }
          }
        }
      }

      forv_Vec(CallExpr, call, *fn->calledBy) {
        //
        // call not in a nested function; handle the toFollower/toLeader cases
        // Note: outerCall=true implies the 'call' does not see defPoint
        // of the var 'use->key' anywhere in call's enclosing scopes. With
        // toFollower/toLeader, the 'call' does not see defPoint of 'fn'
        // either.
        //
        bool outerCall = false;

        if (FnSymbol* parent = toFnSymbol(call->parentSymbol)) {
          if (!nestedFunctionSet.set_in(parent)) {
            form_Map(SymbolMapElem, use, *uses) {
              if (use->key->defPoint->parentSymbol != parent &&
                  !isOuterVar(use->key, parent)) {
                outerCall = true;
              }
            }

            if (outerCall) {
              SymbolMap* usesCopy = new SymbolMap();

              outerFunctionSet.set_add(parent);
              nestedFunctionSet.set_add(parent);
              nestedFunctions.add(parent);


              form_Map(SymbolMapElem, use, *uses) {
                usesCopy->put(use->key, gNil);
              }

              args_map.put(parent, usesCopy);

              change = true;
            }
          }
        }
      }
    }
  } while (change);

  // update all call sites of nested functions this must be done
  // before updating the function so that the outer var actuals can be
  // updated with the outer var functions when the formals are updated
  // (in nested functions that call one another)
  forv_Vec(FnSymbol, fn, nestedFunctions) {
    SymbolMap* uses = args_map.get(fn);

    forv_Vec(CallExpr, call, *fn->calledBy) {
      bool outerCall = false;

      if (FnSymbol* parent = toFnSymbol(call->parentSymbol))
        outerCall = outerFunctionSet.set_in(parent);

      addVarsToActuals(call, uses, outerCall);
    }
  }

  // move nested functions to module level
  forv_Vec(FnSymbol, fn, nestedFunctions) {
    if (isFnSymbol(fn->defPoint->parentSymbol)) {
      ModuleSymbol* mod = fn->getModule();
      Expr* def = fn->defPoint;
      def->remove();
      mod->block->insertAtTail(def);
    }
  }

  // add extra formals to nested functions
  forv_Vec(FnSymbol, fn, nestedFunctions)
    addVarsToFormals(fn, args_map.get(fn));

  // replace outer variable uses with added formals
  forv_Vec(FnSymbol, fn, nestedFunctions)
    replaceVarUsesWithFormals(fn, args_map.get(fn));

  //
  // remove types from functions
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (FnSymbol* fn = toFnSymbol(ts->defPoint->parentSymbol)) {
      fn->defPoint->insertBefore(ts->defPoint->remove());
    }
  }
}
