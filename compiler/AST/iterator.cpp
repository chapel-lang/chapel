#include "astutil.h"
#include "bb.h"
#include "bitVec.h"
#include "expr.h"
#include "stmt.h"
#include "iterator.h"
#include "optimizations.h"
#include "view.h"


//#define DEBUG_LIVE


IteratorInfo::IteratorInfo() :
  iterator(NULL),
  icType(NULL),
  advance(NULL),
  zip1(NULL),
  zip2(NULL),
  zip3(NULL),
  zip4(NULL),
  hasMore(NULL),
  getValue(NULL),
  leader(NULL),
  follower(NULL),
  iterator2leader(NULL),
  iterator2follower(NULL)
{}


//
// initialize temp to default value (recursive for records)
//
static void
insertSetMemberInits(FnSymbol* fn, Symbol* var) {
  Type* type = var->type;
  if (type->defaultValue) {
    fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, var, type->defaultValue));
  } else {
    ClassType* ct = toClassType(type);
    INT_ASSERT(ct);
    for_fields(field, ct) {
      if (field->type->symbol->hasPragma(PRAG_REF)) {
        if (getValueType(field->type)->symbol->hasPragma(PRAG_ARRAY))
          continue; // skips array types
        Symbol* tmp = new VarSymbol("_tmp", field->type);
        tmp->addPragma(PRAG_TEMP);
        fn->insertAtTail(new DefExpr(tmp));
        fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, gNilRef));
        fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, var, field, tmp));
      } else if (field->type->refType) { // skips array types (how to handle arrays?) ( sjd later: really? )
        Symbol* tmp = new VarSymbol("_tmp", field->type);
        tmp->addPragma(PRAG_TEMP);
        fn->insertAtTail(new DefExpr(tmp));
        insertSetMemberInits(fn, tmp);
        fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, var, field, tmp));
      }
    }
  }
}


//
// Determines that an iterator has a single loop with a single yield
// in it by checking the following conditions:
//
//   1. There is exactly one for-loop and no other loops.
//   2. The single for-loop is top-level to the function.
//   3. There is exactly one yield.
//   4. The single yield is top-level to the for-loop.
//   5. There are no goto statements.
//
// I believe these conditions can be relaxed.
//
static CallExpr*
isSingleLoopIterator(FnSymbol* fn, Vec<BaseAST*>& asts) {
  if (fNoOptimizeLoopIterators)
    return NULL;
  BlockStmt* singleFor = NULL;
  CallExpr* singleYield = NULL;
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_YIELD)) {
        if (singleYield) {
          return NULL;
        } else if (BlockStmt* block = toBlockStmt(call->parentExpr)) {
          if (block->blockInfo &&
              (block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_FOR_LOOP) ||
               block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_WHILEDO_LOOP))) {
            singleYield = call;
          } else {
            return NULL;
          }
        } else {
          return NULL;
        }
      }
    } else if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->blockInfo) {
        if (singleFor) {
          return NULL;
        } else if ((block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_FOR_LOOP) ||
                    block->blockInfo->isPrimitive(PRIMITIVE_BLOCK_WHILEDO_LOOP)) &&
                   block->parentExpr == fn->body) {
          singleFor = block;
        } else {
          return NULL;
        }
      }
    } else if (ast->astTag == STMT_GOTO) {
      return NULL;
    }
  }
  if (singleFor && singleYield)
    return singleYield;
  else
    return NULL;
}


