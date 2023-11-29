use GPU;

config param withLoc = false;

config const kind = "min";
const isMin = kind=="min";
assert(isMin || kind=="max");

config const n = 4_000_000_000;

// testing large data:
// 1. is only meaningful if the reduction is actually done on a device
// 2. times out testing if we use CPU-based reduction, especially if it is a
//    fallback.
// So, override n to be something smaller
var nOverride = n;
extern proc chpl_gpu_can_reduce(): bool;
if !chpl_gpu_can_reduce() then nOverride = 100;

config const setIdx = nOverride-1;
assert(nOverride>setIdx);

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
  var Arr: [0..#nOverride] uint(8) = 10;
  Arr[setIdx] = expectedVal;

  result = doReduce(Arr);
}

if printResult then writeln("Result: ", result);

if result != expected then
  writef("Invalid result. Expected %?, actual %?\n", expected, result);
