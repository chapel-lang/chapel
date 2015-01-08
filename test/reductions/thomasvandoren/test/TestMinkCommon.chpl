/*
 * Common mink() test utilities.
 */

use mink;
use Random;

config const randSeed = SeedGenerator.currentTime;

var A: [1..1000] real,
  randStream = new RandomStream(randSeed);

randStream.fillRandom(A);
var B = [i in 1..1000] (A[i] * 1000): int;
