class C {
  var A: [1..3] int;
}

class D: C { }

var A: [1..3] int;

var d = new unmanaged D(A);
d.A(2) = 2;
writeln(d);
writeln(A);

delete d;
