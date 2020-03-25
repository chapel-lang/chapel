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

use SSCA2_compilation_config_params, Time, BlockDist;

// File names to dump the edge list and the constructed graph, resp.
// By default, printouts, if requested, are traditional.
config const rmatEdgeGenFile = "";
config const rmatGraphConFile = "";

// These control which variations of the code to run.
config const newEG = true;      // do new-style edge generation?
config const parEG = true;      // do newEG permutations in parallel?
config const numDepths = -1;    // number of edge tweaks; -1 for baseline
config const edgeChecks = true; // run edge consistency checks?
config const parGC = true;      // do Graph Generation in parallel?
// Set to false for parallel computation (=> faster but not reproducible).
config param SERIAL_GRAPH_GEN = REPRODUCIBLE_PROBLEMS;

var stopwatch : Timer;

record directed_vertex_pair {
  var start = 1: int;
  var end   = 1: int;
}


// ============================
// Quadrant selection algorithm
// ============================

inline proc directed_vertex_pair.assign_quadrant
  (u: real, a: real, b: real, c: real, d: real, bit: int)
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

    if u <= a then
      {}
    else if u <= a + b then
      { end_inc = 1;}
    else if u <= a + b + c then
      { start_inc = 1;}
    else
      { start_inc = 1; end_inc = 1;};

    this.start += bit * start_inc;
    this.end   += bit * end_inc;
  }


// ====================================
// Main RMAT Graph Generation Procedure
// ====================================~

proc Gen_RMAT_graph ( a : real,
                     b : real,
                     c : real,
                     d : real,
                     vertex_domain,
                     SCALE :int,
                     N_VERTICES : int,
                     n_raw_edges : int,
                     MAX_EDGE_WEIGHT :int,
                     G )

