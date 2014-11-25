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

#include "bb.h"

#include "astutil.h"
#include "bitVec.h"
#include "stlUtil.h"
#include "stmt.h"
#include "view.h"
#include "WhileStmt.h"

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
    if (s->isLoop() == true) {
      bool cForLoop  = s->isCForLoop();

      // for c for loops, add the init expr before the loop body
      if (cForLoop) {
        CallExpr* info = s->blockInfoGet();

        for_alist(stmt, toBlockStmt(info->get(1))->body) {
          buildBasicBlocks(fn, stmt, mark);
        }
      }

      // mark the top of the loop
      BasicBlock* top = basicBlock;

      restart(fn);

      // Mark and add the test expr at the loop top
      if (cForLoop) {
        CallExpr* info = s->blockInfoGet();

        for_alist(stmt, toBlockStmt(info->get(2))->body) {
          buildBasicBlocks(fn, stmt, true);
        }

      // add the condition expr at the loop top; this is not quite right for DoWhile
      } else if (WhileStmt* whileStmt = toWhileStmt(stmt)) {
        CallExpr* info = whileStmt->condExprGet();

        append(info->get(1), true);

      // PARAM_LOOP and FOR_LOOP
      } else {
        CallExpr* info = s->blockInfoGet();

        append(info, true);
      }

      BasicBlock* loopTop = basicBlock;

      for_alist(stmt, s->body) {
        buildBasicBlocks(fn, stmt, mark);
      }

      // for c for loops, add the incr expr after the loop body
      if (cForLoop) {
        CallExpr* info = s->blockInfoGet();

        for_alist(stmt, toBlockStmt(info->get(3))->body) {
          buildBasicBlocks(fn, stmt, mark);
        }
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
    LabelSymbol* label = toLabelSymbol(toSymExpr(s->label)->var);

    if (BasicBlock* bb = labelMaps.get(label)) {
      thread(basicBlock, bb);

    } else {
      std::vector<BasicBlock*>* vbb = gotoMaps.get(label);

      if (!vbb)
        vbb = new std::vector<BasicBlock*>();

      vbb->push_back(basicBlock);

      gotoMaps.put(label, vbb);
    }

    append(s, mark); // Put the goto at the end of its block.
    restart(fn);

  } else {
    DefExpr*      def = toDefExpr(stmt);
    Vec<BaseAST*> asts;

    collect_asts(stmt, asts);

    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* call = toCallExpr(ast)) {
        // mark function calls as essential
        if (call->isResolved() != NULL)
          mark = true;

        // mark essential primitives as essential
        else if (call->primitive && call->primitive->isEssential)
          mark = true;

        // mark assignments to global variables as essential
        else if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
          if (SymExpr* se = toSymExpr(call->get(1))) {
            if (se->var->type->refType == NULL)
              mark = true;
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
    }
  }
}

void BasicBlock::restart(FnSymbol* fn) {
  fn->basicBlocks->push_back(steal());
  basicBlock = new BasicBlock();
}

void BasicBlock::append(Expr* expr, bool mark) {
  basicBlock->exprs.push_back(expr);
  basicBlock->marks.push_back(mark);
}

void BasicBlock::thread(BasicBlock* src, BasicBlock* dst) {
  dst->ins.push_back(src);
  src->outs.push_back(dst);
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
      for (int j = 0; j < IN[i]->ndata; j++) {
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

    for (int j = 0; j < IN[i]->ndata; j++) {
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
           def->var->name,
           def->var->id,
           def->getStmtExpr()->id);
  }

  printf("\n");
}

void BasicBlock::printLocalsVectorSets(std::vector<BitVec*>& sets, Vec<Symbol*> locals) {
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

void BasicBlock::printBitVectorSets(std::vector<BitVec*>& sets) {
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
