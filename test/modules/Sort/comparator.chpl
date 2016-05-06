/*
 *  Check correctness of comparator functionality, output nothing if correct.
 *
 * Note -- This also checks correctness of all sorts with comparators.
 *         This could potentially live in a separate test.
 */

use Sort;

proc main() {

  // Array to sort
  var Arr = [-1,-4, 2, 3];

  // Correctness arrays
  const ArrAbsSorted = [-1, 2, 3, -4],
           ArrSorted = [-4, -1, 2, 3];

  // Comparators
  const key = new keycomparator();
  const compare = new compcomparator();

  // Bubble
  BubbleSort(Arr);
  checksort(Arr, ArrSorted, 'BubbleSort');

  BubbleSort(Arr, comparator=key);
  checksort(Arr, ArrAbsSorted, 'BubbleSort', 'key');

  BubbleSort(Arr, comparator=compare);
  checksort(Arr, ArrAbsSorted, 'BubbleSort', 'compare');

  // Insertion
  InsertionSort(Arr);
  checksort(Arr, ArrSorted, 'InsertionSort');

  InsertionSort(Arr, comparator=key);
  checksort(Arr, ArrAbsSorted, 'InsertionSort', 'key');

  InsertionSort(Arr, comparator=compare);
  checksort(Arr, ArrAbsSorted, 'InsertionSort', 'compare');

  // Quick
  QuickSort(Arr);
  checksort(Arr, ArrSorted, 'QuickSort');

  QuickSort(Arr, comparator=key);
  checksort(Arr, ArrAbsSorted, 'QuickSort', 'key');

  QuickSort(Arr, comparator=compare);
  checksort(Arr, ArrAbsSorted, 'QuickSort', 'compare');

  // Heap
  HeapSort(Arr);
  checksort(Arr, ArrSorted, 'HeapSort');

  HeapSort(Arr, comparator=key);
  checksort(Arr, ArrAbsSorted, 'HeapSort', 'key');

  HeapSort(Arr, comparator=compare);
  checksort(Arr, ArrAbsSorted, 'HeapSort', 'compare');

  /* TODO -- SelectionSort testing when comparator support implemented */

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
  array = [-1,-4, 2, 3];
}


/* Defines manipulation of value that should be used for comparison */
record keycomparator { }
proc keycomparator.key(a) { return abs(a); }


/* Defines compare behavior, return 1, 0, or -1 */
record compcomparator { }
proc compcomparator.compare(a, b) {
  if abs(a) > abs(b) {
    return 1;
  } else if abs(a) < abs(b) {
    return -1;
  } else {
    return 0;
  }
}
