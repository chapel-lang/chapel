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
  getIterator(NULL),
  iclass(NULL),
  irecord(NULL),
  advance(NULL),
  zip1(NULL),
  zip2(NULL),
  zip3(NULL),
  zip4(NULL),
  hasMore(NULL),
  getValue(NULL)
{}


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
      if (call->isPrimitive(PRIM_YIELD)) {
        if (singleYield) {
          return NULL;
        } else if (BlockStmt* block = toBlockStmt(call->parentExpr)) {
          if (block->blockInfo &&
              (block->blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP) ||
               block->blockInfo->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP))) {
            singleYield = call;
          } else {
            return NULL;
          }
        } else {
          return NULL;
        }
      }
    } else if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->blockInfo && !(block->blockInfo->isPrimitive(PRIM_BLOCK_LOCAL) ||
                                block->blockInfo->isPrimitive(PRIM_BLOCK_UNLOCAL))) {
        if (singleFor) {
          return NULL;
        } else if ((block->blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP) ||
                    block->blockInfo->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP)) &&
                   block->parentExpr == fn->body) {
          singleFor = block;
        } else {
          return NULL;
        }
      }
    } else if (isGotoStmt(ast)) {
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
        Symbol* field = local2field.get(se->var);
        if (call && call->isPrimitive(PRIM_ADDR_OF)) {
          call->primitive = primitives[PRIM_GET_MEMBER];
          call->insertAtHead(ic);
          se->var = field;
        } else {
          VarSymbol* tmp = newTemp(se->var->type);
          Expr* stmt = se->getStmtExpr();
          BlockStmt* loop = NULL;
          if (!stmt->list) {
            loop = toBlockStmt(se->parentExpr->parentExpr);
            INT_ASSERT(loop);
            stmt = loop;
          }
          if (call && call->isPrimitive(PRIM_GET_MEMBER)) {
            INT_ASSERT(tmp->type->refType);
            tmp->type = tmp->type->refType;
          }
          stmt->insertBefore(new DefExpr(tmp));
          asts.add(tmp->defPoint);
          if (useSet.set_in(se)) {
            if (tmp->type == field->type->refType)
              stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, ic, field)));
            else
              stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ic, field)));
            if (loop) {
              if (tmp->type == field->type->refType)
                loop->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, ic, field)));
              else
                loop->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ic, field)));
            }
          }
          if (defSet.set_in(se) ||
              (call && call->isPrimitive(PRIM_SET_MEMBER))) {
            ArgSymbol* arg = toArgSymbol(se->var);
            if (arg && (arg->intent == INTENT_INOUT ||
                        arg->intent == INTENT_OUT ||
                        arg->intent == INTENT_REF)) {
              // This is the writeback of a formal temp into its corresponding arg:
              // (move x (= x _formal_tmp_x))
              // Since these arguments have reference types, we use the code
              // (move tmp0 (.v ic field))
              // to load the passed-in pointer-to-actual-arg.  The C dereference
              // operator (*) is applied to variables of ref type during code generation
              // The read of the formal will be replaced by the same code. Thus, we have:
              // (move tmp0 (.v ic field_x))
              // (move tmp1 (.v ic field_x))
              // (move tmp2 (.v ic field_formal_tmp_x))
              // (move tmp0 (= tmp1 tmp2))
              // which is sufficient.  
              // The normal code below adds a writeback of the temp result of a call:
              // (= ic field_x tmp0)
              // but that is trivial, since ic->field_x and tmp0 are already equal.
              stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ic, field)));
            } else {
              if (loop) {
                loop->insertAtHead(new CallExpr(PRIM_SET_MEMBER, ic, field, tmp));
              } else {
                stmt->insertAfter(new CallExpr(PRIM_SET_MEMBER, ic, field, tmp));
              }
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
                                      new CallExpr(PRIM_SET_MEMBER, ii->zip1->_this, ii->iclass->getField("more"), new_IntSymbol(1)),
                                      new CallExpr(PRIM_SET_MEMBER, ii->zip1->_this, ii->iclass->getField("more"), new_IntSymbol(0))));
  ii->zip1->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
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
      if (call->isPrimitive(PRIM_YIELD))
        break;
    if (!isDefExpr(expr))
      ii->zip2->insertAtTail(expr->copy(&map));
  }
  ii->zip2->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
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
        if (call->isPrimitive(PRIM_YIELD))
          flag = false;
      continue;
    }
    if (!isDefExpr(expr))
      ii->zip3->insertAtTail(expr->copy(&map));
  }
  CallExpr* blockInfo = singleLoop->blockInfo->copy(&map);
  ii->zip3->insertAtTail(new CondStmt(blockInfo->get(1)->remove(),
                                      new CallExpr(PRIM_SET_MEMBER, ii->zip3->_this, ii->iclass->getField("more"), new_IntSymbol(1)),
                                      new CallExpr(PRIM_SET_MEMBER, ii->zip3->_this, ii->iclass->getField("more"), new_IntSymbol(0))));
  ii->zip3->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
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
  ii->zip4->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
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
  Vec<LabelSymbol*> labels;
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIM_YIELD)) {
        call->insertBefore(new CallExpr(PRIM_SET_MEMBER, ic, ii->iclass->getField("more"), new_IntSymbol(i)));
        call->insertBefore(new GotoStmt(GOTO_ITER_END, end));
        LabelSymbol* label = new LabelSymbol(astr("_jump_", istr(i)));
        call->insertBefore(new DefExpr(label));
        labels.add(label);
        call->remove();
        i++;
      } else if (call->isPrimitive(PRIM_RETURN)) {
        call->insertBefore(new CallExpr(PRIM_SET_MEMBER, ic, ii->iclass->getField("more"), new_IntSymbol(0)));
        call->remove(); // remove old return
      }
    }
  }

  // insert jump table at head of advance
  i = 2;
  Symbol* tmp = newTemp(dtBool);
  Symbol* more = new VarSymbol("more", dtInt[INT_SIZE_DEFAULT]);

  forv_Vec(LabelSymbol, label, labels) {
    GotoStmt* igs = new GotoStmt(GOTO_ITER_RESUME, label);
    label->iterResumeGoto = igs;
    ii->advance->insertAtHead(new CondStmt(new SymExpr(tmp), igs));
    ii->advance->insertAtHead(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_EQUAL, more, new_IntSymbol(i++))));
  }
  ii->advance->insertAtHead(new CallExpr(PRIM_MOVE, more, new CallExpr(PRIM_GET_MEMBER_VALUE, ic, ii->iclass->getField("more"))));
  ii->advance->insertAtHead(new DefExpr(tmp));
  ii->advance->insertAtHead(new DefExpr(more));
  ii->advance->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
}


