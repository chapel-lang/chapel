// This tests just checks that the following iterators have the anonymous range
// iteration applied to them.
//   "for i in chpl_build_bounded_range(1,2)"
// is expected to be replaced with
//   "for i in chpl_direct_range_iterator(1,2)"
// and generate clean C for loops without range constructors
//
// To to do it essentially greps the generated code for the code that we
// expect to be generated as a result of this optimization.

proc main() {
  for i in 1..2 do write(i); writeln();
  for i in 2..2+1 do write(i); writeln();
  var lo=3, hi=4; for i in lo..hi do write(i); writeln();
  for i in 4..5 by 2 do write(i); writeln();
  for (i, j) in zip(1..10 by 3, 1..10 by -3) do write(i,j); writeln();
  var r = 1..10 by 2; for (i, j) in zip(1..10 by 2, r) do write(i, j); writeln();
  coforall i in 5..5 do write(i); writeln();
  writeln();
}
