use BlockDist;

config const filename = "hello.txt";

var Dom = newBlockDom({1..5, 1..4});
var A: [Dom] int;

for (i,j) in Dom {
  A[i,j] = i*10 + j;
}

checkpointArray(filename, A);
writeln("Wrote checkpoint file");
A = 0;
writeln("Zeroed 'A'");
readArrayCheckpoint(filename, A);
writeln("Read checkpoint file");
writeln(A);

proc readArrayCheckpoint(filename: string, A:[] ?t, nTasks: int = dataParTasksPerLocale) {
  var targetLocs = reshape(A.targetLocales(), {1..A.targetLocales().numElements});
  var offsetDom = targetLocs.domain;
  var offsetArr: [offsetDom] int;

  forall idx in targetLocs.domain {
    offsetArr[idx] = + reduce A.localSubdomains(targetLocs(idx)).numIndices;
  }

  var cumulativeOffsets = + scan offsetArr;

  coforall idx in offsetDom do on targetLocs[idx] {
    var f = open(filename, iomode.r);
    const eltSize = numBytes(A.eltType);
    const start = eltSize * if idx == offsetDom.low then 0 else cumulativeOffsets[idx-1];
    const end = eltSize * cumulativeOffsets[idx];
    var ch = f.reader(kind = ionative, start = start, end = end);

    for sub in A.localSubdomains(targetLocs(idx)) {
      ch.read(A[sub]);
    }
  }
}


/*
proc readArrayCheckpoint(filename: string, A:[] ?t, nTasks: int = dataParTasksPerLocale) {
  use Barriers, RangeChunk;
  var targetLocs = reshape(A.targetLocales(), {1..A.targetLocales().numElements});
  var offsetDom = targetLocs.domain;
  var offsetArr: [offsetDom] int;

  forall idx in targetLocs.domain {
    offsetArr[idx] = + reduce A.localSubdomains(targetLocs(idx)).numIndices;
  }

  var cumulativeOffsets = + scan offsetArr;
  var bar = new Barrier(nTasks);

  coforall idx in offsetDom do on targetLocs[idx] {
    var f = open(filename, iomode.r);
/*
    var ch = f.reader(kind = ionative,
                      start = start*numBytes(A.eltType),
                      end = cumulativeOffsets[idx]*numBytes(A.eltType));
*/
    coforall tid in 0..nTasks {
      var start = if idx == offsetDom.low then 0 else cumulativeOffsets[idx-1];
      var ch = f.reader(kind = ionative,
                        start = start*numBytes(A.eltType),
                        end = cumulativeOffsets[idx]*numBytes(A.eltType));
      for sub in A.localSubdomains(targetLocs(idx)) {
        //var myChunk: domain(A.rank);
        var dims: A.rank*range;
        for param dim in 2..A.rank {
          dims(dim) = sub.dim(dim);
        }
        dims(1) = chunk(sub.dim(1), nTasks, tid);
        var myChunk = {(...dims)};
        var myStart = start + myChunk.low - sub.low;
        var ch = f.reader(kind = ionative,
                          start = myStart*numBytes(A.eltType),
                          end = (myStart+myChunk.numIndices)*numBytes(A.eltType));
        ch.read(A[myChunk]);
        bar.barrier();
        if tid == 0 then
          start += sub.numIndices;
        bar.barrier();
      }
    }
  }
}
*/

proc checkpointArray(filename: string, A:[] ?t, nTasks: int = dataParTasksPerLocale) {
  var f = open(filename, iomode.cw); // create/clear the file
  var targetLocs = reshape(A.targetLocales(), {1..A.targetLocales().numElements});
  var offsetDom = targetLocs.domain;
  var offsetArr: [offsetDom] int;

  forall idx in targetLocs.domain {
    offsetArr[idx] = + reduce A.localSubdomains(targetLocs(idx)).numIndices;
  }

  var cumulativeOffsets = + scan offsetArr;

  coforall idx in offsetDom do on targetLocs[idx] {
    var f = open(filename, iomode.rw); // need to open the file on each locale
    const eltSize = numBytes(A.eltType);
    const start = eltSize * if idx == offsetDom.low then 0 else cumulativeOffsets[idx-1];
    const end = eltSize * cumulativeOffsets[idx];
    var ch = f.writer(kind = ionative,
                      start = start,
                      end = end);
    for sub in A.localSubdomains(targetLocs(idx)) {
      ch.write(A[sub]);
    }
  }
}
/*
  coforall loc in A.targetLocales() do on loc {
    coforall tid in 0..#nTasks {
      for dom in A.localSubdomains() {
        var ch = f.writer(kind=ionative, start=?, end=?);
      }
    }
  }
*/
/*
proc checkpointArray(filename: string, A:[] ?t, nTasks: int = dataParTasksPerLocale) {
  var f = open(filename, iomode.cw);
  var beSerial = false;
  //var map: [A.targetLocales().domain.dim(1), 1..nTasks] int;
  var Dim: domain(A.rank*range);
  var map: [Dim] int;
  
  serial beSerial {
    var runningSum = 0;
    for followThis in A.domain.these(iterKind.leader) {
      writeln((followThis, runningSum));
      Dim += followThis;
      map[followThis] = runningSum;
      runningSum += {(...followThis)}.numIndices;
    }
  }

  writeln(map);
/*
  for followThis in A.domain.these(iterKind.leader) {
    var ch = f.writer(kind=ionative, start=?, end=?);
    
    for i in followThis {
      ch.write(A[i]);
    }
  /*
      writeln(here, " ", followThis);
      const locD = {(...followThis)};
      writeln(A[locD]);
  */
  }
*/
*/
