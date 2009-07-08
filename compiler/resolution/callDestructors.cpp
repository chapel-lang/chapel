#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"


static void
fixupDestructors() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_DESTRUCTOR)) {
      ClassType* ct = toClassType(fn->_this->type);
      if (ct->symbol->hasFlag(FLAG_REF))
        ct = toClassType(ct->getValueType());
      INT_ASSERT(ct);

      //
      // insert calls to destructors for all 'value' fields
      //
      for_fields_backward(field, ct) {
        if (field->type->destructor) {
          ClassType* fct = toClassType(field->type);
          INT_ASSERT(fct);
          if (!isClass(fct) || fct->symbol->hasFlag(FLAG_SYNC)) {
            bool useRefType = // !fct->symbol->hasFlag(FLAG_ARRAY) && !fct->symbol->hasFlag(FLAG_DOMAIN) &&
              !fct->symbol->hasFlag(FLAG_SYNC);
            VarSymbol* tmp = useRefType ? newTemp(fct->refType) : newTemp(fct);
            fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
            fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp,
              new CallExpr(useRefType ? PRIM_GET_MEMBER : PRIM_GET_MEMBER_VALUE, fn->_this, field)));
            fn->insertBeforeReturnAfterLabel(new CallExpr(field->type->destructor, tmp));
            if (fct->symbol->hasFlag(FLAG_SYNC))
              fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_CHPL_FREE, tmp));
          }
        } else if (field->type == dtString && !ct->symbol->hasFlag(FLAG_TUPLE)) {
          VarSymbol* tmp = newTemp(dtString);
          fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
          fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp,
            new CallExpr(PRIM_GET_MEMBER_VALUE, fn->_this, field)));
          fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_CHPL_FREE, tmp));
        }
      }

      //
      // insert call to parent destructor
      //
      INT_ASSERT(ct->dispatchParents.n <= 1);
      if (ct->dispatchParents.n >= 1 && isClass(ct)) {
        // avoid destroying record fields more than once
        if (FnSymbol* parentDestructor = ct->dispatchParents.v[0]->destructor) {
          Type* tmpType = isClass(ct) // || ct->symbol->hasFlag(FLAG_ARRAY) || ct->symbol->hasFlag(FLAG_DOMAIN)
            ?
            ct->dispatchParents.v[0] : ct->dispatchParents.v[0]->refType;
          VarSymbol* tmp = newTemp(tmpType);
          fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
          fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp,
            new CallExpr(PRIM_CAST, tmpType->symbol, fn->_this)));
          fn->insertBeforeReturnAfterLabel(new CallExpr(parentDestructor, tmp));
        }
      }
    }
  }
}


void
callDestructors() {
  fixupDestructors();
}
