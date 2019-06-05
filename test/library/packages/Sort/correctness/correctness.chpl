/*
 *  Check correctness of sort routines. Output nothing if correct.
 */

use Sort;
use Random;

proc main() {

  // Comparators
  const absKey = new AbsKeyCmp(),
        absComp = new AbsCompCmp(),
        revAbsKey = new ReverseComparator(absKey),
        revAbsComp = new ReverseComparator(absComp),
        tupleKey = new TupleCmp();
  const absKeyClass = new AbsKeyCmpClass(),
        absCompClass = new AbsCompCmpClass(),
        revAbsKeyClass = new ReverseComparator(absKeyClass.borrow()),
        revAbsCompClass = new ReverseComparator(absCompClass.borrow()),
        tupleKeyClass = new TupleCmpClass();


  // Arrays and Domains
  const largeD = {1..20}, // quickSort requires domain.size > 16   XXX But has different implementations for strided/nonstrided, so we're only testing the former.
        strideD = {2..8 by 2},
        strideAlignD = {2..9 by 2 align 3},
      strideRevD = {2..8 by -2};
  var largeA: [largeD] int,
      strideA: [strideD] int = [-3, -1, 4, 5],
      strideAlignA: [strideAlignD] int = [-3, -1, 4, 5],
      strideRevA: [strideRevD] int = [-3, -1, 4, 5];
    [i in largeD] largeA[i] = i;

  // Pre-sorted arrays paired with comparators to test
  var tests = (
                // Testing A.eltType
                ([-4, -1, 2, 3], defaultComparator),
                (['Brad', 'anthony', 'ben', 'david'], defaultComparator),

                // Testing D.idxType / D.dims()
                (largeA, defaultComparator),
                (strideA, defaultComparator),
                (strideAlignA, defaultComparator),
                (strideRevA, defaultComparator),

                // Testing comparators
                ([-1, 2, 3, -4], absKey),
                ([-1, 2, 3, -4], absKeyClass.borrow()),
                ([-1, 2, 3, -4], absComp),
                ([-1, 2, 3, -4], absCompClass.borrow()),
                ([ 3, 2, -1, -4], reverseComparator),
                ([ -4, 3, 2, -1], revAbsKey),
                ([ -4, 3, 2, -1], revAbsKeyClass),
                ([ -4, 3, 2, -1], revAbsComp),
                ([ -4, 3, 2, -1], revAbsCompClass),
                ([-4, -1, 2, 3], tupleKey),
                ([-4, -1, 2, 3], tupleKeyClass.borrow())
              );


  /* Correctness test isSorted(), so that it can test the remaining sorts*/
  {
    var isSortedWorks= true;

    for param i in 1..tests.size {
      var (arr, cmp) = tests(i);

      writeln("pre-checking test ", i);
      if !isSorted(arr, comparator=cmp) {
        writeln('isSorted failed to sort array:');
        writeln('failed to sort:');
        writeln('eltType:    ', arr.eltType:string);
        writeln('idxType:    ', arr.domain.idxType:string);
        writeln('dimensions: ', arr.domain.dims());
        writeln('comparator: ', cmp.name());
        isSortedWorks = false;
      }
    }

    if !isSortedWorks then
      halt('isSorted() failed - cannot test remaining sort functions');
  }

  /* Correctness tests for sort routines */
  // TODO -- functionalize these tests when FCF support allows it
  {
    for param i in 1..tests.size {
      var (arr, cmp) = tests(i);
      writeln("sort test ", i);
      resetArray(arr, cmp);
      sort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for sort() function.\n');
    }

    for param i in 1..tests.size {
      ref (arr, cmp) = tests(i);
      writeln("bubble sort test ", i);
      resetArray(arr, cmp);
      BubbleSort.bubbleSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for bubbleSort() function.\n');
    }

    for param i in 1..tests.size {
      var (arr, cmp) = tests(i);
      writeln("insertion sort test ", i);
      resetArray(arr, cmp);
      InsertionSort.insertionSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for insertionSort() function.\n');
    }

    for param i in 1..tests.size {
      var (arr, cmp) = tests(i);
      writeln("binary insertion sort test ", i);
      resetArray(arr, cmp);
      BinaryInsertionSort.binaryInsertionSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for binaryInsertionSort() function.\n');
    }

    for param i in 1..tests.size {
      var (arr, cmp) = tests(i);
      writeln("quicksort test ", i);
      resetArray(arr, cmp);
      QuickSort.quickSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for quickSort() function.\n');
    }

    for param i in 1..tests.size {
      var (arr, cmp) = tests(i);
      writeln("heapsort test ", i);
      resetArray(arr, cmp);
      HeapSort.heapSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for heapSort() function.\n');
    }

    for param i in 1..tests.size {
      var (arr, cmp) = tests(i);
      writeln("selection sort test ", i);
      resetArray(arr, cmp);
      SelectionSort.selectionSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for selectionSort() function.\n');
    }

    for param i in 1..tests.size {
      var (arr, cmp) = tests(i);
      writeln("merge sort test ", i);
      resetArray(arr, cmp);
      MergeSort.mergeSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for mergeSort() function.\n');
    }
  }
}


/* Checks if array is sorted with provided comparator */
proc checkSort(arr, cmp) {
  var result = true;

  // Check result
  if !isSorted(arr, cmp) {
      writeln('failed to sort:');
      writeln('eltType:    ', arr.eltType:string);
      writeln('idxType:    ', arr.domain.idxType:string);
      writeln('dimensions: ', arr.domain.dims());
      writeln('comparator: ', cmp.name());
      result = false;
  }
  return result;
}


/* Checks if array is sorted */
proc checkSort(arr) {
  var result = true;

  // Check result
  if !isSorted(arr) {
      writeln('failed to sort:');
      writeln('eltType:    ', arr.eltType:string);
      writeln('idxType:    ', arr.domain.idxType:string);
      writeln('dimensions: ', arr.domain.dims());
      result = false;
  }
  return result;
}


/* Reset array by shuffling until it's not sorted */
proc resetArray(ref array) {
  while isSorted(array) {
    Random.shuffle(array);
  }
}


/* Reset array by shuffling until it's not sorted with provided comparator */
proc resetArray(ref array, cmp) {
  while isSorted(array, cmp) {
    Random.shuffle(array);
  }
}


/* Enables more useful error messages */
proc DefaultComparator.name() { return 'DefaultComparator';}
proc ReverseComparator.name() { return 'ReverseComparator';}


/* Key Sort by absolute value */
record AbsKeyCmp {
  proc key(a) { return abs(a); }
  proc name() { return 'AbsKeyCmp'; }
}
class AbsKeyCmpClass {
  proc key(a) { return abs(a); }
  proc name() { return 'AbsKeyCmpClass'; }
}



/* Compare Sort by absolute value */
record AbsCompCmp {
  proc compare(a, b) { return abs(a) - abs(b); }
  proc name() { return 'AbsCompCmp'; }
}
class AbsCompCmpClass {
  proc compare(a, b) { return abs(a) - abs(b); }
  proc name() { return 'AbsCompCmpClass'; }
}



/* Key method can return a non-numerical/string type, such as tuple */
record TupleCmp {
  proc key(a) { return (a, a); }
  proc name() { return 'TupleCmp'; }
}
class TupleCmpClass {
  proc key(a) { return (a, a); }
  proc name() { return 'TupleCmpClass'; }
}

