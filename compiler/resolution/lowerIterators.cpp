#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "resolution.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "iterator.h"


//
// Declare a set of recursive iterators and create a function to
// compute this set.  This set is used to make sure we only inline the
// body of a loop into an iterator, rather than the whole iterator, in
// places where we would otherwise attempt to inline a recursive
// iterator.
//
static Vec<FnSymbol*> recursiveIteratorSet;
static void computeRecursiveIteratorSet() {
  compute_call_sites();

  forv_Vec(FnSymbol, ifn, gFnSymbols) {
    if (ifn->hasFlag(FLAG_ITERATOR_FN)) {
      bool recursive = false;
      Vec<FnSymbol*> fnSet;
      Vec<FnSymbol*> fnVec;
      fnSet.set_add(ifn);
      fnVec.add(ifn);
      forv_Vec(FnSymbol, fn, fnVec) {
        forv_Vec(CallExpr, call, *fn->calledBy) {
          FnSymbol* parent = toFnSymbol(call->parentSymbol);
          INT_ASSERT(parent);
          if (parent == ifn) {
            recursive = true;
            break;
          } else if (!fnSet.set_in(parent)) {
            fnSet.set_add(parent);
            fnVec.add(parent);
          }
        }
        if (recursive)
          break;
      }
      if (recursive)
        recursiveIteratorSet.set_add(ifn);
    }
  }
}


//
// If a local block has no yields, returns, gotos or labels it can safely
// be left unfragmented.
//
static bool leaveLocalBlockUnfragmented(BlockStmt* block) {
  Vec<BaseAST*> asts;
  collect_asts(block, asts);

  forv_Vec(BaseAST, ast, asts) {
    CallExpr* call = toCallExpr(ast);
    DefExpr* def = toDefExpr(ast);
    if ((call && call->isPrimitive(PRIM_YIELD))  ||
        (call && call->isPrimitive(PRIM_RETURN)) ||
         isGotoStmt(ast) || (def && isLabelSymbol(def->sym))) {
      return false;
    }
  }

  return true;
}


//
// replace a local block by smaller local blocks that do not contain
// goto statements, yields, and returns.
//
static void
fragmentLocalBlocks() {
  Vec<Expr*> preVec; // stmts to be inserted before new local blocks
  Vec<Expr*> inVec;  // stmts to be inserted in new local blocks

  //
  // collect all local blocks into localBlocks vector
  //
  Vec<BlockStmt*> localBlocks; // old local blocks
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (block->blockInfo && block->blockInfo->isPrimitive(PRIM_BLOCK_LOCAL) && !leaveLocalBlockUnfragmented(block))
      localBlocks.add(block);
  }

  //
  // collect first statements of local blocks into queue vector
  //
  Vec<Expr*> queue;
  forv_Vec(BlockStmt, block, localBlocks) {
    if (block->body.head)
      queue.add(block->body.head);
  }

  forv_Vec(Expr, expr, queue) {
    for (Expr* current = expr; current; current = current->next) {
      bool insertNewLocal = false;
      CallExpr* call = toCallExpr(current);
      DefExpr* def = toDefExpr(current);

      //
      // if this statement is a yield, a return, a label definition, a
      // goto, a conditional, or a block, insert a new local block
      // that contains all the statements seen to this point (by
      // setting insertNewLocal to true) and add the first statements
      // of blocks and conditional blocks to the queue; otherwise, if
      // this statement is a definition, add it to preVec; otherwise,
      // add this statement to inVec.
      //
      if ((call && call->isPrimitive(PRIM_YIELD)) ||
          (call && call->isPrimitive(PRIM_RETURN)) ||
          (def && isLabelSymbol(def->sym)) ||
          isGotoStmt(current) ||
          isCondStmt(current) ||
          isBlockStmt(current)) {
        insertNewLocal = true;
        if (BlockStmt* block = toBlockStmt(current)) {
          if (block->body.head)
            queue.add(block->body.head);
        } else if (CondStmt* cond = toCondStmt(current)) {
          if (cond->thenStmt && cond->thenStmt->body.head)
            queue.add(cond->thenStmt->body.head);
          if (cond->elseStmt && cond->elseStmt->body.head)
            queue.add(cond->elseStmt->body.head);
        }
      } else if (isDefExpr(current)) {
        preVec.add(current);
      } else {
        inVec.add(current);
      }

      //
      // if ready to insert a new local block or we are on the last
      // statement in a block, insert a new local block containing all
      // the statements in inVec; move all the statements in preVec to
      // before this local block.
      //
      if (insertNewLocal || !current->next) {
        if (preVec.n || inVec.n) {
          BlockStmt* newLocalBlock = new BlockStmt();
          newLocalBlock->blockInfo = new CallExpr(PRIM_BLOCK_LOCAL);
          current->insertBefore(newLocalBlock);
          forv_Vec(Expr, expr, preVec) {
            newLocalBlock->insertBefore(expr->remove());
          }
          preVec.clear();
          forv_Vec(Expr, expr, inVec) {
            newLocalBlock->insertAtTail(expr->remove());
          }
          inVec.clear();
        }
      }
    }
  }

  //
  // remove old local blocks
  //
  forv_Vec(BlockStmt, block, localBlocks) {
    block->blockInfo->remove();
  }
}


