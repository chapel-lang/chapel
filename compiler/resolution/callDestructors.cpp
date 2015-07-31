/*
 * Copyright 2004-2015 Cray Inc.
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

#define HILDE_MM 1

#include "passes.h"

#include "astutil.h"
#include "expr.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "stlUtil.h"
#include "stmt.h"
#include "symbol.h"

#include <set>

#ifndef HILDE_MM
// Clear autoDestroy flags on variables that get assigned to the return value of
// certain functions.
//
// FLAG_INSERT_AUTO_DESTROY is applied to some variables early in compilation,
// before the type of the variable is known (e.g. in the build and normalize
// passes), so it is easier to handle special cases after resolution has
// occurred.
static void cullAutoDestroyFlags()
{
  forv_Vec(FnSymbol, fn, gFnSymbols)
  {
    if (VarSymbol* ret = toVarSymbol(fn->getReturnSymbol()))
    {
      // The return value of an initCopy function should not be autodestroyed.
      // Normally, the return value of a function is autoCopied, but since
      // autoCopy is typically defined in terms of initCopy, this would lead to
      // infinite recursion.  That is, the return value of initCopy must be
      // handled specially.
      if (fn->hasFlag(FLAG_INIT_COPY_FN))
        ret->removeFlag(FLAG_INSERT_AUTO_DESTROY);

      // This is just a workaround for memory management being handled specially
      // for internally reference-counted types. (sandboxing)
      TypeSymbol* ts = ret->type->symbol;
      if (ts->hasFlag(FLAG_ARRAY) ||
          ts->hasFlag(FLAG_DOMAIN))
        ret->removeFlag(FLAG_INSERT_AUTO_DESTROY);
      }
      // Do we need to add other record-wrapped types here?  Testing will tell.

      // NOTE 1: When the value of a record field is established in a default
      // constructor, it is initialized using a MOVE.  That means that ownership
      // of that value is shared between the formal_tmp and the record field.
      // If the autodestroy flag is left on that formal temp, then it will be
      // destroyed which -- for ref-counted types -- can result in a dangling
      // reference.  So here, we look for that case and remove it.  
      if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR))
      {
        Map<Symbol*,Vec<SymExpr*>*> defMap;
        Map<Symbol*,Vec<SymExpr*>*> useMap;
        buildDefUseMaps(fn, defMap, useMap);

        std::vector<DefExpr*> defs;
        collectDefExprs(fn, defs);

        for_vector(DefExpr, def, defs)
        {
          if (VarSymbol* var = toVarSymbol(def->sym))
          {
            // Examine only those bearing the explicit autodestroy flag.
            if (! var->hasFlag(FLAG_INSERT_AUTO_DESTROY))
              continue;

            // Look for a use in a PRIM_SET_MEMBER where the field is a record
            // type, and remove the flag.
            // (We don't actually check that var is of record type, because
            // chpl__autoDestroy() does nothing when applied to all other types.
            for_uses(se, useMap, var)
            {
              CallExpr* call = toCallExpr(se->parentExpr);
              if (call->isPrimitive(PRIM_SET_MEMBER) &&
                  toSymExpr(call->get(3))->var == var)
                var->removeFlag(FLAG_INSERT_AUTO_DESTROY);
            }
          }
        }

        freeDefUseMaps(defMap, useMap);
      }
    }
  }
}


// Clear autodestroy flags on variables that get assigned to the return symbols
// of a function.
//
// Such a variable cannot be autodestroyed because its contents are owned by the
// caller.  This weirdness is caused by changeRetToArgAndClone() when it pulls
// the call utilizing a return value into the callee.
static void cullExplicitAutoDestroyFlags()
{
  forv_Vec(FnSymbol, fn, gFnSymbols)
  {
    if (! fn->hasFlag(FLAG_INIT_COPY_FN))
      continue;

    Map<Symbol*,Vec<SymExpr*>*> defMap;
    Map<Symbol*,Vec<SymExpr*>*> useMap;
    buildDefUseMaps(fn, defMap, useMap);

    std::vector<DefExpr*> defs;
    collectDefExprs(fn, defs);

    Symbol* retVar = fn->getReturnSymbol();

    for_vector(DefExpr, def, defs)
    {
      if (VarSymbol* var = toVarSymbol(def->sym))
      {
        // Examine only those bearing an autodestroy flag.
        if (! var->hasFlag(FLAG_INSERT_AUTO_DESTROY) &&
            ! var->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW))
          continue;

        // Look for the specific breaking case and amend that.
        for_uses(se, useMap, var)
        {
          CallExpr* call = toCallExpr(se->parentExpr);
          if (call->isPrimitive(PRIM_MOVE) &&
              toSymExpr(call->get(1))->var == retVar)
          {
            var->removeFlag(FLAG_INSERT_AUTO_DESTROY);
            var->removeFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW);
          }
        }
      }
    }

    freeDefUseMaps(defMap, useMap);
  }
}


/******************************** | *********************************
*                                                                   *
* A set of functions that scan every BlockStmt to determine whether *
* it is necessary to insert autoDestroy calls at any of the exit    *
* points rom the block.                                             *
*                                                                   *
* This computation consists of a linear scan of every BlockStmt     *
* scanning for                                                      *
*                                                                   *
*    1) Statements that define a variable that requires an          *
*       autoDestroy operation.                                      *
*                                                                   *
*    2) Statements that contain, recursively, a transfer of         *
*       control that exits the BlockStmt being scanned.             *
*                                                                   *
*    3) Statements "at the end" of the BlockStmt.                   *
*                                                                   *
*                                                                   *
********************************* | ********************************/

