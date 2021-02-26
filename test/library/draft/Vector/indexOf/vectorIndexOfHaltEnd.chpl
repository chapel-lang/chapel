private use Vector;

config const testIters = 8;

var lst: vector(int);

for i in 1..testIters do
  lst.append(i);

lst.pop();

var idx = lst.indexOf(testIters, 1, testIters);

