/*
 * Copyright 2017 Advanced Micro Devices, Inc.
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

#include "passes.h"

#include "astutil.h"
#include "bb.h"
#include "bitVec.h"
#include "CForLoop.h"
#include "dominator.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "ParamForLoop.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "timer.h"
#include "optimizations.h"
#include "WhileStmt.h"

#include <algorithm>
#include <set>
#include <stack>



//#define debugHoisting
#ifdef debugHoisting
  #define printDebug(string) printf string
#else
  #define printDebug(string) //do nothing
#endif


// #define detailedTiming

#ifdef detailedTiming
  #define startTimer(timer) timer.start()
  #define stopTimer(timer)  timer.stop()
#else
  #define startTimer(timer) // do nothing
  #define stopTimer(timer)  // do nothing
#endif


Timer allOperandsAreLoopInvariantTimer;
Timer computeAliasTimer;
Timer collectSymExprAndDefTimer;
Timer calculateActualDefsTimer;
Timer buildBBTimer;
Timer computeDominatorTimer;
Timer collectNaturalLoopsTimer;
Timer canPerformCodeMotionTimer;
Timer buildLocalDefMapsTimer;
Timer computeLoopInvariantsTimer;
Timer overallTimer;

#define MAX_NUM_ALIASES 200000

//TODO The alias analysis is extremely conservative. Beyond possibly not hoisting
//things that can be, it is also a performance issue because you have a lot more
//definitions to consider before declaration something invariant.

//TODO some other possible optimizations are if you're looking at an outer
//loop in a nest you can ignore everything inside the inner loop(s) since you
//already know they can't be hoisted


/*
* This is really just a wrapper for the collection of basic blocks that
* make up a loop. However, it also stores the header, and builds the bit
* representation as the loop is built to save time. The bit representation
* and the bit representation of the exits can be gotten, putting them
* in a centralized location.
*/
class Loop {
private:
    std::vector<BasicBlock*>* loopBlocks;
    LoopStmt*                 loopAST;
    BasicBlock*               header;
    BitVec*                   bitBlocks;
    BitVec*                   bitExits;

public:
    Loop(int nBlocks) {
      loopBlocks = new std::vector<BasicBlock*>;
      bitBlocks  = new BitVec(nBlocks);
      bitExits   = new BitVec(nBlocks);
    }

    ~Loop() {
      delete loopBlocks;
      delete bitBlocks;
      delete bitExits;
    }

    // This function exists to place an expr in the "preheader" of the loop
    void insertBefore(Expr* expr) {
      if (loopAST) {
        loopAST->insertBefore(expr->remove());
      }
    }

    //Set the header, insert the header into the loop blocks, and build up the
    //relationship between the basic-block loop and the actual loop AST
    void setHeader(BasicBlock* setHeader) {
      header = setHeader;
      insertBlock(setHeader);

      loopAST = NULL;
      if (header->exprs.size() != 0) {
        if (BlockStmt* blockStmt = toBlockStmt(header->exprs.at(0)->parentExpr)) {
          if (LoopStmt* loop = toLoopStmt(blockStmt)) {
            loopAST = loop;
          } else if (blockStmt->blockTag == BLOCK_C_FOR_LOOP) {
            loopAST = CForLoop::loopForClause(blockStmt);
          }
        }
      }
    }

    //add all the blocks from other loop to this loop
    void combine(Loop* otherLoop) {
      for_vector(BasicBlock, block, *otherLoop->getBlocks()) {
        insertBlock(block);
      }
    }

    //insert a block and update the bit representation
    void insertBlock(BasicBlock* block) {
      //if this block is already in the loop, do nothing
      if(bitBlocks->test(block->id)) {
        return;
      }

      //add the block to the list of blocks and to the bit representation
      loopBlocks->push_back(block);
      bitBlocks->set(block->id);
    }

    //check if a block is in the loop based on block id
    bool contains(int i) {
      return bitBlocks->test(i);
    }

    //check if a block is in the loop
    bool contains(BasicBlock* block) {
      return bitBlocks->test(block->id);
    }

    //get the header block
    BasicBlock* getHeader() {
      return header;
    }

    //get the actual blocks in the loop
    std::vector<BasicBlock*>* getBlocks() {
      return loopBlocks;
    }

    //get the bitvector that represents the blocks in the loop
    BitVec* getBitBlocks() {
      return bitBlocks;
    }

    //get the bitvector that represents the exit blocks
    //the exit blocks are the blocks that have a next basic
    //block that is outside of the loop.
    BitVec* getBitExits() {
      bitExits->reset();
      for_vector(BasicBlock, block, *loopBlocks) {
        for_vector(BasicBlock, out, block->outs) {
          if(bitBlocks->test(out->id) == false) {
            bitExits->set(block->id);
          }
        }
      }
      return bitExits;
    }

    //return the number of blocks in the loop
    unsigned size() {
      return loopBlocks->size();
    }

    //used for sorting a collection of loops (to organize from most to least nested)
     static bool LoopSort(Loop* a, Loop* b) {return a->size() < b->size();}

};

typedef std::vector<BasicBlock*> BasicBlocks;
typedef std::map<Symbol*,std::vector<SymExpr*>*> symToVecSymExprMap;

//These two functions are used to collect all natural loops from a bunch of basic blocks and ensure the loops are stored
//from most nested to least nested for any give loop nest
void collectNaturalLoops(std::vector<Loop*>& loops, BasicBlocks& basicBlocks, BasicBlock* entryBlock, std::vector<BitVec*>& dominators);
void collectNaturalLoopForEdge(Loop* loop, BasicBlock* header, BasicBlock* tail);


