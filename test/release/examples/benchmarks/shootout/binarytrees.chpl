/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Casey Battaglino
   modified by Ben Harshbarger and Brad Chamberlain
*/

//
// Use this helper module to get the dynamic iterator
//
use AdvancedIters;

//
// The problem size determines the maximum depth of the trees created
//
config const n = 10;

//
// A simple tree class
//
class Tree {
  const item: int;
  const left, right: Tree;
}


proc main() {
  const minDepth = 4,                      // the shallowest tree
        maxDepth = max(minDepth + 2, n),   // the deepest tree
        depths = minDepth..maxDepth by 2;  // the range of depths to create

  //
  // Create the "stretch" tree, checksum it, print its stats, and free it
  //
  const stretchDepth = maxDepth + 1;
  const stretchTree = buildTree(0, stretchDepth);
  writeln("stretch tree of depth ", stretchDepth, "\t check: ", 
          checksum(stretchTree));
  free(stretchTree);

  //
  // Build our long-lived tree
  //
  const longLivedTree = buildTree(0, maxDepth);

  //
  // Declare an array for storing stats for the trees
  //
  var results: [depths] 2*int;

  //
  // Iterate over the depths we're exploring, dynamically mapping out a
  // single iteration per task at a time
  //
  forall depth in dynamic(depths, chunkSize=1) {
    const iterations = 1 << (maxDepth - depth + minDepth);
    var check = 0;
			
    for i in 1..iterations {
      const posT = buildTree( i, depth), 
            negT = buildTree(-i, depth);
      check += checksum(posT) + checksum(negT);
      free(posT);
      free(negT);
    }
    results[depth] = (2*iterations, check);
  }

  //
  // Print out the results for the trees of varying depths
  //
  for depth in depths do
    writeln(results[depth](1), "\t trees of depth ", depth, "\t check: ", 
            results[depth](2));

  //
  // Checksum the long-lived tree, print its results, and free it
  //
  writeln("long lived tree of depth ", maxDepth, "\t check: ",
          checksum(longLivedTree));
  free(longLivedTree);
}

//
// Build a tree
//
proc buildTree(item, depth): Tree {
  if depth <= 0 then
    return new Tree(item);
  else 
    return new Tree(item, buildTree(2*item-1, depth-1), 
                          buildTree(2*item  , depth-1));
}

//
// Compute a simple checksum on a tree
//
proc checksum(T): int {
  if (T.left == nil) then
    return T.item; 
  else 
    return (T.item + checksum(T.left) - checksum(T.right));
}

//
// Recursively free a tree
//
proc free(T) {
  if (T.left) {
    free(T.left);
    free(T.right);
  }
  delete T;
}
