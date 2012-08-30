module Graph500_main
{
  proc main () {

  use BlockDist;
  use Time;
  use Random;

  use Graph500_defs;
  use Scalable_Graph_Generator;
  use Construct_Graph;
  use Create_Parent_Tree;
  use Verify;

  const A: real = 0.57;
  const B: real = 0.19;
  const C: real = 0.19;
  const D: real = 0.05;


  extern proc output_results (scale: int, NV: int, edgefactor:int,
                const A: real, const B: real, const C: real, const D: real,
                generation_time: real, construction_time: real,
                nbfs: int, 
                inout BFS_time_array: real, inout BFS_nedges_traversed: int );

        
  writeln ( "Problem Dimensions");
  writeln ( "              Scale: ", SCALE );
  writeln ( " Number of Vertices: ", N_VERTICES );
  writeln ( "        Edge factor: ", EDGEFACTOR );

  if (PRINT_LOCALE_INFORMATION) {
  coforall loc in Locales do
    {
    on loc do
      writeln("Locale ID: ", loc.id, " of ", numLocales);
      writeln("Locale ID: ", loc.id, " Number of cores " , loc.numCores);
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

  var G = new Graph (vertex_domain);

  constructGraph (Edges, G);

  construction_time.stop();
  if(PRINT_TIMING_STATISTICS){
    writeln("Kernel 1, Graph Construction Time = ", construction_time.elapsed()); 
  }

  // Generate a list of valid starting roots
  // Valid starting roots have at least one edge to another node
  var Rand_Gen = new RandomStream (seed = 9);
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
     if ( G.Vertices[root].neighbor_count.read() > 0 ){

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
             BFS_time_array(1), BFS_nedges_traversed(1) );
      }

}

}