/*
 * This function identifies all of the natural loops from a collection of basic blocks
 * using the dominators of each block. The basic idea is that for each basic block
 * look at the basic blocks that it can go to. If block(1)'s successor is block(2)
 * and block(2) dominates block(1) then we know that block(1) is a back-edge and that
 * block(2) is the header of a natural loop.
 *
 * From there we check if the loop header is already part of another loop (shared header)
 * which should be infrequent in chapel generated code. We treat loops with shared headers
 * as a single loop. The last step is to sort the list of loops from the loops with smallest
 * number of blocks to the largest. This has the effect of organizing the loops from most to
 * least nested for any given loop nest.
 *
 * The end result is that you have a list of all the natural loops that appeared in the basic
 * blocks and that they are sorted from most nested to least nested in the sense that for any
 * given nested loop structure the most nested one is guaranteed to appear before (closer to
 * index 0) than the more outer loops.)
 */
void collectNaturalLoops(std::vector<Loop*>& loops, BasicBlocks& basicBlocks, BasicBlock* entryBlock, std::vector<BitVec*>& dominators) {

  for_vector(BasicBlock, block, basicBlocks) {
    //Skip entry blocks
    if(block == entryBlock) {
      continue;
    }

    //for each successor
    for_vector(BasicBlock, successor, block->outs) {
      //if the successor dominates the block, block is a back-edge and successor is a header
      if(dominates(successor->id, block->id, dominators)) {
        //check if this loop shares a header with any previous one, and if so combine them into one
        bool sharedHeader = false;
        for_vector(Loop, loop, loops) {
          //if this loop shares a header with an existing one, add this loop to the existing one
          if(loop->getHeader() == successor) {
            sharedHeader = true;
            collectNaturalLoopForEdge(loop, successor, block);
          }
        }
        //if the headers weren't shared, create a new loop
        if(sharedHeader == false) {
          Loop* loop = new Loop(basicBlocks.size());
          collectNaturalLoopForEdge(loop, successor, block);
          loops.push_back(loop);
        }
      }
    }
  }
  //sort by most nested, to least nested for any given loop nest
  std::sort(loops.begin(), loops.end(), Loop::LoopSort);
}


/*
 * This function collects the blocks in a natural loop that goes from the header to the
 * tail (back edge) and stores all of the blocks that are in the loop in an initially
 * empty loop.
 *
 * Starting from the tail of the loop, work your way backwards and iteratively collect
 * all of the predecessors until you reach the head of the loop
 */
void collectNaturalLoopForEdge(Loop* loop, BasicBlock* header, BasicBlock* tail) {

  std::stack<BasicBlock*> workList;
  loop->setHeader(header);

  //If we don't have a one block loop, add the tail
  //to the worklist and start finding predecessors from there
  if(header != tail) {
   loop->insertBlock(tail);
   workList.push(tail);
  }

  //While we have more blocks to look at
  while(workList.empty() == false) {
    BasicBlock* block = workList.top();
    workList.pop();
    //look at all the predecessors so long as they are not already added
    for_vector(BasicBlock, in, block->ins) {
      if(loop->contains(in) == false) {
        loop->insertBlock(in);
        workList.push(in);
      }
    }
  }
}

// Returns rhs var if lhs aliases rhs
//
//
static Symbol*
rhsAlias(CallExpr* call) {

  // TODO: check that this handles reference variables correctly

  bool hasRef = false;
  for_alist(expr, call->argList) {
    if(SymExpr* symExpr = toSymExpr(expr)) {
      Type* symType = symExpr->symbol()->type->symbol->type;
      if(symExpr->isRef() || isRecordWrappedType(symType)) {
        hasRef = true;
      }
    }
  }
  if (!hasRef) {
    return NULL;
  }

  if (call->isPrimitive(PRIM_MOVE) ||
      call->isPrimitive(PRIM_ASSIGN) ||
      call->isPrimitive(PRIM_SET_MEMBER) ||
      call->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
    SymExpr* lhs;
    Expr *rhsT;
    if (call->isPrimitive(PRIM_MOVE) ||
        call->isPrimitive(PRIM_ASSIGN)) {
      lhs = toSymExpr(call->get(1));
      rhsT = call->get(2);
    } else {
      lhs = toSymExpr(call->get(2));
      rhsT = call->get(3);
    }
    INT_ASSERT(lhs);
    if (SymExpr* rhs = toSymExpr(rhsT)) {
      // direct alias
      return rhs->symbol();
    } else if (CallExpr* rhsCall = toCallExpr(rhsT)) {
      if (rhsCall->primitive) {
        if (rhsCall->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
            rhsCall->isPrimitive(PRIM_GET_MEMBER)) {
          SymExpr* rhs = toSymExpr(rhsCall->get(2));
          INT_ASSERT(rhs);
          return rhs->symbol();
        } else if (rhsCall->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
                   rhsCall->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
          // get_svec's second argument is the integer offset to the field,
          // instead of the field itself. We need to return the field so that
          // we note that the lhs aliases the field and not just an integer
          return getSvecSymbol(rhsCall);
        } else if(rhsCall->isPrimitive(PRIM_ADDR_OF) ||
                  rhsCall->isPrimitive(PRIM_SET_REFERENCE)) {
          SymExpr* rhs = toSymExpr(rhsCall->get(1));
          INT_ASSERT(rhs);
          return rhs->symbol();
        } else if (rhsCall->isPrimitive(PRIM_GET_REAL) ||
                   rhsCall->isPrimitive(PRIM_GET_IMAG)) {
          SymExpr* rhs = toSymExpr(rhsCall->get(1));
          INT_ASSERT(rhs);
          return rhs->symbol();
        }
      } else {
        // alias via autocopy
        SymExpr* fnExpr = toSymExpr(rhsCall->baseExpr);
        INT_ASSERT(fnExpr);
        if (fnExpr->symbol()->hasFlag(FLAG_AUTO_COPY_FN)) {
          SymExpr* rhs = toSymExpr(rhsCall->get(1));
          INT_ASSERT(rhs);
          return rhs->symbol();
        }
      }
    }
  }
  return NULL;
}


/*
 * Some primitives will not produce loop invariant results.
 * this is the list of ones that will
 */
