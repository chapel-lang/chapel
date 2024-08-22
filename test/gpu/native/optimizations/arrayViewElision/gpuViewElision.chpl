use Time;

config type elemType = int;
var t: stopwatch;
config const correctness = false;
config const useLoop = false;
config const arrSize = 10;
config const totalSeconds = if correctness then 0 else 5;
config const checkGranularity = if correctness then 1 else 10000;

var totalOps = 0;
on here.gpus[0] {
  var Arr1: [1..arrSize] elemType;
  var Arr2: [1..arrSize] elemType = 1;

  t.start();
  do {
    for 1..checkGranularity {
      Arr1[1..arrSize] = Arr2[1..arrSize];
    }

    totalOps += checkGranularity;
  } while t.elapsed() < totalSeconds;
  t.stop();



  if correctness {
    writeln(Arr1[4]);
  }
  else {
    const memCopied: real = totalOps*arrSize*numBytes(elemType);
    writef("Total time(s): %.2dr\n", t.elapsed());
    writef("Total memory copied(GB): %.5dr\n", memCopied/2**30);
    writef("Sustained throughput(GB/s): %.5dr\n", memCopied/2**30/t.elapsed());
  }
}
