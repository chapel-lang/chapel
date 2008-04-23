//
// Transformations for begin, cobegin, and on statements
//

#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
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
  currentLineno = fcall->lineno;
  currentFilename = fcall->filename;
  ModuleSymbol* mod = fcall->getModule();
  FnSymbol* fn = fcall->isResolved();

  // create a new class to capture refs to locals
  ClassType* ctype = new ClassType( CLASS_CLASS);
  TypeSymbol* new_c = new TypeSymbol( astr("_class_locals", 
                                           fn->name),
                                      ctype);
  new_c->addPragma("no object");
  new_c->addPragma("no wide class");

  // add the function args as fields in the class
  int i = 1;
  bool first = true;
  for_actuals(arg, fcall) {
    if ((fn->hasPragma("on") || fn->hasPragma("cobegin")) && first) {
      first = false;
      continue;
    }
    SymExpr *s = toSymExpr(arg);
    Symbol  *var = s->var; // arg or var
    var->isConcurrent = true;
    VarSymbol* field = new VarSymbol(astr("_", istr(i), "_", var->name), var->type);
    ctype->fields.insertAtTail(new DefExpr(field));
    i++;
  }
  mod->block->insertAtHead(new DefExpr(new_c));
    
  // create the class variable instance and allocate it
  VarSymbol *tempc = new VarSymbol( astr( "_args_for", 
                                          fn->name),
                                    ctype);
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
    if ((fn->hasPragma("on") || fn->hasPragma("cobegin")) && first) {
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
  if (fn->hasPragma("on")) {
    wrap_fn->addPragma("on block");
    ArgSymbol* locale = new ArgSymbol(INTENT_BLANK, "_dummy_locale_arg", dtInt[INT_SIZE_32]);
    wrap_fn->insertFormalAtTail(locale);
  } else if (fn->hasPragma("cobegin")) {
    wrap_fn->addPragma("cobegin block");
    ArgSymbol* task_list = new ArgSymbol(INTENT_BLANK, "_task_list", dtTaskList);
    wrap_fn->insertFormalAtTail(task_list);
    FnSymbol* fns = toFnSymbol(fcall_def->sym);
    fns->getFormal(1)->defPoint->remove(); // remove bogus first formal
  }
  ArgSymbol *wrap_c = new ArgSymbol( INTENT_BLANK, "c", ctype);
  wrap_fn->insertFormalAtTail( wrap_c);

  mod->block->insertAtTail(new DefExpr(wrap_fn));
  if (fn->hasPragma("on") || fn->hasPragma("cobegin")) {
    fcall->insertBefore(new CallExpr(wrap_fn, fcall->get(1)->remove(), tempc));
  } else
    fcall->insertBefore(new CallExpr(wrap_fn, tempc));

  if (fn->hasPragma("begin") || fn->hasPragma("cobegin")) {
    if (fn->hasPragma("begin"))
      wrap_fn->addPragma("begin block");
    wrap_fn->insertAtHead(new CallExpr(PRIMITIVE_THREAD_INIT));
  }
    
  // translate the original cobegin function
  CallExpr *new_cofn = new CallExpr( (toSymExpr(fcall->baseExpr))->var);
  if (fn->hasPragma("on")/* || fn->hasPragma("cobegin")*/)
    new_cofn->insertAtTail(new_IntSymbol(0)); // bogus actual
  for_fields(field, ctype) {  // insert args

    VarSymbol* tmp = new VarSymbol("_tmp", field->type);
    tmp->isCompilerTemp = true;
    wrap_fn->insertAtTail(new DefExpr(tmp));
    wrap_fn->insertAtTail(
      new CallExpr(PRIMITIVE_MOVE, tmp,
        new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, wrap_c, field)));
    new_cofn->insertAtTail(tmp);
  }

  wrap_fn->retType = dtVoid;
  wrap_fn->insertAtTail(new_cofn);     // add new call
  if (fn->hasPragma("on"))
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
    VarSymbol* var = new VarSymbol("_endCount", endCountType);
    var->isCompilerTemp = true;
    fn->insertAtHead(new DefExpr(var));
    endCountMap.put(fn, var);
    queue.add(fn);
  } else {
    ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "_endCount", endCountType);
    fn->insertFormalAtTail(arg);
    VarSymbol* var = new VarSymbol("_endCount", endCountType);
    var->isCompilerTemp = true;
    fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, var, arg));
    fn->insertAtHead(new DefExpr(var));
    endCountMap.put(fn, var);
    queue.add(fn);
  }
}


