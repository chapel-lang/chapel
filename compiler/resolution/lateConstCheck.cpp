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

#include "lateConstCheck.h"

#include "AggregateType.h"
#include "driver.h"
#include "expr.h"
#include "iterator.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "symbol.h"

#include <set>

/* This file implements late (after cull over references)
   const checking.
   
   Const checking can't be complete before then since
   it's not known if ref or value or const ref return
   overloads will be used.
 */

// Map pointing to first occurring use for a symbol.
typedef std::map<BaseAST*, BaseAST*> UseMap;

// Used for debugging this pass.
static const int breakOnId1 = 0;
static const int breakOnId2 = 0;
static const int breakOnId3 = 0;

static const bool doPrintDebugInfo = false;

#define DEBUG_SYMBOL(sym__) \
  do { \
    if (sym__->id == breakOnId1 || sym__->id == breakOnId2 || \
        sym__->id == breakOnId3) { \
      gdbShouldBreakHere(); \
    } \
  } while (0)

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

static bool isTupleFunctionToSkip(FnSymbol* calledFn) {

  // Skip chpl__unref for tuples because it's triggering weird constness
  // violations. TODO: Remove when the below can pass without the check:
  //    release/examples/spec/Tuples/tuple-return-behavior.chpl
  if (calledFn->hasFlag(FLAG_UNREF_FN)) {
    return true;
  }

  // Skip chpl__initCopy because it modifies formal intents in weird ways
  // when the formal requires FLAG_COPY_MUTATE (e.g. owned or tuples of
  // owned).
  if (calledFn->hasFlag(FLAG_INIT_COPY_FN)) {
    return true;
  }

  // Don't delve into chpl__autoCopy.
  if (calledFn->hasFlag(FLAG_AUTO_COPY_FN)) {
    return true;
  }

  return false;
}

// Print use chains when debugging.
static void printFormalUseChain(ArgSymbol* formal, UseMap* um) {
  if (um == NULL) {
    return;
  }

  printf("Printing use chain for formal %s [%d]\n",
            formal->name,
            formal->id);

  BaseAST* tmp = formal;
  int count = 0;

  while (tmp != NULL) {
    printf("%d: %d\n", count, tmp->id);
    tmp = um->count(tmp) ? um->at(tmp) : NULL;
    count++;
  }
}

// Are a tuple's field qualifiers ref instead of const? If so, then some
// code somewhere set a const tuple element...
static bool checkTupleFormalUses(FnSymbol* calledFn, ArgSymbol* formal,
                                 UseMap* um) {

  if (isTupleFunctionToSkip(calledFn)) {
    return false;
  }

  // Leave if formal is not a tuple.
  AggregateType* at = toAggregateType(formal->getValType());
  if (at == NULL || !at->symbol->hasFlag(FLAG_TUPLE)) {
    return false;
  }

  // Leave if we have no info about this formal's constness.
  if (formal->fieldQualifiers == NULL) {
    return false;
  }

  bool isFormalIntentMaybeConst = formal->intent == INTENT_REF_MAYBE_CONST;
  bool isFormalIntentConst = formal->intent & INTENT_CONST;
  bool isFormalIntentRef = formal->intent & INTENT_REF;

  // Leave if formal intent is ref.
  if (isFormalIntentRef && !isFormalIntentConst) {
    return false;
  }

  if (isFormalIntentConst && !formal->qualType().isConst()) {
    INT_FATAL(formal, "has const intent %s but not a const qualtype",
                      intentDescrString(formal->intent));
  }

  DEBUG_SYMBOL(formal);

  // True if errors were emitted for any tuple field.
  bool result = false;

  int fieldIdx = 0;
  for_fields(field, at) {
    fieldIdx++;

    Qualifier q = formal->fieldQualifiers[fieldIdx];

    // Skip non-ref fields.
    if (!field->isRef()) {
      INT_ASSERT(q == QUAL_UNKNOWN);
      continue;
    }

    bool isFieldMarkedConst = QualifiedType::qualifierIsConst(q);

    // Skip ref tuple fields that are never set.
    if (isFieldMarkedConst) {
      continue;
    }

    // TODO: Special handling for managed class wrappers?
    Type* ft = field->getValType();

    // TODO: Handle tuples containing tuples.
    if (ft->symbol->hasFlag(FLAG_TUPLE)) {
      continue;
    }

    // Select the "field intent" based on the formal intent.
    IntentTag fieldIntent = INTENT_BLANK;
    if (isFormalIntentMaybeConst) {
      fieldIntent = blankIntentForType(ft);
    } else if (isFormalIntentConst) {
      fieldIntent = constIntentForType(ft);
    } else {
      INT_FATAL(formal, "unhandled intent %s for formal, field %d",
                        intentDescrString(formal->intent),
                        fieldIdx);
    }
    
    // Check ref/ref-if-modified fields in "checkTupleFormalToActual".
    if (fieldIntent == INTENT_REF_MAYBE_CONST ||
        fieldIntent == INTENT_REF) {
      INT_ASSERT(!isFormalIntentConst);
      continue;
    }

    // The tuple intent is const, but it is used somewhere...
    if ((fieldIntent & INTENT_CONST) && !isFieldMarkedConst) {
      BaseAST* use = NULL;

      // Only fetch the closest use if the UseMap exists.
      if (um != NULL && um->count(formal)) {
        use = um->at(formal);
      }

      // Use the DefExpr only once to print the containing function.
      BaseAST* pin = !result ? (BaseAST*) formal->defPoint : formal;

      USR_FATAL_CONT(pin, "tuple formal '%s' of '%s' is const and "
                          "cannot be modified",
                          formal->name,
                          calledFn->name);
                          
      // TODO: Pin IFF the element is a user type.
      USR_PRINT("tuple element #%d of type %s",
                fieldIdx-1,
                ft->symbol->name);

      // TODO: Cannot indicate which field was set with the current UseMap.
      // We need to adjust the key type (maybe to GraphNode) to store the
      // field index before we can use this functionality.
      if (use != NULL) {
        USR_PRINT(use, "possibly set here");
      }

      if (doPrintDebugInfo) {
        printFormalUseChain(formal, um);
      }

      result = true;
    }
  }

  return result;
}

