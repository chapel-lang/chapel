// Random

/*
   This primer demonstrates usage of the standard module Random.chpl.
*/

use Random;

//
// This primer shows two ways to generate a sequence of random numbers:
// The first is by creating an array of random numbers using the
// :proc:`Random.fillRandom` function. The second way is to use
// a :class:`~PCGRandom.PCGRandomStream` instance.
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


// Using RandomStream
// ------------------

//
// The second way to generate a sequence of random numbers is by creating a
// :class:`~PCGRandom.PCGRandomStream` class instance.  The first argument is the
// type of the elements that the instance should generate. If a particular seed
// is desired, it should be specified upon creation of this instance.
//
var randStream = new RandomStream(real);
var randStreamSeeded = new RandomStream(real, seed);

//
// Then the instance can be used to obtain the numbers.  This can be done in a
// large chunk by calling :proc:`~PCGRandom.PCGRandomStream.fillRandom`:
//
var randsFromStream: [1..10] real;
randStream.fillRandom(randsFromStream);

//
// Or random numbers can be requested one at a a time.
//
var firstRand = randStreamSeeded.getNext();
writeln(firstRand == randsSeeded[0]);

// Note that since since we are using the same seed, the numbers generated will
// match those generated earlier by ``fillRandom(randsSeeded, seed)``.

//
// The next random number generated will follow the most
// recent...
//
var nextRand = randStreamSeeded.getNext();
writeln(nextRand == randsSeeded[1]);

// ...unless the position to look at has been changed.
randStreamSeeded.skipToNth(6);
var rand6 = randStreamSeeded.getNext();
writeln(rand6 == randsSeeded[6]);

//
// A specific random number in the stream can be obtained by
// specifying the position.  This argument must be greater
// than or equal to ``0``.
//
var rand1 = randStreamSeeded.getNth(1);
writeln(rand1 == nextRand);

//
// This position can be earlier or later than the most recent.
//
var rand3 = randStreamSeeded.getNth(3);
writeln(rand3 == randsSeeded[3]);


//
// The stream can be used to iterate over a specified set of positions.
//
for i in randStreamSeeded.iterate({5..10}, real) {
  writeln(i);
}


//
// By default, access using the :class:`~PCGRandom.PCGRandomStream` instance will
// be safe in the presence of parallelism. This can be changed for the entire
// stream during class creation.  As a result, two parallel accesses or updates
// to the position from which reading is intended may conflict.
//
var parallelUnsafe       = new RandomStream(real, parSafe=false);
var parallelSeededUnsafe = new RandomStream(real, seed, false);

// Now :class:`~PCGRandom.PCGRandomStream` functions, such as
// ``parallelUnsafe.getNext()`` and ``parallelSeededUnsafe.getNext()`` can be
// called.

//
// The ``RandomStream`` instances above were created with ``new [owned]``
// and so are automatically deleted when they go out of scope.
//