static bool       stmtDefinesAnAutoDestroyedVariable(Expr* stmt);
static VarSymbol* stmtTheDefinedVariable(Expr* stmt);

static void       updateJumpsFromBlockStmt(Expr*            stmt,
                                           BlockStmt*       block,
                                           Vec<VarSymbol*>& vars);
static bool       gotoExitsBlock(GotoStmt* gotoStmt, BlockStmt* block);

static bool       stmtMustExitBlock(Expr* stmt);
static bool       stmtIsLabelDefnBeforeReturn(Expr* stmt);
static bool       stmtIsLabelDefn(Expr* stmt);
static bool       stmtIsReturn(Expr* stmt);
static bool       stmtIsDownEndCount(Expr* stmt);

static void       updateBlockExit(Expr*            stmt,
                                  BlockStmt*       block,
                                  Vec<VarSymbol*>& vars);

static void insertAutoDestroyCalls() {
  forv_Vec(BlockStmt, block, gBlockStmts) {
    // Ignore BlockStmts for a Module
    if (isModuleSymbol(block->parentSymbol) == false) {

      Vec<VarSymbol*> vars;

      // A linear traversal of the statements in the body
      for_alist(stmt, block->body) {

        if (stmtDefinesAnAutoDestroyedVariable(stmt) == true) {
          vars.add(stmtTheDefinedVariable(stmt));
        }

        // It is appropriate to skip this analysis if there aren't
        // currently any variables that need autoDestroy calls
        if (vars.n > 0) {
          updateJumpsFromBlockStmt(stmt, block, vars);
        }

        if (stmtMustExitBlock(stmt) == true) {
          updateBlockExit(stmt, block, vars);
          break;
        }
      }
    }
  }
}

static bool stmtDefinesAnAutoDestroyedVariable(Expr* stmt) {
  bool retval = false;

  if (DefExpr* def = toDefExpr(stmt)) {
    if (VarSymbol* var = toVarSymbol(def->sym)) {
      if (
          // Flagged with "simple" AUTO_DESTROY
          var->hasFlag(FLAG_INSERT_AUTO_DESTROY) ||

          // Flagged with "complex" AUTO_DESTROY
          (var->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW) &&
           !var->type->symbol->hasFlag(FLAG_ITERATOR_RECORD)       &&
           !isRefCountedType(var->type))) {

        // There are variables that have been tagged with an AUTO_DESTROY
        // flag, presumably before the type was known, that should not in
        // fact be auto-destroyed.  Don't gum things up by collecting them.
        if (autoDestroyMap.get(var->type) != 0) {
          if (var->isType() == false) {
            retval = true;
          }
        }
      }
    }
  }

  return retval;
}

static VarSymbol* stmtTheDefinedVariable(Expr* stmt) {
  VarSymbol* retval = 0;

  if (DefExpr* def = toDefExpr(stmt)) {
    retval = toVarSymbol(def->sym);
  }

  return retval;
}

// Update any jumps *contained within* this stmt that escape the block
static void updateJumpsFromBlockStmt(Expr*            stmt,
                                     BlockStmt*       block,
                                     Vec<VarSymbol*>& vars) {
  if (isDefExpr(stmt)  == false &&
      isSymExpr(stmt)  == false &&
      isCallExpr(stmt) == false &&
      isGotoStmt(stmt) == false) {
    std::vector<GotoStmt*> gotoStmts;

    collectGotoStmts(stmt, gotoStmts);

    for_vector(GotoStmt, gotoStmt, gotoStmts) {
      if (gotoExitsBlock(gotoStmt, block)) {
        forv_Vec(VarSymbol, var, vars) {
          if (FnSymbol* autoDestroyFn = autoDestroyMap.get(var->type)) {
            SET_LINENO(var);
            
            gotoStmt->insertBefore(new CallExpr(autoDestroyFn, var));
          }
        }
      }
    }
  }
}

// The outer loop of this business logic is walking a given BlockStmt
// and is inspecting every goto-stmt that is recursively within this
// block.  

// This function is testing with a particular goto jumps to a point
// outside the block being tested.

