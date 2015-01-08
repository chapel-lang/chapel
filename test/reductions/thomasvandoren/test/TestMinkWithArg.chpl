/*
 * Verify user defined mink reduction works with arguments.
 */

use TestMinkCommon;

var intResult = mink(k=100) reduce B,
  realResult = mink(k=100) reduce A;

writeln("mink(100) int result: ", intResult);
writeln("mink(100) real result: ", realResult);
