private use Vector;

config const testIters = 8;

var vec: vector(int);

for i in 1..testIters do
  vec.append(i);

assert(vec.size == testIters);

var removed = vec.remove(testIters);

assert(vec.size == (testIters - 1));
assert(removed == 1);

//
// Nothing should happen when a user attempts to remove an element not
// present in the vector.
//

var oldSize = vec.size;

removed = vec.remove(testIters);

assert(removed == 0);
assert(vec.size == oldSize);


