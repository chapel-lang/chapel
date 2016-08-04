/*
 *  Check correctness of sort routines. Output nothing if correct.
 *
 *  TODO -- remove deprecated testing in 1.15
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
     ArrAbsRevSorted = [ -4, 3, 2, -1],
        StrArrSorted = ['Brad', 'anthony', 'ben', 'david'];

  // Comparators
  const key = new keycomparator(),
        compare = new compcomparator(),
        keycompare = new keycompcomparator(),
        revkey = new ReverseComparator(key),
        revcompare = new ReverseComparator(compare),
        tuplekey = new tuplecomparator();


  /* Test data types */

  // Integers
  sort(Arr);
  checkSort(Arr, ArrSorted, 'sort');

  // Strings
  sort(StrArr);
  checkSort(StrArr, StrArrSorted, 'sort');

  // Integers sorted as tuples
  sort(Arr, comparator=tuplekey);
  checkSort(Arr, ArrSorted, 'sort');

  /* Key + Compare */

  // key method sorts by abs(a), while compare is normal sort
  sort(Arr, comparator=keycompare);
  checkSort(Arr, ArrAbsSorted, 'sort');

  /* Reverse */

  // Default reversecomparator
  sort(Arr, comparator=reverseComparator);
  checkSort(Arr, ArrRevSorted, 'sort');

  // Reverse key comparator (absolute value)
  sort(Arr, comparator=revkey);
  checkSort(Arr, ArrAbsRevSorted, 'sort');

  // Reverse compare comparator (absolute value)
  sort(Arr, comparator=revcompare);
  checkSort(Arr, ArrAbsRevSorted, 'sort');

  /* Test Sorts */

  // bubbleSort
  bubbleSort(Arr);
  checkSort(Arr, ArrSorted, 'bubbleSort');

  bubbleSort(Arr, comparator=key);
  checkSort(Arr, ArrAbsSorted, 'bubbleSort', 'key');

  bubbleSort(Arr, comparator=compare);
  checkSort(Arr, ArrAbsSorted, 'bubbleSort', 'compare');

  // insertionSort
  insertionSort(Arr);
  checkSort(Arr, ArrSorted, 'insertionSort');

  insertionSort(Arr, comparator=key);
  checkSort(Arr, ArrAbsSorted, 'insertionSort', 'key');

  insertionSort(Arr, comparator=compare);
  checkSort(Arr, ArrAbsSorted, 'insertionSort', 'compare');

  // quickSort
  quickSort(Arr);
  checkSort(Arr, ArrSorted, 'quickSort');

  quickSort(Arr, comparator=key);
  checkSort(Arr, ArrAbsSorted, 'quickSort', 'key');

  quickSort(Arr, comparator=compare);
  checkSort(Arr, ArrAbsSorted, 'quickSort', 'compare');

  // heapSort
  heapSort(Arr);
  checkSort(Arr, ArrSorted, 'heapSort');

  heapSort(Arr, comparator=key);
  checkSort(Arr, ArrAbsSorted, 'heapSort', 'key');

  heapSort(Arr, comparator=compare);
  checkSort(Arr, ArrAbsSorted, 'heapSort', 'compare');

  // selectionSort
  selectionSort(Arr);
  checkSort(Arr, ArrSorted, 'selectionSort');

  selectionSort(Arr, comparator=key);
  checkSort(Arr, ArrAbsSorted, 'selectionSort', 'key');

  selectionSort(Arr, comparator=compare);
  checkSort(Arr, ArrAbsSorted, 'selectionSort', 'compare');

  /* TODO -- selectionSort testing when comparator support implemented */

  /* Test deprecated sorts */
  // TODO - remove these some day

  // BubbleSort
  BubbleSort(Arr);
  checkSort(Arr, ArrSorted, 'BubbleSort');

  BubbleSort(Arr, doublecheck=true);
  checkSort(Arr, ArrSorted, 'BubbleSort');

  BubbleSort(Arr, reverse=true);
  checkSort(Arr, ArrRevSorted, 'BubbleSort');

  // InsertionSort
  InsertionSort(Arr);
  checkSort(Arr, ArrSorted, 'InsertionSort');

  InsertionSort(Arr, doublecheck=true);
  checkSort(Arr, ArrSorted, 'InsertionSort');

  InsertionSort(Arr, reverse=true);
  checkSort(Arr, ArrRevSorted, 'InsertionSort');

  // quickSort
  QuickSort(Arr);
  checkSort(Arr, ArrSorted, 'quickSort');

  QuickSort(Arr, doublecheck=true);
  checkSort(Arr, ArrSorted, 'quickSort');

  QuickSort(Arr, reverse=true);
  checkSort(Arr, ArrRevSorted, 'quickSort');

  // HeapSort
  HeapSort(Arr);
  checkSort(Arr, ArrSorted, 'HeapSort');

  HeapSort(Arr, doublecheck=true);
  checkSort(Arr, ArrSorted, 'HeapSort');

  HeapSort(Arr, reverse=true);
  checkSort(Arr, ArrRevSorted, 'HeapSort');

  // SelectionSort (not actually deprecated yet)
  SelectionSort(Arr);
  checkSort(Arr, ArrSorted, 'SelectionSort');

  SelectionSort(Arr, doublecheck=true);
  checkSort(Arr, ArrSorted, 'SelectionSort');

  SelectionSort(Arr, reverse=true);
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


/* Defines manipulation of value that should be used for comparison */
record keycomparator { }
proc keycomparator.key(a) { return abs(a); }


/* Defines compare behavior, return 1, 0, or -1 */
record compcomparator { }
proc compcomparator.compare(a, b) {
  return abs(a) - abs(b);
}

/* key method should take priority over compare method */
record keycompcomparator { }
proc keycompcomparator.key(a) { return abs(a); }
proc keycompcomparator.compare(a, b) {
  return a - b;
}

/* Key method can return a non-numerical/string type, such as tuple */
record tuplecomparator { }
proc tuplecomparator.key(a) { return (a, a); }
