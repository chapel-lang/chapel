#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

static Map<Type*,FnSymbol*> freeMap;  // map of types to _free functions
static Map<Type*,FnSymbol*> touchMap; // map of types to _touch functions

//
// insertGCTemps inserts two sets of temporaries: (1) temporary
// variables (_gc_ret_tmp) to capture the return results of all
// functions that return a value other than void but that are not
// captured; this is necessary because the symbol that is returned is
// not freed, relying instead on the free of the symbol at the call
// site that captures this value, and (2) temporary variables
// (_gc_norm_tmp) to capture the rhs of a move if the symbol on the
// lhs is part of the rhs since otherwise a free of the lhs symbol
// would potentially free it before it is used on the rhs
//
static void
insertMissingReturnTemps() {
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (FnSymbol* fn = call->isResolved()) {
        if (fn->retType != dtVoid) {
          CallExpr* parent = dynamic_cast<CallExpr*>(call->parentExpr);
          if (!parent) { // no move
            VarSymbol* tmp = new VarSymbol("_gc_ret_tmp", fn->retType);
            DefExpr* def = new DefExpr(tmp);
            call->insertBefore(def);
            def->insertAfter(new CallExpr(PRIMITIVE_MOVE, tmp, call->remove()));
          }
        }
      } else if (call->isPrimitive(PRIMITIVE_MOVE)) {
        if (Symbol* lhs = dynamic_cast<SymExpr*>(call->get(1))->var) {
          if (CallExpr* rhs = dynamic_cast<CallExpr*>(call->get(2))) {
            bool requires_temp = false;
            for_actuals(actual, rhs) {
              if (SymExpr* sym = dynamic_cast<SymExpr*>(actual)) {
                if (sym->var == lhs) {
                  requires_temp = true;
                }
              }
            }
            if (requires_temp) {
              VarSymbol* tmp = new VarSymbol("_gc_norm_tmp", lhs->type);
              DefExpr* def = new DefExpr(tmp);
              call->insertBefore(def);
              call->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, rhs->remove()));
              call->insertAtTail(new SymExpr(tmp));
            }
          }
        }
      }
    }
  }
}


static bool
disableReferences(Symbol* sym) {
  if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym))
    if (var->is_ref || var->on_heap)
      return true;
  return false;
}


//
// isReferenceCounted(type) returns true if the type uses reference
// counting to free its associated dynamic memory
//
static bool
isReferenceCounted(Type* t) {
  if (ClassType* ct = dynamic_cast<ClassType*>(t))
    if (ct->classTag == CLASS_CLASS)
      return true;
  return false;
}


//
// buildFreeFunctions() builds _free functions for all structured
// types
//
static void
insertSubclassFrees(ClassType* ct, FnSymbol* fn, ArgSymbol* arg, Symbol* ret) {
  forv_Vec(Type, t, ct->dispatchChildren) {
    if (ClassType* sct = dynamic_cast<ClassType*>(t)) {
      BlockStmt* block = new BlockStmt();
      VarSymbol* tmp = new VarSymbol("tmp", sct);
      block->insertAtTail(new DefExpr(tmp));
      block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_CAST, sct->symbol, arg)));
      block->insertAtTail(new CallExpr(freeMap.get(sct), tmp));
      block->insertAtTail(new GotoStmt(goto_normal, ret));
      fn->body->body->head->insertAfter(new CondStmt(new CallExpr(PRIMITIVE_GETCID, arg, new_IntSymbol(sct->id)), block));
    }
  }
}

static bool
requiresFreeFunction(ClassType* ct) {
  if (ct->symbol->hasPragma("no gc"))
    return false;
  if (freeMap.get(ct))
    return false;
  if (ct->classTag == CLASS_CLASS)
    return true;
  for_fields(field, ct) {
    if (freeMap.get(field->type))
      return true;
  }
  return false;
}