static void
replaceLocalsWithFields(IteratorInfo* ii,
                        Vec<BaseAST*>& asts,
                        SymbolMap& local2field,
                        Vec<Symbol*>& locals) {
  FnSymbol* fn = ii->iterator;
  Symbol* ic = ii->advance->getFormal(1);

  Vec<SymExpr*> defSet;
  Vec<SymExpr*> useSet;
  buildDefUseSets(locals, fn, defSet, useSet);

  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* se = toSymExpr(ast)) {
      if (se->parentSymbol && (useSet.set_in(se) || defSet.set_in(se))) {
        CallExpr* call = toCallExpr(se->parentExpr);
        if (call && call->isPrimitive(PRIMITIVE_SET_REF)) {
          Symbol* field = local2field.get(se->var);
          call->primitive = primitives[PRIMITIVE_GET_MEMBER];
          call->insertAtHead(ic);
          se->var = field;
        } else {
          Symbol* field = local2field.get(se->var);
          VarSymbol* tmp = new VarSymbol("_tmp", se->var->type);
          Expr* stmt = se->getStmtExpr();
          BlockStmt* loop = NULL;
          if (!stmt->list) {
            loop = toBlockStmt(se->parentExpr->parentExpr);
            INT_ASSERT(loop);
            stmt = loop;
          }
          if (call && call->isPrimitive(PRIMITIVE_GET_MEMBER)) {
            INT_ASSERT(tmp->type->refType);
            tmp->type = tmp->type->refType;
          }
          stmt->insertBefore(new DefExpr(tmp));
          asts.add(tmp->defPoint);
          if (useSet.set_in(se)) {
            if (tmp->type == field->type->refType)
              stmt->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER, ic, field)));
            else
              stmt->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, ic, field)));
            if (loop) {
              if (tmp->type == field->type->refType)
                loop->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER, ic, field)));
              else
                loop->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, ic, field)));
            }
          }
          if (defSet.set_in(se) ||
              call && call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
            if (loop) {
              loop->insertAtHead(new CallExpr(PRIMITIVE_SET_MEMBER, ic, field, tmp));
            } else {
              stmt->insertAfter(new CallExpr(PRIMITIVE_SET_MEMBER, ic, field, tmp));
            }
          }
          se->var = tmp;
        }
      }
    }
  }
}


static void
buildZip1(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {
  Symbol* ic = ii->advance->getFormal(1);
  SymbolMap map;
  map.put(ic, ii->zip1->_this);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!isArgSymbol(def->sym))
        ii->zip1->insertAtTail(def->copy(&map));
  }
  for_alist(expr, ii->iterator->body->body) {
    if (expr == singleLoop)
      break;
    if (!isDefExpr(expr))
      ii->zip1->insertAtTail(expr->copy(&map));
  }
  CallExpr* blockInfo = singleLoop->blockInfo->copy(&map);
  ii->zip1->insertAtTail(new CondStmt(blockInfo->get(1)->remove(),
                                      new CallExpr(PRIMITIVE_SET_MEMBER, ii->zip1->_this, ii->icType->getField("more"), new_IntSymbol(1)),
                                      new CallExpr(PRIMITIVE_SET_MEMBER, ii->zip1->_this, ii->icType->getField("more"), new_IntSymbol(0))));
  ii->zip1->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
}


static void
buildZip2(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {
  Symbol* ic = ii->advance->getFormal(1);
  SymbolMap map;
  map.put(ic, ii->zip2->_this);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!isArgSymbol(def->sym))
        ii->zip2->insertAtTail(def->copy(&map));
  }
  for_alist(expr, singleLoop->body) {
    if (CallExpr* call = toCallExpr(expr))
      if (call->isPrimitive(PRIMITIVE_YIELD))
        break;
    if (!isDefExpr(expr))
      ii->zip2->insertAtTail(expr->copy(&map));
  }
  ii->zip2->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
}


static void
buildZip3(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {
  Symbol* ic = ii->advance->getFormal(1);
  SymbolMap map;
  map.put(ic, ii->zip3->_this);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!isArgSymbol(def->sym))
        ii->zip3->insertAtTail(def->copy(&map));
  }
  bool flag = true;
  for_alist(expr, singleLoop->body) {
    if (flag) {
      if (CallExpr* call = toCallExpr(expr))
        if (call->isPrimitive(PRIMITIVE_YIELD))
          flag = false;
      continue;
    }
    if (!isDefExpr(expr))
      ii->zip3->insertAtTail(expr->copy(&map));
  }
  CallExpr* blockInfo = singleLoop->blockInfo->copy(&map);
  ii->zip3->insertAtTail(new CondStmt(blockInfo->get(1)->remove(),
                                      new CallExpr(PRIMITIVE_SET_MEMBER, ii->zip3->_this, ii->icType->getField("more"), new_IntSymbol(1)),
                                      new CallExpr(PRIMITIVE_SET_MEMBER, ii->zip3->_this, ii->icType->getField("more"), new_IntSymbol(0))));
  ii->zip3->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
}


