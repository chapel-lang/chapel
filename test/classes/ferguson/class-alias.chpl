class A {
  var x:int;
}

type C = A;

var c = new unmanaged C(1);
writeln(c);
writeln(c.x);

delete c;
