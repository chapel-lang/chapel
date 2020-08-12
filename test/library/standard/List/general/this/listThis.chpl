private use List;

config const testIters = 8;

var lst: list(int);

for i in 1..testIters do
  lst.append(i);

assert(!lst.isEmpty());

for i in 0..#testIters do
  lst[i] *= 2;

// Perform an in place reversal of the list.
for i in 0..#(lst.size / 2) do {
  ref a = lst[lst.size - (i+1)];
  ref b = lst[i];
  a <=> b;
}

for i in 0..#testIters do
  lst[i] /= 2;

for i in 0..#testIters do
  assert((testIters - i) == lst[i]);
