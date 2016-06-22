//Binary-Tree Shootout
//http://benchmarksgame.alioth.debian.org
//Casey Battaglino

use DynamicIters;

config const n = 10;

class Tree {
  const item: int;
  const left, right: Tree;
}

proc main() {
  const minDepth = 4, maxDepth = max(minDepth + 2, n);
  const stretchDepth = maxDepth + 1;
  const stretchTree = bottomUpTree(0,stretchDepth);
  const check = itemCheck(stretchTree);
  free(stretchTree);

  writeln("stretch tree of depth ",stretchDepth,"\t check: ",check);

  var results: [1..maxDepth, 1..2] int;
  const longLivedTree : Tree = bottomUpTree(0, maxDepth);

  forall depth in dynamic(minDepth..maxDepth by 2, 1) {
    const iterations: int = 1 << (maxDepth - depth + minDepth);
    var check: int = 0;
			
    for i in 1..iterations {
      const posT = bottomUpTree(i,depth), negT = bottomUpTree(-i,depth);
      check += itemCheck(posT) + itemCheck(negT);
      free(posT);
      free(negT);
    }
    results[depth,1] = iterations*2;
    results[depth,2] = check;
  }

  for depth in minDepth..maxDepth by 2 {
    writeln(results[depth,1],"\t trees of depth ",depth,"\t check: ",results[depth,2]);
  }

  writeln("long lived tree of depth ",maxDepth,"\t check: ",itemCheck(longLivedTree));

  free(longLivedTree);
}

proc bottomUpTree(const item: int, const depth: int): Tree {
  if depth <= 0 then return new Tree(item);
  else return new Tree(item, bottomUpTree(2*item-1, depth-1), bottomUpTree(2*item, depth-1));
}

proc itemCheck(const T: Tree): int {
  if (T.left==nil) then return T.item; 
  else return (T.item + itemCheck(T.left) - itemCheck(T.right));
}

proc free(const T: Tree) {
  if (T.left!=nil) {
    free(T.left);
    free(T.right);
  }
  delete T;
}
