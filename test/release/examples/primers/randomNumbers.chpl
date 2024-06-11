// Random

/*
   This primer demonstrates usage of the standard module Random.chpl.
*/

use Random;

//
// This primer shows two ways to generate a sequence of random numbers:
// The first is by creating an array of random numbers using the top-level
// :proc:`Random.fillRandom` function. The second way is to use
// a :record:`~Random.randomStream` instance.
//

// Using fillRandom
// ----------------

// Call :proc:`Random.fillRandom` with an array argument. The array
// will be filled with random numbers.
var rands: [1..10] real;
fillRandom(rands);
// Now ``writeln(rands)`` would output these random values, but
// then this program's output would not be deterministic.

//
// To produce deterministic output, specify the starting seed to use when
// filling the array.
//
var randsSeeded: [0..9] real;
var seed = 17;
fillRandom(randsSeeded, seed);
writeln("randsSeeded = ", randsSeeded); // Here the output is deterministic
writeln();

// Other numeric types are supported:
var rand16s: [1..10] uint(16);
fillRandom(rand16s, seed);
writeln("rand16s = ", rand16s); // Here the output is deterministic
writeln();


// Using ``randomStream``
// ----------------------

//
// The second way to generate a sequence of random numbers is by creating a
// :record:`~Random.randomStream` instance.  The first argument in its
// initializer is the type of the elements that the instance should generate.
// The second argument is a seed value of type ``int``.
var randStream = new randomStream(real);
var randStreamSeeded = new randomStream(real, seed);

//
// Then the instance can be used to obtain the numbers.  This can be done in a
// large chunk by calling :proc:`~Random.randomStream.fill`:
//
var randsFromStream: [1..10] real;
randStream.fill(randsFromStream);

//
// Or random numbers can be requested one at a a time:
//
var firstRand = randStreamSeeded.next();
writeln(firstRand == randsSeeded[0]);

// Note that since since we are using the same seed, the numbers generated will
// match those generated earlier by ``fillRandom(randsSeeded, seed)``.

//
// The next random number generated will follow the most
// recent...
//
var nextRand = randStreamSeeded.next();
writeln(nextRand == randsSeeded[1]);

//
// ...unless the position to look at has been changed. In this case,
// the 6th random number in the stream will be generated. Note that
// the position argument must be greater than or equal to ``0``.
//
randStreamSeeded.skipTo(6);
var rand6 = randStreamSeeded.next();
writeln(rand6 == randsSeeded[6]);

//
// The position can be earlier or later than the most recent.
//
randStreamSeeded.skipTo(3);
var rand3 = randStreamSeeded.next();
writeln(rand3 == randsSeeded[3]);


//
// The stream can also be used to iterate over a domain.
// Note that this doesn't yield the 5th through 10th elements
// of the stream, but rather the next 5 random numbers.
//
for i in randStreamSeeded.next({5..10}) {
  writeln(i);
}
