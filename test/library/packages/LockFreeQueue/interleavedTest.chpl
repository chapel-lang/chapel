use LockFreeQueue;

const InitialQueueSize = 1024 * 1024;
const OperationsPerThread = 1024 * 1024;

proc main() {
  var lfq = new unmanaged LockFreeQueue(int);

  // Fill the queue and warm up the cache.
  forall i in 1..InitialQueueSize with (var tok = lfq.getToken()) do lfq.enqueue(i, tok);

  coforall tid in 1..here.maxTaskPar {
    var tok = lfq.getToken();
    // Even tasks handle enqueue, odd tasks handle dequeue...
    if tid % 2 == 0 {
      for i in 1..OperationsPerThread do lfq.enqueue(i, tok);
    } else {
      for i in 1..OperationsPerThread do lfq.dequeue(tok);
    }
  }
  lfq.tryReclaim();
}