static bool gotoExitsBlock(GotoStmt* gotoStmt, BlockStmt* block) {
  bool retval = false;

  // Every GOTO that implements a RETURN is sure to be exiting the
  // block.  This test is necessary to handle an edge case in the more
  // general logic below; a return from the outer-most BlockStmt
  // for a procedure.  It also provides a small performance gain.
  if (gotoStmt->gotoTag == GOTO_RETURN) {
    retval = true;

  // This test is more general than the first check and handles
  // break, continue, yield and most, but not all, uses of return.
  //
  // The code finds the definition-point for the label that the
  // goto has targetted and then works up the parent chain until
  // it hits NULL or the block under consideration.
  //
  // If it traverses to NULL then it is certain the target label
  // is outside the block being scanned.  However it will not
  // be NULL, currently, if the goto is implementing a return
  // for a statement at the top-level of the procedure.

  } else {
    SymExpr* labelSymExpr = toSymExpr(gotoStmt->label);
    Expr*    expr         = labelSymExpr->var->defPoint;

    while (expr != 0 && expr != block) {
      expr = expr->parentExpr;
    }

    retval = (expr == 0) ? true : false;
  }

  return retval;
}

static bool stmtMustExitBlock(Expr* stmt) {
  Expr* next   = stmt->next;
  bool  retval = false;

  if (next == 0                         ||
      isGotoStmt(next)                  ||
      stmtIsLabelDefnBeforeReturn(next) ||
      stmtIsReturn(next)                ||
      stmtIsDownEndCount(next))
    retval = true;

  return retval;
}

static bool stmtIsLabelDefnBeforeReturn(Expr* stmt) {
  return stmtIsLabelDefn(stmt) && stmtIsReturn(stmt->next);
}

static bool stmtIsLabelDefn(Expr* stmt) {
  bool retval = false;

  if (DefExpr* defn = toDefExpr(stmt)) {
    retval = isLabelSymbol(defn->sym);
  }

  return retval;
}

static bool stmtIsReturn(Expr* stmt) {
  bool retval = false;

  if (CallExpr* call = toCallExpr(stmt)) {
    retval = call->isPrimitive(PRIM_RETURN);
  }

  return retval;
}

static bool stmtIsDownEndCount(Expr* stmt) {
  bool retval = false;

  if (CallExpr* call = toCallExpr(stmt)) {
    if (call->isResolved()) {
      retval = (strcmp(call->isResolved()->name, "_downEndCount") == 0);
    }
  }

  return retval;
}

static void updateBlockExit(Expr*            stmt,
                            BlockStmt*       block,
                            Vec<VarSymbol*>& vars) {
  forv_Vec(VarSymbol, var, vars) {
    if (FnSymbol* autoDestroyFn = autoDestroyMap.get(var->type)) {
      SET_LINENO(var);
      stmt->insertAfter(new CallExpr(autoDestroyFn, var));
    }
  }
}


//
// Cache to avoid cloning functions that return records if the copy
// of the returned argument is done in the same way as at another
// call site; the key into the cache is the old function, the values
// are stored in a vector based on the copy function (copy function
// 1, new function 1, copy function 2, new function 2, ...)
//
static Map<FnSymbol*,Vec<FnSymbol*>*> retToArgCache;

// Helper method for changeRetToArgAndClone, assisting in symbol replacement
//
// This method takes in the current call which we are replacing around
// (focalPt), the VarSymbol we are trying to replace (oldSym), the symbol we are
// replacing it with (newSym), and the function that was called in the first
// use of oldSym in the callee, to replace oldSym with newSym without breaking
// the AST.
inline static void
replacementHelper(CallExpr* focalPt, VarSymbol* oldSym, Symbol* newSym,
                  FnSymbol* useFn) {
  focalPt->insertAfter(new CallExpr(PRIM_MOVE, newSym,
                                    new CallExpr(useFn, oldSym)));
}


// Clone fn, add a ref arg to the end of the argument list, remove the return
// primitive and change the return type of the function to void.  
// In the body of the clone, replace updates to the return value variable with
// calls to the useFn in the calling context.
//
// This effectively replaces return-by-value from the given function into
// return-by-reference through the new argument.  It allows the result to be
// written directly into sapce allocated in the caller, thus avoiding a
// verbatim copy.
// 
static FnSymbol*
createClonedFnWithRetArg(FnSymbol* fn, FnSymbol* useFn)
{
  SET_LINENO(fn);
  FnSymbol* newFn = fn->copy();
  ArgSymbol* arg = new ArgSymbol(blankIntentForType(useFn->retType->refType), "_retArg", useFn->retType->refType);
  newFn->insertFormalAtTail(arg);
  VarSymbol* ret = toVarSymbol(newFn->getReturnSymbol());
  INT_ASSERT(ret);
  Expr* returnPrim = newFn->body->body.tail;
  returnPrim->replace(new CallExpr(PRIM_RETURN, gVoid));
  newFn->retType = dtVoid;
  fn->defPoint->insertBefore(new DefExpr(newFn));

  std::vector<SymExpr*> symExprs;
  collectSymExprs(newFn, symExprs);

  // In the body of the function, replace references to the original
  // ret symbol with copies of the return value reference.  A local
  // deref temp is inserted if needed.  The result is fed through a
  // call to the useFn -- effectively sucking the use function call
  // inside the clone function.
  for_vector(SymExpr, se, symExprs) {
    if (se->var == ret) {
      CallExpr* move = toCallExpr(se->parentExpr);
      if (move && move->isPrimitive(PRIM_MOVE) && move->get(1) == se) {
        SET_LINENO(move);
        replacementHelper(move, ret, arg, useFn);
      } else {
        // Any other call or primitive.
        FnSymbol* calledFn = move->isResolved();
        CallExpr* parent = toCallExpr(move->parentExpr);
        if (calledFn && !strcmp(calledFn->name, "=") &&
            // Filter out case handled above.
            (!parent || !parent->isPrimitive(PRIM_MOVE))) {
          replacementHelper(move, ret, arg, useFn);
        } else {
          Symbol* tmp = newTemp("ret_to_arg_derefTmp", useFn->retType);
          se->getStmtExpr()->insertBefore(new DefExpr(tmp));
          se->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_DEREF, arg)));
          se->var = tmp;
        }
      }
    }
  }
  return newFn;
}


