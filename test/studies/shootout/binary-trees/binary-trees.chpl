//Binary-Tree Shootout
//http://benchmarksgame.alioth.debian.org
//Casey Battaglino

use DynamicIters;

config const n = 10;

class Tree {
  const left, right: owned Tree?;
}

proc main() {
  const minDepth = 4, maxDepth = max(minDepth + 2, n);
  {
    const stretchDepth = maxDepth + 1;
    const stretchTree = bottomUpTree(stretchDepth);
    const check = itemCheck(stretchTree);
    // stretchTree is destroyed here
    writeln("stretch tree of depth ",stretchDepth,"\t check: ",check);
  }

  var results: [1..maxDepth, 1..2] int;
  const longLivedTree : owned Tree = bottomUpTree(maxDepth);

  forall depth in dynamic(minDepth..maxDepth by 2, 1) {
    const iterations: int = 1 << (maxDepth - depth + minDepth);
    var check: int = 0;
			
    for i in 1..iterations {
      const t = bottomUpTree(depth);
      check += itemCheck(t);
    }
    results[depth,1] = iterations;
    results[depth,2] = check;
  }

  for depth in minDepth..maxDepth by 2 {
    writeln(results[depth,1],"\t trees of depth ",depth,"\t check: ",results[depth,2]);
  }

  writeln("long lived tree of depth ",maxDepth,"\t check: ",itemCheck(longLivedTree));
}

proc bottomUpTree(const depth: int): owned Tree {
  if depth <= 0 then return new owned Tree();
  else return new owned Tree(bottomUpTree(depth-1), bottomUpTree(depth-1));
}

proc itemCheck(const T: borrowed Tree): int {
  if (T.left==nil) then return 1; 
  else return (1 + itemCheck(T.left!) + itemCheck(T.right!));
}
