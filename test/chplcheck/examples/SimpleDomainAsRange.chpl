/*
  Loops over single-range domains should iterate over the range directly.

  A simple 1-dimensional domain and range result in equivalent iterations,
  prefer using a range to avoid the performance overhead of a more complex
  domain type.
*/

// This loop uses a domain around a simple range, which violates the rule.
for i in {1..10} {
  writeln(i);
}

// This loop uses the range directly, which is correct.
for i in 1..10 {
  writeln(i);
}
