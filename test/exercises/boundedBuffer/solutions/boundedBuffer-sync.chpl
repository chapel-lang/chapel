/* boundedBuffer.chpl - a simple bounded buffer pattern.
 *
 * Based on a UW CSEP 524 exercise developed by Brandon Myers, February 2013
 * Updated by Brad Chamberlain, November 2016
 *
 */

use Random, Time;                  // get a RNG and sleep() to introduce noise
use IO;                            // allow use of stderr

config const numProducers = 5,     // the number of producers to create
             numConsumers = 5,     // the number of consumers to create
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

proc main() {
  // a shared bounded buffer with the requested capacity
  var buffer = new owned BoundedBuffer(capacity=bufferSize);

  // per-producer/consumer counts of the number of items they processed
  var prodCounts: [1..numProducers] int,
      consCounts: [1..numConsumers] int;

  // spawn two tasks using a 'cobegin'
  cobegin {
    // Task 1: run a single producer and store the number of things
    // it produces in 'prodCounts[1]'.  When it's done, write a
    // sentinel value per consumer.
    {
      coforall pid in 1..numProducers do
        prodCounts[pid] = producer(buffer, pid);
      // We know all producers are done due to the coforall
      for i in 1..numConsumers do
        buffer.writeSentinel();
    }

    // Task 2: create a consumer and store the number of things it
    // consumers in 'consCounts[1]'.
    coforall cid in 1..numConsumers do
      consCounts[cid] = consumer(buffer, cid);
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
// This is a generic bounded buffer class
//
class BoundedBuffer {
  type eltType = real;    // it can store scalar types; the default is 'real'

  const capacity: int,                     // the capacity of the buffer
        sentinel: eltType = -1.0;          // the sentinel value

  var buff$: [0..#capacity] sync eltType,  // the sync values, empty by default
      head$, tail$: sync int = 0;      // the cursor positions, full by default

  var rng = new owned RandomStream(real);

  // TODO: I don't think this should be required
  proc init(type eltType = real, capacity: int = 0) {
    this.eltType = eltType;
    this.capacity = capacity;
  }

  //
  // Place an item at the head position of the buffer, assuming
  // it's available (empty).  If not, the write to 'buff$[head]' will
  // block until it is.  Then advance the 'head' position.
  //
  proc produce(item: eltType) {
    if noisy then sleep(rng.getNext() / prodNoiseScale);

    buff$[advance(head$)].writeEF(item);
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

    const val = buff$[advance(tail$)].readFE();
    return (val, val != sentinel);
  }

  //
  // a simple helper function for advancing the head or tail position.
  //
  inline proc advance(ref pos$: sync int) {
    const prevPos = pos$.readFE();

    pos$.writeEF((prevPos + 1) % capacity);

    return prevPos;
  }
}
