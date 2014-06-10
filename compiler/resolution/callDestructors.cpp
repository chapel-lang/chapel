#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "stmt.h"
#include "symbol.h"
#include "stlUtil.h"



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
          ts->hasFlag(FLAG_DOMAIN) ||
          ts->hasFlag(FLAG_SYNC))
        ret->removeFlag(FLAG_INSERT_AUTO_DESTROY);
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
        collectDefExprsSTL(fn, defs);

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
    collectDefExprsSTL(fn, defs);

    Symbol* retVar = fn->getReturnSymbol();

    for_vector(DefExpr, def, defs)
    {
      if (VarSymbol* var = toVarSymbol(def->sym))
      {
        // Examine only those bearing the explicit autodestroy flag.
        if (! var->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW))
          continue;

        // Look for the specific breaking case and amend that.
        for_uses(se, useMap, var)
        {
          CallExpr* call = toCallExpr(se->parentExpr);
          if (call->isPrimitive(PRIM_MOVE) &&
              toSymExpr(call->get(1))->var == retVar)
            var->removeFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW);
        }
      }
    }

    freeDefUseMaps(defMap, useMap);
  }
}


static bool
doesExitBlock(BlockStmt* block, GotoStmt* gotoStmt) {
  SymExpr* labelSymExpr = toSymExpr(gotoStmt->label);
  INT_ASSERT(labelSymExpr);
  Expr* expr = labelSymExpr->var->defPoint;
  while (expr) {
    if (expr == block)
      return false;
    expr = expr->parentExpr;
  }
  return true;
}


//
// This is a helper function that determines, when we're creating a
// new function call to 'fn' with actual 'arg', whether the argument
// needs to be replaced by a reference to the argument.  If it does,
// it creates a new reference temp and returns it; otherwise it
// returns the original arg.  
//
// It also returns, via the 'refTmpAssign' argument, an assignment
// that defines the ref tmp in question.  If this is NULL, it's an
// indication that no ref temp was used; if it's non-NULL, the caller
// must insert the assignment returned through this argument, as well
// as a DefExpr for the ref tmp itself, before the call occurs.
//
static VarSymbol*
createRefArgIfNeeded(FnSymbol* fn, VarSymbol* arg, CallExpr** refTmpAssign) {
  DefExpr* formalDef = toDefExpr(fn->formals.only());
  INT_ASSERT(formalDef);
  Type* formalType = formalDef->sym->type;
  VarSymbol* refTmp = NULL;
  //
  // Does the formal expect a ref, yet the actual is not one?
  //
  if (formalType->symbol->hasFlag(FLAG_REF) && 
      formalType->getValType() == arg->type) {
    SET_LINENO(arg);
    refTmp = newTemp("_ref_tmp_", formalType);
    *refTmpAssign = new CallExpr(PRIM_MOVE, refTmp, 
                                 new CallExpr(PRIM_ADDR_OF, arg));
    return refTmp;
  } else {
    *refTmpAssign = NULL;
    return arg;
  }
}


