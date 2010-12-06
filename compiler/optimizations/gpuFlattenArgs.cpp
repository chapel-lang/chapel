// In order to achieve good memory performance on the gpu, it is very important
// that we pass our arguments to the GPU kernel as scalars whenever possible, rather
// than strctures. In order to do this, one of the goals of this compiler pass is to
// compute the GPU def/use sets and pass in only those arguments which are used.

#include "expr.h"
#include "stmt.h"
#include "optimizations.h"
#include "passes.h"
#include "astutil.h"

static Map<Symbol*, Expr*> cc_cache_map;

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
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(fn, defMap, useMap);
  forv_Vec(CallExpr, fncall, *fn->calledBy) {
    int argnum = 0;
    for_formals_actuals(formal, actual, fncall) {
      argnum++;
      // Don't remove the first 6 args (numBlocks, threadsPerBlock,..) from the
      // formal/actual list since they are also used in between the <<< >>>
      if (isDeadArg(formal, defMap, useMap) && argnum > 6) {
        formal->defPoint->remove();
        // Remove any previous definitions/uses of the shared memory
        // array before kernel invocation

        /* ALBERT */
        if ((formal->hasFlag(FLAG_SHARED) || 
              formal->typeInfo()->symbol->hasFlag(FLAG_SHARED)) && 
            formal->typeInfo()->symbol->hasFlag(FLAG_DATA_CLASS)) {
          SymExpr *actExpr = toSymExpr(actual);
          Vec<CallExpr*> calls;
          collectCallExprs(fncall->getFunction(), calls);
          forv_Vec(CallExpr, call, calls) {
            if (call->isPrimitive(PRIM_MOVE)) {
              SymExpr* lhs = toSymExpr(call->get(1));
              if (lhs->var == actExpr->var) {
                call->remove();
                actExpr->var->defPoint->remove();
              }
            }
          }
        }
        actual->remove();
      }
    }
  }
  freeDefUseMaps(defMap, useMap);
}

static void
implicitDataTransfer(Map<Symbol*,Vec<SymExpr*>*> baseSet, 
    Map<Symbol*, Type*> typeSet,
    FnSymbol *fn,
    Map<Symbol*,Vec<SymExpr*>*>& defMap, 
    Map<Symbol*,Vec<SymExpr*>*>& useMap) {

  compute_call_sites();
  freeDefUseMaps(defMap, useMap);
  buildDefUseMaps(defMap, useMap);
  Vec<Symbol*> keys;
  Type *ts = NULL;
  baseSet.get_keys(keys);
  forv_Vec(Symbol, key, keys) {
    Vec<SymExpr *>* parv = baseSet.get(key);
    int totalUse = 0;
    int totalDef = 0;
    printf("total parv per base = %d\n", parv->n);
    forv_Vec(SymExpr, var, *parv) {
      Vec<SymExpr*>* uses = useMap.get(var->var);
      Vec<SymExpr*>* defs = defMap.get(var->var);
      if (uses)
        totalUse += uses->n;
      if (defs)
        totalDef += defs->n;
      ts = var->typeInfo()->symbol->type;
    }
    printf("total use = %d total def = %d\n", totalUse, totalDef - parv->n);
    if (totalUse > 0) {
      forv_Vec(CallExpr, fncall, *fn->calledBy) {
        Type* ts = typeSet.get(key);
        CallExpr *copyExpr = new CallExpr(PRIM_IMPLICIT_COPY_HOST_GPU, 
            formal_to_actual(fncall, key)->copy(), ts->getValType()->symbol);
        fncall->getStmtExpr()->insertBefore(copyExpr);
      }
    }
    if (totalDef - parv->n > 0) {
      printf(".... = %d\n", totalDef);
      forv_Vec(CallExpr, fncall, *fn->calledBy) {
        Type* ts = typeSet.get(key);
        CallExpr *copyExpr = new CallExpr(PRIM_IMPLICIT_COPY_GPU_HOST, 
            formal_to_actual(fncall, key)->copy(), ts->getValType()->symbol);
        fncall->getStmtExpr()->insertAfter(copyExpr);
      }
    }
  }
}

