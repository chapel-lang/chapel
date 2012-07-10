module analyze_RMAT_graph_associative_array {

  config const initialRMATNeighborListLength = 16;

  // set to "." for traditional printouts
  config const rmatEdgeGenFile = "rmatalt.weights";
  config const rmatGraphConFile = "rmatalt.neis";

  // +========================================================================+
  // |  Define associative array-based representations for general sparse     |
  // |  graphs. Provide execution template to generate a random RMAT graph    |
  // |  of a specified size and execute and verify SSCA2 kernels 2 through 4. |
  // =========================================================================+

  proc  generate_and_analyze_associative_array_RMAT_graph_representation {

    // -----------------------------------------------------------------
    // compute a random power law graph with 2^SCALE vertices, using 
    // the RMAT generator. Initially generate a list of triples. 
    // Then convert it to a Chapel representation of a sparse graph, 
    // timing this step (Kernel 1).  Finally, execute Kernels 2, 3 and 4
    // of SSCA #2, using identically the same code as in the various
    // torus cases.
    // -----------------------------------------------------------------

    use SSCA2_compilation_config_params, SSCA2_execution_config_consts;
  
    use SSCA2_driver, SSCA2_RMAT_graph_generator;
    use Time;
    use BlockDist;

    var stopwatch : Timer;

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
    // graph using rectangular domains and arrays.  
    // Each node in the graph has a list of neighbors and a corresponding list
    // of (integer) weights for the implicit edges.  
    // ------------------------------------------------------------------------

    const vertex_domain = 
      if DISTRIBUTION_TYPE == "BLOCK" then
        {1..N_VERTICES} dmapped Block ( {1..N_VERTICES} )
      else
        {1..N_VERTICES} ;

    record VertexData {
      type vertex;
      var ndom = {1..initialRMATNeighborListLength};
      var neighborIDs: [ndom] int(64);
      var edgeWeights: [ndom] int(64);

      // This is used for graph construction only.
      // TODO: move to a separate array, to be deallocated after construction.
      // Note: beware of --noRefCount vs. deallocation.
      var firstAvailableNeighborPosition$: sync ndom.idxType = ndom.low;

      // keep some statistics; guarded by firstAvailableNeighborPosition$
      var growCount = 0, shrinkCount = 0;

      proc addEdgeOnVertex(uArg, vArg, wArg) {
        on this do {
          // todo: the compiler should make these values local automatically!
          const /*u = uArg,*/ v = vArg, w = wArg;
          local {
            // grab the vertex lock
            const edgePos = firstAvailableNeighborPosition$;
            const prevNdomLen = ndom.high;
            if edgePos > prevNdomLen {
              // grow our arrays, by 2x
              growCount += 1;
              ndom = {1..prevNdomLen * 2};
              // bounds checking below will ensure (edgePos <= ndom.high)
            }
            // release the lock - don't need it any more
            firstAvailableNeighborPosition$ = edgePos + 1;

            // store the edge
            neighborIDs[edgePos] = v;
            edgeWeights[edgePos] = w;
          }
        } // on
      }

      proc shrinkNeighbors() {
        local {
          const edgeCount = firstAvailableNeighborPosition$.readFF() - 1;
          // TODO: ideally if we don't save much memory, do not resize
          if edgeCount != ndom.numIndices {
            shrinkCount += 1;
            ndom = 1..edgeCount;
          }
          // writeln("stats ", growCount, " ", shrinkCount, ".");
        }
      }

    } // record VertexData
	
    class Associative_Graph {
      const vertices;
      var   Row      : [vertices] VertexData (index (vertices));

      // Simply forward the array's parallel iterator
      // FYI: no fast follower opt
      iter   Neighbors  ( v : index (vertices) ) {
        for u in Row (v).neighborIDs do
          yield u;
      }

      iter   Neighbors  ( v : index (vertices), param tag: iterKind)
      where tag == iterKind.leader {
        for block in Row(v).neighborIDs._value.these(tag) do
          yield block;
      }

      iter   Neighbors  ( v : index (vertices), param tag: iterKind, followThis)
      where tag == iterKind.follower {
        for elem in Row(v).neighborIDs._value.these(tag, followThis) do
          yield elem;
      }

      iter   edge_weight (v : index (vertices) ) {
        for w in Row (v).edgeWeights do
          yield w;}

      // Simply forward the array's parallel iterator
      // FYI: no fast follower opt
      iter   edge_weight(v : index (vertices), param tag: iterKind)
      where tag == iterKind.leader {
        for block in Row(v).edgeWeights._value.these(tag) do
          yield block;
      }

      iter   edge_weight(v : index (vertices), param tag: iterKind, followThis)
      where tag == iterKind.follower {
        for elem in Row(v).edgeWeights._value.these(tag, followThis) do
          yield elem;
      }

      proc   n_Neighbors (v : index (vertices) ) 
      {return Row (v).ndom.numIndices;}

    } // class Associative_Graph

    writeln("allocating Associative_Graph");
    if PRINT_TIMING_STATISTICS then stopwatch.start ();

    var G = new Associative_Graph (vertex_domain);

    if PRINT_TIMING_STATISTICS then {
      stopwatch.stop();
      writeln("Elapsed time for Graph Allocation: ", stopwatch.elapsed(),
              " seconds");
      stopwatch.clear ();
    }

    // ------------------------------------------------------------------
    // generate RMAT graph of the specified size, based on input config
    // values for quadrant assignment.
    // ------------------------------------------------------------------

    Gen_RMAT_graph ( RMAT_a, RMAT_b, RMAT_c, RMAT_d, 
		     SCALE, N_VERTICES, n_raw_edges, MAX_EDGE_WEIGHT, G ); 

    execute_SSCA2 ( G );
    writeln (); writeln ();
    delete G;
  }

}
