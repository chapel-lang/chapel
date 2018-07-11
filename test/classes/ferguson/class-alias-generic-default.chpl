class A {
  type t = int;
  var x:t;
}

type C = A;

var c = new unmanaged C(x=1);
writeln(c);
writeln(c.x);

delete c;
