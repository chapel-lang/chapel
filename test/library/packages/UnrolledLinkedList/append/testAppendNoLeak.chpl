/*
  This test mirrors a similar test written for array.push_back, now rewritten
  to use UnrolledLinkedList.append instead.
*/

use UnrolledLinkedList;

record WrappedList {
  var List: unrolledLinkedList(int);

  proc append(node) {
    this.List.append(node);
  }
}

proc extend(ref L: unrolledLinkedList(?t, ?), n: int) {
  var default: t;
  for 1..n do
    L.append(default);
}
var wrappedLists: unrolledLinkedList(WrappedList);

const n = 10; // n is parsed from a file

extend(wrappedLists, n);

for i in 0..#n do
  wrappedLists[i].append(i);