static void
findStoredRef(FnSymbol *fn, Vec<Symbol*>& formalList, 
    Map<Symbol*,Vec<SymExpr*>*>& defMap) {
  Vec<CallExpr*> calls;
  collectCallExprs(fn, calls);
  forv_Vec(CallExpr, call, calls) {
    bool workToDo = true;
    if (call->isPrimitive(PRIM_MOVE)) {
      SymExpr *base = toSymExpr(call->get(1));
      if (base && isReferenceType(base->var->type)) {
        while (!isArgSymbol(base->var) && workToDo) {
          for_defs(def, defMap, base->var) {
            INT_ASSERT(def);
            if (CallExpr* subcall = toCallExpr(def->parentExpr)) {
              if (subcall->isPrimitive(PRIM_MOVE)) {
                if (CallExpr* rhs = toCallExpr(subcall->get(2))) {
                  if (rhs->isPrimitive(PRIM_GPU_GET_VAL)) {
                    base = toSymExpr(rhs->get(1));
                    
                    if (base->var->hasFlag(FLAG_SHARED)) {
                      workToDo = false;
                      continue;
                    }
                   
                    INT_ASSERT(base);
                  }
                  else if (rhs->isPrimitive(PRIM_SET_REF) ||
                           rhs->isPrimitive(PRIM_GET_MEMBER) ||
                           rhs->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                           rhs->isPrimitive(PRIM_SET_SVEC_MEMBER) ||
                           rhs->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                           rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
                    workToDo = false;
                    continue;
                  }
                }
              }
            }
          }
        }
        // Added formals that we don't want to spill
        formalList.add(base->var);
      }
    }
  }
}

static void
spillMaxArgs(FnSymbol *fn) {
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(fn, defMap, useMap);

  forv_Vec(CallExpr, fncall, *fn->calledBy) {
    int argnum = 0;
    ModuleSymbol* mod = fncall->getModule();
    Vec<Symbol*> formalList;
    findStoredRef(fn, formalList, defMap);
    for_formals_actuals(formal, actual, fncall) {
      argnum++;
      // Don't remove the first 6 args from the formal/actual list
      // since they are also used in between the <<< >>>
      if (argnum > maxGPUargs+6) {
        if (!formalList.in(formal)) {
          VarSymbol *tmpvar = new VarSymbol(formal->name, formal->type);
          tmpvar->addFlag(FLAG_GPU_CALL);
          mod->block->insertAtHead(new DefExpr(tmpvar));
          for_uses(use, useMap, formal) {
            use->var = tmpvar;
          }
          fncall->getStmtExpr()->insertBefore(new CallExpr(PRIM_GPU_COPY_SCALAR_TO_CONST_MEM, tmpvar, actual->copy(), new_IntSymbol(1)));
          formal->defPoint->remove();
          actual->remove();
        }
      }
      else if (formal->hasFlag(FLAG_GEN_CC)) {
        // test --- 0424
        printf("in this code ..\n");
        forv_Vec(VarSymbol, var, gVarSymbols) {
          //if (var->hasFlag(FLAG_CONST_MEM_SIZE)) {
            if (!strcmp(var->cname, "foo"))
            printf("HERE?!?!?\n");
          //}
        }

        VarSymbol *tmpvar = new VarSymbol(formal->name, formal->type);
        tmpvar->addFlag(FLAG_GPU_CALL);
        mod->block->insertAtHead(new DefExpr(tmpvar));
        mod->block->insertAtHead(new CallExpr(PRIM_DECLARE_CONSTANT_MEM, tmpvar, new_IntSymbol(2048))); // <---- FIX ASAP -- 2048 used for MRI
        //mod->block->insertAtHead(new CallExpr(PRIM_DECLARE_CONSTANT_MEM, tmpvar, new_IntSymbol(4000))); // <---- FIX ASAP -- 4000 used for CP
        for_uses(use, useMap, formal)
          use->var = tmpvar;
        SymExpr *base = toSymExpr(cc_cache_map.get(formal));
        fncall->getStmtExpr()->insertBefore(new CallExpr(PRIM_GPU_COPY_ARRAY_TO_CONST_MEM, tmpvar, actual->copy(), base));
        formal->defPoint->remove();
        actual->remove();
      }


    }
  }
  freeDefUseMaps(defMap, useMap);
}

