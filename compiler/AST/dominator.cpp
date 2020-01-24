/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "dominator.h"

#include "stlUtil.h"

/*
 * TODO Is there a need to do the post dominators and strictly post dominators?
 *       - The algorithm to do so is the same as computing dominators but you 
 *         look swap the exit and entry node and look at look at the "outs" of 
 *         block rather than the ins.        
 */
 

/*
 * This function exists so that dead or unreachable basic blocks do not screw
 * up the dominator computation. Currently in the basic block creation gotos
 * will create a basic block immediately following them, regardless as to
 * whether there is actually any code or not.  
 *
 * For the example:
 * if cond then {
 *   // stuff
 *   goto _lbl;
 *   // unreachable stuff
 * }
 * // more stuff
 * _lbl:
 *
 * we would get something like
 *
 * 0: > 1 3
 *  cond
 * 1: 0 > 4
 *  // stuff
 *  goto _lbl;
 * 2: > 3
 *  // unreachable stuff
 * 3: 0 2 > 4
 *  // more stuff
 * 4: 1 3 >
 *  _lbl:
 * 
 * In this case an extra basic block is created and if there is nothing where 
 * unreachable stuff is, then we are creating a dead empty basic block. If we
 * have two goto's that goto the same label we get two empty basic blocks that
 * have no ins and a third empty basic block that is also empty but has the two
 * other empty basic blocks as ins. For this reason we can't just say ignore
 * all basic blocks with no ins because there can be more complex cases of dead
 * blocks that have ins (from other dead basic blocks.) The dominator analysis
 * assumed that there are no dead blocks as it leads to case where dead blocks
 * dominate every other blocks, which is clearly the  opposite of the truth.
 * 
 * This was an issue for LICM, where things that were not actually loops
 * were being identified as loops. Right now these dead empty basic blocks are
 * created even if there is no actual dead code in the AST. This is a result of
 * how basic blocks are currently created. It may be changed sometime after the
 * 1.8 release, in which case this function may become unnecessary. It can be
 * removed if we can guarantee that there are no dead blocks, and that the
 * basic block creation doesn't create any artificial dead blocks. 
 */
static void unsetDeadBlocks(std::vector<BitVec*>& dominators, 
    std::vector<BasicBlock*>& basicBlocks) {

  unsigned nBlocks = basicBlocks.size();
  unsigned oldNumBlocks = ~0;
  std::set<unsigned> deadBlocks;

  //Find any dead basic blocks: blocks that have no ins, or blocks that have
  //predecessors that have no ins. 
  while(deadBlocks.size() != oldNumBlocks) {
    oldNumBlocks = deadBlocks.size();
    for(unsigned i = 1; i < nBlocks; i++) {
      if(basicBlocks[i]->ins.size() == 0)
        deadBlocks.insert(i);
      
      bool allInsAreDead = true;
      for_vector(BasicBlock, in, basicBlocks[i]->ins) {
        if(deadBlocks.count(in->id) == 0) {
          allInsAreDead = false;
        }
      }
      if(allInsAreDead) {
        deadBlocks.insert(i);
      }
    }
  }
  
  //Set it so that any dead blocks are not dominated by any other blocks and
  //that they do not dominate any blocks. 
  for(unsigned i2 = 1; i2 < nBlocks; i2++) {
    if(deadBlocks.count(i2) == 1) {
      for(unsigned j2 = 1; j2 < nBlocks; j2++) {
        dominators[j2]->reset(i2);
      }
      dominators[i2]->reset();
    }
  }

}


/*
 * Computes the dominators for the set of basic blocks.
 * 
 * This algorithm is based on the dominator computation in Advanced Compiler Design 
 * Implementation. While the running time is O(n^2e)for a flowgraph with n nodes and 
 * e edges this algorithm is probably the most efficient as the number of basic blocks 
 * we look at at a time is usually pretty small. There are more complex algorithms that
 * can run in near linear time, but have poor performance for computations that don't 
 * contain at least a few hundred nodes. 
 * 
 * If a node a dominates node b then dominators[b]->get(a) will be true. That is to say that
 * dominators[i] stores the set of all nodes that dominate node i
 * 
 * This algorithm will be most efficient if the basic blocks are in a depth-first order. 
 */
