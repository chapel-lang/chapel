// Exercise enumerate iterator.
config const lo = 17;
config const hi = 42;
const strFormat = "%5i : %i\n";

writeln("=== with zippering ===");
for (value, i) in zip(lo..hi, 1..) do
  writef(strFormat, i, value);

// Serial
writeln("=== with serial enumerate ===");
for (value, i) in enumerate(lo..hi) do
  writef(strFormat, i, value);

// Parallel
writeln("=== with parallel enumerate ===");
forall (value, i) in enumerate(lo..hi) do
  writef(strFormat, i, value);
