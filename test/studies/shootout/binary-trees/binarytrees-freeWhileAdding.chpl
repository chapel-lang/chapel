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
  const strTree = Tree.build(strDepth);
  writeln("stretch tree of depth ", strDepth, "\t check: ", strTree.sum());
  delete strTree;

  //
  // Build the long-lived tree.
  //
  const llTree = Tree.build(maxDepth);

  //
  // Iterate over the depths in parallel, dynamically assigning them
  // to tasks.  At each depth, create the required trees, compute
  // their sums, and free them.
  //
  forall depth in dynamic(depths, chunkSize=1) {
    const iterations = 1 << (maxDepth - depth + minDepth);
    var sum = 0;
			
    for i in 1..iterations {
      const t = Tree.build(depth);
      sum += t.sum();
      delete t;
    }
    stats[depth] = (iterations, sum);
  }

  //
  // Print out the stats for the trees of varying depths.
  //
  for (depth, (numTrees, checksum)) in zip(depths, stats) do
    writeln(numTrees, "\t trees of depth ", depth, "\t check: ", checksum);

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
  const left, right: unmanaged Tree?;

  proc type build(depth): unmanaged Tree {
    if depth <= 0 then
      return new unmanaged Tree();
    else
      return new unmanaged Tree(Tree.build(depth-1),
                                Tree.build(depth-1));
  }

  //
  // Add up tree node, freeing as we go
  //
  proc sum(): int {
    var sum = 1;
    if left {
      sum += left!.sum() + right!.sum();
      delete left;
      delete right;
    }
    return sum;
  }
}