static void
expandIteratorInline(CallExpr* call) {
  BlockStmt* body;
  Symbol* index = toSymExpr(call->get(1))->var;
  Symbol* ic = toSymExpr(call->get(2))->var;
  FnSymbol* iterator = ic->type->defaultConstructor->getFormal(1)->type->defaultConstructor;
  BlockStmt* ibody = iterator->body->copy();

  if (recursiveIteratorSet.set_in(iterator)) {
    INT_FATAL(call, "recursive iterator inlining is not yet supported");
  }

  reset_line_info(ibody, call->lineno);
  body = toBlockStmt(call->parentExpr);
  call->remove();
  body->replace(ibody);
  Vec<BaseAST*> asts;
  collect_asts(ibody, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIM_YIELD)) {
        Symbol* yieldedIndex = newTemp("_yieldedIndex", index->type);
        SymbolMap map;
        map.put(index, yieldedIndex);
        BlockStmt* bodyCopy = body->copy(&map);

        Symbol* yieldedSymbol = toSymExpr(call->get(1))->var;
        INT_ASSERT(yieldedSymbol);

        // remove move to return-temp that is defined at top of iterator
        bool inserted = false;
        if (CallExpr* prev = toCallExpr(call->prev)) {
          if (prev->isPrimitive(PRIM_MOVE)) {
            if (SymExpr* lhs = toSymExpr(prev->get(1))) {
              if (lhs->var == yieldedSymbol) {
                lhs->var = yieldedIndex;
                prev->insertBefore(new DefExpr(yieldedIndex));
                inserted = true;
              }
            }
          }
        }
        call->replace(bodyCopy);
        if (!inserted) {
          bodyCopy->insertAtHead(new CallExpr(PRIM_MOVE, yieldedIndex, call->get(1)));
          bodyCopy->insertAtHead(new DefExpr(yieldedIndex));
        }
      }
      if (call->isPrimitive(PRIM_RETURN)) // remove return
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
          VarSymbol* tmp = newTemp(formal->name, formal->type);
          stmt->insertBefore(new DefExpr(tmp));
          stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, ic, new_IntSymbol(count))));
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
      if (call->isPrimitive(PRIM_YIELD))
        count++;
  }
  if (count == 1)
    return true;
  return false;
}

static void
getRecursiveIterators(Vec<Symbol*>& iterators, Symbol* gIterator) {
  if (gIterator->type->symbol->hasFlag(FLAG_TUPLE)) {
    ClassType* iteratorType = toClassType(gIterator->type);
    for (int i=1; i <= iteratorType->fields.length; i++) {
      Symbol *iterator = toSymbol(iteratorType->getField(i));
      if (iterator)
        getRecursiveIterators(iterators, iterator);
    }
  }
  else {
    iterators.add(gIterator);
  }
}

