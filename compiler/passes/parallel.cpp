//
// Transformations for begin, cobegin, and on statements
//

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "stringutil.h"
#include "driver.h"
#include "files.h"


// Package args into a class and call a wrapper function with that
// object. The wrapper function will then call the function
// created by the previous parallel pass. This is a way to pass along
// multiple args through the limitation of one arg in the runtime's
// thread creation interface. 
static void
bundleArgs(CallExpr* fcall) {
  SET_LINENO(fcall);
  ModuleSymbol* mod = fcall->getModule();
  FnSymbol* fn = fcall->isResolved();

  // create a new class to capture refs to locals
  ClassType* ctype = new ClassType( CLASS_CLASS);
  TypeSymbol* new_c = new TypeSymbol( astr("_class_locals", 
                                           fn->name),
                                      ctype);
  new_c->addFlag(FLAG_NO_OBJECT);
  new_c->addFlag(FLAG_NO_WIDE_CLASS);

  // add the function args as fields in the class
  int i = 1;
  bool first = true;
  for_actuals(arg, fcall) {
    if (fn->hasFlag(FLAG_ON) && first) {
      first = false;
      continue;
    }
    SymExpr *s = toSymExpr(arg);
    Symbol  *var = s->var; // arg or var
    var->addFlag(FLAG_CONCURRENTLY_ACCESSED);
    VarSymbol* field = new VarSymbol(astr("_", istr(i), "_", var->name), var->type);
    ctype->fields.insertAtTail(new DefExpr(field));
    i++;
  }
  mod->block->insertAtHead(new DefExpr(new_c));
    
  // create the class variable instance and allocate it
  VarSymbol *tempc = newTemp(astr("_args_for", fn->name), ctype);
  fcall->insertBefore( new DefExpr( tempc));
  CallExpr *tempc_alloc = new CallExpr( PRIMITIVE_CHPL_ALLOC_PERMIT_ZERO,
                                        ctype->symbol,
                                        new_StringSymbol( astr( "instance of class ", ctype->symbol->name)));
  fcall->insertBefore( new CallExpr( PRIMITIVE_MOVE,
                                     tempc,
                                     tempc_alloc));
  
  // set the references in the class instance
  i = 1;
  first = true;
  for_actuals(arg, fcall) {
    if (fn->hasFlag(FLAG_ON) && first) {
      first = false;
      continue;
    }
    SymExpr *s = toSymExpr(arg);
    Symbol  *var = s->var; // var or arg
    CallExpr *setc=new CallExpr(PRIMITIVE_SET_MEMBER,
                                tempc,
                                ctype->getField(i),
                                var);
    fcall->insertBefore( setc);
    i++;
  }
  
  // create wrapper-function that uses the class instance

  FnSymbol *wrap_fn = new FnSymbol( astr("wrap", fn->name));
  DefExpr  *fcall_def= (toSymExpr( fcall->baseExpr))->var->defPoint;
  if (fn->hasFlag(FLAG_ON)) {
    wrap_fn->addFlag(FLAG_ON_BLOCK);
    if (fn->hasFlag(FLAG_NON_BLOCKING))
      wrap_fn->addFlag(FLAG_NON_BLOCKING);
    ArgSymbol* locale = new ArgSymbol(INTENT_BLANK, "_dummy_locale_arg", dtInt[INT_SIZE_32]);
    wrap_fn->insertFormalAtTail(locale);
  } else if (fn->hasFlag(FLAG_COBEGIN_OR_COFORALL)) {
    wrap_fn->addFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK);
  }
  ArgSymbol *wrap_c = new ArgSymbol( INTENT_BLANK, "c", ctype);
  wrap_fn->insertFormalAtTail( wrap_c);

  mod->block->insertAtTail(new DefExpr(wrap_fn));
  if (fn->hasFlag(FLAG_ON)) {
    fcall->insertBefore(new CallExpr(wrap_fn, fcall->get(1)->remove(), tempc));
  } else
    fcall->insertBefore(new CallExpr(wrap_fn, tempc));

  if (fn->hasFlag(FLAG_BEGIN) || fn->hasFlag(FLAG_COBEGIN_OR_COFORALL)) {
    if (fn->hasFlag(FLAG_BEGIN))
      wrap_fn->addFlag(FLAG_BEGIN_BLOCK);
    wrap_fn->insertAtHead(new CallExpr(PRIMITIVE_THREAD_INIT));
  }
    
  // translate the original cobegin function
  CallExpr *new_cofn = new CallExpr( (toSymExpr(fcall->baseExpr))->var);
  if (fn->hasFlag(FLAG_ON))
    new_cofn->insertAtTail(new_IntSymbol(0)); // bogus actual
  for_fields(field, ctype) {  // insert args

    VarSymbol* tmp = newTemp(field->type);
    wrap_fn->insertAtTail(new DefExpr(tmp));
    wrap_fn->insertAtTail(
      new CallExpr(PRIMITIVE_MOVE, tmp,
        new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, wrap_c, field)));
    new_cofn->insertAtTail(tmp);
  }

  wrap_fn->retType = dtVoid;
  wrap_fn->insertAtTail(new_cofn);     // add new call
  if (fn->hasFlag(FLAG_ON))
    fcall->insertAfter(new CallExpr(PRIMITIVE_CHPL_FREE, tempc));
  else
    wrap_fn->insertAtTail(new CallExpr(PRIMITIVE_CHPL_FREE, wrap_c));

  fcall->remove();                     // rm orig. call
  fcall_def->remove();                 // move orig. def
  mod->block->insertAtTail(fcall_def); // to top-level
  normalize(wrap_fn);
}


