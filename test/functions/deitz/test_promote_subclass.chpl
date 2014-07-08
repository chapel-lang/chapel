class C {
  var x: int;
}

class D: C {
  var y: int;
  var z: int;
}

var A: [1..3] D = [i in 1..3] new D(x=i);

proc foo(c: C) { writeln(c); }

writeln(A);
serial {
  foo(A);
}
