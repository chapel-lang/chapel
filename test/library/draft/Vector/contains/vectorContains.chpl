use Vector;

config const testIters = 10;

var v: vector(int) = 1..testIters;

for i in 1..testIters {
  assert(v.contains(i));
}

for i in -1..-testIters {
  assert(!v.contains(i));
}

v.clear();
for i in 1..testIters {
  assert(!v.contains(i));
}