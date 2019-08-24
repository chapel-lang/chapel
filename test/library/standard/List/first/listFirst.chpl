private use List;

config const testIters = 8;
var didThrow = false;

var lst: list(int, true);

try {
  ref elem = lst.first();
} catch e: IllegalArgumentError {
  didThrow = true;
} catch {}

assert(didThrow);

for i in 1..testIters do
  lst.append(i);

assert(!lst.isEmpty());

ref elem = try! lst.first();

assert(elem == 1);

