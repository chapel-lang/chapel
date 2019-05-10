/*
 * Verify user defined isSorted reduction works.
 */

use sorted;
use Common;

var rA: [1..10] real;
randStream.fillRandom(rA);
var A = [i in rA] (10 * i): int;

writeln("A: ", A);
writeln("sorted? ", isSorted reduce for a in A do a);

sort(A);
writeln("A: ", A);
writeln("sorted? ", isSorted reduce for a in A do a);
