#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "iterator.h"


void
expandIteratorInline(CallExpr* call) {
  BlockStmt* body;
  Symbol* index = toSymExpr(call->get(1))->var;
  Symbol* ic = toSymExpr(call->get(2))->var;
  FnSymbol* iterator = ic->type->defaultConstructor;
  BlockStmt* ibody = iterator->body->copy();
  reset_line_info(ibody, call->lineno);
  body = toBlockStmt(call->parentExpr);
  call->remove();
  body->replace(ibody);
  Vec<BaseAST*> asts;
  collect_asts(ibody, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_YIELD)) {
        BlockStmt* bodyCopy = body->copy();
        call->replace(bodyCopy);
        bodyCopy->insertAtHead(new CallExpr(PRIMITIVE_MOVE, index, call->get(1)));
      }
      if (call->isPrimitive(PRIMITIVE_RETURN)) // remove return
        call->remove();
    }
  }
  int count = 1;
  for_formals(formal, iterator) {
    forv_Vec(BaseAST, ast, asts) {
      if (SymExpr* se = toSymExpr(ast)) {
        if (se->var == formal) {
          // count is used to get the nth field out of the iterator class;
          // it is replaced by the field once the iterator class is created
          Expr* stmt = se->getStmtExpr();
          VarSymbol* tmp = new VarSymbol(formal->name, formal->type);
          tmp->addFlag(FLAG_TEMP);
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
  if (gIterator->type->symbol->hasFlag(FLAG_TUPLE)) {
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
    if (type->symbol->hasFlag(FLAG_REF))
      type = getValueType(type);
    if (type->symbol->hasFlag(FLAG_RANGE)) {
      if (!strcmp(type->substitutions.v[1].value->name, "bounded"))
        return true;
      else
        return false;
    }
  }
  return true;
}


static void
getIteratorChildren(Vec<Type*>& children, Type* type) {
  forv_Vec(Type, child, type->dispatchChildren) {
    if (child != dtObject) {
      children.add_exclusive(child);
      getIteratorChildren(children, child);
    }
  }
}


#define ZIP1 1
#define ZIP2 2
#define ZIP3 3
#define ZIP4 4
#define HASMORE 5
#define GETVALUE 6

static void
buildIteratorCallInner(BlockStmt* block, Symbol* ret, int fnid, Symbol* iterator) {
  IteratorInfo* ii = iterator->type->defaultConstructor->iteratorInfo;
  FnSymbol* fn = NULL;
  switch (fnid) {
  case ZIP1: fn = ii->zip1; break;
  case ZIP2: fn = ii->zip2; break;
  case ZIP3: fn = ii->zip3; break;
  case ZIP4: fn = ii->zip4; break;
  case HASMORE: fn = ii->hasMore; break;
  case GETVALUE: fn = ii->getValue; break;
  }
  CallExpr* call = new CallExpr(fn, iterator);
  if (ret) {
    if (fn->retType == ret->type) {
      block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, ret, call));
    } else {
      VarSymbol* tmp = new VarSymbol("_tmp", fn->retType);
      tmp->addFlag(FLAG_TEMP);
      block->insertAtTail(new DefExpr(tmp));
      block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, call));
      block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, ret, new CallExpr(PRIMITIVE_CAST, ret->type->symbol, tmp)));
    }
  } else {
    block->insertAtTail(call);
  }
}

static BlockStmt*
buildIteratorCall(Symbol* ret, int fnid, Symbol* iterator, Vec<Type*>& children) {
  BlockStmt* block = new BlockStmt();
  BlockStmt* outerBlock = block;
  forv_Vec(Type, type, children) {
    VarSymbol* cid = new VarSymbol("_tmp", dtBool);
    cid->addFlag(FLAG_TEMP);
    block->insertAtTail(new DefExpr(cid));
    block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cid,
                          new CallExpr(PRIMITIVE_GETCID,
                                       iterator, type->symbol)));
    BlockStmt* thenStmt = new BlockStmt();
    BlockStmt* elseStmt = new BlockStmt();
    VarSymbol* childIterator = new VarSymbol("_tmp", type);
    childIterator->addFlag(FLAG_TEMP);
    thenStmt->insertAtTail(new DefExpr(childIterator));
    thenStmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, childIterator, new CallExpr(PRIMITIVE_CAST, type->symbol, iterator)));
    buildIteratorCallInner(thenStmt, ret, fnid, childIterator);
    block->insertAtTail(new CondStmt(new SymExpr(cid), thenStmt, elseStmt));
    block = elseStmt;
  }
  buildIteratorCallInner(block, ret, fnid, iterator);
  return outerBlock;
}


