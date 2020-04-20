module SSCA2_kernels

//  +==========================================================================+
//  | VERSION USING ATOMIC OPERATIONS:                                         |
//  | As of March, 2010, the atomic keyword is NOT supported in Chapel.        |
//  | This code, therefore, is only a sketch, which can only be executed       |
//  | sequentially.  All potential parallel loops are instantiated as          |
//  | "forall" loops, so execution will fail unless the code is compiled with  |
//  | the "--serial" flag.                                                     |
//  |                                                                          |
//  | This code has NOT been tested running in parallel.  See the sync         |
//  | variable version for a mostly parallel implementation.                   |
//  +==========================================================================+

//  +==========================================================================+
//  |  Polymorphic Implementation of SSCA #2, Kernels 2-4                      |
//  |                                                                          |
//  |  Each kernel takes a graph argument which provides for each vertex       |
//  |  1.  an iterator for its set of neighbors                                |
//  |  2.  a parallel integer array of edge weights, which can be zipper       |
//  |      iterated with the set of neighbors                                  |
//  |  3.  the number of neighbors                                             |
//  |                                                                          |
//  |  These are the only requirements on the representation of the graph.     |
//  |                                                                          |
//  |  Filtering in Kernel 4 is turned on or off by a compilation time param.  |
//  |  The equivalent of "ifdef" is provided by conditionals that are          |
//  |  evaluated at compile time.  Dead code is eliminated by the compiler.    |
//  +==========================================================================+

