// Copy of binary trees which uses our new initializers story, to validate
// that initializers can be called recursively (since you already know their
// return type)

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
  const strTree = new unmanaged Tree(0, strDepth);
  writeln("stretch tree of depth ", strDepth, "\t check: ", strTree.sum());
  delete strTree;

  //
  // Build the long-lived tree.
  //
  const llTree = new unmanaged Tree(0, maxDepth);

  //
  // Iterate over the depths in parallel, dynamically assigning them
  // to tasks.  At each depth, create the required trees, compute
  // their sums, and free them.
  //
  forall depth in dynamic(depths, chunkSize=1) {
    const iterations = 1 << (maxDepth - depth + minDepth);
    var sum = 0;
			
    for i in 1..iterations {
      const posT = new unmanaged Tree( i, depth), 
            negT = new unmanaged Tree(-i, depth);
      sum += posT.sum() + negT.sum();
      delete posT;
      delete negT;
    }
    stats[depth] = (2*iterations, sum);
  }

  //
  // Print out the stats for the trees of varying depths.
  //
  for depth in depths do
    writeln(stats[depth](0), "\t trees of depth ", depth, "\t check: ",
            stats[depth](1));

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
  const item: int;
  const left, right: unmanaged Tree?;

  proc init(item, left, right) { // Leaves the type of left and right ambiguous
    this.item = item;
    this.left = left;
    this.right = right;
  }

  proc init(item, depth) {
    if depth <= 0 then
      this.init(item, nil, nil);
    else
      this.init(item, new unmanaged Tree(2*item-1, depth-1),
                new unmanaged Tree(2*item  , depth-1));
  }

  //
  // Add up tree node, freeing as we go
  //
  proc sum(): int {
    var sum = item;
    if left {
      sum += left!.sum() - right!.sum();
      delete left;
      delete right;
    }
    return sum;
  }
}
