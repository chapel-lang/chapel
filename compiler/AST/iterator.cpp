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

//  se -- A sym expression which accesses a live local variable.
//  ic -- The iterator class containing the given field.
static void replaceLocalWithFieldTemp(SymExpr* se, Symbol* ic, Symbol* field,
                                      bool is_def, bool is_use, Vec<BaseAST*>& asts)
{
  // Get the expression that sets or uses the symexpr.
  CallExpr* call = toCallExpr(se->parentExpr);
  
  // Create a new temp and load the field value into it.
  VarSymbol* tmp = newTemp(se->var->type);
  // Find the statement containing the symexpr access.
  Expr* stmt = se->getStmtExpr();
  // It's either a block statment or a loop.
  BlockStmt* loop = NULL;
  if (!stmt->list) {    // If a loop, we assume the statment list is null.
    loop = toBlockStmt(se->parentExpr->parentExpr);
    INT_ASSERT(loop);   // Check our assumption.

    // We use 'stmt' as the insertion point for declarations and loads 
    // (initial loads if the access lies within a loop).
    // So if the access lies within a loop, we hoist the insertion point
    // to just before the loop.
    stmt = loop;
  }

  if (call && call->isPrimitive(PRIM_GET_MEMBER)) {
    // Get member returns the address of the member, so we convert the 
    // type of the corresponding temp to a reference type.
    INT_ASSERT(tmp->type->refType);
    tmp->type = tmp->type->refType;
  }

  // OK, insert the declaration.
  stmt->insertBefore(new DefExpr(tmp));
  asts.add(tmp->defPoint);  // hilde sez: I don't think this is necessary.

  // If this symexpr is used here,
  if (is_use) {
    // Load the temp from the field before the statement that uses it.
    if (tmp->type == field->type->refType)
      stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, ic, field)));
    else
      stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ic, field)));
    // If in a loop, we also have to reload this temp at the bottom of the loop.
    if (loop) {
      if (tmp->type == field->type->refType)
        loop->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER, ic, field)));
      else
        loop->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ic, field)));
    }
  }

// This conditional assertion fails.
// That means we probably need to update the definition of findDefUseSets()
// so it recognizes PRIM_SET_MEMBER as a definition 
// (of the member, tho maybe of the containing struct as well).
//  if (call && call->isPrimitive(PRIM_SET_MEMBER)) {
//    INT_ASSERT(is_def);
//  }

  // If the symexpr is set here,
  if (is_def ||
      // Isn't this redundant? If not, why not?
      (call && call->isPrimitive(PRIM_SET_MEMBER))) {
    ArgSymbol* arg = toArgSymbol(se->var);
    if (arg)
    {
      if (arg->intent == INTENT_INOUT ||
          arg->intent == INTENT_OUT ||
          arg->intent == INTENT_REF) {
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
      }
      else
      {
        // Arg symbols which do not have a writeback component are not written back.
      }
    } else {
      if (loop) {
        // The writeback (to the class field) occurs at the head of the loop.
        loop->insertAtHead(new CallExpr(PRIM_SET_MEMBER, ic, field, tmp));
        // Why not after as well?
      } else {
        // Otherwise (not a loop)
        // The writeback occurs after the statement.
        stmt->insertAfter(new CallExpr(PRIM_SET_MEMBER, ic, field, tmp));
      }
    }
  }
  // After all of that, the local variable is bridged out and the temp used instead.
  se->var = tmp;
}


// In the body of an iterator function, replace references to local variables
// with references to fields in the iterator class instead.
static void
replaceLocalsWithFields(FnSymbol* fn,           // the iterator function
                        Vec<BaseAST*>& asts,    // the asts in that function, listed postorder.*
                        SymbolMap& local2field, // Map: local symbol --> class field
                        Vec<Symbol*>& locals) { // The list of live locals in the iterator body.
// *TODO: Can we just regenerate this ast list locally?

  IteratorInfo* ii = fn->iteratorInfo;
  Symbol* ic = ii->advance->getFormal(1);   // The iterator class

  Vec<SymExpr*> defSet;
  Vec<SymExpr*> useSet;
  buildDefUseSets(locals, fn, defSet, useSet);

  // Traverse the asts in the iterator and select just the sym expressions.
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* se = toSymExpr(ast)) {

      // Ignore symexprs that are not in the tree.
      if (!se->parentSymbol)
        continue;

      // SymExpr is among those we are interested in: def or use of a live local.
      if (useSet.set_in(se) || defSet.set_in(se)) {

        // Get the corresponding field in the iterator class.
        Symbol* field = local2field.get(se->var);

        // Get the expression that sets or uses the symexpr.
        CallExpr* call = toCallExpr(se->parentExpr);

        if (call && call->isPrimitive(PRIM_ADDR_OF)) {
          // Convert (addr of var) to (. _ic field).
          call->primitive = primitives[PRIM_GET_MEMBER];
          call->insertAtHead(ic);
          se->var = field;
        } else {
          replaceLocalWithFieldTemp(se, ic, field,
                                    defSet.set_in(se), useSet.set_in(se), asts);
        }
      }
    }
  }
}


