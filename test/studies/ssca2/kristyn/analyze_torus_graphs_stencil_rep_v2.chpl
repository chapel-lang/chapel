module analyze_torus_graphs {

  // +========================================================================+
  // |  Define sparse subdomain stencil representations for                   |
  // |  k-dimensional torus graphs. For each k, provide execution template    |
  // |  for executing and verifying SSCA2 kernels 2 through 4.                |
  // +========================================================================+
  // |                          VERSION 2                                     |
  // |  Semi-implicit version computes neighbor vertex indices by storing     |
  // |  a complete array of neighbor indices over the domain of vertices      |
  // |  extended by one in each plane.  Its storage overhead is slightly      |
  // |  larger than one tuple per vertex.                                     |
  // +========================================================================+


  use SSCA2_compilation_config_params, 
      SSCA2_execution_config_consts,
      SSCA2_driver, 
      torus_graph_generator_utilities,
      BlockDist;


  // =====================================================================
  // generic class for stencil-based graph for torus of any dimensionality
  // =====================================================================

  class torus_semi_implicit_stencil_graph {
    const vertex_domain;
    const dense_stencil;
    const torus_vertex;
    param dimensions = vertex_domain.rank;

    var   torus_stencil : sparse subdomain (dense_stencil);
    const vertices = vertex_domain;
    
    iter   Neighbors (v : index (vertices) ){
      for s in torus_stencil do
	yield torus_vertex (v+s);
    }

    var edge_weight : [vertex_domain] [torus_stencil] int;
    proc n_Neighbors (v : index (vertices) ) return 2*dimensions;
  }


 // ======================================================================
  // As of March 2010, one tuples and integers are sufficiently different
  // that the one-dimensional case is different and awkward.  That
  // distinction affects this code in that we must use a less clean syntax
  // to reference the low and high bounds of the index ranges for the vertex 
  // domain in the general "analyze_nD_torus..." routine.
  // ======================================================================

  // =====================================================================
  // generic procedure to document and execute kernels 2, 3 and 4 of SSCA2
  // =====================================================================
  
  proc analyze_nD_torus_semi_implicit_stencil_graph 
                                         ( const vertex_domain : domain,
					   const dense_stencil : domain,
					   const torus_vertex  ) {

    var G = new torus_semi_implicit_stencil_graph (vertex_domain, 
						   dense_stencil,
						   torus_vertex);

    writeln ( "------------------------------------" );
    writeln ( "Torus of dimensionality ", G.dimensions ) ;
    writeln ( " dimension  lower  upper" );
    writeln ( "            bound  bound" );

    for d in 0 .. #G.dimensions do
      writef ("%{########}%{########}%{########}\n", 
              d, vertex_domain.dim(d).low, vertex_domain.dim(d).high);

    writeln ( "semi-implicit stencil representation");
    writeln ( "------------------------------------" );
    writeln ();
	  
    // ------------------------------------------------------
    // create stencil for torus connections
    // (must happen after graph object is constructed because
    //  sparse subdomains are local to graph object.)
    // ------------------------------------------------------

    if dense_stencil.rank > 1 then
      for s in dense_stencil do {
	if ( + reduce [d in 1..G.dimensions] abs (s(d)) ) == 1 then
	  G.torus_stencil.add (s);
      }
    else 
      { G.torus_stencil.add (-1);  G.torus_stencil.add (+1);}

    if DEBUG_GRAPH_GENERATOR then {
      writeln ("vertex   Neighbors");
      forall i in G.vertices do 
	writeln ( i, ":    ", G.Neighbors (i) );
    }
    
    torus_random_edge_weights ( MAX_EDGE_WEIGHT, G );

    execute_SSCA2 ( G );
    delete G;
  }   


  // ====================================================
  // compute graph of one dimensional torus with  2^SCALE
  // vertices, then execute Kernels 2, 3 and 4 of SSCA #2
  // ====================================================

  proc generate_and_analyze_1D_torus {

    const d = 2**SCALE;

    const vertex_domain = 
      if DISTRIBUTION_TYPE == "BLOCK" then
        [ Torus_Base_Index..#d ] dmapped Block ( [Torus_Base_Index..#d] )
      else
        [ Torus_Base_Index..#d ];

    const dense_stencil = [-1..1];
    type  vertex = index (vertex_domain);

    const extended_domain = vertex_domain.expand(1);
    var   torus_vertex : [extended_domain] vertex = extended_domain;

    // ----------------------------------------------------------------
    // generate auxiliary data supporting a one dimensional torus graph
    // torus_vertex wraps the torus with an additional plane in each
    // dimension. fill in the wrap-around vertices in the ghost cells.
    // ----------------------------------------------------------------

    const h = vertex_domain.high, l = vertex_domain.low;
    torus_vertex (l-1) = torus_vertex (h);
    torus_vertex (h+1) = torus_vertex (l);

    // --------------------------------------
    // create the graph and run SSCA2 kernels
    // --------------------------------------

    const exact_between_centrality = 0.25 * (d - 2) * d  -  d/2 + 1;

    writeln ();
    writeln ("One D Torus: all nodes should have",
	     " exact betweenness centrality: ", exact_between_centrality );
    writeln ("             (upper bound on approximate or filtered", 
                         " betweenness centrality)");
    writeln ();

    analyze_nD_torus_semi_implicit_stencil_graph ( vertex_domain, 
						   dense_stencil,
						   torus_vertex );

    writeln (); writeln ();
  }


  // ==============================================
  // compute graph of an approximately square two 
  // dimensional torus with  2^SCALE vertices, then 
  // execute Kernels 2, 3 and 4 of SSCA #2
  // ==============================================

  proc generate_and_analyze_2D_torus {

    const lg2d1 : int = SCALE / 2,
          lg2d2 = SCALE - lg2d1,
          d1    = 2**lg2d1,
          d2    = 2**lg2d2;

    const vertex_domain = 
     if DISTRIBUTION_TYPE == "BLOCK" then
       [ Torus_Base_Index..#d1,
	 Torus_Base_Index..#d2 ] 
	 dmapped Block ( [ Torus_Base_Index..#d1,
			   Torus_Base_Index..#d2 ] )
     else
       [ Torus_Base_Index..#d1,
	 Torus_Base_Index..#d2 ];

    const dense_stencil = [-1..1, -1..1];

    type  vertex = index (vertex_domain);

    const extended_domain = vertex_domain.expand(1);
    var   torus_vertex : [extended_domain] vertex = extended_domain;

    // ----------------------------------------------------------------
    // generate auxiliary data supporting a two dimensional torus graph
    // torus_vertex wraps the torus with an additional plane in each
    // dimension. fill in the wrap-around vertices in the ghost cells.
    // ----------------------------------------------------------------

    const h1 = vertex_domain.high(1), l1 = vertex_domain.low (1);
    const h2 = vertex_domain.high(2), l2 = vertex_domain.low (2);
    torus_vertex (l1-1, ..) = torus_vertex (h1, ..);
    torus_vertex (h1+1, ..) = torus_vertex (l1, ..);
    torus_vertex (.., l2-1) = torus_vertex (.., h2);
    torus_vertex (.., h2+1) = torus_vertex (.., l2);

    // --------------------------------------
    // create the graph and run SSCA2 kernels
    // --------------------------------------

    var exact_between_centrality : real;

    if ( SCALE % 2 == 1 ) then
      exact_between_centrality = 0.75 * 2 ** ( (3*SCALE-1) / 2 )
                                 - 2**SCALE + 1;
    else
      exact_between_centrality = 2 ** ( (3*SCALE/2) - 1)
	                         - 2**SCALE + 1;

    writeln ();
    writeln ("Two D Torus: all nodes should have",
	     " exact betweenness centrality: ", exact_between_centrality );
    writeln ("             (upper bound on approximate or filtered", 
                         " betweenness centrality)");
    writeln ();

    analyze_nD_torus_semi_implicit_stencil_graph (vertex_domain, 
						  dense_stencil,
						  torus_vertex);

    writeln (); writeln ();
  }


  // ===============================================
  // compute graph of an approximately cubical three
  // dimensional torus with  2^SCALE vertices, then 
  // execute Kernels 2, 3 and 4 of SSCA #2
  // ===============================================
	    
  proc generate_and_analyze_3D_torus {

    const lg2d1 : int = SCALE / 3,
          lg2d2 : int = lg2d1,
          lg2d3 = SCALE - ( lg2d1 + lg2d2);

    const d1 = 2**lg2d1,
          d2 = 2**lg2d2,
          d3 = 2**lg2d3;

    const vertex_domain = 
     if DISTRIBUTION_TYPE == "BLOCK" then
       [ Torus_Base_Index..#d1, 
	 Torus_Base_Index..#d2, 
	 Torus_Base_Index..#d3 ]
	 dmapped Block ( [ Torus_Base_Index..#d1,
			   Torus_Base_Index..#d2, 
			   Torus_Base_Index..#d3 ] )
     else
       [ Torus_Base_Index..#d1, 
	 Torus_Base_Index..#d2, 
	 Torus_Base_Index..#d3 ];

    const dense_stencil = [-1..1, -1..1, -1..1];

    type  vertex = index (vertex_domain);

    const extended_domain = vertex_domain.expand(1);
    var   torus_vertex : [extended_domain] vertex = extended_domain;

    // ----------------------------------------------------------------
    // generate auxiliary data supporting a two dimensional torus graph
    // torus_vertex wraps the torus with an additional plane in each
    // dimension. fill in the wrap-around vertices in the ghost cells.
    // ----------------------------------------------------------------

    const h1 = vertex_domain.high(1), l1 = vertex_domain.low (1);
    const h2 = vertex_domain.high(2), l2 = vertex_domain.low (2);
    const h3 = vertex_domain.high(3), l3 = vertex_domain.low (3);
    torus_vertex ( l1-1, .., .. ) = torus_vertex ( h1, .., .. );
    torus_vertex ( h1+1, .., .. ) = torus_vertex ( l1, .., .. );
    torus_vertex ( .., l2-1, .. ) = torus_vertex ( .., h2, .. );
    torus_vertex ( .., h2+1, .. ) = torus_vertex ( .., l2, .. );
    torus_vertex ( .., .., l3-1 ) = torus_vertex ( .., .., h3 );
    torus_vertex ( .., .., h3+1 ) = torus_vertex ( .., .., l3 );

    // --------------------------------------
    // create the graph and run SSCA2 kernels
    // --------------------------------------

    analyze_nD_torus_semi_implicit_stencil_graph (vertex_domain, 
						  dense_stencil,
						  torus_vertex);

    writeln (); writeln ();
  }


  // ============================================================
  // compute graph of an approximately four-cubical torus with
  // 2^SCALE vertices, then execute Kernels 2, 3 and 4 of SSCA #2
  // ============================================================

  proc generate_and_analyze_4D_torus {

    const lg2d1 : int = SCALE / 4,
          lg2d2 : int = lg2d1,
          lg2d3 : int = lg2d1,
          lg2d4 = SCALE - ( lg2d1 + lg2d2 + lg2d3);

    const d1 = 2**lg2d1,
          d2 = 2**lg2d2,
          d3 = 2**lg2d3,
          d4 = 2**lg2d4;

    const vertex_domain = 
     if DISTRIBUTION_TYPE == "BLOCK" then 
       [ Torus_Base_Index..#d1, 
	 Torus_Base_Index..#d2, 
	 Torus_Base_Index..#d3, 
	 Torus_Base_Index..#d4 ]
	 dmapped Block ( [ Torus_Base_Index..#d1,
			   Torus_Base_Index..#d2, 
			   Torus_Base_Index..#d3, 
			   Torus_Base_Index..#d4 ])
     else
       [ Torus_Base_Index..#d1, 
	 Torus_Base_Index..#d2, 
	 Torus_Base_Index..#d3, 
	 Torus_Base_Index..#d4 ];

    const dense_stencil = [-1..1, -1..1, -1..1, -1..1 ];

    type  vertex = index (vertex_domain);

    const extended_domain = vertex_domain.expand(1);
    var   torus_vertex : [extended_domain] vertex = extended_domain;

    // ----------------------------------------------------------------
    // generate auxiliary data supporting a two dimensional torus graph
    // torus_vertex wraps the torus with an additional plane in each
    // dimension. fill in the wrap-around vertices in the ghost cells.
    // ----------------------------------------------------------------

    const h1 = vertex_domain.high(1), l1 = vertex_domain.low (1);
    const h2 = vertex_domain.high(2), l2 = vertex_domain.low (2);
    const h3 = vertex_domain.high(3), l3 = vertex_domain.low (3);
    const h4 = vertex_domain.high(4), l4 = vertex_domain.low (4);
    torus_vertex ( l1-1, .., .., .. ) = torus_vertex ( h1, .., .., .. );
    torus_vertex ( h1+1, .., .., .. ) = torus_vertex ( l1, .., .., .. );
    torus_vertex ( .., l2-1, .., .. ) = torus_vertex ( .., h2, .., .. );
    torus_vertex ( .., h2+1, .., .. ) = torus_vertex ( .., l2, .., .. );
    torus_vertex ( .., .., l3-1, .. ) = torus_vertex ( .., .., h3, .. );
    torus_vertex ( .., .., h3+1, .. ) = torus_vertex ( .., .., l3, .. );
    torus_vertex ( .., .., .., l4-1 ) = torus_vertex ( .., .., .., h4 );
    torus_vertex ( .., .., .., h4+1 ) = torus_vertex ( .., .., .., l4 );

    // --------------------------------------
    // create the graph and run SSCA2 kernels
    // --------------------------------------

    analyze_nD_torus_semi_implicit_stencil_graph (vertex_domain, 
						  dense_stencil,
						  torus_vertex);

    writeln (); writeln ();
  }

}