static void
insertEndCount(FnSymbol* fn,
               Type* endCountType,
               Vec<FnSymbol*>& queue,
               Map<FnSymbol*,Symbol*>& endCountMap) {
  if (fn == chpl_main) {
    VarSymbol* var = newTemp("_endCount", endCountType);
    fn->insertAtHead(new DefExpr(var));
    endCountMap.put(fn, var);
    queue.add(fn);
  } else {
    ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "_endCount", endCountType);
    fn->insertFormalAtTail(arg);
    VarSymbol* var = newTemp("_endCount", endCountType);
    fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, var, arg));
    fn->insertAtHead(new DefExpr(var));
    endCountMap.put(fn, var);
    queue.add(fn);
  }
}


static ClassType*
buildHeapType(Type* type) {
  static Map<Type*,ClassType*> heapTypeMap;
  if (heapTypeMap.get(type))
    return heapTypeMap.get(type);

  ClassType* heap = new ClassType(CLASS_CLASS);
  TypeSymbol* ts = new TypeSymbol(astr("heap_", type->symbol->cname), heap);
  ts->addFlag(FLAG_NO_OBJECT);
  ts->addFlag(FLAG_HEAP);
  theProgram->block->insertAtTail(new DefExpr(ts));
  heap->fields.insertAtTail(new DefExpr(new VarSymbol("value", type)));
  heapTypeMap.put(type, heap);
  return heap;
}


static void
freeHeapAllocatedVars(Vec<Symbol*> heapAllocatedVars) {
  Vec<FnSymbol*> fnsContainingTaskll;

  // start with the functions created from begin, cobegin, and coforall statements
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_BEGIN) || fn->hasFlag(FLAG_COBEGIN_OR_COFORALL)) {
      fnsContainingTaskll.add(fn);
    }
  }
  // add any functions that call the functions added so far
  forv_Vec(FnSymbol, fn, fnsContainingTaskll) {
    forv_Vec(CallExpr, call, *fn->calledBy) {
      FnSymbol* caller = toFnSymbol(call->parentSymbol);
      INT_ASSERT(caller);
      fnsContainingTaskll.add_exclusive(caller);
    }
  }

  Vec<Symbol*> symSet;
  Vec<BaseAST*> asts;
  Vec<SymExpr*> symExprs;
  collect_asts(rootModule, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (def->parentSymbol) {
        if (isVarSymbol(def->sym) || isArgSymbol(def->sym)) {
          symSet.set_add(def->sym);
        }
      }
    } else if (SymExpr* se = toSymExpr(ast)) {
      symExprs.add(se);
    }
  }
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(symSet, symExprs, defMap, useMap);

  forv_Vec(Symbol, var, heapAllocatedVars) {
    // find out if the variable just put on the heap could be passed in as an argument
    // to a function created from a begin, cobegin, or coforall statement;
    // if not, free the heap memory just allocated at the end of the block
    if (defMap.get(var)->n == 1) {
      bool freeVar = true;
      Vec<Symbol*> varsToTrack;
      varsToTrack.add(var);
      forv_Vec(Symbol, v, varsToTrack) {
        if (useMap.get(v)) {
          forv_Vec(SymExpr, se, *useMap.get(v)) {
            if (CallExpr* call = toCallExpr(se->parentExpr)) {
              if (call->isPrimitive(PRIMITIVE_SET_REF) || call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
                  call->isPrimitive(PRIMITIVE_GET_LOCALEID))
                call = toCallExpr(call->parentExpr);
              if (call->isPrimitive(PRIMITIVE_MOVE))
                varsToTrack.add(toSymExpr(call->get(1))->var);
              else if (fnsContainingTaskll.in(call->isResolved())) {
                freeVar = false;
                break;
              }
            }
          }
          if (!freeVar) break;
        }
      }
      if (freeVar) {
        CallExpr* move = toCallExpr(defMap.get(var)->v[0]->parentExpr);
        INT_ASSERT(move && move->isPrimitive(PRIMITIVE_MOVE));
        FnSymbol* fn = toFnSymbol(move->parentSymbol);
        if (fn && move->parentExpr == fn->body)
          fn->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_CHPL_FREE, move->get(1)->copy()));
        else if (BlockStmt* parentBlock = toBlockStmt(move->parentExpr))
          parentBlock->insertAtTailBeforeGoto(new CallExpr(PRIMITIVE_CHPL_FREE, move->get(1)->copy()));
        else
          INT_FATAL(move, "unexpected case");
      }
    }
  }
}


