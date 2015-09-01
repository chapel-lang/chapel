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
param uts_version = "2.1";


/**** UTS CONFIG VARS ****/
config const SEED:        uint = 16;    // Seed for the hash of the root node
config const MAX_CHILDREN: int = 10;    // Max. Children a node may have
config const B_0:          int = 4;     // Branching Factor
config const MAX_DEPTH:    int = 6;     // Depth limit
config const nonLeafProb: real = 0.10;  // Probability of a non-leaf (binomial)
config const nonLeafBF:    int = 4;     // Non-leaf branching factor (binomial)
config const shiftDepth:  real = 0.5;   // Depth at which hybrid trees go from Geo=>Bin

config const distrib: NodeDistrib = NodeDistrib.Geometric;
config const geoDist: GeoDistrib  = GeoDistrib.GeoFixed;
config const testMode             = false;

// Parallel search parameters
config const MAX_THREADS:   int = 4;
config const chunkSize:     int = 10;
config const threading_throttle = 1;

// Global thread counter
var thread_cnt: sync int = 0;
var threads_spawned: int = 0; // "Locked" by thread_cnt

// Global stats, updated by a thread when it exits
// FIXME: These will become hotspots
var global_count: sync int = 0;
var global_maxDepth: sync int = 0;

// Shared termination detection
var terminated: single bool;


/**** State of the load balancer ****/
record LDBalanceState {
  var throttle_period: int = threading_throttle;
  var throttle: int;
}


/**** UTS TreeNode Class ****/
class TreeNode {
  var depth:   int;
  var hash:    [1..20] uint(8);

  var nChildren: int = 0;

  // Generate this node's children
  proc genChildren(ref q: DeQueue(TreeNode)): int {
    select distrib {
      when NodeDistrib.Geometric do 
        nChildren = numGeoChildren(geoDist);
      when NodeDistrib.Binomial do
        nChildren = numBinChildren();
      when NodeDistrib.Hybrid do
        nChildren = numHybridChildren();
      when NodeDistrib.Constant do
        nChildren = numConstChildren();
    }

    if debug then writeln("Constructing ", nChildren, " children");

    for i in 0..nChildren-1 {
      if debug then writeln("  + (", depth, ", ", i, ")");
      var t = new TreeNode(depth+1);
      rng_spawn(hash[1], t.hash[1], i:sha_int);
      q.pushTop(t);
    }

    return nChildren;
  }


  // Constant Distribution: Find the number of children
  //  Note: This is a _balanced_ tree.
  proc numConstChildren(): int {
    return if depth < MAX_DEPTH then B_0 else 0;
  }


  // Hybrid trees are geometric at the top of the tree and binomial
  // below a certain depth.
  proc numHybridChildren(): int {
    if (depth < shiftDepth * MAX_DEPTH) then
      return numGeoChildren(geoDist);
    
    else 
      return numBinChildren();
  }

  // Binomial Distribution: Find the number of children
  //  Note: distribution is identical everywhere below root
  proc numBinChildren(): int {
    if (depth == 0) then
      return B_0;
    
    else
      return if to_prob(rng_rand(hash[1])) < nonLeafProb then nonLeafBF else 0;
  }


  // Geometric Distribution: Find the number of children
  proc numGeoChildren(shape: GeoDistrib): int {
    var b_i: real = B_0;
  
    // use shape function to compute target b_i
    if (depth > 0) {
      select shape {
      
        // Expected size is polynomial in depth
        when GeoDistrib.GeoPoly do
          b_i = B_0 * (depth ** (-log(B_0:real)/log(MAX_DEPTH:real)));
        
        // Expected size is cyclic
        when GeoDistrib.GeoCyclic {
          if (depth > 5 * MAX_DEPTH) then
            b_i = 0.0;
          else
            b_i = B_0 ** sin(2.0*3.141592653589793*depth/MAX_DEPTH);
        }

        // Expected size is the same at all nodes up to max depth
        when GeoDistrib.GeoFixed do
          b_i = if (depth < MAX_DEPTH) then B_0 else 0;
 
        // Expected size decreases linearly in b_i
        when GeoDistrib.GeoLinear do 
          b_i =  B_0 * (1.0 - depth:real / MAX_DEPTH:real);
        
      }
    }

    // given target b_i, find prob p so expected value of 
    // geometric distribution is b_i.
    var p: real = 1.0 / (1.0 + b_i);

    // get uniform random number on [0,1)
    var u: real = to_prob(rng_rand(hash[1]));

    // max number of children at this cumulative probability
    // (from inverse geometric cumulative density function)
    return floor(log(1 - u) / log(1 - p)):int; 
  }
}


