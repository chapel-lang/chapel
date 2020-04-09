/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "bb.h"

#include "astutil.h"
#include "bitVec.h"
#include "CForLoop.h"
#include "DoWhileStmt.h"
#include "driver.h"
#include "ForLoop.h"
#include "stlUtil.h"
#include "stmt.h"
#include "view.h"
#include "WhileDoStmt.h"

#include <queue>


int                                          BasicBlock::nextID     = 0;
BasicBlock*                                  BasicBlock::basicBlock = NULL;
Map<LabelSymbol*, std::vector<BasicBlock*>*> BasicBlock::gotoMaps;
Map<LabelSymbol*, BasicBlock*>               BasicBlock::labelMaps;

BasicBlock::BasicBlock() {
  id = nextID++;
}

// Reset the shared statics.
void BasicBlock::reset(FnSymbol* fn) {
  clear(fn);

  gotoMaps.clear();
  labelMaps.clear();

  fn->basicBlocks = new std::vector<BasicBlock*>();

  nextID = 0;
}

void BasicBlock::clear(FnSymbol* fn) {
  if (fn->basicBlocks != NULL) {
    for_vector(BasicBlock, bb, *fn->basicBlocks)
      delete bb;

    delete fn->basicBlocks;

    fn->basicBlocks = 0;
  }
}

// This is the top-level (public) builder function.
void BasicBlock::buildBasicBlocks(FnSymbol* fn) {
  reset(fn);

  basicBlock = new BasicBlock();

  buildBasicBlocks(fn, fn->body, false);

  fn->basicBlocks->push_back(BasicBlock::steal());

  removeEmptyBlocks(fn);

  if (fVerify)
    INT_ASSERT(verifyBasicBlocks(fn));
}

BasicBlock* BasicBlock::steal() {
  BasicBlock* temp = basicBlock;

  basicBlock = 0;

  return temp;
}

