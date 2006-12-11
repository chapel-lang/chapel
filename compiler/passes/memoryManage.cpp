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
// insertMissingReturnTemps inserts temporary variables that are not
// used to capture the return results of all functions that return a
// value other than void; this is necessary because the symbol that is
// returned is not freed, relying instead on the free of the symbol at
// the call site that captures this value
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
      }
    }
  }
}


static bool
disableReferences(Symbol* sym) {
  if (sym->isReference)
    return true;
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
buildFreeFunctions() {
  forv_Vec(TypeSymbol, type, gTypes) {
    if (ClassType* ct = dynamic_cast<ClassType*>(type->type)) {
      FnSymbol* fn = new FnSymbol("_free");
      fn->retType = dtVoid;
      ArgSymbol* arg = new ArgSymbol(INTENT_REF, "a", ct);
      fn->insertFormalAtTail(arg);
      if (isReferenceCounted(ct)) {
        fn->insertAtTail(new CondStmt(new CallExpr(PRIMITIVE_CLASS_NULL, arg),
                                      new ReturnStmt()));
        fn->insertAtTail(new CallExpr(PRIMITIVE_GC_FREE, arg));
        fn->insertAtTail(new CondStmt(new CallExpr(PRIMITIVE_GC_ISPOS, arg),
                                      new ReturnStmt()));
        fn->insertAtTail(new CondStmt(new CallExpr(PRIMITIVE_GC_ISNEG, arg),
                                      new CallExpr(PRIMITIVE_INT_ERROR)));
      }
      type->defPoint->getModule()->stmts->insertAtTail(new DefExpr(fn));
      normalize(fn);
      freeMap.put(ct, fn);
    }
  }
  forv_Vec(TypeSymbol, type, gTypes) {
    if (ClassType* ct = dynamic_cast<ClassType*>(type->type)) {
      FnSymbol* fn = freeMap.get(ct);
      ArgSymbol* arg = fn->getFormal(1);
      for_fields(field, ct) {
        if (!disableReferences(field))
          if (FnSymbol* fieldFree = freeMap.get(field->type))
            fn->insertBeforeReturn(
              new CallExpr(fieldFree,
                new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, arg, field)));
      }
      if (isReferenceCounted(ct))
        fn->insertBeforeReturn(new CallExpr(PRIMITIVE_CHPL_FREE, arg));
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
      FnSymbol* fn = new FnSymbol("_touch");
      fn->retType = dtVoid;
      ArgSymbol* arg = new ArgSymbol(INTENT_REF, "a", ct);
      fn->insertFormalAtTail(arg);
      if (ct->classTag == CLASS_CLASS) {
        fn->insertAtTail(new CondStmt(new CallExpr(PRIMITIVE_CLASS_NULL, arg),
                                      new ReturnStmt()));
        fn->insertAtTail(new CallExpr(PRIMITIVE_GC_TOUCH, arg));
      }
      type->defPoint->getModule()->stmts->insertAtTail(new DefExpr(fn));
      normalize(fn);
      touchMap.put(ct, fn);
    }
  }
  forv_Vec(TypeSymbol, type, gTypes) {
    if (ClassType* ct = dynamic_cast<ClassType*>(type->type)) {
      if (!isReferenceCounted(ct)) {
        FnSymbol* fn = touchMap.get(ct);
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


static bool
requiresTouch(Expr* expr) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (call->isResolved()) {
      return false;
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
  if (!free_memory)
    return;
  insertMissingReturnTemps();
  buildFreeFunctions();
  buildTouchFunctions();

  forv_Vec(FnSymbol, fn, gFns) {
    Vec<BaseAST*> asts;
    collect_asts(&asts, fn->body);
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
        if (call->isPrimitive(PRIMITIVE_MOVE) ||
            call->isPrimitive(PRIMITIVE_REF)) {
          if (Symbol* lhs = dynamic_cast<SymExpr*>(call->get(1))->var) {
            if (!disableReferences(lhs)) // disable free on references--leak!!
              if (FnSymbol* _free = freeMap.get(lhs->type))
                call->insertBefore(new CallExpr(_free, lhs));
            if (FnSymbol* _touch = touchMap.get(lhs->type))
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
        }
      }
    }

    // Insert _free call on all local variables except the returned one
    if (fn->getModule()->initFn != fn) {
      forv_Vec(BaseAST, ast, asts) {
        if (DefExpr* def = dynamic_cast<DefExpr*>(ast))
          if (VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym))
            if (!disableReferences(var)) // leak!! see above
              if (FnSymbol* _free = freeMap.get(var->type))
                if (fn->getReturnSymbol() != var)
                  fn->insertBeforeReturn(new CallExpr(_free, var));
      }
    }
  }
  freeMap.clear();
  touchMap.clear();
  insertGCInits();
}