// TODO: Could we could get rid of this if we just propagated constness to
// coerce temporaries instead?
static Symbol* getOriginalTupleFromCoerceTmp(Symbol* sym) {
  Symbol* result = NULL;

  AggregateType* at = toAggregateType(sym->getValType());
  if (at == NULL || !at->symbol->hasFlag(FLAG_TUPLE)) {
    return NULL;
  }

  // Find a PRIM_SET_MEMBER for the coerce temp. The third argument is
  // the read field temporary.
  //    ('.=' coerce_tmp x0 read_x0)
  for_SymbolSymExprs(se, sym) {

    // The parent call should be a PRIM_SET_MEMBER.
    CallExpr* set = toCallExpr(se->parentExpr);
    if (set == NULL || !set->isPrimitive(PRIM_SET_MEMBER)) {
      continue;
    }

    // Make sure the coerce temp is in the correct position.
    SymExpr* coerceTmpLhs = toSymExpr(set->get(1));
    if (coerceTmpLhs == NULL || coerceTmpLhs->symbol() != sym) {
      continue;
    }

    // Third argument should be the read temp. 
    SymExpr* fromReadTmp = toSymExpr(set->get(3));
    if (fromReadTmp == NULL) {
      continue;
    }

    Symbol* readTmp = fromReadTmp->symbol();

    // Find a call to PRIM_MOVE for the read field. The RHS expression
    // will contain the original actual.
    //    ('move' read_x0 ('.' tup x0))
    for_SymbolSymExprs(useReadTmp, readTmp) {
      CallExpr* move = toCallExpr(useReadTmp->parentExpr);
      if (move == NULL || !move->isPrimitive(PRIM_MOVE)) {
        continue;
      }

      SymExpr* lhsUse = toSymExpr(move->get(1));
      Symbol* lhs = lhsUse->symbol();

      if (lhs != readTmp) {
        continue;
      }

      // Get the PRIM_GET_MEMBER from the move expression.
      //    ('.' tup x0)
      CallExpr* get = toCallExpr(move->get(2));
      if (get == NULL || !get->isPrimitive(PRIM_GET_MEMBER)) {
        continue;
      }

      // The first argument is the original actual.
      SymExpr* useOriginal = toSymExpr(get->get(1));
      INT_ASSERT(useOriginal != NULL);

      // Done!
      result = useOriginal->symbol();
    }

    if (result != NULL) {
      break;
    }
  }

  return result;
}

