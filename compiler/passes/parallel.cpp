//
// Transformations for begin, cobegin, and on statements
//

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "../resolution/resolution.h"
#include "stmt.h"
#include "symbol.h"
#include "stringutil.h"
#include "driver.h"
#include "files.h"

static void insertEndCounts();
static void passArgsToNestedFns(Vec<FnSymbol*>& nestedFunctions);
static void create_block_fn_wrapper(CallExpr* fcall, ClassType* ctype, VarSymbol* tempc);
static void findBlockRefActuals(Vec<Symbol*>& refSet, Vec<Symbol*>& refVec);
static void findHeapVarsAndRefs(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                Vec<Symbol*>& refSet, Vec<Symbol*>& refVec,
                                Vec<Symbol*>& varSet, Vec<Symbol*>& varVec);
static void convertNilToObject();
static void buildWideClasses();
static void widenClasses();
static void buildWideRefMap();
static void widenRefs();
static void insertElementAccessTemps();
static void narrowWideClassesThroughCalls();
static void insertWideClassTempsForNil();
static void insertWideCastTemps();
static void derefWideStringActuals();
static void derefWideRefsToWideClasses();
static void widenGetPrivClass();
static void moveAddressSourcesToTemp(void);
static CallExpr* findLocaleLookup(CallExpr* onBlockCall, Map<Symbol*, Vec<SymExpr*>*>& defMap);
static CallExpr* optimizeOnCall(CallExpr* call);

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
  TypeSymbol* new_c = new TypeSymbol(astr("_class_locals", fn->name), ctype);
  new_c->addFlag(FLAG_NO_OBJECT);
  new_c->addFlag(FLAG_NO_WIDE_CLASS);

  // add the function args as fields in the class
  int i = 0;    // Fields are numbered for uniqueness.
  for_actuals(arg, fcall) {
    SymExpr *s = toSymExpr(arg);
    Symbol  *var = s->var; // arg or var
    var->addFlag(FLAG_CONCURRENTLY_ACCESSED);
    VarSymbol* field = new VarSymbol(astr("_", istr(i), "_", var->name), var->type);
    ctype->fields.insertAtTail(new DefExpr(field));
    i++;
  }
  mod->block->insertAtHead(new DefExpr(new_c));

  // create the class variable instance and allocate space for it
  VarSymbol *tempc = newTemp(astr("_args_for", fn->name), ctype);
  fcall->insertBefore( new DefExpr( tempc));
  CallExpr* tempc_alloc = heapAllocate(ctype);
  fcall->insertBefore(new CallExpr(PRIM_MOVE, tempc, tempc_alloc));
  
  // set the references in the class instance
  i = 1;
  for_actuals(arg, fcall) {
    SymExpr *s = toSymExpr(arg);
    Symbol  *var = s->var; // var or arg
    CallExpr *setc=new CallExpr(PRIM_SET_MEMBER,
                                tempc,
                                ctype->getField(i),
                                var);
    fcall->insertBefore( setc);
    i++;
  }

  // insert autoCopy for array/domain/distribution before begin call
  // and insert autoDestroy at end of begin function (i.e., reference count)
  if (fn->hasFlag(FLAG_BEGIN)) {
    for_actuals(arg, fcall) {
      SymExpr* s = toSymExpr(arg);
      Symbol* var = s->var; // var or arg
      Type* baseType = arg->typeInfo();
      if (isReferenceType(baseType)) {
        baseType = arg->typeInfo()->getField("_val", true)->type;
      }
      if (isRefCountedType(baseType)) {
        FnSymbol* autoCopyFn = getAutoCopy(baseType);
        FnSymbol* autoDestroyFn = getAutoDestroy(baseType);
        VarSymbol* valTmp = newTemp(baseType);
        fcall->insertBefore(new DefExpr(valTmp));
        if (baseType == arg->typeInfo()) {
          fcall->insertBefore(new CallExpr(PRIM_MOVE, valTmp, var));
        } else {
          fcall->insertBefore(new CallExpr(PRIM_MOVE, valTmp, new CallExpr(PRIM_DEREF, var)));
        }
        fcall->insertBefore(new CallExpr(PRIM_MOVE, valTmp, new CallExpr(autoCopyFn, valTmp)));
        VarSymbol* derefTmp = newTemp(baseType);
        fn->insertBeforeReturnAfterLabel(new DefExpr(derefTmp));
        if (baseType == arg->typeInfo()) {
          fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, derefTmp, new SymExpr(actual_to_formal(arg))));
        } else {
          fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, derefTmp, new CallExpr(PRIM_DEREF, new SymExpr(actual_to_formal(arg)))));
        }
        fn->insertBeforeReturnAfterLabel(new CallExpr(autoDestroyFn, derefTmp));
      }
    }
  }

  // create wrapper-function that uses the class instance
  create_block_fn_wrapper(fcall, ctype, tempc);
}


static void create_block_fn_wrapper(CallExpr* fcall, ClassType* ctype, VarSymbol* tempc)
{
  ModuleSymbol* mod = fcall->getModule();
  FnSymbol* fn = fcall->isResolved();
  FnSymbol *wrap_fn = new FnSymbol( astr("wrap", fn->name));
  // Add a special flag to the wrapper-function as appropriate.
  // These control aspects of code generation.
  if (fn->hasFlag(FLAG_ON))                     wrap_fn->addFlag(FLAG_ON_BLOCK);
  if (fn->hasFlag(FLAG_NON_BLOCKING))           wrap_fn->addFlag(FLAG_NON_BLOCKING);
  if (fn->hasFlag(FLAG_COBEGIN_OR_COFORALL))    wrap_fn->addFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK);
  if (fn->hasFlag(FLAG_BEGIN))                  wrap_fn->addFlag(FLAG_BEGIN_BLOCK);

  if (fn->hasFlag(FLAG_ON)) {
    // The wrapper function for 'on' block has an additional argument, which
    // passes the new wide locale pointer to the fork function.
    // This argument is stripped from the wrapper function during code generation.
    // As far as the compiler knows, the call looks like:
    //  wrapon_fn(new_locale, wrapped_args)
    // and the wrapon_fn has a matching signature.  But at codegen time, this is
    // translated to:
    //  fork(new_locale.locale.node, wrapon_fn, wrapped_args)
    // The fork function effective generates the call
    //  wrapon_fn(wrapped_args)
    // (without the locale arg).

    // The locale arg is originally attached to the on_fn, but we steal it here.
    // The on_fn does not need this extra argument, and can find out its locale
    // by reading the task-private "here" pointer.
    DefExpr* localeArg = toDefExpr(fn->formals.get(1)->remove());
    wrap_fn->insertFormalAtTail(localeArg);
  }

  ArgSymbol *wrap_c = new ArgSymbol( INTENT_BLANK, "c", ctype);
  wrap_fn->insertFormalAtTail(wrap_c);

  mod->block->insertAtTail(new DefExpr(wrap_fn));

  // The wrapper function is called with the bundled argument list.
  if (fn->hasFlag(FLAG_ON))
    // For an on block, the first argument is also passed directly
    // to the wrapper function.
    // The forking function uses this to fork a task on the target locale.
    fcall->insertBefore(new CallExpr(wrap_fn, fcall->get(1)->remove(), tempc));
  else
    fcall->insertBefore(new CallExpr(wrap_fn, tempc));

  // Create a call to the original function
  CallExpr *call_orig = new CallExpr( (toSymExpr(fcall->baseExpr))->var);
  VarSymbol* locTemp = NULL;
  bool first = true;
  for_fields(field, ctype)
  {
    // insert args
    VarSymbol* tmp = newTemp(field->type);
    wrap_fn->insertAtTail(new DefExpr(tmp));
    wrap_fn->insertAtTail(
        new CallExpr(PRIM_MOVE, tmp,
        new CallExpr(PRIM_GET_MEMBER_VALUE, wrap_c, field)));

    // Special case: 
    // If this is an on block,  remember the first field,
    // but don't add to the list of actuals passed to the original on_fn.
    // It contains the locale on which the new task is launched.
    if (first && fn->hasFlag(FLAG_ON))
      locTemp = tmp;
    else
      call_orig->insertAtTail(tmp);

    first = false;
  }

  VarSymbol* oldHere = NULL;
  if (fn->hasFlag(FLAG_ON))
  {
    // Special case for the first argument of an on_fn, which carries
    // the destination locale ID.
    // We save off the current values on the stack and then
    // set the task-private values to those passed in.
    // The saved values are restored when the on block is exited.
    oldHere = newTemp(dtLocale);
    wrap_fn->insertAtTail(new DefExpr(oldHere));
    wrap_fn->insertAtTail(new CallExpr(PRIM_MOVE, oldHere,
                                       new CallExpr(PRIM_TASK_GET_HERE)));
    wrap_fn->insertAtTail(new CallExpr(PRIM_TASK_SET_LOCALE,
                                       new CallExpr(PRIM_WIDE_GET_LOCALE, locTemp)));
    wrap_fn->insertAtTail(new CallExpr(PRIM_TASK_SET_HERE,
                                       new CallExpr(PRIM_WIDE_GET_ADDR, locTemp)));
  }

  wrap_fn->retType = dtVoid;
  wrap_fn->insertAtTail(call_orig);     // add new call

  if (fn->hasFlag(FLAG_ON)) {
    // Restore old here value
    wrap_fn->insertAtTail(new CallExpr(PRIM_TASK_SET_LOCALE,
                                  new CallExpr(PRIM_WIDE_GET_LOCALE, oldHere)));
    wrap_fn->insertAtTail(new CallExpr(PRIM_TASK_SET_HERE,
                                  new CallExpr(PRIM_WIDE_GET_ADDR, oldHere)));
  }

  if (fn->hasFlag(FLAG_ON))
    fcall->insertAfter(new CallExpr(PRIM_TASK_FREE, tempc));
  else
    wrap_fn->insertAtTail(new CallExpr(PRIM_TASK_FREE, wrap_c));

  wrap_fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  DefExpr  *fcall_def= (toSymExpr( fcall->baseExpr))->var->defPoint;
  fcall->remove();                     // remove orig. call
  fcall_def->remove();                 // move orig. def
  mod->block->insertAtTail(fcall_def); // to top-level
