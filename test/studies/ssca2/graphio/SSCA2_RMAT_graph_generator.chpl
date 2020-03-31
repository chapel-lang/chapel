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

use SSCA2_compilation_config_params;
private use IO;

record directed_vertex_pair {
  var start = 1: int;
  var end   = 1: int;
}

proc +(l: directed_vertex_pair, r: directed_vertex_pair)
    return new directed_vertex_pair (l.start + r.start, l.end + r.end);


// ============================
// Quadrant selection algorithm
// ============================

proc assign_quadrant ( u: real, a: real, b: real, c: real, d : real,
                      bit : int ) : directed_vertex_pair
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
    var edge : directed_vertex_pair;

    if u <= a then
      {}
    else if u <= a + b then
      { end_inc = 1;}
    else if u <= a + b + c then
      { start_inc = 1;}
    else
      { start_inc = 1; end_inc = 1;};

    edge.start = bit * start_inc;
    edge.end   = bit * end_inc;
    return ( edge );
  }


// ====================================
// Main RMAT Graph Generation Procedure
// ====================================~

proc Gen_RMAT_graph ( a : real,
                     b : real,
                     c : real,
                     d : real,
                     SCALE :int,
                     N_VERTICES : int,
                     n_raw_edges : int,
                     MAX_EDGE_WEIGHT :int,
                     G )

  { use Random;
    writeln("generating RMAT graph");

    const vertex_range = 1..N_VERTICES,
          edge_range   = 1..n_raw_edges,
          rand_range   = 1..n_raw_edges + 1;

    // Random Numbers return in the range [0.0, 1.0)

    var Rand_Gen = if REPRODUCIBLE_PROBLEMS then
                     new owned NPBRandomStream (real, seed = 0556707007)
                   else
                     new owned NPBRandomStream (real);

    var   Noisy_a     : [edge_range] real,
          Noisy_b     : [edge_range] real,
          Noisy_c     : [edge_range] real,
          Noisy_d     : [edge_range] real,
          norm        : [edge_range] real,
          Unif_Random : [edge_range] real,
          Edges       = [edge_range] new directed_vertex_pair ();

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

    var bit = 1 << SCALE;
    var   skip : real;

    for depth in 1..SCALE do {
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

        Edges += assign_quadrant ( Unif_Random, Noisy_a, Noisy_b,
                                   Noisy_c, Noisy_d, bit );

      };

    // ---------------------------------------------------------------------
    // Assign weights to edges randomly, then randomly relabel the vertices
    // to avoid locality from the obvious imbalance that will arise when
    // one of the coefficients is clearly larger than the others
    // ---------------------------------------------------------------------

    var permutation : [vertex_range] int = vertex_range;
    var Edge_Weight : [edge_range] int;

    Rand_Gen.fillRandom ( Unif_Random  );
    Edge_Weight = floor (1 + Unif_Random * MAX_EDGE_WEIGHT) : int;

    Rand_Gen.fillRandom ( Unif_Random (vertex_range) );

    for v in vertex_range do
      { var new_id : int;
        new_id = floor (1 + Unif_Random (v) * N_VERTICES) : int;
        permutation (v) <=> permutation (new_id);
      };

    var node_count : [vertex_range] int = 0;

    //      for e in edge_range do {
    //	Edges(e).start = permutation (Edges(e).start);
    //	Edges(e).end   = permutation (Edges(e).end  );
    //      }
    Edges.start = permutation (Edges.start);
    Edges.end   = permutation (Edges.end  );

    if DEBUG_GRAPH_GENERATOR || DEBUG_WEIGHT_GENERATOR then {
      writeln ();
      for e in edge_range do
        writeln ("edge (", e, ") : (", Edges(e), ", ",
                 Edge_Weight (e), ")" );
    }

    // --------------------------
    // Graph consistency checking
    // --------------------------

    assert ( Edges.start > 0,
             "edge start vertices out of low end of range");

    assert ( Edges.end > 0,
             "edge end vertices out of low end of range");

    assert ( Edges.start <= 2**SCALE,
             "edge start vertices out of high end of range");

    assert ( Edges.end <= 2**SCALE,
             "edge end vertices out of high end of range");

    assert ( Edge_Weight > 0,
             "edge weight out of low end of range");

    assert ( Edge_Weight <= MAX_EDGE_WEIGHT,
             "edge weight out of high end of range");

    writeln (); writeln ("Vertex Set in G:", G.vertices);

    // ----------------------------------------------------------
    // Kernel 1: assemble graph from list of triples.  Include
    // only non-self incident edges.  in case of duplicates, last
    // in wins (+= instead of = works to take sum of weights)
    // ----------------------------------------------------------

    var collisions = 0, self_edges = 0;
    for e in edge_range do {
      var u = Edges (e).start;
      var v = Edges (e).end  ;

      if ( v != u ) then {
        if G.Neighbors (u).contains(v) then {
          collisions += 1;
        }
        else {
          G.Neighbors (u).add (v);
          G.Row(u).Weight (v) = Edge_Weight (e);
        }
      }
      else
        self_edges += 1;
    }

    writeln ( "# of raw edges generated  ", n_raw_edges );
    writeln ( "# of duplicate edges      ", collisions );
    writeln ( "# of self edges           ", self_edges );
    writeln ( "# of edges in final graph ", graphTotalEdges(G));

    if DEBUG_GRAPH_GENERATOR || DEBUG_WEIGHT_GENERATOR then {
      writeln ();
      writeln ("tuples denote (edge, weight)");
      writeln ();
      for u in G.vertices do {
        write ( "row ", u, ": [", G.n_Neighbors (u), "] " );
        for (v,w) in (G .Neighbors (u), G.edge_weight (u) ) do
          write ( (v, w) );
        writeln ();
      }
    }

    var max_edges = max reduce [v in vertex_range] G.n_Neighbors (v);

    var edge_count : [0..max_edges] int = 0;

    for v in G.vertices do
      edge_count (G.n_Neighbors (v)) += 1;

    writeln ("histograph of node distributions by number of outgoing edges");
    writeln ( "# edges  # nodes");
    for count in 0..max_edges do
      writeln ( count, "  ", edge_count (count) );

    writeln ();

  }

