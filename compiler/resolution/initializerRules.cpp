/*
 * Copyright 2004-2016 Cray Inc.
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

#include "initializerRules.h"

#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"

// Helper file for verifying the rules placed on initializers, and providing
// the extra functionality associated with them.

void temporaryInitializerFixup(CallExpr* call) {
  if (UnresolvedSymExpr* usym = toUnresolvedSymExpr(call->baseExpr)) {
    // Support super.init() calls (for instance) when the super type does not
    // define either an initializer or a constructor.  Also ignores errors from
    // improperly inserted .init() calls (so be sure to check here if something
    // is behaving oddly - Lydia, 08/19/16)
    if (!strcmp(usym->unresolved, "init")) {
      for_actuals(actual, call) {
        if (NamedExpr* named = toNamedExpr(actual)) {
          if (!strcmp(named->name, "meme")) {
            if (SymExpr* sym = toSymExpr(named->actual)) {
              if (AggregateType* ct = toAggregateType(sym->var->type)) {
                if (ct->initializerStyle == DEFINES_NONE_USE_DEFAULT) {
                  // This code should be removed when the compiler generates
                  // initializers as the default method of construction and
                  // initialization for a type (Lydia note, 08/19/16)
                  usym->unresolved = astr("_construct_", ct->symbol->name);
                } else if (ct->initializerStyle == DEFINES_CONSTRUCTOR) {
                  // This code should be removed when initializers are fully
                  // supported and old style constructors are deprecated
                  // (Lydia note, 08/19/16)
                  USR_FATAL(call, "can't make init call on type with old constructor style");
                }
              }
            }
          }
        }
      }
    }
  }
}

static
Expr* getInitCall(FnSymbol* fn);

static
void phase1Analysis(BlockStmt* body, AggregateType* t, Expr* initCall);




void handleInitializerRules(FnSymbol* fn, AggregateType* t) {
  Expr* initCall = getInitCall(fn);

  if (initCall != NULL) {
    phase1Analysis(fn->body, t, initCall);

    // Insert analysis of initCall here
  }

  // Insert phase 2 analysis here
}

static
bool isInitCall (Expr* expr) {
  // The following set of nested if statements is looking for a CallExpr
  // representing this.init(args) or super.init(args), which at this point
  // in compilation will be represented by:
  // call (call ("." [ super | this ] "init") args)
  // If we match, return true.
  if (CallExpr* call = toCallExpr(expr)) {
    if (CallExpr* inner = toCallExpr(call->baseExpr)) {
      if (inner->isNamed(".")) {
        if (SymExpr* sym = toSymExpr(inner->get(2))) {
          if (VarSymbol* var = toVarSymbol(sym->var)) {
            if (var->immediate->const_kind == CONST_KIND_STRING) {
              if (!strcmp(var->immediate->v_string, "init")) {
                return true;
              }
            }
          }
        }
      }
    }
    // Behavior is not yet correct for super/this.init() calls within
    // loops or if statements.  TODO: fix this
  }
  return false;
}

// This function traverses the body of the initializer until it finds a
// super.init(...) or this.init(...) call, or it reaches the end of the
// initializer's body, whichever comes first.  It will return that call,
// or NULL if it didn't find a call matching that description.
static
Expr* getInitCall(FnSymbol* fn) {
  for (Expr* curExpr = fn->body->body.head; curExpr; curExpr = curExpr->next) {
    if (isInitCall(curExpr)) {
      return curExpr;
    }
  }
  return NULL;
}

static
void phase1Analysis(BlockStmt* body, AggregateType* t, Expr* initCall) {
  DefExpr* curField = toDefExpr(t->fields.head);
  // the super field is always first
  bool *seenField = (bool*)calloc(t->fields.length, sizeof(bool));
  if (curField)
    seenField[0] = true;

  Expr* curExpr = body->body.head;
  int curIndex = 0;
  while (curExpr != NULL && curExpr != initCall) {
    // Verify that:
    // - fields are initialized in declaration order
    // - The "this" instance is only used to clarify a field initialization (or
    //   to refer to a field which has already been initialized
    // - (no later fields are accessed by earlier ones?  Or will def/use
    //   analysis handle this?)
    // - no parent fields are accessed
    // - loops and parallel statements do not contain field initialization
    //   - no begin expressions?

    // Additionally, perform the following actions:
    // - add initialization for omitted fields

    if (CallExpr* call = toCallExpr(curExpr)) {
      if (call->isNamed("=")) {
        if (CallExpr* inner = toCallExpr(call->get(1))) {
          if (inner->isNamed(".")) {
            SymExpr* potenThis = toSymExpr(inner->get(1));
            if (potenThis && potenThis->var->hasFlag(FLAG_ARG_THIS)) {
              // It's an access of this!
              if (t->fields.length == 0) {
                // If there aren't fields, then this is obviously wrong.
                // I don't think we'll actually reach this branch, though.
                USR_FATAL_CONT(call, "attempted method call too early during initialization");
              }
              INT_ASSERT(curField);

              SymExpr* sym = toSymExpr(inner->get(2));
              INT_ASSERT(sym);
              // Could it be . . . a field?  The anticipation is killing me!
              if (VarSymbol* var = toVarSymbol(sym->var)) {
                if (var->immediate->const_kind == CONST_KIND_STRING) {
                  DefExpr* next = toDefExpr(curField->next);
                  bool foundMatch = false;
                  int nextIdx = curIndex + 1;
                  while (next) {
                    if (!strcmp(next->sym->name, var->immediate->v_string)) {
                      // The name matches a later field than the last we saw
                      // Moved forward

                      // NOTE: will want to insert initialization of fields we
                      // passed along the way if this skipped some.
                      curField = next;
                      curIndex = nextIdx;
                      seenField[nextIdx] = true;
                      foundMatch = true;
                      break;
                    } else {
                      next = toDefExpr(next->next);
                      nextIdx++;
                    }
                  }
                  if (!foundMatch) {
                    DefExpr* prev = curField;
                    nextIdx = curIndex;
                    while (prev) {
                      if (!strcmp(prev->sym->name, var->immediate->v_string)) {
                        if (seenField[nextIdx]) {
                          USR_FATAL_CONT(call, "multiple initializations of field \"%s\" detected", prev->sym->name);
                        } else {
                          USR_FATAL_CONT(call, "field initialization out of order");
                          USR_PRINT(call, "initialization of fields before .init() call must be in order");
                        }
                        foundMatch = true;
                        break;
                      } else {
                        prev = toDefExpr(prev->prev);
                        nextIdx--;
                      }
                    }
                    if (!foundMatch) {
                      USR_FATAL_CONT(call, "attempted method call too early during initialization");
                    }
                  }
                }
              }
            }
            // otherwise, we're accessing the field of something else.
            // Carry on.
          }
        }
      }
    }

    curExpr = curExpr->next;
  }

  free (seenField);
}