static bool
findBaseVar(SymExpr **base, Map<Symbol*,Vec<SymExpr*>*>& defMap,
    Vec<CallExpr*>& deleteableCallExprs) {

  Vec<CallExpr*> tentativeDeletedExprs;

  bool workToDo = true;
  while (!isArgSymbol((*base)->var) && workToDo) {
    if (!defMap.get((*base)->var))
      return false;
    for_defs(def, defMap, (*base)->var) {
      INT_ASSERT(def);
      if (CallExpr* subcall = toCallExpr(def->parentExpr)) {
        if (subcall->isPrimitive(PRIM_MOVE)) {
          if (CallExpr* rhs = toCallExpr(subcall->get(2))) {
            if (rhs->isPrimitive(PRIM_GPU_GET_VAL) || 
                rhs->isPrimitive(PRIM_GET_REF) ||
                rhs->isPrimitive(PRIM_CAST)) {
              return false;
            }
            else if (rhs->isPrimitive(PRIM_GET_MEMBER) ||
                rhs->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                rhs->isPrimitive(PRIM_SET_SVEC_MEMBER) ||
                rhs->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
              *base = toSymExpr(rhs->get(1));
              INT_ASSERT(*base);
              tentativeDeletedExprs.add(subcall);
            }
            else {
              INT_FATAL("Unhandled case in GPUFlattenargs");
            }
          }
          else if (SymExpr* rhs = toSymExpr(subcall->get(2))) {
            *base = rhs;
            INT_ASSERT(*base);
            tentativeDeletedExprs.add(subcall);
          }
          else {
            INT_FATAL("Unhandled case in GPUFlattenargs");
          }
        }
      }
    }
  }
  if (tentativeDeletedExprs.length() > 0)
    deleteableCallExprs.append(tentativeDeletedExprs);
  return true;
}

