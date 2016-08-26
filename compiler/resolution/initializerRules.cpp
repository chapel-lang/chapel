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
void reorganizeBody(FnSymbol* fn, BlockStmt* phase1, BlockStmt* phase2,
                    BlockStmt* otherInit);
static
void phase1Analysis(AggregateType* t, BlockStmt* phase1, BlockStmt* phase2,
                    BlockStmt* otherInit);




void handleInitializerRules(FnSymbol* fn, AggregateType* t) {
  BlockStmt* phase1 = fn->body;
  BlockStmt* phase2 = new BlockStmt();
  BlockStmt* otherInit = new BlockStmt();

  reorganizeBody(fn, phase1, phase2, otherInit);

  phase1->insertAtTail(otherInit);
  phase1->insertAtTail(phase2);

  phase1Analysis(t, phase1, phase2, otherInit);
}

// This function traverses the body of the initializer backwards, moving the
// statements it finds into the phase2 block statement until it encounters the
// super/this.init() call or the start of the body, whichever comes first.
// It then moves the super/this.init() call and the statements it relies on
// into the otherInit block statement.
static
void reorganizeBody(FnSymbol* fn, BlockStmt* phase1, BlockStmt* phase2,
                    BlockStmt* otherInit) {
  while (phase1->body.length > 0) {
    // Note - to make the default for an initializer body be phase 1,
    // reverse the traversal order and perform some swaps of which block
    // statement is receiving the code.  Will also need to update the creation
    // of the phase1 block at the callsite, likely.

    if (CallExpr* call = toCallExpr(phase1->body.tail)) {
      if (CallExpr* inner = toCallExpr(call->baseExpr)) {
        if (inner->isNamed(".")) {
          if (SymExpr* sym = toSymExpr(inner->get(2))) {
            if (VarSymbol* var = toVarSymbol(sym->var)) {
              if (var->immediate->const_kind == CONST_KIND_STRING) {
                if (!strcmp(var->immediate->v_string, "init")) {
                  // While going backwards, we found the super/this.init() call
                  // Time to stop moving into the phase2 block statement.
                  otherInit->insertAtHead(call->remove());
                  break;
                }
              }
            }
          }
        }
      }
    }
    // Behavior is not yet correct for super/this.init() calls within
    // loops or if statements.  TODO: fix this

    phase2->insertAtHead(phase1->body.tail->remove());
  }
}

static
void phase1Analysis(AggregateType* t, BlockStmt* phase1, BlockStmt* phase2,
                    BlockStmt* otherInit) {
  DefExpr* curField = toDefExpr(t->fields.head);
  // the super field is always first
  INT_ASSERT(curField);

  Expr* curExpr = phase1->body.head;
  while (curExpr != NULL && curExpr != phase2 && curExpr != otherInit) {
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
              SymExpr* sym = toSymExpr(inner->get(2));
              INT_ASSERT(sym);
              // Could it be . . . a field?  The anticipation is killing me!
              if (VarSymbol* var = toVarSymbol(sym->var)) {
                if (var->immediate->const_kind == CONST_KIND_STRING) {
                  DefExpr* next = toDefExpr(curField->next);
                  bool foundMatch = false;
                  while (next) {
                    if (!strcmp(next->sym->name, var->immediate->v_string)) {
                      // The name matches a later field than the last we saw
                      // Moved forward

                      // NOTE: will want to insert initialization of fields we
                      // passed along the way if this skipped some.
                      curField = next;
                      foundMatch = true;
                      break;
                    } else {
                      next = toDefExpr(next->next);
                    }
                  }
                  if (!foundMatch) {
                    DefExpr* prev = curField;
                    while (prev) {
                      if (!strcmp(prev->sym->name, var->immediate->v_string)) {
                        USR_FATAL_CONT(call, "field initialization out of order");
                        USR_PRINT(call, "initialization of fields before .init() call must be in order");
                        foundMatch = true;
                        break;
                      } else {
                        prev = toDefExpr(prev->prev);
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
}
