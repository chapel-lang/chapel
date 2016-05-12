/*
 *  Check correctness of comparator functionality, output nothing if correct.
 *
 * Note -- This also checks correctness of all sorts with comparators.
 *         This could potentially live in a separate test.
 */

use Sort;
use Random;

proc main() {

  // Array to sort
  var Arr = [-1,-4, 2, 3],
   StrArr = ['anthony', 'david', 'Brad', 'ben'];

  // Correctness arrays
  const ArrAbsSorted = [-1, 2, 3, -4],
           ArrSorted = [-4, -1, 2, 3],
        ArrRevSorted = [ 3, 2, -1, -4],
        StrArrSorted = ['Brad', 'anthony', 'ben', 'david'];

  // Comparators
  const key = new keycomparator();
  const compare = new compcomparator();

  /* Test data types */

  // Strings
  bubbleSort(StrArr);
  checksort(StrArr, StrArrSorted, 'bubbleSort');


  /* Test Sorts */

  // bubble
  bubbleSort(Arr);
  checksort(Arr, ArrSorted, 'bubbleSort');

  bubbleSort(Arr, comparator=key);
  checksort(Arr, ArrAbsSorted, 'bubbleSort', 'key');

  bubbleSort(Arr, comparator=compare);
  checksort(Arr, ArrAbsSorted, 'bubbleSort', 'compare');

  // insertion
  insertionSort(Arr);
  checksort(Arr, ArrSorted, 'insertionSort');

  insertionSort(Arr, comparator=key);
  checksort(Arr, ArrAbsSorted, 'insertionSort', 'key');

  insertionSort(Arr, comparator=compare);
  checksort(Arr, ArrAbsSorted, 'insertionSort', 'compare');

  // Quick
  quickSort(Arr);
  checksort(Arr, ArrSorted, 'QuickSort');

  quickSort(Arr, comparator=key);
  checksort(Arr, ArrAbsSorted, 'QuickSort', 'key');

  quickSort(Arr, comparator=compare);
  checksort(Arr, ArrAbsSorted, 'QuickSort', 'compare');

  // heap
  heapSort(Arr);
  checksort(Arr, ArrSorted, 'heapSort');

  heapSort(Arr, comparator=key);
  checksort(Arr, ArrAbsSorted, 'heapSort', 'key');

  heapSort(Arr, comparator=compare);
  checksort(Arr, ArrAbsSorted, 'heapSort', 'compare');

  /* TODO -- SelectionSort testing when comparator support implemented */

  /* Test deprecated sorts */

}


/* Checks array and resets values -- any output results in failure */
proc checksort(ref array, correct, sort:string, comparator:string='none') {
  if !array.equals(correct) {
    writeln(sort, 'with comparator: ', comparator, ' failed');
    writeln('Incorrect array:');
    writeln(array);
    writeln('Expected correct array:');
    writeln(correct);
  }
  shuffle(array, seed=42);
}


/* Defines manipulation of value that should be used for comparison */
record keycomparator { }
proc keycomparator.key(a) { return abs(a); }


/* Defines compare behavior, return 1, 0, or -1 */
record compcomparator { }
proc compcomparator.compare(a, b) {
  return abs(a) - abs(b);
}
