#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "stmt.h"
#include "iterator.h"
#include "optimizations.h"
#include "view.h"


//#define DEBUG_LIVE


IteratorInfo::IteratorInfo() :
  classType(NULL),
  advance(NULL),
  hasMore(NULL),
  getValue(NULL),
  zip1(NULL),
  zip2(NULL),
  zip3(NULL),
  zip4(NULL)
{}


static FnSymbol*
buildEmptyIteratorMethod(const char* name, ClassType* ct) {
  FnSymbol* fn = new FnSymbol(name);
  fn->addPragma("auto ii"); 
  fn->global = true;
  fn->insertFormalAtTail(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken));
  fn->_this = new ArgSymbol(INTENT_BLANK, "this", ct);
  fn->insertFormalAtTail(fn->_this);
  return fn;
}


static VarSymbol* newTemp(FnSymbol* fn, Type* type, const char* name = "_tmp") {
  VarSymbol* var = new VarSymbol(name, type);
  var->isCompilerTemp = true;
  fn->insertAtHead(new DefExpr(var));
  return var;
}


void prototypeIteratorClass(FnSymbol* fn) {
  currentLineno = fn->lineno;

  IteratorInfo* ii = new IteratorInfo();
  fn->iteratorInfo = ii;

  ii->classType = new ClassType(CLASS_CLASS);
  const char* className = astr("_ic_", fn->name);
  if (fn->_this)
    className = astr(className, "_", fn->_this->type->symbol->cname);
  TypeSymbol* cts = new TypeSymbol(className, ii->classType);
  cts->addPragma("iterator class");
  cts->addPragma("no object");
  if (fn->retTag == RET_VAR)
    cts->addPragma("ref iterator class");
  fn->defPoint->insertBefore(new DefExpr(cts));

  ii->advance = buildEmptyIteratorMethod("advance", ii->classType);
  ii->advance->retType = dtVoid;

  ii->zip1 = buildEmptyIteratorMethod("zip1", ii->classType);
  ii->zip1->retType = dtVoid;

  ii->zip2 = buildEmptyIteratorMethod("zip2", ii->classType);
  ii->zip2->retType = dtVoid;

  ii->zip3 = buildEmptyIteratorMethod("zip3", ii->classType);
  ii->zip3->retType = dtVoid;

  ii->zip4 = buildEmptyIteratorMethod("zip4", ii->classType);
  ii->zip4->retType = dtVoid;

  ii->hasMore = buildEmptyIteratorMethod("hasMore", ii->classType);
  ii->hasMore->retType = dtInt[INT_SIZE_32];
  ii->hasMore->addPragma("inline");

  ii->getValue = buildEmptyIteratorMethod("getValue", ii->classType);
  ii->getValue->retType = fn->retType;
  ii->getValue->addPragma("inline");

  fn->defPoint->insertBefore(new DefExpr(ii->zip1));
  fn->defPoint->insertBefore(new DefExpr(ii->zip2));
  fn->defPoint->insertBefore(new DefExpr(ii->zip3));
  fn->defPoint->insertBefore(new DefExpr(ii->zip4));
  fn->defPoint->insertBefore(new DefExpr(ii->advance));
  fn->defPoint->insertBefore(new DefExpr(ii->hasMore));
  fn->defPoint->insertBefore(new DefExpr(ii->getValue));

  ii->classType->defaultConstructor = fn;
  ii->classType->scalarPromotionType = fn->retType;
  fn->retType = ii->classType;
  fn->retTag = RET_VALUE;
}