static bool isLoopInvariantPrimitive(PrimitiveOp* primitiveOp)
{
  switch (primitiveOp->tag)
  {
      case PRIM_MOVE:
      case PRIM_UNARY_MINUS:
      case PRIM_UNARY_PLUS:
      case PRIM_UNARY_NOT:
      case PRIM_UNARY_LNOT:
      case PRIM_ADD:
      case PRIM_SUBTRACT:
      case PRIM_MULT:
      case PRIM_DIV:
      case PRIM_MOD:
      case PRIM_LSH:
      case PRIM_RSH:
      case PRIM_EQUAL:
      case PRIM_NOTEQUAL:
      case PRIM_LESSOREQUAL:
      case PRIM_GREATEROREQUAL:
      case PRIM_LESS:
      case PRIM_GREATER:
      case PRIM_AND:
      case PRIM_OR:
      case PRIM_XOR:
      case PRIM_POW:

      case PRIM_ASSIGN:
      case PRIM_ADD_ASSIGN:
      case PRIM_SUBTRACT_ASSIGN:
      case PRIM_MULT_ASSIGN:
      case PRIM_DIV_ASSIGN:
      case PRIM_MOD_ASSIGN:
      case PRIM_LSH_ASSIGN:
      case PRIM_RSH_ASSIGN:
      case PRIM_AND_ASSIGN:
      case PRIM_OR_ASSIGN:
      case PRIM_XOR_ASSIGN:

      case PRIM_MIN:
      case PRIM_MAX:

      case PRIM_SETCID:
      case PRIM_TESTCID:
      case PRIM_GETCID:
      case PRIM_SET_UNION_ID:
      case PRIM_GET_UNION_ID:
      case PRIM_GET_MEMBER:
      case PRIM_GET_MEMBER_VALUE:
      case PRIM_SET_MEMBER:
      case PRIM_CHECK_NIL:
      case PRIM_GET_REAL:
      case PRIM_GET_IMAG:

      case PRIM_SET_SVEC_MEMBER:
      case PRIM_GET_SVEC_MEMBER:
      case PRIM_GET_SVEC_MEMBER_VALUE:

      case PRIM_ADDR_OF:
      case PRIM_SET_REFERENCE:
      case PRIM_DEREF:
        return true;
    default:
      break;
    }

  // otherwise
  return false;
}


/*
 * Simple function to check if a symExpr is constant
 */
static bool isConst(SymExpr* symExpr) {
  if(VarSymbol* varSymbol = toVarSymbol(symExpr->symbol())) {
    if(varSymbol->immediate) {
      return true;
    }
  }
  return false;

  //TODO can we use vass's backend const?
  //check if its chapel const?
  //check for other things that can make it const?
}


/*
 * TODO The following three functions duplicate most of the functionality that is in the
 * routines found in astUtil. However, these use the STL containers instead of the
 * old map and vec. Additionally the astUtil implementation is wrong for the primitive
 * set member (its not identified as a def -- there is a similar note in iterator.cpp.)
 * This is a shunt until the astutil can be updated to use STL and is corrected. After
 * that these three functions can be replaced by one that just uses the buildDefUseMaps
 * that takes the set of symbols and symExpr to look in
 */


/*
 * Small helper method for adding a def/use so clean up the code for doing so a bit
 */
static void addDefOrUse(symToVecSymExprMap& localDefOrUseMap, Symbol* var, SymExpr* defOrUse) {
  if(localDefOrUseMap[var] == NULL) {
    localDefOrUseMap[var] = new std::vector<SymExpr*>;
  }
  localDefOrUseMap[var]->push_back(defOrUse);
}


/*
 * Build the local def use maps for a loop and while we're at it build the local map which is the map from each
 * symExpr to the block it it is defined in.
 */
static void buildLocalDefUseMaps(Loop* loop, symToVecSymExprMap& localDefMap, symToVecSymExprMap& localUseMap, std::map<SymExpr*, int>& localMap) {

  for_vector(BasicBlock, block, *loop->getBlocks()) {
    for_vector(Expr, expr, block->exprs) {
      if(CallExpr* callExpr = toCallExpr(expr)) {
        //if we have a prim set member .= (or svec) then the field is defed
        if(callExpr->isPrimitive(PRIM_SET_MEMBER) ||
           callExpr->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
          if(SymExpr* symExpr = toSymExpr(callExpr->get(2))) {
            if(callExpr->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
              addDefOrUse(localDefMap, getSvecSymbol(callExpr), symExpr);
            } else {
              addDefOrUse(localDefMap, symExpr->symbol(), symExpr);
            }
          }
          //and the "class" itself may also be defed. For instance if you change
          //the field of a record or union you have to note that the
          //record/union has also changed. This is not true in the case of an
          //actual class (class_class), since it's just a pointer. If the
          //pointer to a class is changed that will be detected by isDefAndOrUse
          if(SymExpr* symExpr = toSymExpr(callExpr->get(1))) {
            Type* type = symExpr->symbol()->type->symbol->type;
            if(AggregateType* curClass = toAggregateType(type)) {
              if(! curClass->isClass()) {
                addDefOrUse(localDefMap, symExpr->symbol(), symExpr);
              }
            }
          }
        }
      }

      //Check each symExpr to see if its a use and or def and add to the appropriate lists
      std::vector<SymExpr*> symExprs;
      collectLcnSymExprs(expr, symExprs);
      for_vector(SymExpr, symExpr, symExprs) {
        if(symExpr->parentSymbol) {
            localMap[symExpr] = block->id;
            int result = isDefAndOrUse(symExpr);
            //Add defs
            if(result & 1) {
              addDefOrUse(localDefMap, symExpr->symbol(), symExpr);
            }
            //add uses
            if(result & 2) {
              addDefOrUse(localUseMap, symExpr->symbol(), symExpr);
            }
            //if we have a function call, assume any "classes" fields are changed
            if(CallExpr* callExpr = toCallExpr(symExpr->parentExpr)) {
              if(callExpr->isResolved() || callExpr->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
                addDefOrUse(localDefMap, symExpr->symbol(), symExpr);
                Type* type = symExpr->symbol()->type->symbol->type;
                if(AggregateType* curClass = toAggregateType(type)) {
                  for_alist(classField, curClass->fields) {
                    if(DefExpr* classFieldDef = toDefExpr(classField)) {
                      addDefOrUse(localDefMap, classFieldDef->sym, symExpr);
                    }
                  }
                }
              }
            }
        }
      }
    }
  }
}


