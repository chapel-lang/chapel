/*
 * Standard Module: Random primer
 *
 * This primer demonstrates usage of the standard module Random.chpl.
 *
 */

use Random;

//
// This module contains a random number generator based on
// the one used in the NPB benchmarks.  Tailoring the NPB comments to
// this code, we can say the following:
//
//   This generator returns uniform pseudorandom real values in the
//   range (0, 1) by using the linear congruential generator
//
//     x_{k+1} = a x_k  (mod 2**46)
//
//   where 0 < x_k < 2**46 and 0 < a < 2**46.  This scheme generates
//   2**44 numbers before repeating.  The seed value must be an odd
//   64-bit integer in the range (1, 2^46).  The generated values are
//   normalized to be between 0 and 1, i.e., 2**(-46) * x_k.
//
//   This generator should produce the same results on any computer
//   with at least 48 mantissa bits for real(64) data.
//
// The generation functionality is currently restricted to default-sized
// reals, imags, and complex values.
//
// By default the random stream seed is initialized based on the current
// time in microseconds.
//


//
// There are two ways to generate a sequence of random numbers:
// The first is by creating an array of random numbers using the fillRandom
// function
//
var rands: [1..10] real;
fillRandom(rands);
//writeln(rands); // Commented out for deterministic testing output.

//
// In this case, you can specify the starting seed to use when filling the
// array.  This seed must be a an odd integer between 0 and 2**46
//
var randsSeeded: [1..10] real;
var seed = 17;
fillRandom(randsSeeded, seed);
writeln("randsSeeded = ", randsSeeded); // Here the output is deterministic
writeln();

//
// The second way to generate a sequence of random numbers is by creating
// a RandomStream class instance.  If one is desired, the seed must be
// specified upon creation of this instance.
//
var randStream: RandomStream = new RandomStream();
var randStreamSeeded: RandomStream = new RandomStream(seed);

//
// Then the instance can be used to obtain the numbers.
// This can be done in a large chunk...
//
var randsFromStream: [1..10] real;
randStream.fillRandom(randsFromStream);
//writeln(randsFromStream); // Commented out for deterministic testing output.

//
// ... or individually.  Since we are using the same seed, the
// numbers generated will match the contents of randsSeeded.
//
var nextRand = randStreamSeeded.getNext();
writeln(nextRand == randsSeeded[1]);

//
// The next random number generated will follow the most
// recent...
//
var secondRand = randStreamSeeded.getNext();
writeln(secondRand == randsSeeded[2]);

// ...unless the position to look at has been changed.
randStreamSeeded.skipToNth(7);
var seventhRand = randStreamSeeded.getNext();
writeln(seventhRand == randsSeeded[7]);

//
// A specific random number in the stream can be obtained by
// specifying the position.  This argument must be greater
// than 0.
//
var secondRand2 = randStreamSeeded.getNth(2);
writeln(secondRand2 == secondRand);

//
// This position can be earlier or later than the most recent.
//
var fourthRand = randStreamSeeded.getNth(4);
writeln(fourthRand == randsSeeded[4]);


//
// The stream can be used to iterate over a specified set of positions.
//
for i in randStreamSeeded.iterate({5..10}, real) {
  writeln(i);
}


//
// By default, access using the RandomStream instance will be safe in the
// presence of parallelism. This can be changed for the entire stream during
// class creation.  As a result, two parallel accesses or updates to the
// position from which reading is intended may conflict.
//
var parallelUnsafe = new RandomStream(parSafe=false);
var parallelSeededUnsafe = new RandomStream(seed, false);
// Commented out for deterministic testing output.
//writeln(parallelUnsafe.getNext());
//writeln(parallelSeededUnsafe.getNext());
