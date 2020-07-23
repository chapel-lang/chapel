private use Vector;

config const testIters = 16;

var vec = new vector(int, true);

for i in 1..testIters do
  vec.append(i);

for i in 1..testIters by -1 {
  const elem = vec.pop();
  assert(elem == i);
}