// Build the zip1 function, copying expression out of the iterator body up to the singleLoop.
static void
buildZip1(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {

  // In copied expressions, replace _ic --> zip1->_this .
  SymbolMap map;
  Symbol* ic = ii->advance->getFormal(1);
  map.put(ic, ii->zip1->_this);

  // Copy def expressions which are not arg defs.
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!isArgSymbol(def->sym))
        ii->zip1->insertAtTail(def->copy(&map));
  }

  // Copy all iterator body expressions which are not declarations.
  for_alist(expr, ii->iterator->body->body) {
    // Quit if we reach singleLoop (?)
    if (expr == singleLoop)
      break;
    if (!isDefExpr(expr))
      ii->zip1->insertAtTail(expr->copy(&map));
  }

  // Complete the rest of the function.
  CallExpr* blockInfo = singleLoop->blockInfo->copy(&map);
  ii->zip1->insertAtTail(new CondStmt(blockInfo->get(1)->remove(),
                                      new CallExpr(PRIM_SET_MEMBER, ii->zip1->_this, ii->iclass->getField("more"), new_IntSymbol(1)),
                                      new CallExpr(PRIM_SET_MEMBER, ii->zip1->_this, ii->iclass->getField("more"), new_IntSymbol(0))));
  ii->zip1->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
}


// Build the zip2 function, copying expressions out of the singleLoop body up to the first yield.
static void
buildZip2(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {

  // In copied expressions, replace _ic with zip2->_this .
  SymbolMap map;
  Symbol* ic = ii->advance->getFormal(1);
  map.put(ic, ii->zip2->_this);

  // Copy non-arg def expressions.
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


// Build the zip3 function, copying expressions out of the singleLoop body up to the yield
// but also copy non-declarations following the yield.
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


// Build the zip4 function, copying expressions out of the iterator body, up to singleLoop,
// but also copy exprs that have a next pointer.
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


// Build hasMore function.
static void
buildHasMore(IteratorInfo* ii) {
  VarSymbol* tmp = newTemp(ii->hasMore->retType);
  ii->hasMore->insertAtTail(new DefExpr(tmp));
  ii->hasMore->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ii->hasMore->getFormal(1), ii->iclass->getField("more"))));
  ii->hasMore->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
}


// Build getValue function.
static void
buildGetValue(IteratorInfo* ii) {
  VarSymbol* tmp = newTemp(ii->getValue->retType);
  ii->getValue->insertAtTail(new DefExpr(tmp));
  ii->getValue->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ii->getValue->getFormal(1), ii->iclass->getField("value"))));
  ii->getValue->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
}


// Collect local variables that are live at the point of any yield.
static void collectLiveLocalVariables(Vec<Symbol*>& syms, FnSymbol* fn, BlockStmt* singleLoop)
{
  Vec<Symbol*> locals;
  Map<Symbol*,int> localMap;
  Vec<SymExpr*> useSet;
  Vec<SymExpr*> defSet;
  std::vector<BitVec*> OUT;

  liveVariableAnalysis(fn, locals, localMap, useSet, defSet, OUT);

  int block = 0;
  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    bool collect = false;
    for_vector(Expr, expr, bb->exprs) {
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
        if (OUT[block]->get(j))
          live.set(j);
      }
      for (int k = bb->exprs.size() - 1; k >= 0; k--) {
        CallExpr* call = toCallExpr(bb->exprs[k]);
        if ((call && call->isPrimitive(PRIM_YIELD)) ||
            (singleLoop && bb->exprs[k] == singleLoop->next) ||
            (singleLoop && bb->exprs[k] == singleLoop->body.head)) {
          for (int j = 0; j < locals.n; j++) {
            if (live.get(j)) {
              syms.add_exclusive(locals.v[j]);
            }
          }
        }
        Vec<SymExpr*> symExprs;
        collectSymExprs(bb->exprs[k], symExprs);
        forv_Vec(SymExpr, se, symExprs) {
          if (defSet.set_in(se))
            live.unset(localMap.get(se->var));
          if (useSet.set_in(se))
            live.set(localMap.get(se->var));
        }
      }
    }
    block++;
  }

  for_vector(BitVec, out, OUT)
    delete out, out = 0;
}


