/*
 *  Check correctness of sort routines. Output nothing if correct.
 */

//use Sort; // commented out to demonstrate that qualified access works
// This version of the test was created to demonstrate that the issues with
// qualified access of modules that are included by default now works.  Should
// Sort no longer be default included, a new test should probably be written.
use Random;

proc main() {

  // Array to sort
  var Arr = [-1,-4, 2, 3],
   StrArr = ['anthony', 'david', 'Brad', 'ben'];

  // Correctness arrays
  const ArrAbsSorted = [-1, 2, 3, -4],
           ArrSorted = [-4, -1, 2, 3],
        ArrRevSorted = [ 3, 2, -1, -4],
     ArrAbsRevSorted = [ -4, 3, 2, -1],
        StrArrSorted = ['Brad', 'anthony', 'ben', 'david'];


  /* Test data types */
  // Integers
  Sort.sort(Arr);
  checkSort(Arr, ArrSorted, 'sort');

  // Strings
  Sort.sort(StrArr);
  checkSort(StrArr, StrArrSorted, 'sort');

  /* Test Sorts */

  // bubbleSort
  Sort.bubbleSort(Arr);
  checkSort(Arr, ArrSorted, 'bubbleSort');

  // insertionSort
  Sort.insertionSort(Arr);
  checkSort(Arr, ArrSorted, 'insertionSort');

  // quickSort
  Sort.quickSort(Arr);
  checkSort(Arr, ArrSorted, 'quickSort');

  // heapSort
  Sort.heapSort(Arr);
  checkSort(Arr, ArrSorted, 'heapSort');

  // selectionSort
  Sort.selectionSort(Arr);
  checkSort(Arr, ArrSorted, 'selectionSort');

  /* Test deprecated sorts */
  // TODO - remove these for 1.15

  // BubbleSort
  Sort.BubbleSort(Arr);
  checkSort(Arr, ArrSorted, 'BubbleSort');

  Sort.BubbleSort(Arr, doublecheck=true);
  checkSort(Arr, ArrSorted, 'BubbleSort');

  Sort.BubbleSort(Arr, reverse=true);
  checkSort(Arr, ArrRevSorted, 'BubbleSort');

  // InsertionSort
  Sort.InsertionSort(Arr);
  checkSort(Arr, ArrSorted, 'InsertionSort');

  Sort.InsertionSort(Arr, doublecheck=true);
  checkSort(Arr, ArrSorted, 'InsertionSort');

  Sort.InsertionSort(Arr, reverse=true);
  checkSort(Arr, ArrRevSorted, 'InsertionSort');

  // quickSort
  Sort.QuickSort(Arr);
  checkSort(Arr, ArrSorted, 'quickSort');

  Sort.QuickSort(Arr, doublecheck=true);
  checkSort(Arr, ArrSorted, 'quickSort');

  Sort.QuickSort(Arr, reverse=true);
  checkSort(Arr, ArrRevSorted, 'quickSort');

  // HeapSort
  Sort.HeapSort(Arr);
  checkSort(Arr, ArrSorted, 'HeapSort');

  Sort.HeapSort(Arr, doublecheck=true);
  checkSort(Arr, ArrSorted, 'HeapSort');

  Sort.HeapSort(Arr, reverse=true);
  checkSort(Arr, ArrRevSorted, 'HeapSort');

  // SelectionSort (not actually deprecated yet)
  Sort.SelectionSort(Arr);
  checkSort(Arr, ArrSorted, 'SelectionSort');

  Sort.SelectionSort(Arr, doublecheck=true);
  checkSort(Arr, ArrSorted, 'SelectionSort');

  Sort.SelectionSort(Arr, reverse=true);
  checkSort(Arr, ArrRevSorted, 'SelectionSort');

}


/* Checks array and resets values -- any output results in failure */
proc checkSort(ref array, correct, sort:string, comparator:string='none') {
  if !array.equals(correct) {
    writeln(sort, ' with comparator: ', comparator, ' failed');
    writeln('Incorrect array:');
    writeln(array);
    writeln('Expected correct array:');
    writeln(correct);
  }
  shuffle(array, seed=42);
}
