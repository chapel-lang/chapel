private use List;

var lst : list(int);
var idx = 0;
config const testIters = 8;

for i in 1..testIters do
  lst.append(i);

idx = lst.indexOf(3);
assert(idx == 2);