void BasicBlock::buildBasicBlocks(FnSymbol* fn, Expr* stmt, bool mark) {
  if (stmt == 0) {

  } else if (BlockStmt* s = toBlockStmt(stmt)) {
    if (s->isLoopStmt() == true) {

      // for c for loops, add the init expr before the loop body
      if (CForLoop* cforLoop = toCForLoop(s)) {
        for_alist(stmt, cforLoop->initBlockGet()->body) {
          buildBasicBlocks(fn, stmt, mark);
        }
      }

      // mark the top of the loop
      BasicBlock* top = basicBlock;

      restart(fn);

      // Mark and add the test expr at the loop top
      if (CForLoop* cforLoop = toCForLoop(s)) {
        for_alist(stmt, cforLoop->testBlockGet()->body) {
          buildBasicBlocks(fn, stmt, true);
        }

      // add the condition expr at the loop top
      } else if (WhileDoStmt* whileDoStmt = toWhileDoStmt(stmt)) {
        SymExpr* condExpr = whileDoStmt->condExprForTmpVariableGet();

        append(condExpr, true);

      // wait to add the conditionExpr at the end of the block
      } else if (isDoWhileStmt(stmt) == true) {

      } else if (ForLoop* forLoop = toForLoop(stmt)) {
        append(forLoop->indexGet(),    true);
        append(forLoop->iteratorGet(), true);

      // PARAM_LOOP
      } else {
        CallExpr* info = s->blockInfoGet();

        append(info, true);
      }

      BasicBlock* loopTop = basicBlock;

      for_alist(bodyStmt, s->body) {
        buildBasicBlocks(fn, bodyStmt, mark);
      }

      // for c for loops, add the incr expr after the loop body
      if (CForLoop* cforLoop = toCForLoop(s)) {
        for_alist(stmt, cforLoop->incrBlockGet()->body) {
          buildBasicBlocks(fn, stmt, mark);
        }

      } else if (DoWhileStmt* doWhileStmt = toDoWhileStmt(stmt)) {
        SymExpr* condExpr = doWhileStmt->condExprForTmpVariableGet();

        append(condExpr, true);
      }

      BasicBlock* loopBottom = basicBlock;

      restart(fn);

      BasicBlock* bottom = basicBlock;

      // thread the basic blocks of the pre-loop, loop, and post-loop together
      thread(top,        loopTop);
      thread(loopBottom, bottom);
      thread(loopBottom, loopTop);
      thread(top,        bottom);

    } else {
      for_alist(stmt, s->body)
        buildBasicBlocks(fn, stmt, mark);
    }

  } else if (CondStmt* s = toCondStmt(stmt)) {
    INT_ASSERT(s->condExpr);

    // Mark the conditional expression
    append(s->condExpr, true);

    BasicBlock* top = basicBlock;

    restart(fn);
    thread(top, basicBlock);
    buildBasicBlocks(fn, s->thenStmt, mark);

    BasicBlock* thenBottom = basicBlock;

    restart(fn);

    if (s->elseStmt) {
      thread(top, basicBlock);

      buildBasicBlocks(fn, s->elseStmt, mark);

      BasicBlock* elseBottom = basicBlock;

      restart(fn);

      thread(elseBottom, basicBlock);

    } else {
      thread(top, basicBlock);
    }

    thread(thenBottom, basicBlock);

  } else if (GotoStmt* s = toGotoStmt(stmt)) {
    LabelSymbol* label = toLabelSymbol(toSymExpr(s->label)->symbol());

    if (BasicBlock* bb = labelMaps.get(label)) {
      // Thread this block to its destination label.
      thread(basicBlock, bb);

    } else {
      // Set up goto map, so this block's successor can be back-patched later.
      std::vector<BasicBlock*>* vbb = gotoMaps.get(label);

      if (vbb == NULL) {
        vbb = new std::vector<BasicBlock*>();
      }

      vbb->push_back(basicBlock);

      gotoMaps.put(label, vbb);
    }

    append(s, mark); // Put the goto at the end of its block.

    // We need a new block, so we can not thread the one containing the goto to
    // this new one.  There is a break in the flow.  If the new block does not
    // begin with a label, it is unreachable and can be removed.
    restart(fn);

  } else {
    DefExpr*              def = toDefExpr(stmt);
    std::vector<BaseAST*> asts;

    collect_asts(stmt, asts);

    for_vector(BaseAST, ast, asts) {
      if (CallExpr* call = toCallExpr(ast)) {
        // mark function calls as essential
        if (call->resolvedFunction() != NULL) {
          mark = true;

        // mark essential primitives as essential
        } else if (call->primitive && call->primitive->isEssential) {
          mark = true;

        // mark assignments to global variables as essential
        } else if (call->isPrimitive(PRIM_MOVE) ||
                 call->isPrimitive(PRIM_ASSIGN)) {
          if (SymExpr* se = toSymExpr(call->get(1))) {
            if (se->symbol()->type->refType == NULL) {
              mark = true;
            }
          }
        }
      }
    }

    if (def && toLabelSymbol(def->sym)) {
      // If a label appears in the middle of a block,
      // we start a new block.
      if (basicBlock->exprs.size() > 0) {
        BasicBlock* top = basicBlock;

        restart(fn);

        thread(top, basicBlock);
      }

      append(def, mark); // Put the label def at the start of its block.

      // OK, this statement is a label def, so get the label.
      LabelSymbol* label = toLabelSymbol(def->sym);

      // See if we have any unresolved references to this label,
      // and resolve them.
      if (std::vector<BasicBlock*>* vbb = gotoMaps.get(label)) {
        for_vector(BasicBlock, bb, *vbb) {
          thread(bb, basicBlock);
        }
      }

      labelMaps.put(label, basicBlock);
    } else {
      append(stmt, mark);

      // For the sake of live variable analysis, a yield ends one block and
      // begins another.  For now, we just thread one block into the next.
      // We could get fancier and thread the block containing the yield to the
      // end of the function and the start of the function to the block
      // following the yield, but just putting in a block break is good enough
      // for now.
      if (CallExpr* call = toCallExpr(stmt)) {
        if (call->isPrimitive(PRIM_YIELD)) {
          BasicBlock* curr = basicBlock;

          restart(fn);

          thread(curr, basicBlock);
        }
      }
    }
  }
}

void BasicBlock::restart(FnSymbol* fn) {
  fn->basicBlocks->push_back(steal());
  basicBlock = new BasicBlock();
}

void BasicBlock::append(Expr* expr, bool mark) {
  INT_ASSERT(expr);

  basicBlock->exprs.push_back(expr);
  basicBlock->marks.push_back(mark);
}

void BasicBlock::thread(BasicBlock* src, BasicBlock* dst) {
  dst->ins.push_back(src);
  src->outs.push_back(dst);
}

