use GPU, ChplConfig;

config param withLoc = false;

config const kind = "min";
const isMin = kind=="min";
assert(isMin || kind=="max");

config var n = 4_000_000_000;
config var setIdx = n-1;

// testing large data:
// 1. is only meaningful if the reduction is actually done on a device
// 2. times out testing if we use CPU-based reduction, especially if it is a
//    fallback.
// So, override n/setIdx to be something smaller
if CHPL_GPU=="cpu" {
  n = 100;
  setIdx = n-1;
}

assert(n>setIdx);

config const printResult = false;

const expectedVal = if isMin then 7:uint(8) else 13:uint(8);
const expected = if withLoc then (expectedVal, setIdx) else expectedVal;

inline proc doReduce(Arr) {
  if withLoc then
    return if isMin then gpuMinLocReduce(Arr) else gpuMaxLocReduce(Arr);
  else
    return if isMin then gpuMinReduce(Arr) else gpuMaxReduce(Arr);
}

var result: if withLoc then (uint(8), int) else uint(8);
on here.gpus[0] {
  var Arr: [0..#n] uint(8) = 10;
  Arr[setIdx] = expectedVal;

  result = doReduce(Arr);
}

if printResult then writeln("Result: ", result);

if result != expected then
  writef("Invalid result. Expected %?, actual %?\n", expected, result);