static void
expand_for_loop(CallExpr* call) {
  BlockStmt* block = toBlockStmt(call->parentExpr);
  if (!block || block->blockInfo != call)
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
      canInlineIterator(iterator->type->defaultConstructor) &&
      (iterator->type->dispatchChildren.n == 0 ||
       iterator->type->dispatchChildren.n == 1 &&
       iterator->type->dispatchChildren.v[0] == dtObject)) {
    expandIteratorInline(call);
  } else {
    SET_LINENO(call);
    Vec<Symbol*> iterators;
    Vec<Symbol*> indices;
    setupSimultaneousIterators(iterators, indices, iterator, index, block);
    VarSymbol* firstCond = NULL;
    for (int i = 0; i < iterators.n; i++) {
      Vec<Type*> children;
      getIteratorChildren(children, iterators.v[i]->type);
      VarSymbol* cond = new VarSymbol("_cond", dtBool);
      cond->addFlag(FLAG_TEMP);

      block->insertBefore(buildIteratorCall(NULL, ZIP1, iterators.v[i], children));

      block->insertBefore(new DefExpr(cond));
      block->insertBefore(buildIteratorCall(cond, HASMORE, iterators.v[i], children));
      block->insertAtHead(buildIteratorCall(indices.v[i], GETVALUE, iterators.v[i], children));

      block->insertAtTail(buildIteratorCall(NULL, ZIP3, iterators.v[i], children));
      block->insertAtTail(buildIteratorCall(cond, HASMORE, iterators.v[i], children));
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
      block->insertAtHead(buildIteratorCall(NULL, ZIP2, iterators.v[i], children));
      block->insertAfter(buildIteratorCall(NULL, ZIP4, iterators.v[i], children));
    }
    call->get(2)->remove();
    call->get(1)->remove();
    if (firstCond)
      call->insertAtTail(firstCond);
    else
      call->insertAtTail(gTrue);
  }
}


static void
buildIterator2Leader(IteratorInfo* ii) {
  if (!ii->leader)
    return;

  FnSymbol* fn = new FnSymbol("iterator2leader");
  fn->retType = ii->leader->icType;
  ArgSymbol* iterator = new ArgSymbol(INTENT_BLANK, "iterator", ii->icType);
  fn->insertFormalAtTail(iterator);
  VarSymbol* leader = new VarSymbol("leader", ii->leader->icType);
  fn->insertAtTail(new DefExpr(leader));
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, leader, new CallExpr(PRIMITIVE_CHPL_ALLOC, ii->leader->icType->symbol, new_StringSymbol("leader iterator class"))));
  fn->insertAtTail(new CallExpr(PRIMITIVE_SETCID, leader));

  if (ii->iterator->numFormals() < ii->leader->iterator->numFormals() - 1) {
    USR_FATAL(ii->leader->iterator, "leader iterator has too many formal arguments");
  } else if (ii->iterator->numFormals() > ii->leader->iterator->numFormals() - 1) {
    USR_FATAL(ii->leader->iterator, "leader iterator has too few formal arguments");
  }

  int i = 0, j = 1;
  for_formals(leaderFormal, ii->leader->iterator) {
    i++;
    if (!strcmp(leaderFormal->name, "leader"))
      continue;
    ArgSymbol* iteratorFormal = ii->iterator->getFormal(j);
    if (strcmp(iteratorFormal->name, leaderFormal->name)) {
      USR_FATAL(ii->iterator, "name of argument %d in iterator '%s' does not match leader variant", i, ii->iterator->name);
    }
    if (iteratorFormal->type != leaderFormal->type) {
      USR_FATAL(ii->iterator, "type of argument %d in iterator '%s' does not match leader variant", i, ii->iterator->name);
    }
    VarSymbol* tmp = new VarSymbol("_tmp", iteratorFormal->type);
    tmp->addFlag(FLAG_TEMP);
    fn->insertAtTail(new DefExpr(tmp));
    fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, iterator, ii->icType->getField(j+1))));
    fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, leader, ii->leader->icType->getField(i+1), tmp));
    j++;
  }

  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, leader));
  ii->iterator2leader = fn;
  ii->getValue->defPoint->insertAfter(new DefExpr(fn));
}


