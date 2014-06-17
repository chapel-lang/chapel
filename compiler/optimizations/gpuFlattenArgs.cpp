/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


// In order to achieve good memory performance on the gpu, it is very important
// that we pass our arguments to the GPU kernel as scalars whenever possible, rather
// than strctures. In order to do this, one of the goals of this compiler pass is to
// compute the GPU def/use sets and pass in only those arguments which are used.

#include "expr.h"
#include "stmt.h"
#include "optimizations.h"
#include "passes.h"
#include "astutil.h"

static bool 
isDeadArg(Symbol* var,
    Map<Symbol*,Vec<SymExpr*>*>& defMap,
    Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  if (isReferenceType(var->type)) {
    Vec<SymExpr*>* uses = useMap.get(var);
    Vec<SymExpr*>* defs = defMap.get(var);
    return (!uses || uses->n == 0) && (!defs || defs->n <= 1);
  } else {
    Vec<SymExpr*>* uses = useMap.get(var);
    return !uses || uses->n == 0;
  }
}

static void
deadArgumentElimination(FnSymbol *fn) {
  Vec<BaseAST*> asts;
  compute_call_sites();
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(fn, defMap, useMap);
  forv_Vec(CallExpr, fncall, *fn->calledBy) {
    int argnum = 0;
    for_formals_actuals(formal, actual, fncall) {
      argnum++;
      // Don't remove the first 2 args (numBlocks, threadsPerBlock) from the formal/actual list
      // since they are also used in between the <<< >>>
      if (isDeadArg(formal, defMap, useMap)  && argnum > 2) {
        formal->defPoint->remove();
        actual->remove();
      }
    }
  }
  freeDefUseMaps(defMap, useMap);
}

