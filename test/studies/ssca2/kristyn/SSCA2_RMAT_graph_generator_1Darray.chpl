module SSCA2_RMAT_graph_generator_1Darray

{
  // +=========================================================================+
  // |             RMAT approximate power law graph generator                  |
  // |                                 and                                     |
  // |                           SSCA #2 Kernel 1                              |
  // |                                                                         |
  // |  Generate approximate power law graph                                   |
  // |                                                                         |
  // |  This procedure accepts a generic graph representation which must       |
  // |  provide only the capabilities to:                                      |
  // |    1. add a vertex to a neighbor list for some other vertex             |
  // |    2. assign an integer weight to an entry in an associated weight list |
  // |                                                                         |
  // |  The RMAT procedure implicitly assumes that vertices are integers       |
  // |  in the range [0, 2^SCALE].  So vertices are integers in this           |
  // |  procedure and edges are pairs of integers.                             |
  // |                                                                         |
  // |  This implementation first generates the RMAT graph as a list of        |
  // |  triples, using an array of edges and an associated array of weights.   |
  // |  These are transformed into the Chapel graph representation using       |
  // |  native Chapel syntax for associative and sparse domains and arrays.    |
  // |  That transformation is Kernel 1 of the SSCA #2 benchmark.              |
  // |                                                                         |
  // |  The code uses two auxiliary procedures for clarity and to help the     |
  // |  compiler use more abstract, higher level, code.                        |
  // +=========================================================================+

  use SSCA2_compilation_config_params;

  record directed_vertex_pair {
    var start = 1: int;
    var end   = 1: int; 
  }
    
  proc +(l: directed_vertex_pair, r: directed_vertex_pair) 
      return new directed_vertex_pair (l.start + r.start, l.end + r.end);


  // ============================
  // Quadrant selection algorithm
  // ============================

  proc assign_quadrant ( u: real, a: real, b: real, c: real, d : real, 
			bit : int ) : directed_vertex_pair
    {
      // ---------------------------------------------------------------------
      // The heart of the RMAT random graph generator is a procedure that
      // assigns a single bit of the edge starting and ending vertex by
      // assigning the edge with specified probability to one of the four
      // quadrants of a 2 x 2 grid.
      //
      // Chapel is able to promote this conditional-based scalar procedure to
      // array operations where it is not able to promote conditional code
      // directly. 
      //
      // Determine randomly in which quadrant of the grid a point lies, 
      // based on the following picture:
      //
      //     +---------------------------+
      //     | u < a         | u < a+b   |
      //     |-------------+-------------|
      //     | u < a + b + c | otherwise |
      //     +---------------------------+
      //
      // The probability of the edge falling in the upper left quadrant is  a,
      // in the upper right quadrant, b, the lower left quadrant  c
      // and the lower right quadrant  d, where the probabilities are
      // normalized to sum to one.
      // ---------------------------------------------------------------------

      var start_inc = 0;
      var end_inc   = 0;
      var edge : directed_vertex_pair;

      if u <= a then
	{}
      else if u <= a + b then
	{ end_inc = 1;}
      else if u <= a + b + c then
	{ start_inc = 1;}
      else
	{ start_inc = 1; end_inc = 1;};

      edge.start = bit * start_inc;
      edge.end   = bit * end_inc;
      return ( edge );
    }


  // ====================================
  // Main RMAT Graph Generation Procedure (1D Array)
  // ====================================~

proc Gen_RMAT_graph_1D_array ( a : real, b : real, c : real, d : real, 
		       SCALE :int, 
		       N_VERTICES : int,
		       n_raw_edges : int,
		       MAX_EDGE_WEIGHT :int,
		       G ) 
{ 
      use BlockDist;
      use Random; 
      use Time;

      const vertex_range = 1..N_VERTICES, 
	    edge_range   = 1..n_raw_edges;

// The data structure used to store the edges is an array of records

  const edgelist_domain =
    if DISTRIBUTION_TYPE == "BLOCK" then
      [1..n_raw_edges] dmapped Block ( [1..n_raw_edges] )
    else
      [1..n_raw_edges] ;


      // Random Numbers return in the range [0.0, 1.0)

      var Rand_Gen = if REPRODUCIBLE_PROBLEMS then 
	               new RandomStream (seed = 0556707007)
		     else
		       new RandomStream ();

      var   Noisy_a      : [edgelist_domain] real, 
	    Noisy_b      : [edgelist_domain] real, 
            Noisy_c      : [edgelist_domain] real, 
            Noisy_d      : [edgelist_domain] real, 
            norm         : [edgelist_domain] real,  
	    Unif_Random  : [edgelist_domain] real,
	    Unif_Random2 : [edgelist_domain] real;

      var   Edges:[edgelist_domain] directed_vertex_pair;

      var   graph_gen_time: Timer;

      // Step 0: Construct permutation array

      graph_gen_time.clear();
      graph_gen_time.start();

      var permutation$ : [G.vertices] sync int = vertex_range;

      // Note: need to be very careful with sync variables as a writeln
      // invokes a read, which then sets to empty

      for i in 1..SCALE do {
         var   skip : real;
         Rand_Gen.fillRandom ( Unif_Random );
         skip = Rand_Gen.getNext ();
         Rand_Gen.fillRandom ( Unif_Random2 );

         forall j in edgelist_domain do
         {

          // Choose two locations at random
          var ndx1 = floor (1 + Unif_Random (j) * N_VERTICES) : int;
          var ndx2 = floor (1 + Unif_Random2(j) * N_VERTICES) : int;

          // If the locations are not the same, then swap
          if (ndx1 != ndx2){

            // If the first location is greater than the second, swap. Insures
            // that the locations are locked in order, preventing deadlock
            if (ndx1 > ndx2) {ndx1 <=> ndx2;};

            // Lock locations in permutation array

            var label1 = permutation$ (ndx1).readFE () : int;
            var label2 = permutation$ (ndx2).readFE () : int;

            // Swap labels

            permutation$ (ndx1).writeEF (label2);
            permutation$ (ndx2).writeEF (label1);

          };

        };
     };
      graph_gen_time.stop();
      writeln("Time for SDG: Construct Perm Array  = ", graph_gen_time.elapsed());

// Step 1: Construct edges
      graph_gen_time.clear();
      graph_gen_time.start();


      // ---------------------------------------------------------------
      // The RMAT algorithm is based on recursively sub-dividing a grid.
      // In this case, the grid is square of order 2^SCALE by 2^SCALE.
      // So exactly "SCALE" steps of recursion will be required and the
      // recursion can be implemented directly by iteration.
      // ---------------------------------------------------------------

      // -----------------------------------------------------------------
      // Note on random number generators -- the RMAT generator creates
      // 5*SCALE vectors of length 2**SCALE.  The dependence on powers
      // of two provides an opportunity to expose statistical correlations
      // in the pseudo-random numbers.  This certainly occurs with the
      // current Chapel Random module.  The "skips" in the random number
      // sequence dramatically change the results.  Without them, the
      // Chapel RMAT matrices are inconsistent with what is seen in
      // other implementations.
      // -----------------------------------------------------------------
      
      writeln ("Random graph generated by stride of 1 in one random stream",
	       " with skips" );

      var bit = 1 << SCALE;
      var   skip : real;

      for depth in 1..SCALE do {
	  bit >>= 1;

	  // randomize the coefficients, tweaking them by numbers in [-.05, .05)

	  skip = Rand_Gen.getNext ();
	  Rand_Gen.fillRandom (Unif_Random);
	  Noisy_a = a * (0.95 + 0.1 * Unif_Random);

	  skip = Rand_Gen.getNext ();
	  Rand_Gen.fillRandom (Unif_Random);
	  Noisy_b = b * (0.95 + 0.1 * Unif_Random);

	  skip = Rand_Gen.getNext ();
	  Rand_Gen.fillRandom (Unif_Random);
	  Noisy_c = c * (0.95 + 0.1 * Unif_Random);

	  skip = Rand_Gen.getNext ();
	  Rand_Gen.fillRandom (Unif_Random);
	  Noisy_d = d * (0.95 + 0.1 * Unif_Random);

	  norm     = 1.0 / (Noisy_a + Noisy_b + Noisy_c + Noisy_d);
	  Noisy_a *= norm;
	  Noisy_b *= norm;
	  Noisy_c *= norm;
	  Noisy_d *= norm;

	  skip = Rand_Gen.getNext ();
	  Rand_Gen.fillRandom (Unif_Random);

	  Edges += assign_quadrant ( Unif_Random, Noisy_a, Noisy_b, 
				     Noisy_c, Noisy_d, bit );
      
      };

      graph_gen_time.stop();
      writeln("Time for SDG: Construct Edges  = ", graph_gen_time.elapsed());

      // ---------------------------------------------------------------------
      // Assign weights to edges randomly, then randomly relabel the vertices
      // to avoid locality from the obvious imbalance that will arise when
      // one of the coefficients is clearly larger than the others
      // ---------------------------------------------------------------------
      
      var Edge_Weight : [edgelist_domain] int;

      Rand_Gen.fillRandom ( Unif_Random  );
      Edge_Weight = floor (1 + Unif_Random * MAX_EDGE_WEIGHT) : int;

      graph_gen_time.clear();
      graph_gen_time.start();

      forall e in edgelist_domain do {
         Edges(e).start = permutation$ (Edges(e).start).readFF();
         Edges(e).end   = permutation$ (Edges(e).end  ).readFF();
      };
      graph_gen_time.stop();
      writeln("Time for SDG: Permute labels  = ", graph_gen_time.elapsed());


      if DEBUG_GRAPH_GENERATOR || DEBUG_WEIGHT_GENERATOR then {
	writeln ();
	for e in edgelist_domain do
	  writeln ("edge (", e, ") : (", Edges(e), ", ", 
		   Edge_Weight (e), ")" );
      }

      // --------------------------
      // Graph consistency checking
      // --------------------------

      assert ( Edges.start > 0, 
	       "edge start vertices out of low end of range");

      assert ( Edges.end > 0, 
	       "edge end vertices out of low end of range");

      assert ( Edges.start <= 2**SCALE, 
	       "edge start vertices out of high end of range");

      assert ( Edges.end <= 2**SCALE, 
	       "edge end vertices out of high end of range");

      assert ( Edge_Weight > 0, 
	       "edge weight out of low end of range");

      assert ( Edge_Weight <= MAX_EDGE_WEIGHT,  
	       "edge weight out of high end of range");

      writeln (); writeln ("Vertex Set in G:", G.vertices);

      // ----------------------------------------------------------
      // Kernel 1: assemble graph from list of triples.  Include
      // only non-self incident edges.  in case of duplicates, last
      // in wins (+= instead of = works to take sum of weights)
      // ----------------------------------------------------------

      var Histogram$ : [G.vertices] sync int = 0;

      // Generate a histogram from the Edges to guide the distribution 
      // of the graph

      forall e in Edges do {
         var u = e.start;
         var v = e.end;
         Histogram$[u] += 1;
         Histogram$[v] += 1;
      }

      // Resize Neighbor lists for each vertex based on Histogram

      forall v in G.vertices {
          G.Row[v].nd = [1..Histogram$[v].readFF()];
      }

//    Note that duplicate edges and self-edges are now included with
//    the vertex_struct

      forall e in edgelist_domain do {
	var u: index(G.vertices) = Edges(e).start;
	var v: index(G.vertices) = Edges(e).end  ;
        if ( v != u) then {
	   G.Row (u).add_Neighbor (v, Edge_Weight(e));
        }
        else {
           G.Row (u).add_self_edge();
        }
      }

      // Resize Neighbor lists and associated weights to account for 
      // skipped duplicates and self edges

      forall v in G.vertices {
          G.Row[v].nd = [1..G.Row[v].neighbor_count];
      }

      var self_edges = (+ reduce [v in G.Row] v.self_edges );
      var duplicates = (+ reduce [v in G.Row] v.duplicates );

      writeln ( "# of raw edges generated  ", n_raw_edges );
      writeln ( "# of duplicate edges      ", duplicates  );
      writeln ( "# of self edges           ", self_edges );
      writeln ( "# of edges in final graph ", 
		+ reduce [v in G.vertices] G.n_Neighbors (v) );

      if DEBUG_GRAPH_GENERATOR || DEBUG_WEIGHT_GENERATOR then {
	writeln ();
	writeln ("tuples denote (edge, weight)");
	writeln ();
	for u in G.vertices do {
	  write ( "row ", u, ": [", G.n_Neighbors (u), "] " );
	  for (v,w) in (G .Neighbors (u), G.edge_weight (u) ) do
	    write ( (v, w) );
	  writeln (); 
	}
      }

      var max_edges = max reduce [v in vertex_range] G.n_Neighbors (v);

      var edge_count : [0..max_edges] int = 0;

      for v in G.vertices do
	edge_count (G.n_Neighbors (v)) += 1;

      writeln ("histograph of node distributions by number of outgoing edges");
      writeln ( "# edges  # nodes");
      for count in 0..max_edges do
	writeln ( count, "  ", edge_count (count) );

      writeln ();
      
    }
}

