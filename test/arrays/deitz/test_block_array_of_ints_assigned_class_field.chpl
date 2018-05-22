use BlockDist;

const D = {1..4} dmapped Block({1..4});

class C {
  var i: int = -1;
}

var c = new borrowed C();

var A: [D] int = c.i;

writeln(A);

c.i = -2;

forall e in A do
  e = c.i;

writeln(A);
