//Binary-Tree Shootout
//http://shootout.alioth.debian.org
//Casey Battaglino

config const n = 10;

class Tree {
  var item: int;
  var left, right: Tree;
}

proc main() {
  const minDepth = 4, maxDepth = max(minDepth + 2, n);
  const stretchDepth = maxDepth + 1;
  var stretchTree = bottomUpTree(0,stretchDepth);
  var check = itemCheck(stretchTree);
  free(stretchTree);

  writeln("stretch tree of depth ",stretchDepth,"\t check: ",check);

  var results: [1..maxDepth, 1..2] int;
  var longLivedTree : Tree = bottomUpTree(0, maxDepth);

  forall depth in minDepth..maxDepth by 2 {
    var iterations: int = 1 << (maxDepth - depth + minDepth);
    var check: int = 0;
			
    for i in 1..iterations {
      var posT = bottomUpTree(i,depth), negT = bottomUpTree(-i,depth);
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

proc bottomUpTree(item: int, depth: int): Tree {
  var T = new Tree(item);
  if depth > 0 {
    T.left  = bottomUpTree(2*item-1, depth-1);
    T.right = bottomUpTree(2*item, depth-1);
  }
  return T;
}

proc itemCheck(T: Tree): int {
  if (T.left==nil) then return T.item; 
  else return (T.item + itemCheck(T.left) - itemCheck(T.right));
}

proc free(T: Tree) {
  if (T.left!=nil) {
    free(T.left);
    free(T.right);
  }
  delete T;
}