/*
 * Free the def and use maps
 */
static void freeLocalDefUseMaps(symToVecSymExprMap& localDefMap, symToVecSymExprMap& localUseMap) {
 symToVecSymExprMap::iterator it;
  for(it = localDefMap.begin(); it != localDefMap.end(); it++) {
    delete it->second;
  }
  for(it = localUseMap.begin(); it != localUseMap.end(); it++) {
    delete it->second;
  }
}


/*
 * Determines if the expr is loop invariant, based on the current
 * list of loop invariants
 *
 * This should only be called externally on a call expr whose lhs has only one def in a loop
 */
static bool allOperandsAreLoopInvariant(Expr* expr, std::set<SymExpr*>& loopInvariants, std::set<SymExpr*>& loopInvariantInstructions, Loop* loop,   std::map<SymExpr*, std::set<SymExpr*> >& actualDefs) {

  //if we have an assignment, recursively compute if all operands are invariant
  //if there was a different loop invariant operand, make sure all its arguments
  //are invariant
  if(CallExpr* callExpr = toCallExpr(expr)) {
    if(callExpr->primitive && isLoopInvariantPrimitive(callExpr->primitive)) {
      if(callExpr->isPrimitive(PRIM_MOVE) || callExpr->isPrimitive(PRIM_ASSIGN)) {
        return allOperandsAreLoopInvariant(callExpr->get(2), loopInvariants, loopInvariantInstructions, loop, actualDefs);
      }
      else {
        for_alist(arg, callExpr->argList) {
          if(allOperandsAreLoopInvariant(arg, loopInvariants, loopInvariantInstructions, loop, actualDefs) == false) {
            return false;
          }
        }
        return true;
      }
    }
    return false;
  } else if(SymExpr* symExpr = toSymExpr(expr)) {

    //If the operand is invariant (0 defs in the loop, or constant)
    //it is invariant
    if(loopInvariants.count(symExpr) == 1) {
      return true;
    }

    //else check if there is only one def for the symExpr. That the
    //def is invariant, and that the def occurs before this symExpr
    int numDefs = 0;
    SymExpr* def = NULL;

    if(actualDefs.count(symExpr) == 1) {
      numDefs += actualDefs[symExpr].size();
      if(actualDefs[symExpr].size() == 1) {
        def = *actualDefs[symExpr].begin();
      }
    }

    if(numDefs > 1 || def == NULL) {
      return false;
    }

    //if the def is loop invariant
    if(loopInvariantInstructions.count(def) == 1)  {
      //if the def comes from a call, we can't guarantee
      //its invariant without more analysis
      if(CallExpr* call = toCallExpr(def->parentExpr)) {
        if (call->isResolved()) {
          return false;
        }
      }

      //check that the def occurs before this symExpr
      bool sawDef = false;
      for_vector(BasicBlock, block, *loop->getBlocks()) {
        for_vector(Expr, expr, block->exprs) {
          std::vector<SymExpr*> symExprs;
          collectSymExprs(expr, symExprs);
          for_vector(SymExpr, symExpr2, symExprs) {

            //mark that we have seen the definition of the operand
            if(def == symExpr2) {
              sawDef = true;
            }
             //if we saw a use of the var before its def, not invariant
            if(symExpr2->symbol() == def->symbol()) {
              if(sawDef == false)
                return false;
            }
            //if we have seen our symExpr that uses the def after the actual def,we're good
            if(symExpr == symExpr2) {
              if(sawDef == true) {
                  return true;
              }
            }
          }
        }
      }
    }
    return false;
  }
  return false;
}

