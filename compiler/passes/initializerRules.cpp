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
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"

#include <map>

// Helper file for verifying the rules placed on initializers, and providing
// the extra functionality associated with them.

// Map storing the default initialization for fields of a type.  Will be used
// when traversing initializer bodies if a field is omitted
static std::map<AggregateType*, std::map<const char*, std::pair<Expr*, Expr*> >* > defaultInits;

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
  // Insert else branch which adds default initialization of all fields and an
  // argumentless super.init() call at the beginning of the body.

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
void errorCases(AggregateType* t, DefExpr* curField, const char* fieldname,
                bool seenField[], Expr* call);

static
const char* getFieldName(Expr* curExpr);

static
bool isLaterFieldAccess(DefExpr* curField, const char* fieldname);

static
void insertOmittedField(Expr* next, const char* nextField,
                        std::map<const char*, std::pair<Expr*, Expr*> >* inits,
                        AggregateType* t);


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
  int index = 0;

  std::map<const char*, std::pair<Expr*, Expr*> >* inits = defaultInits[t];

  Expr* curExpr = body->body.head;
  // We are guaranteed to never reach the end of the body, due to the
  // conditional surrounding the call to this function.
  while (curField != NULL || curExpr != initCall) {
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
    if (curField != NULL && curExpr != initCall) {
      // still have phase 1 statements and fields left to traverse

      if (const char* fieldname = getFieldName(curExpr)) {
        if (!strcmp(fieldname, curField->sym->name)) {
          // It's a match!  Advance both and move on
          curField = toDefExpr(curField->next);
          curExpr = curExpr->next;
          seenField[index] = true;
          index++;
        } else if (isLaterFieldAccess(curField, fieldname)) {
          insertOmittedField(curExpr, curField->sym->name, inits, t);
          index++;
          curField = toDefExpr(curField->next);
        } else {
          // It's not a valid field access at all.  Error cases!
          errorCases(t, curField, fieldname, seenField, curExpr);
          curExpr = curExpr->next;
        }
      } else {
        // Wasn't a field access, only update curExpr;
        curExpr = curExpr->next;
      }
    } else if (curField != NULL) {
      // only fields left

      insertOmittedField(curExpr, curField->sym->name, inits, t);
      curField = toDefExpr(curField->next);
      index++;
    } else {
      // only phase 1 statements left.

      if (const char* fieldname = getFieldName(curExpr)) {
        errorCases(t, curField, fieldname, seenField, curExpr);
      }
      curExpr = curExpr->next;
    }
  }

  free (seenField);
}

// Checks if the current expression is a call expression that sets the value
// of a field, and if so returns that field name.
static
const char* getFieldName(Expr* curExpr) {
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
                return var->immediate->v_string;
              }
            }
          }
        }
      }
    }
  }
  return NULL;
}

static
bool isLaterFieldAccess(DefExpr* curField, const char* fieldname) {
  DefExpr* next = curField;
  while (next) {
    if (!strcmp(next->sym->name, fieldname)) {
      return true;
    }
    next = toDefExpr(next->next);
  }
  return false;
}

// If the fieldname given is the name of a field after the last seen field in
// the class type t, then everything is fine.  Otherwise, give an appropriate
// error message depending on if the field is being initialized out of order,
// if the field has already been initialized, or if what is being initialized
// is not a field at all.
static
void errorCases(AggregateType* t, DefExpr* curField, const char* fieldname,
                bool seenField[], Expr* call) {
  int index = 0;
  for (DefExpr* fieldDef = toDefExpr(t->fields.head); fieldDef != curField;
       fieldDef = toDefExpr(fieldDef->next), index++) {
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
      return; // exit early due to error case.
    }
  }
  if (isParentField(t, fieldname)) {
    USR_FATAL_CONT(call, "can't set value of field \"%s\" from parent type during phase 1", fieldname);
  } else {
    // We didn't find the field match, even on our parent type.  It is a method.
    USR_FATAL_CONT(call, "attempted method call too early during initialization");
  }
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

static
void replaceArgsWithFields(AggregateType* t, Expr* context, Symbol* _this);

