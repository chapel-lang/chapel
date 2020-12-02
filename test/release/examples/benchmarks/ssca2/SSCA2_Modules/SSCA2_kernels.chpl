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
  use SSCA2_compilation_config_params, Time, Barriers, DSIUtil;

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

      // ---------------------------------------------------------
      // find heaviest edge weight in a single pass over all edges
      // ---------------------------------------------------------

      proc maxweight(s) {
	var mymax = 0;
	for w in G.edge_weight[s] do
	  if w > mymax then mymax = w;
	return mymax;
      }

      const heaviest_edge_weight = max reduce [s in G.vertices] maxweight(s);

      // ---------------------------------------------
      // in a second pass over all edges, extract list 
      // of all edges  matching the heaviest weight
      // ---------------------------------------------

      forall s in G.vertices with (ref heavy_edge_list) do
        forall (t, w) in G.NeighborPairs (s) with (ref heavy_edge_list) do

	  if w == heaviest_edge_weight then {
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
	writeln ( "Heaviest weight      : ", heaviest_edge_weight); 
	writeln ( "Number of heavy edges:", heavy_edge_list.size );
	writeln ();
	writeln ( "Edges with largest weight and other neighbors:" );
	for (s,t) in heavy_edge_list do {
	  writeln ("edge   ", (s,t));
	  for (v,w) in G.NeighborPairs (s) do
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
	var min_distance             : [vertex_domain] atomic int;
        forall m in min_distance do m.write(-1);
	  
	if DEBUG_KERNEL3 then 
	  writeln ( " Building heavy edge subgraph from pair:", (x,y) );
	Active_Level.add ( y );
	Next_Level.clear ();
	Heavy_Edge_Subgraph ( (x, y) )!.nodes.clear ();
	Heavy_Edge_Subgraph ( (x, y) )!.edges.clear ();
	min_distance ( y ).write(0);

	Heavy_Edge_Subgraph ( (x, y) )!.edges.add ( (x, y) );
	Heavy_Edge_Subgraph ( (x, y) )!.nodes.add ( x );
	Heavy_Edge_Subgraph ( (x, y) )!.nodes.add ( y );
  
	for path_length in 1 .. max_path_length do {
	    
	  forall v in Active_Level do {

	    forall w in G.Neighbors (v) do {


              if min_distance(w).compareAndSwap(-1, path_length) then {
                Next_Level.add (w);
                Heavy_Edge_Subgraph ( (x, y) )!.nodes.add (w);
	      }

	      if min_distance(w).read() == path_length then {
		Heavy_Edge_Subgraph ( (x, y) )!.edges.add ( (v, w) );
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


  use BlockDist;
  // For task-private temporary variables
  config const defaultNumTPVs = 16;
  config var numTPVs = min(defaultNumTPVs, numLocales);
  // Would be nice to use PrivateDist, but aliasing is not supported (yet)
  const PrivateSpace = LocaleSpace dmapped Block(boundingBox=LocaleSpace);

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

      // Considering using a dense 1-d array instead.  This would
      // complicate the Level_Set implementation a little, but would
      // probably be more efficient.
      type Sparse_Vertex_List = domain(index(vertex_domain));

      var Between_Cent$ : [vertex_domain] atomic real;
      var Sum_Min_Dist$ : atomic real;

      //
      // Throughout kernel 4, we use distributed arrays that are
      // private to each iteration of the outer loop.  Because set-up
      // and clean-up of such variables is very expensive, it is
      // desirable to do it at most only once per task versus once
      // per iteration, as a task executes multiple iterations of a
      // forall loop.
      //
      // To avoid this per-iteration overhead, we have implemented
      // an optimization that make certain variables private to each
      // task.  These variables are reinitialized each iteration of
      // the loop, but not reallocated, reprivatized, etc.
      //
      // Note that the Chapel group and collaborators are currently in
      // the process of bringing the idea of task private variable to
      // the language.
      //

      numTPVs = min(numTPVs, starting_vertices.size);
      const TPVSpace = {0..#numTPVs};
      var TPVLocales: [TPVSpace] locale =
        for t in TPVSpace do
          if numTPVs <= numLocales then
        // Here we set up the locales array for our task private temporary
        // variables.  We use _computChunkStartEnd() to distribute the
        // variables approximately evenly across the 1d locales array.
            Locales[_computeChunkStartEnd(numLocales, numTPVs, t+1)[1]-1]
          else
            Locales[((t-1)/numTPVs)/numLocales];

      const TPVLocaleSpace = TPVSpace dmapped Block(boundingBox=TPVSpace,
                                                    targetLocales=TPVLocales);

      // There will be numTPVs copies of the temps, thus throttling the
      // number of starting vertices being considered simultaneously.
      var TPV: [TPVLocaleSpace] unmanaged taskPrivateData(domain(index(vertex_domain)),
                                                vertex_domain.type)?;

      // Initialize
      coforall tpvElem in TPV do on tpvElem {
          const tpv = new unmanaged taskPrivateData(domain(index(vertex_domain)), vertex_domain);
          tpvElem = tpv;
          forall v in vertex_domain do
            tpv.BCaux[v].children_list.nd = {1..G.n_Neighbors[v]};
          for loc in Locales do on loc {
            const curr = new unmanaged Level_Set (Sparse_Vertex_List);
            const next = new unmanaged Level_Set (Sparse_Vertex_List);
            tpv.Active_Level[here.id] = curr;
            curr.previous = nil;
            curr.next = next;
            next.previous = curr;
            next.next = nil;
          }
      }
      var TPVM = new unmanaged TPVManager(TPV);

      // ------------------------------------------------------ 
      // Each iteration of the outer loop of Brandes's algorithm
      // computes the contribution (the "dependency" metric) for
      // one particular vertex  (s)  independently.
      // ------------------------------------------------------
  
      if PRINT_TIMING_STATISTICS then stopwatch.start ();

      forall s in starting_vertices do on vertex_domain.dist.idxToLocale(s) {

        const shere = here.id;

	if DEBUG_KERNEL4 then writeln ( "expanding from starting node ", s );

        // Initialize task private variables
        const tid = TPVM.gettid();
        const tpv = TPVM.getTPV(tid);
        ref BCaux = tpv.BCaux;
        pragma "dont disable remote value forwarding"
        inline proc f1(BCaux, v) {
          BCaux[v].path_count$.write(0.0);
        }
        forall v in vertex_domain do {
          BCaux[v].depend = 0.0;
          BCaux[v].min_distance.write(-1);
          f1(BCaux, v);
          BCaux[v].children_list.child_count.write(0);
        }
	var Lcl_Sum_Min_Dist: atomic real;

	// The structure of the algorithm depends on a breadth-first
	// traversal. Each vertex will be marked by the length of
	// the shortest path (min_distance) from s to it. The array
	// path_count$ will hold a count of the number of shortest
	// paths from s to this node.  The number of paths in moderate
	// sized tori exceeds 2**64.
  
        //
        // Used to check termination of the forward pass
        //
        // We could use a task-private reduction variable here.  This
        // is yet another concept that the Chapel group is planning to
        // implement.
        //

        var remaining = true;

        //
        // Each locale will have its own level sets.  A locale's level set
        // will only contain nodes that are physically allocated on that
        // particular locale.
        //
        ref Active_Level = tpv.Active_Level;
        pragma "dont disable remote value forwarding"
        inline proc f2(BCaux, s) {
          BCaux[s].path_count$.write(1.0);
        }

        var barrier = new Barrier(numLocales);

        coforall loc in Locales with (ref remaining, ref barrier) do on loc {
          const AL = Active_Level[here.id]!;
          AL.Members.clear();
          AL.next!.Members.clear();
          if vertex_domain.dist.idxToLocale(s) == here {
            // Establish the initial level sets for the breadth-first
            // traversal from s
            AL.Members.add(s);
            BCaux[s].min_distance.write(0);
            f2(BCaux, s);
          }
          barrier.barrier();

          var current_distance : int = 0;
  
          while remaining do {
	    // ------------------------------------------------
	    // expand the neighbor sets for all vertices at the
	    // current distance from the starting vertex  s
	    // ------------------------------------------------
      
	    current_distance += 1;

            // The Chapel compiler is still a bit conservative when it
            // comes to forwarding read-only variables to remote
            // locales, so we make a const copy here to insure it is
            // forwarded to the remote locale in the following
            // coforall loop.
            const current_distance_c = current_distance;
            pragma "dont disable remote value forwarding"
            inline proc f3(BCaux, v, u, current_distance_c, Active_Level, out dist_temp) {

                  // --------------------------------------------
                  // add any unmarked neighbors to the next level
                  // --------------------------------------------
  
                  if  BCaux[v].min_distance.compareAndSwap(-1, current_distance_c) {
                    Active_Level[here.id]!.next!.Members.add (v);
                    if VALIDATE_BC then
                      dist_temp = current_distance_c;
                  }


                  // ------------------------------------------------
                  // only neighbors of  u  that are in the next level
                  // are on shortest paths from s through v.  Some
                  // task will have set  min_distance (v) by the
                  // time this code is reached, whether  v  lies in
                  // the previous, the current or the next level.
                  // ------------------------------------------------
  
                  if BCaux[v].min_distance.read() == current_distance_c {
                    BCaux[v].path_count$.add(BCaux[u].path_count$.read());
                    //f3(BCaux, v, u);
                    BCaux[u].children_list.add_child (v);
                  }

            }

            const AL = Active_Level[here.id]!;

            forall u in AL.Members do {
              forall v in G.FilteredNeighbors(u) do on vertex_domain.dist.idxToLocale(v) {
                      var dist_temp: real;
                      f3(BCaux, v, u, current_distance_c, Active_Level, dist_temp);
                      if VALIDATE_BC && dist_temp != 0 then
                        Lcl_Sum_Min_Dist.add(dist_temp);
                }
            };

            // This barrier is needed to insure all updates to the next
            // level are completed before updating to use the next level

            // do some work while we wait
            // barrier.notify(); // This is expensive without network atomics
            //  for now, just do a normal barrier

            if AL.next!.next == nil {
              AL.next!.next = new unmanaged Level_Set (Sparse_Vertex_List);
              AL.next!.next!.previous = AL.next;
              AL.next!.next!.next = nil;
            } else {
              AL.next!.next!.Members.clear();
            }
            // barrier.wait(); // ditto
            barrier.barrier();
            if here.id==shere {
              remaining = false;
            }

            barrier.barrier();
            Active_Level[here.id] = AL.next!;
            if Active_Level[here.id]!.Members.size:bool then
              remaining = true;

            barrier.barrier();

          };  // end forward pass

          if VALIDATE_BC then
            if here.id==0 then
              Sum_Min_Dist$.add(Lcl_Sum_Min_Dist.read());

          // -------------------------------------------------------------
          // compute the dependencies recursively, traversing the vertices 
          // of the graph in non-increasing order of distance (reverse 
          // ordering from the initial traversal)
          // -------------------------------------------------------------

          const graph_diameter = current_distance - 1;

          if DEBUG_KERNEL4 then 
            if here.id==0 then
              writeln ( " graph diameter from starting node ", s, 
                        "  is ", graph_diameter );

          pragma "dont disable remote value forwarding"
          inline proc f4(BCaux, Between_Cent$, u) {
            BCaux[u].depend = + reduce [v in BCaux[u].children_list.Row_Children[1..BCaux[u].children_list.child_count.read()]]
              ( BCaux[u].path_count$.read() / 
                BCaux[v].path_count$.read() )      *
              ( 1.0 + BCaux[v].depend );
            Between_Cent$(u).add(BCaux[u].depend);
          }

          // back up to last level
          var curr_Level =  Active_Level[here.id]!.previous!;
  
          for current_distance in 2 .. graph_diameter by -1 {
            curr_Level = curr_Level.previous!;

            for u in curr_Level.Members do on vertex_domain.dist.idxToLocale(u) {
                f4(BCaux, Between_Cent$, u);
            }

            barrier.barrier();
          }
        }

        TPVM.releaseTPV(tid);

      }; // closure of outer embarrassingly parallel forall

      if PRINT_TIMING_STATISTICS then {
	stopwatch.stop ();
	var K4_time = stopwatch.elapsed ();
	stopwatch.clear ();
	writeln ( "Elapsed time for Kernel 4: ", K4_time, " seconds");

	const n_edges          = G.num_edges;
	const N_VERTICES       = vertex_domain.size;
	const N_START_VERTICES = if starting_vertices == G.vertices
                                 then N_VERTICES
                                      - + reduce [v in vertex_domain]
                                        (G.n_Neighbors (v) == 0)
                                 else starting_vertices.size;
	const TEPS             = 7.0 * N_VERTICES * N_START_VERTICES / K4_time;
	const Adjusted_TEPS    = n_edges * N_START_VERTICES / K4_time;

	writeln ( "                     TEPS: ", TEPS );
	writeln ( " edge count adjusted TEPS: ", Adjusted_TEPS );
      }

      if VALIDATE_BC then
        Sum_Min_Dist = Sum_Min_Dist$.read();
      
      Between_Cent = Between_Cent$.read();

      if DELETE_KERNEL4_DS {
        coforall tpvElem in TPV do on tpvElem {
          const tpv = tpvElem!;
          var al = tpv.Active_Level;
          coforall loc in Locales do on loc {
            var level: unmanaged Level_Set? = al[here.id];
            var prev = level!.previous;
            while prev != nil {
              var p2 = prev!.previous;
              delete prev;
              prev = p2;
            }
            while level != nil {
                var l2 = level!.next;
                delete level;
                level = l2;
            }
          }
          delete tpv;
        }
        delete TPVM;
      }

    } // end of Brandes' betweenness centrality calculation




  //
  // Addition support data structures for kernel 4
  //

  // ------------------------------------------------------------
  // The set of vertices at a particular distance from s form a
  // level set.  The class allows the full set of vertices to be
  // partitioned into a linked list of level sets.  Each instance
  // of the outer loop in kernel 4 creates such a partitioning.
  // ============================================================

  class Level_Set {
    type Sparse_Vertex_List;
    var Members  : Sparse_Vertex_List;
    var previous : unmanaged Level_Set (Sparse_Vertex_List)?;
    var next : unmanaged Level_Set (Sparse_Vertex_List)?;
  }

  //
  // Data structure to save Children lists between the forward
  //  and backwards passes
  //
  record child_struct {
    type vertex;
    var nd: domain(1);
    var Row_Children: [nd] vertex;
    var child_count: atomic int;

    // This function should only be called using unique vertices
    proc add_child ( new_child: vertex ) {
      var c = child_count.fetchAdd(1)+1;
      Row_Children[c] = new_child;
    }
  }

  //
  // Implementation of task-private variables for kernel 4
  //
  record taskPrivateArrayData {
    type vertex;
    var min_distance  : chpl__processorAtomicType(int); // used only on home locale
    var path_count$   : atomic real;
    var depend        : real;
    var children_list : child_struct(vertex);
  }

  class taskPrivateData {
    type Sparse_Vertex_List;
    const vertex_domain;
    var used  : atomic bool;
    var BCaux : [vertex_domain] taskPrivateArrayData(index(vertex_domain));
    var Active_Level : [PrivateSpace] unmanaged Level_Set (Sparse_Vertex_List)?;
  }

  // This is a simple class that hands out task private variables from the
  // array of task private variables, TPV.
  class TPVManager {
    const TPV;
    var currTPV: atomic int;
    proc gettid() {
      const tid = this.currTPV.fetchAdd(1)%numTPVs;
      on this.TPV[tid] do
        while this.TPV[tid]!.used.testAndSet() do chpl_task_yield();
      return tid;
    }
    proc getTPV(tid) {
      return this.TPV[tid]!;
    }
    proc releaseTPV(tid) {
      this.TPV[tid]!.used.clear();
    }
  }
}
