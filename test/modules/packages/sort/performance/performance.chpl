/*
    General performance test of sort routines. Sorts 2**M bytes of random data

    Note: The correctness test for this is simply checking that it compiles and
          runs without errors
 */

use Sort;
use Random;
use Time;

config const M: int = 6,                    // 2**M bytes
             correctness: bool = true,      // Disables output
             sorts: string = 'qhims';       // Sorts to use (first letter)

// Array properties
config type T = int;                // Type of array
config const strided = false;       // Stride of array

// Number of elements
const N: int = (2**M / numBytes(T)): int;

proc main() {
  if !strided then
    default(N);
  else
    stride(N);
}

proc default(N) {
  print('default');
  const D = {1..N};
  var A: [D] T;
  fillRandom(A, seed=42);
  gatherTimings(A);
}

proc stride(N) {
  print('stride');
  const D = {1..N*2 by 2};
  var A: [D] T;
  fillRandom(A, seed=42);
  gatherTimings(A);
}

/* Collect timings for sort functions */
proc gatherTimings(const ref A) {
  var t = new Timer();
  print('Time taken to sort ', 2**M, ' bytes (', A.size, ' ', T:string, 's)');
  if sorts.find('q')
  {
    var B = A;
    t.start();
    quickSort(B);
    t.stop();
    print('quickSort (seconds): ', t.elapsed());
    t.clear();
  }
  if sorts.find('h')
  {
    var B = A;
    t.start();
    heapSort(B);
    t.stop();
    print('heapSort (seconds): ', t.elapsed());
    t.clear();
  }
  if sorts.find('i')
  {
    var B = A;
    t.start();
    insertionSort(B);
    t.stop();
    print('insertionSort (seconds): ', t.elapsed());
    t.clear();
  }
  if sorts.find('m')
  {
    var B = A;
    t.start();
    mergeSort(B);
    t.stop();
    print('mergeSort (seconds): ', t.elapsed());
    t.clear();
  }
  if sorts.find('s')
  {
    var B = A;
    t.start();
    selectionSort(B);
    t.stop();
    print('selectionSort (seconds): ', t.elapsed());
    t.clear();
  }
  if sorts.find('b')
  {
    var B = A;
    t.start();
    bubbleSort(B);
    t.stop();
    print('bubbleSort (seconds): ', t.elapsed());
    t.clear();
  }
}

proc print(args...) {
  if !correctness then
    writeln((...args));
}
