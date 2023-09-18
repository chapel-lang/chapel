private use List;

config const testIters = 8;

var lst: list(int);

for i in 1..testIters do
  lst.pushBack(i);

assert(lst.size == testIters);

var removed = lst.remove(testIters);

assert(lst.size == (testIters - 1));
assert(removed == 1);

//
// Nothing should happen when a user attempts to remove an element not
// present in the list.
//

var oldSize = lst.size;

removed = lst.remove(testIters);

assert(removed == 0);
assert(lst.size == oldSize);
