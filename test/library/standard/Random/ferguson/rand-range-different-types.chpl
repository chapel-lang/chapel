use Random;

var rng = createRandomStream(real);

var expectInt = rng.getNext(int, 1, 10);
assert(expectInt.type == int);
assert(1 <= expectInt && expectInt <= 10);
var expectReal = rng.getNext(real, 1.0, 10.0);
assert(expectReal.type == real);
assert(1.0 <= expectReal && expectReal <= 10.0);
