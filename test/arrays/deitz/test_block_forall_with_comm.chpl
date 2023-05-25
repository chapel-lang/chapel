use BlockDist;

const D = {1..4} dmapped Block({1..4});

class C {
  var i: int;
}

var c = (new owned C()).borrow();

forall i in D {
  writeln(c.i);
}

var A: [D] int;

forall e in A {
  writeln(c.i);
}