//
// RMAT graph I/O
//
// Uses the XMT/KM RMAT graph storage format for SSCA#2.
//

config const VE_FILENAME     = "vertex_edge_info.data";
config const SV2_FILENAME    = "sv2_snapshot.data";
config const EV2_FILENAME    = "ev2_snapshot.data";
config const START_FILENAME  = "start_snapshot.data";
config const WEIGHT_FILENAME = "weight_snapshot.data";
config type  IONumType       = int(64);

config const checkOnlyOnRead = false;

///////// progress indicator /////////

config var progressFrequency = 100000;
var progressCount = 0;
proc resetProgress() {
  progressCount = 0;
}
proc reportProgress() {
  var result = false;
  progressCount -= 1;
  if progressCount <= 0 {
    result = true;
    progressCount = progressFrequency;
  }
  return result;
}

///////// graph helpers /////////

proc graphTotalEdges(G)  return + reduce [v in G.vertices] G.n_Neighbors(v);
proc graphNumVertices(G) return G.vertices.size;

///////// I/O helpers /////////

proc createGraphChannel(prefix:string, suffix:string, param forWriting:bool) {
  const f = open(prefix+suffix,
                 if forWriting then iomode.cw else iomode.r,
                 IOHINT_SEQUENTIAL);
  const chan = if forWriting
    then f.writer(iokind.big, false)
    else f.reader(iokind.big, false);
  return chan;
}

proc ensureEOFofDataFile(chan, snapshot_prefix, file_suffix): void {
  var temp:IONumType;
  try! {
    chan.read(temp);
  } catch e: SystemError {
    // temp==0 is a workaround for unending large files
    if e.err != EEOF && temp != 0 then
      myerror("did not reach EOF in '", snapshot_prefix, file_suffix,
              "'  the next value is ", temp);
  }
}

proc writeNum(ch, num): void { ch.write(num:IONumType); }
proc readNum(ch): IONumType  return ch.read(IONumType);

///////// misc /////////

proc reportNumVerticesError(G, snapshot_prefix, vCount) {
  const vcountLog2 =
    if vCount <= 0 then -1:int(64) else floor(log2(vCount)):int(64);
  const helpMessage =
    if vCount <= 0 then "Note: got 0 or negative vertex count."
    else if exp2(vcountLog2:real(64)) == vCount:real(64) then
      "Pass --SCALE=" + vcountLog2:string + " to suppress the error."
    else "Note: got a vertex count that is not a power of 2.";

  myerror("inconsistent vertex count: graph ", graphNumVertices(G), " vs. ",
          vCount, " in ", snapshot_prefix, VE_FILENAME, "\n", helpMessage);
}