//  normalize(wrap_fn);
}


static void
insertEndCount(FnSymbol* fn,
               Type* endCountType,
               Vec<FnSymbol*>& queue,
               Map<FnSymbol*,Symbol*>& endCountMap) {
  if (fn == chpl_gen_main) {
    VarSymbol* var = newTemp("_endCount", endCountType);
    fn->insertAtHead(new DefExpr(var));
    endCountMap.put(fn, var);
    queue.add(fn);
  } else {
    ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, "_endCount", endCountType);
    fn->insertFormalAtTail(arg);
    VarSymbol* var = newTemp("_endCount", endCountType);
    fn->insertAtHead(new CallExpr(PRIM_MOVE, var, arg));
    fn->insertAtHead(new DefExpr(var));
    endCountMap.put(fn, var);
    queue.add(fn);
  }
}


static void
replicateGlobalRecordWrappedVars(DefExpr *def) {
  ModuleSymbol* mod = toModuleSymbol(def->parentSymbol);
  Expr* stmt = mod->initFn->body->body.head;
  Expr* useFirst = NULL;
  Symbol *currDefSym = def->sym;
  bool found = false;
  // Try to find the first definition of this variable in the
  //   module initialization function
  while (stmt->next && !found) {
    stmt = stmt->next;
    Vec<SymExpr*> symExprs;
    collectSymExprs(stmt, symExprs);
    forv_Vec(SymExpr, se, symExprs) {
      if (se->var == currDefSym) {
        INT_ASSERT(se->parentExpr);
        int result = isDefAndOrUse(se);
        if (result & 1) {
          // first use/def of the variable is a def (normal case)
          INT_ASSERT(useFirst==NULL);
          found = true;
          break;
        } else if (result & 2) {
          if (useFirst == NULL) {
            // This statement captures a reference to the variable
            // to pass it to the function that builds the initializing
            // expression
            CallExpr *parent = toCallExpr(se->parentExpr);
            INT_ASSERT(parent);
            INT_ASSERT(parent->isPrimitive(PRIM_ADDR_OF));
            INT_ASSERT(isCallExpr(parent->parentExpr));
            // Now start looking for the first use of the captured
            // reference
            currDefSym = toSymExpr(toCallExpr(parent->parentExpr)->get(1))->var;
            INT_ASSERT(currDefSym);
            // This is used to flag that we have found the first use
            // of the variable
            useFirst = stmt;
          } else {
            // This statement builds the initializing expression, so
            // we can insert the broadcast after this statement

            // These checks may need to change if we change the way
            // we handle domain literals, forall expressions, and/or
            // depending on how we add array literals to the language
            INT_ASSERT(toCallExpr(stmt));
            INT_ASSERT(toCallExpr(stmt)->primitive==NULL);
            found = true;
            break;
          }
        }
      }
    }
  }
  stmt->insertAfter(new CallExpr(PRIM_PRIVATE_BROADCAST, def->sym));
}


static ClassType*
buildHeapType(Type* type) {
  static Map<Type*,ClassType*> heapTypeMap;
  if (heapTypeMap.get(type))
    return heapTypeMap.get(type);

  SET_LINENO(type->symbol);
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
    if (fn->hasFlag(FLAG_BEGIN) || fn->hasFlag(FLAG_COBEGIN_OR_COFORALL) ||
        fn->hasFlag(FLAG_NON_BLOCKING))
      fnsContainingTaskll.add(fn);
  }
  // add any functions that call the functions added so far
  forv_Vec(FnSymbol, fn, fnsContainingTaskll) {
    forv_Vec(CallExpr, call, *fn->calledBy) {
      if (call->parentSymbol) {
        FnSymbol* caller = toFnSymbol(call->parentSymbol);
        INT_ASSERT(caller);
        fnsContainingTaskll.add_exclusive(caller);
      }
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
    // find out if a variable that was put on the heap could be passed in as an
    // argument to a function created from a begin, cobegin, or coforall statement;
    // if not, free the heap memory just allocated at the end of the block
    if (defMap.get(var)->n == 1) {
      bool freeVar = true;
      Vec<Symbol*> varsToTrack;
      varsToTrack.add(var);
      forv_Vec(Symbol, v, varsToTrack) {
        if (useMap.get(v)) {
          forv_Vec(SymExpr, se, *useMap.get(v)) {
            if (CallExpr* call = toCallExpr(se->parentExpr)) {
              if (call->isPrimitive(PRIM_ADDR_OF) ||
                  call->isPrimitive(PRIM_GET_MEMBER) ||
                  call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                  call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
                  call->isPrimitive(PRIM_WIDE_GET_NODE) ||
                  call->isPrimitive(PRIM_WIDE_GET_SUBLOC))
                // Treat the use of these primitives as a use of their arguments.
                call = toCallExpr(call->parentExpr);
              if (call->isPrimitive(PRIM_MOVE))
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
        INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
        Expr* innermostBlock = NULL;
        // find the innermost block that contains all uses of var
        INT_ASSERT(useMap.get(var));
        forv_Vec(SymExpr, se, *useMap.get(var)) {
          bool useInInnermostBlock = false;
          BlockStmt* curInnermostBlock = toBlockStmt(se->parentExpr);
          INT_ASSERT(!curInnermostBlock); // assumed to be NULL at this point
          for (Expr* block = se->parentExpr->parentExpr;
               block && !useInInnermostBlock;
               block = block->parentExpr) {
            if (!curInnermostBlock)
              curInnermostBlock = toBlockStmt(block);
            if (!innermostBlock) {
              innermostBlock = toBlockStmt(block);
              if (innermostBlock)
                useInInnermostBlock = true;
            } else if (block == innermostBlock)
              useInInnermostBlock = true;
          }
          if (!useInInnermostBlock) {
            // the current use is not contained within innermostBlock,
            // so find out if the innermost block that contains the current use
            // also contains innermostBlock
            Expr* block = innermostBlock;
            while (block && block != curInnermostBlock)
              block = block->parentExpr;
            if (block)
              innermostBlock = curInnermostBlock;
            else {
              // the innermost block that contains the current use is disjoint
              // from the innermost block that contains previously encountered use(s)
              INT_ASSERT(innermostBlock && !block);
              while ((innermostBlock = innermostBlock->parentExpr)) {
                for (block = curInnermostBlock->parentExpr; block && block != innermostBlock;
                     block = block->parentExpr)
                  /* do nothing */;
                if (block) break;
              }
              if (!innermostBlock)
                INT_FATAL(move, "cannot find a block that contains all uses of var\n");
            }
          }
        }
        FnSymbol* fn = toFnSymbol(move->parentSymbol);
        SET_LINENO(var);
        // These were allocated using the system allocator, so do not have to use PRIM_TASK_FREE.
        if (fn && innermostBlock == fn->body)
          fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_CHPL_FREE, move->get(1)->copy()));
        else {
          BlockStmt* block = toBlockStmt(innermostBlock);
          INT_ASSERT(block);
          block->insertAtTailBeforeGoto(new CallExpr(PRIM_CHPL_FREE, move->get(1)->copy()));
        }
      }
    }
  }
}

static bool
needHeapVars() {
  if (fLocal) return false;

  if (!strcmp(CHPL_COMM, "ugni") ||
      (!strcmp(CHPL_COMM, "gasnet") &&
       !strcmp(CHPL_GASNET_SEGMENT, "everything")))
    return false;

  return true;
}

static void findBlockRefActuals(Vec<Symbol*>& refSet, Vec<Symbol*>& refVec)
{
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_BEGIN) ||
        (fn->hasFlag(FLAG_ON) &&
         (needHeapVars() || fn->hasFlag(FLAG_NON_BLOCKING)))) {
      for_formals(formal, fn) {
        if (formal->type->symbol->hasFlag(FLAG_REF)) {
          refSet.set_add(formal);
          refVec.add(formal);
        }
      }
    }
  }
}