static void
buildZip4(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {
  Symbol* ic = ii->advance->getFormal(1);
  SymbolMap map;
  map.put(ic, ii->zip4->_this);
  forv_Vec(BaseAST, ast, asts) {
      if (DefExpr* def = toDefExpr(ast))
        if (!isArgSymbol(def->sym))
          ii->zip4->insertAtTail(def->copy(&map));
    }
  bool flag = true;
  for_alist(expr, ii->iterator->body->body) {
    if (flag) {
      if (expr == singleLoop)
        flag = false;
      continue;
    }
    if (!isDefExpr(expr) && expr->next)
      ii->zip4->insertAtTail(expr->copy(&map));
  }
  ii->zip4->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
}


static void
buildAdvance(FnSymbol* fn,
             Vec<BaseAST*>& asts,
             SymbolMap& local2field,
             Vec<Symbol*>& locals) {
  IteratorInfo* ii = fn->iteratorInfo;
  Symbol* ic = ii->advance->getFormal(1);

  //
  // build standard iterator advance method (advance)
  //

  for_alist(expr, fn->body->body)
    ii->advance->insertAtTail(expr->remove());

  Symbol* end = new LabelSymbol("_end");
  ii->advance->insertAtTail(new DefExpr(end));

  // change yields to labels and gotos
  int i = 2; // 1 = not started, 0 = finished
  Vec<Symbol*> labels;
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_YIELD)) {
        call->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, ic, ii->icType->getField("more"), new_IntSymbol(i)));
        call->insertBefore(new GotoStmt(GOTO_NORMAL, end));
        Symbol* label = new LabelSymbol(astr("_jump_", istr(i)));
        call->insertBefore(new DefExpr(label));
        labels.add(label);
        call->remove();
        i++;
      } else if (call->isPrimitive(PRIMITIVE_RETURN)) {
        call->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, ic, ii->icType->getField("more"), new_IntSymbol(0)));
        call->remove(); // remove old return
      }
    }
  }

  // insert jump table at head of advance
  i = 2;
  Symbol* tmp = new VarSymbol("_tmp", dtBool);
  tmp->addPragma(PRAG_TEMP);
  Symbol* more = new VarSymbol("more", dtInt[INT_SIZE_32]);

  forv_Vec(Symbol, label, labels) {
    ii->advance->insertAtHead(new CondStmt(new SymExpr(tmp), new GotoStmt(GOTO_NORMAL, label)));
    ii->advance->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_EQUAL, more, new_IntSymbol(i++))));
  }
  ii->advance->insertAtHead(new CallExpr(PRIMITIVE_MOVE, more, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, ic, ii->icType->getField("more"))));
  ii->advance->insertAtHead(new DefExpr(tmp));
  ii->advance->insertAtHead(new DefExpr(more));
  ii->advance->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
}


static void
buildHasMore(IteratorInfo* ii) {
  VarSymbol* tmp = new VarSymbol("_tmp", ii->hasMore->retType);
  ii->hasMore->insertAtTail(new DefExpr(tmp));
  ii->hasMore->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, ii->hasMore->getFormal(1), ii->icType->getField("more"))));
  ii->hasMore->insertAtTail(new CallExpr(PRIMITIVE_RETURN, tmp));
}


static void
buildGetValue(IteratorInfo* ii) {
  VarSymbol* tmp = new VarSymbol("_tmp", ii->getValue->retType);
  ii->getValue->insertAtTail(new DefExpr(tmp));
  ii->getValue->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, ii->getValue->getFormal(1), ii->icType->getField("value"))));
  ii->getValue->insertAtTail(new CallExpr(PRIMITIVE_RETURN, tmp));
}


static void
addLiveLocalVariables(Vec<Symbol*>& syms, FnSymbol* fn, BlockStmt* singleLoop) {
  buildBasicBlocks(fn);

#ifdef DEBUG_LIVE
  printf("Iterator\n");
  list_view(fn);
#endif

#ifdef DEBUG_LIVE
  printf("Basic Blocks\n");
  printBasicBlocks(fn);
#endif

  Vec<Symbol*> locals;
  Map<Symbol*,int> localMap;
  Vec<SymExpr*> useSet;
  Vec<SymExpr*> defSet;
  Vec<BitVec*> OUT;
  liveVariableAnalysis(fn, locals, localMap, useSet, defSet, OUT);

  int i = 0;
  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    bool collect = false;
    forv_Vec(Expr, expr, bb->exprs) {
      CallExpr* call = toCallExpr(expr);
      if (call && call->isPrimitive(PRIMITIVE_YIELD))
        collect = true;
      if (singleLoop && expr == singleLoop->next)
        collect = true;
      if (singleLoop && expr == singleLoop->body.head)
        collect = true;
    }
    if (collect) {
      BitVec live(locals.n);
      for (int j = 0; j < locals.n; j++) {
        if (OUT.v[i]->get(j))
          live.set(j);
      }
      for (int k = bb->exprs.n - 1; k >= 0; k--) {
        CallExpr* call = toCallExpr(bb->exprs.v[k]);
        if (call && call->isPrimitive(PRIMITIVE_YIELD) ||
            singleLoop && bb->exprs.v[k] == singleLoop->next ||
            singleLoop && bb->exprs.v[k] == singleLoop->body.head) {
          for (int j = 0; j < locals.n; j++) {
            if (live.get(j)) {
              syms.add_exclusive(locals.v[j]);
            }
          }
        }
        Vec<BaseAST*> asts;
        collect_asts(bb->exprs.v[k], asts);
        forv_Vec(BaseAST, ast, asts) {
          if (SymExpr* se = toSymExpr(ast)) {
            if (defSet.set_in(se)) {
              live.unset(localMap.get(se->var));
            }
            if (useSet.set_in(se)) {
              live.set(localMap.get(se->var));
            }
          }
        }
      }
    }
    i++;
  }

