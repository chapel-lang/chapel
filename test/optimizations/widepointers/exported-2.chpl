// Adapted from #20861

use CTypes;
use AllLocalesBarriers;

config param shouldCrash : bool = true;

var globalDom : domain(1, int);
var globalArr : [globalDom] atomic real;

class GlobalBuffer {
  var dom : domain(1, int);
  var arr : [dom] atomic real;
}

var alternative = new GlobalBuffer();

require "exported-2.h";

extern proc bar(procID : c_int, size : c_int);

export proc foo(sendBuf : c_ptr(real), recvBuf : c_ptr(real), count : c_ptr(c_int)) {
  allLocalesBarrier.barrier();

  const n = count.deref():int;
  const indices = 0 ..# n;

  ref dom = if shouldCrash then globalDom else alternative.dom;
  ref arr = if shouldCrash then globalArr else alternative.arr;

  // Grow the buffer if it's not big enough
  if here.id == 0 {
    if n > dom.size then
      dom = {indices};

    // IMPORTANT!
    arr.write(0, memoryOrder.relaxed);
  }

  // Make sure locale 0 has had the chance to resize before proceeding          
  allLocalesBarrier.barrier();
  assert(arr.size >= n);

  // Have all locales atomically add their results to the atomicBuff            
  forall i in indices do
    arr[i].add(sendBuf[i], memoryOrder.relaxed);

  // Make sure all locales have accumulated their contributions                 
  allLocalesBarrier.barrier();

  // Have each locale copy the results out into its buffer                      
  forall i in indices do
    recvBuf[i] = arr[i].read();
}

proc main() {
  coforall loc in Locales do on loc {
    for i in 1 .. 5 {
      writeln(i);
      bar(loc.id:c_int, i:c_int);
    }
  }
}