static bool computeAliases(FnSymbol* fn, std::map<Symbol*, std::set<Symbol*> >& aliases) {
  //Since the current alias analysis is pretty conservative, you can run into
  //the case where you have so many aliases that you run of space in memory to
  //hold all of the aliases (since we keep track of all pairs) , which leads to
  //outrageous compilations times. Until the alias analysis is made less
  //conservative we will just not hoist from a function if there are too many
  //aliases. Note that this will affect very very few cases and thus will not
  //prevent LICM in all but a few cases. The threshold is not a compiler option
  //as it seems unlikely that anybody would be willing to wait the 2+ hours it
  //takes for a program to actual compile. However, if compilation times become
  //unpredictable a compiler flag can be added. With less conservative analysis
  //though, this threshold should be eliminated.
  int numAliases = 0;
  //compute the map of aliases for each symbol
  startTimer(computeAliasTimer);

  //Compute the aliases for the function's parameters. Any args passed by ref
  //can potentially alias each other.
  // TODO: pull out this alias analysis
  // TODO: use results of computeNoAliasSets
  for_alist(formal1, fn->formals) {
    for_alist(formal2, fn->formals) {
      if(formal1 == formal2)
        continue;
      if(ArgSymbol* arg1 = toArgSymbol(toDefExpr(formal1)->sym)) {
        if(ArgSymbol* arg2 = toArgSymbol(toDefExpr(formal2)->sym)) {
          // TODO: should this handle const ref?
          if(arg1->intent == INTENT_REF && arg2->intent == INTENT_REF) {
            aliases[arg1].insert(arg2);
            aliases[arg2].insert(arg1);
          }
        }
      }
    }
  }

  for_vector(BasicBlock, block2, *fn->basicBlocks) {
    //if there are too many aliases, just return. Since nothing has been added
    //to the list of invariants, nothing will be hoisted from the current fn
    if(numAliases > MAX_NUM_ALIASES) {
#ifdef detailedTiming
       FILE* tooManyAliasesFile = fopen(astr(CHPL_HOME,"/LICMaliases.txt"), "a");
       fprintf(tooManyAliasesFile, "Skipping fn %s %d of module %s %d "
           "because there were too many aliases\n", fn->name, fn->id,
           fn->getModule()->name, fn->getModule()->id);

       fclose(tooManyAliasesFile);
#endif
      stopTimer(computeAliasTimer);
      return true;
    }

    for_vector(Expr, expr, block2->exprs) {
      if(CallExpr* call = toCallExpr(expr)) {
        Symbol* rhs = rhsAlias(call);
        if(rhs != NULL) {
          Symbol* lhs = NULL;
          if(call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN))
            lhs = toSymExpr(call->get(1))->symbol();
          else
            lhs = toSymExpr(call->get(2))->symbol();

          for_set(Symbol, rhsAlias, aliases[rhs]) {
            printDebug(("%s %d aliases %s %d\n", lhs->name, lhs->id,
              rhsAlias->name, rhsAlias->id));

            aliases[rhsAlias].insert(lhs);
            aliases[lhs].insert(rhsAlias);
            numAliases += 2;
          }
          printDebug(("%s %d aliases %s %d\n", lhs->name, lhs->id, rhs->name,
            rhs->id));

          aliases[rhs].insert(lhs);
          aliases[lhs].insert(rhs);
          numAliases += 2;
        }
      }
    }
  }
  stopTimer(computeAliasTimer);

  if (fReportAliases) {
    if (fn->getModule()->modTag == MOD_USER) {
      printf("LICM: may-alias report for a loop in function %s:\n", fn->name);
      // Print out aliases for user variables
      std::map<Symbol*, std::set<Symbol*> >::iterator it;
      for (it = aliases.begin(); it != aliases.end(); ++it) {
        Symbol* sym = it->first;
        std::set<Symbol*> &others = it->second;
        for_set(Symbol, otherSym, others) {

          // Don't report each pair more than once
          if (sym->id < otherSym->id) {
            bool symTemp = (sym->hasFlag(FLAG_TEMP) && !isArgSymbol(sym));
            bool otherTemp = (otherSym->hasFlag(FLAG_TEMP) && !isArgSymbol(otherSym));
            const char* symName = NULL;
            const char* otherSymName = NULL;

            if (developer) {
              symName = sym->name;
              otherSymName = otherSym->name;
            } else {
              if (sym->hasFlag(FLAG_RETARG))
                symName = "return argument";
              else if (!symTemp)
                symName = sym->name;

              if (otherSym->hasFlag(FLAG_RETARG))
                otherSymName = "return argument";
              else if (!otherTemp)
                otherSymName = otherSym->name;
            }

            if (symName && otherSymName)
              printf("  %s may alias %s\n", symName, otherSymName);

            // But make sure (a,b) is also recorded as (b,a)
            INT_ASSERT(aliases[otherSym].count(sym) > 0);
          }
        }
      }
    }
  }
  return false;
}

/*
 * The basic algorithm will be to find all of the constants, and then find things that
 * have no definitions in the loop. We also need to consider a symbols aliases when we're
 * talking about if it has any definitions. So if symbol and all its aliases have no definitions
 * then it is loop invariant. We need to think carefully about this because the alias will definitely
 * have one definition and that is where it is assigned to whatever it is aliasing. So if we have a = &b
 * Then a is an alias for b. The current alias analysis is extremely conservative. If there is only one def
 * of a variable check if it is composed of loop invariant operands and operations.
 */