static void
buildHasMore(IteratorInfo* ii) {
  VarSymbol* tmp = newTemp(ii->hasMore->retType);
  ii->hasMore->insertAtTail(new DefExpr(tmp));
  ii->hasMore->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ii->hasMore->getFormal(1), ii->iclass->getField("more"))));
  ii->hasMore->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
}


static void
buildGetValue(IteratorInfo* ii) {
  VarSymbol* tmp = newTemp(ii->getValue->retType);
  ii->getValue->insertAtTail(new DefExpr(tmp));
  ii->getValue->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ii->getValue->getFormal(1), ii->iclass->getField("value"))));
  ii->getValue->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
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
      if (call && call->isPrimitive(PRIM_YIELD))
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
        if ((call && call->isPrimitive(PRIM_YIELD)) ||
            (singleLoop && bb->exprs.v[k] == singleLoop->next) ||
            (singleLoop && bb->exprs.v[k] == singleLoop->body.head)) {
          for (int j = 0; j < locals.n; j++) {
            if (live.get(j)) {
              syms.add_exclusive(locals.v[j]);
            }
          }
        }
        Vec<SymExpr*> symExprs;
        collectSymExprs(bb->exprs.v[k], symExprs);
        forv_Vec(SymExpr, se, symExprs) {
          if (defSet.set_in(se))
            live.unset(localMap.get(se->var));
          if (useSet.set_in(se))
            live.set(localMap.get(se->var));
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
    if (sym != ret && !isArgSymbol(sym) && sym->type->symbol->hasFlag(FLAG_REF)) {
      foundRef = true;
    }
  }
  if (foundRef) {
    Map<Symbol*,Vec<SymExpr*>*> defMap;
    Map<Symbol*,Vec<SymExpr*>*> useMap;
    buildDefUseMaps(fn, defMap, useMap);
    forv_Vec(Symbol, sym, syms) {
      if (sym != ret && !isArgSymbol(sym) && sym->type->symbol->hasFlag(FLAG_REF)) {
        Vec<SymExpr*>* defs = defMap.get(sym);
        if (defs->n != 1) {
          INT_FATAL(sym, "invalid assumption about reference");
        }
        CallExpr* move = toCallExpr(defs->v[0]->parentExpr);
        INT_ASSERT(move);
        INT_ASSERT(move->isPrimitive(PRIM_MOVE));
        SymExpr* se = toSymExpr(move->get(2));
        CallExpr* call = toCallExpr(move->get(2));
        if (se) {
          INT_ASSERT(se->var->type->symbol->hasFlag(FLAG_REF));
          if (se->var->defPoint->parentSymbol == fn) {
            syms.add_exclusive(se->var);
          }
        } else if (call->isPrimitive(PRIM_ADDR_OF) ||
                   call->isPrimitive(PRIM_GET_MEMBER) ||
                   call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
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
        if (!var->type->symbol->hasFlag(FLAG_REF) || var->hasFlag(FLAG_INDEX_VAR))
          syms.add(var);
  }
}


static void
rebuildIterator(IteratorInfo* ii,
                SymbolMap& local2field,
                Vec<Symbol*>& locals) {
  if (ii->iclass->dispatchParents.n == 0) {
    ii->iclass->dispatchParents.add(dtObject);
    dtObject->dispatchChildren.add(ii->iclass);
  }
  FnSymbol* fn = ii->iterator;
  for_alist(expr, fn->body->body)
    expr->remove();
  fn->defPoint->remove();
  fn->retType = ii->irecord;
  Symbol* iterator = newTemp("_ir", ii->irecord);
  fn->insertAtTail(new DefExpr(iterator));
  forv_Vec(Symbol, local, locals) {
    Symbol* field = local2field.get(local);
    if (toArgSymbol(local)) {
      if (local->type == field->type->refType) {
        Symbol* tmp = newTemp(field->type);
        fn->insertAtTail(new DefExpr(tmp));
        fn->insertAtTail(
          new CallExpr(PRIM_MOVE, tmp,
            new CallExpr(PRIM_DEREF, local)));
        fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, iterator, field, tmp));
      } else {
        fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, iterator, field, local));
      }
    }
  }
  fn->insertAtTail(new CallExpr(PRIM_RETURN, iterator));
  ii->getValue->defPoint->insertAfter(new DefExpr(fn));
  fn->addFlag(FLAG_INLINE);
}


