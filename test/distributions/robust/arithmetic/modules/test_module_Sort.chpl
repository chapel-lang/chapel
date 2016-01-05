// Test the standard module Sort

use driver_real_arrays;
use Random;
use Sort;

var rng = new RandomStream(314159265);

enum SortType { BUBBLE=0, INSERTION, MERGE, SELECTION, QUICK, HEAP };

writeln("Sort array");
rng.fillRandom(R1D);
BubbleSort(R1D, doublecheck=true);
rng.fillRandom(R1D);
InsertionSort(R1D, doublecheck=true);
rng.fillRandom(R1D);
MergeSort(R1D, doublecheck=true);
rng.fillRandom(R1D);
SelectionSort(R1D, doublecheck=true);
rng.fillRandom(R1D);
QuickSort(R1D, doublecheck=true);
rng.fillRandom(R1D);
HeapSort(R1D, doublecheck=true);

writeln("Sort aliased array");
var aR1D => R1D;
rng.fillRandom(R1D);
BubbleSort(aR1D, doublecheck=true);
rng.fillRandom(R1D);
InsertionSort(aR1D, doublecheck=true);
rng.fillRandom(R1D);
MergeSort(aR1D, doublecheck=true);
rng.fillRandom(R1D);
SelectionSort(aR1D, doublecheck=true);
rng.fillRandom(R1D);
QuickSort(aR1D, doublecheck=true);
rng.fillRandom(R1D);
HeapSort(aR1D, doublecheck=true);

writeln("Sort reindexed array");
proc foo(D: domain, A: [D], st: SortType) {
  select st {
    when SortType.BUBBLE do BubbleSort(A, doublecheck=true);
    when SortType.INSERTION do InsertionSort(A, doublecheck=true);
    when SortType.MERGE do MergeSort(A, doublecheck=true);
    when SortType.SELECTION do SelectionSort(A, doublecheck=true);
    when SortType.QUICK do QuickSort(A, doublecheck=true);
    when SortType.HEAP do HeapSort(A, doublecheck=true);
    }
}
const TD1D: domain(1) = Space1.translate(-o5);
for param st in 0..5 /* this is annoying */ {
  rng.fillRandom(R1D);
  foo(TD1D, R1D.reindex(TD1D), st:SortType);
}

writeln("Sort rank changed array (2D->1D)");
var rc2DR1D => R2D(n2-1, Dom2D.dim(2));
rng.fillRandom(rc2DR1D);
BubbleSort(rc2DR1D, doublecheck=true);
rng.fillRandom(rc2DR1D);
InsertionSort(rc2DR1D, doublecheck=true);
rng.fillRandom(rc2DR1D);
MergeSort(rc2DR1D, doublecheck=true);
rng.fillRandom(rc2DR1D);
SelectionSort(rc2DR1D, doublecheck=true);
rng.fillRandom(rc2DR1D);
QuickSort(rc2DR1D, doublecheck=true);
rng.fillRandom(rc2DR1D);
HeapSort(rc2DR1D, doublecheck=true);

writeln("Sort rank changed array (3D->1D)");
var rc3DR1D => R3D(n3-1, n3-1, Dom3D.dim(3));
rng.fillRandom(rc3DR1D);
BubbleSort(rc3DR1D, doublecheck=true);
rng.fillRandom(rc3DR1D);
InsertionSort(rc3DR1D, doublecheck=true);
rng.fillRandom(rc3DR1D);
MergeSort(rc3DR1D, doublecheck=true);
rng.fillRandom(rc3DR1D);
SelectionSort(rc3DR1D, doublecheck=true);
rng.fillRandom(rc3DR1D);
QuickSort(rc3DR1D, doublecheck=true);
rng.fillRandom(rc3DR1D);
HeapSort(rc3DR1D, doublecheck=true);

writeln("Sort rank changed array (4D->1D)");
var rc4DR1D => R4D(n4-1, n4-1, n4-1, Dom4D.dim(4));
rng.fillRandom(rc4DR1D);
BubbleSort(rc4DR1D, doublecheck=true);
rng.fillRandom(rc4DR1D);
InsertionSort(rc4DR1D, doublecheck=true);
rng.fillRandom(rc4DR1D);
MergeSort(rc4DR1D, doublecheck=true);
rng.fillRandom(rc4DR1D);
SelectionSort(rc4DR1D, doublecheck=true);
rng.fillRandom(rc4DR1D);
QuickSort(rc4DR1D, doublecheck=true);
rng.fillRandom(rc4DR1D);
HeapSort(rc4DR1D, doublecheck=true);
