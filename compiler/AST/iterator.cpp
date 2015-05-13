/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "iterator.h"

#include "astutil.h"
#include "oldCollectors.h"
#include "bb.h"
#include "bitVec.h"
#include "CForLoop.h"
#include "expr.h"
#include "ForLoop.h"
#include "stmt.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "optimizations.h"
#include "view.h"
#include "WhileStmt.h"

//
// This file implements lowerIterator() called by the lowerIterators pass
//
// lowerIterator() rewrites the original iterator function to fill in
// the iterator record and build the various functions needed to build
// the final loop: zip*, hasMore, advance, getValue, and getIterator.
//

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
  getValue(NULL),
  init(NULL),
  incr(NULL)
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

  // Yank the return statement.
  ret->remove();

  // We cannot remove rsym's definition, because rsym
  // may also be referenced in an autoDestroy call.
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
CallExpr*
isSingleLoopIterator(FnSymbol* fn, Vec<BaseAST*>& asts) {
  if (fNoOptimizeLoopIterators)
    return NULL;
  BlockStmt* singleFor = NULL;
  CallExpr* singleYield = NULL;
  forv_Vec(BaseAST, ast, asts) {
    // If a yield statement,
    if (CallExpr* call = toCallExpr(ast)) {
      if (call->isPrimitive(PRIM_YIELD)) {
        if (singleYield) {
          // We already saw a yield stmt.  This is the second one, so fail.
          return NULL;
        }
        
        // Select yield statements whose parent expression is a loop statement
        // (except for dowhile statements, for some reason....

        // This test is not logically related to the preceding quick-exit, so
        // putting "else" here would be misleading.
        if (isLoopStmt(call->parentExpr)) {
          // NOAKES 2014/11/25  It is interesting the DoWhile loops aren't supported
          if (isDoWhileStmt(call->parentExpr))
            return NULL;

          // We expect that ParamForLoops have already been removed from the tree
          if (isParamForLoop(call->parentExpr))
          {
            INT_FATAL(call->parentExpr, "Unexpected ParamForLoop construct.");
          }

          singleYield = call;
        } else {
          return NULL;
        }
      }
    }

    // This clause captures the first loop statement (except for while-do
    // statements, for some reason ...).
    else if (isLoopStmt(ast)) {
      // NOAKES 2014/11/25  It is interesting the DoWhile loops aren't supported
      if (isDoWhileStmt(ast))
        return NULL;

      // We expect that ParamForLoops have already been removed from the tree
      if (isParamForLoop(ast))
      {
        INT_FATAL(ast, "Unexpected ParamForLoop construct.");
      }

      Expr*      expr  = toExpr(ast);
      BlockStmt* block = toBlockStmt(ast);

      if (expr->parentExpr == fn->body) {
        // This captures the first loop statement, but does not fail if there
        // is more than one.  Compare the test for a single yield above.
        // Is this intentional?
        if (singleFor == NULL)
          singleFor = block;
        // 2015-02-23 hilde: TODO: Uncomment the following, and see what breaks
//        else
//          INT_FATAL(expr, "Iterator contains a second for loop.")
        // I think the existing code works because each loop should contain at
        // least one yield, so the second yield causes the singleYield test
        // above to fail and return NULL preemptively.  Bad news if that
        // assumption fails.
        // This question would not arise if the search were rewritten in a more
        // straightforward fashion: First, look for a single loop; then, within
        // that single loop, look for a single yield.  Simple, obvious,
        // foolproof.
        // Also, probably the redundant code would be removed.
      } else {
        return NULL;
      }
    }

    // If the iterator  contains a goto statement, it is not considered to be a
    // single loop iterator.
    else if (isGotoStmt(ast)) {
      return NULL;
    }
  }

  if (singleFor && singleYield) {
    return singleYield;
  } else {
    return NULL;
  }
}

