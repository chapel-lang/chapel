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

#include "lateConstCheck.h"

#include "AggregateType.h"
#include "driver.h"
#include "expr.h"
#include "iterator.h"
#include "resolution.h"
#include "symbol.h"

/* This file implements late (after cull over references)
   const checking.
   
   Const checking can't be complete before then since
   it's not known if ref or value or const ref return
   overloads will be used.
 */

static bool isTupleOfTuples(Type* t)
{
  AggregateType* at = toAggregateType(t->getValType());

  if (at && at->symbol->hasFlag(FLAG_TUPLE)) {
    for_fields(field, at) {
      if (field->getValType()->symbol->hasFlag(FLAG_TUPLE))
        return true;
    }
  }

  return false;
}

static void printReason(BaseAST* reason, BaseAST** lastPrintedReason)
{
  // First, figure out the module and function it's in
  Expr* expr = toExpr(reason);
  if (Symbol* s = toSymbol(reason))
    expr = s->defPoint;
  ModuleSymbol* inModule = expr->getModule();
  FnSymbol* inFunction = NULL;

  if (FnSymbol* fn = toFnSymbol(reason))
    inFunction = fn;
  else
    inFunction = expr->getFunction();

  // We'll output differently based upon whether it's
  // in a user-defined module or a compiler-generated function.
  bool user = inModule->modTag == MOD_USER;
  bool compilerGenerated = false;
  if (inFunction != NULL)
    compilerGenerated = inFunction->hasFlag(FLAG_COMPILER_GENERATED);

  BaseAST* last = *lastPrintedReason;
  bool same = (last != NULL &&
               reason->fname() == last->fname() &&
               reason->linenum() == last->linenum());

  if (developer || (user && !compilerGenerated && !same)) {
    if (isArgSymbol(reason) || isFnSymbol(reason))
      USR_PRINT(reason, "to ref formal here");
    else if (TypeSymbol* ts = toTypeSymbol(reason))
      USR_PRINT(reason, "to formal of type %s", ts->name);
    else
      USR_PRINT(reason, "passed as ref here");

    // useful for debugging this pass
    if (developer)
      USR_PRINT(reason, "id %i", reason->id);
  } else {
    if (TypeSymbol* ts = toTypeSymbol(reason))
      USR_PRINT("to formal of type %s", ts->name);
  }

  *lastPrintedReason = reason;
}


/*
If 'fn' is a leader iterator, it does not access its formals
the way the corresponding follower iterator does.
If a formal has the "ref-maybe-const" intent, it may turn into
"const-ref" for the leader even though it IS modified
by the follower and so is racy.
This may happen, for example for receivers of record methods.

So, instead of the leader we will analyze the corresponding
serial iterator, if we can get to it easily.
Getting the follower, which is probably more adequate, is tricker.
*/
static FnSymbol* getSerialIterator(FnSymbol* fn) {
  if (IteratorGroup* igroup = fn->iteratorGroup)
    if (fn == igroup->leader)
      return igroup->serial;

  // Otherwise stick with the original 'fn', ex. for standalone.
  return fn;
}

/* Since const-checking can depend on ref-pair determination
   or upon the determination of whether an array formal with
   blank intent is passed by ref or by value, do final const checking here.

   TODO: decide if we also need const checking in functionResolution.cpp.
 */
