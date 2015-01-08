/*
 * Verify user defined mink reduction works with defaults.
 */

use TestMinkCommon;

var intResult = mink reduce B,
  realResult = mink reduce A;

writeln("mink int result: ", intResult);
writeln("mink real result: ", realResult);
