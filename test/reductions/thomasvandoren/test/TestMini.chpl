/*
 * Verify user defined mini reduction works with defaults.
 */

use mini;
use Common;

// Verify the regular 1000 element arrays produce the correct mini values.
// NOTE: For the future to pass, the zip() expression might need to become a
//       list comprehension (e.g. [(v, i) in zip(intArray, 1..)] (v, i)).
//       (thomasvandoren, 2015-01-07)
var intResult = mini reduce zip(intArray, 1..),
  realResult = mini reduce zip(realArray, 1..);
writeln("mini int result: ", intResult);
// use round() to avoid floating-point-precision issues
writeln("mini real result: ", (round(realResult(0)*1e8), realResult(1)));

// Verify calling mini on emptyArray returns (max(eltType), 0).
writeln("mini int empty result: ",
        mini reduce zip(emptyArray, 1..));