//  se -- A sym expression which accesses a live local variable.
//  ic -- The iterator class containing the given field.
static void replaceLocalWithFieldTemp(SymExpr*       se,
                                      Symbol*        ic,
                                      Symbol*        field,
                                      bool           is_def,
                                      bool           is_use,
                                      Vec<BaseAST*>& asts)
{
  // Get the expression that sets or uses the symexpr.
  CallExpr* call = toCallExpr(se->parentExpr);

  // Create a new temp and load the field value into it.
  VarSymbol* tmp = newTemp(se->var->type);

  // Find the statement containing the symexpr access.
  Expr* stmt = se->getStmtExpr();

  // NOAKES 2014/12/02 This is a brittle way to determine if the
  // SymExpr is in a loop header.  Revisit
  BlockStmt* block = toBlockStmt(stmt);
  BlockStmt* loop  = (block != 0 && block->isLoopStmt()) ? block : 0;

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

  // If the symexpr is set here,
  if (is_def ||
      // Currently buildDefUseSets() does not identify PRIM_SET_MEMBER and
      // PRIM_SVEC_SET_MEMBER as defs.
      (call &&
       (call->isPrimitive(PRIM_SET_MEMBER) ||
        call->isPrimitive(PRIM_SET_SVEC_MEMBER)) &&
       call->get(1) == se))
  {
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


//
// What is oneLocalYS ?
//
// oneLocalYS is set when all PRIM_YIELDs in the iterator
// yield the same Symbol *and* that Symbol is defined locally
// within the iterator.
//
// When oneLocalYS: that Symbol is represented directly by ic.value.
// I.e. the Symbol is mapped directly to valField, in
// local2field, replaceLocalsWithFields, etc.
// ic.value is not updated explicitly upon a yield - because it always
// has the up-to-date value of that Symbol.
//
// When !oneLocalYS: each local Symbol is represented by its own field
// ic.FNN_xxx. Non-local Symbols are not represented in ic's fields.
// Upon each yield, ic.value aka valField is explicitly updated
// with yield's expression.
// E.g. 'yield localvar' is converted to ic.value = ic.FNN_localvar.
// 


// In the body of an iterator function, replace references to local variables
// with references to fields in the iterator class instead.
static void
replaceLocalsWithFields(FnSymbol* fn,           // the iterator function
                        Vec<BaseAST*>& asts,    // the asts in that function, listed postorder.
                        SymbolMap& local2field, // Map: local symbol --> class field
                        Vec<Symbol*>& yldSymSet,// The set of locals that appear in yields.
                        Symbol* valField,       // ic.value field - value being yielded.
                        bool oneLocalYS,        // fn's yields has exactly one yield symbol, and it is local.
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
        if (oneLocalYS) {
          // ySym is already tracked in ic.value. No need to do anything.

          // While here, sanity-check a couple of things.
          INT_ASSERT(ySym->defPoint->parentSymbol == fn);
          INT_ASSERT(yldSymSet.set_in(ySym));
          INT_ASSERT(local2field.get(ySym) == valField);

        } else {
          // Update ic.value.
          SymExpr* upd = new SymExpr(ySym);
          pc->insertBefore(new CallExpr(PRIM_SET_MEMBER, ic, valField, upd));
          if (ySym->defPoint->parentSymbol == fn) {
            // Need to convert 'upd' itself, too.
            Symbol* field = local2field.get(se->var);
            INT_ASSERT(field && field != valField);
            replaceLocalWithFieldTemp(upd, ic, field, false, true, asts);
          }
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

//
// Build zip functions for the single loop iterator optimization
//
// The single iterator loop optimization is applied to loops
// recognized as above in isSingleLoopIterator().  Such loops can be
// generated in a specialized form with simplified control flow.  When
// the optimization does not apply, zip1 and zip3 simply call advance,
// and zip2 and zip4 are empty (see below in lowerIterator()).
//

// Build the zip1 function, copying expressions out of the iterator
// body that occur *before* the start of the loop proper (i.e. before the
// singleLoop construct).
static void
buildZip1(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {
  BlockStmt* zip1body = new BlockStmt();
  Symbol*    ic       = ii->advance->getFormal(1);
  SymbolMap  map;

  // In copied expressions, replace _ic --> zip1->_this .
  map.put(ic, ii->zip1->_this);

  // Copy non-arg def expressions from the original iterator
  // 2015-02-23 hilde: TODO #1:
  // This is sloppy.  We only need local variables that are actually used
  // within the zip1 body.  So we can probably gen the DefExprs we need by
  // scanning the exprs to be copied and just populating the map with the
  // symbols we actually use.  This utility can be factored out of all of the
  // buildZip functions.
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!isArgSymbol(def->sym))
        zip1body->insertAtTail(def->copy(&map));
  }

  // Copy all iterator body expressions before singleLoop that are not
  // declarations
  for_alist(expr, ii->iterator->body->body) {
    // Quit when we reach singleLoop
    if (expr == singleLoop)
      break;

    if (!isDefExpr(expr))
      zip1body->insertAtTail(expr->copy(&map));
  }

  if (WhileStmt* stmt = toWhileStmt(singleLoop)) {
    // By the time we get here, the condExpr has been passed through _cond_test
    // and the result stored in a temp, so condExprForTmpVariableGet just
    // returns the temp result.  So we can simply say:  (.= this more tmp)
    // This simplification depends on the current interpretation of more
    // in a single-loop, single-yield iterator (i.e. 1 = more; 0 = done).
    SymExpr* condExpr = stmt->condExprForTmpVariableGet()->copy(&map);
    Type* moreType = ii->iclass->getField("more")->type;
    VarSymbol* condTemp = newTemp("cond_tmp", moreType);
    zip1body->insertAtTail(new DefExpr(condTemp));
    zip1body->insertAtTail(new CallExpr(PRIM_MOVE, condTemp,
                                        new CallExpr(PRIM_CAST, moreType->symbol, condExpr)));
    zip1body->insertAtTail(new CallExpr(PRIM_SET_MEMBER, ii->zip1->_this,
                                        ii->iclass->getField("more"), condTemp));
  } else if (ForLoop* forLoop = toForLoop(singleLoop)) {
    // 2015-02-23 hilde: TODO: See if we can apply the above simplification
    // here and in buildZip3 as well.
    SymExpr* index = forLoop->indexGet()->copy(&map);

    zip1body->insertAtTail(new CondStmt(index,
                                        new CallExpr(PRIM_SET_MEMBER, ii->zip1->_this, ii->iclass->getField("more"), new_IntSymbol(1)),
                                        new CallExpr(PRIM_SET_MEMBER, ii->zip1->_this, ii->iclass->getField("more"), new_IntSymbol(0))));
  }

  zip1body->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->zip1->body->replace(zip1body);
}


// Build the zip2 function, copying expressions out of the singleLoop
// body that are *before* the yield
static void
buildZip2(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {

  // In copied expressions, replace _ic with zip2->_this .
  SymbolMap map;
  Symbol* ic = ii->advance->getFormal(1);
  map.put(ic, ii->zip2->_this);

  // This block will replace the body of the zip2 function stubbed in
  // during resolution.
  BlockStmt* zip2body = new BlockStmt();

  // Copy non-arg def expressions from the original iterator
  // See TODO #1 above.
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!isArgSymbol(def->sym))
        zip2body->insertAtTail(def->copy(&map));
  }

  // Copy all non-defs in singleLoop before the yield
  for_alist(expr, singleLoop->body) {
    if (CallExpr* call = toCallExpr(expr))
      if (call->isPrimitive(PRIM_YIELD))
        break;
    if (!isDefExpr(expr))
      zip2body->insertAtTail(expr->copy(&map));
  }
  zip2body->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->zip2->body->replace(zip2body);
}


// Build the zip3 function, copying expressions out of the singleLoop
// body that are *after* the yield
static void
buildZip3(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {

  BlockStmt* zip3body = new BlockStmt();
  bool       flag     = true;
  Symbol*    ic       = ii->advance->getFormal(1);
  SymbolMap  map;

  // In copied expressions, replace _ic with zip2->_this .
  map.put(ic, ii->zip3->_this);

  // Copy non-arg def expressions from the original iterator
  // See TODO #1 above.
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!isArgSymbol(def->sym))
        zip3body->insertAtTail(def->copy(&map));
  }

  // Copy all non-defs in singleLoop after the yield
  for_alist(expr, singleLoop->body) {
    // Skip everything before the yield
    if (flag) {
      if (CallExpr* call = toCallExpr(expr))
        if (call->isPrimitive(PRIM_YIELD))
          flag = false;

    } else if (!isDefExpr(expr)) {
      zip3body->insertAtTail(expr->copy(&map));
    }
  }

  // Check for more (only for non c for loops)
  if (WhileStmt* stmt = toWhileStmt(singleLoop)) {
    SymExpr* condExpr = stmt->condExprForTmpVariableGet()->copy(&map);
    Type* moreType = ii->iclass->getField("more")->type;
    VarSymbol* condTemp = newTemp("cond_tmp", moreType);
    zip3body->insertAtTail(new DefExpr(condTemp));
    zip3body->insertAtTail(new CallExpr(PRIM_MOVE, condTemp,
                                        new CallExpr(PRIM_CAST, moreType->symbol, condExpr)));
    zip3body->insertAtTail(new CallExpr(PRIM_SET_MEMBER, ii->zip3->_this,
                                        ii->iclass->getField("more"), condTemp));
  } else if (ForLoop* forLoop = toForLoop(singleLoop)) {
    SymExpr* index = forLoop->indexGet()->copy(&map);

    zip3body->insertAtTail(new CondStmt(index,
                                        new CallExpr(PRIM_SET_MEMBER, ii->zip3->_this, ii->iclass->getField("more"), new_IntSymbol(1)),
                                        new CallExpr(PRIM_SET_MEMBER, ii->zip3->_this, ii->iclass->getField("more"), new_IntSymbol(0))));
  }

  zip3body->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->zip3->body->replace(zip3body);
}


// Build the zip4 function, copy expressions out of the iterator body
// that are *after* the singleLoop
static void
buildZip4(IteratorInfo* ii, Vec<BaseAST*>& asts, BlockStmt* singleLoop) {

  // In copied expressions, replace _ic with zip4->_this .
  Symbol* ic = ii->advance->getFormal(1);
  SymbolMap map;
  map.put(ic, ii->zip4->_this);

  // This block will replace the body of the zip4 function stubbed in
  // during resolution.
  BlockStmt* zip4body = new BlockStmt();

  // Copy non-arg def expressions from the original iterator
  // See TODO #1 above.
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!isArgSymbol(def->sym))
        zip4body->insertAtTail(def->copy(&map));
  }

  // Copy all iterator body expressions after singleLoop that are not
  // declarations
  bool flag = true;

  for_alist(expr, ii->iterator->body->body) {
    // Skip everything before the singleLoop
    if (flag) {
      if (expr == singleLoop)
        flag = false;

    } else if (!isDefExpr(expr)) {
      zip4body->insertAtTail(expr->copy(&map));
    }
  }

  zip4body->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->zip4->body->replace(zip4body);
}


