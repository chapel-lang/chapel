//
// Input/output of an RMAT graph to files in binary format.
// Presently, uses the XMT/KM RMAT graph storage format for SSCA#2.
//
use SSCA2_compilation_config_params, Time, ReplicatedDist;
private use IO, DSIUtil;

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
config param IOendianness    = iokind.big;
config const IOserial        = false;
config const IOsingleTaskPerLocale = true;

// Debugging-related settings.
config const IOprogress      = true;
config var progressFrequency = 100000;
config const IOgate          = false;

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

  var stopwatch : Timer;
  if PRINT_TIMING_STATISTICS then stopwatch.start ();

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
  for (u, ux) in zip(G.vertices, G.Row) {
    if dRow then write("row ", u, ": [", ux.numNeighbors(), "] ");
    if dEdge then writeln(dstyle, " vertex ", u);
    writeNum(sta, startIx);

    for (v, w) in ux.neighborList {
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

  if PRINT_TIMING_STATISTICS then {
    stopwatch.stop ();
    writeln ( "Elapsed time for writing RMAT graph: ", stopwatch.elapsed (),
              " seconds");
    stopwatch.clear ();
  }

  write("DONE writing RMAT graph");
  if dON then write(" to '", snapshot_prefix, "'*");
  //else write(" with ", numVertices, " vertices, ", startIx, " edges");
  writeln(); writeln();

} // Writeout_RMAT_graph()

//
// For each data file, we create one Chapel 'file' per locale when parallel.
// Cheap-to-compute decls are here, the rest are in Readin_RMAT_graph().
//
param repfileLow = 1, repfileHi = 5,
      repfileSV = 1, repfileEV = 2, repfileWW = 3,
      repfileSTA = 4, repfileST2 = 5;
const repfileBase = {repfileLow .. repfileHi};

//
// Readin_RMAT_graph()
//   Reads in the graph from files.
//   The file names and the debugging style are the same as
//   for Writeout_RMAT_graph().
//
proc Readin_RMAT_graph(G, snapshot_prefix:string, dstyle = "-"): void {
  writeln("reading RMAT graph with ", graphNumVertices(G), " vertices",
          if IOserial then " serially" else
            if DISTRIBUTION_TYPE == "BLOCK" && IOsingleTaskPerLocale
              then " in parallel, 1 task per locale" else " in parallel",
          " from '", snapshot_prefix, "'*");

  var stopwatch : Timer;
  if PRINT_TIMING_STATISTICS then stopwatch.start ();

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

  G.num_edges = eCount;
  debug("files contain  ", vCount, " vertices  ", eCount, " edges");
  const sb = numBytes(IONumType);
  debug("expected file sizes  ", 2*sb, "  ", (vCount+2)*sb, "  ", eCount*sb);

  if vCount != graphNumVertices(G) then
    reportNumVerticesError(G, snapshot_prefix, vCount);

  ref GRow = G.Row;
  const uxIDs = GRow.domain.dim(0);
  type VType = uxIDs.idxType;
  compilerAssert(!uxIDs.stridable); // for efficiency

  if IOserial {
    const sv = createGraphChannel(snapshot_prefix, SV2_FILENAME, rea);
    const ev = createGraphChannel(snapshot_prefix, EV2_FILENAME, rea);
    const ww = createGraphChannel(snapshot_prefix, WEIGHT_FILENAME, rea);
    const sta = createGraphChannel(snapshot_prefix, START_FILENAME, rea);

    var startIxCnt = 0:IONumType, startIxData = readNum(sta);
    if startIxCnt != startIxData then
      halt("the first index in ", snapshot_prefix, START_FILENAME,
           " is ", startIxData, " but should be ", startIxCnt);

    for u in uxIDs do
      readOneVertex(GRow, VType, vCount, u, dON, dRow, dEdge, dstyle,
                    sv, ev, ww, startIxData = readNum(sta), startIxCnt);

    debug("done reading ", vCount, " vertices, ", startIxData, " edges");

    // one more entry in 'sta'
    startIxData = readNum(sta);
    if startIxData != startIxCnt then
      myerror("wrong last entry in ", snapshot_prefix, START_FILENAME,
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

  } else {
    // !IOserial
    const repfileDom = repfileBase dmapped Replicated(Locales);
    var repfiles: [repfileDom] file;

    coforall l in Locales do on l {
repfiles[repfileSV] = createGraphFile(snapshot_prefix, SV2_FILENAME, rea);
repfiles[repfileEV] = createGraphFile(snapshot_prefix, EV2_FILENAME, rea);
repfiles[repfileWW] = createGraphFile(snapshot_prefix, WEIGHT_FILENAME, rea);
repfiles[repfileSTA] = createGraphFile(snapshot_prefix, START_FILENAME, rea);
repfiles[repfileST2] = createGraphFile(snapshot_prefix, START_FILENAME, rea);
    }

   if DISTRIBUTION_TYPE == "BLOCK" && IOsingleTaskPerLocale {
      coforall loc in GRow.targetLocales() do on loc {
        const myIDs = GRow.localSubdomain().dim(0);

        // All work is done in graphReaderReal() iterator.
        for graphReaderReal(GRow, uxIDs, VType, vCount, eCount, repfiles,
                            dON, dRow, dEdge, dstyle, myIDs) do;
      }

   } else {
    // !IOsingleTaskPerLocale && !IOserial

    // All work is done in graphReaderIterator() follower iterator.
    // GRow is used only to distribute/parallelize the computation.
    forall (_,_) in zip(GRow,
      graphReaderIterator(GRow, uxIDs, VType, vCount, eCount, repfiles,
                          dON, dRow, dEdge, dstyle))
        do;

   } // if IOsingleTaskPerLocale

    coforall l in Locales do on l {
        repfiles[repfileSV].close();
        repfiles[repfileEV].close();
        repfiles[repfileWW].close();
        repfiles[repfileSTA].close();
        repfiles[repfileST2].close();
    }

  } // if IOserial

  if PRINT_TIMING_STATISTICS then {
    stopwatch.stop ();
    writeln ( "Elapsed time for reading RMAT graph: ", stopwatch.elapsed (),
              " seconds");
    stopwatch.clear ();
  }

  write("DONE reading RMAT graph");
  if dON then write(" from '", snapshot_prefix, "'*");
  else        write(" with ", vCount, " vertices, ", eCount, " edges");
  writeln(); writeln();

} // Readin_RMAT_graph()


///////// parallel Readin_RMAT_graph helpers /////////

// For use with IOgate=true: allows only a single graphReaderIterator
// follower invocation at a time.
var IOgate$: sync bool;

// Seems we need this serial iterator declaration to keep the compiler happy.
// But it is not implemented. Use --IOserial instead.
//
iter graphReaderIterator(GRow, uxIDs, type VType, vCount, eCount, repfiles,
                         dON, dRow, dEdge, dstyle) {
  halt("serial graphReaderIterator should not be invoked");
  yield 0:VType;

}

// This is the follower iterator.
// We do not need the leader iterator.
//
iter graphReaderIterator(GRow, uxIDs, type VType, vCount, eCount, repfiles,
                         dON, dRow, dEdge, dstyle,
                         param tag: iterKind, followThis)
  where tag == iterKind.follower
{
  // ensure we got unstridable range with VType-typed indices
  compilerAssert(followThis.type ==
                 1*range(VType, BoundedRangeType.bounded, false));

  const myIDs = unDensify(followThis(0), uxIDs);

  // Redirect to graphReaderReal() iterator.
  for dummy in graphReaderReal(GRow, uxIDs, VType, vCount, eCount, repfiles,
                               dON, dRow, dEdge, dstyle, myIDs)
    do yield dummy;
}

// This iterator reads the part of the graph corresponding to myIDs,
// on the locale where it is run.
//
iter graphReaderReal(GRow, uxIDs, type VType, vCount, eCount, repfiles,
                     dON, dRow, dEdge, dstyle, myIDs)
{
  if IOgate then IOgate$ = true;

  compilerAssert(!myIDs.stridable); // for efficiency, also for v1,v2
  // start/end IDs
  const v1 = myIDs.low, v2 = myIDs.high;
  //writeln("loc ", here.id, "  myIDs ", v1, "..", v2, "  of ", uxIDs);

  // These indices better be all local, so take advantage of that.
  ref GRowLocal = GRow.localSlice(myIDs);

  // Returns the offset of edgeStart[v] in staf, 1 <= v <= numVertices+2.
  proc staOffsetForVID(v: int) return (v-1) * numBytes(IONumType);
  // Returns the offset of startVertex/endVertex/weight[e] in
  //  svf, evf, wwf, resp; 1 <= e <= numEdges.
  proc svOffsetForEID(e: int) return (e-1) * numBytes(IONumType);

  // We need to read edgeStart(v1) and edgeStart(v2) (in the terminology
  // of the commit message for r19646) - to determine the span of
  // the channels sv, ev, ww.
  // Then we need everything in between - to know how many edges
  // each vertex has.
  //
  // But we need to avoid the undefined behavior when
  // channels refer to overlapping parts of a file.
  // So we read edgeStart(v1) from 'ST2' and the rest from 'STA'.
  //
  const sta_v1 = repfiles[repfileST2].reader(kind = IOendianness,
                                             locking = false,
                                             start = staOffsetForVID(v1),
                                             end = staOffsetForVID(v1+1));
  const sta1 = readNum(sta_v1) + 1;
  sta_v1.close();

  // We read edgeStart(v2) from its own channel.
  const sta_v2 = repfiles[repfileSTA].reader(IOendianness, false,
                            staOffsetForVID(v2+1), staOffsetForVID(v2+1+1));
  const sta2 = readNum(sta_v2);
  sta_v2.close();

  //writeln("edgeStart ", sta1, "..", sta2);
  if !(sta1 < sta2) then
    halt("parallel graph I/O for a small number of vertices is not supported\n",
         "  loc ", here.id, "  myIDs ", v1, "..", v2, "  of ", uxIDs,
         "  edgeStart ", sta1, "..", sta2);

  // We access only our parts these files.
  const sv = repfiles[repfileSV].reader(IOendianness, false,
                        svOffsetForEID(sta1), svOffsetForEID(sta2+1));
  const ev = repfiles[repfileEV].reader(IOendianness, false,
                        svOffsetForEID(sta1), svOffsetForEID(sta2+1));
  const ww = repfiles[repfileWW].reader(IOendianness, false,
                        svOffsetForEID(sta1), svOffsetForEID(sta2+1));

  // 'sta' covers edgeStart(v1+1..v2).
  // Do not include v1, as another process will be reading it from its 'STA'.
  const sta = repfiles[repfileSTA].reader(IOendianness, false,
                         staOffsetForVID(v1+1), staOffsetForVID(v2+1+1));

  var startIxCnt = sta1 - 1;

  for u in v1:VType..v2:VType {
    readOneVertex(GRowLocal, VType, vCount, u, dON, dRow, dEdge, dstyle,
                  sv, ev, ww, startIxData = readNum(sta), startIxCnt);

    // this needs to look like an iterator
    yield u;

  } // for u

  if startIxCnt != sta2 then
    halt("*** mismatch ***  ", startIxCnt, " vs. ", sta2);

  sv.close();
  ev.close();
  ww.close();
  sta.close();

  if IOgate then IOgate$;
} // graphReaderReal

proc readOneVertex(GRow, type VType, vCount, u, dON, dRow, dEdge, dstyle,
                   sv, ev, ww, startIxData, inout startIxCnt)
{
  const numEdges = startIxData - startIxCnt;
  if dRow then write("row ", u, ": [", numEdges, "] ");
  if dEdge then writeln(dstyle, " vertex ", u);

  // we know how many neighbors we have for this vertex
  //
  GRow(u).ndom = 1..numEdges;
  ref neighborList = GRow(u).neighborList;

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
    neighborList(ix) = nleMake(v, w);

    // more reporting
    startIxCnt += 1;
    if dON && reportProgress() then
      writeln("processing...  vertex ", u, "  edge ", startIxCnt);
  }
  if dRow then writeln();

  // because we run the loop exactly the right number of times
  assert(startIxCnt == startIxData);

} // readOneVertex

///////// helpers /////////

//
// 'dstyle' specifies the debugging style:
//   "-"        no debug output, just start/end/summary
//   "summary"  verbose summary - do not use when measuring performance
//                while IOprogress=true
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
// Especially useful with --checkOnlyOnRead.
//
proc myerror(args...) {
  try! stderr.writeln("ERROR: ", (...args));
  try! stderr.writeln("(proceeding nonetheless)");
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

var progressCount = 0;
proc resetProgress() {
  progressCount = 0;
}
proc reportProgress() {
  if !IOprogress then return false;
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
proc graphNumVertices(G) return G.vertices.size;

///////// I/O helpers /////////

proc createGraphChannel(prefix:string, suffix:string, param forWriting:bool) {
  const f = createGraphFile(prefix, suffix, forWriting);
  const chan = if forWriting
    then f.writer(IOendianness, false)
    else f.reader(IOendianness, false);
  return chan;
}

proc createGraphFile(prefix:string, suffix:string, param forWriting:bool) {
  return open(prefix+suffix,
              if forWriting then iomode.cw else iomode.r,
              IOHINT_SEQUENTIAL);
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