/*
** Print out search parameters
*/
proc uts_showSearchParams() {
  writeln("UTS v", uts_version, " - Unbalanced Tree Search (", 
      if parallel then "Parallel Chapel (DeQueue)" else "Sequential Chapel (DeQueue)", ")");

  writeln("Tree type: ", distrib);
  writeln("Tree shape parameters:");
  writeln("  root branching factor b_0 = ", B_0, ", root seed = ", SEED);
    
  if (distrib == NodeDistrib.Geometric || distrib == NodeDistrib.Hybrid) then
    writeln("  Geo. params: Max_Depth = ", MAX_DEPTH, ", shape function = ", geoDist);

  if (distrib == NodeDistrib.Binomial || distrib == NodeDistrib.Hybrid) then
    writeln("  Bin. params: nonLeafProb=", nonLeafProb, " nonLeafBF=", nonLeafBF);

  if (distrib == NodeDistrib.Hybrid) then
    writeln("  Hybrid params: shiftDepth=", shiftDepth);

  writeln("Random number generator: ", rng_getName());

  if (parallel) {
    writeln("Parallel execution parameters:");
    writeln("  MAX_THREADS = ", MAX_THREADS, ", chunkSize = ", chunkSize);
  }

  writeln();
}


proc balance_load(ref state: LDBalanceState, ref q: DeQueue(TreeNode)): int {
  if (parallel) {
    // Trade some imbalance here for blocking overhead
    if (q.size > 2*chunkSize && thread_cnt.readXX() < MAX_THREADS) {
      if debug then writeln(" ** dequeue ", q.id, " splitting off ", chunkSize, " nodes");

      // Attempt to reduce thread creation overhead
      if state.throttle < state.throttle_period {
        state.throttle += 1;
        return 0;
      } else {
        state.throttle = 0;
      }
      
      // Split off chunkSize nodes into a new queue
      var work = q.split(chunkSize);

      // Spawn a new worker on this queue
      var tmp = thread_cnt; // Lock the access to threads_spawned
      threads_spawned += 1;
      thread_cnt = tmp + 1;
      begin with (in work) create_tree(work);
      return 1;
    }
  }

  return 0;
}

/*
**  Parallel Tree Creation
*/
proc create_tree(ref q: DeQueue(TreeNode)) {
  var count, maxDepth: int;
  var ldbal_state = new LDBalanceState();

  if q.isEmpty() {
    writeln("create_tree(): Warning, called with no work");
  }

  while !q.isEmpty() {
    var node = q.popTop();
    count += node.genChildren(q);
    maxDepth = max(maxDepth, node.depth);
    delete node;

    if (q.size > 2*chunkSize) then
      balance_load(ldbal_state, q);
  }

  // Update search stats
  global_count += count;
  global_maxDepth = max(global_maxDepth.readFE(), maxDepth);

  // Update thread counts and detect termination
  var thread_cnt_l = thread_cnt;
  thread_cnt_l -= 1;
  if thread_cnt_l == 0 then
    terminated = true;
  thread_cnt = thread_cnt_l;
} 


proc main() {
  var t_create: Timer;
  var root: TreeNode;
  var queue: DeQueue(TreeNode);
 
  // Create the root and push it into a queue
  root = new TreeNode(0);
  rng_init(root.hash[1], SEED:sha_int);
  global_count += 1;
  queue.pushTop(root);

  uts_showSearchParams();

  writeln("Performing ", if parallel then "parallel" else "serial", " tree search...");

  t_create.start();
  thread_cnt.writeXF(1);
  create_tree(queue);
  if parallel then while !terminated { } // Wait for termination
  t_create.stop();

  writeln();
  if !testMode then writeln("Threads spawned: ", threads_spawned);
  writeln("Tree size = ", global_count.readFF(),
            ", depth = ", global_maxDepth.readFF());
  if !testMode then writeln("Time: t_create= ", t_create.elapsed(),
          " (", global_count.readXX()/t_create.elapsed(), " nodes/sec)");


}
