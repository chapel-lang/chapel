
// The optimization passes must determine that
// p may alias q.
proc foo(n:int, ref p:int, ref q:int)
{
  if n <= 0 then return;

  // Recursive (and not tail recursive) to avoid inlining.
  foo(n/2, p, q);

  for i in 1..n {
    p += q;
  }
}

var p = 1;
var q = 1;
var s = 1;
foo(10, q, q);
writeln("q=", q);

foo(12, p, s);
writeln("p=", p, " s=", s);