static void findHeapVarsAndRefs(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                Vec<Symbol*>& refSet, Vec<Symbol*>& refVec,
                                Vec<Symbol*>& varSet, Vec<Symbol*>& varVec)
{
  forv_Vec(DefExpr, def, gDefExprs) {
    SET_LINENO(def);
    if (def->sym->hasFlag(FLAG_COFORALL_INDEX_VAR)) {
      if (def->sym->type->symbol->hasFlag(FLAG_REF)) {
        refSet.set_add(def->sym);
        refVec.add(def->sym);
      } else if (!isPrimitiveType(def->sym->type) ||
                 toFnSymbol(def->parentSymbol)->retTag==RET_VAR) {
        varSet.set_add(def->sym);
        varVec.add(def->sym);
      }
    } else if (!fLocal &&
               isModuleSymbol(def->parentSymbol) &&
               def->parentSymbol != rootModule &&
               isVarSymbol(def->sym) &&
               !def->sym->hasFlag(FLAG_PRIVATE) &&
               !def->sym->hasFlag(FLAG_EXTERN)) {
      if (def->sym->hasFlag(FLAG_CONST) &&
          (is_int_type(def->sym->type) ||
           is_uint_type(def->sym->type) ||
           is_real_type(def->sym->type) ||
           def->sym->type == dtBool ||
           (isRecord(def->sym->type) &&
            !isRecordWrappedType(def->sym->type) &&
            // sync/single are currently classes, so this shouldn't matter
            !isSyncType(def->sym->type)))) {
        // replicate global const of primitive type
        INT_ASSERT(defMap.get(def->sym) && defMap.get(def->sym)->n == 1);
        for_defs(se, defMap, def->sym) {
          se->getStmtExpr()->insertAfter(new CallExpr(PRIM_PRIVATE_BROADCAST, def->sym));
        }
      } else if (isRecordWrappedType(def->sym->type)) {
        // replicate address of global arrays, domains, and distributions
        replicateGlobalRecordWrappedVars(def);
      } else {
        // put other global constants and all global variables on the heap
        varSet.set_add(def->sym);
        varVec.add(def->sym);
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

  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap);

  findBlockRefActuals(refSet, refVec);
  findHeapVarsAndRefs(defMap, refSet, refVec, varSet, varVec);

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
      //      INT_ASSERT(defMap.get(var)->n == 1);
      for_defs(def, defMap, var) {
        INT_ASSERT(def);
        if (CallExpr* call = toCallExpr(def->parentExpr)) {
          if (call->isPrimitive(PRIM_MOVE)) {
            if (CallExpr* rhs = toCallExpr(call->get(2))) {
              if (rhs->isPrimitive(PRIM_ADDR_OF)) {
                SymExpr* se = toSymExpr(rhs->get(1));
                INT_ASSERT(se);
                if (!varSet.set_in(se->var)) {
                  varSet.set_add(se->var);
                  varVec.add(se->var);
                }
              } else if (rhs->isPrimitive(PRIM_GET_MEMBER) ||
                         rhs->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                         rhs->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                         rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
                SymExpr* se = toSymExpr(rhs->get(1));
                INT_ASSERT(se);
                if (se->var->type->symbol->hasFlag(FLAG_REF)) {
                  if (!refSet.set_in(se->var)) {
                    refSet.set_add(se->var);
                    refVec.add(se->var);
                  }
                } else if (!varSet.set_in(se->var)) {
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

    if (var->hasFlag(FLAG_EXTERN)) {
      // don't widen external variables
      continue;
    }
    SET_LINENO(var);

    if (ArgSymbol* arg = toArgSymbol(var)) {
      VarSymbol* tmp = newTemp(var->type);
      varSet.set_add(tmp);
      varVec.add(tmp);
      SymExpr* firstDef = new SymExpr(tmp);
      arg->getFunction()->insertAtHead(new CallExpr(PRIM_MOVE, firstDef, arg));
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

    //
    // allocate local variables on the heap; global variables are put
    // on the heap during program startup
    //
    if (!isModuleSymbol(var->defPoint->parentSymbol) &&
        ((useMap.get(var) && useMap.get(var)->n > 0) ||
         (defMap.get(var) && defMap.get(var)->n > 0))) {
      CallExpr* alloc_call =
        new CallExpr(PRIM_CHPL_ALLOC, heapType->symbol,
                     newMemDesc("local heap-converted data"));
      var->defPoint->getStmtExpr()->insertAfter(
        new CallExpr(PRIM_MOVE, var,
                     new CallExpr(PRIM_CAST, heapType->symbol, alloc_call)));
      heapAllocatedVars.add(var);
    }

    for_defs(def, defMap, var) {
      if (CallExpr* call = toCallExpr(def->parentExpr)) {
        SET_LINENO(call);
        if (call->isPrimitive(PRIM_MOVE)) {
          VarSymbol* tmp = newTemp(var->type);
          call->insertBefore(new DefExpr(tmp));
          call->insertBefore(new CallExpr(PRIM_MOVE, tmp, call->get(2)->remove()));
          call->replace(new CallExpr(PRIM_SET_MEMBER, call->get(1)->copy(), heapType->getField(1), tmp));
        } else if (call->isResolved() &&
                   call->isResolved()->hasFlag(FLAG_AUTO_DESTROY_FN)) {
          call->remove();
        } else {
          VarSymbol* tmp = newTemp(var->type);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, def->var, heapType->getField(1))));
          def->replace(new SymExpr(tmp));
        }
      } else
        INT_FATAL(var, "unexpected case");
    }

    for_uses(use, useMap, var) {
      if (CallExpr* call = toCallExpr(use->parentExpr)) {
        if (call->isPrimitive(PRIM_ADDR_OF)) {
          CallExpr* move = toCallExpr(call->parentExpr);
          INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
          if (move->get(1)->typeInfo() == heapType) {
            call->replace(use->copy());
          } else {
            call->replace(new CallExpr(PRIM_GET_MEMBER, use->var, heapType->getField(1)));
          }
        } else if (call->isResolved()) {
          if (call->isResolved()->hasFlag(FLAG_AUTO_DESTROY_FN_SYNC)) {
            //
            // We don't move sync vars to the heap and don't do the
            // analysis to determine whether or not they outlive a
            // task that refers to them, so conservatively remove
            // their autodestroy calls to avoid freeing them before
            // all tasks are done with them.  While this is
            // unfortunate and needs to be fixed in the future to
            // avoid leaks (TODO), it is better than the previous
            // version of this code that would remove all autodestroy
            // calls in this conditional.  See the commit message for
            // this commenet for more detail.
            //
            call->remove();
          } else if (actual_to_formal(use)->type == heapType) {
            // do nothing
          } else {
            VarSymbol* tmp = newTemp(var->type);
            call->getStmtExpr()->insertBefore(new DefExpr(tmp));
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, use->var, heapType->getField(1))));
            use->replace(new SymExpr(tmp));
          }
        } else if ((call->isPrimitive(PRIM_GET_MEMBER) ||
                    call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                    call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                    call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) ||
                    call->isPrimitive(PRIM_WIDE_GET_LOCALE) || //I'm not sure this is cricket.
                    call->isPrimitive(PRIM_WIDE_GET_NODE) ||// what member are we extracting?
                    call->isPrimitive(PRIM_WIDE_GET_SUBLOC) ||
                    call->isPrimitive(PRIM_SET_SVEC_MEMBER) ||
                    call->isPrimitive(PRIM_SET_MEMBER)) &&
                   call->get(1) == use) {
          VarSymbol* tmp = newTemp(var->type->refType);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, use->var, heapType->getField(1))));
          use->replace(new SymExpr(tmp));
        } else {
          VarSymbol* tmp = newTemp(var->type);
          call->getStmtExpr()->insertBefore(new DefExpr(tmp));
          call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, use->var, heapType->getField(1))));
          use->replace(new SymExpr(tmp));
        }
      } else if (use->parentExpr)
        INT_FATAL(var, "unexpected case");
    }

    var->type = heapType;
  }

  freeHeapAllocatedVars(heapAllocatedVars);
}