static void
insertAutoDestroyCalls() {
  forv_Vec(BlockStmt, block, gBlockStmts) {
    // Module blocks are not of interest, so skip them.
    if (isModuleSymbol(block->parentSymbol))
      continue;

    Vec<VarSymbol*> vars;
    for_alist(stmt, block->body) {
      //
      // find variables that should be destroyed before exiting block
      //
      if (DefExpr* def = toDefExpr(stmt))
        if (VarSymbol* var = toVarSymbol(def->sym))
          if (var->hasFlag(FLAG_INSERT_AUTO_DESTROY) ||
              (var->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW) &&
               !var->type->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
               !isRefCountedType(var->type)))
            // TODO: I think this can be changed into an assert:
            // All type variables removed from the AST by this stage.
            if (!var->hasFlag(FLAG_TYPE_VARIABLE))
              vars.add(var);

      //
      // find 'may' exit points and insert autoDestroy calls
      //   a 'may' exit point is a point where control may exit the
      //   block such as a 'break' or 'continue' out of a loop, or a
      //   'return' in a conditional
      // do not clear vector of variables
      //
      if (!isDefExpr(stmt) &&
          !isSymExpr(stmt) &&
          !isCallExpr(stmt) &&
          !isGotoStmt(stmt)) {
        Vec<GotoStmt*> gotoStmts;
        collectGotoStmts(stmt, gotoStmts);
        forv_Vec(GotoStmt, gotoStmt, gotoStmts) {
          if (doesExitBlock(block, gotoStmt)) {
            forv_Vec(VarSymbol, var, vars) {
              if (FnSymbol* autoDestroyFn = autoDestroyMap.get(var->type)) {
                SET_LINENO(var);
                gotoStmt->insertBefore(new CallExpr(autoDestroyFn, var));
              }
            }
          }
        }
      }

      //
      // find 'must' exit points and insert autoDestroy calls
      // clear vector of variables
      // A 'must' exit point is typically the last statement in a block
      // (i.e. the stmt has no successor).  We also consider the block ended if
      // the next statement is a goto, a return or a call to _downEndCount.
      //
      CallExpr* call = toCallExpr(stmt->next);
      if (!stmt->next ||
          isGotoStmt(stmt->next) ||
          (call && (call->isPrimitive(PRIM_RETURN) || // If the return statement
                    (call->isResolved() &&
                     !strcmp(call->isResolved()->name, "_downEndCount"))))) {
        forv_Vec(VarSymbol, var, vars) {
          if (FnSymbol* autoDestroyFn = autoDestroyMap.get(var->type)) {
            SET_LINENO(var);
            CallExpr* refTmpAssign = NULL;
            var = createRefArgIfNeeded(autoDestroyFn, var, &refTmpAssign);
            stmt->insertAfter(new CallExpr(autoDestroyFn, var));
            if (refTmpAssign) {
              stmt->insertAfter(refTmpAssign);
              stmt->insertAfter(new DefExpr(var));
            }
          }
        }
        vars.clear();
        break;
      }
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
static void
replacementHelper(CallExpr* focalPt, VarSymbol* oldSym, Symbol* newSym,
                  FnSymbol* useFn) {
  CallExpr* refTmpAssign = NULL;
  oldSym = createRefArgIfNeeded(useFn, oldSym, &refTmpAssign);
  focalPt->insertAfter(new CallExpr(PRIM_MOVE, newSym,
                                    new CallExpr(useFn, oldSym)));
  if (refTmpAssign) {
    focalPt->insertAfter(refTmpAssign);
    focalPt->insertAfter(new DefExpr(oldSym));
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

  // In the suffix of the containing function, look for a use of the lhs of the
  // move containing the call to fn.
  SymExpr* use = NULL;
  if (useMap.get(lhs) && useMap.get(lhs)->n == 1) {
    use = useMap.get(lhs)->v[0];
  } else {
    for (Expr* stmt = move->next; stmt && !use; stmt = stmt->next) {
      if (!isCallExpr(stmt) && !isDefExpr(stmt) && !isSymExpr(stmt))
        break;
      Vec<SymExpr*> symExprs;
      collectSymExprs(stmt, symExprs);
      forv_Vec(SymExpr, se, symExprs) {
        if (se->var == lhs) {
          use = se;
          break;
        }
      }
    }
  }

  // If such a use is found, create a copy of the called function, replacing
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
  if (use) {
    if (CallExpr* useCall = toCallExpr(use->parentExpr)) {
      if (FnSymbol* useFn = useCall->isResolved()) {
        if ((!strcmp(useFn->name, "=") && use == useCall->get(2)) ||
            useFn->hasFlag(FLAG_AUTO_COPY_FN) ||
            useFn->hasFlag(FLAG_INIT_COPY_FN)) {

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
            SET_LINENO(fn);
            newFn = fn->copy();
            ArgSymbol* arg = new ArgSymbol(blankIntentForType(useFn->retType->refType), "_retArg", useFn->retType->refType);
            newFn->insertFormalAtTail(arg);
            VarSymbol* ret = toVarSymbol(newFn->getReturnSymbol());
            INT_ASSERT(ret);
            Expr* returnPrim = newFn->body->body.tail;
            returnPrim->replace(new CallExpr(PRIM_RETURN, gVoid));
            newFn->retType = dtVoid;
            fn->defPoint->insertBefore(new DefExpr(newFn));

            Vec<SymExpr*> symExprs;
            collectSymExprs(newFn, symExprs);

            // In the body of the function, replace references to the original
            // ret symbol with copies of the return value reference.  A local
            // deref temp is inserted if needed.  The result is fed through a
            // call to the useFn -- effectively sucking the use function call
            // inside the clone function.
            forv_Vec(SymExpr, se, symExprs) {
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
                    Symbol* tmp = newTemp("ret_to_arg_tmp_", useFn->retType);
                    se->getStmtExpr()->insertBefore(new DefExpr(tmp));
                    se->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_DEREF, arg)));
                    se->var = tmp;
                  }
                }
              }
            }

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
            if (!useLhs->type->symbol->hasFlag(FLAG_REF)) {
              // I think this code is not needed because insertReferenceTemps()
              // will add it.
              useLhs = newTemp("ret_to_arg_ref_tmp_", useFn->retType->refType);
              move->insertBefore(new DefExpr(useLhs));
              move->insertBefore(new CallExpr(PRIM_MOVE, useLhs, new CallExpr(PRIM_ADDR_OF, useMove->get(1)->remove())));
            }
            // lhs->defPoint->remove();
            move->replace(call->remove());
            useMove->remove();
            call->insertAtTail(useLhs);

            return;
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
            INT_ASSERT(use == useCall->get(2));

            Symbol* useLhs = toSymExpr(useCall->get(1))->var;
            move->replace(call->remove());
            call->insertAtTail(useLhs);

            return;
          }
        }
      }
    }
    // When this was USR_WARN, release/examples/hello.chpl generates 10 warnings
    // if this was uncommented.  Really, it should be an internal error, though.
    //INT_FATAL(move, "possible premature free");
  } else {
    if (useMap.get(lhs) && useMap.get(lhs)->n > 0) {
      // When this was USR_WARN, release/examples/hello.chpl generates 10
      // warnings if this was uncommented.  Really, it should be an internal
      // error, though.
      //INT_FATAL(move, "possible premature free (use not found)");
    }
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


static void
fixupDestructors() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_DESTRUCTOR)) {
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
          if (!isClass(fct) || isSyncType(fct)) {
            bool useRefType = !isRefCountedType(fct) && !isSyncType(fct);
            VarSymbol* tmp = newTemp("_field_destructor_tmp_", useRefType ? fct->refType : fct);
            fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
            fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp,
              new CallExpr(useRefType ? PRIM_GET_MEMBER : PRIM_GET_MEMBER_VALUE, fn->_this, field)));
            FnSymbol* autoDestroyFn = autoDestroyMap.get(field->type);
            if (autoDestroyFn && autoDestroyFn->hasFlag(FLAG_REMOVABLE_AUTO_DESTROY))
              fn->insertBeforeReturnAfterLabel(new CallExpr(autoDestroyFn, tmp));
            else
              fn->insertBeforeReturnAfterLabel(new CallExpr(field->type->destructor, tmp)); 

            // WORKAROUND:
            // This is a temporary bug fix that results in leaked memory
            //  for sync and single vars in user defined records.
            //
            // We can only free a sync or single field that is part of a
            //  reference counted type because they are currently
            //  implemented as classes and thus can be copied as references.
            //  i.e.,
            //    isArrayClass(ct) || isDomainClass(ct) || isDistClass(ct)
            // We'll also will allow syncs that are declared in the standard
            //  and internal modules to be freed, assuming this will be okay.
            //  Since the reference counted types are declared within the
            //  standard and internal modules, we don't need to to the above
            //  check unless the assumptions proves incorrect.
            //
            // The problem is related to that of records with classes 
            //  described in insertFormalTemps() in functionResolution.cpp.
            //  Specifically, we do not call constructors for records,
            //  so if a record has sync or single fields, the memory
            //  for the fields is not allocated and the pointer to the
            //  field is copied in the autocopy.  The corresponding
            //  autodestroys may delete the data twice.
            //
            if (isSyncType(fct) &&
                ((ct->getModule()->modTag==MOD_INTERNAL) ||
                 (ct->getModule()->modTag==MOD_STANDARD)))
              fn->insertBeforeReturnAfterLabel(callChplHereFree(tmp));
          }
        } else if (FnSymbol* autoDestroyFn = autoDestroyMap.get(field->type)) {
          VarSymbol* tmp = newTemp("_field_destructor_tmp_", field->type);
          fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
          fn->insertBeforeReturnAfterLabel(
                new CallExpr(PRIM_MOVE, tmp,
                  new CallExpr(PRIM_GET_MEMBER_VALUE, fn->_this, field)));
          fn->insertBeforeReturnAfterLabel(new CallExpr(autoDestroyFn, tmp));
        } else if (field->type == dtString && !ct->symbol->hasFlag(FLAG_TUPLE)) {
// Temporary expedient: Leak strings like crazy.
//          VarSymbol* tmp = newTemp("_field_destructor_tmp_", dtString);
//          fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
//          fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp,
//            new CallExpr(PRIM_GET_MEMBER_VALUE, fn->_this, field)));
//          fn->insertBeforeReturnAfterLabel(callChplHereFree(tmp));
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


static void insertGlobalAutoDestroyCalls() {
  const char* name = "chpl__autoDestroyGlobals";
  SET_LINENO(baseModule);
  FnSymbol* fn = new FnSymbol(name);
  fn->retType = dtVoid;
  if( ! fNoInternalModules ) {
    chpl_gen_main->defPoint->insertBefore(new DefExpr(fn));
    chpl_gen_main->insertBeforeReturnAfterLabel(new CallExpr(fn));
  }
  forv_Vec(DefExpr, def, gDefExprs) {
    if (isModuleSymbol(def->parentSymbol))
      if (def->parentSymbol != rootModule)
        if (VarSymbol* var = toVarSymbol(def->sym))
          if (!var->isParameter() && !var->hasFlag(FLAG_TYPE_VARIABLE))
            if (!var->hasFlag(FLAG_NO_AUTO_DESTROY))
              if (FnSymbol* autoDestroy = autoDestroyMap.get(var->type)) {
                SET_LINENO(var);
                CallExpr* refTmpAssign = NULL;
                var = createRefArgIfNeeded(autoDestroy, var, &refTmpAssign);
                if (refTmpAssign) {
                  fn->insertAtTail(new DefExpr(var));
                  fn->insertAtTail(refTmpAssign);
                }
                fn->insertAtTail(new CallExpr(autoDestroy, var));
              }
  }
  fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
}


static void insertDestructorCalls()
{
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_CALL_DESTRUCTOR)) {
      Type* type = call->get(1)->typeInfo();
      if (!type->destructor) {
        call->remove();
      } else if (call->get(1)->typeInfo()->refType == type->destructor->_this->type) {
        SET_LINENO(call);
        VarSymbol* tmp = newTemp("_destructor_tmp_", type->destructor->_this->type);
        call->insertBefore(new DefExpr(tmp));
        call->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_ADDR_OF, call->get(1)->remove())));
        call->replace(new CallExpr(type->destructor, tmp));
      } else {
        SET_LINENO(call);
        call->replace(new CallExpr(type->destructor, call->get(1)->remove()));
      }
    }
  }
}


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


static void insertReferenceTemps() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if ((call->parentSymbol && call->isResolved()) ||
        call->isPrimitive(PRIM_VMT_CALL)) {
      //
      // Insert reference temps for function arguments that expect them.
      //
      for_formals_actuals(formal, actual, call) {
        if (formal->type == actual->typeInfo()->refType) {
          SET_LINENO(call);
          VarSymbol* tmp = newTemp("_ref_tmp_", formal->type);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          actual->replace(new SymExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_ADDR_OF, actual)));
        }
      }
    }
  }
}


void
callDestructors() {
  fixupDestructors();
  insertDestructorCalls();
  insertAutoCopyTemps();
  cullAutoDestroyFlags();
  cullExplicitAutoDestroyFlags();
  insertAutoDestroyCalls();
  returnRecordsByReferenceArguments();
  insertYieldTemps();
  insertGlobalAutoDestroyCalls();
  insertReferenceTemps();
}
