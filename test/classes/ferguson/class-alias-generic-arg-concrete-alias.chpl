class A {
  type T;
  var x : T;
}

proc doit(type C)
{
  var c = new unmanaged C(1);
  writeln(c);
  writeln(c.x);
  delete c;
}

type t = A(int);
doit(t);