//
// re-privatize privatized object fields in iterator classes
//
static void
reprivatizeIterators() {
  Vec<Symbol*> privatizedFields;

  forv_Vec(ClassType, ct, gClassTypes) {
    for_fields(field, ct) {
      if (ct->symbol->hasFlag(FLAG_ITERATOR_CLASS) &&
          field->type->symbol->hasFlag(FLAG_PRIVATIZED_CLASS)) {
        privatizedFields.set_add(field);
      }
    }
  }

  forv_Vec(SymExpr, se, gSymExprs) {
    if (privatizedFields.set_in(se->var)) {
      SET_LINENO(se);
      if (CallExpr* call = toCallExpr(se->parentExpr)) {
        if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
          CallExpr* move = toCallExpr(call->parentExpr);
          INT_ASSERT(move->isPrimitive(PRIM_MOVE));
          SymExpr* lhs = toSymExpr(move->get(1));
          ClassType* ct = toClassType(se->var->type);
          VarSymbol* tmp = newTemp(ct->getField("pid")->type);
          move->insertBefore(new DefExpr(tmp));
          lhs->replace(new SymExpr(tmp));
          move->insertAfter(new CallExpr(PRIM_MOVE, lhs->var, new CallExpr(PRIM_GET_PRIV_CLASS, lhs->var->type->symbol, tmp)));
        } else if (call->isPrimitive(PRIM_GET_MEMBER)) {
          CallExpr* move = toCallExpr(call->parentExpr);
          INT_ASSERT(move->isPrimitive(PRIM_MOVE));
          SymExpr* lhs = toSymExpr(move->get(1));
          ClassType* ct = toClassType(se->var->type);
          VarSymbol* tmp = newTemp(ct->getField("pid")->type);
          move->insertBefore(new DefExpr(tmp));
          lhs->replace(new SymExpr(tmp));
          call->primitive = primitives[PRIM_GET_MEMBER_VALUE];
          VarSymbol* valTmp = newTemp(lhs->getValType());
          move->insertBefore(new DefExpr(valTmp));
          move->insertAfter(new CallExpr(PRIM_MOVE, lhs, new CallExpr(PRIM_ADDR_OF, valTmp)));
          move->insertAfter(new CallExpr(PRIM_MOVE, valTmp, new CallExpr(PRIM_GET_PRIV_CLASS, lhs->getValType()->symbol, tmp)));
        } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
          ClassType* ct = toClassType(se->var->type);
          VarSymbol* tmp = newTemp(ct->getField("pid")->type);
          call->insertBefore(new DefExpr(tmp));
          call->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, call->get(3)->remove(), ct->getField("pid"))));
          call->insertAtTail(new SymExpr(tmp));
        } else
          INT_FATAL(se, "unexpected case in re-privatization in iterator");
      } else
        INT_FATAL(se, "unexpected case in re-privatization in iterator");
    }
  }

  forv_Vec(Symbol, sym, privatizedFields) if (sym) {
    sym->type = dtInt[INT_SIZE_DEFAULT];
  }
}


// Converts blocks implementing various parallel constructs into functions, 
// so they can be invoked through a fork.
// The body of the original block becomes the body of the function,
// and the inline location of the parallel block is replaced by a call
// to the implementing function.
// A subsequent step (flattenNesteFunctions) adds arguments to these functions
// to pass in values or references from the context which are used in the body
// of the block.
// As a special case, the target locale is prepended to the arguments passed 
// to the "on" function.
static void createNestedFunctions(Vec<FnSymbol*>& nestedFunctions)
{
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (CallExpr* info = block->blockInfo) {
      SET_LINENO(block);
      FnSymbol* fn = NULL;
      if (info->isPrimitive(PRIM_BLOCK_BEGIN)) {
        fn = new FnSymbol("begin_fn");
        fn->addFlag(FLAG_BEGIN);
      } else if (info->isPrimitive(PRIM_BLOCK_COBEGIN)) {
        fn = new FnSymbol("cobegin_fn");
        fn->addFlag(FLAG_COBEGIN_OR_COFORALL);
      } else if (info->isPrimitive(PRIM_BLOCK_COFORALL)) {
        fn = new FnSymbol("coforall_fn");
        fn->addFlag(FLAG_COBEGIN_OR_COFORALL);
      } else if (info->isPrimitive(PRIM_BLOCK_ON) ||
                 info->isPrimitive(PRIM_BLOCK_ON_NB)) {
        fn = new FnSymbol("on_fn");
        fn->addFlag(FLAG_ON);
        if (block->blockInfo->isPrimitive(PRIM_BLOCK_ON_NB))
          fn->addFlag(FLAG_NON_BLOCKING);

        ArgSymbol* locarg = new ArgSymbol(INTENT_BLANK, "_dummy_locale_arg", dtLocale);
        fn->insertFormalAtTail(locarg);
      }
      else if (// info->isPrimitive(PRIM_BLOCK_PARAM_LOOP) || // resolution should remove this case.
               info->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP) ||
               info->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP) ||
               info->isPrimitive(PRIM_BLOCK_FOR_LOOP) ||
               info->isPrimitive(PRIM_BLOCK_XMT_PRAGMA_FORALL_I_IN_N) ||
               info->isPrimitive(PRIM_BLOCK_XMT_PRAGMA_NOALIAS) ||
               info->isPrimitive(PRIM_BLOCK_LOCAL) ||
               info->isPrimitive(PRIM_BLOCK_UNLOCAL))
        ; // Not a parallel block construct, so do nothing special.
      else
        INT_FATAL(block, "Unhandled blockInfo case.");

      if (fn) {
        nestedFunctions.add(fn);
        CallExpr* call = new CallExpr(fn);
        if (block->blockInfo->isPrimitive(PRIM_BLOCK_ON) ||
            block->blockInfo->isPrimitive(PRIM_BLOCK_ON_NB)) {
          // This puts the target locale ID and address at the start of the call.
          call->insertAtTail(block->blockInfo->get(1)->remove());
        }

        block->insertBefore(new DefExpr(fn));
        block->insertBefore(call);
        block->blockInfo->remove();
        // This block becomes the body of the new function.
        fn->insertAtTail(block->remove());
        fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
        fn->retType = dtVoid;
      }
    }
  }
}


void
parallel(void) {
  // convert begin/cobegin/coforall/on blocks into nested functions and flatten
  Vec<FnSymbol*> nestedFunctions;

  createNestedFunctions(nestedFunctions);

  flattenNestedFunctions(nestedFunctions);

  compute_call_sites();

  remoteValueForwarding(nestedFunctions);

  reprivatizeIterators();

  makeHeapAllocations();

  insertEndCounts();

  passArgsToNestedFns(nestedFunctions);
}


static void insertEndCounts()
{
  Vec<FnSymbol*> queue;
  Map<FnSymbol*,Symbol*> endCountMap;

  forv_Vec(CallExpr, call, gCallExprs) {
    SET_LINENO(call);
    if (call->isPrimitive(PRIM_GET_END_COUNT)) {
      FnSymbol* pfn = call->getFunction();
      if (!endCountMap.get(pfn))
        insertEndCount(pfn, call->typeInfo(), queue, endCountMap);
      call->replace(new SymExpr(endCountMap.get(pfn)));
    } else if (call->isPrimitive(PRIM_SET_END_COUNT)) {
      FnSymbol* pfn = call->getFunction();
      if (!endCountMap.get(pfn))
        insertEndCount(pfn, call->get(1)->typeInfo(), queue, endCountMap);
      call->replace(new CallExpr(PRIM_MOVE, endCountMap.get(pfn), call->get(1)->remove()));
    }
  }

  forv_Vec(FnSymbol, fn, queue) {
    forv_Vec(CallExpr, call, *fn->calledBy) {
      SET_LINENO(call);
      Type* endCountType = endCountMap.get(fn)->type;
      FnSymbol* pfn = call->getFunction();
      if (!endCountMap.get(pfn))
        insertEndCount(pfn, endCountType, queue, endCountMap);
      call->insertAtTail(endCountMap.get(pfn));
    }
  }
}


