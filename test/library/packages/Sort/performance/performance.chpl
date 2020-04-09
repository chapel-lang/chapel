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
             sorts: string = 'qhimsr';      // Sorts to use (first letter)

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
  if sorts.find('q') != -1
  {
    var B = A;
    t.start();
    QuickSort.quickSort(B);
    t.stop();
    if !isSorted(B) then
      writeln('quickSort failed to sort data');
    else
      print('quickSort (seconds): ', t.elapsed());
    t.clear();
  }
  if sorts.find('h') != -1
  {
    var B = A;
    t.start();
    HeapSort.heapSort(B);
    t.stop();
    if !isSorted(B) then
      writeln('heapSort failed to sort data');
    else
      print('heapSort (seconds): ', t.elapsed());
    t.clear();
  }
  if sorts.find('i') != -1
  {
    var B = A;
    t.start();
    InsertionSort.insertionSort(B);
    t.stop();
    if !isSorted(B) then
      writeln('insertionSort failed to sort data');
    else
      print('insertionSort (seconds): ', t.elapsed());
    t.clear();
  }
  if sorts.find('r') != -1
  {
    var B = A;
    t.start();
    BinaryInsertionSort.binaryInsertionSort(B);
    t.stop();
    if !isSorted(B) then
      writeln('binaryInsertionSort failed to sort data');
    else
      print('binaryInsertionSort (seconds): ', t.elapsed());
    t.clear();
  }
  if sorts.find('m') != -1
  {
    var B = A;
    t.start();
    MergeSort.mergeSort(B);
    t.stop();
    if !isSorted(B) then
      writeln('mergeSort failed to sort data');
    else
      print('mergeSort (seconds): ', t.elapsed());
    t.clear();
  }
  if sorts.find('s') != -1
  {
    var B = A;
    t.start();
    SelectionSort.selectionSort(B);
    t.stop();
    if !isSorted(B) then
      writeln('selectionSort failed to sort data');
    else
      print('selectionSort (seconds): ', t.elapsed());
    t.clear();
  }
  if sorts.find('b') != -1
  {
    var B = A;
    t.start();
    BubbleSort.bubbleSort(B);
    t.stop();
    if !isSorted(B) then
      writeln('bubbleSort failed to sort data');
    else
      print('bubbleSort (seconds): ', t.elapsed());
    t.clear();
  }
}

proc print(args...) {
  if correctness && args.size == 2 then
    writeln(args(0), "x.xxx");
  else
    writeln((...args));
}