{ use Random, IO;
    use analyze_RMAT_graph_associative_array;

    if PRINT_TIMING_STATISTICS then stopwatch.start ();

    const vertex_range = 1..N_VERTICES,
          edge_range   = 1..n_raw_edges;

    const edge_domain =
      if DISTRIBUTION_TYPE == "BLOCK" then
        {edge_range} dmapped Block ( {edge_range} )
      else
        {edge_range} ;

    var Edges       : [edge_domain] directed_vertex_pair;
    // TODO: fold edge weight into directed_vertex_pair
    var Edge_Weight : [edge_domain] int;
    var permutation : [vertex_domain] int = vertex_range;

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
    if newEG then
      writeln("Using the new edge generation scheme, ",
              if parEG then "in parallel" else "serially");
    if SERIAL_GRAPH_GEN then
      writeln("Serializing certain parallel loops");

    const numDep = if numDepths == -1 then SCALE else numDepths;
    if numDep != SCALE then
      writeln("randomizing with ", numDep, " depths instead of ",
              SCALE, " depths");

  if newEG {
    // new edge generation

    var rndPos = 1;
    const seed = if REPRODUCIBLE_PROBLEMS then 0556707007
                                          else SeedGenerator.oddCurrentTime;

    const delta = n_raw_edges + 1; // 1 corresponds to 'skip' in "old" code
    rndPos += 1;                   // start with a skip
    var bit = 1 << numDep;

    for depth in 1..numDep {
      bit >>= 1;
      const bitCst = bit;

      forall (e, r1, r2, r3, r4, r5) in zip(Edges,
// RandomPrivate_iterate() generates a random number per elem of edge_domain
// starting at 'start'-th point in the random number stream for 'seed'.
NPBRandomPrivate_iterate(real, edge_domain, seed, start=rndPos+0*delta),
NPBRandomPrivate_iterate(real, edge_domain, seed, start=rndPos+1*delta),
NPBRandomPrivate_iterate(real, edge_domain, seed, start=rndPos+2*delta),
NPBRandomPrivate_iterate(real, edge_domain, seed, start=rndPos+3*delta),
NPBRandomPrivate_iterate(real, edge_domain, seed, start=rndPos+4*delta)) {
       local {

        var Noisy_a = a * (0.95 + 0.1 * r1);
        var Noisy_b = b * (0.95 + 0.1 * r2);
        var Noisy_c = c * (0.95 + 0.1 * r3);
        var Noisy_d = d * (0.95 + 0.1 * r4);

        var norm     = 1.0 / (Noisy_a + Noisy_b + Noisy_c + Noisy_d);
        Noisy_a *= norm;
        Noisy_b *= norm;
        Noisy_c *= norm;
        Noisy_d *= norm;

        e.assign_quadrant(r5, Noisy_a, Noisy_b, Noisy_c, Noisy_d, bitCst);

       }  // local
      }  // forall

      rndPos += delta * 5;

    }  // for depth

    forall (w, rnd) in zip(Edge_Weight,
      NPBRandomPrivate_iterate(real, edge_domain, seed, start=rndPos-1))
    do
     local do
      w = floor (1 + rnd * MAX_EDGE_WEIGHT) : int;
    rndPos += n_raw_edges;

    if parEG {
      var permutation$ : [vertex_domain] sync int = vertex_range;

      serial(SERIAL_GRAPH_GEN) {
        forall (v, rnd) in zip(vertex_domain,
          NPBRandomPrivate_iterate(real, edge_domain, seed, start=rndPos-1))
        {
          const u = floor (1 + rnd * N_VERTICES) : int;
          if u != v {
            // All this does is permutation$(u) <=> permutation$(v).
            // Implementation notes:
            // * Lock the smaller index first, to ensure progress.
            // * Access each of the two sync vars on its locale.
            // * Factors out the 'on's that are implicit in
            //   the accesses to the sync variables into explicit 'on's.
            // * Factors out indexing into permutation$ using
            //   by-reference argument passing.

            const (ix1, ix2) = if v <= u then (v, u) else (u, v);
            swapTwo(permutation$(ix1), permutation$(ix2));

            proc swapTwo(p1$: sync int, p2$: sync int): void {
              on p1$ {
                const label1 = p1$;
                var label2: int;
                on p2$ {
                  label2 = p2$;
                  p2$ = label1;
                }
                p1$ = label2;
              }
            }  // proc swapTwo
          }
        }  // forall
      }
      rndPos += N_VERTICES;

      forall (pm, pm$) in zip(permutation, permutation$) {
        pm = pm$.readXX();
      }

    } else {  // !parEG
      for (v, rnd) in zip(vertex_domain,
        NPBRandomPrivate_iterate(real, vertex_domain, seed, start=rndPos-1))
      {
        const new_id = floor (1 + rnd * N_VERTICES) : int;
        permutation (v) <=> permutation (new_id);
      }
      rndPos += N_VERTICES;

    } // if parEG

  } else {  // !newEG

    // Random Numbers return in the range [0.0, 1.0)

    var Rand_Gen = if REPRODUCIBLE_PROBLEMS then
                     new NPBRandomStream (real, seed = 0556707007)
                   else
                     new NPBRandomStream (real);

    var   Noisy_a     : [edge_domain] real,
          Noisy_b     : [edge_domain] real,
          Noisy_c     : [edge_domain] real,
          Noisy_d     : [edge_domain] real,
          norm        : [edge_domain] real,
          Unif_Random : [edge_domain] real;

    var bit = 1 << numDep;
    var   skip : real;

    for depth in 1..numDep do {
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

        Edges.assign_quadrant ( Unif_Random, Noisy_a, Noisy_b,
                                   Noisy_c, Noisy_d, bit );
      };

    // ---------------------------------------------------------------------
    // Assign weights to edges randomly, then randomly relabel the vertices
    // to avoid locality from the obvious imbalance that will arise when
    // one of the coefficients is clearly larger than the others
    // ---------------------------------------------------------------------

    Rand_Gen.fillRandom ( Unif_Random  );
    Edge_Weight = floor (1 + Unif_Random * MAX_EDGE_WEIGHT) : int;

    Rand_Gen.fillRandom ( Unif_Random (vertex_range) );

    for v in vertex_range do
      { var new_id : int;
        new_id = floor (1 + Unif_Random (v) * N_VERTICES) : int;
        permutation (v) <=> permutation (new_id);
      };

  } // if newEG

    forall e in Edges do {
      e.start = permutation(e.start);
      e.end   = permutation(e.end);
    }

    if PRINT_TIMING_STATISTICS then {
      stopwatch.stop ();
      writeln ( "Elapsed time for Edge Generation: ", stopwatch.elapsed (),
                " seconds");
      stopwatch.clear ();
    }

    if DEBUG_WEIGHT_GENERATOR then {
      writeln ();
     if rmatEdgeGenFile != "" {
      writeln("writing edges to ", rmatEdgeGenFile);
      const fl = open(rmatEdgeGenFile, iomode.cw);
      const ch = fl.writer();
      for (ed, w) in zip(Edges, Edge_Weight) do
        ch.writeln (ed.start, " ", ed.end, " ", w);
      ch.close();
      fl.close();
     } else
      for e in edge_range do
        writeln ("edge (", e, ") : (", Edges(e), ", ",
                 Edge_Weight (e), ")" );
    }

    // --------------------------
    // Graph consistency checking
    // --------------------------

    if edgeChecks {
      // Check that 'permutation' was a permutation.
      var permuteCounts : [vertex_domain] atomic int;
      forall p in permutation do
        permuteCounts[p].add(1);
      forall (pc, ix) in zip(permuteCounts, vertex_domain) do
        if !(pc.read() == 1) then halt(
             "'permutation' is not a permutation: vertex ", ix,
             " is mentioned ", pc.read(), " times");

      // Sanity checks on edges and weights.
      forall (e, w) in zip(Edges, Edge_Weight) {

        if !(e.start > 0) then halt(
             "edge start vertices out of low end of range");

        if !(e.end > 0) then halt(
             "edge end vertices out of low end of range");

        if !(e.start <= 2**SCALE) then halt(
             "edge start vertices out of high end of range");

        if !(e.end <= 2**SCALE) then halt(
             "edge end vertices out of high end of range");

        if !(w > 0) then halt(
             "edge weight out of low end of range");

        if !(w <= MAX_EDGE_WEIGHT) then halt(
             "edge weight out of high end of range");
      }
      // exit the scope to enable memory deallocation, if any
    }

    writeln (); writeln ("Vertex Set in G:", G.vertices);

    // ----------------------------------------------------------
    // Kernel 1: assemble graph from list of triples.
    // Include only non-self incident edges.
    // In case of (start, end) duplicates, the first triple wins.
    // Alternatively, could skip the contains(v) check (so the last triple
    // wins) or also use += instead of = (to sum duplicates' weights).
    // ----------------------------------------------------------

    var firstAvailNeighbor$: [vertex_domain] sync int = initialFirstAvail;

    writeln("Starting Graph Generation ",
            if parGC then "in parallel" else "serially");
    if PRINT_TIMING_STATISTICS then stopwatch.start ();

    var collisions = 0, self_edges = 0;

  if parGC {

    var self_edges$: atomic int;
    serial(SERIAL_GRAPH_GEN) {
      forall (e, w) in zip(Edges, Edge_Weight) do {
        const u = e.start;
        const v = e.end;

        if ( v == u ) then {
          // self-edge, ignore
          self_edges$.add(1);
        } else {
          // Both the vertex and firstAvail must be passed by reference.
          // TODO: possibly compute how many neighbors the vertex has, first.
          // Then allocate that big of a neighbor list right away.
          // That way there will be no need for a sync, just an atomic.
          G.Row[u].addEdgeOnVertex(u, v, w, firstAvailNeighbor$[u]);
        }
      }
    }

    self_edges = self_edges$.read();

  } else {  // !parGC

    for e in edge_range do {
      const u = Edges (e).start;
      const v = Edges (e).end  ;

      if ( v == u ) then {
        // self-edge, ignore
        self_edges += 1;
      } else {
        const w = Edge_Weight(e);
        // Both the vertex and firstAvail must be passed by reference.
        // TODO: skip locking in this serial version.
        G.Row[u].addEdgeOnVertex(u, v, w, firstAvailNeighbor$[u]);
      }
    }

  } // if parGC

    forall (vx, firstAvail$) in zip(G.Row, firstAvailNeighbor$) do
      vx.tidyNeighbors(firstAvail$);

    if PRINT_TIMING_STATISTICS then {
      stopwatch.stop ();
      writeln ( "Elapsed time for Kernel 1: ", stopwatch.elapsed (),
                " seconds");
      stopwatch.clear ();
    }

    G.num_edges = + reduce [v in G.vertices] G.n_Neighbors (v);

    writeln ( "# of raw edges generated  ", n_raw_edges );
    writeln ( "# of duplicate edges      ", "not counted" /*collisions*/ );
    writeln ( "# of self edges           ", self_edges );
    writeln ( "# of edges in final graph ", G.num_edges );

    if DEBUG_GRAPH_GENERATOR then {
     writeln ();
     if rmatGraphConFile != "" {
      writeln("writing neighbor lists to ", rmatGraphConFile);
      const fl = open(rmatGraphConFile, iomode.cw);
      const ch = fl.writer();
      for (u, vx) in zip(G.vertices, G.Row) do
        for (v, w) in vx.neighborList do
          ch.writeln(u, " ", v, " ", w);
      ch.close();
      fl.close();
     } else {
      writeln ("tuples denote (edge, weight)");
      writeln ();
      for u in G.vertices do {
        write ( "row ", u, ": [", G.n_Neighbors (u), "] " );
        for (v,w) in G .NeighborPairs (u) do
          write ( (v, w) );
        writeln ();
      }
     }
    }

    var max_edges = max reduce [v in vertex_range] G.n_Neighbors (v);
    writeln ("max number of outgoing edges ", max_edges);
    if !DEBUG_NEIGHBOR_HISTOGRAM {
      var zero_edge_vs = + reduce [v in G.Row] (v.numNeighbors() == 0):int;
      writeln("number of disconnected vertices ", zero_edge_vs);
    }

   if DEBUG_NEIGHBOR_HISTOGRAM {
    var edge_count : [0..max_edges] int = 0;

    for v in G.vertices do
      edge_count (G.n_Neighbors (v)) += 1;

    writeln ("histograph of node distributions by number of outgoing edges");
    writeln ( "# edges  # nodes");
    for count in 0..max_edges do
      writeln ( count, "  ", edge_count (count) );
   }

    writeln ();

  }
