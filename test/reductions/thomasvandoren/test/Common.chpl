/*
 * Common mink() test utilities.
 */

public use Random;

config const randSeed = SeedGenerator.oddCurrentTime;

var emptyArray: [1..0] int(8),
  realArray: [1..1000] real,
  randStream = new owned NPBRandomStream(real, randSeed);

randStream.fillRandom(realArray);
var intArray = [i in 1..1000] (realArray[i] * 1000): int;
