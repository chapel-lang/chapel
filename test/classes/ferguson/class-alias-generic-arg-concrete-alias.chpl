class A {
  var x;
}

proc doit(type C)
{
  var c = new C(1);
  writeln(c);
  writeln(c.x);
  delete c;
}

type t = A(int);
doit(t);

