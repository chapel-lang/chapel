module analyze_RMAT_graph_1D_array {

  // +========================================================================+
  // |  Define 1D array-based representations for general sparse              |
  // |  graphs. Provide execution template to generate a random RMAT graph    |
  // |  of a specified size and execute and verify SSCA2 kernels 2 through 4. |
  // =========================================================================+

  proc  generate_and_analyze_1D_array_RMAT_graph_representation {

    // -----------------------------------------------------------------
    // compute a random power law graph with 2^SCALE vertices, using 
    // the RMAT generator. Initially generate a list of triples. 
    // Then convert it to a Chapel representation of a sparse graph, 
    // timing this step (Kernel 1).  Finally, execute Kernels 2, 3 and 4
    // of SSCA #2, using identically the same code as in the various
    // torus cases.
    // -----------------------------------------------------------------

    use SSCA2_compilation_config_params, SSCA2_execution_config_consts;
  
    use SSCA2_driver, SSCA2_RMAT_graph_generator_1Darray;

    use BlockDist;

    var n_raw_edges = 8 * N_VERTICES;

    assert ( SCALE > 1, "SCALE must be greater than 1");

    select SCALE {
      when 2 do n_raw_edges = N_VERTICES / 2;
      when 3 do n_raw_edges = N_VERTICES;
      when 4 do n_raw_edges = 2 * N_VERTICES;
      when 5 do n_raw_edges = 4 * N_VERTICES;
      }

    writeln ('-------------------------------------');
    writeln ('Order of RMAT generated graph:', N_VERTICES);
    writeln ('          number of raw edges:', n_raw_edges);
    writeln ('-------------------------------------');
    writeln ();

    // ------------------------------------------------------------------------
    // The data structures below are chosen to implement an irregular (sparse)
    // graph using associative domains and arrays.  
    // Each node in the graph has a list of neighbors and a corresponding list
    // of (integer) weights for the implicit edges.  
    // The list of neighbors is really just a set; the only properties we need
    // are that we be able to build it (add vertices to it) and that we be
    // able to iterate over it.  Those properties are satisfied by Chapel's
    // associative domains, so each neighbor set is represented by an
    // associative domain.  The weights are an integer array over the 
    // neighbor domain.
    //
    // We would have liked to have defined the global set of neighbors as
    // an array of associative domains, but that is not supported in Chapel.
    // Consequently we build an array of records, where each record provides
    // the neighbor set and the weights for a particular node.  The name
    // "row_struct" anticipates the planned use of sparse matrices for this same
    // kind of graph structure.  
    // ------------------------------------------------------------------------

    const vertex_domain = 
      if DISTRIBUTION_TYPE == "BLOCK" then
        [1..N_VERTICES] dmapped Block ( [1..N_VERTICES] )
      else
	[1..N_VERTICES] ;

//    record row_struct {
//      type vertex;
//      var Row_Neighbors : domain (vertex);
//      var Weight        : [Row_Neighbors] int;
//    }
    record vertex_struct {
      type vertex;
      var nd: domain(1);
      var Row_Neighbors : [nd] vertex;
      var Weight        : [nd] int;
      var neighbor_count: int=0;
      var self_edges: int=0;
      var duplicates: int=0;
      var vlock$: sync bool = true;

      proc is_a_neighbor (new_vertex_ID: vertex) {
         var is_member: bool = false;
         forall n in Row_Neighbors (1..neighbor_count) {
            if (new_vertex_ID == n ) then is_member = true;
         }
         return is_member;
      }

      proc add_self_edge () {
         vlock$.readFE();
         self_edges += 1;
         vlock$.writeEF(true);
      }

      proc add_duplicate () {
         vlock$.readFE();
         duplicates += 1;
         vlock$.writeEF(true);
      }

      proc add_Neighbor (new_vertex_ID: vertex, weight: int) {
         vlock$.readFE();
         var ID: vertex = new_vertex_ID;
//       Check again to make sure another thread did not recently
//       add v to u's neighbor list
         if is_a_neighbor(ID) then {
           duplicates +=1;
         }
         else
         {
           if (neighbor_count >= Row_Neighbors.numElements) {
             grow_helper();
           }
           neighbor_count += 1;
           Row_Neighbors[neighbor_count]= new_vertex_ID;
           Weight[neighbor_count]= weight;
         }
         vlock$.writeEF(true);
      }

      proc grow_helper() {
          halt("Should not call grow_helper");
          var new_nd = Row_Neighbors.numElements + 1;
          nd = [1..new_nd];
      }

    }

	
    class RMAT_Array1D_Graph {
      const vertices;
      var   Row      : [vertices] vertex_struct (index (vertices));

      proc   Neighbors  ( v : index (vertices) ) {return Row (v).Row_Neighbors;}

      iter   edge_weight (v : index (vertices) ) ref {
        for w in Row (v).Weight do
          yield w;}  // var iterator to avoid a copy

      // Simply forward the domain's parallel iterator
      // FYI: no fast follower opt
      iter   edge_weight(v : index (vertices), param tag: iterKind)
      where tag == iterKind.leader {
        for block in Row(v).Weight._value.these(tag) do
          yield block;
      }

      iter   edge_weight(v : index (vertices), param tag: iterKind, followThis)
      where tag == iterKind.follower {
        for elem in Row(v).Weight._value.these(tag, followThis) do
          yield elem;
      }

      proc   n_Neighbors (v : index (vertices) ) 
      {return Row (v).neighbor_count;}

    }

    var G = new RMAT_Array1D_Graph (vertex_domain);

    // ------------------------------------------------------------------
    // generate RMAT graph of the specified size, based on input config
    // values for quadrant assignment.
    // ------------------------------------------------------------------

    Gen_RMAT_graph_1D_array ( RMAT_a, RMAT_b, RMAT_c, RMAT_d, 
		     SCALE, N_VERTICES, n_raw_edges, MAX_EDGE_WEIGHT, G ); 

    execute_SSCA2 ( G );
    writeln (); writeln ();
    delete G;
  }

}
