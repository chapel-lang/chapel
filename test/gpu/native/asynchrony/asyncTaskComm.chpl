use Time;

config const multitask = false;
config const printOutput = false;
config const numTasks = 2;
config const taskSize = 100;
config const n = 1000;
config const reps = 10;
const numChunks = n/taskSize;

var curChunk: atomic int;

extern proc printf(s...);

var HostIn: [0..#n] int = 2;
var HostOut: [0..#n] int;

var commTimer, kernelTimer: stopwatch;

var t: stopwatch;
on here.gpus[0] {
  t.start();

  if multitask {
    coforall tid in 0..#numTasks {
      var MyIn, MyOut: [0..#taskSize] int;

      while true {
        const myChunkId = curChunk.fetchAdd(1);
        if myChunkId >= numChunks then break;

        const myChunk = myChunkId*taskSize..#taskSize;


        MyIn = HostIn[myChunk];

        /*printf("task %d entering kernel %f\n", tid, t.elapsed());*/
        foreach (inData, outData) in zip(MyIn, MyOut) {
          for 0..#reps do outData += inData + 1;
        }
        /*printf("task %d exiting kernel %f\n", tid, t.elapsed());*/

        HostOut[myChunk] = MyOut;
      }
    }
  }
  else {
    var MyIn, MyOut: [0..#n] int;

    commTimer.start();
    MyIn = HostIn;
    commTimer.stop();

    kernelTimer.start();
    foreach (inData, outData) in zip(MyIn, MyOut) {
      for 0..#reps do outData += inData + 1;
    }
    kernelTimer.stop();

    commTimer.start();
    HostOut = MyOut;
    commTimer.stop();
  }
  t.stop();

}

if printOutput then writeln(HostOut);

writeln("Comm: ", commTimer.elapsed());
writeln("Kern: ", kernelTimer.elapsed());
writeln("Total: ", t.elapsed());