static void
makeHeapAllocations() {
  Vec<Symbol*> refSet;
  Vec<Symbol*> refVec;
  Vec<Symbol*> varSet;
  Vec<Symbol*> varVec;

  // compute def uses with postorder asts set so that the first def is
  // the defining one; see ack!! below
  Vec<BaseAST*> asts;
  collect_asts(rootModule, asts);
  Vec<Symbol*> symSet;
  Vec<SymExpr*> symExprs;
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (def->parentSymbol) {
        if (isVarSymbol(def->sym) || isArgSymbol(def->sym)) {
          symSet.set_add(def->sym);
        }
      }
    } else if (SymExpr* se = toSymExpr(ast)) {
      symExprs.add(se);
    }
  }
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(symSet, symExprs, defMap, useMap);

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_BEGIN) || fn->hasFlag(FLAG_ON)) {
      for_formals(formal, fn) {
        if (formal->type->symbol->hasFlag(FLAG_REF)) {
          refSet.set_add(formal);
          refVec.add(formal);
        }
      }
    }
  }

  forv_Vec(DefExpr, def, gDefExprs) {
    if (def->sym->hasFlag(FLAG_HEAP_ALLOCATE)) {
      if (def->sym->type->symbol->hasFlag(FLAG_REF)) {
        refSet.set_add(def->sym);
        refVec.add(def->sym);
      } else {
        varSet.set_add(def->sym);
        varVec.add(def->sym);
      }
    } else if (!fLocal &&
               isModuleSymbol(def->parentSymbol) &&
               def->parentSymbol != rootModule &&
               isVarSymbol(def->sym) &&
               !def->sym->hasFlag(FLAG_PRIVATE)) {
      if (def->sym->hasFlag(FLAG_CONST) &&
          (is_int_type(def->sym->type) ||
           is_uint_type(def->sym->type) ||
           is_real_type(def->sym->type) ||
           def->sym->type == dtBool)) {
        // replicate global const of primitive type
        INT_ASSERT(defMap.get(def->sym) && defMap.get(def->sym)->n == 1);
        for_defs(se, defMap, def->sym) {
          se->getStmtExpr()->insertAfter(new CallExpr(PRIMITIVE_PRIVATE_BROADCAST, def->sym));
        }
      } else if (def->sym->type->symbol->hasFlag(FLAG_ARRAY) ||
                 def->sym->type->symbol->hasFlag(FLAG_DOMAIN)) {
        SymExpr* se = defMap.get(def->sym)->v[0];
        INT_ASSERT(se);
        se->getStmtExpr()->insertAfter(new CallExpr(PRIMITIVE_PRIVATE_BROADCAST, def->sym));
      } else {
        // put other global constants and all global variables on the heap
        varSet.set_add(def->sym);
        varVec.add(def->sym);
      }
    }
  }

  forv_Vec(Symbol, ref, refVec) {
    if (ArgSymbol* arg = toArgSymbol(ref)) {
      FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);
      forv_Vec(CallExpr, call, *fn->calledBy) {
        SymExpr* se = NULL;
        for_formals_actuals(formal, actual, call) {
          if (formal == arg)
            se = toSymExpr(actual);
        }
        INT_ASSERT(se->var->type->symbol->hasFlag(FLAG_REF));
        if (!refSet.set_in(se->var)) {
          refSet.set_add(se->var);
          refVec.add(se->var);
        }
      }
    } else if (VarSymbol* var = toVarSymbol(ref)) {
      INT_ASSERT(defMap.get(var)->n == 1);
      for_defs(def, defMap, var) {
        INT_ASSERT(def);
        if (CallExpr* call = toCallExpr(def->parentExpr)) {
          if (call->isPrimitive(PRIMITIVE_MOVE)) {
            if (CallExpr* rhs = toCallExpr(call->get(2))) {
              if (rhs->isPrimitive(PRIMITIVE_SET_REF)) {
                SymExpr* se = toSymExpr(rhs->get(1));
                INT_ASSERT(se);
                if (!varSet.set_in(se->var)) {
                  varSet.set_add(se->var);
                  varVec.add(se->var);
                }
              } else if (rhs->isPrimitive(PRIMITIVE_GET_MEMBER) ||
                         rhs->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)) {
                SymExpr* se = toSymExpr(rhs->get(1));
                INT_ASSERT(se);
                if (!varSet.set_in(se->var)) {
                  varSet.set_add(se->var);
                  varVec.add(se->var);
                }
              }
              //
              // Otherwise assume reference is to something that is
              // already on the heap!  This is concerning...  SJD:
              // Build a future that returns a reference in an
              // iterator to something that is not on the heap
              // (including not in an array).
              //
              // The alternative to making this assumption is to
              // follow the returned reference (assuming this is a
              // function call) through the function and make sure
              // that whatever it returns is on the heap.  Then if we
              // eventually see a GET_ARRAY primitive, we know it is
              // already on the heap.
              //
              // To debug this case, add an else INT_FATAL here.
              //
            } else if (SymExpr* rhs = toSymExpr(call->get(2))) {
              INT_ASSERT(rhs->var->type->symbol->hasFlag(FLAG_REF));
              if (!refSet.set_in(rhs->var)) {
                refSet.set_add(rhs->var);
                refVec.add(rhs->var);
              }
            } else
              INT_FATAL(ref, "unexpected case");
          } else
            INT_FATAL(ref, "unexpected case");
        } else
          INT_FATAL(ref, "unexpected case");
      }
    }
  }

  Vec<Symbol*> heapAllocatedVars;

  forv_Vec(Symbol, var, varVec) {
    INT_ASSERT(!var->type->symbol->hasFlag(FLAG_REF));

    if (ArgSymbol* arg = toArgSymbol(var)) {
      VarSymbol* tmp = newTemp(var->type);
      varSet.set_add(tmp);
      varVec.add(tmp);
      SymExpr* firstDef = new SymExpr(tmp);
      arg->getFunction()->insertAtHead(new CallExpr(PRIMITIVE_MOVE, firstDef, arg));
      addDef(defMap, firstDef);
      arg->getFunction()->insertAtHead(new DefExpr(tmp));
      for_defs(def, defMap, arg) {
        def->var = tmp;
        addDef(defMap, def);
      }
      for_uses(use, useMap, arg) {
        use->var = tmp;
        addUse(useMap, use);
      }
      continue;
    }
    ClassType* heapType = buildHeapType(var->type);
    bool first = true;

    //
    // global heap variables are put on the heap during program startup
    //
    if (isModuleSymbol(var->defPoint->parentSymbol)) {
      //      chpl_main->insertAtHead(new CallExpr(PRIMITIVE_MOVE, var, new CallExpr(PRIMITIVE_CHPL_ALLOC, heapType->symbol, new_StringSymbol("heap class"))));
      first = false;
    }

    //
    // handle variables with no definitions
    //  temporaries for destructured indices
    //
    if (!defMap.get(var) || defMap.get(var)->n == 0) {
      //
      // In this case, where should we put this?  ack!! let's assume
      // we can put it in front of the first use!
      //
      useMap.get(var)->v[0]->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, var, new CallExpr(PRIMITIVE_CHPL_ALLOC, heapType->symbol, new_StringSymbol(heapType->symbol->name))));
      heapAllocatedVars.add(var);
    }

    for_defs(def, defMap, var) {

      // ack!! this is troublesome: we're assuming that the first
      // definition in the defs vector is the initial definition
      if (first) {
        first = false;
        CallExpr* move = toCallExpr(def->parentExpr);
        INT_ASSERT(move && move->isPrimitive(PRIMITIVE_MOVE));
        VarSymbol* tmp = newTemp(var->type);
        move->insertBefore(new DefExpr(tmp));
        move->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, move->get(2)->remove()));
        move->insertAtTail(new CallExpr(PRIMITIVE_CHPL_ALLOC, heapType->symbol, new_StringSymbol(heapType->symbol->name)));
        move->insertAfter(new CallExpr(PRIMITIVE_SET_MEMBER, move->get(1)->copy(), heapType->getField(1), tmp));
        heapAllocatedVars.add(var);
      } else if (CallExpr* call = toCallExpr(def->parentExpr)) {
        if (call->isPrimitive(PRIMITIVE_MOVE)) {
          VarSymbol* tmp = newTemp(var->type);
          call->insertBefore(new DefExpr(tmp));
          call->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, call->get(2)->remove()));
          call->replace(new CallExpr(PRIMITIVE_SET_MEMBER, call->get(1)->copy(), heapType->getField(1), tmp));
        } else {
          VarSymbol* tmp = newTemp(var->type);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, def->var, heapType->getField(1))));
          def->replace(new SymExpr(tmp));
        }
      } else
        INT_FATAL(var, "unexpected case");
    }

    for_uses(use, useMap, var) {
      if (CallExpr* call = toCallExpr(use->parentExpr)) {
        if (call->isPrimitive(PRIMITIVE_SET_REF)) {
          CallExpr* move = toCallExpr(call->parentExpr);
          INT_ASSERT(move && move->isPrimitive(PRIMITIVE_MOVE));
          if (move->get(1)->typeInfo() == heapType) {
            call->replace(use->copy());
          } else {
            call->replace(new CallExpr(PRIMITIVE_GET_MEMBER, use->var, heapType->getField(1)));
          }
        } else if (call->isResolved()) {
          if (actual_to_formal(use)->type == heapType) {
            // do nothing
          } else {
            VarSymbol* tmp = newTemp(var->type);
            call->getStmtExpr()->insertBefore(new DefExpr(tmp));
            call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, use->var, heapType->getField(1))));
            use->replace(new SymExpr(tmp));
          }
        } else if (call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
                   call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE) ||
                   call->isPrimitive(PRIMITIVE_SET_MEMBER) &&
                   call->get(1) == use) {
          VarSymbol* tmp = newTemp(var->type->refType);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER, use->var, heapType->getField(1))));
          use->replace(new SymExpr(tmp));
        } else {
          VarSymbol* tmp = newTemp(var->type);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, use->var, heapType->getField(1))));
          use->replace(new SymExpr(tmp));
        }
      } else if (use->parentExpr)
        INT_FATAL(var, "unexpected case");
    }

    var->type = heapType;
  }

  freeHeapAllocatedVars(heapAllocatedVars);
}