static void replaceRemainingUses(Vec<SymExpr*>& use, SymExpr* firstUse,
                                 Symbol* actual)
{
  // for each remaining use "se"
  //   replace se with deref of the actual return value argument, unless parent is
  //   accessing its address
  forv_Vec(SymExpr, se, use) {
    // Because we've already handled the first use
    if (se != firstUse) {
      CallExpr* parent = toCallExpr(se->parentExpr);
      if (parent) {
        SET_LINENO(parent);
        if (parent->isPrimitive(PRIM_ADDR_OF)) {
          parent->replace(new SymExpr(actual));
        } else {
          FnSymbol* parentFn = parent->isResolved();
          if (!(parentFn->hasFlag(FLAG_AUTO_COPY_FN) ||
                parentFn->hasFlag(FLAG_INIT_COPY_FN))) {
            // Leave the auto copies/inits in, we'll need them for
            // moving information back to us.

            // Copy the information we currently have into the temp
            se->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, se->var, new CallExpr(PRIM_DEREF, actual)));
          }
        }
      }
    }
  }
}


// Create a copy of the called function, replacing
// the return statement in that function with a copy of the call which uses
// the result of the above call to that function.  Maybe a picture would
// help.
//   ('move' lhs (fn args ...))
//   . . .
//   ('move useLhs (useFn lhs))
// gets converted to
//   (newFn args ... useLhs)
//   . . .
//   <removed>
// where a call to useFn replaces the return that used to be at the end of
// newFn.  The use function is expected to be assignment, initCopy or
// autoCopy.  All other cases are ignored.
static void replaceUsesOfFnResultInCaller(CallExpr* move, CallExpr* call,
                                          Vec<SymExpr*>& use, FnSymbol* fn)
{
  SymExpr* firstUse = use.v[0];
  // If this isn't a call expression, we've got problems.
  if (CallExpr* useCall = toCallExpr(firstUse->parentExpr)) {
    if (FnSymbol* useFn = useCall->isResolved()) {
      if ((!strcmp(useFn->name, "=") && firstUse == useCall->get(2)) ||
          useFn->hasFlag(FLAG_AUTO_COPY_FN) ||
          useFn->hasFlag(FLAG_INIT_COPY_FN)) {
        Symbol* actual;
        FnSymbol* newFn = NULL;

        //
        // check cache for new function
        //
        if (Vec<FnSymbol*>* vfn = retToArgCache.get(fn)) {
          for (int i = 0; i < vfn->n; i++) {
            if (vfn->v[i] == useFn) {
              newFn = vfn->v[i+1];
            }
          }
        }

        if (!newFn) {
          newFn = createClonedFnWithRetArg(fn, useFn);

          //
          // add new function to cache
          //
          Vec<FnSymbol*>* vfn = retToArgCache.get(fn);
          if (!vfn)
            vfn = new Vec<FnSymbol*>();
          vfn->add(useFn);
          vfn->add(newFn);
          retToArgCache.put(fn, vfn);
        }

        SET_LINENO(call);
        call->baseExpr->replace(new SymExpr(newFn));

        CallExpr* useMove = toCallExpr(useCall->parentExpr);
        if (useMove)
        {
          INT_ASSERT(useMove->isPrimitive(PRIM_MOVE));

          Symbol* useLhs = toSymExpr(useMove->get(1))->var;
          if (!useLhs->type->symbol->hasFlag(FLAG_REF))
          {
            useLhs = newTemp("ret_to_arg_ref_tmp_", useFn->retType->refType);
            move->insertBefore(new DefExpr(useLhs));
            move->insertBefore(new CallExpr(PRIM_MOVE, useLhs, new CallExpr(PRIM_ADDR_OF, useMove->get(1)->remove())));
          }

          move->replace(call->remove());
          useMove->remove();
          call->insertAtTail(useLhs);

          actual = useLhs;
        }
        else
        {
          // We assume the useFn is an assignment.
          if (strcmp(useFn->name, "="))
          {
            INT_FATAL(useFn, "should be an assignment function");
            return;
          }

          // We expect that the used symbol is the second actual passed to
          // the "=".  That is, it is an assignment from the result of the
          // call to fn to useLhs.
          INT_ASSERT(firstUse == useCall->get(2));

          Symbol* useLhs = toSymExpr(useCall->get(1))->var;
          move->replace(call->remove());
          call->insertAtTail(useLhs);

          actual = useLhs;
        }
        if (actual) {
          replaceRemainingUses(use, firstUse, actual);
        }
      }
    }
  }
}


