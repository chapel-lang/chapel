use BlockDist;
proc myFn(x: int,
          y: int,
          z: int,
          w: int) {
  var r = x..y;
  var A = Block.createArray(r, real);
  forall (a, i) in zip(A,A.domain) do
    writeln("a=", a);
    writeln("After loop?");

  if x < y && y < w then
    writeln("x < y < z");
}

