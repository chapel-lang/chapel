/*
 * Copyright 2004-2014 Cray Inc.
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

#include <cstdlib>
#include "astutil.h"
#include "bb.h"
#include "bitVec.h"
#include "expr.h"
#include "stmt.h"
#include "view.h"

BasicBlock::BasicBlock()
  : id(nextid++) {}

#define BB_START()                              \
  basicBlock = new BasicBlock()

// The assert tests that the expression we are adding to this basic block
// has not already been deleted.
#define BB_ADD(expr)                            \
  do {                                          \
    INT_ASSERT(expr);                           \
    basicBlock->exprs.push_back(expr);          \
  } while (0)

#define BB_STOP()                               \
  fn->basicBlocks->push_back(Steal())

#define BB_RESTART()                            \
  do {                                          \
    BB_STOP();                                  \
    BB_START();                                 \
  } while (0)

#define BBB(stmt)                               \
    buildBasicBlocks(fn, stmt)

#define BB_THREAD(src, dst)                     \
  do {                                          \
    (dst)->ins.push_back(src);                  \
    (src)->outs.push_back(dst);                 \
  } while (0)


//# Statics
BasicBlock* BasicBlock::basicBlock;
Map<LabelSymbol*,std::vector<BasicBlock*>*> BasicBlock::gotoMaps;
Map<LabelSymbol*,BasicBlock*> BasicBlock::labelMaps;
int BasicBlock::nextid;


// Returns true if the class invariants have been preserved.
bool BasicBlock::isOK()
{
  // Expressions must be live (non-NULL);
  for_vector(Expr, expr, exprs)
    if (expr == 0) return false;

  // Every in edge must have a corresponding out edge in the source block.
  for_vector(BasicBlock, source, ins) {
    bool found = false;
    for_vector(BasicBlock, bb, source->outs)
      if (bb == this) { found = true; break; }
    if (!found) return false;
  }

  // Every out edge must have a corresponding in edge in the target block.
  for_vector(BasicBlock, target, outs) {
    bool found = false;
    for_vector(BasicBlock, bb, target->ins)
      if (bb == this) { found = true; break; }
    if (!found) return false;
  }

  return true;
}

void BasicBlock::clear(FnSymbol* fn)
{
  if (!fn->basicBlocks)
    return;

  for_vector(BasicBlock, bb, *fn->basicBlocks)
    delete bb, bb = 0;
  delete fn->basicBlocks; fn->basicBlocks = 0;
}

// Reset the shared statics.
void BasicBlock::reset(FnSymbol* fn)
{
  clear(fn);
  gotoMaps.clear();
  labelMaps.clear();
  fn->basicBlocks = new std::vector<BasicBlock*>();
  nextid = 0;
}

BasicBlock* BasicBlock::Steal()
{
  BasicBlock* temp = basicBlock;
  basicBlock = 0;
  return temp;
}

// This is the top-level (public) builder function.
void buildBasicBlocks(FnSymbol* fn)
{
  BasicBlock::reset(fn);
  BasicBlock::basicBlock = new BasicBlock();    // BB_START();
  BasicBlock::buildBasicBlocks(fn, fn->body);   // BBB(fn->body);
  fn->basicBlocks->push_back(BasicBlock::Steal());    // BB_STOP();

  INT_ASSERT(verifyBasicBlocks(fn));
}

void BasicBlock::buildBasicBlocks(FnSymbol* fn, Expr* stmt)
{
  if (!stmt) return;

  if (BlockStmt* s = toBlockStmt(stmt))
  {
    CallExpr* loop = toCallExpr(s->blockInfo);
    if (loop)
    {
      bool cForLoop = loop->isPrimitive(PRIM_BLOCK_C_FOR_LOOP);
      // for c for loops, add the init expr before the loop body
      if (cForLoop) {
        for_alist(stmt, toBlockStmt(loop->get(1))->body) { BBB(stmt); }
      }

      // mark the top of the loop
      BasicBlock* top = basicBlock;
      BB_RESTART();

      // for c for loops, add the test expr at the loop top
      if (cForLoop) {
        for_alist(stmt, toBlockStmt(loop->get(2))->body) { BBB(stmt); }
      }

      // add the CallExpr that is the loop itself and then add the loops body
      BB_ADD(loop);
      BasicBlock* loopTop = basicBlock;
      for_alist(stmt, s->body) {
        BBB(stmt);
      }

      // for c for loops, add the incr expr after the loop body
      if (cForLoop) {
        for_alist(stmt, toBlockStmt(loop->get(3))->body) { BBB(stmt); }
      }

      BasicBlock* loopBottom = basicBlock;
      BB_RESTART();
      BasicBlock* bottom = basicBlock;

      // thread the basic blocks of the pre-loop, loop, and post-loop together
      BB_THREAD(top, loopTop);
      BB_THREAD(loopBottom, bottom);
      BB_THREAD(loopBottom, loopTop);
      BB_THREAD(top, bottom);
    }
    else
    {
      for_alist(stmt, s->body)
        BBB(stmt);
    }
  }
  else if (CondStmt* s = toCondStmt(stmt))
  {
    INT_ASSERT(s->condExpr);
    BB_ADD(s->condExpr);
    BasicBlock* top = basicBlock;
    BB_RESTART();
    BB_THREAD(top, basicBlock);
    BBB(s->thenStmt);
    BasicBlock* thenBottom = basicBlock;
    BB_RESTART();
    if (s->elseStmt)
    {
      BB_THREAD(top, basicBlock);
      BBB(s->elseStmt);
      BasicBlock* elseBottom = basicBlock;
      BB_RESTART();
      BB_THREAD(elseBottom, basicBlock);
    }
    else
    {
      BB_THREAD(top, basicBlock);
    }
    BB_THREAD(thenBottom, basicBlock);
  } else if (GotoStmt* s = toGotoStmt(stmt)) {
    LabelSymbol* label = toLabelSymbol(toSymExpr(s->label)->var);
    if (BasicBlock* bb = labelMaps.get(label)) {
      BB_THREAD(basicBlock, bb);
    } else {
      std::vector<BasicBlock*>* vbb = gotoMaps.get(label);
      if (!vbb)
        vbb = new std::vector<BasicBlock*>();
      vbb->push_back(basicBlock);
      gotoMaps.put(label, vbb);
    }
    BB_ADD(s); // Put the goto at the end of its block.
    BB_RESTART();
  } else {
    DefExpr* def = toDefExpr(stmt);
    if (def && toLabelSymbol(def->sym)) {
      // If a label appears in the middle of a block,
      // we start a new block.
      if (basicBlock->exprs.size() > 0)
      {
        BasicBlock* top = basicBlock;
        BB_RESTART();
        BB_THREAD(top, basicBlock);
      }
      BB_ADD(def); // Put the label def at the start of its block.

      // OK, this statement is a label def, so get the label.
      LabelSymbol* label = toLabelSymbol(def->sym);

      // See if we have any unresolved references to this label,
      // and resolve them.
      if (std::vector<BasicBlock*>* vbb = gotoMaps.get(label)) {
        for_vector(BasicBlock, bb, *vbb) {
          BB_THREAD(bb, basicBlock);
        }
      }
      labelMaps.put(label, basicBlock);
    } else {
      BB_ADD(stmt);
    }
  }
}

// Returns true if the basic block structure is OK, false otherwise.
bool verifyBasicBlocks(FnSymbol* fn)
{
  for_vector(BasicBlock, bb, *fn->basicBlocks)
  {
    if (! bb->isOK())
      return false;
  }
  return true;
}

void buildLocalsVectorMap(FnSymbol* fn,
                          Vec<Symbol*>& locals,
                          Map<Symbol*,int>& localMap) {
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


//#define DEBUG_FLOW
void backwardFlowAnalysis(FnSymbol* fn,
                          std::vector<BitVec*>& GEN,
                          std::vector<BitVec*>& KILL,
                          std::vector<BitVec*>& IN,
                          std::vector<BitVec*>& OUT) {
  bool iterate = true;
  while (iterate) {
    iterate = false;
    int i = 0;
    for_vector(BasicBlock, bb, *fn->basicBlocks) {
      for (int j = 0; j < IN[i]->ndata; j++) {
        unsigned new_in = (OUT[i]->data[j] & ~KILL[i]->data[j]) | GEN[i]->data[j];
        if (new_in != IN[i]->data[j]) {
          IN[i]->data[j] = new_in;
          iterate = true;
        }
        unsigned new_out = 0;
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
    printf("IN\n"); printBitVectorSets(IN);
    printf("OUT\n"); printBitVectorSets(OUT);
#endif
  }
}


void forwardFlowAnalysis(FnSymbol* fn,
                         std::vector<BitVec*>& GEN,
                         std::vector<BitVec*>& KILL,
                         std::vector<BitVec*>& IN,
                         std::vector<BitVec*>& OUT,
                         bool intersect) {
  size_t nbbq = fn->basicBlocks->size(); // size of bb queue
  std::vector<int> bbq;
  BitVec bbs(nbbq);
  int iq = -1, nq = nbbq-1;      // index to first and last bb in bbq
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
      printf("IN\n"); printBitVectorSets(IN);
      printf("OUT\n"); printBitVectorSets(OUT);
    }
#endif
    BasicBlock* bb = (*fn->basicBlocks)[i];
    bool change = false;
    for (int j = 0; j < IN[i]->ndata; j++) {
      if (bb->ins.size() > 0) {
        unsigned new_in = (intersect) ? (unsigned)(-1) : 0;
        for_vector(BasicBlock, bbin, bb->ins) {
          if (intersect)
            new_in &= OUT[bbin->id]->data[j];
          else
            new_in |= OUT[bbin->id]->data[j];
        }
        if (new_in != IN[i]->data[j]) {
          IN[i]->data[j] = new_in;
          change = true;
        }
      }
      unsigned new_out = (IN[i]->data[j] & ~KILL[i]->data[j]) | GEN[i]->data[j];
      if (new_out != OUT[i]->data[j]) {
        OUT[i]->data[j] = new_out;
        change = true;
      }
    }
    if (change) {
      for_vector(BasicBlock, bbout, bb->outs) {
        if (!bbs.get(bbout->id)) {
          nq = (nq + 1) % nbbq;
          bbs.set(bbout->id);
          bbq[nq] = bbout->id;
        }
      }
    }
  }
}


void printBasicBlocks(FnSymbol* fn) {
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
      else
        printf("0 (null)\n");
    }
    printf("\n");
  }
}

void printLocalsVector(Vec<Symbol*> locals, Map<Symbol*,int>& localMap) {
  printf("Local Variables\n");
  forv_Vec(Symbol, local, locals) {
    printf("%2d: %s[%d]\n", localMap.get(local), local->name, local->id);
  }
  printf("\n");
}

void printDefsVector(std::vector<SymExpr*> defs, Map<SymExpr*,int>& defMap) {
  printf("Variable Definitions\n");
  for_vector(SymExpr, def, defs) {
    printf("%2d: %s[%d] in %d\n", defMap.get(def), def->var->name,
           def->var->id, def->getStmtExpr()->id);
  }
  printf("\n");
}

void printLocalsVectorSets(std::vector<BitVec*>& sets, Vec<Symbol*> locals) {
  int i = 0;
  for_vector(BitVec, set, sets) {
    printf("%2d: ", i);
    for (int j = 0; j < set->size(); j++) {
      if (set->get(j))
        printf("%s[%d] ", locals.v[j]->name, locals.v[j]->id);
    }
    printf("\n");
    i++;
  }
  printf("\n");
}

void printBitVectorSets(std::vector<BitVec*>& sets) {
  int i = 0;
  for_vector(BitVec, set, sets) {
    printf("%2d: ", i);
    for (int j = 0; j < set->size(); j++) {
      printf("%d", (set->get(j)) ? 1 : 0);
      if ((j+1) % 10 == 0) printf(" ");
    }
    printf("\n");
    i++;
  }
  printf("\n");
}
