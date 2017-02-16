/* boundedBuffer.chpl - a simple bounded buffer pattern.
 *
 * Based on a UW CSEP 524 exercise developed by Brandon Myers, February 2013
 * Updated by Brad Chamberlain, November 2016
 *
 */

use Random, Time;                  // get a RNG and sleep() to introduce noise

config const numProducers = 1,     // the number of producers to create
             numConsumers = 1,     // the number of consumers to create
             bufferSize = 30,      // the capacity of the bounded buffer
             numItems = 1000,      // the number of items to produce/consume

             verbose = true,       // print what's produced/consumed?

             //
             // the following configs control whether or not noise is
             // injected into the computation in the form of sleep()
             // calls for random numbers of seconds (0.0-1.0), scaled
             // by the given factors for the producer and consumer.
             //
             noisy = true,         // inject noise into the computation?
             prodNoiseScale = 100,
             consNoiseScale = prodNoiseScale;

//
// STEP 0: Compile and run the code as-is.  In the default --verbose
// mode, you'll see a trace of what the single producer and consumer
// are doing and tallies at the end showing how many values were
// produced and consumed.
//

//
// The following check is here to make sure that nobody tries to run
// the baseline framework with multiple producers and consumers, since
// it's not designed to handle that case well.
//
if (numProducers > 1 || numConsumers > 1) then
  halt("This program needs changes to handle multiple producers/consumers");
//
// STEP 1: When you're ready to start modifying the code, remove the
// stopgap test + halt just above.  You may also want to change the
// default number of producers and consumers above to avoid having to
// set them on each run (I had good luck with 5 of each).
//
// STEP 2: Modify task 1 of the cobegin below such that it creates
// 'numProducers' producers numbered 1..numProducers.  Similarly, modify
// task 2 such that it creates 'numConsumers' consumers numbered
// 1..numConsumers.
//
proc main() {
  // a shared bounded buffer with the requested capacity
  var buffer = new BoundedBuffer(capacity=bufferSize);

  // per-producer/consumer counts of the number of items they processed
  var prodCounts: [1..numProducers] int,
      consCounts: [1..numConsumers] int;

  // spawn two tasks using a 'cobegin'
  cobegin {
    // Task 1: run a single producer and store the number of things
    // it produces in 'prodCounts[1]'.  When it's done, write a
    // sentinel value per consumer.
    {
      prodCounts[1] = producer(buffer, pid=1);
      for i in 1..numConsumers do
        buffer.writeSentinel();
    }

    // Task 2: create a consumer and store the number of things it
    // consumers in 'consCounts[1]'.
    consCounts[1] = consumer(buffer, cid=1);
  }

  // if we're in verbose mode, print out the counts
  if (verbose) {
    writeln("Producer counts: ", prodCounts);
    writeln("Consumer counts: ", consCounts);
  }

  // count the total number of things produced and consumed (not
  // particularly interested until there are multiple producers
  // and consumers).
  const prodTot = + reduce prodCounts,
        consTot = + reduce consCounts;

  // verify that the counts came out as expected
  if (prodTot != numItems) then
    stderr.writef("Producer(s) produced %i items rather than %i\n",
                  prodTot, numItems);
  else if (consTot != numItems) then
    stderr.writef("Consumer(s) consumed %i items rather than %i\n",
                  consTot, numItems);
  else
    stderr.writef("Producers/consumers processed %i items total.\n", numItems);

  delete buffer;
}


//
// produce 1/numProducers of the requested 'numItems' items using an
// aligned strided range.  Return the number of items we produced.
//
proc producer(b: BoundedBuffer, pid: int) {
  var myItems = 0..#numItems by numProducers align pid-1;

  for i in myItems {
    if verbose then writeln("producer ", pid, " producing ", i);
    b.produce(i);
  }

  return myItems.size;
}

//
// consume items greedily until a sentinel value is found.  Return
// the number of items we successfully consumed.
//
proc consumer(b: BoundedBuffer, cid: int) {
  var count = 0;
  do {
    const (data, more) = b.consume();
    if more {
      if verbose then writeln("consumer ", cid, " consumed ", data);
      count += 1;
    } else {
      if verbose then writeln("consumer ", cid, " got its sentinel value ");
    }
  } while (more);

  return count;
}


//
// STEP 3: The head and tail member variables below are currently
// unsynchronized, meaning that if multiple producers and consumers
// are running, their reads and writes to them will race.  Protect
// against this by changing them to 'sync' or 'atomic' types.  (Hint:
// 'sync' is easier, so start there).  In addition to changing their
// definitions, you'll need to change the advance() helper function
// below.
//
// STEP 4: Now try doing step 3 again using the other type (presumably
// atomics).  Save both versions so you can compare the performance of
// them (note: you may want to turn off the --noisy config when doing
// timings).
//
// Hints for the atomic-based solution:
//
// (1) atomics currently can't be initialized (something we need to
// fix in the language), so to initialize them, use a constructor of
// the form:
//
//   proc BoundedBuffer() {
//     ... your code to initialize them here ...
//   }
//
// (2) read(), write() and compareExchange() are going to be your
// friends.  If you haven't worked with atomics before, ask one of
// the helpers or refer to the online documentation:
//
//   http://chapel.cray.com/docs/latest/builtins/Atomics.html
//
// STEP 5: If you were not able to complete the distributed memory
// ray-tracer earlier, this would be a great time to try it again
// now that you've heard more about domain maps.
//
// STEP 6 (optional): What would it take to make this bounded buffer
// code a distributed memory program?
//
// STEP 7 (optional and macho): What would it take to make this
// bounded buffer code into a *scalable* distributed memory program?
//


//
// This is a generic bounded buffer class
//
class BoundedBuffer {
  type eltType = real;    // it can store scalar types; the default is 'real'

  const capacity: int,                     // the capacity of the buffer
        sentinel: eltType = -1.0;          // the sentinel value

  var buff$: [0..#capacity] sync eltType,  // the sync values, empty by default
      head = 0,                            // the head's cursor position
      tail = 0;                            // the tail's cursor position

  var rng = new RandomStream(real);

  //
  // Place an item at the head position of the buffer, assuming
  // it's available (empty).  If not, the write to 'buff$[head]' will
  // block until it is.  Then advance the 'head' position.
  //
  proc produce(item: eltType) {
    if noisy then sleep(rng.getNext() / prodNoiseScale);

    buff$[advance(head)] = item;
  }

  //
  // Mark that all the producers are done.  This should be called once
  // per consumer and only once all producers are done.
  //
  proc writeSentinel() {
    produce(sentinel);
  }

  //
  // Consume() an item from the tail position of the buffer, assuming
  // it's available (full).  If not, the read from 'buff$[tail]' will
  // block until it is.  Then advance the 'tail' position.  Return a
  // tuple containing (1) the data value read and (2) 'true' if there
  // is more data to be read (the sentinel value has not been found),
  // 'false' otherwise.
  //
  proc consume(): (eltType, bool) {
    if noisy then sleep(rng.getNext() / consNoiseScale);

    const val = buff$[advance(tail)];
    return (val, val != sentinel);
  }

  //
  // a simple helper function for advancing the head or tail position.
  //
  inline proc advance(ref pos) {
    const prevPos = pos;

    pos = (pos + 1) % capacity;

    return prevPos;
  }

  //
  // Clean up after ourselves
  //
  proc deinit() {
    delete rng;
  }
}