static void
changeRetToArgAndClone(CallExpr* move, Symbol* lhs,
                       CallExpr* call, FnSymbol* fn,
                       Map<Symbol*,Vec<SymExpr*>*>& defMap,
                       Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  // Here are some relations between the arguments that can be relied upon.
  INT_ASSERT(call->parentExpr == move);
  INT_ASSERT(call->isResolved() == fn);

  // In the suffix of the containing function, look for uses of the lhs of the
  // move containing the call to fn.
  Vec<SymExpr*> use;
  if (useMap.get(lhs) && useMap.get(lhs)->n == 1) {
    use = *useMap.get(lhs);
  } else {
    for (Expr* stmt = move->next; stmt; stmt = stmt->next) {
      std::vector<SymExpr*> symExprs;
      collectSymExprs(stmt, symExprs);
      for_vector(SymExpr, se, symExprs) {
        if (se->var == lhs) {
          use.add(se);
        }
      }
    }
  }

  // If such a use is found, create a copy of the called function, replacing
  // the return statement in that function with a copy of the call which uses
  // the result of the above call to that function.
  if (use.n > 0) {
    replaceUsesOfFnResultInCaller(move, call, use, fn);
  }
}


static void
returnRecordsByReferenceArguments() {
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap);

  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol) {
      if (FnSymbol* fn = requiresImplicitDestroy(call)) {
        if (fn->hasFlag(FLAG_EXTERN))
          continue;
        CallExpr* move = toCallExpr(call->parentExpr);
        INT_ASSERT(move->isPrimitive(PRIM_MOVE));
        SymExpr* lhs = toSymExpr(move->get(1));
        INT_ASSERT(!lhs->var->hasFlag(FLAG_TYPE_VARIABLE));
        changeRetToArgAndClone(move, lhs->var, call, fn, defMap, useMap);
      }
    }
  }
  freeDefUseMaps(defMap, useMap);
}
#endif

static void
fixupDestructors() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_DESTRUCTOR) &&
        // TODO: Right now, we flesh out the body of a freeIterator function
        // "by hand".  But if we can reuse this code, that would be much better.
        !fn->hasFlag(FLAG_AUTO_II)) {
      AggregateType* ct = toAggregateType(fn->_this->getValType());
      INT_ASSERT(ct);

      //
      // insert calls to destructors for all 'value' fields
      //
      for_fields_backward(field, ct) {
        SET_LINENO(field);
        if (field->type->destructor) {
          AggregateType* fct = toAggregateType(field->type);
          INT_ASSERT(fct);
          if (!isClass(fct)) {
            bool useRefType = !isRefCountedType(fct);
            VarSymbol* tmp = newTemp("_field_destructor_tmp_", useRefType ? fct->refType : fct);
            fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
            fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp,
              new CallExpr(useRefType ? PRIM_GET_MEMBER : PRIM_GET_MEMBER_VALUE, fn->_this, field)));
            FnSymbol* autoDestroyFn = autoDestroyMap.get(field->type);
            if (autoDestroyFn && autoDestroyFn->hasFlag(FLAG_REMOVABLE_AUTO_DESTROY))
              fn->insertBeforeReturnAfterLabel(new CallExpr(autoDestroyFn, tmp));
            else
              fn->insertBeforeReturnAfterLabel(new CallExpr(field->type->destructor, tmp));
          }
        } else if (FnSymbol* autoDestroyFn = autoDestroyMap.get(field->type)) {
          VarSymbol* tmp = newTemp("_field_destructor_tmp_", field->type);
          fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
          fn->insertBeforeReturnAfterLabel(
                new CallExpr(PRIM_MOVE, tmp,
                  new CallExpr(PRIM_GET_MEMBER_VALUE, fn->_this, field)));
          fn->insertBeforeReturnAfterLabel(new CallExpr(autoDestroyFn, tmp));
        }
      }

      //
      // insert call to parent destructor
      //
      INT_ASSERT(ct->dispatchParents.n <= 1);
      if (ct->dispatchParents.n >= 1 && isClass(ct)) {
        // avoid destroying record fields more than once
        if (FnSymbol* parentDestructor = ct->dispatchParents.v[0]->destructor) {
          SET_LINENO(fn);
          Type* tmpType = isClass(ct) ?
            ct->dispatchParents.v[0] : ct->dispatchParents.v[0]->refType;
          VarSymbol* tmp = newTemp("_parent_destructor_tmp_", tmpType);
          fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
          fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp,
            new CallExpr(PRIM_CAST, tmpType->symbol, fn->_this)));
          fn->insertBeforeReturnAfterLabel(new CallExpr(parentDestructor, tmp));
        }
      }
    }
  }
}


