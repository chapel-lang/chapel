private use List;

config const testIters = 8;

var lst: list(int, true);
var idx = 0;

for i in 1..testIters {
  lst.append(i);
  idx = lst.indexOf(i);
  assert(idx == i);
}

for i in 1..testIters do
  lst.append(i);

idx = lst.indexOf(testIters, testIters + 1, testIters * 2);

assert(idx > 0);

lst.pop();

idx = lst.indexOf(testIters, testIters + 1);

assert(idx < 0);

