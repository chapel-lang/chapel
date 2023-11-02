/*
 * Common mink() test utilities.
 */

public use Random;
private use Time;

config const randSeed = (timeSinceEpoch().totalSeconds()*2_000_000+1):int;

var emptyArray: [1..0] int(8),
  realArray: [1..1000] real,
  randStream = new randomStream(real, randSeed, false);

randStream.fill(realArray);
var intArray = [i in 1..1000] (realArray[i] * 1000): int;
