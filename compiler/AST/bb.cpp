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
    basicBlock->exprs.add(expr);                \
  } while (0)

#define BB_STOP()                               \
  fn->basicBlocks->add(Steal())

#define BB_RESTART()                            \
  do {                                          \
    BB_STOP();                                  \
    BB_START();                                 \
  } while (0)

#define BBB(stmt)                               \
    buildBasicBlocks(fn, stmt)

#define BB_THREAD(src, dst)                     \
  do {                                          \
    dst->ins.add(src);                          \
    src->outs.add(dst);                         \
  } while (0)


//# Statics
BasicBlock* BasicBlock::basicBlock;
Map<LabelSymbol*,Vec<BasicBlock*>*> BasicBlock::gotoMaps;
Map<LabelSymbol*,BasicBlock*> BasicBlock::labelMaps;
int BasicBlock::nextid;


// Returns true if the class invariants have been preserved.
bool BasicBlock::isOK()
{
  // Expressions must be live (non-NULL);
  forv_Vec(Expr, expr, exprs)
    if (expr == 0) return false;

  // Every in edge must have a corresponding out edge in the source block.
  forv_Vec(BasicBlock, source, ins) {
    bool found = false;
    forv_Vec(BasicBlock, bb, source->outs)
      if (bb == this) { found = true; break; }
    if (!found) return false;
  }

  // Every out edge must have a corresponding in edge in the target block.
  forv_Vec(BasicBlock, target, outs) {
    bool found = false;
    forv_Vec(BasicBlock, bb, target->ins)
      if (bb == this) { found = true; break; }
    if (!found) return false;
  }

  return true;
}

// Reset the shared statics.
void BasicBlock::reset(FnSymbol* fn)
{
  if (fn->basicBlocks)
  {
    forv_Vec(BasicBlock, bb, *fn->basicBlocks)
      delete bb;
    delete fn->basicBlocks;
  }
  fn->basicBlocks = new Vec<BasicBlock*>();
  gotoMaps.clear();
  labelMaps.clear();
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
  fn->basicBlocks->add(BasicBlock::Steal());    // BB_STOP();

  INT_ASSERT(verifyBasicBlocks(fn));
}

