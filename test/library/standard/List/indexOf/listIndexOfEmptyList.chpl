private use List;

config const testIters = 8;

var lst: list(int);

var idx = lst.indexOf(testIters);
assert(idx == -1);