// Check REF (e.g. atomic) and REF_IF_MODIFIED (e.g. array) tuple elements
// and compare formals to actuals. If the formal is const and the actual
// is not, then emit an error. Skip if the tuple formal intent is not
// REF_IF_MODIFIED.
static bool checkTupleFormalToActual(ArgSymbol* formal, Expr* actual,
                                     CallExpr* call, UseMap* um) {

  FnSymbol* calledFn = call->resolvedFunction();
  INT_ASSERT(calledFn != NULL);

  if (isTupleFunctionToSkip(calledFn)) {
    return false;
  }
 
  AggregateType* at = toAggregateType(formal->getValType());
  if (at == NULL || !at->symbol->hasFlag(FLAG_TUPLE)) {
    return false;
  }

  // Nothing to do if the tuple formal is not REF_MAYBE_CONST.
  if (!(formal->intent == INTENT_REF_MAYBE_CONST)) {
    return false;
  }

  // Leave if we have no info about this formal's constness.
  if (formal->fieldQualifiers == NULL) {
    return false;
  }

  DEBUG_SYMBOL(formal);

  // True if errors were emitted for any tuple field.
  bool result = false;

  int fieldIdx = 0;
  for_fields(field, at) {
    fieldIdx++;

    Qualifier q = formal->fieldQualifiers[fieldIdx];

    // Skip non-ref fields.
    if (!field->isRef()) {
      INT_ASSERT(q == QUAL_UNKNOWN);
      continue;
    }

    bool isFieldMarkedConst = QualifiedType::qualifierIsConst(q);
    Type* ft = field->getValType();

    // TODO: Handle tuples containing tuples.
    if (ft->symbol->hasFlag(FLAG_TUPLE)) {
      continue;
    }

    IntentTag fieldIntent = blankIntentForType(ft);

    // Only worry about REF and REF_IF_MODIFIED formals.
    if (fieldIntent != INTENT_REF_MAYBE_CONST &&
        fieldIntent != INTENT_REF) {
      continue;
    }

    // If a ref-if-modified field is never set, then it's const.
    if (fieldIntent == INTENT_REF_MAYBE_CONST && isFieldMarkedConst) {
      continue;
    }

    bool isActualFieldConst = false;
    bool isActualConst = false;
    Symbol* actualSym = NULL;

    // Determine where the actual is coming from and if its origin is const
    // or not. There are three cases that we cover so far:
    //
    //    - coerce tmps: when a value tuple (e.g. VarSymbol) is coerced to
    //                   a referential tuple.
    //    - call tmps: look for a _build_tuple call (there should be exactly
    //                 one for each tuple expression). If we find it then
    //                 we check if the argument to the call is const.
    //    - ArgSymbol: great, it should already be a referential tuple and
    //                 we can check its constness.
    //
    if (SymExpr* se = toSymExpr(actual)) {
      Symbol* sym = se->symbol();

      INT_ASSERT(toAggregateType(sym->getValType()) == at);

      // Case: Actual is coerced from value tuple.
      if (sym->hasFlag(FLAG_COERCE_TEMP)) {

        // Walk backwards from coerce_tmp to original value tuple.
        Symbol* original = getOriginalTupleFromCoerceTmp(sym);

        if (original == NULL) {
          INT_FATAL(sym, "Unable to unpack coercion temp: %d", sym->id);
        }

        if (original->qualType().isConst()) {
          DEBUG_SYMBOL(sym);

          isActualFieldConst = true;
          isActualConst = true;
          actualSym = original;
        }

      // Case: Actual is referential tuple built from expression.
      } else if (sym->hasFlag(FLAG_TEMP)) {
        CallExpr* build = NULL;

        // Fetch the _build_tuple call for the actual.
        for_SymbolSymExprs(se, sym) {
          CallExpr* move = toCallExpr(se->parentExpr);
          if (move == NULL || !move->isPrimitive(PRIM_MOVE)) {
            continue;
          }

          SymExpr* useCheck = toSymExpr(move->get(1));
          Symbol* check = useCheck->symbol();
          if (check != sym) {
            continue;
          }

          CallExpr* buildCall = toCallExpr(move->get(2));
          if (buildCall == NULL) {
            continue;
          }

          FnSymbol* buildFn = buildCall->resolvedFunction();
          if (buildFn == NULL || !buildFn->hasFlag(FLAG_BUILD_TUPLE)) {
            continue;
          }

          build = buildCall;
        }

        // Check the argument to _build_tuple. Is it const?
        if (build != NULL) {
          Expr* buildArg = build->get(fieldIdx);

          INT_ASSERT(buildArg->getValType() == field->getValType());

          SymExpr* buildSymExpr = toSymExpr(buildArg);
          if (buildSymExpr != NULL) {

            Symbol* buildSym = buildSymExpr->symbol();
            if (buildSym->qualType().isConst()) {
              DEBUG_SYMBOL(buildSym);

              isActualFieldConst = true;
              actualSym = buildSym;
            }
          }
        }

      // Case: Actual is a formal.
      } else if (ArgSymbol* arg = toArgSymbol(sym)) {

        // But not _this_ formal...
        if (arg != NULL && arg != formal) {
          DEBUG_SYMBOL(arg);

          if (arg->intent & INTENT_CONST && arg->qualType().isConst()) {
            isActualFieldConst = true;
            isActualConst = true;
            actualSym = arg;
          } else if (arg->intent == INTENT_REF_MAYBE_CONST) {

            if (fieldIntent == INTENT_REF_MAYBE_CONST) {
              Qualifier q = arg->fieldQualifiers[fieldIdx];
              isActualFieldConst = QualifiedType::qualifierIsConst(q);
              actualSym = arg;
            }
          }
        }
      }
    } else {
      INT_FATAL(actual, "unhandled actual %d", actual->id);
    }

    if (isActualFieldConst && !isFieldMarkedConst) {
      INT_ASSERT(actualSym != NULL);

      BaseAST* use = NULL;
      if (um != NULL && um->count(formal)) {
        use = um->at(formal);
      }

      USR_FATAL_CONT(actual, "const actual element is passed to %s tuple "
                             "formal '%s' of '%s'",
                             intentDescrString(INTENT_REF),
                             formal->name,
                             calledFn->name);

      // TODO: Pin if the element is a user type.
      USR_PRINT("tuple element #%d of type '%s'",
                fieldIdx-1,
                ft->symbol->name);

      const char* descriptor = isActualConst ? "tuple" : "element";
      USR_PRINT(actualSym->defPoint, "const %s declared here",
                                     descriptor);

      if (fieldIntent == INTENT_REF_MAYBE_CONST) {
        USR_PRINT(use, "formal element has %s due to modification, "
                       "possibly here",
                       intentDescrString(INTENT_REF));

        if (doPrintDebugInfo) {
          printFormalUseChain(formal, um);
        }
      }

      result = true;
    }
  }

  return result;
}