//
// when ast is a function fn
//   insert "ic.f = v" or "t = &v; ic.f = t" at tail of fn
// when ast is an expression expr
//   insert "ic.f = v" or "t = &v; ic.f = t" after expr
//
static void
insertSetMember(BaseAST* ast, Symbol* ic, Symbol* f, BaseAST* v) {
  Symbol* local = toSymbol(v);
  INT_ASSERT(local);
  
  if (FnSymbol* fn = toFnSymbol(ast)) {
    if (local->type == f->type->refType) {
      Symbol* tmp = newTemp(fn, f->type);
      fn->insertAtTail(
        new CallExpr(PRIMITIVE_MOVE, tmp,
          new CallExpr(PRIMITIVE_GET_REF, local)));
      fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, ic, f, tmp));
    } else
      fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, ic, f, local));
  } else if (Expr* expr = toExpr(ast)) {
    if (local->type == f->type->refType) {
      Symbol* tmp = newTemp(expr->getFunction(), f->type);
      expr->getStmtExpr()->insertAfter(
        new CallExpr(PRIMITIVE_SET_MEMBER, ic, f, tmp));
      expr->getStmtExpr()->insertAfter(
        new CallExpr(PRIMITIVE_MOVE, tmp,
          new CallExpr(PRIMITIVE_GET_REF, local)));
    } else
      expr->getStmtExpr()->insertAfter(
        new CallExpr(PRIMITIVE_SET_MEMBER, ic, f, local));
  } else
    INT_FATAL(ast, "unexpected case in insertSetMember");
}


