config param rank = 1;
config param debug = false;
config param doVerboseComm = false;
config type myType = int;

config param doBlock = true;
config param doCyclic = true;
config param doBlockCyclic = false;

config const doLocal = false;

use BlockDist;
use CyclicDist;
use BlockCycDist;
use CommDiagnostics;

// Stole this from the setupTargetLocalesArray()
const factors = _factor(rank, numLocales);
var ranges: rank*range;
for param i in 0..rank-1 do
  ranges(i) = 0..#factors(i);
const D = {(...ranges)};

proc printCommDiagnosticsHere(s) {
  printCommDiagnostics(here.id, s);
}
proc printCommDiagnostics(id, s) {
  var diagnostics = getCommDiagnostics()(id);
  writeln(s, "(Locale", id, "): gets: ", diagnostics.get);
  writeln(s, "(Locale", id, "): puts: ", diagnostics.put);
  writeln(s, "(Locale", id, "): executeOns: ", diagnostics.execute_on);
  writeln(s, "(Locale", id, "): fast executeOns: ", diagnostics.execute_on_fast);
  writeln(s, "(Locale", id, "): nb executeOns: ", diagnostics.execute_on_nb);
}

proc nextN() return (here.id+1)%numLocales;
proc prevN() return (here.id+numLocales-1)%numLocales;

proc getIdx(loc) {
  if rank==1 {
    if debug then writeln("getIdx: ", loc, "=>", loc);
    return loc;
  } else {
    var dims = D.dims(); // assume zero-based in all dimensions
    var idx: rank*D.idxType;
    var cur = loc;
    for param i in 0..rank-1 by -1 {
      if dims(i).high!= 0 {
        idx(i) = cur%(dims(i).high+1);
        cur /= dims(i).high+1;
      }
    }
    if debug then writeln("getIdx: ", loc, "=>", idx);
    return idx;
  }
}

proc doit(D, str) {
  var A: [D] myType;
  const s = "["+rank:string+"D "+str+"]";
  if debug then [i in D] writeln("A[", i, "] is on ", A[i].locale);

  for l in Locales {
    A = [i in D] -here.id;
    on l {
      if debug then writeln("on ", here, ":");
      var hereId = here.id;
      var hereIdx = getIdx(hereId);
      var nextId = nextN();
      var nextIdx = getIdx(nextId);
      var prevId = prevN();
      var prevIdx = getIdx(prevId);
      if doLocal {
        A[hereIdx];
        if doVerboseComm then startVerboseCommHere();
        resetCommDiagnosticsHere();
        startCommDiagnosticsHere();
        const c = A[hereIdx];
        stopCommDiagnosticsHere();
        if doVerboseComm then stopVerboseCommHere();
        printCommDiagnosticsHere(s+" Local read  ");
        if debug then writeln("hereId=", hereId, "c=", c);
      }

      A[nextIdx];
      if doVerboseComm then startVerboseCommHere();
      resetCommDiagnosticsHere();
      startCommDiagnosticsHere();
      const c = A[nextIdx];
      stopCommDiagnosticsHere();
      if doVerboseComm then stopVerboseCommHere();
      printCommDiagnosticsHere(s+" Remote read ");
      if debug then writeln("nextId=", nextId, " c=", c);

      if doLocal {
        A[nextIdx];
        if doVerboseComm then startVerboseCommHere();
        resetCommDiagnosticsHere();
        startCommDiagnosticsHere();
        A[hereIdx] = hereId;
        stopCommDiagnosticsHere();
        if doVerboseComm then stopVerboseCommHere();
        printCommDiagnosticsHere(s+" Local write ");
        if debug then writeln("hereId=", hereId,
                              " A[", hereIdx, "]=", A[hereIdx]);
      }

      A[prevIdx];
      if doVerboseComm then startVerboseCommHere();
      resetCommDiagnosticsHere();
      startCommDiagnosticsHere();
      A[prevIdx] = prevId;
      stopCommDiagnosticsHere();
      if doVerboseComm then stopVerboseCommHere();
      printCommDiagnosticsHere(s+" Remote write ");
      if debug then writeln("prevId=", prevId,
                            " A[", prevIdx, "]=", A[prevIdx]);
    }
    if debug then writeln(A);
  }
}

proc fillIt(stuff: ?stuffType) {
  var t: rank*stuffType;
  for param i in 0..rank-1 do
    t(i) = stuff;
  return t;
}

const zeroT = fillIt(0);
const oneT = fillIt(1);

proc main() {
  writeln(rank, " dimensional array of ", myType:string);

  if doBlock {
    writeln();
    const bD: domain(rank) dmapped new dmap (new Block(boundingBox=D)) = D;
    doit(bD, "Block");
  }

  if doCyclic {
    writeln();
    const cD: domain(rank) dmapped new dmap (new Cyclic(startIdx=zeroT)) = D;
    doit(cD, "Cyclic");
  }

  if doBlockCyclic {
    writeln();
    const bcD: domain(rank) dmapped new dmap (new BlockCyclic(rank=rank,idxType=int,startIdx=zeroT,blocksize=oneT)) = D;
    doit(bcD, "BlockCyclic");
  }

}
