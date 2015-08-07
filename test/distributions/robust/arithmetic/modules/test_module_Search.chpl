// Test the standard module Search

use driver_real_arrays;
use Random;
use Sort;
use Search;

var rng = new RandomStream(314159265);

var found: bool;
var foundIdx: int;
var idx = n1/2+1;
var elem: real;

proc reset(A) {
  rng.fillRandom(A);
  QuickSort(A);
  elem = A[idx];
}

proc checkSearch(found, foundIdx, A, searchType) {
  if found && (foundIdx==idx) then
    writeln("\t", searchType, ": SUCCESS");
  else
    writeln("\t", searchType, ": FAILED (", found, ", ", foundIdx, ")=",
            A[foundIdx]);
}

writeln("Search array");
reset(R1D);
(found, foundIdx) = LinearSearch(R1D, elem);
checkSearch(found, foundIdx, R1D, "LinearSearch");
reset(R1D);
(found, foundIdx) = BinarySearch(R1D, elem);
checkSearch(found, foundIdx, R1D, "BinarySearch");

writeln("Search aliased array");
var aR1D => R1D;
reset(R1D);
(found, foundIdx) = LinearSearch(aR1D, elem);
checkSearch(found, foundIdx, R1D, "LinearSearch");
reset(R1D);
(found, foundIdx) = BinarySearch(aR1D, elem);
checkSearch(found, foundIdx, R1D, "BinarySearch");

writeln("Search reindexed array");
proc foo(D: domain, A: [D], useLinear=true) {
  if useLinear then
    return LinearSearch(A, elem);
  else
    return BinarySearch(A, elem);
}
const TD1D: domain(1) = Space1.translate(-o5);
reset(R1D);
(found, foundIdx) = foo(TD1D, R1D.reindex(TD1D));
checkSearch(found, foundIdx+o5, R1D, "LinearSearch");
reset(R1D);
(found, foundIdx) = foo(TD1D, R1D.reindex(TD1D), false);
checkSearch(found, foundIdx+o5, R1D, "BinarySearch");


writeln("Search rank changed array (2D->1D)");
var rc2DR1D =>  R2D(Dom2D.dim(1), n2-1);
idx = Dom2D.dim(1).length/2+1;
reset(rc2DR1D);
(found, foundIdx) = LinearSearch(rc2DR1D, elem);
checkSearch(found, foundIdx, rc2DR1D, "LinearSearch");
reset(rc2DR1D);
(found, foundIdx) = BinarySearch(rc2DR1D, elem);
checkSearch(found, foundIdx, rc2DR1D, "BinarySearch");

writeln("Search rank changed array (3D->1D)");
var rc3DR1D =>  R3D(Dom3D.dim(1), n3-1, n3-1);
idx = Dom3D.dim(1).length/2+1;
reset(rc3DR1D);
(found, foundIdx) = LinearSearch(rc3DR1D, elem);
checkSearch(found, foundIdx, rc3DR1D, "LinearSearch");
reset(rc3DR1D);
(found, foundIdx) = BinarySearch(rc3DR1D, elem);
checkSearch(found, foundIdx, rc3DR1D, "BinarySearch");

writeln("Search rank changed array (4D->1D)");
var rc4DR1D =>  R4D(Dom4D.dim(1), n4-1, n4-1, n4-1);
idx = Dom4D.dim(1).length/2+1;
reset(rc4DR1D);
(found, foundIdx) = LinearSearch(rc4DR1D, elem);
checkSearch(found, foundIdx, rc4DR1D, "LinearSearch");
reset(rc4DR1D);
(found, foundIdx) = BinarySearch(rc4DR1D, elem);
checkSearch(found, foundIdx, rc4DR1D, "BinarySearch");