static void addAutoDestroyCallsForModule(ModuleSymbol* mod, FnSymbol* fn,
                                         std::set<ModuleSymbol*>& visited)
{
// Termination
  if (visited.count(mod) > 0)
    return;
  visited.insert(mod);

// Recursion
  // Visit my parent.
  if (ModuleSymbol* parent = mod->defPoint->getModule())
    if (parent != theProgram && parent != rootModule)
      addAutoDestroyCallsForModule(parent, fn, visited);

  // Visit my explicit dependencies.
  forv_Vec(ModuleSymbol, usedMod, mod->modUseList)
    addAutoDestroyCallsForModule(usedMod, fn, visited);

// Real work
  for_alist(expr, mod->block->body)
  {
    if (DefExpr* def = toDefExpr(expr))
      if (VarSymbol* var = toVarSymbol(def->sym))
      {
        if (var->hasFlag(FLAG_NO_AUTO_DESTROY))
          continue;

        // Don't destroy type variables (they have no run-time representation).
        if (var->hasFlag(FLAG_TYPE_VARIABLE))
          continue;

        if (FnSymbol* autoDestroy = autoDestroyMap.get(var->type))
        {
          // Skip destructors for class types (only nude RWT types at this point).
          if (AggregateType* at = toAggregateType(var->type))
            if (isClass(at))
              continue;

          SET_LINENO(var);
          fn->insertAtHead(new CallExpr(autoDestroy, var));
        }
      }
  }
}


static void insertGlobalAutoDestroyCalls() {
  // --ipe does not build chpl_gen_main
  if (chpl_gen_main == NULL)
    return;

  SET_LINENO(baseModule);

  const char* name = "chpl__autoDestroyGlobals";
  FnSymbol* fn = new FnSymbol(name);

  // TODO: Would like to use unordered_set (C++11) instead, when it is available.
  std::set<ModuleSymbol*> visited;
  addAutoDestroyCallsForModule(mainModule, fn, visited);

  fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  fn->retType = dtVoid;

  chpl_gen_main->defPoint->insertBefore(new DefExpr(fn));
  chpl_gen_main->insertBeforeReturnAfterLabel(new CallExpr(fn));
}


static void insertDestructorCalls()
{
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_CALL_DESTRUCTOR)) {
      Type* type = call->get(1)->typeInfo();
      if (!type->destructor) {
        call->remove();
      } else {
        SET_LINENO(call);
        call->replace(new CallExpr(type->destructor, call->get(1)->remove()));
      }
    }
  }
}

#ifndef HILDE_MM
static void insertAutoCopyTemps()
{
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap);

  forv_Vec(VarSymbol, sym, gVarSymbols) {
    if (sym->hasFlag(FLAG_INSERT_AUTO_COPY)) {
      CallExpr* move = NULL;
      for_defs(def, defMap, sym) {
        CallExpr* defCall = toCallExpr(def->parentExpr);
        if (defCall->isPrimitive(PRIM_MOVE)) {
          CallExpr* rhs = toCallExpr(defCall->get(2));
          if (!rhs || !rhs->isNamed("=")) {
            INT_ASSERT(!move);
            move = defCall;
          }
        }
      }

      // 2015/01/21 hilde: Workaround for incomplete implementation of
      // SymExpr::remove() in the context of a ForLoop (as its mIndex field).
      // This operation is required by the early operation of
      // deadBlockElimination().

      // In that case, the DefExpr for the symbol should no longer exist, so we
      // would never reach here.  Given that it is never defined and we *do*
      // reach here, there is no harm in not creating the autoCopy temp.  This
      // code will probably all be deprecated when the new AMM story comes
      // online anyway, so it would be a waste of time trying to "do things
      // right" in this routine.
      if (! move)
        continue;

      INT_ASSERT(move);
      SET_LINENO(move);
      Symbol* tmp = newTemp("_autoCopy_tmp_", sym->type);
      move->insertBefore(new DefExpr(tmp));
      move->insertAfter(new CallExpr(PRIM_MOVE, sym, new CallExpr(autoCopyMap.get(sym->type), tmp)));
      move->get(1)->replace(new SymExpr(tmp));
    }
  }

  freeDefUseMaps(defMap, useMap);
}
#endif

// This routine inserts autoCopy calls ahead of yield statements as necessary,
// so the calling routine "owns" the returned value.
// The copy is necessary for yielded values of record type returned by value.
// In the current implementation, types marked as "iterator record" and
// "runtime type value" are excluded.
static void insertYieldTemps()
{
  // Examine all calls.
  forv_Vec(CallExpr, call, gCallExprs)
  {
    // Select only yield primitives.
    if (! call->isPrimitive(PRIM_YIELD))
      continue;

    // Filter out calls that are not in the tree.
    if (! call->parentSymbol)
      continue;

    // This is the symbol passed back in the yield.
    SymExpr* yieldExpr = toSymExpr(call->get(1));

    // The transformation is applied only if is has a normal record type
    // (passed by value).
    Type* type = yieldExpr->var->type;
    if (isRecord(type) &&
        !type->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
        !type->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE))
    {
      SET_LINENO(call);

      // Replace:
      //   yield <yieldExpr>
      // with:
      //   (def _yield_expr_tmp_:type)
      //   (move _yield_expr_tmp_ ("chpl__autoCopy" <yieldExpr>))
      //   yield _yield_expr_tmp_
      Symbol* tmp = newTemp("_yield_expr_tmp_", type);
      Expr* stmt = call->getStmtExpr();
      stmt->insertBefore(new DefExpr(tmp));
      stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(autoCopyMap.get(type), yieldExpr->remove())));
      call->insertAtHead(new SymExpr(tmp)); // New first argument.
    }
  }
}