static void
hoistGPUstructs(Vec<CallExpr*>& deleteableCallExprs) {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    Map<Symbol*, Type*> typeSet;
    Map<Symbol*,Vec<SymExpr*>*> baseSet;
    Map<Symbol*, int> ccsizeMap;
    Map<Symbol*, SymbolMap*> duplicateArrays;
    SymbolMap dupGpuArrays;

    if (fn->hasFlag(FLAG_GPU_ON)) {

      //removeUnnecessaryAutoCopyCalls(fn);

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
        if (call->isPrimitive(PRIM_ARRAY_GET)) {
          // Convert normal host array accesses into gpu accesses
          call->primitive = primitives[PRIM_GPU_GET_VALUE];
        }
        if (call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
            // test
            call->isPrimitive(PRIM_GET_MEMBER) ||
            call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
            call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
          SymExpr *base = toSymExpr(call->get(1));

          // If the structure base is a formal argument to the kernel, hoist it
          // If copy propagation has not been enabled and/or the structure is
          // not an argument
          // Need to refactor to prevent potential infinite loops
          bool ret = findBaseVar(&base, defMap, deleteableCallExprs);
          if (ret && base) {
            SymExpr *value = toSymExpr(call->get(2));
            forv_Vec(CallExpr, fncall, *fn->calledBy) {
                SymbolMap *duparray = duplicateArrays.get(base->var);
                if (duparray == NULL || duparray->get(value->var) == NULL) {
                  ArgSymbol* arg = NULL;
                  CallExpr* newCall = NULL;
                  VarSymbol* tmp = NULL;
                  if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
                    tmp = newTemp(value->typeInfo()->symbol->type);
                    newCall = new CallExpr(PRIM_GET_MEMBER_VALUE,
                        formal_to_actual(fncall,base->var)->copy(), value->var);
                    arg = new ArgSymbol(INTENT_BLANK, value->var->name, value->typeInfo()->symbol->type);
                    // If the array is from constant memory, save off the size and name of it for later use
#if 0
                    if (value->typeInfo()->symbol->hasFlag(FLAG_CCACHE)) {
                      long test;
                      VarSymbol *tmp = newTemp(base->typeInfo()->getField("size")->type);
                      printf("test is = %ld\n", tmp);
                    }
#endif
                  }
                  else if (call->isPrimitive(PRIM_GET_MEMBER)) {
                    printf("Inside PRIM_GET_MEMBER\n");
                    tmp = newTemp(value->typeInfo()->symbol->type);
                    tmp->type = tmp->type->refType;
                    newCall = new CallExpr(PRIM_GET_MEMBER,
                        formal_to_actual(fncall,base->var)->copy(), value->var);
                    arg = new ArgSymbol(INTENT_BLANK, value->var->name, value->typeInfo()->symbol->type);
                  }
                  else if (call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
                    tmp = newTemp(call->typeInfo()->symbol->type);
                    newCall = new CallExpr(PRIM_GET_SVEC_MEMBER_VALUE,
                        formal_to_actual(fncall,base->var)->copy(), value->var);
                    arg = new ArgSymbol(INTENT_BLANK, base->var->name, call->typeInfo()->symbol->type);
                  }
                  else if (call->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
                    printf("Inside PRIM_GET_SVEC_MEMBER\n");
                    tmp = newTemp(call->typeInfo()->symbol->type);
                    newCall = new CallExpr(PRIM_GET_SVEC_MEMBER,
                        formal_to_actual(fncall,base->var)->copy(), value->var);
                    arg = new ArgSymbol(INTENT_BLANK, base->var->name, call->typeInfo()->symbol->type);
                  }
                  else
                    INT_FATAL("Unhandled case in GPUFlattenargs");

                  fncall->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  fncall->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, newCall));
                  fncall->insertAtTail(tmp);
                  fn->insertFormalAtTail(new DefExpr(arg));
                  SymExpr* newarg = new SymExpr(arg);
                  call->replace(newarg);
                  if (!duparray) {
                    duparray = new SymbolMap();
                    duplicateArrays.put(base->var, duparray);
                  }
                  duparray->put(value->var, newarg->var);
                }
                else {
                  // Found base argument previously
                  call->replace(new SymExpr(duparray->get(value->var)));
                }
            }
          }
        }
        // Hoist all GET_MEMBER_VALUE calls from the device kernel to the caller of the kernel
        else if (call->isPrimitive(PRIM_GPU_GET_VALUE)) {
          SymExpr *base = toSymExpr(call->get(1));
          bool ret = findBaseVar(&base, defMap, deleteableCallExprs);
          if (ret && base) {
            if (base->typeInfo()->symbol->hasFlag(FLAG_IMPLICIT_GPU_ARRAY)) {
              if (CallExpr *cexpr = toCallExpr(call->parentExpr)) {
                SymExpr *parentBase = toSymExpr(cexpr->get(1));
                Vec<SymExpr *>* parv = baseSet.get(base->var);
                if (parv)
                  parv->add(parentBase);
                else {
                  parv = new Vec<SymExpr*>();
                  parv->add(parentBase);
                  baseSet.put(base->var, parv);
                }
              }
            }

            SymExpr *value = toSymExpr(call->get(2));
            forv_Vec(CallExpr, fncall, *fn->calledBy) {

              Symbol* duparray = dupGpuArrays.get(base->var);
              if (!duparray) {
                SymExpr* newarg = NULL;
                VarSymbol* tmp = NULL;
                tmp = newTemp(call->typeInfo()->symbol->type);

                if (base->typeInfo()->symbol->hasFlag(FLAG_SHARED)) {
                  printf("SHARED!\n");
                  fn->insertAtHead(new DefExpr(tmp));
                  tmp->addFlag(FLAG_SHARED);
                  tmp->addFlag(FLAG_GEN_SHARED);
                  newarg = new SymExpr(tmp);
                }
                else {
                  fncall->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  // Use ARRAY_GET in order to get the ->_data member for 
                  // constant cache arrays
                  if (base->typeInfo()->symbol->hasFlag(FLAG_CCACHE)) {
                    fncall->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, 
                          new CallExpr(PRIM_ARRAY_GET,
                            formal_to_actual(fncall, base->var)->copy(), new_IntSymbol(0))));
                  }
                  else {
                  // Use GPU_GET_ARRAY in order to get the ->_cudata member
                    fncall->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, 
                          new CallExpr(PRIM_GPU_GET_ARRAY, 
                            formal_to_actual(fncall, base->var)->copy())));
                  }
                  fncall->insertAtTail(tmp);
                  SymExpr *old_val = toSymExpr(call->get(1));

                  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, 
                      old_val->var->name, call->typeInfo()->symbol->type);
                  fn->insertFormalAtTail(new DefExpr(arg));
                  newarg = new SymExpr(arg);
                  // Associate a base to the array arg in order to pull out the arraysize
                  if (base->typeInfo()->symbol->hasFlag(FLAG_CCACHE)) {
                    cc_cache_map.put(arg, formal_to_actual(fncall,base->var)->copy());
                    arg->addFlag(FLAG_GEN_CC);
                  }

                }
                call->replace(new CallExpr(PRIM_GPU_GET_VAL, newarg, value->var));
                dupGpuArrays.put(base->var, newarg->var);
                typeSet.put(base->var, newarg->typeInfo()->symbol->type);
              }
              else {
                call->replace(new CallExpr(PRIM_GPU_GET_VAL, duparray, value->var));
                typeSet.put(base->var, duparray->typeInfo()->symbol->type);
              }

            }
          }
        }
        // Remove any halts etc.. (not sure where else to put it)
        else if (call->isNamed("halt")) {
          call->remove();
        }
      //  else {
      //    // Need some place to remove those pesky destructors that make it in
      //    FnSymbol *fcall = call->isResolved();
      //    if (fcall && !strcmp(fcall->name, "~chpl_destroy")) {
      //      printf("removing chpl_destroy from the kernel : fcall name = %s\n", fcall->name);
      //      call->remove();
      //    }
      //  }
      }
      // Lets copy data from host to gpu (or vice versa)
      if (baseSet.length() > 0) 
        implicitDataTransfer(baseSet, typeSet, fn, defMap, useMap);
    }
  }
}


