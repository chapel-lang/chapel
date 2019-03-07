class A {
  type T;
  var x : T;
}

type C = A(int);

var c = new unmanaged C(1);
writeln(c);
writeln(c.x);

delete c;
