/*
 * Verify user defined isSorted reduction works.
 */

use sorted;
use Common;

use Sort;

var rA: [1..10] real;
randStream.fillRandom(rA);
var A = [i in rA] (10 * i): int;

writeln("A: ", A);
writeln("sorted? ", isSorted reduce A);

QuickSort(A);
writeln("A: ", A);
writeln("sorted? ", isSorted reduce A);
