private use List;

config const testIters = 8;

var lst: list(int);

for i in 1..testIters do
  lst.append(i);

var idx = lst.indexOf(testIters, -1, testIters-1);