// Given the call to a nested on block, find the definition of the first argument
// (carrying the locale pointer) in the context and return the containing move primitive.
static CallExpr*
findLocaleLookup(CallExpr* onBlockCall, Map<Symbol*, Vec<SymExpr*>*>& defMap)
{
  // The first argument of an on block call is the locale ID or object passed to the on statement.
  Expr* onExpr = onBlockCall->get(1);
  INT_ASSERT_AND_RETURN_NULL(onExpr);
  // Assume that the on expression is a temp.
  SymExpr* locTemp = toSymExpr(onExpr);
  INT_ASSERT_AND_RETURN_NULL(locTemp);

  // Extract its definition.
  Expr* locDef = defMap.get(locTemp->var)->only()->parentExpr;
  INT_ASSERT_AND_RETURN_NULL(locDef);
  CallExpr* toLocMove = toCallExpr(locDef);
  INT_ASSERT(toLocMove->isPrimitive(PRIM_MOVE));

  return toLocMove;
}


// The destination locale expression (onExpr) for an on clause has one of two forms:
// (1) onExpr = (chpl_on_locale_num x)
// (2) tmpLocID = (_wide_get_locale x)
//     onExpr = ("chpl_localeID_to_locale" tmpLocID)
// depending on whether the original x is of type localeID or is just a wide pointer.
//
// We want to inspect the destination locale ID without having to execute 
// PRIM_WIDE_GET_LOCALE a second time.  So we just extract the argument to the expected
// chpl_localeID_to_locale call and return it.
static Expr* findDestinationLocaleID(CallExpr* toLocMove)
{
  // This is somewhat fragile, because other substitutions could have been made.
  CallExpr* toLocCall = toCallExpr(toLocMove->get(2));
  INT_ASSERT_AND_RETURN_NULL(toLocCall);

// toLocCall should be a call to chpl_localeID_to_locale or the chpl_on_locale_num primitive.
  Expr* locID = NULL;
  // Try the latter.
  if (toLocCall->isPrimitive(PRIM_ON_LOCALE_NUM))
    locID = toLocCall->get(1);
  else
  {
    // Try the former.
    FnSymbol* locFn = toLocCall->isResolved();
    if (!strcmp(locFn->name, "chpl_localeID_to_locale"))
      locID = toLocCall->get(1);
  }

  INT_ASSERT_AND_RETURN_NULL(locID);
  INT_ASSERT_AND_RETURN_NULL(locID->typeInfo() == dtLocaleID);
  return locID;
}


// Find the object that is used in the "on" expression.
static Expr* findDestLocale(CallExpr* toLocMove, Map<Symbol*, Vec<SymExpr*>*>& defMap)
{
  CallExpr* toLocCall = toCallExpr(toLocMove->get(2));
  INT_ASSERT_AND_RETURN_NULL(toLocCall);

// toLocCall should be a call to chpl_localeID_to_locale or the chpl_on_locale_num primitive.
  Expr* locID = NULL;
  // Try the latter.
  if (toLocCall->isPrimitive(PRIM_ON_LOCALE_NUM))
    // This is not the case we're looking for.
    return NULL;
  
  // Try the former.
  FnSymbol* locFn = toLocCall->isResolved();
  if (!strcmp(locFn->name, "chpl_localeID_to_locale"))
    locID = toLocCall->get(1);
  INT_ASSERT_AND_RETURN_NULL(locID);

  // We expect locID to be a temp.
  SymExpr* locIDTemp = toSymExpr(locID);
  INT_ASSERT_AND_RETURN_NULL(locIDTemp);

  // Extract its definition.
  Expr* locIDDef = defMap.get(locIDTemp->var)->only()->parentExpr;
  INT_ASSERT_AND_RETURN_NULL(locIDDef);
  CallExpr* locIDMove = toCallExpr(locIDDef);
  INT_ASSERT_AND_RETURN_NULL(locIDMove->isPrimitive(PRIM_MOVE));

  // We expect only PRIM_WIDE_GET_LOCALE here.
  CallExpr* locIDget = toCallExpr(locIDMove->get(2));
  INT_ASSERT_AND_RETURN_NULL(locIDget);
  INT_ASSERT_AND_RETURN_NULL(locIDget->isPrimitive(PRIM_WIDE_GET_LOCALE));

  // Get the operand of the primitive.
  Expr* obj = locIDget->get(1);
  if (isSubClass(obj->typeInfo()->getValType(), dtLocale))
    // Found it! The object is a locale, so we can use it directly.
    return obj;

  // Too bad.  The optimization fails.
  return NULL;
}


// Perform optimizations related to on_fn calls.
// Overhead is high for on statements, since in general they call a forking function.
// The main optimization is to insert a conditional, so the forking function is called
// only if needed.
// If locale==here and the call to on_fn is supposed to block, we can run the on_fn
// in the same task and thus avoid the fork.  This is done by calling on_fn() directly
// rather than creating a fork to wrapon_fn().
// For on_fn() calls marked as non-blocking, we could also perform this optimization if
// the serial state is true.  To do that, we would have to hoist the relevant code into
// the compiler generated code from the codegen function in which it currently resides
// (chpl_comm_nonblocking_on).
//
// One other optimizations are applied here: Calls to chpl_localeID_to_locale() 
// are short-circuited if the expression x passed to the "on" clause 
//
//  blockInfo = new CallExpr(PRIM_BLOCK_ON, x);
//
// has type dtLocale.  The type of the "on" expression can be any lvalue type, but the
// parser inserts a call to chpl_localeID_to_locale so every on expression has type 
// dtLocale:
//
//  x->replace(new CallExpr("chpl_localeID_to_locale", new CallExpr(PRIM_WIDE_GET_LOCALE, x)));
//
// Obviously, this is not needed if x is of type dtLocale -- but the type of x is not 
// known to the parser.  That's why we put the code in then, and selectively take it back
// out now.
static CallExpr* optimizeOnCall(CallExpr* call)
{
  FnSymbol* fn = call->isResolved();

  // The definition of the on expression should live within the parent of the call.
  BlockStmt* parentBlock = toBlockStmt(call->parentExpr);
  Map<Symbol*, Vec<SymExpr*>*> defMap;
  Map<Symbol*, Vec<SymExpr*>*> useMap;
  buildDefUseMaps(parentBlock, defMap, useMap);

  // Look for some important parts of the call.
  CallExpr* localeLookup = findLocaleLookup(call, defMap);
  Expr* localeID = findDestinationLocaleID(localeLookup);

  // This is non-null only if the type of the original lvalue supplied as the argument
  // to the "on" clause was dtLocale (or derived from dtLocale).
  Expr* destLocale = findDestLocale(localeLookup, defMap);

  // If the dest object is a locale object, bridge out the localeID_to_lcoale call.
  CallExpr* forkedCall = call;
  if (destLocale)
  {
    SymbolMap map;
    Symbol* destLocaleTmp = toSymExpr(destLocale)->var;
    if (isReferenceType(destLocale->typeInfo()))
    {
      VarSymbol* derefTmp = newTemp(destLocale->typeInfo()->getValType());
      call->insertBefore(new DefExpr(derefTmp));
      call->insertBefore(new CallExpr(PRIM_MOVE, derefTmp, new CallExpr(PRIM_DEREF, destLocaleTmp)));
      destLocaleTmp = derefTmp;
    }
    map.put(toSymExpr(call->get(1))->var, destLocaleTmp);
    forkedCall = call->copy(&map);
    call->insertBefore(forkedCall);
    call->remove();
  }

  if (!fn->hasFlag(FLAG_NON_BLOCKING))
  {
    VarSymbol* tmpLocale = newTemp(localeID->typeInfo());
    VarSymbol* tmpBool = newTemp(dtBool);
    forkedCall->insertBefore(new DefExpr(tmpLocale));
    forkedCall->insertBefore(new DefExpr(tmpBool));
    forkedCall->insertBefore(new CallExpr(PRIM_MOVE, tmpLocale, localeID->copy()));
    forkedCall->insertBefore(new CallExpr(PRIM_MOVE, tmpBool,
                                    new CallExpr(PRIM_IS_HERE, tmpLocale)));
    BlockStmt* lblock = new BlockStmt();
    BlockStmt* rblock = new BlockStmt();
    forkedCall->insertBefore(new CondStmt(new SymExpr(tmpBool), lblock, rblock));

    // When the locale IDs agree, we call the on function directly.
    CallExpr* directCall = forkedCall->copy();
    // In which case, we first remove the unneeded locale arg.
    // After the wrapon_fn is created, the dummy locale arg is removed from
    // the signature of the on_fn.
    directCall->get(1)->remove();
    lblock->insertAtHead(directCall);

    // Otherwise, the on function is called through a fork.
    // Optmization: Move locale lookup into the "else" branch.
    // We are already on the correct locale if we take the "if" branch.
    rblock->insertAtHead(forkedCall->remove());
  }

  // We only need the locale lookup if the original object was not a locale.
  // And we only need it in the right branch in any case.
  localeLookup->remove();
  if (!destLocale)
    forkedCall->insertBefore(localeLookup);

  // Bundling only affects the original call -- 
  // the one in rblock if the above conditional fires.
  return forkedCall;
}


