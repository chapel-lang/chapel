record A {
  var x;
}

type C = A;

var c = new C(1);
writeln(c);
writeln(c.x);

