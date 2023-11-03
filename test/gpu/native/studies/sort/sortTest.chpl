import GpuSort;
import Random;
import Time;

config const arrSize = 10;
config const validate = true;
config const printArr = false;
config const printTimes = false;

proc checkSorted(arr: [] uint) {
  for i in 0..<arr.size-1 {
    if arr[i] > arr[i+1] {
      writeln("Not sorted");
      return;
    }
    if arr[i]==0 {
      writeln("Zero");  // sanity check. This should *not* happen
                        //based on our random seed
      return;
    }
  }
  writeln("Sorted");
}

on here.gpus[0]{
  var timer: Time.stopwatch;
  var arr: [0..<arrSize] uint;
  var seed = 17; // Make it deterministic
  Random.fillRandom(arr, seed);
  if printArr then writeln(arr);

  timer.start();
  GpuSort.sort(arr);
  timer.stop();

  if printArr then writeln(arr);
  if printTimes then writeln("Time: ", timer.elapsed());
  if validate then checkSorted(arr);
}
