module analyze_torus_graphs {

  // +========================================================================+
  // |  Define sparse subdomain stencil representations for                   |
  // |  k-dimensional torus graphs. For each k, provide execution template    |
  // |  for executing and verifying SSCA2 kernels 2 through 4.                |
  // +========================================================================+
  // |                          VERSION 1                                     |
  // |  Explicit neighbor lists as sparse rectangular arrays                  |
  // |  defined over the torus stencil as a sparse domain.  Storage overhead  |
  // |  is as many tuples as nonzeros.                                        |
  // +========================================================================+


  use SSCA2_compilation_config_params, 
      SSCA2_execution_config_consts,
      SSCA2_driver, 
      torus_graph_generator_utilities,
      BlockDist;


  // =====================================================================
  // generic class for stencil-based graph for torus of any dimensionality
  // =====================================================================

  class torus_explicit_stencil_graph {
      
    const vertex_domain;
    const torus_stencil;
    param dimensions = vertex_domain.rank;

    const vertices = vertex_domain;
    type  vertex = index (vertex_domain);

    var Neighbors   : [vertex_domain] [torus_stencil] vertex;
    var edge_weight : [vertex_domain] [torus_stencil] int;

    proc num_edges {
      return + reduce [v in vertex_domain] n_Neighbors (v);
    }

    iter FilteredNeighbors( v : index (vertices) ) {
      const ref neighbors = Neighbors[v];
      const ref weights = edge_weight[v];
      for n in torus_stencil do
        if (FILTERING && weights(n)%8 != 0) || !FILTERING then
          yield neighbors(n);
    }

    iter FilteredNeighbors( v : index (vertices), param tag: iterKind)
    where tag == iterKind.standalone {
      const ref neighbors = Neighbors[v];
      const ref weights = edge_weight[v];
      forall n in torus_stencil do
        if (FILTERING && weights(n)%8 != 0) || !FILTERING then
          yield neighbors(n);
    }

    // iterate over all neighbor (ID, weight) pairs

    iter NeighborPairs( v : index (vertices) ) {
      const ref neighbors = Neighbors[v];
      const ref weights = edge_weight[v];
      for n in torus_stencil do
        yield (neighbors(n), weights(n));
    }

    iter NeighborPairs( v : index (vertices), param tag: iterKind)
    where tag == iterKind.standalone {
      const ref neighbors = Neighbors[v];
      const ref weights = edge_weight[v];
      forall n in torus_stencil do
        yield (neighbors(n), weights(n));
    }

    proc n_Neighbors (v : index (vertices) ) return 2*dimensions;
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

    type  vertex = index (vertex_domain);

    const dense_stencil = {-1..1};
    const torus_stencil : sparse subdomain (dense_stencil) = ( -1, 1 );

    var G = new unmanaged torus_explicit_stencil_graph ( vertex_domain, 
					       torus_stencil );

    const exact_between_centrality = 0.25 * (d - 2) * d  -  d/2 + 1;

    writeln ();
    writeln ("One D Torus: all nodes should have",
	     " exact betweenness centrality: ", exact_between_centrality );
    writeln ("             (upper bound on approximate or filtered", 
                         " betweenness centrality)");
    writeln ();

    writeln ( "--------------------------------" );
    writeln ( "One D Torus with ", d, " nodes" );
    writeln ( "explicit stencil representation");
    writeln ( "--------------------------------" );
    writeln ();
	  
    const extended_domain = vertex_domain.expand(1);
    const h = G.vertices.high, l = G.vertices.low;
    var   torus_vertex : [extended_domain] vertex = extended_domain;

    if DEBUG_GRAPH_GENERATOR then {
      writeln ();
      writeln ( "One Dimension Torus");
      writeln ( "   index lower bound: ", l ); 
      writeln ( "   index upper bound: ", h );
      writeln ();
      writeln ("vertex   Neighbors");}

    // torus_vertex wraps the torus with an additional plane in each
    //  dimension.  fill in the wrap-around vertices in the ghost cells

    torus_vertex (l-1) = torus_vertex (h);
    torus_vertex (h+1) = torus_vertex (l);

