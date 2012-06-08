//
// Input/output of an RMAT graph to files in binary format.
// Presently, uses the XMT/KM RMAT graph storage format for SSCA#2.
//
module io_RMAT_graph
{
  //
  // The data is stored in 5 files per graph.
  // The individual files' names are obtained by appending
  // the following strings to the "prefix" arg of the functions
  // Writeout_RMAT_graph() and Readin_RMAT_graph().
  //
  config const VE_FILENAME     = "vertex_edge_info.data";
  config const SV2_FILENAME    = "sv2_snapshot.data";
  config const EV2_FILENAME    = "ev2_snapshot.data";
  config const START_FILENAME  = "start_snapshot.data";
  config const WEIGHT_FILENAME = "weight_snapshot.data";
  config type  IONumType       = int(64);


  //
  // Writeout_RMAT_graph()
  //   Writes out the graph to files.
  //   The file names are a concatenation of 'snapshot_prefix'
  //     and the *_FILENAME constants above.
  //   The debugging style is specified by 'dstyle' and is described below
  //     (see proc interpretDStyle()).
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
    for (u, ux) in (G.vertices, G.Row) {
      if dRow then write("row ", u, ": [", ux.numNeighbors(), "] ");
      if dEdge then writeln(dstyle, " vertex ", u);
      writeNum(sta, startIx);

      for (v, w) in (ux.neighborIDs, ux.edgeWeights) {
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

    const numVertices = G.vertices.numIndices;
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
  // Readin_RMAT_graph()
  //   Reads in the graph from files.
  //   The file names and the debugging style are the same as
  //   for Writeout_RMAT_graph().
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

      // we know how many neighbors we have for this vertex
      G.Row(u).ndom = 1..numEdges;
      var neighborIDs => G.Row(u).neighborIDs;
      var edgeWeights => G.Row(u).edgeWeights;

      for ix in 1..numEdges {
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

        // record the edge in the vertex
        neighborIDs(ix) = v;
        edgeWeights(ix) = w;

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


  ///////// helpers /////////

  //
  // 'dstyle' specifies the debugging style:
  //   "-"        no debug output, just start/end/summary
  //   "summary"  verbose summary
  //   "row"      DEBUG_GRAPH_GENERATOR-style one line per row
  //   otherwise  one line per edge (enables sorting), prefixed by 'dstyle'
  //
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
  // Especially usefil with --checkOnlyOnRead.
  //
  proc myerror(args...) {
    stderr.writeln("ERROR: ", (...args));
    stderr.writeln("(proceeding nonetheless)");
  }

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

  proc graphTotalEdges(G)  return + reduce [v in G.Row] v.numNeighbors();
  proc graphNumVertices(G) return G.vertices.numIndices;

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
    var err:syserr = ENOERR, temp:IONumType;
    chan.read(temp, error=err);
    // temp==0 is a workaround for unending large files
    if err != EEOF && temp != 0 then
      myerror("did not reach EOF in '", snapshot_prefix, file_suffix,
              "'  the next value is ", temp);
  }

  proc writeNum(ch, num): void { ch.write(num:IONumType); }
  proc readNum(ch): IONumType  return ch.read(IONumType);

}
