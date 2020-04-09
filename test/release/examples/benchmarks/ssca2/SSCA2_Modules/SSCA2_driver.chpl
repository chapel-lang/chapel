module SSCA2_driver
{

  //  +========================================================================+
  //  |  Driver for Kernels 2 through 4 of SSCA #2                             |
  //  |                                                                        |
  //  |  This code accepts a generic graph argument that provides for each     |
  //  |  vertex:                                                               |
  //  |                                                                        |
  //  |  1.  an iterator for its set of neighbors                              |
  //  |  2.  a parallel integer array of edge weights, which can be zipper     |
  //  |      iterated with the set of neighbors                                |
  //  |  3.  the number of neighbors                                           |
  //  |                                                                        |
  //  |  It executes each kernel in order and provides diagnostic or           |
  //  |  performance output.                                                   |
  //  |                                                                        |
  //  |  Kernel 4 (betweenness centrality) has two modes --                    |
  //  |     exact betweenness centrality based on all vertices                 |
  //  |        and                                                             |
  //  |     approximate betweenness centrality based on a random subset of     |
  //  |     vertices with a prescribed size for the subset                     |
  //  |                                                                        |
  //  |  This implementation is capable of both and presently calculates       |
  //  |  approximate betweenness centrality for a range of sizes for the       |
  //  |  random starting set.                                                  |
  //  +========================================================================+


  class Generated_Subgraph {
    type vertex;
    var nodes : domain (vertex);
    var edges : domain (2*vertex);
  }

       
  proc execute_SSCA2 ( G )
    {
      // ---------------------------------
      // execute SSCA2 kernels 2 through 4
      // ---------------------------------

      use SSCA2_compilation_config_params, 
	  SSCA2_execution_config_consts,
	  SSCA2_kernels, 
          Random;

      type vertex = index (G.vertices),
           edge   = 2*vertex;

      if RUN_KERNEL2 || RUN_KERNEL3 {
        var Heavy_Edge_List : domain (edge);

        // --------
        // Kernel 2:
        // --------

        largest_edges ( G, Heavy_Edge_List );

        if RUN_KERNEL3 {
          var Heavy_Edge_Subgraphs : [Heavy_Edge_List] unmanaged Generated_Subgraph (vertex)?;

          for (x,y) in Heavy_Edge_List do
            Heavy_Edge_Subgraphs ( (x, y) ) = new unmanaged Generated_Subgraph (vertex);
        	    
          // --------
          // Kernel 3:
          // --------

          rooted_heavy_subgraphs ( G, Heavy_Edge_List, Heavy_Edge_Subgraphs,
                                   SUBGRAPH_PATH_LENGTH );
          [(x,y) in Heavy_Edge_List] delete Heavy_Edge_Subgraphs ( (x,y) );
        }
      }

      if RUN_KERNEL4 {
        var Between_Cent : [G.vertices] real;
        var BC_starting_vertices : domain (1);
        var Sum_Min_Dist : real;
        const Vertex_Count_Constraint = N_VERTICES : real * (N_VERTICES -1);

        for approx_scale in { max (1, LOW_APPROX_SCALE) .. 
                              min (TOP_APPROX_SCALE, SCALE) } {
  
	  if approx_scale == SCALE {
            //  --------------------------------------
            // "Exact" Between_Centrality computed
            //  by BFS from every vertex in the graph.
            //  --------------------------------------

            writeln (); writeln ();
            writeln ( "Computing Betweenness Centrality exactly" );

            // -----------------------------
            // Kernel 4 (exact calculation):
            // -----------------------------

            approximate_betweenness_centrality ( G, G.vertices, 
                                                 Between_Cent, 
                                                 Sum_Min_Dist );
          } else {
            //  -----------------------------------------------
            // "Approximate" Between_Centrality computed by BFS
            //  from a random subset of vertices in the graph.
            //  -----------------------------------------------

            var BC_starting_vertices : sparse subdomain 
              ( {(...G.vertices.dims())} );
            var vertex_indices       : domain (1) = {1..N_VERTICES};
            var random_indices       : sparse subdomain (vertex_indices);
            var linear_index         : index (vertex_indices);

            var Rand_Gen = if REPRODUCIBLE_PROBLEMS then 
                             new unmanaged NPBRandomStream (real, seed = 3217900597)
                           else
                             new unmanaged NPBRandomStream (real);

            var V_s      = 0;

            while V_s < 2**approx_scale {
                linear_index = floor (1 + Rand_Gen.getNext() * N_VERTICES) 
                               : index (vertex_indices);
                if !random_indices.contains (linear_index) {
                  V_s += 1;
                  random_indices.add (linear_index);
                }
              };

            for (s, linear_index) in zip( G.vertices, 1.. ) do
              if random_indices.contains (linear_index) then
                BC_starting_vertices.add (s);

            writeln (); writeln ();
            writeln ( "Approximating Betweenness Centrality with ", V_s,
                      " starting nodes" );

            if DEBUG_KERNEL4 then {
              writeln ( "starting vertices for Betweenness Centrality");
              writeln ( BC_starting_vertices );
            }

            // -----------------------------------
            // Kernel 4 (approximate calculation):
            // -----------------------------------

            approximate_betweenness_centrality ( G, BC_starting_vertices, 
                                                 Between_Cent, 
                                                 Sum_Min_Dist );

            delete Rand_Gen;
          }

	  // --------------------------------------------------
	  // Diagnostic Output at various levels of information
	  // --------------------------------------------------

	  if DEBUG_KERNEL4 then {
	    writeln ();
	    writeln ("Between Centrality:");
	    for v in G.vertices do
	      writeln ("  BC(", v, ") = ", Between_Cent (v) );
	  }

	  var max_Between_Cent = max reduce Between_Cent;
	  var min_Between_Cent = min reduce Between_Cent;

	  writeln (); 
	  writeln ("Max Betweenness Centrality: ", max_Between_Cent);
	  writeln ("Min Betweenness Centrality: ", min_Between_Cent);

          if VALIDATE_BC then {
            if max_Between_Cent <= Sum_Min_Dist {
              writeln (); 
              writeln ("Computed betweenness centrality satisfies ",
                       "minimum distance constraint");
            } else {
              writeln (); writeln ("Computed betweenness centrality does not ",
                                   "satisfy minimum distance constraint");
            };
            writeln ("All betweenness centrality scores should be <= ", 
                     Sum_Min_Dist);
          };

          var meet_vertex_count_constraint : bool = 
            || reduce [v in G.vertices]
            ( if G.n_Neighbors (v) == 0 then
               ( Between_Cent (v) == 0.0 )
              else
                ( ( Between_Cent (v) > 0.0 ) &&
                  ( Between_Cent (v) <= Vertex_Count_Constraint ) ) );
          
          if meet_vertex_count_constraint {
            writeln (); 
            writeln ("Computed betweenness centrality satisfies ",
                     "vertex count constraint");
          } else {
            writeln (); 
            writeln ("Computed betweenness centrality does not ",
                     "satisfy vertex count constraint");
          };
          writeln ( "All betweenness centrality scores should be in range [0, ",
                    Vertex_Count_Constraint, "]" );
	}
      }
    }

}
