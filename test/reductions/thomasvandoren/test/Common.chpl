/*
 * Common mink() test utilities.
 */

public use Random;
private use Time;

config const randSeed = -1;

var emptyArray: [1..0] int(8),
  realArray: [1..1000] real,
  randStream = if randSeed == -1
    then new randomStream(real)
    else new randomStream(real, randSeed);

randStream.fill(realArray);
var intArray = [i in 1..1000] (realArray[i] * 1000): int;
