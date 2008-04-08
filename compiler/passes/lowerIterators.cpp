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
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->fnTag == FN_ITERATOR) {
      collapseBlocks(fn->body);
      removeUnnecessaryGotos(fn);
      if (!fNoCopyPropagation)
        localCopyPropagation(fn);
      if (!fNoDeadCodeElimination) {
        deadVariableElimination(fn);
        deadExpressionElimination(fn);
      }
    }
  }
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->fnTag == FN_ITERATOR) {
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

  //
  // make _getIterator(ic: _iteratorClass) implement a shallow copy to
  // avoid clashing during simultaneous iterations of the same
  // iterator class
  //
  // see functions/deitz/iterators/test_generic_use_twice2.chpl
  //
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->hasPragma("iterator class copy")) {
      BlockStmt* block = new BlockStmt();
      ArgSymbol* ic = fn->getFormal(1);
      INT_ASSERT(ic);
      ClassType* ct = toClassType(ic->type);
      INT_ASSERT(ct);
      VarSymbol* cp = new VarSymbol("ic_copy", ct);
      cp->isCompilerTemp = true;
      block->insertAtTail(new DefExpr(cp));
      block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cp, new CallExpr(PRIMITIVE_CHPL_ALLOC, ct->symbol, new_StringSymbol("iterator class copy"))));
      for_fields(field, ct) {
        VarSymbol* tmp = new VarSymbol("_tmp", field->type);
        block->insertAtTail(new DefExpr(tmp));
        block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, ic, field)));
        block->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, cp, field, tmp));
      }
      block->insertAtTail(new CallExpr(PRIMITIVE_RETURN, cp));
      fn->body->replace(block);
    }
  }
}
