/* 
 *   Producer-Consumer Example. 
 *
 *   This example uses a shared array of synchronization variables to
 *   implement a simple buffered producer/consumer relationship
 *   between two tasks.
 *
 *   To see the interleaving of producer and consumer work, try
 *   running with the --verbose=true flag.
 */


config const buffersize = 1024,        // size of the circular buffer
             numItems = 8*buffersize,  // number of items to write to buffer
             verbose = false;          // display producer's work


//
// Circular buffer of synchronization variables, which store
// full/empty state in addition to their numeric values.  By default,
// reads to these variables will block until they are "full", leaving
// them "empty".  Writes will block until "empty", leaving "full".
//
var buff$: [0..buffersize-1] sync int;


//
// The main procedure simply creates a producer and a consumer task to
// run in parallel
//
proc main() {
  cobegin {
    producer();
    consumer();
  }
}


//
// the producer loops over the requested number of items, writing them
// to the buffer starting at location 0 and wrapping around when it
// hits the end of the buffer.  It then writes the value -1 as a
// sentinel to the next position.
//
proc producer() {
  for i in 1..numItems {
    const buffInd = (i-1) % buffersize;
    buff$(buffInd).writeEF(i);

    if (verbose) then writeln("producer wrote value #", i);
  }
  buff$(numItems % buffersize).writeEF(-1);
}


//
// the consumer invokes an iterator to control its loop and yield
// values from the shared buffer.  It writes them out to the console.
//
proc consumer() {
  for buffVal in readFromBuff() {
    writeln("Consumer got: ", buffVal);
  }
}


//
// The readFromBuff() iterator simply reads values from the shared
// buffer starting at the 0th position and yields them.  Once it finds
// the sentinel value of -1, it exits.
//
iter readFromBuff() {
  var ind = 0,              
      nextVal = buff$(0).readFE();

  while (nextVal != -1) {
    yield nextVal;

    ind = (ind + 1)%buffersize;
    nextVal = buff$(ind).readFE();
  }
}
