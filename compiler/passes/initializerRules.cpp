/*
 * Copyright 2004-2017 Cray Inc.
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
#include "symbol.h"
#include "type.h"

#include <map>

enum InitBody {
  DID_NOT_FIND_INIT,
  FOUND_SUPER_INIT,
  FOUND_THIS_INIT,
  FOUND_BOTH
};


static InitBody getInitCall(FnSymbol* fn);
static void     phase1Analysis(BlockStmt* body, AggregateType* t);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void handleInitializerRules(FnSymbol* fn, AggregateType* ct) {
  if (fn->hasFlag(FLAG_NO_PARENS)) {
    USR_FATAL(fn, "an initializer cannot be declared without parentheses");
  } else {
    InitBody bodyStyle = getInitCall(fn);

    if (isClass(ct) == true) {
      buildClassAllocator(fn, ct);
      fn->addFlag(FLAG_INLINE);
    }

    if (bodyStyle != DID_NOT_FIND_INIT) {
      phase1Analysis(fn->body, ct);

      // Insert analysis of initCall here
      if (bodyStyle == FOUND_SUPER_INIT && isRecord(ct)) {
        // Need to find and remove any and all super.init() statements
        // if the type is a record, as they will not resolve (inheritance
        // and records is still being ironed out).

      }

    } else {
      // Adds default initialization of all fields and an argumentless
      // super.init() call at the beginning of the body for Phase-2-only
      // initializers.
      SET_LINENO(fn->body);

      // LYDIA NOTE (11/30/16): This would be a really good spot for a
      // re-entrant compiler call on what I'd like to create, which is
      // a much simpler CallExpr, instead of what I have to do today,
      // which is insert a fragile copy of the super.init() calls I
      // check for, entirely dependent on how such a user call gets
      // transformed by the preceding passes.
      CallExpr* superPortion = new CallExpr(".",
                                            new SymExpr(fn->_this),
                                            new_StringSymbol("super"));
      SymExpr*  initPortion  = new SymExpr(new_StringSymbol("init"));
      CallExpr* base         = new CallExpr(".", superPortion, initPortion);
      CallExpr* superCall    = new CallExpr(base);

      fn->body->insertAtHead(superCall);

      phase1Analysis(fn->body, ct);

      if (isRecord(ct)) {
        // We haven't finalized what inheritance means for records yet.
        // Until we do, this call (while necessary for the divide between
        // the phases), won't resolve.
        superCall->remove();
      }
    }

    // Insert phase 2 analysis here

    if (isClass(ct) == false) {
      fn->insertAtTail(new CallExpr(PRIM_RETURN, new SymExpr(fn->_this)));
    } else {
      Symbol* voidType = dtVoid->symbol;

      fn->retExprType = new BlockStmt(new SymExpr(voidType), BLOCK_SCOPELESS);
    }
  }
}

// Returns true only if what was provided was a SymExpr whose symbol is "this"
static
bool storesThisTop (Expr* expr) {
  if (SymExpr* stores = toSymExpr(expr)) {
    return stores->symbol()->hasFlag(FLAG_ARG_THIS);
  }
  return false;
}

static
bool storesSpecificName (Expr* expr, const char* name) {
  if (SymExpr* sym = toSymExpr(expr)) {
    if (VarSymbol* var = toVarSymbol(sym->symbol())) {
      if (var->immediate->const_kind == CONST_KIND_STRING) {
        if (!strcmp(var->immediate->v_string, name)) {
          return true;
        }
      }
    }
  }
  return false;
}

static
InitBody getInitCall (Expr* expr) {
  // The following set of nested if statements is looking for a CallExpr
  // representing this.init(args) or super.init(args), which at this point
  // in compilation will be represented by:
  // call (call ("." [ super | this ] "init") args)
  // If we match, return the InitBody enum which represents whether the call is
  // a super.init() or this.init() call
  if (CallExpr* call = toCallExpr(expr)) {
    if (CallExpr* inner = toCallExpr(call->baseExpr)) {
      if (inner->isNamed(".")) {
        if (storesSpecificName(inner->get(2), "init")) {
          if (CallExpr* subject = toCallExpr(inner->get(1))) {
            if (storesThisTop(subject->get(1))) {
              if (storesSpecificName(subject->get(2), "super")) {
                // The expr is a "(this.)super.init()" call
                return FOUND_SUPER_INIT;
              }
            }
          } else if (storesThisTop(inner->get(1))) {
            // The expr is a "this.init()" call
            return FOUND_THIS_INIT;
          }
        }
      }
    }
  }
  return DID_NOT_FIND_INIT;
}

static
Expr* getNextStmt(Expr* curExpr, BlockStmt* body, bool enterLoops);

// This function traverses the body of the initializer until it finds a
// super.init(...) or this.init(...) call, or it reaches the end of the
// initializer's body, whichever comes first.  It will return the enum
// description of that call, or DID_NOT_FIND_INIT if it didn't find a call
// matching that description.
static
InitBody getInitCall(FnSymbol* fn) {
  // Behavior is not yet correct for super/this.init() calls within
  // if statements.  TODO: fix this
  Expr* curExpr = fn->body->body.head;

  BlockStmt* block = toBlockStmt(curExpr);
  while (block && !block->isLoopStmt()) {
    curExpr = block->body.head;
    block = toBlockStmt(curExpr);
  }

  InitBody body = DID_NOT_FIND_INIT;
  while (curExpr != NULL) {
    body = getInitCall(curExpr);
    if (body != DID_NOT_FIND_INIT) {
      return body;
    }
    curExpr = getNextStmt(curExpr, fn->body, true);
  }
  return body;
}

// Finds the appropriate next statement to examine
static
Expr* getNextStmt(Expr* curExpr, BlockStmt* body, bool enterLoops) {
  Expr* toReturn = NULL;
  if (curExpr->next) {
    // If there is a next statement at this level, return it (in some form)
    toReturn = curExpr->next;
  } else if (curExpr->parentExpr != body) {
    // Otherwise, if we have dived into some nested scope, try going up a level
    // from that scope to find the next statement.  If there isn't one, keep
    // going up until there is a next statement or we reach "body".
    Expr* scope = curExpr->parentExpr;
    while (scope->next == NULL && scope->parentExpr != body) {
      scope = scope->parentExpr;
    }
    toReturn = scope->next;
  }
  if (toReturn != NULL) {
    while (BlockStmt* block = toBlockStmt(toReturn)) {
      if (block->isLoopStmt() && !enterLoops) {
        // We found a loop and we don't want to dive into loops.  Return it.
        return block;
      } else {
        // continue to dive in
        toReturn = block->body.head;
      }
    }
    // While loop is exited the first time we find an Expr that isn't a
    // BlockStmt.  We return from the loop if the Expr is a BlockStmt we don't
    // want to dive into.
  }
  return toReturn;
}

static
void errorCases(AggregateType* t, DefExpr* curField, const char* fieldname,
                bool seenField[], Expr* call);

static
const char* getFieldName(Expr* curExpr);

static
bool isLaterFieldAccess(DefExpr* curField, const char* fieldname);

static
bool loopAnalysis(BlockStmt* loop, DefExpr* curField, bool* seenField,
                       int* index, AggregateType* t);

static
void insertOmittedField(Expr* next, DefExpr* field, AggregateType* t);


static
bool isParentField(AggregateType* t, const char* name);

static
void phase1Analysis(BlockStmt* body, AggregateType* t) {
  DefExpr* curField = toDefExpr(t->fields.head);
  bool *seenField = (bool*)calloc(t->fields.length, sizeof(bool));
  if (curField) {
    if (isClass(t)) {
      INT_ASSERT(curField->sym->hasFlag(FLAG_SUPER_CLASS));
      // the super field is always first
      seenField[0] = true;
    }
  }
  int index = 0;

  Expr* curExpr = body->body.head;
  InitBody isInit = getInitCall(curExpr);
  if (isInit == DID_NOT_FIND_INIT) {
    // solution to fence post issue of diving into nested block statements
    BlockStmt* block = toBlockStmt(curExpr);
    while (block && !block->isLoopStmt()) {
      curExpr = block->body.head;
      block = toBlockStmt(curExpr);
    }
  }
  // We are guaranteed to never reach the end of the body, due to the
  // conditional surrounding the call to this function.
  while (curField != NULL || (isInit == DID_NOT_FIND_INIT)) {
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
    if (curField != NULL && (isInit == DID_NOT_FIND_INIT)) {
      // still have phase 1 statements and fields left to traverse

      if (BlockStmt* block = toBlockStmt(curExpr)) {
        if (block->isLoopStmt()) {
          // Special handling for loops.
          bool foundInit = loopAnalysis(block, curField, seenField, &index, t);
          if (foundInit) {
            // If the init call was in the loop body we just checked, any
            // further action is incorrect (including the insertion of leftover
            // omitted fields, since those fields would be inserted into the
            // loop body and we don't allow that), so just exit the loop through
            // the body and allow normal clean up to occur.
            break;
          }
          curExpr = getNextStmt(curExpr, body, false);
          isInit = getInitCall(curExpr);
          continue;
        }
      }/* else if (CondStmt* cond = toCondStmt(curExpr)) {
        // Special handling for conditionals.
        }*/


      if (const char* fieldname = getFieldName(curExpr)) {
        if (!strcmp(fieldname, curField->sym->name)) {
          // It's a match!  Advance both and move on
          curField = toDefExpr(curField->next);
          curExpr = getNextStmt(curExpr, body, false);
          isInit = getInitCall(curExpr);
          seenField[index] = true;
          index++;
        } else if (isLaterFieldAccess(curField, fieldname)) {
          insertOmittedField(curExpr, curField, t);
          index++;
          curField = toDefExpr(curField->next);
        } else {
          // It's not a valid field access at all.  Error cases!
          errorCases(t, curField, fieldname, seenField, curExpr);
          curExpr = getNextStmt(curExpr, body, false);
          isInit = getInitCall(curExpr);
        }
      } else {
        // Wasn't a field access, only update curExpr;
        curExpr = getNextStmt(curExpr, body, false);
        isInit = getInitCall(curExpr);
      }
    } else if (curField != NULL) {
      // only fields left

      insertOmittedField(curExpr, curField, t);
      curField = toDefExpr(curField->next);
      index++;
    } else {
      // only phase 1 statements left.

      if (BlockStmt* block = toBlockStmt(curExpr)) {
        if (block->isLoopStmt()) {
          // Special handling for loops.
          bool foundInit = loopAnalysis(block, curField, seenField, &index, t);
          if (foundInit) {
            // If the init call was in the loop body we just checked, any
            // further action is incorrect (including the insertion of leftover
            // omitted fields, since those fields would be inserted into the
            // loop body and we don't allow that), so just exit the loop through
            // the body and allow normal clean up to occur.
            break;
          }
        }
      } else if (const char* fieldname = getFieldName(curExpr)) {
        errorCases(t, curField, fieldname, seenField, curExpr);
      }
      curExpr = getNextStmt(curExpr, body, false);
      isInit = getInitCall(curExpr);
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
          if (potenThis && potenThis->symbol()->hasFlag(FLAG_ARG_THIS)) {
            // It's an access of this!
            SymExpr* sym = toSymExpr(inner->get(2));
            INT_ASSERT(sym);
            // Could it be . . . a field?  The anticipation is killing me!
            if (VarSymbol* var = toVarSymbol(sym->symbol())) {
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
    USR_FATAL_CONT(call, "can't set value of field \"%s\" from parent type during phase 1 of initialization", fieldname);
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
void insertOmittedField(Expr* next, DefExpr* field, AggregateType* t) {
  const char* nextField = field->sym->name;

  SET_LINENO(next);
  // Do something appropriate with "super"
  if (field->sym->hasFlag(FLAG_SUPER_CLASS)) {
    return;
  }

  // For all other fields, insert an assignment into that field with the given
  // initialization, if we have one.
  if (!field->init && !field->exprType) {
    USR_FATAL_CONT(next, "can't omit initialization of field \"%s\", no type or default value provided", nextField);
    return;
  }

  Symbol* _this = toFnSymbol(next->parentSymbol)->_this;
  CallExpr* thisAccess = new CallExpr(".", _this,
                                      new_CStringSymbol(nextField));
  CallExpr* newInit = NULL;
  if (field->init) {
    newInit = new CallExpr("=", thisAccess, field->init->copy());
  } else {
    INT_ASSERT(field->exprType);
    VarSymbol* tmp = newTemp("call_tmp");
    next->insertBefore(new DefExpr(tmp));
    next->insertBefore(new CallExpr(PRIM_MOVE, new SymExpr(tmp),
                                    new CallExpr(PRIM_INIT,
                                                 field->exprType->copy())));
    newInit = new CallExpr("=", thisAccess, new SymExpr(tmp));
  }
  next->insertBefore(newInit);
}

// Takes in the current expr (which must be a loop), the current field
// expected, the list of fields seen, the index into that list, the type whose
// fields are being iterated over (this is for error message purposes), and the
// expected init call for this function.  Walks the statements in the loop, and
// if any of them are field initializations, will generate an error message
// (as field initialization must be executed in order during Phase 1, and
// cannot be repeated).  If the initCall is encountered while performing this
// traversal, immediately cease performing this check and indicate that we
// encountered it by returning "true".
static
bool loopAnalysis(BlockStmt* loop, DefExpr* curField, bool* seenField,
                  int* index, AggregateType* t) {
  Expr* stmt = loop->body.head;
  InitBody isInit = getInitCall(stmt);
  while(stmt != NULL) {
    if (BlockStmt* inner = toBlockStmt(stmt)) {
      bool sawInit = loopAnalysis(inner, curField, seenField, index, t);
      if (sawInit) {
        return sawInit;
      }
    }
    if (isInit == FOUND_SUPER_INIT || isInit == FOUND_THIS_INIT) {
      // We encountered the .init() call while traversing this loop.  Stop
      // performing Phase 1 analysis and return.
      USR_FATAL_CONT(stmt, "use of %s.init() call in loop body", isInit == FOUND_SUPER_INIT ? "super" : "this");
      return true;
    }

    if (const char* fieldname = getFieldName(stmt)) {
      if (curField && !strcmp(fieldname, curField->sym->name)) {
        USR_FATAL_CONT(stmt, "can't initialize field \"%s\" inside a loop during phase 1 of initialization", fieldname);
        seenField[*index] = true;
        (*index)++;
        stmt = getNextStmt(stmt, loop, true);
        isInit = getInitCall(stmt);
      } else if (curField && isLaterFieldAccess(curField, fieldname)) {
        (*index)++;
        curField = toDefExpr(curField->next);
      } else {
        // Something else invalid has occurred
        // Should I also warn that it is occurring inside a loop?
        errorCases(t, curField, fieldname, seenField, stmt);
        stmt = getNextStmt(stmt, loop, true);
        isInit = getInitCall(stmt);
      }
    } else {
      stmt = getNextStmt(stmt, loop, true);
      isInit = getInitCall(stmt);
    }
  }
  // traverse the statements of the loop, diving into loops and blocks as
  // encountered

  return false;
}

/************************************* | **************************************
*                                                                             *
* Consider                                                                    *
*                                                                             *
*   var x = new MyClass(10, 20, 30);                                          *
*                                                                             *
* and assume MyClass defines an initializer that accepts 3 integers.          *
*                                                                             *
* The goal is to allocate an instance of MyClass on the heap and then invoke  *
* the appropriate init method on this instance.                               *
*                                                                             *
* This is implemented by defining an internal "type method" on MyClass that   *
* performs the allocation and then invokes the init method on the resulting   *
* instance.  Note that there is a distinct _new method for every init         *
* method.                                                                     *
*                                                                             *
************************************** | *************************************/

FnSymbol* buildClassAllocator(FnSymbol* initMethod, AggregateType* ct) {
  SET_LINENO(ct);

  FnSymbol*  fn          = new FnSymbol("_new");
  BlockStmt* body        = fn->body;
  ArgSymbol* type        = new ArgSymbol(INTENT_BLANK, "t", ct);
  VarSymbol* newInstance = newTemp("instance", ct);
  CallExpr*  allocCall   = callChplHereAlloc(ct);
  CallExpr*  initCall    = new CallExpr("init", gMethodToken, newInstance);

  type->addFlag(FLAG_TYPE_VARIABLE);

  fn->addFlag(FLAG_METHOD);
  fn->addFlag(FLAG_COMPILER_GENERATED);

  fn->retExprType = new BlockStmt(new SymExpr(ct->symbol), BLOCK_SCOPELESS);

  // Add the formal that provides the type for the type method
  fn->insertFormalAtTail(type);

  //
  // Walk the formals to the init method
  // Ignore _mt and _this
  //   1) add a corresponding formal to the new type method
  //   2) add that formal to the call to "init"
  //
  int count = 1;

  for_formals(formal, initMethod) {
    // Ignore _mt and this
    if (count >= 3) {
      ArgSymbol* arg = formal->copy();

      fn->insertFormalAtTail(arg);
      initCall->insertAtTail(new SymExpr(arg));
    }

    count = count + 1;
  }

  // Construct the body
  body->insertAtTail(new DefExpr(newInstance));

  body->insertAtTail(new CallExpr(PRIM_MOVE,   newInstance, allocCall));
  body->insertAtTail(new CallExpr(PRIM_SETCID, newInstance));

  body->insertAtTail(initCall);

  body->insertAtTail(new CallExpr(PRIM_RETURN, newInstance));


  // Insert the definition in to the tree
  ct->symbol->defPoint->insertBefore(new DefExpr(fn));

  return fn;
}