// For each "nested" function created to represent remote execution, 
// bundle args so they can be passed through a fork function.
// Fork functions in general have the signature
//  fork(int32_t destNode, void (*)(void* args), void* args, ...);
// In Chapel, we wrap the arguments passed to the nested function in an object
// whose type is just a list of the arguments passed to the nested function.
// Those arguments consist of variables in the scope of the nested function call
// that are accessed within the body of the nested function (recursively, of course).
static void passArgsToNestedFns(Vec<FnSymbol*>& nestedFunctions)
{
  forv_Vec(FnSymbol, fn, nestedFunctions) {
    // We assume that there is a one-to-one relation between nested function callers and callees.
    CallExpr* call = (fn->calledBy)->only();

    SET_LINENO(call);

    if (fn->hasFlag(FLAG_ON))
      call = optimizeOnCall(call);
    
    bundleArgs(call);
  }
}


ClassType* wideStringType = NULL;

static void
buildWideClass(Type* type) {
  SET_LINENO(type->symbol);
  ClassType* wide = new ClassType(CLASS_RECORD);
  TypeSymbol* wts = new TypeSymbol(astr("__wide_", type->symbol->cname), wide);
  wts->addFlag(FLAG_WIDE_CLASS);
  theProgram->block->insertAtTail(new DefExpr(wts));
  wide->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtLocaleID)));
  wide->fields.insertAtTail(new DefExpr(new VarSymbol("addr", type)));

  //
  // Strings need an extra field in their wide class to hold their length
  //
  if (type == dtString) {
    wide->fields.insertAtTail(new DefExpr(new VarSymbol("size", dtInt[INT_SIZE_DEFAULT])));
    if (wideStringType) {
      INT_FATAL("Created two wide string types");
    }
    wideStringType = wide;
  }

  //
  // set reference type of wide class to reference type of class since
  // it will be widened
  //
  if (type->refType)
    wide->refType = type->refType;

  wideClassMap.put(type, wide);
}

Type* getOrMakeRefTypeDuringCodegen(Type* type) {
  Type* refType;
  refType = type->refType;
  if( ! refType ) {
    SET_LINENO(type->symbol);
    ClassType* ref = new ClassType(CLASS_RECORD);
    TypeSymbol* refTs = new TypeSymbol(astr("_ref_", type->symbol->cname), ref);
    refTs->addFlag(FLAG_REF);
    refTs->addFlag(FLAG_NO_DEFAULT_FUNCTIONS);
    refTs->addFlag(FLAG_NO_OBJECT);
    theProgram->block->insertAtTail(new DefExpr(refTs));
    ref->fields.insertAtTail(new DefExpr(new VarSymbol("_val", type)));
    refType = ref;
    type->refType = ref;
  }
  return refType;
}

// This function is called if the wide reference type does not already
// exist to cause it to be code generated even though it was not
// needed by earlier passes.
Type* getOrMakeWideTypeDuringCodegen(Type* refType) {
  Type* wideType;
  INT_ASSERT(refType == dtNil ||
             isClass(refType) ||
             refType->symbol->hasFlag(FLAG_REF));
  // First, check if the wide type already exists.
  if( isClass(refType) ) {
    wideType = wideClassMap.get(refType);
    if( wideType ) return wideType;
  }
  // For a ref to a class, isClass seems to return true...
  wideType = wideRefMap.get(refType);
  if( wideType ) return wideType;

  // Now, create a wide pointer type.
  ClassType* wide = new ClassType(CLASS_RECORD);
  TypeSymbol* wts = new TypeSymbol(astr("chpl____wide_", refType->symbol->cname), wide);
  if( refType->symbol->hasFlag(FLAG_REF) || refType == dtNil )
    wts->addFlag(FLAG_WIDE);
  else
    wts->addFlag(FLAG_WIDE_CLASS);
  theProgram->block->insertAtTail(new DefExpr(wts));
  wide->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtLocaleID)));
  wide->fields.insertAtTail(new DefExpr(new VarSymbol("addr", refType)));
  if( isClass(refType) ) {
    wideClassMap.put(refType, wide);
  } else {
    wideRefMap.put(refType, wide);
  }
  return wide;
}

 
//
// This is a utility function that handles a case when wide strings
// are passed to extern functions.  If strings were a little better
// behaved, it arguably wouldn't/shouldn't be required.
//
bool passingWideStringToExtern(Type* t) {
  ClassType* ct = toClassType(t);
  if (ct) {
    Symbol* valField = ct->getField("_val", false);
    if (valField && valField->type == wideStringType) {
      return true;
    }
  }
  return false;
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
    stmt->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, se->copy()));
  }
  stmt->insertBefore(new DefExpr(var));
  stmt->insertBefore(new CallExpr(PRIM_MOVE, var, se->copy()));
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
    case PRIM_ARRAY_SET: /* Fallthru */
    case PRIM_ARRAY_SET_FIRST:
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIM_MOVE:
      if (CallExpr* rhs = toCallExpr(call->get(2))) {
        if (rhs->isPrimitive(PRIM_DEREF)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
              rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            insertLocalTemp(rhs->get(1));
            if (!rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
              rhs->replace(rhs->get(1)->remove());
            }
          }
          break;
        } 
#if 0
        else if (rhs->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
                 rhs->isPrimitive(PRIM_WIDE_GET_NODE) ||
                 rhs->isPrimitive(PRIM_WIDE_GET_SUBLOC)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
            if (rhs->get(1)->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
              insertLocalTemp(rhs->get(1));
            }
          }
          break;
        }
#endif
        else if (rhs->isPrimitive(PRIM_GET_MEMBER) ||
                   rhs->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                   rhs->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                   rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
              rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            SymExpr* sym = toSymExpr(rhs->get(2));
            INT_ASSERT(sym);
            if (!sym->var->hasFlag(FLAG_SUPER_CLASS)) {
              insertLocalTemp(rhs->get(1));
            }
          }
          break;
        } else if (rhs->isPrimitive(PRIM_ARRAY_GET) ||
                   rhs->isPrimitive(PRIM_ARRAY_GET_VALUE)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            SymExpr* lhs = toSymExpr(call->get(1));
            Expr* stmt = call->getStmtExpr();
            INT_ASSERT(lhs && stmt);
            insertLocalTemp(rhs->get(1));
            VarSymbol* localVar = NULL;
            if (rhs->isPrimitive(PRIM_ARRAY_GET))
              localVar = newTemp(astr("local_", lhs->var->name),
                                 lhs->var->type->getField("addr")->type);
            else
              localVar = newTemp(astr("local_", lhs->var->name),
                                 lhs->var->type);
            stmt->insertBefore(new DefExpr(localVar));
            lhs->replace(new SymExpr(localVar));
            stmt->insertAfter(new CallExpr(PRIM_MOVE, lhs,
                                           new SymExpr(localVar)));
          }
          break;
        } else if (rhs->isPrimitive(PRIM_GET_UNION_ID)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
            insertLocalTemp(rhs->get(1));
          }
          break;
        } else if (rhs->isPrimitive(PRIM_TESTCID) ||
                   rhs->isPrimitive(PRIM_GETCID)) {
          if (rhs->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            insertLocalTemp(rhs->get(1));
          }
          break;
        }
        ;
      }
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
          !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        break;
      }
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
          !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
          !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIM_DYNAMIC_CAST:
      if (call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        insertLocalTemp(call->get(2));
        if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS) ||
            call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
          toSymExpr(call->get(1))->var->type = call->get(1)->typeInfo()->getField("addr")->type;
        }
      }
      break;
    case PRIM_SETCID:
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIM_SET_UNION_ID:
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
        insertLocalTemp(call->get(1));
      }
      break;
    case PRIM_SET_MEMBER:
    case PRIM_SET_SVEC_MEMBER:
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
        if (block->blockInfo->isPrimitive(PRIM_BLOCK_LOCAL))
          queue.add(block);
  }

  forv_Vec(BlockStmt, block, queue) {
    Vec<CallExpr*> calls;
    collectCallExprs(block, calls);
    forv_Vec(CallExpr, call, calls) {
      localizeCall(call);
      if (FnSymbol* fn = call->isResolved()) {
        if (FnSymbol* alreadyLocal = cache.get(fn)) {
          call->baseExpr->replace(new SymExpr(alreadyLocal));
        } else {
          if (!fn->hasFlag(FLAG_EXTERN)) {
            FnSymbol* local = fn->copy();
            local->name = astr("_local_", fn->name);
            local->cname = astr("_local_", fn->cname);
            fn->defPoint->insertBefore(new DefExpr(local));
            call->baseExpr->replace(new SymExpr(local));
            queue.add(local->body);
            cache.put(fn, local);
            cache.put(local, local); // to handle recursion
            if (local->retType->symbol->hasFlag(FLAG_WIDE)) {
              CallExpr* ret = toCallExpr(local->body->body.tail);
              INT_ASSERT(ret && ret->isPrimitive(PRIM_RETURN));
              // Capture the return expression in a local temp.
              insertLocalTemp(ret->get(1));
              local->retType = ret->get(1)->typeInfo();
            }
          }
        }
      }
    }
  }
}


