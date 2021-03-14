// The optimization passes should be able to determine that
// p cannot alias q

proc foo(n:int, ref p:real, ref q:int)
{
  if n <= 0 then return;

  // Recursive (and not tail recursive) to avoid inlining.
  foo(n/2, p, q);

  for i in 1..n {
    p += q;
  }
}

proc doit() {
  var p = 0.0;
  var q = 1;
  foo(10, p, q);

  writeln("p=", p, " q=", q);
}

proc main() {
  doit();
}