//
// initialize temp to default value (recursive for records)
//
static void
insertSetMemberInits(FnSymbol* fn, Symbol* var) {
  Type* type = var->type;
  if (type->symbol->hasPragma("ref"))
    type = getValueType(type); // unexecuted none/gasnet on 4/25/08
  if (type->defaultValue) {
    fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, var, type->defaultValue));
  } else {
    ClassType* ct = toClassType(type);
    INT_ASSERT(ct);
    for_fields(field, ct) {
      if (field->type->symbol->hasPragma("ref")) {
        if (getValueType(field->type)->symbol->hasPragma("array"))
          continue; // skips array types
        Symbol* tmp = new VarSymbol("_tmp", field->type);
        tmp->isCompilerTemp = true;
        fn->insertAtTail(new DefExpr(tmp));
        fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, gNilRef));
        fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, var, field, tmp));
      } else if (field->type->refType) { // skips array types (how to handle arrays?) ( sjd later: really? )
        Symbol* tmp = new VarSymbol("_tmp", field->type);
        tmp->isCompilerTemp = true;
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
  BlockStmt* singleFor = NULL;
  CallExpr* singleYield = NULL;
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIMITIVE_YIELD)) {
        if (singleYield) {
          return NULL;
        } else if (BlockStmt* block = toBlockStmt(call->parentExpr)) {
          if (block->loopInfo &&
              (block->loopInfo->isPrimitive(PRIMITIVE_LOOP_FOR) ||
               block->loopInfo->isPrimitive(PRIMITIVE_LOOP_WHILEDO))) {
            singleYield = call;
          } else {
            return NULL;
          }
        } else {
          return NULL;
        }
      }
    } else if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->loopInfo) {
        if (singleFor) {
          return NULL;
        } else if ((block->loopInfo->isPrimitive(PRIMITIVE_LOOP_FOR) ||
                    block->loopInfo->isPrimitive(PRIMITIVE_LOOP_WHILEDO)) &&
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
buildAdvance(FnSymbol* fn,
             Vec<BaseAST*>& asts,
             Map<Symbol*,Symbol*>& local2field,
             Vec<Symbol*>& locals,
             Vec<SymExpr*>& defSet,
             Vec<SymExpr*>& useSet) {
  IteratorInfo* ii = fn->iteratorInfo;
  Symbol* iterator = ii->advance->getFormal(1);

  // replace locals with fields
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* se = toSymExpr(ast)) {
      if (se->parentSymbol && (useSet.set_in(se) || defSet.set_in(se))) {
        CallExpr* call = toCallExpr(se->parentExpr);
        if (call && call->isPrimitive(PRIMITIVE_SET_REF)) {
          Symbol* field = local2field.get(se->var);
          call->primitive = primitives[PRIMITIVE_GET_MEMBER];
          call->insertAtHead(iterator);
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
              stmt->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER, iterator, field)));
            else
              stmt->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, iterator, field)));
            if (loop) {
              if (tmp->type == field->type->refType)
                loop->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER, iterator, field)));
              else
                loop->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, iterator, field)));
            }
          }
          if (defSet.set_in(se) ||
              call && call->isPrimitive(PRIMITIVE_SET_MEMBER)) {
            if (loop) {
              loop->insertAtHead(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, field, tmp));
            } else {
              stmt->insertAfter(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, field, tmp));
            }
          }
          se->var = tmp;
        }
      }
    }
  }

  //
  // build optimized single loop iterator advance methods (zip*)
  //
  CallExpr* yield = isSingleLoopIterator(fn, asts);
  if (!fNoOptimizeLoopIterators && yield) {
    BlockStmt* loop = toBlockStmt(yield->parentExpr);
    INT_ASSERT(loop);
    Map<Symbol*,Symbol*> map;
    bool flag;

    map.clear();
    map.put(iterator, ii->zip1->_this);
    forv_Vec(BaseAST, ast, asts) {
      if (DefExpr* def = toDefExpr(ast))
        if (!isArgSymbol(def->sym))
          ii->zip1->insertAtTail(def->copy(&map));
    }
    for_alist(expr, fn->body->body) {
      if (expr == loop)
        break;
      if (!isDefExpr(expr))
        ii->zip1->insertAtTail(expr->copy(&map));
    }
    CallExpr* loopInfo = loop->loopInfo->copy(&map);
    ii->zip1->insertAtTail(new CondStmt(loopInfo->get(1)->remove(),
                                        new CallExpr(PRIMITIVE_SET_MEMBER, ii->zip1->_this, ii->classType->getField("more"), new_IntSymbol(1)),
                                        new CallExpr(PRIMITIVE_SET_MEMBER, ii->zip1->_this, ii->classType->getField("more"), new_IntSymbol(0))));

    map.clear();
    map.put(iterator, ii->zip2->_this);
    forv_Vec(BaseAST, ast, asts) {
      if (DefExpr* def = toDefExpr(ast))
        if (!isArgSymbol(def->sym))
          ii->zip2->insertAtTail(def->copy(&map));
    }
    for_alist(expr, loop->body) {
      if (expr == yield)
        break;
      if (!isDefExpr(expr))
        ii->zip2->insertAtTail(expr->copy(&map));
    }

    {
      map.clear();
      map.put(iterator, ii->zip3->_this);
      flag = true;
      forv_Vec(BaseAST, ast, asts) {
        if (DefExpr* def = toDefExpr(ast))
          if (!isArgSymbol(def->sym))
            ii->zip3->insertAtTail(def->copy(&map));
      }
      for_alist(expr, loop->body) {
        if (flag) {
          if (expr == yield)
            flag = false;
          continue;
        }
        if (!isDefExpr(expr))
          ii->zip3->insertAtTail(expr->copy(&map));
      }
      CallExpr* loopInfo = loop->loopInfo->copy(&map);
      ii->zip3->insertAtTail(new CondStmt(loopInfo->get(1)->remove(),
                                          new CallExpr(PRIMITIVE_SET_MEMBER, ii->zip3->_this, ii->classType->getField("more"), new_IntSymbol(1)),
                                          new CallExpr(PRIMITIVE_SET_MEMBER, ii->zip3->_this, ii->classType->getField("more"), new_IntSymbol(0))));
    }

    map.clear();
    map.put(iterator, ii->zip4->_this);
    flag = true;
    forv_Vec(BaseAST, ast, asts) {
      if (DefExpr* def = toDefExpr(ast))
        if (!isArgSymbol(def->sym))
          ii->zip4->insertAtTail(def->copy(&map));
    }
    for_alist(expr, fn->body->body) {
      if (flag) {
        if (expr == loop)
          flag = false;
        continue;
      }
      if (!isDefExpr(expr) && expr->next)
        ii->zip4->insertAtTail(expr->copy(&map));
    }
  } else {
    ii->zip1->insertAtTail(new CallExpr(ii->advance, ii->zip1->_this));
    ii->zip3->insertAtTail(new CallExpr(ii->advance, ii->zip3->_this));
  }
  ii->zip1->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
  ii->zip2->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
  ii->zip3->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
  ii->zip4->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
  ii->zip1->addPragma("inline");
  ii->zip2->addPragma("inline");
  ii->zip3->addPragma("inline");
  ii->zip4->addPragma("inline");

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
        call->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, ii->classType->getField("more"), new_IntSymbol(i)));
        call->insertBefore(new GotoStmt(GOTO_NORMAL, end));
        Symbol* label = new LabelSymbol(astr("_jump_", istr(i)));
        call->insertBefore(new DefExpr(label));
        labels.add(label);
        call->remove();
        i++;
      } else if (call->isPrimitive(PRIMITIVE_RETURN)) {
        call->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, iterator, ii->classType->getField("more"), new_IntSymbol(0)));
        call->remove(); // remove old return
      }
    }
  }

  // insert jump table at head of advance
  i = 2;
  Symbol* t1 = newTemp(ii->advance, dtBool);
  Symbol* more = new VarSymbol("more", dtInt[INT_SIZE_32]);

  forv_Vec(Symbol, label, labels) {
    ii->advance->insertAtHead(new CondStmt(new SymExpr(t1), new GotoStmt(GOTO_NORMAL, label)));
    ii->advance->insertAtHead(new CallExpr(PRIMITIVE_MOVE, t1, new CallExpr(PRIMITIVE_EQUAL, more, new_IntSymbol(i++))));
  }
  ii->advance->insertAtHead(new CallExpr(PRIMITIVE_MOVE, more, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, iterator, ii->classType->getField("more"))));
  ii->advance->insertAtHead(new DefExpr(more));
  ii->advance->insertAtTail(new CallExpr(PRIMITIVE_RETURN, gVoid));
}


