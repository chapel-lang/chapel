module torus_graph_generator_utilities {

  // ----------------------------------------------------------------
  // Assign random integer edge weights to regular tori graphs.
  // This procedure is polymorphic over torus dimensionality and over
  // graph representation as long as vertices and edge weights can be
  // iterated together in zipper fashion.
  // ----------------------------------------------------------------

  proc torus_random_edge_weights ( MAX_EDGE_WEIGHT : int, G ) {
    use SSCA2_compilation_config_params, Random;

    var Rand_Gen = if REPRODUCIBLE_PROBLEMS then 
                     new unmanaged NPBRandomStream (seed = 8737935341)
		    else
		      new unmanaged NPBRandomStream ();

    const n_neighbors = 2*G.dimensions;
    var Unif_Random : [1..n_neighbors] real;

    if DEBUG_WEIGHT_GENERATOR then {
      writeln ();
      writeln ( "maximum edge weight:", MAX_EDGE_WEIGHT);
      writeln ();
      writeln ('vertex:   weights');
    };

    for v in G.vertices do
      {
	Rand_Gen.fillRandom ( Unif_Random );

      /* Issue #12424: instead of the for-loop below, we would like to write:
	G.edge_weight (v) = floor ( 1 + 
				    Unif_Random * MAX_EDGE_WEIGHT )
	  : int;
      */
        for (w,r) in zip(G.edge_weight(v), Unif_Random) do
          w = floor ( 1 + r * MAX_EDGE_WEIGHT ) : int;

	if DEBUG_WEIGHT_GENERATOR then {
	  writeln ( v, ": ", G.edge_weight (v) );}
      }

    // ---------------------------------------------------------------------
    // We would like to write the above as:  
    //       Rand_Gen.fillRandom ( Unif_Random );
    //       G.edge_weight = floor ( 1 + Unif_Random * MAX_EDGE_WEIGHT ):int
    // Neither library nor compiler support this yet for tuples.
    // ---------------------------------------------------------------------

    delete Rand_Gen;
  }
}

