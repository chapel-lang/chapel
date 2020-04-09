use Set;

record testRecord {
  var dummy: int = 0;
}

class testClass {
  var dummy: int = 0;
}

var s1: set(int);
var s2: set(testRecord);
var s3: set(borrowed testClass);

assert(s1.size == 0);
assert(s2.size == 0);
assert(s3.size == 0);
