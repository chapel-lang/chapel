//Binary-Tree Shootout
//http://benchmarksgame.alioth.debian.org

config const n = 10;

class Tree {
  var left, right: unmanaged Tree?;
}

proc main() {
  const minDepth = 4, maxDepth = max(minDepth + 2, n);
  const stretchDepth: int = maxDepth + 1;
  var stretchTree = bottomUpTree(stretchDepth);
  var check: int = itemCheck(stretchTree);
  free_iter(stretchTree);

  writeln("stretch tree of depth ",stretchDepth,"\t check: ",check);

  var results: [1..maxDepth, 1..2] int;
  var longLivedTree : unmanaged Tree = bottomUpTree(maxDepth);

  forall depth in minDepth..maxDepth by 2 {
    var iterations: int = 1 << (maxDepth - depth + minDepth);
    var check: int = 0;
			
    for i in 1..iterations {
      var t = bottomUpTree(depth);
      check += itemCheck(t);
      free_iter(t);
    }
    results[depth,1] = iterations;
    results[depth,2] = check;
  }

  for depth in minDepth..maxDepth by 2 {
    writeln(results[depth,1],"\t trees of depth ",depth,"\t check: ",results[depth,2]);
  }

  writeln("long lived tree of depth ",maxDepth,"\t check: ",itemCheck(longLivedTree));

  free_iter(longLivedTree);
}

proc bottomUpTree(depth: int): unmanaged Tree {
  var T = new unmanaged Tree();
  if depth > 0 {
    T.left  = bottomUpTree(depth-1);
    T.right = bottomUpTree(depth-1);
  }
  return T;
}

proc itemCheck(T: borrowed Tree): int{
  if (T.left==nil) then return 1;
  else return 1 + itemCheck(T.left!) + itemCheck(T.right!);
}

// Feel free to turn this into a parallel leader-follower
// for additional testing goodness
iter postorder(tree: unmanaged Tree?): unmanaged Tree {
  if tree != nil {
    for child in postorder(tree!.left) do yield child;
    for child in postorder(tree!.right) do yield child;
    yield tree!;
  }
}

proc free_iter(T: unmanaged Tree) {
  for node in postorder(T) { delete node; }
}
