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

  // First we test isSorted() against the arrays as written here, so
  // they have to be sorted correctly to start with.  Then we'll
  // shuffle them and sort them again.

  // quickSort requires domain.size > 16
  const largeD = {1..20},
        strideD = {2..40 by 2},
        strideAlignD = {2..41 by 2 align 3},
        strideRevD = {2..40 by -2};
  var largeA: [largeD] int,
      strideA: [strideD] int,
      strideAlignA: [strideAlignD] int,
      strideRevA: [strideRevD] int;

  largeA = [-17, -10, -4, -2, 0, 1, 2, 3, 5, 8,
	    13, 21, 34, 55, 89, 4242, 424242, 42424242, 4242424242, 424242424242 ];
  strideA = largeA;
  strideAlignA = largeA;
  strideRevA = largeA;

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
                ([-1, 2, 3, -4, 5, 6, -7, 8, 9, -10, 11, 12, -13, 14, 15, -16, 17, 18, -19, 20], absKey),
                ([-1, 2, 3, -4, 5, 6, -7, 8, 9, -10, 11, 12, -13, 14, 15, -16, 17, 18, -19, 20], absKeyClass.borrow()),
                ([-1, 2, 3, -4, 5, 6, -7, 8, 9, -10, 11, 12, -13, 14, 15, -16, 17, 18, -19, 20], absComp),
                ([-1, 2, 3, -4, 5, 6, -7, 8, 9, -10, 11, 12, -13, 14, 15, -16, 17, 18, -19, 20], absCompClass.borrow()),
                ([20, 18, 17, 15, 14, 12, 11, 9, 8, 6, 5, 3, 2, -1, -4, -7, -10, -13, -16, -19], reverseComparator),
                ([20, -19, 18, 17, -16, 15, 14, -13, 12, 11, -10, 9, 8, -7, 6, 5, -4, 3, 2, -1], revAbsKey),
                ([20, -19, 18, 17, -16, 15, 14, -13, 12, 11, -10, 9, 8, -7, 6, 5, -4, 3, 2, -1], revAbsKeyClass),
                ([20, -19, 18, 17, -16, 15, 14, -13, 12, 11, -10, 9, 8, -7, 6, 5, -4, 3, 2, -1], revAbsComp),
                ([20, -19, 18, 17, -16, 15, 14, -13, 12, 11, -10, 9, 8, -7, 6, 5, -4, 3, 2, -1], revAbsCompClass),
                ([-19, -16, -13, -10, -7, -4, -1, 2, 3, 5, 6, 8, 9, 11, 12, 14, 15, 17, 18, 20], tupleKey),
                ([-19, -16, -13, -10, -7, -4, -1, 2, 3, 5, 6, 8, 9, 11, 12, 14, 15, 17, 18, 20], tupleKeyClass.borrow())
              );


  /* Correctness test isSorted(), so that it can test the remaining sorts*/
  {
    var isSortedWorks= true;

    for param i in 0..tests.size-1 {
      var (arr, cmp) = tests(i);

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
    for param i in 1..tests.size-1 {
      var (arr, cmp) = tests(i);
      resetArray(arr, cmp);
      sort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for sort() function.\n');
    }

    for param i in 1..tests.size-1 {
      ref (arr, cmp) = tests(i);
      resetArray(arr, cmp);
      BubbleSort.bubbleSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for bubbleSort() function.\n');
    }

    for param i in 1..tests.size-1 {
      var (arr, cmp) = tests(i);
      resetArray(arr, cmp);
      InsertionSort.insertionSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for insertionSort() function.\n');
    }

    for param i in 1..tests.size-1 {
      var (arr, cmp) = tests(i);
      resetArray(arr, cmp);
      BinaryInsertionSort.binaryInsertionSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for binaryInsertionSort() function.\n');
    }

    for param i in 1..tests.size-1 {
      var (arr, cmp) = tests(i);
      resetArray(arr, cmp);
      QuickSort.quickSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for quickSort() function.\n');
    }

    for param i in 1..tests.size-1 {
      var (arr, cmp) = tests(i);
      resetArray(arr, cmp);
      HeapSort.heapSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for heapSort() function.\n');
    }

    for param i in 1..tests.size-1 {
      var (arr, cmp) = tests(i);
      resetArray(arr, cmp);
      SelectionSort.selectionSort(arr, comparator=cmp);
      if !checkSort(arr, cmp) then
        writeln('  for selectionSort() function.\n');
    }

    for param i in 1..tests.size-1 {
      var (arr, cmp) = tests(i);
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