void
parallel(void) {
  compute_call_sites();

  Vec<FnSymbol*> queue;
  Map<FnSymbol*,Symbol*> endCountMap;

  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr * call = toCallExpr(ast)) {
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

  Vec<CallExpr*> calls;
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast))
      calls.add(call);
  }

  forv_Vec(CallExpr, call, calls) {
    if (call->isResolved() && (call->isResolved()->hasPragma("on") ||
                               call->isResolved()->hasPragma("begin") ||
                               call->isResolved()->hasPragma("cobegin")))
      bundleArgs(call);
  }
}


static void
buildWideClass(Type* type) {
  ClassType* wide = new ClassType(CLASS_RECORD);
  TypeSymbol* wts = new TypeSymbol(astr("_wide_", type->symbol->cname), wide);
  wts->addPragma("wide class");
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
  TypeSymbol* rts = new TypeSymbol(astr("_ref_wide_", type->symbol->cname), ref);
  rts->addPragma("ref");
  theProgram->block->insertAtTail(new DefExpr(rts));
  ref->fields.insertAtTail(new DefExpr(new VarSymbol("_val", type)));
  wide->refType = ref;
}


//
// change all classes into wide classes
// change all references into wide references
//
void
insertWideReferences(void) {
  FnSymbol* heapAllocateGlobals = new FnSymbol("_heapAllocateGlobals");
  heapAllocateGlobals->retType = dtVoid;
  theProgram->block->insertAtTail(new DefExpr(heapAllocateGlobals));
  heapAllocateGlobals->insertAtHead(new CallExpr(PRIMITIVE_ALLOC_GVR));

  if (fLocal) {
    heapAllocateGlobals->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
    return;
  }

  //
  // change dtNil into dtObject
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        if (fn->retType == dtNil)
          fn->retType = dtObject;
      } else if (!isTypeSymbol(def->sym)) {
        if (def->sym->type == dtNil)
          def->sym->type = dtObject;
      }
    }
  }

  wideClassMap.clear();

  //
  // build wide class type for every class type
  //
  forv_Vec(TypeSymbol, ts, gTypes) {
    ClassType* ct = toClassType(ts->type);
    if (ct && ct->classTag == CLASS_CLASS && !ts->hasPragma("ref") && !ts->hasPragma("no wide class")) {
      buildWideClass(ct);
    }
  }
  buildWideClass(dtString);
  //
  // change all classes into wide classes
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = toDefExpr(ast)) {

      //
      // do not change class field in wide class type
      //
      if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol))
        if (ts->hasPragma("wide class"))
          continue;
      //
      // do not change super class field - it's really a record
      //
      if (def->sym->hasPragma("super class"))
        continue;

      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        if (Type* wide = wideClassMap.get(fn->retType))
          if (!fn->isExtern)
            fn->retType = wide;
      } else if (!isTypeSymbol(def->sym)) {
        if (Type* wide = wideClassMap.get(def->sym->type)) {
          if (def->parentSymbol->isExtern) {
            if (toArgSymbol(def->sym))
              continue; // don't change extern function's arguments
          }
          def->sym->type = wide;
        }
      }
    }
  }

  //
  // change arrays of classes into arrays of wide classes
  //
  forv_Vec(TypeSymbol, ts, gTypes) {
    if (ts->hasPragma("data class")) {
      if (Type* nt = wideClassMap.get(toType(ts->type->substitutions.v[0].value))) {
        ts->type->substitutions.v[0].value = nt;
      }
    }
  }

  wideRefMap.clear();

  //
  // build wide reference type for every reference type
  //
  forv_Vec(TypeSymbol, ts, gTypes) {
    if (ts->hasPragma("ref")) {
      ClassType* wide = new ClassType(CLASS_RECORD);
      TypeSymbol* wts = new TypeSymbol(astr("_wide_", ts->cname), wide);
      wts->addPragma("wide");
      theProgram->block->insertAtTail(new DefExpr(wts));
      wide->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtInt[INT_SIZE_32])));
      wide->fields.insertAtTail(new DefExpr(new VarSymbol("addr", ts->type)));
      wideRefMap.put(ts->type, wide);
    }
  }

  //
  // change all references into wide references
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = toDefExpr(ast)) {

      //
      // do not change reference field in wide reference type
      //
      if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol))
        if (ts->hasPragma("wide"))
          continue;
      //
      // do not change super field - it's really a record
      //
      if (def->sym->hasPragma("super class"))
        continue;

      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        if (Type* wide = wideRefMap.get(fn->retType))
          fn->retType = wide;
      } else if (!isTypeSymbol(def->sym)) {
        if (Type* wide = wideRefMap.get(def->sym->type))
          def->sym->type = wide;
      }
    }
  }

  //
  // Special case string literals passed to functions, set member primitives
  // and array element initializers by pushing them into temps first.
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (SymExpr* se = toSymExpr(ast)) {
      if (se->var->type == dtString) {
        if (VarSymbol* var = toVarSymbol(se->var)) {
          if (var->immediate) {
            if (CallExpr* call = toCallExpr(se->parentExpr)) {
              if (call->isResolved() && !call->isResolved()->isExtern) {
                if (Type* type = actual_to_formal(se)->typeInfo()) {
                  VarSymbol* tmp = new VarSymbol("tmp", type);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  se->replace(new SymExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, se));
                }
              } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
                if (SymExpr* wide = toSymExpr(call->get(2))) {
                  Type* type = wide->var->type;
                  VarSymbol* tmp = new VarSymbol("tmp", type);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  se->replace(new SymExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, se));
                }
              } else if (call->isPrimitive(PRIMITIVE_ARRAY_SET_FIRST)) {
                if (SymExpr* wide = toSymExpr(call->get(3))) {
                  Type* type = wide->var->type;
                  VarSymbol* tmp = new VarSymbol("tmp", wideClassMap.get(type));
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
  }


  //
  // Turn calls to extern functions involving wide classes into moves
  // of the wide class into a non-wide type and then use that in the call.
  // After the call, copy the value back into the wide class.
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isResolved() && call->isResolved()->isExtern) {
        for_alist(arg, call->argList) {
          SymExpr* sym = toSymExpr(arg);
          INT_ASSERT(sym);
          if (sym->typeInfo()->symbol->hasPragma("wide class") ||
              sym->typeInfo()->symbol->hasPragma("wide")) {
            VarSymbol* var = new VarSymbol("_tmp", sym->typeInfo()->getField("addr")->type);
            call->getStmtExpr()->insertBefore(new DefExpr(var));
            if (var->type->symbol->hasPragma("ref"))
              call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, var, sym->copy()));
            else
              call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, var, new CallExpr(PRIMITIVE_GET_REF, sym->copy())));
            call->getStmtExpr()->insertAfter(new CallExpr(PRIMITIVE_MOVE, sym->copy(), var));
            sym->replace(new SymExpr(var));
          }
        }
      }
    }
  }



  //
  // insert wide class temps for nil
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (SymExpr* se = toSymExpr(ast)) {
      if (se->var == gNil) {
        if (CallExpr* call = toCallExpr(se->parentExpr)) {
          if (call->isResolved()) {
            if (Type* type = actual_to_formal(se)->typeInfo()) {
              if (type->symbol->hasPragma("wide class")) {
                VarSymbol* tmp = new VarSymbol("_tmp", type);
                call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                se->replace(new SymExpr(tmp));
                call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, se));
              }
            }
          } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
            if (Type* wtype = call->get(1)->typeInfo()) {
              if (wtype->symbol->hasPragma("wide")) {
                if (Type* wctype = wtype->getField("addr")->type->getField("_val")->type) {
                  if (wctype->symbol->hasPragma("wide class")) {
                    VarSymbol* tmp = new VarSymbol("_tmp", wctype);
                    call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                    se->replace(new SymExpr(tmp));
                    call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, se));
                  }
                }
              }
            }
          } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
            if (Type* wctype = call->get(2)->typeInfo()) {
              if (wctype->symbol->hasPragma("wide class")) {
                VarSymbol* tmp = new VarSymbol("_tmp", wctype);
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
  // insert cast temps if lhs type does not match cast type
  //   allows separation of the remote put with the wide cast
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_CAST)) {
        if (CallExpr* move = toCallExpr(call->parentExpr)) {
          if (move->isPrimitive(PRIMITIVE_MOVE)) {
            if (move->get(1)->typeInfo() != call->typeInfo()) {
              VarSymbol* tmp = new VarSymbol("_tmp", call->typeInfo());
              move->insertBefore(new DefExpr(tmp));
              call->replace(new SymExpr(tmp));
              move->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, call));
            }
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

  Vec<Symbol*> heapVars;
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isNamed("_heapAllocGlobal") ||
          call->isNamed("_heapAllocConstGlobal")) {
        CallExpr* move = toCallExpr(call->parentExpr);
        INT_ASSERT(move);
        SymExpr* lhs = toSymExpr(move->get(1));
        INT_ASSERT(lhs);

        //
        // ack!! check for tmp before global
        // this can happen due to coercion wrappers
        // this is a worrisome fix
        //
        if (lhs->var->isCompilerTemp) {
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

        if (lhs->var->type->symbol->hasPragma("wide class")) {

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
  }

  freeDefUseMaps(defMap, useMap);

  //
  // dereference wide string actual argument to primitive
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->primitive) {
        if (call->primitive->tag == PRIMITIVE_UNKNOWN ||
            call->isPrimitive(PRIMITIVE_CAST)) {
          for_actuals(actual, call) {
            if (actual->typeInfo()->symbol->hasPragma("wide class")) {
              if (actual->typeInfo()->getField("addr")->typeInfo() == dtString) {
                VarSymbol* tmp = new VarSymbol("tmp", actual->typeInfo()->getField("addr")->typeInfo());
                call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_REF, actual->copy())));
                actual->replace(new SymExpr(tmp));
              }
            }
          }
        }
      }
    }
  }

  VarSymbol* tmp = new VarSymbol("_tmp", dtBool);
  tmp->isCompilerTemp = true;
  heapAllocateGlobals->insertAtTail(new DefExpr(tmp));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_LOCALE_ID)));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_EQUAL, tmp, new_IntSymbol(0))));
  BlockStmt* block = new BlockStmt();
  forv_Vec(Symbol, sym, heapVars) {
    block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, sym, new CallExpr(PRIMITIVE_CHPL_ALLOC, sym->type->getField("addr")->type->symbol, new_StringSymbol("global var heap allocation"))));
    block->insertAtTail(new CallExpr(PRIMITIVE_SETCID, sym));
  }
  heapAllocateGlobals->insertAtTail(new CondStmt(new SymExpr(tmp), block));
  int i = 0;
  forv_Vec(Symbol, sym, heapVars) {
    heapAllocateGlobals->insertAtTail(new CallExpr(PRIMITIVE_HEAP_REGISTER_GLOBAL_VAR, new_IntSymbol(i++), sym));
  }
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIMITIVE_HEAP_BROADCAST_GLOBAL_VARS, new_IntSymbol(i)));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
  numGlobalsOnHeap = i;
}