static bool
canInlineSingleYieldIterator(Symbol* gIterator) {
  Vec<Symbol*> iterators;
  getRecursiveIterators(iterators, gIterator);
  for (int i = 0; i < iterators.n; i++) {
    FnSymbol* iterator= iterators.v[i]->type->defaultConstructor->getFormal(1)->type->defaultConstructor;
    BlockStmt *block = iterator->body;

    INT_ASSERT(block);
    Vec<BaseAST*> asts;
    collect_asts(block, asts);

    int numYields = 0;
    forv_Vec(BaseAST, ast, asts) {
      CallExpr* call = toCallExpr(ast);
      if (call && call->isPrimitive(PRIM_YIELD)) {
        numYields++;
        if (iterator->body != call->parentExpr) return false;
      }
    }
    if (numYields != 1) return false;
  }
  return true;
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
    for (int i=1; i <= iteratorType->fields.length; i++) {
      Symbol* iterator = newTemp("_iterator", iteratorType->getField(i)->type);
      loop->insertBefore(new DefExpr(iterator));
      loop->insertBefore(new CallExpr(PRIM_MOVE, iterator, new CallExpr(PRIM_GET_MEMBER_VALUE, gIterator, iteratorType->getField(i))));

      Symbol* index = newTemp("_index", indexType->getField(i)->type);
      loop->insertAtHead(new CallExpr(PRIM_SET_MEMBER, gIndex, indexType->getField(i), index));
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
      type = type->getValueType();
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
  IteratorInfo* ii = iterator->type->defaultConstructor->getFormal(1)->type->defaultConstructor->iteratorInfo;
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
      block->insertAtTail(new CallExpr(PRIM_MOVE, ret, call));
    } else {
      VarSymbol* tmp = newTemp(fn->retType);
      block->insertAtTail(new DefExpr(tmp));
      block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, call));
      block->insertAtTail(new CallExpr(PRIM_MOVE, ret, new CallExpr(PRIM_CAST, ret->type->symbol, tmp)));
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
    VarSymbol* cid = newTemp(dtBool);
    block->insertAtTail(new DefExpr(cid));
    block->insertAtTail(new CallExpr(PRIM_MOVE, cid,
                          new CallExpr(PRIM_GETCID,
                                       iterator, type->symbol)));
    BlockStmt* thenStmt = new BlockStmt();
    BlockStmt* elseStmt = new BlockStmt();
    VarSymbol* childIterator = newTemp(type);
    thenStmt->insertAtTail(new DefExpr(childIterator));
    thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, childIterator, new CallExpr(PRIM_CAST, type->symbol, iterator)));
    buildIteratorCallInner(thenStmt, ret, fnid, childIterator);
    block->insertAtTail(new CondStmt(new SymExpr(cid), thenStmt, elseStmt));
    block = elseStmt;
  }
  buildIteratorCallInner(block, ret, fnid, iterator);
  return outerBlock;
}