static bool containsRefVar(Vec<Symbol*>& syms, FnSymbol* fn)
{
  Symbol* ret = fn->getReturnSymbol();

  forv_Vec(Symbol, sym, syms)
    if (sym != ret && !isArgSymbol(sym) && sym->type->symbol->hasFlag(FLAG_REF))
      return true;

  return false;
}


static void insertLocalsForRefs(Vec<Symbol*>& syms, FnSymbol* fn)
{
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(fn, defMap, useMap);

  // Walk the variables in this (iterator) function
  // which are not the return symbol nor argument, and are ref symbols.
  Symbol* ret = fn->getReturnSymbol();
  forv_Vec(Symbol, sym, syms) {
    if (sym == ret || isArgSymbol(sym))
      continue;

    if (sym->type->symbol->hasFlag(FLAG_REF)) {

      Vec<SymExpr*>* defs = defMap.get(sym);
      if (defs->n != 1) {
        INT_FATAL(sym, "invalid assumption about reference");
      }

      CallExpr* move = toCallExpr(defs->v[0]->parentExpr);
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
        syms.add_exclusive(rhs->var);
      } else if (FnSymbol* fn = call->isResolved()) {
        for_actuals(actual, call) {
          SymExpr* se = toSymExpr(actual);
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

  collectLiveLocalVariables(syms, fn, singleLoop);

#ifdef DEBUG_LIVE
  printf("LIVE at Yield Points\n");
  forv_Vec(Symbol, sym, syms) {
    printf("%s[%d]\n", sym->name, sym->id);
  }
  printf("\n");
#endif

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
  if (containsRefVar(syms, fn))
    insertLocalsForRefs(syms, fn);
}


// Collect all local variables
// (Called only if live variable analysis is turned off.)
static void
addAllLocalVariables(Vec<Symbol*>& syms, Vec<BaseAST*>& asts) {
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (VarSymbol* var = toVarSymbol(def->sym))
        if (!var->type->symbol->hasFlag(FLAG_REF) || var->hasFlag(FLAG_INDEX_VAR))
          syms.add(var);
  }
}

// Is 'theCall' the only call to 'callee' ?
static bool
noOtherCalls(FnSymbol* callee, CallExpr* theCall) {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call != theCall && call->inTree()) {
      if (FnSymbol* rc = call->isResolved()) {
        if (rc == callee)
          return false;
      }
    }
  }
  return true;
}


// Preceding calls to the various build...() functions have copied out interesting parts
// of the iterator function.
// This function rips the guts out of the original iterator function and replaces them
// with a simple function that just initializes the fields in the iterator class
// with formal arguments of the original iterator that are live at yield sites within it.
static void
rebuildIterator(IteratorInfo* ii,
                SymbolMap& local2field,
                Vec<Symbol*>& locals) {

  if (ii->iclass->dispatchParents.n == 0) {
    // Insert this iterator class into the iterator class hierarchy at the top level
    // (just below 'object').
    ii->iclass->dispatchParents.add(dtObject);
    dtObject->dispatchChildren.add(ii->iclass);
  }

  // Remove the original iterator function.
  FnSymbol* fn = ii->iterator;
  Vec<CallExpr*> icalls;
  collectCallExprs(fn, icalls);
  // ... and the task functions that it calls.
  forv_Vec(CallExpr, call, icalls) {
    if (FnSymbol* taskFn = resolvedToTaskFun(call)) {
      // What to do if multiple calls? may or may not cause unwanted deletion.
      if (false) // this assert is expensive to compute
        INT_ASSERT(noOtherCalls(taskFn, call));
      taskFn->defPoint->remove();
    }
  }
  for_alist(expr, fn->body->body)
    expr->remove();
  fn->defPoint->remove();

  // Now the iterator creates and returns a copy of the iterator record.
  fn->retType = ii->irecord;
  Symbol* iterator = newTemp("_ir", ii->irecord);
  fn->insertAtTail(new DefExpr(iterator));

  // For each live argument
  forv_Vec(Symbol, local, locals) {
    if (!toArgSymbol(local)) 
      continue;

    // Get the corresponding field in the iterator class
    Symbol* field = local2field.get(local);

    if (local->type == field->type->refType) {
      // If a ref var, 
      // load the local into a temp and then set the value of the corresponding field.
      Symbol* tmp = newTemp(field->type);
      fn->insertAtTail(new DefExpr(tmp));
      fn->insertAtTail(
        new CallExpr(PRIM_MOVE, tmp,
                     new CallExpr(PRIM_DEREF, local)));
      fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, iterator, field, tmp));
    } else {
      // Otherwise, just set the iterator class field directly.
      fn->insertAtTail(new CallExpr(PRIM_SET_MEMBER, iterator, field, local));
    }
  }

  // Return the filled-in iterator record.
  fn->insertAtTail(new CallExpr(PRIM_RETURN, iterator));
  ii->getValue->defPoint->insertAfter(new DefExpr(fn));
  fn->addFlag(FLAG_INLINE);
}