proc interpretDStyle(dstyle:string) {
  const dON   = dstyle != "-";
  const dVerb = dstyle == "summary";
  const dRow  = dstyle == "row";
  // otherwise
  const dEdge = dON && !(dRow || dVerb);

  return (dON, dRow, dEdge);
}

//
// For debugging, we are making these errors non-fatal.
// Especially useful with --checkOnlyOnRead.
//
proc myerror(args...) {
  stderr.writeln("ERROR: ", (...args));
  stderr.writeln("(proceeding nonetheless)");
}

///////// interface functions: /////////
////////   Writeout_RMAT_graph  ////////
////////   Readin_RMAT_graph    ////////

//
// Write out the graph to files starting with 'snapshot_prefix'
// and ending with _FILENAME constants above.
//
// 'dstyle' - debug style:
//   "-"        no debug output, just start/end/summary
//   "summary"  verbose summary
//   "row"      DEBUG_GRAPH_GENERATOR-style one line per row
//   otherwise  one line per edge (enables sorting), prefixed by 'dstyle'
//
proc Writeout_RMAT_graph(G, snapshot_prefix:string, dstyle = "-"): void {
  writeln("writing RMAT graph with ", graphNumVertices(G), " vertices, ",
        graphTotalEdges(G), " edges to '", snapshot_prefix, "'*");

  param wri = true;

  const (dON, dRow, dEdge) = interpretDStyle(dstyle);
  // this presently would leak too much string memory
  //proc debugRow(msg...)  { if dRow then write((...msg)); }
  // ... but this is not called in a loop
  proc debug(msg...) { if dON then writeln((...msg)); }

  const veCount = createGraphChannel(snapshot_prefix, VE_FILENAME, wri);
  writeNum(veCount, graphNumVertices(G));
  writeNum(veCount, graphTotalEdges(G));
  veCount.close();

  const sv = createGraphChannel(snapshot_prefix, SV2_FILENAME, wri);
  const ev = createGraphChannel(snapshot_prefix, EV2_FILENAME, wri);
  const ww = createGraphChannel(snapshot_prefix, WEIGHT_FILENAME, wri);
  const sta = createGraphChannel(snapshot_prefix, START_FILENAME, wri);

  // Tracks the starting index for each vertex, 0-based, for START_FILENAME.
  var startIx = 0: IONumType;
  resetProgress();

  // iterate sequentially
  for u in G.vertices {
    if dRow then write("row ", u, ": [", G.n_Neighbors(u), "] ");
    if dEdge then writeln(dstyle, " vertex ", u);
    writeNum(sta, startIx);

    for v in G.Neighbors(u).sorted() {
      const w = G.Row(u).Weight(v);
      if dRow then write((v, w));
      if dEdge then writeln(dstyle, " ", u, " ", v, " ", w);
      writeNum(sv, u-1);
      writeNum(ev, v-1);
      writeNum(ww, w);
      startIx += 1;
      if dON && reportProgress() then
        writeln("processing...  vertex ", u, "  edge ", startIx);
    }
    if dRow then writeln();
  }

  const numVertices = G.vertices.size;
  debug("done writing ", numVertices, " vertices, ", startIx, " edges");
  assert(startIx == graphTotalEdges(G));
  const sb = numBytes(IONumType);
  debug("file sizes  ", 2 * sb, "  ", (graphNumVertices(G) + 2) * sb,
        "  ", graphTotalEdges(G) * sb);
  writeNum(sta, startIx);
  writeNum(sta, startIx);

  sv.close();
  ev.close();
  ww.close();
  sta.close();

  write("DONE writing RMAT graph");
  if dON then write(" to '", snapshot_prefix, "'*");
  //else write(" with ", numVertices, " vertices, ", startIx, " edges");
  writeln(); writeln();

} // Writeout_RMAT_graph()