static void
inlineSingleYieldIterator(CallExpr* call) {
  BlockStmt* block = toBlockStmt(call->parentExpr);
  SymExpr* se1 = toSymExpr(call->get(1));
  SymExpr* se2 = toSymExpr(call->get(2));
  VarSymbol* index = toVarSymbol(se1->var);
  VarSymbol* iterator = toVarSymbol(se2->var);

  SET_LINENO(call);
  Vec<Symbol*> iterators;
  Vec<Symbol*> indices;
  setupSimultaneousIterators(iterators, indices, iterator, index, block);

  CallExpr *noop = new CallExpr(PRIM_NOOP);
  block->insertAtHead(noop);
  for (int i = 0; i < iterators.n; i++) {
    FnSymbol *iterator = iterators.v[i]->type->defaultConstructor->getFormal(1)->type->defaultConstructor;
    Vec<Expr*> exprs;

    BlockStmt *ibody = iterator->body->copy();

    Vec<BaseAST*> asts;
    collect_asts(ibody, asts);
    bool afterYield = false;
    for_alist(expr, ibody->body) {
      if (CallExpr *curr_expr = toCallExpr(expr)) {
        if (curr_expr->isPrimitive(PRIM_YIELD)) {
          afterYield = true;
          noop->insertAfter(new CallExpr(PRIM_MOVE, indices.v[i], curr_expr->get(1)->remove()));
        } else if (!curr_expr->isPrimitive(PRIM_RETURN)) {
          if (afterYield == false)
            noop->insertBefore(curr_expr->remove());
          else 
            block->insertAtTail(curr_expr->remove());
        }
      } else {
        if (afterYield == false)
          noop->insertBefore(expr->remove());
        else 
          block->insertAtTail(expr->remove());
      }
    }

    int count = 1;
    for_formals(formal, iterator) {
      forv_Vec(BaseAST, ast, asts) {
        if (SymExpr* se = toSymExpr(ast)) {
          if (se->var == formal) {
            //if ((se->var->type == formal->type) && (!strcmp(se->var->name, formal->name))) {
            // count is used to get the nth field out of the iterator class;
            // it is replaced by the field once the iterator class is created
            Expr* stmt = se->getStmtExpr();
            VarSymbol* tmp = newTemp(formal->name, formal->type);
            stmt->insertBefore(new DefExpr(tmp));
            stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, iterators.v[i], new_IntSymbol(count))));
            se->var = tmp;
          }
        }
      }
      count++;
    }

  }
  noop->remove();
  call->remove();
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
      iterator->type->defaultConstructor->getFormal(1)->type->defaultConstructor->iteratorInfo &&
      canInlineIterator(iterator->type->defaultConstructor->getFormal(1)->type->defaultConstructor) &&
      (iterator->type->dispatchChildren.n == 0 ||
       (iterator->type->dispatchChildren.n == 1 &&
       iterator->type->dispatchChildren.v[0] == dtObject))) {
    expandIteratorInline(call);
  } else if (!fNoInlineIterators && canInlineSingleYieldIterator(iterator)) {
    inlineSingleYieldIterator(call);
  } else {
    SET_LINENO(call);
    Vec<Symbol*> iterators;
    Vec<Symbol*> indices;
    setupSimultaneousIterators(iterators, indices, iterator, index, block);

    VarSymbol* firstCond = NULL;
    for (int i = 0; i < iterators.n; i++) {
      Vec<Type*> children;
      getIteratorChildren(children, iterators.v[i]->type);
      VarSymbol* cond = newTemp("_cond", dtBool);

      block->insertBefore(buildIteratorCall(NULL, ZIP1, iterators.v[i], children));

      block->insertBefore(new DefExpr(cond));
      block->insertBefore(buildIteratorCall(cond, HASMORE, iterators.v[i], children));
      block->insertAtHead(buildIteratorCall(indices.v[i], GETVALUE, iterators.v[i], children));

      block->insertAtTail(buildIteratorCall(NULL, ZIP3, iterators.v[i], children));
      block->insertAtTail(buildIteratorCall(cond, HASMORE, iterators.v[i], children));
      if (isBoundedIterator(iterators.v[i]->type->defaultConstructor->getFormal(1)->type->defaultConstructor)) {
        if (!firstCond) {
          firstCond = cond;
        } else if (!fNoBoundsChecks) {
          VarSymbol* tmp = newTemp(dtBool);
          block->insertAtHead(new CondStmt(new SymExpr(tmp), new CallExpr(PRIM_RT_ERROR, new_StringSymbol("zippered iterations have non-equal lengths"))));
          block->insertAtHead(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_EQUAL, cond, new_IntSymbol(0))));
          block->insertAtHead(new DefExpr(tmp));
          block->insertAfter(new CondStmt(new SymExpr(cond), new CallExpr(PRIM_RT_ERROR, new_StringSymbol("zippered iterations have non-equal lengths"))));
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

    if (!strcmp(index->name, "_indexOfInterest"))
      block->insertAtHead(index->defPoint->remove());
  }
}


static void
inlineIterators() {
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (block->parentSymbol) {
      if (block->blockInfo && block->blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP)) {
        Symbol* iterator = toSymExpr(block->blockInfo->get(2))->var;
        if (iterator->type->defaultConstructor->getFormal(1)->type->defaultConstructor->hasFlag(FLAG_INLINE_ITERATOR)) {
          expandIteratorInline(block->blockInfo);
        }
      }
    }
  }
}


