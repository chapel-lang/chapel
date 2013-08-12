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


// Return the PRIM_YIELD CallExpr* or NULL.
static inline CallExpr* asYieldExpr(BaseAST* e) {
  if (CallExpr* call = toCallExpr(e))
    if (call->isPrimitive(PRIM_YIELD))
      return call;
  return NULL;
}
static inline CallExpr* parentYieldExpr(SymExpr* se) {
  return asYieldExpr(se->parentExpr);
}


// The set of FnSymbols where we have removed the initialization
// (via PRIM_INIT) of 'ret' symbols.
Vec<FnSymbol*> iteratorsWithRemovedRetInitSet;

// Helper for removeRetSymbolAndUses().
static inline void
removeRetInitialization(DefExpr* rdef, Symbol* rsym) {
  for (Expr* stmt = rdef->next; stmt; stmt = stmt->next) {
    // If the iterator has a declared return type, 'sym' is
    // initialized to its type's default value.
    // Find that and yank it.
    if (CallExpr* call = toCallExpr(stmt)) {
      if (call->isPrimitive(PRIM_MOVE)) {
        if (SymExpr* callTarget = toSymExpr(call->get(1))) {
          if (callTarget->var == rsym) {
            call->remove();
            return;
          }
        }
      }
    }
  }

  // We did not find the initialization even though we expected it.
  //
  // BTW the above is executed only when we expect the initialization.
  // I.e. when the return type was specified explicitly and we have
  // not removed the initialization. Otherwise we avoid traversing the
  // (top-level?) statement list (from rdef->next), in the name of
  // reducing the compilation time.
  //
  INT_ASSERT(false);
}


//
// Now that we have localized yield symbols, the return symbol
// and the PRIM_RETURN CallExpr are not needed and would cause trouble.
// Returns the type yielded by the iterator. (fn->retType is not it.)
//
static Type*
removeRetSymbolAndUses(FnSymbol* fn) {
  // follows getReturnSymbol()
  CallExpr* ret = toCallExpr(fn->body->body.last());
  INT_ASSERT(ret && ret->isPrimitive(PRIM_RETURN));
  SymExpr* rse = toSymExpr(ret->get(1));
  INT_ASSERT(rse);
  Symbol*  rsym = rse->var;
  DefExpr* rdef = rsym->defPoint;

  // Yank the return statement.
  ret->remove();
  // Yank rsym's initialization. We expect it only when
  // the iterator's return type was specified explicitly.
  if (fn->hasFlag(FLAG_SPECIFIED_RETURN_TYPE) &&
      !iteratorsWithRemovedRetInitSet.set_in(fn))
    removeRetInitialization(rdef, rsym);
  // Yank rsym's definition.
  rdef->remove();

  // We could assert here that no uses of rsym remain.
  // But that's expensive for large iterators. Instead,
  // if we overlooked something, we will get a verification
  // failure "SymExpr::var::defPoint is not in AST".

  return rsym->type;
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
                        Vec<BaseAST*>& asts,    // the asts in that function, listed postorder.
                        SymbolMap& local2field, // Map: local symbol --> class field
                        Vec<Symbol*>& yldSymSet,// The set of locals that appear in yields.
                        Symbol* valField,       // ic.value field - value being yielded.
                        Vec<Symbol*>& locals) { // The list of live locals in the iterator body.

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

      if (CallExpr* pc = parentYieldExpr(se)) {
        // SymExpr is in a yield.

        Symbol* ySym = se->var;
        if (ySym->defPoint->parentSymbol == fn) {
          // This is a local symbol. It must have been assigned to,
          // and that assignment must have been converted to a store
          // to ic.value. No need to do anything. The yield remains as-is.

          // While here, sanity-check a couple of things.
          INT_ASSERT(yldSymSet.set_in(ySym));
          INT_ASSERT(local2field.get(ySym) == valField);

        } else {
          // This is a non-local symbol. We do not track assignments to these.
          // So we need to assign it to ic.value. The yield remains as-is.
          pc->insertBefore(new CallExpr(PRIM_SET_MEMBER, ic, valField, ySym));
        }
      } else if (useSet.set_in(se) || defSet.set_in(se)) {
        // SymExpr is among those we are interested in: def or use of a live local.

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
        INT_ASSERT(false); // should have been removed with removeRetSymbolAndUses()
      }
    }
  }

  // iterator is done ==> more=0
  end->defPoint->insertBefore(new CallExpr(PRIM_SET_MEMBER, ic, ii->iclass->getField("more"), new_IntSymbol(0)));

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


static bool containsRefVar(Vec<Symbol*>& syms, FnSymbol* fn,
                           Vec<Symbol*>& yldSymSet)
{
  forv_Vec(Symbol, sym, syms)
    if (!isArgSymbol(sym) && sym->type->symbol->hasFlag(FLAG_REF) &&
        !yldSymSet.set_in(sym))
      return true;

  return false;
}


