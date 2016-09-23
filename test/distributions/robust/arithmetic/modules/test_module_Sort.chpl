// Test the standard module Sort

use driver_real_arrays;
use Random;
use Sort;

var rng = new RandomStream(314159265);

enum SortType { BUBBLE=0, INSERTION, MERGE, SELECTION, QUICK, HEAP };

writeln("Sort array");
rng.fillRandom(R1D);
bubbleSort(R1D);
rng.fillRandom(R1D);
insertionSort(R1D);
rng.fillRandom(R1D);
mergeSort(R1D);
rng.fillRandom(R1D);
selectionSort(R1D);
rng.fillRandom(R1D);
quickSort(R1D);
rng.fillRandom(R1D);
heapSort(R1D);

writeln("Sort aliased array");
var aR1D => R1D;
rng.fillRandom(R1D);
bubbleSort(aR1D);
rng.fillRandom(R1D);
insertionSort(aR1D);
rng.fillRandom(R1D);
mergeSort(aR1D);
rng.fillRandom(R1D);
selectionSort(aR1D);
rng.fillRandom(R1D);
quickSort(aR1D);
rng.fillRandom(R1D);
heapSort(aR1D);

writeln("Sort reindexed array");
proc foo(D: domain, A: [D], st: SortType) {
  select st {
    when SortType.BUBBLE do bubbleSort(A);
    when SortType.INSERTION do insertionSort(A);
    when SortType.MERGE do mergeSort(A);
    when SortType.SELECTION do selectionSort(A);
    when SortType.QUICK do quickSort(A);
    when SortType.HEAP do heapSort(A);
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
bubbleSort(rc2DR1D);
rng.fillRandom(rc2DR1D);
insertionSort(rc2DR1D);
rng.fillRandom(rc2DR1D);
mergeSort(rc2DR1D);
rng.fillRandom(rc2DR1D);
selectionSort(rc2DR1D);
rng.fillRandom(rc2DR1D);
quickSort(rc2DR1D);
rng.fillRandom(rc2DR1D);
heapSort(rc2DR1D);

writeln("Sort rank changed array (3D->1D)");
var rc3DR1D => R3D(n3-1, n3-1, Dom3D.dim(3));
rng.fillRandom(rc3DR1D);
bubbleSort(rc3DR1D);
rng.fillRandom(rc3DR1D);
insertionSort(rc3DR1D);
rng.fillRandom(rc3DR1D);
mergeSort(rc3DR1D);
rng.fillRandom(rc3DR1D);
selectionSort(rc3DR1D);
rng.fillRandom(rc3DR1D);
quickSort(rc3DR1D);
rng.fillRandom(rc3DR1D);
heapSort(rc3DR1D);

writeln("Sort rank changed array (4D->1D)");
var rc4DR1D => R4D(n4-1, n4-1, n4-1, Dom4D.dim(4));
rng.fillRandom(rc4DR1D);
bubbleSort(rc4DR1D);
rng.fillRandom(rc4DR1D);
insertionSort(rc4DR1D);
rng.fillRandom(rc4DR1D);
mergeSort(rc4DR1D);
rng.fillRandom(rc4DR1D);
selectionSort(rc4DR1D);
rng.fillRandom(rc4DR1D);
quickSort(rc4DR1D);
rng.fillRandom(rc4DR1D);
heapSort(rc4DR1D);

delete rng;