static void
buildIterator2Follower(IteratorInfo* ii) {
  if (!ii->follower)
    return;

  INT_ASSERT(ii->leader);

  FnSymbol* fn = new FnSymbol("iterator2follower");
  fn->retType = ii->follower->icType;
  ArgSymbol* iterator = new ArgSymbol(INTENT_BLANK, "iterator", ii->icType);
  fn->insertFormalAtTail(iterator);
  ArgSymbol* leaderIndex = new ArgSymbol(INTENT_BLANK, "leaderIndex", ii->leader->getValue->retType);
  fn->insertFormalAtTail(leaderIndex);
  VarSymbol* follower = new VarSymbol("follower", ii->follower->icType);
  fn->insertAtTail(new DefExpr(follower));
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, follower, new CallExpr(PRIMITIVE_CHPL_ALLOC, ii->follower->icType->symbol, new_StringSymbol("follower iterator class"))));
  fn->insertAtTail(new CallExpr(PRIMITIVE_SETCID, follower));

  if (ii->iterator->numFormals() < ii->follower->iterator->numFormals() - 1) {
    USR_FATAL(ii->follower->iterator, "follower iterator has too many formal arguments");
  } else if (ii->iterator->numFormals() > ii->follower->iterator->numFormals() - 1) {
    USR_FATAL(ii->follower->iterator, "follower iterator has too few formal arguments");
  }

  int i = 0, j = 1;
  for_formals(followerFormal, ii->follower->iterator) {
    i++;
    if (!strcmp(followerFormal->name, "follower")) {
      fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, follower, ii->follower->icType->getField(i+1), leaderIndex));
      continue;
    }
    ArgSymbol* iteratorFormal = ii->iterator->getFormal(j);
    if (strcmp(iteratorFormal->name, followerFormal->name)) {
      USR_FATAL(ii->iterator, "name of argument %d in iterator '%s' does not match follower variant", i, ii->iterator->name);
    }
    if (iteratorFormal->type != followerFormal->type) {
      USR_FATAL(ii->iterator, "type of argument %d in iterator '%s' does not match follower variant", i, ii->iterator->name);
    }
    VarSymbol* tmp = new VarSymbol("_tmp", iteratorFormal->type);
    tmp->addFlag(FLAG_TEMP);
    fn->insertAtTail(new DefExpr(tmp));
    fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, iterator, ii->icType->getField(j+1))));
    fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, follower, ii->follower->icType->getField(i+1), tmp));
    j++;
  }

  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, follower));
  ii->iterator2follower = fn;
  ii->getValue->defPoint->insertAfter(new DefExpr(fn));
}