static bool isFunctionToSkip(FnSymbol* calledFn) {

  // A 'const' record should be able to be initialized.
  if (calledFn->isInitializer() || calledFn->isCopyInit()) {
    return true;
  }

  // A 'const' record should be able to be destroyed.
  if (calledFn->name == astrDeinit ||
      calledFn->hasFlag(FLAG_AUTO_DESTROY_FN) ||
      calledFn->hasFlag(FLAG_DESTRUCTOR)) {
    return true;
  }

  // For now, ignore errors with tuple construction/build_tuple.
  if (calledFn->hasFlag(FLAG_BUILD_TUPLE) ||
      calledFn->hasFlag(FLAG_INIT_TUPLE)) {
    return true;
  }

  // For now, ignore errors with calls to promoted functions.
  // To turn this off, get this example working:
  //   test/functions/ferguson/ref-pair/plus-reduce-field-in-const.chpl
  if (calledFn->hasFlag(FLAG_PROMOTION_WRAPPER)) {
    return true;
  }

  return false;
}

/* Since const-checking can depend on ref-pair determination
   or upon the determination of whether an array formal with
   blank intent is passed by ref or by value, do final const checking here.

   TODO: decide if we also need const checking in functionResolution.cpp.
 */
void lateConstCheck(std::map<BaseAST*, BaseAST*> * reasonNotConst) {
  std::set<FnSymbol*> visitedFunctions;

  forv_Vec(CallExpr, call, gCallExprs) {

    // Ignore calls removed earlier by this pass.
    if (call->parentExpr == NULL)
      continue;

    if (FnSymbol* calledFn = call->resolvedFunction()) {
      char        cn1          = calledFn->name[0];
      const char* calleeParens = (isalpha(cn1) || cn1 == '_') ? "()" : "";
      int formalIdx = 0;

      // Run checks for tuple formals that are not REF/IF-MODIFIED once.
      if (!visitedFunctions.count(calledFn)) {
        visitedFunctions.insert(calledFn);

        for_formals(formal, calledFn) {
          if (!isFunctionToSkip(calledFn)) {
            checkTupleFormalUses(calledFn, formal, reasonNotConst);
          }
        }
      }

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
          } else if ((formal->intent == INTENT_IN ||
                      formal->originalIntent == INTENT_IN) &&
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

        // Is the called function a case error reporting should ignore?
        bool skip = isFunctionToSkip(calledFn);

        // Skip if in function marked with FLAG_SUPPRESS_LVALUE_ERRORS.
        FnSymbol* inFn = call->parentSymbol->getFunction();
        if (inFn->hasFlag(FLAG_SUPPRESS_LVALUE_ERRORS)) {
          skip = true;
        }

        if (!skip) {
          checkTupleFormalToActual(formal, actual, call, reasonNotConst);
        }

        // TODO: Really ought to incorporate tuple error messages into this.
        if (!skip && error) {
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