static void computeLoopInvariants(std::vector<SymExpr*>& loopInvariants,
    std::set<Symbol*>& defsInLoop, Loop* loop, symToVecSymExprMap& localDefMap,
    std::map<Symbol*, std::set<Symbol*> >& aliases) {

  // collect all of the symExprs, defExprs, and callExprs in the loop
  startTimer(collectSymExprAndDefTimer);
  std::vector<SymExpr*> loopSymExprs;
  std::vector<CallExpr*> callsInLoop;
  for_vector(BasicBlock, block, *loop->getBlocks()) {
    for_vector(Expr, expr, block->exprs) {
      collectFnCalls(expr, callsInLoop);
      collectSymExprs(expr, loopSymExprs);
      if (DefExpr* defExpr = toDefExpr(expr)) {
        if (toVarSymbol(defExpr->sym)) {
          defsInLoop.insert(defExpr->sym);
        }
      }
    }
  }
  stopTimer(collectSymExprAndDefTimer);

  //calculate the actual defs of a symbol including the defs of
  //its aliases. If there are no defs or we have a constant,
  //add it to the list of invariants
  startTimer(calculateActualDefsTimer);
  std::set<SymExpr*> loopInvariantOperands;
  std::set<SymExpr*> loopInvariantInstructions;
  std::map<SymExpr*, std::set<SymExpr*> > actualDefs;
  for_vector(SymExpr, symExpr, loopSymExprs) {

    //skip already known invariants
    if(loopInvariantOperands.count(symExpr) == 1) {
      continue;
    }

    //mark all the const operands
    if(isConst(symExpr)) {
      loopInvariantOperands.insert(symExpr);
    }

    //calculate defs of the aliases
    if(aliases.count(symExpr->symbol()) == 1) {
      for_set(Symbol, symbol, aliases[symExpr->symbol()]) {
        if(localDefMap.count(symbol) == 1) {
          //for each symExpr that defines the alias
          for_vector(SymExpr, aliasSymExpr, *localDefMap[symbol]) {
            if(CallExpr* call = toCallExpr(aliasSymExpr->parentExpr)) {
              if(symExpr->symbol() == rhsAlias(call)) {
                //do nothing
              }
              else if(aliases[symExpr->symbol()].count(rhsAlias(call)) == 0) {
                actualDefs[symExpr].insert(aliasSymExpr);
              }
            }
          }
        }
      }
    }

    // We have to note that for records if a field is deffed then the record
    // itself is also deffed. This is handled in buildDefUseMaps for normal
    // variables but we have to handle when a variable aliases a record's field
    // and the alias is deffed.
    if(isLcnSymbol(symExpr->symbol())) {
      // if the current variable is a record
      if(AggregateType* ct = toAggregateType(symExpr->symbol()->type->symbol->type)) {
        if(isRecord(symExpr->symbol()->type->symbol->type)) {
          // go through each field and check for aliases
          for_fields(field, ct) {
            if(aliases.count(field) == 1) {
              // for each alias
              for_set(Symbol, alias, aliases[field]) {
                if(localDefMap.count(alias) == 1) {
                  // mark each def of the alias as a def of the record (so long
                  // as that def isn't the alias creation.)
                  for_vector(SymExpr, aliasDef, *localDefMap[alias]) {
                    if(CallExpr* call = toCallExpr(aliasDef->parentExpr)) {
                      if(field != rhsAlias(call)) {
                        actualDefs[symExpr].insert(aliasDef);
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    //add the defs of the symbol itself
    if(localDefMap.count(symExpr->symbol()) == 1) {
      for_vector(SymExpr, aliasSymExpr, *localDefMap[symExpr->symbol()]) {
        actualDefs[symExpr].insert(aliasSymExpr);
      }
    }

    bool mightHaveBeenDeffedElseWhere = false;
    // assume that anything passed in by ref has been changed elsewhere
    // Note that not all things that are passed by ref will have the ref intent
    // flag, and may just be ref variables. This is a known bug, see comments
    // in addVarsToFormals(): flattenFunctions.cpp.
    if (isArgSymbol(symExpr->symbol()) &&
        symExpr->getValType()->symbol->hasFlag(FLAG_ITERATOR_CLASS) == false) {
      if(ArgSymbol* argSymbol = toArgSymbol(symExpr->symbol())) {
        if(argSymbol->isRef()) {
          mightHaveBeenDeffedElseWhere = true;
        }
      }
      for_set(Symbol, aliasSym, aliases[symExpr->symbol()]) {
        if(ArgSymbol* argSymbol = toArgSymbol(aliasSym)) {
          if(argSymbol->isRef()) {
            mightHaveBeenDeffedElseWhere = true;
          }
        }
      }
    }
    // Find where the variable is defined.
    Symbol* defScope = symExpr->symbol()->defPoint->parentSymbol;
    // if the variable is a module level (global) variable
    if (isModuleSymbol(defScope)) {
      // if there are any function calls inside the loop, assume that one of
      // the functions may have changed the value of the global. Note that we
      // don't have to worry about a different task updating the global since
      // that would have to be protected with a sync or be atomic in which case
      // no hoisting will occur in the function at all. Any defs to the global
      // inside of this loop will be detected just like any other variable
      // definitions.
      // TODO this could be improved to check which functions modify the global
      // and see if any of those functions are being called in this loop.
      if (callsInLoop.size() != 0) {
        mightHaveBeenDeffedElseWhere = true;
      }
    }
    if (symExpr->symbol()->isRef()) {
        mightHaveBeenDeffedElseWhere = true;
    }
    for_set(Symbol, aliasSym, aliases[symExpr->symbol()]) {
      if (aliasSym->isRef()) {
        mightHaveBeenDeffedElseWhere = true;
      }
    }
    //if there were no defs of the symbol, it is invariant
    if(actualDefs.count(symExpr) == 0 && !mightHaveBeenDeffedElseWhere) {
      loopInvariantOperands.insert(symExpr);
    }
  }
  stopTimer(calculateActualDefsTimer);

  //now we want to iteratively search for all of the variables that have
  //operands that themselves are loop invariant.
  unsigned oldNumInvariants = 0;
  do {
    oldNumInvariants = loopInvariantInstructions.size();

    for_vector(SymExpr, symExpr2, loopSymExprs) {

      if(loopInvariantInstructions.count(symExpr2) == 1 || loopInvariantOperands.count(symExpr2) == 1) {
        continue;
      }
      //if there is only one def of that variable check if it is loop invariant too
      //Also check that the definition of the variable is inside the loop. This ensures
      //that the variable is not live into the loop
      if(actualDefs.count(symExpr2) == 1) {
        if(actualDefs[symExpr2].size() == 1) {
          if(defsInLoop.count(symExpr2->symbol()) == 1) {
            if(CallExpr* callExpr = toCallExpr(symExpr2->parentExpr)) {
              if(callExpr->isPrimitive(PRIM_MOVE) ||
                 callExpr->isPrimitive(PRIM_ASSIGN)) {
                if(callExpr->get(1) == symExpr2) {
                  startTimer(allOperandsAreLoopInvariantTimer);
                  bool loopInvarOps = allOperandsAreLoopInvariant(callExpr, loopInvariantOperands, loopInvariantInstructions, loop, actualDefs);
                  stopTimer(allOperandsAreLoopInvariantTimer);
                  if(loopInvarOps){
                    loopInvariantInstructions.insert(symExpr2);
                    loopInvariants.push_back(symExpr2);
                  }
                }
              }
            }
          }
        }
      }
    }
  }  while(oldNumInvariants != loopInvariantInstructions.size());


#ifdef debugHoisting
  printf("\n");
  printf("HOISTABLE Invariants\n");
  for_set(SymExpr, loopInvariant, loopInvariantInstructions) {
    printf("Symbol %s with id %d is a hoistable loop invariant\n", loopInvariant->symbol()->name, loopInvariant->symbol()->id);
  }

  printf("\n\n\n");
  printf("Invariant OPERANDS\n");
  for_set(SymExpr, symExpr2, loopInvariantOperands) {
    printf("%s %d is invariant\n", symExpr2->symbol()->name, symExpr2->id);
  }

  printf("\n\n\n");
  for_vector(SymExpr, symExpr3, loopSymExprs) {
    printf("%s %d is used/ defed\n", symExpr3->symbol()->name, symExpr3->symbol()->id);
  }
#endif

}


/*
 * In order to be hoisted a definition of a variable needs to dominate all uses.
 *
 * Consider a case where you have
 * if(first)
 *   first = false;
 *
 * Even though first = false is loop invariant, you clearly don't want to hoist it
 * because that would have the effect of executing first = false before the use.
 *
 */
static bool defDominatesAllUses(Loop* loop, SymExpr* def, std::vector<BitVec*>& dominators, std::map<SymExpr*, int>& localMap, symToVecSymExprMap& localUseMap) {

  if(localUseMap.count(def->symbol()) == 0 ) {
    return false;
  }

  int defBlock = localMap[def];

  for_vector(SymExpr, symExpr, *localUseMap[def->symbol()]) {
    if(dominates(defBlock, localMap[symExpr], dominators) == false) {
      return false;
    }
  }
  return true;
}


/*
 * Check if a definition dominates all the loop exits. The loop exits can be gotten from the loop, and beyond that we
 * need the dominators and the map from SymExpr to the block they are used/defed in
 *
 * Consider a case where you have a live out variable from the loop:
 * if(cond1)
 *   goto exitLoop;
 * if (cond2)
 *   b = 4;
 *
 * exitLoop:
 *   writeln(b);
 *
 * b's definition is loop invariant, but you have to ensure that the def will dominate the exit blocks
 * where it may be used.
 *
 */
static bool defDominatesAllExits(Loop* loop, SymExpr* def, std::vector<BitVec*>& dominators, std::map<SymExpr*, int>& localMap) {
  int defBlock = localMap[def];

  BitVec* bitExits = loop->getBitExits();

  for(size_t i = 0; i < bitExits->size(); i++) {
    if(bitExits->test(i)) {
      if(dominates(defBlock, i, dominators) == false) {
        return false;
      }
    }
  }
  return true;
}



/*
 * Collects the uses and defs of symbols the baseAST
 * and checks for any synchronization variables such as
 * atomics, syncs, and singles.
 */
static bool containsSynchronizationVar(BaseAST* ast) {
  std::vector<SymExpr*> symExprs;

  collectLcnSymExprs(ast, symExprs);

  for_vector(SymExpr, symExpr, symExprs) {
      Type* symType = symExpr->symbol()->type;
      Type* valType = symType->getValType();

      if (isSyncType(symType)    ||
          isSingleType(symType)  ||
          isAtomicType(symType)  ||

          isSyncType(valType)    ||
          isSingleType(valType)  ||
          isAtomicType(valType)) {

        return true;
      }
  }

  return false;
}


// TODO Looking for synchronization variables is very
// similar to what is currently being done in remote
// value forwarding. It would be a good idea to unify
// the two implementations.

/*
 * Checks if a loop can have loop invariant code motion
 * performed on it. Specifically we do not want to hoist
 * from loops that have sync, single, or atomic vars in
 * them, though there may be others in the future. Also,
 * Do not perform code motion if any function calls or
 * nested function calls contains syncs, atomics, or
 * singles.
 */
static bool canPerformCodeMotion(Loop* loop) {

  for_vector(BasicBlock, block, *loop->getBlocks()) {
    for_vector(Expr, expr, block->exprs) {

      //Check for nested function calls containing
      //synchronization variables
      std::set<FnSymbol*> fnSymbols;
      collectUsedFnSymbols(expr, fnSymbols);
      for_set(FnSymbol, fnSymbol2, fnSymbols) {
        if(containsSynchronizationVar(fnSymbol2)) {
          return false;
        }
      }

      //Check if there are any synchronization variables
      //in the current expr
      if(containsSynchronizationVar(expr)) {
        return false;
      }
    }
  }
  return true;
}


/*
 * The basic algorithm for loop invariant code motion is as follows:
 * First figure out where the loops actually are. To do this the dominators need
 * to be computed and then the natural loops can be collected.
 *
 * Now that you have identified the loops you can compute the invariants. A definition
 * is invariant if the operations are loop and invariant and all the operands are loop
 * invariant. An operand is loop invariant if it is constant, has no definitions that
 * reach it located inside of the loop, or it has one definition that reaches it, that
 * definition is in the loop, and that definition is itself loop invariant.
 *
 * You now have a list of all the definitions that are loop invariant, these can be
 * hoisted before the loop(into a preheader of sorts) so long as they definition dominates
 * all uses in the loop, and the block that the definition is located in dominates all exits.
 */
// This function returns the number of loops LICM'd
static long licmFn(FnSymbol* fn) {
  long numLoops = 0;
  //build the basic blocks, where the first bb is the entry block
  startTimer(buildBBTimer);

  BasicBlock::buildBasicBlocks(fn);

  std::vector<BasicBlock*> basicBlocks = *fn->basicBlocks;

  BasicBlock* entryBlock = basicBlocks[0];

  unsigned nBlocks = basicBlocks.size();

  stopTimer(buildBBTimer);

  //compute the dominators
  startTimer(computeDominatorTimer);
  std::vector<BitVec*> dominators;
  for(unsigned i = 0; i < nBlocks; i++) {
    dominators.push_back(new BitVec(nBlocks));
  }
  computeDominators(dominators, basicBlocks);
  stopTimer(computeDominatorTimer);

  //Collect all of the loops
  startTimer(collectNaturalLoopsTimer);
  std::vector<Loop*> loops;
  collectNaturalLoops(loops, basicBlocks, entryBlock, dominators);
  stopTimer(collectNaturalLoopsTimer);

  //For each loop found
  for_vector(Loop, curLoop, loops) {

    //check that this loop doesn't have anything that
    //would prevent code motion from occurring
    startTimer(canPerformCodeMotionTimer);
    bool performCodeMotion = canPerformCodeMotion(curLoop);
    stopTimer(canPerformCodeMotionTimer);
    if(performCodeMotion == false) {
      continue;
    }

    //build the defUseMaps
    startTimer(buildLocalDefMapsTimer);
    symToVecSymExprMap localDefMap;
    symToVecSymExprMap localUseMap;
    std::map<SymExpr*, int> localMap;
    buildLocalDefUseMaps(curLoop, localDefMap, localUseMap, localMap);
    stopTimer(buildLocalDefMapsTimer);

    //and use the defUseMaps to compute loop invariants
    startTimer(computeLoopInvariantsTimer);
    std::vector<SymExpr*> loopInvariants;
    std::set<Symbol*> defsInLoop;
    std::map<Symbol*, std::set<Symbol*> > aliases;
    bool tooManyAliases = computeAliases(fn, aliases);
    if (tooManyAliases) {
      return 0;
    }
    computeLoopInvariants(loopInvariants, defsInLoop, curLoop, localDefMap, aliases);
    stopTimer(computeLoopInvariantsTimer);

    //For each invariant, only move it if its def, dominates all uses and all exits
    for_vector(SymExpr, symExpr, loopInvariants) {
      if(CallExpr* call = toCallExpr(symExpr->parentExpr)) {
        if(defDominatesAllUses(curLoop, symExpr, dominators, localMap, localUseMap)) {
          if(defDominatesAllExits(curLoop, symExpr, dominators, localMap)) {
            if(defsInLoop.count(symExpr->symbol()) == 1) {
              curLoop->insertBefore(symExpr->symbol()->defPoint);
            }
            curLoop->insertBefore(call);
          }
        }
      }
    }

    freeLocalDefUseMaps(localDefMap, localUseMap);
  }
  numLoops += loops.size();

  for_vector(Loop, loop, loops) {
    delete loop;
    loop = 0;
  }

  for_vector(BitVec, bitVec, dominators) {
    delete bitVec;
    bitVec = 0;
  }

  return numLoops;
}

void loopInvariantCodeMotion(void) {

  // compute array element alias sets
  computeNoAliasSets();

  // optimize certain statements in foralls to unordered
  optimizeForallUnorderedOps();

  if(fNoLoopInvariantCodeMotion) {
    return;
  }

  startTimer(overallTimer);
  long numLoops = 0;

  //TODO use stl routine here
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    numLoops += licmFn(fn);
  }

  stopTimer(overallTimer);

#ifdef detailedTiming
  FILE *timingFile;
  FILE *maxTimeFile;
  timingFile = fopen(astr(CHPL_HOME,"/LICMtiming.txt"), "a");
  maxTimeFile = fopen(astr(CHPL_HOME,"/LICMmaxTime.txt"), "a");

  fprintf(timingFile, "For compilation of %s:                         \n", compileCommand );
  fprintf(timingFile, "Spent %2.3f seconds building basic blocks      \n", buildBBTimer.elapsedSecs());
  fprintf(timingFile, "Spent %2.3f seconds computing dominators       \n", computeDominatorTimer.elapsedSecs());
  fprintf(timingFile, "Spent %2.3f seconds collecting natural loops   \n", collectNaturalLoopsTimer.elapsedSecs());
  fprintf(timingFile, "Spent %2.3f seconds building local def maps    \n", buildLocalDefMapsTimer.elapsedSecs());
  fprintf(timingFile, "Spent %2.3f seconds on can perform code motion \n", canPerformCodeMotionTimer.elapsedSecs());
  fprintf(timingFile, "Spent %2.3f seconds computing loop invariants  \n", computeLoopInvariantsTimer.elapsedSecs());

  double estimateOverall = buildBBTimer.elapsedSecs() + computeDominatorTimer.elapsedSecs() + collectNaturalLoopsTimer.elapsedSecs() + \
  buildLocalDefMapsTimer.elapsedSecs() + canPerformCodeMotionTimer.elapsedSecs() + computeLoopInvariantsTimer.elapsedSecs();


  fprintf(timingFile, "Spent %2.3f seconds loop hoisting on %ld loops \n", overallTimer.elapsedSecs(), numLoops);
  fprintf(timingFile, "      %2.3f seconds unaccounted for            \n", overallTimer.elapsedSecs() - estimateOverall);

  double estimateInvariant = allOperandsAreLoopInvariantTimer.elapsedSecs() + computeAliasTimer.elapsedSecs() + \
  collectSymExprAndDefTimer.elapsedSecs() + calculateActualDefsTimer.elapsedSecs();

  fprintf(timingFile, "\n");
  fprintf(timingFile, "In loop invariant computation:                 \n");
  fprintf(timingFile, "Spent %2.3f seconds collecting sym/def Expr    \n", collectSymExprAndDefTimer.elapsedSecs());
  fprintf(timingFile, "Spent %2.3f seconds computing aliases          \n", computeAliasTimer.elapsedSecs());
  fprintf(timingFile, "Spent %2.3f seconds calculating actual defs    \n", calculateActualDefsTimer.elapsedSecs());
  fprintf(timingFile, "Spent %2.3f seconds in allOperandsAreInvariant \n", allOperandsAreLoopInvariantTimer.elapsedSecs());

  fprintf(timingFile, "      %2.3f seconds unaccounted for            \n\n", computeLoopInvariantsTimer.elapsedSecs() - estimateInvariant);
  fprintf(timingFile, "_______________________________________________\n\n\n");


  fprintf(maxTimeFile, "For compilation of %s:                         \n", compileCommand );
  fprintf(maxTimeFile, "Spent %2.3f seconds loop hoisting on %ld loops \n\n", overallTimer.elapsedSecs(), numLoops);
  fprintf(maxTimeFile, "_______________________________________________\n\n\n");

  fclose(timingFile);
  fclose(maxTimeFile);
#endif

}

