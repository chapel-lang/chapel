use GPU;
use Time;
use ChplConfig;

config type dataType = int;
config const useGpu = true;
config const useExpr = true;

config const n = 100;
config const reportPerf = true;

var t: stopwatch;

inline proc doSumReduce(const ref Arr) {
  if useExpr then
    return + reduce Arr;
  else
    return gpuSumReduce(Arr);
}

writeln("Using ", if useGpu then "gpu" else "cpu");
if reportPerf then // otherwise a complication for correctness testing
  writeln("Using ", if useExpr then "expression" else "function");

on if useGpu then here.gpus[0] else here {
  var Arr: [0..<n] dataType;

  forall i in Arr.domain do Arr[i] = (i%100):dataType;

  var result;
  t.start();
  result = doSumReduce(Arr);
  t.stop();
  writeln(result);
}

if reportPerf {
  writeln("Time (s): ", t.elapsed());
  writeln("Throughput (GiOP/s): ", n/t.elapsed()/(1<<30));
  writeln("Throughput (GiB/s): ", numBytes(dataType)*n/t.elapsed()/(1<<30));
}
