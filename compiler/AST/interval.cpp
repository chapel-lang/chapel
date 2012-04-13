#include <cstdlib>
#include "astutil.h"
#include "interval.h"
#include "bb.h"
#include "bitVec.h"
#include "expr.h"
#include "stmt.h"
#include "view.h"

#define CHPL_USER_START 1

//static int globalIntervalCounter = CHPL_USER_START;

Interval::Interval(int init_id) : id(init_id) {}

#define INTERVAL_START(intervals)               \
  do {                                          \
    interval = new Interval(id++);              \
    intervals->add(interval);                   \
  } while (0)

#define INTERVAL_ADD(block,intv)                \
  intv->bb.add(block)

static void expandInterval(Map<BasicBlock*, Interval*>& intervalMap, 
    Interval *currInt, BasicBlock *block, Vec<BasicBlock *>& bbNotSelectedSet) {
  forv_Vec(BasicBlock, bbOut, block->outs) {
    bool isInInterval = true;
    // Here if we reach the special forall block, we stop the interval
    if (bbOut->forAll == false){
      forv_Vec(BasicBlock, bbIn, bbOut->ins) {
        Interval *inInt = intervalMap.get(bbIn);
        if (inInt != currInt)
          isInInterval = false;
      }
      if (isInInterval) {
        // SAEED -- Start
        if (block->outs.n > 1){
          bbOut->condExprs.copy(block->condExprs);
          bbOut->condBools.copy(block->condBools);
          bbOut->condExprs.add(block->branch);
          if (bbOut == block->thenBB){
            bbOut->condBools.add(true);
          } else if (bbOut == block->elseBB) {
            bbOut->condBools.add(false);
          }
        }
        // SAEED -- End
        intervalMap.put(bbOut, currInt);
        INTERVAL_ADD(bbOut, currInt);
        bbNotSelectedSet.remove(bbNotSelectedSet.index(bbOut));
        expandInterval(intervalMap, currInt, bbOut, bbNotSelectedSet);
      }
    }
  }
}

static void innerBB(FnSymbol* fn, Map<BasicBlock*, Interval*>& intervalMap, Interval *interval, BasicBlock *bb, int id) {

  Vec<BasicBlock *> bbNotSelectedSet;

  bbNotSelectedSet.copy(*fn->basicBlocks);

  bool isRoot = true;
  int i = 0;
  while (isRoot) {
    BasicBlock *block = fn->basicBlocks->v[i++];
    if (block->ins.n == 0) {
      isRoot = false;
      INTERVAL_START(fn->intervals);
      INTERVAL_ADD(block, interval);
      intervalMap.put(block, interval);
      bbNotSelectedSet.remove(bbNotSelectedSet.index(block));
      expandInterval(intervalMap, interval, block, bbNotSelectedSet);
    }
  }

  while (bbNotSelectedSet.n > 0) {
    BasicBlock *headBlock = NULL;
    bool foundSelected = false;
    forv_Vec(BasicBlock, block, bbNotSelectedSet) {
      if (block->ins.n == 0) {
        foundSelected = true;
      } else {
        forv_Vec(BasicBlock, predBlock, block->ins) {
          if (!bbNotSelectedSet.in(predBlock))
            foundSelected = true;
        }
      }	
      /* Albert - block->ins == 0 is a weird case resulting from GOTOs */
      if (foundSelected) {
        headBlock = block;
        break;
      }
    }
    if (foundSelected) {
      INTERVAL_START(fn->intervals);
      INTERVAL_ADD(headBlock, interval);
      intervalMap.put(headBlock, interval);
      bbNotSelectedSet.remove(bbNotSelectedSet.index(headBlock));
      if (headBlock->forAll == false)
        expandInterval(intervalMap, interval, headBlock, bbNotSelectedSet);
    }
  }

  forv_Vec(BasicBlock, block, *fn->basicBlocks) {
    Interval *currInt = intervalMap.get(block);
    forv_Vec(BasicBlock, inBB, block->ins) {
      Interval *predInt = intervalMap.get(inBB);
      if (predInt != currInt) {
        predInt->outs.add(currInt);
        currInt->ins.add(predInt);
        predInt->sourceEdges.add(inBB);
      }
    }
  }
}

static void
determineIntervalType(FnSymbol *fn, Interval *interval) {
  Vec<BasicBlock*> vecBB = interval->bb;
  BasicBlock *headBB = vecBB.v[0];
  if (headBB && headBB->exprs.n > 0) {
    Expr *headExpr = headBB->exprs.v[0];
    if (CallExpr *call = toCallExpr(headExpr)) {
      if (call->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP)) {
        interval->intervalTag = FOR_LOOP;
        return;
      } else if (call->isPrimitive(PRIM_ON_IL)) {
        interval->intervalTag = FORALL_LOOP;
        interval->low = toSymExpr(call->get(1))->var;
        interval->high = toSymExpr(call->get(2))->var;
        interval->stride = toSymExpr(call->get(3))->var;
        interval->ILIndex = toSymExpr(call->get(4))->var;
        return;
      }
    }
  }
  interval->intervalTag = BLOCK;
}


void buildIntervals(FnSymbol* fn, Map<BasicBlock*, Interval*>& intervalMaps) {
  static Interval* interval;
  static int id = CHPL_USER_START;

  if (fn->intervals) {
    forv_Vec(Interval, interval, *fn->intervals) {
      delete interval;
    }
    delete fn->intervals;
  }
  fn->intervals = new Vec<Interval*>();
  intervalMaps.clear();

  BasicBlock* headBB = fn->basicBlocks->v[0];
  if (headBB == NULL)
    INT_FATAL("CODE HAS NO BASIC BLOCKS");
  /* We are the start node with no predecessors */
  if (headBB->ins.n != 0)
    INT_FATAL("INITIAL BB HAS PREDECESORS");

  /* do the actual interval partitioning */
  innerBB(fn, intervalMaps, interval, headBB, id);

  /* Is this a forall function? */
  forv_Vec(Interval, interval, *fn->intervals)
    determineIntervalType(fn, interval);
}
