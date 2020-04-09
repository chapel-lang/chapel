record A {
  type T;
  var x : T;
}

type C = A(int);

var c = new C(1);
writeln(c);
writeln(c.x);

