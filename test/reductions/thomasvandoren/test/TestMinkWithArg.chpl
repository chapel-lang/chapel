/*
 * Verify user defined mink reduction works with arguments.
 */

use mink;
use Common;

var intResult = mink(k=15) reduce intArray,
  realResult = mink(k=15) reduce realArray;

writeln("mink(15) int result: ", intResult);
writeln("mink(15) real result: ", realResult);
