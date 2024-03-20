use Time;

config const multitask = false;
config const printOutput = false;
config const printSum = false;
config const printTimes = false;
config const numTasks = 2;
config const taskSize = 100;
config const n = 1000;
config const reps = 10;
const numChunks = n/taskSize;

var curChunk: atomic int;

extern proc printf(s...);

var HostIn: [0..#n] real = 2;
var HostOut: [0..#n] real;

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

        MyIn = HostIn[myChunk];
        kernel(MyIn, MyOut);
        HostOut[myChunk] = MyOut;
      }
    }
  }
  else {
    var MyIn, MyOut: [0..#n] real;

    MyIn = HostIn;
    kernel(MyIn, MyOut);
    HostOut = MyOut;
  }
}
t.stop();

if printOutput then writeln(HostOut);
if printSum {
  var sum = 0.0;
  for o in HostOut do sum += o;
  writeln("Sum = ", sum);
}

if printTimes {
  writeln("Total (s): ", t.elapsed());
  writeln("Throughput (GB/s): ", (n*8)/t.elapsed()/1e9);
}
