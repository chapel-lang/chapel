/* boundedBuffer.chpl - a simple bounded buffer pattern.
 *
 * Based on a UW CSEP 524 exercise developed by Brandon Myers, February 2013
 * Updated by Brad Chamberlain, November 2016
 *
 */

config const numProducers = 1,   // the number of producers to create
             numConsumers = 1,   // the number of consumers to create
             bufferSize = 4,     // the capacity of the bounded buffer
             numItems = 1000,    // the number of items to produce/consume
             verbose = true;     // print what's produced/consumed?

//
// You'll need to modify the program to handle multiple producers and
// consumers.  Until then, this check is here to avoid mistakes.
//
if (numProducers > 1 || numConsumers > 1) then
  halt("This program needs changes to handle multiple producers/consumers");


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
}


//
// produce 1/numProducers of the requested 'numItems' items using an
// aligned strided range.  Return the number of items we produced.
//
proc producer(b: BoundedBuffer, pid: int) {
  var myItems = 0..#numItems by numProducers align pid;

  for i in myItems {
    if verbose then writeln("producer ", pid, " producing ", i);
    b.produce(i);
  }

  return myItems.size;
}

//
// consume items greedily until a sentinel value is found.  Return
// the number of items we successfuly consumed.
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
        sentinel: eltType = -1.0;          // the senteinel value

  var buff$: [0..#capacity] sync eltType,  // the sync values, empty by default
      head = 0,                            // the head's cursor position
      tail = 0;                            // the tail's cursor position

  //
  // Place an item at the head position of the buffer, assuming
  // it's available (empty).  If not, the write to 'buff$[head]' will
  // block until it is.  Then advance the 'head' position.
  //
  proc produce(item: eltType) {
    buff$[head] = item;
    advance(head);
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
    const val = buff$[tail];
    advance(tail);
    return (val, val != sentinel);
  }

  //
  // a simple helper function for advancing the head or tail position.
  //
  inline proc advance(ref pos) {
    pos += 1;
    pos %= capacity;
  }
}