static void
buildHasMore(FnSymbol* fn) {
  IteratorInfo* ii = fn->iteratorInfo;
  VarSymbol* tmp = new VarSymbol("_tmp", ii->hasMore->retType);
  ii->hasMore->insertAtTail(new DefExpr(tmp));
  ii->hasMore->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, ii->hasMore->getFormal(1), ii->classType->getField("more"))));
  ii->hasMore->insertAtTail(new CallExpr(PRIMITIVE_RETURN, tmp));
}


static void
buildGetValue(FnSymbol* fn) {
  IteratorInfo* ii = fn->iteratorInfo;
  VarSymbol* tmp = new VarSymbol("_tmp", ii->getValue->retType);
  ii->getValue->insertAtTail(new DefExpr(tmp));
  ii->getValue->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, ii->getValue->getFormal(1), ii->classType->getField("value"))));
  ii->getValue->insertAtTail(new CallExpr(PRIMITIVE_RETURN, tmp));
}


static void
addLocalVariablesLiveAtYields(Vec<Symbol*>& syms, FnSymbol* fn, CallExpr* singleLoopYield) {
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
  Vec<Vec<bool>*> OUT;
  liveVariableAnalysis(fn, locals, localMap, useSet, defSet, OUT);

  BlockStmt* loop = NULL;
  if (singleLoopYield)
    loop = toBlockStmt(singleLoopYield->parentExpr);

  int i = 0;
  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    bool collect = false;
    forv_Vec(Expr, expr, bb->exprs) {
      CallExpr* call = toCallExpr(expr);
      if (call && call->isPrimitive(PRIMITIVE_YIELD))
        collect = true;
      if (loop && expr == loop->next)
        collect = true;
      if (loop && expr == loop->body.head)
        collect = true;
    }
    if (collect) {
      Vec<bool> live;
      for (int j = 0; j < locals.n; j++) {
        live.add(OUT.v[i]->v[j]);
      }
      for (int k = bb->exprs.n - 1; k >= 0; k--) {
        CallExpr* call = toCallExpr(bb->exprs.v[k]);
        if (call && call->isPrimitive(PRIMITIVE_YIELD) ||
            loop && bb->exprs.v[k] == loop->next ||
            loop && bb->exprs.v[k] == loop->body.head) {
          for (int j = 0; j < locals.n; j++) {
            if (live.v[j]) {
              syms.add_exclusive(locals.v[j]);
            }
          }
        }
        Vec<BaseAST*> asts;
        collect_asts(bb->exprs.v[k], asts);
        forv_Vec(BaseAST, ast, asts) {
          if (SymExpr* se = toSymExpr(ast)) {
            if (defSet.set_in(se)) {
              live.v[localMap.get(se->var)] = false;
            }
            if (useSet.set_in(se)) {
              live.v[localMap.get(se->var)] = true;
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

  forv_Vec(Vec<bool>, out, OUT)
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
    if (sym != ret && !isArgSymbol(sym) && sym->type->symbol->hasPragma("ref")) {
      foundRef = true;
    }
  }
  if (foundRef) {
    Map<Symbol*,Vec<SymExpr*>*> defMap;
    Map<Symbol*,Vec<SymExpr*>*> useMap;
    buildDefUseMaps(fn, defMap, useMap);
    forv_Vec(Symbol, sym, syms) {
      if (sym != ret && !isArgSymbol(sym) && sym->type->symbol->hasPragma("ref")) {
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
          INT_ASSERT(se->var->type->symbol->hasPragma("ref"));
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
        if (!var->type->symbol->hasPragma("ref") || var->hasPragma("index var"))
          syms.add(var);
  }
}


void lowerIterator(FnSymbol* fn) {
  IteratorInfo* ii = fn->iteratorInfo;

  currentLineno = fn->lineno;
  Vec<BaseAST*> asts;
  collect_asts_postorder(fn, asts);

  // make fields for all local variables and arguments
  // optimization note: only variables live at yield points are required
  Map<Symbol*,Symbol*> local2field;
  Vec<Symbol*> locals;

  for_formals(formal, fn)
    locals.add(formal);
  if (fNoLiveAnalysis)
    addAllLocalVariables(locals, asts);
  else {
    CallExpr* yield = isSingleLoopIterator(fn, asts);
    addLocalVariablesLiveAtYields(locals, fn, yield);
  }
  locals.add_exclusive(fn->getReturnSymbol());

  int i = 0;
  forv_Vec(Symbol, local, locals) {
    const char* fieldName = (local == fn->getReturnSymbol())
      ? "value"
      : astr("F", istr(i++), "_", local->name);
    Type* type = local->type;
    if (type->symbol->hasPragma("ref") && isArgSymbol(local))
      type = getValueType(type);
    Symbol* field = new VarSymbol(fieldName, type);
    local2field.put(local, field);
    ii->classType->fields.insertAtTail(new DefExpr(field));
  }

  ii->classType->fields.insertAtTail(new DefExpr(new VarSymbol("more", dtInt[INT_SIZE_32])));

  Vec<SymExpr*> defSet;
  Vec<SymExpr*> useSet;
  buildDefUseSets(locals, fn, defSet, useSet);
  buildAdvance(fn, asts, local2field, locals, defSet, useSet);
  buildHasMore(fn);
  buildGetValue(fn);

  // rebuild iterator function

  for_alist(expr, fn->body->body)
    expr->remove();
  fn->defPoint->remove();
  fn->retType = ii->classType;
  Symbol* t1 = newTemp(fn, ii->classType);
  fn->insertAtTail(new CallExpr(PRIMITIVE_MOVE, t1, new CallExpr(PRIMITIVE_CHPL_ALLOC, ii->classType->symbol, new_StringSymbol("iterator class"))));
  forv_Vec(Symbol, local, locals) {
    Symbol* field = local2field.get(local);
    if (toArgSymbol(local)) {
      insertSetMember(fn, t1, field, local);
    } else if (isRecordType(local->type)) {
      if (field->type->refType) { // skips array types (how to handle arrays?)
        Symbol* tmp = new VarSymbol("_tmp", field->type);
        tmp->isCompilerTemp = true;
        fn->insertAtTail(new DefExpr(tmp));
        insertSetMemberInits(fn, tmp);
        fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, t1, field, tmp));
      }
    } else if (field->type->symbol->hasPragma("ref")) {
      // do not initialize references
    } else if (field->type->defaultValue) {
      insertSetMember(fn, t1, field, field->type->defaultValue);
    }
  }
  fn->insertAtTail(new CallExpr(PRIMITIVE_SET_MEMBER, t1, ii->classType->getField("more"), new_IntSymbol(1)));
  fn->insertAtTail(new CallExpr(PRIMITIVE_RETURN, t1));
  ii->getValue->defPoint->insertAfter(new DefExpr(fn));
  fn->addPragma("inline");
}
