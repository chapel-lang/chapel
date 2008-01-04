//
// complex2record
//
// This pass changes the primitive complex type to a record.  As an
// optimization, unimplemented, it can be turned off to take advantage
// of C99 support of complex types.
//

#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"

static ClassType*
buildComplexRecord(const char* name, Type* real) {
  ClassType* ct = new ClassType(CLASS_RECORD);
  TypeSymbol* ts = new TypeSymbol(name, ct);
  ts->addPragma("defined in runtime");
  ct->fields.insertAtTail(new DefExpr(new VarSymbol("re", real)));
  ct->fields.insertAtTail(new DefExpr(new VarSymbol("im", real)));
  theProgram->block->insertAtTail(new DefExpr(ts));
  return ct;
}

#define complex2rec(t)                                          \
  ((t == dtComplex[COMPLEX_SIZE_64]) ? complex64 :              \
   ((t == dtComplex[COMPLEX_SIZE_128]) ? complex128 :           \
    ((t == dtComplex[COMPLEX_SIZE_256]) ? complex256 : 0)))

#define complex2real(t)                                                 \
  ((t == dtComplex[COMPLEX_SIZE_64]) ? dtReal[FLOAT_SIZE_32] :         \
   ((t == dtComplex[COMPLEX_SIZE_128]) ? dtReal[FLOAT_SIZE_64] :       \
    ((t == dtComplex[COMPLEX_SIZE_256]) ? dtReal[FLOAT_SIZE_128] : 0)))

void
complex2record() {
  ClassType* complex64 = buildComplexRecord("_complex64", dtReal[FLOAT_SIZE_32]);
  ClassType* complex128 = buildComplexRecord("_complex128", dtReal[FLOAT_SIZE_64]);
  ClassType* complex256 = buildComplexRecord("_complex256", dtReal[FLOAT_SIZE_128]);
  complex64->refType = dtComplex[COMPLEX_SIZE_64]->refType;
  complex128->refType = dtComplex[COMPLEX_SIZE_128]->refType;
  complex256->refType = dtComplex[COMPLEX_SIZE_256]->refType;


  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (is_complex_type(def->sym->type))
        def->sym->type = complex2rec(def->sym->type);
      if (FnSymbol* fn = toFnSymbol(def->sym))
        if (is_complex_type(fn->retType))
          fn->retType = complex2rec(fn->retType);
    } else if (SymExpr* se = toSymExpr(ast)) {
      if (VarSymbol* var = toVarSymbol(se->var)) {
        if (is_complex_type(var->type)) {
          if (var->immediate) {
            ClassType* ct = complex2rec(se->var->type);
            VarSymbol* tmp = new VarSymbol("_tmp", ct);
            se->getStmtExpr()->insertBefore(new DefExpr(tmp));
            se->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, tmp, ct->getField(1), complex2real(se->var->type)->defaultValue));
            se->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, tmp, ct->getField(2), complex2real(se->var->type)->defaultValue));
            se->replace(new SymExpr(tmp));
          }
        }
      }
    }
  }

  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_GET_REAL)) {
        call->primitive = primitives[PRIMITIVE_GET_MEMBER];
        ClassType* ct = toClassType(call->get(1)->typeInfo());
        if (isReference(ct))
          ct = toClassType(getValueType(ct));
        call->insertAtTail(ct->getField(1));
      } else if (call->isPrimitive(PRIMITIVE_GET_IMAG)) {
        call->primitive = primitives[PRIMITIVE_GET_MEMBER];
        ClassType* ct = toClassType(call->get(1)->typeInfo());
        if (isReference(ct))
          ct = toClassType(getValueType(ct));
        call->insertAtTail(ct->getField(2));
      }
    }
  }

}
