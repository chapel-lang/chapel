use Pythonic;

// Exercise enumerate iterator.
config const lo = 17;
config const hi = 42;
const strFormat = "%5i : %i\n";

writeln("=== with zippering ===");
for (value, i) in zip(lo..hi, 1..) do
  writef(strFormat, i, value);

// Serial
writeln("=== with serial enumerate ===");
for (i, value) in enumerate(lo..hi, start=1) do
  writef(strFormat, i, value);

var A: [1..#(lo..hi).size] int;

// Parallel
writeln("=== with parallel enumerate ===");
forall (i, value) in enumerate(lo..hi, start=1) do
  A[i] = value;

for i in A.domain do
  writef(strFormat, i, A[i]);

// Serial
writeln("=== with serial enumerate ===");
for (i, value) in enumerate(lo..hi) do
  writef(strFormat, i, value);

var B: [0..#(lo..hi).size] int;

// Parallel
writeln("=== with parallel enumerate ===");
forall (i, value) in enumerate(lo..hi) do
  B[i] = value;

for i in B.domain do
  writef(strFormat, i, B[i]);