void computeDominators(std::vector<BitVec*>& dominators, std::vector<BasicBlock*>& basicBlocks) {
  unsigned nBlocks = basicBlocks.size();
  
  //The root node cannot be dominated 
  dominators[0]->set(0);
  
  //Initially set all other nodes as dominated by every other node 
  for(unsigned i = 1; i < nBlocks; i++) {
    dominators[i]->set();
  }
  
  //Iteratively look for changes. A node a dominates b 
  //if a = b, a is immediate predecessor of b, of i 
  //b has multiple predecessors but a dominates 
  //all of them.
  bool changed = true;
  while(changed) {
    changed = false;
    
    for(unsigned i = 1; i < nBlocks; i++) {
      BitVec temp = BitVec(nBlocks);
      temp.set();
      
      BasicBlock* curBB = basicBlocks[i];
      for(unsigned j = 0; j < curBB->ins.size(); j++) {
        //TODO I would ideally like this to be &=
        temp.intersection(*dominators[curBB->ins[j]->id]);        
      }
      temp.set(i);
      
      //TODO would like to implement == and != 
      if(temp.equals(*dominators[i]) == false) {
        changed = true;
        dominators[i]->reset();
        dominators[i]->disjunction(temp);
      }
    }
  }

  unsetDeadBlocks(dominators, basicBlocks);
}


/* 
 * Checks if a node a dominates node b in the set of dominators 
 *
 * A node a dominates node b if every path path from the entry node 
 * to node b must go through a.
 */
bool dominates(unsigned a, unsigned b, std::vector<BitVec*> & dominators) {
  if(dominators[b]->test(a)) {
   return true;
  } 
  return false;
}


/*
 * Checks if a node a strictly dominates node b in the set of dominators 
 * 
 * A node a strictly dominates node b if a dominates b and a!= b
 */
bool strictlyDominates(unsigned a, unsigned b, std::vector<BitVec*> & dominators) {
 if(a == b) {
   return false;
 }
 return dominates(a, b, dominators);
}


/*
 * Computes the immediate dominators from the dominators 
 * 
 * This algorithm is based on the immediate dominator computation found in Advanced Compiler 
 * Design Implementation. It takes the already computed dominator set and set the immediate
 * dominators. For instance to get the immediate dominator of i simply get 
 * immediate Dominators[i]
 *
 * A node a immediately dominates node b if and only if a strictly dominates b
 * and there does not exist a node c such that a strictly dominates c and c 
 * strictly dominates b
 */
void computeImmediateDominators(std::vector<unsigned>& immediateDominators, std::vector<BitVec*>& dominators) {
  unsigned nBlocks = dominators.size();
  
  //Create a temporary dominator set
  std::vector<BitVec*> temp;
  for(unsigned i = 0; i < nBlocks; i++) {
    temp.push_back(new BitVec(nBlocks));
  }
  
  //Set temp[i] to the dominators[i] - {i} (the strict dominators) 
  for(unsigned i = 0; i < nBlocks; i++) {
    for(unsigned j = 0; j < nBlocks; j++) {
      if(strictlyDominates(i, j, dominators)) {
        temp[j]->set(i);
      }
    }
  }
 
  //For each node i check whether each node that dominates i
  //has dominators other than itself and if so remove them.
  //This leaves each non-root node having a single dominator
  for(unsigned i = 1; i < nBlocks; i++) {
    for(unsigned j = 0; j < nBlocks; j++){
      for(unsigned k = 0; k < nBlocks; k++) {
        if(strictlyDominates(k, i, temp) && strictlyDominates(j, i, temp)) {
          if(strictlyDominates(k, j, temp)) {
            temp[i]->reset(k);
          }
        }        
      }
    }
  }

  //Extract the node that is the immediate dominator 
  //and store it in immediateDominators
  for(unsigned i = 1; i < nBlocks; i++) {
    for(unsigned j = 0; j < nBlocks; j++) {
      if(strictlyDominates(j, i, temp)) {
        immediateDominators[i] = j;
      }
    }
  }
}

