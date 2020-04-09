module analyze_torus_graphs {

  // +========================================================================+
  // |  Define sparse subdomain stencil representations for                   |
  // |  k-dimensional torus graphs. For each k, provide execution template    |
  // |  for executing and verifying SSCA2 kernels 2 through 4.                |
  // +========================================================================+
  // |                          VERSION 3                                     |
  // |  Completely implicit version computes neighbor vertex indices          |
  // |  with torus wrap-around directly for each iteration over the           |
  // |  neighbor vertices.  It requires no storage beyond actual data.        |
  // +========================================================================+


  use SSCA2_compilation_config_params, 
      SSCA2_execution_config_consts,
      SSCA2_driver, 
      torus_graph_generator_utilities,
      BlockDist;


  // ========================================
  // function to work around the distinction 
  // between a one-tuple and a simple scalar.
  // ========================================

  proc tuple_index (D: domain) type
    return if D.rank == 1 then 1*index(D) else index(D);


  // ======================================================================
  // generic class for stencil-based graph for torus of any dimensionality.
  // ======================================================================
  //
  // ======================================================================
  // As of March 2010, one tuples and integers are sufficiently different
  // that the one-dimensional case is different and awkward.  We work
  // around the distinction in four ways:
  // 1. the function above is used to make the type of a neighbor a
  //    one-tuple rather than an integer in the one-dimensional case
  // 2. the variables ending in "_" are used to transfer from the native
  //    type (int or multi-dimensional tuple) to the desired type (tuple)
  // 3. the conditional in the yield statement delivers the necessary
  //    output (int or multi-dimensional tuple)
  // 4. the less clean syntax referencing the low and high bounds of the
  //    index ranges for the vertex domain, in the class definition below
  //    and in the general "analyze_nD_torus..." routine.
  // These four changes allow the special case to be treated with the same 
  // code as used for the general multi-dimensional case.
  // ======================================================================

  class torus_fully_implicit_stencil_graph {
    const vertex_domain;
    const dense_stencil;
    param dimensions = vertex_domain.rank;

    var   torus_stencil : sparse subdomain (dense_stencil);
    const vertices = vertex_domain;

    type  vertex_tuple = tuple_index (vertex_domain);

    iter Neighbors (v_ : index (vertices) ){
      const v = if vertex_domain.rank == 1 then (v_,) else v_;
      for s_ in torus_stencil do {
    const s = if vertex_domain.rank == 1 then (s_,) else s_;
    var neighbor : vertex_tuple; 
    for d in 0..#dimensions {
        neighbor (d) =
          if v (d) + s (d) < vertex_domain.dim(d).low then
             vertex_domain.dim(d).high
          else if v (d) + s (d) > vertex_domain.dim(d).high then
             vertex_domain.dim(d).low
          else
         v (d) + s (d);
    }
    yield if vertex_domain.rank == 1 then neighbor (1) else neighbor;
      }
    }

    var edge_weight : [vertex_domain] [torus_stencil] int;
    proc n_Neighbors (v : index (vertices) ) return 2*dimensions;
  }


  // ========================================================
  // generic procedure to instantiate an n-dimensional torus,
  // document and execute kernels 2, 3 and 4 of SSCA2
  // ========================================================
  
  proc generate_and_analyze_nD_torus ( const vertex_domain : domain, 
                                      const dense_stencil : domain ) {

    // -----------------
    // instantiate graph
    // -----------------

    var G = new torus_fully_implicit_stencil_graph 
                     ( vertex_domain, dense_stencil );

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


    writeln ( "-------------------------------------" );
    writeln ( "Torus of dimensionality ", G.dimensions ) ;
    writeln ( " dimension  lower  upper" );
    writeln ( "            bound  bound" );

    for d in 0 .. #G.dimensions do
      writef ("%{########}%{########}%{########}\n", 
              d, vertex_domain.dim(d).low, vertex_domain.dim(d).high);

    writeln ( "fully implicit stencil representation");
    writeln ( "-------------------------------------" );
    writeln ();
      
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
        { Torus_Base_Index..#d } dmapped Block ( {Torus_Base_Index..#d} )
      else
        { Torus_Base_Index..#d };

    const dense_stencil = {-1..1};

    const exact_between_centrality = 0.25 * (d - 2) * d  -  d/2 + 1;

    writeln ();
    writeln ("One D Torus: all nodes should have",
         " exact betweenness centrality: ", exact_between_centrality );
    writeln ("             (upper bound on approximate or filtered", 
                         " betweenness centrality)");
    writeln ();

    generate_and_analyze_nD_torus ( vertex_domain, dense_stencil );

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
       { Torus_Base_Index..#d1,
     Torus_Base_Index..#d2 } 
     dmapped Block ( { Torus_Base_Index..#d1,
               Torus_Base_Index..#d2 } )
     else
       { Torus_Base_Index..#d1,
     Torus_Base_Index..#d2 };

    const dense_stencil = {-1..1, -1..1};

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

    generate_and_analyze_nD_torus ( vertex_domain, dense_stencil );

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
       { Torus_Base_Index..#d1, 
     Torus_Base_Index..#d2, 
     Torus_Base_Index..#d3 }
     dmapped Block ( { Torus_Base_Index..#d1,
               Torus_Base_Index..#d2, 
               Torus_Base_Index..#d3 } )
     else
       { Torus_Base_Index..#d1, 
     Torus_Base_Index..#d2, 
     Torus_Base_Index..#d3 };

    const dense_stencil = {-1..1, -1..1, -1..1};

    generate_and_analyze_nD_torus ( vertex_domain, dense_stencil );

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
       { Torus_Base_Index..#d1, 
     Torus_Base_Index..#d2, 
     Torus_Base_Index..#d3, 
     Torus_Base_Index..#d4 }
     dmapped Block ( { Torus_Base_Index..#d1,
               Torus_Base_Index..#d2, 
               Torus_Base_Index..#d3, 
               Torus_Base_Index..#d4 })
     else
       { Torus_Base_Index..#d1, 
     Torus_Base_Index..#d2, 
     Torus_Base_Index..#d3, 
     Torus_Base_Index..#d4 };

    const dense_stencil = {-1..1, -1..1, -1..1, -1..1 };

    generate_and_analyze_nD_torus ( vertex_domain, dense_stencil );

    writeln (); writeln ();
  }

}
