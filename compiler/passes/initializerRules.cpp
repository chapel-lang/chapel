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
DefExpr* handleField(AggregateType* t, DefExpr* curField, const char* fieldname,
                     bool seenField[], CallExpr* call);

static
bool isParentField(AggregateType* t, const char* name);

static
void phase1Analysis(BlockStmt* body, AggregateType* t, Expr* initCall) {
  DefExpr* curField = toDefExpr(t->fields.head);
  bool *seenField = (bool*)calloc(t->fields.length, sizeof(bool));
  if (curField) {
    INT_ASSERT(curField->sym->hasFlag(FLAG_SUPER_CLASS));
    // the super field is always first
    seenField[0] = true;
  }

  Expr* curExpr = body->body.head;
  // We are guaranteed to never reach the end of the body, due to the
  // conditional surrounding the call to this function.
  while (curExpr != initCall) {
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
                  curField = handleField(t, curField, var->immediate->v_string,
                                         seenField, call);
                }
              }
            }
            // otherwise, we're accessing the field or method of something else.
            // Carry on.
          }
        }
      }
    }

    curExpr = curExpr->next;
  }

  free (seenField);
}

// If the fieldname given is the name of a field after the last seen field in
// the class type t, then everything is fine.  Otherwise, give an appropriate
// error message depending on if the field is being initialized out of order,
// if the field has already been initialized, or if what is being initialized
// is not a field at all.
static
DefExpr* handleField(AggregateType* t, DefExpr* lastSeen, const char* fieldname,
                     bool seenField[], CallExpr* call) {
  bool passedLastSeen = false;
  int index = 0;
  for (DefExpr* fieldDef = toDefExpr(t->fields.head); fieldDef;
       fieldDef = toDefExpr(fieldDef->next), index++) {
    if (!passedLastSeen) {
      if (!strcmp(fieldDef->sym->name, fieldname)) {
        // We found a field match before the field we most recently saw
        // initialization for.
        if (seenField[index]) {
          // There was a previous initialization of this same field
          USR_FATAL_CONT(call, "multiple initializations of field \"%s\"", fieldname);
        } else {
          USR_FATAL_CONT(call, "field initialization out of order");
          USR_PRINT(call, "initialization of fields before .init() call must be in field declaration order");
        }
        return lastSeen; // exit early due to error case.
      } else if (!strcmp(fieldDef->sym->name, lastSeen->sym->name)) {
        // We didn't find a field matching this name yet, but we did just run
        // into the last field we saw initialized.  If we find a match after
        // this point, it fulfills the phase 1 order of initialization rules.
        passedLastSeen = true;
      }
    } else {
      if (!strcmp(fieldDef->sym->name, fieldname)) {
        seenField[index] = true;
        return fieldDef;
      }
    }
  }
  if (isParentField(t, fieldname)) {
    USR_FATAL_CONT(call, "can't set value of parent fields in phase 1");
  } else {
    // We didn't find the field match, even on our parent type.  It is a method.
    USR_FATAL_CONT(call, "attempted method call too early during initialization");
  }
  return lastSeen;
}

static
bool isParentField(AggregateType* t, const char *name) {
  if (t->dispatchParents.n > 0) {
    forv_Vec(Type, pt, t->dispatchParents) {
      if (AggregateType* cpt = toAggregateType(pt)) {
        for_fields(field, cpt) {
          if (!strcmp(field->name, name)) {
            return true;
          }
        }
        if (isParentField(cpt, name)) {
          return true;
        }
      }
    }
    return false;
  } else {
    return false;
  }
}
