/*
**  UTS -- The Unbalanced Tree Search
**
**  James Dinan <dinan@cray.com>
**  July, 2007
*/
 
use Math;
use Time;
use sha1_rng;
use dequeue;


/**** UTS TYPES ****/
enum NodeDistrib { Binomial, Geometric, Hybrid, Constant };
enum GeoDistrib  { GeoFixed, GeoLinear, GeoPoly, GeoCyclic };


/**** COMPILE PARAMS ****/
config param debug:    bool = false;
config param parallel: bool = true;
param uts_version:   string = "2.1";


/**** UTS CONFIG VARS ****/
config const SEED:        uint = 16;    // Seed for the hash of the root node
config const MAX_CHILDREN: int = 10;    // Max. Children a node may have
config const B_0:          int = 4;     // Branching Factor
config const MAX_DEPTH:    int = 6;     // Depth limit
config const nonLeafProb: real = 0.10;  // Probability of a non-leaf (binomial)
config const nonLeafBF:    int = 4;     // Non-leaf branching factor (binomial)
config const shiftDepth:  real = 0.5;   // Depth at which hybrid trees go from Geo=>Bin

config const distrib: NodeDistrib = Geometric;
config const geoDist: GeoDistrib  = GeoFixed;

config const MAX_THREADS:  int = 20;
config const MIN_THREADS:  int = MAX_THREADS/4;

// Global thread counter
var thread_cnt: sync int = 0;
var threads_spawned: int = 0; // "Locked" by thread_cnt


/**** UTS TreeNode Class ****/
class TreeNode {
  var depth:   int;
  var hash:    string = "__Here_be_dragons__";  // Handle delicately, if this is ever copied, things will go off the rails

  var nChildren: int = 0;

  // Generate this node's children
  def genChildren(inout q: DeQueue(TreeNode)): int {
    select distrib {
      when Geometric do 
        nChildren = numGeoChildren(geoDist);
      when Binomial do
        nChildren = numBinChildren();
      when Hybrid do
        nChildren = numHybridChildren();
      when Constant do
        nChildren = numConstChildren();
    }

    if debug then writeln("Constructing ", nChildren, " children");

    forall i in [0..nChildren-1] {
      if debug then writeln("  + (", depth, ", ", i, ")");
      var t = TreeNode(depth+1);
      rng_spawn(hash, t.hash, i);
      q.pushTop(t);
    }

    return nChildren;
  }


  // Constant Distribution: Find the number of children
  //  Note: This is a _balanced_ tree.
  def numConstChildren(): int {
    return if depth < MAX_DEPTH then B_0 else 0;
  }


  // Hybrid trees are geometric at the top of the tree and binomial
  // below a certain depth.
  def numHybridChildren(): int {
    if (depth < shiftDepth * MAX_DEPTH) then
      return numGeoChildren(geoDist);
    
    else 
      return numBinChildren();
  }

  // Binomial Distribution: Find the number of children
  //  Note: distribution is identical everywhere below root
  def numBinChildren(): int {
    if (depth == 0) then
      return B_0;
    
    else
      return if to_prob(rng_rand(hash)) < nonLeafProb then nonLeafBF else 0;
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


/*
** Print out search parameters
*/
def uts_showSearchParams() {
  writeln("UTS v", uts_version, " - Unbalanced Tree Search (", 
      if parallel then "Parallel Chapel (DeQueue)" else "Sequential Chapel (DeQueue)", ")");

  writeln("Tree type: ", distrib);
  writeln("Tree shape parameters:");
  writeln("  root branching factor b_0 = ", B_0, ", root seed = ", SEED);
    
  if (distrib == Geometric || distrib == Hybrid) then
    writeln("  Geo. params: Max_Depth = ", MAX_DEPTH, ", shape function = ", geoDist);

  if (distrib == Binomial || distrib == Hybrid) then
    writeln("  Bin. params: nonLeafProb=", nonLeafProb, " nonLeafBF=", nonLeafBF);

  if (distrib == Hybrid) then
    writeln("  Hybrid params: shiftDepth=", shiftDepth);

  writeln("Random number generator: ", rng_getName());

  if (parallel) {
    writeln("Parallel execution parameters:");
    writeln("  MAX_THREADS = ", MAX_THREADS, ", MIN_THREADS = ", MIN_THREADS);
  }

  writeln();
}


/*
**  Parallel Tree Creation
*/
def create_tree(inout q: DeQueue(TreeNode)): int {
  var count: int;

  if q.isEmpty() {
    writeln("create_tree(): Warning, called with no work");
    return 0;
  }

  while !q.isEmpty() {
    var node = q.popTop();
    count += node.genChildren(q);
  }

  return count;
} 


def main {
  var t_create: Timer();
  var created: int;
  var root: TreeNode;
  var queue: DeQueue(TreeNode);
  
  root = TreeNode(0);
  rng_init(root.hash, SEED:int);
  queue.pushTop(root);

  uts_showSearchParams();

  t_create.start();
  created = create_tree(queue);
  t_create.stop();

  writeln();
  writeln("Tree size = ", created+1);
  writeln("Time: t_create= ", t_create.elapsed(), " (", created/t_create.elapsed(), " nodes/sec)");
}