static void
deleteOldCalls(Vec<CallExpr*>& deleteableCallExprs) {
  forv_Vec(CallExpr, call, deleteableCallExprs) {
    if (call->parentSymbol) {
      if (call->isPrimitive(PRIM_MOVE)) {
        Symbol* lhs = toSymExpr(call->get(1))->var;
        Symbol* rhs = NULL;
        if (CallExpr* subcall = toCallExpr(call->get(2))) {
          if (subcall->isPrimitive(PRIM_GET_REF) ||
              subcall->isPrimitive(PRIM_GET_MEMBER) ||
              subcall->isPrimitive(PRIM_SET_SVEC_MEMBER) ||
              subcall->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
              subcall->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) ||
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
}

static void
cleanupGPUCode() {
  // Apply copy propagation just on GPU functions
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
      spillMaxArgs(fn);
    }
  }
}

void 
gpuFlattenArgs()  {

  Vec<CallExpr*> deleteableCallExprs; // Vector of expression that we can remove

  compute_call_sites();
  hoistGPUstructs(deleteableCallExprs);

  // Delete expressions and definitions that are not used anymore as a result 
  // of our transformation above
  deleteOldCalls(deleteableCallExprs);

  // Cleanup:
  // 1) (If we can) perform various code elimination transformations
  // 2) Remove any dead arguments from the actual/formal list as a result of the 
  // earlier hoisting transformation
  // 3) Spill any remaining args out into Constant Cache
  cleanupGPUCode();

  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap);
  // Delete any old/stale shared memory arrays that are outside of the kernel
  /* ALBERT
  forv_Vec(DefExpr, def, gDefExprs) {
    if (def->sym->hasFlag(FLAG_SHARED) && !def->sym->hasFlag(FLAG_GEN_SHARED)) {
      for_uses(use, useMap, def->sym) {
        if (CallExpr* exp = toCallExpr(use->parentExpr))
          exp->remove();
      }
    }
  }
  */
}