//  +==========================================================================+
//  |  Parallelism note -- March 2010                                          |
//  |                                                                          |
//  |  A common motif in all three kernels is a pair of loops of the form      |
//  |      forall vertices in some_set do                                      |
//  |         forall neighbors of each vertex do ....                          |
//  |  Even when this appears inside an enclosing embarrassingly parallel      |
//  |  outer loop, it will be important to parallelize this pair of loops.     |
//  |  In the most important cases, the size of the set of vertices varies     |
//  |  considerably during the execution of the kernel and there can be wide   |
//  |  variation in the number of neighbors.  So parallelization should        |
//  |  consider both loops together.                                           |
//  |                                                                          |
//  |  That said, currently Chapel cannot parallelize the innermost loops,     |
//  |  even when the innermost loop is a reduction.  Some or all of the        |
//  |  iterators that appear in this inner loop do not have parallel forms     |
//  |  at present; they use sparse or associative domains.  So the inner loops |
//  |  all appear in this code as "for" rather than "forall".  They are all    |
//  |  parallelizable and should become parallel loops eventually.             |
//  +==========================================================================+
{ 
  use SSCA2_compilation_config_params, Time;

  var stopwatch : Timer;

  // ========================================================
  //                           KERNEL 2:
  // ========================================================
  // Find the edges with the largest edges.  Return a list of 
  // edges, all of which have the largest weight.
  // ========================================================
  
  proc largest_edges ( G, heavy_edge_list :domain )
    
    // edge_weights can be either an array over an associative
    // domain or over a sparse domain.  the output  heavy_edge_list
    // can either kind of domain or something else purpose-built
    // for this task.
    {
      if PRINT_TIMING_STATISTICS then stopwatch.start ();
      var heaviest_edge_weight : int = 0;

      // ---------------------------------------------------------
      // find heaviest edge weight in a single pass over all edges
      // ---------------------------------------------------------

      //       heaviest_edge_weight = max reduce [ s in G.vertices ] 
      //        	                         [ w in G.edge_weight (s) ] w;

      forall s in G.vertices with (ref heaviest_edge_weight) do
        forall w in G.edge_weight (s) do
	  atomic heaviest_edge_weight = max ( w, heaviest_edge_weight );

      // ---------------------------------------------
      // in a second pass over all edges, extract list 
      // of all edges  matching the heaviest weight
      // ---------------------------------------------

      forall s in G.vertices with (ref heavy_edge_list) do
	forall (t, w) in ( G.Neighbors (s), G.edge_weight (s) ) with (ref heavy_edge_list) do

	  // should be forall, requires a custom parallel iterator in the 
	  // random graph case and zippering for associative domains may 
	  // also present a problem

	  if w == heaviest_edge_weight then
	      atomic heavy_edge_list.add ( (s,t) ); 

      if PRINT_TIMING_STATISTICS then {
	stopwatch.stop ();
	writeln ( "Elapsed time for Kernel 2: ", stopwatch.elapsed (), 
		  " seconds");
	stopwatch.clear ();
      }

      // ------------------------------------------------
      // should be able to write a user-defined "maxlocs" 
      // reduction more efficiently than this scheme
      // ------------------------------------------------

      if DEBUG_KERNEL2 then {
	writeln ();
	writeln ( "Heaviest weight      : ", heaviest_edge_weight ); 
	writeln ( "Number of heavy edges:", heavy_edge_list.size );
	writeln ();
	writeln ( "Edges with largest weight and other neighbors:" );
	for (s,t) in heavy_edge_list do {
	  writeln ("edge   ", (s,t));
	  for (v,w) in (G.Neighbors (s), G.edge_weight (s) ) do
	    writeln ("      ", v, " ", w);}
      }
    };
	      
  
  // ===================================================================
  //                              KERNEL 3:
  // ===================================================================
  // For each root (heavy) edge, find the subgraph (vertices and edges)
  // defined by directed paths of length no greater than max_path_length
  // in which the first edge traversed is the root edge
  // ===================================================================
  
  proc rooted_heavy_subgraphs ( G, 
			       Heavy_Edge_List     : domain,
			       Heavy_Edge_Subgraph : [],
			       in max_path_length  : int )
    
    // -------------------------------------------------------------------------
    // there is a classic space versus time tradeoff.  if the subgraphs expanded
    // by breadth first search are small, it would make sense to use a hash
    // table or an associative domain to represent the search.  If the subgraphs
    // are large, using a full length vector to represent the search is more
    // appropriate.  We expect small diameters for power law graphs, so we
    // expect large subgraphs.
    // -------------------------------------------------------------------------
  
    {
      if PRINT_TIMING_STATISTICS then stopwatch.start ();

      const vertex_domain = G.vertices;
       
      forall ( x, y ) in Heavy_Edge_List do {
	var Active_Level, Next_Level : domain ( index (vertex_domain) );
	var min_distance             : [vertex_domain] int = -1;
	  
	if DEBUG_KERNEL3 then 
	  writeln ( " Building heavy edge subgraph from pair:", (x,y) );
	Active_Level.add ( y );
	Next_Level.clear ();
	Heavy_Edge_Subgraph ( (x, y) ).nodes.clear ();
	Heavy_Edge_Subgraph ( (x, y) ).edges.clear ();
	min_distance ( y ) = 0;

	Heavy_Edge_Subgraph ( (x, y) ).edges.add ( (x, y) );
	Heavy_Edge_Subgraph ( (x, y) ).nodes.add ( x );
	Heavy_Edge_Subgraph ( (x, y) ).nodes.add ( y );
  
	for path_length in 1 .. max_path_length do {
	    
	  forall v in Active_Level do {

	    forall w in G.Neighbors (v) do { // eventually, will be forall

	      atomic if min_distance (w) < 0 then {
		Next_Level.add (w);
		Heavy_Edge_Subgraph ( (x, y) ).nodes.add (w);
		min_distance (w) = path_length;
	      }
			 
	      // min_distance must have been set by some thread by now

              atomic {
                if min_distance (w) == path_length then
		  Heavy_Edge_Subgraph ( (x, y) ).edges.add ( (v, w) );
              }
	    }
	  }
  
	  if path_length < max_path_length then {
	    Active_Level = Next_Level;
	    Next_Level.clear ();
	  }
	}
      }

      if PRINT_TIMING_STATISTICS then {
	stopwatch.stop ();
	writeln ( "Elapsed time for Kernel 3: ", stopwatch.elapsed (), 
		  " seconds");
	stopwatch.clear ();
      }
    } // end of rooted_heavy_subgraphs


  // ============================================================
  // generic class structure must be defined outside of 
  // generic procedure.  used by Betweenness Centrality kernel 4.
  // ------------------------------------------------------------
  // The set of vertices at a particular distance from s form a
  // level set.  The class allows the full set of vertices to be
  // partitioned into a linked list of level sets.  Each instance
  // of the outer loop in kernel 4 creates such a partitioning.
  // ============================================================

  class Level_Set {
    type Sparse_Vertex_List;
    var Members  : Sparse_Vertex_List;
    var previous : Level_Set (Sparse_Vertex_List);
  }
  

  // ==================================================================
  //                              KERNEL 4
  // ==================================================================
  // Calculate Betweenness Centrality for simple unweighted directed or
  // undirected graphs, using Madduri, et.al.'s modification of 
  // Brandes's 2001 algorithm
  // ==================================================================

  proc approximate_betweenness_centrality ( G, starting_vertices, 
					   Between_Cent : [] real,
					   out Sum_Min_Dist : real )
  
    // -----------------------------------------------------------------------
    // The betweenness centrality metric for a given node  v  is defined
    // as the double sum over s not equal to v and  t not equal to
    // either s or v of the ratio of the number of shortest paths from s to t
    // passing through v to the number of shortest paths from s to t.
    //
    // Brandes's algorithm decomposes the computation of this metric into,
    // first, separate sums for each vertex s, which can be computed
    // independently in parallel, and 
    // two, a recursive, tree-based, calculation of the path counts for 
    // any particular s.  
    // The complexity of this algorithm is O ( |V||E| ) time for an unweighted
    // graph.  The algorithm requires O ( |V| ) temporary space for each
    // process that executes instances of the outermost loop.
    // -----------------------------------------------------------------------
    {       
      const vertex_domain = G.vertices;

      // Had to change declaration below
      //    type Sparse_Vertex_List = sparse subdomain ( G.vertices );
      // to accommodate block distribution of G.vertices

      type Sparse_Vertex_List = sparse subdomain ( {(...vertex_domain.dims())} );

      // ------------------------------------------------------ 
      // Each iteration of the outer loop of Brandes's algorithm
      // computes the contribution (the "dependency" metric) for
      // one particular vertex  (s)  independently.
      // ------------------------------------------------------
  
      if PRINT_TIMING_STATISTICS then stopwatch.start ();

      Between_Cent = 0.0;
      Sum_Min_Dist = 0.0;

      forall s in starting_vertices do {
  
	if DEBUG_KERNEL4 then writeln ( "expanding from starting node ", s );

	// --------------------------------------------------
	// all locally declared variables become private data 
	// for each instance of the parallel for loop
	// --------------------------------------------------
  
  	var min_distance : [vertex_domain] int       = -1;
	var path_count   : [vertex_domain] real (64) = 0;
	var depend       : [vertex_domain] real      = 0.0;

	// The structure of the algorithm depends on a breadth-first
	// traversal. Each vertex will be marked by the length of
	// the shortest path (min_distance) from s to it. The array
	// path_count will hold a count of the number of shortest
	// paths from s to this node.  The number of paths in moderate
	// sized tori exceeds 2**64.
  
	var Active_Level = new Level_Set (Sparse_Vertex_List);
	var Next_Level   = new Level_Set (Sparse_Vertex_List);
  
	var current_distance : int = 0;
  
	// establish the initial level sets for the
	// breadth-first traversal from s

	Active_Level.Members.add ( s );
	Next_Level.Members.clear ();

	min_distance (s)      = 0;
	Active_Level.previous = nil;
	Next_Level.previous   = Active_Level;
	path_count (s)        = 1;
  
	while Active_Level.Members.size > 0 do { 
  
	    // ------------------------------------------------
	    // expand the neighbor sets for all vertices at the
	    // current distance from the starting vertex  s
	    // ------------------------------------------------
      
	    current_distance += 1;

	    forall u in Active_Level.Members  do {

	      forall (v, w) in ( G.Neighbors (u), G.edge_weight (u) ) do {

		// should be forall, requires a custom parallel iterator in the
		// random graph case and zippering for associative domains

		// --------------------------------------------
		// add any unmarked neighbors to the next level
		// --------------------------------------------
  
		if  ( FILTERING &&  w % 8 != 0 ) || !FILTERING  then
		  atomic {
		    if  min_distance (v) < 0  then {
		      min_distance (v) = current_distance;
		      Next_Level.Members.add (v);
		      if VALIDATE_BC then
			Sum_Min_Dist += current_distance;
		    }
		  };

		// ------------------------------------------------
		// only neighbors of  u  that are in the next level
		// are on shortest paths from s through v.  Some
		// thread will have set  min_distance (v) by the
		// time this code is reached, whether  v  lies in
		// the previous, the current or the next level.
		// ------------------------------------------------
                atomic {
                  if  min_distance (v)== current_distance then
                    path_count (v) += path_count (u);
                }
	      }
	    };
  
	    Active_Level = Next_Level;
	    Next_Level   = new Level_Set (Sparse_Vertex_List);
        
	    Next_Level.Members.clear ();
	    Next_Level.previous = Active_Level;

	  };  // end forward pass

	// -------------------------------------------------------------
	// compute the dependencies recursively, traversing the vertices 
	// of the graph in non-increasing order of distance (reverse 
	// ordering from the initial traversal)
	// -------------------------------------------------------------

	var graph_diameter = current_distance - 1;

	if DEBUG_KERNEL4 then 
	  writeln ( " graph diameter from starting node ", s, 
		    "  is ", graph_diameter );

	delete Next_Level;	               // it's empty
	Next_Level   = Active_Level.previous;  // back up to last level
	delete Active_Level;
	Active_Level = Next_Level;
  
	for current_distance in 2 .. graph_diameter by -1 do {

	  Next_Level   = Active_Level.previous;
	  delete Active_Level;
	  Active_Level = Next_Level;

	  // inner reduction should parallelize eventually; compiler
	  // serializes it today (and warns us that it did)

	  forall u in Active_Level.Members do
	    {
	      depend (u) = + reduce 
		[ (v, w)  in ( G.Neighbors (u), G.edge_weight (u) ) ]
		if ( min_distance (v) == current_distance ) && 
		( ( FILTERING && w % 8 != 0 || !FILTERING ) )
		then
		  ( path_count (u) / path_count (v) )
		    * ( 1 + depend (v) );

	      // do not need conditional u != s

	      atomic Between_Cent (u) += depend (u);
	    }
	};
	delete Active_Level;

      }; // closure of outer embarrasingly parallel forall
  
      if PRINT_TIMING_STATISTICS then {
	stopwatch.stop ();
	var K4_time = stopwatch.elapsed ();
	stopwatch.clear ();
	writeln ( "Elapsed time for Kernel 4: ", K4_time, " seconds");

	var n0            = + reduce [v in vertex_domain] (G.n_Neighbors (v)== 0);
	var n_edges       = + reduce [v in vertex_domain] G.n_Neighbors (v);
	var N_VERTICES    = vertex_domain.size;
	var TEPS          = 7.0 * N_VERTICES * (N_VERTICES - n0) / K4_time;
	var Adjusted_TEPS = n_edges * (N_VERTICES - n0) / K4_time;

	writeln ( "                     TEPS: ", TEPS );
	writeln ( " edge count adjusted TEPS: ", Adjusted_TEPS );
      }

    } // end of Brandes' betweenness centrality calculation

}
