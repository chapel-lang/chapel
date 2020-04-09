
proc main() {
  // At one point the compiler would encounter an internal assertion when the
  // body of a loop-expr contained a short-circuiting operation. The
  // short-circuit would introduce a string literal,  which violated some
  // internal assertion for global variables before normalization.
  var x = for i in 1..100 do i % 2 == 0 && i % 3 == 0;

  for i in 1..100 do if x[i] == true then writeln(i);
}
