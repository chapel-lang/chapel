class C {
  var x: int;
}

class D: C {
  var y: int;
  var z: int;
}

var A: [1..3] unmanaged D = [i in 1..3] new unmanaged D(x=i);

proc foo(c: borrowed C) { writeln(c); }

writeln(A);

serial {
  foo(A);
}

for d in A do delete d;