void
parallel(void) {
  //
  // convert begin/cobegin/coforall/on blocks into nested functions and flatten
  //
  Vec<FnSymbol*> nestedFunctions;
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (block->blockInfo) {
      SET_LINENO(block);
      FnSymbol* fn = NULL;
      if (block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_BEGIN)) {
        fn = new FnSymbol("begin_fn");
        fn->addFlag(FLAG_BEGIN);
      } else if (block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_COBEGIN)) {
        fn = new FnSymbol("cobegin_fn");
        fn->addFlag(FLAG_COBEGIN_OR_COFORALL);
      } else if (block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_COFORALL)) {
        fn = new FnSymbol("coforall_fn");
        fn->addFlag(FLAG_COBEGIN_OR_COFORALL);
      } else if (block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_ON) ||
                 block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_ON_NB)) {
        fn = new FnSymbol("on_fn");
        fn->addFlag(FLAG_ON);
        if (block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_ON_NB))
          fn->addFlag(FLAG_NON_BLOCKING);
        ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "_dummy_locale_arg", dtInt[INT_SIZE_32]);
        fn->insertFormalAtTail(arg);
      }
      if (fn) {
        nestedFunctions.add(fn);
        CallExpr* call = new CallExpr(fn);
        if (block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_ON) ||
            block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_ON_NB))
          call->insertAtTail(block->blockInfo->get(1)->remove());
        block->insertBefore(new DefExpr(fn));
        block->insertBefore(call);
        block->blockInfo->remove();
        fn->insertAtTail(block->remove());
        fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
        fn->retType = dtVoid;
      }
    }
  }
  flattenNestedFunctions(nestedFunctions);

  compute_call_sites();

  optimizeReadOnlyReferenceArguments(nestedFunctions);

  makeHeapAllocations();

  Vec<FnSymbol*> queue;
  Map<FnSymbol*,Symbol*> endCountMap;

  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIMITIVE_GET_END_COUNT)) {
      FnSymbol* pfn = call->getFunction();
      if (!endCountMap.get(pfn))
        insertEndCount(pfn, call->typeInfo(), queue, endCountMap);
      call->replace(new SymExpr(endCountMap.get(pfn)));
    } else if (call->isPrimitive(PRIMITIVE_SET_END_COUNT)) {
      FnSymbol* pfn = call->getFunction();
      if (!endCountMap.get(pfn))
        insertEndCount(pfn, call->get(1)->typeInfo(), queue, endCountMap);
      call->replace(new CallExpr(PRIMITIVE_MOVE, endCountMap.get(pfn), call->get(1)->remove()));
    }
  }

  forv_Vec(FnSymbol, fn, queue) {
    forv_Vec(CallExpr, call, *fn->calledBy) {
      Type* endCountType = endCountMap.get(fn)->type;
      FnSymbol* pfn = call->getFunction();
      if (!endCountMap.get(pfn))
        insertEndCount(pfn, endCountType, queue, endCountMap);
      call->insertAtTail(endCountMap.get(pfn));
    }
  }

  forv_Vec(FnSymbol, fn, nestedFunctions) {
    forv_Vec(CallExpr, call, *fn->calledBy) {
      bundleArgs(call);
    }
  }
}