//
// Insert reference temps for function arguments that expect them.
//
void insertReferenceTemps(CallExpr* call)
{
  for_formals_actuals(formal, actual, call) {
    if (formal->type == actual->typeInfo()->refType) {
      SET_LINENO(call);
      Expr* stmt = call->getStmtExpr();
      VarSymbol* tmp = newTemp("_ref_tmp_", formal->type);
      tmp->addFlag(FLAG_REF_TEMP);
      stmt->insertBefore(new DefExpr(tmp));
      actual->replace(new SymExpr(tmp));
      stmt->insertBefore(
        new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_ADDR_OF, actual)));
    }
  }
}


static bool isLeaderFollowerIteratorCall(CallExpr* call)
{
  // These tests are copied verbatim from the tests that select the calls
  // of interest in cleanupLeaderFollowerIteratorCalls().
  if (FnSymbol* fn = call->isResolved()) {
    if (fn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD) ||
        (isDefExpr(fn->formals.tail) &&
         !strcmp(toDefExpr(fn->formals.tail)->sym->name, "_retArg") &&
         toDefExpr(fn->formals.tail)->sym->getValType() &&
         toDefExpr(fn->formals.tail)->sym->getValType()->symbol->hasFlag(FLAG_ITERATOR_RECORD))) {
      if (!strcmp(call->parentSymbol->name, "_toLeader") ||
          !strcmp(call->parentSymbol->name, "_toFollower") ||
          !strcmp(call->parentSymbol->name, "_toFastFollower") ||
          !strcmp(call->parentSymbol->name, "_toStandalone")) {
        return true;
      }
    }
  }
  return false;
}


void insertReferenceTemps() {
  forv_Vec(CallExpr, call, gCallExprs)
  {
    // Skip calls that are not in the tree.
    if (! call->parentSymbol)
      continue;

    // Do not insert reference temps on _toLeader and _toFollower calls before
    // iterator lowering is complete.
    // A certain structure for these calls is expected in
    // cleanupLeaderFollowerIteratorCalls() and inserting deref temps disturbs
    // that form.
    // TODO: The design for LeaderFollower 2.0 should avoid these nonconforming
    // modifications of the AST.
    if (! iteratorsLowered)
      if (isLeaderFollowerIteratorCall(call))
        continue;

    if (call->isResolved() ||
        call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL))
    {
      insertReferenceTemps(call);
    }
  }
}


//
// Insert dereference temps as needed to make reference arguments and variables
// match formals that expect their arguments by value
// (This should be rare, except for fundamental types, small records and
//  task arguments passed using remote value forwarding.)
//

static inline void
insertDerefTemp(Expr* expr)
{
  Expr* stmt = expr->getStmtExpr();
  SET_LINENO(stmt);

  Type* t = expr->typeInfo();
  INT_ASSERT(t->symbol->hasFlag(FLAG_REF));

  VarSymbol* tmp = newTemp("derefTmp", t->getValType());
  stmt->insertBefore(new DefExpr(tmp));

  expr->replace(new SymExpr(tmp));
  stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp,
                                  new CallExpr(PRIM_DEREF, expr)));
}

void insertDerefTemps(CallExpr* call)
{
  if ((call->isResolved()) ||
      call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL))
  {
    // This is a function call (not a primitive).
    for_formals_actuals(formal, actual, call)
    {
      if (actual->typeInfo() == formal->type->refType)
      {
        // A deref temp is needed.
        insertDerefTemp(actual);
      }
    }
  }

  if (call->primitive)
  {
    // A primitive.
    // Do nothing in the general case.
    // In specific cases, insert a deref temp if required.
    switch (call->primitive->tag)
    {
     default:
      // Default: Do not insert a deref temp.
      break;

     case PRIM_ADDR_OF:
      {
        Expr* actual = call->get(1);
        if (isReferenceType(actual->typeInfo()))
        {
          // Can't take the address of a reference, so just remove this call.
          call->replace(actual->remove());
        }
      }
      break;

     case PRIM_MOVE:
      {
        Expr* actual = call->get(2);
        // If the RHS of the move is an addr-of call, we skip it.
        //  - It will be removed by the PRIM_ADDR_OF clause if its operand is
        //    already a reference, and
        //  - Calling typeInfo on an 'addr of' primitive whose argument is
        //    already of ref cause a compiler error.  (Chapel only supports one
        //    level of references.)
        if (CallExpr* aoc = toCallExpr(actual))
          if (aoc->isPrimitive(PRIM_ADDR_OF))
            break;

        Expr* lhs = call->get(1);
        if (actual->typeInfo() == lhs->typeInfo()->refType)
          insertDerefTemp(actual);
      }
      break;

     case PRIM_RETURN:
      {
        // The type of the argument to the return primitive should match the
        // return type of the function that contains it.
        Expr* actual = call->get(1);
        FnSymbol* fn = toFnSymbol(call->parentSymbol);
        INT_ASSERT(fn);
        if (actual->typeInfo() == fn->retType->refType)
          insertDerefTemp(actual);
      }
      break;

     case PRIM_SET_MEMBER:
      {
        Expr* actual = call->get(3);
        Expr* field = call->get(2);
        Type* target = field->typeInfo();
        if (actual->typeInfo() == target->refType)
          insertDerefTemp(actual);
      }
      break;
    }
  }

  // String literals are represented as DefExpr(CallExpr('_construct_string',
  // string_literal)), so they are niether primitives nor resolved calls.
}


