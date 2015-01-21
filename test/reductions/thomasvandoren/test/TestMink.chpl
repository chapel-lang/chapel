/*
 * Verify user defined mink reduction works with defaults.
 */

use mink;
use Common;

// Verify the regular 1000 element arrays produce the correct mink values.
var intResult = mink reduce intArray,
  realResult = mink reduce realArray;
writeln("mink int result: ", intResult);
writeln("mink real result: ", realResult);

// Verify calling reduction on array of less than k elements returns those
// elements, in sorted order, with the other values filled in with max(real).
var smallA: [1..5] real;
randStream.fillRandom(smallA);
writeln("mink int small result: ", mink reduce smallA);

// Verify calling reduction on empty array returns array of max(eltType)
// values.
writeln("mink int empty result: ", mink reduce emptyArray);
