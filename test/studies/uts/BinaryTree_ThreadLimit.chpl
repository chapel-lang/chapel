use Math;

type data_t = (int, int);

config const DEPTH: int = 15;
config const PAR_DEPTH: int = 2;
config const MAX_THREADS = 4;

// Global thread counter
var thread_cnt: sync int = 0;


class BinaryTree {
  type elem_t;
  var data: elem_t;
  
  var left:  BinaryTree;
  var right: BinaryTree;
}

    
// Parallel tree traversal
def dfs_count(n: BinaryTree, d: int = 0):int {
  var isParallel = false;
  var count = 0;
  
  if (n != nil) {
    var nleft, nright:int;

    // Trade some imbalance here for blocking overhead
    if (readXX(thread_cnt) < MAX_THREADS) {
      var thread_cnt_l = thread_cnt;

      // Try to get a ticket to run in parallel
      if (thread_cnt_l < MAX_THREADS) {
        isParallel = true;
        thread_cnt = thread_cnt_l + 2;
        writeln("Threads: ", thread_cnt_l+2);
      } else {
        thread_cnt = thread_cnt_l;
      }
    }
    
    // Parallel Depth-First Traversal
    serial (!isParallel) cobegin {
      nleft  = dfs_count(n.left, d+1);
      nright = dfs_count(n.right, d+1);
    }

    count = 1 + nleft + nright;
  } else {
    count = 0;
  }

  if (isParallel) then thread_cnt -= 1;
  return count;
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
    expected += exp2(i):int;
  
  if count == expected then
    writeln("Success: Counted ", count, " nodes");
  else
    writeln("Failed: Counted ", count, " nodes, expected ", expected);
}