static
void insertOmittedField(Expr* next, const char* nextField,
                        std::map<const char*,
                        std::pair<Expr*, Expr*> >* inits,
                        AggregateType* t) {
  SET_LINENO(next);
  // Do something appropriate with "super"
  if (!strcmp(nextField, "super")) {
    return;
  }
  // For all other fields, insert an assignment into that field with the given
  // initialization, if we have one.
  if (inits->find(nextField) == inits->end()) {
    USR_FATAL_CONT(next, "can't omit initialization of field \"%s\", no type or default value provided", nextField);
    return;
  }

  std::pair<Expr*, Expr*> initExpr = (*inits)[nextField];

  // If that field has no initialization or type, we can't insert the omitted
  // initialization for it, it must be explicitly initialized.
  if (initExpr.first == NULL && initExpr.second == NULL) {
    USR_FATAL_CONT(next, "can't omit initialization of field \"%s\", no type or default value provided", nextField);
    return;
  }
  Symbol* _this = toFnSymbol(next->parentSymbol)->_this;
  CallExpr* thisAccess = new CallExpr(".", _this,
                                      new_CStringSymbol(nextField));
  CallExpr* newInit = NULL;
  if (initExpr.first != NULL &&
      !(isSymExpr(initExpr.first) &&
        toSymExpr(initExpr.first)->var == gTypeDefaultToken)) {
    Expr* init = initExpr.first->copy();

    // Since the init expression we're storing utilizes the arguments
    // corresponding to the fields instead of the fields themselves, we must
    // replace those references to the ArgSymbols with references to the fields
    replaceArgsWithFields(t, init, _this);

    newInit = new CallExpr("=", thisAccess, init);
  } else {
    BlockStmt* secondBody = toBlockStmt(initExpr.second);
    INT_ASSERT(secondBody);
    Expr* typeStart = secondBody->body.head;
    if (SymExpr* simpleCase = toSymExpr(typeStart)) {
      if (TypeSymbol* sym = toTypeSymbol(simpleCase->var)) {
        newInit = new CallExpr(PRIM_SET_MEMBER,
                               toFnSymbol(next->parentSymbol)->_this,
                               new_CStringSymbol(nextField),
                               new CallExpr(PRIM_INIT, new SymExpr(sym)));
      } else {
        INT_FATAL("Expected TypeSymbol to be stored, got other symbol");
      }
    } else {
      VarSymbol* tmp = newTemp("call_tmp");
      next->insertBefore(new DefExpr(tmp));
      next->insertBefore(new CallExpr(PRIM_MOVE, new SymExpr(tmp),
                                      new CallExpr(PRIM_INIT,
                                                   typeStart->copy())));
      newInit = new CallExpr("=", thisAccess, new SymExpr(tmp));
    }
  }
  next->insertBefore(newInit);
}

// Should only be called over expressions that were inserted into the ArgSymbol
// information for the default constructor on t.  Because the default
// constructor for t has only the fields of t and "meme" as its arguments, if
// we find an ArgSymbol mentioned in context, it should be a field (as "meme"
// is inserted by the compiler and not utilized for the default expression of
// anything we would encounter).
static
void replaceArgsWithFields(AggregateType* t, Expr* context, Symbol* _this) {
  std::vector<SymExpr*> syms;
  collectSymExprs(context, syms);
  for_vector(SymExpr, se, syms) {
    if (ArgSymbol* arg = toArgSymbol(se->var)) {
      for_fields(field, t) {
        if (!strcmp(field->name, arg->name)) {
          CallExpr* fieldAccess = new CallExpr(".", _this, new_CStringSymbol(field->name));
          se->replace(fieldAccess);
          break;
        }
      }
    }
  }
}

// Takes in the type, the name of the field, and the expr used to initialize it.
// This will be stored in a Map of type to a Map of fields to init expressions,
// which will be used to provide omitted initialization of fields when
// traversing the body of a user-defined initializer.
void storeFieldInit(AggregateType* t, const char* fieldname, Expr* init,
                    Expr* type) {
  // Lydia NOTE 09/13/16: This code depends on our generation of default
  // constructors.  It should not be used as a justification to keep the old
  // implementation around, but should instead be regarded as a step that should
  // eventually become unnecessary.
  if (defaultInits[t] != NULL) {
    defaultInits[t]->insert(std::pair<const char*, std::pair<Expr*, Expr*> >(fieldname, std::pair<Expr*, Expr*>(init, type)));
  } else {
    defaultInits[t] = new std::map<const char*, std::pair<Expr*, Expr*> >;
    defaultInits[t]->insert(std::pair<const char*, std::pair<Expr*, Expr*> >(fieldname, std::pair<Expr*, Expr*>(init, type)));
  }
}