void lateConstCheck(std::map<BaseAST*, BaseAST*> * reasonNotConst)
{
  forv_Vec(CallExpr, call, gCallExprs) {

    // Ignore calls removed earlier by this pass.
    if (call->parentExpr == NULL) {
      continue;
    }

    if (FnSymbol* calledFn = call->resolvedFunction()) {
      char        cn1          = calledFn->name[0];
      const char* calleeParens = (isalpha(cn1) || cn1 == '_') ? "()" : "";
      int formalIdx = 0;

      // resolved calls
      for_formals_actuals(formal, actual, call) {
        bool error = false;
        formalIdx++;

        if (actual->qualType().isConst() && ! formal->qualType().isConst()) {
          // But... there are exceptions

          // If the formal intent is INTENT_REF_MAYBE_CONST,
          // earlier cullOverReferences should have changed it
          // to INTENT_REF or INTENT_CONST_REF. If not, it's something
          // that was ignored by earlier portions of this pass.
          if (formal->intent == INTENT_REF_MAYBE_CONST) {
            // OK, not an error

          // it's OK if we're calling a function marked
          // FLAG_REF_TO_CONST_WHEN_CONST_THIS and the result is
          // marked const. In that case, we pretend that the `this`
          // argument would be marked const too.
          } else if (calledFn->hasFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS) &&
                     formal->hasFlag(FLAG_ARG_THIS)) {
            CallExpr* move = toCallExpr(call->parentExpr);
            if (move && move->isPrimitive(PRIM_MOVE)) {
              SymExpr* lhs = toSymExpr(move->get(1));
              Symbol* lhsSym = lhs->symbol();
              if (lhsSym->qualType().isConst())
                ; // OK, lhsSym has const Qualifier
              else if (lhsSym->hasFlag(FLAG_REF_TO_CONST))
                ; // OK, lhsSym is marked with FLAG_REF_TO_CONST
              else
                error = true; // l-value error
            }

          // Or, if passing a 'const' thing into an 'in' formal,
          // that's OK
          } else if (formal->intent == INTENT_IN &&
                     !formal->type->symbol->hasFlag(FLAG_COPY_MUTATES)) {
            // OK, not an error
          } else {
            error = true;
          }
        }

        // check for a build_tuple call containing e.g. Owned

        if (calledFn->hasFlag(FLAG_BUILD_TUPLE) &&
            !calledFn->hasFlag(FLAG_ALLOW_REF)) {
          if (actual->qualType().isConst() &&
              actual->getValType()->symbol->hasFlag(FLAG_COPY_MUTATES)) {
            USR_FATAL_CONT(actual,
                           "argument %i for tuple construction is const "
                           "but tuple construction takes ownership",
                           formalIdx
                          );
          }
        }


        // TODO: check tuple const-ness:
        //   make analysis above more complete
        //   work with toSymExpr(actual)->symbol()->fieldQualifiers
        //   handle tuples containing tuples properly

        FnSymbol* inFn = call->parentSymbol->getFunction();

        // Ignore errors in functions marked with FLAG_SUPPRESS_LVALUE_ERRORS.
        if (inFn->hasFlag(FLAG_SUPPRESS_LVALUE_ERRORS)) {
          error = false;
        }

        // A 'const' record should be able to be initialized
        if (calledFn->isInitializer() || calledFn->isCopyInit()) {
          error = false;
        }

        // A 'const' record should be able to be destroyed
        if (calledFn->name == astrDeinit ||
            calledFn->hasFlag(FLAG_AUTO_DESTROY_FN) ||
            calledFn->hasFlag(FLAG_DESTRUCTOR)) {
          error = false;
        }

        // For now, ignore errors with tuple construction/build_tuple
        if (calledFn->hasFlag(FLAG_BUILD_TUPLE) ||
            calledFn->hasFlag(FLAG_INIT_TUPLE)) {
          error = false;
        }

        // For now, ignore errors with tuples-of-tuples.
        // Otherwise errors with e.g.
        //   const tup = (("a", 1), ("b", 2));
        //   for x in tup { writeln(x); }
        if (isTupleOfTuples(formal->type)) {
          error = false;
        }

        // For now, ignore errors with calls to promoted functions.
        // To turn this off, get this example working:
        //   test/functions/ferguson/ref-pair/plus-reduce-field-in-const.chpl
        if (calledFn->hasFlag(FLAG_PROMOTION_WRAPPER)) {
          error = false;
        }

        if (error) {
          const char* calledName = calledFn->name;

          if (calledFn->hasFlag(FLAG_AUTO_COPY_FN)) {
            calledName = "implicit copy initializer";
            calleeParens = "";
          }
          if (calledFn->hasFlag(FLAG_INIT_COPY_FN)) {
            calledName = "copy initializer";
            calleeParens = "";
          }

          USR_FATAL_CONT(actual,
                         "const actual is passed to %s formal '%s' of %s%s",
                         formal->intentDescrString(),
                         formal->name,
                         calledName, calleeParens);

          BaseAST* lastPrintedReason = NULL;

          printReason(formal->getValType()->symbol, &lastPrintedReason);

          SymExpr* actSe = toSymExpr(actual);

          if (actSe != NULL &&
              actSe->symbol()->hasFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT)) {
            printTaskOrForallConstErrorNote(actSe->symbol());
          }

          printReason(formal, &lastPrintedReason);

          if (reasonNotConst != NULL) {
            BaseAST* reason     = (*reasonNotConst)[formal];

            BaseAST* lastReason = formal;

            while (reason) {
              BaseAST* printCause = reason;

              // If the last reason and the this reason are both Symbols,
              // try to figure out what links them by looking at uses
              // of lastReason.
              if (isSymbol(lastReason) && isArgSymbol(reason)) {
                Symbol*    lastSym   = toSymbol(lastReason);
                ArgSymbol* curFormal = toArgSymbol(reason);

                for_SymbolSymExprs(se, lastSym) {
                  if (CallExpr* parentCall = toCallExpr(se->parentExpr)) {
                    if (parentCall->isResolved()) {
                      if (curFormal == actual_to_formal(se)) {
                        printReason(se, &lastPrintedReason);
                        break;
                      }
                    }
                  }
                }
              }

              // Go from LHS VarSymbol to called fn
              // for better reporting for the reason of context-call
              if (VarSymbol* v = toVarSymbol(reason)) {
                for_SymbolDefs(def, v) {
                  if (CallExpr* parentCall = toCallExpr(def->parentExpr)) {
                    if (parentCall->isPrimitive(PRIM_MOVE)) {
                      if (CallExpr* rhsCall = toCallExpr(parentCall->get(2))) {
                        if (FnSymbol* rhsCalledFn = rhsCall->resolvedFunction()) {
                          printReason(def,         &lastPrintedReason);
                          printReason(rhsCalledFn, &lastPrintedReason);
                          printCause = NULL;

                          break;
                        }
                      }
                    }
                  }
                }
              }

              if (printCause) {
                // Print out an annotation line
                printReason(printCause, &lastPrintedReason);
              }

              if (reasonNotConst->count(reason) != 0) {
                lastReason = reason;
                reason     = (*reasonNotConst)[reason];
              } else {
                break;
              }
            }
          }
        }
      }
    }

    // For now, don't check primitives. Compiler can be loose
    // with const-ness on its own internal temporaries.
  }

  // Additionally check that promotion wrappers using a scalar this
  // don't take it in by `ref`
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_PROMOTION_WRAPPER) &&
        fn->hasFlag(FLAG_INLINE_ITERATOR)    )
    {
      fn = getSerialIterator(fn);
      for_formals(formal, fn) {
        if (formal->intent == INTENT_REF) {
          Type* vt = formal->getValType();
          if (vt->scalarPromotionType == NULL &&
              !(isAtomicType(vt) || isSyncType(vt) || isSingleType(vt)) &&
              !formal->hasFlag(FLAG_ERROR_VARIABLE)) {
            if (formal == fn->_this)
              USR_FATAL_CONT(fn, "Racy promotion of scalar method receiver");
            else
              USR_FATAL_CONT(fn,
                    "Racy promotion of scalar argument for the formal '%s'",
                    formal->name);
          }
        }
      }
    }
  }
}
