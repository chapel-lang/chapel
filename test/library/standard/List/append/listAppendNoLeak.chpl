/*
  This test mirrors a similar test written for array.push_back, now rewritten
  to use list.pushBack instead.
*/

use List;

var wrappedLists: list(WrappedList);

const n = 10; // n is parsed from a file

extend(wrappedLists, n);

for i in 0..#n do
  wrappedLists[i].pushBack(i);


record WrappedList {
  var List: list(int);

  proc pushBack(node) {
    this.List.pushBack(node);
  }
}

proc extend(ref L: list(?t, ?), n: int) {
  var default: t;
  for 1..n do
    L.pushBack(default);
}
