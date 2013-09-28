#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "stmt.h"
#include "symbol.h"
#include "view.h"


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


static void
insertAutoDestroyTemps() {
  forv_Vec(BlockStmt, block, gBlockStmts) {
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
      //
      CallExpr* call = toCallExpr(stmt->next);
      if ((call && call->isPrimitive(PRIM_RETURN)) ||
          (!stmt->next) ||
          (isGotoStmt(stmt->next))) {
        forv_Vec(VarSymbol, var, vars) {
          if (FnSymbol* autoDestroyFn = autoDestroyMap.get(var->type)) {
            SET_LINENO(var);
            stmt->insertAfter(new CallExpr(autoDestroyFn, var));
          }
        }
        vars.clear();
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

static void
changeRetToArgAndClone(CallExpr* move, Symbol* lhs,
                       CallExpr* call, FnSymbol* fn,
                       Map<Symbol*,Vec<SymExpr*>*>& defMap,
                       Map<Symbol*,Vec<SymExpr*>*>& useMap) {
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

  if (use) {
    if (CallExpr* useCall = toCallExpr(use->parentExpr)) {
      if (FnSymbol* useFn = useCall->isResolved()) {
        if (!strcmp(useFn->name, "=") ||
            useFn->hasFlag(FLAG_AUTO_COPY_FN) ||
            useFn->hasFlag(FLAG_INIT_COPY_FN)) {

//           printf("CALL SITE\n");
//           list_view(move->getFunction());
//           printf("MOVE => ");
//           list_view(move);
//           printf("OLD FUNCTION\n");
//           list_view(fn);

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
            Symbol* ret = newFn->getReturnSymbol();
            newFn->body->body.tail->replace(new CallExpr(PRIM_RETURN, gVoid));
            newFn->retType = dtVoid;
            fn->defPoint->insertBefore(new DefExpr(newFn));
            Vec<SymExpr*> symExprs;
            collectSymExprs(newFn, symExprs);
            forv_Vec(SymExpr, se, symExprs) {
              if (se->var == ret) {
                CallExpr* move = toCallExpr(se->parentExpr);
                if (move && move->isPrimitive(PRIM_MOVE) && move->get(1) == se) {
                  SET_LINENO(move);
                  if (!strcmp(useFn->name, "=")) {
                    Symbol* tmp = newTemp("_ret_to_arg_tmp_", useFn->retType);
                    move->insertBefore(new DefExpr(tmp));
                    move->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_DEREF, arg)));
                    move->insertAfter(new CallExpr(PRIM_MOVE, arg, new CallExpr(useFn, tmp, ret)));
                  } else {
                    move->insertAfter(new CallExpr(PRIM_MOVE, arg, new CallExpr(useFn, ret)));
                  }
                } else {
                  Symbol* tmp = newTemp("ret_to_arg_tmp_", useFn->retType);
                  se->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  se->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_DEREF, arg)));
                  se->var = tmp;
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
          INT_ASSERT(useMove && useMove->isPrimitive(PRIM_MOVE));

          Symbol* useLhs = toSymExpr(useMove->get(1))->var;
          if (!useLhs->type->symbol->hasFlag(FLAG_REF)) {
            useLhs = newTemp("ret_to_arg_ref_tmp_", useFn->retType->refType);
            move->insertBefore(new DefExpr(useLhs));
            move->insertBefore(new CallExpr(PRIM_MOVE, useLhs, new CallExpr(PRIM_ADDR_OF, useMove->get(1)->remove())));
          }
          // lhs->defPoint->remove();
          move->replace(call->remove());
          useMove->remove();
          call->insertAtTail(useLhs);

//           printf("NEW FUNCTION\n");
//           list_view(newFn);
//           printf("TRANSFORMED CALL SITE\n");
//           list_view(call->getFunction());
//           printf("MOVE => ");
//           list_view(move);

          return;
        }
      }
    }
    //USR_WARN(move, "possible premature free");
    //nprint_view(move);
    //  list_view(move->getFunction());
    //  list_view(fn);
  } else {
    if (useMap.get(lhs) && useMap.get(lhs)->n > 0) {
      //USR_WARN(move, "possible premature free (use not found)");
      //nprint_view(move);
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
      ClassType* ct = toClassType(fn->_this->getValType());
      INT_ASSERT(ct);

      //
      // insert calls to destructors for all 'value' fields
      //
      for_fields_backward(field, ct) {
        SET_LINENO(field);
        if (field->type->destructor) {
          ClassType* fct = toClassType(field->type);
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
                fn->insertAtTail(new CallExpr(autoDestroy, var));
              }
  }
  fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
}


void
callDestructors() {
  fixupDestructors();

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

  insertAutoDestroyTemps();

  freeDefUseMaps(defMap, useMap);

  returnRecordsByReferenceArguments();

  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol) {
      if (call->isPrimitive(PRIM_YIELD)) {
        SymExpr* yieldExpr = toSymExpr(call->get(1));
        CallExpr* move = toCallExpr(call->prev);
        INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
        SymExpr* lhs = toSymExpr(move->get(1));
        INT_ASSERT(lhs && lhs->var == yieldExpr->var);
        Type* type = yieldExpr->var->type;
        if (isRecord(type) &&
            !type->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
            !type->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
          SET_LINENO(call);
          Symbol* tmp = newTemp("_yield_expr_tmp_", type);
          move->insertBefore(new DefExpr(tmp));
          move->insertAfter(new CallExpr(PRIM_MOVE, yieldExpr->var, new CallExpr(autoCopyMap.get(tmp->type), tmp)));
          lhs->var = tmp;
        }
      }
    }
  }

  insertGlobalAutoDestroyCalls();
}
