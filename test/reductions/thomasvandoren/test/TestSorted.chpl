/*
 * Verify user defined isSorted reduction works.
 */

use sorted;
use Common;

var rA: [1..10] real;
randStream.fillRandom(rA);
var A = [i in rA] (10 * i): int;

writeln("A: ", A);
writeln("sorted? ", isSorted reduce A);

quickSort(A);
writeln("A: ", A);
writeln("sorted? ", isSorted reduce A);

delete randStream;
