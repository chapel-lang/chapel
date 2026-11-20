/*
  Loops over single-range domains should iterate over the range directly.
*/

// This loop uses a domain around a simple range, which violates the rule.
for i in {1..10} {
  writeln(i);
}

// This loop uses the range directly, which is correct.
for i in 1..10 {
  writeln(i);
}