// Builds the standard (non-optimized) iterator body by replacing yields with
// statements that update the "more" field in the iterator class, gotos and
// labels.  Then adds a jump table at the beginning of advance, so execution
// will continue from the next label the next time the iterator is entered.
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
  // This block will replace the stubbed-in body.
  BlockStmt* advanceBody = new BlockStmt();

  for_alist(expr, fn->body->body)
    advanceBody->insertAtTail(expr->remove());

  Symbol* end = new LabelSymbol("_end");
  advanceBody->insertAtTail(new DefExpr(end));

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
    } else if (LoopStmt* loop = toLoopStmt(ast)) {
      loop->orderIndependentSet(false);
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
    advanceBody->insertAtHead(new CondStmt(new SymExpr(tmp), igs));
    advanceBody->insertAtHead(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_EQUAL, more, new_IntSymbol(i++))));
  }
  advanceBody->insertAtHead(new CallExpr(PRIM_MOVE, more, new CallExpr(PRIM_GET_MEMBER_VALUE, ic, ii->iclass->getField("more"))));
  advanceBody->insertAtHead(new DefExpr(tmp));
  advanceBody->insertAtHead(new DefExpr(more));
  advanceBody->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->advance->body->replace(advanceBody);
}


// Build hasMore function.
static void
buildHasMore(IteratorInfo* ii, BlockStmt* singleLoop) {
  BlockStmt* hasMoreBody = new BlockStmt();
  VarSymbol* tmp         = newTemp(ii->hasMore->retType);

  hasMoreBody->insertAtTail(new DefExpr(tmp));

  if (singleLoop != NULL && singleLoop->isCForLoop() == true) {
    CForLoop*  cforLoop  = toCForLoop(singleLoop);
    BlockStmt* testBlock = NULL;

    Symbol*    ic        = ii->advance->getFormal(1);
    SymbolMap  map;

    // In copied expressions, replace _ic with hasMore->_this .
    map.put(ic, ii->hasMore->_this);

    // 2015-02-23 hilde: There was an unnecessary call to copy(&map) here --
    // pointed out by mnoakes -- which I removed.
    testBlock = cforLoop->testBlockGet();

    for_alist(expr, testBlock->body) {
      if (expr != testBlock->body.tail) {
        hasMoreBody->insertAtTail(expr->copy(&map));
      }
    }

    hasMoreBody->insertAtTail(new CallExpr(PRIM_MOVE, tmp, testBlock->body.tail->copy(&map)));

  } else {
    hasMoreBody->insertAtTail(new CallExpr(PRIM_MOVE,
                                           tmp,
                                           new CallExpr(PRIM_GET_MEMBER_VALUE,
                                                        ii->hasMore->getFormal(1),
                                                        ii->iclass->getField("more"))));
  }

  hasMoreBody->insertAtTail(new CallExpr(PRIM_RETURN, tmp));

  ii->hasMore->body->replace(hasMoreBody);
}