void BasicBlock::buildBasicBlocks(FnSymbol* fn, Expr* stmt)
{
  if (!stmt) return;

  if (BlockStmt* s = toBlockStmt(stmt))
  {
    // If a loop statement.
    if (s->blockInfo)
    {
      BasicBlock* top = basicBlock;
      BB_RESTART();
      // Assumes that blockInfo is never null.
      BB_ADD(s->blockInfo);
      BasicBlock* loopTop = basicBlock;
      for_alist(stmt, s->body) {
        BBB(stmt);
      }
      BasicBlock* loopBottom = basicBlock;
      BB_RESTART();
      BasicBlock* bottom = basicBlock;
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
      Vec<BasicBlock*>* vbb = gotoMaps.get(label);
      if (!vbb)
        vbb = new Vec<BasicBlock*>();
      vbb->add(basicBlock);
      gotoMaps.put(label, vbb);
    }
    BB_ADD(s); // Put the goto at the end of its block.
    BB_RESTART();
  } else {
    DefExpr* def = toDefExpr(stmt);
    if (def && toLabelSymbol(def->sym)) {
      // If a label appears in the middle of a block,
      // we start a new block.
      if (basicBlock->exprs.count() > 0)
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
      if (Vec<BasicBlock*>* vbb = gotoMaps.get(label)) {
        forv_Vec(BasicBlock, bb, *vbb) {
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
  forv_Vec(BasicBlock, bb, *fn->basicBlocks)
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
  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    forv_Vec(Expr, expr, bb->exprs) {
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
                          Vec<BitVec*>& GEN,
                          Vec<BitVec*>& KILL,
                          Vec<BitVec*>& IN,
                          Vec<BitVec*>& OUT) {
  bool iterate = true;
  while (iterate) {
    iterate = false;
    int i = 0;
    forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
      for (int j = 0; j < IN.v[i]->ndata; j++) {
        unsigned new_in = (OUT.v[i]->data[j] & ~KILL.v[i]->data[j]) | GEN.v[i]->data[j];
        if (new_in != IN.v[i]->data[j]) {
          IN.v[i]->data[j] = new_in;
          iterate = true;
        }
        unsigned new_out = 0;
        forv_Vec(BasicBlock, bbout, bb->outs) {
          new_out = new_out | IN.v[bbout->id]->data[j];
        }
        if (new_out != OUT.v[i]->data[j]) {
          OUT.v[i]->data[j] = new_out;
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
                         Vec<BitVec*>& GEN,
                         Vec<BitVec*>& KILL,
                         Vec<BitVec*>& IN,
                         Vec<BitVec*>& OUT,
                         bool intersect) {
  int nbbq = fn->basicBlocks->n; // size of bb queue
  Vec<int> bbq;
  BitVec bbs(nbbq);
  int iq = -1, nq = nbbq-1;      // index to first and last bb in bbq
  for (int i = 0; i < fn->basicBlocks->n; i++) {
    bbq.add(i);
    bbs.set(i);
  }
  while (iq != nq) {
    iq = (iq + 1) % nbbq;
    int i = bbq.v[iq];
    bbs.unset(i);
#ifdef DEBUG_FLOW
    if (iq == 0) {
      printf("IN\n"); printBitVectorSets(IN);
      printf("OUT\n"); printBitVectorSets(OUT);
    }
#endif
    BasicBlock* bb = fn->basicBlocks->v[i];
    bool change = false;
    for (int j = 0; j < IN.v[i]->ndata; j++) {
      if (bb->ins.n > 0) {
        unsigned new_in = (intersect) ? (unsigned)(-1) : 0;
        forv_Vec(BasicBlock, bbin, bb->ins) {
          if (intersect)
            new_in &= OUT.v[bbin->id]->data[j];
          else
            new_in |= OUT.v[bbin->id]->data[j];
        }
        if (new_in != IN.v[i]->data[j]) {
          IN.v[i]->data[j] = new_in;
          change = true;
        }
      }
      unsigned new_out = (IN.v[i]->data[j] & ~KILL.v[i]->data[j]) | GEN.v[i]->data[j];
      if (new_out != OUT.v[i]->data[j]) {
        OUT.v[i]->data[j] = new_out;
        change = true;
      }
    }
    if (change) {
      forv_Vec(BasicBlock, bbout, bb->outs) {
        if (!bbs.get(bbout->id)) {
          nq = (nq + 1) % nbbq;
          bbs.set(bbout->id);
          bbq.v[nq] = bbout->id;
        }
      }
    }
  }
}


void printBasicBlocks(FnSymbol* fn) {
  forv_Vec(BasicBlock, b, *fn->basicBlocks) {
    printf("%2d:  ", b->id);
    forv_Vec(BasicBlock, bb, b->ins) {
      printf("%d ", bb->id);
    }
    printf(" >  ");
    forv_Vec(BasicBlock, bb, b->outs) {
      printf("%d ", bb->id);
    }
    printf("\n");
    forv_Vec(Expr, expr, b->exprs) {
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

void printDefsVector(Vec<SymExpr*> defs, Map<SymExpr*,int>& defMap) {
  printf("Variable Definitions\n");
  forv_Vec(SymExpr, def, defs) {
    printf("%2d: %s[%d] in %d\n", defMap.get(def), def->var->name,
           def->var->id, def->getStmtExpr()->id);
  }
  printf("\n");
}

void printLocalsVectorSets(Vec<BitVec*>& sets, Vec<Symbol*> locals) {
  int i = 0;
  forv_Vec(BitVec, set, sets) {
    printf("%2d: ", i);
    for (int j = 0; j < set->size; j++) {
      if (set->get(j))
        printf("%s[%d] ", locals.v[j]->name, locals.v[j]->id);
    }
    printf("\n");
    i++;
  }
  printf("\n");
}

void printBitVectorSets(Vec<BitVec*>& sets) {
  int i = 0;
  forv_Vec(BitVec, set, sets) {
    printf("%2d: ", i);
    for (int j = 0; j < set->size; j++) {
      printf("%d", (set->get(j)) ? 1 : 0);
      if ((j+1) % 10 == 0) printf(" ");
    }
    printf("\n");
    i++;
  }
  printf("\n");
}
