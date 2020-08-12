//
// Bounded Buffer Exercise
//
// Create a bounded buffer to manage pipeline parallelism
//

config const problemSize: int = 10;
config const nConsumers:int = 2;
config const nMiddleSteps:int = 3;

class BoundedBuffer {
  param bufSize:int = 2;
  type eltType = int;

  var buffer$: [0..#bufSize] sync eltType;
  var producerPos$: sync int = 0;
  var consumerPos$: sync int = 0;

  // TODO: I don't think this should be required
  proc init(param bufSize = 2, type eltType = int) {
    this.bufSize = bufSize;
    this.eltType = eltType;
  }

  // Add a value to the circular buffer. If it is full, wait until a
  // value has been consumed
  proc add(i: eltType) {
    var c = producerPos$.readFE();
    producerPos$.writeEF((c + 1) % bufSize);
    buffer$(c).writeEF(i);
  }

  // Remove a value from the buffer. If it is empty, wait until a
  // value has been produced
  proc remove(): eltType {
    var c = consumerPos$.readFE();
    consumerPos$.writeEF((c + 1) % bufSize);
    return buffer$(c).readFE();
  }

  // Yield all values in the buffer until the -1 sentinel value is found
  iter these() {
    var val = remove();
    while val != -1 {
      yield val;
      val = remove();
    }
    add(-1);
  }
}

var buffers: [0..nMiddleSteps] unmanaged BoundedBuffer = [0..nMiddleSteps] new unmanaged BoundedBuffer();

// Given a value, do some work on it to create the next value. In this
// case, the work is sleeping for a second and leaving the value unchanged.
proc createNextValue(val) {
  use Time;
  sleep(1);
  return val;
}

// Produce the numbers from 1 to nProducts by adding them to the first
// bounded buffer. -1 is used as a sentinel to indicate that the final
// value has been produced.
proc producer(nProducts: int) {
  for i in 1..nProducts {
    createNextValue(i);
    writeln("producer producing ", i);
    buffers(0).add(i);
  }
  writeln("producer producing ", -1);
  buffers(0).add(-1);
}

// Act as both a consumer and a producer. It consumes a value from buffer
// i-1 and produces it to buffer i.
proc middleStep(i: int) {
  for val in buffers(i-1) {
    writeln("middleStep ", i, " consumed ", val);
    var next = createNextValue(val);
    writeln("middleStep ", i, " producing ", val);
    buffers(i).add(next);
  }
  writeln("middleStep ", i, " producing ", -1);
  buffers(i).add(-1);
}

// Consume values from the final buffer. These values were created by the
// producer and have passed through each middleStep.
proc consumer(consumerNumber: int) {
  for consumedValue in buffers(nMiddleSteps) {
    writeln("consumer ", consumerNumber, ": Consumed: ", consumedValue);
  }
}

proc main {
  // Simultaneously start a producer, nMiddleSteps middleSteps and
  // nConsumers consumers.
  cobegin {
    producer(problemSize);

    coforall i in 1..nMiddleSteps do
      middleStep(i);

    coforall i in 1..nConsumers do
      consumer(i);
  }

  for b in buffers do delete b;
}