// Build getValue function.
static void
buildGetValue(IteratorInfo* ii) {
  BlockStmt* getMoreBody = new BlockStmt();
  VarSymbol* tmp         = newTemp(ii->getValue->retType);

  getMoreBody->insertAtTail(new DefExpr(tmp));
  getMoreBody->insertAtTail(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_GET_MEMBER_VALUE, ii->getValue->getFormal(1), ii->iclass->getField("value"))));
  getMoreBody->insertAtTail(new CallExpr(PRIM_RETURN, tmp));

  ii->getValue->body->replace(getMoreBody);
}

static void
buildInit(IteratorInfo* ii, BlockStmt* singleLoop) {
  BlockStmt* initBody = new BlockStmt();

  if (singleLoop != NULL && singleLoop->isCForLoop() == true) {
    CForLoop*  cforLoop  = toCForLoop(singleLoop);
    BlockStmt* initBlock = NULL;

    Symbol*    ic        = ii->advance->getFormal(1);
    SymbolMap  map;

    // In copied expressions, replace _ic with init->_this .
    map.put(ic, ii->init->_this);

    initBlock = cforLoop->initBlockGet();

    for_alist(expr, initBlock->body) {
      initBody->insertAtTail(expr->copy(&map));
    }
  }

  initBody->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->init->body->replace(initBody);
}

