/*
  This test checks the ability of DistBag_DFS to implement a distributed
  backtracking algorithm to find the exact solution count of N-Queens
  instances.

  Precomputed number of solutions:

  N | Total | Unique
  1 	 1 	 1
  2 	 0 	 0
  3 	 0 	 0
  4 	 2 	 1
  5 	 10 	 2
  6 	 4 	 1
  7 	 40 	 6
  8 	 92 	 12
  9 	 352 	 46
  10 	 724 	 92
  11 	 2,680 	 341
  12 	 14,200 	 1,787
  13 	 73,712 	 9,233
  14 	 365,596 	 45,752
  15 	 2,279,184 	 285,053
  16 	 14,772,512 	 1,846,955
  17 	 95,815,104 	 11,977,939
  18 	 666,090,624 	 83,263,591
*/

use AllLocalesBarriers;
use DistributedBag_DFS;
use PrivateDist;
use List;


/*** Command-line constants ***/

config const N = 12; // number of queens
if (N > 18) then halt("N must be <= 18");


/*** Internal constants ***/

const BUSY = false;
const IDLE = true;


/*** N-Queens specific ***/

record Node
{
  var board: [0..#N] int;   // chess board with queens positions
  var depth: int;           // depth of the node in the tree

  // Root-initializer
  proc init() {
    this.board = 0..#N;
  }

  // Copy-initializer
  proc init(other: Node) {
    this.board = other.board;
    this.depth = other.depth;
  }
}

 // Return the pre-computed solution count of the N-Queens instance (see above).
inline proc getSolutionCount(): int
{
  var solutionCount: [1..18] int
    = [1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200, 73712, 365596,
      2279184, 14722512, 95815104, 666090624];

  return solutionCount(N);
}


/*** Backtracking operators ***/

// Check if the position is safe, i.e., no two queens share the same diagonals.
proc isSafe(const board: [] int, const queen_num: int, const row_pos: int): bool
{
  // For each queen before this one
  for i in 0..#queen_num {
    // Get the row position
    const other_row_pos = board[i];

    // Check diagonals
    if (other_row_pos == row_pos - (queen_num - i) ||
        other_row_pos == row_pos + (queen_num - i)) {
      return false;
    }
  }

  return true;
}

// Decompose a node into a set of children nodes.
proc decompose(const parent: Node, ref tree_loc: int,
  ref num_sol: int): list
{
  var children: list(Node);

  const depth = parent.depth;

  if (depth == N) then num_sol += 1; // all queens are placed
  else {
    for j in depth..N-1 {
      if isSafe(parent.board, depth, parent.board[j]) {
        var child = new Node(parent);
        child.board[depth] <=> child.board[j];
        child.depth += 1;
        children.pushBack(child);
        tree_loc += 1;
      }
    }
  }

  return children;
}

// Check termination condition
inline proc allIdle(const arr: [] atomic bool): bool
{
  for elt in arr do
    if (elt.read() == BUSY) then return false;

  return true;
}


/*** Distributed Search ***/

// Print results
proc print_results(const subNodeExplored: [] int, const subSolExplored: [] int)
{
  var treeSize = (+ reduce subNodeExplored);
  var nbSol = (+ reduce subSolExplored);

  writeln("Size of the explored tree: ", treeSize);
  writeln("Number of explored solutions: ", nbSol);
  if (nbSol == getSolutionCount()) then writeln("   SUCCESS");
  else writeln("   FAIL");
}

// Search
proc main ()
{
  // Global variables (synchronization, termination)
  const PrivateSpace: domain(1) dmapped Private();
  var eachLocaleState: [PrivateSpace] atomic bool = BUSY;
  allLocalesBarrier.reset(here.maxTaskPar);

  // Statistics
  var eachExploredTree: [PrivateSpace] int;
  var eachExploredSol: [PrivateSpace] int;

  // Initialization
  var bag = new DistBag_DFS(Node, targetLocales = Locales);
  var root = new Node();
  bag.add(root, 0);

  // Parallel search
  coforall loc in Locales do on loc {

    // Local variables
    var eachTaskState: [0..#here.maxTaskPar] atomic bool = BUSY;

    // Local statistics
    var eachLocalExploredTree: [0..#here.maxTaskPar] int;
    var eachLocalExploredSol: [0..#here.maxTaskPar] int;

    coforall taskId in 0 .. #here.maxTaskPar {

      // Task variables
      ref tree_loc = eachLocalExploredTree[taskId];
      ref num_sol = eachLocalExploredSol[taskId];

      allLocalesBarrier.barrier();

      while true {

        // Try to get an element
        var (hasWork, parent) = bag.remove(taskId);

        // Check termination
        if (hasWork == 1) { // we had an element
          eachTaskState[taskId].write(BUSY);
          eachLocaleState[here.id].write(BUSY);
        }
        else if (hasWork == 0) { // we didn't have an element (fast exit)
          eachTaskState[taskId].write(IDLE);
          continue;
        }
        else { // we didn't have an element (fail)
          eachTaskState[taskId].write(IDLE);
          if allIdle(eachTaskState) {
            eachLocaleState[here.id].write(IDLE);
            if allIdle(eachLocaleState) {
              break; // end of the global search
            }
          }
          continue;
        }

        // Decompose an element
        var children = decompose(parent, tree_loc, num_sol);
        bag.addBulk(children, taskId);
      }
    }

    eachExploredTree[here.id] += (+ reduce eachLocalExploredTree);
    eachExploredSol[here.id] += (+ reduce eachLocalExploredSol);
  }

  // Print statistics and test result
  print_results(eachExploredTree, eachExploredSol);

  return 0;
}
