use CTypes;
use Time;

config type elemType = int;
var t: stopwatch;
config const totalSeconds = 5;
config const useLoop = false;
config const arrSize = 10;
config const checkGranularity = 10000;
config const correctness = false;

const arrRange = 1..arrSize;
const sliceRange = 2..arrSize;
var Arr1, Arr2: [1..arrSize] elemType;

Arr2 = 2;

Arr2[2] = 4;

t.start();
var totalOps = 0;
do {
  if useLoop {
    for 1..checkGranularity {
      for i in 2..arrSize {
        Arr1[i] = Arr2[i];
      }
    }
  }
  else {
    for 1..checkGranularity {
      Arr1[2..arrSize] = Arr2[2..arrSize];
    }
  }

  totalOps += checkGranularity;
} while t.elapsed() < totalSeconds;
t.stop();

assert(Arr2[2] == 4);

if correctness {
  assert(totalOps == 1);
  for a in Arr2[3..] do assert(a == 2);
}
else {
  const memCopied: real = totalOps*sliceRange.size*numBytes(elemType);
  writef("Total time(s): %.2dr\n", t.elapsed());
  writef("Total memory copied(GB): %.2dr\n", memCopied/2**30);
  writef("Sustained throughput(GB/s): %.2dr\n", memCopied/2**30/t.elapsed());
}
