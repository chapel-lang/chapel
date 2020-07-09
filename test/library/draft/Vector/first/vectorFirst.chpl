private use Vector;

config const testIters = 8;

var lst: vector(int, true);

for i in 1..testIters do
  lst.append(i);

assert(!lst.isEmpty());

ref elem = lst.first();

assert(elem == 1);

