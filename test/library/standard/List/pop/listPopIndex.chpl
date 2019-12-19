private use List;

config const testIters = 8;

var lst: list(int);

for i in 1..testIters do
  lst.append(i);

for i in 1..testIters {
  const elem = lst.pop(0);
  assert(elem == i);
}


