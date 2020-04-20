// Test the standard module Search

use driver_real_arrays;
use Random;
use Sort;
use Search;

var rng = new owned RandomStream(real, 314159265);

var found: bool;
var foundIdx: int;
var idx = n1/2+1;
var elem: real;

proc reset(A) {
  rng.fillRandom(A);
  sort(A);
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
(found, foundIdx) = linearSearch(R1D, elem);
checkSearch(found, foundIdx, R1D, "linearSearch");
reset(R1D);
(found, foundIdx) = binarySearch(R1D, elem);
checkSearch(found, foundIdx, R1D, "binarySearch");

writeln("Search aliased array");
ref aR1D = R1D;
reset(R1D);
(found, foundIdx) = linearSearch(aR1D, elem);
checkSearch(found, foundIdx, R1D, "linearSearch");
reset(R1D);
(found, foundIdx) = binarySearch(aR1D, elem);
checkSearch(found, foundIdx, R1D, "binarySearch");

writeln("Search reindexed array");
proc foo(D: domain, A: [D], useLinear=true) {
  if useLinear then
    return linearSearch(A, elem);
  else
    return binarySearch(A, elem);
}
const TD1D: domain(1) = Space1.translate(-o5);
reset(R1D);
(found, foundIdx) = foo(TD1D, R1D.reindex(TD1D));
checkSearch(found, foundIdx+o5, R1D, "linearSearch");
reset(R1D);
(found, foundIdx) = foo(TD1D, R1D.reindex(TD1D), false);
checkSearch(found, foundIdx+o5, R1D, "binarySearch");


writeln("Search rank changed array (2D->1D)");
ref rc2DR1D =  R2D(Dom2D.dim(0), n2-1);
idx = Dom2D.dim(0).size/2+1;
reset(rc2DR1D);
(found, foundIdx) = linearSearch(rc2DR1D, elem);
checkSearch(found, foundIdx, rc2DR1D, "linearSearch");
reset(rc2DR1D);
(found, foundIdx) = binarySearch(rc2DR1D, elem);
checkSearch(found, foundIdx, rc2DR1D, "binarySearch");

writeln("Search rank changed array (3D->1D)");
ref rc3DR1D =  R3D(Dom3D.dim(0), n3-1, n3-1);
idx = Dom3D.dim(0).size/2+1;
reset(rc3DR1D);
(found, foundIdx) = linearSearch(rc3DR1D, elem);
checkSearch(found, foundIdx, rc3DR1D, "linearSearch");
reset(rc3DR1D);
(found, foundIdx) = binarySearch(rc3DR1D, elem);
checkSearch(found, foundIdx, rc3DR1D, "binarySearch");

writeln("Search rank changed array (4D->1D)");
ref rc4DR1D =  R4D(Dom4D.dim(0), n4-1, n4-1, n4-1);
idx = Dom4D.dim(0).size/2+1;
reset(rc4DR1D);
(found, foundIdx) = linearSearch(rc4DR1D, elem);
checkSearch(found, foundIdx, rc4DR1D, "linearSearch");
reset(rc4DR1D);
(found, foundIdx) = binarySearch(rc4DR1D, elem);
checkSearch(found, foundIdx, rc4DR1D, "binarySearch");