static void
buildWideClass(Type* type) {
  ClassType* wide = new ClassType(CLASS_RECORD);
  TypeSymbol* wts = new TypeSymbol(astr("__wide_", type->symbol->cname), wide);
  wts->addFlag(FLAG_WIDE_CLASS);
  theProgram->block->insertAtTail(new DefExpr(wts));
  wide->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtInt[INT_SIZE_32])));
  wide->fields.insertAtTail(new DefExpr(new VarSymbol("addr", type)));
  //
  // Strings need an extra field in their wide class to hold their length
  //
  if (type == dtString)
    wide->fields.insertAtTail(new DefExpr(new VarSymbol("size", dtInt[INT_SIZE_32])));
  wideClassMap.put(type, wide);

  //
  // build reference type for wide class type
  //
  ClassType* ref = new ClassType(CLASS_CLASS);
  TypeSymbol* rts = new TypeSymbol(astr("__ref_wide_", type->symbol->cname), ref);
  rts->addFlag(FLAG_REF);
  theProgram->block->insertAtTail(new DefExpr(rts));
  ref->fields.insertAtTail(new DefExpr(new VarSymbol("_val", type)));
  wide->refType = ref;
}


//
// The argument expr is a use of a wide reference. Insert a check to ensure
// that it is on the current locale, then drop its wideness by moving the
// addr field into a non-wide of otherwise the same type. Then, replace its
// use with the non-wide version.
//
static void insertLocalTemp(Expr* expr) {
  SymExpr* se = toSymExpr(expr);
  Expr* stmt = expr->getStmtExpr();
  INT_ASSERT(se && stmt);
  SET_LINENO(se);
  VarSymbol* var = newTemp(astr("local_", se->var->name),
                           se->var->type->getField("addr")->type);
  if (!fNoLocalChecks) {
    stmt->insertBefore(new CallExpr(PRIMITIVE_LOCAL_CHECK, se->copy()));
  }
  stmt->insertBefore(new DefExpr(var));
  stmt->insertBefore(new CallExpr(PRIMITIVE_LOCAL_DEREF, se->copy(), var));
  se->replace(new SymExpr(var));
}


//
// If call has the potential to cause communication, assert that the wide
// reference that might cause communication is local and remove its wide-ness
//
// The organization of this function follows the order of CallExpr::codegen()
// leaving out primitives that don't communicate.
//
static void localizeCall(CallExpr* call) {
  if (call->primitive) {
    switch (call->primitive->tag) {
    case PRIMITIVE_ARRAY_SET: /* Fallthru */
    case PRIMITIVE_ARRAY_SET_FIRST:
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIMITIVE_MOVE:
      if (CallExpr* rhs = toCallExpr(call->get(2))) {
        if (rhs->isPrimitive(PRIMITIVE_GET_LOCALEID)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
            if (rhs->get(1)->typeInfo()->getValueType()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
              insertLocalTemp(rhs->get(1));
            }
          }
        } else if (rhs->isPrimitive(PRIMITIVE_GET_REF)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
              rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            insertLocalTemp(rhs->get(1));
            if (!rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
              INT_ASSERT(rhs->get(1)->typeInfo() == dtString);
              // special handling for wide strings
              rhs->replace(rhs->get(1)->remove());
            }
          }
        } else if (rhs->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
              rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            SymExpr* sym = toSymExpr(rhs->get(2));
            INT_ASSERT(sym);
            if (!sym->var->hasFlag(FLAG_SUPER_CLASS)) {
              insertLocalTemp(rhs->get(1));
            }
          }
        } else if (rhs->isPrimitive(PRIMITIVE_ARRAY_GET) ||
                   rhs->isPrimitive(PRIMITIVE_ARRAY_GET_VALUE)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            SymExpr* lhs = toSymExpr(call->get(1));
            Expr* stmt = call->getStmtExpr();
            INT_ASSERT(lhs && stmt);
            insertLocalTemp(rhs->get(1));
            VarSymbol* localVar = newTemp(astr("local_", lhs->var->name),
                                          lhs->var->type->getField("addr")->type);
            stmt->insertBefore(new DefExpr(localVar));
            lhs->replace(new SymExpr(localVar));
            stmt->insertAfter(new CallExpr(PRIMITIVE_MOVE, lhs,
                                           new SymExpr(localVar)));
          }
        } else if (rhs->isPrimitive(PRIMITIVE_UNION_GETID)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
            insertLocalTemp(rhs->get(1));
          }
        } else if (rhs->isPrimitive(PRIMITIVE_GETCID)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            insertLocalTemp(rhs->get(1));
          }
        }
      } else if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
                 !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
                 !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIMITIVE_DYNAMIC_CAST:
      if (call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        insertLocalTemp(call->get(2));
        if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) ||
            call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
          toSymExpr(call->get(1))->var->type = call->get(1)->typeInfo()->getField("addr")->type;
        }
      }
    break;
    case PRIMITIVE_SETCID:
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIMITIVE_UNION_SETID:
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIMITIVE_SET_MEMBER:
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) ||
          call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
        insertLocalTemp(call->get(1));
      }
      break;
    default:
      break;
    }
  }
}