#ifdef DEBUG_LIVE
  printf("LIVE at Yield Points\n");
  forv_Vec(Symbol, sym, syms) {
    printf("%s[%d]\n", sym->name, sym->id);
  }
  printf("\n");
#endif

  forv_Vec(BitVec, out, OUT)
    delete out;

  //
  // If we have live references to local variables, then we need to
  // make sure that we add these local variables to the iterator class
  // along with the reference.
  //
  // See test/functions/deitz/iterators/test_fock_simplification.chpl
  // and make sure that the iterator foo is not inlined even though
  // the iterators for the loops inside this one are inlined.  At the
  // time of this comment, the two yields kept foo from being
  // inlined.
  //
  Symbol* ret = fn->getReturnSymbol();
  bool foundRef = false;
  forv_Vec(Symbol, sym, syms) {
    if (sym != ret && !isArgSymbol(sym) && sym->type->symbol->hasPragma(PRAG_REF)) {
      foundRef = true;
    }
  }
  if (foundRef) {
    Map<Symbol*,Vec<SymExpr*>*> defMap;
    Map<Symbol*,Vec<SymExpr*>*> useMap;
    buildDefUseMaps(fn, defMap, useMap);
    forv_Vec(Symbol, sym, syms) {
      if (sym != ret && !isArgSymbol(sym) && sym->type->symbol->hasPragma(PRAG_REF)) {
        Vec<SymExpr*>* defs = defMap.get(sym);
        if (defs->n != 1) {
          INT_FATAL(sym, "invalid assumption about reference");
        }
        CallExpr* move = toCallExpr(defs->v[0]->parentExpr);
        INT_ASSERT(move);
        INT_ASSERT(move->isPrimitive(PRIMITIVE_MOVE));
        SymExpr* se = toSymExpr(move->get(2));
        CallExpr* call = toCallExpr(move->get(2));
        if (se) {
          INT_ASSERT(se->var->type->symbol->hasPragma(PRAG_REF));
          if (se->var->defPoint->parentSymbol == fn) {
            syms.add_exclusive(se->var);
          }
        } else if (call->isPrimitive(PRIMITIVE_SET_REF) ||
                   call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
                   call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)) {
          SymExpr* rhs = toSymExpr(call->get(1));
          INT_ASSERT(rhs);
          syms.add_exclusive(rhs->var);
        } else if (FnSymbol* fn = call->isResolved()) {
          for_actuals(actual, call) {
            SymExpr* se = toSymExpr(actual);
            INT_ASSERT(se);
            if (se->var->defPoint->parentSymbol == fn) {
              syms.add_exclusive(se->var);
            }
          }
        } else {
          INT_FATAL(sym, "invalid assumption about reference");
        }
      }
    }
    freeDefUseMaps(defMap, useMap);
  }
}


static void
addAllLocalVariables(Vec<Symbol*>& syms, Vec<BaseAST*>& asts) {
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (VarSymbol* var = toVarSymbol(def->sym))
        if (!var->type->symbol->hasPragma(PRAG_REF) || var->hasPragma(PRAG_INDEX_VAR))
          syms.add(var);
  }
}


