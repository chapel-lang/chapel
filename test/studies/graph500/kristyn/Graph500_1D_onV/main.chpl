module Graph500_main
{
  proc main () {

  use BlockDist;
  use Time;
  use Random;
  use Sort;

  use Graph500_defs;
  use Scalable_Graph_Generator;
  use Construct_Graph;
  use Create_Parent_Tree;
  use Verify;

  const A: real = 0.57;
  const B: real = 0.19;
  const C: real = 0.19;
  const D: real = 0.05;

  const NSTAT = 9;  
  const DEFAULT_INT_SIZE = 64;

  proc statistics (inout res, inout data, n: int(64)) {
    var s, mean: real;
    var t: real;
    var k: int;

    /* Quartiles */
    
    //sort (data);
    res[0] = data[0];
    t = (n+1) / 4.0;
    k = t:int;
    if (t == k) then
      res[1] = data[k];
    else
      res[1] = 3*(data[k]/4.0) + data[k+1]/4.0;
    t = (n+1) / 2.0;
    k = t:int;
    if (t == k) then
      res[2] = data[k];
    else
      res[2] = data[k]/2.0 + data[k+1]/2.0;
    t = 3*((n+1) / 4.0);
    k = t:int;
    if (t == k) then
      res[3] = data[k];
    else
      res[3] = data[k]/4.0 + 3*(data[k+1]/4.0);
    res[4] = data[n-1];
    
    s = data[n-1];
    for k in n-1..0 by -1 do {
      s += data[k-1];
    }
    mean = s/n;
    res[5] = mean;
    s = data[n-1] - mean;
    s *= s;
    for k in n-1..0 by -1 do {
      var tmp = data[k-1] - mean;
      s += tmp * tmp;
    }
    res[6] = sqrt (s/(n-1));
    
    if data[0] != 0 then {
      s = 1:int(32) / data[0];
    } else {
      s = 0;
    }
    for k in 1..n do {
      if data[k] != 0 then {
        s = 1:real(32) / data[k];
      } else {
        s = 0;
      }
    }
    res[7] = n/s;
    mean = s/n;
    
    /*
      Nilan Norris, The Standard Errors of the Geometric and Harmonic
      Means and Their Application to Index Numbers, 1940.
      http://www.jstor.org/stable/2235723
    */
    if data[0] != 0 then {
      s = 1:real(32) / data[0];
    } else {
      s = 0;
    }
    s -= mean;
    s *= s;
    for k in 1..n do {
      var tmp: real;
      if data[k] != 0 then {
        tmp = 1:real(32) / data[k];
      } else {
        tmp = 0;
      }
      tmp -= mean;
      s += tmp * tmp;
    }
    s = (sqrt (s)/(n-1)) * res[7] * res[7];
    res[8] = s;
  }

  proc output_results (scale: int, NV: int, edgefactor:int,
                       const A: real, const B: real, const C: real,
                       const D: real, generation_time: real,
                       construction_time: real, nbfs: int, 
                       inout BFS_time_array: [] real,
                       inout BFS_nedges_traversed: []int ) {
    var k, sz: int;
    var tm: [0..nbfs-1] real;
    var stats: [0..NSTAT-1] real;

    sz = (1:int(32) << scale) * edgefactor * 2 * DEFAULT_INT_SIZE;
    writeln ("SCALE: ", scale, " ");
    writeln ("nvtx: ", NV, " ");
    writeln ("edgefactor: ", edgefactor, " ");
    writef ("terasize: %20.17er\n", sz/1.0e12);
    writef ("A: %20.17er\n", D);
    writef ("B: %20.17er\n", B);
    writef ("C: %20.17er\n", C);
    writef ("D: %20.17er\n", D);
    writef ("generation_time: %20.17er\n", generation_time);
    writef ("construction_time: %20.17er\n", construction_time);
    writeln ("nbfs: ", nbfs);

    proc PRINT_STATS(lbl, israte) {
      do {
        writeln ("min_%s: %20.17e", lbl, stats[0]);
        writeln ("firstquartile_%s: %20.17e", lbl, stats[1]);
        writeln ("median_%s: %20.17e", lbl, stats[2]);
        writeln ("thirdquartile_%s: %20.17e", lbl, stats[3]);
        writeln ("max_%s: %20.17e", lbl, stats[4]);
        if (!israte) {
          writeln ("mean_%s: %20.17e", lbl, stats[5]);
          writeln ("stddev_%s: %20.17e", lbl, stats[6]);
        } else {
          writeln ("harmonic_mean_%s: %20.17e", lbl, stats[7]);
          writeln ("harmonic_stddev_%s: %20.17e", lbl, stats[8]);
        }
      } while (0);
    }
    sort(tm);


    forall k in 0..nbfs-1 do
      tm[k] = BFS_time_array[k];
    statistics (stats, tm, nbfs);
    PRINT_STATS("time", false);
    
    forall k in 0..nbfs-1 do
      tm[k] = BFS_nedges_traversed[k];
    statistics (stats, tm, nbfs);
    PRINT_STATS("nedge", false);
    
    forall k in 0..nbfs-1 do
      tm[k] = BFS_nedges_traversed[k] / BFS_time_array[k];
    statistics (stats, tm, nbfs);
    PRINT_STATS("TEPS", true);
  }

        
  writeln ( "Problem Dimensions");
  writeln ( "              Scale: ", SCALE );
  writeln ( " Number of Vertices: ", N_VERTICES );
  writeln ( "        Edge factor: ", EDGEFACTOR );

  if (PRINT_LOCALE_INFORMATION) {
    coforall loc in Locales do on loc {
      writeln("Locale ID: ", loc.id, " of ", numLocales);
      writeln("Locale ID: ", loc.id, " Number of cores " , loc.numPUs());
      writeln("Locale ID: ", loc.id, " Max task parallelism " , loc.maxTaskPar);
    }
  }

  const edge_range =  1..N_RAWEDGES;

  var Edges:[edgelist_domain] directed_vertex_pair;
  var Histogram:[vertex_domain] vertex_id=0;

  var generation_time: Timer;
  var construction_time: Timer;
  var BFS_time: [1..NUMROOTS] Timer;
  var BFS_time_array: [1..NUMROOTS] real;
  var BFS_nedges_traversed: [1..NUMROOTS] int;

  generation_time.start(); 
  Scalable_Data_Generator( SCALE, N_VERTICES, N_RAWEDGES, Edges);
  generation_time.stop();

  if(PRINT_TIMING_STATISTICS){
    writeln("Time for Scalable Data Generator = ", generation_time.elapsed()); 
  }

// Check min/max Node ID's

  var min_vertex_ID_start = min reduce Edges.start;
  var min_vertex_ID_end = min reduce Edges.end;
  var max_vertex_ID_start = max reduce Edges.start;
  var max_vertex_ID_end = max reduce Edges.end;

  var min_vertex_ID = min (min_vertex_ID_start, min_vertex_ID_end);
  var max_vertex_ID = max (max_vertex_ID_start, max_vertex_ID_end);

  writeln ();
  writeln ("Check for valid minimum and maximum vertex IDs ");
  writeln ("Minimum vertex ID ", min_vertex_ID);
  writeln ("Maximum vertex ID ", max_vertex_ID);
  writeln ();

  construction_time.start();

  var G = new unmanaged Graph (vertex_domain);

  constructGraph (Edges, G);

  construction_time.stop();
  if(PRINT_TIMING_STATISTICS){
    writeln("Kernel 1, Graph Construction Time = ", construction_time.elapsed()); 
  }

  // Generate a list of valid starting roots
  // Valid starting roots have at least one edge to another node
  var Rand_Gen = new owned NPBRandomStream (seed = 9);
  var Unif_Random: [1..NUM_CANDIDATES] real;
  Rand_Gen.fillRandom ( Unif_Random );
  var runID: int = 1;
  var candidate: int = 1;
  var root: vertex_id;

  var ParentTree:[vertex_domain] vertex_id=-1;
  var nedges_traversed: int;
  var nTEPs: real;

  writeln("Select candidate root and run BFS if criteria satisfied");
  writeln();

  while ( (runID <= NUMROOTS) && (candidate <= NUM_CANDIDATES )) do
  {
     root = floor (1+ Unif_Random[candidate]* N_VERTICES): vertex_id;
     writeln("Candidate root = ", root);

     // Verify that root is a valid candidate, ie has at least one edge
     if ( G.Vertices[root].neighbor_count > 0 ){

       BFS_time[runID].start(); 
       BFS ( root, ParentTree, G);
       BFS_time[runID].stop(); 

       BFS_time_array[runID] = BFS_time[runID].elapsed();
       BFS_nedges_traversed[runID] = verify_bfs_tree ( root, ParentTree, Edges);

       nTEPs = BFS_nedges_traversed[runID] / BFS_time[runID].elapsed();

       writeln("Root =  ", root );
       writeln("  Number of traversed edges = ", BFS_nedges_traversed[runID]);

       if (PRINT_TIMING_STATISTICS) {
          writeln("Time for BFS = ", BFS_time[runID].elapsed()); 
          writeln("TEPS = ", nTEPs);
       }

       runID += 1; candidate += 1;
       ParentTree = -1;
       
     }
     else
     {
        writeln("Root not a valid candidate");
        candidate += 1;
     }

  }

      if (PRINT_TIMING_STATISTICS) {
        output_results (SCALE, N_VERTICES, EDGEFACTOR, A, B, C, D,
             generation_time.elapsed(), construction_time.elapsed(), NUMROOTS,
             BFS_time_array, BFS_nedges_traversed );
      }

    delete G;
  }

}
