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
  if (call->isPrimitive(PRIMITIVE_MOVE))
    if (call->get(1) == sym)
      return true;
  return false;
}

static bool
isReferenced(SymExpr* sym) {
  CallExpr* call = dynamic_cast<CallExpr*>(sym->parentExpr);
  if (!call)
    return true; // maybe, e.g., in a return stmt
  if (call->isPrimitive(PRIMITIVE_MOVE))
    if (call->get(2) == sym)
      if (SymExpr* lhs = dynamic_cast<SymExpr*>(call->get(1)))
        if (lhs->var->isReference)
          return true;
  if (call->isResolved())
    if (actual_to_formal(sym)->intent == INTENT_REF)
      return true;
  return false;
}

// removes references that are not necessary including those that
// should not be references in the first place.  this happens in the
// case where an if expression evaluates to a literal, for example.
void cullOverReferences() {
  compute_sym_uses();
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
            if (refCount == 0 && writeCount <= 1) {
              change = true;
              var->isReference = false;
            } else if (refCount == 0 && writeCount == 2) {
              // hack for if_fn handling of if expressions
              if (!strncmp("_ret__if_fn", var->cname, 11)) {
                change = true;
                var->isReference = false;
              }
            }
          }
        }
      }
    }
  } while (change);
}
