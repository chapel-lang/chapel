/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Jade Abraham
   based on the Chapel #3 version by Casey Battaglino, Ben Harshbarger, and
     Brad Chamberlain
*/

use Allocators;

config const n = 10;                         // the maximum tree depth

proc main() {
  const minDepth = 4,                        // the shallowest tree
        maxDepth = max(minDepth + 2, n),     // the deepest normal tree
        strDepth = maxDepth + 1,             // the depth of the "stretch" tree
        depths = minDepth..maxDepth by 2,    // the range of depths to create
        nodeSize = 24;                       // the size of a node
  var stats: [depths] (int,int);             // stores statistics for the trees

  inline proc poolSize(depth, num=1) do return num*2**(depth+1)*nodeSize;

  //
  // Create the short-lived "stretch" tree, checksum it, and print its stats.
  //
  {
    const pool = new bumpPtrMemPool(poolSize(strDepth), alignment=0),
          strTree = newWithAllocator(pool, unmanaged Tree, strDepth, pool);
    writeln("stretch tree of depth ", strDepth, "\t check: ", strTree.sum());
  }

  //
  // Build the long-lived tree.
  //
  const pool = new bumpPtrMemPool(poolSize(maxDepth), alignment=0),
        llTree = newWithAllocator(pool, unmanaged Tree, maxDepth, pool);

  //
  // Iterate over the depths. At each depth, create the required trees in
  // parallel, compute their sums, and free them.
  //
  for depth in depths {
    const iterations = 2**(maxDepth - depth + minDepth),
          ps = poolSize(depth, iterations/here.maxTaskPar);
    var sum = 0;

    forall 1..iterations
      with (+ reduce sum,
            var pool = new bumpPtrMemPool(ps, alignment=0)) {
      const t = newWithAllocator(pool, unmanaged Tree, depth, pool);
      sum += t.sum();
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
}


//
// A simple balanced tree node class
//
class Tree {
  var left, right: unmanaged Tree?;

  //
  // A Tree-building initializer
  //
  proc init(depth, pool) {
    if depth > 0 {
      const d = depth - 1;
      left  = newWithAllocator(pool, unmanaged Tree, d, pool);
      right = newWithAllocator(pool, unmanaged Tree, d, pool);
    }
  }

  //
  // Add up tree node, freeing as we go
  //
  proc sum(): int {
    var sum = 1;
    if left {
      sum += left!.sum() + right!.sum();
    }
    return sum;
  }
}