void insertDerefTemps(FnSymbol* fn)
{
  std::vector<CallExpr*> callExprs;
  collectCallExprs(fn, callExprs);
  for_vector(CallExpr, call, callExprs)
    insertDerefTemps(call);
}


void insertDerefTemps() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (! call->parentSymbol)
      // Not in tree, so skip
      continue;

    insertDerefTemps(call);
  }
}


// If the given symbol is a temporary that is defined uniqely as the LHS of a
// deref primitive, then return the SymExpr that is the operand of the deref;
// otherwise, return NULL.
static SymExpr* rhsOfDerefTmp(Symbol* sym,
                              Map<Symbol*,Vec<SymExpr*>*>& defMap)
{
  // Is this symbol a temporary?
  if (! sym->hasFlag(FLAG_TEMP))
    // Nope. Fail outright.
    return NULL;

  // Find the definitions of the given symbol
  if (Vec<SymExpr*>* defs = defMap.get(sym))
  {
    // Ensure that it is uniquely defined.
    if (defs->n != 1)
      return NULL;

    SymExpr* def = defs->only();
    if (CallExpr* move = toCallExpr(def->parentExpr))
      if (move->isPrimitive(PRIM_MOVE))
      {
        // If this is a simple transfer, punch through and see if we get a
        // deref somewhere back up the def-use chain.
        if (SymExpr* rhs = toSymExpr(move->get(2)))
          return rhsOfDerefTmp(rhs->var, defMap);

        if (CallExpr* deref = toCallExpr(move->get(2)))
          if (deref->isPrimitive(PRIM_DEREF))
          {
            SymExpr* ref = toSymExpr(deref->get(1));
            // We expect the RHS of a deref primitive to be a SymExpr.
            INT_ASSERT(ref);
            return ref;
          }
        // fall through
      }
    // fall through
  }
  return NULL;
}


static void replaceValArgsWithRefArgs(CallExpr* call,
                                      Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                      Map<Symbol*,Vec<SymExpr*>*>& useMap)
{
  for_formals_actuals(/*ArgSymbol*/ formal, /*Expr*/ actual, call)
  {
    // Check: Is the formal of ref type?
    // If not, then there is nothing to do here.
    if (! formal->type->symbol->hasFlag(FLAG_REF))
      continue;

    // Check: Is the actual of value type?
    // If not, there is nothing to do here.
    SymExpr* actualSE = toSymExpr(actual);
    // We assume that this actual argument is a SymExpr.
    INT_ASSERT(actualSE);
    Symbol* actualSym = actualSE->var;
    if (actualSym->type->symbol->hasFlag(FLAG_REF))
      continue;

    // If the argument is a deref temp, use the rhs of the deref primitive to
    // get an actual argument of ref type.
    if (SymExpr* refActual = rhsOfDerefTmp(actualSym, defMap))
    {
      // Replace the deref temp argument of the autoCopy call
      // with the reference arg that created the deref temp.
      SET_LINENO(actualSE);
      actualSE->replace(new SymExpr(refActual->var));

      // If possible, remove the deref temp creation and the
      // deref temp itself.
      Vec<SymExpr*>* uses = useMap.get(refActual->var);
      if (uses->n == 1 && uses->only() == actual)
      {
        // This is the MOVE that defines actualSym.
        Expr* move = refActual->parentExpr->parentExpr;
        move->remove();
        actualSym->defPoint->remove();
      }
    }
  }
}


void replaceValArgsWithRefArgs(FnSymbol* fn)
{
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(fn, defMap, useMap);

  std::vector<CallExpr*> callExprs;
  collectCallExprs(fn, callExprs);
  for_vector(CallExpr, call, callExprs)
  {
    // Ignore calls that are not in the tree.
    if (! call->parentSymbol)
      continue;

    // Weed out primitives
    // This probably catches virtual method calls, so we may need a more
    // precise test later.
    if (! call->isResolved())
      continue;

    replaceValArgsWithRefArgs(call, defMap, useMap);
  }

  freeDefUseMaps(defMap, useMap);
}

                              
void replaceValArgsWithRefArgs()
{
  forv_Vec(FnSymbol, fn, gFnSymbols)
  {
    replaceValArgsWithRefArgs(fn);
  }
}

void
callDestructors() {
  fixupDestructors();
  insertDestructorCalls();
#ifndef HILDE_MM
  insertAutoCopyTemps();
  cullAutoDestroyFlags();
  cullExplicitAutoDestroyFlags();
  insertAutoDestroyCalls();
  returnRecordsByReferenceArguments();
#endif
  insertYieldTemps();
  insertGlobalAutoDestroyCalls();
  insertReferenceTemps();
}
