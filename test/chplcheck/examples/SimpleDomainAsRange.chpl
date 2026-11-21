/*
  Loops over single-range domains should iterate over the range directly.

  A domain is a first-class value with its own runtime representation, so
  writing ``{1..10}`` in a loop creates a domain every time the loop is
  evaluated, even though it describes exactly one range.  Iterating over the
  range itself skips that work.  This is a performance anti-pattern, not just
  a matter of style.
*/

// This loop uses a domain around a simple range, which violates the rule.
for i in {1..10} {
  writeln(i);
}

// This loop uses the range directly, which is correct.
for i in 1..10 {
  writeln(i);
}
