private use List;

config const testIters = 16;
var didThrow = false;

var lst = new list(int, true);

try {
  const elem = lst.pop();
} catch e: IllegalArgumentError {
  didThrow = true;
} catch {}

assert(didThrow);
didThrow = false;

for i in 1..testIters do
  lst.append(i);

for i in 1..testIters by -1 {
  const elem = try! lst.pop();
  assert(elem == i);
}

for i in 1..testIters do
  lst.append(i);

for i in 1..testIters {
  const elem = try! lst.pop(1);
  assert(elem == i);
}

for i in 1..testIters do
  lst.append(i);

try! lst.pop();

try {
  const elem = lst.pop(testIters);
} catch e: IllegalArgumentError {
  didThrow = true;
} catch {}

assert(didThrow);