static void insertLocalsForRefs(Vec<Symbol*>& syms, FnSymbol* fn,
                                Vec<Symbol*>& yldSymSet)
{
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(fn, defMap, useMap);

  // Walk the variables in this (iterator) function
  // which are not the return symbol nor argument, and are ref symbols.
  forv_Vec(Symbol, sym, syms) {
    if (isArgSymbol(sym) || yldSymSet.set_in(sym))
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
addLiveLocalVariables(Vec<Symbol*>& syms, FnSymbol* fn, BlockStmt* singleLoop,
                      Vec<Symbol*>& yldSymSet)
{
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
  if (containsRefVar(syms, fn, yldSymSet))
    insertLocalsForRefs(syms, fn, yldSymSet);
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
      // TODO: This forv + filter casts a wide net.
      // Try to make the filter reject as many cases as possible
      // by first matching on the callee and then testing if call == theCall.
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


// All "newRet" symbols used in yield expressions will need special handling,
// so we collect them into a set.
static void
collectYieldSymbols(FnSymbol* fn, Vec<BaseAST*>& asts, Vec<Symbol*>& yldSymSet)
{
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* yCall = asYieldExpr(ast)) {
      SymExpr* ySymExpr = toSymExpr(yCall->get(1));
      INT_ASSERT(ySymExpr);
      Symbol* ySym = ySymExpr->var;
      if (ySym->defPoint->parentSymbol == fn) {
        // It is a local symbol, add it.
        yldSymSet.set_add(ySym);
      } else {
        // Non-local symbols should not be considered.
        // FYI. Or can this be anything else?
        INT_ASSERT(ySym->isConstant() || ySym->isParameter());
      }
    }
  }
}


// Apparently the yield symbols are not considered "live" at yield points.
// So have to add them manually.
static void
addYieldSymbols(Vec<Symbol*>& locals, Vec<Symbol*>& yldSymSet) {
  forv_Vec(Symbol, ySym, yldSymSet) {
    if (ySym)
      locals.add_exclusive(ySym);
  }
}


// After replaceLocalsWithFields() all locals are replaced with field accesses.
static void
removeLocals(Vec<Symbol*>& locals, Vec<BaseAST*>& asts, Vec<Symbol*>& yldSymSet, FnSymbol* fn) {
  forv_Vec(Symbol, l, locals) {
    INT_ASSERT(l->defPoint->parentSymbol == fn);
    if (!isArgSymbol(l))
      l->defPoint->remove();
  }
}


// Creates (and returns) an iterator class field.
// 'type' is used if local==NULL.
static inline Symbol* createICField(int& i, Symbol* local, Type* type,
                                    bool isYieldSym, FnSymbol* fn) {
  // The field name is "value" for the return value of the iterator,
  // or F<int>_<local->name> otherwise.
  const char* fieldName = isYieldSym
    ? "value"
    : astr("F", istr(i++), "_", local->name);

  if (local) {
    type = local->type;
    // The return value is automatically dereferenced (I guess).
    if (local == fn->_this && type->symbol->hasFlag(FLAG_REF))
      type = type->getValType();
  }

  // Add a field to the class
  Symbol* field = new VarSymbol(fieldName, type);
  fn->iteratorInfo->iclass->fields.insertAtTail(new DefExpr(field));

  return field;
}

// Fills in the iterator class and record types with fields corresponding to the 
// local variables defined in the iterator function (or its static context)
// and live at any yield.
static void addLocalsToClassAndRecord(Vec<Symbol*>& locals, FnSymbol* fn, 
                                      Vec<Symbol*>& yldSymSet, Type* yieldedType,
                                      Symbol** valFieldRef,
                                      SymbolMap& local2field, SymbolMap& local2rfield)
{
  IteratorInfo* ii = fn->iteratorInfo;
  Symbol* valField = NULL;

  int i = 0;    // This numbers the fields.
  forv_Vec(Symbol, local, locals) {
    Symbol* field = NULL;
    bool isYieldSym = yldSymSet.set_in(local);
    if (isYieldSym) {
      INT_ASSERT(local->type == yieldedType);
    }

    if (isYieldSym && valField) {
      // We have already computed the corresponding field.
      // Return symbols are never arguments, so not in local2rfield.
      local2field.put(local, valField);

    } else {
      field = createICField(i, local, NULL, isYieldSym, fn);
      local2field.put(local, field);
      if (isYieldSym) {
        valField = field;
      }
    }

    // Only (live) arguments are added to the record.
    if (isArgSymbol(local)) {
      // 'field' is NULL only if isYieldSym - but then 'local' is not an arg.
      INT_ASSERT(field);
      Symbol* rfield = new VarSymbol(field->name, field->type);
      local2rfield.put(local, rfield);
      ii->irecord->fields.insertAtTail(new DefExpr(rfield));
    }
  }

  ii->iclass->fields.insertAtTail(new DefExpr(new VarSymbol("more", dtInt[INT_SIZE_DEFAULT])));

  if (!valField) {
    valField = createICField(i, NULL, yieldedType, true, fn);
  }
  *valFieldRef = valField;
}


// Function resolution just adds a shell for the iterator records and classes.
// This function takes a pointer to an iterator and fills in those types.
void lowerIterator(FnSymbol* fn) {

  SET_LINENO(fn);
  Vec<BaseAST*> asts;
  Type* yieldedType = removeRetSymbolAndUses(fn);
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
  Vec<Symbol*> yldSymSet;
  Symbol* valField;

  for_formals(formal, fn)
    locals.add(formal);

  collectYieldSymbols(fn, asts, yldSymSet);

  if (fNoLiveAnalysis)
    addAllLocalVariables(locals, asts);
  else
    addLiveLocalVariables(locals, fn, singleLoop, yldSymSet);

  addYieldSymbols(locals, yldSymSet);

  addLocalsToClassAndRecord(locals, fn, yldSymSet, yieldedType, &valField,
                            local2field, local2rfield);

  replaceLocalsWithFields(fn, asts, local2field, yldSymSet, valField, locals);

  removeLocals(locals, asts, yldSymSet, fn);

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
