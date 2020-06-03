/* boundedBuffer.chpl - a simple bounded buffer pattern.
 *
 * Based on a UW CSEP 524 exercise developed by Brandon Myers, February 2013
 * Updated by Brad Chamberlain, November 2016
 *
 */

use Random, Time;                  // get a RNG and sleep() to introduce noise
use IO;                            // allow use of stderr

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
// set them on every run (I had good results with 5 of each).
//
// STEP 2: Modify task 1 of the cobegin below to create 'numProducers'
// producers numbered 1..numProducers.  Similarly, modify task 2 so
// that it creates 'numConsumers' consumers numbered 1..numConsumers.
// While you can compile and run at this point (and should), note that
// the program is probably not correct (see STEP 3 below).
//
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
// unsynchronized, meaning that when multiple producers and consumers
// are running, their reads and writes to them may race.  The
// problematic statement is the following one from 'advance()':
//
//     pos = (pos + 1) % capacity;
//
// Specifically, if two tasks are executing this statement
// simultaneously, their operations may interleave as follows:
// Task A reads pos -- let's say that it has the value '21'
// Task B reads pos, which is still '21'
// Task A computes the new value of pos as '22' and writes it
// Task B also computes the value of '22' and writes it
// each task returns '21'
//
// This is a race because it means that both tasks will have obtained
// the same position rather than each getting a unique position.
//
// While the chances of this race are extremely slim (and slimmer in
// --verbose=true mode because of the time required for all of the
// printing), if this were your bank account or airline reservation,
// you wouldn't want to take the risk of not hitting it.
//
// A conventional approach might be to put some sort of mutual
// exclusion around the noted statement so that only one task could
// execute it at a time, but in Chapel, we prefer data-centric
// coordination between tasks.  To that end, guard against this race
// by changing head and tail to 'sync' variables.  Note that in
// addition to changing their definitions, you'll need to change the
// advance() helper function below.
//
// STEP 4: Now try doing step 3 again by making head and tail 'atomic'
// variables (save a copy of your 'sync' version in case you want to
// return to it later).
//
// Hints for the atomic-based solution:
//
// (1) atomic variables can't currently be initialized at their
// declaration point (something we're working on fixing in the
// language), but since they are integers, their initial values
// will be zero, so you probably will not need to initialize them.
// If you do, add an initializer to 'BoundedBuffer' of the form:
//
//   proc init() {
//     ... put your code to initialize the atomics here ...
//   }
//
// (2) read(), write() and compareAndSwap() are going to be the most
// useful methods on atomics for this exercise.  If you haven't worked
// with atomics before, refer to the online documentation or ask one
// of the helpers for a hint:
//
//   https://chapel-lang.org/docs/builtins/internal/Atomics.html
//
// STEP 5 (optional): Compare the performance of your two versions
// (note: you may want to turn off the --noisy and/or --verbose
// configs when doing timings, and as always for performance runs, be
// sure to compile with the --fast flag).
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

  var rng = new owned RandomStream(real);

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
}
