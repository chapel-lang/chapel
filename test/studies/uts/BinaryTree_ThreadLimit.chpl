use Math;
use Time;

type data_t = (int, int);

config const DEPTH: int = 15;
config const MAX_THREADS = 4;

// Global thread counter
var thread_cnt: sync int = 0;


class BinaryTree {
  type elem_t;
  var data: elem_t;
  
  var left:  BinaryTree;
  var right: BinaryTree;
}


def threadsAvailable(n: int): bool {
    // Trade some imbalance here for blocking overhead
    if (readXX(thread_cnt) < MAX_THREADS) {
      var thread_cnt_l = thread_cnt;

      // Try to get a ticket to run in parallel
      if (thread_cnt_l < MAX_THREADS) {
        thread_cnt = thread_cnt_l + n;
        //writeln("Threads: ", thread_cnt_l+2);
        return true;
      } else {
        thread_cnt = thread_cnt_l;
      }
    }

    return false;
}

    
// Parallel tree traversal
def dfs_count(n: BinaryTree, d: int = 0):int {
  var isParallel = false;
  var count = 0;
  
  if (n != nil) {
    var nleft, nright:int;

    isParallel = threadsAvailable(2);

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
  var isParallel = false;

  if (lvl < to_depth) {
    parent.left  = BinaryTree(data_t, (lvl, idx));
    parent.right = BinaryTree(data_t, (lvl, idx+1));

    isParallel = threadsAvailable(2);

    // Recurse in parallel
    serial !isParallel cobegin {
      create_tree(lvl+1, idx*2, to_depth, parent.left);
      create_tree(lvl+1, (idx+1)*2, to_depth, parent.right);
    }
    
    if (isParallel) then thread_cnt -= 1;
  }
} 



def main {
  var t_create: Timer();
  var t_dfs   : Timer();
  var count, expected: int;
  var root = BinaryTree(data_t, (0, 0));

  writeln("Parallel Binary Tree Creation/Traversal: Thread Limit = ", MAX_THREADS);

  writeln("Performing parallel tree creation..");
  t_create.start();
  create_tree(0, 0, DEPTH, root);
  t_create.stop();

  // Reset thread counter
  var old_threads = thread_cnt;
  thread_cnt = 0;

  writeln("Performing parallel tree traversal..");
  t_dfs.start();
  count = dfs_count(root);
  t_dfs.stop();

  for i in [0..DEPTH] do
    expected += exp2(i):int;
  
  writeln("t_create= ", t_create.elapsed(), " t_dfs = ", t_dfs.elapsed());

  if count == expected then
    writeln("Success: Counted ", count, " nodes");
  else
    writeln("Failed: Counted ", count, " nodes, expected ", expected);
}
