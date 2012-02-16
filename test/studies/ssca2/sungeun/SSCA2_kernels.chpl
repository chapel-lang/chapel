module SSCA2_kernels

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
      var heaviest_edge_weight$ : sync int = 0;

      // ---------------------------------------------------------
      // find heaviest edge weight in a single pass over all edges
      // ---------------------------------------------------------

      //       heaviest_edge_weight = max reduce [ s in G.vertices ] 
      //        	                         [ w in G.edge_weight (s) ] w;

      forall s in G.vertices do
        forall w in G.edge_weight (s) do
	  heaviest_edge_weight$ = max ( w, heaviest_edge_weight$ );

      // ---------------------------------------------
      // in a second pass over all edges, extract list 
      // of all edges  matching the heaviest weight
      // ---------------------------------------------

      forall s in G.vertices do
        forall (t, w) in ( G.Neighbors (s), G.edge_weight (s) ) do

	  // should be forall, requires a custom parallel iterator in the 
	  // random graph case and zippering for associative domains may 
	  // also present a problem

	  if w == heaviest_edge_weight$.readXX () then {
	    heavy_edge_list.add ( (s,t) ); 
	  };

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
	writeln ( "Heaviest weight      : ", 
		  heaviest_edge_weight$ . readFF ()); 
	writeln ( "Number of heavy edges:", heavy_edge_list.numIndices );
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
	var min_distance$            : [vertex_domain] sync int = -1;
	  
	if DEBUG_KERNEL3 then 
	  writeln ( " Building heavy edge subgraph from pair:", (x,y) );
	Active_Level.add ( y );
	Next_Level.clear ();
	Heavy_Edge_Subgraph ( (x, y) ).nodes.clear ();
	Heavy_Edge_Subgraph ( (x, y) ).edges.clear ();
	min_distance$ ( y ).writeFF (0);

	Heavy_Edge_Subgraph ( (x, y) ).edges.add ( (x, y) );
	Heavy_Edge_Subgraph ( (x, y) ).nodes.add ( x );
	Heavy_Edge_Subgraph ( (x, y) ).nodes.add ( y );
  
	for path_length in 1 .. max_path_length do {
	    
	  forall v in Active_Level do {

	    forall w in G.Neighbors (v) do { // eventually, will be forall

	      if min_distance$ (w).readXX () < 0 then {

		if min_distance$ (w).readFE () < 0 then {
		  Next_Level.add (w);
		  Heavy_Edge_Subgraph ( (x, y) ).nodes.add (w);
		  min_distance$ (w).writeEF (path_length);
		}
		else
		  min_distance$ (w).writeEF (path_length);
	      }
			 
	      // min_distance$ must have been set by some thread by now

	      if min_distance$ (w).readFF () == path_length then {
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



  use ReplicatedDist;
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

  //
  // Data structure to save Children lists between the forward
  //  and backwards pass
  //
  record child_struct {
    type vertex;
    var nd: domain(1);
    var Row_Children: [nd] vertex;
    var child_count: int=0;
    var vlock$: sync bool = true;

    // This function should only be called using unique vertices
    proc add_child ( new_child: vertex ) {
      // int-fetch-add
       vlock$.readFE();
       child_count += 1;
       Row_Children[child_count] = new_child;
       vlock$.writeEF(true);
    }
  }

  //
  // Implementation of task-private variables for kernel 4
  //
  use BlockDist;
  const myPrivateSpace = [LocaleSpace] dmapped Block(boundingBox=[LocaleSpace]);
  class taskPrivateData {
    const vertex_domain;
    var tid$           : sync chpl_taskID_t = chpl_nullTaskID;
    var min_distance$  : [vertex_domain] sync int;
    var path_count$    : [vertex_domain] sync real (64);
    var depend         : [vertex_domain] real;
    var children_list  : [vertex_domain] child_struct(index(vertex_domain));
    var Active_Remaining: [myPrivateSpace] bool;
  };
  inline proc =(a: chpl_taskID_t, b: chpl_taskID_t) return b;
  inline proc !=(a: chpl_taskID_t, b: chpl_taskID_t) return __primitive("!=", a, b);
  class localePrivateData {
    const vertex_domain;
    const numTasks = if dataParTasksPerLocale==0 then here.numCores
      else dataParTasksPerLocale;
    var r = [0..#numTasks];
    var temps: [r] taskPrivateData(vertex_domain.type);
    proc gettid() {
      extern proc chpl_task_getId(): chpl_taskID_t;
      var mytid = chpl_task_getId();
      var slot = (mytid:uint % (numTasks:uint)):int;
      // Would be nice to have CAS
      var tid: chpl_taskID_t = temps[slot].tid$; // lock
      while ((tid != chpl_nullTaskID) && (tid != mytid)) {
        temps[slot].tid$ = tid;                  // unlock
        slot = (slot+1)%numTasks;
        tid = temps[slot].tid$;                  // lock
      }
      temps[slot].tid$ = mytid;                  // unlock
      return slot;
    }
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

      type Sparse_Vertex_List = domain(index(vertex_domain));

      var Between_Cent$ : [vertex_domain] sync real = 0.0;
      var Sum_Min_Dist$ : sync real = 0.0;

      // Initialize task private data
      var localePrivate: [myPrivateSpace] localePrivateData(vertex_domain.type);
      for l in localePrivate {
        l = new localePrivateData(vertex_domain);
        // l.temps = [i in l.r] new taskPrivateData(vertex_domain);
        forall t in l.temps do
          t = new taskPrivateData(vertex_domain);
      }

      // ------------------------------------------------------ 
      // Each iteration of the outer loop of Brandes's algorithm
      // computes the contribution (the "dependency" metric) for
      // one particular vertex  (s)  independently.
      // ------------------------------------------------------
  
      if PRINT_TIMING_STATISTICS then stopwatch.start ();

      forall s in starting_vertices do {

	if DEBUG_KERNEL4 then writeln ( "expanding from starting node ", s );

        // sungeun: 8/2011
        // Privatization of the following distributed arrays may
        // be of concern.

	// --------------------------------------------------
	// all locally declared variables become private data 
	// for each instance of the parallel for loop
	// --------------------------------------------------
        const lp = localePrivate[here.id];
        const tid = lp.gettid();
        var depend => lp.temps[tid].depend;
        var min_distance$ => lp.temps[tid].min_distance$;
        var path_count$   => lp.temps[tid].path_count$;
        forall v in [vertex_domain] {
          depend[v] = 0.0;
          min_distance$[v].writeXF(-1);
          path_count$[v].writeXF(0.0);
        }
  	// var min_distance$  : [vertex_domain] sync int       = -1;
	// var path_count$    : [vertex_domain] sync real (64) = 0.0;
        // var depend         : [vertex_domain] real;

	var Lcl_Sum_Min_Dist: sync real                     = 0.0;

        var children_list => lp.temps[tid].children_list;
        // var children_list: [vertex_domain] child_struct(index(vertex_domain));
        // Initialize size of child lists for each vertex to its neighbor count
        forall v in vertex_domain {
          children_list[v].child_count = 0;
          children_list[v].vlock$.writeXF(true);
          children_list[v].nd = [1..G.n_Neighbors[v]];
        }

	// The structure of the algorithm depends on a breadth-first
	// traversal. Each vertex will be marked by the length of
	// the shortest path (min_distance$) from s to it. The array
	// path_count$ will hold a count of the number of shortest
	// paths from s to this node.  The number of paths in moderate
	// sized tori exceeds 2**64.
  
        // Used to check termination of the forward pass
        //
        // sungeun: 8/2011
        // Added replicated level sets
        //
        // Each locale will have its own level sets.  A locale's level set
        // will only contain nodes that are physically allocated on that
        // particular locale.  We implement this using the replicated
        // distribution.
        // sungeun: 8/2011
        // Can possibly use a replicated bool with rcCollect(), but
        // not sure of the performance implications for large numbers
        // of locales.
        // sungeun: 10/2011
        // Distributed Active_Remaining to reduce communication overhead.
        // Similar scaling concerns exists for the reduction as for
        // replacing it with a replicated bool.
        var Active_Remaining => lp.temps[tid].Active_Remaining;
        Active_Remaining = true;
        var remaining = true;

        // Replicated level sets
	var Active_Level    : [rcDomain] Level_Set (Sparse_Vertex_List);
        var Next_Level      : [rcDomain] Level_Set (Sparse_Vertex_List);
        coforall loc in Locales do on loc {
          rcLocal(Active_Level) = new Level_Set (Sparse_Vertex_List);
          rcLocal(Active_Level).previous = nil;
          rcLocal(Next_Level) = new Level_Set (Sparse_Vertex_List);
          rcLocal(Next_Level).previous = rcLocal(Active_Level);
        }

	var current_distance : int = 0;
  
	// establish the initial level sets for the
	// breadth-first traversal from s

        on s {
          rcLocal(Active_Level).Members.add ( s );
          rcLocal(Next_Level).Members.clear ();
          min_distance$ (s) . writeFF (0);
          path_count$ (s) . writeFF (1);
        }

	while remaining do { 
  
	    // ------------------------------------------------
	    // expand the neighbor sets for all vertices at the
	    // current distance from the starting vertex  s
	    // ------------------------------------------------
      
	    current_distance += 1;

            // sungeun: 8/2011
            // basic single use barrier
            var count: sync int = numLocales;
            var barrier: single bool;

            // sungeun: 8/2011
            // Copy this value to a constant to enable remote value
            // forwarding optimization.
            const current_distance_c = current_distance;
            coforall loc in Locales do on loc {
             forall u in rcLocal(Active_Level).Members do { // sparse

               forall (v, w) in ( G.Neighbors (u), G.edge_weight (u) ) do on vertex_domain.dist.idxToLocale(v) {
		// --------------------------------------------
		// add any unmarked neighbors to the next level
		// --------------------------------------------
  
		if  ( FILTERING &&  w % 8 != 0 ) || !FILTERING  then
		  if  min_distance$ (v) . readXX () < 0  then
		    { 
		      if  min_distance$ (v) . readFE () < 0  then
			{ 
			  min_distance$ (v).writeEF (current_distance_c);
			  rcLocal(Next_Level).Members.add (v);
			  if VALIDATE_BC then
			    Lcl_Sum_Min_Dist += current_distance_c;
			}
		      else {
                        // could min_distance$(v) be < current_distance?
			min_distance$ (v) . writeEF (current_distance_c);
                      }
		    }


		// ------------------------------------------------
		// only neighbors of  u  that are in the next level
		// are on shortest paths from s through v.  Some
		// thread will have set  min_distance$ (v) by the
		// time this code is reached, whether  v  lies in
		// the previous, the current or the next level.
		// ------------------------------------------------
  
		if  min_distance$ (v).readFF () == current_distance_c  
		  then {
		    path_count$ (v) += path_count$ (u).readFF ();
                    children_list(u).add_child (v);
                  }

	      }
	    };

            // sungeun: 8/2011
            // This (split-phase) barrier is needed to insure all updates
            // to Next_Level are completed before creating the next
            // Next_Level.
            var myc = count;
            if myc!=1 {
              count = myc-1; // release the lock
              // do some work while we wait
              rcLocal(Active_Level) = rcLocal(Next_Level);

              barrier;       // wait for everyone
            } else {         // last one here
              barrier=true;  // release everyone first
              rcLocal(Active_Level) = rcLocal(Next_Level);
            }

            rcLocal(Next_Level)   = new Level_Set (Sparse_Vertex_List);
	    rcLocal(Next_Level).previous = rcLocal(Active_Level);
            Active_Remaining[here.id] =
              rcLocal(Active_Level).Members.numIndices:bool;

          }

          remaining = || reduce Active_Remaining;

	};  // end forward pass

        // Resize the arrays to the actual count to free up some memory
        forall v in G.vertices {
            children_list[v].nd = [1..children_list[v].child_count];
        }

	if VALIDATE_BC then
	  Sum_Min_Dist$ += Lcl_Sum_Min_Dist;

	// -------------------------------------------------------------
	// compute the dependencies recursively, traversing the vertices 
	// of the graph in non-increasing order of distance (reverse 
	// ordering from the initial traversal)
	// -------------------------------------------------------------

	var graph_diameter = current_distance - 1;

	if DEBUG_KERNEL4 then 
	  writeln ( " graph diameter from starting node ", s, 
		    "  is ", graph_diameter );

        // sungeun: 8/2011
        // basic single use barrier
        var count: sync int = numLocales;
        // to simplify synchronization between multiple barriers
        var barrier: [2..graph_diameter] single bool;

        // writeln((tid, depend));
        coforall loc in Locales do on loc {
          delete rcLocal(Next_Level);	               // it's empty
          rcLocal(Next_Level)   = rcLocal(Active_Level).previous;  // back up to last level
          delete rcLocal(Active_Level);
          rcLocal(Active_Level) = rcLocal(Next_Level);
  
         for current_distance in 2 .. graph_diameter by -1 do {
            rcLocal(Next_Level)   = rcLocal(Active_Level).previous;
            delete rcLocal(Active_Level);
            rcLocal(Active_Level) = rcLocal(Next_Level);

            // inner reduction should parallelize eventually; compiler
            // serializes it today (and warns us that it did)

            forall u in rcLocal(Active_Level).Members do
              {
	      depend (u) = + reduce [v in children_list(u).Row_Children]
		  ( path_count$ (u) . readFF () / 
		    path_count$ (v) . readFF () )      *
		    ( 1.0 + depend (v) );

              Between_Cent$ (u) += depend (u);
              }
            // sungeun: 8/2011
            // This barrier is needed to insure all updates to depend are
            // complete before the next pass.
            var myc = count;
            if myc==1 {
              count = numLocales;
              barrier[current_distance] = true;
            } else {
              count = myc-1;
              barrier[current_distance];
            }
            // writeln((tid, depend));
          };
          delete rcLocal(Active_Level);
        }

      }; // closure of outer embarassingly parallel forall

      if PRINT_TIMING_STATISTICS then {
	stopwatch.stop ();
	var K4_time = stopwatch.elapsed ();
	stopwatch.clear ();
	writeln ( "Elapsed time for Kernel 4: ", K4_time, " seconds");

	var n0            = + reduce [v in vertex_domain] (G.n_Neighbors (v)== 0);
	var n_edges       = + reduce [v in vertex_domain] G.n_Neighbors (v);
	var N_VERTICES    = vertex_domain.numIndices;
	var TEPS          = 7.0 * N_VERTICES * (N_VERTICES - n0) / K4_time;
	var Adjusted_TEPS = n_edges * (N_VERTICES - n0) / K4_time;

	writeln ( "                     TEPS: ", TEPS );
	writeln ( " edge count adjusted TEPS: ", Adjusted_TEPS );
      }

      if VALIDATE_BC then
	Sum_Min_Dist = Sum_Min_Dist$;
      
      Between_Cent = Between_Cent$;
  
      for l in localePrivate {
        [i in l.r] delete l.temps[i];
        delete l;
      }

    } // end of Brandes' betweenness centrality calculation

}

