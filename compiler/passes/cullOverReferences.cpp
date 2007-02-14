#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

static bool
isWritten(SymExpr* sym) {
  CallExpr* call = dynamic_cast<CallExpr*>(sym->parentExpr);
  if (!call)
    return false;
  if (call->isPrimitive(PRIMITIVE_MOVE) ||
      call->isPrimitive(PRIMITIVE_SET_MEMBER) ||
      call->isPrimitive(PRIMITIVE_SET_MEMBER_REF_TO))
    if (call->get(1) == sym)
      return true;
  return false;
}

static bool
isReferenced(SymExpr* sym) {
  if (CallExpr* call = dynamic_cast<CallExpr*>(sym->parentExpr)) {
    if (call->isPrimitive(PRIMITIVE_REF))
      if (call->get(2) == sym)
        return true;
    if (call->isResolved()) {
      ArgSymbol* formal = actual_to_formal(sym);
      if (formal->intent == INTENT_REF)
        return true;
      if (formal->defPoint->getFunction()->_this == formal)
        return true;
    }
  }
  CallExpr* ret = dynamic_cast<CallExpr*>(sym->getStmtExpr());
  if (ret && ret->isPrimitive(PRIMITIVE_RETURN) && sym->getFunction()->retRef)
    return true;
  return false;
}

// removes references that are not necessary including those that
// should not be references in the first place.  this happens in the
// case where an if expression evaluates to a literal, for example.
void cullOverReferences() {
  compute_sym_uses();
  compute_call_sites();
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  bool change;
  do {
    change = false;
    forv_Vec(BaseAST, ast, asts) {
      if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
        if (VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym)) {
          if (var->isReference) {
            int refCount = 0;
            int writeCount = 0;
            forv_Vec(SymExpr, sym, var->uses) {
              if (isWritten(sym))
                writeCount++;
              if (isReferenced(sym))
                refCount++;
            }
            if (FnSymbol* fn = def->getFunction()) {
              if (fn->getReturnSymbol() == var && !fn->retRef)
                writeCount = refCount = 0;
            }
            if (refCount == 0 && writeCount == 0) {
              change = true;
              var->isReference = false;
              forv_Vec(SymExpr, sym, var->uses) {
                if (CallExpr* call = dynamic_cast<CallExpr*>(sym->parentExpr))
                  if (call->isPrimitive(PRIMITIVE_REF))
                    if (sym == call->get(1))
                      call->primitive = primitives[PRIMITIVE_MOVE];
              }
            }
          }
        } else if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def->sym)) {
          if (fn->retRef) {
            int refCount = 0;
            forv_Vec(CallExpr, call, *fn->calledBy) {
              if (CallExpr* ref = dynamic_cast<CallExpr*>(call->parentExpr))
                if (ref->isPrimitive(PRIMITIVE_REF))
                  if (ref->get(2) == call)
                    refCount++;
            }
            if (refCount == 0) {
              change = true;
              fn->retRef = false;
            }
          }
        }
      }
    }
  } while (change);
}