static void
buildFreeFunctions() {
  bool change = true;
  while (change) {
    change = false;
    forv_Vec(TypeSymbol, type, gTypes) {
      if (ClassType* ct = dynamic_cast<ClassType*>(type->type)) {
        if (requiresFreeFunction(ct)) {
          FnSymbol* fn = new FnSymbol("_free");
          fn->isCompilerTemp = true;
          fn->retType = dtVoid;
          ArgSymbol* arg = new ArgSymbol(INTENT_REF, "a", ct);
          fn->insertFormalAtTail(arg);
          if (isReferenceCounted(ct)) {
            fn->insertAtTail(new CondStmt(new CallExpr(PRIMITIVE_CLASS_NULL, arg),
                                          new CallExpr(PRIMITIVE_RETURN, gVoid)));
            fn->insertAtTail(new CallExpr(PRIMITIVE_GC_FREE, arg));
            fn->insertAtTail(new CondStmt(new CallExpr(PRIMITIVE_GC_ISPOS, arg),
                                          new CallExpr(PRIMITIVE_RETURN, gVoid)));
            fn->insertAtTail(new CondStmt(new CallExpr(PRIMITIVE_GC_ISNEG, arg),
                                          new CallExpr(PRIMITIVE_INT_ERROR)));
          }
          type->defPoint->getModule()->stmts->insertAtTail(new DefExpr(fn));
          normalize(fn);
          freeMap.put(ct, fn);
          change = true;
        }
      }
    }
  }
  forv_Vec(TypeSymbol, type, gTypes) {
    if (ClassType* ct = dynamic_cast<ClassType*>(type->type)) {
      if (FnSymbol* fn = freeMap.get(ct)) {
        ArgSymbol* arg = fn->getFormal(1);
        Symbol* ret = fn->getReturnLabel();
        if (ct->classTag == CLASS_CLASS)
          insertSubclassFrees(ct, fn, arg, ret);
        for_fields(field, ct) {
          if (!disableReferences(field))
            if (FnSymbol* fieldFree = freeMap.get(field->type))
              fn->insertBeforeReturn(
                new CallExpr(fieldFree,
                  new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, arg, field)));
        }
        if (ct->symbol->hasPragma("data class")) {
          Type* eltType = dynamic_cast<Type*>(ct->substitutions.v[0].value);
          if (!eltType)
            INT_FATAL(ct, "bad data class element type");
          if (FnSymbol* eltFree = freeMap.get(eltType)) {
            VarSymbol* ivar = new VarSymbol("_tmp", dtInt[INT_SIZE_32]);
            fn->insertBeforeReturn(new DefExpr(ivar));
            fn->insertBeforeReturn(new CallExpr(PRIMITIVE_ARRAY_FREE_ELTS,
                                                arg, ivar,
                                                new CallExpr(eltFree, new CallExpr(PRIMITIVE_ARRAY_GET_VALUE, arg, ivar))));
          }
          fn->insertBeforeReturn(new CallExpr(PRIMITIVE_ARRAY_FREE, arg));
        }
        if (isReferenceCounted(ct))
          fn->insertBeforeReturn(new CallExpr(PRIMITIVE_CHPL_FREE, arg));
      }
    }
  }
}


//
// buildTouchFunctions() builds _touch functions for all structured
// types
//
static void
buildTouchFunctions() {
  forv_Vec(TypeSymbol, type, gTypes) {
    if (ClassType* ct = dynamic_cast<ClassType*>(type->type)) {
      if (freeMap.get(ct)) {
        FnSymbol* fn = new FnSymbol("_touch");
        fn->isCompilerTemp = true;
        fn->retType = dtVoid;
        ArgSymbol* arg = new ArgSymbol(INTENT_REF, "a", ct);
        fn->insertFormalAtTail(arg);
        if (ct->classTag == CLASS_CLASS) {
          fn->insertAtTail(new CondStmt(new CallExpr(PRIMITIVE_CLASS_NULL, arg),
                                        new CallExpr(PRIMITIVE_RETURN, gVoid)));
          fn->insertAtTail(new CallExpr(PRIMITIVE_GC_TOUCH, arg));
        }
        type->defPoint->getModule()->stmts->insertAtTail(new DefExpr(fn));
        normalize(fn);
        touchMap.put(ct, fn);
      }
    }
  }
  forv_Vec(TypeSymbol, type, gTypes) {
    if (ClassType* ct = dynamic_cast<ClassType*>(type->type)) {
      if (!isReferenceCounted(ct)) {
        if (FnSymbol* fn = touchMap.get(ct)) {
          ArgSymbol* arg = fn->getFormal(1);
          for_fields(field, ct) {
            if (FnSymbol* fieldTouch = touchMap.get(field->type))
              fn->insertBeforeReturn(
                new CallExpr(fieldTouch,
                  new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, arg, field)));
          }
        }
      }
    }
  }
}


static bool
requiresTouch(Expr* expr) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (FnSymbol* fn = call->isResolved()) {
      Symbol* symbol = fn->getReturnSymbol();
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(symbol)) {
        if (!var->isReference)
          return false;
      }
    }
  }
  return true;
}


