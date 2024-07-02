/* The Computer Language Benchmarks Game
   https://salsa.debian.org/benchmarksgame-team/benchmarksgame/

   contributed by Casey Battaglino, Ben Harshbarger, Brad Chamberlain, and Jade Abraham
   derived from the GNU C version by Jeremy Zerfas
*/


use DynamicIters;
use Allocators;

config const n = 10;         // the maximum tree depth
config const globalPoolSize = 2 ** 32;
config const localPoolSize = 2 ** 32;

var globalPool = new bumpPtrMemPool(globalPoolSize, parSafe=false);

proc main() {
  const minDepth = 4,                      // the shallowest tree
        maxDepth = max(minDepth + 2, n),   // the deepest normal tree
        strDepth = maxDepth + 1,           // the depth of the "stretch" tree
        depths = minDepth..maxDepth by 2;  // the range of depths to create
  var stats: [depths] (int,int);           // stores statistics for the trees

  //
  // Create the short-lived "stretch" tree, checksum it, and print its stats.
  //
  {
    const strTree = __primitive("new with allocator", globalPool, unmanaged Tree, strDepth, globalPool);
    writeln("stretch tree of depth ", strDepth, "\t check: ", strTree.sum());
  }

  //
  // Build the long-lived tree.
  //
  const llTree = __primitive("new with allocator", globalPool, unmanaged Tree, maxDepth, globalPool);

  //
  // Iterate over the depths. At each depth, create the required trees in
  // parallel, compute their sums, and free them.
  //
  for depth in depths {
    const iterations = 2**(maxDepth - depth + minDepth);
    var sum = 0;

    forall i in 1..iterations
      with (+ reduce sum,
            var localPool = new bumpPtrMemPool(localPoolSize, parSafe=false)) {
      const t = __primitive("new with allocator", localPool, unmanaged Tree, depth, localPool);
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
  proc init(depth, ref pool) {
    if depth > 0 {
      const d = depth - 1;
      left  = __primitive("new with allocator", pool, unmanaged Tree, d, pool);
      right = __primitive("new with allocator", pool, unmanaged Tree, d, pool);
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