//
// Do a breadth first search starting from functions generated for local blocks
// for all function calls in each level of the search, if they directly cause
// communication, add a local temp that isn't wide. If it is a resolved call,
// meaning that it isn't a primitive or external function, clone it and add it
// to the queue of functions to handle at the next iteration of the BFS.
//
static void handleLocalBlocks() {
  Map<FnSymbol*,FnSymbol*> cache; // cache of localized functions
  Vec<BlockStmt*> queue; // queue of blocks to localize

  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (block->parentSymbol)
      if (block->blockInfo)
        if (block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_LOCAL))
          queue.add(block);
  }

  forv_Vec(BlockStmt, block, queue) {
    Vec<BaseAST*> asts;
    collect_asts(block, asts);
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = toCallExpr(ast)) {
        localizeCall(call);
        if (FnSymbol* fn = call->isResolved()) {
          if (FnSymbol* alreadyLocal = cache.get(fn)) {
            call->baseExpr->replace(new SymExpr(alreadyLocal));
          } else {
            FnSymbol* local = fn->copy();
            local->name = astr("_local_", fn->name);
            local->cname = astr("_local_", fn->cname);
            fn->defPoint->insertBefore(new DefExpr(local));
            call->baseExpr->replace(new SymExpr(local));
            queue.add(local->body);
            cache.put(fn, local);
            if (local->retType->symbol->hasFlag(FLAG_WIDE) ||
                local->retType->symbol->hasFlag(FLAG_WIDE_CLASS)) {
              CallExpr* ret = toCallExpr(local->body->body.tail);
              INT_ASSERT(ret && ret->isPrimitive(PRIMITIVE_RETURN));
              Type* narrowType = local->retType->getField("addr")->type;
              local->retType = narrowType;
              VarSymbol* tmp = newTemp(narrowType);
              ret->insertBefore(new DefExpr(tmp));
              if (!fNoLocalChecks)
                ret->insertBefore(new CallExpr(PRIMITIVE_LOCAL_CHECK, ret->get(1)->copy()));
              ret->insertBefore(new CallExpr(PRIMITIVE_LOCAL_DEREF, ret->get(1)->copy(), tmp));
              ret->get(1)->replace(new SymExpr(tmp));
            }
          }
        }
      }
    }
  }
}


static void
narrowReferences() {
  Vec<Symbol*> wideSet;
  forv_Vec(DefExpr, def, gDefExprs) {
    if (isVarSymbol(def->sym) && isFnSymbol(def->parentSymbol)) {
      if (def->sym->type->symbol->hasFlag(FLAG_WIDE)) {
        wideSet.set_add(def->sym);
      }
    }
  }

  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(wideSet, defMap, useMap);

  forv_Vec(Symbol, sym, wideSet) if (sym) {
    bool narrow = true;

    for_defs(def, defMap, sym) {
      bool narrowDef = false;
      if (CallExpr* move = toCallExpr(def->parentExpr)) {
        if (move->isPrimitive(PRIMITIVE_MOVE)) {
          if (CallExpr* rhs = toCallExpr(move->get(2))) {
            if (FnSymbol* fn = rhs->isResolved())
              if (fn->retType->symbol->hasFlag(FLAG_REF))
                narrowDef = true;
          } else if (SymExpr* rhs = toSymExpr(move->get(2))) {
            if (!rhs->var->type->symbol->hasFlag(FLAG_REF) &&
                !rhs->var->type->symbol->hasFlag(FLAG_WIDE))
              narrowDef = true;
          }
        }
      }
      if (!narrowDef)
        narrow = false;
    }

    for_uses(use, useMap, sym) {
      bool narrowUse = false;
      if (CallExpr* call = toCallExpr(use->parentExpr)) {
        if (call->isPrimitive(PRIMITIVE_GET_REF))
          narrowUse = true;
      }
      if (!narrowUse)
        narrow = false;
    }

    if (narrow) {
      sym->type = sym->type->getField("addr")->type;
    }
  }

  freeDefUseMaps(defMap, useMap);
}


