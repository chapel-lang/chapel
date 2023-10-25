
use GPU;

config const kind = "min";
const isMin = kind=="min";
assert(isMin || kind=="max");

config const n = 2*max(int(32));
config const setIdx = n-1;
assert(n>setIdx);

config const printResult = false;
const expected = if isMin then 7:uint(8) else 13:uint(8);

var result: uint(8);
on here.gpus[0] {
  var Arr: [0..#n] uint(8) = 10;

  Arr[setIdx] = expected;

  result = if isMin then gpuMinReduce(Arr) else gpuMaxReduce(Arr);
}

if printResult then writeln("Result: ", result);

if result != expected then
  writef("Invalid result. Expected %u, actual %u\n", expected, result);
