import HeapTest;

record Y {
  var value = 0;
}

proc <(left: Y, right: Y) {
  return left.value < right.value;
}

type t = Y;

HeapTest.testHeap(t);