// Add symbols bearing the FLAG_HEAP flag to a list of heapVars.
static void getHeapVars(Vec<Symbol*>& heapVars)
{
  // Look at all def expressions.
  forv_Vec(DefExpr, def, gDefExprs)
  {
    // We are interested only in var symbols.
    if (!isVarSymbol(def->sym))
      continue;

    // We only want symbols at the module level.
    if (!isModuleSymbol(def->parentSymbol))
      continue;

    // But we don't want any from the root module.
    if (def->parentSymbol == rootModule)
      continue;

    // Okey-dokey.  List up those heap variables.
    if (def->sym->type->symbol->hasFlag(FLAG_HEAP))
      heapVars.add(def->sym);
  }
}


//
// change all classes into wide classes
// change all references into wide references
//
void
insertWideReferences(void) {
  SET_LINENO(baseModule);
  FnSymbol* heapAllocateGlobals = new FnSymbol("chpl__heapAllocateGlobals");
  heapAllocateGlobals->addFlag(FLAG_EXPORT);
  heapAllocateGlobals->retType = dtVoid;
  theProgram->block->insertAtTail(new DefExpr(heapAllocateGlobals));
  heapAllocateGlobals->insertAtHead(new CallExpr(PRIM_ALLOC_GVR));

  if (fLocal) {
    heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    return;
  }

  INT_ASSERT(!fLocal);

  Vec<Symbol*> heapVars;
  getHeapVars(heapVars);

  convertNilToObject();
  wideClassMap.clear();
  buildWideClasses();
  widenClasses();

  wideRefMap.clear();
  buildWideRefMap();
  widenRefs();
  insertElementAccessTemps();
  narrowWideClassesThroughCalls();
  insertWideClassTempsForNil();
  insertWideCastTemps();
  derefWideStringActuals();
  derefWideRefsToWideClasses();
  widenGetPrivClass();

  CallExpr* nodeID = new CallExpr(PRIM_NODE_ID);
  VarSymbol* tmp = newTemp(nodeID->typeInfo());
  VarSymbol* tmpBool = newTemp(dtBool);

  heapAllocateGlobals->insertAtTail(new DefExpr(tmp));
  heapAllocateGlobals->insertAtTail(new DefExpr(tmpBool));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_MOVE, tmp, nodeID));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_MOVE, tmpBool, new CallExpr(PRIM_EQUAL, tmp, new_IntSymbol(0))));
  BlockStmt* block = new BlockStmt();
  forv_Vec(Symbol, sym, heapVars) {
    block->insertAtTail(new CallExpr(PRIM_MOVE, sym, new CallExpr(PRIM_CHPL_ALLOC, sym->type->getField("addr")->type->symbol, newMemDesc("global heap-converted data"))));
  }
  heapAllocateGlobals->insertAtTail(new CondStmt(new SymExpr(tmpBool), block));
  int i = 0;
  forv_Vec(Symbol, sym, heapVars) {
    heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_HEAP_REGISTER_GLOBAL_VAR, new_IntSymbol(i++), sym));
  }
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_HEAP_BROADCAST_GLOBAL_VARS, new_IntSymbol(i)));
  heapAllocateGlobals->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  numGlobalsOnHeap = i;

  handleLocalBlocks();
  narrowWideReferences();
  moveAddressSourcesToTemp();
}


static void convertNilToObject()
{
  //
  // change dtNil return type into dtObject
  // replace symbols of type nil by nil
  //
  forv_Vec(DefExpr, def, gDefExprs) {
    if (FnSymbol* fn = toFnSymbol(def->sym)) {
      if (fn->retType == dtNil)
        fn->retType = dtObject;
    } else if (!isTypeSymbol(def->sym)) {
      if (def->sym != gNil &&
          def->sym->type == dtNil &&
          !isTypeSymbol(def->parentSymbol))
        def->remove();
    }
  }
  forv_Vec(SymExpr, se, gSymExprs) {
    if (se->var != gNil && se->var->type == dtNil) {
      se->var = gNil;
      if (CallExpr* parent = toCallExpr(se->parentExpr))
        if (parent->isPrimitive(PRIM_MOVE) && parent->get(1) == se)
          parent->remove();
    }
  }
}


static void buildWideClasses()
{
  //
  // build wide class type for every class type
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    ClassType* ct = toClassType(ts->type);
    if (ct && isClass(ct) && !ts->hasFlag(FLAG_REF) && !ts->hasFlag(FLAG_NO_WIDE_CLASS)) {
      buildWideClass(ct);
    }
  }
  buildWideClass(dtString);
}


static void widenClasses()
{
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
        if (!fn->hasEitherFlag(FLAG_EXTERN,FLAG_LOCAL))
          fn->retType = wide;
    } else if (!isTypeSymbol(def->sym)) {
      if (Type* wide = wideClassMap.get(def->sym->type)) {
        if (def->parentSymbol->hasEitherFlag(FLAG_EXTERN,FLAG_LOCAL)) {
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
      if (Type* nt = wideClassMap.get(getDataClassType(ts)->type)) {
        setDataClassType(ts, nt->symbol);
      }
    }
  }
}


static void buildWideRefMap()
{
  //
  // build wide reference type for every reference type
  //
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->hasFlag(FLAG_REF)) {
      ClassType* wide = new ClassType(CLASS_RECORD);
      TypeSymbol* wts = new TypeSymbol(astr("__wide_", ts->cname), wide);
      wts->addFlag(FLAG_WIDE);
      theProgram->block->insertAtTail(new DefExpr(wts));
      wide->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtLocaleID)));
      wide->fields.insertAtTail(new DefExpr(new VarSymbol("addr", ts->type)));
      wideRefMap.put(ts->type, wide);
    }
  }
}


static void widenRefs()
{
  //
  // change all references into wide references
  //
  forv_Vec(DefExpr, def, gDefExprs) {
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
}


static void insertElementAccessTemps()
{
  //
  // Special case string literals passed to functions, set member primitives
  // and array element initializers by pushing them into temps first.
  //
  forv_Vec(SymExpr, se, gSymExprs) {
    if (se->var->type == dtString) {
      if (VarSymbol* var = toVarSymbol(se->var)) {
        if (var->immediate) {
          if (CallExpr* call = toCallExpr(se->parentExpr)) {
            if (call->isPrimitive(PRIM_VMT_CALL) ||
                (call->isResolved() && !call->isResolved()->hasEitherFlag(FLAG_EXTERN,FLAG_LOCAL))) {
              if (Type* type = actual_to_formal(se)->typeInfo()) {
                VarSymbol* tmp = newTemp(type);
                SET_LINENO(se);
                call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                se->replace(new SymExpr(tmp));
                call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
              }
            } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
              if (SymExpr* wide = toSymExpr(call->get(2))) {
                Type* type = wide->var->type;
                VarSymbol* tmp = newTemp(type);
                SET_LINENO(se);
                call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                se->replace(new SymExpr(tmp));
                call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
              }
            } else if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
              Type* valueType = call->get(1)->getValType();
              Type* componentType = valueType->getField("x1")->type;
              if (componentType->symbol->hasFlag(FLAG_WIDE_CLASS)) {
                VarSymbol* tmp = newTemp(componentType);
                SET_LINENO(se);
                call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                se->replace(new SymExpr(tmp));
                call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
              }
            } else if (call->isPrimitive(PRIM_ARRAY_SET_FIRST)) {
              if (SymExpr* wide = toSymExpr(call->get(3))) {
                Type* type = wide->var->type;
                VarSymbol* tmp = newTemp(wideClassMap.get(type));
                call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                se->replace(new SymExpr(tmp));
                call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
              }
            }
          }
        }
      }
    }
  }
}


