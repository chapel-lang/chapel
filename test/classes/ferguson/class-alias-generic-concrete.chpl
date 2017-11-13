class A {
  var x;
}

type C = A(int);

var c = new C(1);
writeln(c);
writeln(c.x);

delete c;
