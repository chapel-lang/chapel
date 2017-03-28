// Random

/*
   This primer demonstrates usage of the standard module Random.chpl.
*/

use Random;

//
// This primer shows two ways to generate a sequence of random numbers:
// The first is by creating an array of random numbers using the
// :proc:`Random.fillRandom` function. The second way is to use
// a :class:`~PCGRandom.RandomStream` instance.
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
var randsSeeded: [1..10] real;
var seed = 17;
fillRandom(randsSeeded, seed);
writeln("randsSeeded = ", randsSeeded); // Here the output is deterministic
writeln();

// Other numeric types are supported:
var rand16s: [1..10] uint(16);
fillRandom(rand16s, seed);
writeln("rand16s = ", rand16s); // Here the output is deterministic
writeln();


// Using RandomStream
// ------------------

//
// The second way to generate a sequence of random numbers is by creating a
// :class:`~PCGRandom.RandomStream` class instance.  The first argument is the
// type of the elements that the instance should generate. If a particular seed
// is desired, it should be specified upon creation of this instance.
//
var randStream:       RandomStream(real) = new RandomStream(real);
var randStreamSeeded: RandomStream(real) = new RandomStream(real, seed);

//
// Then the instance can be used to obtain the numbers.  This can be done in a
// large chunk by calling :proc:`~PCGRandom.RandomStream.fillRandom`:
//
var randsFromStream: [1..10] real;
randStream.fillRandom(randsFromStream);

//
// Or random numbers can be requested one at a a time.
//
var nextRand = randStreamSeeded.getNext();
writeln(nextRand == randsSeeded[1]);

// Note that since since we are using the same seed, the numbers generated will
// match those generated earlier by ``fillRandom(randsSeeded, seed)``.

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
// than ``0``.
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
// By default, access using the :class:`~PCGRandom.RandomStream` instance will
// be safe in the presence of parallelism. This can be changed for the entire
// stream during class creation.  As a result, two parallel accesses or updates
// to the position from which reading is intended may conflict.
//
var parallelUnsafe       = new RandomStream(real, parSafe=false);
var parallelSeededUnsafe = new RandomStream(real, seed, false);

// Now :class:`~PCGRandom.RandomStream` functions, such as
// ``parallelUnsafe.getNext()`` and ``parallelSeededUnsafe.getNext()`` can be
// called.

//
// At present, ``RandomStream`` instances are classes and so they must be
// deleted.
//
delete parallelSeededUnsafe;
delete parallelUnsafe;
delete randStreamSeeded;
delete randStream;
