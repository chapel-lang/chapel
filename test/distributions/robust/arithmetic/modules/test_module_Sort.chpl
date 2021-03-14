// Test the standard module Sort

use driver_real_arrays;
use Random;
use Sort;

var rng = new owned RandomStream(real, 314159265);

enum SortType { BUBBLE=0, INSERTION, MERGE, SELECTION, QUICK, HEAP };


writeln("Sort array");

rng.fillRandom(R1D);
BubbleSort.bubbleSort(R1D);
if !isSorted(R1D) then writeln('bubbleSort failed');

rng.fillRandom(R1D);
InsertionSort.insertionSort(R1D);
if !isSorted(R1D) then writeln('insertionSort failed');

rng.fillRandom(R1D);
MergeSort.mergeSort(R1D);
if !isSorted(R1D) then writeln('mergeSort failed');

rng.fillRandom(R1D);
SelectionSort.selectionSort(R1D);
if !isSorted(R1D) then writeln('selectionSort failed');

rng.fillRandom(R1D);
QuickSort.quickSort(R1D);
if !isSorted(R1D) then writeln('quickSort failed');

rng.fillRandom(R1D);
HeapSort.heapSort(R1D);
if !isSorted(R1D) then writeln('heapSort failed');


writeln("Sort aliased array");
ref aR1D = R1D;

rng.fillRandom(R1D);
BubbleSort.bubbleSort(aR1D);
if !isSorted(aR1D) then writeln('bubbleSort failed');

rng.fillRandom(R1D);
InsertionSort.insertionSort(aR1D);
if !isSorted(aR1D) then writeln('insertionSort failed');

rng.fillRandom(R1D);
MergeSort.mergeSort(aR1D);
if !isSorted(aR1D) then writeln('mergeSort failed');

rng.fillRandom(R1D);
SelectionSort.selectionSort(aR1D);
if !isSorted(aR1D) then writeln('selectionSort failed');

rng.fillRandom(R1D);
QuickSort.quickSort(aR1D);
if !isSorted(aR1D) then writeln('quickSort failed');

rng.fillRandom(R1D);
HeapSort.heapSort(aR1D);
if !isSorted(aR1D) then writeln('heapSort failed');


writeln("Sort reindexed array");
proc doSort(D: domain, A: [D], st: SortType) {
  select st {
    when SortType.BUBBLE {
      BubbleSort.bubbleSort(A);
      if !isSorted(A) then writeln('bubbleSort failed');
    }
    when SortType.INSERTION {
      InsertionSort.insertionSort(A);
      if !isSorted(A) then writeln('insertionSort failed');
    }
    when SortType.MERGE {
      MergeSort.mergeSort(A);
      if !isSorted(A) then writeln('mergeSort failed');
    }
    when SortType.SELECTION {
      SelectionSort.selectionSort(A);
      if !isSorted(A) then writeln('selectionSort failed');
    }
    when SortType.QUICK {
      QuickSort.quickSort(A);
      if !isSorted(A) then writeln('quickSort failed');
    }
    when SortType.HEAP {
      HeapSort.heapSort(A);
      if !isSorted(A) then writeln('heapSort failed');
    }
  }
}

const TD1D: domain(1) = Space1.translate(-o5);
for param st in 0..5 /* this is life without FCFs...*/ {
  rng.fillRandom(R1D);
  doSort(TD1D, R1D.reindex(TD1D), st:SortType);
}


writeln("Sort rank changed array (2D->1D)");
ref rc2DR1D = R2D(n2-1, Dom2D.dim(1));

rng.fillRandom(rc2DR1D);
BubbleSort.bubbleSort(rc2DR1D);
if !isSorted(rc2DR1D) then writeln('bubbleSort failed');

rng.fillRandom(rc2DR1D);
InsertionSort.insertionSort(rc2DR1D);
if !isSorted(rc2DR1D) then writeln('insertionSort failed');

rng.fillRandom(rc2DR1D);
MergeSort.mergeSort(rc2DR1D);
if !isSorted(rc2DR1D) then writeln('mergeSort failed');

rng.fillRandom(rc2DR1D);
SelectionSort.selectionSort(rc2DR1D);
if !isSorted(rc2DR1D) then writeln('selectionSort failed');

rng.fillRandom(rc2DR1D);
QuickSort.quickSort(rc2DR1D);
if !isSorted(rc2DR1D) then writeln('quickSort failed');

rng.fillRandom(rc2DR1D);
HeapSort.heapSort(rc2DR1D);
if !isSorted(rc2DR1D) then writeln('heapSort failed');


writeln("Sort rank changed array (3D->1D)");
ref rc3DR1D = R3D(n3-1, n3-1, Dom3D.dim(2));

rng.fillRandom(rc3DR1D);
BubbleSort.bubbleSort(rc3DR1D);
if !isSorted(rc3DR1D) then writeln('bubbleSort failed');

rng.fillRandom(rc3DR1D);
InsertionSort.insertionSort(rc3DR1D);
if !isSorted(rc3DR1D) then writeln('insertionSort failed');

rng.fillRandom(rc3DR1D);
MergeSort.mergeSort(rc3DR1D);
if !isSorted(rc3DR1D) then writeln('mergeSort failed');

rng.fillRandom(rc3DR1D);
SelectionSort.selectionSort(rc3DR1D);
if !isSorted(rc3DR1D) then writeln('selectionSort failed');

rng.fillRandom(rc3DR1D);
QuickSort.quickSort(rc3DR1D);
if !isSorted(rc3DR1D) then writeln('quickSort failed');

rng.fillRandom(rc3DR1D);
HeapSort.heapSort(rc3DR1D);
if !isSorted(rc3DR1D) then writeln('heapSort failed');


writeln("Sort rank changed array (4D->1D)");
ref rc4DR1D = R4D(n4-1, n4-1, n4-1, Dom4D.dim(3));

rng.fillRandom(rc4DR1D);
BubbleSort.bubbleSort(rc4DR1D);
if !isSorted(rc4DR1D) then writeln('bubbleSort failed');

rng.fillRandom(rc4DR1D);
InsertionSort.insertionSort(rc4DR1D);
if !isSorted(rc4DR1D) then writeln('insertionSort failed');

rng.fillRandom(rc4DR1D);
MergeSort.mergeSort(rc4DR1D);
if !isSorted(rc4DR1D) then writeln('mergeSort failed');

rng.fillRandom(rc4DR1D);
SelectionSort.selectionSort(rc4DR1D);
if !isSorted(rc4DR1D) then writeln('selectionSort failed');

rng.fillRandom(rc4DR1D);
QuickSort.quickSort(rc4DR1D);
if !isSorted(rc4DR1D) then writeln('quickSort failed');

rng.fillRandom(rc4DR1D);
HeapSort.heapSort(rc4DR1D);
if !isSorted(rc4DR1D) then writeln('heapSort failed');
