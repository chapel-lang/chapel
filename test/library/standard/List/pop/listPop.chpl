private use List;

config const testIters = 16;

var lst = new list(int, true);

for i in 1..testIters do
  lst.pushBack(i);

for i in 1..testIters by -1 {
  const elem = lst.popBack();
  assert(elem == i);
}
