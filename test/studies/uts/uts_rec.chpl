/*
**  UTS -- The Unbalanced Tree Search
**
**  James Dinan <dinan@cray.com>
**  July, 2007
*/
 
use Math;
use Time;
use sha1_rng;


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

config const MAX_THREADS:  int = 20;
config const MIN_THREADS:  int = MAX_THREADS/4;

// Global thread counter
var thread_cnt: sync int = 0;
var threads_spawned: int = 0; // "Locked" by thread_cnt


/**** UTS TreeNode Class ****/
class TreeNode {
  var depth:   int;
  var hash:    [1..20] uint(8);

  // By default, children will be empty since it has range [1..0]
  var nChildren: int = 0;
  var childDom = {1..nChildren};
  var children:  [childDom] unmanaged TreeNode?;


  // Generate this node's children
  proc genChildren(): int {
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

    if debug then writeln("Constructing ", nChildren, " children: ", childDom);

    childDom  = {1..nChildren};

    forall i in childDom {
      if debug then writeln("  + (", depth, ", ", i, ")");
      children[i]    = new unmanaged TreeNode(depth+1);
      rng_spawn(hash[1], children[i]!.hash[1], (i-1):sha_int);
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
      if parallel then "Parallel Chapel" else "Sequential Chapel", ")");

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
    writeln("  MAX_THREADS = ", MAX_THREADS, ", MIN_THREADS = ", MIN_THREADS);
  }

  writeln();
}


/*
** Request n threads.  Get back the number of threads granted.
*/
proc requestThreads(n:int): int {
  if (parallel) {
    // Trade some imbalance here for blocking overhead
    if (thread_cnt.readXX() < MIN_THREADS) {
      var thread_cnt_l = thread_cnt;

      // Try to get a ticket to run in parallel
      if (thread_cnt_l < MAX_THREADS) {
        var threads_avail   = MAX_THREADS-thread_cnt_l;
        var threads_granted = if (n <= threads_avail) then n else threads_avail;
        
        thread_cnt = thread_cnt_l + threads_granted;
        threads_spawned += threads_granted;
        return threads_granted;
      } else {
        thread_cnt = thread_cnt_l;
      }
    }
  }

  return 0;
}


/*
**  Parallel DFS
*/
proc dfs_count(n: unmanaged TreeNode, wasParallel: bool = false):int {
  if n != nil {
    if (parallel) {
      var threads_granted = requestThreads(n.nChildren);
      var count: sync int = 0;

      coforall i in 1..threads_granted {
        count += dfs_count(n.children[i]!, true);
      }

      for i in threads_granted+1..n.nChildren {
        count += dfs_count(n.children[i]!, false);
      }

      if (wasParallel) then thread_cnt -= 1;
      return count+1;

    } else {
      var count: int;

      for i in n.childDom {
        count += dfs_count(n.children[i]!, false);
      }
      return count+1;
    }
  } else {
    return 0;
  }
}


/*
**  Parallel Tree Creation
*/
proc create_tree(parent: unmanaged TreeNode, wasParallel: bool = false): int {
  if parent == nil {
    writeln("create_tree(): Warning, called with nil");
    return 0;
  }

  if (parallel) {
    var count: sync int = parent.genChildren();
    var threads_granted = requestThreads(parent.nChildren);

    // Spawn threads
    coforall i in 1..threads_granted {
      count += create_tree(parent.children[i]!, true);
    }
    
    // Run the rest sequentially
    for i in threads_granted+1..parent.nChildren {
      count += create_tree(parent.children[i]!, false);
    }

    if (wasParallel) then thread_cnt -= 1;
    return count;

  } else {
    var count: int = parent.genChildren();

    for i in parent.childDom do
      count += create_tree(parent.children[i], false);
    return count;
  }
} 


proc destroyTree(tn: unmanaged TreeNode) {
  for child in tn.children do
    destroyTree(child!);
  delete tn;
}


proc main {
  var t_create: Timer();
  var t_dfs   : Timer();
  var counted, created: int;
  var root = new unmanaged TreeNode(0);
  rng_init(root.hash[1], SEED:sha_int);

  uts_showSearchParams();

  writeln("Performing tree creation..");
  t_create.start();
  created = create_tree(root);
  t_create.stop();

  if parallel && !testMode then writeln("  threads_spawned= ", threads_spawned);
  threads_spawned = 0;

  writeln("Performing tree traversal..");
  t_dfs.start();
  counted = dfs_count(root);
  t_dfs.stop();

  if parallel && !testMode then writeln("  threads_spawned= ", threads_spawned);
 
  writeln();
  writeln("Total nodes: Created = ", created, ", ", "Counted = ", counted);
  if !testMode then writeln("Time: t_create= ", t_create.elapsed(),
          " (", created/t_create.elapsed(), " nodes/sec)", ", t_dfs = ", t_dfs.elapsed(),
          " (", counted/t_dfs.elapsed(), " nodes/sec)");

  destroyTree(root);
}
