use Math;
config const DEPTH: int = 10;
config const PAR_DEPTH: int = 2;

type data_t = (int, int);

class BinaryTree {
  type elem_t;
  var data: elem_t;
  
  var left:  BinaryTree;
  var right: BinaryTree;
}

// Parallel tree traversal
def dfs_count(n: BinaryTree, d: int = 0):int {
  if n != nil {
    var nleft, nright:int;
    serial d > PAR_DEPTH cobegin {
      nleft  = dfs_count(n.left, d+1);
      nright = dfs_count(n.right, d+1);
    }
    return 1 + nleft + nright;
  } else {
    return 0;
  }
}

// Parallel tree creation
def create_tree(lvl: int, idx: int, to_depth: int, parent: BinaryTree) {
  if (lvl < to_depth) {
    parent.left  = BinaryTree(data_t, (lvl, idx));
    parent.right = BinaryTree(data_t, (lvl, idx+1));

    // Recurse in parallel
    serial lvl > PAR_DEPTH cobegin {
      create_tree(lvl+1, idx*2, to_depth, parent.left);
      create_tree(lvl+1, (idx+1)*2, to_depth, parent.right);
    }
  }
} 



def main {
  var count, expected: int;
  var root = BinaryTree(data_t, (0, 0));

  writeln("Performing parallel tree creation..");
  create_tree(0, 0, DEPTH, root);
  writeln("Performing parallel tree traversal..");
  count = dfs_count(root);

  for i in [0..DEPTH] do
    expected += exp2(i);
  
  if count == expected then
    writeln("Success: Counted ", count, " nodes");
  else
    writeln("Failed: Counted ", count, " nodes, expected ", expected);
}