// Removes a block from the basic block structure by traversing its lists of
// predecessors and successors and removing any back-links.
// The caller must then remove this block from any container and free it.
void BasicBlock::remove() {
  for_vector(BasicBlock, pred, this->ins) {
    BasicBlockVector& pred_outs = pred->outs;

    // Look for this block in the list of successors of this predecessor.
    BasicBlockVector::iterator i;

    for (i = pred_outs.begin(); i != pred_outs.end(); ++i) {
      if (*i == this)
        break;
    }

    // This block is in the list, right?
    INT_ASSERT(i != pred_outs.end());

    pred_outs.erase(i);
  }

  for_vector(BasicBlock, succ, this->outs) {
    BasicBlockVector&          succ_ins = succ->ins;
    BasicBlockVector::iterator i;

    // Look for this block in the list of predecessors of this successor.
    for (i = succ_ins.begin(); i != succ_ins.end(); ++i)
      if (*i == this)
        break;

    INT_ASSERT(i != succ_ins.end());

    succ_ins.erase(i);
  }
}

// Look for and remove empty blocks with no predecessor and whose successor is
// the next block in sequence.  These blocks get created when a block ends in a
// goto statement and the enclosing construct calls restart immediately.
// We have to wait until basic block analysis is done, because we don't know if
// a block has predecessors until after threading is performed, and this is
// sometimes delayed.
void BasicBlock::removeEmptyBlocks(FnSymbol* fn) {
  // Create a new vector that contains just the items we want to preserve.
  int               newId     = 0;
  BasicBlockVector* newBlocks = new BasicBlockVector();

  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    // Look for empty blocks with no predecessors.
    if (bb->ins.size() == 0 && bb->exprs.size() == 0) {
      // This block will be removed.  It is no longer a predecessor of anyone,
      // so we must update the back links.
      bb->remove();

      delete bb;
    } else {
      bb->id = newId++;

      newBlocks->push_back(bb);
    }
  }

  delete fn->basicBlocks;

  fn->basicBlocks = newBlocks;
}

// Returns true if the basic block structure is OK, false otherwise.
bool BasicBlock::verifyBasicBlocks(FnSymbol* fn) {
  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    if (bb->isOK() == false)
      return false;
  }

  return true;
}

// Returns true if the class invariants have been preserved.
bool BasicBlock::isOK() {
  // Ensure exprs[] and marks[] are same length
  if (exprs.size() != marks.size())
    return false;

  // Every empty interior block must have a predecessor.
  // Non-empty blocks with no predecessors are dead code, which may be
  // removed by a client of BB analysis.  These dead blocks cannot be
  // identified without BB analysis, so non-empty blocks with no
  // predecessors are valid.
  if (ins.size() == 0 &&
      exprs.size() == 0)
    return false;

  // Expressions must be live (non-NULL);
  for_vector(Expr, expr, exprs)
    if (expr == 0)
      return false;

  // Every in edge must have a corresponding out edge in the source block.
  for_vector(BasicBlock, source, ins) {
    bool found = false;

    for_vector(BasicBlock, bb, source->outs) {
      if (bb == this) {
        found = true;
        break;
      }
    }

    if (found == false)
      return false;
  }

  // Every out edge must have a corresponding in edge in the target block.
  for_vector(BasicBlock, target, outs) {
    bool found = false;

    for_vector(BasicBlock, bb, target->ins) {
      if (bb == this) {
        found = true;
        break;
      }
    }

    if (found == false)
      return false;
  }

  return true;
}


// This routine removes unreachable (interior) blocks from the flow graph
// without modifying the underlying AST.  It is a workaround for the fact that
// dead block removal does not succeed in removing all unreachable blocks from
// the tree.
void BasicBlock::ignoreUnreachableBlocks(FnSymbol* fn) {
  BasicBlockSet     reachable;
  int               newId     = 0;
  BasicBlockVector* newBlocks = new BasicBlockVector();

  BasicBlock::getReachableBlocks(fn, reachable);

  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    if (reachable.count(bb)) {
      // Add reachable blocks to the new BB vector.
      bb->id = newId++;

      newBlocks->push_back(bb);

    } else {
      // Remove unreachable blocks from the flow graph.
      bb->remove();
    }
  }

  delete fn->basicBlocks;

  fn->basicBlocks = newBlocks;
}


// Populates the passed-in basic block set with blocks that are reachable from
// the root (block 0).  The blocks which are not in this set are unreachable
// and may be removed.
void BasicBlock::getReachableBlocks(FnSymbol* fn, BasicBlockSet& reachable) {
  // We set up a work queue to perform a BFS on reachable blocks, and seed it
  // with the first block in the function.
  std::queue<BasicBlock*> workQueue;

  workQueue.push((*fn->basicBlocks)[0]);

  // Then we iterate until there are no more blocks to visit.
  while (!workQueue.empty()) {
    // Fetch and remove the next block.
    BasicBlock* bb = workQueue.front();

    workQueue.pop();

    // Ignore it if we've already seen it.
    if (reachable.count(bb) == 0) {
      // Otherwise, mark it as reachable,
      // and append all of its successors to the work queue.
      reachable.insert(bb);

      for_vector(BasicBlock, out, bb->outs)
        workQueue.push(out);
    }
  }
}