void 
gpuFlattenArgs()  {

  Vec<CallExpr*> deleteableCallExprs; // Vector of expression that we can remove

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_GPU_ON)) {
      Vec<CallExpr*> calls;
      Map<Symbol*,Vec<SymExpr*>*> defMap;
      Map<Symbol*,Vec<SymExpr*>*> useMap;
      buildDefUseMaps(fn, defMap, useMap);
      collectCallExprs(fn, calls);

      // Hoist all GET_MEMBER_VALUE calls from the device kernel to the caller 
      // of the kernel.
      // TODO: This might be too optimistic in scalarizing ALL get_member_value 
      // accesses
      forv_Vec(CallExpr, call, calls) {
        if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
          SymExpr *base = toSymExpr(call->get(1));
          SymExpr *value= toSymExpr(call->get(2));

          // If the structure base is a formal argument to the kernel, hoist it
          if (base && isArgSymbol(base->var)) {
            forv_Vec(CallExpr, fncall, *fn->calledBy) {
              VarSymbol* tmp = newTemp(value->typeInfo()->symbol->type);
              fncall->getStmtExpr()->insertBefore(new DefExpr(tmp));
              fncall->getStmtExpr()->insertBefore(
                  new CallExpr(PRIM_MOVE, tmp, 
                    new CallExpr(PRIM_GET_MEMBER_VALUE, 
                      formal_to_actual(fncall, base->var)->copy(), value->var)));
              fncall->insertAtTail(tmp);
              ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, value->var->name, 
                  value->typeInfo()->symbol->type);
              fn->insertFormalAtTail(new DefExpr(arg));
              // Replace old structure access with just the arg
              call->replace(new SymExpr(arg));
            }
          }
          // If copy propagation has not been enabled and/or the structure is 
          // not an argument
          else {
            // Need to refactor to prevent potential infinite loops
            while (!isArgSymbol(base->var)) {
              for_defs(def, defMap, base->var) {
                INT_ASSERT(def);
                if (CallExpr* subcall = toCallExpr(def->parentExpr)) {
                  if (subcall->isPrimitive(PRIM_MOVE)) {
                    if (CallExpr* rhs = toCallExpr(subcall->get(2))) {
                      if (rhs->isPrimitive(PRIM_GET_MEMBER) ||
                          rhs->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
                        base = toSymExpr(rhs->get(1));
                        INT_ASSERT(base);
                      }
                    }
                    else if (SymExpr* rhs = toSymExpr(subcall->get(2))) {
                      base = rhs;
                      INT_ASSERT(base);
                    }
                    else
                      INT_FATAL("Unhandled case in GPUFlattenargs");
                    // This call expression will be removed later
                    deleteableCallExprs.add(subcall);
                  }
                }
              }
            }
            forv_Vec(CallExpr, fncall, *fn->calledBy) {
              VarSymbol* tmp = newTemp(value->typeInfo()->symbol->type);
              fncall->getStmtExpr()->insertBefore(new DefExpr(tmp));
              fncall->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, 
                    new CallExpr(PRIM_GET_MEMBER_VALUE, 
                      formal_to_actual(fncall,base->var)->copy(), value->var)));
              fncall->insertAtTail(tmp);
              ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, value->var->name, value->typeInfo()->symbol->type);
              fn->insertFormalAtTail(new DefExpr(arg));
              call->replace(new SymExpr(arg));
            }
          }
        }
        // Hoist all GET_MEMBER_VALUE calls from the device kernel to the caller of the kernel
        else if (call->isPrimitive(PRIM_GPU_GET_VALUE)) {
          SymExpr *base = toSymExpr(call->get(1));
          SymExpr *rhs= toSymExpr(call->get(2));
          if (base && isArgSymbol(base->var)) {
            forv_Vec(CallExpr, fncall, *fn->calledBy) {
              VarSymbol *tmp = newTemp(call->typeInfo()->symbol->type);
              fncall->getStmtExpr()->insertBefore(new DefExpr(tmp));
              fncall->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GPU_GET_ARRAY, formal_to_actual(fncall, base->var)->copy())));
              fncall->insertAtTail(tmp);
              ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, rhs->var->name, call->typeInfo()->symbol->type);
              fn->insertFormalAtTail(new DefExpr(arg));
              call->replace(new CallExpr(PRIM_GPU_GET_VAL, new SymExpr(arg), rhs->var));
            }
          }
          // If copy propagation has not been enabled or the structure is not an argument
          else {
            SymExpr *base = toSymExpr(call->get(1));
            SymExpr *member = toSymExpr(call->get(2));
            while (!isArgSymbol(base->var)) {
              for_defs(def, defMap, base->var) {
                INT_ASSERT(def);
                if (CallExpr* subcall = toCallExpr(def->parentExpr)) {
                  if (subcall->isPrimitive(PRIM_MOVE)) {
                    if (CallExpr* rhs = toCallExpr(subcall->get(2))) {
                      if (rhs->isPrimitive(PRIM_GET_MEMBER) ||
                          rhs->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                          rhs->isPrimitive(PRIM_DEREF)) {
                        base = toSymExpr(rhs->get(1));
                        INT_ASSERT(base);
                      }
                    }
                    else if (SymExpr* rhs = toSymExpr(subcall->get(2))) {
                      base = rhs;
                      INT_ASSERT(base);
                    }
                    else {
                      INT_FATAL("Unhandled case in GPUFlattenargs");
                    }
                    // This call expression will be removed later
                    deleteableCallExprs.add(subcall);
                  }
                }
              }
            }
            forv_Vec(CallExpr, fncall, *fn->calledBy) {
              VarSymbol *tmp = newTemp(call->typeInfo()->symbol->type);
              fncall->getStmtExpr()->insertBefore(new DefExpr(tmp));
              fncall->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, 
                    new CallExpr(PRIM_GPU_GET_ARRAY, 
                      formal_to_actual(fncall, 
                        base->var)->copy())));
              fncall->insertAtTail(tmp);
              ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, member->var->name, call->typeInfo()->symbol->type);
              fn->insertFormalAtTail(new DefExpr(arg));
              call->replace(new CallExpr(PRIM_GPU_GET_VAL, new SymExpr(arg), member->var));
            }
          }
        }
      }
    }
  }

  // Delete expressions and definitions that are not used anymore as a result 
  // of our transformation above
  forv_Vec(CallExpr, call, deleteableCallExprs) {
    if (call->parentSymbol) {
      if (call->isPrimitive(PRIM_MOVE)) {
        Symbol* lhs = toSymExpr(call->get(1))->var;
        Symbol* rhs = NULL;
        if (CallExpr* subcall = toCallExpr(call->get(2))) {
          if (subcall->isPrimitive(PRIM_DEREF) ||
              subcall->isPrimitive(PRIM_GET_MEMBER) ||
              subcall->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
            rhs = toSymExpr(subcall->get(1))->var;
          }
        }
        else if (SymExpr *symexp = toSymExpr(call->get(2))) {
          rhs = symexp->var;
        }

        if (!fNoDeadCodeElimination && !fNoCopyPropagation) {
          if (!isArgSymbol(lhs) && lhs->defPoint && lhs->defPoint->parentSymbol)
            lhs->defPoint->remove();
          if (!isArgSymbol(rhs) && rhs->defPoint && rhs->defPoint->parentSymbol)
            rhs->defPoint->remove();
        }
      }
      call->remove();
    }
  }

  // If Function has unused arguments, remove them from actual/formal list
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_GPU_ON)) {
      if (!fNoCopyPropagation) {
        localCopyPropagation(fn);
        if (!fNoDeadCodeElimination) {
          deadCodeElimination(fn);
        }
      }
      if (!fNoCopyPropagation) {
        globalCopyPropagation(fn);
        singleAssignmentRefPropagation(fn);
        if (!fNoDeadCodeElimination) {
          deadCodeElimination(fn);
        }
      }
      deadArgumentElimination(fn);
    }
  }
}
