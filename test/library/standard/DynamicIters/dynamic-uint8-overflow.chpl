// Regression test for https://github.com/chapel-lang/chapel/issues/14618
//
// variation of dynamic-int-overflow using uint(8) for easier
// arithmetic, and so that the loop body doesn't need to be empty.
//
// Checks that there's not a problem when the previous chunk's end
// point plus the chunkSize > max(uint(8)).
//
// r and chunkSize are chosen so that one chunk will end towards the
// end of r, but adding chunkSize will overlow max(uint(8))
//
// Excercise serial, standalone, leader, and follower iterators

use DynamicIters;

// end == 255 - 16
var r = 0:uint(8) .. 239:uint(8);

// desired chunks 0-37, 38-75, 76-113, 114-151, 152-189, 190-227, 228-239
// noting 228 + 38 == 266 > max(uint(8))

var A: [r] atomic int;

for i in dynamic(r, chunkSize=38) { A[i].add(1); }
writeln(A);

forall i in dynamic(r, chunkSize=38) { A[i].add(1); }
writeln(A);

forall (i,j) in zip(dynamic(r, chunkSize=38), r) { A[i].add(1); }
writeln(A);

forall (i,j) in zip(r, dynamic(r, chunkSize=38)) { A[j].add(1); }
writeln(A);
