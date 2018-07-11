class C {
  var A: [1..3] int;
}

var A: [1..3] int;

var c = new unmanaged C(A);
c.A(2) = 2;
writeln(c);
writeln(A);
delete c;
