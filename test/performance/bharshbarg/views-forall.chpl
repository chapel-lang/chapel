//
// Measures both array iteration and array access time
//

use MemDiagnostics, Time, Types;

config const memFraction = 1000;
config const printPerf = false;

type elemType = uint(8);

const totalMem = here.physicalMemory(unit = MemUnits.Bytes);
const target = (totalMem / numBytes(elemType)) / memFraction;
// set a maximum problem size
const n = min(target, 8 * 1e9) : int;

const space = 1..n;
var data : [space] elemType;
var multiDim : [1..1, space] elemType;

proc testIter(A: [], id : string) {
  var x : elemType = 2;
  var time : stopwatch;
  time.start();
  forall i in A with (ref x) do
    x *= i;
  time.stop();
  if printPerf then writeln(id, "-iter: ", time.elapsed());
}

proc testIndex(A: [], id : string) {
  var x : elemType = 2;
  var time : stopwatch;
  const dom = A.domain;
  time.start();
  forall i in dom with (ref x) do
    x *= A[i];
  time.stop();
  if printPerf then writeln(id, "-index: ", time.elapsed());
}

proc test(A: [], id : string) {
  testIter(A, id);
  testIndex(A, id);
}

test(data, "non-view");
test(data[1..n], "slice");
test(data.reindex(0..#n), "reindex");
test(multiDim[1, ..], "rankchange");
