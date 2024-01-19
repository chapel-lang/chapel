/*
  This test checks the ability of distBag to implement a distributed
  backtracking algorithm to find the exact solution count of N-Queens
  instances. It solves the instances of N-Queens from 1 to maxN.

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
use DistributedBag;
use PrivateDist;
use List;


/*** Command-line constants ***/

config const maxN = 12;
if (maxN < 1 || maxN > 18) then halt("maxN must be > 0 and <= 18");


/*** Internal constants ***/

const BUSY = false;
const IDLE = true;


/*** N-Queens Node ***/

record Node
{
  var dom: domain(1);   // domain of the chess board
  var board: [dom] int; // chess board with queens positions
  var depth: int;       // depth of the node in the tree

  // Default-initializer
  proc init() {}

  // Root-initializer
  proc init(const N: int) {
    this.dom = {0..#N};
    this.board = 0..#N;
  }

  // Copy-initializer
  proc init(other: Node) {
    this.dom   = other.dom;
    this.board = other.board;
    this.depth = other.depth;
  }
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
proc decompose(const parent: Node, const N: int, ref tree_loc: int,
  ref num_sol: int): list(Node)
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

proc nqueens_search(const N: int)
{
  // Global variables (synchronization, termination)
  var eachLocaleState: [PrivateSpace] atomic bool = BUSY;
  allLocalesBarrier.reset(here.maxTaskPar);

  // Statistics
  var eachExploredTree: [PrivateSpace] int;
  var eachExploredSol: [PrivateSpace] int;

  // Initialization
  var bag = new distBag(Node, targetLocales = Locales);
  var root = new Node(N);
  bag.add(root, 0);

  // Parallel search
  coforall loc in Locales with (ref eachExploredTree, ref eachExploredSol,
    ref eachLocaleState) do on loc {

    // Local variables
    var eachTaskState: [0..#here.maxTaskPar] atomic bool = BUSY;

    // Local statistics
    var eachLocalExploredTree: [0..#here.maxTaskPar] int;
    var eachLocalExploredSol: [0..#here.maxTaskPar] int;

    coforall taskId in 0..#here.maxTaskPar with (ref eachLocalExploredTree,
      ref eachLocalExploredSol, ref eachTaskState, ref eachLocaleState) {

      // Task variables
      ref tree_loc = eachLocalExploredTree[taskId];
      ref num_sol = eachLocalExploredSol[taskId];
      ref curTaskState = eachTaskState[taskId];
      ref curLocState = eachLocaleState[here.id];

      allLocalesBarrier.barrier();

      while true {

        // Try to get an element
        var (hasWork, parent) = bag.remove(taskId);

        // Check termination
        if hasWork {
          curTaskState.write(BUSY);
          curLocState.write(BUSY);
        }
        else {
          curTaskState.write(IDLE);
          if allIdle(eachTaskState) {
            curLocState.write(IDLE);
            if allIdle(eachLocaleState) && bag.getSize() == 0 {
              break; // end of the global search
            }
          }
          continue;
        }

        // Decompose an element
        var children = decompose(parent, N, tree_loc, num_sol);
        bag.addBulk(children, taskId);
      }
    }

    eachExploredTree[here.id] += (+ reduce eachLocalExploredTree);
    eachExploredSol[here.id] += (+ reduce eachLocalExploredSol);
  }

  const treeSize = (+ reduce eachExploredTree);
  const solCount = (+ reduce eachExploredSol);

  return (treeSize, solCount);
}


/*** Tests ***/

// Checks results
proc check_results(const N: int, const treeSize: int, const solCount: int)
{
  // Pre-computed solution count of the N-Queens instances (see above).
  const solutionCount: [1..18] int
    = [1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200, 73712, 365596,
      2279184, 14722512, 95815104, 666090624];

  if (solCount == solutionCount[N]) then return true;
  else return false;
}

proc main()
{
  var res = true;

  for N in 1..maxN do {
    var (treeSize, solCount) = nqueens_search(N);

    if !check_results(N, treeSize, solCount) {
      res = false;
      writeln(N, "-queens failed");
    }
  }

  if res then writeln("All tests passed");
  else writeln("\nSome tests failed...");
}
