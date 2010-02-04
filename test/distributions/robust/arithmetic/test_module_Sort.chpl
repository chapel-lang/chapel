// Test the standard module Sort

use driver;
use Random;
use Sort;

var rng = new RandomStream(314159265);

def checkSort(A, len, sortType) {
  var errs = 0;
  for i in 1..len-1 do
    if A[i] > A[i+1] {
      errs += 1;
      writeln("\t", sortType, " error: A[", i, "]=", A[i],
              ", A[", i+1, "]=", A[i+1]);
    }
  writeln("\t", sortType, ": ", errs, " errors");
}

writeln("Sort array");
rng.fillRandom(R1D);
QuickSort(R1D);
checkSort(R1D, n1, "QuickSort");
rng.fillRandom(R1D);
InsertionSort(R1D);
checkSort(R1D, n1, "InsertionSort");


writeln("Sort aliased array");
var aR1D => R1D;
rng.fillRandom(R1D);
QuickSort(aR1D);
checkSort(R1D, n1, "QuickSort");
rng.fillRandom(R1D);
InsertionSort(aR1D);
checkSort(R1D, n1, "InsertionSort");


writeln("Sort reindexed array");
def foo(D: domain, A: [D], useQuickSort=true) {
  if useQuickSort then
    QuickSort(A);
  else
    InsertionSort(A);
}
const TD1D: domain(1) = Space1 - (o5);
rng.fillRandom(R1D);
foo(TD1D, R1D);
checkSort(R1D, n1, "QuickSort");
rng.fillRandom(R1D);
foo(TD1D, R1D, false);
checkSort(R1D, n1, "InsertionSort");


writeln("Sort rank changed array (2D->1D)");
var rc2DR1D => R2D(n2-1, Dom2D.dim(2));
rng.fillRandom(rc2DR1D);
QuickSort(rc2DR1D);
checkSort(rc2DR1D, Dom2D.dim(2).length, "QuickSort");
rng.fillRandom(rc2DR1D);
InsertionSort(rc2DR1D);
checkSort(rc2DR1D, Dom2D.dim(2).length, "InsertionSort");

writeln("Sort rank changed array (3D->1D)");
var rc3DR1D => R3D(n3-1, n3-1, Dom3D.dim(3));
rng.fillRandom(rc3DR1D);
QuickSort(rc3DR1D);
checkSort(rc3DR1D, Dom3D.dim(3).length, "QuickSort");
rng.fillRandom(rc3DR1D);
InsertionSort(rc3DR1D);
checkSort(rc3DR1D, Dom3D.dim(3).length, "InsertionSort");

writeln("Sort rank changed array (4D->1D)");
var rc4DR1D => R4D(n4-1, n4-1, n4-1, Dom4D.dim(4));
rng.fillRandom(rc4DR1D);
QuickSort(rc4DR1D);
checkSort(rc4DR1D, Dom4D.dim(4).length, "QuickSort");
rng.fillRandom(rc4DR1D);
InsertionSort(rc4DR1D);
checkSort(rc4DR1D, Dom4D.dim(4).length, "InsertionSort");

