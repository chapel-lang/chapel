import Random;
import Time;
import GPU;

config const useGpuId = 0;
config const arrSize = 100_000;
config const validate = true;
config const printArr = false;
config const printTimes = false;
config const chunkSize = 6250;
config const bitsAtATime = 8;
config const noisy = false;
config const low=10;


proc checkSorted(arr: [] uint) {
  const minVal = min reduce arr;
  for i in arr.domain {
    if i< arr.domain.high && arr[i] > arr[i+1] {
      writeln("Not sorted");
      return;
    }

    // Sanity Check:
    // The algorithm has a tendency to mess up in a way that the output array
    // ends up with either some zeroes at the beginning or all zeroes. This is
    // technically a sorted array, but it's is the wrong answer,
    // This check will catch that case unless the minVal is 0.
    if arr[i]<minVal {
      writeln("Bad Sort");
      return;
    }
  }
  writeln("Sorted");
}


var cpuArr: [low..#arrSize] uint;
config const seed = (new randomStream(int(32))).seed;
Random.fillRandom(cpuArr, seed);
var cpuArr2 = cpuArr;

writeln("Starting Sort");
var timer: Time.stopwatch;
if printArr then writeln(cpuArr);

on here.gpus[useGpuId]{
  var arr = cpuArr; // Copy to gpu
  timer.start();
  GPU.parallelRadixSort(arr, bitsAtATime, chunkSize, noisy);
  timer.stop();
  cpuArr = arr; // Copy back to cpu
}
const time1 = timer.elapsed();

on here.gpus[useGpuId] {
  var arr = cpuArr; // Copy to gpu
  timer.clear();
  timer.start();
  GPU.gpuSort(arr);
  timer.stop();
  cpuArr2 = arr; // Copy back to cpu
}
const time2 = timer.elapsed();

if printArr then writeln(cpuArr);
if printTimes {
  writeln("Time1 : ", time1);
  writeln("Time2 : ", time2);
}
if validate {
  checkSorted(cpuArr);
  checkSorted(cpuArr2);
}
