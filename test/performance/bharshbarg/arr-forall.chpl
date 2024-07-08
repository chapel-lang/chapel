//
// Measures both array iteration and array access time
//

use MemDiagnostics, Time, Types;

config const zipIter = false;
config const memFraction = 1000;
config const printPerf = false;

type elemType = uint(8);

const totalMem = here.physicalMemory(unit = MemUnits.Bytes);
const target = (totalMem / numBytes(elemType)) / memFraction;
// set a maximum problem size
const n = min(target, 8 * 1e9) : int;

const space = 1..n;
var data : [space] elemType;
var x : elemType = 2;

var time : stopwatch;
time.start();
if zipIter {
  forall (i, j) in zip(data, data) with (ref x) do
    x *= i;
} else {
  forall i in data with (ref x) do
    x *= i;
}
time.stop();

if printPerf then writeln("Time: ", time.elapsed());