static void
insertGCInits(void) {
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isPrimitive(PRIMITIVE_CHPL_ALLOC)) {
        if (CallExpr* parent = dynamic_cast<CallExpr*>(call->parentExpr)) {
          if (parent->isPrimitive(PRIMITIVE_MOVE)) {
            if (SymExpr* sym = dynamic_cast<SymExpr*>(parent->get(1))) {
              if (isReferenceCounted(sym->var->type)) {
                parent->insertAfter(new CallExpr(PRIMITIVE_GC_INIT, sym->copy()));
              }
            }
          }
        }
      }
    }
  }
}


void memoryManage(void) {
  if (no_gc)
    return;
  insertMissingReturnTemps();
  buildFreeFunctions();
  buildTouchFunctions();

  forv_Vec(FnSymbol, fn, gFns) {
    Vec<BaseAST*> asts;
    collect_top_asts(&asts, fn->body);
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
        if (call->isPrimitive(PRIMITIVE_MOVE)) {
          if (Symbol* lhs = dynamic_cast<SymExpr*>(call->get(1))->var) {
            if (!disableReferences(lhs)) // disable on_heap/is_ref
              if (FnSymbol* _free = freeMap.get(lhs->type))
                call->insertBefore(new CallExpr(_free, lhs));
            if (FnSymbol* _touch = touchMap.get(lhs->type))
              if (!disableReferences(lhs)) // disable on_heap/is_ref
                if (requiresTouch(call->get(2)))
                  call->insertAfter(new CallExpr(_touch, lhs));
          }
        } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
          if (Symbol* lhs = dynamic_cast<SymExpr*>(call->get(3))->var) {
            if (FnSymbol* _free = freeMap.get(lhs->type))
              if (fn->fnClass != FN_CONSTRUCTOR) // first set
                call->insertBefore(
                  new CallExpr(_free,
                    new CallExpr(PRIMITIVE_GET_MEMBER_VALUE,
                                 call->get(1)->copy(),
                                 call->get(2)->copy())));
            if (FnSymbol* _touch = touchMap.get(lhs->type))
              call->insertAfter(
                new CallExpr(_touch,
                  new CallExpr(PRIMITIVE_GET_MEMBER_VALUE,
                               call->get(1)->copy(),
                               call->get(2)->copy())));
          }
        } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER_REF_TO)) {
          if (Symbol* lhs = dynamic_cast<SymExpr*>(call->get(3))->var) {
            // need to free -- parallel leak?
            if (FnSymbol* _touch = touchMap.get(lhs->type))
              call->insertAfter(
                new CallExpr(_touch, call->get(3)->copy()));
          }
        } else if (call->isPrimitive(PRIMITIVE_ARRAY_SET)) {
          if (Symbol* lhs = dynamic_cast<SymExpr*>(call->get(3))->var) {
            if (FnSymbol* _free = freeMap.get(lhs->type))
              call->insertBefore(
                new CallExpr(_free,
                  new CallExpr(PRIMITIVE_ARRAY_GET_VALUE,
                               call->get(1)->copy(),
                               call->get(2)->copy())));
            if (FnSymbol* _touch = touchMap.get(lhs->type))
              call->insertAfter(
                new CallExpr(_touch,
                  new CallExpr(PRIMITIVE_ARRAY_GET_VALUE,
                               call->get(1)->copy(),
                               call->get(2)->copy())));
          }
        } else if (call->isPrimitive(PRIMITIVE_ARRAY_SET_FIRST)) {
          if (Symbol* lhs = dynamic_cast<SymExpr*>(call->get(3))->var) {
            if (FnSymbol* _touch = touchMap.get(lhs->type))
              call->insertAfter(
                new CallExpr(_touch,
                  new CallExpr(PRIMITIVE_ARRAY_GET_VALUE,
                               call->get(1)->copy(),
                               call->get(2)->copy())));
          }
        }
      }
    }

    // Insert _free call on all local variables except the returned one
    if (fn->getModule()->initFn != fn) {
      forv_Vec(BaseAST, ast, asts) {
        if (DefExpr* def = dynamic_cast<DefExpr*>(ast))
          if (VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym))
            if (!disableReferences(var)) // disable on_heap/is_ref
              if (!var->isReference) // do not free aliases
                if (FnSymbol* _free = freeMap.get(var->type))
                  if (fn->getReturnSymbol() != var)
                    fn->insertBeforeReturnAfterLabel(new CallExpr(_free, var));
      }
    }
  }
  freeMap.clear();
  touchMap.clear();
  insertGCInits();
}
