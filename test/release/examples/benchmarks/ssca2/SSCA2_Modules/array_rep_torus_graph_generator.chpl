module array_rep_torus_graph_generator

//  +==========================================================================+
//  |  Generate graphs corresponding to 1, 2, 3 and 4D regular tori,           |
//  |  using dense arithmetic arrays to represent the neighbor lists and       |
//  |  edge weights.                                                           |
//  +==========================================================================+

{
  use SSCA2_compilation_config_params;

  //  ----------------------------------------------------------------------
  //  Auxiliary functions to define torus neighbor within a given dimension, 
  //  allowing for wrap around at either end of the dimension.
  //  ----------------------------------------------------------------------

  proc right_nbr ( i, low, high ) : int
    {
      if ( i != high ) then
	return i+1;
      else 
	return low;
    }

  proc left_nbr ( i, low, high ) : int
    {
      if ( i != low ) then
	return i-1;
      else 
	return high;
    }


  // =======================================================
  // generate graph corresponding to a one dimensional torus
  // =======================================================

  proc gen_1D_torus_array_representation ( G ) 
     where G.vertices.rank == 1

    {
      var h = G.vertices.high, l = G.vertices.low;

      if DEBUG_GRAPH_GENERATOR then {
	writeln ();
	writeln ( "One Dimension Torus");
	writeln ( "   index lower bound: ", l ); 
	writeln ( "   index upper bound: ", h );
	writeln ();
	writeln ("vertex   Neighbors");}

      for (i) in G.vertices do
	{
	  G.Neighbors (i) (1) = ( right_nbr (i, l, h) );
	  G.Neighbors (i) (2) = ( left_nbr (i, l, h) );

	  if DEBUG_GRAPH_GENERATOR then 
	    writeln ( i, ":    ", G.Neighbors (i) );
	}

    } // end gen_1D_torus


  // =======================================================
  // generate graph corresponding to a two dimensional torus
  // =======================================================

  proc gen_2D_torus_array_representation ( G ) 
    where G.vertices.rank == 2

    {
      var l1 = G.vertices.low (1), h1 = G.vertices.high (1), 
          l2 = G.vertices.low (2), h2 = G.vertices.high (2);

      if DEBUG_GRAPH_GENERATOR then {
	writeln ();
	writeln ( "Two Dimension Torus");
	writeln ( "   index lower bounds: ", l1, ", ", l2 ); 
	writeln ( "   index upper bounds: ", h1, ", ", h2 );
	writeln ();
	writeln ("vertex    Neighbors");}

      for (i,j) in G.vertices do
	{
	  G.Neighbors (i, j) (1) = ( right_nbr (i, l1, h1), j );
	  G.Neighbors (i, j) (2) = ( left_nbr (i, l1, h1), j );
	  G.Neighbors (i, j) (3) = ( i, right_nbr (j, l2, h2) );
	  G.Neighbors (i, j) (4) = ( i, left_nbr (j, l2, h2) );

	  if DEBUG_GRAPH_GENERATOR then 
	    writeln ((i,j), ":  ", G.Neighbors (i,j) );
	}

    }  // end gen_2D_torus


  // =========================================================
  // generate graph corresponding to a three dimensional torus
  // =========================================================

   proc gen_3D_torus_array_representation ( G ) 
    where G.vertices.rank == 3

    {
     var l1 = G.vertices.low (1), h1 = G.vertices.high (1), 
         l2 = G.vertices.low (2), h2 = G.vertices.high (2), 
         l3 = G.vertices.low (3), h3 = G.vertices.high (3);

      if DEBUG_GRAPH_GENERATOR then {
	writeln ();
	writeln ( "Three Dimension Torus");
	writeln ( "   index lower bounds: ", l1, ", ", l2, ", ", l3 ); 
	writeln ( "   index upper bounds: ", h1, ", ", h2, ", ", h3);
	writeln ();
	writeln ("vertex       Neighbors");
      }

      forall (i, j, k) in G.vertices do
	{
	  G.Neighbors (i, j, k) (1) = ( right_nbr (i, l1, h1), j, k );
	  G.Neighbors (i, j, k) (2) = ( left_nbr (i, l1, h1), j, k );
	  G.Neighbors (i, j, k) (3) = ( i, right_nbr (j, l2, h2), k );
	  G.Neighbors (i, j, k) (4) = ( i, left_nbr (j, l2, h2), k );
	  G.Neighbors (i, j, k) (5) = ( i, j, right_nbr (k, l3, h3) );
	  G.Neighbors (i, j, k) (6) = ( i, j, left_nbr (k, l3, h3) );

	  if DEBUG_GRAPH_GENERATOR then 
	    writeln ((i,j,k), ":  ", G.Neighbors (i,j,k) );
	}

    }  // end gen_3D_torus


   // ========================================================
   // generate graph corresponding to a four dimensional torus
   // ========================================================

   proc gen_4D_torus_array_representation ( G ) 
    where G.vertices.rank == 4

    {
      var l1 = G.vertices.low (1), h1 = G.vertices.high (1), 
          l2 = G.vertices.low (2), h2 = G.vertices.high (2), 
          l3 = G.vertices.low (3), h3 = G.vertices.high (3),
          l4 = G.vertices.low (4), h4 = G.vertices.high (4);

      if DEBUG_GRAPH_GENERATOR then {
	writeln ();
	writeln ( "Four Dimension Torus");
	writeln ( "   index lower bounds: ", l1, ", ", l2, ", ", 
		  l3, ", ", l4 ); 
	writeln ( "   index upper bounds: ", h1, ", ", h2, ", ", 
		  h3, ", ", h4);
	writeln ();
	writeln ("vertex          Neighbors");}

      forall (i, j, k, l) in G.vertices do
	{
	  G.Neighbors (i, j, k, l) (1) = ( right_nbr (i, l1, h1), j, k, l );
	  G.Neighbors (i, j, k, l) (2) = ( left_nbr (i, l1, h1), j, k, l );
	  G.Neighbors (i, j, k, l) (3) = ( i, right_nbr (j, l2, h2), k, l );
	  G.Neighbors (i, j, k, l) (4) = ( i, left_nbr (j, l2, h2), k, l );
	  G.Neighbors (i, j, k, l) (5) = ( i, j, left_nbr (k, l3, h3), l );
	  G.Neighbors (i, j, k, l) (6) = ( i, j, left_nbr (k, l3, h3), l );
	  G.Neighbors (i, j, k, l) (7) = ( i, j, k, right_nbr (l, l4, h4) );
	  G.Neighbors (i, j, k, l) (8) = ( i, j, k, left_nbr (l, l4, h4) );

	  if DEBUG_GRAPH_GENERATOR then 
	    writeln ( (i, j, k, l), ":  ", G.Neighbors (i, j, k, l) );
	}

    }  // end gen_4D_torus

}