void lowerIterators() {
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast))
      if (call->parentSymbol)
        if (call->isPrimitive(PRIMITIVE_BLOCK_FOR_LOOP))
          if (call->numActuals() > 1)
            expand_for_loop(call);
  }
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->hasFlag(FLAG_ITERATOR_FN)) {
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
    if (fn->hasFlag(FLAG_ITERATOR_FN)) {
      lowerIterator(fn);
    }
  }
  // fix GET_MEMBER primitives that access fields of an iterator class
  // via a number
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->parentSymbol && call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
        ClassType* ct = toClassType(call->get(1)->typeInfo());
        if (ct->symbol->hasFlag(FLAG_REF))
          ct = toClassType(getValueType(ct));
        long num;
        if (get_int(call->get(2), &num)) {
          Symbol* field = ct->getField(num+1); // add 1 for super
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
  // iterator class (copies the dynamic iterator type)
  //
  // see functions/deitz/iterators/test_generic_use_twice2.chpl
  //
  // also build iterator2leader and iterator2follower functions
  //
  Vec<FnSymbol*> getIteratorVec;
  Map<Type*,FnSymbol*> getIteratorMap;
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->hasFlag(FLAG_ITERATOR_CLASS_COPY)) {
      getIteratorVec.add(fn);
      getIteratorMap.put(fn->retType, fn);
    } else if (fn->iteratorInfo) {
      buildIterator2Leader(fn->iteratorInfo);
      buildIterator2Follower(fn->iteratorInfo);
    }
  }

  forv_Vec(FnSymbol, fn, getIteratorVec) {
    BlockStmt* block = new BlockStmt();
    BlockStmt* outerBlock = block;
    ArgSymbol* ic = fn->getFormal(1);
    INT_ASSERT(ic);
    ClassType* ct = toClassType(ic->type);
    INT_ASSERT(ct);
    VarSymbol* cp = new VarSymbol("ic_copy", ct);
    cp->addFlag(FLAG_TEMP);
    block->insertAtTail(new DefExpr(cp));

    Vec<Type*> children;
    getIteratorChildren(children, ic->type);

    forv_Vec(Type, type, children) {
      VarSymbol* cid = new VarSymbol("_tmp", dtBool);
      cid->addFlag(FLAG_TEMP);
      block->insertAtTail(new DefExpr(cid));
      block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cid,
                            new CallExpr(PRIMITIVE_GETCID,
                                         ic, type->symbol)));
      BlockStmt* thenStmt = new BlockStmt();
      BlockStmt* elseStmt = new BlockStmt();
      VarSymbol* childIterator = new VarSymbol("_tmp", type);
      childIterator->addFlag(FLAG_TEMP);
      thenStmt->insertAtTail(new DefExpr(childIterator));
      thenStmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, childIterator, new CallExpr(PRIMITIVE_CAST, type->symbol, ic)));
      thenStmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, childIterator, new CallExpr(getIteratorMap.get(type), childIterator)));
      thenStmt->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cp, new CallExpr(PRIMITIVE_CAST, ic->type->symbol, childIterator)));
      block->insertAtTail(new CondStmt(new SymExpr(cid), thenStmt, elseStmt));
      block = elseStmt;
    }

    block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, cp, new CallExpr(PRIMITIVE_CHPL_ALLOC, ct->symbol, new_StringSymbol("iterator class copy"))));
    block->insertAtTail(new CallExpr(PRIMITIVE_SETCID, cp));
    for_fields(field, ct) {
      if (!field->hasFlag(FLAG_SUPER_CLASS)) {
        VarSymbol* tmp = new VarSymbol("_tmp", field->type);
        block->insertAtTail(new DefExpr(tmp));
        block->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, ic, field)));
        block->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, cp, field, tmp));
      }
    }
    outerBlock->insertAtTail(new CallExpr(PRIMITIVE_RETURN, cp));
    fn->body->replace(outerBlock);
  }

  //
  // insert calls to iterator2leader and iterator2follower and make
  // sure we dynamically dispatch on _getIterator
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_TO_LEADER)) {
        CallExpr* iterator2leader = new CallExpr(call->get(1)->typeInfo()->defaultConstructor->iteratorInfo->iterator2leader);
        iterator2leader->insertAtTail(call->get(1)->remove());
        call->replace(iterator2leader);
      } else if (call->isPrimitive(PRIMITIVE_TO_FOLLOWER)) {
        CallExpr* iterator2follower = new CallExpr(call->get(1)->typeInfo()->defaultConstructor->iteratorInfo->iterator2follower);
        iterator2follower->insertAtTail(call->get(1)->remove());
        iterator2follower->insertAtTail(call->get(1)->remove());
        call->replace(iterator2follower);
      }
    }
  }
}
