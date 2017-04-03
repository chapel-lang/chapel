/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Casey Battaglino, Ben Harshbarger, and Brad Chamberlain
   derived from the GNU C version by Jeremy Zerfas
*/


use DynamicIters;

config const n = 10;         // the maximum tree depth

proc main() {
  const minDepth = 4,                      // the shallowest tree
        maxDepth = max(minDepth + 2, n),   // the deepest normal tree
        strDepth = maxDepth + 1,           // the depth of the "stretch" tree
        depths = minDepth..maxDepth by 2;  // the range of depths to create
  var stats: [depths] (int,int);           // stores statistics for the trees

  //
  // Create the "stretch" tree, checksum it, print its stats, and free it.
  //
  const strTree = new Tree(strDepth);
  writeln("stretch tree of depth ", strDepth, "\t check: ", strTree.sum());
  delete strTree;

  //
  // Build the long-lived tree.
  //
  const llTree = new Tree(maxDepth);

  //
  // Iterate over the depths in parallel, dynamically assigning them
  // to tasks.  At each depth, create the required trees, compute
  // their sums, and free them.
  //
  forall depth in dynamic(depths, chunkSize=1) {
    const iterations = 2**(maxDepth - depth + minDepth);
    var sum = 0;
			
    for i in 1..iterations {
      const t = new Tree(depth);
      sum += t.sum();
      delete t;
    }
    stats[depth] = (iterations, sum);
  }

  //
  // Print out the stats for the trees of varying depths.
  //
  for depth in depths do
    writeln(stats[depth](1), "\t trees of depth ", depth, "\t check: ",
            stats[depth](2));

  //
  // Checksum the long-lived tree, print its stats, and free it.
  //
  writeln("long lived tree of depth ", maxDepth, "\t check: ", llTree.sum());
  delete llTree;
}


//
// A simple balanced tree node class
//
class Tree {
  var left, right: Tree;

  //
  // A Tree-building initializer
  //
  proc init(depth) {
    if depth > 0 {
      left  = new Tree(depth-1);
      right = new Tree(depth-1);
    }
  }

  //
  // Add up tree node, freeing as we go
  //
  proc sum(): int {
    var sum = 1;
    if left {
      sum += left.sum() + right.sum();
      delete left;
      delete right;
    }
    return sum;
  }
}
