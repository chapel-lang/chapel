#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "iterator.h"


static void
expandIteratorInline(CallExpr* call) {
  BlockStmt* body;
  Symbol* index = toSymExpr(call->get(1))->var;
  Symbol* ic = toSymExpr(call->get(2))->var;
  FnSymbol* iterator = ic->type->defaultConstructor;
  ASTMap map;
  BlockStmt* ibody = iterator->body->copy(&map);
  CallExpr* yield = NULL;
  reset_file_info(ibody, call->lineno, call->filename);
  body = toBlockStmt(call->parentExpr);
  call->remove();
  body->replace(ibody);
  Vec<BaseAST*> asts;
  collect_asts(&asts, ibody);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_YIELD)) {
        yield = call;
        call->replace(body);
      }
      if (call->isPrimitive(PRIMITIVE_RETURN)) // remove return
        call->remove();
    }
  }
  body->insertAtHead(new CallExpr(PRIMITIVE_MOVE, index, yield->get(1)));
  int count = 0;
  for_formals(formal, iterator) {
    VarSymbol* var = new VarSymbol(formal->name, formal->type);
    // count is used to get the nth field out of the iterator class;
    // it is replaced by the field once the iterator class is created
    CallExpr* access = new CallExpr(isReference(formal->type) ? PRIMITIVE_GET_MEMBER : PRIMITIVE_GET_MEMBER_VALUE, ic, new_IntSymbol(++count));
    ibody->insertAtHead(new CallExpr(PRIMITIVE_MOVE, var, access));
    ibody->insertAtHead(new DefExpr(var));
    forv_Vec(BaseAST, ast, asts) {
      if (SymExpr* se = toSymExpr(ast)) {
        if (se->var == formal) {
          se->var = var;
        }
      }
    }
  }
}


void lowerIterators() {
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_LOOP_INLINE)) {
        expandIteratorInline(call);
      }
    }
  }
  if (!fBaseline) {
    forv_Vec(FnSymbol, fn, gFns) {
      if (fn->fnClass == FN_ITERATOR) {
        collapseBlocks(fn->body);
        removeUnnecessaryGotos(fn);
        localCopyPropagation(fn);
        deadVariableElimination(fn);
        deadExpressionElimination(fn);
      }
    }
  }
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->fnClass == FN_ITERATOR) {
      lowerIterator(fn);
    }
  }
  // fix GET_MEMBER primitives that access fields of an iterator class
  // via a number
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
          call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)) {
        ClassType* ct = toClassType(call->get(1)->typeInfo());
        long num;
        if (get_int(call->get(2), &num)) {
          Symbol* field = ct->getField(num);
          call->get(2)->replace(new SymExpr(field));
          if (CallExpr* parent = toCallExpr(call->parentExpr))
            if (parent->isPrimitive(PRIMITIVE_MOVE))
              if (isReference(parent->get(1)->typeInfo()) &&
                  isReference(field->type))
                call->primitive = primitives[PRIMITIVE_GET_MEMBER];
        }
      }
    }
  }
}
