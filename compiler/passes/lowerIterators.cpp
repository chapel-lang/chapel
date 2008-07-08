#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "iterator.h"


static void
expandIteratorInline(CallExpr* call) {
  BlockStmt* body;
  Symbol* index = toSymExpr(call->get(1))->var;
  Symbol* ic = toSymExpr(call->get(2))->var;
  FnSymbol* iterator = ic->type->defaultConstructor;
  BlockStmt* ibody = iterator->body->copy();
  CallExpr* yield = NULL;
  reset_line_info(ibody, call->lineno);
  body = toBlockStmt(call->parentExpr);
  call->remove();
  body->replace(ibody);
  Vec<BaseAST*> asts;
  collect_asts(ibody, asts);
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
  int count = 1;
  for_formals(formal, iterator) {
    forv_Vec(BaseAST, ast, asts) {
      if (SymExpr* se = toSymExpr(ast)) {
        if (se->var == formal) {
          // count is used to get the nth field out of the iterator class;
          // it is replaced by the field once the iterator class is created
          Expr* stmt = se->getStmtExpr();
          VarSymbol* tmp = new VarSymbol(formal->name, formal->type);
          tmp->isCompilerTemp = true;
          stmt->insertBefore(new DefExpr(tmp));
          stmt->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER, ic, new_IntSymbol(count))));
          se->var = tmp;
        }
      }
    }
    count++;
  }
}


static bool
canInlineIterator(FnSymbol* iterator) {
  Vec<BaseAST*> asts;
  collect_asts(iterator, asts);
  int count = 0;
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast))
      if (call->isPrimitive(PRIMITIVE_YIELD))
        count++;
  }
  if (count == 1)
    return true;
  return false;
}

static void
setupSimultaneousIterators(Vec<Symbol*>& iterators,
                           Vec<Symbol*>& indices,
                           Symbol* gIterator,
                           Symbol* gIndex,
                           BlockStmt* loop) {
  if (gIterator->type->symbol->hasPragma("tuple")) {
    ClassType* iteratorType = toClassType(gIterator->type);
    ClassType* indexType = toClassType(gIndex->type);
    for (int i=1; i <= iteratorType->fields.length(); i++) {
      Symbol* iterator = new VarSymbol("_iterator", iteratorType->getField(i)->type);
      loop->insertBefore(new DefExpr(iterator));
      loop->insertBefore(new CallExpr(PRIMITIVE_MOVE, iterator, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, gIterator, iteratorType->getField(i))));

      Symbol* index = new VarSymbol("_index", indexType->getField(i)->type);
      loop->insertAtHead(new CallExpr(PRIMITIVE_SET_MEMBER, gIndex, indexType->getField(i), index));
      loop->insertAtHead(new DefExpr(index));
      setupSimultaneousIterators(iterators, indices, iterator, index, loop);
    }
  } else {
    iterators.add(gIterator);
    indices.add(gIndex);
  }
}


static bool
isBoundedIterator(FnSymbol* fn) {
  if (fn->_this) {
    Type* type = fn->_this->type;
    if (type->symbol->hasPragma("ref"))
      type = getValueType(type);
    if (type->symbol->hasPragma("range")) {
      if (!strcmp(toSymbol(type->substitutions.v[1].value)->name, "bounded"))
        return true;
      else
        return false;
    }
  }
  return true;
}


static void
expand_for_loop(CallExpr* call) {
  BlockStmt* block = toBlockStmt(call->parentExpr);
  if (!block || block->loopInfo != call)
    INT_FATAL(call, "bad for loop primitive");
  SymExpr* se1 = toSymExpr(call->get(1));
  SymExpr* se2 = toSymExpr(call->get(2));
  if (!se1 || !se2)
    INT_FATAL(call, "bad for loop primitive");
  VarSymbol* index = toVarSymbol(se1->var);
  VarSymbol* iterator = toVarSymbol(se2->var);
  if (!index || !iterator)
    INT_FATAL(call, "bad for loop primitive");

  if (!fNoInlineIterators &&
      iterator->type->defaultConstructor->iteratorInfo &&
      canInlineIterator(iterator->type->defaultConstructor)) {
    expandIteratorInline(call);
  } else {
    currentLineno = call->lineno;
    Vec<Symbol*> iterators;
    Vec<Symbol*> indices;
    setupSimultaneousIterators(iterators, indices, iterator, index, block);
    VarSymbol* firstCond = NULL;
    for (int i = 0; i < iterators.n; i++) {
      IteratorInfo* ii = iterators.v[i]->type->defaultConstructor->iteratorInfo;
      VarSymbol* cond = new VarSymbol("_cond", dtBool);
      cond->isCompilerTemp = true;
      block->insertBefore(new CallExpr(ii->zip1, iterators.v[i]));
      block->insertBefore(new DefExpr(cond));
      block->insertBefore(new CallExpr(PRIMITIVE_MOVE, cond, new CallExpr(ii->hasMore, iterators.v[i])));
      block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, indices.v[i], new CallExpr(ii->getValue, iterators.v[i])));

      block->insertAtTail(new CallExpr(ii->zip3, iterators.v[i]));
      block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cond, new CallExpr(ii->hasMore, iterators.v[i])));
      if (isBoundedIterator(iterators.v[i]->type->defaultConstructor)) {
        if (!firstCond) {
          firstCond = cond;
        } else if (!fNoBoundsChecks) {
          VarSymbol* tmp = new VarSymbol("_tmp", dtBool);
          block->insertAtHead(new CondStmt(new SymExpr(tmp), new CallExpr(PRIMITIVE_RT_ERROR, new_StringSymbol("zippered iterations have non-equal lengths"))));
          block->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_EQUAL, cond, new_IntSymbol(0))));
          block->insertAtHead(new DefExpr(tmp));
          block->insertAfter(new CondStmt(new SymExpr(cond), new CallExpr(PRIMITIVE_RT_ERROR, new_StringSymbol("zippered iterations have non-equal lengths"))));
        }
      }
      block->insertAtHead(new CallExpr(ii->zip2, iterators.v[i]));
      block->insertAfter(new CallExpr(ii->zip4, iterators.v[i]));
    }
    call->get(2)->remove();
    call->get(1)->remove();
    if (firstCond)
      call->insertAtTail(firstCond);
    else
      call->insertAtTail(gTrue);
  }
}


void lowerIterators() {
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast))
      if (call->isPrimitive(PRIMITIVE_LOOP_FOR))
        if (call->numActuals() > 1)
          expand_for_loop(call);
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
      if (call->parentSymbol && call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        ClassType* ct = toClassType(call->get(1)->typeInfo());
        if (ct->symbol->hasPragma("ref"))
          ct = toClassType(getValueType(ct));
        long num;
        if (get_int(call->get(2), &num)) {
          Symbol* field = ct->getField(num);
          call->get(2)->replace(new SymExpr(field));
          CallExpr* parent = toCallExpr(call->parentExpr);
          INT_ASSERT(parent->isPrimitive(PRIMITIVE_MOVE));
          Symbol* local = toSymExpr(parent->get(1))->var;
          if (local->type == field->type)
            call->primitive = primitives[PRIMITIVE_GET_MEMBER_VALUE];
          else if (local->type != field->type->refType)
            INT_FATAL(call, "unexpected case");
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
