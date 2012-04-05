module analyze_torus_graphs {

  // +========================================================================+
  // |  Define dense arithmetic array-based representations for               |
  // |  k-dimensional torus graphs. For each k, provide execution template    |
  // |  for executing and verifying SSCA2 kernels 2 through 4.                |
  // +========================================================================+


  use SSCA2_compilation_config_params, 
      SSCA2_execution_config_consts,
      SSCA2_driver, 
      array_rep_torus_graph_generator,
      torus_graph_generator_utilities, 
      BlockDist;


  // ====================================================================
  // generic class for array-based graph for torus of any dimensionality.
  // ====================================================================

  class torus_array_graph {
    const vertex_domain;
    param dimensions = vertex_domain.rank;
    type  vertex     = index (vertex_domain);
    const vertices   = vertex_domain;

    var   Neighbors   : [vertex_domain] [1..2*dimensions] vertex;
    var   edge_weight : [vertex_domain] [1..2*dimensions] int;
    proc   n_Neighbors (v : index (vertex_domain) ) return 2*dimensions;
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

    var exact_between_centrality = 0.25 * (d - 2) * d  -  d/2 + 1;

    writeln ();
    writeln ("One D Torus: all nodes should have",
	     " exact betweenness centrality: ", exact_between_centrality );
    writeln ("             (upper bound on approximate or filtered", 
                         " betweenness centrality)");
    writeln ();
    
    var G = new torus_array_graph (vertex_domain);

    writeln ( "--------------------------------" );
    writeln ( "One D Torus with ", d, " nodes" );
    writeln ( "array representation of torus" );
    writeln ( "--------------------------------" );
    writeln ();
	  
    gen_1D_torus_array_representation ( G );

    torus_random_edge_weights (MAX_EDGE_WEIGHT, G );

    execute_SSCA2 ( G );
    delete G;

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

    var G = new torus_array_graph (vertex_domain);
	  
    writeln ( "--------------------------------" );
    writeln ( "Two D Torus with ", d1, " x ", d2, " nodes" );
    writeln ( "array representation of torus" );
    writeln ( "--------------------------------" );
    writeln ();

    gen_2D_torus_array_representation ( G );

    torus_random_edge_weights ( MAX_EDGE_WEIGHT, G );

    execute_SSCA2 ( G );
    delete G;

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

    var G = new torus_array_graph (vertex_domain);
	  
    writeln ( "--------------------------------" );
    writeln ( "Three D Torus with ", d1, " x ", d2, " x ", d3, 
	      " nodes" );
    writeln ( "array representation of torus" );
    writeln ( "--------------------------------" );
    writeln ();
    

    gen_3D_torus_array_representation ( G );

    torus_random_edge_weights ( MAX_EDGE_WEIGHT, G );

    execute_SSCA2 ( G );
    delete G;

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

    var G = new torus_array_graph (vertex_domain);
	  
    writeln ( "--------------------------------" );
    writeln ( "Four D Torus with ", d1, " x ", d2, 
	      " x ", d3, " x ", d4, " nodes" );
    writeln ( "array representation of torus" );
    writeln ( "--------------------------------" );
    writeln ();

    gen_4D_torus_array_representation ( G );

    torus_random_edge_weights ( MAX_EDGE_WEIGHT, G );

    execute_SSCA2 ( G );
    writeln (); writeln ();
    delete G;

  } // end of 4D torus

}