static void
rebuildIterator(IteratorInfo* ii,
                SymbolMap& local2field,
                Vec<Symbol*>& locals) {
  if (ii->icType->dispatchParents.n == 0) {
    ii->icType->dispatchParents.add(dtObject);
    dtObject->dispatchChildren.add(ii->icType);
  }
  FnSymbol* fn = ii->iterator;
  for_alist(expr, fn->body->body)
    expr->remove();
  fn->defPoint->remove();
  fn->retType = ii->icType;
  Symbol* iterator = new VarSymbol("ic", ii->icType);
  iterator->addPragma(PRAG_TEMP);
  fn->insertAtTail(new DefExpr(iterator));
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, iterator, new CallExpr(PRIMITIVE_CHPL_ALLOC, ii->icType->symbol, new_StringSymbol("iterator class"))));
  fn->insertAtTail(new CallExpr(PRIMITIVE_SETCID, iterator));
  forv_Vec(Symbol, local, locals) {
    Symbol* field = local2field.get(local);
    if (toArgSymbol(local)) {
      if (local->type == field->type->refType) {
        Symbol* tmp = new VarSymbol("_tmp", field->type);
        tmp->addPragma(PRAG_TEMP);
        fn->insertAtTail(new DefExpr(tmp));
        fn->insertAtTail(
          new CallExpr(PRIMITIVE_MOVE, tmp,
            new CallExpr(PRIMITIVE_GET_REF, local)));
        fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, field, tmp));
      } else {
        fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, field, local));
      }
    } else if (isRecordType(local->type)) {
      if (field->type->refType) { // skips array types (how to handle arrays?)
        Symbol* tmp = new VarSymbol("_tmp", field->type);
        tmp->addPragma(PRAG_TEMP);
        fn->insertAtTail(new DefExpr(tmp));
        insertSetMemberInits(fn, tmp);
        fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, field, tmp));
      }
    } else if (field->type->symbol->hasPragma(PRAG_REF)) {
      // do not initialize references
    } else if (field->type->defaultValue) {
      fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, field, field->type->defaultValue));
    }
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, ii->icType->getField("more"), new_IntSymbol(1)));
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, iterator));
  ii->getValue->defPoint->insertAfter(new DefExpr(fn));
  fn->addPragma(PRAG_INLINE);
}


void lowerIterator(FnSymbol* fn) {
  IteratorInfo* ii = fn->iteratorInfo;

  SET_LINENO(fn);
  Vec<BaseAST*> asts;
  collect_asts_postorder(fn, asts);

  CallExpr* singleLoopYield = isSingleLoopIterator(fn, asts);
  BlockStmt* singleLoop = NULL;
  if (singleLoopYield)
    singleLoop = toBlockStmt(singleLoopYield->parentExpr);

  //
  // create fields for all local variables and arguments; however, if
  // live variable analysis is enabled, make fields only for local
  // variables that are live at places where the iterator methods will
  // yield control back to the loop body.
  //
  SymbolMap local2field;
  Vec<Symbol*> locals;

  for_formals(formal, fn)
    locals.add(formal);
  if (fNoLiveAnalysis)
    addAllLocalVariables(locals, asts);
  else {
    addLiveLocalVariables(locals, fn, singleLoop);
  }
  locals.add_exclusive(fn->getReturnSymbol());

  Symbol* super = new VarSymbol("super", dtObject);
  super->addPragma(PRAG_SUPER_CLASS);
  ii->icType->fields.insertAtTail(new DefExpr(super));

  int i = 0;
  forv_Vec(Symbol, local, locals) {
    const char* fieldName = (local == fn->getReturnSymbol())
      ? "value"
      : astr("F", istr(i++), "_", local->name);
    Type* type = local->type;
    if (type->symbol->hasPragma(PRAG_REF) && local == fn->_this)
      type = getValueType(type);
    Symbol* field = new VarSymbol(fieldName, type);
    local2field.put(local, field);
    ii->icType->fields.insertAtTail(new DefExpr(field));
  }
  ii->icType->fields.insertAtTail(new DefExpr(new VarSymbol("more", dtInt[INT_SIZE_32])));

  replaceLocalsWithFields(ii, asts, local2field, locals);
  if (singleLoop) {
    buildZip1(ii, asts, singleLoop);
    buildZip2(ii, asts, singleLoop);
    buildZip3(ii, asts, singleLoop);
    buildZip4(ii, asts, singleLoop);
  } else {
    ii->zip1->insertAtTail(new CallExpr(ii->advance, ii->zip1->_this));
    ii->zip1->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
    ii->zip2->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
    ii->zip3->insertAtTail(new CallExpr(ii->advance, ii->zip3->_this));
    ii->zip3->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
    ii->zip4->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
  }
  buildAdvance(fn, asts, local2field, locals);
  buildHasMore(ii);
  buildGetValue(ii);
  rebuildIterator(ii, local2field, locals);
}

