/*
**  UTS -- The Unbalanced Tree Search
**
**  James Dinan <dinan@cray.com>
**  July, 2007
*/
 
use Math;
use Time;
use lcg_rng;


/**** UTS TYPES ****/
enum NodeDistrib { Binomial, Geometric, Constant };
enum GeoDistrib  { GeoFixed, GeoLinear, GeoPoly, GeoCyclic };


/**** COMPILE PARAMS ****/
param debug: bool = false;


/**** UTS CONFIG VARS ****/
config const SEED:        uint = 73;    // Seed for the hash of the root node
config const MAX_CHILDREN: int = 2;     // Max. Children a node may have
config const B_0:          int = 2;     // Branching Factor
config const MAX_DEPTH:    int = 2;     // Depth limit
config const nonLeafProb: real = 0.01;  // Probability of a non-leaf (binomial)
config const nonLeafBF:    int = 2;     // Non-leaf branching factor (binomial)

config const distrib: NodeDistrib = Geometric;
config const geoDist: GeoDistrib  = GeoFixed;


class TreeNode {
  var depth:   int;
  var hash:    RNG_state;

  // By default, children will be empty since it has range [1..0]
  var nChildren: int = 0;
  var childDom = [1..nChildren];
  var children:  [childDom] TreeNode;


  // Generate this node's children
  def genChildren(): int {
    select distrib {
      when Geometric do 
        nChildren = numGeoChildren(geoDist);
      when Binomial do
        nChildren = numBinChildren();
      when Constant do
        nChildren = numConstChildren();
    }

    if debug then writeln("Constructing ", nChildren, " children: ", childDom);

    childDom  = [1..nChildren];

    forall i in childDom {
      var child_hash: RNG_state;

      if debug then writeln("  + (", depth, ", ", i, ")");
      rng_spawn(hash, child_hash, i);
      children[i]    = TreeNode(depth+1, child_hash);
    }

    return nChildren;
  }


  // Constant Distribution: Find the number of children
  //  Note: This is a _balanced_ tree.
  def numConstChildren(): int {
    return if depth < MAX_DEPTH then B_0 else 0;
  }


  // Binomial Distribution: Find the number of children
  //  Note: distribution is identical everywhere below root
  def numBinChildren(): int {
    var d: real = to_prob(rng_rand(hash));
    var numChildren = if d < nonLeafProb then nonLeafBF else 0;

    return numChildren;
  }


  // Geometric Distribution: Find the number of children
  def numGeoChildren(shape: GeoDistrib): int {
    var b_i: real = B_0;
  
    // use shape function to compute target b_i
    if (depth > 0) {
      select shape {
      
        // Expected size is polynomial in depth
        when GeoPoly do
          b_i = B_0 * (depth:real ** (-log(B_0:real)/log(MAX_DEPTH:real)));
        
        // Expected size is cyclic
        when GeoCyclic {
          if (depth > 5 * MAX_DEPTH) then
            b_i = 0.0;
          else
            b_i = B_0 ** sin(2.0*3.141592653589793*depth:real / MAX_DEPTH:real);
        }

        // Expected size is the same at all nodes up to max depth
        when GeoFixed do
          b_i = if (depth < MAX_DEPTH) then B_0 else 0;
 
        // Expected size decreases linearly in b_i
        when GeoLinear do 
          b_i =  B_0 * (1.0 - depth:real / MAX_DEPTH:real);
        
      }
    }

    // given target b_i, find prob p so expected value of 
    // geometric distribution is b_i.
    var p: real = 1.0 / (1.0 + b_i);

    // get uniform random number on [0,1)
    var u: real = to_prob(rng_rand(hash));

    // max number of children at this cumulative probability
    // (from inverse geometric cumulative density function)
    return floor(log(1 - u) / log(1 - p)); 
  }
}


def showSearchParams() {
  if distrib == Geometric then
    writeln("Tree is: Geometric, ", geoDist);
  else
    writeln("Tree is: ", distrib);

  writeln("  SEED=", SEED, " B_0=", B_0, " MAX_DEPTH=", MAX_DEPTH,
    " nonLeafProb=", nonLeafProb, " nonLeafBF=", nonLeafBF);
  
}

def dfs_count(n: TreeNode):int {
  if n != nil {
    var count: int;
    if debug then writeln("  - ", n.depth);
    forall i in n.childDom {
      count += dfs_count(n.children[i]);
    }
    return 1 + count;
  } else {
    return 0;
  }
}


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
  var t_create: Timer();
  var t_dfs   : Timer();
  var counted, created: int;
  var root: TreeNode;
  var hash: RNG_state = RNG_state();
  
  rng_init(hash, SEED);
  root = TreeNode(0, hash, Constant);

  showSearchParams();

  t_create.start();
  created = create_tree(root);
  t_create.stop();

  t_dfs.start();
  counted = dfs_count(root);
  t_dfs.stop();

  writeln("Created = ", created, " Counted = ", counted);
  writeln("t_create= ", t_create.elapsed(), " t_dfs = ", t_dfs.elapsed());
}