    // =======================================================
    // generate graph corresponding to a one dimensional torus
    // =======================================================

    forall i in G.vertices do {
      forall nbr in torus_stencil do 
	G.Neighbors (i) (nbr) = torus_vertex ( i + nbr );

      if DEBUG_GRAPH_GENERATOR then 
	writeln ( i, ":    ", G.Neighbors (i) );
    }
    
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
       { Torus_Base_Index..#d1,
	 Torus_Base_Index..#d2 }
	 dmapped Block ( { Torus_Base_Index..#d1,
			   Torus_Base_Index..#d2 } )
     else
       { Torus_Base_Index..#d1,
	 Torus_Base_Index..#d2 };

    type  vertex = index (vertex_domain);

    const dense_stencil = {-1..1,-1..1};
    const torus_stencil : sparse subdomain (dense_stencil) = 
                                        ( (-1,0), (1,0), (0,-1), (0,1) );

    var G = new unmanaged torus_explicit_stencil_graph ( vertex_domain, torus_stencil );
	  
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

    writeln ( "--------------------------------" );
    writeln ( "Two D Torus with ", d1, " x ", d2, " nodes" );
    writeln ( "explicit stencil representation");
    writeln ( "--------------------------------" );
    writeln ();

    const extended_domain = vertex_domain.expand(1);
    const h1 = G.vertices.high (0), l1 = G.vertices.low (0);
    const h2 = G.vertices.high (1), l2 = G.vertices.low (1);
    var   torus_vertex : [extended_domain] vertex = extended_domain;

    if DEBUG_GRAPH_GENERATOR then {
      writeln ();
      writeln ( "Two Dimension Torus");
      writeln ( "   index lower bounds: ", l1, ", ", l2 ); 
      writeln ( "   index upper bounds: ", h1, ", ", h2 );
      writeln ();
      writeln ("vertex    Neighbors");
    }

    // torus_vertex wraps the torus with an additional plane in each dimension
    // fill in the wrap-around vertices in the ghost cells

    torus_vertex (l1-1, ..) = torus_vertex (h1 ,..);
    torus_vertex (h1+1, ..) = torus_vertex (l1,..);
    torus_vertex (.., l2-1) = torus_vertex (.., h2);
    torus_vertex (.., h2+1) = torus_vertex (.., l2);

  // =======================================================
  // generate graph corresponding to a two dimensional torus
  // =======================================================

    forall i in G.vertices do {
      forall nbr in torus_stencil do 
	G.Neighbors (i) (nbr) = torus_vertex ( i + nbr );

      if DEBUG_GRAPH_GENERATOR then 
	writeln ( i, ":    ", G.Neighbors (i) );
    }
    
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

    type  vertex = index (vertex_domain);
    const dense_stencil = {-1..1, -1..1, -1..1};
    const torus_stencil : sparse subdomain (dense_stencil) = 
      ( (-1, 0, 0 ), (1, 0, 0), (0, -1, 0), (0, 1, 0), (0, 0, -1), (0, 0, 1) );

    var G = new unmanaged torus_explicit_stencil_graph ( vertex_domain, torus_stencil );
	  
    writeln ( "--------------------------------" );
    writeln ( "Three D Torus with ", d1, " x ", d2, " x ", d3, 
	      " nodes" );
    writeln ( "explicit stencil representation");
    writeln ( "--------------------------------" );
    writeln ();

    const extended_domain = vertex_domain.expand(1);
    const h1 = G.vertices.high (0), l1 = G.vertices.low (0);
    const h2 = G.vertices.high (1), l2 = G.vertices.low (1);
    const h3 = G.vertices.high (2), l3 = G.vertices.low (2);
    var   torus_vertex : [extended_domain] vertex = extended_domain;
  
    if DEBUG_GRAPH_GENERATOR then {
      writeln ();
      writeln ( "Three Dimension Torus");
      writeln ( "   index lower bounds: ", l1, ", ", l2, ", ", l3 ); 
      writeln ( "   index upper bounds: ", h1, ", ", h2, ", ", h3);
      writeln ();
      writeln ("vertex       Neighbors");
    }

    // torus_vertex wraps the torus with an additional plane in each dimension
    // fill in the wrap-around vertices in the ghost cells

    torus_vertex (l1-1, .., ..) = torus_vertex (h1, .., ..);
    torus_vertex (h1+1, .., ..) = torus_vertex (l1, .., ..);
    torus_vertex (.., l2-1, ..) = torus_vertex (.., h2, ..);
    torus_vertex (.., h2+1, ..) = torus_vertex (.., l2, ..);
    torus_vertex (.., .., l3-1) = torus_vertex (.., .., h3);
    torus_vertex (.., .., h3+1) = torus_vertex (.., .., l3);

  // =========================================================
  // generate graph corresponding to a three dimensional torus
  // =========================================================

    forall i in G.vertices do {
      forall nbr in torus_stencil do 
	G.Neighbors (i) (nbr) = torus_vertex ( i + nbr );

      if DEBUG_GRAPH_GENERATOR then 
	writeln ( i, ":    ", G.Neighbors (i) );
    }
    
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

    type  vertex = index (vertex_domain);
    const dense_stencil = {-1..1, -1..1, -1..1, -1..1};
    const torus_stencil : sparse subdomain (dense_stencil) = 
      ( (-1, 0, 0, 0), (1, 0, 0, 0), (0, -1, 0, 0), (0, 1, 0, 0), 
	(0, 0, -1, 0), (0, 0, 1, 0), (0, 0, 0, -1), (0, 0, 0, 1) );

    var G = new unmanaged torus_explicit_stencil_graph ( vertex_domain, torus_stencil );
	  
	  
    writeln ( "--------------------------------" );
    writeln ( "Four D Torus with ", d1, " x ", d2, 
	      " x ", d3, " x ", d4, " nodes" );
    writeln ( "explicit stencil representation");
    writeln ( "--------------------------------" );
    writeln ();

    const extended_domain = vertex_domain.expand(1);
    const h1 = G.vertices.high (0), l1 = G.vertices.low (0);
    const h2 = G.vertices.high (1), l2 = G.vertices.low (1);
    const h3 = G.vertices.high (2), l3 = G.vertices.low (2);
    const h4 = G.vertices.high (3), l4 = G.vertices.low (3);
    var   torus_vertex : [extended_domain] vertex = extended_domain;

      if DEBUG_GRAPH_GENERATOR then {
	writeln ();
	writeln ( "Four Dimension Torus");
	writeln ( "   index lower bounds: ", l1, ", ", l2, ", ", 
		  l3, ", ", l4 ); 
	writeln ( "   index upper bounds: ", h1, ", ", h2, ", ", 
		  h3, ", ", h4);
	writeln ();
	writeln ("vertex          Neighbors");}

    // torus_vertex wraps the torus with an additional plane in each dimension
    // fill in the wrap-around vertices in the ghost cells

    torus_vertex (l1-1, .., .., ..) = torus_vertex (h1, .., .., ..);
    torus_vertex (h1+1, .., .., ..) = torus_vertex (l1, .., .., ..);
    torus_vertex (.., l2-1, .., ..) = torus_vertex (.., h2, .., ..);
    torus_vertex (.., h2+1, .., ..) = torus_vertex (.., l2, .., ..);
    torus_vertex (.., .., l3-1, ..) = torus_vertex (.., .., h3, ..);
    torus_vertex (.., .., h3+1, ..) = torus_vertex (.., .., l3, ..);
    torus_vertex (.., .., .., l4-1) = torus_vertex (.., .., .., h4);
    torus_vertex (.., .., .., h4+1) = torus_vertex (.., .., .., l4);

  // ========================================================
  // generate graph corresponding to a four dimensional torus
  // ========================================================

    forall i in G.vertices do {
      forall nbr in torus_stencil do 
	G.Neighbors (i) (nbr) = torus_vertex ( i + nbr );

      if DEBUG_GRAPH_GENERATOR then 
	writeln ( i, ":    ", G.Neighbors (i) );
    }
    
    torus_random_edge_weights ( MAX_EDGE_WEIGHT, G );

    execute_SSCA2 ( G );
    delete G;

    writeln (); writeln ();
  }

}