//
// change all classes into wide classes
// change all references into wide references
//
void
insertWideReferences(void) {
  if (fRuntime)
    return;

  FnSymbol* heapAllocateGlobals = new FnSymbol("_heapAllocateGlobals");
  heapAllocateGlobals->retType = dtVoid;
  theProgram->block->insertAtTail(new DefExpr(heapAllocateGlobals));
  heapAllocateGlobals->insertAtHead(new CallExpr(PRIMITIVE_ALLOC_GVR));

  if (fLocal) {
    heapAllocateGlobals->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
    return;
  }

  Vec<Symbol*> heapVars;
  forv_Vec(DefExpr, def, gDefExprs) {
    if (!fLocal &&
        isModuleSymbol(def->parentSymbol) &&
        def->parentSymbol != rootModule &&
        isVarSymbol(def->sym) &&
        def->sym->type->symbol->hasFlag(FLAG_HEAP)) {
      heapVars.add(def->sym);
    }
  }

  //
  // change dtNil into dtObject
  //
  forv_Vec(DefExpr, def, gDefExprs) {
    if (FnSymbol* fn = toFnSymbol(def->sym)) {
      if (fn->retType == dtNil)
        fn->retType = dtObject;
    } else if (!isTypeSymbol(def->sym)) {
      if (def->sym != gNil && def->sym->type == dtNil)
        def->sym->type = dtObject;
    }
  }

  wideClassMap.clear();

  //
  // build wide class type for every class type
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    ClassType* ct = toClassType(ts->type);
    if (ct && ct->classTag == CLASS_CLASS && !ts->hasFlag(FLAG_REF) && !ts->hasFlag(FLAG_NO_WIDE_CLASS)) {
      buildWideClass(ct);
    }
  }
  buildWideClass(dtString);
  //
  // change all classes into wide classes
  //
  forv_Vec(DefExpr, def, gDefExprs) {
    //
    // do not widen literals or nil reference
    //
    if (VarSymbol* var = toVarSymbol(def->sym))
      if (var->immediate)
        continue;

    //
    // do not change class field in wide class type
    //
    if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol))
      if (ts->hasFlag(FLAG_WIDE_CLASS))
        continue;

    //
    // do not change super class field - it's really a record
    //
    if (def->sym->hasFlag(FLAG_SUPER_CLASS))
      continue;

    if (FnSymbol* fn = toFnSymbol(def->sym)) {
      if (Type* wide = wideClassMap.get(fn->retType))
        if (!fn->hasFlag(FLAG_EXTERN))
          fn->retType = wide;
    } else if (!isTypeSymbol(def->sym)) {
      if (Type* wide = wideClassMap.get(def->sym->type)) {
        if (def->parentSymbol->hasFlag(FLAG_EXTERN)) {
          if (toArgSymbol(def->sym))
            continue; // don't change extern function's arguments
        }
        def->sym->type = wide;
      }
    }
  }

  //
  // change arrays of classes into arrays of wide classes
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->hasFlag(FLAG_DATA_CLASS)) {
      if (Type* nt = wideClassMap.get(toTypeSymbol(ts->type->substitutions.v[0].value)->type)) {
        ts->type->substitutions.v[0].value = nt->symbol;
      }
    }
  }

  wideRefMap.clear();

  //
  // build wide reference type for every reference type
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->hasFlag(FLAG_REF)) {
      ClassType* wide = new ClassType(CLASS_RECORD);
      TypeSymbol* wts = new TypeSymbol(astr("__wide_", ts->cname), wide);
      wts->addFlag(FLAG_WIDE);
      theProgram->block->insertAtTail(new DefExpr(wts));
      wide->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtInt[INT_SIZE_32])));
      wide->fields.insertAtTail(new DefExpr(new VarSymbol("addr", ts->type)));
      wideRefMap.put(ts->type, wide);
    }
  }

  //
  // change all references into wide references
  //
  forv_Vec(DefExpr, def, gDefExprs) {
    //
    // do not widen reference nil
    //
    if (def->sym == gNilRef)
      continue;

    //
    // do not change reference field in wide reference type
    //
    if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol))
      if (ts->hasFlag(FLAG_WIDE))
        continue;

    //
    // do not change super field - it's really a record
    //
    if (def->sym->hasFlag(FLAG_SUPER_CLASS))
      continue;

    if (FnSymbol* fn = toFnSymbol(def->sym)) {
      if (Type* wide = wideRefMap.get(fn->retType))
        fn->retType = wide;
    } else if (!isTypeSymbol(def->sym)) {
      if (Type* wide = wideRefMap.get(def->sym->type))
        def->sym->type = wide;
    }
  }

  //
  // Special case string literals passed to functions, set member primitives
  // and array element initializers by pushing them into temps first.
  //
  forv_Vec(SymExpr, se, gSymExprs) {
    if (se->var->type == dtString) {
      if (VarSymbol* var = toVarSymbol(se->var)) {
        if (var->immediate) {
          if (CallExpr* call = toCallExpr(se->parentExpr)) {
            if (call->isResolved() && !call->isResolved()->hasFlag(FLAG_EXTERN)) {
              if (Type* type = actual_to_formal(se)->typeInfo()) {
                VarSymbol* tmp = newTemp(type);
                call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                se->replace(new SymExpr(tmp));
                call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, se));
              }
            } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
              if (SymExpr* wide = toSymExpr(call->get(2))) {
                Type* type = wide->var->type;
                VarSymbol* tmp = newTemp(type);
                call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                se->replace(new SymExpr(tmp));
                call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, se));
              }
            } else if (call->isPrimitive(PRIMITIVE_ARRAY_SET_FIRST)) {
              if (SymExpr* wide = toSymExpr(call->get(3))) {
                Type* type = wide->var->type;
                VarSymbol* tmp = newTemp(wideClassMap.get(type));
                call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                se->replace(new SymExpr(tmp));
                call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, se));
              }
            }
          }
        }
      }
    }
  }

  //
  // Turn calls to extern functions involving wide classes into moves
  // of the wide class into a non-wide type and then use that in the call.
  // After the call, copy the value back into the wide class.
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isResolved() && call->isResolved()->hasFlag(FLAG_EXTERN)) {
      for_alist(arg, call->argList) {
        SymExpr* sym = toSymExpr(arg);
        INT_ASSERT(sym);
        if (sym->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) ||
            sym->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
          VarSymbol* var = newTemp(sym->typeInfo()->getField("addr")->type);
          call->getStmtExpr()->insertBefore(new DefExpr(var));
          if (var->type->symbol->hasFlag(FLAG_REF))
            call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, var, sym->copy()));
          else
            call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, var, new CallExpr(PRIMITIVE_GET_REF, sym->copy())));
          call->getStmtExpr()->insertAfter(new CallExpr(PRIMITIVE_MOVE, sym->copy(), var));
          sym->replace(new SymExpr(var));
        }
      }
    }
  }

  //
  // insert wide class temps for nil
  //
  forv_Vec(SymExpr, se, gSymExprs) {
    if (se->var == gNil || se->var == gNilRef) {
      if (CallExpr* call = toCallExpr(se->parentExpr)) {
        if (call->isResolved()) {
          if (Type* type = actual_to_formal(se)->typeInfo()) {
            if (type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
              VarSymbol* tmp = newTemp(type);
              call->getStmtExpr()->insertBefore(new DefExpr(tmp));
              se->replace(new SymExpr(tmp));
              call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, se));
            }
          }
        } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
          if (Type* wtype = call->get(1)->typeInfo()) {
            if (wtype->symbol->hasFlag(FLAG_WIDE)) {
              if (Type* wctype = wtype->getField("addr")->type->getField("_val")->type) {
                if (wctype->symbol->hasFlag(FLAG_WIDE_CLASS)) {
                  VarSymbol* tmp = newTemp(wctype);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  se->replace(new SymExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, se));
                }
              }
            }
          }
        } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
          if (Type* wctype = call->get(2)->typeInfo()) {
            if (wctype->symbol->hasFlag(FLAG_WIDE_CLASS) ||
                wctype->symbol->hasFlag(FLAG_WIDE)) {
              VarSymbol* tmp = newTemp(wctype);
              call->getStmtExpr()->insertBefore(new DefExpr(tmp));
              se->replace(new SymExpr(tmp));
              call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, se));
            }
          }
        }
      }
    }
  }

  //
  // insert cast temps if lhs type does not match cast type
  //   allows separation of the remote put with the wide cast
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIMITIVE_CAST)) {
      if (CallExpr* move = toCallExpr(call->parentExpr)) {
        if (move->isPrimitive(PRIMITIVE_MOVE)) {
          if (move->get(1)->typeInfo() != call->typeInfo()) {
            VarSymbol* tmp = newTemp(call->typeInfo());
            move->insertBefore(new DefExpr(tmp));
            call->replace(new SymExpr(tmp));
            move->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, call));
          }
        }
      }
    }
  }

  //
  // initialize global variables across locales
  //
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap); // for "ack!!" below

  forv_Vec(CallExpr, call, gCallExprs) {
    //
    // if this is a call to _heapAllocGlobal or
    // _heapAllocConstGlobal (or a wrapper thereof) but not from
    // within a wrapper, then this is a global variable
    //
    if ((call->isNamed("_heapAllocGlobal") ||
         call->isNamed("_heapAllocConstGlobal")) &&
        !call->parentSymbol->hasFlag(FLAG_WRAPPER)) {

      CallExpr* move = toCallExpr(call->parentExpr);
      INT_ASSERT(move);
      SymExpr* lhs = toSymExpr(move->get(1));
      INT_ASSERT(lhs);

      //
      // ack!! check for tmp before global
      // this can happen due to coercion wrappers
      // this is a worrisome fix
      //
      if (lhs->var->hasFlag(FLAG_TEMP)) {
        if (CallExpr* move2 = toCallExpr(move->next)) {
          Vec<SymExpr*>* defs = defMap.get(lhs->var);
          Vec<SymExpr*>* uses = useMap.get(lhs->var);
          if (defs && defs->n == 1 &&
              uses && uses->n == 1 &&
              uses->v[0] == move2->get(2)) {
            move2->get(2)->replace(call->remove());
            move->remove();
            move = move2;
            lhs->var->defPoint->remove();
            lhs = toSymExpr(move->get(1));
          }
        }
      }

      if (lhs->var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {

        //
        // handle global variables on the heap
        //
        move->replace(new CallExpr(PRIMITIVE_SET_MEMBER, lhs->remove(), lhs->var->type->getField("addr")->type->getField("_val"), call->get(1)->remove()));
        heapVars.add(lhs->var);

      } else {

        //
        // handle constant global variables not on the heap
        //
        call->replace(call->get(1)->remove());
        move->insertAfter(new CallExpr(PRIMITIVE_PRIVATE_BROADCAST, lhs->var));
      }
    }
  }

  freeDefUseMaps(defMap, useMap);

  //
  // dereference wide string actual argument to primitive
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->primitive) {
      if (call->primitive->tag == PRIMITIVE_UNKNOWN ||
          call->isPrimitive(PRIMITIVE_CAST)) {
        for_actuals(actual, call) {
          if (actual->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            if (actual->typeInfo()->getField("addr")->typeInfo() == dtString) {
              VarSymbol* tmp = newTemp(actual->typeInfo()->getField("addr")->typeInfo());
              call->getStmtExpr()->insertBefore(new DefExpr(tmp));
              call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_REF, actual->copy())));
              actual->replace(new SymExpr(tmp));
            }
          }
        }
      }
    }
  }

  //
  // dereference wide references to wide classes in select primitives;
  // this simplifies the implementation of these primitives
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
        call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE) ||
        call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
          call->get(1)->typeInfo()->getValueType()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        VarSymbol* tmp = newTemp(call->get(1)->typeInfo()->getValueType());
        call->getStmtExpr()->insertBefore(new DefExpr(tmp));
        call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_REF, call->get(1)->remove())));
        call->insertAtHead(tmp);
      }
    }
  }

  CallExpr* localeID = new CallExpr(PRIMITIVE_LOCALE_ID);
  VarSymbol* tmp = newTemp(localeID->typeInfo());
  VarSymbol* tmpBool = newTemp(dtBool);

  heapAllocateGlobals->insertAtTail(new DefExpr(tmp));
  heapAllocateGlobals->insertAtTail(new DefExpr(tmpBool));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, localeID));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmpBool, new CallExpr(PRIMITIVE_EQUAL, tmp, new_IntSymbol(0))));
  BlockStmt* block = new BlockStmt();
  forv_Vec(Symbol, sym, heapVars) {
    block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, sym, new CallExpr(PRIMITIVE_CHPL_ALLOC, sym->type->getField("addr")->type->symbol, new_StringSymbol("global var heap allocation"))));
  }
  heapAllocateGlobals->insertAtTail(new CondStmt(new SymExpr(tmpBool), block));
  int i = 0;
  forv_Vec(Symbol, sym, heapVars) {
    heapAllocateGlobals->insertAtTail(new CallExpr(PRIMITIVE_HEAP_REGISTER_GLOBAL_VAR, new_IntSymbol(i++), sym));
  }
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIMITIVE_HEAP_BROADCAST_GLOBAL_VARS, new_IntSymbol(i)));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
  numGlobalsOnHeap = i;

  handleLocalBlocks();
  narrowReferences();
}