//
// Read in the graph from the files as for Writeout_RMAT_graph().
// 'dstyle' - debug style, same as for Writeout_RMAT_graph().
//
proc Readin_RMAT_graph(G, snapshot_prefix:string, dstyle = "-"): void {
  writeln("reading RMAT graph with ", graphNumVertices(G), " vertices",
          " from '", snapshot_prefix, "'*");

  param rea = false;

  const (dON, dRow, dEdge) = interpretDStyle(dstyle);
  // this presently would leak too much string memory
  //proc debugRow(msg...)  { if dRow then write((...msg)); }
  // ... but this is not called in a loop
  proc debug(msg...) { if dON then writeln((...msg)); }

  const veCount = createGraphChannel(snapshot_prefix, VE_FILENAME, rea);
  const vCount    = readNum(veCount),
        eCount    = readNum(veCount);
  ensureEOFofDataFile(veCount, snapshot_prefix, VE_FILENAME);
  veCount.close();

  debug("files contain  ", vCount, " vertices  ", eCount, " edges");
  const sb = numBytes(IONumType);
  debug("expected file sizes  ", 2*sb, "  ", (vCount+2)*sb, "  ", eCount*sb);

  if vCount != graphNumVertices(G) then
    reportNumVerticesError(G, snapshot_prefix, vCount);

  const sv = createGraphChannel(snapshot_prefix, SV2_FILENAME, rea);
  const ev = createGraphChannel(snapshot_prefix, EV2_FILENAME, rea);
  const ww = createGraphChannel(snapshot_prefix, WEIGHT_FILENAME, rea);
  const sta = createGraphChannel(snapshot_prefix, START_FILENAME, rea);

  // see whether the edges in the data file are ordered, for each edge
  //var outOfOrderCnt = 0, prevEnd = 0:IONumType;

  var startIxCnt = 0:IONumType, startIxData = readNum(sta);
  if startIxCnt != startIxData then
    halt("the first index in ", snapshot_prefix, START_FILENAME,
         " is ", startIxData, " but should be ", startIxCnt);
  resetProgress();
  type VType = index(G.vertices);

  for u in 1..(vCount:VType) {
    startIxData = readNum(sta);
    //prevEnd = min(prevEnd.type);
    const numEdges = startIxData - startIxCnt;
    if dRow then write("row ", u, ": [", numEdges, "] ");
    if dEdge then writeln(dstyle, " vertex ", u);

    for 1..numEdges {
      const curStart = readNum(sv)+1,
            v = (readNum(ev)+1):VType,
            w = readNum(ww):VType;

      // initial reporting
      if dRow then write((v, w));
      if dEdge then writeln(dstyle, " ", u, " ", v, " ", w);

      // sanity checks
      if curStart != u then
        myerror("*** start vertex mismatch: expected ",
                u, ", got ", curStart);
      if v < 1 || v > vCount then
        myerror("*** illegal end vertex number: ", v);

      // add the edge to the graph
      if !checkOnlyOnRead {
        G.Neighbors (u).add (v);
        G.Row(u).Weight (v) = w;
      }

      // more reporting
      startIxCnt += 1;
      //if v <= prevEnd then
      //  outOfOrderCnt += 1;
      if dON && reportProgress() then
        writeln("processing...  vertex ", u, "  edge ", startIxCnt);
                //("  outOfOrderCnt ", outOfOrderCnt);
      //prevEnd = v;
    }
    if dRow then writeln();

    // because we run the loop exactly the right number of times
    assert(startIxCnt == startIxData);

  } // for G.vertices

  debug("done reading ", vCount, " vertices, ", startIxData, " edges");
  //debug(if outOfOrderCnt then "outOfOrder edges: " + outOfOrderCnt:string
  //        else "all edges are inOrder");

  // one more entry in 'sta'
  startIxData = readNum(sta);
  if startIxData != startIxCnt then
    myerror("wrong last entry in ", START_FILENAME,
          "  expected ", startIxCnt, "  got ", startIxData);
  if startIxData != eCount then
    myerror(snapshot_prefix, VE_FILENAME, " claimed ", eCount,
          " edges, the other files gave ", startIxData, " edges");

  ensureEOFofDataFile(sv, snapshot_prefix, SV2_FILENAME);
  ensureEOFofDataFile(ev, snapshot_prefix, EV2_FILENAME);
  ensureEOFofDataFile(ww, snapshot_prefix, WEIGHT_FILENAME);
  ensureEOFofDataFile(sta, snapshot_prefix, START_FILENAME);

  sv.close();
  ev.close();
  ww.close();
  sta.close();

  write("DONE reading RMAT graph");
  if dON then write(" from '", snapshot_prefix, "'*");
  else        write(" with ", vCount, " vertices, ", eCount, " edges");
  writeln(); writeln();

} // Readin_RMAT_graph()
