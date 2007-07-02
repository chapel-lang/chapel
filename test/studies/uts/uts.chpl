use Math;

config const DEPTH:        int = 10;
config const PAR_DEPTH:    int = 2;

config const SEED:        uint = 73;  // Seed for the hash of the root node
config const MAX_CHILDREN: int = 8;
config const B_FACTOR:     int = 10;
config const DEPTH_LIMIT:  int = 10;


class TreeNode {
  var depth: int;
  var hash: uint;

  // By default, children will be empty since it has range [1..0]
  var nChildren: int = 0;
  var childDom = [1..nChildren];
  var children:  [childDom] TreeNode;

  // Generate this node's children
  def genChildren(): int {
    // How many children I have depends on my hash
    var p = lcg_prob(hash);

    // Number of nodes is equally random at all nodes down to the depth limit
    nChildren = if depth < DEPTH_LIMIT
                   then (p*MAX_CHILDREN):int
                   else 0;

    childDom  = [1..nChildren];

    forall i in childDom {
      var child_hash = rng_spawn(hash, i);
      children[i]    = TreeNode(depth+1, child_hash);
    }

    return nChildren;
  }
    
}

// Spawn the i'th child's hash given v, the parent's hash
// Parallel Note: Either all the spawns must be done by the same
//   processor, or i must be shipped along with the child
def rng_spawn(v: uint, i: int): uint {
  return lcg_rng(v + i:uint);
}

// Linear Congruential pseudoRandom Number Generator
def lcg_rng(v: uint): uint {
  var v_n: uint(64) = v;

  v_n *= 279470273:uint(64);
  v_n %= 4294967291:uint(64);
  
  return v_n:uint(32);
}

// Convert random number to a probability
def lcg_prob(v: uint): real {
  return v:real/max(uint);
}


// Parallel tree traversal
def dfs_count(n: TreeNode):int {
  if n != nil {
    var count: int;
    forall i in n.childDom {
      count += dfs_count(n.children[i]);
    }
    return 1 + count;
  } else {
    return 0;
  }
}


// Parallel tree creation
def create_tree(parent: TreeNode): int {
  var count: int;

  if parent == nil {
    writeln("create_tree(): Warning, called with nil");
    return 0;
  }

  count += parent.genChildren();

  forall i in parent.childDom {
    count += create_tree(parent.children[i]);
  }

  return count;
} 


def main {
  var counted, created: int;
  var root = TreeNode(0, lcg_rng(SEED));

  created = create_tree(root);
  counted = dfs_count(root);

  writeln("Created = ", created, " Counted = ", counted);
}