static void
buildIncr(IteratorInfo* ii, BlockStmt* singleLoop) {
  BlockStmt* incrBody = new BlockStmt();

  if (singleLoop != NULL && singleLoop->isCForLoop() == true) {
    CForLoop*  cforLoop  = toCForLoop(singleLoop);
    BlockStmt* incrBlock = NULL;

    Symbol*    ic        = ii->advance->getFormal(1);
    SymbolMap  map;

    // In copied expressions, replace _ic with incr->_this .
    map.put(ic, ii->incr->_this);

    incrBlock = cforLoop->incrBlockGet();

    for_alist(expr, incrBlock->body) {
      incrBody->insertAtTail(expr->copy(&map));
    }
  }

  incrBody->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  ii->incr->body->replace(incrBody);
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
    delete out;

  // C_FOR_LOOP needs to ensure the for-loop init variables are also
  // converted to fields.  The test/incr fields are handled correctly
  // as a result of being inserted in to the body of the loop
  if (singleLoop != NULL && singleLoop->isCForLoop() == true) {
    Vec<SymExpr*> symExprs;
    CForLoop*     cforLoop = toCForLoop(singleLoop);

    collectSymExprs(cforLoop->initBlockGet(), symExprs);

    forv_Vec(SymExpr, se, symExprs) {
      if (useSet.set_in(se)) {
        syms.add_exclusive(se->var);
      }
    }
  }
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
      if (defs == NULL || defs->n != 1) {
        INT_FATAL(sym, "Expected sym to have exactly one definition");
      }

      // Do we need to consider PRIM_ASSIGN as well?
      CallExpr* move = toCallExpr(defs->v[0]->parentExpr);
      INT_ASSERT(move->isPrimitive(PRIM_MOVE));

      if (SymExpr* se = toSymExpr(move->get(2)))
      {
        // The symbol is defined through a bitwise (pointer) copy.
        INT_ASSERT(se->var->type->symbol->hasFlag(FLAG_REF));
        if (se->var->defPoint->parentSymbol == fn) {
          syms.add_exclusive(se->var);
        }
      }
      else if (CallExpr* call = toCallExpr(move->get(2)))
      {
        // The RHS is a function call.
        if (FnSymbol* fn = call->isResolved()) {
          for_actuals(actual, call) {
            SymExpr* se = toSymExpr(actual);
            if (se->var->defPoint->parentSymbol == fn) {
              syms.add_exclusive(se->var);
            }
          }
        }
        else
        {
          // The RHS is not a function call: it must be a primitive instead.

          if (call->isPrimitive(PRIM_ADDR_OF) ||
              call->isPrimitive(PRIM_GET_MEMBER) ||
              // If we are reading a reference out of a field, I'm not sure we
              // capture the right rhs below.  (The actual target of the ref lies
              // outside the struct that contains the ref.)
              call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
            SymExpr* rhs = toSymExpr(call->get(1));
            syms.add_exclusive(rhs->var);
          }
          else
          {
            INT_FATAL(sym, "Unhandled case: Ref returned by a primitive from which we did not expect one.");
          }
        }
      }
      else
      {
        // What else could it be?
        INT_FATAL(move, "RHS of a move is neither a SymExpr nor a CallExpr.");
      }
    }
  }
  freeDefUseMaps(defMap, useMap);
}


