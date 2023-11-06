import GpuSort;
import Random;
import Time;

config const arrSize = 100_000;
config const validate = true;
config const printArr = false;
config const printTimes = false;
config const chunkSize = 6250;
config const bitsAtATime = 8;
config const noisy = false;
config const low=10;


proc checkSorted(arr: [] uint) {
  for i in arr.domain {
    if i< arr.domain.high && arr[i] > arr[i+1] {
      writeln("Not sorted");
      return;
    }
    if arr[i]==0 {
      writeln("Zero");  // sanity check. This should *not* happen
                        // based on our random seed
      return;
    }
  }
  writeln("Sorted");
}


var cpuArr: [low..#arrSize] uint;
var seed = 17; // Make it deterministic
Random.fillRandom(cpuArr, seed);

writeln("Starting Sort");
var timer: Time.stopwatch;
if printArr then writeln(cpuArr);

on here.gpus[0]{
  var arr = cpuArr; // Copy to gpu
  timer.start();
  GpuSort.parallelRadixSort(arr, bitsAtATime, chunkSize, noisy);
  timer.stop();
  cpuArr = arr; // Copy back to cpu
}

if printArr then writeln(cpuArr);
if printTimes then writeln("Time: ", timer.elapsed());
if validate then checkSorted(cpuArr);
