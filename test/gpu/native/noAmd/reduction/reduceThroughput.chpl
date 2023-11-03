use GPU;
use Time;
use ChplConfig;

config type dataType = int;
config param useGpu = CHPL_LOCALE_MODEL == "gpu";

config const n = 100;
config const reportPerf = true;

var t: stopwatch;

inline proc doSumReduce(const ref Arr) {
  if useGpu then
    return gpuSumReduce(Arr);
  else {
    return + reduce Arr;
  }
}

writeln("Using ", if useGpu then "gpu" else "cpu");

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