static void
addLiveLocalVariables(Vec<Symbol*>& syms, FnSymbol* fn, BlockStmt* singleLoop,
                      Vec<Symbol*>& yldSymSet)
{
  BasicBlock::buildBasicBlocks(fn);

#ifdef DEBUG_LIVE
  printf("Iterator\n");
  list_view(fn);
#endif

#ifdef DEBUG_LIVE
  printf("Basic Blocks\n");
  BasicBlock::printBasicBlocks(fn);
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
      if (fVerify) // this assert is expensive to compute
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
collectYieldSymbols(FnSymbol* fn, Vec<BaseAST*>& asts, Vec<Symbol*>& yldSymSet,
                    bool* oneLocalYSRef)
{
  bool gotNonlocalYS = false;
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* yCall = asYieldExpr(ast)) {
      SymExpr* ySymExpr = toSymExpr(yCall->get(1));
      INT_ASSERT(ySymExpr);
      Symbol* ySym = ySymExpr->var;
      if (ySym->defPoint->parentSymbol == fn) {
        // It is a local symbol, add it.
        yldSymSet.set_add(ySym);
      } else {
        // Non-local symbols are not included in yldSymSet.
        gotNonlocalYS = true;
        // FYI.
        INT_ASSERT(ySym->isConstant() || ySym->isParameter());
      }
    }
  }
  *oneLocalYSRef = !gotNonlocalYS && (yldSymSet.count() == 1);
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
                                    bool isValueField, FnSymbol* fn) {
  // The field name is "value" for the return value of the iterator,
  // or F<int>_<local->name> otherwise.
  const char* fieldName = isValueField
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
                                      Symbol** valFieldRef, bool oneLocalYS,
                                      SymbolMap& local2field, SymbolMap& local2rfield)
{
  IteratorInfo* ii = fn->iteratorInfo;
  Symbol* valField = NULL;

  int i = 0;    // This numbers the fields.
  forv_Vec(Symbol, local, locals) {
    bool isYieldSym = yldSymSet.set_in(local);
    Symbol* field = createICField(i, local, NULL, isYieldSym && oneLocalYS, fn);
    local2field.put(local, field);
    if (isYieldSym) {
      INT_ASSERT(local->type == yieldedType);
      if (oneLocalYS) {
        INT_ASSERT(valField == NULL); // there is exactly 1 yield symbol
        valField = field;
      }
    }

    // Only (live) arguments are added to the record.
    if (isArgSymbol(local)) {
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


// Function resolution just adds a shell for the iterator records and classes,
// and stubs in implementations for the advance, zip1, zip2, zip3, zip4,
// hasMore, getValue, init and incr methods.
// (see protoIteratorClass())
// This function takes a pointer to an iterator and fills in those types.
void lowerIterator(FnSymbol* fn) {

  SET_LINENO(fn);
  Vec<BaseAST*> asts;
  Type* yieldedType = removeRetSymbolAndUses(fn);
  collect_asts_postorder(fn, asts);

  BlockStmt* singleLoop = NULL;
  if (CallExpr* singleLoopYield = isSingleLoopIterator(fn, asts)) {
    // If the iterator contains a single loop statement containing a single
    // yield, singleLoop is that loop statement; otherwise, it is NULL.
    singleLoop = toBlockStmt(singleLoopYield->parentExpr);
  }

  //
  // create fields for all local variables and arguments; however, if
  // live variable analysis is enabled, make fields only for local
  // variables that are live at places where the iterator methods will
  // yield control back to the loop body.
  //
  SymbolMap local2field;  // map from arg/local to class field
  SymbolMap local2rfield; // map from arg to record field
  Vec<Symbol*> locals;
  Vec<Symbol*> yldSymSet; // local symbols appearing in yield(s)
  Symbol* valField;
  bool oneLocalYS;  // see comment earlier

  // Add all formals to the set of local symbols.
  for_formals(formal, fn)
    locals.add(formal);

  collectYieldSymbols(fn, asts, yldSymSet, &oneLocalYS);

  if (fNoLiveAnalysis)
    addAllLocalVariables(locals, asts);
  else
    addLiveLocalVariables(locals, fn, singleLoop, yldSymSet);

  addYieldSymbols(locals, yldSymSet);

  addLocalsToClassAndRecord(locals, fn, yldSymSet, yieldedType, &valField,
                            oneLocalYS, local2field, local2rfield);

  replaceLocalsWithFields(fn, asts, local2field, yldSymSet, valField,
                          oneLocalYS, locals);

  removeLocals(locals, asts, yldSymSet, fn);

  //------------------------------------------------------------------------
  // The above should probably be factored out.
  //------------------------------------------------------------------------

  IteratorInfo* ii = fn->iteratorInfo;
  if (!fn->hasFlag(FLAG_INLINE_ITERATOR)) {
    // Does this force iterators marked as inline to be inlined
    // even if --no-inline-iterators is specified?
    // Isn't that bad?
    if (singleLoop) {
      if (fReportOptimizedLoopIterators) {
        ModuleSymbol *mod = toModuleSymbol(fn->defPoint->parentSymbol);
        INT_ASSERT(mod);

        if (developer || mod->modTag == MOD_USER) {
          printf("Optimized single yield/loop iterator (%s) in module %s (%s:%d)\n",
                 fn->cname, mod->name, fn->fname(), fn->linenum());
        }
      }

      buildZip1(ii, asts, singleLoop);
      buildZip2(ii, asts, singleLoop);
      buildZip3(ii, asts, singleLoop);
      buildZip4(ii, asts, singleLoop);
    } else {
      // For iterators that are not single-loop/single-yield, zip2 and zip4 are
      // no-ops and zip1 and zip3 simply call advance().
      ii->zip1->insertAtHead(new CallExpr(ii->advance, ii->zip1->_this));
      ii->zip3->insertAtHead(new CallExpr(ii->advance, ii->zip3->_this));
    }
    buildAdvance(fn, asts, local2field, locals);
    // Note that buildInit and buildIncr are essentially no-ops for non c for
    // loops and that the behavior of buildHasMore changes for C for loops.
    // For c for loops these basically just takes the init, test, and incr
    // expressions from the c for loop primitives and put them in the iterator
    // functions.
    buildHasMore(ii, singleLoop);
    buildGetValue(ii);
    buildInit(ii, singleLoop);
    buildIncr(ii, singleLoop);
  }
  rebuildIterator(ii, local2rfield, locals);
  rebuildGetIterator(ii);
}