void lowerIterators() {
  fragmentLocalBlocks();

  computeRecursiveIteratorSet();

  inlineIterators();
  
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ITERATOR_FN)) {
      collapseBlocks(fn->body);
      removeUnnecessaryGotos(fn);
    }
  }

  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol)
      if (call->isPrimitive(PRIM_BLOCK_FOR_LOOP))
        if (call->numActuals() > 1)
          expand_for_loop(call);
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
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
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_ITERATOR_FN)) {
      lowerIterator(fn);
    }
  }
  // fix GET_MEMBER primitives that access fields of an iterator class
  // via a number
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol && call->isPrimitive(PRIM_GET_MEMBER)) {
      ClassType* ct = toClassType(call->get(1)->typeInfo());
      if (ct->symbol->hasFlag(FLAG_REF))
        ct = toClassType(ct->getValueType());
      long num;
      if (get_int(call->get(2), &num)) {
        Symbol* field = ct->getField(num+1); // add 1 for super
        call->get(2)->replace(new SymExpr(field));
        CallExpr* parent = toCallExpr(call->parentExpr);
        INT_ASSERT(parent->isPrimitive(PRIM_MOVE));
        Symbol* local = toSymExpr(parent->get(1))->var;
        if (local->type == field->type)
          call->primitive = primitives[PRIM_GET_MEMBER_VALUE];
        else if (local->type != field->type->refType)
          INT_FATAL(call, "unexpected case");
      }
    }
  }

  //
  // cleanup leader and follower iterator calls
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol) {
      if (FnSymbol* fn = call->isResolved()) {
        if (fn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
          if (!strcmp(call->parentSymbol->name, "_toLeader") ||
              !strcmp(call->parentSymbol->name, "_toFollower")) {
            ArgSymbol* iterator = toFnSymbol(call->parentSymbol)->getFormal(1);
            Type* iteratorType = iterator->type;
            if (iteratorType->symbol->hasFlag(FLAG_REF))
              iteratorType = iteratorType->getValueType();
            int i = 2; // first field is super
            for_actuals(actual, call) {
              SymExpr* se = toSymExpr(actual);
              if (isArgSymbol(se->var) && call->parentSymbol != se->var->defPoint->parentSymbol) {
                Symbol* field = toClassType(iteratorType)->getField(i);
                VarSymbol* tmp = NULL;
                if (field->type == se->var->type) {
                  tmp = newTemp(field->type);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, iterator, field)));
                } else if (field->type->refType == se->var->type) {
                  tmp = newTemp(field->type->refType);
                  call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                  call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, iterator, field)));
                }
                actual->replace(new SymExpr(tmp));
                i++;
              }
            }
          }
        }
      }
    }
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->defPoint->parentSymbol && fn->iteratorInfo) {
      FnSymbol* getIterator = fn->iteratorInfo->getIterator;
      INT_ASSERT(getIterator->defPoint->parentSymbol);
      ClassType* irecord = fn->iteratorInfo->irecord;
      if (irecord->dispatchChildren.n > 0) {
        LabelSymbol* label = new LabelSymbol("end");
        getIterator->insertBeforeReturn(new DefExpr(label));
        Symbol* ret = getIterator->getReturnSymbol();
        forv_Vec(Type, type, irecord->dispatchChildren) {
          VarSymbol* tmp = newTemp(irecord->getField(1)->type);
          VarSymbol* cid = newTemp(dtBool);
          BlockStmt* thenStmt = new BlockStmt();
          VarSymbol* recordTmp = newTemp(type);
          VarSymbol* classTmp = newTemp(type->defaultConstructor->iteratorInfo->getIterator->retType);
          thenStmt->insertAtTail(new DefExpr(recordTmp));
          thenStmt->insertAtTail(new DefExpr(classTmp));
          ClassType* ct = toClassType(type);
          for_fields(field, ct) {
            VarSymbol* ftmp = newTemp(getIterator->getFormal(1)->type->getField(field->name)->type);
            thenStmt->insertAtTail(new DefExpr(ftmp));
            thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, ftmp, new CallExpr(PRIM_GET_MEMBER_VALUE, getIterator->getFormal(1), getIterator->getFormal(1)->type->getField(field->name))));
            if (ftmp->type == field->type) {
              thenStmt->insertAtTail(new CallExpr(PRIM_SET_MEMBER, recordTmp, field, ftmp));
            } else {
              VarSymbol* ftmp2 = newTemp(field->type);
              thenStmt->insertAtTail(new DefExpr(ftmp2));
              thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, ftmp2, new CallExpr(PRIM_CAST, field->type->symbol, ftmp)));
              thenStmt->insertAtTail(new CallExpr(PRIM_SET_MEMBER, recordTmp, field, ftmp2));
            }
          }
          thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, classTmp, new CallExpr(type->defaultConstructor->iteratorInfo->getIterator, recordTmp)));
          thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, ret, new CallExpr(PRIM_CAST, ret->type->symbol, classTmp)));
          thenStmt->insertAtTail(new GotoStmt(GOTO_NORMAL, label));
          ret->defPoint->insertAfter(new CondStmt(new SymExpr(cid), thenStmt));
          ret->defPoint->insertAfter(new CallExpr(PRIM_MOVE, cid, new CallExpr(PRIM_GETCID, tmp, type->defaultConstructor->iteratorInfo->irecord->getField(1)->type->symbol)));
          ret->defPoint->insertAfter(new DefExpr(cid));
          ret->defPoint->insertAfter(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, getIterator->getFormal(1), irecord->getField(1))));
          ret->defPoint->insertAfter(new DefExpr(tmp));
        }
      }
    }
  }
}
