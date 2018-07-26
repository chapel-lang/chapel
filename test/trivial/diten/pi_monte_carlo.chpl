// Calcuate pi using a monte carlo simulation

use Random, Time;

config const n = 10000, seed = 314159;
config const verbose: bool = false;

var count:int;
var pi, startTime, totalTime: real;
var rs = new owned NPBRandomStream(real, seed);

startTime = getCurrentTime(TimeUnits.microseconds);

// Find random points on the complex plane in (0..1+i)
// and count how many are outside the unit circle.
count = + reduce for 1..n do abs(rs.getNext()**2 + rs.getNext()**2):int;

// The probability a point is inside the unit circle is pi / 4.
pi = 4 * (n-count):real(64) / n;

// Write out the results
writeln(pi);
totalTime = (getCurrentTime(TimeUnits.microseconds) - startTime) / 1000000;
if (verbose) then
  writeln("Calculation took: ", totalTime, " seconds");