static void narrowWideClassesThroughCalls()
{
  //
  // Turn calls to extern functions involving wide classes into moves
  // of the wide class into a non-wide type and then use that in the call.
  // After the call, copy the value back into the wide class.
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isResolved() && call->isResolved()->hasEitherFlag(FLAG_EXTERN,FLAG_LOCAL)) {
      for_alist(arg, call->argList) {
        SymExpr* sym = toSymExpr(arg);
        INT_ASSERT(sym);
        Type* symType = sym->typeInfo();
        if (symType->symbol->hasFlag(FLAG_WIDE_CLASS) ||
            symType->symbol->hasFlag(FLAG_WIDE)) {
          Type* narrowType = symType->getField("addr")->type;
          
          VarSymbol* var = newTemp(narrowType);
          SET_LINENO(call);
          call->getStmtExpr()->insertBefore(new DefExpr(var));
          if ((symType->symbol->hasFlag(FLAG_WIDE_CLASS) &&
               var->type->symbol->hasEitherFlag(FLAG_EXTERN,FLAG_LOCAL)) ||
              passingWideStringToExtern(narrowType)) {
            // Insert a local check because we cannot reflect any changes
            // made to the class back to another locale
            if (!fNoLocalChecks)
              call->getStmtExpr()->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, sym->copy()));
            // If we pass a extern class to an extern function, we must treat
            // it like a reference (this is by definition)
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, var, sym->copy()));
          } else if (var->type->symbol->hasEitherFlag(FLAG_REF,FLAG_DATA_CLASS))
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, var, sym->copy()));
          else
            call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, var, sym->copy()));
          call->getStmtExpr()->insertAfter(new CallExpr(PRIM_MOVE, sym->copy(), var));
          sym->replace(new SymExpr(var));
        }
      }
    }
  }
}


static void insertWideClassTempsForNil()
{
  //
  // insert wide class temps for nil
  //
  forv_Vec(SymExpr, se, gSymExprs) {
    if (se->var == gNil) {
      if (CallExpr* call = toCallExpr(se->parentExpr)) {
        if (call->isResolved()) {
          if (Type* type = actual_to_formal(se)->typeInfo()) {
            if (type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
              VarSymbol* tmp = newTemp(type);
              SET_LINENO(se);
              call->getStmtExpr()->insertBefore(new DefExpr(tmp));
              se->replace(new SymExpr(tmp));
              call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
            }
          }
        } else if (call->isPrimitive(PRIM_MOVE)) {
          if (Type* wtype = call->get(1)->typeInfo()) {
            if (wtype->symbol->hasFlag(FLAG_WIDE)) {
              if (Type* wctype = wtype->getField("addr")->type->getField("_val")->type) {
                if (wctype->symbol->hasFlag(FLAG_WIDE_CLASS)) {
                  VarSymbol* tmp = newTemp(wctype);
                  SET_LINENO(se);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  se->replace(new SymExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
                }
              }
            }
          }
        } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
          if (Type* wctype = call->get(2)->typeInfo()) {
            if (wctype->symbol->hasFlag(FLAG_WIDE_CLASS) ||
                wctype->symbol->hasFlag(FLAG_WIDE)) {
              VarSymbol* tmp = newTemp(wctype);
              SET_LINENO(se);
              call->insertBefore(new DefExpr(tmp));
              se->replace(new SymExpr(tmp));
              call->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
            }
          }
        } else if (call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
          Type* valueType = call->get(1)->getValType();
          Type* componentType = valueType->getField("x1")->type;
          if (componentType->symbol->hasFlag(FLAG_WIDE_CLASS) ||
              componentType->symbol->hasFlag(FLAG_WIDE)) {
            VarSymbol* tmp = newTemp(componentType);
            SET_LINENO(se);
            call->insertBefore(new DefExpr(tmp));
            se->replace(new SymExpr(tmp));
            call->insertBefore(new CallExpr(PRIM_MOVE, tmp, se));
          }
        } else if (call->isPrimitive(PRIM_RETURN)) {
          FnSymbol* fn = toFnSymbol(call->parentSymbol);
          INT_ASSERT(fn);
          VarSymbol* tmp = newTemp(fn->retType);
          SET_LINENO(se);
          call->insertBefore(new DefExpr(tmp));
          call->insertBefore(new CallExpr(PRIM_MOVE, tmp, gNil));
          se->var = tmp;
        }
      }
    }
  }
}


static void insertWideCastTemps()
{
  //
  // insert cast temps if lhs type does not match cast type
  //   allows separation of the remote put with the wide cast
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_CAST)) {
      if (CallExpr* move = toCallExpr(call->parentExpr)) {
        if (move->isPrimitive(PRIM_MOVE)) {
          if (move->get(1)->typeInfo() != call->typeInfo()) {
            VarSymbol* tmp = newTemp(call->typeInfo());
            SET_LINENO(call);
            move->insertBefore(new DefExpr(tmp));
            call->replace(new SymExpr(tmp));
            move->insertBefore(new CallExpr(PRIM_MOVE, tmp, call));
          }
        }
      }
    }
  }
}


static void derefWideStringActuals()
{
  //
  // dereference wide string actual argument to primitive
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol && call->primitive) {
      if (call->primitive->tag == PRIM_UNKNOWN ||
          call->isPrimitive(PRIM_CAST)) {
        for_actuals(actual, call) {
          if (actual->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
            if (actual->typeInfo()->getField("addr")->typeInfo() == dtString) {
              VarSymbol* tmp = newTemp(actual->typeInfo()->getField("addr")->typeInfo());
              SET_LINENO(call);
              call->getStmtExpr()->insertBefore(new DefExpr(tmp));
              call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_DEREF, actual->copy())));
              actual->replace(new SymExpr(tmp));
            }
          }
        }
      }
    }
  }
}


static void derefWideRefsToWideClasses()
{
  //
  // dereference wide references to wide classes in select primitives;
  // this simplifies the implementation of these primitives
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_GET_MEMBER) ||
        call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
        call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
        call->isPrimitive(PRIM_WIDE_GET_NODE) ||
        call->isPrimitive(PRIM_WIDE_GET_SUBLOC) ||
        call->isPrimitive(PRIM_WIDE_GET_ADDR) ||
        call->isPrimitive(PRIM_SET_MEMBER)) {
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
          call->get(1)->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
        VarSymbol* tmp = newTemp(call->get(1)->getValType());
        SET_LINENO(call);
        call->getStmtExpr()->insertBefore(new DefExpr(tmp));
        call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_DEREF, call->get(1)->remove())));
        call->insertAtHead(tmp);
      }
    }
  }
}


static void widenGetPrivClass()
{
  //
  // widen class types in certain primitives, e.g., GET_PRIV_CLASS
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_GET_PRIV_CLASS)) {
      SET_LINENO(call);
      if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS))
        call->get(1)->replace(new SymExpr(wideClassMap.get(call->get(1)->typeInfo())->symbol));
      else
        call->get(1)->replace(new SymExpr(call->get(1)->typeInfo()->symbol));
    }
  }
}

static void moveAddressSourcesToTemp()
{
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_MOVE)) {
      if ((call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE) ||
           call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) &&
          call->get(1)->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
          call->get(2)->typeInfo() == call->get(1)->getValType()->getField("addr")->type) {
        //
        // widen rhs class
        //
        VarSymbol* tmp = newTemp(call->get(1)->getValType());
        SET_LINENO(call);
        call->insertBefore(new DefExpr(tmp));
        call->insertBefore(new CallExpr(PRIM_MOVE, tmp, call->get(2)->remove()));
        call->insertAtTail(tmp);
      }
    }
  }
}
