class C {
  var x: real;
}

class CC : C {
  var y: int;
}

// The optimization passes should be able to determine that
// p can alias q (because C inherits from CC or they are both objects)
proc foo(n:int, ref p:borrowed CC, ref q:borrowed C)
{
  if n <= 0 then return;

  // Recursive (and not tail recursive) to avoid inlining.
  foo(n/2, p, q);

  for i in 1..n {
    p.x += q.x;
    p.y += q.x:int;
  }
}

proc doit() {
  var p  = new borrowed CC(0.0, 0);
  var q  = new borrowed C(1.0);
  var pC = p:borrowed C;

  foo(10, p, q);
  writeln("p=", p, " q=", q);

  p.x = 1.0; p.y = 0;
  q.x = 1.0;

  foo(8, p, pC);

  writeln("p=", p, " q=", q);
}

proc main() {
  doit();
}
