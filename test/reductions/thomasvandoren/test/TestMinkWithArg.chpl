/*
 * Verify user defined mink reduction works with arguments.
 */

use mink;
use Common;

var intResult = mink(eltType=int, k=15) reduce intArray,
   realResult = mink(eltType=real, k=15) reduce realArray;

writeln("mink(15) int result: ", intResult);
writeln("mink(15) real result: ", realResult);