void BasicBlock::buildLocalsVectorMap(FnSymbol*          fn,
                                      Vec<Symbol*>&      locals,
                                      Map<Symbol*, int>& localMap) {
  int i = 0;

  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    for_vector(Expr, expr, bb->exprs) {
      if (DefExpr* def = toDefExpr(expr)) {
        if (toVarSymbol(def->sym)) {
          locals.add(def->sym);
          localMap.put(def->sym, i++);
        }
      }
    }
  }
}

static
void computeOrders(FnSymbol* fn,
                   std::set<int>& visited,
                   BasicBlock* bb,
                   // mapping bb->id -> order in pre-order
                   std::vector<int>& Pre,
                   // mapping bb->id -> order in post-order
                   std::vector<int>& Post,
                   int & preJ,
                   int & postI) {

  // Following Muchnick p 180 Depth_First_search_PP

  int x = bb->id;
  visited.insert(x);
  Pre[x] = preJ;
  preJ += 1;

  for_vector(BasicBlock, bbout, bb->outs) {
    int y = bbout->id;
    if (visited.count(y) == 0) {
      computeOrders(fn, visited, bbout, Pre, Post, preJ, postI);
    }
  }

  Post[x] = postI;
  postI += 1;
}

void BasicBlock::computeForwardOrder(FnSymbol* fn,
                                     std::vector<int> & order) {

  size_t nbbs = fn->basicBlocks->size();
  std::set<int> visited;
  std::vector<int> Pre(nbbs);
  std::vector<int> Post(nbbs);

  int preJ = 0;
  int postI = 0;

  BasicBlock* root = (*fn->basicBlocks)[0];
  computeOrders(fn, visited, root, Pre, Post, preJ, postI);

  // This will fail for unreachable blocks
  INT_ASSERT(Pre.size() == nbbs && Post.size() == nbbs);

  order.resize(nbbs);

  // Pre and Post map bb ids -> order, but the order to return
  // is the inverse of that permutation. Plus we want reverse
  // postorder...
  for (size_t ii = 0; ii < nbbs; ii++) {
    int index = Post[ii];
    int reverseIndex = nbbs-index-1;
    order[reverseIndex] = ii;
  }
}

void BasicBlock::computeBackwardOrder(FnSymbol* fn,
                                      std::vector<int> & order) {

  size_t nbbs = fn->basicBlocks->size();
  std::set<int> visited;
  std::vector<int> Pre(nbbs);
  std::vector<int> Post(nbbs);

  int preJ = 0;
  int postI = 0;

  BasicBlock* root = (*fn->basicBlocks)[0];
  computeOrders(fn, visited, root, Pre, Post, preJ, postI);

  // This will fail for unreachable blocks
  INT_ASSERT(Pre.size() == nbbs && Post.size() == nbbs);

  order.resize(nbbs);

  // Pre and Post map bb ids -> order, but the order to return
  // is the inverse of that permutation. Plus we want reverse
  // preorder...
  for (size_t ii = 0; ii < nbbs; ii++) {
    int index = Pre[ii];
    int reverseIndex = nbbs-index-1;
    order[reverseIndex] = ii;
  }
}

//#define DEBUG_FLOW
void BasicBlock::backwardFlowAnalysis(FnSymbol*             fn,
                                      std::vector<BitVec*>& GEN,
                                      std::vector<BitVec*>& KILL,
                                      std::vector<BitVec*>& IN,
                                      std::vector<BitVec*>& OUT) {
  bool iterate = true;

  while (iterate) {
    int i = 0;

    iterate = false;

    for_vector(BasicBlock, bb, *fn->basicBlocks) {
      for (size_t j = 0; j < IN[i]->ndata; j++) {
        unsigned int new_in  = (OUT[i]->data[j] & ~KILL[i]->data[j]) | GEN[i]->data[j];
        unsigned int new_out = 0;

        if (new_in != IN[i]->data[j]) {
          IN[i]->data[j] = new_in;
          iterate = true;
        }

        for_vector(BasicBlock, bbout, bb->outs) {
          new_out = new_out | IN[bbout->id]->data[j];
        }

        if (new_out != OUT[i]->data[j]) {
          OUT[i]->data[j] = new_out;
          iterate = true;
        }
      }

      i++;
    }
#ifdef DEBUG_FLOW
    printf("IN\n");  printBitVectorSets(IN);
    printf("OUT\n"); printBitVectorSets(OUT);
#endif
  }
}


