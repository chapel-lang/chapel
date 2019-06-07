use BlockDist;

config const filename = "myBlockArr.bin";

config const m = 5, n = 4;

var Dom = newBlockDom({1..m, 1..n});

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
