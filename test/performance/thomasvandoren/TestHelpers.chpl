/* Test helpers... */

use Random;
use Time;

config const randSeed = SeedGenerator.currentTime;
config const printC = true;
var randStream = new RandomStream(randSeed),
  timer: Timer;

// Assume C = A * B is the calculation.
var A: [1..10, 1..5] int,
  B: [1..5, 1..7] int,
  C: [1..10, 1..7] int;

// Fill A and B with random values.
fillRandom(A);
fillRandom(B);

/* Fill int array with random values. */
proc fillRandom(ref A: [] int) {
  // Use serial loop so A filling is reproducible when seed is same.
  for a in A {
    a = (randStream.getNext() * A.size): int;
  }
}

/* Print C, C[7, 7], and elapsed time. */
proc printResults() {
  if printC then
    writeln(C);
  writeln("C[7, 7] = ", C[7, 7]);
  writeln("Elapsed = ", timer.elapsed());
}
