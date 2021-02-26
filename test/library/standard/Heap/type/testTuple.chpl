import HeapTest;

type t = (shared HeapTest.T, shared HeapTest.T);

HeapTest.testHeap(t);