// Fills in the body of the getIterator function.
static void
rebuildGetIterator(IteratorInfo* ii) {
  FnSymbol* getIterator = ii->getIterator;
  Symbol* ret = getIterator->getReturnSymbol();
  ArgSymbol* arg = getIterator->getFormal(1);   // This is the iterator record instance.

  // Set it iterator class (state object) so it initially signals more elements available.
  getIterator->insertBeforeReturn(new CallExpr(PRIM_SET_MEMBER, ret, ii->iclass->getField("more"), new_IntSymbol(1)));

  // Enumerate the fields in the iterator record (argument).
  for_fields(field, ii->irecord) {
    // Load the record field into a temp, and then use that to set the corresponding class field.
    VarSymbol* tmp = newTemp(field->type);
    getIterator->insertBeforeReturn(new DefExpr(tmp));
    getIterator->insertBeforeReturn(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, arg, field)));
    getIterator->insertBeforeReturn(new CallExpr(PRIM_SET_MEMBER, ret, ii->iclass->getField(field->name), tmp));
  }

  // The return is supplied in the shell function created during functionResolution.
  // (See protoIteratorClass().)
}


// Fills in the iterator class and record types with fields corresponding to the 
// local variables defined in the iterator function (or its static context)
// and live at any yield.
static void addLocalsToClassAndRecord(Vec<Symbol*>& locals, FnSymbol* fn, 
                                      SymbolMap& local2field, SymbolMap& local2rfield)
{
  IteratorInfo* ii = fn->iteratorInfo;

  // The iterator class derives from dtObject.
  // This gives it a cid, so it can have polymorphic behavior.
  Symbol* super = new VarSymbol("super", dtObject);
  super->addFlag(FLAG_SUPER_CLASS);
  ii->iclass->fields.insertAtTail(new DefExpr(super));

  int i = 0;    // This numbers the fields.
  forv_Vec(Symbol, local, locals) {

    // The field name is "value" for the return value of the iterator,
    // or F<int>_<local->name> otherwise.
    const char* fieldName = (local == fn->getReturnSymbol())
      ? "value"
      : astr("F", istr(i++), "_", local->name);

    // The return value is automatically dereferenced (I guess).
    Type* type = local->type;
    if (type->symbol->hasFlag(FLAG_REF) && local == fn->_this)
      type = type->getValType();

    // Add a field to the class
    Symbol* field = new VarSymbol(fieldName, type);
    local2field.put(local, field);
    ii->iclass->fields.insertAtTail(new DefExpr(field));

    // Only (live) arguments are added to the record.
    if (isArgSymbol(local)) {
      Symbol* rfield = new VarSymbol(fieldName, type);
      local2rfield.put(local, rfield);
      ii->irecord->fields.insertAtTail(new DefExpr(rfield));
    }
  }
  // Is this not a Boolean?
  ii->iclass->fields.insertAtTail(new DefExpr(new VarSymbol("more", dtInt[INT_SIZE_DEFAULT])));
}


// Function resolution just adds a shell for the iterator records and classes.
// This function takes a pointer to an iterator and fills in those types.
void lowerIterator(FnSymbol* fn) {

  SET_LINENO(fn);
  Vec<BaseAST*> asts;
  collect_asts_postorder(fn, asts);

  BlockStmt* singleLoop = NULL;
  if (CallExpr* singleLoopYield = isSingleLoopIterator(fn, asts))
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
  else
    addLiveLocalVariables(locals, fn, singleLoop);
  locals.add_exclusive(fn->getReturnSymbol());

  addLocalsToClassAndRecord(locals, fn, local2field, local2rfield);

  replaceLocalsWithFields(fn, asts, local2field, locals);

  IteratorInfo* ii = fn->iteratorInfo;
  if (!fn->hasFlag(FLAG_INLINE_ITERATOR)) {
    // Does this force iterators marked as inline to be inlined
    // even if --no-inline-iterators is specified?
    // Isn't that bad?
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

