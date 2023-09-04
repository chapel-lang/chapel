use Time;

config const multitask = false;
config const printOutput = false;
config const validateOutput = false;
config const printSumOnly = validateOutput;
config const numTasks = 2;
config const taskSize = 100;
config const n = 1000;
config const reps = 10;
const numChunks = n/taskSize;

var curChunk: atomic int;

extern proc printf(s...);

var HostIn: [0..#n] real = 2;
var HostOut: [0..#n] real;

var inCommTimer, outCommTimer, kernelTimer: stopwatch;

inline proc kernel(ref MyIn, ref MyOut) {
  @assertOnGpu
  foreach (inData, outData) in zip(MyIn, MyOut) {
    var result = 0.0;
    for i in 0..#reps do result += inData + sqrt(i);
    outData = result;
  }
}

var t: stopwatch;
t.start();
on here.gpus[0] {

  if multitask {
    coforall tid in 0..#numTasks {
      var MyIn, MyOut: [0..#taskSize] real;

      while true {
        const myChunkId = curChunk.fetchAdd(1);
        if myChunkId >= numChunks then break;

        const myChunk = myChunkId*taskSize..#taskSize;

        //writeln(myChunk);
        //printf("%d starting slice %f\n", tid, t.elapsed());
        //ref inRef = HostIn[myChunk];
        //printf("%d starting in copy %f\n", tid, t.elapsed());
        MyIn = HostIn[myChunk];
        //printf("%d finished in copy %f\n", tid, t.elapsed());

        //printf("%d starting kernel %f\n", tid, t.elapsed());
        kernel(MyIn, MyOut);
        //printf("%d finished kernel %f\n", tid, t.elapsed());

        //printf("%d starting out copy %f\n", tid, t.elapsed());
        HostOut[myChunk] = MyOut;
        //printf("%d finished out copy %f\n", tid, t.elapsed());
      }
    }
  }
  else {
    var MyIn, MyOut: [0..#n] real;

    inCommTimer.start();
    MyIn = HostIn;
    inCommTimer.stop();

    kernelTimer.start();
    kernel(MyIn, MyOut);
    kernelTimer.stop();

    outCommTimer.start();
    HostOut = MyOut;
    outCommTimer.stop();
  }
}
t.stop();

if printOutput then writeln(HostOut);
if validateOutput {
  var sum = 0.0;
  for o in HostOut do sum += o;
  if printSumOnly {
    writeln("Sum = ", sum);
  }
  else {
    // only for simpler math
    const expected = n*(2*reps + reps*(reps-1)/2);
    if sum==expected {
      writeln("validation successful");
    }
    else {
      writeln("validation failed sum=", sum, " expected=", expected);
    }
  }
}

writeln("In Comm: ", inCommTimer.elapsed());
writeln("Out Comm: ", outCommTimer.elapsed());
writeln("Kern: ", kernelTimer.elapsed());
writeln("Total (s): ", t.elapsed());
writeln("Throughput (GB/s): ", (n*8)/t.elapsed()/1e9);
