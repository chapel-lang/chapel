class A {
  type t = int;
  var x:t;
}

type C = A;

var c = new C(x=1);
writeln(c);
writeln(c.x);

delete c;