static void
rebuildGetIterator(IteratorInfo* ii) {
  FnSymbol* getIterator = ii->getIterator;
  Symbol* ret = getIterator->getReturnSymbol();
  ArgSymbol* arg = getIterator->getFormal(1);
  getIterator->insertBeforeReturn(new CallExpr(PRIM_SET_MEMBER, ret, ii->iclass->getField("more"), new_IntSymbol(1)));
  for_fields(field, ii->irecord) {
    VarSymbol* tmp = newTemp(field->type);
    getIterator->insertBeforeReturn(new DefExpr(tmp));
    getIterator->insertBeforeReturn(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, arg, field)));
    getIterator->insertBeforeReturn(new CallExpr(PRIM_SET_MEMBER, ret, ii->iclass->getField(field->name), tmp));
  }
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
  SymbolMap local2field;  // map from arg/local to class field
  SymbolMap local2rfield; // map from arg to record field
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
  super->addFlag(FLAG_SUPER_CLASS);
  ii->iclass->fields.insertAtTail(new DefExpr(super));

  int i = 0;
  forv_Vec(Symbol, local, locals) {
    const char* fieldName = (local == fn->getReturnSymbol())
      ? "value"
      : astr("F", istr(i++), "_", local->name);
    Type* type = local->type;
    if (type->symbol->hasFlag(FLAG_REF) && local == fn->_this)
      type = type->getValType();
    Symbol* field = new VarSymbol(fieldName, type);
    local2field.put(local, field);
    ii->iclass->fields.insertAtTail(new DefExpr(field));
    if (isArgSymbol(local)) {
      Symbol* rfield = new VarSymbol(fieldName, type);
      local2rfield.put(local, rfield);
      ii->irecord->fields.insertAtTail(new DefExpr(rfield));
    }
  }
  ii->iclass->fields.insertAtTail(new DefExpr(new VarSymbol("more", dtInt[INT_SIZE_DEFAULT])));

  replaceLocalsWithFields(ii, asts, local2field, locals);
  if (!fn->hasFlag(FLAG_INLINE_ITERATOR)) {
    if (singleLoop) {
      buildZip1(ii, asts, singleLoop);
      buildZip2(ii, asts, singleLoop);
      buildZip3(ii, asts, singleLoop);
      buildZip4(ii, asts, singleLoop);
    } else {
      ii->zip1->insertAtTail(new CallExpr(ii->advance, ii->zip1->_this));
      ii->zip1->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
      ii->zip2->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
      ii->zip3->insertAtTail(new CallExpr(ii->advance, ii->zip3->_this));
      ii->zip3->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
      ii->zip4->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    }
    buildAdvance(fn, asts, local2field, locals);
    buildHasMore(ii);
    buildGetValue(ii);
  }
  rebuildIterator(ii, local2rfield, locals);
  rebuildGetIterator(ii);
}