void BasicBlock::forwardFlowAnalysis(FnSymbol*             fn,
                                     std::vector<BitVec*>& GEN,
                                     std::vector<BitVec*>& KILL,
                                     std::vector<BitVec*>& IN,
                                     std::vector<BitVec*>& OUT,
                                     bool                  intersect) {
  size_t           nbbq = fn->basicBlocks->size(); // size of bb queue
  std::vector<int> bbq;
  BitVec           bbs(nbbq);
  int              iq = -1;
  int              nq = nbbq - 1;

  for (size_t i = 0; i < nbbq; i++) {
    bbq.push_back(i);
    bbs.set(i);
  }

  while (iq != nq) {
    iq = (iq + 1) % nbbq;

    int i = bbq[iq];

    bbs.unset(i);

#ifdef DEBUG_FLOW
    if (iq == 0) {
      printf("IN\n");  printBitVectorSets(IN);
      printf("OUT\n"); printBitVectorSets(OUT);
    }
#endif

    BasicBlock* bb     = (*fn->basicBlocks)[i];
    bool        change = false;

    for (size_t j = 0; j < IN[i]->ndata; j++) {
      if (bb->ins.size() > 0) {
        unsigned int new_in = (intersect) ? (unsigned int) (-1) : 0;

        for_vector(BasicBlock, bbin, bb->ins) {
          if (intersect)
            new_in &= OUT[bbin->id]->data[j];
          else
            new_in |= OUT[bbin->id]->data[j];
        }

        if (new_in != IN[i]->data[j]) {
          IN[i]->data[j] = new_in;
          change         = true;
        }
      }

      unsigned int new_out = (IN[i]->data[j] & ~KILL[i]->data[j]) | GEN[i]->data[j];

      if (new_out != OUT[i]->data[j]) {
        OUT[i]->data[j] = new_out;
        change          = true;
      }
    }

    if (change) {
      for_vector(BasicBlock, bbout, bb->outs) {
        if (!bbs.get(bbout->id)) {
          nq      = (nq + 1) % nbbq;

          bbs.set(bbout->id);

          bbq[nq] = bbout->id;
        }
      }
    }
  }
}

void BasicBlock::printBasicBlocks(FnSymbol* fn) {
  for_vector(BasicBlock, b, *fn->basicBlocks) {

    printf("%2d:  ", b->id);

    for_vector(BasicBlock, bb, b->ins) {
      printf("%d ", bb->id);
    }

    printf(" >  ");

    for_vector(BasicBlock, bc, b->outs) {
      printf("%d ", bc->id);
    }

    printf("\n");

    for_vector(Expr, expr, b->exprs) {
      if (expr)
        list_view_noline(expr);
    }

    printf("\n");
  }
}

void BasicBlock::printLocalsVector(Vec<Symbol*> locals, Map<Symbol*,int>& localMap) {
  printf("Local Variables\n");

  forv_Vec(Symbol, local, locals) {
    printf("%2d: %s[%d]\n", localMap.get(local), local->name, local->id);
  }

  printf("\n");
}

void BasicBlock::printDefsVector(std::vector<SymExpr*> defs, Map<SymExpr*,int>& defMap) {
  printf("Variable Definitions\n");

  for_vector(SymExpr, def, defs) {
    printf("%2d: %s[%d] in %d\n",
           defMap.get(def),
           def->symbol()->name,
           def->symbol()->id,
           def->getStmtExpr()->id);
  }

  printf("\n");
}

void BasicBlock::printLocalsVectorSets(std::vector<BitVec*>& sets, Vec<Symbol*> locals) {
  int i = 0;

  for_vector(BitVec, set, sets) {
    printf("%2d: ", i);

    for (size_t j = 0; j < set->size(); j++) {
      if (set->get(j))
        printf("%s[%d] ", locals.v[j]->name, locals.v[j]->id);
    }

    printf("\n");

    i++;
  }

  printf("\n");
}

void BasicBlock::printBitVectorSets(std::vector<BitVec*>& sets) {
  int i = 0;

  for_vector(BitVec, set, sets) {
    printf("%2d: ", i);

    for (size_t j = 0; j < set->size(); j++) {
      printf("%d", (set->get(j)) ? 1 : 0);
      if ((j+1) % 10 == 0) printf(" ");
    }

    printf("\n");

    i++;
  }

  printf("\n");
}
